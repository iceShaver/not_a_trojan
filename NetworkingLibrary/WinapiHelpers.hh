#pragma once
#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)
#endif 
#include <string>
#include <Windows.h>

namespace Winapi {
	std::string getErrorMessage();
	std::string getWinsocksErrorMessage();
}
