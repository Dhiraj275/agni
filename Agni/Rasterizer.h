#pragma once
#include "Bindable.h"
class Rasterizer: public Bindable
{
public:
	Rasterizer(Graphics& gfx);
	Rasterizer(Graphics& gfx, D3D11_CULL_MODE mode);
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterState;
};

