#pragma once
#include "Bindable.h"
class Sampler:public Bindable
{
public:
		Sampler(Graphics & gfx);
		void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};

