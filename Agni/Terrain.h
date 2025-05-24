#pragma once
#include "DrawableBase.h"
#include "PerlinNoise.h"
#include "NoiseProcessor.h"
class Terrain: public DrawableBase<Terrain>
{
    public:
    struct Vertex {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT3 color;
    };
    Terrain(Graphics& gfx);
    void Update(float dt) noexcept override;
    void Update(Graphics & gfx) noexcept;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
    std::vector<Vertex> GetVertices() const;
    void SetPos(DirectX::XMFLOAT3 pos) noexcept;
    void SpawnControlWindow() noexcept;
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
private:
private:
    std::vector<uint32_t> indices;
    std::vector<Vertex> vertices;
    std::unique_ptr<ComputeVertexBuffer> vertexBuffer;
    std::unique_ptr<NoiseProcessor> noiseProcessor;
    float frequency = 0;
    float amp = 0;
    float testValue = 0.0f;
    int res = 100;
    float angle = 0;
};

