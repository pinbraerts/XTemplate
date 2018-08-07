#ifndef LAYOUT_H
#define LAYOUT_H

#include "../core/Hoverable.hpp"
#include "../core/Initializer.hpp"

using namespace core;

namespace widgets {

template<class... Widgets>
struct Layout: RectangleShape, std::tuple<Widgets...> {
    using Base = std::tuple<Widgets...>;
    using IndicesSequence = std::index_sequence_for<Widgets...>;

    static constexpr size_t Length = sizeof...(Widgets);

    template<class... Ws> Layout(Ws&&... w): Base(w...) {
        join();
    }

private:
    template<size_t... Ints>
    inline void _draw(DrawContext& dc, std::index_sequence<Ints...>) {
        (std::get<Ints>(*this).draw(dc), ...);
    }

    template<size_t... Ints>
    inline bool _clip(const Point& cursor, std::index_sequence<Ints...>) {
        bool res = false;
        ((is_hoverable<std::tuple_element_t<Ints, Base>> ?
            (res = std::get<Ints>(*this).clip(cursor) || res) : false), ...);
        return res;
    }

    template<size_t... Ints>
    inline bool _press(const Point& cursor, unsigned btn,
        std::index_sequence<Ints...>) {
        bool res = false;
        ((is_clickable<std::tuple_element_t<Ints, Base>> ?
            (res = std::get<Ints>(*this).press(cursor, btn) || res) :
            false), ...);
        return res;
    }

    template<size_t... Ints>
    inline void _join(std::index_sequence<Ints...>) {
        ((clientRect() |= std::get<Ints>(*this)), ...);
    }

    template<size_t... Ints>
    inline bool _release(const Point& cursor, unsigned btn,
        std::index_sequence<Ints...>) {
        bool res = false;
        ((is_clickable<std::tuple_element_t<Ints, Base>> ?
            (res = std::get<Ints>(*this).release(cursor, btn) || res) :
            false), ...);
        return res;
    }

public:
    inline void join() {
        _join(IndicesSequence{});
    }

    inline void draw(DrawContext& dc) {
        _draw(dc, IndicesSequence{});
    }

    inline bool clip(const Point& cursor) {
        return _clip(cursor, IndicesSequence{});
    }

    inline bool press(const Point& cursor, unsigned btn) {
        return _press(cursor, btn, IndicesSequence{});
    }

    inline bool release(const Point& cursor, unsigned btn) {
        return _release(cursor, btn, IndicesSequence{});
    }
};

template<class... Widgets> Layout(Widgets&&...) -> Layout<deinit<Widgets>...>;

} // namespace widgets

#endif // !LAYOUT_H
