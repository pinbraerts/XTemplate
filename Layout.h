#ifndef LAYOUT_H
#define LAYOUT_H

#include "Hoverable.h"

template<class... Widgets>
struct Layout: std::tuple<Widgets...> {
    using Base = std::tuple<Widgets...>;

    using Base::Base;

    template<size_t N = std::tuple_size_v<Base> - 1>
    inline void draw(DrawContext& dc) {
        std::cout << "Draw: " << N << std::endl;
        std::get<N>(*this).draw(dc);
        if constexpr(N) draw<N - 1>(dc);
    }
};

template<class... Widgets> Layout(Widgets&&...) -> Layout<Widgets...>;

#endif // !LAYOUT_H
