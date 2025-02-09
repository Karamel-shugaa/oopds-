#ifndef SHIPLIST_H
#define SHIPLIST_H

#include <memory>

class Ship;

class ShipList
{
private:
    struct Node
    {
        std::unique_ptr<Ship> ship;
        std::unique_ptr<Node> next;
        // move to transfer ownership
        Node(std::unique_ptr<Ship> s) : ship(std::move(s)), next(nullptr) {}
    };
    std::unique_ptr<Node> head;

public:
    ShipList();
    void append(std::unique_ptr<Ship>);
    // use raw pointer to remove from list (not delete yet)
    std::unique_ptr<Ship> remove(Ship *);
    void display();
    
    class Iterator
    {
        Node *current;
        
        public:
        Iterator(Node *node) : current(node) {}
        bool operator!=(const Iterator &other) { return current != other.current; }
        void operator++() { current = current->next.get(); }
        Ship *operator*() { return current ? current->ship.get() : nullptr; }
        Node *getNode() { return current; }
    };
    
    Iterator begin() { return Iterator(head.get()); }
    Iterator end() { return Iterator(nullptr); }
    std::unique_ptr<Ship> &getUniquePtr(Iterator it)
    {
        return it.getNode()->ship;
    }
};

#endif // SHIPLIST_H
