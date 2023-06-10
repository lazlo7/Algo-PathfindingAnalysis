CC=g++
CFLAGS=-std=c++2b -g

graph-generation: graph_generation_test.cpp
	$(CC) $(CFLAGS) graph_generation_test.cpp -o graph-generation-test

prufer-sequence: prufer_sequence_test.cpp
	$(CC) $(CFLAGS) prufer_sequence_test.cpp -o prufer-sequence-test

dijkstra: dijkstra_test.cpp
	$(CC) $(CFLAGS) dijkstra_test.cpp -o dijkstra-test

all: graph-generation prufer-sequence dijkstra
.DEFAULT_GOAL := all

clean:
	rm -f graph-generation-test prufer-sequence-test dijkstra-test