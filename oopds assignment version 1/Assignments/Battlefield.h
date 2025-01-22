#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "Ship.h"

class Battlefield {
    int width, height;
    char **grid;

public:
    Battlefield(int width, int height);
    ~Battlefield();

    void placeShip(Ship *ship);
    void moveShip(Ship *ship, int oldX, int oldY);
    void display();
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif // BATTLEFIELD_H
