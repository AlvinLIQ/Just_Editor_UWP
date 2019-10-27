//
// DrnKeyboard.xaml.h
// Declaration of the DrnKeyboard class
//

#pragma once

#include "DrnControls/DrnKeyboard.g.h"
#include "DrnHeaders/Drn_UWP.h"

const wchar_t drnKeyMap[] = L"`1234567890-=\nqwertyuiop\nasdfghjkl\nzxcvbnm[]\n,. ;\n";
const wchar_t drnShiftKeyMap[] = L"~!@#$%^&*()-=\nQWERTYUIOP\nASDFGHJKL\nZXCVBNM{}\n<> :\n";
const wchar_t drnKeyCodeMap[] = L"_\t1234567890-=_QWERTYUIOP[]_ASDFGHJKL;'`_ZXCVBNM,./___ ___________________________________________________________";
const wchar_t drnShiftKeyCodeMap[] = L"_\t!@#$%^&*()_+_QWERTYUIOP{}_ASDFGHJKL:\"~_ZXCVBNM<>?___ ___________________________________________________________";


namespace Just_Editor_UWP
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public delegate void drnKeyEventHandler(unsigned int keyCode);
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnKeyboard sealed
	{
	public:
		DrnKeyboard();
		event drnKeyEventHandler^ drnKeyDown;
		event drnKeyEventHandler^ drnKeyUp;
		void Show();
		void Hide();
	private:
		Windows::Foundation::EventRegistrationToken eventToken;
		void InitKey();
		void keyGrid_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void keyBtn_Clicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void keyGrid_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnBackRequested(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ args);
	};
}
