#ifndef DRAW_CONTEXT_H
#define DRAW_CONTEXT_H

#include "../geometry/Shape.hpp"

using namespace geometry;

namespace core {

constexpr unsigned NUM = 10;
constexpr unsigned SEGMENTS_NUM = NUM + 1;
constexpr unsigned CELL_WIDTH = 20;

struct DrawContext {
    Display* d;
    Drawable w;
    GC gc;

    inline void setForeground(unsigned color) {
        XSetForeground(d, gc, color);
    }

    inline void drawRectangle(const XRectangle& rect) {
        XDrawRectangle(
            d, w, gc,
            rect.x, rect.y, rect.width, rect.height
        );
    }
    inline void drawRectangle(Coord_t x, Coord_t y, Coord_t wi, Coord_t h) {
        XDrawRectangle(d, w, gc, x, y, wi, h);
    }
    inline void drawRectangle(const Rectangle& rect) {
        XDrawRectangle(
            d, w, gc,
            static_cast<short>(rect.x), static_cast<short>(rect.y),
            static_cast<short>(rect.width), static_cast<short>(rect.height)
        );
    }

    inline void fillRectangle(const XRectangle& rect) {
        XFillRectangle(
            d, w, gc,
            rect.x, rect.y, rect.width, rect.height
        );
    }
    inline void fillRectangle(const Rectangle& rect) {
        XFillRectangle(
            d, w, gc,
            static_cast<short>(rect.x), static_cast<short>(rect.y),
            static_cast<short>(rect.width), static_cast<short>(rect.height)
        );
    }
    inline void fillRectangle(Coord_t x, Coord_t y, Coord_t wi, Coord_t h) {
        XFillRectangle(d, w, gc, x, y, wi, h);
    }

    inline void drawLine(Coord_t x1, Coord_t y1, Coord_t x2, Coord_t y2) {
        XDrawLine(d, w, gc, x1, y1, x2, y2);
    }
    inline void drawLine(const XPoint& point1, const XPoint& point2) {
        XDrawLine(d, w, gc, point1.x, point1.y, point2.x, point2.y);
    }
    inline void drawLine(const Point& point1, const Point& point2) {
        XDrawLine(
            d, w, gc,
            static_cast<short>(point1.x), static_cast<short>(point1.y),
            static_cast<short>(point2.x), static_cast<short>(point2.y)
        );
    }

    inline void drawSegments(XSegment* seg, size_t len) {
        XDrawSegments(d, w, gc, seg, len);
    }
    template<size_t N> inline void drawSegments(XSegment(&seg)[N]) {
        drawSegments(seg, length(seg));
    }

    inline void drawPoint(Coord_t x, Coord_t y) {
        XDrawPoint(d, w, gc, x, y);
    }
    inline void drawPoint(const XPoint& point) {
        XDrawPoint(d, w, gc, point.x, point.y);
    }
    inline void drawPoint(const Point& point) {
        XDrawPoint(
            d, w, gc,
            static_cast<short>(point.x), static_cast<short>(point.y)
        );
    }
};

} // namespace core

#endif // !DRAW_CONTEXT_H
