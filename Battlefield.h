#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <fstream>

class Ship; // Forward declare

class Battlefield
{
private:
    int width, height;
    char **grid = nullptr;
    bool **islandGrid; // For amphibious checks
    void read_grid(std::ifstream &file);

public:
    Battlefield(std::string);
    ~Battlefield();
    void placeShip(Ship *ship);
    void moveShip(Ship *ship, int oldX, int oldY);
    void display();
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isCellValid(int x, int y) const;              // For normal ships
    bool isCellValidForAmphibious(int x, int y) const; // For amphibious
    void loadMatrix(int **m);

    // Additional getters for occupant
    char getCell(int x, int y) const;
    void setCell(int x, int y, char val);
};

#endif // BATTLEFIELD_H
