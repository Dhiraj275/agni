// Texture.cpp
#include "Texture.h"
#include <DirectXTK/WICTextureLoader.h>
#include <stdexcept>
#pragma comment(lib, "DirectXTK.lib")


Texture::Texture(Graphics & gfx)
{
    DirectX::CreateWICTextureFromFile(
        GetDevice(gfx),
        GetContext(gfx),
        L"file.png",
        nullptr,
        &pTextureView
    );
}
