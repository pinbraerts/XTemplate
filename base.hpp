#ifndef BASE_H
#define BASE_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <type_traits>
#include <tuple>
#include <cmath>

constexpr Coord_t NUM = 10;
constexpr Coord_t SEGMENTS_NUM = NUM + 1;
constexpr Size_t CELL_WIDTH = 20;

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
    inline void fillRectangle(const XRectangle& rect) {
        XFillRectangle(
            d, w, gc,
            rect.x, rect.y, rect.width, rect.height
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
};

template<class T, size_t N> constexpr size_t length(T(&)[N]) { return N; }

// template<class T, class Out> struct _ifcv;
// template<class T, class Out> struct _ifcv<const T, Out> {
//     using value = const Out;
// };
// template<class T, class Out> struct _ifcv<volatile T, Out> {
//     using value = volatile Out;
// };
// template<class T, class Out> struct _ifcv<const volatile T, Out> {
//     using value = const volatile Out;
// };
// template<class T, class Out> struct _ifcv<T, Out> {
//     using value = Out;
// };
// template<class T, class Out> using IfCV = class _ifcv<T, Out>::value;

#endif // !BASE_H
