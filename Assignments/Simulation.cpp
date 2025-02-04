#include "Simulation.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <sstream>

Simulation::Simulation(std::string filename) : battlefield(filename), shipCount(0)
{
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
    for (int i = 0; i < shipCount; i++)
    {
        delete ships[i];
    }
    delete[] ships;
}

void Simulation::read_iterations(std::ifstream &file)
{
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
void Simulation::addShip(Ship *ship)
{
    if (shipCount < 100)
    {
        ships[shipCount++] = ship;
        battlefield.placeShip(ship);
    }
}

// polymorphism used here
Ship *Simulation::createShip(const std::string &ship_name)
{
    if (ship_name == "Battleship")
        return new Battleship();
    else if (ship_name == "Cruiser")
        return new Cruiser();
    else
    {
        std::cerr << "Error: Invalid ship type " << ship_name << "\n";
        return nullptr;
    }
    // else if (ship_name == "Destroyer")
    //     return new Destroyer();
    // else if (ship_name == "Frigate")
    //     return new Frigate();
    // else if (ship_name == "Corvette")
    //     return new Corvette();
    // else if (ship_name == "Amphibious")
    //     return new Amphibious();
    // else if (ship_name == "SuperShip")
    //     return new SuperShip()
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
        ship->setName(shipName);
        ship->setSymbol(symbol);
        ship->setTeam(team);
        std::cout << "Ship " << shipName << " created.\n";
        // ship->displayDetails();
    }
}

// void Simulation::run(int iterations) {
//     std::ofstream logFile("simulation_log.txt");
//     if (!logFile.is_open()) {
//         std::cerr << "Error: Could not open simulation_log.txt for writing.\n";
//         return;
//     }

//     for (int i = 0; i < iterations; i++) {
//         logFile << "\nTurn " << i + 1 << ":\n";
//         std::cout << "\nTurn " << i + 1 << ":\n";

//         battlefield.display();

//         for (int j = 0; j < shipCount; j++) {
//             if (ships[j]->getLives() > 0) {
//                 int oldX = ships[j]->getX();
//                 int oldY = ships[j]->getY();

//                 ships[j]->look();
//                 std::this_thread::sleep_for(std::chrono::milliseconds(500));
//                 ships[j]->move();
//                 battlefield.moveShip(ships[j], oldX, oldY);
//                 std::this_thread::sleep_for(std::chrono::milliseconds(500));
//                 ships[j]->shoot();
//                 std::this_thread::sleep_for(std::chrono::milliseconds(500));

//                 logFile << ships[j]->getTeam() << " " << ships[j]->getSymbol()
//                         << " moved to (" << ships[j]->getX() << ", " << ships[j]->getY() << ")\n";
//             }
//         }

//         std::this_thread::sleep_for(std::chrono::seconds(1));

//         int teamACount = 0, teamBCount = 0;
//         for (int j = 0; j < shipCount; j++) {
//             if (ships[j]->getLives() > 0) {
//                 if (ships[j]->getTeam() == "A") {
//                     teamACount++;
//                 } else if (ships[j]->getTeam() == "B") {
//                     teamBCount++;
//                 }
//             }
//         }

//         if (teamACount == 0 || teamBCount == 0) {
//             logFile << "\nGame Over! ";
//             std::cout << "\nGame Over! ";
//             if (teamACount == 0) {
//                 logFile << "Team B Wins!\n";
//                 std::cout << "Team B Wins!\n";
//             } else {
//                 logFile << "Team A Wins!\n";
//                 std::cout << "Team A Wins!\n";
//             }
//             break;
//         }
//     }

//     logFile.close();
// }

int main()
{
    Simulation sim("game.txt");
    return 0;
}
