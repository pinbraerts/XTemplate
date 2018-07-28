#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "Layout.h"

class Game: public RectangleShape {
protected:
    DrawContext dc;
    Field left_field, right_field;
    Button button;
    bool requestRender = false;
    Atom wmDeleteMessage;

public:
    Game(short x0, short y0):
        RectangleShape { x0, y0, 0, 0 },
        left_field { x, y },
        right_field { (short)(x + left_field.cell_width() + left_field.width), y },
        button {
            (short)(x + left_field.width + left_field.cell_width() / 2),
            (short)(y + left_field.height + 2 * left_field.cell_height()),
            (short unsigned)(left_field.cell_width() * 5),
            (short unsigned)(left_field.cell_height() * 2)
        }
    {
        width = left_field.width + left_field.cell_width() + right_field.width;
        height = left_field.height;
        dc.d = XOpenDisplay(nullptr);
        int s = DefaultScreen(dc.d);
        dc.w = XCreateSimpleWindow(
            dc.d, RootWindow(dc.d, s),
            10, 10, 600, 800,
            1, BlackPixel(dc.d, s), WhitePixel(dc.d, s)
        );

        XSetStandardProperties(dc.d, dc.w, "Battleship", nullptr, None, nullptr, 0, nullptr);

        XSelectInput(dc.d, dc.w,
            ExposureMask | KeyPressMask |
            StructureNotifyMask |
            ButtonPressMask | ButtonReleaseMask |
            PointerMotionMask);
        XMapWindow(dc.d, dc.w);

        wmDeleteMessage = XInternAtom(dc.d, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(dc.d, dc.w, &wmDeleteMessage, 1);

        dc.gc = DefaultGC(dc.d, s);
    }

    void run() {
        while(true) {
            XEvent e;
            bool running = true;
            char c[255];
            KeySym key;

            if(requestRender) {
                XClearWindow(dc.d, dc.w);
                render();
                requestRender = false;
            }
            XNextEvent(dc.d, &e);
            switch(e.type) {
            case Expose:
                render();
                break;
            case ClientMessage:
                if (e.xclient.data.l[0] == wmDeleteMessage)
                    running = false;
                break;
            case KeyPress:
                std::cout << e.xkey.keycode << std::endl;
                if(XLookupString(&e.xkey, c, 255, &key, 0)) {
                    std::cout << "You entered: " << c[0] << std::endl;
                }
                break;
            case ButtonPress:
                buttonPressed({ (short)e.xbutton.x, (short)e.xbutton.y }, e.xbutton.button);
                break;
            case ButtonRelease:
                buttonReleased({ (short)e.xbutton.x, (short)e.xbutton.y }, e.xbutton.button);
                break;
            case MotionNotify:
                mouseMove({ (short)e.xmotion.x, (short)e.xmotion.y });
                break;
            }
            if(!running) break;
        }
    }

    void render() {
        // right_field.draw(dc);
        // left_field.draw(dc);
        // button.draw(dc);
        Layout {
            Button { 10, 10, 100, 50 },
            Button { 120, 10, 100, 50 }
        }.draw(dc);
    }

    void mouseMove(const Point& cursor) {
        requestRender = left_field.clip(cursor) || requestRender;
        requestRender = right_field.clip(cursor) || requestRender;
        requestRender = button.clip(cursor) || requestRender;
    }

    void buttonPressed(const Point& cursor, unsigned short btn) {
        requestRender = left_field.press(cursor, btn) || requestRender;
        requestRender = button.press(cursor, btn) || requestRender;
    }

    void buttonReleased(const Point& cursor, unsigned short btn) {
        requestRender = left_field.release(cursor, btn) || requestRender;
        requestRender = button.release(cursor, btn) || requestRender;
    }

    ~Game() {
        XDestroyWindow(dc.d, dc.w);
        XCloseDisplay(dc.d);
    }
};

#endif // !GAME_H
