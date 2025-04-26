#include "TransformCBuf.h"
TransformCBuf::TransformCBuf(Graphics& gfx, Drawable & parent)
	:parent(parent)
{
	INFOMAN(gfx);
	if (!pVertexConstantBuffer)
	{
		pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCBuf::Bind(Graphics& gfx) noexcept
{
    
	//update
	INFOMAN(gfx);
	pVertexConstantBuffer->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetCamera() * gfx.GetProjection()
		)
	);
	
	//bind
	pVertexConstantBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuf::pVertexConstantBuffer;
