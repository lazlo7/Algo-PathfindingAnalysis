CC=g++
CFLAGS=-std=c++2b -g
GRAPH_SRC=../src/pathfinder/graphs.cpp
PATHFINDER_SRC=$(GRAPH_SRC) ../src/pathfinder/pathfinders.cpp

graph-generation: graph_generation_test.cpp
	$(CC) $(CFLAGS) $(GRAPH_SRC) graph_generation_test.cpp -o graph-generation-test

prufer-sequence: prufer_sequence_test.cpp
	$(CC) $(CFLAGS) $(GRAPH_SRC) prufer_sequence_test.cpp -o prufer-sequence-test

pathfinder: pathfinder_test.cpp
	$(CC) $(CFLAGS) $(PATHFINDER_SRC) pathfinder_test.cpp -o pathfinder-test

all: graph-generation prufer-sequence pathfinder
.DEFAULT_GOAL := all

clean:
	rm -f graph-generation-test prufer-sequence-test pathfinder-test