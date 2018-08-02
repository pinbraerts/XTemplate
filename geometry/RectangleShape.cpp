#include "RectangleShape.hpp"

namespace geometry {

Point RectangleShape::origin() const {
    return Rectangle::origin() + offset;
}

void RectangleShape::origin(const Point& other) {
    Rectangle::origin() = other - offset;
}

void RectangleShape::origin(const Rectangle& other) {
    Rectangle::origin() = other.origin() - offset;
}

void RectangleShape::origin(const RectangleShape& other) {
    Rectangle::origin() = other.origin() - offset;
}

bool RectangleShape::contains(Point point) const {
    point.x -= x;
    point.y -= y;
    return point.x >= 0 && point.x <= width &&
        point.y >= 0 && point.y <= height;
}

RectangleShape::RectangleShape(
    Coord_t x0, Coord_t y0,
    Size_t w, Size_t h,
    const Vector2D& offs
): Rectangle { x - offs.x, y - offs.y, w, h }, offset(offs) { }
RectangleShape::RectangleShape(
    Coord_t x0, Coord_t y0,
    Size_t w, Size_t h,
    Coord_t ox, Coord_t oy
): Rectangle { x0 - ox, y0 - oy, w, h }, offset { ox, oy } { }
RectangleShape::RectangleShape(
    Coord_t x0, Coord_t y0,
    const Size& s,
    Coord_t ox, Coord_t oy
): Rectangle { x0 - ox, y0 - oy, s }, offset { ox, oy } { }
RectangleShape::RectangleShape(
    Coord_t x0, Coord_t y0,
    const Size& s,
    const Vector2D& offs
): Rectangle { x0 - offs.x, y0 - offs.y, s }, offset(offs) { }
RectangleShape::RectangleShape(
    const Point& orig,
    Size_t w, Size_t h,
    const Vector2D& offs
): Rectangle { orig - offs, w, h }, offset(offs) { }
RectangleShape::RectangleShape(
    const Point& orig,
    const Size& s,
    const Vector2D& offs
): Rectangle { orig - offs, s }, offset(offs) {}
RectangleShape::RectangleShape(
    const Point& orig,
    Size_t w, Size_t h,
    Coord_t ox, Coord_t oy
): Rectangle { orig.x - ox, orig.y - oy, w, h }, offset { ox, oy } { }
RectangleShape::RectangleShape(
    const Point& orig,
    const Size& s,
    Coord_t ox, Coord_t oy
): Rectangle { orig.x - ox, orig.y - oy, s }, offset { ox, oy } { }
RectangleShape::RectangleShape(
    const Rectangle& other,
    const Vector2D& offs
): Rectangle { other - offs }, offset(offs) { }
RectangleShape::RectangleShape(
    const Rectangle& other,
    Coord_t ox, Coord_t oy
): Rectangle { other.x - ox, other.y - oy, other.size() },
    offset { ox, oy } { }

} // namespace geometry
