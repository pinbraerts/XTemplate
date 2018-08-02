#ifndef RECTANGLE_SHAPE_H
#define RECTANGLE_SHAPE_H

#include "Rectangle.hpp"

namespace geometry {

struct RectangleShape: Rectangle {
    Vector2D offset = Vector2D::Zero();

    Point origin() const;
    void origin(const Point& other);
    void origin(const Rectangle& other);
    void origin(const RectangleShape& other);

    bool contains(Point point) const;

    inline RectangleShape& clientRect() { return *this; }
    inline const RectangleShape& clientRect() const { return *this; }

    using Rectangle::Rectangle;
    RectangleShape(Coord_t x0, Coord_t y0, Size_t w, Size_t h, const Vector2D& offs);
    RectangleShape(Coord_t x0, Coord_t y0, Size_t w, Size_t h, Coord_t ox, Coord_t oy);
    RectangleShape(Coord_t x0, Coord_t y0, const Size& s, Coord_t ox, Coord_t oy);
    RectangleShape(Coord_t x0, Coord_t y0, const Size& s, const Vector2D& offs);
    RectangleShape(const Point& orig, Size_t w, Size_t h, const Vector2D& offs);
    RectangleShape(const Point& orig, const Size& s, const Vector2D& offs);
    RectangleShape(const Point& orig, Size_t w, Size_t h, Coord_t ox, Coord_t oy);
    RectangleShape(const Point& orig, const Size& s, Coord_t ox, Coord_t oy);
    RectangleShape(const Rectangle& other, const Vector2D& offs);
    RectangleShape(const Rectangle& other, Coord_t ox, Coord_t oy);
};

} // namespace geometry

#endif // !RECTANGLE_SHAPE_H
