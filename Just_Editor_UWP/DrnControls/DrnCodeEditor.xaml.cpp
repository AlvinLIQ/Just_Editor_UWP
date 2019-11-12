//
// DrnCodeEditor.xaml.cpp
// Implementation of the DrnCodeEditor class
//

#include "pch.h"
#include "DrnCodeEditor.xaml.h"

#include <windowsnumerics.h>

using namespace Just_Editor_UWP;

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DrnCodeEditor::DrnCodeEditor()
{
	InitializeComponent();
	returnMenu->SelectedIndex = 0;
	encodeMenu->SelectedIndex = 0;
	highlightMenu->SelectedIndex = 0;
}


void Just_Editor_UWP::DrnCodeEditor::drnCoreEditor_CursorChanged(default::uint32 col, default::uint32 ln, default::uint32 lineNum)
{
	columnStatus->Text = (col + 1).ToString();
	lineStatus->Text = (ln + 1).ToString();
	SetLineNum(lineNum);
}


void Just_Editor_UWP::DrnCodeEditor::drnCoreEditor_EditorViewChanging(default::float64 verticalOffset)
{
	if (lineNumVisual != nullptr)
		lineNumVisual->Offset = Windows::Foundation::Numerics::float3(lineNumVisual->Offset.x, (float)-verticalOffset, lineNumVisual->Offset.z);
//	lineNumTrans->Y = -verticalOffset;
//	lineNumScroll->ChangeView(lineNumScroll->HorizontalOffset, verticalOffset, lineNumScroll->ZoomFactor);
}


void Just_Editor_UWP::DrnCodeEditor::drnLineNum_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	lineNumVisual = Hosting::ElementCompositionPreview::GetElementVisual(drnLineNum);
}


void Just_Editor_UWP::DrnCodeEditor::encodeMenu_MenuSelected(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	switch (encodeMenu->SelectedIndex)
	{
	case 0:
		//UTF-8
		EditorReloadRequested(Windows::Storage::Streams::UnicodeEncoding::Utf8);
		break;
	case 1:
		//UTF-16LE
		EditorReloadRequested(Windows::Storage::Streams::UnicodeEncoding::Utf16LE);
		break;
	case 2:
		//UTF-16BE
		EditorReloadRequested(Windows::Storage::Streams::UnicodeEncoding::Utf16BE);
		break;
	}
}
