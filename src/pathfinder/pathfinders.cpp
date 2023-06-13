#include "pathfinders.hpp"
#include "graphs.hpp"

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
            if (!std::ranges::contains(q, adjacent)) {
                continue;
            }

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

Path FloydWarshall::pathfind(Graph const& graph, Vertex from, Vertex to)
{
    std::vector<std::vector<DistType>> dist(graph.vertex_count(), std::vector<DistType>(graph.vertex_count(), kDistInf));
    std::vector<Path> next(graph.vertex_count(), Path(graph.vertex_count(), kVertexError));

    for (auto const& [u, edge] : graph) {
        for (auto const [v, weight] : edge) {
            dist[u][v] = weight;
            next[u][v] = v;
        }
    }

    for (auto k = 0uz; k < graph.vertex_count(); ++k) {
        for (auto i = 0uz; i < graph.vertex_count(); ++i) {
            for (auto j = 0uz; j < graph.vertex_count(); ++j) {
                if (dist[i][k] != kDistInf && dist[k][j] != kDistInf && dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    Path path = { from };
    while (from != to) {
        from = next.at(from).at(to);
        path.push_back(from);
    }

    return path;
}

Path BellmanFord::pathfind(const Graph& graph, Vertex from, Vertex to)
{
    std::vector<DistType> dist(graph.vertex_count(), kDistInf);
    Path prev(graph.vertex_count(), kVertexError);
    
    dist[from] = 0;
    for (auto i = 1uz; i < graph.vertex_count(); ++i) {
        for (auto edge_it = graph.edges_begin(); edge_it != graph.edges_end(); ++edge_it) {
            auto const[u, v] = *edge_it;
            if (dist[u] == kDistInf) {
                continue;
            }

            auto const weight = graph.adjacent(u).at(v);
            auto const alt = dist[u] + weight;
            if (dist[v] > alt) {
                dist[v] = alt;
                prev[v] = u;
            }
        }
    }

    Path path = { to };
    while (from != to) {
        to = prev[to];
        path.push_back(to);
    }

    std::ranges::reverse(path);
    return path;
}
}
