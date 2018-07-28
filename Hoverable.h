#ifndef HOVERABLE_H
#define HOVERABLE_H

#include "Shape.h"

#define mixin template<class Origin> struct
#define umixin template<class Origin> using
#define self_cast(CLS) static_cast<CLS&>(*this)
#define self self_cast(Origin)
#define cself self_cast(const Origin)

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

template<class T> constexpr bool is_focusable = std::is_convertible_v<T, _fbase>;

#define if_focus if constexpr(is_focusable<Origin>)

struct _hbase {
    mutable bool hovered = false;
};

template<class T> constexpr bool is_hoverable = std::is_convertible_v<T, _hbase>;

mixin Hoverable: _hbase {
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

struct _bbase {
    bool pressed = false;
};

template<class T> constexpr bool is_clickable = std::is_convertible_v<T, _hbase>;

template<class Origin, bool hoverable> struct _clickable: _bbase {
    bool press(const Point& cursor, unsigned short btn) {
        if(self.hovered) {
            if(!pressed)
                return pressed = true;
        }
        return false;
    }
    bool release(const Point& cursor, unsigned short btn) {
        if(pressed) {
            pressed = false;
            if(self.hovered)
                return self.clicked(cursor, btn);
            else return true;
        }
        return false;
    }
};
mixin _clickable<Origin, false>: Hoverable<Origin>, _clickable<Origin, true> { };

umixin Clickable = _clickable<Origin, std::is_convertible_v<Origin, _hbase>>;

#endif // !HOVERABLE_H
