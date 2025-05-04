#include "TransformCBuf.h"
TransformCBuf::TransformCBuf(Graphics& gfx, Drawable & parent)
	:parent(parent)
{
	INFOMAN(gfx);
	if (!pVertexConstantBuffer)
	{
		pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
	}
}

void TransformCBuf::Bind(Graphics& gfx) noexcept
{
    
	//update
	INFOMAN(gfx);
	const auto modelView = parent.GetTransformXM()*gfx.GetCamera();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
	pVertexConstantBuffer->Update(gfx,
		tf
	);
	
	//bind
	pVertexConstantBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCBuf::Transforms>> TransformCBuf::pVertexConstantBuffer;
