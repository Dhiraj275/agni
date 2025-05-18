#pragma once
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"

class StructuredBuffer : public Bindable
{
public:
	template<class V>
	StructuredBuffer(Graphics& gfx, const std::vector<V>& initData, UINT slot = 0u)
		:
		stride(sizeof(V)),
		slot(slot)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.ByteWidth = UINT(sizeof(V) * initData.size());
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = initData.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pStructuredBuffer));

		// Create shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.ElementOffset = 0;
		srvDesc.Buffer.NumElements = (UINT)initData.size();

		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pStructuredBuffer.Get(), &srvDesc, &pSRV));

		// Create UAV (Unordered Access View)
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = (UINT)initData.size();
		uavDesc.Buffer.Flags = 0;

		GFX_THROW_INFO(GetDevice(gfx)->CreateUnorderedAccessView(pStructuredBuffer.Get(), &uavDesc, &pUAV));

	}
	void Bind(Graphics& gfx) noexcept override;
	void BindUAV(Graphics& gfx) noexcept;
	ID3D11UnorderedAccessView* GetUAV() const noexcept {
		return pUAV.Get();
	}
	ID3D11ShaderResourceView* GetSRV() const noexcept {
		return pSRV.Get();
	}
protected:
	UINT stride;
	UINT slot;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> pUAV;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pStructuredBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;

};
