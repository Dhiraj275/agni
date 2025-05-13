#include "PointLight.h"
#include "imgui/imgui.h"
PointLight::PointLight(Graphics& gfx)
	:
	cbuf(gfx)
{
}

void PointLight::SpawnControlWindow() noexcept
{
	bool opened = true;

	ImGui::SetNextWindowSize(ImVec2(1280.0f / 6, 720.0 / 5));
	ImGui::SetNextWindowPos(ImVec2(0, 720.0 / 3));

	if (ImGui::Begin("Light", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	pos = { 0.0f,0.0f,0.0f };
}



void PointLight::Bind(Graphics& gfx) const noexcept
{
	cbuf.Update(gfx, PointLightCBuf{ pos });
	cbuf.Bind(gfx);
}

void PointLight::Draw(Graphics& gfx) noexcept
{
	static DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
	sphere = std::make_unique<Sphere>(gfx,0.0f, 0.0f, 0.0f, 0.5f);
	sphere->SetPos(pos);
	sphere->Draw(gfx);
}