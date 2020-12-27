#pragma once
#include <chrono>
#include <cstdio>
#include <iostream>
#include "cloud.hpp"

class Stream {
 public:
  Stream(std::FILE* stream = stdout);
  ~Stream();

  bool write_cloud_separator();
  bool write_point(const PointCyl& pt_cyl);
  bool write_cloud(const CloudCyl& cloud_cyl);
  bool get_status() const;

 private:
  bool status_;
  std::FILE* stream_;
  size_t count_;
  std::chrono::steady_clock::time_point last_cloud_;
};
