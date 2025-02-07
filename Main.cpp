#include <iostream>
#include "Simulation.h"

int main()
{
    srand(time(0));
    Simulation sim("game.txt");
    sim.run();
    return 0;
}
