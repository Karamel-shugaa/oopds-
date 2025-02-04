#include "Battlefield.h"
#include <iostream>
#include <fstream>
#include <sstream>

Battlefield::Battlefield(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        exit(1);
    }
    read_grid(file);
    file.close();
    this->display();
}

Battlefield::~Battlefield()
{
    for (int i = 0; i < height; i++)
    {
        delete[] grid[i];
    }
    delete[] grid;
}

void Battlefield::read_grid(std::ifstream &file)
{
    std::string line;
    // read height and width from file
    {
        std::string itemName;
        int itemValue;
        std::getline(file, line); // read first line to skip iterations
        std::getline(file, line); // read second line to get width
        std::istringstream string_stream(line);
        string_stream >> itemName >> itemValue;
        width = itemValue;
        std::getline(file, line); // read third line to get height
        std::istringstream string_stream2(line);
        string_stream2 >> itemName >> itemValue;
        height = itemValue;
    }
    // array of pointers to char arrays (2D array), height is the number of rows
    grid = new char *[height];
    // current line indicator from the start of the grid
    int currentLine = 0;
    // skip all the lines before the grid
    while (std::getline(file, line))
    {
        // iterate until grid is found (line starting with 0 or 1)
        if (line[0] == '0' || line[0] == '1')
            break;
    }
    do
    {
        int length = line.length();
        currentLine++;
        // check if the number of rows in the file is greater than the height (exit case)
        if (currentLine > height)
        {
            std::cerr << "Error: Too many lines in file." << "\n";
            exit(1);
        }
        grid[currentLine - 1] = new char[width];
        // check if the width of the line is not equal to the absolute width of the grid (including spaces)
        if (length != 2 * width - 1)
        {
            std::cerr << "Error: Line " << currentLine + 10 << " in file does not have the correct width\n";
            exit(1);
        }
        // iterate through the line and assign the characters to the grid
        for (int i = 0; i < width; i++)
        {
            grid[currentLine - 1][i] = line[2 * i]; // 2 * i to skip the spaces
        }
    } while (std::getline(file, line));
    file.close();
}
void Battlefield::placeShip(Ship *ship)
{
    if (grid[ship->getY()][ship->getX()] == '0')
    {
        grid[ship->getY()][ship->getX()] = ship->getSymbol();
    }
    else
    {
        std::cout << "Cannot place ship at occupied position!\n";
    }
}

void Battlefield::moveShip(Ship *ship, int oldX, int oldY)
{
    grid[oldY][oldX] = '0';
    if (grid[ship->getY()][ship->getX()] == '0')
    {
        grid[ship->getY()][ship->getX()] = ship->getSymbol();
    }
    else
    {
        std::cout << "Cannot move ship to occupied position!\n";
    }
}

void Battlefield::display()
{
    std::cout << "Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << grid[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}