#pragma once

#include "graph.hpp"
#include <algorithm>
#include <deque>
#include <vector>

namespace Pathfinders {
using Path = std::vector<Graphs::Vertex>;
};

template <class PathfinderImpl>
class Pathfinder {
public:
    static Pathfinders::Path pathfind(Graph const& graph, Graphs::Vertex from, Graphs::Vertex to)
    {
        return PathfinderImpl::pathfind(graph, from, to);
    }
};

// Original Dijkstra's algorithm (with no priority queue optimization).
class DijkstraPathfinderImpl {
public:
    static Pathfinders::Path pathfind(Graph const& graph, Graphs::Vertex from, Graphs::Vertex to)
    {
        std::vector<Graphs::DistType> dist(graph.vertex_count(), Graphs::kDistInf);
        Pathfinders::Path prev(graph.vertex_count(), Graphs::kVertexError);

        std::deque<Graphs::Vertex> q(graph.vertex_count());
        std::ranges::generate(q, [it { graph.begin() }] mutable { return (it++)->first; });

        dist[from] = 0;

        while (!q.empty()) {
            auto const u_it = std::ranges::min_element(q, [&dist](auto const lhs, auto const rhs) { return dist[lhs] < dist[rhs]; });
            auto const u = *u_it;

            if (u == to) {
                break;
            }

            q.erase(u_it);

            for (auto const[adjacent, adjacent_dist] : graph.adjacent(u)) {
                auto const alt = dist[u] + adjacent_dist;
                if (alt < dist[adjacent]) {
                    dist[adjacent] = alt;
                    prev[adjacent] = u;
                }
            }
        }

        Pathfinders::Path path;
        while (to != Graphs::kVertexError) {
            path.push_back(to);
            to = prev[to];
        }

        std::ranges::reverse(path);
        return path;
    }
};

using DijkstraPathfinder = Pathfinder<DijkstraPathfinderImpl>;