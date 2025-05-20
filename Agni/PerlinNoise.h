#pragma once
#include <cmath>
#include <array>
#include <numeric>
#include <random>

class PerlinNoise3D {
public:
    PerlinNoise3D(unsigned int seed = 2025) {
        // Fill p with values from 0 to 255
        std::iota(p.begin(), p.begin() + 256, 0);

        // Shuffle using the seed
        std::default_random_engine engine(seed);
        std::shuffle(p.begin(), p.begin() + 256, engine);

        // Duplicate the permutation vector
        for (int i = 0; i < 256; ++i)
            p[256 + i] = p[i];
    }

    // Generate Perlin noise for coordinates (x, y, z)
    float Noise(float x, float y, float z) const {
        // Find unit cube that contains point
        int X = int(floorf(x)) & 255;
        int Y = int(floorf(y)) & 255;
        int Z = int(floorf(z)) & 255;

        // Find relative x, y, z in cube
        x -= floorf(x);
        y -= floorf(y);
        z -= floorf(z);

        // Compute fade curves
        float u = Fade(x);
        float v = Fade(y);
        float w = Fade(z);

        // Hash coordinates
        int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
        int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

        // Add blended results from 8 corners of the cube
        return Lerp(w,
            Lerp(v,
                Lerp(u, Grad(p[AA], x, y, z),
                    Grad(p[BA], x - 1, y, z)),
                Lerp(u, Grad(p[AB], x, y - 1, z),
                    Grad(p[BB], x - 1, y - 1, z))),
            Lerp(v,
                Lerp(u, Grad(p[AA + 1], x, y, z - 1),
                    Grad(p[BA + 1], x - 1, y, z - 1)),
                Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
                    Grad(p[BB + 1], x - 1, y - 1, z - 1)))
        );
    }

private:
    std::array<int, 512> p;

    static float Fade(float t) {
        // 6t^5 - 15t^4 + 10t^3
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static float Lerp(float t, float a, float b) {
        return a + t * (b - a);
    }

    static float Grad(int hash, float x, float y, float z) {
        int h = hash & 15;          // Convert to 0–15
        float u = h < 8 ? x : y;    // Choose x or y
        float v = h < 4 ? y : (h == 12 || h == 14 ? x : z); // Choose y or z
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }
};
