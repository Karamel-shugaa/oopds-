#include "Ship.h"

Ship::Ship(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : name(name), symbol(symbol), x(x), y(y), lives(lives), team(team) {}

char Ship::getSymbol() const { return symbol; }
int Ship::getX() const { return x; }
int Ship::getY() const { return y; }
int Ship::getLives() const { return lives; }
int Ship::getkills() const { return kills; }
bool Ship::getdeathflag() const { return deathflag; }
std::string Ship::getTeam() const { return team; }

void Ship::takeDamage()
{
    lives--;
}

void Ship::setPosition(int newX, int newY)
{
    x = newX;
    y = newY;
}
void Ship::recordkills()
{
    kills++;
}
void Ship::death()
{
    if (lives <= 0)
    {
        std::cout << name << " has been destroyed!\n";
        deathflag = true;
    }
}

BattleShip::BattleShip(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

void BattleShip::turn()
{
    std::cout << "BattleShip started its turn!!" << std::endl;
    look();
    move();
    shoot();
    shoot();
}
void BattleShip::look()
{
    std::cout << "Battleship " << name << " is scanning its surrouding.\n";
}

void BattleShip::move()
{
    int direction = rand() % 4;
    switch (direction)
    {
    case 0:
        x++;
        break; // Move right
    case 1:
        x--;
        break; // Move left
    case 2:
        y++;
        break; // Move down
    case 3:
        y--;
        break; // Move up
    }
    std::cout << "Battleship " << name << " moved to (" << x << ", " << y << ").\n";
}

void BattleShip::shoot()
{
    // if statment, first one if it shoots randomly, second one if it hits a target
    std::cout << "Battleship " << name << "fires randomly\n";
    std::cout << "Battleship " << name << " fires at enemy positions!\n";
}

Cruiser::Cruiser(const std::string &name, char symbol, int x, int y, int lives, const std::string &team)
    : Ship(name, symbol, x, y, lives, team) {}

void Cruiser::turn()
{
    std::cout << "Cruiser started its turn!!" << std::endl;
    look();
    move();
    ram();
}

void Cruiser::look()
{
    std::cout << "Cruiser " << name << " scans the nearby area.\n";
}

void Cruiser::move()
{
    // if statment if it scans an enemy ship = false
    std::cout << "Cruiser " << name << " moves cautiously.\n";
}

void Cruiser::ram()
{
    // if statment if scans enemy ship = true
    std::cout << "Cruiser " << name << "Rams and destroies an enemy ship!!\n";
}

void Destroyer::turn()
{

    std::cout << "Destroyer started its turn!!" << std::endl;
    look();
    move();
    ram();
    shoot();
    shoot();
}
void Destroyer::look()
{
    std::cout << "Destroyer " << name << " scans the nearby area.\n";
}
void Destroyer::move()
{
    // if statment if it scans enemy ship = false
    std::cout << "Destroyer " << name << " moves cautiously.\n";
}

void Destroyer::ram()
{
    // if statment if scans enemy ship = true
    std::cout << "Destroyer " << name << "Rams and destroies an enemy ship!!\n";
}

void Destroyer::shoot()
{
    // if statment, first one if it shoots randomly, second one if it hits a target
    std::cout << "Destroyer " << name << " Fires!!\n";
    std::cout << "Destroyer " << name << " Hits an enemy!!\n";
}
void Frigate::turn()
{
    std::cout << "Frigate " << name << " started its turn!!\n";
    shoot();
}
void Frigate::shoot()
{
    std::cout << "Frigate " << name << " Fires!!\n";
}
int Frigate::gettargetx() const
{
    return targetx;
}
int Frigate::gettargety() const
{
    return targety;
}
int Frigate::nexttarget()
{
    targetx++;
    targety++;
}
void Corvette::turn()
{
    std::cout << "Corvette " << name << " started its turn!!\n"
              << std::endl;
    shoot();
}
void Corvette::shoot()
{
    std::cout << "Corvette " << name << " has taken a shot!!\n"
              << std::endl;
}
void Amphibious::turn()
{
    std::cout << "Amphibious " << name << " started its turn!!\n"
              << std::endl;
    look();
    move();
    shoot();
    shoot();
}
void Amphibious::move()
{
    std::cout << "Amphibious " << name << " has moved!!\n"
              << std::endl;
}
void Amphibious::shoot()
{
    // if statment, first one if it shoots randomly, second one if it hits a target
    std::cout << "Amphibious " << name << " shoots!!\n"
              << std::endl;
    std::cout << "Amphibious " << name << " hits an enemy!!\n";
}
void SuperShip::turn()
{

    std::cout << "SuperShip " << name << " started its turn!!\n"
              << std::endl;
    look();
    move();
    ram();
    shoot();
    shoot();
    shoot();
}
void SuperShip::look()
{
    std::cout << "SuperShip " << name << " is scanning its surroundings\n";
}
void SuperShip::move()
{
    // if statment if it scans enemy ship = false

    std::cout << "SuperShip " << name << " has moved!!\n"
              << std::endl;
}
void SuperShip::ram()
{
    // if statment if it scans enemy ship = true
    std::cout << "SuperShip " << name << " rams another ship!!\n";
}
void SuperShip::shoot()
{
    // if statment, first one if it shoots randomly, second one if it hits a target
    std::cout << "SuperShip " << name << " shoots!!\n";
    std::cout << "SuperShip " << name << " shoots!!\n";
}
void Terrorist::turn()
{

    std::cout << "Terrorist " << name << "started its turn!!" << std::endl;
    move();
    mine();
}
void Terrorist::move()
{
    std::cout << "Terrorist has moved to a random position!!" << std::endl;
}
void Terrorist::mine()
{

    std::cout << "Terrorist has placed a mine!!" << std::endl;
}