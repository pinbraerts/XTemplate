#include "Rectangle.hpp"

namespace geometry {

Rectangle::Rectangle(Coord_t x0, Coord_t y0, Size_t w, Size_t h):
    Point { x0, y0 }, Size { w, h } {}
Rectangle::Rectangle(const Point& orig, Size_t w, Size_t h):
    Point { orig }, Size { w, h } {}
Rectangle::Rectangle(const Point& orig, const Size& size):
    Point { orig }, Size { size } {}
Rectangle::Rectangle(Coord_t ox, Coord_t oy, const Size& size):
    Point { ox, oy }, Size { size } {}

void Rectangle::move(Coord_t dx, Coord_t dy) {
    x += dx;
    y += dy;
}

Rectangle Rectangle::operator+(const Vector2D& other) const {
    return { x + other.x, y + other.y, width, height };
}

Rectangle Rectangle::operator-(const Vector2D& other) const {
    return { x - other.x, y - other.y, width, height };
}

Rectangle Rectangle::operator|(const Rectangle& other) const {
    Rectangle res = *this;
    return res |= other;
}

Rectangle& Rectangle::operator|=(const Rectangle& other) {
    if(other.x < x) x = other.x;
    if(other.y < y) y = other.y;
    Point lr1 = lower_right(), lr2 = other.lower_right();
    if(lr2.x > lr1.x) width = lr2.x - x;
    if(lr2.y > lr1.y) height = lr2.y - y;
    return *this;
}

} // namespace core
