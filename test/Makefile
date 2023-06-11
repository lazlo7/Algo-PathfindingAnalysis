CC=g++
CFLAGS=-std=c++2b -g
GRAPH_SRC=../src/pathfinder/graphs.cpp
PATHFINDER_SRC=$(GRAPH_SRC) ../src/pathfinder/pathfinders.cpp

graph-generation: graph_generation_test.cpp
	$(CC) $(CFLAGS) $(GRAPH_SRC) graph_generation_test.cpp -o graph-generation-test

prufer-sequence: prufer_sequence_test.cpp
	$(CC) $(CFLAGS) $(GRAPH_SRC) prufer_sequence_test.cpp -o prufer-sequence-test

dijkstra: dijkstra_test.cpp
	$(CC) $(CFLAGS) $(PATHFINDER_SRC) dijkstra_test.cpp -o dijkstra-test

all: graph-generation prufer-sequence dijkstra
.DEFAULT_GOAL := all

clean:
	rm -f graph-generation-test prufer-sequence-test dijkstra-test