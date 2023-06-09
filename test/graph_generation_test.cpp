#include "../src/pathfinder/graph.hpp"
#include <iostream>
#include <variant>
#include <vector>

template <class T>
void printGraph(Graph<T> const& graph)
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
    printGraph(Graph<T>::generate(2));
    printGraph(Graph<T>::generate(3));
    printGraph(Graph<T>::generate(5));
    printGraph(Graph<T>::generate(10));
}

int main(void)
{
    std::cout << "Full graphs:\n";
    testGraph<FullGraph>();

    std::cout << "Partial graphs:\n";
    testGraph<PartialGraph>();

    std::cout << "Tree graphs:\n";
    testGraph<TreeGraph>();

    return 0;
}