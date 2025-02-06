#include "ShipList.h"
#include "Ship.h"

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

void ShipList::remove(Ship *ship)
{
    if (head == nullptr)
        return;

    // If head node itself is to be deleted
    if (head->ship.get() == ship)
    {
        head = std::move(head->next);
        return;
    }
    Node *current = head.get();
    Node *prev = nullptr;
    // Traverse the list to find the node to delete
    while (current != nullptr && current->ship.get() != ship)
    {
        prev = current;
        current = current->next.get();
    }
    if (current == nullptr)
        return;
    if (prev != nullptr) // ship found
    {
        prev->next = std::move(current->next);
    }
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