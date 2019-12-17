//
// DrnNotification.xaml.cpp
// Implementation of the DrnNotification class
//

#include "pch.h"
#include "DrnNotification.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DrnNotification::DrnNotification()
{
	InitializeComponent();
	thisVisual = Windows::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(this);
}


void Just_Editor_UWP::DrnNotification::clsBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Hide();
}
