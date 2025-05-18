#pragma once
#include "DrawableBase.h"

class GeoSphere : public DrawableBase<GeoSphere> {
public:
    struct Vertex {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 normal;
    };
    GeoSphere(Graphics& gfx, float x, float y, float z, float radius);
    void Update(float dt) noexcept override;
    void Update(Graphics& gfx);
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
    std::vector<Vertex> GetVertices() const;
    void SetPos(DirectX::XMFLOAT3 pos) noexcept;
private:
    // Generate sphere vertices and indices
   
    std::vector<Vertex> vertices;
    std::vector<Vertex> baseVertices;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    float x;
    float y;
    float z;
    float radius;
    float speed;
    float angle;
};
