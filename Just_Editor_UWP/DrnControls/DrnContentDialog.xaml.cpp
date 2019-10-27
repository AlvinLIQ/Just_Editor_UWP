//
// DrnContentDialog.xaml.cpp
// Implementation of the DrnContentDialog class
//

#include "pch.h"
#include "DrnContentDialog.xaml.h"

using namespace Just_Editor_UWP;

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

Just_Editor_UWP::DrnContentDialog::DrnContentDialog(Platform::String^ content, Platform::String^ Fst, Platform::String^ Sec, Platform::String^ Cls)
{
	InitializeComponent();
	this->Content = content;
	this->PrimaryButtonText = Fst;
	this->SecondaryButtonText = Sec;
	this->CloseButtonText = Cls;
}
