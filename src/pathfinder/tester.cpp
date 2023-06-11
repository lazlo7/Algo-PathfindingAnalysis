#include "tester.hpp"
#include "util.hpp"

#include <iostream>

void Tester::runTests(std::ostream& output_stream, size_t test_repeat_count)
{
    output_stream << "graph_type,vertex_count,edge_count,pathfinder,time_nanos\n";

    auto current_test_number = 0;
    for (auto const& graph_generator : _graphGenerators) {
        for (auto vertex_count = kMinVertexCount; vertex_count <= kMaxVertexCount; vertex_count += kVertexCountStep) {
            auto const graph = std::visit([vertex_count](auto&& g) { return g.generate(vertex_count); }, graph_generator);
            auto const from = util::getRandomNumber(0uz, graph.vertex_count() - 1);
            auto const to = util::getRandomNumber(0uz, graph.vertex_count() - 1);

            for (auto const& pathfinder : _pathfinders) {
                auto const graph_name = std::visit([](auto&& g) { return g.name(); }, graph_generator);
                auto const pathfinder_name = std::visit([](auto&& p) { return p.name(); }, pathfinder);

                std::cout << "Test -> " << ++current_test_number << "\t"
                          << "Graph: [type=" << graph_name
                          << ", vertices=" << graph.vertex_count()
                          << ", edges=" << graph.edge_count() << "] + "
                          << pathfinder_name << " pathfinder"
                          << '\n';

                auto const result = std::visit([&graph, from, to, test_repeat_count](auto&& p) {
                    TestResult test_result {};

                    for (auto i = 0uz; i < test_repeat_count; ++i) {
                        auto const start = std::chrono::high_resolution_clock::now();
                        p.pathfind(graph, from, to);
                        auto const end = std::chrono::high_resolution_clock::now();

                        test_result += std::chrono::duration_cast<TestResult>(end - start);
                    }

                    test_result /= test_repeat_count;
                    return test_result;
                },
                    pathfinder);

                output_stream << graph_name << ','
                              << vertex_count << ','
                              << graph.vertex_count() << ','
                              << graph.edge_count() << ','
                              << pathfinder_name << ','
                              << result.count()
                              << '\n';

                std::cout << "\t\t\t---> Result: "
                          << result.count() << "ns"
                          << '\n';
            }
        }
    }
}