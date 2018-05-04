#include "IpAddress.hh"
#include "StringTools.hh"
#include "Exceptions.hh"
#include <sstream>
#include <iostream>

IpAddress::IpAddress() : addr(0) {}

IpAddress::IpAddress(const std::string& addr) : addr(0) {
	auto octets = StringTools::split(addr, '.');
	if (octets.size() != 4) { throw IPAddressParseException("String to IpAddress parsing error"); }
	try {
		this->addr |= std::stoi(octets[0]) << 24;
		this->addr |= std::stoi(octets[1]) << 16;
		this->addr |= std::stoi(octets[2]) << 8;
		this->addr |= std::stoi(octets[3]);
	}
	catch (const std::exception & e) {
		throw IPAddressParseException("String to IpAddress parsing error: "s + e.what());
	}
}
IpAddress::IpAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d) : addr(0) {
	addr |= a << 24;
	addr |= b << 16;
	addr |= c << 8;
	addr |= d;
}


IpAddress::~IpAddress()
{
}



IpAddress::operator std::string()const {
	auto result = std::stringstream();
	result <<
		((addr & 0xFF000000) >> 24) << '.' <<
		((addr & 0x00FF0000) >> 16) << '.' <<
		((addr & 0x0000FF00) >> 8) << '.' <<
		((addr & 0x000000FF));
	return result.str();
}



IpAddress::operator uint32_t()const { return addr; }

const IpAddress IpAddress::LOCALHOST{ 127,0,0,1 };