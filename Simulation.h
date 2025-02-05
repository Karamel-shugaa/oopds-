#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "Battlefield.h"
#include "ShipList.h"
#include "RespawnQueue.h"

class Ship; // Forward declare

class Simulation
{
private:
    Battlefield battlefield;
    int shipCount;
    ShipList activeShips;
    RespawnQueue respawnQueue;
    int iterations = 0; // from file
    bool gameOver = false;
    Ship *createShip(const std::string &ship_name);

public:
    Simulation(std::string);
    ~Simulation();

    void read_iterations(std::ifstream &);
    void read_teams_details(std::ifstream &);
    void addShip(Ship *);
    void load_ship(std::string &, char &, char &, int &);
    void getInstance(); // If you want it
    void run();
    void respawnShips();
    void display();

    ShipList &getShips() { return activeShips; }

    // Additional Helpers
    bool inBounds(int x, int y);
    Ship *getOccupantAt(int x, int y);
    char get_cell(int x, int y) const { return battlefield.getCell(x, y); };
    bool canMoveTo(int x, int y, bool canOccupyEnemy, bool amphibious = false);

    // Called after a ship moves, to update the battlefield grid
    void updateBattlefieldPosition(Ship *ship, int oldX, int oldY);

    // Upgrades oldShip => newShipType (string like "Destroyer", "Corvette", etc.)
    void upgradeShip(Ship *oldShip, const std::string &newType);

    // For limiting or checking re-spawns
    bool canRespawn(Ship *s);

    void checkVictoryCondition();
};

#endif // SIMULATION_H
