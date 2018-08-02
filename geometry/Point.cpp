#include "Point.hpp"

namespace geometry {

PointBase::operator XPoint() {
    return { static_cast<short>(x), static_cast<short>(y) };
}

Vector2D Vector2D::operator+(const PointBase& other) const {
    return { x + other.x, y + other.y };
}

Vector2D& Vector2D::operator+() { return *this; }

const Vector2D& Vector2D::operator+() const { return *this; }

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return { x - other.x, y - other.y };
}
Vector2D Vector2D::operator-() const { return { -x, -y }; }

Vector2D& Vector2D::operator-=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}


Point& Point::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Point& Point::operator-=(const Vector2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Point Point::operator+(const Vector2D& other) const {
    return { x + other.x, y + other.y };
}

Point Point::operator-(const Vector2D& other) const {
    return { x - other.x, y - other.y };
}


Size::operator Vector2D() { return { width, height }; }

Size& Size::operator/=(float scale) {
    width /= scale;
    height /= scale;
    return *this;
}

Size Size::operator/(float scale) const {
    return {
        width / scale,
        height / scale
    };
}

Size& Size::operator*=(float scale) {
    width *= scale;
    height *= scale;
    return *this;
}

Size Size::operator*(float scale) const {
    return {
        width * scale,
        height * scale
    };
}

} // namespace geometry
