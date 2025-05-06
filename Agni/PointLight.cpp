#include "PointLight.h"
#include "imgui/imgui.h"
PointLight::PointLight(Graphics& gfx)
	:
	cbuf(gfx)
{
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
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
	cube = std::make_unique<Cube>(gfx,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, color);
	cube->SetPos(pos);
	cube->Draw(gfx);
}