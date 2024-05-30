# Makefile for running and cleaning up a Python script

# Default values for variables
GAP = -1
MATCH = 1
MISMATCH = -1

# Default target
all: run

build:
	@echo "No compiling needed for Python scripts"

# Command to run the Python script with user-specified or default parameters
run:
	python3 hw3.py -i input.txt -o output.phy --gap $(GAP) --match $(MATCH) --mismatch $(MISMATCH)

clean:
	rm -rf _pycache_/

# Phony targets ensure make knows these are not filenames
.PHONY: build run clean