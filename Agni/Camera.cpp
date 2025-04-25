#include "Camera.h"
#include "ImGui/imgui.h"
DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
    namespace dx = DirectX;
    auto pos = dx::XMVector3Transform(
        dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
        dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
    );
    return 
        dx::XMMatrixLookAtLH(
            pos,
            dx::XMVectorZero(),
            dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
        )
        *
		dx::XMMatrixRotationRollPitchYaw(
			pitch,
			-yaw,
			roll
		);
}

void Camera::SpawnCameraController()
{
    //imgui setup
    if (ImGui::Begin("Camera Controller")) {
		ImGui::Text("Camera Position");
		ImGui::SliderFloat("Radius", &r, 0.1f, 100.0f);
        ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
        ImGui::SliderAngle("Phi", &phi, -89, 89.0f);

        ImGui::Text("Camera Orientation");
        ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
        ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
        ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
    }
    ImGui::End();

}

void Camera::ResetCamera()
{
  
}
