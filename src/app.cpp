#include "../include/app.hpp"
#include <algorithm>

App::App(std::vector<std::string>& args) {
  running_ = true;

  // Print help
  if (args.empty()) {
    print_help();
    running_ = false;
    return;
  }

  // RPLDAR port
  std::string port = args[0];

  // RPLIDAR RPM
  auto rpm = DefaultRPLIDARRPM;
  std::stringstream(get_argument(args, "-r", "--rpm")) >> rpm;

  // RPLIDAR Mode
  RPLIDARScanModes mode;
  unsigned mode_temp = unsigned(RPLIDARScanModes::SENSITIVITY);
  std::stringstream(get_argument(args, "-m", "--mode")) >> mode_temp;
  mode = static_cast<RPLIDARScanModes>(mode_temp %
                                       unsigned(RPLIDARScanModes::RPLIDAR_SCAN_MODES_COUNT));

  // Grabber Mode
  GrabberMode grabber_mode = GrabberMode::CLOUD_BY_CLOUD;
  if (get_flag(args, "-p", "--point-by-point")) {
    grabber_mode = GrabberMode::POINT_BY_POINT;
  }

  // Initialize objects
  cloud_ = std::make_unique<CloudCyl>();
  lidar_grabber_ =
      std::make_unique<RPLIDARPortGrabber>(port, DefaultBaudrate, grabber_mode, mode, rpm);
  if (!lidar_grabber_->get_status()) {
    running_ = false;
    return;
  }
  stream_ = std::make_unique<Stream>();
  if (!stream_->get_status()) {
    running_ = false;
    return;
  }

  // Try to reset the RPLIDAR driver
  if (get_flag(args, "--reset", "--reset")) {
    running_ = false;
    return;
  }
}

App::~App() {}

int App::run() {
  auto grabber_mode = lidar_grabber_->get_grabber_mode();

  // Main program loop
  while (running_) {
    lidar_grabber_->read(*cloud_);
    if (grabber_mode == GrabberMode::CLOUD_BY_CLOUD) {
      stream_->write_cloud(*cloud_);
    } else if (grabber_mode == GrabberMode::POINT_BY_POINT) {
      long long timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                                std::chrono::system_clock::now().time_since_epoch())
                                .count();
      for (int i = 0; i < cloud_->size; i++)
        stream_->write_point(cloud_->pts[i], timestamp);
    }
  }

  return 0;
}

void App::print_help() {
  std::cout << "Source:\thttps://github.com/knei-knurow/lidar-scan\n"
            << "Usage:\tlidar-scan port [options]\n"
            << "Options:\n"
            << "\t-h --help\tPrint this message\n"
            << "\t-m --mode\tRPLIDAR mode (0 - 4)\n"
            << "\t-r --rpm \tRPLIDAR revolutions per minute (" << MinRPLIDARRPM << " - "
            << MaxRPLIDARRPM << ")\n"
            << "\t-p --point-by-point\t Point-by-point scanning\n"
            << "\t--reset  \tTry to reset the RPLIDAR driver and close\n";
}

void App::close() {
  std::clog << "lidar-scan: exit signal recieved\n";
  running_ = false;
}

bool App::get_flag(std::vector<std::string>& all_args,
                   const std::string& flag_short,
                   const std::string& flag_long) {
  auto it = std::find_if(
      all_args.begin(), all_args.end(),
      [flag_short, flag_long](const std::string& s) { return s == flag_short || s == flag_long; });

  if (it == all_args.end()) {
    return false;
  }

  all_args.erase(it);
  return true;
}

std::string App::get_argument(std::vector<std::string>& all_args,
                              const std::string& arg_short,
                              const std::string& arg_long,
                              const std::string& default_value) {
  auto it = std::find_if(
      all_args.begin(), all_args.end(),
      [arg_short, arg_long](const std::string& s) { return s == arg_short || s == arg_long; });

  std::string value = default_value;
  if (it == all_args.end()) {
  } else if (it + 1 == all_args.end()) {
    all_args.erase(it);
  } else {
    value = *(it + 1);
    all_args.erase(it, it + 2);
  }
  return value;
}
