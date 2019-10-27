//
// DrnMenu.xaml.cpp
// Implementation of the DrnMenu class
//

#include "pch.h"
#include "DrnMenu.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;


// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DrnMenu::DrnMenu()
{
	InitializeComponent();
//	menuItem = menuList->Items;
}


void Just_Editor_UWP::DrnMenu::menuList_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	if (UserMode)
	{
		MenuSelected(menuList->SelectedItem, e);
		menuList->SelectedItem = nullptr;
	}
	else
		this->Content = menuList->SelectedItem;

	this->Flyout->Hide();
}
