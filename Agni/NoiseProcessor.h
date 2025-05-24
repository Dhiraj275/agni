#pragma once
// Usage example combining both classes
#include "ComputeShader.h"
#include "ComputeVertexBuffer.h"
#include "ConstantBuffers.h"
struct NoiseParams
{
    float amplitude;
    float frequency;
    float time;
    float seed;
};
class NoiseProcessor
{
private:
    std::unique_ptr<ComputeVertexBuffer> pVertexBuffer;
    std::unique_ptr<ComputeShader> pComputeShader;
    std::unique_ptr<ComputeConstantBuffer<NoiseParams>> pNoiseParams;
    NoiseParams noiseParam;
public:
    template<class V>
    NoiseProcessor(Graphics& gfx, const std::vector<V>& vertices)
    {
        pVertexBuffer = std::make_unique<ComputeVertexBuffer>(gfx, vertices);
        pComputeShader = std::make_unique<ComputeShader>(gfx, L"ComputeShader.hlsl", "ApplyNoise");
        NoiseParams noiseParam = {
            0,
            0,
            0.0f,
            2005
        };

        // Initialize noise parameters (you'd need to define NoiseParams struct)
         pNoiseParams = std::make_unique<ComputeConstantBuffer<NoiseParams>>(gfx, noiseParam, 0u);
    }

    void ApplyNoise(Graphics& gfx, float amplitude, float frequency, float time, float seed)
    {
        // Update noise parameters
        NoiseParams noiseParam = { amplitude, frequency, time, seed };
        pNoiseParams->Update(gfx, noiseParam);

        // Bind compute shader and resources
        pComputeShader->Bind(gfx);
        pVertexBuffer->BindForCompute(gfx, 0u);
        pNoiseParams->Bind(gfx);

        // Dispatch compute shader
        UINT threadGroups = ComputeShader::CalculateThreadGroups(pVertexBuffer->GetVertexCount());
        pComputeShader->Dispatch(gfx, threadGroups);

        // Clean up and copy results
        pVertexBuffer->UnbindFromCompute(gfx, 0u);
        pVertexBuffer->CopyComputeToVertex(gfx);  // Copy compute results to vertex buffer
        pVertexBuffer->Bind(gfx);

    }

    // Get the vertex buffer for normal rendering
    ComputeVertexBuffer& GetVertexBuffer()
    {
        return *pVertexBuffer;
    }
};