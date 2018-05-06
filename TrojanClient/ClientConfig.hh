#pragma once
#include <string>
#include "IpAddress.hh"
using namespace std::string_literals;
namespace ClientConfig {
	constexpr const char * TARGET_ACCOUNT_NUMBER = "8888888888555566622";
	constexpr PCSTR PORT = "55555";
	const auto IP_ADDRESS = IpAddress{"127.0.0.1"};
	constexpr uint64_t AUTH_PACKET = 2564788125965854;
}
