#ifndef INCLUDES_H
#define INCLUDES_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <type_traits>
#include <tuple>
#include <cmath>
#include <iostream>

template<class T, size_t N> constexpr size_t length(T(&)[N]) { return N; }

#endif // !INCLUDES_H
