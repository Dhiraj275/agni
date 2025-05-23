#pragma once
#include "Bindable.h"
#include "ComputeVertexBuffer.h"
#include <exception>

// Compute Shader bindable for organizing compute operations
class ComputeShader : public Bindable
{
public:
    ComputeShader(Graphics& gfx, const std::wstring& path, const std::string& entryPoint = "CSMain")
    {
        INFOMAN(gfx);

        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob;

        GFX_THROW_INFO(D3DCompileFromFile(
            path.c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint.c_str(),
            "cs_5_0",
            D3DCOMPILE_ENABLE_STRICTNESS,
            0,
            &pBlob,
            &pErrorBlob
        ));

        GFX_THROW_INFO(GetDevice(gfx)->CreateComputeShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &pComputeShader
        ));
    }

    void Bind(Graphics& gfx) noexcept override
    {
        GetContext(gfx)->CSSetShader(pComputeShader.Get(), nullptr, 0);
    }

    void Dispatch(Graphics& gfx, UINT threadGroupsX, UINT threadGroupsY = 1, UINT threadGroupsZ = 1) noexcept
    {
        GetContext(gfx)->Dispatch(threadGroupsX, threadGroupsY, threadGroupsZ);
    }

    // Helper to calculate thread groups based on vertex count
    static UINT CalculateThreadGroups(UINT vertexCount, UINT threadsPerGroup = 256)
    {
        return (vertexCount + threadsPerGroup - 1) / threadsPerGroup;
    }

protected:
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;
};

// Usage example combining both classes
class NoiseProcessor
{
private:
    std::unique_ptr<ComputeVertexBuffer> pVertexBuffer;
    std::unique_ptr<ComputeShader> pComputeShader;
    std::unique_ptr<class ConstantBuffer<struct NoiseParams>> pNoiseParams;

public:
    template<class V>
    NoiseProcessor(Graphics& gfx, const std::vector<V>& vertices)
    {
        pVertexBuffer = std::make_unique<ComputeVertexBuffer>(gfx, vertices);
        pComputeShader = std::make_unique<ComputeShader>(gfx, L"NoiseCompute.hlsl");

        // Initialize noise parameters (you'd need to define NoiseParams struct)
        // pNoiseParams = std::make_unique<ConstantBuffer<NoiseParams>>(gfx, 0u);
    }

    void ApplyNoise(Graphics& gfx, float amplitude, float frequency, float time, float seed)
    {
        // Update noise parameters
        // NoiseParams params = { amplitude, frequency, time, seed };
        // pNoiseParams->Update(gfx, params);

        // Bind compute shader and resources
        pComputeShader->Bind(gfx);
        pVertexBuffer->BindForCompute(gfx, 0u);
        // pNoiseParams->Bind(gfx);

        // Dispatch compute shader
        UINT threadGroups = ComputeShader::CalculateThreadGroups(pVertexBuffer->GetVertexCount());
        pComputeShader->Dispatch(gfx, threadGroups);

        // Clean up
        pVertexBuffer->UnbindFromCompute(gfx, 0u);
    }

    // Get the vertex buffer for normal rendering
    ComputeVertexBuffer& GetVertexBuffer()
    {
        return *pVertexBuffer;
    }
};