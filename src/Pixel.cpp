#include "Pixel.hpp"
namespace pixel {
    static constexpr double eps = 1e-12;
    bool compare(double x, double y) {
        return std::fabs(x - y) < eps;
    }

    // compare x and y, returns true if x >= y within an epsilon toleranse
    // if x < y, we still consider x >= y if x+eps >= y.
    bool compare_greater_or_equal(double x, double y) {
        return (x + eps) >= y;
    }

    // compare x and y, returns true if x > y within an epsilon toleranse
    // if x < y, we still consider x > y if x+eps >= y.
    bool compare_greater(double x, double y) {
        return (x - eps) > y;
    }

    int round(double value) {
        return (int) std::round(value);
    }
}
