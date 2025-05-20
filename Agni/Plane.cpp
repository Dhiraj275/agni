#include "Plane.h"
#include "TransformCBuf.h"
#include <cmath>

Plane::Plane(Graphics& gfx, float x, float y, float z, float speed, float angle) :
	x(x),
	y(y),
	z(z),
	speed(speed),
	angle(angle)
{
	if (!IsStaticInitialized()) {
		struct Vertex {
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 tex;
		};
		auto tileUV = [](int x, int y) -> std::pair<float, float> {
			const float tileWidth = 384.0f;
			const float tileHeight = 384.0f;
			const float textureWidth = 1536.0f;
			const float textureHeight = 1152.0f;

			return {
				(x * tileWidth) / textureWidth,
				(y * tileHeight) / textureHeight
			};
			};

		// Get UV bounds for each face
		auto [uSideMin, vSideMin] = tileUV(1, 1);
		auto [uSideMax, vSideMax] = tileUV(2, 2);

		auto [uTopMin, vTopMin] = tileUV(1, 0);
		auto [uTopMax, vTopMax] = tileUV(2, 1);

		auto [uBottomMin, vBottomMin] = tileUV(1, 2);
		auto [uBottomMax, vBottomMax] = tileUV(2, 3);

		const std::vector<Vertex> vertices = {
			// Front face (side texture)
			{ { -1.0f, -1.0f, -1.0f }, { uSideMin, vSideMax } },
			{ { -1.0f,  1.0f, -1.0f }, { uSideMin, vSideMin } },
			{ {  1.0f,  1.0f, -1.0f }, { uSideMax, vSideMin } },
			{ {  1.0f, -1.0f, -1.0f }, { uSideMax, vSideMax } },

			// Back face (side texture)
			{ { -1.0f, -1.0f, 1.0f }, { uSideMax, vSideMax } },
			{ {  1.0f, -1.0f, 1.0f }, { uSideMin, vSideMax } },
			{ {  1.0f,  1.0f, 1.0f }, { uSideMin, vSideMin } },
			{ { -1.0f,  1.0f, 1.0f }, { uSideMax, vSideMin } },

			// Top face (grass top)
			{ { -1.0f, 1.0f, -1.0f }, { uTopMin, vTopMax } },
			{ { -1.0f, 1.0f,  1.0f }, { uTopMin, vTopMin } },
			{ {  1.0f, 1.0f,  1.0f }, { uTopMax, vTopMin } },
			{ {  1.0f, 1.0f, -1.0f }, { uTopMax, vTopMax } },

			// Bottom face (dirt)
			{ { -1.0f, -1.0f, -1.0f }, { uBottomMax, vBottomMax } },
			{ {  1.0f, -1.0f, -1.0f }, { uBottomMin, vBottomMax } },
			{ {  1.0f, -1.0f,  1.0f }, { uBottomMin, vBottomMin } },
			{ { -1.0f, -1.0f,  1.0f }, { uBottomMax, vBottomMin } },

			// Left face (side texture)
			{ { -1.0f, -1.0f,  1.0f }, { uSideMin, vSideMax } },
			{ { -1.0f,  1.0f,  1.0f }, { uSideMin, vSideMin } },
			{ { -1.0f,  1.0f, -1.0f }, { uSideMax, vSideMin } },
			{ { -1.0f, -1.0f, -1.0f }, { uSideMax, vSideMax } },

			// Right face (side texture)
			{ { 1.0f, -1.0f, -1.0f }, { uSideMin, vSideMax } },
			{ { 1.0f,  1.0f, -1.0f }, { uSideMin, vSideMin } },
			{ { 1.0f,  1.0f,  1.0f }, { uSideMax, vSideMin } },
			{ { 1.0f, -1.0f,  1.0f }, { uSideMax, vSideMax } },
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PlaneVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PlanePS.cso"));

		const std::vector<uint32_t> indices = {
			// Front face
			0, 1, 2, 0, 2, 3,
			// Back face
			4, 5, 6, 4, 6, 7,
			// Top face
			8, 9,10, 8,10,11,
			// Bottom face
		   12,13,14,12,14,15,
		   // Left face
		  16,17,18,16,18,19,
		  // Right face
		 20,21,22,20,22,23
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12u,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		AddStaticBind(std::make_unique<Texture>(gfx, L"Texture/grass.jpg"));
		AddStaticBind(std::make_unique<Sampler>(gfx));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

void Plane::Update(float dt) noexcept
{
	angle += 1.0f * dt;
}

DirectX::XMMATRIX Plane::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixRotationY(angle) *
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixScaling(2.0f,2.0f,2.0f)*
		DirectX::XMMatrixTranslation(x, y, z);
}

void Plane::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	x = pos.x;
	y = pos.y;
	z = pos.z;
}
