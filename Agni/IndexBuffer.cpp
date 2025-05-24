#include "IndexBuffer.h"
#include "GraphicsThrowMacros.h"

IndexBuffer::IndexBuffer( Graphics& gfx,const std::vector<uint32_t>& indices )
	:
	count( (UINT)indices.size() )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT( count * sizeof(uint32_t) );
	ibd.StructureByteStride = sizeof(uint32_t);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &ibd,&isd,&pIndexBuffer ) );


	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(uint32_t) * count;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = sizeof(uint32_t);

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = indices.data(); // vector<uint32_t> or uint[]

	GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &initData, &pIndexStructuredBuffer));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN; // important for structured buffers
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = count;

	GFX_THROW_INFO(GetDevice(gfx)->CreateUnorderedAccessView(pIndexStructuredBuffer.Get(), &uavDesc, &pIndexUAV));
}

void IndexBuffer::Bind( Graphics& gfx ) noexcept
{
	GetContext( gfx )->IASetIndexBuffer( pIndexBuffer.Get(),DXGI_FORMAT_R32_UINT,0u );
	GetContext (gfx )->CSSetUnorderedAccessViews(1, 1, pIndexUAV.GetAddressOf(), nullptr); // u1
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
