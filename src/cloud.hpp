#pragma once
#include <rplidar.h>
#include <array>
#include <cmath>

using namespace rp::standalone;

const size_t MaxRPLIDARCloudSize = rplidar::RPlidarDriver::MAX_SCAN_NODES;

enum class CoordSystem {
  CART,
  CYL,
};

struct PointCart {
  int x = 0;
  int y = 0;
};

struct PointCyl {
  float angle = 0;
  float dist = 0;

  PointCart to_cart() const;
  PointCart to_cart(float scale, float origin_x, float origin_y) const;
};

template <typename T, size_t max_size>
struct Cloud {
  std::array<T, max_size> pts;
  size_t size = 0;
};

typedef Cloud<PointCyl, MaxRPLIDARCloudSize> CloudCyl;
typedef Cloud<PointCart, MaxRPLIDARCloudSize> CloudCart;
