#include "RespawnQueue.h"
#include <iostream>
#include "Ship.h"

void RespawnQueue::enqueue(std::unique_ptr<Ship> ship)
{
    auto newNode = std::make_unique<Node>(std::move(ship));

    if (head == nullptr) 
    {
        head = std::move(newNode);
        tail = head.get();
    }
    else
    {
        tail->next = std::move(newNode);
        tail = tail->next.get();
    }
}

std::unique_ptr<Ship> RespawnQueue::dequeue()
{
    if (head == nullptr)
    {
        std::cout << "RespawnQueue is empty.\n";
        return nullptr;
    }

    std::unique_ptr<Ship> removedShip = std::move(head->ship); // transfer ownership
    head = std::move(head->next); // next node becomes head

    if (head == nullptr)
        tail = nullptr;
    return removedShip;
}

bool RespawnQueue::isEmpty() {
    return head == nullptr;
}

void RespawnQueue::display()
{
    Node *current = head.get();
    std::cout << "RespawnQueue: ";
    while (current)
    {
        std::cout << current->ship->getName() << ' ' << current->ship->getTeam() << current->ship->getSymbol() << " -> ";
        current = current->next.get();
    }
    std::cout << std::endl;
}
