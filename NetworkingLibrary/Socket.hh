#pragma once

#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)   
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)   
#endif  
#ifndef UNICODE
#define UNICODE
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <string>
#include "IpAddress.hh"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <future>
#include "DataType.hh"
#include "Metadata.hh"
#pragma comment(lib, "Ws2_32.lib")
enum class SocketType {
	SERVER_SOCKET,
	CONNECT_SOCKET,
};



class Socket
{
public:
	Socket(SocketType socketType, PCSTR port, IpAddress ipAddress = { 0,0,0,0 }, int bufferSize = 4096);
	explicit Socket(SOCKET socket, int bufferSize = 4096);
	int SendBytes(const char * data, int length);
	int SendMetadata(Metadata metadata);
	Metadata RecvMetadata();
	std::vector<char> RecvBytes(int size);
	std::string RecvString(const Metadata& metadata);
	~Socket();
private:
	int send(const char * data, int size);
	std::vector<char> recv(int size);
	SOCKET socket;
	const int bufferSize;
};
