//
// DrnNotification.xaml.h
// Declaration of the DrnNotification class
//

#pragma once

#include "DrnControls/DrnNotification.g.h"
#include "DrnHeaders/Drn_UWP.h"

namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnNotification sealed
	{
	public:
		DrnNotification();

		event Windows::UI::Xaml::RoutedEventHandler^ PrmBtnClick
		{
			Windows::Foundation::EventRegistrationToken add(Windows::UI::Xaml::RoutedEventHandler^ eventHandler) { return (prmBtn->Click += eventHandler); }
			void remove(Windows::Foundation::EventRegistrationToken token) { prmBtn->Click -= token; }
		};
		property Platform::Object^ msgContent;
		property Platform::Object^ prmBtnContent;

		void Show()
		{
			this->Height = 30;
			Drn_UWP::DrnAnimeX(thisVisual, thisVisual->Offset.x, thisVisual->Offset.x, 20, false);
		}
		void Hide()
		{
			Drn_UWP::DrnAnimeX(thisVisual, thisVisual->Offset.x, thisVisual->Offset.x, 20, true);
			this->Height = 0;
		}
	private:
		Windows::UI::Composition::Visual^ thisVisual;

		void clsBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
