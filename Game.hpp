#ifndef GAME_H
#define GAME_H

#include "Field.hpp"
#include "Layout.hpp"

class Game: public RectangleShape {
protected:
    DrawContext dc;
    Layout<FieldBase, FieldBase, ButtonBase> layout {
        Field { x, y },
        Field {
            (short)(x + 220), y
        },
        Button {
            (short)(x + 210),
            (short)(y + 240),
            100, 40
        }
    };
    bool requestRender = false;
    Atom wmDeleteMessage;

public:
    Game(short x0, short y0):
        RectangleShape { x0, y0, 440, 250 }
    {
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
        layout.draw(dc);
    }

    void mouseMove(const Point& cursor) {
        requestRender = layout.clip(cursor) || requestRender;
    }

    void buttonPressed(const Point& cursor, unsigned short btn) {
        requestRender = layout.press(cursor, btn) || requestRender;
    }

    void buttonReleased(const Point& cursor, unsigned short btn) {
        requestRender = layout.release(cursor, btn) || requestRender;
    }

    ~Game() {
        XDestroyWindow(dc.d, dc.w);
        XCloseDisplay(dc.d);
    }
};

#endif // !GAME_H
