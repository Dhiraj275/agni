#pragma once
#include "DrawableBase.h"
#include <random>
class Model3d : public DrawableBase<Model3d>
{
public:
	Model3d(Graphics& gfx,
		float x, float y, float z, float speed, float angle, DirectX::XMFLOAT3 materialColor);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnController();
private:
	float x, y, z, pitch, roll, yaw;
	float speed;
	float angle;
	DirectX::XMFLOAT3 materialColor;

};