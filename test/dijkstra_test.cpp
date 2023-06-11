#include "../src/pathfinder/pathfinders.hpp"
#include <iostream>

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

Vertex randomVertex(Graph const& graph)
{
    return std::next(graph.begin(), util::getRandomNumber(0uz, graph.vertex_count() - 1))->first;
}

int main(void)
{
    auto const graph = Graphs::Tree::generate(100);

    auto const from = randomVertex(graph);
    auto const to = randomVertex(graph);
    std::cout << "Path: from = " << from << " | to = " << to << '\n';

    auto const path = Pathfinders::Dijkstra::pathfind(graph, from, to);
    for (auto const v : path) {
        std::cout << v << ' ';
    }
    std::cout << '\n';

    return 0;
}