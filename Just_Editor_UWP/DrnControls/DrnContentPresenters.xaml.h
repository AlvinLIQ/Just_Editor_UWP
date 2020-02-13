//
// DrnContentPresenters.xaml.h
// Declaration of the DrnContentPresenters class
//

#pragma once

#include "DrnHeaders/Drn_UWP.h"
#include "DrnHeaders/DrnD2D.h"

#define TXTCONTENT Windows::UI::Xaml::Controls::ContentPresenter

namespace Just_Editor_UWP
{
	public ref class DrnContentPrensenter sealed
	{
	public:
		DrnContentPrensenter()
		{

		}

		property Platform::String^ Content
		{
			Platform::String^ get()
			{
				return content;
			}
			void set(Platform::String^ newStr)
			{
				content = newStr;
			}
		};
	private:
		Platform::String^ content;
	};
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnContentPresenters sealed : Windows::UI::Xaml::FrameworkElement
	{
	public:
		DrnContentPresenters();

		property Windows::Foundation::Collections::IVector<DrnContentPrensenter^>^ Items
		{
			Windows::Foundation::Collections::IVector<DrnContentPrensenter^>^ get()
			{
				return items;
			}
		};
		property float cursorX;
	private:
		void Draw();
		void Init();

		Microsoft::WRL::ComPtr<IDXGISurface> surface;
		Microsoft::WRL::ComPtr<IVirtualSurfaceImageSourceNative> m_sisNative;
//		Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget;

		Windows::UI::Composition::Visual^ tVisual;

		Windows::Foundation::Collections::IVector<DrnContentPrensenter^>^ items;

		DrnD2D* drnD2D = nullptr;

		Platform::String^ textContent;
		void OnSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
	};
}
