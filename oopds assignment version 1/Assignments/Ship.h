#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <string>

// Main abstract class
class Ship
{
protected:
    std::string name;
    char symbol;
    int x, y;
    int lives;
    int kills = 0;
    bool deathflag = false;
    std::string team;

public:
    virtual void turn() = 0; // Pure virtual function responsible for starting a ship's turn

    Ship(const std::string &name, char symbol, int x, int y, int lives, const std::string &team);
    virtual ~Ship() {}

    char getSymbol() const;
    int getX() const;
    int getY() const;
    int getLives() const;
    int getKills() const;
    bool getDeathFlag() const;
    std::string getTeam() const;

    void takeDamage();                    // Records damage taken
    void setPosition(int newX, int newY); // Position of the ship
    void recordKills();                   // Record ship's kills
    void death();                         // Determines if a ship is dead
};

class movingship : virtual public Ship
{
public:
    virtual void move() = 0;
    virtual ~movingship() {}
};

class seeingship : virtual public Ship
{
public:
    virtual void look() = 0;
    virtual ~seeingship() {}
};

class shootingship : virtual public Ship
{
public:
    virtual void shoot() = 0;
    virtual ~shootingship() {}
};

class ramingship : virtual public Ship
{
public:
    virtual void ram() = 0;
    virtual ~ramingship() {}
};

class miningship : virtual public Ship
{
public:
    virtual void mine() = 0;
    virtual ~miningship() {}
};

// Derived classes
class BattleShip : public movingship, public seeingship, public shootingship
{
public:
    BattleShip(const std::string &name, char symbol, int x, int y, int lives, const std::string &team);
    void turn() override;
    void move() override;
    void look() override;
    void shoot() override;
};

class Cruiser : public movingship, public seeingship, public ramingship
{
public:
    Cruiser(const std::string &name, char symbol, int x, int y, int lives, const std::string &team);
    void turn() override;
    void move() override;
    void look() override;
    void ram() override;
};

class Frigate : public shootingship
{
private:
    int targetx;
    int targety;

public:
    Frigate(const std::string &name, char symbol, int x, int y, int lives, const std::string &team);
    void turn() override;
    void shoot() override;
    int getTargetX() const;
    int getTargetY() const;
    void nextTarget();
};

class Destroyer : public movingship, public seeingship, public shootingship, public ramingship
{
public:
    Destroyer(const std::string &name, char symbol, int x, int y, int lives, const std::string &team);
    void turn() override;
    void move() override;
    void look() override;
    void ram() override;
    void shoot() override;
};

class Corvette : public shootingship
{
public:
    Corvette(const std::string &name, char symbol, int x, int y, int lives, const std::string &team);
    void turn() override;
    void shoot() override;
};

class Amphibious : public movingship, public seeingship, public shootingship
{
public:
    Amphibious(const std::string &name, char symbol, int x, int y, int lives, const std::string &team);
    void turn() override;
    void move() override;
    void look() override;
    void shoot() override;
};

class SuperShip : public movingship, public seeingship, public shootingship, public ramingship
{
public:
    SuperShip(const std::string &name, char symbol, int x, int y, int lives, const std::string &team);
    void turn() override;
    void move() override;
    void look() override;
    void ram() override;
    void shoot() override;
};

class Terrorist : public movingship, public miningship
{
public:
    Terrorist(const std::string &name, char symbol, int x, int y, int lives, const std::string &team);
    void turn() override;
    void move() override;
    void mine() override;
};

#endif // SHIP_H
