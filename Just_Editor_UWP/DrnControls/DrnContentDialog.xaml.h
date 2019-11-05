//
// DrnContentDialog.xaml.h
// Declaration of the DrnContentDialog class
//

#pragma once

#include "DrnControls/DrnContentDialog.g.h"


namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnContentDialog sealed
	{
	public:
		DrnContentDialog(Platform::String^ content, Platform::String^ Fst, Platform::String^ Sec, Platform::String^ Cls);
		property Just_Editor_UWP::DrnConfig^ AppConfig
		{
			Just_Editor_UWP::DrnConfig^ get()
			{
				return ((App^)App::Current)->AppConfig;
			}
		};
		property bool IsClosed;
	private:
		void ContentDialog_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ContentDialog_Closed(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogClosedEventArgs^ args);
	};
}
