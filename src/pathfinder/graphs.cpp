#include "graphs.hpp"

#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <cassert>

namespace Graphs {
Graph::Graph(Graphs::StdRepresentation const& graph, size_t edge_count)
    : _graph(graph)
    , _vertex_count(graph.size())
    , _edge_count(edge_count)
{
    if (_vertex_count <= 1) {
        throw std::invalid_argument("The number of vertices must be at least 2");
    }
}

size_t Graph::vertex_count() const
{
    return _vertex_count;
}

size_t Graph::edge_count() const
{
    return _edge_count;
}

VertexIterator Graph::begin()
{
    return _graph.begin();
}

VertexIterator Graph::end()
{
    return _graph.end();
}

VertexConstIterator Graph::begin() const
{
    return _graph.begin();
}

VertexConstIterator Graph::end() const
{
    return _graph.end();
}

StdRepresentation::mapped_type const& Graph::adjacent(Graphs::Vertex u) const
{
    return _graph.at(u);
}

Graph Full::generate(size_t vertex_count)
{
    Graphs::StdRepresentation graph;
    graph.reserve(vertex_count);

    for (size_t v = 0; v < vertex_count; ++v) {
        for (size_t adjacent = 0; adjacent < vertex_count; ++adjacent) {
            if (v == adjacent) {
                continue;
            }

            graph[v][adjacent] = graph[adjacent][v] != 0 ? graph[adjacent][v]
                                                         : util::getRandomNumber(Graphs::kMinRandomWeight, Graphs::kMaxRandomWeight);
        }
    }

    auto const edge_count = static_cast<size_t>(0.5 * vertex_count * (vertex_count - 1));
    return { graph, edge_count };
}

Graph Partial::generate(size_t vertex_count)
{
    auto const density = util::getRandomNumber(kMinDensity, kMaxDensity);
    auto const edge_count = static_cast<size_t>(0.5 * density * vertex_count * (vertex_count - 1));
    auto added_edge_count = 0uz;

    // Random Walk Approach.
    // Inspired by: https://stackoverflow.com/a/14618505
    std::vector<Graphs::Vertex> available;
    available.reserve(vertex_count);
    for (auto i = 0uz; i < vertex_count; ++i) {
        available.push_back(i);
    }

    std::vector<bool> visited(vertex_count);
    auto const current_index = util::getRandomNumber(0uz, available.size() - 1);
    auto current_vertex = available[current_index];
    available.erase(available.begin() + current_index);
    visited[current_vertex] = true;

    Graphs::StdRepresentation graph;

    while (!available.empty()) {
        auto const adjacent_index = util::getRandomNumber(0uz, available.size() - 1);
        auto const adjacent_vertex = available[adjacent_index];

        if (!visited[adjacent_vertex]) {
            addEdgeWithRandomWeight(graph, current_vertex, adjacent_vertex);
            ++added_edge_count;
            available.erase(available.begin() + adjacent_index);
            visited[adjacent_vertex] = true;
        }

        current_vertex = adjacent_vertex;
    }

    if (added_edge_count < edge_count) {
        auto const remainder_edge_count = edge_count - added_edge_count;
        addRandomEdges(graph, remainder_edge_count);
    }

    return { graph, edge_count };
}

void Partial::addEdgeWithRandomWeight(Graphs::StdRepresentation& graph, Graphs::Vertex u, Graphs::Vertex v)
{
    auto const weight = util::getRandomNumber(Graphs::kMinRandomWeight, Graphs::kMaxRandomWeight);
    graph[u][v] = weight;
    graph[v][u] = weight;
}

void Partial::addRandomEdges(Graphs::StdRepresentation& graph, size_t edge_count)
{
    for (auto i = 0uz; i < edge_count; ++i) {
        for (;;) {
            auto const u = util::getRandomNumber(0uz, graph.size() - 1);
            auto const v = util::getRandomNumber(0uz, graph.size() - 1);

            if (u == v) {
                continue;
            }

            if (graph[u][v] == 0) {
                addEdgeWithRandomWeight(graph, u, v);
                break;
            }
        }
    }
}

Graph Tree::generate(size_t vertex_count)
{
    // Using Prüfer sequence to generate trees.
    auto prufer = generatePruferSequence(vertex_count);
    std::vector<Vertex> vertices;
    std::vector<size_t> freq(vertex_count);

    for (auto i = 0uz; i + 2 < vertex_count; ++i) {
        ++freq[prufer[i] - 1];
    }

    StdRepresentation graph;
    graph.reserve(vertex_count);

    for (auto i = 0uz; i + 2 < vertex_count; ++i) {
        for (auto j = 0uz; j < vertex_count; ++j) {
            if (freq[j] == 0) {
                --freq[j];
                Partial::addEdgeWithRandomWeight(graph, j, prufer[i] - 1);
                --freq[prufer[i] - 1];
                break;
            }
        }
    }

    auto j = 0uz;
    Vertex first = kVertexError;
    for (auto i = 0uz; i < vertex_count; i++) {
        if (freq[i] == 0 && j == 0) {
            first = i;
            ++j;
        } else if (freq[i] == 0 && j == 1) {
            assert(first != kVertexError && "'first' must have already been initialized");
            Partial::addEdgeWithRandomWeight(graph, first, i);
        }
    }

    return { graph, vertex_count - 1 };
}

std::vector<Graphs::Vertex> Tree::generatePruferSequence(size_t vertex_count)
{
    auto const to_pick_count = vertex_count - 2;
    std::vector<Graphs::Vertex> result(to_pick_count);

    for (auto i = 0uz; i < to_pick_count; ++i) {
        result[i] = util::getRandomNumber(1uz, vertex_count);
    }

    return result;
}

};