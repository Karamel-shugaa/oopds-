#include "Simulation.h"
#include "Ship.h"
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>

Simulation::Simulation(std::string filename) : battlefield(filename), shipCount(0)
{
    std::cout << "Simulation created.\n";
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        exit(1);
    }
    read_iterations(file);
    // return to the start of the file
    file.seekg(0, std::ios::beg);
    read_teams_details(file);
    std::cout << "\niterations: " << iterations << std::endl;
    file.close();
}

Simulation::~Simulation()
{
}
void Simulation::read_iterations(std::ifstream &file)
{
    std::cout << "Reading iterations\n";
    std::string itemName;
    int itemNumber;
    std::string line;
    std::getline(file, line);
    std::istringstream string_stream(line);
    string_stream >> itemName >> iterations; // store the number of game turns from game.txt file
    // ships = new Ship *[maxShips];
}
void Simulation::read_teams_details(std::ifstream &file)
{
    std::string line;
    std::istringstream string_stream;
    std::string itemName;
    char teamSymbol;
    int value; // how many lines for each team (equal to the num of ships)
    while (std::getline(file, line))
    {
        string_stream.clear();
        string_stream.str(line);
        std::cout << "Line: " << line << std::endl;
        // iterate until first team is found (line starting with "Team")
        if (line.find("Team") != std::string::npos)
        {
            // itemName to skip "Team" word
            string_stream >> itemName >> teamSymbol >> value;
            std::cout << "Value: " << value << std::endl;
            break;
        }
    }
    // store the first team's details
    {
        char shipSymbol;
        int num_of_lines = value;
        for (int i = 0; i < num_of_lines; i++)
        {
            string_stream.clear();
            std::getline(file, line);
            std::cout << "Line: " << line << std::endl;
            string_stream.str(line);
            string_stream >> itemName >> shipSymbol >> value;
            load_ship(itemName, shipSymbol, teamSymbol, value);
        }
    }
    std::cout << "-----------------Second Team -------------------\n";
    // read seconds team's details
    {
        char shipSymbol;
        int num_of_lines = value;
        string_stream.clear();
        std::getline(file, line);
        std::cout << "Line: " << line << std::endl;
        string_stream.str(line);
        if (line.find("Team") != std::string::npos)
        {
            // itemName to skip "Team" word
            string_stream >> itemName >> teamSymbol >> value;
        }
        num_of_lines = value;
        for (int i = 0; i < num_of_lines; i++)
        {
            string_stream.clear();
            std::getline(file, line);
            std::cout << "Line: " << line << std::endl;
            string_stream.str(line);
            string_stream >> itemName >> shipSymbol >> value;
            load_ship(itemName, shipSymbol, teamSymbol, value);
        }
    }
}

// polymorphism used here
Ship *Simulation::createShip(const std::string &ship_name)
{
    if (ship_name == "Battleship")
        return new BattleShip(*this);
    // else if (ship_name == "Cruiser")
    //     return new Cruiser(*this);
    // else if (ship_name == "Destroyer")
    //     return new Destroyer(*this);
    // else if (ship_name == "Frigate")
    //     return new Frigate(*this);
    // else if (ship_name == "Corvette")
    //     return new Corvette(*this);
    // else if (ship_name == "Amphibious")
    //     return new Amphibious(*this);
    // else if (ship_name == "Supership")
    //     return new SuperShip(*this);
    else
    {
        std::cerr << "Error: Invalid ship type " << ship_name << "\n";
        return nullptr;
    }
}
void Simulation::load_ship(std::string &shipName, char &symbol, char &team, int &count)
{
    // From game.txt: ShipName Symbol    N
    //                setname  setsymbol for N ships for team "team"
    for (int i = 0; i < count; i++)
    {
        Ship *ship = createShip(shipName);
        if (ship == nullptr)
        {
            std::cerr << "Error: Could not create ship " << shipName << "\n";
            return;
        }
        // give it a random position on map
        int x = rand() % battlefield.getWidth();
        int y = rand() % battlefield.getHeight();
        ship->setPosition(x, y);
        // set details
        ship->setSymbol(symbol);
        ship->setTeam(team);
        std::cout << "Ship " << shipName << " created.\n";
        addShip(ship);
    }
}

void Simulation::addShip(Ship *ship)
{
    battlefield.placeShip(ship);
    activeShips.append(ship);
}

void Simulation::display()
{
    battlefield.display();
}
// void Simulation::respawnShips()
// {
//     // Each turn, up to 2 ships re-enter if they haven't used up 3 spawns
//     for (int i = 0; i < 2 && !respawnQueue.isEmpty(); i++)
//     {
//         Ship *ship = respawnQueue.dequeue();
//         if (!ship)
//             break;
//         // Check if can still respawn
//         if (ship->getRespawnCount() < 3)
//         {
//             ship->incrementRespawnCount();
//             ship->setLives(3);
//             // random place
//             int tries = 0;
//             while (tries < 100)
//             {
//                 int nx = rand() % battlefield.getWidth();
//                 int ny = rand() % battlefield.getHeight();
//                 // normal canMoveTo for occupant = false
//                 if (canMoveTo(nx, ny, false))
//                 {
//                     ship->setPosition(nx, ny);
//                     battlefield.placeShip(ship);
//                     activeShips.addShip(ship);
//                     std::cout << "Ship " << ship->getTeam() << " " << ship->getSymbol()
//                               << " respawned at (" << nx << ", " << ny << "), used "
//                               << ship->getRespawnCount() << " out of 3.\n";
//                     break;
//                 }
//                 tries++;
//             }
//         }
//         else
//         {
//             std::cout << "Ship " << ship->getTeam() << " " << ship->getSymbol()
//                       << " has no respawns left.\n";
//         }
//     }
// }

void Simulation::run()
{
    for (int i = 0; i < iterations; i++)
    {
        std::cout << "\nTurn " << i + 1 << ":\n";
        battlefield.display();
        // Let each ship take a turn
        ShipList::Iterator it = activeShips.begin();
        while (it != activeShips.end())
        {
            Ship *s = *it;
            ++it;
            s->action();
        }
    }
    display();
}
// void Simulation::run(int iters)
// {
//     if (iters > 0)
//         iterations = iters;

//     for (int i = 0; i < iterations; i++)
//     {
//         if (gameOver)
//             break;
//         std::cout << "\nTurn " << i + 1 << ":\n";
//         battlefield.display();

//         // Let each ship take a turn
//         ShipList::Iterator it = activeShips.begin();
//         while (it != activeShips.end())
//         {
//             Ship *s = *it;
//             ++it;
//             if (!s->getDeathFlag() && s->getLives() > 0)
//             {
//                 s->turn();
//             }
//             // If destroyed this turn
//             if (s->getDeathFlag())
//             {
//                 respawnQueue.enqueue(s);
//                 activeShips.removeShip(s);
//             }
//         }

//         respawnShips();
//         checkVictoryCondition();
//         if (gameOver)
//             break;

//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     }
// }

bool Simulation::inBounds(int x, int y)
{
    if (x < 0 || x >= battlefield.getWidth())
        return false;
    if (y < 0 || y >= battlefield.getHeight())
        return false;
    return true;
}

Ship *Simulation::getOccupantAt(int x, int y)
{
    // We'll iterate the activeShips list to see if any ship is at (x,y)
    ShipList::Iterator it = activeShips.begin();
    while (it != activeShips.end())
    {
        Ship *s = *it;
        ++it;
        if (s->getX() == x && s->getY() == y)
        {
            return s;
        }
    }
    return nullptr;
}

// bool Simulation::canMoveTo(int x, int y, bool canOccupyEnemy, bool amphibious)
// {
//     if (!inBounds(x, y))
//         return false;
//     // If amphibious => use isCellValidForAmphibious, else normal
//     if (amphibious)
//     {
//         if (!battlefield.isCellValidForAmphibious(x, y))
//             return false;
//     }
//     else
//     {
//         if (!battlefield.isCellValid(x, y))
//         {
//             // If it’s not valid due to occupant or island
//             // we only allow occupant if canOccupyEnemy==true
//             if (!inBounds(x, y))
//                 return false; // not needed, but safe
//             if (getOccupantAt(x, y) && canOccupyEnemy)
//                 return true;
//             return false;
//         }
//     }
//     return true;
// }

// void Simulation::updateBattlefieldPosition(Ship *ship, int oldX, int oldY)
// {
//     battlefield.moveShip(ship, oldX, oldY);
// }

// void Simulation::upgradeShip(Ship *oldShip, const std::string &newType)
// {
//     // remove old from active list (but do not delete yet)
//     activeShips.removeShip(oldShip);

//     // Create new object of newType at same coords, same kills
//     int oldX = oldShip->getX();
//     int oldY = oldShip->getY();
//     int oldLives = oldShip->getLives();
//     int oldKills = oldShip->getKills();
//     int oldRespawns = oldShip->getRespawnCount();
//     std::string oldName = oldShip->getTeam() + "Upgraded"; // or keep oldShip->name
//     char oldSym = oldShip->getSymbol();                    // might change
//     std::string oldTeam = oldShip->getTeam();

//     // We'll delete old ship after we create the new one
//     // Because we must keep the simulation reference
//     Ship *newShip = nullptr;

//     if (newType == "Destroyer")
//     {
//         newShip = new Destroyer(*this, oldName, oldSym, oldX, oldY, oldLives, oldTeam);
//     }
//     else if (newType == "SuperShip")
//     {
//         newShip = new SuperShip(*this, oldName, oldSym, oldX, oldY, oldLives, oldTeam);
//     }
//     else if (newType == "Corvette")
//     {
//         newShip = new Corvette(*this, oldName, oldSym, oldX, oldY, oldLives, oldTeam);
//     }
//     else
//     {
//         // fallback: new Ship is same type
//         newShip = oldShip;
//         std::cout << "No recognized upgrade type. Aborting upgrade.\n";
//         return;
//     }

//     // copy kills & respawnCount
//     for (int i = 0; i < oldKills; i++)
//         newShip->recordKills();
//     for (int i = 0; i < oldRespawns; i++)
//         newShip->incrementRespawnCount();

//     // place newShip in the battlefield
//     battlefield.placeShip(newShip);
//     activeShips.addShip(newShip);

//     // now safe to "destroy" oldShip
//     delete oldShip;
// }

// bool Simulation::canRespawn(Ship *s)
// {
//     // if s->getRespawnCount() < 3, we can
//     return (s->getRespawnCount() < 3);
// }

// void Simulation::checkVictoryCondition()
// {
//     // Count living ships per team
//     int teamACount = 0;
//     int teamBCount = 0;
//     ShipList::Iterator it = activeShips.begin();
//     while (it != activeShips.end())
//     {
//         Ship *s = *it;
//         ++it;
//         if (s->getLives() > 0)
//         {
//             if (s->getTeam() == "A")
//                 teamACount++;
//             else if (s->getTeam() == "B")
//                 teamBCount++;
//         }
//     }
//     if (teamACount == 0 && teamBCount == 0)
//     {
//         std::cout << "All ships destroyed! It's a tie?!\n";
//         gameOver = true;
//     }
//     else if (teamACount == 0)
//     {
//         std::cout << "Team B wins!\n";
//         gameOver = true;
//     }
//     else if (teamBCount == 0)
//     {
//         std::cout << "Team A wins!\n";
//         gameOver = true;
//     }
// }
