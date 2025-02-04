#include <iostream>
#include "Ship.cpp"

int main()
{

    Ship *baseship = new BattleShip("12345",'#',4,7,3,"team");
    // this is only an example of values of how it is supposed to run
    //user will input team comp, symbols that will be used for each ship and number of turns, we need a way 
    // to manipulate the game.txt file to follow the user input 
    baseship->turn();
    // battleShip action
}
