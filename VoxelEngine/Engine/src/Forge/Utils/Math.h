#pragma once
#include <vector>

namespace Math
{
    class PerlinNoise
    {
    public:
        static void GenerateHeightMap(int xCoord, int yCoord, int size, int* heightMap);

    private:
        static double Noise(double x, double y);

        static void GenerateRandomValues(uint32_t seed);

    private:
        static std::unique_ptr<PerlinNoise> sInstance;
        static std::once_flag sInitInstanceFlag;

        static std::vector<int> mRandomArray;
    };

    static double Fade(double t);
    static double Lerp(double t, double a, double b);
    static double Grad(int hash, double x, double y);
}