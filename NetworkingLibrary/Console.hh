#pragma once
#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)
#endif  
#include <windows.h>

namespace Console {
	// maximum mumber of lines the output console should have
	static const WORD MAX_CONSOLE_LINES = 500;
	void CreateConsole();
	//End of File
}