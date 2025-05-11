#pragma 
#include "BindableBase.h"
#include "Graphics.h"
class Texture: public Bindable
{
public:
	Texture(Graphics & gfx);
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};

