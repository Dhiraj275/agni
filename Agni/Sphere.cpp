#include "Sphere.h"
#include "TransformCBuf.h"
#include <cmath>
Sphere::Sphere(Graphics& gfx, float x, float y, float z, float radius)
    : x(x), y(y), z(z), radius(radius)
{
    // Generate sphere vertices and indices
    struct Vertex {
        struct {
            float x;
            float y;
            float z;
        } pos;
    };

    // Parameters for sphere generation
    const int latDiv =12;  // Latitude divisions
    const int longDiv = 24; // Longitude divisions

    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;

    // Reserve space
    vertices.reserve((latDiv + 1) * (longDiv + 1));

    // Generate vertices
    for (int i = 0; i <= latDiv; i++) {
        float lat = DirectX::XM_PI * float(i) / latDiv;
        float y = radius * cosf(lat);
        float ringRadius = radius * sinf(lat);

        for (int j = 0; j <= longDiv; j++) {
            float lon = 2.0f * DirectX::XM_PI * float(j) / longDiv;

            Vertex v;
            v.pos.x = ringRadius * cosf(lon);
            v.pos.y = y;
            v.pos.z = ringRadius * sinf(lon);

            vertices.push_back(v);
        }
    }

    // Generate indices
    indices.reserve(latDiv * longDiv * 6);
    for (int i = 0; i < latDiv; i++) {
        for (int j = 0; j < longDiv; j++) {
            int current = i * (longDiv + 1) + j;
            int next = current + 1;
            int bottom = current + (longDiv + 1);
            int bottomNext = bottom + 1;

            // Two triangles per quad
            indices.push_back(current);
            indices.push_back(bottom);
            indices.push_back(next);

            indices.push_back(next);
            indices.push_back(bottom);
            indices.push_back(bottomNext);
        }
    }

    // Add bindings similar to Box class
    AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

    auto pvs = std::make_unique<VertexShader>(gfx, L"SolidSphereVS.cso");
    auto pvsbc = pvs->GetBytecode();
    AddBind(std::move(pvs));

    AddBind(std::make_unique<PixelShader>(gfx, L"SolidSpherePS.cso"));

    AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

    // Color data
    struct ConstantBuffer2 {
        struct {
            float r;
            float g;
            float b;
            float a;
        } face_colors[6];
    };

    const ConstantBuffer2 cb2 = {
        {
            { 1.0f, 0.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f },
            { 1.0f, 1.0f, 0.0f },
            { 0.0f, 1.0f, 1.0f },
        }
    };

    AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

    // Input layout
    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
    AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    // Transform
    AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

void Sphere::Update(float dt) noexcept
{

}

DirectX::XMMATRIX Sphere::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixTranslation(x, y, z);
}