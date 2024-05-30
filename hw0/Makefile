# Compiler settings
CXX = g++
CXXFLAGS = -Wall -g

# Makefile settings
APPNAME = hw0
SOURCES = main.cpp SparseMatrix.cpp Node.cpp

all: compile

compile: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(APPNAME)

clean:
	rm -f $(APPNAME)
