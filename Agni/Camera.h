#pragma once
#include <DirectXMath.h>
class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnCameraController();
	void ResetCamera();
	void Update(float dt);
private:
	//position
	float r = 0.1f;
	float theta = 0.0f;
	float phi = 0.0f;
	//orientation
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll= 0.0f;

};

