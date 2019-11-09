//
// DrnCodeEditor.xaml.h
// Declaration of the DrnCodeEditor class
//

#pragma once

#include "DrnControls/DrnCodeEditor.g.h"
#include "DrnCoreEditor.xaml.h"
#include "DrnMenu.xaml.h"

const wchar_t returnMap[][3] = {L"\r\n", L"\n", L"\r"};

namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnCodeEditor sealed
	{
	public:
		DrnCodeEditor();

		property Windows::UI::Xaml::Media::SolidColorBrush^ LineNumBackground;

		property DrnCoreEditor^ coreEditor 
		{
			DrnCoreEditor^ get() { return drnCoreEditor; }
		};

		event EditorTextChangedEventHandler^ EditorTextChanged 
		{
			Windows::Foundation::EventRegistrationToken add(EditorTextChangedEventHandler^ changedHandler) { return (drnCoreEditor->EditorTextChanged += changedHandler); }
			void remove(Windows::Foundation::EventRegistrationToken token) { drnCoreEditor->EditorTextChanged -= token; }
		};
		event EditorTextChangedEventHandler^ EditorSavedRequested
		{
			Windows::Foundation::EventRegistrationToken add(EditorTextChangedEventHandler^ changedHandler) { return (drnCoreEditor->EditorSaveRequested += changedHandler); }
			void remove(Windows::Foundation::EventRegistrationToken token) { drnCoreEditor->EditorSaveRequested -= token; }
		};

		void UnloadEditor()
		{
			drnCoreEditor->UnloadEditor();
		}
		
		void AppendStr(Platform::String^ newStr) 
		{ 
			drnCoreEditor->AppendUserStr(newStr); 
		}
		void Clear()
		{
			drnLineNum->Children->Clear();
			drnCoreEditor->Clear();
			SetLineNum(1);
		}
		Platform::String^ GetStr()
		{
			Platform::String^ result = L"";
			for (unsigned int i = 0; i < drnLineNum->Children->Size - 1; i++)
				result += drnCoreEditor->GetLineStr(i) + ref new Platform::String(returnMap[returnMenu->SelectedIndex]);
			if (drnLineNum->Children->Size)
				result += drnCoreEditor->GetLineStr(drnLineNum->Children->Size - 1);

			return result;
		}
		void SetLineNum(unsigned int newLineNum)
		{
			while (drnLineNum->Children->Size < newLineNum)
			{
				auto tItem = ref new Windows::UI::Xaml::Controls::ContentPresenter;
				tItem->Height = fHeight;
				tItem->Content = (drnLineNum->Children->Size + 1).ToString();
				drnLineNum->Children->Append(tItem);
			}
			while (drnLineNum->Children->Size > newLineNum)
			{
				drnLineNum->Children->RemoveAtEnd();
			}
		}
	private:
		Windows::UI::Composition::Visual^ lineNumVisual = nullptr;

		void drnCoreEditor_CursorChanged(default::uint32 col, default::uint32 ln, default::uint32 lineNum);
		void drnCoreEditor_EditorViewChanging(default::float64 verticalOffset);
		void drnLineNum_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void encodeMenu_MenuSelected(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
