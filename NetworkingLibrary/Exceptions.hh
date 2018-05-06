#pragma once
#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)
#endif  
#include <exception>
#include <string>

class Exception : public std::exception {
public:
	explicit Exception(const std::string& message) : std::exception(message.c_str()) {}
	Exception() : std::exception() {}

};
//_______________________________WINAPI EXCEPTIONS____________________________________________
class SocketException : public Exception {
public:
	explicit SocketException(const std::string& message) : Exception(message) {}
	SocketException() = default;
};
class IPAddressParseException : public Exception {
public:
	explicit IPAddressParseException(const std::string& message) : Exception(message) {}
	IPAddressParseException() = default;
};
class GetAddrInfoException : public SocketException {
public:
	explicit GetAddrInfoException(const std::string& message)
		: SocketException(message) {}

	GetAddrInfoException() = default;
};
class SocketConnectException : public SocketException {
public:

	explicit SocketConnectException(const std::string& message)
		: SocketException(message) {}

	SocketConnectException() = default;
};
class InvalidSocketException : public SocketException {
public:

	explicit InvalidSocketException(const std::string& message)
		: SocketException(message) {}

	InvalidSocketException() = default;
};
class SocketSendException : public SocketException {

public:
	explicit SocketSendException(const std::string& message)
		: SocketException(message) {}

	SocketSendException() = default;
};
class SocketShutdownException : public SocketException {

public:
	explicit SocketShutdownException(const std::string& message)
		: SocketException(message) {}

	SocketShutdownException() = default;
};
class WSAStartupException : public SocketException {

public:
	explicit WSAStartupException(const std::string& message)
		: SocketException(message) {}

	WSAStartupException() = default;
};
class WindowCreationException : public Exception {
public:
	explicit WindowCreationException(const std::string& message)
		: Exception(message) {}

	WindowCreationException() = default;
};
class ClipboardListenerException : public Exception {

public:
	explicit ClipboardListenerException(const std::string& message)
		: Exception(message) {}

	ClipboardListenerException() = default;
};
class BitmapProcessingException : public Exception {

public:
	explicit BitmapProcessingException(const std::string& message)
		: Exception(message) {}

	BitmapProcessingException() = default;
};

class BmpFileCreationException : public Exception {

public:
	explicit BmpFileCreationException(const std::string& message)
		: Exception(message) {}

	BmpFileCreationException() = default;
};
class ScreenshotException : public Exception {

public:
	explicit ScreenshotException(const std::string& message)
		: Exception(message) {}

	ScreenshotException() = default;
};