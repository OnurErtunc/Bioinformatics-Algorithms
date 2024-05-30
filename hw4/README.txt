Author: Onur Ertunç
Bilkent ID: 21802760
CS 481 - Spring 2024 - HW4
Date: 01/05/2024

Instructions:

Important note: The code uses some libraries. To import them, I put these statements. Bio is not available in dijkstra environment.
import argparse
from Bio import SeqIO
import csv
from Bio import Phylo
from io import StringIO
import matplotlib.pyplot as plt


1- Extract the zip file
2- Open the terminal and navigate to the directory of the extracted files. Named hw4.
3 - Run the python script hw4.py using the makefile. An important note, the homework directory does not contain input.fasta
    There are two options. If you want to run the script with the default input file, run the following command:
        make
    If you want to run the script with a different input file, run a following command like this:
        make run MATCH=2 MISMATCH=-2
4 - The output file will be created in the same directory with the name seqs.nwk, seqs.tsv and seqs.png
5 - If you want to clean the directory, run the following command:
    make clean


Program Explanation:
The program constructs guide tree using UPGMA algorithm . The script reads the sequences from the input file and aligns them
using the Needleman Wunsch Algorithm. There is an issue in the program, especially in the dendrogram. The dendrogram is not
constructed correctly. The reason behind it is a bug in the UPGMA algorithm. The distance matrix is calculated correctly.
The newick format is also created correctly. But I could not solve the bug in the UPGMA algorithm. I am sure that the issue is in
the UPGMA algorithm because I used libraries in that part to test newick format and distance matrix. They were correctly resulted.
Since we are not allowed to use libraries in the UPGMA, I left what I did.

@Input params:
The program takes the following parameters
--fasta:
Description: The input FASTA file containing all sequences to be used for tree construction.
Usage: --fasta FILE
Example: --fasta input.fasta

--outtree:
Description: The output file name where the Newick tree will be saved.
Usage: --outtree FILE
Example: --outtree seqs.nwk

--gapopen:
Description: The gap opening penalty value.
Usage: --gapopen VALUE
Example: --gapopen -5

--gapext:
Description: The gap extension penalty value.
Usage: --gapext VALUE
Example: --gapext -2

--match:
Description: The match score value.
Usage: --match VALUE
Example: --match 1

--mismatch:
Description: The mismatch penalty value.
Usage: --mismatch VALUE
Example: --mismatch -1

--outmat:
Description: The output file name where the distance or similarity matrix will be saved.
Usage: --outmat FILE
Example: --outmat seqs.tsv

--outplot:
Description: The output file name where the tree plot will be saved.
Usage: --outplot FILE
Example: --outplot seqs.png


@Output:
The output file will be created in the same directory with the name seqs.nwk, seqs.tsv and seqs.png


Extra explanations about references and external sources:
I used needleman_wunsch algorithm, argument parser from the previous homework. I also used the matplotlib library to plot the
dendrogram.
