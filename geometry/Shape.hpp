#ifndef SHAPE_H
#define SHAPE_H

#include "RectangleShape.hpp"

namespace geometry {

struct RoundShape: RectangleShape {
    bool contains(const Point& point) const {
        double tx, ty;
        tx = (double)(point.x - x) / (double)width;
        ty = (double)(point.y - y) / (double)height;
        return sqrt(tx * tx + ty * ty) < 1;
    }
};

} // namespace geometry

#endif // !SHAPE_H
