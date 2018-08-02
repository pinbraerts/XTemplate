#ifndef GAME_H
#define GAME_H

#include "../widgets/Layout.hpp"

using namespace widgets;

namespace core {

mixin GameBase {
protected:
    DrawContext dc;
    bool requestRender = false;
    Atom wmDeleteMessage;

    GameBase() {
        dc.d = XOpenDisplay(nullptr);
        int s = DefaultScreen(dc.d);
        dc.w = XCreateSimpleWindow(
            dc.d, RootWindow(dc.d, s),
            10, 10, self.width + 20, self.height + 20,
            1, BlackPixel(dc.d, s), WhitePixel(dc.d, s)
        );

        XSetStandardProperties(dc.d, dc.w, "Battleship", nullptr, None, nullptr, 0, nullptr);

        XSelectInput(dc.d, dc.w,
            ExposureMask | KeyPressMask |
            StructureNotifyMask |
            ButtonPressMask | ButtonReleaseMask |
            PointerMotionMask
        );
        XMapWindow(dc.d, dc.w);

        wmDeleteMessage = XInternAtom(dc.d, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(dc.d, dc.w, &wmDeleteMessage, 1);

        dc.gc = DefaultGC(dc.d, s);
    }

public:
    void run() {
        while(true) {
            XEvent e;
            bool running = true;
            char c[255];
            KeySym key;

            if(requestRender) {
                XClearWindow(dc.d, dc.w);
                self.draw(dc);
                requestRender = false;
            }
            XNextEvent(dc.d, &e);
            switch(e.type) {
            case Expose:
                self.draw(dc);
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
                requestRender = self.press(
                    { (Coord_t)e.xbutton.x, (Coord_t)e.xbutton.y },
                    e.xbutton.button
                ) || requestRender;
                break;
            case ButtonRelease:
                requestRender = self.release(
                    { (Coord_t)e.xbutton.x, (Coord_t)e.xbutton.y },
                    e.xbutton.button
                ) || requestRender;
                break;
            case MotionNotify:
                requestRender = self.clip(
                    { (Coord_t)e.xmotion.x, (Coord_t)e.xmotion.y }
                ) || requestRender;
                break;
            }
            if(!running) break;
        }
    }

    ~GameBase() {
        XDestroyWindow(dc.d, dc.w);
        XCloseDisplay(dc.d);
    }
};

template<class... Widgets>
struct Game: Layout<Widgets...>, GameBase<Game<Widgets...>> {
public:
    using LBase = Layout<Widgets...>;
    using LBase::LBase;
    using LBase::x;
    using LBase::y;
    using LBase::width;
    using LBase::height;
    using LBase::draw;
    using LBase::press;
    using LBase::release;
    using LBase::clip;
};

template<class... Widgets> Game(Widgets&&... ws) -> Game<deinit<Widgets>...>;

} // namespace core

#endif // !GAME_H
