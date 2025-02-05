#ifndef SHIPLIST_H
#define SHIPLIST_H

class Ship; // Forward declaration

class ShipList
{
private:
    struct Node
    {
        Ship *ship;
        Node *next;
        Node(Ship *s) : ship(s), next(nullptr) {}
    };
    Node *head;

public:
    ShipList();
    ~ShipList();
    void append(Ship *ship);
    void remove(Ship *ship);
    void display();

    class Iterator
    {
        Node *current;

    public:
        Iterator(Node *node) : current(node) {}
        bool operator!=(const Iterator &other) { return current != other.current; }
        void operator++() { current = current->next; }
        Ship *operator*() { return current ? current->ship : nullptr; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

#endif // SHIPLIST_H
