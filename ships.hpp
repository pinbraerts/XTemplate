#ifndef SHIPS_H
#define SHIPS_H

struct Ship;

struct BaseShip {
protected:
    unsigned ndecks;

    static constexpr char const* const names[] = {
        "Error!",
        "Destroyer", // торедный катер
        "Submarine", // эсминец
        "Cruiser", // крейсер
        "Battleship", // линкор
        "Carrier" // авианосец
    };

public:
    BaseShip(unsigned n): ndecks(n) {}

    constexpr const char* to_str() const {
        return ndecks < 6 ? names[ndecks] : names[0];
    }
};

#include "Game.h"


#endif // !SHIPS_H
