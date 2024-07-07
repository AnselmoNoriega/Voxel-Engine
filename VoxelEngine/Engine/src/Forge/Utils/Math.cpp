
#include "pch.h"
#include "Math.h"

#include <iostream>
#include <numeric>
#include <random>

namespace Math
{
    std::unique_ptr<PerlinNoise> PerlinNoise::sInstance;
    std::once_flag PerlinNoise::sInitInstanceFlag;

    std::vector<int> PerlinNoise::mRandomArray;

    void PerlinNoise::GenerateRandomValues(uint32_t seed)
    {
        mRandomArray.resize(256);
        std::iota(mRandomArray.begin(), mRandomArray.end(), 0);
        std::default_random_engine engine(seed);
        shuffle(mRandomArray.begin(), mRandomArray.end(), engine);
        mRandomArray.insert(mRandomArray.end(), mRandomArray.begin(), mRandomArray.end());
    }

    double PerlinNoise::Noise(double x, double y)
    {
        int X = static_cast<int>(floor(x)) & 255;
        int Y = static_cast<int>(floor(y)) & 255;

        x -= floor(x);
        y -= floor(y);

        double u = Fade(x);
        double v = Fade(y);

        std::call_once(sInitInstanceFlag,
            []()
            {
                sInstance.reset(new PerlinNoise());

                std::random_device rd;
                std::mt19937 engine(rd());
                std::uniform_int_distribution<int> dist(-255, 255);

                sInstance->GenerateRandomValues(dist(engine));
            }
        );

        int aa = mRandomArray[mRandomArray[X] + Y];
        int ab = mRandomArray[mRandomArray[X] + Y + 1];
        int ba = mRandomArray[mRandomArray[X + 1] + Y];
        int bb = mRandomArray[mRandomArray[X + 1] + Y + 1];

        double res = Lerp(v, Lerp(u, Grad(aa, x, y), Grad(ba, x - 1, y)), Lerp(u, Grad(ab, x, y - 1), Grad(bb, x - 1, y - 1)));
        return (res + 1.0) / 2.0;
    }

    void PerlinNoise::GenerateHeightMap(int xCoord, int yCoord, int size, int* heightMap)
    {
        // Frequency affects the level of detail in the noise
        double frequency = 1.0;
        // Amplitude affects the range of height values
        // Desired height range
        double amplitude = 60.0;
        double midpointOffsetFromAmplitude = -20.0;

        for (int x = 0; x < size; ++x)
        {
            for (int y = 0; y < size; ++y)
            {
                double frequencyPerSize = frequency / size;
                double noiseValue = Noise((x + xCoord) * frequencyPerSize, (y + yCoord) * frequencyPerSize);
                heightMap[x + (y * size)] = static_cast<int>(((noiseValue + 1) / 2) * amplitude + midpointOffsetFromAmplitude);
            }
        }
    }

    double Fade(double t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    double Lerp(double t, double a, double b)
    {
        return a + t * (b - a);
    }

    double Grad(int hash, double x, double y)
    {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
}
