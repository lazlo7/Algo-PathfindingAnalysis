#include "pathfinders.hpp"

#include <algorithm>
#include <deque>

namespace Pathfinders {
Path Dijkstra::pathfind(Graph const& graph, Vertex from, Vertex to)
{
    std::vector<DistType> dist(graph.vertex_count(), kDistInf);
    Path prev(graph.vertex_count(), kVertexError);

    std::deque<Vertex> q(graph.vertex_count());
    std::ranges::generate(q, [it { graph.begin() }] mutable { return (it++)->first; });

    dist[from] = 0;

    while (!q.empty()) {
        auto const u_it = std::ranges::min_element(q, [&dist](auto const lhs, auto const rhs) { return dist[lhs] < dist[rhs]; });
        auto const u = *u_it;

        if (u == to) {
            break;
        }

        q.erase(u_it);

        for (auto const [adjacent, adjacent_dist] : graph.adjacent(u)) {
            auto const alt = dist[u] + adjacent_dist;
            if (alt < dist[adjacent]) {
                dist[adjacent] = alt;
                prev[adjacent] = u;
            }
        }
    }

    Pathfinders::Path path;
    while (to != kVertexError) {
        path.push_back(to);
        to = prev[to];
    }

    std::ranges::reverse(path);
    return path;
}
}
