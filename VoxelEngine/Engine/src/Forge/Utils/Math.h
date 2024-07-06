#pragma once
#include <vector>

namespace Math
{
    class PerlinNoise
    {
    public:
        static std::vector<std::vector<double>> GenerateHeightMap(int width, int height);

    private:
        static double Noise(double x, double y);
        static double Noise(double x, double y, uint32_t seed);

        static void GenerateRandomValues();
        static void GenerateRandomValues(uint32_t seed);

    private:
        static std::unique_ptr<PerlinNoise> sInstance;
        static std::once_flag sInitInstanceFlag;

        static std::vector<int> RandomArray;
    };

    static double Fade(double t);
    static double Lerp(double t, double a, double b);
    static double Grad(int hash, double x, double y);
}