#pragma once

#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)   
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)   
#endif  
#include <string>
#include "IpAddress.hh"
#include <windows.h>
#include <winsock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>
#include <future>
#include <minwinbase.h>
#include "DataType.hh"
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
	template<typename T>
	int sendObject(T data);
	int Recv();
	~Socket();
private:
	int send(const char * buf, int length);
	SOCKET socket;
};
