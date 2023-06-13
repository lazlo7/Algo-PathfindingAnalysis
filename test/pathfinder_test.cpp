#include "../src/pathfinder/pathfinders.hpp"
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

Vertex randomVertex(Graph const& graph)
{
    return std::next(graph.begin(), util::getRandomNumber(0uz, graph.vertex_count() - 1))->first;
}

int main(void)
{
    using PathfinderTs = std::variant<Pathfinders::Dijkstra, Pathfinders::FloydWarshall>;
    std::vector<PathfinderTs> pathfinders = { Pathfinders::Dijkstra {}, Pathfinders::FloydWarshall {} };
    static constexpr int test_count = 100;

    for (auto i = 0; i < test_count; ++i) {
        auto const graph = Tree::generate(100);
        auto const from = randomVertex(graph);
        auto const to = randomVertex(graph);

        bool firstDefined = false;
        std::string firstName;
        Pathfinders::Path controlPath;
        for (auto const& pathfinder : pathfinders) {
            auto const [path, name] = std::visit([&graph, from, to](auto&& p) { return std::make_tuple(p.pathfind(graph, from, to), p.name()); }, pathfinder);
            if (!firstDefined) {
                controlPath = path;
                firstName = name;
                firstName = firstDefined = true;
            } else {
                if (path != controlPath) {
                    std::cout << "Discrepancy found!\n";
                    printGraph(graph);
                    std::cout << "Algorithms: " << firstName << " | " << name << '\n';
                    return 0;
                }
            }
        }
    }

    std::cout << "All tests passed.\n";

    return 0;
}