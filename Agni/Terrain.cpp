#include "Terrain.h"
#include "TransformCBuf.h"
#include "DynamicVertexBuffer.h"
#include <cmath>
#include "ImGui/imgui.h"
#include <algorithm>
#include "ComputeShader.h"
namespace dx = DirectX;

Terrain::Terrain(Graphics& gfx)
{
    int resolution = std::max(1, res);

    float step = 2.0f / resolution;

    // Generate vertices
    for (int y = 0; y <= resolution; ++y)
    {
        for (int x = 0; x <= resolution; ++x)
        {
            float fx = -1.0f + step * x;
            float fy = -1.0f + step * y;
            dx::XMFLOAT3 pos = { fx, 0.0f, fy };
            float noise = 0;
            pos = { fx, 0.0f, fy };
            dx::XMVECTOR p = dx::XMLoadFloat3(&pos);
            p = dx::XMVectorScale(p, 10.0f);
            dx::XMStoreFloat3(&pos, p);
            //float normal
            vertices.push_back({
                pos,         // Position
                { 0.0f, 0.0f, 0.0f },     // Normal
                { 0.6f, 0.6f, 0.6f } // TexCoord
                });
        }
    }

    // Generate indices
    for (int y = 0; y < resolution; ++y)
    {
        for (int x = 0; x < resolution; ++x)
        {
            unsigned short i0 = y * (resolution + 1) + x;
            unsigned short i1 = i0 + 1;
            unsigned short i2 = i0 + resolution + 1;
            unsigned short i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint32_t i0 = indices[i];
        uint32_t i1 = indices[i + 1];
        uint32_t i2 = indices[i + 2];

        dx::XMVECTOR p0 = dx::XMLoadFloat3(&vertices[i0].pos);
        dx::XMVECTOR p1 = dx::XMLoadFloat3(&vertices[i1].pos);
        dx::XMVECTOR p2 = dx::XMLoadFloat3(&vertices[i2].pos);

        dx::XMVECTOR v1 = dx::XMVectorSubtract(p1, p0);
        dx::XMVECTOR v2 = dx::XMVectorSubtract(p2, p0);
        dx::XMVECTOR normal = dx::XMVector3Cross(v1, v2);
        normal = dx::XMVector3Normalize(normal);

        dx::XMFLOAT3 n;
        dx::XMStoreFloat3(&n, normal);

        // Accumulate normals to vertices
        vertices[i0].normal.x += n.x;
        vertices[i0].normal.y += n.y;
        vertices[i0].normal.z += n.z;

        vertices[i1].normal.x += n.x;
        vertices[i1].normal.y += n.y;
        vertices[i1].normal.z += n.z;

        vertices[i2].normal.x += n.x;
        vertices[i2].normal.y += n.y;
        vertices[i2].normal.z += n.z;
    }
    // Add bindings similar to Box class
    

    //AddBind(std::move(vertexBuffer));
    auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
    auto pvsbc = pvs->GetBytecode();
    AddBind(std::move(pvs));

    AddBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

    AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));


    // Input layout
    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
    AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    AddStaticBind(std::make_unique<Rasterizer>(gfx, D3D11_CULL_NONE));

    // Transform
    AddBind(std::make_unique<TransformCBuf>(gfx, *this));
    struct MaterialColorConstantBuffer {
        DirectX::XMFLOAT3 materialColor;
        float padding = 0.0f;
    };
    MaterialColorConstantBuffer mc = {
        {0.8f, 0.8f, 0.8f},
        0.0f
    };
    AddBind(std::make_unique<ComputeShader>(gfx, L"ComputeShader.cso", vertices));
    AddBind(std::make_unique<PixelConstantBuffer<MaterialColorConstantBuffer>>(gfx, mc, 1u));

    //auto buffer = std::make_unique<StructuredBuffer>(gfx, vertices, 0u);
    //buffer->BindUAV(gfx);
    //AddBind(std::move(buffer));

    //AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
    AddBind(std::make_unique<DynamicVertexBuffer>(gfx, *this));
}

void Terrain::Update(float dt) noexcept
{
    angle += 0.01f;
    //for (auto& v : vertices)
    //{
    //    float noise = FractalNoise(v.pos, noiseStrength, amp);
    //    v.pos.y = noise;
    //    v.normal = { 0.0f, 0.0f, 0.0f }; // Reset normals here
    //}
    //for (size_t i = 0; i < indices.size(); i += 3)
    //{
    //    uint32_t i0 = indices[i];
    //    uint32_t i1 = indices[i + 1];
    //    uint32_t i2 = indices[i + 2];
    //    
    //    dx::XMVECTOR p0 = dx::XMLoadFloat3(&vertices[i0].pos);
    //    dx::XMVECTOR p1 = dx::XMLoadFloat3(&vertices[i1].pos);
    //    dx::XMVECTOR p2 = dx::XMLoadFloat3(&vertices[i2].pos);

    //   
    //    



    //    //vertex normals

    //    dx::XMVECTOR v1 = dx::XMVectorSubtract(p1, p0);
    //    dx::XMVECTOR v2 = dx::XMVectorSubtract(p2, p0);
    //    dx::XMVECTOR normal = dx::XMVector3Cross(v1, v2);
    //    normal = dx::XMVector3Normalize(normal);

    //    dx::XMFLOAT3 n;
    //    dx::XMStoreFloat3(&n, normal);

    //    // Accumulate normals to vertices
    //    vertices[i0].normal.x += n.x;
    //    vertices[i0].normal.y += n.y;
    //    vertices[i0].normal.z += n.z;

    //    vertices[i1].normal.x += n.x;
    //    vertices[i1].normal.y += n.y;
    //    vertices[i1].normal.z += n.z;

    //    vertices[i2].normal.x += n.x;
    //    vertices[i2].normal.y += n.y;
    //    vertices[i2].normal.z += n.z;
    //}
}


DirectX::XMMATRIX Terrain::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixTranslation(0, 0, 0)*dx::XMMatrixRotationY(angle);
}

std::vector<Terrain::Vertex> Terrain::GetVertices() const
{
    return vertices;
}


void Terrain::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
 /*   x = pos.x;
    y = pos.y;
    z = pos.z;*/
}

void Terrain::SpawnControlWindow() noexcept
{
    bool opened = true;

    //ImGui::SetNextWindowSize(ImVec2(1280.0f / 6, 720.0 / 3));
    ImGui::SetNextWindowSize(ImVec2(1280.0f / 6, 720.0 / 3.80f));

    ImGui::SetNextWindowPos(ImVec2(0.0f, (720.0 / 3) + (720.0 / 5)));


    if (ImGui::Begin("Model"))
    {
        ImGui::SliderFloat("Test Value", &testValue, 0.0f, 5.0f);
        ImGui::SliderFloat("Amplitude", &amp, 0.0f, 5.0f);
        ImGui::SliderFloat("Noise X", &noiseStrength.x, 0.0f, 5.0f);
        ImGui::SliderFloat("Noise Y", &noiseStrength.y, 0.0f, 5.0f);
        ImGui::SliderFloat("Noise Z", &noiseStrength.z, 0.0f, 5.0f);

    }
    ImGui::End();


}
