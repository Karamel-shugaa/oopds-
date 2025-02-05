#ifndef SHIP_H
#define SHIP_H

#include <string>

// Forward declare Simulation to avoid circular includes here
class Simulation;

// Base abstract class
class Ship
{
protected:
    char symbol;
    int x, y;
    int lives;
    char team;
    Simulation &simulation;

public:
    Ship(Simulation &simulation) : simulation(simulation) {};
    virtual ~Ship() {}
    // use Ship(x, y) to set position
    void operator()(int x, int y) { setPosition(x, y); }
    virtual void action() = 0;
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

// Intermediate abstract classes
class MovingShip : virtual public Ship
{
public:
    MovingShip(Simulation &simulation) : Ship(simulation) {}
    virtual void move() = 0;
    virtual ~MovingShip() {}
};

class SeeingShip : virtual public Ship
{
public:
    SeeingShip(Simulation &simulation) : Ship(simulation) {}
    virtual void look() = 0;
    virtual ~SeeingShip() {}
};

class ShootingShip : virtual public Ship
{
public:
    ShootingShip(Simulation &simulation) : Ship(simulation) {}
    virtual void shoot() = 0;
    virtual ~ShootingShip() {}
};

class RamingShip : virtual public Ship
{
public:
    RamingShip(Simulation &simulation) : Ship(simulation) {}
    virtual void ram() = 0;
    virtual ~RamingShip() {}
};

class MiningShip : virtual public Ship
{
public:
    MiningShip(Simulation &simulation) : Ship(simulation) {}
    virtual void mine() = 0;
    virtual ~MiningShip() {}
};

// -----------------------------------------------------------------------------
// Derived classes
// -----------------------------------------------------------------------------

class BattleShip : public MovingShip, public SeeingShip, public ShootingShip
{
public:
    BattleShip(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), ShootingShip(simulation) {};
    void action() override;
    void move() override;
    void look() override;
    void shoot() override;
};

class Cruiser : public MovingShip, public SeeingShip, public RamingShip
{
public:
    Cruiser(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), RamingShip(simulation) {};
    void action() override;
    void move() override;
    void look() override;
    // void ram() override;
};

class Frigate : public ShootingShip
{
private:
    int targetx;
    int targety;
    // We add a direction index to cycle through 8 directions
    int directionIndex = 0;

public:
    Frigate(Simulation &simulation) : Ship(simulation), ShootingShip(simulation) {};
    void action() override;
    void shoot() override;
    int getTargetX() const;
    int getTargetY() const;
    void nextTarget();
};

class Destroyer : public MovingShip, public SeeingShip, public ShootingShip, public RamingShip
{
public:
    Destroyer(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), ShootingShip(simulation), RamingShip(simulation) {};
    void action() override;
    void move() override;
    void look() override;
    // void ram() override;
    void shoot() override;
};

class Corvette : public ShootingShip
{
public:
    Corvette(Simulation &simulation) : Ship(simulation), ShootingShip(simulation) {};
    void action() override;
    void shoot() override;
};

class Amphibious : public MovingShip, public SeeingShip, public ShootingShip
{
public:
    Amphibious(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), ShootingShip(simulation) {};
    void action() override;
    void move() override;
    void look() override;
    void shoot() override;
};

class SuperShip : public MovingShip, public SeeingShip, public ShootingShip, public RamingShip
{
public:
    SuperShip(Simulation &simulation) : Ship(simulation), MovingShip(simulation), SeeingShip(simulation), ShootingShip(simulation), RamingShip(simulation) {};
    void action() override;
    void move() override;
    void look() override;
    // void ram() override;
    void shoot() override;
};

// class Terrorist : public MovingShip, public MiningShip
// {
// public:
//     Terrorist(Simulation &simulation) : Ship(simulation), MovingShip(simulation), MiningShip(simulation) {};
//     void action() override;
//     void move() override;
//     void mine() override;
// };

#endif // SHIP_H
