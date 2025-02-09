#ifndef RESPAWNQUEUE_H
#define RESPAWNQUEUE_H

#include "Ship.h"
#include <memory>

class RespawnQueue {
private:    
    struct Node {
        std::unique_ptr<Ship> ship;
        std::unique_ptr<Node> next;
        Node(std::unique_ptr<Ship> s) : ship(std::move(s)), next(nullptr) {}
    };
    // use unique_ptr here to auto delete nodes :) (DOESN'T HAVE TO DO WITH SHIP)
    std::unique_ptr<Node> head;
    Node *tail;

public:
    RespawnQueue() : head(nullptr), tail(nullptr) {}
    void enqueue(std::unique_ptr<Ship> ship);
    std::unique_ptr<Ship> dequeue();
    void display();
    bool isEmpty();
};

#endif 
// RESPAWNQUEUE_H
