#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <fstream>

class Ship; // Forward declare

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
    bool inBounds(int x, int y) const;
    // Additional getters for occupant
    char getCell(int x, int y) const;
    void setCell(int x, int y, char val);
};

#endif // BATTLEFIELD_H
