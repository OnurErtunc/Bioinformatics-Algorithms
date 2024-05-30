// Node.cpp
#include "Node.h"
#include <cstddef> // for NULL

Node::Node(int val, int rIndex, int cIndex)
    : value(val), rowIndex(rIndex), colIndex(cIndex), prev(NULL), next(NULL)
{
    // Constructor body, if necessary
}