#pragma once

#include "graphs.hpp"
#include <vector>

namespace Pathfinders {
using namespace Graphs;
using Path = std::vector<Vertex>;

// Original Dijkstra's algorithm (with no priority queue optimization).
class Dijkstra {
public:
    static Path pathfind(Graph const& graph, Vertex from, Vertex to);
    static constexpr inline char const* name()
    {
        return "Dijkstra";
    }
};

// Standard Floyd-Warshall implementation
class FloydWarshall {
public:
    static Path pathfind(Graph const& graph, Vertex from, Vertex to);
    static constexpr inline char const* name()
    {
        return "Floyd-Warshall";
    }
};
};
