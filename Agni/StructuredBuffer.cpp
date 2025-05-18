#include "StructuredBuffer.h"

void StructuredBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->CSSetShaderResources(slot, 1u, pSRV.GetAddressOf());
}

void StructuredBuffer::BindUAV(Graphics& gfx) noexcept
{
	GetContext(gfx)->CSSetUnorderedAccessViews(slot, 1u, pUAV.GetAddressOf(), nullptr);
}
