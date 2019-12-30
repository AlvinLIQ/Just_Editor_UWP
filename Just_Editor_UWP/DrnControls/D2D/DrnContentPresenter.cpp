//
// DrnContentPresenter.xaml.cpp
// Implementation of the DrnContentPresenter class
//

#include "pch.h"
#include "DrnContentPresenter.h"
#include <windowsnumerics.h>

DrnD2D* drnD2D = nullptr;

using namespace Just_Editor_UWP;

using namespace Microsoft::WRL;

using namespace Platform;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DrnContentPresenter::DrnContentPresenter()
{
	if (drnD2D == nullptr)
		drnD2D = new DrnD2D;
	this->Height = 23;
	this->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
	drnVisual = ElementCompositionPreview::GetElementVisual(this);
	this->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &DrnContentPresenter::ContentPresenter_SizeChanged);
}

void DrnContentPresenter::InitDrnContentPresenter()
{
	if (surfaceNative == NULL)
	{
		auto surfaceSource = ref new VirtualSurfaceImageSource((int)this->ActualWidth, (int)this->ActualHeight);
		IInspectable* sisInspectable = (IInspectable*) reinterpret_cast<IInspectable*>(surfaceSource);
		sisInspectable->QueryInterface(__uuidof(IVirtualSurfaceImageSourceNative), (void**)&surfaceNative);

		auto ib = ref new ImageBrush;
		ib->ImageSource = surfaceSource;
		ib->Stretch = Stretch::None;
		ib->AlignmentX = AlignmentX::Left;
		ib->AlignmentY = AlignmentY::Top;

		this->Background = ib;

		surfaceNative->SetDevice(drnD2D->m_d2dDevice.Get());
	}
	else
	{
		surfaceNative->Resize((int)this->ActualWidth, (int)this->ActualHeight);
	}
}

void DrnContentPresenter::Draw(const wchar_t* tStr, UINT32 color)
{
	rect = { 0,0, (int)this->RenderSize.Width, (int)this->RenderSize.Height };
	surfaceNative->BeginDraw(rect, drnD2D->m_d2dSurface.GetAddressOf(), &offset);

	drnD2D->factory->CreateDxgiSurfaceRenderTarget(drnD2D->m_d2dSurface.Get(), drnD2D->d2dProperties, renderTarget.GetAddressOf());
	auto rt = renderTarget.Get();
	rt->BeginDraw();
	D2D1_RECT_F rectangle2 = D2D1::RectF(
		0,
		0,
		this->RenderSize.Width,
		this->RenderSize.Height
	);

	ID2D1SolidColorBrush* colorBrush;
	renderTarget->Clear();
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(color, 1.0f), &colorBrush);
//	rt->DrawText(tStr, wcslen(tStr), txtFormat, rectangle2, colorBrush);

	IDWriteTextLayout* idwrtLayout;
	drnD2D->wrtFactory->CreateTextLayout(tStr, (UINT32)wcslen(tStr), drnD2D->txtFormat, std::numeric_limits<float>::max(), 0, &idwrtLayout);

	rt->DrawTextLayout({ 0.0f,0.0f }, idwrtLayout, colorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
	rt->EndDraw();
	surfaceNative->EndDraw();
}


void DrnContentPresenter::ContentPresenter_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	if ((int)e->NewSize.Width > sWidth || (int)e->NewSize.Height > sHeight)
	{
		sWidth = (int)e->NewSize.Width;
		sHeight = (int)e->NewSize.Height;
		InitDrnContentPresenter();
		Draw(drnContent->Data());
	}
}
