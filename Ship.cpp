#include "Ship.h"
#include "ShipList.h" // Because we use ShipList::Iterator in shoot()
#include "Simulation.h"
#include <cstdlib>
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>

// ------------- Ship Base Class -------------
char Ship::getSymbol() const { return symbol; }
int Ship::getX() const { return x; }
int Ship::getY() const { return y; }
int Ship::getLives() const { return lives; }
void Ship::setSymbol(char newSymbol)
{
    symbol = newSymbol;
}
void Ship::setTeam(char newTeam)
{
    team = newTeam;
}

void SeeingShip::look()
{
    std::cout << "SeeingShip " << team << " at (" << x << ", " << y << ") is scanning its surroundings.\n";
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int nx = x + dx;
            int ny = y + dy;
            if (simulation.inBounds(nx, ny))
            {
                std::cout << "Found " << simulation.get_cell(nx, ny) << " at (" << nx << ", " << ny << ".)\n";
            }
        }
    }
}
// void Ship::setLives(int life) { lives = life; }
// int Ship::getKills() const { return kills; }
// bool Ship::getDeathFlag() const { return deathflag; }
// std::string Ship::getTeam() const { return team; }

// void Ship::takeDamage()
// {
//     if (lives > 0)
//         lives--;
//     if (lives <= 0)
//         death();
// }

void Ship::setPosition(int newX, int newY)
{
    x = newX;
    y = newY;
}

void Ship::displayDetails()
{
    std::cout << "Team: " << team << ", Symbol: " << symbol << ", Position: (" << x << ", " << y << "), Lives: " << lives << std::endl;
}
// void Ship::recordKills()
// {
//     kills++;
// }

// void Ship::death()
// {
//     if (!deathflag)
//     {
//         std::cout << name << " has been destroyed!\n";
//         deathflag = true;
//     }
// }

void BattleShip::action()
{
    std::cout << "BattleShip " << team << symbol << " at (" << x << ", " << y << ") started its turn.\n";
    look();
}

// Cannot move onto occupant cell unless upgraded to Destroyer

void BattleShip::look()
{
    // Implement a 3×3 scan around (x, y)
    char symbols[8];
    std::cout << "BattleShip " << team << symbol << " at (" << x << ", " << y << ") is scanning its surroundings.\n";
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int nx = x + dx;
            int ny = y + dy;
            if (ny == y && nx == x)
                continue;
            if (simulation.inBounds(nx, ny))
            {
                std::cout << "[ " << simulation.get_cell(nx, ny) << " at (" << nx << ", " << ny << ") ] ";
            }
        }
    }
    std::cout << std::endl;
}

void BattleShip::move() {}
// {
//     // Check if we are upgraded (≥4 kills => effectively a Destroyer)
//     // bool canMoveOntoOccupied = (kills >= 4);

//     // Check 4 neighbors up/down/left/right

//     // Validate new position
//     if (!simulation.canMoveTo(x, y, canMoveOntoOccupied))
//     {
//         // revert
//         x = oldX;
//         y = oldY;
//         std::cout << "BattleShip " << name << " couldn't move; invalid or occupied.\n";
//     }
//     else
//     {
//         // If occupant is enemy and canMoveOntoOccupied, ram/destroy them
//         if (canMoveOntoOccupied)
//         {
//             Ship *occupant = simulation.getOccupantAt(x, y);
//             if (occupant && occupant->getTeam() != team)
//             {
//                 std::cout << "BattleShip " << name << " (upgraded to Destroyer) rams " << occupant->getTeam()
//                           << " " << occupant->getSymbol() << "!\n";
//                 // Kill occupant
//                 occupant->takeDamage();
//                 while (occupant->getLives() > 0)
//                     occupant->takeDamage();
//                 recordKills();
//             }
//         }
//         std::cout << "BattleShip " << name << " moved to (" << x << ", " << y << ").\n";
//         simulation.updateBattlefieldPosition(this, oldX, oldY);
//     }
// }

void BattleShip::shoot() {}
// {
//     int dx = rand() % 5 - 2;
//     int dy = rand() % 5 - 2;
//     // city block distance <= 5 check
//     if (std::abs(dx) + std::abs(dy) > 5)
//         return;

//     int targetX = x + dx;
//     int targetY = y + dy;

//     if (!simulation.inBounds(targetX, targetY))
//         return;

//     Ship *occupant = simulation.getOccupantAt(targetX, targetY);
//     if (occupant && occupant->getTeam() != team)
//     {
//         occupant->takeDamage();
//         recordKills();

//         // If we just reached 4 kills => we can ram occupant cells
//         if (kills == 4)
//         {
//             std::cout << name << " has been upgraded to a Destroyer (can ram)!\n";
//         }
//     }
// }

void Cruiser::action() {}
// {
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     look();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     move();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     ram();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     // If kills >=3 => upgrade to Destroyer
//     if (kills >= 3)
//     {
//         std::cout << name << " (Cruiser) has reached 3 kills => upgrading to Destroyer.\n";
//         simulation.upgradeShip(this, "Destroyer");
//     }
// }

void Cruiser::look() {}
// {
//     std::cout << "Cruiser " << name << " scans the 3x3 area.\n";
//     for (int dy = -1; dy <= 1; dy++)
//     {
//         for (int dx = -1; dx <= 1; dx++)
//         {
//             int nx = x + dx;
//             int ny = y + dy;
//             if (simulation.inBounds(nx, ny))
//             {
//                 Ship *occupant = simulation.getOccupantAt(nx, ny);
//                 if (occupant)
//                 {
//                     std::cout << "  Cruiser sees " << occupant->getTeam() << " " << occupant->getSymbol()
//                               << " at (" << nx << ", " << ny << ").\n";
//                 }
//             }
//         }
//     }
// }

// Move to one adjacent cell, prefer an enemy occupant to ram
void Cruiser::move() {}
// {
//     std::cout << "Cruiser " << name << " tries to move.\n";

//     // Check 4 neighbors up/down/left/right
//     struct
//     {
//         int dx, dy;
//     } directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
//     Ship *enemyInNeighbor = nullptr;
//     int bestDx = 0, bestDy = 0;

//     for (auto &d : directions)
//     {
//         int nx = x + d.dx;
//         int ny = y + d.dy;
//         if (simulation.inBounds(nx, ny))
//         {
//             Ship *occupant = simulation.getOccupantAt(nx, ny);
//             // If occupant is an enemy, prefer it
//             if (occupant && occupant->getTeam() != team)
//             {
//                 enemyInNeighbor = occupant;
//                 bestDx = d.dx;
//                 bestDy = d.dy;
//                 break;
//             }
//             // else if occupant is null => a possible free cell we can move to
//             else if (!occupant && !enemyInNeighbor)
//             {
//                 // keep track in case we find no enemies
//                 bestDx = d.dx;
//                 bestDy = d.dy;
//             }
//         }
//     }
//     int oldX = x, oldY = y;
//     x += bestDx;
//     y += bestDy;

//     if (!simulation.canMoveTo(x, y, /* canMoveOntoOccupied= */ (enemyInNeighbor != nullptr)))
//     {
//         // revert
//         x = oldX;
//         y = oldY;
//         std::cout << "Cruiser " << name << " can't move.\n";
//     }
//     else
//     {
//         std::cout << "Cruiser " << name << " moves to (" << x << ", " << y << ").\n";
//         // If we stepped onto enemy occupant
//         if (enemyInNeighbor && enemyInNeighbor->getLives() > 0)
//         {
//             std::cout << "Cruiser " << name << " rams enemy occupant!\n";
//             enemyInNeighbor->takeDamage();
//             while (enemyInNeighbor->getLives() > 0)
//                 enemyInNeighbor->takeDamage();
//             recordKills();
//         }
//         simulation.updateBattlefieldPosition(this, oldX, oldY);
//     }
// }

// void Cruiser::ram()
// {
//     // Already performed the ramming logic in move() if occupant is found
//     std::cout << "Cruiser " << name << " rams an enemy ship if present in move.\n";
// }

void Frigate::action() {}
// {
//     std::cout << "Frigate " << name << " started its turn!\n";
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     shoot();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     // If kills >=3 => upgrade to Corvette
//     if (getKills() >= 3)
//     {
//         std::cout << name << " (Frigate) has 3 kills => upgrading to Corvette.\n";
//         simulation.upgradeShip(this, "Corvette");
//     }
// }

// Shoots exactly one of the 8 directions in a fixed repeating pattern
void Frigate::shoot() {}
// {
//     // Directions in clockwise order: up, up-right, right, down-right, down, down-left, left, up-left
//     static int D[8][2] = {
//         {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};
//     int dx = D[directionIndex][0];
//     int dy = D[directionIndex][1];
//     directionIndex = (directionIndex + 1) % 8;

//     int tx = x + dx;
//     int ty = y + dy;

//     std::cout << "Frigate " << name << " fires at (" << tx << ", " << ty << ")!\n";
//     if (simulation.inBounds(tx, ty))
//     {
//         Ship *occupant = simulation.getOccupantAt(tx, ty);
//         if (occupant && occupant->getTeam() != team)
//         {
//             occupant->takeDamage();
//             recordKills();
//         }
//     }
// }

int Frigate::getTargetX() const { return targetx; }
int Frigate::getTargetY() const { return targety; }
void Frigate::nextTarget()
{
    targetx++;
    targety++;
}

// ------------- Destroyer -------------
void Destroyer::action() {}
// {
//     std::cout << "Destroyer " << name << " started its turn!\n";
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     look();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     move();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     ram();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     shoot();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     shoot();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));

//     // If kills >=3 => upgrade to SuperShip
//     if (getKills() >= 3)
//     {
//         std::cout << name << " (Destroyer) reached 3 kills => upgrading to SuperShip.\n";
//         simulation.upgradeShip(this, "SuperShip");
//     }
// }

void Destroyer::move() {}
// {
//     std::cout << "Destroyer " << name << " cautiously moves to a new position.\n";
//     // Move like a Cruiser => prefer occupant if enemy
//     struct
//     {
//         int dx, dy;
//     } directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
//     Ship *enemyInNeighbor = nullptr;
//     int bestDx = 0, bestDy = 0;

//     for (auto &d : directions)
//     {
//         int nx = x + d.dx;
//         int ny = y + d.dy;
//         if (simulation.inBounds(nx, ny))
//         {
//             Ship *occupant = simulation.getOccupantAt(nx, ny);
//             if (occupant && occupant->getTeam() != team)
//             {
//                 enemyInNeighbor = occupant;
//                 bestDx = d.dx;
//                 bestDy = d.dy;
//                 break;
//             }
//             else if (!occupant && !enemyInNeighbor)
//             {
//                 bestDx = d.dx;
//                 bestDy = d.dy;
//             }
//         }
//     }

//     int oldX = x, oldY = y;
//     x += bestDx;
//     y += bestDy;
//     if (!simulation.canMoveTo(x, y, true)) // Destroyer can always step on occupant
//     {
//         x = oldX;
//         y = oldY;
//     }
//     else
//     {
//         std::cout << "Destroyer " << name << " moves to (" << x << ", " << y << ").\n";
//         if (enemyInNeighbor && enemyInNeighbor->getLives() > 0)
//         {
//             std::cout << "Destroyer " << name << " rams occupant!\n";
//             while (enemyInNeighbor->getLives() > 0)
//                 enemyInNeighbor->takeDamage();
//             recordKills();
//         }
//         simulation.updateBattlefieldPosition(this, oldX, oldY);
//     }
// }

void Destroyer::look() {}
// {
//     std::cout << "Destroyer " << name << " scans the surroundings.\n";
//     for (int dy = -1; dy <= 1; dy++)
//     {
//         for (int dx = -1; dx <= 1; dx++)
//         {
//             int nx = x + dx;
//             int ny = y + dy;
//             if (simulation.inBounds(nx, ny))
//             {
//                 Ship *occupant = simulation.getOccupantAt(nx, ny);
//                 if (occupant)
//                 {
//                     std::cout << "  " << occupant->getTeam() << " " << occupant->getSymbol()
//                               << " at (" << nx << ", " << ny << ").\n";
//                 }
//             }
//         }
//     }
// }

// void Destroyer::ram()
// {
//     std::cout << "Destroyer " << name << " rams an enemy ship if occupying the same cell.\n";
//     // In move() we do the actual occupant removal
// }

void Destroyer::shoot() {}
// {
//     std::cout << "Destroyer " << name << " fires!\n";
//     // Same city block random approach as Battleship
//     int dx = rand() % 5 - 2;
//     int dy = rand() % 5 - 2;
//     if (std::abs(dx) + std::abs(dy) > 5)
//         return;
//     int tx = x + dx;
//     int ty = y + dy;

//     if (!simulation.inBounds(tx, ty))
//         return;

//     Ship *occupant = simulation.getOccupantAt(tx, ty);
//     if (occupant && occupant->getTeam() != team)
//     {
//         occupant->takeDamage();
//         recordKills();
//     }
// }

// ------------- Corvette -------------

// void Corvette::action()
// {
//     std::cout << "Corvette " << name << " started its turn!\n";
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     shoot();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
// }

// Shoots a random one of the 8 neighboring cells each turn
void Corvette::shoot() {}
// {
//     std::cout << "Corvette " << name << " fires randomly at one neighbor!\n";
//     static int D[8][2] = {
//         {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};
//     int r = rand() % 8;
//     int tx = x + D[r][0];
//     int ty = y + D[r][1];

//     if (simulation.inBounds(tx, ty))
//     {
//         Ship *occupant = simulation.getOccupantAt(tx, ty);
//         if (occupant && occupant->getTeam() != team)
//         {
//             occupant->takeDamage();
//             recordKills();
//         }
//     }
// }

// ------------- Amphibious -------------

void Amphibious::action() {}
// {
//     std::cout << "Amphibious " << name << " started its turn!\n";
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     look();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     move();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     shoot();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));

//     // If kills >=4 => becomes SuperShip
//     if (getKills() >= 4)
//     {
//         std::cout << name << " (Amphibious) reached 4 kills => upgrading to SuperShip.\n";
//         simulation.upgradeShip(this, "SuperShip");
//     }
// }

void Amphibious::move() {}
// {
//     std::cout << "Amphibious " << name << " moves to a strategic position.\n";
//     // same move logic as Battleship, but can step on land
//     int direction = rand() % 4;
//     int oldX = x, oldY = y;
//     switch (direction)
//     {
//     case 0:
//         x++;
//         break;
//     case 1:
//         x--;
//         break;
//     case 2:
//         y++;
//         break;
//     case 3:
//         y--;
//         break;
//     }

//     // Let amphibious move on land => canMoveOntoOccupied = false for occupant check,
//     // but we skip island check in the canMoveTo function
//     if (!simulation.canMoveTo(x, y, false, /*amphibious=*/true))
//     {
//         x = oldX;
//         y = oldY;
//     }
//     else
//     {
//         std::cout << "Amphibious " << name << " moved to (" << x << ", " << y << ").\n";
//         simulation.updateBattlefieldPosition(this, oldX, oldY);
//     }
// }

void Amphibious::look() {}
// {
//     std::cout << "Amphibious " << name << " scans for opportunities.\n";
//     for (int dy = -1; dy <= 1; dy++)
//     {
//         for (int dx = -1; dx <= 1; dx++)
//         {
//             int nx = x + dx;
//             int ny = y + dy;
//             if (simulation.inBounds(nx, ny))
//             {
//                 Ship *occupant = simulation.getOccupantAt(nx, ny);
//                 if (occupant)
//                 {
//                     std::cout << "  Found " << occupant->getTeam() << " " << occupant->getSymbol()
//                               << " at (" << nx << ", " << ny << ").\n";
//                 }
//             }
//         }
//     }
// }

void Amphibious::shoot() {}
// {
//     std::cout << "Amphibious " << name << " fires at an enemy!\n";
//     // same random city block approach
//     int dx = rand() % 5 - 2;
//     int dy = rand() % 5 - 2;
//     if (std::abs(dx) + std::abs(dy) > 5)
//         return;
//     int tx = x + dx;
//     int ty = y + dy;
//     if (!simulation.inBounds(tx, ty))
//         return;

//     Ship *occupant = simulation.getOccupantAt(tx, ty);
//     if (occupant && occupant->getTeam() != team)
//     {
//         occupant->takeDamage();
//         recordKills();
//     }
// }

// ------------- SuperShip -------------

// void SuperShip::action()
// {
//     std::cout << "SuperShip " << name << " started its turn!\n";
//     look();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     move();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     ram();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     shoot();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     shoot();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     shoot();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
// }

void SuperShip::move() {}
// {
//     std::cout << "SuperShip " << name << " makes a bold move.\n";
//     // Moves like a Cruiser => can ram occupant
//     struct
//     {
//         int dx, dy;
//     } directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
//     Ship *enemyInNeighbor = nullptr;
//     int bestDx = 0, bestDy = 0;

//     for (auto &d : directions)
//     {
//         int nx = x + d.dx;
//         int ny = y + d.dy;
//         if (simulation.inBounds(nx, ny))
//         {
//             Ship *occupant = simulation.getOccupantAt(nx, ny);
//             if (occupant && occupant->getTeam() != team)
//             {
//                 enemyInNeighbor = occupant;
//                 bestDx = d.dx;
//                 bestDy = d.dy;
//                 break;
//             }
//             else if (!occupant && !enemyInNeighbor)
//             {
//                 bestDx = d.dx;
//                 bestDy = d.dy;
//             }
//         }
//     }
//     int oldX = x, oldY = y;
//     x += bestDx;
//     y += bestDy;
//     if (!simulation.canMoveTo(x, y, true))
//     {
//         x = oldX;
//         y = oldY;
//     }
//     else
//     {
//         std::cout << "SuperShip " << name << " moves to (" << x << ", " << y << ").\n";
//         if (enemyInNeighbor && enemyInNeighbor->getLives() > 0)
//         {
//             std::cout << "SuperShip " << name << " rams occupant!\n";
//             while (enemyInNeighbor->getLives() > 0)
//                 enemyInNeighbor->takeDamage();
//             recordKills();
//         }
//         simulation.updateBattlefieldPosition(this, oldX, oldY);
//     }
// }

void SuperShip::look() {}
// {
//     std::cout << "SuperShip " << name << " scans its surroundings.\n";
//     for (int dy = -1; dy <= 1; dy++)
//     {
//         for (int dx = -1; dx <= 1; dx++)
//         {
//             int nx = x + dx;
//             int ny = y + dy;
//             if (simulation.inBounds(nx, ny))
//             {
//                 Ship *occupant = simulation.getOccupantAt(nx, ny);
//                 if (occupant)
//                 {
//                     std::cout << "  sees " << occupant->getTeam() << " " << occupant->getSymbol()
//                               << " at (" << nx << ", " << ny << ").\n";
//                 }
//             }
//         }
//     }
// }

// void SuperShip::ram()
// {
//     std::cout << "SuperShip " << name << " rams an enemy if present.\n";
// }

void SuperShip::shoot() {}
// {
//     std::cout << "SuperShip " << name << " fires multiple shots with high accuracy!\n";
//     // 3 shots => we call this function 3 times in action() anyway
//     int tx = x + (rand() % 5 - 2);
//     int ty = y + (rand() % 5 - 2);
//     if (!simulation.inBounds(tx, ty))
//         return;
//     Ship *occupant = simulation.getOccupantAt(tx, ty);
//     if (occupant && occupant->getTeam() != team)
//     {
//         occupant->takeDamage();
//         recordKills();
//     }
// }

// ------------- Terrorist -------------

// void Terrorist::action()
// {
//     std::cout << "Terrorist " << name << " started its turn!\n";
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     move();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     mine();
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
// }

// void Terrorist::move()
// {
//     std::cout << "Terrorist " << name << " moves to a random position.\n";
//     int oldX = x, oldY = y;
//     int direction = rand() % 4;
//     switch (direction)
//     {
//     case 0:
//         x++;
//         break;
//     case 1:
//         x--;
//         break;
//     case 2:
//         y++;
//         break;
//     case 3:
//         y--;
//         break;
//     }
//     if (!simulation.canMoveTo(x, y, false))
//     {
//         x = oldX;
//         y = oldY;
//     }
//     else
//     {
//         simulation.updateBattlefieldPosition(this, oldX, oldY);
//     }
// }

// void Terrorist::mine()
// {
//     std::cout << "Terrorist " << name << " places a mine on the battlefield (not fully implemented).\n";
//     // You could add logic if a ship steps here, it blows up, etc.
// }
