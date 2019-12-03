//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"
#include "DrnHeaders/Drn_UWP.h"


namespace Just_Editor_UWP
{
	public delegate void AppSettingsUpdatedEventHandler(bool isDark);
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
		property bool IsSmartDetectEnabled
		{
			bool get()
			{
				return isSmartDetectEnabled;
			}
			void set(bool newState)
			{
				if (isSmartDetectEnabled != newState)
				{
					isSmartDetectEnabled = newState;
					Drn_UWP::WriteSetting(L"EditorSettings", L"SmartDetect", newState);
				}
			}
		};
		property Windows::UI::Xaml::ElementTheme CurrentTheme
		{
			Windows::UI::Xaml::ElementTheme get()
			{
				return this->IsDark ? Windows::UI::Xaml::ElementTheme::Dark : Windows::UI::Xaml::ElementTheme::Light;
			}
		};
		void UpdateConfig()
		{
			if (this->IsDark)
			{
				this->CoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x55, 0x12, 0x12, 0x12 });
				this->SubCoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x80, 0x34, 0x34, 0x34 });
				this->BackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x60, 0x78, 0x78, 0x78 });
				this->SubBackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x60, 0x23, 0x23, 0x23 });
				this->HoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x90, 0x43, 0x43, 0x43 });
				this->SelectedBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x6F, 0x34, 0x34, 0x34 });
			}
			else
			{
				this->CoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0xA0, 0xD8, 0xD8, 0xD8 });
				this->SubCoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x80, 0xD5, 0xD5, 0xD5 });
				this->BackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0xA0, 0xFF, 0xFF, 0xFF });
				this->SubBackBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x40, 0xF0, 0xF0, 0xF0 });
				this->HoverBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0xCC, 0xCB, 0xCB, 0xCB });
				this->SelectedBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Color{ 0x6F, 0xEF, 0xEF, 0xEF });
			}
			Updated(this->IsDark);
		}
		DrnConfig(bool isDark)
		{
			this->IsDark = isDark;

			auto sValue = Drn_UWP::ReadSetting(L"EditorSettings", L"SmartDetect");
			isSmartDetectEnabled = (sValue != nullptr && (bool)sValue);
			UpdateConfig();
		}
	private:
		bool isSmartDetectEnabled;
	};
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
		property Just_Editor_UWP::DrnConfig^ AppConfig;

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnUpdated(bool isDark);
	};
}
