#pragma once
#include "Graphics.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight(Graphics& gfx);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Bind(Graphics& gfx) const noexcept;
private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos;
		float padding;
	};
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};
