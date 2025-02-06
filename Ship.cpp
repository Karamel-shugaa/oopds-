#include "Ship.h"
#include "ShipList.h" // Because we use ShipList::Iterator in shoot()
#include "Simulation.h"
#include <cstdlib>
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>

// ------------- Ship Base Class -------------

char Ship::getSymbol() const { return cell.symbol; }
int Ship::getX() const { return cell.x; }
int Ship::getY() const { return cell.y; }
int Ship::getLives() const { return lives; }
char Ship::getTeam() const { return team; }
void Ship::setSymbol(char newSymbol)
{
    cell.symbol = newSymbol;
}
void Ship::setTeam(char newTeam)
{
    team = newTeam;
}

void Ship::setName(std::string newName)
{
    name = newName;
}

void SeeingShip::look()
{
    // Implement a 3×3 scan around (x, y)
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
    int index = 0;
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            int nx = cell.x + x;
            int ny = cell.y + y;
            if (ny == cell.y && nx == cell.x)
                continue;
            if (simulation.inBounds(nx, ny))
            {
                neighbouring[index] = {simulation.getCell(nx, ny), nx, ny};
                std::cout << "[ " << simulation.getCell(nx, ny) << " at (" << nx << ", " << ny << ") ] ";
            }
            else
            {
                neighbouring[index] = {'X', nx, ny};
                std::cout << "[ X" << " at (" << nx << ", " << ny << ") ] ";
            }
            index++;
        }
    }
    std::cout << std::endl;
}
// void Ship::setLives(int life) { lives = life; }
// int Ship::getKills() const { return kills; }
// bool Ship::getDeathFlag() const { return deathflag; }
// std::string Ship::getTeam() const { return team; }

void Ship::die()
{
    simulation.setCell(cell.x, cell.y, '0');
    lives--;
    if (lives == 0)
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") has fallen!\n";
    }
    else
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") has " << lives << " lives left.\n";
    }
    simulation.removeShip(this);
}

void Ship::setPosition(int newX, int newY)
{
    cell.x = newX;
    cell.y = newY;
}

void Ship::displayDetails()
{
    std::cout << "Team: " << team << ", Symbol: " << cell.symbol << ", Position: (" << cell.x << ", " << cell.y << "), Lives: " << lives << std::endl;
}
void Ship::recordKill()
{
    kills++;
}

void BattleShip::action()
{
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    move();
    simulation.display();
    shoot();
    shoot();
    // if (kills == 4)
    // {
    //     std::cout << name << team << cell.symbol << " has reached 4 kills => upgrading to Cruiser.\n";
    //     simulation.upgradeShip(this, "Cruiser");
    // }
}

void BattleShip::move()
{
    int index = rand() % 8;
    bool free = false;
    for (int i = 0; i < 8; i++)
    {
        if (neighbouring[i].symbol == '0')
        {
            while (neighbouring[index].symbol != '0')
            {
                index = rand() % 8;
            }
            free = true;
            break;
        }
    }
    if (!free)
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        return;
    }
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is moving to (" << neighbouring[index].x << ", " << neighbouring[index].y << ").\n";
    simulation.setCell(cell.x, cell.y, '0');
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
    std::cout << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
}

void BattleShip::shoot()
{
    int dx, dy;
    do
    {
        dx = (rand() % 11) - 5; // num between -5 and 5
        dy = (rand() % 11) - 5;
    } while (abs(dx) + abs(dy) > 5 || (dx == 0 && dy == 0)); // avoid self targeting and limit to city block distance of 5
    int target_x = cell.x + dx;
    int target_y = cell.y + dy;

    if (!simulation.inBounds(target_x, target_y))
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and missed (out of bounds).\n";
        return;
    }
    std::cout << "Symbol at target: " << simulation.getCell(target_x, target_y) << std::endl;
    Ship *target = simulation.getShipAt(target_x, target_y);
    if (target != nullptr)
    {
        if (target->getTeam() != team)
        {
            std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and hit " << target->getTeam() << ' ' << target->getSymbol() << "!\n";
            target->die();
            recordKill();
        }
        // to be removed
        else
        {
            std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and missed (friendly fire).\n";
        }
    }
    else
    {
        std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
    }
}

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

// void Cruiser::look() {}
// {
//     std::cout << "Cruiser " << name << " scans the 3x3 area.\n";
//     for (int y = -1; y <= 1; y++)f
//     {
//         for (int y = -1; y <= 1; y++)
//         {
//             int nx = x + y;
//             int ny = y + y;
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
//         int y, y;
//     } directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
//     Ship *enemyInNeighbor = nullptr;
//     int besty = 0, besty = 0;

//     for (auto &d : directions)
//     {
//         int nx = x + d.y;
//         int ny = y + d.y;
//         if (simulation.inBounds(nx, ny))
//         {
//             Ship *occupant = simulation.getOccupantAt(nx, ny);
//             // If occupant is an enemy, prefer it
//             if (occupant && occupant->getTeam() != team)
//             {
//                 enemyInNeighbor = occupant;
//                 besty = d.y;
//                 besty = d.y;
//                 break;
//             }
//             // else if occupant is null => a possible free cell we can move to
//             else if (!occupant && !enemyInNeighbor)
//             {
//                 // keep track in case we find no enemies
//                 besty = d.y;
//                 besty = d.y;
//             }
//         }
//     }
//     int oly = x, oly = y;
//     x += besty;
//     y += besty;

//     if (!simulation.canMoveTo(x, y, /* canMoveOntoOccupied= */ (enemyInNeighbor != nullptr)))
//     {
//         // revert
//         x = oly;
//         y = oly;
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
//         simulation.updateBattlefieldPosition(this, oly, oly);
//     }
// }

// void Cruiser::ram()
// {
//     // Alreay performed the ramming logic in move() if occupant is found
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
//     int y = D[directionIndex][0];
//     int y = D[directionIndex][1];
//     directionIndex = (directionIndex + 1) % 8;

//     int tx = x + y;
//     int ty = y + y;

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
//         int y, y;
//     } directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
//     Ship *enemyInNeighbor = nullptr;
//     int besty = 0, besty = 0;

//     for (auto &d : directions)
//     {
//         int nx = x + d.y;
//         int ny = y + d.y;
//         if (simulation.inBounds(nx, ny))
//         {
//             Ship *occupant = simulation.getOccupantAt(nx, ny);
//             if (occupant && occupant->getTeam() != team)
//             {
//                 enemyInNeighbor = occupant;
//                 besty = d.y;
//                 besty = d.y;
//                 break;
//             }
//             else if (!occupant && !enemyInNeighbor)
//             {
//                 besty = d.y;
//                 besty = d.y;
//             }
//         }
//     }

//     int oly = x, oly = y;
//     x += besty;
//     y += besty;
//     if (!simulation.canMoveTo(x, y, true)) // Destroyer can always step on occupant
//     {
//         x = oly;
//         y = oly;
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
//         simulation.updateBattlefieldPosition(this, oly, oly);
//     }
// }

// void Destroyer::look() {}
// {
//     std::cout << "Destroyer " << name << " scans the surroundings.\n";
//     for (int y = -1; y <= 1; y++)
//     {
//         for (int y = -1; y <= 1; y++)
//         {
//             int nx = x + y;
//             int ny = y + y;
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
//     int y = rand() % 5 - 2;
//     int y = rand() % 5 - 2;
//     if (std::abs(y) + std::abs(y) > 5)
//         return;
//     int tx = x + y;
//     int ty = y + y;

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
//     int oly = x, oly = y;
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
//         x = oly;
//         y = oly;
//     }
//     else
//     {
//         std::cout << "Amphibious " << name << " moved to (" << x << ", " << y << ").\n";
//         simulation.updateBattlefieldPosition(this, oly, oly);
//     }
// }

// void Amphibious::look() {}
// {
//     std::cout << "Amphibious " << name << " scans for opportunities.\n";
//     for (int y = -1; y <= 1; y++)
//     {
//         for (int y = -1; y <= 1; y++)
//         {
//             int nx = x + y;
//             int ny = y + y;
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
//     int y = rand() % 5 - 2;
//     int y = rand() % 5 - 2;
//     if (std::abs(y) + std::abs(y) > 5)
//         return;
//     int tx = x + y;
//     int ty = y + y;
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
//         int y, y;
//     } directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
//     Ship *enemyInNeighbor = nullptr;
//     int besty = 0, besty = 0;

//     for (auto &d : directions)
//     {
//         int nx = x + d.y;
//         int ny = y + d.y;
//         if (simulation.inBounds(nx, ny))
//         {
//             Ship *occupant = simulation.getOccupantAt(nx, ny);
//             if (occupant && occupant->getTeam() != team)
//             {
//                 enemyInNeighbor = occupant;
//                 besty = d.y;
//                 besty = d.y;
//                 break;
//             }
//             else if (!occupant && !enemyInNeighbor)
//             {
//                 besty = d.y;
//                 besty = d.y;
//             }
//         }
//     }
//     int oly = x, oly = y;
//     x += besty;
//     y += besty;
//     if (!simulation.canMoveTo(x, y, true))
//     {
//         x = oly;
//         y = oly;
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
//         simulation.updateBattlefieldPosition(this, oly, oly);
//     }
// }

// void SuperShip::look() {}
// {
//     std::cout << "SuperShip " << name << " scans its surroundings.\n";
//     for (int y = -1; y <= 1; y++)
//     {
//         for (int y = -1; y <= 1; y++)
//         {
//             int nx = x + y;
//             int ny = y + y;
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
//     int oly = x, oly = y;
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
//         x = oly;
//         y = oly;
//     }
//     else
//     {
//         simulation.updateBattlefieldPosition(this, oly, oly);
//     }
// }

// void Terrorist::mine()
// {
//     std::cout << "Terrorist " << name << " places a mine on the battlefield (not fully implemented).\n";
//     // You could add logic if a ship steps here, it blows up, etc.
// }
