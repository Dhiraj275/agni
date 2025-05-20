#include "DynamicVertexBuffer.h"
DynamicVertexBuffer::DynamicVertexBuffer(Graphics& gfx, GeoSphere& parent)
	:parent(parent),
	stride(sizeof(GeoSphere::Vertex))
{
	INFOMAN(gfx);

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;
	bd.ByteWidth = UINT(stride * parent.GetVertices().size());
	bd.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = parent.GetVertices().data();

	GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));

}




void DynamicVertexBuffer::Bind(Graphics& gfx) noexcept
{

	//update
	INFOMAN(gfx);
	
	auto vertices =  parent.GetVertices();

	D3D11_MAPPED_SUBRESOURCE msr;
	GFX_THROW_INFO(GetContext(gfx)->Map(
		pVertexBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	));
	memcpy(msr.pData, vertices.data(), stride * vertices.size());
	GetContext(gfx)->Unmap(pVertexBuffer.Get(), 0u);

	//bind

	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

}

