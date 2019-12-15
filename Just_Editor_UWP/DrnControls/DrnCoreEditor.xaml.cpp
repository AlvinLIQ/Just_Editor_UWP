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
using namespace Windows::UI::Text::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

bool isIME = false, isIMEStopped = true;


DrnCoreEditor::DrnCoreEditor()
{
	InitializeComponent();

	isSmartDetectEnabled = ((App^)App::Current)->AppConfig->IsSmartDetectEnabled;
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
			RemoveFocus();
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

	
	wchar_t	virtualWChar = (wchar_t)e->VirtualKey;


	if (isCtrlHeld)
	{
		switch (virtualWChar)
		{
		case L'F':
			if (searchFlyout != nullptr)
			{
				RemoveFocus();

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
		case L'Z':
			if (!isShiftHeld)
			{
				Undo();
				break;
			}
		case L'Y':
			Redo();
			break;
		}
	}
	else
	{
		std::wstring wStr;
		virtualWChar %= 128;
		if (virtualWChar != 13 && (e->KeyStatus.IsExtendedKey || virtualWChar < 32))
		{
			switch (virtualWChar)
			{
			case 16:
				isShiftHeld = true;
				
				break;
			case 17:
				isCtrlHeld = true;

				break;
			case 27://Escape
				if (IdentifiersList->Width)
					IdentifiersList->Hide();
				break;
			case 8://Backspace
				if (IsTextSelected())
					ClearSelection();
				else if (cursor)
				{
					wStr = currentBlock->Content->ToString()->Data();

					currentBlock->Content = ref new String((wStr.substr(0, cursor - 1) + wStr.substr(cursor, currentLength - cursor)).c_str());
					cursor--;
					currentLength--;
					cursorX -= GetWCharWidth(wStr[cursor]);
					EditorTextChanged();
					UpdateCursor();

					CheckAction(2);
					SetAction(wStr[cursor] + currentAction.Text);
				}
				else if (currentLine)
				{
					currentBlock = (TXTBLOCK^)textChildren->Items->GetAt(currentLine - 1);
					wStr = currentBlock->Content->ToString()->Data();
					cursor = (unsigned int)wStr.length();
					if (currentLength)
					{
						currentBlock->Content += ((TXTBLOCK^)textChildren->Items->GetAt(currentLine))->Content->ToString();
					}
					currentLength += cursor;
					cursorX = GetCursorXFromWStr(wStr, cursor);
					RemoveLine(currentLine--);
					CursorChanged(cursor, currentLine, textChildren->Items->Size);
					EditorTextChanged();

					MoveToNextAction();
					currentAction.ActionMode = 4;
					SetAction(L"\r");
				}
				else
					break;

				AutoDetect();
				break;
			case 37://Left
				if (isShiftHeld)
				{
					if (cursor)
					{
						wStr = currentBlock->Content->ToString()->Data();
						cursorX = GetCursorXFromWStr(wStr, --cursor);
						Select(cursor, currentLine);
					}
					else if (currentLine)
					{
						currentBlock = (TXTBLOCK^)textChildren->Items->GetAt(--currentLine);
						wStr = currentBlock->Content->ToString()->Data();
						cursorX = GetCursorXFromWStr(wStr, cursor = (currentLength = (unsigned int)wStr.length()));
						Select(cursor, currentLine);
					}
					UpdateCursor();
				}
				else
					MoveLeft();

				break;
			case 39://Right
				if (isShiftHeld)
				{
					if (cursor < currentLength)
					{
						wStr = currentBlock->Content->ToString()->Data();
						cursorX = GetCursorXFromWStr(wStr, ++cursor);
						Select(cursor, currentLine);
						UpdateCursor();
					}
					else if (currentLine + 1 < textChildren->Items->Size)
					{
						currentBlock = (TXTBLOCK^)textChildren->Items->GetAt(++currentLine);
						currentLength = currentBlock->Content->ToString()->Length();
						cursorX = 0;
						Select(cursor = 0, currentLine);

						UpdateCursor();
					}
				}
				else
					MoveRight();

				break;
			case 38://Up
				if (IdentifiersList->Width)
				{
					IdentifiersList->SelectPre();
				}
				else if (currentLine)
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
			case 40://Down
				if (IdentifiersList->Width)
				{
					IdentifiersList->SelectNxt();
				}
				else if (currentLine + 1 < textChildren->Items->Size)
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
			case 76://Home
				MoveTo(0, currentLine);

				break;
			case 71://End
				MoveTo(-1, currentLine);

				break;
			case 46://Delete
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
						EditorTextChanged();

						CheckAction(3);
						SetAction(currentAction.Text + wStr[cursor]);
					}
					else if (currentLine + 1 < textChildren->Items->Size)
					{
						currentLength += GetLineStrLength(currentLine + 1);
						currentBlock->Content += GetLineStr(currentLine + 1);
						RemoveLine(currentLine + 1);
						CursorChanged(cursor, currentLine, textChildren->Items->Size);
						EditorTextChanged();

						MoveToNextAction();
						currentAction.ActionMode = 4;
						SetAction(L"\n");
					}
					else
						break;

					AutoDetect();
				}
				break;
			}
		}
		else if (virtualWChar == 13)
		{
			if (IsTextSelected())
				ClearSelection();
			AppendWCharAtCursor((wchar_t)13);
		}

	}
}

void DrnCoreEditor::CoreEditor_KeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e)
{
	switch (e->VirtualKey)
	{
	case VirtualKey::Shift:
		isShiftHeld = false;

		break;
	case VirtualKey::Control:
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
			tRect->SetSelection(0, selLine * fHeight, selPosition.X == GetLineWidth(selLine) ? selPosition.X + fWidth / 2 : selPosition.X);
		}

	}

	std::wstring wStr = currentBlock->Content->ToString()->Data();

	if (selLine == ln)
	{
		if (cursorX > (double)selPosition.X)
		{
			selEnd = cursorX;
			selOffset = (double)selPosition.X;
		}
		else
		{
			selOffset = cursorX;
			selEnd = (double)selPosition.X;
		}
	}
	else if (ln > selLine)
	{
		selEnd = cursorX;
		selOffset = 0;
	}
	else
	{
		selOffset = cursorX;
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
		cursorX -= GetWCharWidth(currentBlock->Content->ToString()->Data()[cursor]);
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
		cursorX += GetWCharWidth(currentBlock->Content->ToString()->Data()[cursor]);
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

	cursor = col > currentLength ? currentLength : col;
	cursorX = GetCursorXFromWStr(wStr, cursor);

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

void DrnCoreEditor::AppendWCharAtCursor(wchar_t newWChar, bool withAction)
{
	if (newWChar == L'\r')
		newWChar = L'\n';

	if (newWChar == L'\n')
	{
		if (IdentifiersList->Width && IdentifiersList->IsSelected)
		{
			IdentifiersList->NotifyWordUpdate();
			return;
		}
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
		if (withAction)
		{
			MoveToNextAction();

			currentAction.ActionMode = 5;
			currentAction.Text = L"\n";
			currentAction.Line = currentLine;
		}
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
		cursorX += GetWCharWidth(newWChar);
		
		if (withAction)
		{
			CheckAction(0);
			SetAction(currentAction.Text + newWChar);
		}
	}

	NotifyEditorUpdate();
}

void DrnCoreEditor::AppendUserWchar(unsigned int keyCode, bool isDown)
{
	virtualKeyCode = keyCode;
	if (isDown)
		DrnCoreEditor::CoreEditor_KeyDown(coreWindow.Get(), nullptr);
	else
		DrnCoreEditor::CoreEditor_KeyUp(coreWindow.Get(), nullptr);
}

void DrnCoreEditor::AppendStrAtCursor(const wchar_t *newWStr, bool withAction)
{
//	std::wstring wStr = newWStr;
	unsigned int tLen;
	Platform::String^ tLineStr = L"";
	std::wstring afterCursor = L"";

	if (currentLength > cursor)
	{
		afterCursor = currentBlock->Content->ToString()->Data();

		afterCursor = afterCursor.substr(cursor, tLen = currentLength - cursor);
		currentLength = cursor;
	}
	if (withAction)
	{
		MoveToNextAction();
		currentAction.ActionMode = 6;
		SetAction(L"");
	}
	for (unsigned int sIndex = 0; newWStr[sIndex]; sIndex++)
	{
		if (newWStr[sIndex] == L'\r' && newWStr[sIndex + 1] == L'\n')
		{
			sIndex++;
		}
		if (newWStr[sIndex] == L'\n' || newWStr[sIndex] == L'\r')
		{
			currentBlock->Content += tLineStr;
			if (withAction)
			{
				currentAction.Text += tLineStr + L"\n";
				currentAction.ActionMode++;
			}
			tLineStr = L"";
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
				for (unsigned int i = 4 - cursor % 4; i-- > 0;)
				{
					tLineStr += L" ";
					currentLength++, cursor++, cursorX += fWidth;
				}
			}
			else
			{
				tLineStr = tLineStr + newWStr[sIndex];
				cursor++;
				currentLength++;
				cursorX += GetWCharWidth(newWStr[sIndex]);
			}
		}
	}
	if (afterCursor[0])
	{
		tLineStr += ref new String(afterCursor.c_str());
		currentLength += tLen;
	}

	currentBlock->Content += tLineStr;

	if (withAction)
		currentAction.Text += tLineStr;

	NotifyEditorUpdate();
}


void DrnCoreEditor::EditorContent_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	isTapped = true;
	SetFocus();
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
	if (!doubleClickLock && curPoint->Timestamp - pointTimeStamp < 200000 && abs(curPosition.X - selPosition.X) < fWidth && (unsigned int)(curPosition.Y / fHeight) == (unsigned int)(selPosition.Y / fHeight))
	{
		doubleClickLock = true;

		auto thisWordRange = Drn_UWP::GetWordPosition(currentBlock->Content->ToString(), currentLength, cursor);

		if (thisWordRange.EndCaretPosition != thisWordRange.StartCaretPosition)
		{
			auto wCStr = currentBlock->Content->ToString()->Data();
			selPosition.X = (float)(cursorX - GetCursorXFromWStr(&wCStr[thisWordRange.StartCaretPosition], cursor - (unsigned int)thisWordRange.StartCaretPosition));
			cursorX += GetCursorXFromWStr(&wCStr[cursor], (unsigned int)thisWordRange.EndCaretPosition - cursor);
			cursor += (unsigned int)thisWordRange.EndCaretPosition;
			UpdateCursor();
		}

		Select(thisWordRange.StartCaretPosition, currentLine);
	}
	else
	{
		doubleClickLock = false;

		curPosition.X += (float)(fWidth / 2);
		MoveToPosition(curPosition);
		selPosition.X = (float)cursorX;
		selPosition.Y = (float)currentLine * fHeight;
	}
	pointTimeStamp = curPoint->Timestamp;

//		insideKeyboard->Show();
}

void DrnCoreEditor::EditorContent_PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (coreWindow != nullptr)
		coreWindow->PointerCursor = ref new Windows::UI::Core::CoreCursor(Windows::UI::Core::CoreCursorType::IBeam, 0);
}

void DrnCoreEditor::EditorContent_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (coreWindow != nullptr)
		coreWindow->PointerCursor = ref new Windows::UI::Core::CoreCursor(Windows::UI::Core::CoreCursorType::Arrow, 0);
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

	if (coreTextContext == nullptr)
	{
		inputPane = Windows::UI::ViewManagement::InputPane::GetForCurrentView();

		coreTextContext = CoreTextServicesManager::GetForCurrentView()->CreateEditContext();
		coreTextContext->InputPaneDisplayPolicy = CoreTextInputPaneDisplayPolicy::Manual;
		coreTextContext->InputScope = CoreTextInputScope::Text;
		coreTextContext->TextRequested 
			+= ref new Windows::Foundation::TypedEventHandler<Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextTextRequestedEventArgs^>
			(this, &Just_Editor_UWP::DrnCoreEditor::CoreEditContext_TextRequested);
		coreTextContext->LayoutRequested 
			+= ref new Windows::Foundation::TypedEventHandler<Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextLayoutRequestedEventArgs^>
			([this](Windows::UI::Text::Core::CoreTextEditContext ^ sender, Windows::UI::Text::Core::CoreTextLayoutRequestedEventArgs ^ args) 
				{
					float scaleFactor = (float)Windows::Graphics::Display::DisplayInformation::GetForCurrentView()->RawPixelsPerViewPixel;
					Rect controlRect = Rect((coreWindow->Bounds.X + (float)(cursorTrans->X - editorScrollViewer->HorizontalOffset)) * scaleFactor, 
						(coreWindow->Bounds.Y + (float)(cursorTrans->Y - editorScrollViewer->VerticalOffset) + 75) * scaleFactor,
						textChildren->RenderSize.Width, fHeight);

					auto request = args->Request;
					request->LayoutBounds->ControlBounds = controlRect;
					request->LayoutBounds->TextBounds = controlRect;
				});
		coreTextContext->SelectionRequested 
			+= ref new Windows::Foundation::TypedEventHandler<Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextSelectionRequestedEventArgs^>
			([](Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextSelectionRequestedEventArgs^ args)
				{
					
				});
		coreTextContext->CompositionStarted 
			+= ref new Windows::Foundation::TypedEventHandler<Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextCompositionStartedEventArgs^>
			([this](Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextCompositionStartedEventArgs^ args)
				{
					isIME = true;
					isIMEStopped = false;
					lastRange.StartCaretPosition = 0;
					lastRange.EndCaretPosition = 0;
				});
		coreTextContext->CompositionCompleted
			+= ref new Windows::Foundation::TypedEventHandler<Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextCompositionCompletedEventArgs^>
			([this](Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextCompositionCompletedEventArgs^ args)
				{
					isIME = false;
					CoreTextRange tRange, nRange;
					tRange.StartCaretPosition = 0;
					tRange.EndCaretPosition = lastRange.EndCaretPosition;
					nRange.StartCaretPosition = 0;
					nRange.EndCaretPosition = 0;
					sender->NotifyTextChanged(tRange, 0, nRange);
					if (isIMEStopped)
						AppendStrAtCursor(MsgTest->Content->ToString()->Data());
					else
						isIMEStopped = true;
					MsgTest->Content = L"";
				});
	}
	coreEventToken[5]
		= coreTextContext->TextUpdating
		+= ref new Windows::Foundation::TypedEventHandler<Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextTextUpdatingEventArgs^>(this, &Just_Editor_UWP::DrnCoreEditor::CoreEditContext_TextUpdating);

}

void DrnCoreEditor::CoreEditContext_TextRequested(Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextTextRequestedEventArgs^ args)
{
}

void DrnCoreEditor::CoreEditContext_TextUpdating(Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextTextUpdatingEventArgs^ args)
{
	if (IsTextSelected())
		ClearSelection();
	wchar_t fChar = *args->Text->Data();
	if (!fChar)
		return;
//	MsgTest->Text = args->InputLanguage->LanguageTag;
	if (inputPane->Visible)
	{
		AppendStrAtCursor(&fChar);
	}
	else if (isIME && (fChar >= 65 || fChar == '\''))
	{
		MsgTrans->X = cursorTrans->X;
		MsgTrans->Y = cursorTrans->Y;

		bool isAscii = fChar < 128;
		if (isAscii && args->Range.EndCaretPosition == args->Range.StartCaretPosition)
		{
			MsgTest->Content += args->Text;
			isIMEStopped = false;
		}
		else if (isAscii && args->NewSelection.EndCaretPosition < lastRange.EndCaretPosition)
		{
			std::wstring imeStr = MsgTest->Content->ToString()->Data();
			MsgTest->Content = ref new Platform::String(imeStr.substr(0, imeStr.length() - (args->Range.EndCaretPosition - args->Range.StartCaretPosition)).c_str());
			isIMEStopped = MsgTest->Content == L"";
			if (lastRange.EndCaretPosition > 1)
				MsgTest->Content += args->Text;
		}
		else
		{
			MsgTest->Content = args->Text;
			isIMEStopped = true;
		}
	}
	else
	{
		AppendWCharAtCursor(fChar);
		CoreTextRange tRange, nRange;
		tRange.StartCaretPosition = 0;
		tRange.EndCaretPosition = args->NewSelection.EndCaretPosition;
		nRange.StartCaretPosition = 0;
		nRange.EndCaretPosition = 0;
		sender->NotifyTextChanged(tRange, 0, nRange);
	}
	lastRange.EndCaretPosition = args->NewSelection.EndCaretPosition;
}

void DrnCoreEditor::UnloadEditor()
{
	if (coreWindow == nullptr)
		return;

	coreWindow->KeyDown -= coreEventToken[0];
	coreWindow->KeyUp -= coreEventToken[1];
	coreWindow->PointerPressed -= coreEventToken[2];
	coreWindow->PointerMoved -= coreEventToken[3];
	coreWindow->PointerReleased -= coreEventToken[4];
	coreWindow->PointerCursor = ref new Windows::UI::Core::CoreCursor(Windows::UI::Core::CoreCursorType::Arrow, 0);

	coreTextContext->TextUpdating -= coreEventToken[5];
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


void Just_Editor_UWP::DrnCoreEditor::IdentifiersList_WordRequested(Platform::String^ str, default::uint32 x, default::uint32 y)
{
	if (x != cursor || y != currentLine)
		MoveTo(x, y);
	
	AppendStrAtCursor(str->Data());
}
