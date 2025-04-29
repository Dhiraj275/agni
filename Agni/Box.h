#pragma once
#include "DrawableBase.h"
#include <random>
class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdis,
		DirectX::XMFLOAT3 materialColor
		);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	DirectX::XMFLOAT3 mtc;
	float r = 0.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float chi = 0.0f;

	float roll;
	float yaw;
	float pitch;

	//deltas

	float dpitch;
	float droll;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
	float dr;


};