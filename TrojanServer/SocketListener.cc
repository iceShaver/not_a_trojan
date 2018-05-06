#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "SocketListener.hh"
#include <iostream>
#include "WinapiHelpers.hh"
#include "ClientHandler.hh"
#include <future>
#include "ServerConfig.hh"
#include "Tools.hh"
#include "ServerWindow.hh"

#pragma comment(lib, "Ws2_32.lib")


SocketListener::SocketListener(ServerWindow *pMainWindow)
	: pMainWindow(pMainWindow), listenSocket(INVALID_SOCKET)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		throw SocketException("WSAStrartup error: " + Winapi::GetWinsocksErrorMessage());
	}
	addrinfo *result = NULL;
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, ServerConfig::LISTEN_PORT, &hints, &result)) {
		const auto message = "getaddrinfo failed: " + Winapi::GetWinsocksErrorMessage();
		WSACleanup();
		throw SocketException(message);
	}

	this->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // CREATE SOCKET
	if (this->listenSocket == INVALID_SOCKET) {
		const auto message = "Error at socket(): " + Winapi::GetWinsocksErrorMessage();
		freeaddrinfo(result);
		WSACleanup();
		throw SocketException(message);
	}


	if (bind(this->listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) { // BIND SOCKET WITH NAME(IP, port)
		const auto message = "bind failed with error: " + Winapi::GetWinsocksErrorMessage();
		freeaddrinfo(result);
		closesocket(this->listenSocket);
		WSACleanup();
		throw SocketException(message);
	}
	freeaddrinfo(result);
	if (listen(this->listenSocket, SOMAXCONN) == SOCKET_ERROR) { // LISTENING MODE
		const auto message = "listen failed with error: " + Winapi::GetWinsocksErrorMessage();
		closesocket(this->listenSocket);
		WSACleanup();
		throw SocketException(message);
	}

}


SocketListener::~SocketListener()
{
}

void SocketListener::BlockingListen() {
	while (true) {
		SOCKET clientSocket = INVALID_SOCKET;
		Tools::PrintDebugMessage("Waiting for connection");
		if ((clientSocket = accept(this->listenSocket, NULL, NULL)) == INVALID_SOCKET) {
			auto message = "accept failed with error: " + Winapi::GetWinsocksErrorMessage();
			closesocket(this->listenSocket);
			WSACleanup();
			throw SocketException(message);
		}
		Tools::PrintDebugMessage("Connection accepted");
		std::async(std::launch::async, [clientSocket]() {
			ClientHandler clientHandler = ClientHandler(clientSocket);
			clientHandler.Handle();
		});
	}
}

