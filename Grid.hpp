#ifndef GRID_H
#define GRID_H

#include "DrawContext.hpp"

template<class Cell, size_t hnum, size_t vnum>
struct GridBase: RectangleShape {
public:
    Cell cells[hnum][vnum];

    using CShape = typename Cell::Shape;
    CShape cell_shape;

    GridBase(const Point& orig, const Vector2D& offs = Vector2D::Zero()):
        RectangleShape(orig, Size::Zero(), offs) {
        width = cell_width() * hnum;
        height = cell_height() * vnum;
    }
    GridBase(const Point& orig, const Vector2D& offs, const CShape& cshape):
        RectangleShape(orig, Size::Zero(), offs), cell_shape(cshape) {
        width = cell_width() * hnum;
        height = cell_height() * vnum;
    }

    inline const Size_t& cell_width() const { return cell_shape.width; }
    inline const Size_t& cell_height() const { return cell_shape.height; }

    bool getCellIndex(const Point& point, unsigned& ox, unsigned& oy) const {
        ox = static_cast<short>(point.x - x);
        oy = static_cast<short>(point.y - y);
        if(ox < 0) ox = ox / cell_width() - 1;
        else ox /= cell_width();
        if(oy < 0) oy = oy / cell_height() - 1;
        else oy /= cell_height();
        return ox >= 0 && ox < hnum && oy >= 0 && oy < vnum;
    }

    void draw(DrawContext& dc) {
        cell_shape.origin(origin());
        for(auto& column: cells) {
            for(auto& cell: column) {
                cell.draw(dc, cell_shape);
                cell_shape.move(0, cell_height());
            }
            cell_shape.move(cell_width(), -width);
        }
    }
};

template<class Cell = void, size_t hnum = 0, size_t vnum = 0, bool draw_grid = false> struct Grid;
template<> struct Grid<void, 0, 0, true> {}; // TODO
template<> struct Grid<void, 0, 0, false> {}; // TODO

template<class Cell, size_t hnum, size_t vnum>
struct Grid<Cell, hnum, vnum, true>: GridBase<Cell, hnum, vnum> {
    XSegment grid[hnum + 1 + vnum + 1];

    using Base = GridBase<Cell, hnum, vnum>;

    using Base::cell_width;
    using Base::cell_height;
    using Base::getCellIndex;
    using Base::width;
    using Base::height;
    using Base::x;
    using Base::y;
    using Base::offset;

    Grid(
        const Point& orig,
        const Vector2D& offs = Vector2D::Zero()
    ): Base { orig, offs } {
        fillGrid();
    }

    void origin(const Point& other) {
        RectangleShape::origin(other);
        fillGrid();
    }
    void move(Coord_t dx, Coord_t dy) {
        RectangleShape::move(dx, dy);
        fillGrid();
    }

    void fillGrid() {
        unsigned i = 0;
        for(Coord_t dx = 0; dx <= width && i < length(grid); dx += cell_width(), ++i) {
            short t = x + dx;
            grid[i] = { t, static_cast<short>(y), t, static_cast<short>(y + height) };
        }
        for(Coord_t dy = 0; dy <= height && i < length(grid); dy += cell_height(), ++i) {
            short t = y + dy;
            grid[i] = { static_cast<short>(x), t, static_cast<short>(x + width), t };
        }
    }

    void draw(DrawContext& dc) {
        Base::draw(dc);

        dc.setForeground(0);
        dc.drawSegments(grid);
    }
};

template<class CellT, size_t hnum, size_t vnum>
struct Grid<CellT, hnum, vnum, false>: GridBase<CellT, hnum, vnum> { };

#endif // !GRID_H
