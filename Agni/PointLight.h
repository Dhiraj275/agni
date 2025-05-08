#pragma once
#include "Graphics.h"
#include "ConstantBuffers.h"
#include "Sphere.h"
class PointLight
{
public:
	PointLight(Graphics& gfx);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Bind(Graphics& gfx) const noexcept;
	void Draw(Graphics& gfx) noexcept;
private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos;
		float padding;
	};
	std::unique_ptr<class Sphere> sphere;
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};
