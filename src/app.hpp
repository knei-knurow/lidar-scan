#pragma once
#include <memory>
#include <sstream>
#include "lidar.hpp"
#include "stream.hpp"

class App {
 public:
  App(std::vector<std::string>& args);
  ~App();

  int run();
  void print_help();
  void close();

 private:
  bool check_arg(std::vector<std::string>& all_args,
                 const std::string& short_arg,
                 const std::string& long_arg);
  std::string get_arg_value(std::vector<std::string>& all_args,
                            const std::string& short_arg,
                            const std::string& long_arg,
                            const std::string& default_value = "");

  bool running_;
  std::unique_ptr<CloudCyl> cloud_;
  std::unique_ptr<RPLIDARPortGrabber> lidar_grabber_;
  std::unique_ptr<Stream> stream_;
};
