#include "Ship.h"
#include "ShipList.h" // Because we use ShipList::Iterator in shoot()
#include "Simulation.h"
#include <cstdlib>
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

void Ship::die()
{
    if (name == "Amphibious" || name == "SuperShip")
    {
        simulation.setCell(cell.x, cell.y, islandOrSea);
    }
    if (lives <= 0)
    {
        std::cout << "ship is already dead" << std::endl;
        return;
    }
    lives--;
    if (lives == 0)
    {
        std::cout << std::flush << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") has fallen!\n";
        simulation.loggingFile << std::flush << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") has fallen!\n";
        simulation.removeShip(this);
    }
    else
    {
        std::cout << std::flush << name << ' ' << team << cell.symbol << " died at (" << cell.x << ", " << cell.y << ") and has " << lives << " life/lives left.\n";
        simulation.loggingFile << std::flush << name << ' ' << team << cell.symbol << " died at (" << cell.x << ", " << cell.y << ") and has " << lives << " life/lives left.\n";
        simulation.addToQueue(this);
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
    std::cout << "-------------------- BATTLESHIP -------------------- \n\n";
    simulation.loggingFile << "-------------------- BATTLESHIP -------------------- \n\n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    move();
    shoot();
    shoot();
    simulation.display();
    if (kills >= 3)
    {
        simulation.setCell(cell.x, cell.y, 'D');
        simulation.battleshipToDestroyer(this);
    }
}

void BattleShip::look()
{
    // Implement a 3×3 scan around (x, y)
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
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
            }

            index++;
        }
    }
}

void BattleShip::move()
{
    int directionsIndex[] = {1, 3, 4, 6}; // up, left, right, down
    int index;
    bool free = false;
    for (int i : directionsIndex)
    {
        if (neighbouring[i].symbol == '0')
        {
            free = true;
            break;
        }
    }
    if (!free)
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        return;
    }
    // loop until valid move position found
    do
    {
        index = directionsIndex[rand() % 4];
    } while (neighbouring[index].symbol != '0');

    simulation.setCell(cell.x, cell.y, '0');
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
    std::cout << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
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
    Ship *target = simulation.getShipAt(target_x, target_y);
    if (target != nullptr)
    {
        if (target->getTeam() != team)
        {
            std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and hit " << target->getTeam() << ' ' << target->getSymbol() << "!\n";
            simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and hit " << target->getTeam() << ' ' << target->getSymbol() << "!\n";
            simulation.setCell(target_x, target_y, '0');
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
            simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        }
    }
    else
    {
        std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
    }
}

//-------------Cruiser------------------
void Cruiser::action()
{
    std::cout << "-------------------- CRUISER -------------------- \n\n";
    simulation.loggingFile << "-------------------- CRUISER -------------------- \n\n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    moveorram();
    simulation.display();
    if (kills >= 3)
    {
        simulation.setCell(cell.x, cell.y, 'D');
        simulation.cruiserToDestroyer(this);
    }
}
void Cruiser::look()
{
    enemyShips.clear();
    // Implement a 3×3 scan around (x, y)
    std::cout << name << "  " << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
    simulation.loggingFile << name << "  " << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
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
                if (index == 1 || index == 3 || index == 4 || index == 6)
                {
                    Ship *neighbouringShip = simulation.getShipAt(nx, ny);
                    if (neighbouringShip != nullptr)
                    {
                        if (neighbouringShip->getTeam() != team)
                        {
                            enemyShips.append(neighbouringShip);
                        }
                    }
                }
            }
            index++;
        }
    }
}

void Cruiser::move()
{
    int directionsIndex[] = {1, 3, 4, 6}; // up, left, right, down
    int index;
    bool free = false;
    for (int i : directionsIndex)
    {
        if (neighbouring[i].symbol == '0')
        {
            free = true;
            break;
        }
    }
    if (!free)
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        return;
    }
    // loop until valid move position found
    do
    {
        index = directionsIndex[rand() % 4];
    } while (neighbouring[index].symbol != '0');

    simulation.setCell(cell.x, cell.y, '0');
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
    std::cout << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
}

void Cruiser::ram()
{
    Ship *target = enemyShips.getRandom();
    if (target == nullptr)
    {
        std::cout << "Cruiser returned null target\n";
        return;
    }
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is ramming enemy at (" << target->getX() << ", " << target->getY() << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is ramming enemy at (" << target->getX() << ", " << target->getY() << ").\n";
    simulation.setCell(cell.x, cell.y, '0');
    setPosition(target->getX(), target->getY());
    simulation.setCell(cell.x, cell.y, cell.symbol);
    target->die();
    recordKill();
    std::cout << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
}
void Cruiser::moveorram()
{
    if (enemyShips.empty())
    {
        move();
        return;
    }
    ram();
}

// ------------- Destroyer -------------

void Destroyer::action()
{
    std::cout << "-------------------- DESTROYER -------------------- \n\n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    simulation.loggingFile << "-------------------- DESTROYER -------------------- \n\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    moveorram();
    shoot();
    shoot();
    simulation.display();
    if (kills >= 3)
    {
        simulation.setCell(cell.x, cell.y, 'S');
        simulation.destroyerToSupership(this);
    }
}

void Destroyer::shoot()
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
        simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and missed (out of bounds).\n";
        return;
    }
    Ship *target = simulation.getShipAt(target_x, target_y);
    if (target != nullptr)
    {
        if (target->getTeam() != team)
        {
            std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and hit " << target->getTeam() << ' ' << target->getSymbol() << "!\n";
            simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and hit " << target->getTeam() << ' ' << target->getSymbol() << "!\n";
            simulation.setCell(target_x, target_y, '0');
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
            simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        }
    }
    else
    {
        std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
    }
}

void Destroyer::look()
{
    enemyShips.clear();
    // Implement a 3×3 scan around (x, y)
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
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
                if (index == 1 || index == 3 || index == 4 || index == 6)
                {
                    Ship *neighbouringShip = simulation.getShipAt(nx, ny);
                    if (neighbouringShip != nullptr)
                    {
                        if (neighbouringShip->getTeam() != team)
                            enemyShips.append(neighbouringShip);
                    }
                }
            }
            index++;
        }
    }
}

void Destroyer::move()
{
    int directionsIndex[] = {1, 3, 4, 6}; // up, left, right, down
    int index;
    bool free = false;
    for (int i : directionsIndex)
    {
        if (neighbouring[i].symbol == '0')
        {
            free = true;
            break;
        }
    }
    if (!free)
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        return;
    }
    // loop until valid move position found
    do
    {
        index = directionsIndex[rand() % 4];
    } while (neighbouring[index].symbol != '0');

    simulation.setCell(cell.x, cell.y, '0');
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
    std::cout << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
}

void Destroyer::ram()
{
    Ship *target = enemyShips.getRandom();
    if (target == nullptr)
    {
        std::cout << "Destroyer returned null target\n";
        return;
    }
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is ramming enemy at (" << target->getX() << ", " << target->getY() << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is ramming enemy at (" << target->getX() << ", " << target->getY() << ").\n";
    simulation.setCell(cell.x, cell.y, '0');
    setPosition(target->getX(), target->getY());
    simulation.setCell(cell.x, cell.y, cell.symbol);
    target->die();
    recordKill();
    std::cout << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
}

void Destroyer::moveorram()
{
    if (enemyShips.empty())
    {
        move();
        return;
    }
    ram();
}

//-------------Frigate------------------

void Frigate::action()
{
    std::cout << "-------------------- FRIGATE -------------------- \n\n";
    simulation.loggingFile << "-------------------- FRIGATE -------------------- \n\n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    shoot();
    simulation.display();
    if (kills >= 3)
    {
        simulation.setCell(cell.x, cell.y, 'C');
        simulation.frigateToCorvette(this);
    }
}

void Frigate::shoot()
{
    static const int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    static const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

    Ship *target = nullptr;

    targetx = cell.x + dx[directionIndex];
    targety = cell.y + dy[directionIndex];

    // get the ship at the current position
    target = simulation.getShipAt(targetx, targety);

    // Move to the next direction for the next turn
    nextTarget();

    // Perform shooting action
    if (target != nullptr)
    {
        if (target->getTeam() != team)
        {
            std::cout << name << ' ' << team << cell.symbol << " shot at (" << targetx << ", " << targety << ") and hit " << target->getName() << ' ' << target->getTeam() << target->getSymbol() << "!\n";
            simulation.loggingFile << name << ' ' << team << cell.symbol << " shot at (" << targetx << ", " << targety << ") and hit " << target->getName() << ' ' << target->getTeam() << target->getSymbol() << "!\n";
            simulation.setCell(targetx, targety, '0');
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << targetx << ", " << targety << ") and misses.\n";
            simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << targetx << ", " << targety << ") and misses.\n";
        }
    }
    else
    {
        std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << targetx << ", " << targety << ") and misses.\n";
        simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << targetx << ", " << targety << ") and misses.\n";
    }
}

void Frigate::nextTarget()
{
    directionIndex = (directionIndex + 1) % 8;
}

// ------------- Corvette -------------

void Corvette::action()
{
    std::cout << "-------------------- CORVETTE -------------------- \n\n";
    simulation.loggingFile << "-------------------- CORVETTE -------------------- \n\n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    shoot();
    simulation.display();
}

void Corvette::look()
{
    // Implement a 3×3 scan around (x, y)
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
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
            }

            index++;
        }
    }
}

// shoots a random one of the 8 neighboring cells each turn
void Corvette::shoot()
{
    int neighbourIndex = rand() % 8;
    int target_x = neighbouring[neighbourIndex].x;
    int target_y = neighbouring[neighbourIndex].y;
    Ship *target = simulation.getShipAt(target_x, target_y);
    if (target != nullptr)
    {
        if (target->getTeam() != team)
        {
            std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and hit " << target->getTeam() << ' ' << target->getSymbol() << "!\n";
            simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and hit " << target->getTeam() << ' ' << target->getSymbol() << "!\n";
            simulation.setCell(target_x, target_y, '0');
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
            simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        }
    }
    else
    {
        std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
    }
}

// ------------- Amphibious -------------
void Amphibious::action()
{
    std::cout << "-------------------- AMPHIBIOOUS -------------------- \n\n";
    simulation.loggingFile << "-------------------- AMPHIBIOOUS -------------------- \n\n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    move();
    shoot();
    shoot();
    simulation.display();
    if (kills >= 4)
    {
        simulation.setCell(cell.x, cell.y, 'S');
        simulation.amphibiousToSupership(this);
    }
}

void Amphibious::look()
{
    // Implement a 3×3 scan around (x, y)
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
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
            }

            index++;
        }
    }
}

void Amphibious::move()
{
    int directionsIndex[] = {1, 3, 4, 6}; // up, left, right, down
    int index;
    bool free = false;
    for (int i : directionsIndex)
    {
        if (neighbouring[i].symbol == '0' || neighbouring[i].symbol == '1')
        {
            free = true;
            break;
        }
    }
    if (!free)
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        return;
    }
    // loop until valid move position found
    do
    {
        index = directionsIndex[rand() % 4];
    } while (neighbouring[index].symbol != '0' && neighbouring[index].symbol != '1');
    simulation.setCell(cell.x, cell.y, islandOrSea);
    islandOrSea = neighbouring[index].symbol;
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
    std::cout << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
}

void Amphibious::shoot()
{
    int dx, dy;
    do
    {
        dx = (rand() % 11) - 5; // num between -5 and 5
        dy = (rand() % 11) - 5;
    } while (abs(dx) + abs(dy) > 5 || (dx == 0 && dy == 0)); // avoid self targeting and limit to city block distance of 5
    int target_x = cell.x + dx;
    int target_y = cell.y + dy;
    Ship *target = simulation.getShipAt(target_x, target_y);
    if (target != nullptr)
    {
        if (target->getTeam() != team)
        {   
            std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and hit " << target->getTeam() << ' ' << target->getSymbol() << "!\n";
            simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") shot at (" << target_x << ", " << target_y << ") and hit " << target->getTeam() << ' ' << target->getSymbol() << "!\n";
            simulation.setCell(target_x, target_y, '0');
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
            simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        }
    }
    else
    {
        std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
    }
}
// ------------- SuperShip -------------
void SuperShip::action()
{
    std::cout << "-------------------- SUPERSHIP -------------------- \n\n";
    simulation.loggingFile << "-------------------- SUPERSHIP -------------------- \n\n";
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") started its turn.\n";
    look();
    moveorram();
    shoot();
    shoot();
    shoot();
    simulation.display();
}

void SuperShip::look()
{
    // Implement a 3×3 scan around (x, y)
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is scanning its surroundings.\n";
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
            }
            index++;
        }
    }
    std::cout << std::endl;
}

void SuperShip::move()
{
    int directionsIndex[] = {1, 3, 4, 6}; // up, left, right, down
    int index;
    bool free = false;
    for (int i : directionsIndex)
    {
        if (neighbouring[i].symbol == '0')
        {
            free = true;
            break;
        }
    }
    if (!free)
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot move.\n";
        return;
    }
    // loop until valid move position found
    do
    {
        index = directionsIndex[rand() % 4];
    } while (neighbouring[index].symbol != '0');
    // handle moving from island to sea (when upgraded from amphibious)
    simulation.setCell(cell.x, cell.y, islandOrSea);
    islandOrSea = neighbouring[index].symbol;
    setPosition(neighbouring[index].x, neighbouring[index].y);
    simulation.setCell(cell.x, cell.y, cell.symbol);
    std::cout << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
}

void SuperShip::ram()
{
    Ship *target = enemyShips.getRandom();
    if (target == nullptr)
    {
        std::cout << "SuperShip returned null target\n";
        return;
    }
    std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is ramming enemy at (" << target->getX() << ", " << target->getY() << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") is ramming enemy at (" << target->getX() << ", " << target->getY() << ").\n";
    simulation.setCell(cell.x, cell.y, '0');
    setPosition(target->getX(), target->getY());
    simulation.setCell(cell.x, cell.y, cell.symbol);
    if (target)
    {
        target->die();
        recordKill();
    }
        
    std::cout << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
    simulation.loggingFile << name << ' ' << team << cell.symbol << " moved to (" << cell.x << ", " << cell.y << ").\n";
}

void SuperShip::moveorram()
{
    if (enemyShips.empty())
    {
        std::cout << name << ' ' << team << cell.symbol << " at (" << cell.x << ", " << cell.y << ") cannot ram.\n";
        move();
        return;
    }
    ram();
}

void SuperShip::shoot()
{  
    int target_x, target_y;
    do
    {
        target_x = rand() % simulation.getWidth();
        target_y = rand() % simulation.getHeight(); 
    } while (target_x == cell.x && target_y == cell.y);
    Ship *target = nullptr;
    target = simulation.getShipAt(target_x, target_y);

    if (target != nullptr)
    {
        if (target->getTeam() != team)
        {
            std::cout << name << " shot at (" << target_x << "," << target_y
                      << ") and destroyed " << target->getName() << "!\n";
            simulation.loggingFile << name << " shot at (" << target_x << "," << target_y
                      << ") and destroyed " << target->getName() << "!\n";
            simulation.setCell(target_x, target_y, '0');
            target->die();
            recordKill();
        }
        else
        {
            std::cout << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
            simulation.loggingFile << name << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        }
    }
    else
    {
        std::cout << name << ' ' << team << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
        simulation.loggingFile << name << ' ' << team << " at (" << cell.x << ", " << cell.y << ") fires at (" << target_x << ", " << target_y << ") and misses.\n";
    }
}