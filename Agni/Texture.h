#pragma 
#include "Bindable.h"
class Texture: public Bindable
{
public:
	Texture(Graphics & gfx, const wchar_t* fileName);
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};

