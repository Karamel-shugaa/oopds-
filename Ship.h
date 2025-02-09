#ifndef SHIP_H
#define SHIP_H

#include <string>
#include "TempList.h"
#include <iostream>

// Forward declare Simulation to avoid circular includes here
class Simulation;

// Base abstract class
class Ship
{
protected:
    struct Cell
    {
        char symbol;
        int x, y;
    };
    char islandOrSea = '0';
    Cell cell;
    int lives;
    int kills;
    char team;
    std::string name;
    Cell neighbouring[8]{{'X', -1, -1}};
    Simulation &simulation;

public:
    Ship(Simulation &simulation) : simulation(simulation), lives(3), kills(0) {};
    Ship(const Ship &other) : simulation(other.simulation)
    {
        std::cout << "Ship copy constructor called\n";
        islandOrSea = other.islandOrSea;
        cell = other.cell;
        lives = other.lives;
        kills = 0; // restart kill count
        team = other.team;
        name = other.name;
        for (int i = 0; i < 8; i++)
        {
            neighbouring[i] = other.neighbouring[i];
        }
    }
    virtual ~Ship() {}
    // use Ship(x, y) to set position
    void operator()(int x, int y) { setPosition(x, y); }
    virtual void action() = 0;
    void setSymbol(char newSymbol);
    void setTeam(char newTeam);
    void setName(std::string);
    char getSymbol() const;
    int getX() const;
    int getY() const;
    int getLives() const;
    char getTeam() const;
    void displayDetails(); // for debugging
    void setPosition(int newX, int newY);
    void recordKill();
    void die();
    std::string getName() const { return name; }
};

// Intermediate abstract classes
class MovingShip : virtual public Ship
{
public:
    MovingShip(Simulation &simulation) : Ship(simulation) {}
    MovingShip(const MovingShip &other) : Ship(other) {}
    virtual void move() = 0;
    virtual ~MovingShip() {}
};

class SeeingShip : virtual public Ship
{
public:
    SeeingShip(Simulation &simulation) : Ship(simulation) {}
    SeeingShip(const SeeingShip &other) : Ship(other) {}
    virtual void look() = 0;
    virtual ~SeeingShip() {}
};

class ShootingShip : virtual public Ship
{
public:
    ShootingShip(Simulation &simulation) : Ship(simulation) {}
    ShootingShip(const ShootingShip &other) : Ship(other) {}
    virtual void shoot() = 0;
    virtual ~ShootingShip() {}
};

class RamingShip : virtual public Ship
{
public:
    RamingShip(Simulation &simulation) : Ship(simulation) {}
    RamingShip(const RamingShip &other) : Ship(other) {}
    virtual void ram() = 0;
    virtual ~RamingShip() {}
};
// -----------------------------------------------------------------------------
// Derived classes
// -----------------------------------------------------------------------------

class BattleShip : public MovingShip, public SeeingShip, public ShootingShip
{
public:
    BattleShip(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), ShootingShip(simulation) {};
    void action() override;
    void look () override;
    void move() override;
    void shoot() override;
};

class Cruiser : public MovingShip, public SeeingShip, public RamingShip
{
public:
    TempList enemyShips;
    Cruiser(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), RamingShip(simulation) {};
    void action() override;
    void look () override;
    void moveorram();
    void move() override;
    void ram() override;
};

class Destroyer : public MovingShip, public SeeingShip, public ShootingShip, public RamingShip
{
public:
    TempList enemyShips;
    Destroyer(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), ShootingShip(simulation), RamingShip(simulation) {};
    Destroyer(const BattleShip &battleship) : Ship(battleship), MovingShip(battleship), SeeingShip(battleship), ShootingShip(battleship), RamingShip(*this) {}
    Destroyer(Cruiser &cruiser) : Ship(cruiser), MovingShip(cruiser), SeeingShip(cruiser), ShootingShip(*this), RamingShip(cruiser) 
    {
        name = "Destroyer";
        cell.symbol = 'D';
        if (!cruiser.enemyShips.empty())
        {
            cruiser.enemyShips.clear();
        }
    }
    void action() override;
    void look () override;
    void move() override;
    void moveorram();
    void ram() override;
    void shoot() override;
};
class Frigate : public ShootingShip
{
private:
    int targetx;
    int targety;
    int directionIndex = 0;

public:
    Frigate(Simulation &simulation) : Ship(simulation), ShootingShip(simulation) {};
    void action() override;
    void shoot() override;
    void nextTarget();
};


class Corvette : public SeeingShip, public ShootingShip
{
public:
    Corvette(Simulation &simulation) : Ship(simulation),SeeingShip(simulation), ShootingShip(simulation) {};
    Corvette(Frigate &frigate) : Ship(frigate), SeeingShip(*this), ShootingShip(frigate)
    {
        name = "Corvette";
        cell.symbol = 'C';

    }
    void action() override;
    void look () override;
    void shoot() override;
};

class Amphibious : public MovingShip, public SeeingShip, public ShootingShip
{
public:
    Amphibious(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), ShootingShip(simulation) {};
    void action() override;
    void look () override;
    void move() override;
    void shoot() override;
};

class SuperShip : public MovingShip, public SeeingShip, public ShootingShip, public RamingShip
{
private:
    TempList enemyShips;
public:
    SuperShip(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), ShootingShip(simulation), RamingShip(simulation) {};
    SuperShip(Destroyer &destroyer) : Ship(destroyer), MovingShip(destroyer), SeeingShip(destroyer), ShootingShip(destroyer), RamingShip(destroyer)
    {
        name = "Supership";
        cell.symbol = 'S';
        if (!destroyer.enemyShips.empty())
        {
            destroyer.enemyShips.clear();
        }
    }
    SuperShip(Amphibious &amphibious) : Ship(amphibious), MovingShip(amphibious), SeeingShip(amphibious), ShootingShip(amphibious), RamingShip(*this)
    {
        name = "Supership";
        cell.symbol = 'S';
    }
    void action() override;
    void look () override;
    void move() override;
    void moveorram();
    void ram() override;
    void shoot() override;
};

#endif 
// SHIP_H
