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
	public delegate void EncodeChangedEventHandler(Windows::Storage::Streams::UnicodeEncoding encodeMode);
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

		event EncodeChangedEventHandler^ EditorReloadRequested;

		event EditorTextChangedEventHandler^ EditorActionChanged
		{
			Windows::Foundation::EventRegistrationToken add(EditorTextChangedEventHandler^ changedHandler) { return (drnCoreEditor->EditorActionChanged += changedHandler); }
			void remove(Windows::Foundation::EventRegistrationToken token) { drnCoreEditor->EditorActionChanged -= token; }
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
		
		void AppendStr(Platform::String^ newStr) 
		{ 
			drnCoreEditor->AppendUserStr(newStr); 
		}
		void Clear()
		{
			drnCoreEditor->Clear();
		}
		Platform::String^ GetStr()
		{
			Platform::String^ result = L"";
			for (unsigned int i = 0; i < drnCoreEditor->DrnLineNum->Items->Size - 1; i++)
				result += drnCoreEditor->GetLineStr(i) + ref new Platform::String(returnMap[returnMenu->SelectedIndex]);
			if (drnCoreEditor->DrnLineNum->Items->Size)
				result += drnCoreEditor->GetLineStr(drnCoreEditor->DrnLineNum->Items->Size - 1);

			return result;
		}
		
	private:
		void drnCoreEditor_CursorChanged(default::uint32 col, default::uint32 ln, default::uint32 lineNum);
		void encodeMenu_MenuSelected(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
