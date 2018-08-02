#ifndef GAME_H
#define GAME_H

#include "../widgets/Layout.hpp"

using namespace widgets;

namespace core {

template<class... Widgets>
struct Game: Layout<Widgets...> {
public:
    using LBase = Layout<Widgets...>;
    // template<class... Ws> Game(Ws&&... ws): LBase(ws...) { init(); }
    using LBase::LBase;
    using LBase::x;
    using LBase::y;
    using LBase::width;
    using LBase::height;

protected:
    DrawContext dc;
    bool requestRender = false;
    Atom wmDeleteMessage;

public:
    void init() {
        dc.d = XOpenDisplay(nullptr);
        int s = DefaultScreen(dc.d);
        dc.w = XCreateSimpleWindow(
            dc.d, RootWindow(dc.d, s),
            10, 10, width + 20, height + 20,
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
        init(); // TODO: move init to constructor
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
                buttonPressed({ (Coord_t)e.xbutton.x, (Coord_t)e.xbutton.y }, e.xbutton.button);
                break;
            case ButtonRelease:
                buttonReleased({ (Coord_t)e.xbutton.x, (Coord_t)e.xbutton.y }, e.xbutton.button);
                break;
            case MotionNotify:
                mouseMove({ (Coord_t)e.xmotion.x, (Coord_t)e.xmotion.y });
                break;
            }
            if(!running) break;
        }
    }

    void render() {
        // right_field.draw(dc);
        // left_field.draw(dc);
        // button.draw(dc);
        LBase::draw(dc);
    }

    void mouseMove(const Point& cursor) {
        requestRender = LBase::clip(cursor) || requestRender;
    }

    void buttonPressed(const Point& cursor, Size_t btn) {
        requestRender = LBase::press(cursor, btn) || requestRender;
    }

    void buttonReleased(const Point& cursor, Size_t btn) {
        requestRender = LBase::release(cursor, btn) || requestRender;
    }

    ~Game() {
        XDestroyWindow(dc.d, dc.w);
        XCloseDisplay(dc.d);
    }
};

template<class... Widgets> Game(Widgets&&... ws) -> Game<deinit<Widgets>...>;

} // namespace core

#endif // !GAME_H
