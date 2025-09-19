#pragma once
#include <random>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace MAD::MathUtils
{
    struct Intersect
    {
        bool result;
        sf::Vector2f position;
    };

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

    inline int irand(int a, int b)
    {
        auto [lo, hi] = std::minmax(a, b);
        std::uniform_int_distribution<int> d(lo, hi);
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

    inline float cross(const sf::Vector2f& a, sf::Vector2f& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    inline float VectorLength(const sf::Vector2f& v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    inline float dot(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    inline Intersect LineIntersect(const sf::Vector2f& a,const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d,float eps = 1e-6f)
    {
        sf::Vector2f r = (b - a);

        sf::Vector2f s = (d - c);

        float rxs = cross(r, s);

        sf::Vector2f cma = c - a;

        if (std::fabs(rxs) < eps)
        {
            if (std::fabs(cross(cma, r)) < eps)
            {
                float rr = dot(r, r);
                if (rr < eps) return { false,{} };

                float t0 = dot(c - a, r) / rr;
                float t1 = dot(d - a, r) / rr;
                if (t0 > t1) std::swap(t0, t1);
                float t = std::clamp(t0, 0.0f, 1.0f);
                if (t1 >= 0.0f && t0 <= 1.0f)
                {
                    return { true,a + t * r };
                }
            }
            return { false,{} };
        }

        float t = (cross(cma, s)) / rxs;
        float u = (cross(cma, r)) / rxs;

        if (t >= -eps && t <= 1.0f + eps && u >= -eps && u <= 1.0f + eps)
        {
            return { true,sf::Vector2f(a.x + t * r.x,a.y + t * r.y) };
        }
        else
        {
            return { false, {} };
        }
    }

  

    inline sf::Vector2f normalize(const sf::Vector2f& v)
    {
        float len = VectorLength(v);
        return (len > 0.0f) ? (v / len) : sf::Vector2f(0.0f, 0.0f);
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