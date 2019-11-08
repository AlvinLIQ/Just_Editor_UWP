//
// DrnTabPanel.xaml.cpp
// Implementation of the DrnTabPanel class
//

#include "pch.h"
#include "DrnTabPanel.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Controls;

double newWidth = 0;
unsigned int selectedIndex;


// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DrnTabPanel::DrnTabPanel()
{
	InitializeComponent();
	homePage = ref new HomePage(this->justEditorBtn);
	homePage->NewTabRequested
		+= ref new Just_Editor_UWP::NewTabRequestEventHandler(this, &DrnTabPanel::HomePage_NewTabRequested);
	if (pagePanel->Content == nullptr)
		pagePanel->Content = homePage;

	//forTab
	auto leaveEvent = ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>([](CoreWindow^ coreWindow, Windows::UI::Core::PointerEventArgs^ e)
		{
			SetOldIndex(-1);
		});
	auto coreWindow = CoreWindow::GetForCurrentThread();
	coreWindow->PointerCaptureLost += leaveEvent;
	coreWindow->PointerReleased += leaveEvent;
}

void DrnTabPanel::AddNewTab(Platform::String^ title, Windows::UI::Xaml::UIElement^ content, Windows::Storage::StorageFile^ tabFile)
{
	auto tTab = ref new DrnTab;
	tTab->Width = TabWidth;
	tTab->MaxWidth = TabWidth;
	tTab->Title = title;
	tTab->PageContent = content;
	tTab->BorderBrush = this->HoverBrush;

	tTab->tabClicked 
		+= ref new TabButtonClickedEventHandler(this, &DrnTabPanel::drnTab_Clicked);
	tTab->clsBtnClicked 
		+= ref new TabButtonClickedEventHandler(this, &DrnTabPanel::drnTabClsBtn_Clicked);


	tabPanel->Children->Append(tTab);
	Select(tTab, tabPanel->Children->Size - 1);
}

void DrnTabPanel::drnBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	HomePage_NewTabRequested(L"New File", nullptr);
}

void DrnTabPanel::Select(DrnTab^ targetTab, unsigned int tabIndex = -1)
{
	if (targetTab->isSelected)
		return;

	if (!~tabIndex)
		tabIndex = Drn_UWP::getChildNum(tabPanel->Children, targetTab);
	selectedIndex = tabIndex;

	if (SelectedItem != nullptr)
	{
		SelectedItem->isSelected = false;
		SelectedItem->Background = ref new SolidColorBrush;
	}
	targetTab->isSelected = true;
	targetTab->Background = SelectedColor;
	pagePanel->Content = targetTab->PageContent;
	SelectedItem = targetTab;
	tabScrollViewer_SizeChanged(nullptr, nullptr);
}

void DrnTabPanel::SelectAt(unsigned int page_n)
{
	if(page_n >= tabPanel->Children->Size)
		return;

	Select((DrnTab^)tabPanel->Children->GetAt(page_n), page_n);
}

void DrnTabPanel::RemoveAt(unsigned int page_n)
{
	if ((DrnTab^)tabPanel->Children->GetAt(page_n) == SelectedItem)
	{
//		pagePanel->Content = nullptr;
		if (page_n)
			SelectAt(page_n - 1);
		else if (tabPanel->Children->Size > 1)
			SelectAt(1);
		else
			pagePanel->Content = homePage;

	}
	tabPanel->Children->RemoveAt(page_n);
}

void DrnTabPanel::drnTab_Clicked(DrnTab^ tab)
{
	Select(tab);
}

void DrnTabPanel::drnTabClsBtn_Clicked(DrnTab^ tab)
{
	RemoveAt(Drn_UWP::getChildNum(tabPanel->Children, tab));
}

void DrnTabPanel::HomePage_NewTabRequested(Platform::String^ tabName, Windows::Storage::StorageFile^ tabFile)
{
	const wchar_t* wTabName = tabName->Data();
	switch (wTabName[0])
	{
	case L'?':
		switch (wTabName[1])
		{
		case L'S':
			AddNewTab(ref new String(tabName->Data() + 1), ref new MsgPage, nullptr);
			break;
		case L'C':
			AddNewTab(ref new String(tabName->Data() + 1), ref new CryptPage, nullptr);
			break;
		}
		break;
	default:
		EditorPage^ editorPage;
		AddNewTab(tabName, editorPage = ref new EditorPage(tabFile), tabFile);
		editorPage->thisTab = SelectedItem;
		editorPage->thisTab->FileDialog = ref new DrnFileDialog(L"File Info", tabName, tabFile);
		editorPage->thisTab->FileDialog->InfoChanged += ref new InfoChangedHandler([editorPage](DrnFileDialog^ FileDialog)
			{
				editorPage->thisFile = FileDialog->DialogFile;
				editorPage->thisTab->Title = FileDialog->dialogTitle;
			});
		editorPage->thisTab->MakeSureDialog = ref new DrnContentDialog(L"Before closing", L"Close&Save", L"Close without saving", L"Cancel");
		editorPage->thisTab->MakeSureDialog->PrimaryButtonClick += 
			ref new Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::ContentDialog^, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^>([editorPage](Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ e)
				{
					editorPage->drnCodeEditor_EditorSavedRequested();
					editorPage->thisTab->CloseTab();
				});
		editorPage->thisTab->MakeSureDialog->SecondaryButtonClick +=
			ref new Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::ContentDialog^, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^>([editorPage](Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ e)
				{
					editorPage->thisTab->CloseTab();
				});

	}
}


void Just_Editor_UWP::DrnTabPanel::topPanel_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	newWidth = topPanel->ActualWidth - justEditorBtn->ActualWidth - drnBtn->ActualWidth - RightMargin;
	tabScrollViewer->MaxWidth = newWidth;
}


void Just_Editor_UWP::DrnTabPanel::tabScrollViewer_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	double newHOffset = (double)selectedIndex * TabWidth;
	if (e == nullptr || newHOffset < tabScrollViewer->HorizontalOffset)
		tabScrollViewer->ChangeView(newHOffset, tabScrollViewer->VerticalOffset, tabScrollViewer->ZoomFactor);
	else if (newHOffset + TabWidth > tabScrollViewer->HorizontalOffset + tabScrollViewer->ViewportWidth)
		tabScrollViewer->ChangeView(tabScrollViewer->HorizontalOffset + tabScrollViewer->ViewportWidth , tabScrollViewer->VerticalOffset, tabScrollViewer->ZoomFactor);
}


void Just_Editor_UWP::DrnTabPanel::tabPanel_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	tabScrollViewer->MaxWidth = newWidth;
	tabScrollViewer->ChangeView(tabScrollViewer->ScrollableWidth, tabScrollViewer->VerticalOffset, tabScrollViewer->ZoomFactor);
}
