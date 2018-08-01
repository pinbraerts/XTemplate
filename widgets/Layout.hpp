#ifndef LAYOUT_H
#define LAYOUT_H

#include "../core/Hoverable.hpp"

template<class... Widgets>
struct Layout: RectangleShape, std::tuple<Widgets...> {
    using Base = std::tuple<Widgets...>;

    static constexpr size_t Length = sizeof...(Widgets);

    template<class... Ws> Layout(Ws&&... w): Base(w...) {
        join();
    }

    template<size_t N = Length - 1>
    inline void join() {
        clientRect() |= std::get<N>(*this).clientRect();
        if constexpr(N) join<N - 1>();
    }

    template<size_t N = Length - 1>
    inline void draw(DrawContext& dc) {
        std::get<N>(*this).draw(dc);
        if constexpr(N) draw<N - 1>(dc);
    }

    template<size_t N = Length - 1>
    inline bool clip(const Point& cursor) {
        bool res = false;
        if constexpr(is_hoverable<std::tuple_element_t<N, Base>>)
            res = std::get<N>(*this).clip(cursor) || res;
        if constexpr(N) return clip<N - 1>(cursor) || res;
        return res;
    }

    template<size_t N = Length - 1>
    inline bool press(const Point& cursor, Size_t btn) {
        bool res = false;
        if constexpr(is_clickable<std::tuple_element_t<N, Base>>)
            res = std::get<N>(*this).press(cursor, btn) || res;
        if constexpr(N) return press<N - 1>(cursor, btn) || res;
        return res;
    }

    template<size_t N = Length - 1>
    inline bool release(const Point& cursor, Size_t btn) {
        bool res = false;
        if constexpr(is_clickable<std::tuple_element_t<N, Base>>)
            res = std::get<N>(*this).release(cursor, btn) || res;
        if constexpr(N) return release<N - 1>(cursor, btn) || res;
        return res;
    }
};

template<class... Widgets> Layout(Widgets&&...) -> Layout<deinit<Widgets>...>;
// template<class... Widgets> Layout(const RectangleShape& rsWidgets&&...) -> Layout<deinit<Widgets>...>;

#endif // !LAYOUT_H
