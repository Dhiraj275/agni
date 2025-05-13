#include "Skybox.h"
#include <cmath>

Skybox::Skybox(Graphics& gfx)
{
	if (!IsStaticInitialized()) {
		struct Vertex {
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 tex;
		};
		auto tileUV = [](int x, int y) -> std::pair<float, float> {
			const float textureWidth = 1024*4;
			const float textureHeight = 1024 * 3;
			const float tileWidth = textureWidth / 4.0f;
			const float tileHeight = textureHeight / 3.0f;

			return {
				(x * tileWidth) / textureWidth,
				(y * tileHeight) / textureHeight
			};
			};

		// Corrected UV Coordinates
		// Face order: Front, Back, Top, Bottom, Left, Right
		auto [uFrontMin, vFrontMin] = tileUV(2, 1);
		auto [uFrontMax, vFrontMax] = tileUV(3, 2);

		auto [uBackMin, vBackMin] = tileUV(0, 1);
		auto [uBackMax, vBackMax] = tileUV(1, 2);

		auto [uTopMin, vTopMin] = tileUV(2, 0);
		auto [uTopMax, vTopMax] = tileUV(3, 1);

		auto [uBottomMin, vBottomMin] = tileUV(2, 2);
		auto [uBottomMax, vBottomMax] = tileUV(3, 3);

		auto [uLeftMin, vLeftMin] = tileUV(1, 1);
		auto [uLeftMax, vLeftMax] = tileUV(2, 2);

		auto [uRightMin, vRightMin] = tileUV(3, 1);
		auto [uRightMax, vRightMax] = tileUV(4, 2);

		const std::vector<Vertex> vertices = {
			// Front face
			{ { -1.0f, -1.0f, -1.0f }, { uFrontMin, vFrontMax } },
			{ { -1.0f,  1.0f, -1.0f }, { uFrontMin, vFrontMin } },
			{ {  1.0f,  1.0f, -1.0f }, { uFrontMax, vFrontMin } },
			{ {  1.0f, -1.0f, -1.0f }, { uFrontMax, vFrontMax } },

			// Back face
			{ { -1.0f, -1.0f, 1.0f }, { uBackMin, vBackMax } },
			{ {  1.0f, -1.0f, 1.0f }, { uBackMax, vBackMax } },
			{ {  1.0f,  1.0f, 1.0f }, { uBackMax, vBackMin } },
			{ { -1.0f,  1.0f, 1.0f }, { uBackMin, vBackMin } },

			// Top face
			{ { -1.0f, 1.0f, -1.0f }, { uTopMin, vTopMax } },
			{ { -1.0f, 1.0f,  1.0f }, { uTopMin, vTopMin } },
			{ {  1.0f, 1.0f,  1.0f }, { uTopMax, vTopMin } },
			{ {  1.0f, 1.0f, -1.0f }, { uTopMax, vTopMax } },

			// Bottom face
			{ { -1.0f, -1.0f, -1.0f }, { uBottomMin, vBottomMax } },
			{ {  1.0f, -1.0f, -1.0f }, { uBottomMax, vBottomMax } },
			{ {  1.0f, -1.0f,  1.0f }, { uBottomMax, vBottomMin } },
			{ { -1.0f, -1.0f,  1.0f }, { uBottomMin, vBottomMin } },

			// Left face
			{ { -1.0f, -1.0f,  1.0f }, { uLeftMin, vLeftMax } },
			{ { -1.0f,  1.0f,  1.0f }, { uLeftMin, vLeftMin } },
			{ { -1.0f,  1.0f, -1.0f }, { uLeftMax, vLeftMin } },
			{ { -1.0f, -1.0f, -1.0f }, { uLeftMax, vLeftMax } },

			// Right face
			{ { 1.0f, -1.0f, -1.0f }, { uRightMin, vRightMax } },
			{ { 1.0f,  1.0f, -1.0f }, { uRightMin, vRightMin } },
			{ { 1.0f,  1.0f,  1.0f }, { uRightMax, vRightMin } },
			{ { 1.0f, -1.0f,  1.0f }, { uRightMax, vRightMax } }
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"SkyboxVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PlanePS.cso"));

		const std::vector<unsigned short> indices = {
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
		AddStaticBind(std::make_unique<Texture>(gfx, L"Texture/nightpro.png"));
		AddStaticBind(std::make_unique<Sampler>(gfx));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		AddStaticBind(std::make_unique<Rasterizer>(gfx, D3D11_CULL_FRONT));
	}
	else
	{
		SetIndexFromStatic();
	}
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

void Skybox::Update(float dt) noexcept
{
}

DirectX::XMMATRIX Skybox::GetTransformXM() const noexcept
{
	float scale = 100.0f;
	return DirectX::XMMatrixScaling(scale, scale, scale);
}


Skybox::TransformCBuf::TransformCBuf(Graphics& gfx, Drawable& parent)
	:parent(parent)
{
	INFOMAN(gfx);
	if (!pVertexConstantBuffer)
	{
		pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
	}
}

void Skybox::TransformCBuf::Bind(Graphics& gfx) noexcept
{

	//update
	INFOMAN(gfx);
	// Get the view matrix from the camera
	auto view = gfx.GetCamera();

	// Zero out the translation part to keep the skybox centered
	view.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	// Create modelView without camera translation
	const auto modelView = parent.GetTransformXM() * view;
	const Transforms tf =
	{
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

std::unique_ptr<VertexConstantBuffer<Skybox::TransformCBuf::Transforms>> Skybox::TransformCBuf::pVertexConstantBuffer;