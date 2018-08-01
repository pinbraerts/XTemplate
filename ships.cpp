#include <iostream>
#include <string>

#include "core/Game.hpp"
#include "Field.hpp"

int main(int argc, char** argv) {
    Game game {
        Field { 20, 20 },
        Field {
            240, 20
        },
        Button {
            230, 260,
            100, 40
        }
    };
    game.run();
    return 0;
}
