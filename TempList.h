#ifndef TEMPList_H
#define TEMPList_H

class Ship;

class TempList
{
private:
    int size;
    struct Node
    {
        Ship *ship; 
        Node *next;

        Node(Ship *s) : ship(s), next(nullptr) {}
    };
    Node *head;

public:
    TempList() : head(nullptr), size(0) {};
    ~TempList();
    void append(Ship *ship);
    void clear();
    Ship* getRandom();
    bool empty() const;

    class Iterator
    {
        Node *current;

    public:
        Iterator(Node *node) : current(node) {}
        bool operator!=(const Iterator &other) { return current != other.current; }
        void operator++() { current = current->next; }
        Ship *operator*() { return current ? current->ship : nullptr; }
    };

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};

#endif // tempList_H
