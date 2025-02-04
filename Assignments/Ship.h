#ifndef SHIP_H
#define SHIP_H

#include <string>
#include <iostream>

class Ship
{
protected:
    std::string name;
    char symbol;
    int x, y;
    int lives;
    char team;

public:
    Ship(std::string name = " ", char symbol = ' ', int x = 0, int y = 0, int lives = 3, char team = ' ');
    virtual ~Ship() {}
    // use Ship(x, y) to set position
    void operator()(int x, int y) { setPosition(x, y); }
    virtual void look() = 0;
    virtual void move() = 0;
    virtual void shoot() = 0;
    void setName(const std::string &newName);
    void setSymbol(char newSymbol);
    void setTeam(char newTeam);
    char getSymbol() const;
    int getX() const;
    int getY() const;
    int getLives() const;
    char getTeam() const;
    void displayDetails(); // for debugging
    void setPosition(int newX, int newY);
    void takeDamage();
};

class Battleship : public Ship
{
public:
    // inherit constructor
    using Ship::Ship;
    void look() override;
    void move() override;
    void shoot() override;
};

class Cruiser : public Ship
{
public:
    // inherit constructor
    using Ship::Ship;
    void look() override;
    void move() override;
    void shoot() override;
};

#endif // SHIP_H
