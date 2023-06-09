CC=g++
CFLAGS=-std=c++2b

graph-generation: graph_generation_test.cpp
	$(CC) $(CFLAGS) graph_generation_test.cpp -o graph-generation-test

prufer-sequence: prufer_sequence_test.cpp
	$(CC) $(CFLAGS) prufer_sequence_test.cpp -o prufer-sequence-test

all: graph-generation prufer-sequence
.DEFAULT_GOAL := all

clean:
	rm -f graph-generation-test prufer-sequence-test