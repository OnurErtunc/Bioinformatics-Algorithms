# Makefile for running a Python script for UPGMA phylogenetic tree construction

# Default values for variables
GAP_OPEN = -5
GAP_EXT = -5
MATCH = 1
MISMATCH = -1
FASTA = input.fasta
OUTTREE = seqs.nwk
OUTMAT = seqs.tsv
OUTPLOT = seqs.png

# Default target
all: run

build:
	@echo "No compiling needed for Python scripts"

# Command to run the Python script with user-specified or default parameters
run:
	python3 hw4.py --fasta $(FASTA) --gapopen $(GAP_OPEN) --gapext $(GAP_EXT) --match $(MATCH) --mismatch $(MISMATCH) --outtree $(OUTTREE) --outmat $(OUTMAT) --outplot $(OUTPLOT)

clean:
	rm -rf __pycache__/

# Phony targets ensure make knows these are not filenames
.PHONY: build run clean
