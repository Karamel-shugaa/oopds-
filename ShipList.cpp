#include "ShipList.h"
#include "Ship.h"

ShipList::ShipList() : head(nullptr) {}

ShipList::~ShipList()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }
}

void ShipList::append(Ship *ship)
{
    Node *newNode = new Node(ship);
    if (head == nullptr)
    {
        head = newNode;
        return;
    }
    Node *current = head;
    while (current->next != nullptr)
    {
        current = current->next;
    }
    current->next = newNode;
}

void ShipList::remove(Ship *ship)
{
    if (head == nullptr)
        return;

    Node *current = head;
    Node *prev = nullptr;

    // If head node itself is to be deleted
    if (head->ship == ship)
    {
        head = head->next;
        delete current;
        return;
    }

    // Traverse the list to find the node to delete
    while (current && current->ship != ship)
    {
        prev = current;
        current = current->next;
    }

    // If ship is not found
    if (current == nullptr)
        return;

    // Unlink the node from the linked list
    prev->next = current->next;
    delete current;
}

void ShipList::display()
{
    Node *current = head;
    while (current)
    {
        current->ship->displayDetails();
        current = current->next;
    }
}