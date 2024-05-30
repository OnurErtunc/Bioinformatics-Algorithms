Author: Onur Ertunç
Bilkent ID: 21802760
CS 481 - Spring 2024 - HW1
Date: 01/03/2024

Instructons:
1- Extract the zip file
2- Open the terminal and navigate to the directory of the extracted files. Named hw1.
2a- Put the input files in the directory. The input files should be named "text.fasta" and "patterns.fasta".
3- To compile the program, type "make" in the terminal and press enter.
4a- To run the program with rabin-karp algorithm, type "make run_rk" and press enter.
4b- To run the program with brute-force algorithm, type "make run_bf" and press enter.
5- To clean the directory, type "make clean" and press enter. This will remove the object files and the executable file.

Program Explanation:
The program takes 3 inputs from the user. The first input is the text file, the second input is the patterns file,
and the third input is the algorithm to be used. The program reads the text and the patterns from the files and then 
applies the selected algorithm to find the patterns in the text. The program then prints the results to the terminal.

@Input params:
-i: The input text file
-p: The input patterns file
-m: The algorithm to be used. It can be either RK or BF

Important note: this process is shortened by using the makefile. The user does not need to type the input parameters manually.
Only the text.txt and patterns.txt files are used as input files. The user only needs to type "make run_rk" or "make run_bf" to run the program.
The user only needs to put the input files in the directory and type the command to run the program.

@Output:
The program prints the results to the terminal. The results include the number of patterns found in the text, the time it took to find the patterns, and the indices of the patterns in the text.

A sample Output:
match[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18]
match[22]
match[21]
time: 3.5e-05s
