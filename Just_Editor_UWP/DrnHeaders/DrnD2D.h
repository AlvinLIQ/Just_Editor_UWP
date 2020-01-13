#pragma once

#include <d2d1_3.h>
#include <d3d11_4.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <Windows.ui.xaml.media.dxinterop.h>
#include <iostream>

class DrnD2D
{
public:
	DrnD2D();
	~DrnD2D();

	Microsoft::WRL::ComPtr<ID2D1Factory1> d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext;
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;
	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	Microsoft::WRL::ComPtr<IDXGISwapChain> dxgiSwapChain;
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGISurface> dxgiSurface;

	D2D1_RENDER_TARGET_PROPERTIES d2dProperties;


	Microsoft::WRL::ComPtr<IDWriteFactory> wrtFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> txtFormat;
	Microsoft::WRL::ComPtr<IDWriteFontCollection> fonts;

	DWRITE_TEXT_METRICS GetWCharWidth(const wchar_t tWChar, IDWriteTextLayout** idwrtLayout = nullptr)
	{
		DWRITE_TEXT_METRICS textMetrics;
		wrtFactory->CreateTextLayout(&tWChar, 1, txtFormat.Get(), 0, 0, idwrtLayout);
		(*idwrtLayout)->GetMetrics(&textMetrics);

		return textMetrics;
	}

	DWRITE_TEXT_METRICS GetWCStrWidth(const wchar_t* tWCStr, UINT32 len = -1, IDWriteTextLayout** idwrtLayout = nullptr)
	{
		DWRITE_TEXT_METRICS textMetrics;
#ifdef max
#undef max
#endif
		if (len == -1)
			len = (UINT32)wcslen(tWCStr);
		wrtFactory->CreateTextLayout(tWCStr, len, txtFormat.Get(), std::numeric_limits<float>::max(), 0, idwrtLayout);
		(*idwrtLayout)->GetMetrics(&textMetrics);

		return textMetrics;
	}
private:
	void InitD2D();
};

