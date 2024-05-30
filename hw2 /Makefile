# Compiler settings - Can change the compiler and the default compile flags
CC = g++
CFLAGS = -std=c++11

# Define the targets
all: hw2 SuffixTree

# Rule for building hw2
hw2: hw2.cpp
	$(CC) $(CFLAGS) -o hw2 hw2.cpp

# Rule for building SuffixTree
SuffixTree: SuffixTree.cpp
	$(CC) $(CFLAGS) -o SuffixTree SuffixTree2.cpp

# Custom command for running hw2
run: hw2
	./hw2 -t text.fasta -p patterns.fasta

# Custom command for displaying the output of hw2
run-output:
	cat hw2.out

# Custom command for running hw2_imp
run-suffixtree-output:
	cat hw2_imp.out

# Custom command for running SuffixTree
run-suffixtree: SuffixTree
	./SuffixTree -t text.fasta -p patterns.fasta


# Clean rule for removing all the object files and executables
clean:
	rm -f hw2 SuffixTree hw2.out hw2_imp.out
