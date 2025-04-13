#include "Graphics.h"
#include <iostream>
#include <exception>
#include <sstream>
#include <d3dcompiler.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

namespace wrl = Microsoft::WRL;

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )

#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif



Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height= 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator= 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality= 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.Windowed = TRUE;
	sd.OutputWindow = hWnd;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;

	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));
	// gain access to texture subresource in swap chain (back buffer)

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	);
}




void Graphics::DrawTestTriangle()
{
	namespace wrl = Microsoft::WRL;
	
	HRESULT hr;

	struct Vertex
	{
		float x;
		float y;
		float r;
		float g;
		float b;

	};

	//vertex buffer 
	const Vertex vertices[] =
	{
		{ 0.0f,0.5f, 1.0f,0.0f,0.0f },
		{ 0.5f,-0.5f, 0.0f,1.0f,0.0f },
		{ -0.5f,-0.5f, 0.0f,0.0f,1.0f },
	};
	
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	
	//vertex buffer description
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof( vertices );
	bd.StructureByteStride = sizeof( Vertex );
	//vertex buffer sub resource
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	

	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	
	//blob
	wrl::ComPtr<ID3DBlob> pBlob;
	//vertex shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	//binding vertex shader to the pipe line
	pContext->PSSetShader(pPixelShader.Get(), 0, 0);

	//vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),nullptr, &pVertexShader ));
	//binding vertex shader to the pipe line
	pContext->VSSetShader(pVertexShader.Get(), 0, 0);


	//input layout
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0, DXGI_FORMAT_R32G32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color",0, DXGI_FORMAT_R32G32B32_FLOAT, 0,8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	GFX_THROW_INFO(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	pContext->IASetInputLayout(pInputLayout.Get());


	//render target
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	//set premitive topology to TRIANGLELIST
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//view port desc
	D3D11_VIEWPORT vp;
	vp.Height = 720;
	vp.Width= 1280;
	vp.MinDepth= 0;
	vp.MaxDepth= 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewports(1u, &vp);

	GFX_THROW_INFO_ONLY(pContext->Draw((UINT)std::size(vertices), 0u));
}


void Graphics::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}



// Graphics exception stuff
Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}


const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Agni Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}


std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	wchar_t* errorMsg = nullptr;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM| FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		0,
		(LPWSTR)&errorMsg,
		0,
		nullptr
	);

	std::wstring wresult = errorMsg ? errorMsg : L"Unknown error";
	std::string result(wresult.begin(), wresult.end());
	return result;
	return result;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Agni Graphics Device Removed Exception";
}

//info exception
Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}
std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const char* Graphics::InfoException::GetType() const noexcept
{
	return "Agni Graphics Info Exception";
}
