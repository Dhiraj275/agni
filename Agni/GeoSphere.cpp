#include "GeoSphere.h"
#include "TransformCBuf.h"
#include "DynamicVertexBuffer.h"
#include <cmath>
namespace dx = DirectX;
DirectX::XMFLOAT3 Lerp(const dx::XMFLOAT3& a, const dx::XMFLOAT3& b, float t) {
    dx::XMVECTOR va = dx::XMLoadFloat3(&a);
    dx::XMVECTOR vb = dx::XMLoadFloat3(&b);
    dx::XMVECTOR result = dx::XMVectorLerp(va, vb, t);  // built-in for XMVECTOR
    dx::XMFLOAT3 out;
    dx::XMStoreFloat3(&out, result);
    return out;
}
GeoSphere::GeoSphere(Graphics& gfx, float x, float y, float z, float radius)
    : x(x), y(y), z(z), radius(radius)
{   
    std::vector<unsigned short> indices;
    
    static const std::vector<Vertex> verticesPair = {
    {{  0.0f,  1.0f,  0.0f }},  // 0: Top
    {{  1.0f,  0.0f,  0.0f }},  // 1: Right
    {{  0.0f,  0.0f,  1.0f }},  // 2: Front
    {{ -1.0f,  0.0f,  0.0f }},  // 3: Left
    {{  0.0f,  0.0f, -1.0f }},  // 4: Back
    {{  0.0f, -1.0f,  0.0f }}   // 5: Bottom
    };
    static std::vector<unsigned short> indicesPair = {
     0, 1, 2,  // Top front-right
     0, 2, 3,  // Top front-left
     0, 3, 4,  // Top back-left
     0, 4, 1,  // Top back-right

     5, 2, 1,  // Bottom front-right
     5, 3, 2,  // Bottom front-left
     5, 4, 3,  // Bottom back-left
     5, 1, 4   // Bottom back-right
    };
    
    int N = 15;

    for (int triangle = 0; triangle < indicesPair.size(); triangle += 3) {
        dx::XMFLOAT3 v0 = verticesPair[indicesPair[triangle]].pos;
        dx::XMFLOAT3 v1 = verticesPair[indicesPair[triangle + 1]].pos;
        dx::XMFLOAT3 v2 = verticesPair[indicesPair[triangle + 2]].pos;

        // Move this line INSIDE the loop
        std::vector<std::vector<unsigned short>> indexGrid(N + 1);

        for (int i = 0; i <= N; ++i) {
            float t = (float)i / N;
            dx::XMFLOAT3 left = Lerp(v0, v2, t);
            dx::XMFLOAT3 right = Lerp(v0, v1, t);

            for (int j = 0; j <= i; ++j) {
                float s = (i == 0) ? 0.0f : (float)j / i;
                dx::XMFLOAT3 point = Lerp(left, right, s);
                // Normalize to push onto sphere
                dx::XMVECTOR p = dx::XMLoadFloat3(&point);
              
                p = dx::XMVectorScale(dx::XMVector3Normalize(p), radius);
                dx::XMStoreFloat3(&point, p);

                dx::XMFLOAT3 normal;
                dx::XMStoreFloat3(&normal, p);

                vertices.push_back({ point, normal });
                baseVertices.push_back({ point, normal });

                unsigned short idx = static_cast<unsigned short>(vertices.size() - 1);
                indexGrid[i].push_back(idx);
            }
        }

        // Build indices from the triangle fan
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < i; ++j) {
                // Upper triangle
                indices.push_back(indexGrid[i][j]);
                indices.push_back(indexGrid[i + 1][j]);
                indices.push_back(indexGrid[i + 1][j + 1]);

                // Lower triangle
                indices.push_back(indexGrid[i][j]);
                indices.push_back(indexGrid[i + 1][j + 1]);
                indices.push_back(indexGrid[i][j + 1]);
            }

            // Last triangle in row
            indices.push_back(indexGrid[i][i]);
            indices.push_back(indexGrid[i + 1][i]);
            indices.push_back(indexGrid[i + 1][i + 1]);
        }
    }
    // Add bindings similar to Box class
    vertexBuffer = std::make_unique<VertexBuffer>(gfx, vertices);
    AddBind(std::move(vertexBuffer));
    auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
    auto pvsbc = pvs->GetBytecode();
    AddBind(std::move(pvs));

    AddBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

    AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));


    // Input layout
    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
    AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    AddStaticBind(std::make_unique<Rasterizer>(gfx,D3D11_CULL_NONE));

    // Transform
    AddBind(std::make_unique<TransformCBuf>(gfx, *this));
    struct MaterialColorConstantBuffer {
        DirectX::XMFLOAT3 materialColor;
        float padding = 0.0f;
    };
    MaterialColorConstantBuffer mc = {
        {1.0f, 0.0f, 0.0f},
        0.0f
    };
    AddBind(std::make_unique<PixelConstantBuffer<MaterialColorConstantBuffer>>(gfx, mc, 1u));
    //AddBind(std::make_unique<ComputeShader>(gfx, L"ComputeShader.cso"));
    
    //auto buffer = std::make_unique<StructuredBuffer>(gfx, vertices, 0u);
    //buffer->BindUAV(gfx);
    //AddBind(std::move(buffer));

}

void GeoSphere::Update(float dt) noexcept
{
    for (auto& v : vertices)
    {
        // Load the position as an XMVECTOR
        dx::XMVECTOR p = dx::XMLoadFloat3(&v.pos);

        // Compute the height based on the y coordinate
        float height = 1.0f + sinf(v.pos.y * 25.0f) * 0.05f;

        // Scale the normalized position vector by radius * height
        p = dx::XMVectorScale(dx::XMVector3Normalize(p), radius * height);

        // Store the result back into the vertex
        dx::XMStoreFloat3(&v.pos, p);
    }
}
void GeoSphere::Update(Graphics&gfx)
{
    
}

DirectX::XMMATRIX GeoSphere::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixTranslation(x, y, z);
}

std::vector<GeoSphere::Vertex> GeoSphere::GetVertices() const
{
    return vertices;
}


void GeoSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
    x = pos.x;
    y = pos.y;
    z = pos.z;
}
