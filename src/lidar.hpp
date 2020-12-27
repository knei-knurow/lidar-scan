#include <rplidar.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "cloud.hpp"

using namespace rp::standalone;

const unsigned MinRPLIDARRPM = 200;
const unsigned MaxRPLIDARRPM = 1023;
const unsigned DefaultRPLIDARRPM = 200;

const unsigned DefaultBaudrate = 256000;

enum class RPLIDARScanModes {
  STANDARD,
  EXPRESS,
  BOOST,
  SENSITIVITY,
  STABILITY,
  RPLIDAR_SCAN_MODES_COUNT,
};

class RPLIDARPortGrabber {
 public:
  RPLIDARPortGrabber(std::string portname,
                     unsigned DefaultBaudrate,
                     RPLIDARScanModes scan_mode = RPLIDARScanModes::SENSITIVITY,
                     unsigned rpm = DefaultRPLIDARRPM);
  virtual ~RPLIDARPortGrabber();
  virtual bool read(CloudCyl& cloud);

  bool print_info();
  bool print_health();
  bool print_scan_modes(std::vector<rplidar::RplidarScanMode>& scan_modes, _u16& default_mode);
  bool launch();
  bool scan();
  void stop();

 private:
  const std::string portname_;
  const int baudrate_;
  const RPLIDARScanModes scan_mode_;
  int rpm_;
  rplidar::RPlidarDriver* driver_;
  rplidar_response_measurement_node_hq_t* buffer_;
  size_t buffer_size_;
  bool status_;
};
