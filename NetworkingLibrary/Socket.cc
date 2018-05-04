#ifndef UNICODE
#define UNICODE
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "Socket.hh"
#include "Exceptions.hh"

#include <typeindex>


#pragma comment(lib, "Ws2_32.lib")


Socket::Socket(SocketType socketType, PCSTR port, IpAddress ipAddress) : socket(INVALID_SOCKET) {
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		throw WSAStartupException(std::to_string(WSAGetLastError()));
	}
	addrinfo *result = NULL;
	addrinfo hints{};
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(((std::string)ipAddress).c_str(), port, &hints, &result)) {
		WSACleanup();
		throw GetAddrInfoException(std::to_string(WSAGetLastError()));
	}

	if ((this->socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		throw SocketException(std::to_string(WSAGetLastError()));
	}

	if (connect(this->socket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		closesocket(this->socket);
		this->socket = INVALID_SOCKET;
		throw SocketConnectException("Couldn't connect to specified server");
	}
	freeaddrinfo(result);
	if (this->socket == INVALID_SOCKET) {
		WSACleanup();
		throw InvalidSocketException();
	}
}

int Socket::SendTest() {
	const char * sendbuf = "this is a test";
	int iResult;
	if ((iResult = ::send(this->socket, sendbuf, (int)strlen(sendbuf), 0)) == SOCKET_ERROR) {
		closesocket(this->socket);
		WSACleanup();
		throw SocketSendException(std::to_string(WSAGetLastError()));
	}
	std::cout << "bytes sent: " << iResult << std::endl;
	if (shutdown(this->socket, SD_SEND) == SOCKET_ERROR) {
		closesocket(this->socket);
		WSACleanup();
		throw SocketShutdownException(std::to_string(WSAGetLastError()));
	}
	return iResult;
}

int Socket::SendInt(uint32_t data) {

	int length = sizeof(data);
	this->SendTypeId(DataType::INT32);
	return this->send((char*)&data, length);
}

int Socket::SendTypeId(DataType type) {
	return this->send((char*)type, sizeof(DataType));
}

Socket::~Socket()
{
}

int Socket::send(const char* buf, int length) {
	int iResult;
	if ((iResult = ::send(this->socket, buf, length, 0)) == SOCKET_ERROR) {
		closesocket(this->socket);
		WSACleanup();
		throw SocketSendException(std::to_string(WSAGetLastError()));
	}
	if (shutdown(this->socket, SD_SEND) == SOCKET_ERROR) {
		closesocket(this->socket);
		WSACleanup();
		throw SocketShutdownException(std::to_string(WSAGetLastError()));
	}
	return iResult;
}
