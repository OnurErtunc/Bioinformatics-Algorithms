# Define the compiler
CXX=g++

# Define the flags for the compiler, now using C++11
CXXFLAGS=-std=c++11 -Wall

# Define the target executable
TARGET=hw1

# Define the source files
SRC=hw1.cpp

# Define the pattern and text files
TEXT_FILE=text.fasta
PATTERN_FILE=patterns.fasta

# Default rule to compile and build the executable
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Rule to run Rabin-Karp algorithm
run_rk: $(TARGET)
	./$(TARGET) -i $(TEXT_FILE) -p $(PATTERN_FILE) -m RK

# Rule to run Brute Force algorithm
run_bf: $(TARGET)
	./$(TARGET) -i $(TEXT_FILE) -p $(PATTERN_FILE) -m BF

# Rule to clean the built files
clean:
	rm -f $(TARGET)

# Phony targets for rules that do not represent files
.PHONY: all run_rk run_bf clean
