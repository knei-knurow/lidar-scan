#include "app.hpp"

//
// App
//
App::App(std::vector<std::string>& args) {
  running_ = true;
  if (!parse_args(args)) {
    running_ = false;
  }
}

App::~App() {}

int App::run() {
  cloud_ = std::make_unique<CloudCyl>();
  lidar_grabber_ = std::make_unique<RPLIDARPortGrabber>("com3");
  stream_ = std::make_unique<Stream>();
  while (running_) {
    lidar_grabber_->read(*cloud_);
    stream_->write_cloud(*cloud_);

    running_ = false;
  }

  return 0;
}

void App::print_help() {
  std::cout << "-----------------------------------------------------------\n"
            << "Lidar Scan\n"
            << "-----------------------------------------------------------\n"
            << "Source: https://github.com/knei-knurow/lidar-scan\n";
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

bool App::parse_args(std::vector<std::string>& args) {
  // Print help
  if (check_arg(args, "-h", "--help")) {
    print_help();
    return false;
  }
  return true;
}
