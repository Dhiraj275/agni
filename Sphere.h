#pragma once
#include "Drawable.h"

class Sphere : public Drawable {
public:
    Sphere(Graphics& gfx, float x, float y, float z, float radius, float speed, float angle);
    void Update(float dt) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
    float x;
    float y;
    float z;
    float radius;
    float speed;
    float angle;
};
