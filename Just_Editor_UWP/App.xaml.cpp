//
// App.xaml.cpp
// Implementation of the App class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;


App::App()
{
    InitializeComponent();
	if (AppConfig == nullptr)
	{
		AppConfig = ref new Just_Editor_UWP::DrnConfig(this->RequestedTheme == ApplicationTheme::Dark);
		AppConfig->Updated += ref new Just_Editor_UWP::AppSettingsUpdatedEventHandler(this, &Just_Editor_UWP::App::OnUpdated);
	}
    Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args)
{
    auto mainPage = dynamic_cast<MainPage^>(Window::Current->Content);

    // Do not repeat app initialization when the Window already has content,
    // just ensure that the window is active
	if (mainPage == nullptr)
    {
		mainPage = ref new MainPage;

		mainPage->RequestedTheme = this->AppConfig->IsDark ? ElementTheme::Dark : ElementTheme::Light;

		if (args->PrelaunchActivated == false)
		{
			Window::Current->Content = mainPage;
			Window::Current->Activate();
		}
    }
	else if (args->PrelaunchActivated == false)
	{
		Window::Current->Activate();
	}
}


void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{

}

void App::OnUpdated(bool isDark)
{
	auto mainPage = ref new MainPage;
	mainPage->RequestedTheme = isDark ? ElementTheme::Dark : ElementTheme::Light;

	Window::Current->Content = mainPage;
}

void App::OnFileActivated(Windows::ApplicationModel::Activation::FileActivatedEventArgs^ args)
{
	if (AppConfig == nullptr)
	{
		AppConfig = ref new Just_Editor_UWP::DrnConfig(this->RequestedTheme == ApplicationTheme::Dark);
		AppConfig->Updated += ref new Just_Editor_UWP::AppSettingsUpdatedEventHandler(this, &Just_Editor_UWP::App::OnUpdated);
	}

	auto mainPage = dynamic_cast<MainPage^>(Window::Current->Content);

	if (mainPage == nullptr)
	{
		mainPage = ref new MainPage;
		mainPage->RequestedTheme = this->AppConfig->IsDark ? ElementTheme::Dark : ElementTheme::Light;
		Window::Current->Content = mainPage;
		Window::Current->Activate();
	}

	int filesNum = (int)args->Files->Size;
	Windows::Storage::IStorageItem^ tItem;
	while (--filesNum >= 0)
	{
		tItem = args->Files->GetAt(filesNum);
		if(tItem->IsOfType(Windows::Storage::StorageItemTypes::File))
			mainPage->ReadToEditor((Windows::Storage::StorageFile^)tItem);
	}
}