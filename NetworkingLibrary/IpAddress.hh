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
#include <string>
#include <ostream>

using namespace  std::string_literals;
class IpAddress
{
public:
	IpAddress();
	explicit IpAddress(const std::string& str);
	explicit IpAddress(int addr);
	explicit IpAddress(sockaddr addr);
	IpAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
	~IpAddress();
	explicit operator std::string() const;
	explicit operator std::wstring()const;
	explicit operator uint32_t () const;
	static const IpAddress LOCALHOST;
	static std::string SockaddrToString(sockaddr);
	friend std::ostream& operator<<(std::ostream& o, const IpAddress& ip) { return o << (std::string)ip; }
private:
	uint32_t addr;
};