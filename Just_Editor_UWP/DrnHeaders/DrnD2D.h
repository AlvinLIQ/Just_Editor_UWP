#pragma once

#include <d2d1.h>
#include <d2d1helper.h>
#include <d3d11.h>
#include <dwrite.h>
#include <wincodec.h>
#include <Windows.ui.xaml.media.dxinterop.h>

class DrnD2D
{
public:
	DrnD2D();
	~DrnD2D();

	void InitD2D();

	Microsoft::WRL::ComPtr<ID2D1Factory> m_d2dFactory;
	Microsoft::WRL::ComPtr<IDXGIDevice> m_d2dDevice;
	ID2D1Factory* factory;
	Microsoft::WRL::ComPtr<IDXGISurface> m_d2dSurface;

	D2D1_RENDER_TARGET_PROPERTIES d2dProperties;

	IDWriteFactory* wrtFactory;
	IDWriteTextFormat* txtFormat;


	DWRITE_TEXT_METRICS GetWCharWidth(const wchar_t tWChar)
	{
		DWRITE_TEXT_METRICS textMetrics;
		IDWriteTextLayout* idwrtLayout;
		wrtFactory->CreateTextLayout(&tWChar, 1, txtFormat, 0, 0, &idwrtLayout);
		idwrtLayout->GetMetrics(&textMetrics);

		delete[] idwrtLayout;
		return textMetrics;
	}
	DWRITE_TEXT_METRICS GetWCStrWidth(const wchar_t* tWCStr, UINT32 len = -1)
	{
		DWRITE_TEXT_METRICS textMetrics;
		IDWriteTextLayout* idwrtLayout;
#ifdef max
#undef max
#endif
		if (len == -1)
			len = (UINT32)wcslen(tWCStr);
		wrtFactory->CreateTextLayout(tWCStr, len, txtFormat, std::numeric_limits<float>::max(), 0, &idwrtLayout);
		idwrtLayout->GetMetrics(&textMetrics);

		delete[] idwrtLayout;
		return textMetrics;
	}
private:
};

