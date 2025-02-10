#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <fstream>

class Ship; // Forward declare

class Battlefield
{
protected:
    int width, height;
    char **grid;
    void read_grid(std::ifstream &file);

public:
    Battlefield(std::string);
    ~Battlefield();
    bool placeShip(Ship *ship, bool isRespawn = false);
    void display();
    int getSeaCount();
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool inBounds(int x, int y) const;
    char getCell(int x, int y) const;
    void setCell(int x, int y, char val);
};

#endif // BATTLEFIELD_H
