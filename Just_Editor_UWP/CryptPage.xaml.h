//
// CryptPage.xaml.h
// Declaration of the CryptPage class
//

#pragma once

#include "CryptPage.g.h"

namespace Just_Editor_UWP
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class CryptPage sealed
	{
	public:
		CryptPage();
	private:
		void enBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void deBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
