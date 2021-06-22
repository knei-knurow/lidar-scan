#include "../include/cloud.hpp"

//
// PointCyl
//
PointCart PointCyl::to_cart() const {
    x_cart = dist * std::sin(angle * (acos(-1.0f) / 180.0f));
    y_cart = dist * std::cos(angle * (acos(-1.0f) / 180.0f));
    int x = int(std::round(x_cart));
    int y = int(std::round(y_cart));
    return {x, y};
}

PointCart PointCyl::to_cart(float scale, float origin_x, float origin_y) const {
    x_cyl = dist * std::sin(angle * (acos(-1.0f) / 180.0f)) * scale;
    y_cyl = dist * std::cos(angle * (acos(-1.0f) / 180.0f)) * scale;
    int x = int(std::round(x_cyl)) + origin_x;
    int y = int(std::round(y_cyl)) + origin_y;
    return {x, y};
}
