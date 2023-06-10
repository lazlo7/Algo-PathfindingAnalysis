#pragma once

#include "util.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace Graphs {
using Vertex = int;
using DistType = int;

using StdRepresentation = std::unordered_map<Vertex, std::unordered_map<Vertex, DistType>>;

using VertexIterator = StdRepresentation::iterator;
using VertexConstIterator = StdRepresentation::const_iterator;

using VertexAdjacentIterator = StdRepresentation::mapped_type::iterator;
using VertexAdjacentConstIterator = StdRepresentation::mapped_type::const_iterator;

static DistType constexpr kMinRandomWeight = 1;
static DistType constexpr kMaxRandomWeight = 10;
static DistType constexpr kDistError = -1;
};

class Graph {
public:
    Graph(Graphs::StdRepresentation const& graph)
        : _graph(graph)
        , _vertex_count(graph.size())
    {
        if (_vertex_count <= 1) {
            throw std::invalid_argument("The number of vertices must be at least 2");
        }
    }

    size_t vertex_count() const
    {
        return _vertex_count;
    }

    Graphs::VertexIterator begin()
    {
        return _graph.begin();
    }

    Graphs::VertexIterator end()
    {
        return _graph.end();
    }

    Graphs::VertexConstIterator begin() const
    {
        return _graph.begin();
    }

    Graphs::VertexConstIterator end() const
    {
        return _graph.end();
    }

    Graphs::VertexAdjacentIterator adjacent_begin(Graphs::Vertex v)
    {
        auto it = _graph.find(v);
        return it == _graph.end() ? Graphs::VertexAdjacentIterator(nullptr) : it->second.begin();
    }

    Graphs::VertexAdjacentIterator adjacent_end(Graphs::Vertex v)
    {
        auto it = _graph.find(v);
        return it == _graph.end() ? Graphs::VertexAdjacentIterator(nullptr) : it->second.end();
    }

    Graphs::VertexAdjacentConstIterator adjacent_begin(Graphs::Vertex v) const
    {
        auto it = _graph.find(v);
        return it == _graph.end() ? Graphs::VertexAdjacentIterator(nullptr) : it->second.begin();
    }

    Graphs::VertexAdjacentConstIterator adjacent_end(Graphs::Vertex v) const
    {
        auto it = _graph.find(v);
        return it == _graph.end() ? Graphs::VertexAdjacentIterator(nullptr) : it->second.end();
    }

private:
    Graphs::StdRepresentation _graph;
    size_t _vertex_count {};
};

template <class Generator>
class GraphGenerator {
public:
    static Graph generate(size_t vertex_count)
    {
        return Generator::graph(vertex_count);
    }
};

class FullGraphGenerator : GraphGenerator<FullGraphGenerator> {
public:
    static void fill(Graphs::StdRepresentation& graph, size_t vertex_count)
    {
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
    }

    static Graph generate(size_t vertex_count)
    {
        Graphs::StdRepresentation graph;
        fill(graph, vertex_count);
        return { graph };
    }
};

class PartialGraphGenerator : GraphGenerator<PartialGraphGenerator> {
public:
    static constexpr double kMinDensity = 0.4;
    static constexpr double kMaxDensity = 0.5;

    static Graph generate(size_t vertex_count)
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

        return { graph };
    }

    static void addEdgeWithRandomWeight(Graphs::StdRepresentation& graph, Graphs::Vertex u, Graphs::Vertex v)
    {
        auto const weight = util::getRandomNumber(Graphs::kMinRandomWeight, Graphs::kMaxRandomWeight);
        graph[u][v] = weight;
        graph[v][u] = weight;
    }

private:
    static void addRandomEdges(Graphs::StdRepresentation& graph, size_t edge_count)
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
};

class TreeGraphGenerator : GraphGenerator<TreeGraphGenerator> {
public:
    static Graph generate(size_t vertex_count)
    {
        // Using Prüfer sequence to generate trees.
        std::vector<Graphs::Vertex> S = generatePrefill(vertex_count);

        auto const n = S.size();
        std::unordered_set<Graphs::Vertex> L;
        L.reserve(n + 2);
        for (auto i = 1uz; i < n + 2 + 1; ++i) {
            L.insert(i);
        }

        Graphs::StdRepresentation graph;
        for (auto i = 0uz; i < n; ++i) {
            auto const u = S.back();
            std::vector<Graphs::Vertex> diff;
            std::ranges::set_difference(L, S, std::back_inserter(diff));
            auto const v = *std::min(diff.begin(), diff.end());
            S.pop_back();
            L.erase(v);

            PartialGraphGenerator::addEdgeWithRandomWeight(graph, u - 1, v - 1);
        }

        PartialGraphGenerator::addEdgeWithRandomWeight(graph, *L.begin() - 1, *std::next(L.begin()) - 1);
        return { graph };
    }

private:
    static std::vector<Graphs::Vertex> generatePrefill(size_t vertex_count)
    {
        auto const to_pick_count = vertex_count - 2;
        std::vector<Graphs::Vertex> result(to_pick_count);

        for (auto i = 0uz; i < to_pick_count; ++i) {
            result[i] = util::getRandomNumber(1uz, vertex_count);
        }

        return result;
    }
};
