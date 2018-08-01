#ifndef GRID_H
#define GRID_H

#include "Shape.hpp"

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

    bool getCellIndex(Point& point) const {
        point.x -= x;
        point.y -= y;
        if(point.x < 0) point.x = x / cell_width() - 1;
        else point.x /= cell_width();
        if(point.y < 0) point.y = y / cell_height() - 1;
        else point.y /= cell_height();
        return point.x >= 0 && point.x < hnum && point.y >= 0 && point.y < vnum;
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
            Coord_t t = x + dx;
            grid[i] = { t, y, t, (Coord_t)(y + height) };
        }
        for(Coord_t dy = 0; dy <= height && i < length(grid); dy += cell_height(), ++i) {
            Coord_t t = y + dy;
            grid[i] = { x, t, (Coord_t)(x + width), t };
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
