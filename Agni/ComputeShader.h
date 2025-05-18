#pragma once
#include "Bindable.h"

class ComputeShader : public Bindable
{
public:
	ComputeShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;
};