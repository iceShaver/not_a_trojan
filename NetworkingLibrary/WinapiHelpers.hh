#pragma once
#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)
#endif 
#include <string>
#include <Windows.h>
#include <vector>

namespace Winapi {
	std::string GetErrorMessage();
	std::string GetWinsocksErrorMessage();
	int GetEncoderClsid(const WCHAR *format, CLSID *pClsid);
	std::vector<char> GetScreenshotAsJpegBytes(std::wstring filename, ULONG uQuality = 100);


}
