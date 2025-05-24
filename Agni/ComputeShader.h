#pragma once
#include "Bindable.h"
#include "ComputeVertexBuffer.h"
#include <stdexcept>

// Compute Shader bindable for organizing compute operations
class ComputeShader : public Bindable
{
public:
    ComputeShader(Graphics& gfx, const std::wstring& path, const std::string& entryPoint = "main")
    {
        INFOMAN(gfx);

        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob;

        hr =
            D3DCompileFromFile(
                path.c_str(),                        // Shader filename
                nullptr,                             // Defines
                D3D_COMPILE_STANDARD_FILE_INCLUDE,   // Include handler
                entryPoint.c_str(),                          // Entry point
                "cs_5_0",                            // Target
                D3DCOMPILE_ENABLE_STRICTNESS,        // Compile flags
                0,                                   // Additional flags
                &pBlob,               // Output blob
                &pErrorBlob
            );

        if (FAILED(hr))
        {
            if (pErrorBlob)
            {
                std::string errorMsg = static_cast<char*>(pErrorBlob->GetBufferPointer());
                throw std::runtime_error("Compute shader compilation failed: " + errorMsg);
            }
            GFX_THROW_INFO(hr);
        }



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
