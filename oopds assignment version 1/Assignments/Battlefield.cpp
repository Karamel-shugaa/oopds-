#include "Battlefield.h"
#include <iostream>

Battlefield::Battlefield(int width, int height) : width(width), height(height) {
    grid = new char *[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new char[width];
        for (int j = 0; j < width; j++) {
            grid[i][j] = '0';
        }
    }
}

Battlefield::~Battlefield() {
    for (int i = 0; i < height; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

void Battlefield::placeShip(Ship *ship) {
    if (grid[ship->getY()][ship->getX()] == '0') {
        grid[ship->getY()][ship->getX()] = ship->getSymbol();
    } else {
        std::cout << "Cannot place ship at occupied position!\n";
    }
}

void Battlefield::moveShip(Ship *ship, int oldX, int oldY) {
    grid[oldY][oldX] = '0';
    if (grid[ship->getY()][ship->getX()] == '0') {
        grid[ship->getY()][ship->getX()] = ship->getSymbol();
    } else {
        std::cout << "Cannot move ship to occupied position!\n";
    }
}

void Battlefield::display() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << grid[i][j] << ' ';
        }
        std::cout << '\n';
    }
}
