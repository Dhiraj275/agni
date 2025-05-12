#pragma once
#include "DrawableBase.h"
#include "Bindable.h"
class Skybox : public DrawableBase<Skybox>
{
public:
	Skybox(Graphics& gfx);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
public:
	class TransformCBuf : public Bindable {
	public:
		TransformCBuf(Graphics& gfx, Drawable& parent);
		void Bind(Graphics& gfx) noexcept override;
		struct Transforms
		{
			DirectX::XMMATRIX model;
		};
	private:
		const Drawable& parent;
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertexConstantBuffer;

	};
private:
	struct TransformBuffer {
		DirectX::XMMATRIX transform;
	};
	std::unique_ptr<VertexConstantBuffer<TransformBuffer>> pTransformBuffer;
};