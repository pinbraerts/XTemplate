#ifndef SHAPE_H
#define SHAPE_H

#include "includes.hpp"

using Coord_t = float;
using Size_t = float;

struct PointBase {
    Coord_t x, y;

    operator XPoint() {
        return { static_cast<short>(x), static_cast<short>(y) };
    }
};

struct Vector2D: PointBase {
    Vector2D operator+(const PointBase& other) const {
        return { x + other.x, y + other.y };
    }
    Vector2D& operator+() { return *this; }
    const Vector2D& operator+() const { return *this; }

    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2D operator-(const Vector2D& other) const {
        return { x - other.x, y - other.y };
    }
    Vector2D operator-() const { return { -x, -y }; }

    Vector2D& operator-=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    static inline constexpr Vector2D Zero() { return { 0, 0 }; }
};


struct Point: PointBase {
    Point& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Point operator+(const Vector2D& other) const {
        return { x + other.x, y + other.y };
    }
    Point operator-(const Vector2D& other) const {
        return { x - other.x, y - other.y };
    }
};

struct Size {
    Size_t width;
    Size_t height;

    Size(): width(0), height(0) {}
    Size(Size_t w, Size_t h): width(w), height(h) {}
    Size(Size&& other): width(other.width), height(other.height) {}
    Size(const Size& other): width(other.width), height(other.height) {}

    operator Vector2D() { return { width, height }; }

    Size operator*(float scale) const {
        return {
            width * scale,
            height * scale
        };
    }
    Size& operator*=(float scale) {
        width *= scale;
        height *= scale;
        return *this;
    }
    Size operator/(float scale) const {
        return {
            width / scale,
            height / scale
        };
    }
    Size& operator/=(float scale) {
        width /= scale;
        height /= scale;
        return *this;
    }

    static inline const Size Zero() { return { 0, 0 }; }
};

struct Rectangle: Point, Size {
    inline Point& origin() { return *this; }
    inline const Point& origin() const { return *this; }
    inline Size& size() { return *this; }
    inline const Size& size() const { return *this; }

    Point upper_left() const { return { x, y }; }
    Point upper_right() const { return { x + width, y }; }
    Point lower_left() const { return { x, y + height }; }
    Point lower_right() const { return { x + width, y + height }; }
    Point center() const { return { x + width / 2, y + height / 2 }; }

    Rectangle(Coord_t x0 = 0, Coord_t y0 = 0, Size_t w = 0, Size_t h = 0):
        Point { x0, y0 }, Size { w, h } {}
    Rectangle(const Point& orig, Size_t w = 0, Size_t h = 0):
        Point { orig }, Size { w, h } {}
    Rectangle(const Point& orig, const Size& size):
        Point { orig }, Size { size } {}
    Rectangle(Coord_t ox, Coord_t oy, const Size& size):
        Point { ox, oy }, Size { size } {}

    void move(Coord_t dx, Coord_t dy) {
        x += dx;
        y += dy;
    }
    Rectangle operator+(const Vector2D& other) const {
        return { x + other.x, y + other.y, width, height };
    }
    Rectangle operator-(const Vector2D& other) const {
        return { x - other.x, y - other.y, width, height };
    }

    Rectangle operator|(const Rectangle& other) const {
        Rectangle res = *this;
        return res |= other;
    }
    Rectangle& operator|=(const Rectangle& other) {
        if(other.x < x) x = other.x;
        if(other.y < y) y = other.y;
        Point lr1 = lower_right(), lr2 = other.lower_right();
        if(lr2.x > lr1.x) width = lr2.x - x;
        if(lr2.y > lr1.y) height = lr2.y - y;
        return *this;
    }

    operator Point&() {
        return origin();
    }
    operator const Point&() const {
        return origin();
    }
};

struct RectangleShape: Rectangle {
    Vector2D offset = Vector2D::Zero();

    Point origin() const { return Rectangle::origin() + offset; }
    void origin(const Point& other) { Rectangle::origin() = other - offset; }
    void origin(const Rectangle& other) {
        Rectangle::origin() = other.origin() - offset;
    }
    void origin(const RectangleShape& other) {
        Rectangle::origin() = other.origin() - offset;
    }

    bool contains(Point point) const {
        point.x -= x;
        point.y -= y;
        return point.x >= 0 && point.x <= width &&
            point.y >= 0 && point.y <= height;
    }

    RectangleShape& clientRect() { return *this; }
    const RectangleShape& clientRect() const { return *this; }

    using Rectangle::Rectangle;
    RectangleShape(
        Coord_t x0, Coord_t y0,
        Size_t w, Size_t h,
        const Vector2D& offs
    ): Rectangle { x - offs.x, y - offs.y, w, h }, offset(offs) { }
    RectangleShape(
        Coord_t x0, Coord_t y0,
        Size_t w, Size_t h,
        Coord_t ox, Coord_t oy
    ): Rectangle { x0 - ox, y0 - oy, w, h }, offset { ox, oy } { }
    RectangleShape(
        Coord_t x0, Coord_t y0,
        const Size& s,
        Coord_t ox, Coord_t oy
    ): Rectangle { x0 - ox, y0 - oy, s }, offset { ox, oy } { }
    RectangleShape(
        Coord_t x0, Coord_t y0,
        const Size& s,
        const Vector2D& offs
    ): Rectangle { x0 - offs.x, y0 - offs.y, s }, offset(offs) { }
    RectangleShape(
        const Point& orig,
        Size_t w, Size_t h,
        const Vector2D& offs
    ): Rectangle { orig - offs, w, h }, offset(offs) { }
    RectangleShape(
        const Point& orig,
        const Size& s,
        const Vector2D& offs
    ): Rectangle { orig - offs, s }, offset(offs) {}
    RectangleShape(
        const Point& orig,
        Size_t w, Size_t h,
        Coord_t ox, Coord_t oy
    ): Rectangle { orig.x - ox, orig.y - oy, w, h }, offset { ox, oy } { }
    RectangleShape(
        const Point& orig,
        const Size& s,
        Coord_t ox, Coord_t oy
    ): Rectangle { orig.x - ox, orig.y - oy, s }, offset { ox, oy } { }
    RectangleShape(
        const Rectangle& other,
        const Vector2D& offs
    ): Rectangle { other - offs }, offset(offs) { }
    RectangleShape(
        const Rectangle& other,
        Coord_t ox, Coord_t oy
    ): Rectangle { other.x - ox, other.y - oy, other.size() },
        offset { ox, oy } { }
};

struct RoundShape: RectangleShape {
    bool contains(const Point& point) const {
        double tx, ty;
        tx = (double)(point.x - x) / (double)width;
        ty = (double)(point.y - y) / (double)height;
        return sqrt(tx * tx + ty * ty) < 1;
    }
};

#endif // !SHAPE_H
