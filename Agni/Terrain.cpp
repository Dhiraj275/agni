#include "Terrain.h"
#include "TransformCBuf.h"
#include <cmath>
#include "ImGui/imgui.h"
#include <algorithm>
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
            dx::XMFLOAT3 color = { 0, 0.0f, 0};

            float noise = FractalNoise(pos,noiseStrength,amp);
            pos = { fx, noise, fy };
            dx::XMVECTOR p = dx::XMLoadFloat3(&pos);
            p = dx::XMVectorScale(p, 10.0f);
            dx::XMStoreFloat3(&pos, p);
            //float normal
            vertices.push_back({
                pos,         // Position
                { 0.0f, 0.0f, 0.0f },     // Normal
                color
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
    // Backup original data
    std::vector<Vertex> originalVertices = vertices;
    std::vector<uint32_t> originalIndices = indices;

    vertices.resize(originalIndices.size()); // One vertex per index
    indices.resize(originalIndices.size());  // 1:1 mapping

    for (size_t i = 0; i < originalIndices.size(); i += 3)
    {
        uint32_t i0 = originalIndices[i];
        uint32_t i1 = originalIndices[i + 1];
        uint32_t i2 = originalIndices[i + 2];

        dx::XMVECTOR p0 = dx::XMLoadFloat3(&originalVertices[i0].pos);
        dx::XMVECTOR p1 = dx::XMLoadFloat3(&originalVertices[i1].pos);
        dx::XMVECTOR p2 = dx::XMLoadFloat3(&originalVertices[i2].pos);

        dx::XMVECTOR edge1 = dx::XMVectorSubtract(p1, p0);
        dx::XMVECTOR edge2 = dx::XMVectorSubtract(p2, p0);
        dx::XMVECTOR normal = dx::XMVector3Normalize(dx::XMVector3Cross(edge1, edge2));

        dx::XMFLOAT3 faceNormal;
        dx::XMStoreFloat3(&faceNormal, normal);

        float height = (originalVertices[i0].pos.y + originalVertices[i1].pos.y + originalVertices[i2].pos.y) / 3.0f;

        dx::XMFLOAT3 color;
        if (height < 0.1f)         color = { 0.0f, 0.1f, 0.5f };   // Deep water
        else if (height < 1.2f)    color = { 0.2f, 0.5f, 0.8f };   // Shore
        else if (height < 5.5f)    color = { 0.1f, 0.6f, 0.2f };   // Grass
        else if (height < 7.5f)    color = { 0.4f, 0.3f, 0.2f };   // Rock/mountain
        else                      color = { 0.9f, 0.9f, 0.9f };   // Snow

        // Overwrite with new unique vertices and matching index
        size_t newBase = i;
        vertices[newBase + 0] = { originalVertices[i0].pos, faceNormal, color };
        vertices[newBase + 1] = { originalVertices[i1].pos, faceNormal, color };
        vertices[newBase + 2] = { originalVertices[i2].pos, faceNormal, color };

        indices[newBase + 0] = static_cast<uint32_t>(newBase + 0);
        indices[newBase + 1] = static_cast<uint32_t>(newBase + 1);
        indices[newBase + 2] = static_cast<uint32_t>(newBase + 2);
    }



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

    AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

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
    AddBind(std::make_unique<PixelConstantBuffer<MaterialColorConstantBuffer>>(gfx, mc, 1u));

}

void Terrain::Update(float dt) noexcept
{
}

void Terrain::Update(Graphics& gfx)noexcept {
    angle += 0.006;  
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
        ImGui::SliderFloat("Noise X", &frequency, 0.0f, 5.0f);
        /*ImGui::SliderFloat("Noise Y", &noiseStrength.y, 0.0f, 5.0f);
        ImGui::SliderFloat("Noise Z", &noiseStrength.z, 0.0f, 5.0f);*/

    }
    ImGui::End();


}
