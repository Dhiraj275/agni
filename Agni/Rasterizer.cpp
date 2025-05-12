#include "Rasterizer.h"
Rasterizer::Rasterizer(Graphics& gfx)
{
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthClipEnable = true;

	GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterState);
}

Rasterizer::Rasterizer(Graphics& gfx, D3D11_CULL_MODE mode)
{
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = mode; // Instead of CULL_BACK
	rasterDesc.DepthClipEnable = true;

	GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterState);
}


void Rasterizer::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->RSSetState(pRasterState.Get());
}
