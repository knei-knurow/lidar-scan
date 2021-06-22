#include "../include/cloud.hpp"

PointCart PointCyl::to_cart() const {
    double x_cart = dist * std::sin(angle * (acos(-1.0f) / 180.0f));
    double y_cart = dist * std::cos(angle * (acos(-1.0f) / 180.0f));
    int x = int(std::round(x_cart));
    int y = int(std::round(y_cart));
    return {x, y};
}

PointCart PointCyl::to_cart(float scale, float origin_x, float origin_y) const {
    double x_cart = dist * std::sin(angle * (acos(-1.0f) / 180.0f));
    double y_cart = dist * std::cos(angle * (acos(-1.0f) / 180.0f)) * scale;
    int x = int(std::round(x_cart)) + origin_x;
    int y = int(std::round(y_cart)) + origin_y;
    return {x, y};
}
