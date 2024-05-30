#include "SparseMatrix.h"
#include <iostream>

// Constructor
SparseMatrix::SparseMatrix(int r, int c) : rows(r), cols(c), head(nullptr) {}

// Destructor
SparseMatrix::~SparseMatrix()
{
    Node *current = head;
    if (current)
    {
        Node *next;
        do
        {
            next = current->next;
            delete current;
            current = next;
        } while (current != head);
    }
}

// Copy Constructor
SparseMatrix::SparseMatrix(const SparseMatrix &other) : rows(other.rows), cols(other.cols), head(NULL)
{
    Node *currentOther = other.head;
    if (currentOther)
    {
        do
        {
            this->insert(currentOther->value, currentOther->rowIndex, currentOther->colIndex);
            currentOther = currentOther->next;
        } while (currentOther != other.head);
    }
}

// Copy Assignment Operator
SparseMatrix &SparseMatrix::operator=(const SparseMatrix &other)
{
    if (this != &other)
    {
        this->~SparseMatrix(); // Clean up current resources

        this->rows = other.rows;
        this->cols = other.cols;
        Node *currentOther = other.head;
        if (currentOther)
        {
            do
            {
                this->insert(currentOther->value, currentOther->rowIndex, currentOther->colIndex);
                currentOther = currentOther->next;
            } while (currentOther != other.head);
        }
    }
    return *this;
}

// Insert method
void SparseMatrix::insert(int value, int rowIndex, int colIndex)
{
    // Check if the value is zero, we do not store zero values in sparse matrix
    if (value == 0)
    {
        return;
    }

    Node *newNode = new Node(value, rowIndex, colIndex);

    // If the list is empty, create a new node and point it to itself twice (prev and next)
    if (!head)
    {
        head = newNode;
        newNode->next = newNode->prev = newNode;
        return;
    }

    Node *current = head;

    // If the new node goes before the head, find the last node and adjust pointers
    if ((rowIndex < head->rowIndex) || (rowIndex == head->rowIndex && colIndex < head->colIndex))
    {
        // Find the node which is before the head
        while (current->next != head)
        {
            current = current->next;
        }
        // Insert new node before head
        newNode->next = head;
        newNode->prev = current;
        head->prev = newNode;
        current->next = newNode;
        head = newNode;
        return;
    }

    // Otherwise, find the correct position to insert the new node
    while (current->next != head &&
           (current->next->rowIndex < rowIndex ||
            (current->next->rowIndex == rowIndex && current->next->colIndex < colIndex)))
    {
        current = current->next;
    }

    // Insert new node after the current node
    newNode->next = current->next;
    newNode->prev = current;
    current->next->prev = newNode;
    current->next = newNode;
}
/*
// Addition of two sparse matrices
SparseMatrix SparseMatrix::add(const SparseMatrix &other)
{
    // Implementation of addition logic
    // Make sure to handle the addition of elements with the same row and column index,
    // and the case where one matrix has an element where the other does not.
}
*/
// Scalar multiplication
void SparseMatrix::scalarMultiply(int scalar)
{
    if (head == NULL || scalar == 1)
    {
        // If the list is empty or the scalar is 1, there's nothing to do.
        return;
    }

    Node *temp = head;
    do
    {
        temp->value *= scalar; // Multiply the value of the node by the scalar
        temp = temp->next;     // Move to the next node
    } while (temp != head);    // Since it's a circular list, we end when we reach the head again

    // If the scalar is zero, we need to clear the list as all values would be zero.
    if (scalar == 0)
    {
        Node *current = head;
        Node *next;
        while (current->next != head)
        { // Delete all nodes except the head
            next = current->next;
            delete current;
            current = next;
        }
        delete head; // Now delete the head
        head = NULL; // Set head to NULL as the list is now empty
    }
}

// Transposition
SparseMatrix SparseMatrix::transpose()
{
    // Create a new SparseMatrix with swapped dimensions
    SparseMatrix transposedMatrix(cols, rows);

    // Only transpose if there is something in the matrix
    if (head != NULL)
    {
        Node *current = head;
        // Since it's a circular list, we end when we've gone full circle
        do
        {
            // Insert the transposed node into the new matrix
            transposedMatrix.insert(current->value, current->colIndex, current->rowIndex);
            current = current->next;
        } while (current != head);
    }

    return transposedMatrix;
}

// Display method
void SparseMatrix::display()
{
    if (head == NULL)
    {
        // If the list is empty, print a matrix of all zeros
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                std::cout << "0 ";
            }
            std::cout << std::endl;
        }
    }
    else
    {
        Node *temp = head;
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                // If the current position matches the node's position, print the value
                if (temp->rowIndex == i && temp->colIndex == j)
                {
                    std::cout << temp->value << " ";
                    // Move to the next node if we're not at the end of the list
                    if (temp->next != head)
                    {
                        temp = temp->next;
                    }
                }
                else
                {
                    // If there is no node for this position, print 0
                    std::cout << "0 ";
                }
            }
            std::cout << std::endl;
        }
    }
}

SparseMatrix SparseMatrix::add(const SparseMatrix &other)
{
    if (rows != other.rows || cols != other.cols)
    {
        std::cerr << "Error: Matrices dimensions must match for addition!" << std::endl;
        // Handle error: dimensions do not match
        // For simplicity, we return an empty matrix here
        return SparseMatrix(0, 0);
    }

    SparseMatrix result(rows, cols);

    // Add non-zero elements from the first matrix
    Node *current = this->head;
    if (current)
    {
        do
        {
            result.insert(current->value, current->rowIndex, current->colIndex);
            current = current->next;
        } while (current != this->head);
    }

    // Add non-zero elements from the second matrix
    current = other.head;
    if (current)
    {
        do
        {
            // Check if the element is already in the result, if so, add to it
            Node *resultCurrent = result.head;
            bool found = false;
            if (resultCurrent)
            {
                do
                {
                    if (resultCurrent->rowIndex == current->rowIndex && resultCurrent->colIndex == current->colIndex)
                    {
                        resultCurrent->value += current->value;
                        found = true;
                        break;
                    }
                    resultCurrent = resultCurrent->next;
                } while (resultCurrent != result.head);
            }
            // If the element is not found, insert it
            if (!found)
            {
                result.insert(current->value, current->rowIndex, current->colIndex);
            }
            current = current->next;
        } while (current != other.head);
    }

    return result;
}
