//
// HomePage.xaml.cpp
// Implementation of the HomePage class
//

#include "pch.h"
#include "HomePage.xaml.h"

using namespace Just_Editor_UWP;

using namespace concurrency;

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;


// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

HomePage::HomePage(DrnMenu^ mBtn)
{
	InitializeComponent();
	menuBtn = mBtn;
	menuBtn->MenuSelected 
		+= ref new SelectionChangedEventHandler([this](Object^ sender, SelectionChangedEventArgs^ e)
		{
				switch (menuBtn->SelectedIndex)
				{
				case 0:
					newSocket_Click(sender, nullptr);
					break;
				case 1:
					openText_Click(sender, nullptr);
					break;
				case 2:
					App::Current->Exit();
				//	exit(0);
					break;
				}
		});
}


void Just_Editor_UWP::HomePage::ContentPresenter_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto senderElement = (UIElement^)sender;
	senderElement->Opacity = 0;
	menuBtn->Width = 0;
	menuBtn->IsEnabled = false;
	Drn_UWP::DrnAnimeY(Hosting::ElementCompositionPreview::GetElementVisual(senderElement), 30, 0, 30, senderElement);
}

void Just_Editor_UWP::HomePage::ContentPresenter_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	menuBtn->Width = Width;
	menuBtn->IsEnabled = true;
}


void Just_Editor_UWP::HomePage::openText_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	create_task(Drn_UWP::PickAFile(false)).then([this](Windows::Storage::StorageFile^ tabFile) 
		{
			if (tabFile != nullptr)
				NewTabRequested(tabFile->Name, tabFile);
		}, task_continuation_context::use_current());
}

void Just_Editor_UWP::HomePage::newText_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	NewTabRequested(L"New File", nullptr);
}

void Just_Editor_UWP::HomePage::newSocket_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	NewTabRequested(L"?Socket", nullptr);
}

void Just_Editor_UWP::HomePage::openImage_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	create_task(Drn_UWP::PickAFile(true)).then([this](Windows::Storage::StorageFile^ newFile)
		{
			if (newFile != nullptr)
				create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(ref new Windows::Foundation::Uri(L"ms-appx:///Assets/JustEditor_Logo.png"))).then([](Windows::Storage::StorageFile^ oldFile)
					{
						return oldFile->GetParentAsync();
					}).then([newFile](Windows::Storage::StorageFolder^ oldFolder)
					{
						newFile->CopyAsync(oldFolder, L"bgi", Windows::Storage::NameCollisionOption::ReplaceExisting);
						auto mainPage = (ContentPresenter^)Window::Current->Content;
						if (mainPage != nullptr)
							Drn_UWP::LoadBackgroundImage(mainPage, newFile);
					});
		});
}



void Just_Editor_UWP::HomePage::newCrypt_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	NewTabRequested(L"?Crypt", nullptr);
}


void Just_Editor_UWP::HomePage::DrnMenu_MenuSelected(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
//	mainPage->RequestedTheme = mainPage->RequestedTheme == ElementTheme::Dark ? ElementTheme::Light : ElementTheme::Dark;
	if (sender == nullptr)
		return;
	auto appConfig = ((App^)App::Current)->AppConfig;
	switch (sender->ToString()->Data()[0])
	{
	case L'S':
		appConfig->IsSmartDetectEnabled = appConfig->IsSmartDetectEnabled == false;
		break;
	case L'T':
		appConfig->IsDark = !appConfig->IsDark;
		appConfig->UpdateConfig();
		break;
	}
}
