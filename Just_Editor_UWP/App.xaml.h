//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"

namespace AppConfigs
{
	public ref class DrnConfig sealed
	{
	public:
		property Windows::UI::Xaml::Media::SolidColorBrush^ CoverBrush;
		property Windows::UI::Xaml::Media::SolidColorBrush^ BackBrush;
		property Windows::UI::Xaml::Media::SolidColorBrush^ HoverBrush;
		property Windows::UI::Xaml::Media::SolidColorBrush^ ForeBrush;
		property bool IsDark;
		property Windows::UI::Xaml::ElementTheme CurrentTheme
		{
			Windows::UI::Xaml::ElementTheme get()
			{
				return IsDark ? Windows::UI::Xaml::ElementTheme::Dark : Windows::UI::Xaml::ElementTheme::Light;
			}
		};
		void UpdateConfig()
		{
			if (IsDark)
			{
				CoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x55, 0x12, 0x12, 0x12});
				BackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x60, 0x78, 0x78, 0x78 });
			}
			else
			{
				CoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x70, 0x70, 0x70, 0x55 });
				BackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x60, 0xFF, 0xFF, 0xFF });
			}
		}
		DrnConfig(bool isDark)
		{
			IsDark = isDark;
			UpdateConfig();
		}
	};
}

namespace Just_Editor_UWP
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	public ref class App sealed
	{
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args) override;
		virtual void OnFileActivated(Windows::ApplicationModel::Activation::FileActivatedEventArgs^ args) override;

	internal:
		App();

	public:
		property AppConfigs::DrnConfig^ AppConfig;

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
	};
}
