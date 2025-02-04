#ifndef SIMULATION_H
#define SIMULATION_H

#include "Battlefield.h"
#include <string>
#include <sstream>

class Simulation
{
private:
    Battlefield battlefield;
    Ship **ships;
    int shipCount;
    int iterations;
    Ship *createShip(const std::string &ship_name);

public:
    Simulation(std::string);
    ~Simulation();

    void read_iterations(std::ifstream &);
    void read_teams_details(std::ifstream &);
    void addShip(Ship *ship);
    void load_ship(std::string &, char &, char &, int &);
    // void run(int iterations);
};

#endif // SIMULATION_H
