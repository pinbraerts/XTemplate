#ifndef SHAPE_H
#define SHAPE_H

#include "base.h"

struct Vector2D: XPoint {
    Vector2D operator+(const XPoint& other) const {
        return { (short)(x + other.x), (short)(y + other.y) };
    }
    Vector2D& operator+() { return *this; }
    const Vector2D& operator+() const { return *this; }

    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2D operator-(const Vector2D& other) const {
        return { (short)(x - other.x), (short)(y - other.y) };
    }
    Vector2D operator-() const { return { (short)-x, (short)-y }; }

    Vector2D& operator-=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    static inline constexpr Vector2D Zero() { return { 0, 0 }; }
};


struct Point: XPoint {
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
        return { (short)(x + other.x), (short)(y + other.y) };
    }
    Point operator-(const Vector2D& other) const {
        return { (short)(x - other.x), (short)(y - other.y) };
    }
};

struct Size {
    union {
        struct {
            unsigned short width;
            unsigned short height;
        };
        Vector2D vec;
    };

    Size(): width(0), height(0) {}
    Size(unsigned short w, unsigned short h): width(w), height(h) {}
    Size(Size&& other): width(other.width), height(other.height) {}
    Size(const Size& other): width(other.width), height(other.height) {}
    operator Vector2D&() {
        return vec;
    }
    operator const Vector2D&() const {
        return vec;
    }

    Size operator*(double scale) const {
        return {
            (short unsigned)(width * scale),
            (short unsigned)(height * scale)
        };
    }
    Size& operator*=(double scale) {
        width *= scale;
        height *= scale;
        return *this;
    }
    Size operator/(double scale) const {
        return {
            (short unsigned)(width / scale),
            (short unsigned)(height / scale)
        };
    }
    Size& operator/=(double scale) {
        width /= scale;
        height /= scale;
        return *this;
    }

    static inline const Size Zero() { return { 0, 0 }; }
};

struct Rectangle: XRectangle {
    Point& origin() {
        return *reinterpret_cast<Point*>(this);
    }
    const Point& origin() const {
        return *reinterpret_cast<const Point*>(this);
    }
    void origin(const Point& other) {
        origin() = other;
    }
    Size& size() {
        return *reinterpret_cast<Size*>(&width);
    }
    const Size& size() const {
        return *reinterpret_cast<const Size*>(&width);
    }

    Point upper_left() const { return { x, y }; }
    Point upper_right() const { return { (short)(x + width), y }; }
    Point lower_left() const { return { x, (short)(y + height) }; }
    Point lower_right() const {
        return { (short)(x + width), (short)(y + height) };
    }
    Point center() const {
        return { (short)(x + width / 2), (short)(y + height / 2) };
    }

    Rectangle(short x0 = 0, short y0 = 0, unsigned short w = 0, unsigned short h = 0):
        XRectangle { x0, y0, w, h } {}
    Rectangle(const Point& orig, unsigned short w = 0, unsigned short h = 0):
        XRectangle { orig.x, orig.y, w, h } {}
    Rectangle(const Point& orig, const Size& size):
        XRectangle { orig.x, orig.y, size.width, size.height } {}
    Rectangle(short ox, short oy, const Size& size):
        XRectangle { ox, oy, size.width, size.height } {}

    void move(short dx, short dy) {
        x += dx;
        y += dy;
    }
    Rectangle operator+(const Vector2D& other) {
        return { (short)(x + other.x), (short)(y + other.y), width, height };
    }
    Rectangle operator-(const Vector2D& other) {
        return { (short)(x - other.x), (short)(y - other.y), width, height };
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
        if(lr2.y > lr1.y) height = lr1.x - x;
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
    void origin(const Rectangle& other) { Rectangle::origin() = other.origin() - offset; }
    void origin(const RectangleShape& other) { Rectangle::origin() = other.origin() - offset; }

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
        const Point& orig,
        unsigned short w, unsigned short h,
        const Vector2D& offs
    ): Rectangle { orig - offs, w, h }, offset(offs) { }
    RectangleShape(
        const Point& orig,
        const Size& s,
        const Vector2D& offs
    ): Rectangle { orig - offs, s }, offset(offs) {}
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
