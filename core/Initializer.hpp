#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "../geometry/Shape.hpp"

using namespace geometry;

namespace core {

template<class T>
struct Initializer: RectangleShape {
    using Type = T;
    using RectangleShape::RectangleShape;
};
template<class T>
struct Initializer<Initializer<T>>: Initializer<T> {};

template<class T> bool is_initializer = std::is_convertible_v<T, Initializer<T>>;

template<class T> struct _deinit {
    using res = T;
};
template<class T> struct _deinit<Initializer<T>> {
    using res = T;
};
template<class T> using deinit = typename _deinit<T>::res;

} // namespace core

#endif // !INITIALIZER_H
