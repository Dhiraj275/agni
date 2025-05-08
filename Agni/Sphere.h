#pragma once
#include "DrawableBase.h"

class Sphere : public DrawableBase<Sphere> {
public:
    Sphere(Graphics& gfx, float x, float y, float z, float radius);
    void Update(float dt) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
    void SetPos(DirectX::XMFLOAT3 pos) noexcept;
private:
    float x;
    float y;
    float z;
    float radius;
    float speed;
    float angle;
};
