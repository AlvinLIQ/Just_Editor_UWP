//
// MsgPage.xaml.h
// Declaration of the MsgPage class
//

#pragma once

#include "MsgPage.g.h"
#include "DrnControls/DrnCodeEditor.xaml.h"
#include "DrnHeaders/Drn_Tools.h"

namespace Just_Editor_UWP
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MsgPage sealed
	{
	public:
		MsgPage();
	private:
		SOCKET socket_fd;
		bool isConnected = false;

		void connBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void sendBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void clearBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
