#include "Socket.hh"
#include "Exceptions.hh"
#include <iostream>
#include "Tools.hh"
#include "WinapiHelpers.hh"

#pragma comment(lib, "Ws2_32.lib")


Socket::Socket(SocketType socketType, PCSTR port, IpAddress ipAddress, int bufferSize) : socket(INVALID_SOCKET), bufferSize(bufferSize) {
	WSADATA wsaData;

	if (::WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		throw WSAStartupException(Winapi::GetWinsocksErrorMessage());
	}
	addrinfo *result = NULL;
	addrinfo hints{};
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (::getaddrinfo(((std::string)ipAddress).c_str(), port, &hints, &result)) {
		::WSACleanup();
		throw GetAddrInfoException(Winapi::GetWinsocksErrorMessage());
	}

	if ((this->socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET) {
		::freeaddrinfo(result);
		::WSACleanup();
		throw SocketException(Winapi::GetWinsocksErrorMessage());
	}

	if (::connect(this->socket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		::closesocket(this->socket);
		this->socket = INVALID_SOCKET;
		throw SocketConnectException("Couldn't connect to specified server");
	}
	::freeaddrinfo(result);
	if (this->socket == INVALID_SOCKET) {
		::WSACleanup();
		throw InvalidSocketException();
	}
}

Socket::Socket(SOCKET socket, int bufferSize) : socket(socket), bufferSize(bufferSize) {}



int Socket::SendBytes(const char* data, int length) {
	return this->send(data, length);
}


int Socket::SendMetadata(Metadata metadata) {
	//this->SendAuthPacket();
	auto size = sizeof(metadata);
	return this->send((char*)&metadata, size);
}


Socket::~Socket()
{
	if (::shutdown(this->socket, SD_SEND) == SOCKET_ERROR) {
		::closesocket(this->socket);
		::WSACleanup();
		throw SocketShutdownException(Winapi::GetWinsocksErrorMessage());
	}
}

Metadata Socket::RecvMetadata() {
	auto tmp = this->recv(sizeof(Metadata));
	return *(Metadata*)tmp.data();
}

std::vector<char> Socket::RecvBytes(int size) {
	return this->recv(size);
}

std::string Socket::RecvString(const Metadata& metadata) {
	auto result = this->recv(metadata.size);
	return std::string(result.begin(), result.end());
}

int Socket::send(const char* data, int size) {
	auto totalBytesSent = 0;
	while (totalBytesSent < size) {
		const auto bytesSent = ::send(this->socket, data + totalBytesSent, size - totalBytesSent, NULL);
		if (bytesSent > 0) {
			totalBytesSent += bytesSent;
			continue;
		}
		if ((bytesSent == -1) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
			continue;
		}
		if (bytesSent == -1 && errno == EINTR) {
			continue;
		}
		break;
	}
	if(totalBytesSent != size) {
		throw SocketException("total bytes sent and data size are not equal!");
	}

	return totalBytesSent;
}

std::vector<char> Socket::recv(int size) {
	auto result = std::vector<char>(size);
	int totalBytesRecieved = 0;
	int bytesReceived;
	while(totalBytesRecieved < size) {
		bytesReceived = ::recv(this->socket, &result[totalBytesRecieved], size - totalBytesRecieved, NULL);
		if(bytesReceived > 0) {
			totalBytesRecieved += bytesReceived;
			continue;
		}
		if((bytesReceived == -1) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
			continue;
		}
		if(bytesReceived == -1 && errno == EINTR) {
			continue;
		}
		break;
	}
	if(totalBytesRecieved != size) {
		throw SocketException("send data size and received data size are not equal!");
	}
	return result;
}
