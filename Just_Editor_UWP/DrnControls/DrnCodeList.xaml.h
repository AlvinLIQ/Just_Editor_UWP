//
// DrnCodeList.xaml.h
// Declaration of the DrnCodeList class
//

#pragma once

#include "DrnControls/DrnCodeList.g.h"
#include "DrnHeaders/Drn_UWP.h"


namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnCodeList sealed
	{
	public:
		DrnCodeList();
		void GetWordFromPosition(Platform::String^ srcStr, unsigned int srcLen, unsigned int curPos)
		{
			std::wstring wStr = srcStr->Data();
			currentWord = L"";

			unsigned int tPos;
			for (tPos = curPos; tPos > 0;)
			{
				wchar_t tChar = wStr[--tPos];
				if (IsSplit(tChar))
					break;

				currentWord = tChar + currentWord;
			}
			for (tPos = curPos; tPos < srcLen;)
			{
				wchar_t tChar = wStr[tPos++];
				if (IsSplit(tChar))
					break;

				currentWord += tChar;
			}
		}
		void IdentifiersDetect()
		{
			codeListPanel->Items->Clear();
			if (!currentWord[0])
			{
				Hide();
				return;
			}
			size_t cLen = currentWord.length();
			for (int i = 0; i < 13; i++)
			{
				if (cLen < identifiersMap[i].length() && currentWord == identifiersMap[i].substr(0, cLen))
					codeListPanel->Items->Append(ref new Platform::String(identifiersMap[i].c_str()));
			}
			codeListPanel->Items->Size ? this->Show() : this->Hide();
		}
		void Show()
		{
			this->Width = 150;
		}
		void Hide()
		{
			this->Width = 0;
		}
		void ClearWord()
		{
			codeListPanel->Items->Clear();
			currentWord = L"";
		}
	private:
		std::wstring identifiersMap[13] = { L"const", L"if", L"int", L"internal", L"else", L"extern", L"char", L"wchar_t", L"#include", L"#define", L"using", L"namespace", L"class" };
		std::wstring currentWord = L"";
	};
}
