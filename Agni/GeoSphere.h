#pragma once
#include "DrawableBase.h"

class GeoSphere : public DrawableBase<GeoSphere> {
public:
    struct Vertex {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT3 color;
    };
    GeoSphere(Graphics& gfx, float x, float y, float z, float radius);
    void Update(float dt) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
    std::vector<Vertex> GetVertices() const;
    void SetPos(DirectX::XMFLOAT3 pos) noexcept;
    void SpawnControlWindow() noexcept;
private:
    // Generate sphere vertices and indices
   
    std::vector<Vertex> vertices;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    DirectX::XMFLOAT3 noiseStrength = {1,2,3};
    float amp = 0.5;
    float testValue= 0.0f;
    float x;
    float y;
    float z;
    float radius;
    float speed;
    float angle;
};
