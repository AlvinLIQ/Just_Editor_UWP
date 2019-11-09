//
// DrnTab.xaml.h
// Declaration of the DrnTab class
//

#pragma once

#include "DrnControls/DrnTab.g.h"
#include "DrnControls/DrnFileDialog.xaml.h"
#include "DrnHeaders/Drn_UWP.h"
#include "DrnContentDialog.xaml.h"
#include "DrnTabPanel.xaml.h"

#define ParentChildren ((Windows::UI::Xaml::Controls::Panel^)this->Parent)->Children

void SetOldIndex(int oIndex);

namespace Just_Editor_UWP
{
	public delegate void TabButtonClickedEventHandler(DrnTab^ tab);
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnTab sealed
	{
	public:
		DrnTab();
		property bool isSelected;

		event TabButtonClickedEventHandler^ clsBtnClicked;
		event TabButtonClickedEventHandler^ tabClicked;

		property Platform::String^ Title
		{
			Platform::String^ get() { return titleBlock->Text; } 
			void set(Platform::String^ newTitle) { titleBlock->Text = newTitle; }
		};
		property DrnFileDialog^ FileDialog;
		property DrnContentDialog^ MakeSureDialog;
		property Windows::UI::Xaml::UIElement^ PageContent;

		void CloseTab()
		{
			if (isClosing)
				return;

			isClosing = true;

			auto tVisual = Windows::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(this);

			concurrency::create_task(Drn_UWP::DrnAnimeY(tVisual, tVisual->Offset.y, tVisual->Offset.y - (float)this->ActualHeight, 11, this)).then([this]()
				{
					clsBtnClicked(this);
				}, concurrency::task_continuation_context::use_current());
		}
		void SetExtraStatus(Platform::String^ statusStr)
		{
			statusBlock->Text = statusStr;
		}
	private:
		bool isClosing = false;
		void StackPanel_PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void StackPanel_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void clsBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void StackPanel_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void StackPanel_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
