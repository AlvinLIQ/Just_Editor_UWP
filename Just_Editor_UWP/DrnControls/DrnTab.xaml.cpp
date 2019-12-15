//
// DrnTab.xaml.cpp
// Implementation of the DrnTab class
//

#include "pch.h"
#include "DrnTab.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236
int oldIndex = -1;

void SetOldIndex(int oIndex)
{
	oldIndex = oIndex;
}

DrnTab::DrnTab()
{
	InitializeComponent();
	BorderThickness = 0;
}


void DrnTab::StackPanel_PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (oldIndex != -1)
	{
		auto tChildren = ParentChildren;
		int newIndex = Drn_UWP::getChildNum(ParentChildren, this);
		if (oldIndex != newIndex)
		{
			ParentChildren->Move(oldIndex, newIndex);
			oldIndex = newIndex;
		}
	}
	else if (!this->isSelected)
		this->Background = BorderBrush;
}


void DrnTab::StackPanel_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (!this->isSelected)
		this->Background = ref new SolidColorBrush;
}

void DrnTab::clsBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (isClosing)
		return;

	if (statusBlock->Text == L"*" && MakeSureDialog != nullptr && MakeSureDialog->IsClosed)
		MakeSureDialog->ShowAsync();
	else
		CloseTab();
}


void Just_Editor_UWP::DrnTab::StackPanel_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	auto pressedStatus = e->GetCurrentPoint(this)->Properties;
	if (pressedStatus->IsMiddleButtonPressed)
		clsBtn_Click(this, nullptr);
	else if (pressedStatus->IsLeftButtonPressed)
	{
		oldIndex = Drn_UWP::getChildNum(ParentChildren, this);

		tabClicked(this);
	}
	else
	{
		if (FileDialog != nullptr && FileDialog->IsClosed)
			FileDialog->ShowAsync();
	}
}

void Just_Editor_UWP::DrnTab::StackPanel_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto tVisual = Hosting::ElementCompositionPreview::GetElementVisual(this);
	Drn_UWP::DrnAnimeX(tVisual, tVisual->Offset.x, tVisual->Offset.x, 15, false);
}

