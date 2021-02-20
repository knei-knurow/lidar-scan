#include "../include/stream.hpp"

Stream::Stream(std::FILE* stream) {
  stream_ = stream;
  status_ = 0 <= std::fprintf(stream_, "# Transmission start.\n");
  count_ = 0;
  last_cloud_ = std::chrono::steady_clock::now();
}

Stream::~Stream() {
  write_cloud_separator();
  status_ = 0 <= std::fprintf(stream_, "# Transmission end.\n");
}

bool Stream::write_cloud_separator() {
  auto now = std::chrono::steady_clock::now();
  auto elapsed_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - last_cloud_).count();

  status_ = 0 <= std::fprintf(stream_, "! %i %llu\n", ++count_, elapsed_time);
  last_cloud_ = std::chrono::steady_clock::now();
  return status_;
}

bool Stream::write_point(const PointCyl& pt_cyl, long long time) {
  status_ = 0 <= std::fprintf(stream_, "%f %f %ll\n", pt_cyl.angle, pt_cyl.dist);
  return status_;
}

bool Stream::write_cloud(const CloudCyl& cloud_cyl) {
  write_cloud_separator();
  for (size_t i = 0; i < cloud_cyl.size; i++) {
    if (!status_) {
      return status_;
    }
    status_ = 0 <= std::fprintf(stream_, "%f %f\n", cloud_cyl.pts[i].angle, cloud_cyl.pts[i].dist);
  }
  return status_;
}

bool Stream::get_status() const {
  return status_;
}