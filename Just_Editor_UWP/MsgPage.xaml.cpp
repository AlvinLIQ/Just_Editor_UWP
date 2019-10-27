//
// MsgPage.xaml.cpp
// Implementation of the MsgPage class
//

#include "pch.h"
#include "MsgPage.xaml.h"

using namespace Just_Editor_UWP;

using namespace concurrency;

using namespace Platform;
using namespace Windows::UI::Core;


// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

MsgPage::MsgPage()
{
	InitializeComponent();
}


void Just_Editor_UWP::MsgPage::connBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (isConnected)
	{
		connBtn->IsEnabled = false;
		isConnected = false;

		return;
	}

	connBtn->Content = L"Connecting";
	struct sockaddr_in target_Addr;
	socket_fd = Drn_Tools::initSocket();
//	char* ip = Drn_Tools::wctoc(ipBox->Text->Data());
//	struct hostent *t_Host = gethostbyname(ip);
	target_Addr = Drn_Tools::initAddr(ipBox->Text->Data(), _wtoi(portBox->Text->Data()));
	create_task([target_Addr, this]()
		{
			int result = connect(socket_fd, (struct sockaddr*)&target_Addr, sizeof(target_Addr));
			this->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([result, this]() 
				{
					if (!result)
					{
						connBtn->Content = L"Disconnect";
						resultBlock->Text += L"Connected\n";
						sendBtn->IsEnabled = true;
						isConnected = true;
					}
					else
						resultBlock->Text += WSAGetLastError().ToString();
				}));
			if (!result)
			{
				char rBuf[256];
				int rLen;
				while ((rLen = recv(socket_fd, rBuf, 255, 0)) > 0 && isConnected)
				{
					rBuf[rLen] = 0;
					this->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([rBuf, this]()
						{
							auto result = Drn_Tools::ctowc(rBuf);
							resultBlock->Text += ref new Platform::String(result);
							delete[] result;
						}));
				}
				return;
			}
			Drn_Tools::closeSocket(&socket_fd);
				
		}).then([this]() 
			{
				connBtn->Content = L"Connect";
				resultBlock->Text += "\nClosed\n";
				connBtn->IsEnabled = true;
				sendBtn->IsEnabled = false;
				isConnected = false;
			}, task_continuation_context::use_current());
}


void Just_Editor_UWP::MsgPage::sendBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	String^ sendStr = reqEditor->GetStr();
	resultBlock->Text += sendStr;
	if (send(socket_fd, Drn_Tools::wctos(sendStr->Data()).c_str(), sendStr->Length(), 0) >= 0)
		resultBlock->Text += L"\nSent\n";
	else
		resultBlock->Text += L"\nFailed\n";
}


void Just_Editor_UWP::MsgPage::clearBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	resultBlock->Text = "";
}
