#include "Cube.h"
#include "TransformCBuf.h"
#include "GraphicsThrowMacros.h"
#include <cmath>

Cube::Cube(Graphics& gfx, float x, float y, float z, float speed, float angle) :
	x(x),
	y(y),
	z(z),
	speed(speed),
	angle(angle)
{
	if (!IsStaticInitialized()) {
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 n;
		};
		const std::vector<Vertex> vertices =
		{
			{ { -0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } }, // 0
			{ {  0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } }, // 1
			{ { -0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } }, // 2
			{ {  0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } }, // 3

			// Far face (+Z) - Normal: (0, 0, 1)
			{ { -0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } }, // 4
			{ {  0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } }, // 5
			{ { -0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } }, // 6
			{ {  0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } }, // 7

			// Left face (-X) - Normal: (-1, 0, 0)
			{ { -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } }, // 8
			{ { -0.5f,  0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } }, // 9
			{ { -0.5f, -0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } }, // 10
			{ { -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } }, // 11

			// Right face (+X) - Normal: (1, 0, 0)
			{ {  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } }, // 12
			{ {  0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } }, // 13
			{ {  0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } }, // 14
			{ {  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } }, // 15

			// Bottom face (-Y) - Normal: (0, -1, 0)
			{ { -0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } }, // 16
			{ {  0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } }, // 17
			{ { -0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } }, // 18
			{ {  0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } }, // 19

			// Top face (+Y) - Normal: (0, 1, 0)
			{ { -0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } }, // 20
			{ {  0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } }, // 21
			{ { -0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } }, // 22
			{ {  0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } }  // 23

		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		const std::vector<unsigned short> indices =
		{
			// Near face (-Z)
			0, 2, 1,  2, 3, 1,
			// Far face (+Z)
			4, 5, 6,  5, 7, 6,
			// Left face (-X)
			8, 10, 9,  9, 10, 11,
			// Right face (+X)
			12, 13, 14,  13, 15, 14,
			// Bottom face (-Y)
			16, 17, 18,  17, 19, 18,
			// Top face (+Y)
			20, 22, 21,  21, 22, 23
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
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
		DirectX::XMMatrixTranslation(5.0f, 0.0f, 0.0f);
}