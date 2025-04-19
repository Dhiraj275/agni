#pragma once
#include "Drawable.h"
class Box : public Drawable
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