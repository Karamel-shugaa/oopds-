#include "RespawnQueue.h"
#include "Ship.h"

void RespawnQueue::enqueue(Ship *ship)
{
    Node *newNode = new Node(ship);
    if (!rear)
        front = rear = newNode;
    else
    {
        rear->next = newNode;
        rear = newNode;
    }
}

Ship *RespawnQueue::dequeue()
{
    if (!front)
        return nullptr;
    Node *temp = front;
    Ship *ship = front->ship;
    front = front->next;
    if (!front)
        rear = nullptr;
    delete temp;
    return ship;
}

bool RespawnQueue::isEmpty()
{
    return front == nullptr;
}
