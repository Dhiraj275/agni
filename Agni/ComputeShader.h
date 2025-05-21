#pragma once
#include "Bindable.h"

class ComputeShader : public Bindable
{
public:
	template<class V>
	ComputeShader(Graphics& gfx, const std::wstring& path, const std::vector<V>& vector)
		:
		vertexCount(vector.size())
	{
        INFOMAN(gfx);

        // 1. Create and compile the compute shader
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
        GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
        GFX_THROW_INFO(GetDevice(gfx)->CreateComputeShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &pComputeShader));

        // 2. Create the structured buffer with both UAV and SRV flags
        D3D11_BUFFER_DESC shaderResourceViewBufferDesc = {};
        shaderResourceViewBufferDesc.ByteWidth = static_cast<UINT>(vector.size() * sizeof(V));
        shaderResourceViewBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        shaderResourceViewBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        shaderResourceViewBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        shaderResourceViewBufferDesc.StructureByteStride = sizeof(V);

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vector.data();
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(
            &shaderResourceViewBufferDesc,
            &initData,
            pStructuredBuffer.GetAddressOf()));

        // 3. Create UAV for compute shader to write to
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = static_cast<UINT>(vector.size());
        GFX_THROW_INFO(GetDevice(gfx)->CreateUnorderedAccessView(
            pStructuredBuffer.Get(),
            &uavDesc,
            pUAV.GetAddressOf()));

        // 4. Also create SRV (uncomment this) - might be needed for binding as input
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.ElementOffset = 0;
        srvDesc.Buffer.NumElements = static_cast<UINT>(vector.size());
        GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
            pStructuredBuffer.Get(),
            &srvDesc,
            pShaderResourceView.GetAddressOf()));

        // 5. Create constant buffer for vertex count
        struct Buffer {
            int vertexCount;
            float padding;
            float padding1;
            float padding2;
        };

        Buffer buf = { static_cast<int>(vector.size()) };

        D3D11_BUFFER_DESC cbd = {};
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(Buffer);
        cbd.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA csd = {};
        csd.pSysMem = &buf;
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));

        // Create a staging buffer for reading data back to CPU
        D3D11_BUFFER_DESC stagingDesc = {};
        stagingDesc.Usage = D3D11_USAGE_STAGING;
        stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        stagingDesc.BindFlags = 0;
        stagingDesc.ByteWidth = static_cast<UINT>(vector.size() * sizeof(V));
        stagingDesc.MiscFlags = 0;
        stagingDesc.StructureByteStride = sizeof(V);

        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(
            &stagingDesc,
            nullptr,
            pStagingBuffer.GetAddressOf()));
	}

	void Bind(Graphics& gfx) noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pStructuredBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView >pUAV;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pStagingBuffer;


	UINT vertexCount;
};