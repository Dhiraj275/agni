#pragma once
#include "Drawable.h"
#include <DirectXMath.h>
class TransformCBuf: public Bindable {
public:
	TransformCBuf(Graphics& gfx, Drawable & parent);
	void Bind(Graphics& gfx) noexcept override;
protected:
	const Drawable& parent;
	DirectX::XMMATRIX matrix;
private:
	static Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;

};