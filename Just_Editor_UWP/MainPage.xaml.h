//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "DrnControls/DrnTabPanel.xaml.h"


namespace Just_Editor_UWP
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

		property Just_Editor_UWP::DrnConfig^ AppConfig
		{
			Just_Editor_UWP::DrnConfig^ get()
			{
				return ((App^)App::Current)->AppConfig;
			}
		};

		void ReadToEditor(Windows::Storage::StorageFile^ thisFile)
		{
			if (tabPanel != nullptr && thisFile != nullptr)
				tabPanel->HomePage_NewTabRequested(thisFile->Name, thisFile);
		}
	private:
		void BitmapImage_ImageFailed(Platform::Object^ sender, Windows::UI::Xaml::ExceptionRoutedEventArgs^ e);
		void tabPanel_Drop(Platform::Object^ sender, Windows::UI::Xaml::DragEventArgs^ e);
		void tabPanel_DragEnter(Platform::Object^ sender, Windows::UI::Xaml::DragEventArgs^ e);
	};
}
