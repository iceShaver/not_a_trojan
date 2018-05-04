#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "SocketListener.h"
#include <iostream>
#include "WinapiHelpers.hh"
#include "ClientHandler.h"
#include <future>
#include "ServerConfig.hh"
#include "tools.hh"

#pragma comment(lib, "Ws2_32.lib")


SocketListener::SocketListener(ServerWindow *pMainWindow)
	: pMainWindow(pMainWindow), listenSocket(INVALID_SOCKET)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {

		this->pMainWindow->EmergencyExit(Winapi::getWinsocksErrorMessage());
		return;
	}
	addrinfo *result = NULL;
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, ServerConfig::LISTEN_PORT, &hints, &result)) {
		auto message = "getaddrinfo failed: " + Winapi::getWinsocksErrorMessage();
		WSACleanup();
		this->pMainWindow->EmergencyExit(message);
		return;
	}

	this->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // CREATE SOCKET
	if (this->listenSocket == INVALID_SOCKET) {
		auto message = "Error at socket(): " + Winapi::getWinsocksErrorMessage();
		freeaddrinfo(result);
		WSACleanup();
		this->pMainWindow->EmergencyExit(message);
		return;
	}


	if (bind(this->listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) { // BIND SOCKET WITH NAME(IP, port)
		auto message = "bind failed with error: " + Winapi::getWinsocksErrorMessage();
		freeaddrinfo(result);
		closesocket(this->listenSocket);
		WSACleanup();
		this->pMainWindow->EmergencyExit(message);
		return;
	}
	freeaddrinfo(result);
	if (listen(this->listenSocket, SOMAXCONN) == SOCKET_ERROR) { // LISTENING MODE
		auto message = "listen failed with error: " + Winapi::getWinsocksErrorMessage();
		closesocket(this->listenSocket);
		WSACleanup();
		this->pMainWindow->EmergencyExit(message);
		return;
	}

}


SocketListener::~SocketListener()
{
}

void SocketListener::BlockingListen() {
	auto connections = std::vector<std::future<void>>();


	while (true) {
		SOCKET clientSocket = INVALID_SOCKET;
		Tools::PrintDebugMessage("Waiting for connection");
		if ((clientSocket = accept(this->listenSocket, NULL, NULL)) == INVALID_SOCKET) {
			auto message = "accept failed with error: " + Winapi::getWinsocksErrorMessage();
			closesocket(this->listenSocket);
			WSACleanup();
			this->pMainWindow->EmergencyExit(message);
		}
		Tools::PrintDebugMessage("Connection accepted");
		connections.push_back(std::async(std::launch::async, [clientSocket]() {
			ClientHandler clientHandler = ClientHandler(clientSocket);
		}));
	}
}

void SocketListener::StartListening(ServerWindow* pMainWindow) {
	auto socketListener = ::SocketListener(pMainWindow);
	socketListener.BlockingListen();
}

