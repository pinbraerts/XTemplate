#ifndef HOVERABLE_H
#define HOVERABLE_H

#include "DrawContext.hpp"

#define mixin template<class Origin> struct
#define umixin template<class Origin> using
#define self_cast(CLS) IfCV_cast<CLS&>(*this)
#define self self_cast(Origin)

namespace core {

struct _fbase { };

template<class Origin, class Focus = Origin> struct Focusable: _fbase {
    Focus focus;

    Focusable(const Focus& f): focus(f) { }

    bool checkFocus(const Point& cursor) const {
        return !self.focused;
    }

    void draw(DrawContext& dc) {
        if(self.focused) focus.draw(dc);
    }
};

mixin Focusable<Origin, Origin>: _fbase {};

template<class T> constexpr bool is_focusable = std::is_convertible_v<T*, _fbase*>;

#define if_focus if constexpr(is_focusable<Origin>)

struct _hovbase { };

mixin Hoverable: _hovbase {
    bool clip(const Point& cursor) {
        bool res = self.hovered;
        if(self.contains(cursor)) {
            self.hovered = true;
            res = !res;
            if_focus {
                res = self.checkFocus(cursor) || res;
                self.focused = true;
            }
        } else if(self.hovered) {
            if_focus self.focused = false;
            self.hovered = false;
        }
        return res;
    }
};

template<class T> constexpr bool is_hoverable = std::is_convertible_v<T*, _hovbase*>;

struct _bbase { };

mixin Clickable: _bbase {
    bool press(const Point& cursor, Size_t btn) {
        if(self.hovered) {
            if(!self.pressed)
                return self.pressed = true;
        }
        return false;
    }
    bool release(const Point& cursor, Size_t btn) {
        if(self.pressed) {
            self.pressed = false;
            if(self.hovered)
                return self.clicked(cursor, btn);
            else return true;
        }
        return false;
    }
};

template<class T> constexpr bool is_clickable = std::is_convertible_v<T*, _bbase*>;

} // namespace core

#endif // !HOVERABLE_H
