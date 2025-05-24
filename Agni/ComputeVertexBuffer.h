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

        // Create compute buffer (structured buffer for compute operations)
        D3D11_BUFFER_DESC computeDesc = {};
        computeDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        computeDesc.Usage = D3D11_USAGE_DEFAULT;
        computeDesc.CPUAccessFlags = 0u;
        computeDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        computeDesc.ByteWidth = sizeof(V) * vertexCount;
        computeDesc.StructureByteStride = sizeof(V);

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertices.data();

        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&computeDesc, &sd, &pComputeBuffer));

        // Create vertex buffer (separate buffer for rendering)
        D3D11_BUFFER_DESC vertexDesc = {};
        vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexDesc.CPUAccessFlags = 0u;
        vertexDesc.MiscFlags = 0u;
        vertexDesc.ByteWidth = sizeof(V) * vertexCount;
        vertexDesc.StructureByteStride = sizeof(V);

        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vertexDesc, &sd, &pVertexBuffer));

        // Create Shader Resource View for compute buffer
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = vertexCount;

        GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
            pComputeBuffer.Get(), &srvDesc, &pShaderResourceView));

        // Create Unordered Access View for compute buffer
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = vertexCount;

        GFX_THROW_INFO(GetDevice(gfx)->CreateUnorderedAccessView(
            pComputeBuffer.Get(), &uavDesc, &pUnorderedAccessView));
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

    // Copy compute results to vertex buffer for rendering
    void CopyComputeToVertex(Graphics& gfx) noexcept
    {
        GetContext(gfx)->CopyResource(pVertexBuffer.Get(), pComputeBuffer.Get());
    }

    // Get the vertex buffer for rendering
    ID3D11Buffer* GetVertexBuffer() const noexcept
    {
        return pVertexBuffer.Get();
    }

    // Get the compute buffer
    ID3D11Buffer* GetComputeBuffer() const noexcept
    {
        return pComputeBuffer.Get();
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
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;      // For rendering
    Microsoft::WRL::ComPtr<ID3D11Buffer> pComputeBuffer;     // For compute operations
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> pUnorderedAccessView;
};
