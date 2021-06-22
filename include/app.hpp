#pragma once
#include <memory>
#include <sstream>
#include "lidar.hpp"
#include "stream.hpp"

/// The main entity in the program.
///
/// It is created first and initializes rest of its objects.
class App {
   public:
    App(std::vector<std::string>& args);
    ~App();

    /// Runs the main program loop.
    int run();

    /// Prints help message to stdout.
    void print_help();

    /// Sets App::running_ to false.
    void close();

   private:
    /// Returns true if
    bool get_flag(std::vector<std::string>& all_args,
                  const std::string& flag_short,
                  const std::string& flag_long);

    std::string get_argument(std::vector<std::string>& all_args,
                             const std::string& arg_short,
                             const std::string& arg_long,
                             const std::string& default_value = "");

    bool running_;
    std::unique_ptr<CloudCyl> cloud_;
    std::unique_ptr<RPLIDARPortGrabber> lidar_grabber_;
    std::unique_ptr<Stream> stream_;
};
