//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "DrnControls/DrnTabPanel.xaml.h"

namespace Just_Editor_UWP
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
		void ReadToEditor(Windows::Storage::StorageFile^ thisFile)
		{
			if (thisFile != nullptr)
				tabPanel->HomePage_NewTabRequested(thisFile->Name, thisFile);
		}

	private:
		DrnTabPanel^ tabPanel;
	};
}
