#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <random>
#include <vector>
#include <list>
#include <deque>
#include <array>
#include <stdexcept>
#include <algorithm>
#include <iterator>

namespace MAD::MathUtils
{
    inline std::mt19937& rng() {
        static thread_local std::mt19937 gen{ std::random_device{}() };
        return gen;
    }

    inline float frand(float a, float b)
    {
        auto [lo, hi] = std::minmax(a, b);
        std::uniform_real_distribution<float> d(lo, hi);
        return d(rng());
    }

    inline sf::Color colRand(const sf::Color& a, const sf::Color& b)
    {
        return {
            static_cast<sf::Uint8>(frand(a.r, b.r)),
            static_cast<sf::Uint8>(frand(a.g, b.g)),
            static_cast<sf::Uint8>(frand(a.b, b.b)),
            static_cast<sf::Uint8>(frand(a.a, b.a))
        };
    }

    inline sf::Color colRand()
    {
        return {
            static_cast<sf::Uint8>(frand(0, 255)),
            static_cast<sf::Uint8>(frand(0, 255)),
            static_cast<sf::Uint8>(frand(0, 255)),
            255
		};
    }

    inline sf::Vector2f vrand(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        auto [lx, hx] = std::minmax(a.x, b.x);
        auto [ly, hy] = std::minmax(a.y, b.y);
        return { frand(lx, hx), frand(ly, hy) };
    }

    inline float VectorLength(const sf::Vector2f& v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    inline bool IsMoving(const sf::Vector2f& v, float threshold = 0.01f)
    {
        return VectorLength(v) >= threshold;
    }

    // Contenedores indexados: vector/array/deque (const y no-const)
    template <typename Container>
    auto& PickRandomIndexed(Container& c)
    {
        if (c.empty()) throw std::runtime_error("Empty container");
        std::uniform_int_distribution<std::size_t> d(0, c.size() - 1);
        return c[d(rng())];
    }
    template <typename Container>
    const auto& PickRandomIndexed(const Container& c)
    {
        if (c.empty()) throw std::runtime_error("Empty container");
        std::uniform_int_distribution<std::size_t> d(0, c.size() - 1);
        return c[d(rng())];
    }

    // Contenedores solo iterables: list, (un)ordered_{set,map}, etc.
    template <typename Container>
    auto& PickRandomIterable(Container& c)
    {
        if (c.empty()) throw std::runtime_error("Empty container");
        std::uniform_int_distribution<std::size_t> d(0, c.size() - 1);
        auto it = c.begin();
        std::advance(it, static_cast<long>(d(rng())));
        return *it;
    }
    template <typename Container>
    const auto& PickRandomIterable(const Container& c)
    {
        if (c.empty()) throw std::runtime_error("Empty container");
        std::uniform_int_distribution<std::size_t> d(0, c.size() - 1);
        auto it = c.begin();
        std::advance(it, static_cast<long>(d(rng())));
        return *it;
    }
}