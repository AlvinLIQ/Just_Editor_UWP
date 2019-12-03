//
// DrnCoreEditor.xaml.h
// Declaration of the DrnCoreEditor class
//

#pragma once

#include "DrnControls/DrnCoreEditor.g.h"
#include "DrnCodeList.xaml.h"
#include "DrnCoreEditorSelectionBlock.xaml.h"

#define fHeight 23
#define fWidth ChrBlock->ActualWidth
//#define fWidth 10.799999237060547
#define UpdateCursor() SetCursor(cursorX, currentLine * fHeight)

#define TXTBLOCK Windows::UI::Xaml::Controls::ContentPresenter
#define newTextBlock NewTextBlock()

#define curWidth 3
#define CurrentLineWStr std::wstring(currentBlock->Content->ToString()->Data())
#define GetCurrentBlockLength() (unsigned int)currentBlock->Content->ToString()->Length()
#define GetNextBlock() ((TXTBLOCK^)textChildren->Items->GetAt(currentLine + 1))
#define IsTextSelected() selectionPanel->Children->Size
#define GetSelectionStartLine() (unsigned int)(selPosition.Y / fHeight)
#define CancelSelection() selectionPanel->Children->Clear()

namespace Just_Editor_UWP
{
	public delegate void CursorChangedEventHandler(unsigned int col, unsigned int ln, unsigned int lineNum);
	public delegate void EditorViewChangeEventHandler(double verticalOffset);
	public delegate void EditorTextChangedEventHandler();

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnCoreEditor sealed
	{
	public:
		DrnCoreEditor();
		event CursorChangedEventHandler^ CursorChanged;
		event EditorViewChangeEventHandler^ EditorViewChanging;
		event EditorTextChangedEventHandler^ EditorTextChanged;
		event EditorTextChangedEventHandler^ EditorSaveRequested;

		void AppendUserStr(Platform::String^ uStr)
		{
			AppendStrAtCursor(uStr->Data());
		}
		void AppendUserWchar(unsigned int keyCode, bool isDown);
		Platform::String^ GetLineStr(unsigned int line)
		{
			return ((TXTBLOCK^)textChildren->Items->GetAt(line))->Content->ToString();
		}

		void DrnKeyboard_drnKeyDown(default::uint32 keyCode)
		{
			this->AppendUserWchar(keyCode, true);
		}
		void DrnKeyboard_drnKeyUp(default::uint32 keyCode)
		{
			this->AppendUserWchar(keyCode, false);
		}

		void FindLastAndSelect(Platform::String^ targetStr)
		{
			unsigned int tLine = currentLine, tCur;
			std::wstring wStr = GetLineStr(tLine)->Data(), tStr = targetStr->Data();
			size_t fIndex, tLen = tStr.length();

			tCur = cursor ? cursor - 1 : 0;
			while ((fIndex = wStr.rfind(tStr, tCur)) == -1)
			{
				if (!tLine)
					return;

				wStr = GetLineStr(--tLine)->Data();
				tCur = (unsigned int)wStr.length();
			}
			MoveTo((unsigned int)fIndex, tLine);
			selPosition.X = (float)GetCursorXFromWStr(wStr, (unsigned int)(fIndex + tLen));
			selPosition.Y = (float)(tLine * fHeight);
			Select(cursor, tLine);
//			isActivated = true;
//			drnCursor->Opacity = 1;
		}

		void FindNextAndSelect(Platform::String^ targetStr)
		{
			unsigned int tLine = currentLine, tCur;
			std::wstring wStr = GetLineStr(tLine)->Data(), tStr = targetStr->Data();
			size_t fIndex, tLen = tStr.length();

			tCur = cursor;
			while ((fIndex = wStr.find(tStr, tCur)) == -1)
			{
				if (++tLine >= textChildren->Items->Size)
					return;

				tCur = 0;
				wStr = GetLineStr(tLine)->Data();
			}
			selPosition.X = (float)GetCursorXFromWStr(wStr, (unsigned int)fIndex);
			selPosition.Y = (float)(tLine * fHeight);
			MoveTo((unsigned int)(fIndex + tLen), tLine);
			Select(cursor, tLine);
//			isActivated = true;
//			drnCursor->Opacity = 1;
		}

		void Clear()
		{
			textChildren->Items->Clear();
			currentLength = 0;
			currentLine = 0;
			cursor = 0;
			cursorX = 0;

			UpdateCursor();
		}

		void SetFocus()
		{
			isActivated = true;
			drnCursor->Opacity = 1;
			coreTextContext->NotifyFocusEnter();
			inputPane->TryShow();
			Focus(Windows::UI::Xaml::FocusState::Programmatic);
		}
		void RemoveFocus()
		{
			isActivated = false;
			drnCursor->Opacity = 0;
			coreTextContext->NotifyFocusLeave();
			inputPane->TryHide();
//			Focus(Windows::UI::Xaml::FocusState::Unfocused);
		}

		unsigned int GetCurrentLine()
		{
			return currentLine;
		}

		property Windows::UI::Xaml::Controls::Flyout^ searchFlyout;
	private:
		Windows::UI::Xaml::Media::SolidColorBrush^ IdentifiersHighlightColor = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::DeepSkyBlue);

		TXTBLOCK^ currentBlock = nullptr;

		Platform::Agile<Windows::UI::Core::CoreWindow^> coreWindow;
		Windows::UI::Text::Core::CoreTextEditContext^ coreTextContext = nullptr;

		Platform::Agile<Windows::UI::ViewManagement::InputPane^> inputPane;

		Windows::Foundation::EventRegistrationToken coreEventToken[6];
		
		Windows::Foundation::Point selPosition;

		Windows::UI::Text::Core::CoreTextRange lastRange;

		bool isTapped = false;
		bool isShiftHeld = false;
		bool isCtrlHeld = false;
		bool isWord = false;
		bool isActivated = false;
		bool isSmartDetectEnabled;

		unsigned long long int pointTimeStamp;

		float leftMargin = 0, topMargin = 0;
		unsigned int currentLine = 0, cursor = 0, currentLength = 0, virtualKeyCode = -1, lastWordLen = 0;
		double cursorX = 0, thisWordX = 0;

		void CoreEditor_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CoreEditor_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CoreEditor_KeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e);
		void CoreEditor_KeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e);
		void CoreEditor_PointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void CoreEditor_PointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void CoreEditor_PointerMoved(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void CoreEditContext_TextRequested(Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextTextRequestedEventArgs^ args);
		void CoreEditContext_TextUpdating(Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextTextUpdatingEventArgs^ args);
		void UnloadEditor();
		void SetCursor(double x, double y)
		{
			cursorTrans->X = x;
			cursorTrans->Y = y;
			lineTrans->Y = y;
			CursorChanged(cursor, currentLine, textChildren->Items->Size);
			if (x > editorScrollViewer->HorizontalOffset + editorScrollViewer->ViewportWidth)
				editorScrollViewer->ChangeView(x - editorScrollViewer->ViewportWidth, editorScrollViewer->VerticalOffset, editorScrollViewer->ZoomFactor);
			else if (x < editorScrollViewer->HorizontalOffset)
				editorScrollViewer->ChangeView(x > fWidth ? x - fWidth : 0, editorScrollViewer->VerticalOffset, editorScrollViewer->ZoomFactor);
			else if (y + fHeight > editorScrollViewer->VerticalOffset + editorScrollViewer->ViewportHeight)
				editorScrollViewer->ChangeView(editorScrollViewer->HorizontalOffset, y + fHeight - editorScrollViewer->ViewportHeight, editorScrollViewer->ZoomFactor);
			else if (y < editorScrollViewer->VerticalOffset)
				editorScrollViewer->ChangeView(editorScrollViewer->HorizontalOffset, y, editorScrollViewer->ZoomFactor);
		}
		void Select(unsigned int col, unsigned int ln);
		void SelectAll()
		{
			selPosition.X = 0;
			selPosition.Y = 0;
			MoveTo(GetLineStrLength(textChildren->Items->Size - 1), textChildren->Items->Size - 1);
			Select(cursor, currentLine);
			((DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(0))->SetSelection(0, 0, GetLineWidth(0));
		}
		void MoveLeft();
		void MoveRight();
		void MoveTo(unsigned int col, unsigned int ln);
		void MoveToPosition(Windows::Foundation::Point newPos);
		TXTBLOCK^ NewTextBlock()
		{
			auto tBlock = ref new TXTBLOCK;
			tBlock->Height = fHeight;

			return tBlock;
		}
		void AppendBlockAtEnd();
		void AppendWCharAtCursor(wchar_t newWChar);
		void AppendStrAtCursor(const wchar_t* newWStr);

		bool GetHighlihgtStatus(TXTBLOCK^ block)
		{
			return block->Foreground == (Windows::UI::Xaml::Media::Brush^)IdentifiersHighlightColor;
		}
		unsigned int GetColFromX(std::wstring wStr, double x)
		{
			double curX = 0;
			unsigned int sIndex = 0, result = 0;
			while (wStr[sIndex])
			{
				curX += GetWCharWidth(wStr[sIndex++]);
				if (curX > x)
					break;

				result = sIndex;
			}

			return result;
		}
		double GetCursorXFromWStr(std::wstring wStr, unsigned int sLen)
		{
			double curX = 0;
			unsigned int curOffset = 0;
			while (sLen > curOffset)
				curX += GetWCharWidth(wStr[--sLen]);

			return curX;
		}
		double GetWCharWidth(wchar_t tWChar)
		{
			if (tWChar == 32)
				return fWidth;
			WChrBlock->Text = tWChar.ToString();
			WChrBlock->Measure(Windows::Foundation::Size(500, 500));
			return (double)WChrBlock->ActualWidth;
		}

		void UpdateCursorX(std::wstring wStr, double x)
		{
			double curX = 0;
			unsigned int sIndex = 0;
			cursor = 0;
			cursorX = 0;
			while (wStr[sIndex])
			{
				curX += GetWCharWidth(wStr[sIndex++]);
				if (curX > x)
					break;

				cursor = sIndex;
				cursorX = curX;
			}
		}

		unsigned int GetLineStrLength(unsigned int line)
		{
			if (line >= textChildren->Items->Size)
				line = textChildren->Items->Size - 1;

			return (unsigned int)((TXTBLOCK^)textChildren->Items->GetAt(line))->Content->ToString()->Length();
		}
		double GetLineWidth(unsigned int line, unsigned int offset = 0)
		{
			Platform::String^ lineStr = GetLineStr(line);
			return GetCursorXFromWStr(&lineStr->Data()[offset], lineStr->Length());
		}

		void RemoveLine(unsigned int line)
		{
			textChildren->Items->RemoveAt(line);
			UpdateCursor();
		}

		Platform::String^ GetSelectionStr()
		{
			Platform::String^ result = L"";
			std::wstring wStr;
			if (cursorTrans->Y <= selPosition.Y)
				for (unsigned int i = selectionPanel->Children->Size, curLine, curOffset; i;)
				{
					auto lSelection = (DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(--i);
					curLine = (unsigned int)(lSelection->GetY() / fHeight);
					wStr = GetLineStr(curLine)->Data();
					curOffset = GetColFromX(wStr, lSelection->GetX());
					result += ref new Platform::String(wStr.substr(curOffset, GetColFromX(wStr, lSelection->ActualWidth + lSelection->GetX())).c_str());
					if (i)
						result += "\n";
				}
			else
				for (unsigned int i = 0, curLine, curOffset; i < selectionPanel->Children->Size; i++)
				{
					auto lSelection = (DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(i);
					curLine = (unsigned int)(lSelection->GetY() / fHeight);
					wStr = GetLineStr(curLine)->Data();
					curOffset = GetColFromX(wStr, lSelection->GetX());
					result += ref new Platform::String(wStr.substr(curOffset, GetColFromX(wStr, lSelection->ActualWidth + lSelection->GetX())).c_str());
					if (i + 1 < selectionPanel->Children->Size)
						result += "\n";
				}
			return result;
		}
		void ClearSelection()
		{
			if (!selectionPanel->Children->Size)
				return;

			unsigned int tIndex;
			if (selectionPanel->Children->Size > 1)
			{
				std::wstring wStr;
				unsigned int selLine = GetSelectionStartLine();
				double nxtWidth;
				if (selLine < currentLine)
				{
					tIndex = (unsigned int)((((DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(1))->GetY()) / fHeight);
					MoveTo(0, selLine);
					wStr = currentBlock->Content->ToString()->Data();
					UpdateCursorX(wStr, ((DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(0))->GetX());

					nxtWidth = ((DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(selectionPanel->Children->Size - 1))->ActualWidth;
				}
				else
				{
					tIndex = (unsigned int)((((DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(selectionPanel->Children->Size - 2))->GetY()) / fHeight);
					wStr = currentBlock->Content->ToString()->Data();

					nxtWidth = ((DrnCoreEditorSelectionBlock^)selectionPanel->Children->GetAt(0))->ActualWidth;
				}
				currentBlock->Content = ref new Platform::String(wStr.substr(0, currentLength = cursor).c_str());
				while (selectionPanel->Children->Size > 2)
				{
					RemoveLine(tIndex);
					selectionPanel->Children->RemoveAt(1);
				}

				//Clear SeletionEnd Line
				wStr = GetNextBlock()->Content->ToString()->Data();
				selLine = GetColFromX(wStr, nxtWidth + fWidth / 2);
				tIndex = (unsigned int)wStr.length();
				if (tIndex > selLine)
				{
					currentBlock->Content += ref new Platform::String(wStr.substr(selLine, tIndex -= selLine).c_str());
					currentLength += tIndex;
				}
				RemoveLine(currentLine + 1);
			}
			else
			{
				std::wstring wStr = currentBlock->Content->ToString()->Data();
				if (selPosition.X > cursorX)
				{
					tIndex = GetColFromX(wStr, selPosition.X + fWidth / 2);
				}
				else
				{
					tIndex = cursor;
					UpdateCursorX(wStr, selPosition.X + fWidth / 2);
					UpdateCursor();
				}
				currentBlock->Content = ref new Platform::String((wStr.substr(0, cursor) + wStr.substr(tIndex, currentLength - tIndex)).c_str());
				currentLength -= tIndex - cursor;
			}
			EditorTextChanged();
			
			CancelSelection();
	//		HighlightDetect(currentBlock);
		}
		void Copy()
		{
			auto dataPkg = ref new Windows::ApplicationModel::DataTransfer::DataPackage;
			dataPkg->SetText(GetSelectionStr());
			Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(dataPkg);
		}
		void Cut()
		{
			Copy();
			ClearSelection();
		}
		void Paste()
		{
			if (IsTextSelected())
				ClearSelection();
			auto dataView = Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();
			if (dataView->Contains(Windows::ApplicationModel::DataTransfer::StandardDataFormats::Text))
				concurrency::create_task(dataView->GetTextAsync()).then([this](Platform::String^ clipStr)
					{
						if (clipStr != nullptr && clipStr != L"")
						{
							AppendStrAtCursor(clipStr->Data());
						}
					}, concurrency::task_continuation_context::use_current());
		}
		void EditorContent_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void EditorContent_PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void EditorContent_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void editorScrollViewer_ViewChanging(Platform::Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangingEventArgs^ e);
		void editorScrollViewer_ViewChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e);
		void menuItem_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void AutoDetect()
		{
			if (isSmartDetectEnabled)
			{
				WordsTrans->X = cursorX;
				WordsTrans->Y = cursorTrans->Y + fHeight;
				IdentifiersList->GetWordFromPosition(currentBlock->Content->ToString(), currentLength, cursor, currentLine);
				IdentifiersList->IdentifiersDetect();
			}
		}
		void NotifyEditorUpdate()
		{
			EditorTextChanged();

			selPosition.X = (float)cursorX;
			selPosition.Y = (float)(currentLine * fHeight);
			UpdateCursor();

			AutoDetect();
		}
		void IdentifiersList_WordRequested(Platform::String^ str, default::uint32 x, default::uint32 y);
};
}
