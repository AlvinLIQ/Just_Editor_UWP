﻿//
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

		property Just_Editor_UWP::DrnConfig^ AppConfig
		{
			Just_Editor_UWP::DrnConfig^ get()
			{
				return ((App^)App::Current)->AppConfig;
			}
		};
		property Platform::String^ dialogTitle
		{
			Platform::String^ get() { return FileNameBox->Text; }
		};
		property Windows::Storage::StorageFile^ dialogFile;
		property bool IsClosed;
	private:
		Windows::Storage::StorageFolder^ newFolder = nullptr;

		void DialogClsBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ChangePathBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SaveBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ContentDialog_Closed(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogClosedEventArgs^ args);
		void ContentDialog_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
