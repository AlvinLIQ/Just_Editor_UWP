﻿//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"

namespace Just_Editor_UWP
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed
	{
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;
		virtual void OnFileActivated(Windows::ApplicationModel::Activation::FileActivatedEventArgs^ args) override;

	internal:
		App();

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
	};
}
