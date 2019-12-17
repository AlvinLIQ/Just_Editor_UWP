//
// EditorPage.xaml.cpp
// Implementation of the EditorPage class
//

#include "pch.h"
#include "EditorPage.xaml.h"

using namespace Just_Editor_UWP;

using namespace concurrency;

using namespace Platform;


// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

EditorPage::EditorPage()
{
	InitializeComponent();

	drnCodeEditor->coreEditor->searchFlyout = searchFlyout;

	Windows::UI::Core::CoreWindow::GetForCurrentThread()->Activated
		+= ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::WindowActivatedEventArgs^>
		([this](Windows::UI::Core::CoreWindow^ coreWindow, Windows::UI::Core::WindowActivatedEventArgs^ args)
	{
		if (this->thisTab == nullptr || this->thisTab->FileDialog == nullptr || this->thisTab->FileDialog->DialogFile == nullptr)
			return;

		/*
		auto dataModifiedProperty = ref new Platform::Collections::Vector<Platform::String^>({ L"System.DateModified" });

		concurrency::create_task(FileDialog->DialogFile->Properties->RetrievePropertiesAsync(dataModifiedProperty)).then([FileDialog](Windows::Foundation::Collections::IMap<String^, Object^>^ extraProperties)
		{

		});
		*/
		create_task(thisTab->FileDialog->DialogFile->GetBasicPropertiesAsync()).then([FileDialog = thisTab->FileDialog, this, codeEditor = codeEditor](Windows::Storage::FileProperties::BasicProperties^ basicProperties)
			{
				if (basicProperties->DateModified.UniversalTime != FileDialog->FileModifiedTime)
				{
					reloadDialog->Show();
					FileDialog->FileModifiedTime = basicProperties->DateModified.UniversalTime;
				}
			});
	});
}

void Just_Editor_UWP::EditorPage::UpdateEditor(Windows::Storage::Streams::UnicodeEncoding encodeMode)
{
	if (thisTab != nullptr && thisTab->FileDialog != nullptr && thisTab->FileDialog->DialogFile != nullptr)
	{
		concurrency::create_task(Drn_UWP::ReadTextStorageFile(thisTab->FileDialog->DialogFile, encodeMode)).then([this](Platform::String^ fileStr)
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
					thisTab->FileDialog->UpdateFileProperties();
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
				thisTab->FileDialog->UpdateFileProperties();
				saveBtn->IsEnabled = true;
			});
	}
}

void Just_Editor_UWP::EditorPage::drnCodeEditor_EditorActionChanged()
{
	UndoBtn->IsEnabled = drnCodeEditor->coreEditor->CanUndo;
	RedoBtn->IsEnabled = drnCodeEditor->coreEditor->CanRedo;
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
	if (drnCodeEditor->coreEditor->CanUndo)
	{
		drnCodeEditor->coreEditor->Undo();
	}
}


void Just_Editor_UWP::EditorPage::RedoBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (drnCodeEditor->coreEditor->CanRedo)
	{
		drnCodeEditor->coreEditor->Redo();
	}
}


void Just_Editor_UWP::EditorPage::drnCodeEditor_EditorReloadRequested(Windows::Storage::Streams::UnicodeEncoding encodeMode)
{
	if (thisTab == nullptr || thisTab->FileDialog == nullptr || thisTab->FileDialog->DialogFile == nullptr)
		return;

	drnCodeEditor->Clear();
	UpdateEditor(encodeMode);
}


void Just_Editor_UWP::EditorPage::reloadDialog_PrmBtnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	codeEditor->Clear();
	UpdateEditor(Windows::Storage::Streams::UnicodeEncoding::Utf8);
}
