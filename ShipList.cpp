#include "ShipList.h"
#include "Ship.h"
#include <iostream>

ShipList::ShipList() : head(nullptr) {}

// Must use move semantics to transfer ownership of ship
void ShipList::append(std::unique_ptr<Ship> ship)
{
    // make_unique same as new
    auto newNode = std::make_unique<Node>(std::move(ship));
    if (head == nullptr)
    {
        head = std::move(newNode);
        return;
    }
    Node *current = head.get();
    while (current->next != nullptr)
    {
        current = current->next.get();
    }
    // move ownership
    current->next = std::move(newNode);
}

std::unique_ptr<Ship> ShipList::remove(Ship *ship)
{
    if (head == nullptr)
        return nullptr;
    // If head node itself is to be deleted
    if (head->ship.get() == ship)
    {
        std::unique_ptr<Ship> removedShip = std::move(head->ship); 
        head = std::move(head->next);                              
        return removedShip;                                        
    }
    Node *current = head.get();
    Node *prev = nullptr;
    // traverse the list to find the node to delete
    while (current != nullptr && current->ship.get() != ship)
    {
        prev = current;
        current = current->next.get();
    }
    if (current == nullptr)
    {
        std::cout << "Ship not found in list.\n";
        return nullptr;
    }

    std::unique_ptr<Ship> removedShip = std::move(current->ship);
    if (prev != nullptr) // ship found
    {
        prev->next = std::move(current->next);
    }
    return removedShip;
}


void ShipList::display()
{
    Node *current = head.get();
    while (current)
    {
        current->ship->displayDetails();
        current = current->next.get();
    }
}