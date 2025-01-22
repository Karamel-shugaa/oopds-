#include "Simulation.h"
#include "Ship.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <chrono>

Simulation::Simulation(int width, int height, int maxShips) : battlefield(width, height)
{
    shipCount = 0;
    ships = new Ship *[maxShips];
}

Simulation::~Simulation()
{
    for (int i = 0; i < shipCount; i++)
    {
        delete ships[i];
    }
    delete[] ships;
}

void Simulation::addShip(Ship *ship)
{
    if (shipCount < 100)
    {
        ships[shipCount++] = ship;
        battlefield.placeShip(ship);
    }
}

void Simulation::loadFromFile(const char *filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.find("Team") != std::string::npos)
        {
            char teamName[10];
            int numTypes;
            sscanf(line.c_str(), "Team %s %d", teamName, &numTypes);

            for (int i = 0; i < numTypes; ++i)
            {
                std::getline(file, line);
                char shipType[20], symbol;
                int count;
                sscanf(line.c_str(), "%s %c %d", shipType, &symbol, &count);

                for (int j = 0; j < count; ++j)
                {
                    int x = rand() % battlefield.getWidth();
                    int y = rand() % battlefield.getHeight();
                    if (strcmp(shipType, "Battleship") == 0)
                    {
                        addShip(new BattleShip(shipType, symbol, x, y, 3, teamName));
                    }
                    else if (strcmp(shipType, "Cruiser") == 0)
                    {
                        addShip(new Cruiser(shipType, symbol, x, y, 3, teamName));
                    }
                }
            }
        }
    }
    file.close();
}

void Simulation::run(int iterations)
{
    std::ofstream logFile("simulation_log.txt");
    if (!logFile.is_open())
    {
        std::cerr << "Error: Could not open simulation_log.txt for writing.\n";
        return;
    }

    for (int i = 0; i < iterations; i++)
    {
        logFile << "\nTurn " << i + 1 << ":\n";
        std::cout << "\nTurn " << i + 1 << ":\n";

        battlefield.display();

        for (int j = 0; j < shipCount; j++)
        {
            if (ships[j]->getLives() > 0)
            {
                int oldX = ships[j]->getX();
                int oldY = ships[j]->getY();

                seeship[j]->look();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                moveship[j]->move();
                battlefield.moveShip(ships[j], oldX, oldY);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                shootship[j]->shoot();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                ramship[j]->ram();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                mineship[j]->mine();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                logFile << ships[j]->getTeam() << " " << ships[j]->getSymbol()
                        << " moved to (" << ships[j]->getX() << ", " << ships[j]->getY() << ")\n";
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        int teamACount = 0, teamBCount = 0;
        for (int j = 0; j < shipCount; j++)
        {
            if (ships[j]->getLives() > 0)
            {
                if (ships[j]->getTeam() == "A")
                {
                    teamACount++;
                }
                else if (ships[j]->getTeam() == "B")
                {
                    teamBCount++;
                }
            }
        }

        if (teamACount == 0 || teamBCount == 0)
        {
            logFile << "\nGame Over! ";
            std::cout << "\nGame Over! ";
            if (teamACount == 0)
            {
                logFile << "Team B Wins!\n";
                std::cout << "Team B Wins!\n";
            }
            else
            {
                logFile << "Team A Wins!\n";
                std::cout << "Team A Wins!\n";
            }
            break;
        }
    }

    logFile.close();
}
