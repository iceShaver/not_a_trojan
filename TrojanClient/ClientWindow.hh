#pragma once
#include "Socket.hh"
#include "BaseWindow.hh"
#include <Windows.h>
#include <string>
#include "ClientConfig.hh"

using namespace std::string_literals;
class ClientWindow : public BaseWindow<ClientWindow> {
public:

	ClientWindow();
	PCWSTR ClassName() const override { return L"ClientWindowClass"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	std::string ReadClipboardText();
	void WriteClipboardText(std::string str);
	bool IsAccountNumber(const std::string& str);
	bool SwapAccountNumber();
	void SendClipboardStringToServer(const std::string& str);
	void SendAccountSwapNotificationToServer();
	void OnClipboardChange();
	void BlockingListen();

	std::string GetTargetAccountNumber() const;
	void SetTargetAccountNumber(const std::string& targetAccountNumber);

private:
	std::string targetAccountNumber;
	mutable std::mutex targetAccountNumberMutex;
	Socket socket;

};
