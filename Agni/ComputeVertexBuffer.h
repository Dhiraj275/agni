#pragma once
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
class ComputeVertexBuffer : public Bindable
{
public:
    template<class V>
    ComputeVertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
        : stride(sizeof(V)), vertexCount(static_cast<UINT>(vertices.size()))
    {
        INFOMAN(gfx);

        // Create buffer with compute shader capabilities
        D3D11_BUFFER_DESC bd = {};
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        bd.ByteWidth = sizeof(V) * vertexCount;
        bd.StructureByteStride = sizeof(V);

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertices.data();

        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));

        // Create Shader Resource View
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = vertexCount;

        GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
            pVertexBuffer.Get(), &srvDesc, &pShaderResourceView));

        // Create Unordered Access View
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = vertexCount;

        GFX_THROW_INFO(GetDevice(gfx)->CreateUnorderedAccessView(
            pVertexBuffer.Get(), &uavDesc, &pUnorderedAccessView));
    }

    // Standard vertex buffer binding for rendering
    void Bind(Graphics& gfx) noexcept override
    {
        const UINT offset = 0u;
        GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
    }

    // Bind for compute shader operations
    void BindForCompute(Graphics& gfx, UINT slot = 0u) noexcept
    {
        GetContext(gfx)->CSSetUnorderedAccessViews(slot, 1u, pUnorderedAccessView.GetAddressOf(), nullptr);
    }

    // Unbind from compute shader (important for validation)
    void UnbindFromCompute(Graphics& gfx, UINT slot = 0u) noexcept
    {
        ID3D11UnorderedAccessView* nullUAV = nullptr;
        GetContext(gfx)->CSSetUnorderedAccessViews(slot, 1u, &nullUAV, nullptr);
    }

    // Get the raw buffer pointer if needed
    ID3D11Buffer* GetBuffer() const noexcept
    {
        return pVertexBuffer.Get();
    }

    UINT GetVertexCount() const noexcept
    {
        return vertexCount;
    }

    UINT GetStride() const noexcept
    {
        return stride;
    }

protected:
    UINT stride;
    UINT vertexCount;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> pUnorderedAccessView;
};