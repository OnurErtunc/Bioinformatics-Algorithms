#ifndef NODE_H
#define NODE_H
// Node.h
class Node
{
public:
    int value;
    int rowIndex;
    int colIndex;
    Node *prev;
    Node *next;

    Node(int value, int rowIndex, int colIndex);
};

#endif // NODE_H