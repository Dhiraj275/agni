#pragma once
#include "DrawableBase.h"
class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& gfx, float x, float y, float z, float speed, float angle);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	float x;
	float y;
	float z;
	float speed;
	float angle;
};