#pragma once
#include "DrawableBase.h"
#include <random>
class Cube : public DrawableBase<Cube>
{
public:
	Cube(Graphics& gfx,
		float x, float y, float z, float speed, float angle, DirectX::XMFLOAT3 materialColor);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	float x;
	float y;
	float z;
	float speed;
	float angle;
	DirectX::XMFLOAT3 materialColor;

};