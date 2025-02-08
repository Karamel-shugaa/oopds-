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

// void Ship::setLives(int life) { lives = life; }
// int Ship::getKills() const { return kills; }
// bool Ship::getDeathFlag() const { return deathflag; }
// std::string Ship::getTeam() const { return team; }

void Ship::die()
{
    simulation.setCell(cell.x, cell.y, '0'); // Mark cell as empty
    lives--;

    if (lives == 0)
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") has fallen!\n";
        simulation.removeShip(this);
        simulation.getRespawnQueue().enqueue(this); // (New) Add ship to respawn queue
    }
    else
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") has " << lives << " lives left.\n";
    }
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
//-------------BattleShip------------------

void BattleShip::action()
{
    std::cout << "-------------------- BATTLESHIP -------------------- \n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    move();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    simulation.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    // if (kills == 4)
    // {
    //     std::cout << name << team << cell.symbol << " has reached 4 kills => upgrading to Cruiser.\n";
    //     simulation.upgradeShip(this, "Cruiser");
    // }
}

void BattleShip::look()
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

            index++;
        }
    }
    std::cout << std::endl;
}
void BattleShip::move()
{
    int freeIndices[8];
    int freeCount = 0;

    for (int i = 0; i < 8; i++)
    {
        if (neighbouring[i].symbol == '0')
        {
            freeIndices[freeCount] = i;
            freeCount++;
        }
    }

    if (freeCount == 0) // No free spaces, avoid infinite loop
    {
        std::cout << name << " cannot move.\n";
        return;
    }

    int index = freeIndices[rand() % freeCount];

    std::cout << name << " is moving to (" << neighbouring[index].x << ", " << neighbouring[index].y << ").\n";
    simulation.setCell(cell.x, cell.y, '0');
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
}

void BattleShip::shoot()
{
    int dx, dy, target_x, target_y;
    Ship *target = nullptr;

    do
    {
        dx = (rand() % 11) - 5; // -5 to 5
        dy = (rand() % 11) - 5;
    } while (abs(dx) + abs(dy) > 5 && (dx == 0 && dy == 0));
    // Prevent shooting at (0,0)

    target_x = cell.x + dx;
    target_y = cell.y + dy;

    target = simulation.getShipAt(target_x, target_y);

    // Prevent friendly fire and keeps shooting in bounds
    if (!simulation.inBounds(target_x, target_y) || (target != nullptr && target->getTeam() == team))
    {
        // shooting action
        if (target != nullptr)
        {
            std::cout << name << " shot at (" << target_x << "," << target_y
                      << ") and destroyed " << target->getName() << "!\n";
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " shot at (" << target_x << "," << target_y
                      << ") and missed!\n";
        }
    }
}

//-------------Cruiser------------------
void Cruiser::action()
{
    std::cout << "-------------------- CRUISER -------------------- \n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    moveorram();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    simulation.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
void Cruiser::look()
{
    // Implement a 3×3 scan around (x, y)
    std::cout << name << "  " << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
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

            index++;
        }
    }
    std::cout << std::endl;
}
void Cruiser::move()
{
    int freeIndices[8];
    int freeCount = 0;

    for (int i = 0; i < 8; i++)
    {
        if (neighbouring[i].symbol == '0')
        {
            freeIndices[freeCount] = i;
            freeCount++;
        }
    }

    if (freeCount == 0) // No free spaces, avoid infinite loop
    {
        std::cout << name << " cannot move.\n";
        return;
    }

    int index = freeIndices[rand() % freeCount];

    std::cout << name << " is moving to (" << neighbouring[index].x << ", " << neighbouring[index].y << ").\n";
    simulation.setCell(cell.x, cell.y, '0');
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
}

void Cruiser::ram()
{
    int index = 0;
    Ship *target = nullptr;

    while (index < 8)
    {
        target = simulation.getShipAt(neighbouring[index].x, neighbouring[index].y);

        if (target == nullptr || target->getTeam() == team)
        {
            index++;
            continue;
        }

        std::cout << name << " RAMMED and DESTROYED!! " << target->getName() << " at (" << neighbouring[index].x << ", " << neighbouring[index].y << ")!\n";

        target->die();
        recordKill();

        // Move into the ramming position
        simulation.setCell(cell.x, cell.y, '0'); // Clear old position
        setPosition(neighbouring[index].x, neighbouring[index].y);
        simulation.setCell(cell.x, cell.y, cell.symbol);

        return;
    }
}
void Cruiser::moveorram()
{
    for (int i = 0; i < 8; i++)
    {
        Ship *target = simulation.getShipAt(neighbouring[i].x, neighbouring[i].y);
        if (target != nullptr && target->getTeam() != team)
        {
            ram();
            return;
        }
    }
    move();
}

//-------------Frigate------------------

void Frigate::action()
{
    std::cout << "-------------------- FRIGATE -------------------- \n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    simulation.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void Frigate::shoot()
{
    static const int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    static const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

    Ship *target = nullptr;

    do
    {
        // Calculate the current target position based on `directionIndex`
        targetx = cell.x + dx[directionIndex];
        targety = cell.y + dy[directionIndex];

        // Get the ship at this position
        target = simulation.getShipAt(targetx, targety);

        // Move to the next direction for the next turn
        nextTarget();

    } while (!simulation.inBounds(targetx, targety) || (target != nullptr && target->getTeam() == team));
    // Keeps retrying if out of bounds or a teammate is at the target

    // Perform shooting action
    if (target != nullptr)
    {
        std::cout << name << " shot at (" << targetx << "," << targety << ") and destroyed " << target->getName() << "!\n";
        target->die();
        recordKill();
    }
    else
    {
        std::cout << name << " shot at (" << targetx << "," << targety << ") and missed!\n";
    }
}

int Frigate::getTargetX() const { return targetx; }
int Frigate::getTargetY() const { return targety; }
void Frigate::nextTarget()
{
    directionIndex = (directionIndex + 1) % 8;
}
// ------------- Corvette -------------

void Corvette::action()
{
    std::cout << "-------------------- CORVETTE -------------------- \n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    simulation.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

// Shoots a random one of the 8 neighboring cells each turn
void Corvette::shoot()
{
    int targetx, targety;
    Ship *target = nullptr;

    do
    {
        int shotDirection = (rand() % 4) + 1; // Picks a random number between 1 and 4

        switch (shotDirection)
        {
        case 1: // Shoot UP
            targetx = cell.x;
            targety = cell.y - 1;
            break;
        case 2: // Shoot DOWN
            targetx = cell.x;
            targety = cell.y + 1;
            break;
        case 3: // Shoot LEFT
            targetx = cell.x - 1;
            targety = cell.y;
            break;
        case 4: // Shoot RIGHT
            targetx = cell.x + 1;
            targety = cell.y;
            break;
        }

        target = simulation.getShipAt(targetx, targety);
        if (target != nullptr)
        {
            std::cout << name << " shot at (" << targetx << "," << targety << ") and destroyed " << target->getName() << "!\n";
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " shot at (" << targetx << "," << targety << ") and missed!\n";
        }

    } while (!simulation.inBounds(targetx, targety) || (target != nullptr && target->getTeam() == team));
    // Keeps retrying if out of bounds or a teammate is at the target

    // Perform shooting action
    if (target != nullptr)
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
        std::cout << name << " shot at (" << targetx << "," << targety
                  << ") and destroyed " << target->getName() << "!\n";
        target->die();
        recordKill();
    }
    else
    {
        std::cout << name << " shot at (" << targetx << "," << targety << ") and missed!\n";
    }
}
// ------------- Amphibious -------------
void Amphibious::action()
{
    std::cout << "-------------------- AMPHIBIOOUS -------------------- \n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    move();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    simulation.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void Amphibious::look()
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

            index++;
        }
    }
    std::cout << std::endl;
}

void Amphibious::move()
{
    char islandorsea = simulation.getCell(cell.x, cell.y); // to restore old position after moving
    int freeIndices[8];
    int freeCount = 0;

    // find valid moving square (0 or 1)
    for (int i = 0; i < 8; i++)
    {
        if (neighbouring[i].symbol == '0' || neighbouring[i].symbol == '1')
        {
            freeIndices[freeCount] = i;
            freeCount++;
        }
    }

    if (freeCount == 0) // No free spaces, avoid infinite loop
    {
        std::cout << name << " cannot move.\n";
        return;
    }

    int index = freeIndices[rand() % freeCount];

    std::cout << name << " is moving to (" << neighbouring[index].x << ", " << neighbouring[index].y << ").\n";
    simulation.setCell(cell.x, cell.y, islandorsea);
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
}

void Amphibious::shoot()
{
    int dx, dy, target_x, target_y;
    Ship *target = nullptr;

    do
    {
        dx = (rand() % 11) - 5; // -5 to 5
        dy = (rand() % 11) - 5;
    } while (abs(dx) + abs(dy) > 5 && (dx == 0 && dy == 0));
    // Prevent shooting at (0,0)

    target_x = cell.x + dx;
    target_y = cell.y + dy;

    target = simulation.getShipAt(target_x, target_y);

    // Prevent friendly fire and keeps shooting in bounds
    if (!simulation.inBounds(target_x, target_y) || (target != nullptr && target->getTeam() == team))
    {
        // shooting action
        if (target != nullptr)
        {
            std::cout << name << " shot at (" << target_x << "," << target_y
                      << ") and destroyed " << target->getName() << "!\n";
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " shot at (" << target_x << "," << target_y
                      << ") and missed!\n";
        }
    }
}
// ------------- Destroyer -------------

void Destroyer::action()
{
    std::cout << "-------------------- DESTROYER -------------------- \n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    moveorram();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    simulation.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void Destroyer::look()
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
            index++;
        }
    }
    std::cout << std::endl;
}

void Destroyer::move()
{
    int freeIndices[8];
    int freeCount = 0;

    for (int i = 0; i < 8; i++)
    {
        if (neighbouring[i].symbol == '0')
        {
            freeIndices[freeCount] = i;
            freeCount++;
        }
    }

    if (freeCount == 0) // No free spaces, avoid infinite loop
    {
        std::cout << name << " cannot move.\n";
        return;
    }

    int index = freeIndices[rand() % freeCount];

    std::cout << name << " is moving to (" << neighbouring[index].x << ", " << neighbouring[index].y << ").\n";
    simulation.setCell(cell.x, cell.y, '0');
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
}

void Destroyer::ram()
{
    int index = 0;
    Ship *target = nullptr;

    while (index < 8)
    {
        target = simulation.getShipAt(neighbouring[index].x, neighbouring[index].y);

        if (target == nullptr || target->getTeam() == team)
        {
            index++;
            continue;
        }

        std::cout << name << " RAMMED and DESTROYED " << target->getName()
                  << " at (" << neighbouring[index].x << ", " << neighbouring[index].y << ")!\n";

        target->die();
        recordKill();

        // Move into the ramming position
        simulation.setCell(cell.x, cell.y, '0'); // Clear old position
        setPosition(neighbouring[index].x, neighbouring[index].y);
        simulation.setCell(cell.x, cell.y, cell.symbol);

        return;
    }
}

void Destroyer::moveorram()
{
    for (int i = 0; i < 8; i++)
    {
        Ship *target = simulation.getShipAt(neighbouring[i].x, neighbouring[i].y);
        if (target != nullptr && target->getTeam() != team)
        {
            ram();
            return;
        }
    }
    move();
}

void Destroyer::shoot()
{
    int dx, dy, target_x, target_y;
    Ship *target = nullptr;

    do
    {
        dx = (rand() % 11) - 5; // -5 to 5
        dy = (rand() % 11) - 5;
    } while (abs(dx) + abs(dy) > 5 && (dx == 0 && dy == 0));
    // Prevent shooting at (0,0)

    target_x = cell.x + dx;
    target_y = cell.y + dy;

    target = simulation.getShipAt(target_x, target_y);

    // Prevent friendly fire and keeps shooting in bounds
    if (!simulation.inBounds(target_x, target_y) || (target != nullptr && target->getTeam() == team))
    {
        // shooting action
        if (target != nullptr)
        {
            std::cout << name << " shot at (" << target_x << "," << target_y
                      << ") and destroyed " << target->getName() << "!\n";
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " shot at (" << target_x << "," << target_y
                      << ") and missed!\n";
        }
    }
}
// ------------- SuperShip -------------
void SuperShip::action()
{
    std::cout << "-------------------- SUPERSHIP -------------------- \n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    moveorram();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    simulation.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void SuperShip::look()
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
                index++;
        }
    }
    std::cout << std::endl;
}

void SuperShip::move()
{
    int freeIndices[8];
    int freeCount = 0;

    for (int i = 0; i < 8; i++)
    {
        if (neighbouring[i].symbol == '0')
        {
            freeIndices[freeCount] = i;
            freeCount++;
        }
    }

    if (freeCount == 0) // No free spaces, avoid infinite loop
    {
        std::cout << name << " cannot move.\n";
        return;
    }

    int index = freeIndices[rand() % freeCount];

    std::cout << name << " is moving to (" << neighbouring[index].x << ", " << neighbouring[index].y << ").\n";
    simulation.setCell(cell.x, cell.y, '0');
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
}

void SuperShip::ram()
{
    int index = 0;
    Ship *target = nullptr;

    while (index < 8)
    {
        target = simulation.getShipAt(neighbouring[index].x, neighbouring[index].y);

        if (target == nullptr || target->getTeam() == team)
        {
            index++;
            continue;
        }

        std::cout << name << " RAMMED and DESTROYED " << target->getName()
                  << " at (" << neighbouring[index].x << ", " << neighbouring[index].y << ")!\n";

        target->die();
        recordKill();

        // Move into the ramming position
        simulation.setCell(cell.x, cell.y, '0'); // Clear old position
        setPosition(neighbouring[index].x, neighbouring[index].y);
        simulation.setCell(cell.x, cell.y, cell.symbol);

        return;
    }
}

void SuperShip::moveorram()
{
    for (int i = 0; i < 8; i++)
    {
        Ship *target = simulation.getShipAt(neighbouring[i].x, neighbouring[i].y);
        if (target != nullptr && target->getTeam() != team)
        {
            ram();
            return;
        }
    }
    move();
}

void SuperShip::shoot()
{
    int dx, dy, target_x, target_y;
    Ship *target = nullptr;

    do
    {
        dx = (rand() % 11) - 5; // -5 to 5
        dy = (rand() % 11) - 5;
    } while (abs(dx) + abs(dy) > 5 && (dx == 0 && dy == 0));
    // Prevent shooting at (0,0)

    target_x = cell.x + dx;
    target_y = cell.y + dy;

    target = simulation.getShipAt(target_x, target_y);

    // Prevent friendly fire and keeps shooting in bounds
    if (!simulation.inBounds(target_x, target_y) || (target != nullptr && target->getTeam() == team))
    {
        // shooting action
        if (target != nullptr)
        {
            std::cout << name << " shot at (" << target_x << "," << target_y
                      << ") and destroyed " << target->getName() << "!\n";
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " shot at (" << target_x << "," << target_y
                      << ") and missed!\n";
        }
    }
}

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
