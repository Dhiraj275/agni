#pragma once
#include "Drawable.h"
#include "GeoSphere.h"
#include "Terrain.h"
#include <DirectXMath.h>
#include "ConstantBuffers.h"
class DynamicVertexBuffer : public Bindable {
public:
	DynamicVertexBuffer(Graphics& gfx, Terrain& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	const  Terrain& parent;
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;


};