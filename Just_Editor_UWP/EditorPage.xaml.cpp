//
// EditorPage.xaml.cpp
// Implementation of the EditorPage class
//

#include "pch.h"
#include "EditorPage.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

EditorPage::EditorPage(Windows::Storage::StorageFile^ tFile)
{
	InitializeComponent();
	thisFile = tFile;
	if (thisFile != nullptr)
	{
		concurrency::create_task(Drn_UWP::ReadTextStorageFile(thisFile)).then([this](String^ fileStr)
			{
				drnCodeEditor->AppendStr(fileStr);
				if (thisTab != nullptr)
					thisTab->SetExtraStatus(L"");

			});
	}
	drnCodeEditor->coreEditor->searchFlyout = searchFlyout;
}


void Just_Editor_UWP::EditorPage::saveBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (thisTab == nullptr || !saveBtn->IsEnabled)
		return;
	saveBtn->IsEnabled = false;

	if (thisFile == nullptr)
	{
		concurrency::create_task(Drn_UWP::PickAFolder()).then([this](Windows::Storage::StorageFolder^ saveFolder)
			{
				if (saveFolder == nullptr)
				{
					saveBtn->IsEnabled = true;
					concurrency::cancel_current_task();
				}
				else
					return saveFolder->CreateFileAsync(thisTab->Title, Windows::Storage::CreationCollisionOption::ReplaceExisting);
			}).then([this](Windows::Storage::StorageFile^ saveFile)
				{
					thisFile = saveFile;
					if (thisFile == nullptr)
						return;
					Drn_UWP::WriteTextStorageFile(thisFile, drnCodeEditor->GetStr());
					thisTab->SetExtraStatus(L"");
					thisTab->Title = thisFile->Name;
					saveBtn->IsEnabled = true;
				}, concurrency::task_continuation_context::use_current());
	}
	else
	{
		concurrency::create_task(Drn_UWP::WriteTextStorageFile(thisFile, drnCodeEditor->GetStr())).then([this]()
			{
				thisTab->SetExtraStatus(L"");
				saveBtn->IsEnabled = true;
			});
	}
}


void Just_Editor_UWP::EditorPage::drnCodeEditor_EditorTextChanged()
{
	if (thisTab == nullptr) //Can Set Status
		return;

	thisTab->SetExtraStatus(L"*");
}


void Just_Editor_UWP::EditorPage::drnCodeEditor_EditorSavedRequested()
{
	saveBtn_Click(nullptr, nullptr);
}


void Just_Editor_UWP::EditorPage::searchBox_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	e->Handled = true;
	if (e->Key == Windows::System::VirtualKey::Enter)
		findNextBtn_Click(nullptr, nullptr);
}


void Just_Editor_UWP::EditorPage::findLastBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (searchBox->Text != L"")
		drnCodeEditor->coreEditor->FindLastAndSelect(searchBox->Text);
}


void Just_Editor_UWP::EditorPage::findNextBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (searchBox->Text != L"")
	{
		drnCodeEditor->coreEditor->FindNextAndSelect(searchBox->Text);
//		searchBox->Focus(Windows::UI::Xaml::FocusState::Unfocused);
	}
}
