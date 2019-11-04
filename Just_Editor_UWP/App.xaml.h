//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"

namespace AppConfigs
{
	ref class DrnConfig;
	public delegate void AppSettingsUpdatedEventHandler(DrnConfig^ newConfig);
	public ref class DrnConfig sealed
	{
	public:
		event AppSettingsUpdatedEventHandler^ Updated;
		property Windows::UI::Xaml::Media::SolidColorBrush^ CoverBrush;
		property Windows::UI::Xaml::Media::SolidColorBrush^ SubCoverBrush;
		property Windows::UI::Xaml::Media::SolidColorBrush^ BackBrush;
		property Windows::UI::Xaml::Media::SolidColorBrush^ SubBackBrush;
		property Windows::UI::Xaml::Media::SolidColorBrush^ HoverBrush;
		property Windows::UI::Xaml::Media::SolidColorBrush^ SelectedBrush;
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
				SubCoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x80, 0x34, 0x34, 0x34 });
				BackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x60, 0x78, 0x78, 0x78 });
				SubBackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x60, 0x23, 0x23, 0x23 });
				HoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x90, 0x43, 0x43, 0x43 });
				SelectedBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x6F, 0x34, 0x34, 0x34 });
			}
			else
			{
				CoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x70, 0xE4, 0xE4, 0xE4 });
				SubCoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x80, 0xD5, 0xD5, 0xD5 });
				BackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0xCC, 0xFF, 0xFF, 0xFF });
				SubBackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x9A, 0xDC, 0xDC, 0xDC });
				HoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0xCC, 0xCB, 0xCB, 0xCB });
				SelectedBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x6F, 0xEF, 0xEF, 0xEF });
			}
			Updated(this);
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
		void OnUpdated(AppConfigs::DrnConfig^ newConfig);
	};
}
