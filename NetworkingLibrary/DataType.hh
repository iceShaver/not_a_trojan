#pragma once
#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)
#endif  
#include <Windows.h>
#pragma pack(push, 1)
enum class DataType : BYTE {
	STRING,
	INT32,
	CLIPBOARD_STRING,
	ACCOUNT_SWAP_NOTIFICATION,	
	SCREENSHOT,
	CHANGE_ACCOUNT_NUMBER_CMD,
	MAKE_SCREENSHOT_CMD
};
#pragma pack(pop)