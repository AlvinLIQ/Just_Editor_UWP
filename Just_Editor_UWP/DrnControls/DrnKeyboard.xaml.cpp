//
// DrnKeyboard.xaml.cpp
// Implementation of the DrnKeyboard class
//

#include "pch.h"
#include "DrnKeyboard.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

DrnKeyboard::DrnKeyboard()
{
	InitializeComponent();
	InitKey();
}

void DrnKeyboard::InitKey()
{
	auto keyLine = ref new Grid;;
	int colNum = 0, rowNum = 0, t, newColumn;
	for (int i = 0; i < (int)wcslen(drnKeyMap); i++)
	{

		if (drnKeyMap[i] == L'\n')
		{
			keyGrid->RowDefinitions->Append(ref new RowDefinition);
			keyGrid->Children->Append(keyLine);
			keyGrid->SetRow(keyLine, rowNum++);

			keyLine = ref new Grid;
			colNum = 0;
			continue;
		}
		auto tItem = ref new Windows::UI::Xaml::Controls::Primitives::RepeatButton;
		keyLine->Children->Append(tItem);
		keyLine->SetColumn(tItem, colNum);
		tItem->BorderThickness = 0;
		if (drnKeyMap[i] >= 128)
			tItem->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily(L"Segoe MDL2 Assets");

		if (drnKeyMap[i] == L'')
		{
			tItem->Background = ref new SolidColorBrush;
			tItem->Click += ref new Windows::UI::Xaml::RoutedEventHandler([this, tItem](Object^ sender, RoutedEventArgs^ e)
				{
					if (((SolidColorBrush^)tItem->Background)->Color.A == 0)
					{
						drnKeyDown(42);
						tItem->Background = ref new SolidColorBrush(Windows::UI::Colors::Gray);
						for (unsigned int i = 0, j, k = 0; i < keyGrid->Children->Size; i++)
						{
							auto tLine = (Grid^)keyGrid->Children->GetAt(i);
							for (j = 0; j < tLine->Children->Size; j++, k++)
							{
								((Primitives::RepeatButton^)tLine->Children->GetAt(j))->Content = drnShiftKeyMap[k];
							}
							k++;
						}
					}
					else
					{
						drnKeyUp(42);
						tItem->Background = ref new SolidColorBrush;
						for (unsigned int i = 0, j, k = 0; i < keyGrid->Children->Size; i++)
						{
							auto tLine = (Grid^)keyGrid->Children->GetAt(i);
							for (j = 0; j < tLine->Children->Size; j++, k++)
							{
								((Primitives::RepeatButton^)tLine->Children->GetAt(j))->Content = drnKeyMap[k];
							}
							k++;
						}
					}
				});
		}
		else
			tItem->Click += ref new Windows::UI::Xaml::RoutedEventHandler(this, &DrnKeyboard::keyBtn_Clicked);

		switch (drnKeyMap[i])
		{
		case L' ':
			newColumn = 5;
			break;
//		case L'':
		case L'':
			newColumn = 2;
			break;
		default:
			newColumn = 1;
		}
		for (t = 0; t < newColumn; t++)
			keyLine->ColumnDefinitions->Append(ref new ColumnDefinition);
		keyLine->SetColumnSpan(tItem, newColumn);
		colNum += newColumn;
		tItem->Background = this->Background;
		tItem->Content = drnKeyMap[i];
		tItem->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Stretch;
		tItem->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Stretch;
	}
}

void DrnKeyboard::Show()
{
	if (Height == 250)
		return;

	this->Opacity = 0;
	Height = 250;
	Drn_UWP::DrnAnimeY(Hosting::ElementCompositionPreview::GetElementVisual(this), 250, 0, 25, this);
}

void DrnKeyboard::Hide()
{
	if (!Height)
		return;

	Drn_UWP::DrnAnimeY(Hosting::ElementCompositionPreview::GetElementVisual(this), 0, 250, 25, this).then([this]()
			{
				Height = 0;
			}, concurrency::task_continuation_context::use_current());
}

void DrnKeyboard::keyGrid_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	eventToken = (Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs^>(this, &Just_Editor_UWP::DrnKeyboard::OnBackRequested));
}

void DrnKeyboard::keyBtn_Clicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	wchar_t tKey = ((Windows::UI::Xaml::Controls::Primitives::RepeatButton^)sender)->Content->ToString()->Data()[0];
	if (tKey >= 97 && tKey < 123)
		tKey &= ~32;

	for (unsigned int i = 0; i < 122; i++)
	{
		if (tKey == drnKeyCodeMap[i] || tKey == drnShiftKeyCodeMap[i])
		{
			drnKeyDown(i);
			return;
		}
	}
}


void DrnKeyboard::keyGrid_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->BackRequested -= eventToken;
}


void Just_Editor_UWP::DrnKeyboard::OnBackRequested(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ args)
{
	if (this->ActualHeight)
	{
		args->Handled = true;
		this->Hide();
	}
}

