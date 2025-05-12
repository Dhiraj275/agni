// Texture.cpp
#include "Texture.h"
#include <DirectXTK/WICTextureLoader.h>
#include <stdexcept>
#pragma comment(lib, "DirectXTK.lib")


Texture::Texture(Graphics & gfx,const wchar_t * fileName)
{
    INFOMAN(gfx);

    GFX_THROW_INFO(DirectX::CreateWICTextureFromFile(
        GetDevice(gfx),
        GetContext(gfx),
        fileName,
        nullptr,
        &pTextureView
    ));
}

void Texture::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}

