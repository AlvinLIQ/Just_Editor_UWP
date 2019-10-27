//
// CryptPage.xaml.cpp
// Implementation of the CryptPage class
//

#include "pch.h"
#include "CryptPage.xaml.h"
#include "DrnHeaders/base64.h"

using namespace Just_Editor_UWP;

using namespace Platform;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

CryptPage::CryptPage()
{
	InitializeComponent();
}


void Just_Editor_UWP::CryptPage::enBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (deBox->Text != L"")
		enBox->Text = ref new String(base64_encryptw(deBox->Text->ToString()->Data()));
}


void Just_Editor_UWP::CryptPage::deBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (enBox->Text != L"")
		deBox->Text = ref new String(base64_decryptw(enBox->Text->ToString()->Data()));
}
