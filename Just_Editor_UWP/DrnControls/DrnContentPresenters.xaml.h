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
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnContentPresenters sealed : Windows::UI::Xaml::Controls::StackPanel
	{
	public:
		DrnContentPresenters();

		property Platform::String^ Content
		{
			Platform::String^ get()
			{
				/*
				unsigned int size = this->Children->Size;
				Platform::String^ result = L"";
				for (unsigned int i = 0; i < size; i++)
				{
					result += ((TXTCONTENT^)this->Children->GetAt(i))->Content;
				}

				return result;
				*/
				return textContent;
			}
			void set(Platform::String^ newStr)
			{
				textContent = newStr;
				Draw();
			}
		}
		property float cursorX;
	private:
		void Draw();
		void Init();

		Microsoft::WRL::ComPtr<IDXGISurface> surface;
		Microsoft::WRL::ComPtr<IVirtualSurfaceImageSourceNative> m_sisNative;
//		Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget;

		Windows::UI::Composition::Visual^ tVisual;

		DrnD2D* drnD2D = nullptr;

		Platform::String^ textContent;
		void OnSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
	};
}
