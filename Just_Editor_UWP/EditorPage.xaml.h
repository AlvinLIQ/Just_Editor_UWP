//
// EditorPage.xaml.h
// Declaration of the EditorPage class
//

#pragma once

#include "EditorPage.g.h"
#include "DrnControls/DrnTab.xaml.h"
#include "DrnControls/DrnNotification.xaml.h"
#include "DrnControls/DrnCodeEditor.xaml.h"


namespace Just_Editor_UWP
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class EditorPage sealed
	{
	public:
		EditorPage();

		property Just_Editor_UWP::DrnConfig^ AppConfig
		{
			Just_Editor_UWP::DrnConfig^ get()
			{
				return ((App^)App::Current)->AppConfig;
			}
		};

		property DrnTab^ ThisTab
		{
			DrnTab^ get()
			{
				return this->thisTab;
			}
			void set(DrnTab^ newTab)
			{
				this->thisTab = newTab;	

				UpdateEditor(Windows::Storage::Streams::UnicodeEncoding::Utf8);
			}
		}
		property DrnCodeEditor^ codeEditor
		{
			DrnCodeEditor^ get() { return drnCodeEditor; }
		};

		void UpdateEditor(Windows::Storage::Streams::UnicodeEncoding encodeMode);
		void drnCodeEditor_EditorSavedRequested();
	private:
		DrnTab^ thisTab = nullptr;

		void saveBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void drnCodeEditor_EditorActionChanged();
		void drnCodeEditor_EditorTextChanged();
		void searchBox_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
		void findLastBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void findNextBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void UndoBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void RedoBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void drnCodeEditor_EditorReloadRequested(Windows::Storage::Streams::UnicodeEncoding encodeMode);
		void reloadDialog_PrmBtnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
