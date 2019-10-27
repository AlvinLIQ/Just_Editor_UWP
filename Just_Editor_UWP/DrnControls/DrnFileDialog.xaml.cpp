//
// DrnFileDialog.xaml.cpp
// Implementation of the DrnFileDialog class
//

#include "pch.h"
#include "DrnFileDialog.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;
using namespace concurrency;

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

Just_Editor_UWP::DrnFileDialog::DrnFileDialog(Platform::String^ dialogTitle, Platform::String^ fileName, Windows::Storage::StorageFile^ dFile)
{
	InitializeComponent();
	TitleBlock->Text = dialogTitle;
	FileNameBox->Text = fileName;
	dialogFile = dFile;
	FilePathBlock->Text = dFile == nullptr ? L"(None)" : dFile->Path;
}



void Just_Editor_UWP::DrnFileDialog::DialogClsBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Hide();
}


void Just_Editor_UWP::DrnFileDialog::ChangePathBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	create_task(Drn_UWP::PickAFolder()).then([this](Windows::Storage::StorageFolder^ nFolder) 
		{
			if (nFolder == nullptr)
				return;
			newFolder = nFolder;
			FilePathBlock->Text = newFolder->Path;
		});
}


void Just_Editor_UWP::DrnFileDialog::SaveBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (newFolder != nullptr)
	{
		create_task(newFolder->CreateFileAsync(FileNameBox->Text, Windows::Storage::CreationCollisionOption::ReplaceExisting)).then([this](Windows::Storage::StorageFile^ newFile)
			{
				if (newFile == nullptr)
					return;
				if (dialogFile != nullptr)
					dialogFile->CopyAndReplaceAsync(newFile);

				dialogFile = newFile;
				InfoChanged(this);
			});
	}
	else if (dialogFile != nullptr && dialogFile->Name != FileNameBox->Text)
		create_task(dialogFile->RenameAsync(FileNameBox->Text)).then([this](task<void> changeNameTask) 
			{
				try
				{
					changeNameTask.get();
					InfoChanged(this);
				}
				catch (COMException^ ex)
				{
					Drn_UWP::ShowMessageBox(L"Rename Failed", L"Error Code:" + ex->HResult.ToString() + L"\n" + ex->Message);
				}
			}, task_continuation_context::use_current());
	else
		InfoChanged(this);

	this->Hide();
}
