#include "../include/lidar.hpp"

RPLIDARPortGrabber::RPLIDARPortGrabber(std::string portname,
                                       unsigned baudrate,
                                       RPLIDARScanModes scan_mode,
                                       unsigned rpm)
    : portname_(portname), baudrate_(baudrate), scan_mode_(scan_mode) {
  driver_ = rplidar::RPlidarDriver::CreateDriver();
  buffer_size_ = MaxRPLIDARCloudSize;
  status_ = true;
  buffer_ = new rplidar_response_measurement_node_hq_t[buffer_size_];
  if (rpm < MinRPLIDARRPM || rpm > MaxRPLIDARRPM) {
    std::cerr << "ERROR: RPM value should be between " << MinRPLIDARRPM << " and " << MaxRPLIDARRPM
              << "." << std::endl;
    rpm = DefaultRPLIDARRPM;
  }
  rpm_ = rpm;
  launch();
}

RPLIDARPortGrabber::~RPLIDARPortGrabber() {
  stop();
  delete[] buffer_;
}

bool RPLIDARPortGrabber::read(CloudCyl& cloud) {
  scan();
  if (!status_)
    return false;

  cloud.size = buffer_size_;
  for (size_t i = 0; i < buffer_size_; i++) {
    cloud.pts[i].angle = buffer_[i].angle_z_q14 / 65536.0f * 360;
    cloud.pts[i].dist = buffer_[i].dist_mm_q2 / 4.0f;
  }
  return status_;
}

bool RPLIDARPortGrabber::print_info() {
  if (!status_)
    return false;

  rplidar_response_device_info_t lidar_info;
  auto res = driver_->getDeviceInfo(lidar_info);
  if (IS_FAIL(res)) {
    std::cerr << "ERROR: Unable to get device info response." << std::endl;
    status_ = false;
    return false;
  }
  std::clog << "LIDAR info:" << std::endl;
  std::clog << "  Model:         " << lidar_info.model << std::endl;
  std::clog << "  Firmware ver.: " << lidar_info.firmware_version << std::endl;
  std::clog << "  Hardware ver.: " << unsigned(lidar_info.hardware_version) << std::endl;
  std::clog << "  Serial number: ";
  for (int i = 0; i < 16; i++) {
    std::clog << std::setfill('0') << std::setw(2) << std::hex << int(lidar_info.serialnum[i])
              << std::dec;
  }
  std::clog << std::endl;
  return true;
}

bool RPLIDARPortGrabber::print_health() {
  if (!status_)
    return false;

  rplidar_response_device_health_t lidar_health;
  auto res = driver_->getHealth(lidar_health);
  if (IS_FAIL(res)) {
    std::cerr << "ERROR: Unable to get device health response." << std::endl;
    status_ = false;
    return false;
  }
  std::clog << "LIDAR health: ";
  if (lidar_health.status == RPLIDAR_STATUS_OK)
    std::clog << "Good";
  else if (lidar_health.status == RPLIDAR_STATUS_WARNING)
    std::clog << "Warning";
  else if (lidar_health.status == RPLIDAR_STATUS_ERROR)
    std::clog << "Error";
  std::clog << std::endl;
  return true;
}

bool RPLIDARPortGrabber::print_scan_modes(std::vector<rplidar::RplidarScanMode>& scan_modes,
                                          _u16& default_mode) {
  if (!status_)
    return false;

  scan_modes.clear();
  auto res = driver_->getTypicalScanMode(default_mode);
  res = driver_->getAllSupportedScanModes(scan_modes);
  if (IS_FAIL(res)) {
    std::cerr << "ERROR: Unable to get device scan modes response." << std::endl;
    return false;
  }
  std::clog << "Supported scan modes:" << std::endl;
  for (const auto& scan_mode : scan_modes) {
    std::clog << "  ";
    std::clog << scan_mode.scan_mode << " (" << scan_mode.id;
    if (scan_mode.id == default_mode)
      std::clog << ", DEFAULT";
    std::clog << "), "
              << "sample time: " << scan_mode.us_per_sample << "us, "
              << "max distance: " << scan_mode.max_distance << "m" << std::endl;
  }
  return true;
}

bool RPLIDARPortGrabber::launch() {
  if (!status_)
    return false;

  std::clog << "LIDAR connection:" << std::endl;
  std::clog << "  Port: " << portname_ << "" << std::endl;
  std::clog << "  Baudrate: " << baudrate_ << "" << std::endl;
  auto res = driver_->connect(portname_.c_str(), baudrate_);
  if (IS_FAIL(res)) {
    std::cerr << "ERROR: Unable to establish connection." << std::endl;
    status_ = false;
    return false;
  }
  std::clog << "Connection established." << std::endl;

  print_info();
  std::vector<rplidar::RplidarScanMode> scan_modes;
  _u16 mode;
  print_scan_modes(scan_modes, mode);
  std::clog << "Selected scan mode: " << _u16(scan_mode_) << "." << std::endl;
  std::clog << "Statring motor." << std::endl;
  res = driver_->startMotor();
  if (IS_FAIL(res)) {
    std::cerr << "ERROR: Unable to start motor." << std::endl;
    status_ = false;
    return false;
  }
  std::clog << "Setting motor speed to [rpm]: " << rpm_ << std::endl;
  res = driver_->setMotorPWM(rpm_);
  if (IS_FAIL(res)) {
    std::cerr << "ERROR: Unable to set motor RPM." << std::endl;
    status_ = false;
    return false;
  }
  print_health();
  res = driver_->startScanExpress(false, _u16(scan_mode_));
  if (IS_FAIL(res)) {
    std::cerr << "ERROR: Unable to start scanning." << std::endl;
    status_ = false;
    return false;
  }
  status_ = true;
  return true;
}

bool RPLIDARPortGrabber::scan() {
  size_t buffer_size = MaxRPLIDARCloudSize;
  auto res = driver_->grabScanDataHq(buffer_, buffer_size);
  buffer_size_ = buffer_size;
  if (IS_FAIL(res)) {
    std::cerr << "ERROR: Unable to read scanning data." << std::endl;
    status_ = false;
    return false;
  }
  driver_->ascendScanData(buffer_, buffer_size);
  return true;
}

void RPLIDARPortGrabber::stop() {
  std::clog << "Stopping motor and deallocating LIDAR driver." << std::endl;
  auto res = driver_->stopMotor();
  if (IS_FAIL(res)) {
    std::cerr << "ERROR: Unable to stop motor." << std::endl;
    status_ = false;
  }
  driver_->disconnect();
  rplidar::RPlidarDriver::DisposeDriver(driver_);
}

bool RPLIDARPortGrabber::get_status() const {
  return status_;
}