#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "Ship.h"

class Battlefield
{
private:
    int width, height;
    char **grid;
    void read_grid(std::ifstream &file);

public:
    Battlefield(std::string);
    ~Battlefield();
    void placeShip(Ship *ship);
    void moveShip(Ship *ship, int oldX, int oldY);
    void display();
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif
// BATTLEFIELD_H
