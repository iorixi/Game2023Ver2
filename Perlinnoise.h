#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <numeric>

class PerlinNoise {
private:
    std::vector<int> p;

public:
    PerlinNoise();

    double noise(double x, double y, double z);

    // Compute Perlin noise at coordinates x, y, z with specific octaves and persistence
    double octaveNoise(double x, double y, double z, int octaves, double persistence);

private:
    // �C�[�W���O�֐�
    double fade(double t);

    // ���`���
    double lerp(double t, double a, double b);

    // ���z�x�N�g���ƈʒu�x�N�g����
    double grad(int hash, double x, double y, double z);
};