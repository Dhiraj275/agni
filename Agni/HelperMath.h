#pragma once
#include <DirectXMath.h>
#include "PerlinNoise.h"
DirectX::XMFLOAT3 LerpColor(const DirectX::XMFLOAT3& c1, const DirectX::XMFLOAT3& c2, float t)
{
    return {
        c1.x + (c2.x - c1.x) * t,
        c1.y + (c2.y - c1.y) * t,
        c1.z + (c2.z - c1.z) * t
    };
}
DirectX::XMFLOAT3 Lerp(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, float t) {
    namespace dx = DirectX;
    dx::XMVECTOR va = dx::XMLoadFloat3(&a);
    dx::XMVECTOR vb = dx::XMLoadFloat3(&b);
    dx::XMVECTOR result = dx::XMVectorLerp(va, vb, t);  // built-in for XMVECTOR
    dx::XMFLOAT3 out;
    dx::XMStoreFloat3(&out, result);
    return out;
}
