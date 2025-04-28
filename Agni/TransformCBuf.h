#pragma once
#include "Drawable.h"
#include <DirectXMath.h>
#include "ConstantBuffers.h"
class TransformCBuf: public Bindable {
public:
	TransformCBuf(Graphics& gfx, Drawable & parent);
	void Bind(Graphics& gfx) noexcept override;
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};
private:
	const Drawable& parent;
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertexConstantBuffer;

};