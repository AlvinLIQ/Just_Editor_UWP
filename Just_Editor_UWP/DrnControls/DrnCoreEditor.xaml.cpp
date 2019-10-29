//
// DrnCoreEditor.xaml.cpp
// Implementation of the DrnCoreEditor class
//

#include "pch.h"
#include "DrnCoreEditor.xaml.h"


using namespace Just_Editor_UWP;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::System;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DrnCoreEditor::DrnCoreEditor()
{
	InitializeComponent();
	insideKeyboard = ref new DrnKeyboard;
	insideKeyboard->Height = 0;
	insideKeyboard->drnKeyDown += ref new Just_Editor_UWP::drnKeyEventHandler(this, &DrnCoreEditor::DrnKeyboard_drnKeyDown);
	insideKeyboard->drnKeyUp += ref new Just_Editor_UWP::drnKeyEventHandler(this, &DrnCoreEditor::DrnKeyboard_drnKeyUp);

	AppendBlockAtEnd();
}


void DrnCoreEditor::CoreEditor_PointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	if (!isTapped)
	{
		if (e->CurrentPoint->Position.Y - topMargin >= 0)
			return;
		else
		{
			isActivated = false;
			drnCursor->Opacity = 0;
			insideKeyboard->Hide();
		}
	}
	else
	{
		auto tPosition = e->CurrentPoint->Position;
		leftMargin = tPosition.X - leftMargin + (float)editorScrollViewer->HorizontalOffset;
		topMargin = tPosition.Y - selPosition.Y + (float)editorScrollViewer->VerticalOffset;
	}
}

void DrnCoreEditor::CoreEditor_PointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	isTapped = false;
}

void DrnCoreEditor::CoreEditor_PointerMoved(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	if (!isTapped)
		return;

	auto curPosition = e->CurrentPoint->Position;
	curPosition.X = curPosition.X - leftMargin + (float)editorScrollViewer->HorizontalOffset + (float)(fWidth / 2);
	curPosition.Y = curPosition.Y - topMargin + (float)editorScrollViewer->VerticalOffset + 10;

	if (curPosition.X < 0)
		curPosition.X = 0;
	if (curPosition.Y < 0)
		curPosition.Y = 0;
	
	unsigned int ln = (unsigned int)curPosition.Y / fHeight;

	if (ln >= textChildren->Items->Size)
	{
		ln = textChildren->Items->Size - 1;
		curPosition.Y = (float)ln * fHeight + 5;
	}
	if (ln == currentLine && cursor == currentLength && (double)selPosition.X == cursorX && curPosition.X > selPosition.X)
		return;
	MoveToPosition(curPosition);
	

	Select(cursor, ln);
}


void DrnCoreEditor::CoreEditor_KeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e)
{
	if (!isActivated)
		return;

	if (e != nullptr)
	{
		virtualKeyCode = e->KeyStatus.ScanCode;
		insideKeyboard->Hide();
	}
	if (coreWindow->GetKeyState(Windows::System::VirtualKey::NumberKeyLock) == Windows::UI::Core::CoreVirtualKeyStates::Locked
		&& e->VirtualKey >= VirtualKey::NumberPad0 && e->VirtualKey <= VirtualKey::NumberPad9)
	{
		virtualKeyCode = e->VirtualKey.ToString()->Data()[9] + 73;
	}

	if (isCtrlHeld)
	{
		switch (keyMap[virtualKeyCode])
		{
		case L'F':
			if (searchFlyout != nullptr)
			{
				isActivated = false;
				drnCursor->Opacity = 0;

				searchFlyout->ShowAt(this);
			}
			break;
		case L'A':
			SelectAll();
			break;
		case L'S':
			EditorSaveRequested();
			break;
		case L'V':
			Paste();
		case L'X':
			if (IsTextSelected())
				Cut();
			break;
		case L'C':
			if (IsTextSelected())
				Copy();
			break;
		}
	}
	else
	{
		std::wstring wStr;

		switch (virtualKeyCode)
		{
		case 42:
		case 54:
			isShiftHeld = true;

			break;
		case 29:
			isCtrlHeld = true;

			break;
		case 14://Backspace
			if (IsTextSelected())
				ClearSelection();
			else if (cursor)
			{
				wStr = currentBlock->Content->ToString()->Data();

				currentBlock->Content = ref new String((wStr.substr(0, cursor - 1) + wStr.substr(cursor, currentLength - cursor)).c_str());
				cursor--;
				currentLength--;
				cursorX -= wStr[cursor] < 128 ? fWidth : fBWidth;
				UpdateCursor();
			}
			else if (currentLine)
			{
				currentBlock = (TXTBLOCK^)textChildren->Items->GetAt(currentLine - 1);
				wStr = currentBlock->Content->ToString()->Data();
				cursor = (unsigned int)wStr.length();
				currentLength += cursor;
				cursorX = GetCursorXFromWStr(wStr, cursor);
				RemoveLine(currentLine--);
				CursorChanged(cursor, currentLine, textChildren->Items->Size);
			}
			break;
		case 75://Left
			if (isShiftHeld)
			{
				if (cursor)
				{
					Select(--cursor, currentLine);
					UpdateCursor();
				}
				else if (currentLine)
				{
					Select(cursor = GetLineStrLength(currentLine), --currentLine);
					currentLength = cursor;
					UpdateCursor();
				}
			}
			else
				MoveLeft();

			break;
		case 77://Right
			if (isShiftHeld)
			{
				if (cursor < currentLength)
				{
					Select(++cursor, currentLine);

					wStr = currentBlock->Content->ToString()->Data();
					cursorX = GetCursorXFromWStr(wStr, cursor);
					UpdateCursor();
				}
				else if (currentLine + 1 < textChildren->Items->Size)
				{
					Select(cursor = 0, ++currentLine);
					currentLength = GetLineStrLength(currentLine);

					cursorX = 0;
					UpdateCursor();
				}
			}
			else
				MoveRight();

			break;
		case 72://Up
			if (currentLine)
			{
				MoveTo(cursor, currentLine - 1);
				if (isShiftHeld)
					Select(cursor, currentLine);
				else
				{
					if (IsTextSelected())
						ClearSelection();
					selPosition.X = (float)cursorX;
					selPosition.Y = (float)(currentLine * fHeight);
				}
			}

			break;
		case 80://Down
			if (currentLine + 1 < textChildren->Items->Size)
			{
				MoveTo(cursor, currentLine + 1);
				if (isShiftHeld)
					Select(cursor, currentLine);
				else
				{
					if (IsTextSelected())
						ClearSelection();
					selPosition.X = (float)cursorX;
					selPosition.Y = (float)(currentLine * fHeight);
				}
			}

			break;
		case 71://Home
			MoveTo(0, currentLine);

			break;
		case 79://End
			MoveTo(-1, currentLine);

			break;
		case 83://Delete
			if (e->VirtualKey != VirtualKey::Decimal)
			{
				if (IsTextSelected())
					ClearSelection();
				else if (cursor < currentLength)
				{

						wStr = currentBlock->Content->ToString()->Data();
						if (cursor + 1 == currentLength)
							currentBlock->Content = ref new String(wStr.substr(0, cursor).c_str());
						else
							currentBlock->Content = ref new String((wStr.substr(0, cursor) + wStr.substr(cursor + 1, currentLength - cursor - 1)).c_str());

					currentLength--;
					CursorChanged(cursor, currentLine, textChildren->Items->Size);
				}
				else if (currentLine + 1 < textChildren->Items->Size)
				{
					currentLength += GetLineStrLength(currentLine + 1);
					currentBlock->Content = GetLineStr(currentLine + 1);
					RemoveLine(currentLine + 1);
					CursorChanged(cursor, currentLine, textChildren->Items->Size);
				}
				break;
			}
		default:
			if (keyMap[virtualKeyCode] == L'_')
				break;
			if (IsTextSelected())
				ClearSelection();

			wchar_t tWChar = (wchar_t) * (char*)((isShiftHeld ? shiftKeyMap : keyMap) + virtualKeyCode);
			AppendWCharAtCursor(tWChar);
		}
	}
}

void DrnCoreEditor::CoreEditor_KeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e)
{
	if (e != nullptr)
		virtualKeyCode = e->KeyStatus.ScanCode;

	switch (virtualKeyCode)
	{
	case 42:
		isShiftHeld = false;

		break;
	case 29:
		isCtrlHeld = false;

		break;
	}
}

void DrnCoreEditor::Select(unsigned int col, unsigned int ln)
{
	if (ln >= textChildren->Items->Size)
		ln = textChildren->Items->Size - 1;

	DrnCoreEditorSelectionBlock^ tRect = nullptr;

	unsigned int selLine = GetSelectionStartLine(), lineDist = Drn_UWP::abslteSub(ln, selLine), len;
	double selOffset, selEnd;

	while (selectionPanel->Children->Size > lineDist + 1)
	{
		selectionPanel->Children->RemoveAtEnd();
	}
	while (selectionPanel->Children->Size <= lineDist)
	{
		selectionPanel->Children->Append(ref new DrnCoreEditorSelectionBlock);
	}
	
	if (lineDist)
	{
		unsigned int tIndex;
		if (ln > selLine)
		{
			for (tIndex = 1; tIndex < selectionPanel->Children->Size - 1; tIndex++)
			{
				tRect = (DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(tIndex);
				len = selLine + tIndex;
				tRect->SetSelection(0, len * fHeight, GetLineWidth(len) + fWidth / 2);
			}
			tRect = (DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(0);
			tRect->SetSelection(selPosition.X, selLine * fHeight, GetLineWidth(selLine) - selPosition.X + fWidth / 2);
		}
		else
		{
			for (tIndex = 1; tIndex < selectionPanel->Children->Size - 1; tIndex++)
			{
				tRect = (DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(tIndex);
				len = selLine - tIndex;
				tRect->SetSelection(0, len * fHeight, GetLineWidth(len) + fWidth / 2);
			}
			tRect = (DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(0);
			tRect->SetSelection(0, selLine * fHeight, selPosition.X + fWidth / 2);
		}

	}

	std::wstring wStr = currentBlock->Content->ToString()->Data();

	if (selLine == ln)
	{
		if (cursorTrans->X > (double)selPosition.X)
		{
			selEnd = cursorTrans->X;
			selOffset = (double)selPosition.X;
		}
		else
		{
			selOffset = cursorTrans->X;
			selEnd = (double)selPosition.X;
		}
	}
	else if (ln > selLine)
	{
		selEnd = cursorTrans->X;
		selOffset = 0;
	}
	else
	{
		selOffset = cursorTrans->X;
		selEnd = GetCursorXFromWStr(wStr, currentLength);
	}

	if (lineDist < selectionPanel->Children->Size)
		tRect = (DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(lineDist);
	else
		tRect = (DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(selectionPanel->Children->Size - 1);

	
	tRect->SetSelection(selOffset, ln * fHeight, selEnd - selOffset);
}


void DrnCoreEditor::MoveLeft()
{
	if (IsTextSelected())
	{
		unsigned int tLine = GetSelectionStartLine();

		if (tLine < currentLine || (tLine == currentLine && cursorTrans->X > (double)selPosition.X))
		{
			MoveToPosition(selPosition);
		}
		else
		{
			selPosition.X = (float)cursorTrans->X;
			selPosition.Y = (float)cursorTrans->Y;
		}
		CancelSelection();
	}
	else if (cursor)
	{
		cursor--;
		cursorX -= currentBlock->Content->ToString()->Data()[cursor] < 128 ? fWidth : fBWidth;
		UpdateCursor();
	}
	else if (currentLine)
	{
		MoveTo(-1, currentLine - 1);
	}
	selPosition.X = (float)cursorX;
	selPosition.Y = (float)(currentLine * fHeight);
}

void DrnCoreEditor::MoveRight()
{
	if (IsTextSelected())
	{
		unsigned int tLine = GetSelectionStartLine();
		
		if (tLine > currentLine || (tLine == currentLine && cursorTrans->X < (double)selPosition.X))
		{
			MoveToPosition(selPosition);
		}
		else
		{
			selPosition.X = (float)cursorTrans->X;
			selPosition.Y = (float)cursorTrans->Y;
		}
		CancelSelection();
	}
	else if (cursor < currentLength)
	{
		cursorX += currentBlock->Content->ToString()->Data()[cursor] < 128 ? fWidth : fBWidth;
		cursor++;
		UpdateCursor();
	}
	else if (currentLine + 1 < textChildren->Items->Size)
	{
		MoveTo(0, currentLine + 1);
	}
	selPosition.X = (float)cursorX;
	selPosition.Y = (float)(currentLine * fHeight);
}

void DrnCoreEditor::MoveTo(unsigned int col, unsigned int ln)
{
	if (ln >= textChildren->Items->Size)
		return;

	std::wstring wStr;
	if (ln != currentLine)
	{
		currentBlock = (TXTBLOCK^)textChildren->Items->GetAt(ln);
		wStr = currentBlock->Content->ToString()->Data();
		currentLength = (unsigned int)wStr.length();
	}
	else
		wStr = currentBlock->Content->ToString()->Data();

	if (cursor != col)
	{
		cursor = col > currentLength ? currentLength : col;
		cursorX = GetCursorXFromWStr(wStr, cursor);
	}
	currentLine = ln;
	UpdateCursor();
}

void DrnCoreEditor::MoveToPosition(Windows::Foundation::Point newPos)
{
	unsigned int ln = (unsigned int)(newPos.Y / fHeight);
	if (ln >= textChildren->Items->Size)
		return;

	currentBlock = (TXTBLOCK^)textChildren->Items->GetAt(ln);
	std::wstring wStr = currentBlock->Content->ToString()->Data();
	if (ln != currentLine)
	{
		currentLength = (unsigned int)wStr.length();
	}

	UpdateCursorX(wStr, newPos.X + fWidth / 2);

	currentLine = ln;
	UpdateCursor();
}

void DrnCoreEditor::AppendBlockAtEnd()
{
	if (!textChildren->Items->Size)
	{
		textChildren->Items->Append(currentBlock = newTextBlock);
	}
}

void DrnCoreEditor::AppendWCharAtCursor(wchar_t newWChar)
{
	if (newWChar == L'\r')
		newWChar = L'\n';

	if (newWChar == L'\n')
	{
		auto tItem = currentBlock;
		currentLine++;
		textChildren->Items->InsertAt(currentLine, currentBlock = newTextBlock);

		if (cursor < currentLength)
		{
			std::wstring wStr = tItem->Content->ToString()->Data();
			tItem->Content = ref new String(wStr.substr(0, cursor).c_str());
			currentBlock->Content = ref new String(wStr.substr(cursor, currentLength - cursor).c_str());
			currentLength -= cursor;
		}
		else
		{
			currentLength = 0;
		}

		cursor = 0;
		cursorX = 0;
	}
	else
	{
		if (newWChar >= 65 && newWChar < 91)
		{
			bool isCapLocked = coreWindow->GetKeyState(Windows::System::VirtualKey::CapitalLock) == Windows::UI::Core::CoreVirtualKeyStates::Locked;
			if (!isCapLocked && !isShiftHeld || (isCapLocked && isShiftHeld))
				newWChar |= 32;
		}

		
		if (cursor == currentLength)
		{
			currentBlock->Content = currentBlock->Content->ToString() + newWChar;
		}
		else
		{
			std::wstring wStr = currentBlock->Content->ToString()->Data();
			currentBlock->Content= ref new String((wStr.substr(0, cursor) + newWChar + wStr.substr(cursor, currentLength - cursor)).c_str());
		}

		currentLength++;
		cursor++;
		cursorX += newWChar < 128 ? fWidth : fBWidth;;
	}

	EditorTextChanged();

	selPosition.X = (float)cursorX;
	selPosition.Y = (float)(currentLine * fHeight);
	UpdateCursor();
}

void DrnCoreEditor::AppendUserWchar(unsigned int keyCode, bool isDown)
{
	virtualKeyCode = keyCode;
	if (isDown)
		DrnCoreEditor::CoreEditor_KeyDown(coreWindow.Get(), nullptr);
	else
		DrnCoreEditor::CoreEditor_KeyUp(coreWindow.Get(), nullptr);
}

void DrnCoreEditor::AppendStrAtCursor(const wchar_t *newWStr)
{
	unsigned int sIndex, tLen;
	std::wstring afterCursor = L"";
	if (currentLength > cursor)
	{
		afterCursor = currentBlock->Content->ToString()->Data();
		currentBlock->Content = ref new String(afterCursor.substr(0, cursor).c_str());

		afterCursor = afterCursor.substr(cursor, currentLength -= cursor);
		tLen = currentLength;
	}
	for (sIndex = 0; newWStr[sIndex]; sIndex++)
	{
		if (newWStr[sIndex] == L'\r' && newWStr[sIndex + 1] == L'\n')
		{
			sIndex++;
		}
		if (newWStr[sIndex] == L'\n' || newWStr[sIndex] == L'\r')
		{
			currentLine++;
			textChildren->Items->InsertAt(currentLine, currentBlock = newTextBlock);
			cursorX = 0;
			currentLength = 0;
			cursor = 0;
		}
		else
		{
			if (newWStr[sIndex] == L'\t')
			{
				String^ sTab = L"";
				for (unsigned int i = 4 - cursor % 4; i-- > 0; cursor++, currentLength++, cursorX += fWidth)
				{
					sTab += L" ";
				}
				currentBlock->Content = currentBlock->Content->ToString() + sTab;
			}
			else
			{
				currentBlock->Content = currentBlock->Content->ToString() + newWStr[sIndex];
				cursor++;
				currentLength++;
				cursorX += newWStr[sIndex] < 128 ? fWidth : fBWidth;
			}
		}
	}
	if (afterCursor[0])
	{
		currentBlock->Content += ref new String(afterCursor.c_str());
		currentLength += tLen;
	}

	EditorTextChanged();

	selPosition.X = (float)cursorX;
	selPosition.Y = (float)(currentLine * fHeight);
	UpdateCursor();
}


void DrnCoreEditor::EditorContent_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	isActivated = true;
	isTapped = true;
	drnCursor->Opacity = 1;
	auto curPoint = e->GetCurrentPoint((UIElement^)textChildren);
	if (IsTextSelected())
	{
		if (curPoint->Properties->IsRightButtonPressed)
		{
			e->Handled = true;
			return;
		}
		else
			selectionPanel->Children->Clear();
	}
	auto curPosition = curPoint->Position;

	leftMargin = curPosition.X;
	curPosition.X += (float)(fWidth / 2);
	if (abs(curPosition.X - selPosition.Y) < fWidth && curPosition.Y == selPosition.Y)
	{
		selPosition.X = 0;
		cursorX = 0;
		cursor = 0;
		UpdateCursor();

		Select(cursor, currentLine);
	}
	else
	{
		MoveToPosition(curPosition);
		selPosition.X = (float)cursorX;
		selPosition.Y = (float)currentLine * fHeight;
	}

	if (e->Pointer->PointerDeviceType != Windows::Devices::Input::PointerDeviceType::Mouse)
		insideKeyboard->Show();
}

void DrnCoreEditor::EditorContent_PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Window::Current->CoreWindow->PointerCursor = ref new Windows::UI::Core::CoreCursor(Windows::UI::Core::CoreCursorType::IBeam, 0);
}

void DrnCoreEditor::EditorContent_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Window::Current->CoreWindow->PointerCursor = ref new Windows::UI::Core::CoreCursor(Windows::UI::Core::CoreCursorType::Arrow, 0);
}


void DrnCoreEditor::CoreEditor_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	UnloadEditor();
}


void DrnCoreEditor::CoreEditor_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	coreWindow = Window::Current->CoreWindow;
	coreEventToken[0] 
		= coreWindow->KeyDown
		+= ref new TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &DrnCoreEditor::CoreEditor_KeyDown);

	coreEventToken[1]
		= coreWindow->KeyUp 
		+= ref new TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &DrnCoreEditor::CoreEditor_KeyUp);

	coreEventToken[2] 
		= coreWindow->PointerPressed
		+= ref new TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &DrnCoreEditor::CoreEditor_PointerPressed);

	coreEventToken[3] 
		= coreWindow->PointerMoved
		+= ref new TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &DrnCoreEditor::CoreEditor_PointerMoved);

	coreEventToken[4] 
		= coreWindow->PointerReleased
		+= ref new TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &DrnCoreEditor::CoreEditor_PointerReleased);

	coreEventToken[5] 
		= this->Unloaded
		+= ref new RoutedEventHandler(this, &DrnCoreEditor::CoreEditor_Unloaded);
}


void DrnCoreEditor::editorScrollViewer_ViewChanging(Platform::Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangingEventArgs^ e)
{
	EditorViewChanging(e->FinalView->VerticalOffset);
}


void Just_Editor_UWP::DrnCoreEditor::editorScrollViewer_ViewChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e)
{
	EditorViewChanging(editorScrollViewer->VerticalOffset);
}


void Just_Editor_UWP::DrnCoreEditor::menuItem_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	coreEditorFlyout->Hide();

	switch (Drn_UWP::getChildNum(coreEditorMenu->Children, (UIElement^)sender))
	{
	case 0:
		if (IsTextSelected())
			Cut();
		break;
	case 1:
		if (IsTextSelected())
			Copy();
		break;
	case 2:
		Paste();
		break;
	case 4:
		SelectAll();
		break;
	}
}
