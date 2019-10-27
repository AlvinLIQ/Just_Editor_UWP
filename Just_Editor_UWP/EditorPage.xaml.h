//
// EditorPage.xaml.h
// Declaration of the EditorPage class
//

#pragma once

#include "EditorPage.g.h"
#include "DrnControls/DrnTab.xaml.h"
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
		EditorPage(Windows::Storage::StorageFile^ tFile);

		property Windows::Storage::StorageFile^ thisFile;
		property DrnTab^ thisTab;
		property DrnCodeEditor^ codeEditor
		{
			DrnCodeEditor^ get() { return drnCodeEditor; }
		};

		void drnCodeEditor_EditorSavedRequested();
	private:
		void saveBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void drnCodeEditor_EditorTextChanged();
		void searchBox_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
		void findLastBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void findNextBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
