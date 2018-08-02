#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Point.hpp"

namespace geometry {

struct Rectangle: Point, Size {
    inline Point& origin() { return *this; }
    inline const Point& origin() const { return *this; }
    inline Size& size() { return *this; }
    inline const Size& size() const { return *this; }

    inline explicit operator Point&() { return *this; }
    inline explicit operator const Point&() const { return *this; }
    inline explicit operator Size&() { return *this; }
    inline explicit operator const Size&() const { return *this; }

    inline Point upper_left() const { return { x, y }; }
    inline Point upper_right() const { return { x + width, y }; }
    inline Point lower_left() const { return { x, y + height }; }
    inline Point lower_right() const { return { x + width, y + height }; }
    inline Point center() const { return { x + width / 2, y + height / 2 }; }

    Rectangle(Coord_t x0 = 0, Coord_t y0 = 0, Size_t w = 0, Size_t h = 0);
    Rectangle(const Point& orig, Size_t w = 0, Size_t h = 0);
    Rectangle(const Point& orig, const Size& size);
    Rectangle(Coord_t ox, Coord_t oy, const Size& size);

    void move(Coord_t dx, Coord_t dy);
    Rectangle operator+(const Vector2D& other) const;
    Rectangle operator-(const Vector2D& other) const;

    Rectangle operator|(const Rectangle& other) const;
    Rectangle& operator|=(const Rectangle& other);
};

} // namespace geometry

#endif // !RECTANGLE_H
