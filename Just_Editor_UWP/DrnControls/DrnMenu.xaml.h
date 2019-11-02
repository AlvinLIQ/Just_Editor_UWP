//
// DrnMenu.xaml.h
// Declaration of the DrnMenu class
//

#pragma once

#include "DrnControls/DrnMenu.g.h"

namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnMenu sealed
	{
	public:
		DrnMenu();

		event Windows::UI::Xaml::Controls::SelectionChangedEventHandler^ MenuSelected;

		property Windows::UI::Xaml::Controls::ItemCollection^ menuItems
		{
			Windows::UI::Xaml::Controls::ItemCollection^ get() { return menuList->Items; }
		};
		property int SelectedIndex
		{
			int get() { return menuList->SelectedIndex; }
			void set(int newIndex) 
			{
				menuList->SelectedIndex = newIndex;
			}
		};
		property bool IsCustom 
		{
			bool get() 
			{
				return UserMode;
			}
			void set(bool newStat)
			{
				UserMode = newStat;
			}
		};

	private:
		bool UserMode = false;
		void menuList_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
