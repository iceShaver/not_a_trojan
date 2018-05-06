#include "WinapiHelpers.hh"
#include "Exceptions.hh"
#include <gdiplus.h>
#include <Shlwapi.h>

#include <string>
#include <vector>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Shlwapi.lib")
using namespace std::string_literals;
std::string Winapi::GetErrorMessage() {
	//Get the error message, if any.
	const auto errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return "";

	LPSTR messageBuffer = nullptr;
	const auto size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, errorMessageID, 0, (LPSTR)&messageBuffer, 0, nullptr);

	std::string message(messageBuffer, size);

	LocalFree(messageBuffer);

	return message;
}
std::string Winapi::GetWinsocksErrorMessage() {
	//Get the error message, if any.
	const auto errorMessageID = ::WSAGetLastError();
	if (errorMessageID == 0)
		return "";

	LPSTR messageBuffer = nullptr;
	const auto size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, errorMessageID, 0, (LPSTR)&messageBuffer, 0, nullptr);

	std::string message(messageBuffer, size);

	LocalFree(messageBuffer);

	return message;
}


int Winapi::GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
	unsigned int num = 0, size = 0;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0) return -1;
	Gdiplus::ImageCodecInfo *pImageCodecInfo = (Gdiplus::ImageCodecInfo *)(malloc(size));
	if (pImageCodecInfo == NULL) return -1;
	GetImageEncoders(num, size, pImageCodecInfo);

	for (unsigned int j = 0; j < num; ++j) {
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}

std::vector<char> Winapi::GetScreenshotAsJpegBytes(std::wstring filename, ULONG uQuality)
{
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	HWND hMyWnd = GetDesktopWindow();
	RECT r;
	int w, h;
	HDC dc, hdcCapture;
	int nBPP, nCapture, iRes;
	LPBYTE lpCapture;
	CLSID imageCLSID;
	Gdiplus::Bitmap *pScreenShot;

	// get the area of my application's window     
	GetWindowRect(hMyWnd, &r);
	dc = GetWindowDC(hMyWnd);   // GetDC(hMyWnd) ;
	w = r.right - r.left;
	h = r.bottom - r.top;
	nBPP = GetDeviceCaps(dc, BITSPIXEL);
	hdcCapture = CreateCompatibleDC(dc);

	// create the buffer for the screenshot
	BITMAPINFO bmiCapture = { sizeof(BITMAPINFOHEADER), w, -h, 1, nBPP, BI_RGB, 0, 0, 0, 0, 0, };

	// create a container and take the screenshot
	HBITMAP hbmCapture = CreateDIBSection(dc, &bmiCapture, DIB_PAL_COLORS, (LPVOID *)&lpCapture, NULL, 0);

	// failed to take it
	if (!hbmCapture) {
		DeleteDC(hdcCapture);
		DeleteDC(dc);
		Gdiplus::GdiplusShutdown(gdiplusToken);
		throw ScreenshotException("An error occured while creating screenshot" + Winapi::GetErrorMessage());
	}

	// copy the screenshot buffer
	nCapture = SaveDC(hdcCapture);
	SelectObject(hdcCapture, hbmCapture);
	BitBlt(hdcCapture, 0, 0, w, h, dc, 0, 0, SRCCOPY);
	RestoreDC(hdcCapture, nCapture);
	DeleteDC(hdcCapture);
	DeleteDC(dc);

	// save the buffer to a file   
	pScreenShot = new Gdiplus::Bitmap(hbmCapture, (HPALETTE)NULL);
	Gdiplus::EncoderParameters encoderParams;
	encoderParams.Count = 1;
	encoderParams.Parameter[0].NumberOfValues = 1;
	encoderParams.Parameter[0].Guid = Gdiplus::EncoderQuality;
	encoderParams.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
	encoderParams.Parameter[0].Value = &uQuality;
	Winapi::GetEncoderClsid(L"image/jpeg", &imageCLSID);

	IStream *pImgStream = ::SHCreateMemStream(NULL, 0);
	//Gdiplus::Image imageSrc(pImgStream);
	if(pScreenShot->Save(pImgStream, &imageCLSID, &encoderParams) != Gdiplus::Ok) {
		Gdiplus::GdiplusShutdown(gdiplusToken);
		throw ScreenshotException("An error occured while converting screenshot: " + Winapi::GetErrorMessage());
	}
	//(pScreenShot->Save(filename.c_str(), &imageCLSID, &encoderParams) == Gdiplus::Ok);
	// first let's learn the size of the content
	STATSTG statsg;
	HRESULT hr = pImgStream->Stat(&statsg, STATFLAG_NONAME);
	// check against FAILED(hr)
	// We need to seek to the first position as after writeOutImage it's at the end.
	LARGE_INTEGER seekPos;
	seekPos.QuadPart = 0;
	hr = pImgStream->Seek(seekPos, STREAM_SEEK_SET, NULL);
	// check against FAILED(hr)
	std::vector<char> result(statsg.cbSize.QuadPart);
	ULONG cbRead;
	hr = pImgStream->Read(&result[0], result.size(), &cbRead);
	// here cbRead should be equal to statsg.cbSize.QuadPart.
	// Alternatively, to be memory efficient, one can read in chunk by chunk (in a cycle.)
	delete pScreenShot;
	DeleteObject(hbmCapture);
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return result;
}
