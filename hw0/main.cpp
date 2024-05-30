#include <iostream>
#include <cstdlib> // for std::atoi and std::rand
#include <ctime>   // for std::time
#include "SparseMatrix.h"

// Function to populate a sparse matrix with random numbers
void populateMatrix(SparseMatrix &matrix, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int value = std::rand() % 100; // Random number between 0 and 99
            if (value != 0)
            { // Insert only non-zero values into the sparse matrix
                matrix.insert(value, i, j);
            }
        }
    }
}

int main(int argc, char **argv)
{
    int size = 0;
    int scalar = 0;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-n" && i + 1 < argc)
        {
            size = std::atoi(argv[++i]);
        }
        else if (arg == "-s" && i + 1 < argc)
        {
            scalar = std::atoi(argv[++i]);
        }
        else
        {
            std::cerr << "Usage: " << argv[0] << " -n <size> -s <scalar>" << std::endl;
            return 1;
        }
    }

    if (size <= 0)
    {
        std::cerr << "Matrix size must be a positive integer." << std::endl;
        return 1;
    }

    // Initialize random number generator
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Create and populate the first matrix with random numbers
    SparseMatrix matrix1(size, size);
    populateMatrix(matrix1, size);
    std::cout << "Matrix 1:" << std::endl;
    matrix1.display();

    // Create and populate the second matrix with random numbers
    SparseMatrix matrix2(size, size);
    populateMatrix(matrix2, size);
    std::cout << "\nMatrix 2:" << std::endl;
    matrix2.display();

    // Add the two matrices
    SparseMatrix sumMatrix = matrix1.add(matrix2);
    std::cout << "\nSum of Matrix 1 and Matrix 2:" << std::endl;
    sumMatrix.display();

    // Perform scalar multiplication on the sum
    sumMatrix.scalarMultiply(scalar);
    std::cout << "\nSum Matrix after scalar multiplication by " << scalar << ":" << std::endl;
    sumMatrix.display();

    // Transpose matrix1
    SparseMatrix transposedMatrix1 = matrix1.transpose();
    std::cout << "\nTransposed Matrix 1:" << std::endl;
    transposedMatrix1.display();

    return 0;
}

/*
int main()
{
    // Addition of two 3x3 matrices
    SparseMatrix matrix1(3, 3);
    matrix1.insert(1, 0, 0);
    matrix1.insert(2, 0, 1);
    matrix1.insert(3, 0, 2);
    matrix1.insert(4, 1, 0);
    matrix1.insert(5, 1, 1);
    matrix1.insert(6, 1, 2);
    matrix1.insert(7, 2, 0);
    matrix1.insert(8, 2, 1);
    matrix1.insert(9, 2, 2);

    SparseMatrix matrix2(3, 3);
    matrix2.insert(11, 0, 0);
    matrix2.insert(12, 0, 1);
    matrix2.insert(13, 0, 2);
    matrix2.insert(14, 1, 0);
    matrix2.insert(15, 1, 1);
    matrix2.insert(16, 1, 2);
    matrix2.insert(17, 2, 0);
    matrix2.insert(18, 2, 1);
    matrix2.insert(19, 2, 2);

    SparseMatrix sumMatrix = matrix1.add(matrix2);
    std::cout << "Addition of two 3x3 matrices:" << std::endl;
    sumMatrix.display();

    // Scalar multiplication of a 3x3 matrix by 10
    std::cout << "\nScalar multiplication of a 3x3 matrix by 10:" << std::endl;
    matrix1.scalarMultiply(10);
    matrix1.display();

    // Transposition of a 3x3 matrix
    std::cout << "\nTransposition of a 3x3 matrix:" << std::endl;
    SparseMatrix transposeMatrix = matrix1.transpose();
    transposeMatrix.display();

    // Addition of two 2x2 matrices
    SparseMatrix smallMatrix1(2, 2);
    smallMatrix1.insert(10, 0, 0);
    smallMatrix1.insert(20, 0, 1);
    smallMatrix1.insert(3, 1, 0);
    smallMatrix1.insert(7, 1, 1);

    SparseMatrix smallMatrix2(2, 2);
    smallMatrix2.insert(1, 0, 0);
    smallMatrix2.insert(4, 0, 1);
    smallMatrix2.insert(14, 1, 0);
    smallMatrix2.insert(15, 1, 1);

    SparseMatrix smallSumMatrix = smallMatrix1.add(smallMatrix2);
    std::cout << "\nAddition of two 2x2 matrices:" << std::endl;
    smallSumMatrix.display();

    // Scalar multiplication of a 2x2 matrix by 2
    std::cout << "\nScalar multiplication of a 2x2 matrix by 2:" << std::endl;
    smallMatrix1.scalarMultiply(2);
    smallMatrix1.display();

    // Transposition of a 2x2 matrix
    std::cout << "\nTransposition of a 2x2 matrix:" << std::endl;
    SparseMatrix smallTransposeMatrix = smallMatrix1.transpose();
    smallTransposeMatrix.display();

    return 0;
}
*/