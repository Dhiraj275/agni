#pragma once
#include <DirectXMath.h>
#include "PerlinNoise.h"
float FractalNoise(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 freq, float amp)
{
    namespace dx = DirectX;
    PerlinNoise3D noiseGen(2025);
    float noiseSum = 0;
    float amplitude = amp;
    dx::XMFLOAT3 frequency = freq;
    for (int i = 0;i < 5;i++) {
        noiseSum += noiseGen.Noise(pos.x * frequency.x, pos.y * frequency.y, pos.z * frequency.z) * amplitude;
        frequency = { frequency.x * 2,frequency.y * 2,frequency.z * 2 };
        amplitude *= 0.0f;
    }
    return noiseSum;
    // value in [-1, 1]
}