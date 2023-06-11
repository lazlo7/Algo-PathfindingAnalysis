#pragma once

#include "graphs.hpp"
#include "pathfinders.hpp"

#include <chrono>
#include <ostream>
#include <variant>
#include <vector>

class Tester {
public:
    void runTests(std::ostream& output_stream, size_t test_repeat_count);

private:
    using TestResult = std::chrono::nanoseconds;
    using GraphGeneratorTs = std::variant<Graphs::Full, Graphs::Partial, Graphs::Tree>;
    using PathfinderTs = std::variant<Pathfinders::Dijkstra>;

    static constexpr auto kMinVertexCount = 10uz;
    static constexpr auto kMaxVertexCount = 1010uz;
    static constexpr auto kVertexCountStep = 50uz; 

    std::vector<GraphGeneratorTs> _graphGenerators = { Graphs::Full {}, Graphs::Partial {}, Graphs::Tree {} };
    std::vector<PathfinderTs> _pathfinders = { Pathfinders::Dijkstra {} };
};
