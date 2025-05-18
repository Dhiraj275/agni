#include "ComputeShader.h"
#include "StructuredBuffer.h"


ComputeShader::ComputeShader(Graphics& gfx, const std::wstring& path)
{
	INFOMAN(gfx);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
	GFX_THROW_INFO(GetDevice(gfx)->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComputeShader));
	

}
	
void ComputeShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->CSSetShader(pComputeShader.Get(), nullptr, 0u);
	GetContext(gfx)->Dispatch(512, 1, 1);
}
