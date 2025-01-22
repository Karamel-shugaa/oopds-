#ifndef SIMULATION_H
#define SIMULATION_H

#include "Battlefield.h"
#include <string>

class Simulation {
    Battlefield battlefield;
    Ship **ships;
    movingship **moveship;
    seeingship **seeship;
    shootingship **shootship;
    ramingship **ramship;
    miningship **mineship;
    int shipCount;

public:
    Simulation(int width, int height, int maxShips);
    ~Simulation();

    void addShip(Ship *ship);
    void loadFromFile(const char *filename);
    void run(int iterations);
};

#endif // SIMULATION_H
