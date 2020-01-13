#include <pch.h>
#include "DrnD2D.h"


using namespace Microsoft::WRL;

DrnD2D::DrnD2D()
{
	InitD2D();
}

DrnD2D::~DrnD2D()
{
	d2dFactory.Reset();
	d2dDevice.Reset();
    dxgiSurface.Reset();
}

void DrnD2D::InitD2D()
{
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };
    D3D_FEATURE_LEVEL m_featureLevels;
    D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE,
        0, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        featureLevels, ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION, d3dDevice.GetAddressOf(),
        &m_featureLevels, d3dContext.GetAddressOf());

    d3dDevice.As(&dxgiDevice);
    dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
    dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactory.GetAddressOf());
    d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());
    swapChainDesc = {};
    swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all apps must use this SwapEffect
    swapChainDesc.Flags = 0;
    dxgiFactory->CreateSwapChain(d3dDevice.Get(), &swapChainDesc, dxgiSwapChain.GetAddressOf());
    dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiSurface));
    d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2dContext.GetAddressOf());


	D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_PREMULTIPLIED
	);
	d2dProperties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, pixelFormat);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(wrtFactory), reinterpret_cast<IUnknown**>(wrtFactory.GetAddressOf()));
    wrtFactory->GetSystemFontCollection(&fonts);
	wrtFactory->CreateTextFormat(L"Source Code Variable", fonts.Get(), DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18, L"", &txtFormat);

}