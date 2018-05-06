#include "Socket.hh"
#include "Exceptions.hh"
#include <iostream>
#include "../TrojanClient/ClientWindow.hh"
#include "tools.hh"


#pragma comment(lib, "Ws2_32.lib")


Socket::Socket(SocketType socketType, PCSTR port, IpAddress ipAddress) : socket(INVALID_SOCKET) {
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		throw WSAStartupException(Winapi::GetWinsocksErrorMessage());
	}
	addrinfo *result = NULL;
	addrinfo hints{};
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(((std::string)ipAddress).c_str(), port, &hints, &result)) {
		WSACleanup();
		throw GetAddrInfoException(Winapi::GetWinsocksErrorMessage());
	}

	if ((this->socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		throw SocketException(Winapi::GetWinsocksErrorMessage());
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
		throw SocketSendException(Winapi::GetWinsocksErrorMessage());
	}
	std::cout << "bytes sent: " << iResult << std::endl;
	if (shutdown(this->socket, SD_SEND) == SOCKET_ERROR) {
		closesocket(this->socket);
		WSACleanup();
		throw SocketShutdownException(Winapi::GetWinsocksErrorMessage());
	}
	return iResult;
}

int Socket::SendInt(uint32_t data) {

	int length = sizeof(data);
	this->SendTypeId(DataType::INT32);
	return this->send((char*)&data, length);
}

int Socket::SendBytes(const char* data, int length) {
	return this->send(data, length);
}

int Socket::SendTypeId(DataType type) {
	return this->send((char*)type, sizeof(DataType));
}

int Socket::SendMetadata(Metadata metadata) {
	auto size = sizeof(metadata);
	return this->send((char*)&metadata, size);
}

Socket::~Socket()
{
	if (shutdown(this->socket, SD_SEND) == SOCKET_ERROR) {
		closesocket(this->socket);
		WSACleanup();
		throw SocketShutdownException(Winapi::GetWinsocksErrorMessage());
	}
}

Metadata Socket::RecvMetadata() {
	auto result = Metadata{};
	auto size = sizeof(result);
	int iResult = recv(this->socket, (char*)&result, size, NULL);
	return result;
}

std::string Socket::RecvString(const Metadata& metadata) {
	char* buf = new char[metadata.length];
	int iResult = recv(this->socket, buf, metadata.length, NULL);
	buf[iResult] = 0;
	return std::string(buf);
}

int Socket::send(const char* buf, int length) {
	int iResult;
	if ((iResult = ::send(this->socket, buf, length, 0)) == SOCKET_ERROR) {
		auto message = Winapi::GetWinsocksErrorMessage();
		closesocket(this->socket);
		WSACleanup();
		throw SocketSendException(message);
	}
	
	return iResult;
}
