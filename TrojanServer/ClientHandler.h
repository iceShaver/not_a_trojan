#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "SocketListener.h"
#include <iostream>
#include "WinapiHelpers.hh"
#include "ClientHandler.h"

#pragma comment(lib, "Ws2_32.lib")
class ClientHandler
{
public:
	explicit ClientHandler(SOCKET clientSocket);
	~ClientHandler();
private:
	SOCKET clientSocket;
};

