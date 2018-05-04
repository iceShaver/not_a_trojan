#include "WinapiHelpers.hh"

std::string Winapi::getErrorMessage() {
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
std::string Winapi::getWinsocksErrorMessage() {
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
