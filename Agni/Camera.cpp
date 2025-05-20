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
    bool opened = true;
 
    ImGui::SetNextWindowSize(ImVec2(1280.0f / 6, 720.0/3));
    ImGui::SetNextWindowPos(ImVec2(0,0));

    if (ImGui::Begin("Camera", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
    {
		ImGui::Text("Camera Position");
		ImGui::SliderFloat("Radius", &r, 0.1f, 100.0f);
        ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
        ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);

        ImGui::Text("Camera Orientation");
        ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
        ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
        ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
        if (ImGui::Button("Reset")) {
            ResetCamera();
        }
        if (ImGui::Button("Planet View")) {
            SetCamPlanetView();
        }
    }
    ImGui::End();

}

void Camera::ResetCamera()
{
    r = 20.0f;
    theta = 0.0f;
    phi = 0.0f;
    //orientation
    pitch = 0.0f;
    yaw = 0.0f;
    roll = 0.0f;
}

void Camera::SetCamPlanetView()
{
    r = 10.173f;
    theta = (-71.0f)*PI/180;
    phi = 0.0f;
    //orientation
    pitch = (90.0f) * PI / 180;
    yaw = (-56.0f) * PI / 180;
    roll = (-26.0f) * PI / 180;
}

void Camera::Update(float dt)
{
    r += 5.0f * dt;
    theta += 0.4f * dt;

    // Optional: clamp to prevent it from going too far
    /*if (r > 50.0f) r = 50.0f;
    if (theta > 3.0f) theta = 3.0f;*/
}
