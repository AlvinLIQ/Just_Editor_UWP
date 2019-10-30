//
// DrnTabPanel.xaml.h
// Declaration of the DrnTabPanel class
//

#pragma once

#include "DrnControls/DrnTabPanel.g.h"
#include "DrnTab.xaml.h"
#include "EditorPage.xaml.h"
#include "HomePage.xaml.h"
#include "CryptPage.xaml.h"
#include "MsgPage.xaml.h"

#define TabWidth 180
#define RightMargin 230

namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnTabPanel sealed
	{
	public:
		DrnTabPanel();

		property Windows::UI::Xaml::Media::SolidColorBrush^ HoverBrush;
		property Windows::UI::Xaml::Media::SolidColorBrush^ SelectedColor;

		property DrnTab^ SelectedItem;

		void HomePage_NewTabRequested(Platform::String^ tabName, Windows::Storage::StorageFile^ tabFile);

		void AddNewTab(Platform::String^ title, Windows::UI::Xaml::UIElement^ content, Windows::Storage::StorageFile^ tabFile);
		void SelectAt(unsigned int page_n);
		void RemoveAt(unsigned int page_n);

	private:
		HomePage^ homePage;
		void drnBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void drnTab_Clicked(DrnTab^ tab);
		void drnTabClsBtn_Clicked(DrnTab^ tab);
		void Select(DrnTab^ targetTab, unsigned int tabIndex);
		void topPanel_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void tabScrollViewer_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void tabPanel_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void Grid_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
