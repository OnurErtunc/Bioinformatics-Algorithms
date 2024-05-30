Author: Onur Ertunç
Bilkent ID: 21802760
CS 481 - Spring 2024 - HW2
Date: 24/03/2024

Instructons:
1- Extract the zip file
2- Open the terminal and navigate to the directory of the extracted files. Named ertunc-onur-hw2.
2a- Put the input files in the directory. The input files should be named "text.fasta" and "patterns.fasta". These files are not provided in the zip file.
3- To compile the program, type "make" in the terminal and press enter.
4a- To run the program "make run" and press enter. This will run the code with the properly working part, which is Suffix Array implementation.
4b- To run the program with Suffix Tree implementation, type "make run_suffixtree" and press enter.
Note: Suffix Tree implementation is not working properly. It is implemented but not working as expected. The program will not find the patterns in the text.
As it gives segmentation fault error. Due to this reason, it is not listed in the hw2 file and not included in the hw2.out.
5- To clean the directory, type "make clean" and press enter. This will remove the object files and the executable files.

Program Explanation:
The program reads the input files, text.fasta and patterns.fasta files. The text.fasta file contains the text and the patterns.fasta file contains the patterns. 
The program measures the query times given in the description of the homework and lists the results shown in the example output. Also It creates a file hw2.out that contains the results.

Important note:
The program is implemented with the Suffix Array and Suffix Tree algorithms. The Suffix Tree implementation is not working properly. It is implemented but not working as expected. 
It gives segmentation fault error. Due to this reason, it is not listed in the hw2 file and not included in the hw2.out. Instead, it is compiled with the command "make run_suffixtree" and 
run with the command "make run_suffixtree". The program will not find the patterns in the text. As it gives segmentation fault error. The outputs are listed when the program is run with the Suffix Array implementation.
Also, hw2-imp.out file is created for the Suffix Tree implementation. It is not included in the hw2.out file.

@Output:
The program prints the results to the terminal. The results include the number of patterns found in the text, the time it took to find the patterns, and the indices of the patterns in the text.