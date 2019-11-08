//
// HomePage.xaml.h
// Declaration of the HomePage class
//

#pragma once

#include "HomePage.g.h"
#include "DrnControls/DrnMenu.xaml.h"
#include "DrnHeaders/Drn_UWP.h"

namespace Just_Editor_UWP
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public delegate void NewTabRequestEventHandler(Platform::String^ tabName, Windows::Storage::StorageFile^ tabFile);
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HomePage sealed
	{
	public:
		HomePage(DrnMenu^ mBtn);
		event NewTabRequestEventHandler^ NewTabRequested;
	private:
		DrnMenu^ menuBtn;
		void ContentPresenter_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void openText_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void newText_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void newSocket_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void openImage_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ContentPresenter_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void newCrypt_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void DrnMenu_MenuSelected(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
