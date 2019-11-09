//
// EditorPage.xaml.cpp
// Implementation of the EditorPage class
//

#include "pch.h"
#include "EditorPage.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

EditorPage::EditorPage()
{
	InitializeComponent();

	drnCodeEditor->coreEditor->searchFlyout = searchFlyout;
}

void Just_Editor_UWP::EditorPage::UpdateEditor()
{
	if (thisTab != nullptr && thisTab->FileDialog != nullptr && thisTab->FileDialog->DialogFile != nullptr)
	{
		concurrency::create_task(Drn_UWP::ReadTextStorageFile(thisTab->FileDialog->DialogFile)).then([this](Platform::String^ fileStr)
			{
				drnCodeEditor->AppendStr(fileStr);
				thisTab->SetExtraStatus(L"");

			});
	}
}


void Just_Editor_UWP::EditorPage::saveBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (thisTab == nullptr || thisTab->FileDialog == nullptr || !saveBtn->IsEnabled)
		return;
	saveBtn->IsEnabled = false;

	if (thisTab->FileDialog->DialogFile == nullptr)
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
					thisTab->FileDialog->DialogFile = saveFile;
					if (thisTab->FileDialog->DialogFile == nullptr)
						return;
					Drn_UWP::WriteTextStorageFile(thisTab->FileDialog->DialogFile, drnCodeEditor->GetStr());
					thisTab->SetExtraStatus(L"");
					thisTab->Title = thisTab->FileDialog->DialogFile->Name;
					saveBtn->IsEnabled = true;
				}, concurrency::task_continuation_context::use_current());
	}
	else
	{
		concurrency::create_task(Drn_UWP::WriteTextStorageFile(thisTab->FileDialog->DialogFile, drnCodeEditor->GetStr())).then([this]()
			{
				thisTab->SetExtraStatus(L"");
				thisTab->FileDialog->FileModifiedTime = thisTab->FileDialog->DialogFile->DateCreated.UniversalTime;
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
	else if (e->Key == Windows::System::VirtualKey::Escape)
		searchFlyout->Hide();
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


void Just_Editor_UWP::EditorPage::UndoBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void Just_Editor_UWP::EditorPage::RedoBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
