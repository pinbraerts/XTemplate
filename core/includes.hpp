#ifndef INCLUDES_H
#define INCLUDES_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <type_traits>
#include <tuple>
#include <cmath>
#include <iostream>

template<class T, size_t N> constexpr size_t length(T(&)[N]) { return N; }

template<class R> R IfCV_cast(auto& t) {
    return static_cast<R>(t);
}
template<class R> const std::remove_reference_t<R>& IfCV_cast(const auto& t) {
    return static_cast<const std::remove_reference_t<R>&>(t);
}
template<class R> volatile R IfCV_cast(volatile auto& t) {
    return static_cast<volatile R>(t);
}
template<class R> const volatile R IfCV_cast(const volatile auto& t) {
    return static_cast<const volatile R>(t);
}

#endif // !INCLUDES_H
