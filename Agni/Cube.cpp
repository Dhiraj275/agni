#include "Cube.h"
#include "TransformCBuf.h"
#include "GraphicsThrowMacros.h"
#include <cmath>

Cube::Cube(Graphics& gfx, float x, float y, float z, float speed, float angle, DirectX::XMFLOAT3 materialColor):
	x(x),
	y(y),
	z(z),
	speed(speed),
	angle(angle),
	materialColor(materialColor)
{
	if (!IsStaticInitialized()) {
		struct Vertex {
			struct
			{
				float x;
				float y;
				float z;
			} pos;
		};
		const std::vector<Vertex> vertices = {
			{ -1.0f,-1.0f,-1.0f },
			{ 1.0f,-1.0f,-1.0f },
			{ -1.0f,1.0f,-1.0f },
			{ 1.0f,1.0f,-1.0f },
			{ -1.0f,-1.0f,1.0f },
			{ 1.0f,-1.0f,1.0f },
			{ -1.0f,1.0f,1.0f },
			{ 1.0f,1.0f,1.0f },
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"SolidVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"SolidPS.cso"));

		const std::vector<unsigned short> indices = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

void Cube::Update(float dt) noexcept
{
	angle += 1.0f * dt;
}

DirectX::XMMATRIX Cube::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixRotationY(angle)*
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixTranslation(x, y, z);
}

void Cube::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	x = pos.x;
	y = pos.y;
	z = pos.z;
}
