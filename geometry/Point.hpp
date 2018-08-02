#ifndef POINT_H
#define POINT_H

#include "../core/includes.hpp"

namespace geometry {

using Coord_t = float;
using Size_t = float;

struct PointBase {
    Coord_t x, y;

    operator XPoint();
};

struct Vector2D: PointBase {
    Vector2D operator+(const PointBase& other) const;
    Vector2D& operator+();
    const Vector2D& operator+() const;

    Vector2D& operator+=(const Vector2D& other);

    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator-() const;

    Vector2D& operator-=(const Vector2D& other);

    static inline constexpr Vector2D Zero() { return { 0, 0 }; }
};


struct Point: PointBase {
    Point& operator+=(const Vector2D& other);
    Point& operator-=(const Vector2D& other);
    Point operator+(const Vector2D& other) const;
    Point operator-(const Vector2D& other) const;
};

struct Size {
    Size_t width, height;

    operator Vector2D();

    Size operator*(float scale) const;
    Size& operator*=(float scale);
    Size operator/(float scale) const;
    Size& operator/=(float scale);

    static inline constexpr Size Zero() { return { 0, 0 }; }
};

} // namespace geometry

#endif // !POINT_H
