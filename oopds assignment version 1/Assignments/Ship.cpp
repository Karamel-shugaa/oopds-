#include "Ship.h"
#include <cstdlib> // for rand()

// Ship base class
Ship::Ship(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : name(name), symbol(symbol), x(x), y(y), lives(lives), team(team) {}

// getters for ship class
char Ship::getSymbol() const { return symbol; }
int Ship::getX() const { return x; }
int Ship::getY() const { return y; }
int Ship::getLives() const { return lives; }
int Ship::getKills() const { return kills; } 
bool Ship::getDeathFlag() const { return deathflag; }
std::string Ship::getTeam() const { return team; }

//deathflag of a ship (if true ship is dead and cant return to qeue)
void Ship::death() {
    if (!deathflag) {
        std::cout << name << " has been destroyed!\n";
        deathflag = true;
    }
}

// ship live management, if lives <= 0 calls death 
void Ship::takeDamage() {
    if (lives > 0) lives--;
    if (lives <= 0) death();
}

// updates ship position after an action
void Ship::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}
//kill counter to manage ship upgrades
void Ship::recordKills() {
    kills++;
}



// BattleShip 
BattleShip::BattleShip(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

//sequence of actions a battleship takes each turn
void BattleShip::turn() {
    std::cout << "BattleShip started its turn!\n";
    look();
    move();
    shoot();
    shoot();
}

//randomize movement
//(for team) tba avoiding to move to an allies coardonates
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

//scans the location of ally ships to avoid hitting them
//(for team) tba function to store ally ship coardonates
void BattleShip::look() {
    std::cout << "BattleShip " << name << " is scanning its surroundings.\n";
}

//controls the ship shooting range and disallows it to shoot its target
//(for team) tba if statment to avoid shooting ally ship coardonates 
void BattleShip::shoot() {
    std::cout << "BattleShip " << name << " fires randomly.\n";
}

// Cruiser
Cruiser::Cruiser(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}


//sequence of actions a cruiser takes each turn
void Cruiser::turn() {
    look();
    move();
    ram();
}

//checks if any ally or any ship are in its movement range
//(for team) tba stores coardonates for enemy and ally ship in movement range
void Cruiser::look() {
    std::cout << "Cruiser " << name << " scans the area.\n";
}

//controls movement and is responsible for not moving on ally ships
//(for team) tba avoiding to move to an allies coardonates
void Cruiser::move() {
    std::cout << "Cruiser " << name << " moves cautiously.\n";
}

//forces a cruiser to ram an enemy if it is in range
//(for team) tba a function that does that
void Cruiser::ram() {
    std::cout << "Cruiser " << name << " rams an enemy ship!\n";
}

// Frigate
Frigate::Frigate(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team), targetx(0), targety(0) {}

//sequence of actions a Frigate takes each turn
void Frigate::turn() {
    std::cout << "Frigate " << name << " started its turn!\n";
    shoot();
}

//shoots methadolically in a clockwise direction
//(for team) tba function that implements that
void Frigate::shoot() {
    std::cout << "Frigate " << name << " fires at (" << targetx << ", " << targety << ")!\n";
}

//getters for current target coardonates 
int Frigate::getTargetX() const {
    return targetx;
}

int Frigate::getTargetY() const {
    return targety;
}

//calculates the next target coardonates
//(for team) tba making this  function make sense
void Frigate::nextTarget() {
    targetx++;
    targety++;
}

//Destroyer 
Destroyer::Destroyer(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

//sequence of actions a destroyer takes each turn
void Destroyer::turn() {
    std::cout << "Destroyer " << name << " started its turn!\n";
    look();
    move();
    ram();
    shoot();
}

//controls movement
//(for team) tba avoiding to move to an allies coardonates
void Destroyer::move() {
    std::cout << "Destroyer " << name << " cautiously moves to a new position.\n";
}

//looks and stores location of all ally ships, stores location of enemy ship in area
//(for team) nah i would win (if youre reading this i am done for the day it is 12:14 i will continue the comments later)
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
