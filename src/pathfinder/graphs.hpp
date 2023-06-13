#pragma once

#include "util.hpp"
#include <limits>
#include <unordered_map>

namespace Graphs {
using Vertex = int;
using Edge = std::pair<Vertex, Vertex>;
using DistType = int;

using StdRepresentation = std::unordered_map<Vertex, std::unordered_map<Vertex, DistType>>;
using Edges = std::vector<Edge>;

using VertexIterator = StdRepresentation::iterator;
using VertexConstIterator = StdRepresentation::const_iterator;
using EdgeIterator = Edges::iterator;
using EdgeConstIterator = Edges::const_iterator;

static Vertex constexpr kVertexError = -1;
static DistType constexpr kMinRandomWeight = 1;
static DistType constexpr kMaxRandomWeight = 10;
static DistType constexpr kDistError = -1;
static DistType constexpr kDistInf = std::numeric_limits<DistType>::max();

class Graph {
public:
    Graph(Graphs::StdRepresentation const& graph, size_t edge_count);

    size_t vertex_count() const;
    size_t edge_count() const;
    VertexIterator begin();
    VertexIterator end();
    VertexConstIterator begin() const;
    VertexConstIterator end() const;
    EdgeIterator edges_begin();
    EdgeIterator edges_end();
    EdgeConstIterator edges_begin() const;
    EdgeConstIterator edges_end() const;

    StdRepresentation::mapped_type const& adjacent(Graphs::Vertex u) const;

private:
    StdRepresentation _graph;
    std::vector<Edge> _edges {};
};

class Full {
public:
    static Graph generate(size_t vertex_count);
    static constexpr inline char const* name()
    {
        return "Full";
    }
};

class Partial {
public:
    static constexpr double kMinDensity = 0.4;
    static constexpr double kMaxDensity = 0.5;

    static Graph generate(size_t vertex_count);
    static void addEdgeWithRandomWeight(Graphs::StdRepresentation& graph, Graphs::Vertex u, Graphs::Vertex v);
    static constexpr inline char const* name()
    {
        return "Partial";
    }

private:
    static void addRandomEdges(Graphs::StdRepresentation& graph, size_t edge_count);
};

class Tree {
public:
    static Graph generate(size_t vertex_count);
    static constexpr inline char const* name()
    {
        return "Tree";
    }

private:
    static std::vector<Graphs::Vertex> generatePruferSequence(size_t vertex_count);
};
};

using Graphs::Graph;
