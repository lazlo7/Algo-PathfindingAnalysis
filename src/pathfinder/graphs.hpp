#pragma once

#include "util.hpp"
#include <limits>
#include <unordered_map>

namespace Graphs {
using Vertex = int;
using DistType = int;

using StdRepresentation = std::unordered_map<Vertex, std::unordered_map<Vertex, DistType>>;

using VertexIterator = StdRepresentation::iterator;
using VertexConstIterator = StdRepresentation::const_iterator;

using VertexAdjacentIterator = StdRepresentation::mapped_type::iterator;
using VertexAdjacentConstIterator = StdRepresentation::mapped_type::const_iterator;

static Vertex constexpr kVertexError = -1;
static DistType constexpr kMinRandomWeight = 1;
static DistType constexpr kMaxRandomWeight = 10;
static DistType constexpr kDistError = -1;
static DistType constexpr kDistInf = std::numeric_limits<DistType>::max();

class Graph {
public:
    Graph(Graphs::StdRepresentation const& graph);

    size_t vertex_count() const;
    Graphs::VertexIterator begin();
    Graphs::VertexIterator end();
    Graphs::VertexConstIterator begin() const;
    Graphs::VertexConstIterator end() const;
    StdRepresentation::mapped_type const& adjacent(Graphs::Vertex u) const;

private:
    Graphs::StdRepresentation _graph;
    size_t _vertex_count {};
};

class Full {
public:
    static Graph generate(size_t vertex_count);
};

class Partial {
public:
    static constexpr double kMinDensity = 0.4;
    static constexpr double kMaxDensity = 0.5;

    static Graph generate(size_t vertex_count);
    static void addEdgeWithRandomWeight(Graphs::StdRepresentation& graph, Graphs::Vertex u, Graphs::Vertex v);

private:
    static void addRandomEdges(Graphs::StdRepresentation& graph, size_t edge_count);
};

class Tree {
public:
    static Graph generate(size_t vertex_count);

private:
    static std::vector<Graphs::Vertex> generatePrefill(size_t vertex_count);
};
};

using Graphs::Graph;
