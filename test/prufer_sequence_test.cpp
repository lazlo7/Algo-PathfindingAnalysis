#include "../src/pathfinder/graphs.hpp"
#include <iostream>
#include <vector>

using namespace Graphs;

std::vector<Vertex> generatePrefill(size_t vertex_count)
{
    auto const to_pick_count = vertex_count - 2;
    std::vector<Vertex> result(to_pick_count);

    for (auto i = 0uz; i < to_pick_count; ++i) {
        result[i] = util::getRandomNumber(1uz, vertex_count);
    }

    return result;
}

void printVector(std::vector<int> const& v)
{
    std::cout << "Size: " << v.size() << '\n';
    for (auto elem : v) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}

int main(void)
{
    printVector(generatePrefill(2));
    printVector(generatePrefill(5));
    printVector(generatePrefill(10));
    printVector(generatePrefill(30));

    return 0;
}