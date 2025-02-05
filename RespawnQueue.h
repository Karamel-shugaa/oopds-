#ifndef RESPAWNQUEUE_H
#define RESPAWNQUEUE_H

class Ship; // forward declare

class RespawnQueue
{
private:
    struct Node
    {
        Ship *ship;
        Node *next;
        Node(Ship *s) : ship(s), next(nullptr) {}
    };
    Node *front;
    Node *rear;

public:
    RespawnQueue() : front(nullptr), rear(nullptr) {}
    void enqueue(Ship *ship);
    Ship *dequeue();
    bool isEmpty();
};

#endif // RESPAWNQUEUE_H
