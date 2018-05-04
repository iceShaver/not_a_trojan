#pragma once

#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)
#endif  
#include <string>
#include <ostream>
using namespace  std::string_literals;
class IpAddress
{
public:
	IpAddress();
	explicit IpAddress(const std::string& str);
	IpAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
	~IpAddress();
	explicit operator std::string() const;
	explicit operator uint32_t () const;
	static const IpAddress LOCALHOST;
	friend std::ostream& operator<<(std::ostream& o, const IpAddress& ip) { return o << (std::string)ip; }
private:
	uint32_t addr;
};