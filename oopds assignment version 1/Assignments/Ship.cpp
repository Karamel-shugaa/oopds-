#include "Ship.h"
#include <cstdlib> // for rand()

// Ship base class
Ship::Ship(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : name(name), symbol(symbol), x(x), y(y), lives(lives), team(team) {}

char Ship::getSymbol() const { return symbol; }
int Ship::getX() const { return x; }
int Ship::getY() const { return y; }
int Ship::getLives() const { return lives; }
int Ship::getKills() const { return kills; }
bool Ship::getDeathFlag() const { return deathflag; }
std::string Ship::getTeam() const { return team; }

void Ship::takeDamage() {
    if (lives > 0) lives--;
    if (lives <= 0) death();
}

void Ship::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Ship::recordKills() {
    kills++;
}

void Ship::death() {
    if (!deathflag) {
        std::cout << name << " has been destroyed!\n";
        deathflag = true;
    }
}

// BattleShip
BattleShip::BattleShip(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

void BattleShip::turn() {
    std::cout << "BattleShip started its turn!\n";
    look();
    move();
    shoot();
    shoot();
}

void BattleShip::move() {
    int direction = rand() % 4;
    switch (direction) {
        case 0: x++; break;
        case 1: x--; break;
        case 2: y++; break;
        case 3: y--; break;
    }
    std::cout << "BattleShip " << name << " moved to (" << x << ", " << y << ").\n";
}

void BattleShip::look() {
    std::cout << "BattleShip " << name << " is scanning its surroundings.\n";
}

void BattleShip::shoot() {
    std::cout << "BattleShip " << name << " fires randomly.\n";
}

// Cruiser
Cruiser::Cruiser(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

void Cruiser::turn() {
    look();
    move();
    ram();
}

void Cruiser::look() {
    std::cout << "Cruiser " << name << " scans the area.\n";
}

void Cruiser::move() {
    std::cout << "Cruiser " << name << " moves cautiously.\n";
}

void Cruiser::ram() {
    std::cout << "Cruiser " << name << " rams an enemy ship!\n";
}


Frigate::Frigate(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team), targetx(0), targety(0) {}

void Frigate::turn() {
    std::cout << "Frigate " << name << " started its turn!\n";
    shoot();
}

void Frigate::shoot() {
    std::cout << "Frigate " << name << " fires at (" << targetx << ", " << targety << ")!\n";
}

int Frigate::getTargetX() const {
    return targetx;
}

int Frigate::getTargetY() const {
    return targety;
}

void Frigate::nextTarget() {
    targetx++;
    targety++;
}

Destroyer::Destroyer(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

void Destroyer::turn() {
    std::cout << "Destroyer " << name << " started its turn!\n";
    look();
    move();
    ram();
    shoot();
}

void Destroyer::move() {
    std::cout << "Destroyer " << name << " cautiously moves to a new position.\n";
}

void Destroyer::look() {
    std::cout << "Destroyer " << name << " scans the surroundings.\n";
}

void Destroyer::ram() {
    std::cout << "Destroyer " << name << " rams an enemy ship!\n";
}

void Destroyer::shoot() {
    std::cout << "Destroyer " << name << " fires and hits a target!\n";
}

Corvette::Corvette(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

void Corvette::turn() {
    std::cout << "Corvette " << name << " started its turn!\n";
    shoot();
}

void Corvette::shoot() {
    std::cout << "Corvette " << name << " fires a precise shot!\n";
}

Amphibious::Amphibious(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

void Amphibious::turn() {
    std::cout << "Amphibious " << name << " started its turn!\n";
    look();
    move();
    shoot();
}

void Amphibious::move() {
    std::cout << "Amphibious " << name << " moves to a strategic position.\n";
}

void Amphibious::look() {
    std::cout << "Amphibious " << name << " scans for opportunities.\n";
}

void Amphibious::shoot() {
    std::cout << "Amphibious " << name << " fires at an enemy and hits!\n";
}

SuperShip::SuperShip(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

void SuperShip::turn() {
    std::cout << "SuperShip " << name << " started its turn!\n";
    look();
    move();
    ram();
    shoot();
}

void SuperShip::move() {
    std::cout << "SuperShip " << name << " makes a bold move across the field.\n";
}

void SuperShip::look() {
    std::cout << "SuperShip " << name << " scans its surroundings.\n";
}

void SuperShip::ram() {
    std::cout << "SuperShip " << name << " rams an enemy ship powerfully!\n";
}

void SuperShip::shoot() {
    std::cout << "SuperShip " << name << " fires multiple shots with high accuracy!\n";
}

Terrorist::Terrorist(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

void Terrorist::turn() {
    std::cout << "Terrorist " << name << " started its turn!\n";
    move();
    mine();
}

void Terrorist::move() {
    std::cout << "Terrorist " << name << " moves to a random position.\n";
}

void Terrorist::mine() {
    std::cout << "Terrorist " << name << " places a mine on the battlefield.\n";
}
