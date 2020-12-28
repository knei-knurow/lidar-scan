#include "../include/app.hpp"

//
// App
//
App::App(std::vector<std::string>& args) {
  running_ = true;

  // Print help
  if (check_arg(args, "-h", "--help") || args.empty()) {
    print_help();
    running_ = false;
    return;
  }

  // RPLDAR port
  std::string port = args[0];

  // RPLIDAR RPM
  auto rpm = DefaultRPLIDARRPM;
  std::stringstream(get_arg_value(args, "-r", "--rpm")) >> rpm;

  // RPLIDAR Mode
  RPLIDARScanModes mode;
  unsigned mode_temp = unsigned(RPLIDARScanModes::SENSITIVITY);
  std::stringstream(get_arg_value(args, "-m", "--mode")) >> mode_temp;
  mode = static_cast<RPLIDARScanModes>(mode_temp %
                                       unsigned(RPLIDARScanModes::RPLIDAR_SCAN_MODES_COUNT));

  // Initialize objects
  cloud_ = std::make_unique<CloudCyl>();
  lidar_grabber_ = std::make_unique<RPLIDARPortGrabber>(port, DefaultBaudrate, mode, rpm);
  if (!lidar_grabber_->get_status()) {
    running_ = false;
    return;
  }
  stream_ = std::make_unique<Stream>();
  if (!stream_->get_status()) {
    running_ = false;
    return;
  }
}

App::~App() {}

int App::run() {
  // Main program loop
  while (running_) {
    lidar_grabber_->read(*cloud_);
    stream_->write_cloud(*cloud_);
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
            << MaxRPLIDARRPM << ")\n";
}

void App::close() {
  std::clog << "Exit signal recieved.\n";
  running_ = false;
}

bool App::check_arg(std::vector<std::string>& all_args,
                    const std::string& short_arg,
                    const std::string& long_arg) {
  auto it = std::find_if(
      all_args.begin(), all_args.end(),
      [short_arg, long_arg](const std::string& s) { return s == short_arg || s == long_arg; });

  if (it == all_args.end()) {
    return false;
  }

  all_args.erase(it);
  return true;
}

std::string App::get_arg_value(std::vector<std::string>& all_args,
                               const std::string& short_arg,
                               const std::string& long_arg,
                               const std::string& default_value) {
  auto it = std::find_if(
      all_args.begin(), all_args.end(),
      [short_arg, long_arg](const std::string& s) { return s == short_arg || s == long_arg; });

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