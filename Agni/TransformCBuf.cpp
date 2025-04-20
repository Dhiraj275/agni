#include "TransformCBuf.h"
TransformCBuf::TransformCBuf(Graphics& gfx, Drawable & parent)
	:parent(parent)

{
	//transformation matrix
    matrix = parent.GetTransformXM();
	D3D11_BUFFER_DESC cbd = {};
	cbd.ByteWidth = sizeof(DirectX::XMMATRIX);
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &matrix;
	//binding constant buffer
	GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	//GFX_THROW_INFO();
}

void TransformCBuf::Bind(Graphics& gfx) noexcept
{
    
	INFOMAN(gfx);
	DirectX::XMMATRIX consts = 
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
		);
	D3D11_MAPPED_SUBRESOURCE msr;
	GFX_THROW_INFO(GetContext(gfx)->Map(
		pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	));
	memcpy(msr.pData, &consts, sizeof(consts));
	GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);

    GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
}


