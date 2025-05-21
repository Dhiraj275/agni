#include "ComputeShader.h"
#include "StructuredBuffer.h"



void ComputeShader::Bind(Graphics& gfx) noexcept
{
	UINT numThreadGroups = (vertexCount + 511) / 512;
	GetContext(gfx)->CSSetUnorderedAccessViews(0, 1, pUAV.GetAddressOf(), nullptr);
	GetContext(gfx)->CSSetShader(pComputeShader.Get(), nullptr, 0u);
	GetContext(gfx)->CSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	GetContext(gfx)->Dispatch(numThreadGroups, 1, 1);
}
