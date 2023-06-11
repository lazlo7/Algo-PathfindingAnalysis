#include "tester.hpp"
#include "util.hpp"

#include <iostream>

void Tester::runTests(std::ostream& output_stream, size_t test_repeat_count, size_t endpoints_generation_repeat_count)
{
    output_stream << "graph_type,vertex_count,edge_count,pathfinder,time_nanos\n";

    std::vector<TestResult> test_results(_pathfinders.size());
    auto current_test_number = 0;
    auto const total_test_count = _graphGenerators.size()
        * ((kMaxVertexCount - kMinVertexCount) / kVertexCountStep + 1)
        * _pathfinders.size();

    for (auto const& graph_generator : _graphGenerators) {
        for (auto vertex_count = kMinVertexCount; vertex_count <= kMaxVertexCount; vertex_count += kVertexCountStep) {
            auto const graph = std::visit([vertex_count](auto&& g) { return g.generate(vertex_count); }, graph_generator);
            auto const graph_name = std::visit([](auto&& g) { return g.name(); }, graph_generator);
            std::ranges::fill(test_results, TestResult {});

            for (auto endpoints_i = 0uz; endpoints_i < endpoints_generation_repeat_count; ++endpoints_i) {
                auto const from = util::getRandomNumber(0uz, graph.vertex_count() - 1);
                auto const to = util::getRandomNumber(0uz, graph.vertex_count() - 1);

                for (auto pathfinder_index = 0uz; pathfinder_index < _pathfinders.size(); ++pathfinder_index) {
                    auto const& pathfinder = _pathfinders[pathfinder_index];

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

                    test_results[pathfinder_index] += result;
                }
            }

            for (auto pathfinder_index = 0uz; pathfinder_index < _pathfinders.size(); ++pathfinder_index) {
                auto const& pathfinder = _pathfinders[pathfinder_index];
                auto const pathfinder_name = std::visit([](auto&& p) { return p.name(); }, pathfinder);

                auto const result = test_results[pathfinder_index] / endpoints_generation_repeat_count;

                std::cout << "Test -> " << ++current_test_number << '/' << total_test_count << '\t'
                          << "Graph: [type=" << graph_name
                          << ", vertices=" << graph.vertex_count()
                          << ", edges=" << graph.edge_count() << "] + "
                          << pathfinder_name << " pathfinder"
                          << '\n';

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