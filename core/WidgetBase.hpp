#ifndef WIDGET_BASE_H
#define WIDGET_BASE_H

#include "DrawContext.hpp"

namespace core {

template<unsigned char mask> struct _propbase {
    inline unsigned char& pick() {
        return *reinterpret_cast<unsigned char*>(this);
    }
    inline const unsigned char& pick() const {
        return *reinterpret_cast<const unsigned char*>(this);
    }

    inline operator bool() const {
        return pick() & mask;
    }
    inline bool operator=(bool b) {
        if(b) pick() |= mask;
        else pick() &= ~mask;
        return b;
    }
};

struct WidgetBase {
    union {
        unsigned char _ = 0;

        _propbase<1> hovered;
        _propbase<2> pressed;
        _propbase<4> focused;
    };
};

template<class T> constexpr bool is_widget = std::is_convertible_v<T*, WidgetBase*>;

} // namespace core

#endif // !WIDGET_BASE_H
