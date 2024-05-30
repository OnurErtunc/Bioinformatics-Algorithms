Author: Onur Ertunç
Bilkent ID: 21802760
CS 481 - Spring 2024 - HW3
Date: 16/04/2024

Instructons:
1- Extract the zip file
2- Open the terminal and navigate to the directory of the extracted files. Named hw3.
3 - Run the python script using the makefile. An important note, the homework directory does not contain input.txt and
    sequence files. There are two options. If you want to run the script with the default input file, run the following command:
        make
    If you want to run the script with a different input file, run a following command like this:
        make GAP=-5 MATCH=2 MISMATCH=-2
4 - The output file will be created in the same directory with the name output.phy
5 - If you want to clean the directory, run the following command:
    make clean
    



Program Explanation:
The program makes Progressive Multiple Alignment. The script reads the sequences from the input file and aligns them
using the Needleman Wunsch Algorithm. It utilizes the scoring matrix and gap penalty values given in the input file.
The program writes the aligned sequences to the output file with PHYLIP format.

@Input params:
The program takes the following parameters
1- -i: The input file name. The input file contains the sequences to be aligned.
2- -o: The output file name. The output file will contain the aligned sequences.
3- --gap : The gap penalty value.
4- --match: The match score value.
5- --mismatch: The mismatch score value.


@Output:
The program writes the aligned sequences to the output file with PHYLIP format.
An example:
4 4
Sequence_1 M-PE
Sequence_2 M-KE
Sequence_3 MSKE
Sequence_4 -SKE


Extra explanations about references and external sources:

I used the following sources to understand the Needleman Wunsch Algorithm and the progressive multiple alignment:
https://medium.com/analytics-vidhya/sequence-alignment-and-the-needleman-wunsch-algorithm-710c7b1a23a4

Also I utilized the following github repository to understand the implementation of Needleman Wunsch Algorithm:
https://gist.github.com/slowkow/06c6dba9180d013dfd82bec217d22eb5

I did not copy any code from the above sources. I just used them to understand the algorithm and the implementation.
