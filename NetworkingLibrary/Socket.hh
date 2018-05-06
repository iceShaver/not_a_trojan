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
	Socket(SocketType socketType, PCSTR port, IpAddress ipAddress = { 0,0,0,0 });
	int SendTest();
	int SendInt(uint32_t data);
	int SendString(std::string data);
	int SendBytes(const char * data, int length);
	int SendTypeId(DataType type);
	int SendMetadata(Metadata metadata);
	template<typename T>
	int sendObject(T data);
	int Recv();
	~Socket();
	Metadata RecvMetadata();
	std::string RecvString(const Metadata& metadata);
private:
	int send(const char * buf, int length);
	SOCKET socket;
};
