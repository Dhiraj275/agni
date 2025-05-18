#include "DynamicVertexBuffer.h"
DynamicVertexBuffer::DynamicVertexBuffer(Graphics& gfx, GeoSphere& parent)
	:parent(parent)
{
	INFOMAN(gfx);
	pVertexBuffer = ;
}

void DynamicVertexBuffer::Bind(Graphics& gfx) noexcept
{

	//update
	INFOMAN(gfx);
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};
	auto vertices =  parent.GetVertices();
	pVertexBuffer->Update(gfx,
		vertices
	);

	//bind
	pVertexBuffer->Bind(gfx);
}

