#include "../src/pathfinder/graph.hpp"
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

template <class GraphGenerator>
void testGraph()
{
    printGraph(GraphGenerator::generate(2));
    printGraph(GraphGenerator::generate(3));
    printGraph(GraphGenerator::generate(5));
    printGraph(GraphGenerator::generate(10));
}

int main(void)
{
    std::cout << "Full graphs:\n";
    testGraph<FullGraphGenerator>();

    std::cout << "Partial graphs:\n";
    testGraph<PartialGraphGenerator>();

    std::cout << "Tree graphs:\n";
    testGraph<TreeGraphGenerator>();

    return 0;
}