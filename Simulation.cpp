#include "Simulation.h"
#include "Ship.h"
#include <fstream>
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>

Simulation::Simulation(std::string filename) : Battlefield(filename), shipCount(0)
{
    loggingFile.open("log.txt");
    if (!loggingFile.is_open())
    {
        std::cerr << "Error: Could not open log file.\n";
        exit(1);
    }
    std::ifstream file(filename);
    if (!file.is_open())
    {
        if (!file.good())
            std::cerr << "Error: Could not open file " << filename << "\n";
        std::cerr << "Error: Could not open file " << filename << "\n";
        exit(1);
    }
    read_iterations(file);
    // return to the start of the file
    file.seekg(0, std::ios::beg);
    read_teams_details(file);
    file.close();
}

Simulation::~Simulation()
{
}
void Simulation::read_iterations(std::ifstream &file)
{
    std::string itemName;
    int itemNumber;
    std::string line;
    std::getline(file, line);
    std::istringstream string_stream(line);
    string_stream >> itemName >> iterations; // store the number of game turns from game.txt file
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
        // iterate until first team is found (line starting with "Team")
        if (line.find("Team") != std::string::npos)
        {
            // itemName to skip "Team" word
            string_stream >> itemName >> teamSymbol >> value;
            break;
        }
    }
    // store the first team's details
    std::cout << "-----------------First Team -------------------\n";
    {
        char shipSymbol;
        int num_of_lines = value;
        for (int i = 0; i < num_of_lines; i++)
        {
            string_stream.clear();
            std::getline(file, line);
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
            string_stream.str(line);
            string_stream >> itemName >> shipSymbol >> value;
            load_ship(itemName, shipSymbol, teamSymbol, value);
        }
    }
}

// polymorphism used here
std::unique_ptr<Ship> Simulation::createShip(const std::string &ship_name)
{
    if (ship_name == "Battleship")
        return std::make_unique<BattleShip>(*this);
    else if (ship_name == "Cruiser")
        return std::make_unique<Cruiser>(*this);
    else if (ship_name == "Destroyer")
        return std::make_unique<Destroyer>(*this);
    else if (ship_name == "Frigate")
        return std::make_unique<Frigate>(*this);
    else if (ship_name == "Corvette")
        return std::make_unique<Corvette>(*this);
    else if (ship_name == "Amphibious")
        return std::make_unique<Amphibious>(*this);
    else if (ship_name == "Supership")
        return std::make_unique<SuperShip>(*this);

    std::cerr << "Error: Invalid ship type '" << ship_name << "'.\n";
    return nullptr;
}

void Simulation::load_ship(std::string &shipName, char &symbol, char &team, int &count)
{
    shipCount += count;
    if (shipCount > getSeaCount())
    {
        std::cerr << "Error: Too many ships for the battlefield.\n";
        exit(1);
    }
    for (int i = 0; i < count; i++)
    {
        std::unique_ptr<Ship> ship = createShip(shipName);
        if (ship == nullptr)
        {
            std::cerr << "Error: Could not create ship " << shipName << "\n";
            return;
        }
        // give it a random position on map
        int x = rand() % getWidth();
        int y = rand() % getHeight();
        ship->setPosition(x, y);
        // set details
        ship->setSymbol(symbol);
        ship->setTeam(team);
        ship->setName(shipName);
        addShip(std::move(ship));
    }
}

void Simulation::addShip(std::unique_ptr<Ship> ship)
{
    if (placeShip(ship.get()))
        activeShips.append(std::move(ship));
}

void Simulation::addToQueue(Ship *ship)
{
    std::unique_ptr<Ship> removedShip = activeShips.remove(ship);
    if (removedShip != nullptr)
    {
        respawnQueue.enqueue(std::move(removedShip));
        std::cout << ship->getName() << ' ' << ship->getTeam() << ship->getSymbol() << " added to RespawnQueue.\n";
    }
    else
    {
        std::cerr << "addToQueue(): Ship not found in activeShips.\n";
    }
}

// added by the hard carry yousef
void Simulation::respawnShips()
{
    for (int i = 0; i < 2 && !respawnQueue.isEmpty(); i++)
    {
        std::unique_ptr<Ship> ship = respawnQueue.dequeue();
        if (!ship) // no ships in queue
            break;

        // Ensure it hasn't exceeded respawns
        if (ship->getLives() <= 0)
        {
            std::cout << ship->getName() << " has no respawns left.\n";
            continue;
        }
        addShip(std::move(ship));
    }
}

void Simulation::checkVictoryCondition(int currentTurn)
{
    bool teamA_alive = false;
    bool teamB_alive = false;

    // Check if any ships from Team A or B are still alive
    ShipList::Iterator it = activeShips.begin();
    while (it != activeShips.end())
    {
        Ship *ship = *it;
        ++it;

        if (ship->getTeam() == 'A')
            teamA_alive = true;
        else if (ship->getTeam() == 'B')
            teamB_alive = true;

        if (teamA_alive && teamB_alive)
            return; // Both teams still have ships, continue the game
    }

    if (!teamA_alive && teamB_alive)
    {
        std::cout << "Team B wins! All Team A ships have been destroyed.\n";
    }
    else if (!teamB_alive && teamA_alive)
    {
        std::cout << "Team A wins! All Team B ships have been destroyed.\n";
    }
    gameOver = true;
}
void Simulation::run()
{
    for (int i = 0; i < iterations; i++)
    {
        if (gameOver)
        {
            "\n------------------------GAME OVER--------------------------\n\n Final Battlefield: \n\n";
            display();
            return;
        }
        std::cout << "\n-------------------- TURN " << i + 1 << " -------------------- \n\n";
        respawnQueue.display();
        display();
        loggingFile << "\n-------------------- TURN " << i + 1 << " -------------------- \n\n";
        displayLog();
        ShipList::Iterator it = activeShips.begin();
        while (it != activeShips.end())
        {
            Ship *s = *it;
            ++it;
            if (!s)
                continue;
            s->action();
            // std::cin.get();
            loggingFile << std::endl;
            std::cout << std::endl;
        }
        respawnShips();
        checkVictoryCondition(i); // Check if a team has won
    }
    std::cout << "\n\n------------------------GAME FINISHED WITH NO WINNING TEAM------------------------\n\n-------------------------------------GAME OVER------------------------------------ \n\nFinal Battlefield: \n\n";
    display();
}

Ship *Simulation::getShipAt(int x, int y)
{
    // iterate the activeShips list to see if any ship is at (x,y)
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

void Simulation::removeShip(Ship *s)
{
    if (s == nullptr)
    {
        std::cerr << "removeShip(): Ship is null.\n";
        return;
    }
    activeShips.remove(s);
}

void Simulation::battleshipToDestroyer(Ship *ship)
{
    BattleShip *battleship = dynamic_cast<BattleShip *>(ship);
    if (!battleship)
    {
        std::cerr << "Error: Ship is not a battleship, cannot upgrade to Destroyer!\n";
        return;
    }

    std::cout << "Upgrading " << battleship->getName() << ' ' << battleship->getTeam() << battleship->getSymbol() <<
    " at (" << battleship->getX() << ", " << battleship->getY() << ") to Destroyer...\n";

    for (auto it = activeShips.begin(); it != activeShips.end(); ++it)
    {
        if (*it == ship)
        { 

            // move ownership out of the list temporarily (using reference to unique_ptr because can't create another unique-ptr to same obj)
            std::unique_ptr<Ship> &shipPtr = activeShips.getUniquePtr(it);
            std::unique_ptr<Ship> oldShip = std::move(shipPtr);

            // create a new Destroyer using the old battleship data
            std::unique_ptr<Destroyer> newShip = std::make_unique<Destroyer>(*battleship);

            // replace old ship in the same position (preserves order)
            shipPtr = std::move(newShip);

            std::cout << "Upgrade complete: battleship is now a Destroyer!\n";
            return;
        }
    }

    std::cerr << "Error: battleship not found in activeShips!\n";
}
void Simulation::cruiserToDestroyer(Ship *ship)
{
    Cruiser *cruiser = dynamic_cast<Cruiser *>(ship);
    if (!cruiser)
    {
        std::cerr << "Error: Ship is not a cruiser, cannot upgrade to Destroyer!\n";
        return;
    }

    std::cout << "Upgrading " << cruiser->getName() << ' ' << cruiser->getTeam() << cruiser->getSymbol() << " at (" << cruiser->getX() << ", " << cruiser->getY() << ") to Destroyer...\n";

    for (auto it = activeShips.begin(); it != activeShips.end(); ++it)
    {
        if (*it == ship)
        {

            // move ownership out of the list temporarily (using reference to unique_ptr because can't create another unique-ptr to same obj)
            std::unique_ptr<Ship> &shipPtr = activeShips.getUniquePtr(it);
            std::unique_ptr<Ship> oldShip = std::move(shipPtr);

            // create a new Destroyer using the old cruiser data
            std::unique_ptr<Destroyer> newShip = std::make_unique<Destroyer>(*cruiser);

            // replace old ship in the same position (preserves order)
            shipPtr = std::move(newShip);

            std::cout << "Upgrade complete: cruiser is now a Destroyer!\n";
            return;
        }
    }

    std::cerr << "Error: cruiser not found in activeShips!\n";
}
void Simulation::destroyerToSupership(Ship *ship)
{
    Destroyer *destroyer = dynamic_cast<Destroyer *>(ship);
    if (!destroyer)
    {
        std::cerr << "Error: Ship is not a destroyer, cannot upgrade to Supership!\n";
        return;
    }

    std::cout << "Upgrading " << destroyer->getName() << ' ' << destroyer->getTeam() << destroyer->getSymbol() << " at (" << destroyer->getX() << ", " << destroyer->getY() << ") to Supership...\n";

    for (auto it = activeShips.begin(); it != activeShips.end(); ++it)
    {
        if (*it == ship)
        {

            // move ownership out of the list temporarily (using reference to unique_ptr because can't create another unique-ptr to same obj)
            std::unique_ptr<Ship> &shipPtr = activeShips.getUniquePtr(it);
            std::unique_ptr<Ship> oldShip = std::move(shipPtr);

            // create a new SuperShip using the old destroyer data
            std::unique_ptr<SuperShip> newShip = std::make_unique<SuperShip>(*destroyer);

            // replace old ship in the same position (preserves order)
            shipPtr = std::move(newShip);

            std::cout << "Upgrade complete: destroyer is now a Supership!\n";
            return;
        }
    }

    std::cerr << "Error: destroyer not found in activeShips!\n";
}
void Simulation::frigateToCorvette(Ship *ship)
{
    Frigate *frigate = dynamic_cast<Frigate *>(ship);
    if (!frigate)
    {
        std::cerr << "Error: Ship is not a frigate, cannot upgrade to Supership!\n";
        return;
    }

    std::cout << "Upgrading " << frigate->getName() << ' ' << frigate->getTeam() << frigate->getSymbol() << " at (" << frigate->getX() << ", " << frigate->getY() << ") to Corvette...\n";

    for (auto it = activeShips.begin(); it != activeShips.end(); ++it)
    {
        if (*it == ship)
        {

            // move ownership out of the list temporarily (using reference to unique_ptr because can't create another unique-ptr to same obj)
            std::unique_ptr<Ship> &shipPtr = activeShips.getUniquePtr(it);
            std::unique_ptr<Ship> oldShip = std::move(shipPtr);

            // create a new Corvette using the old frigate data
            std::unique_ptr<Corvette> newShip = std::make_unique<Corvette>(*frigate);

            // replace old ship in the same position (preserves order)
            shipPtr = std::move(newShip);

            std::cout << "Upgrade complete: frigate is now a Corvette!\n";
            return;
        }
    }

    std::cerr << "Error: frigate not found in activeShips!\n";
}

void Simulation::amphibiousToSupership(Ship *ship)
{
    Amphibious *amphibious = dynamic_cast<Amphibious *>(ship);
    if (!amphibious)
    {
        std::cerr << "Error: Ship is not a amphibious, cannot upgrade to Supership!\n";
        return;
    }

    std::cout << "Upgrading " << amphibious->getName() << ' ' << amphibious->getTeam() << amphibious->getSymbol() << " at (" << amphibious->getX() << ", " << amphibious->getY() << ") to Supership...\n";

    for (auto it = activeShips.begin(); it != activeShips.end(); ++it)
    {
        if (*it == ship)
        {

            // move ownership out of the list temporarily (using reference to unique_ptr because can't create another unique-ptr to same obj)
            std::unique_ptr<Ship> &shipPtr = activeShips.getUniquePtr(it);
            std::unique_ptr<Ship> oldShip = std::move(shipPtr);

            // create a new SuperShip using the old amphibious data
            std::unique_ptr<SuperShip> newShip = std::make_unique<SuperShip>(*amphibious);

            // replace old ship in the same position (preserves order)
            shipPtr = std::move(newShip);

            std::cout << "Upgrade complete: amphibious is now a Supership!\n";
            return;
        }
    }

    std::cerr << "Error: amphibious not found in activeShips!\n";
}

void Simulation::displayLog()
{
    loggingFile << "  ";
    for (int i = 0; i < width; i++)
    {
        loggingFile << " " << i;
    }
    loggingFile << std::endl;
    for (int i = 0; i < height; i++)
    {
        loggingFile << i << ": ";
        for (int j = 0; j < width; j++)
        {
            loggingFile << grid[i][j] << ' ';
        }
        loggingFile << std::endl;
    }
}