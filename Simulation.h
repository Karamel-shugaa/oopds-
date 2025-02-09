#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <memory>
#include <fstream>
#include "Battlefield.h"
#include "ShipList.h"
#include "RespawnQueue.h"

class Ship; // Forward declare

class Simulation : public Battlefield
{
private:
    int shipCount;
    ShipList activeShips;
    RespawnQueue respawnQueue;
    int iterations = 0; // from file
    bool gameOver = false;
    std::unique_ptr<Ship> createShip(const std::string &);

public:
    std::ofstream loggingFile;
    Simulation(std::string);
    ~Simulation();

    void read_iterations(std::ifstream &);
    void read_teams_details(std::ifstream &);
    void addShip(std::unique_ptr<Ship>);
    void addToQueue(Ship *);
    void load_ship(std::string &, char &, char &, int &);
    void run();
    void displayLog();
    void respawnShips();
    void checkVictoryCondition(int cturn);
    void battleshipToDestroyer(Ship *ship);
    void cruiserToDestroyer(Ship *ship);
    void destroyerToSupership(Ship *ship);
    void frigateToCorvette(Ship *ship);
    void amphibiousToSupership(Ship *ship);
    RespawnQueue &getRespawnQueue()
    {
        return respawnQueue;
    } // New to include a getter for respawnqueue
    void removeShip(Ship *);
    Ship *getShipAt(int x, int y);
};

#endif // SIMULATION_H
