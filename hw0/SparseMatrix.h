#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include "Node.h"

class SparseMatrix
{
private:
    Node *head;
    int rows;
    int cols;

public:
    SparseMatrix(int rows, int cols);
    ~SparseMatrix();

    // Constructor
    SparseMatrix(const SparseMatrix &other);

    // Copy assignment operator
    SparseMatrix &operator=(const SparseMatrix &other);

    void insert(int value, int rowIndex, int colIndex);
    SparseMatrix add(const SparseMatrix &other);
    void scalarMultiply(int scalar);
    SparseMatrix transpose();
    void display();
};

#endif // SPARSEMATRIX_H
