#ifndef HOVERABLE_H
#define HOVERABLE_H

#include "DrawContext.hpp"

#define mixin template<class Origin> struct
#define umixin template<class Origin> using
#define self_cast(CLS) static_cast<CLS&>(*this)
#define self self_cast(Origin)
#define cself self_cast(const Origin)

namespace core {

struct _fbase {
    mutable bool focused = false;

    bool checkFocus(const Point& cursor) const {
        return !focused;
    }
};

template<class Origin, class Focus = Origin> struct Focusable: _fbase {
    Focus focus;

    Focusable(const Focus& f): focus(f) { }

    void draw(DrawContext& dc) {
        if(focused) focus.draw(dc);
    }
};

mixin Focusable<Origin, Origin>: _fbase {};

template<class T> constexpr bool is_focusable = std::is_convertible_v<T*, _fbase*>;

#define if_focus if constexpr(is_focusable<Origin>)

struct _hovbase {
    mutable bool hovered = false;
};

mixin Hoverable: _hovbase {
    bool clip(const Point& cursor) {
        bool res = hovered;
        if(self.contains(cursor)) {
            hovered = true;
            res = !res;
            if_focus {
                res = self.checkFocus(cursor) || res;
                self.focused = true;
            }
        } else if(hovered) {
            if_focus self.focused = false;
            hovered = false;
        }
        return res;
    }
};

template<class T> constexpr bool is_hoverable = std::is_convertible_v<T*, _hovbase*>;

struct _bbase {
    mutable bool pressed = false;
};

mixin Clickable: _bbase {
    bool press(const Point& cursor, Size_t btn) {
        if(self.hovered) {
            if(!pressed)
                return pressed = true;
        }
        return false;
    }
    bool release(const Point& cursor, Size_t btn) {
        if(pressed) {
            pressed = false;
            if(self.hovered)
                return self.clicked(cursor, btn);
            else return true;
        }
        return false;
    }
};

template<class T> constexpr bool is_clickable = std::is_convertible_v<T*, _bbase*>;

template<class T>
struct Initializer: RectangleShape {
    using Type = T;
    using RectangleShape::RectangleShape;
};
template<class T>
struct Initializer<Initializer<T>>: Initializer<T> {};

template<class T> bool is_initializer = std::is_base_of_v<T, Initializer<T>>;

template<class T> struct _deinit {
    using res = T;
};
template<class T> struct _deinit<Initializer<T>> {
    using res = T;
};
template<class T> using deinit = typename _deinit<T>::res;

}

#endif // !HOVERABLE_H
