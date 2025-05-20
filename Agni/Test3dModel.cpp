#include "Test3dModel.h"
#include "TransformCBuf.h"
#include "GraphicsThrowMacros.h"
#include <cmath>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ImGui/imgui.h"
float scale = 0.2f;
Model3d::Model3d(Graphics& gfx, float x, float y, float z, float speed, float angle, DirectX::XMFLOAT3 materialColor) :
	x(x),
	y(y),
	z(z),
	speed(speed),
	angle(angle),
	materialColor(materialColor)
{
    if (!IsStaticInitialized()) {
        struct Vertex {
            DirectX::XMFLOAT3 pos;
            DirectX::XMFLOAT3 n;
        };

        Assimp::Importer imp;

        const auto pModel = imp.ReadFile("Models\\nanosuit.obj",
            aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate
        );  // Generate normals if missing

        if (!pModel || !pModel->HasMeshes()) {
            throw std::runtime_error("Failed to load model or no meshes found.");
        }

     

        // Create and bind vertex buffer

        // Vertex shader
        auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        // Pixel shader
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));
        std::vector<Vertex> vertices;  // Use Vertex, not VertexBuffer
        std::vector<uint32_t> indices;
        unsigned int vertexOffset = 0;
        for (unsigned int m = 0; m < pModel->mNumMeshes; ++m) {
            const auto pMesh = pModel->mMeshes[m];
            for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
                aiVector3D pos = pMesh->mVertices[i];
                aiVector3D norm = pMesh->HasNormals() ? pMesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f);
                vertices.push_back(Vertex{
                    DirectX::XMFLOAT3{ pos.x * scale, pos.y * scale, pos.z * scale },
                    DirectX::XMFLOAT3{ norm.x, norm.y, norm.z }
                    });
            }

            for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
                const auto& face = pMesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(face.mIndices[j] + vertexOffset);
                }
            }
            vertexOffset += pMesh->mNumVertices;
        }
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
        };
        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
	else
	{
		SetIndexFromStatic();
	}
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
	struct MaterialColorConstantBuffer {
        DirectX::XMFLOAT3 materialColor ={1.0f, 0.0f, 0.0f };
        float padding;
	};
	MaterialColorConstantBuffer mc = {
        {1.0f, 0.0f, 0.0f },    
		0.0f,
	};
	AddBind(std::make_unique<PixelConstantBuffer<MaterialColorConstantBuffer>>(gfx, mc, 1u));
}

void Model3d::Update(float dt) noexcept
{
	angle += 1.0f * dt;
}
void Model3d::SpawnController() {
    bool opened = true;    
    ImGui::SetNextWindowSize(ImVec2(1280.0f / 6, 720.0 / 3.80f));

    ImGui::SetNextWindowPos(ImVec2(0.0f, (720.0 / 3) + (720.0 / 5)));


    if (ImGui::Begin("Model Controller", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
        ImGui::SliderFloat("X", &x, -10.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Y", &y, -10.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Z", &z, -10.0f, 10.0f, "%.2f");
        ImGui::Text("Rotation");
        ImGui::SliderAngle("Pitch", &pitch, -180, 180);
        ImGui::SliderAngle("Roll", &roll, -180, 180);
        ImGui::SliderAngle("Yaw", &yaw, -180, 180);
    }
    ImGui::End();

}
DirectX::XMMATRIX Model3d::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixRotationRollPitchYaw(roll,pitch,yaw)*
		DirectX::XMMatrixTranslation(x, y, z);
}