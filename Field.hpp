#ifndef FIELD_H
#define FIELD_H

#include "Grid.hpp"
#include "Hoverable.hpp"

struct MyCell {
    struct Shape: RectangleShape {
        Shape(): RectangleShape { 0, 0, CELL_WIDTH, CELL_WIDTH } {}
    };

    enum Type {
        Nonetyp = 0b0000,
        Filled  = 0b0001,
        Crossed = 0b0010,
        Dotted  = 0b0100
    } type = Nonetyp;

    void draw(DrawContext& dc, const Shape& shape) {
        switch(type) {
        case Nonetyp:
            break;
        case Dotted:
            dc.setForeground(0);
            dc.drawPoint(shape.center());
            break;
        default:
        if(type & Filled) {
            dc.setForeground(0);
            constexpr Coord_t line = 3; // TODO: change to set bold line
            dc.fillRectangle(
                shape.x, shape.y,
                line + 1, shape.height
            );
            dc.fillRectangle(
                shape.x + line, shape.y,
                shape.width - line, line + 1
            );
            dc.fillRectangle(
                shape.x + shape.width - line, shape.y + line,
                line, shape.height - line
            );
            dc.fillRectangle(
                shape.x + line, shape.y + shape.height - line,
                shape.width - line * 2, line
            );
        }
        if(type & Crossed) {
            dc.setForeground(0);
            dc.drawLine(shape.upper_left(), shape.lower_right());
            dc.drawLine(shape.upper_right(), shape.lower_left());
        }
        }
    }
};

struct FocusRect: RectangleShape {
    unsigned long color = 100;

    using RectangleShape::RectangleShape;

    void draw(DrawContext& dc) {
        dc.setForeground(color);
        dc.fillRectangle(*this);
    }
};

struct FieldBase: Grid<MyCell, NUM, NUM, true>,
    Hoverable<FieldBase>,
    Clickable<FieldBase>, Focusable<FieldBase, FocusRect> {
    using Base = Grid<MyCell, NUM, NUM, true>;
    using FocusBase = Focusable<FieldBase, FocusRect>;

    FieldBase(const RectangleShape& init): Base(init), FocusBase {
        { 0, 0, cell_width(), cell_height() }
    } { }

    bool clicked(Point cursor, Size_t btn) {
        unsigned ox, oy;
        if(getCellIndex(cursor, ox, oy)) {
            auto& t = cells[ox][oy].type;
            if(t & MyCell::Filled) {
                t = (MyCell::Type)(MyCell::Crossed | t);
            } else {
                t = MyCell::Dotted;
            }
            return true;
        }
        return false;
    }

    bool checkFocus(Point cursor) {
        unsigned ox, oy;
        getCellIndex(cursor, ox, oy);
        cursor.x = ox * cell_width();
        cursor.y *= oy * cell_height();
        cursor.x += x;
        cursor.y += y;
        if(focus.x != cursor.x || focus.y != cursor.y) {
            focus.x = cursor.x;
            focus.y = cursor.y;
            return true;
        }
        return !focused;
    }

    void draw(DrawContext& dc) {
        FocusBase::draw(dc);
        Base::draw(dc);
    }
};

using Field = Initializer<FieldBase>;

struct ButtonBase: RectangleShape,
    Hoverable<ButtonBase>, Clickable<ButtonBase> {
    enum Color {
        Black = 0,
        Blue = 100,
        LightBlue = 200
    };

    ButtonBase(
        const RectangleShape& other
    ): RectangleShape { other.clientRect(), Vector2D(other.size() / 2) } { }

    bool clicked(const Point& cursor, Size_t btn) {
        std::cout << "Clicked" << std::endl;
        return false;
    }

    void draw(DrawContext& dc) {
        if(hovered || pressed) {
            dc.setForeground(Blue);
            dc.fillRectangle(clientRect());
            if(!pressed) {
                dc.setForeground(LightBlue);
                auto newRect = clientRect();
                newRect.move(0, -5);
                dc.fillRectangle(newRect);
            }
        } else {
            dc.setForeground(Black);
            dc.drawRectangle(clientRect());
        }
    }
};

using Button = Initializer<ButtonBase>;

#endif // !FIELD_H
