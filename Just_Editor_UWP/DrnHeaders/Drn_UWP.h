#pragma once
#include <windowsnumerics.h>

#define IsSplit(chr) (!chr || (chr != L'_' && chr != L'#' && (chr < '0' || (chr > '9' && chr < 'A') || (chr > 'Z' && chr < 'a') || chr > 'z')))

const char keyMap[] = " \t1234567890-=__QWERTYUIOP[]\n_ASDFGHJKL;'`_\\ZXCVBNM,./_*_ ________________-___+____._____________________________________0123456789";
const char shiftKeyMap[] = " \t!@#$%^&*()_+__QWERTYUIOP{}\n_ASDFGHJKL:\"~_|ZXCVBNM<>?_*_ ________________-___+____._____________________________________0123456789";
const wchar_t FileTypesStr[][6] = {L".txt", L".h", L".c", L".cpp", L".cs", L".js", L".html"};
const wchar_t ImageFileTypesStr[][5] = { L".jpg", L".bmp", L".png"};
const wchar_t edgeMap[] = L" ;.[]{}~!&%^&*()-+=|/'\":`";
const wchar_t UnabledWords[] = L"\\/:*?<>|";

class Drn_UWP
{
public:
	static int getChildNum(Windows::UI::Xaml::Controls::UIElementCollection^ children, Windows::UI::Xaml::UIElement^ child)
	{
		int i = (int)children->Size;
		while (--i >= 0)
			if (children->GetAt(i) == child)
				return i;

		return -1;
	}

	static Platform::String^ SubStr(Platform::String^ sourceStr, size_t st, size_t len)
	{
		std::wstring wSource = sourceStr->Data();
		return ref new Platform::String(wSource.substr(st, len).c_str());
	}

	static Platform::String^ FillStr(wchar_t fChar, size_t len)
	{
		Platform::String^ result = L"";
		while (len--)
			result += fChar;

		return result;
	}

	static unsigned int abslteSub(unsigned int p1, unsigned int p2)
	{
		return p1 > p2 ? p1 - p2 : p2 - p1;
	}

	static concurrency::task<Windows::Storage::StorageFile^> PickAFile(bool isImage)
	{
		auto tPicker = ref new Windows::Storage::Pickers::FileOpenPicker;
		tPicker->CommitButtonText = L"Open";
		tPicker->ViewMode = Windows::Storage::Pickers::PickerViewMode::Thumbnail;
		tPicker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary;
		if (isImage)
			for (int i = 0; i < 3; i++)
				tPicker->FileTypeFilter->Append(ref new Platform::String(ImageFileTypesStr[i]));
		else
			for (int i = 0; i < 7; i++)
				tPicker->FileTypeFilter->Append(ref new Platform::String(FileTypesStr[i]));

		return concurrency::create_task(tPicker->PickSingleFileAsync());
	}
	static concurrency::task<Windows::Storage::StorageFolder^> PickAFolder()
	{
		auto tPicker = ref new Windows::Storage::Pickers::FolderPicker;
		tPicker->CommitButtonText = L"Save";
		tPicker->ViewMode = Windows::Storage::Pickers::PickerViewMode::Thumbnail;
		tPicker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary;
		tPicker->FileTypeFilter->Append("*");

		return concurrency::create_task(tPicker->PickSingleFolderAsync());
	}

	static Windows::Foundation::IAsyncOperation<Platform::String^>^ ReadTextStorageFile(Windows::Storage::StorageFile^ tFile, Windows::Storage::Streams::UnicodeEncoding encodeMode = Windows::Storage::Streams::UnicodeEncoding::Utf8)
	{
		return Windows::Storage::FileIO::ReadTextAsync(tFile, encodeMode);
	}

	static Windows::Foundation::IAsyncAction^ WriteTextStorageFile(Windows::Storage::StorageFile^ tFile, Platform::String^ tStr)
	{
		return Windows::Storage::FileIO::WriteTextAsync(tFile, tStr);
	}

	static concurrency::task<void> DrnAnimeX(Windows::UI::Composition::Visual^ tVisual, float startValue, float finalValue, unsigned int time, bool dOpacity)
	{
		float opacity = 1 / (float)time;
		if (dOpacity)
		{
			tVisual->Opacity = 1;
			opacity = -opacity;
		}
		else
			tVisual->Opacity = 0;

		tVisual->Offset = Windows::Foundation::Numerics::float3(startValue, tVisual->Offset.y, tVisual->Offset.z);

		return concurrency::create_task([tVisual, startValue, finalValue, time, opacity]()
			{
				float moveStep = (finalValue - startValue) / time;
				unsigned int tTime = time;
				
				while (tTime--)
				{
					tVisual->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([tVisual, moveStep, finalValue, opacity]()
						{
							if (abs(finalValue - tVisual->Offset.x) < abs(moveStep))
								tVisual->Offset = Windows::Foundation::Numerics::float3(finalValue, tVisual->Offset.y, tVisual->Offset.z);
							else
								tVisual->Offset = Windows::Foundation::Numerics::float3(tVisual->Offset.x + moveStep, tVisual->Offset.y, tVisual->Offset.z);
							tVisual->Opacity += opacity;
						}));
					Sleep(10);
				}
			});
	}
	static concurrency::task<void> DrnAnimeY(Windows::UI::Composition::Visual^ tVisual, float startValue, float finalValue, unsigned int time, Windows::UI::Xaml::UIElement^ sourceControl = nullptr)
	{
		float moveStep = (finalValue - startValue) / time, opacity = 1 / (float)time;
		if (sourceControl != nullptr && sourceControl->Opacity)
			opacity = -opacity;
		if (tVisual != nullptr)
			tVisual->Offset = Windows::Foundation::Numerics::float3(tVisual->Offset.x, startValue, tVisual->Offset.z);
		return concurrency::create_task([tVisual, moveStep, finalValue, time, opacity, sourceControl]()
			{
				unsigned int tTime = time;

				while (tTime--)
				{
					tVisual->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([tVisual, moveStep, finalValue, sourceControl, opacity]()
						{
							if (tVisual != nullptr)
							{
								if (abs(finalValue - tVisual->Offset.y) < abs(moveStep))
									tVisual->Offset = Windows::Foundation::Numerics::float3(tVisual->Offset.x, finalValue, tVisual->Offset.z);
								else
									tVisual->Offset = Windows::Foundation::Numerics::float3(tVisual->Offset.x, tVisual->Offset.y + moveStep, tVisual->Offset.z);
							}
							if (sourceControl != nullptr)
								sourceControl->Opacity += opacity;
						}));
					Sleep(10);
				}
			});
	}
	static void ShowMessageBox(Platform::String^ title, Platform::Object^ content)
	{
		auto tBox = ref new Windows::UI::Xaml::Controls::ContentDialog;
		tBox->Content = content;
		tBox->Title = title;
		tBox->PrimaryButtonText = "OK";
		tBox->ShowAsync();
	}

	static void LoadBackgroundStream(Windows::UI::Xaml::Controls::ContentPresenter^ page, Windows::Storage::Streams::IRandomAccessStreamWithContentType^ bStream)
	{
		auto bImg = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage();
		bImg->SetSource(bStream);

		Windows::UI::Xaml::Media::ImageBrush^ bBrush;
		if (page->Background != nullptr && page->Background->GetType() == Windows::UI::Xaml::Media::ImageBrush::typeid)
			bBrush = (Windows::UI::Xaml::Media::ImageBrush^)page->Background;
		else
		{
			bBrush = ref new Windows::UI::Xaml::Media::ImageBrush;
			bBrush->Stretch = Windows::UI::Xaml::Media::Stretch::UniformToFill;
			page->Background = bBrush;
		}
		bBrush->ImageSource = bImg;
	}
	static void LoadBackgroundImage(Windows::UI::Xaml::Controls::ContentPresenter^ page, Windows::Storage::StorageFile^ imageFile)
	{
		concurrency::create_task(imageFile->OpenReadAsync()).then([page](Windows::Storage::Streams::IRandomAccessStreamWithContentType^ result)
			{
				LoadBackgroundStream(page, result);
			});
	}
	static void LoadBackgroundImageFromFileName(Windows::UI::Xaml::Controls::ContentPresenter^ page, Platform::String^ fileName)
	{
		concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(ref new Windows::Foundation::Uri(fileName))).then([](concurrency::task<Windows::Storage::StorageFile^> bImage)
			{
				try
				{
					return bImage.get()->OpenReadAsync();
				}
				catch (Platform::COMException^ ex)
				{
					ShowMessageBox(L"Warn", L"Error Code:" + ex->HResult.ToString() + L"\n" + ex->Message);
					concurrency::cancel_current_task();
				}
			}, concurrency::task_continuation_context::use_current()).then([page](Windows::Storage::Streams::IRandomAccessStreamWithContentType^ result)
				{
					LoadBackgroundStream(page, result);
				});
	}

	static void WriteSetting(Platform::String^ ContainerName, Platform::String^ SettingTypeName, Platform::Object^ SettingInfo)
	{
		if (SettingTypeName == nullptr || SettingInfo == nullptr) return;
		Windows::Storage::ApplicationDataContainer^ localSettings = Windows::Storage::ApplicationData::Current->LocalSettings;
		Windows::Storage::ApplicationDataContainer^ container =
			Windows::Storage::ApplicationData::Current->LocalSettings->CreateContainer(ContainerName, Windows::Storage::ApplicationDataCreateDisposition::Always);
		if (localSettings->Containers->HasKey(ContainerName))
		{
			auto values = localSettings->Containers->Lookup(ContainerName)->Values;
			if (values->HasKey(SettingTypeName))
				values->Clear();

			values->Insert(SettingTypeName, SettingInfo);
		}
	}

	static Platform::Object^ ReadSetting(Platform::String^ ContainerName, Platform::String^ SettingTypeName)
	{
		Windows::Storage::ApplicationDataContainer^ localSettings = Windows::Storage::ApplicationData::Current->LocalSettings;
		Windows::Storage::ApplicationDataContainer^ container =
			Windows::Storage::ApplicationData::Current->LocalSettings->CreateContainer(ContainerName, Windows::Storage::ApplicationDataCreateDisposition::Always);
		bool hasContainer = localSettings->Containers->HasKey(ContainerName);
		bool hasSetting = false;
		if (hasContainer)
		{
			auto values = localSettings->Containers->Lookup(ContainerName)->Values;
			hasSetting = values->HasKey(SettingTypeName);
			return hasSetting ? values->Lookup(SettingTypeName) : "";
		}
		return nullptr;
	}

	static Windows::UI::Text::Core::CoreTextRange GetWordPosition(Platform::String^ srcStr, unsigned int srcLen, unsigned int col)
	{
		std::wstring wStr = srcStr->Data();
		
		unsigned int tPos;
		Windows::UI::Text::Core::CoreTextRange thisRange;
		thisRange.StartCaretPosition = (int)col;
		thisRange.EndCaretPosition = (int)col;

		for (tPos = col; tPos;)
		{
			wchar_t tChar = wStr[--tPos];
			if (IsSplit(tChar))
				break;

			thisRange.StartCaretPosition--;
		}
		for (tPos = col; tPos < srcLen;)
		{
			wchar_t tChar = wStr[tPos++];
			if (IsSplit(tChar))
				break;

			thisRange.EndCaretPosition++;
		}

		return thisRange;
	}
};

