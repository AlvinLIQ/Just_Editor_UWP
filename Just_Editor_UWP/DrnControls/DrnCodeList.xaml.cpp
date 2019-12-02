//
// DrnCodeList.xaml.cpp
// Implementation of the DrnCodeList class
//

#include "pch.h"
#include "DrnCodeList.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;


// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DrnCodeList::DrnCodeList()
{
	InitializeComponent();
}


void Just_Editor_UWP::DrnCodeList::codeListPanel_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	e->Handled = true;
	auto curPoint = e->GetCurrentPoint(codeListPanel);
	if (e->Pointer->PointerDeviceType != Windows::Devices::Input::PointerDeviceType::Mouse || curPoint->Timestamp - lastTimeStamp < 200000)
	{
		NotifyWordUpdate();
	}
	else
	{
		lastTimeStamp = curPoint->Timestamp;
		SelectAt((unsigned int)curPoint->Position.Y / 23);
	}
}
