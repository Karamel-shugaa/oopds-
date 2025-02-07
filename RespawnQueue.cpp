#include "RespawnQueue.h"
#include <iostream>
#include "Ship.h"

RespawnQueue::~RespawnQueue() {
    while (!isEmpty()) {
        dequeue();
    }
}

void RespawnQueue::enqueue(Ship *ship) {
    Node *newNode = new Node(ship);
    if (!rear) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    std::cout << "Ship " << ship->getName() << " added to RespawnQueue.\n";  // Debugging
}

Ship *RespawnQueue::dequeue() {
    if (!front) {
        std::cout << "RespawnQueue is empty.\n";
        return nullptr;
    }

    Node *temp = front;
    Ship *ship = front->ship;
    front = front->next;
    if (!front) rear = nullptr;

    delete temp;
    std::cout << "Ship " << ship->getName() << " removed from RespawnQueue for respawn.\n"; // Debugging
    return ship;
}

bool RespawnQueue::isEmpty() {
    return front == nullptr;
}
