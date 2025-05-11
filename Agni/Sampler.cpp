#include "Sampler.h"

Sampler::Sampler(Graphics& gfx)
{
	INFOMAN(gfx);
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	GFX_EXCEPT(GetDevice(gfx)->CreateSamplerState(&sampDesc, &pSampler));
}

void Sampler::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());

}
