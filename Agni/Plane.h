#pragma once
#include "DrawableBase.h"
#include <random>
class Plane : public DrawableBase<Plane>
{
public:
	Plane(Graphics& gfx,
		float x, float y, float z, float speed, float angle);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;

private:
	float x;
	float y;
	float z;
	float speed;
	float angle;
	DirectX::XMFLOAT3 materialColor;

};