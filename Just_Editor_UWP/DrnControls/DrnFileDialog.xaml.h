//
// DrnFileDialog.xaml.h
// Declaration of the DrnFileDialog class
//

#pragma once

#include "DrnControls/DrnFileDialog.g.h"
#include "DrnHeaders/Drn_UWP.h"

namespace Just_Editor_UWP
{
	public delegate void InfoChangedHandler(DrnFileDialog^ sender);
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnFileDialog sealed
	{
	public:
		DrnFileDialog(Platform::String^ dialogTitle, Platform::String^ fileName, Windows::Storage::StorageFile^ dFile);

		event InfoChangedHandler^ InfoChanged;

		property Platform::String^ dialogTitle
		{
			Platform::String^ get() { return FileNameBox->Text; }
		};
		property Windows::Storage::StorageFile^ dialogFile;
	private:
		Windows::Storage::StorageFolder^ newFolder = nullptr;

		void DialogClsBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ChangePathBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SaveBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
