#pragma once
#include "Drawable.h"
class Box : public Drawable
{
public:
	Box(Graphics& gfx);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	float angle=0.0f;
};