#pragma once

#include <concepts>
#include <iterator>
#include <random>

namespace util {
static std::default_random_engine random_engine { std::random_device {}() };
static std::uniform_real_distribution<> random_distribution;

template <class T>
    requires std::integral<T>
constexpr T getRandomNumber(T min, T max)
{
    auto const value = random_distribution(random_engine);
    return std::round(min + (max - min) * value);
}

template <class T>
    requires std::floating_point<T>
constexpr T getRandomNumber(T min, T max)
{
    auto const value = random_distribution(random_engine);
    return min + (max - min) * value;
}
}