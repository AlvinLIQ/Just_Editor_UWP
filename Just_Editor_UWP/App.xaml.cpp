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
	AppConfig = ref new AppConfigs::DrnConfig(true);
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
        if (args->PreviousExecutionState == ApplicationExecutionState::Terminated)
        {
            // TODO: Restore the saved session state only when appropriate, scheduling the
            // final launch steps after the restore is complete

        }

		Window::Current->Content = mainPage;
    }

	if (args->PrelaunchActivated == false)
	{
		Window::Current->Activate();
	}
}


void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{

}

void App::OnFileActivated(Windows::ApplicationModel::Activation::FileActivatedEventArgs^ args)
{
	auto mainPage = dynamic_cast<MainPage^>(Window::Current->Content);

	if (mainPage == nullptr)
	{
		mainPage = ref new MainPage;
		Window::Current->Content = mainPage;
		Window::Current->Activate();
	}

	int filesNum = (int)args->Files->Size;
	while (--filesNum >= 0)
		mainPage->ReadToEditor((Windows::Storage::StorageFile^)args->Files->GetAt(filesNum));
}