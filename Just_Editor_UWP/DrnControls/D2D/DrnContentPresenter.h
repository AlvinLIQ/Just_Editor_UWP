//
// DrnContentPresenter.xaml.h
// Declaration of the DrnContentPresenter class
//

#pragma once

#include "DrnHeaders/DrnD2D.h"

namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnContentPresenter sealed: Windows::UI::Xaml::Controls::Panel
	{
	public:
		DrnContentPresenter();
		property Platform::String^ Content
		{
			Platform::String^ get()
			{
				return drnContent;
			}
			void set(Platform::String^ newStr)
			{
				drnContent = newStr;
				Draw(newStr->Data());
			}
		}
	private:
		void InitDrnContentPresenter();
		void Draw(const wchar_t* tStr, UINT32 color = D2D1::ColorF::White);
		int sWidth;
		int sHeight;

		RECT rect;
		POINT offset;

		Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget;
		Microsoft::WRL::ComPtr<IVirtualSurfaceImageSourceNative> surfaceNative;
		Platform::String^ drnContent = L"";
		Windows::UI::Composition::Visual^ drnVisual;

		void ContentPresenter_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
	};
}
