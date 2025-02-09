#include "tempList.h"
#include "Ship.h"
#include <iostream>


TempList::~TempList()
{
    clear();
}

void TempList::clear()
{
    if (empty())
        return;
    Node *current = head;
    while (current != nullptr)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }
    size = 0;
    head = nullptr;
}

void TempList::append(Ship *ship)
{
    size++;
    Node *newNode = new Node(ship);
    if (!head)
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

bool TempList::empty() const
{
    return head == nullptr;
}
Ship *TempList::getRandom()
{
    if (!head)
        return nullptr; 

    int randomIndex = rand() % size;

    // traverse to the random node
    Node* current = head;
    for (int i = 0; i < randomIndex; i++)
    {
        current = current->next;
    }
    return current->ship;
}