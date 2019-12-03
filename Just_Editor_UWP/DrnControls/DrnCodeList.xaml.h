//
// DrnCodeList.xaml.h
// Declaration of the DrnCodeList class
//

#pragma once

#include "DrnControls/DrnCodeList.g.h"
#include "DrnHeaders/Drn_UWP.h"

#define IdentifiersSize 41

namespace Just_Editor_UWP
{
	public delegate void StringAndSizeEventHandler(Platform::String^ str, unsigned int x, unsigned int y);

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnCodeList sealed
	{
	public:
		DrnCodeList();
		void GetWordFromPosition(Platform::String^ srcStr, unsigned int srcLen, unsigned int col, unsigned int ln)
		{
			std::wstring wStr = srcStr->Data();
			currentWord = L"";

			currentX = col;
			currentY = ln;
			unsigned int tPos;
			
			for (tPos = col; tPos > 0;)
			{
				wchar_t tChar = wStr[--tPos];
				if (IsSplit(tChar))
					break;

				currentWord = tChar + currentWord;
			}
			for (tPos = col; tPos < srcLen;)
			{
				wchar_t tChar = wStr[tPos++];
				if (IsSplit(tChar))
					break;

				currentX++;
				currentWord += tChar;
			}
		}
		void IdentifiersDetect()
		{
			codeListPanel->Children->Clear();
			if (!currentWord[0])
			{
				Hide();
				return;
			}
			size_t cLen = currentWord.length();
			for (int i = 0; i < IdentifiersSize; i++)
			{
				if (cLen < identifiersMap[i].length() && currentWord == identifiersMap[i].substr(0, cLen))
				{
					auto tItem = ref new Windows::UI::Xaml::Controls::ContentPresenter;
					tItem->Height = 23;
					tItem->Content = ref new Platform::String(identifiersMap[i].c_str());
					codeListPanel->Children->Append(tItem);
				}
			}
			codeListPanel->Children->Size ? this->Show() : this->Hide();
		}
		void SelectAt(unsigned int nIndex)
		{
			if (codeSelectedHighlight->Fill != codeSelectedHighlight->Stroke)
			{
				isSelected = true;
				codeSelectedHighlight->Fill = codeSelectedHighlight->Stroke;
			}
			if (nIndex < codeListPanel->Children->Size)
				selectedTrans->Y = nIndex * 23;
		}
		void SelectPre()
		{
			if (codeSelectedHighlight->Fill != codeSelectedHighlight->Stroke)
			{
				isSelected = true;
				codeSelectedHighlight->Fill = codeSelectedHighlight->Stroke;
			}
			else
			{
				unsigned int tIndex = (unsigned int)selectedTrans->Y / 23;
				if (tIndex && codeListPanel->Children->Size)
					selectedTrans->Y -= 23;
			}
		}
		void SelectNxt()
		{
			if (codeSelectedHighlight->Fill != codeSelectedHighlight->Stroke)
			{
				isSelected = true;
				codeSelectedHighlight->Fill = codeSelectedHighlight->Stroke;
			}
			else
			{
				unsigned int tIndex = (unsigned int)selectedTrans->Y / 23;
				if (tIndex + 1 < codeListPanel->Children->Size)
					selectedTrans->Y += 23;
			}
		}
		void Show()
		{
			isSelected = false;
			codeSelectedHighlight->Fill = ref new Windows::UI::Xaml::Media::SolidColorBrush();
			selectedTrans->Y = 0;
			this->Width = 150;
		}
		void Hide()
		{
			this->Width = 0;
		}
		void ClearWord()
		{
			codeListPanel->Children->Clear();
			currentWord = L"";
		}

		void NotifyWordUpdate()
		{
			auto newWord = std::wstring(((Windows::UI::Xaml::Controls::ContentPresenter^)codeListPanel->Children->GetAt((unsigned int)selectedTrans->Y / 23))->Content->ToString()->Data());
			auto cLen = currentWord.length();
			WordRequested(
				ref new Platform::String(newWord.substr(cLen, newWord.length() - cLen).c_str())
				, currentX, currentY);
			this->Hide();
		}

		event StringAndSizeEventHandler^ WordRequested;

		property bool IsSelected
		{
			bool get()
			{
				return isSelected;
			}
		};
	private:
		std::wstring identifiersMap[IdentifiersSize] = { L"as", L"bool", L"byte", L"break", L"char", L"class", L"const", L"do", L"double", L"else", L"extern",
											L"for", L"false", L"float", L"foreach", L"if", L"in", L"int", L"internal", L"long",
											L"null", L"nullptr", L"namespace", L"out", L"object", L"public", L"private", L"protected",
											L"true",L"typedef", L"using", L"unsigned", L"virtual", L"while", L"wchar_t"
											L"#if", L"#ifdef", L"#ifndef", L"#endif", L"#include", L"#define", L"#pragma" };
		std::wstring currentWord = L"";

		bool isSelected = false;

		unsigned long long lastTimeStamp = 0;

		unsigned int currentX = 0, currentY = 0;

		void codeListPanel_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
};
}
