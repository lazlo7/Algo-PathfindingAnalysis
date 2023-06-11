#include "../src/pathfinder/graphs.hpp"
#include <iostream>
#include <variant>
#include <vector>

using namespace Graphs;

void printGraph(Graph const& graph)
{
    std::cout << "Graph (vertex count: " << graph.vertex_count() << "):\n";
    for (auto const& [v, edges] : graph) {
        std::cout << "Vertex " << v << ":\n";
        std::cout << "Adjacent: ";
        for (auto [adj_v, dist] : edges) {
            std::cout << "(" << adj_v << ", dist: " << dist << ") ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

template <class T>
void testGraph()
{
    printGraph(T::generate(2));
    printGraph(T::generate(3));
    printGraph(T::generate(5));
    printGraph(T::generate(10));
}

int main(void)
{
    std::cout << "Full graphs:\n";
    testGraph<Graphs::Full>();

    std::cout << "Partial graphs:\n";
    testGraph<Graphs::Partial>();

    std::cout << "Tree graphs:\n";
    testGraph<Graphs::Tree>();

    return 0;
}