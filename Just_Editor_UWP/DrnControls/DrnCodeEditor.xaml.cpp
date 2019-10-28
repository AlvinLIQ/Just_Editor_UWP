//
// DrnCodeEditor.xaml.cpp
// Implementation of the DrnCodeEditor class
//

#include "pch.h"
#include "DrnCodeEditor.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DrnCodeEditor::DrnCodeEditor()
{
	InitializeComponent();
	mainGrid->Children->Append(drnCoreEditor->insideKeyboard);
	mainGrid->SetRow(drnCoreEditor->insideKeyboard, 1);
	mainGrid->SetColumnSpan(drnCoreEditor->insideKeyboard, 2);
	returnMenu->SelectedIndex = 0;
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
	lineNumTrans->Y = -verticalOffset;
//	lineNumScroll->ChangeView(lineNumScroll->HorizontalOffset, verticalOffset, lineNumScroll->ZoomFactor);
}
