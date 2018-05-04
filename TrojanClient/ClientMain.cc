
#ifndef UNICODE
#define UNICODE
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <iphlpapi.h>

#include <future>
#include "WinapiHelpers.hh"
#include "ClientConfig.hh"
#include "Exceptions.hh"
#include "StringTools.hh"
#include "IpAddress.hh"
#include "Socket.hh"
#include "ClientWindow.hh"
#include "Console.hh"

#pragma comment(lib, "Ws2_32.lib")

using namespace  std::string_literals;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, int nCmdShow) {
#ifdef _DEBUG
	Console::CreateConsole();
#endif

	auto mainWindow = ClientWindow();
	mainWindow.Create(L"", WS_OVERLAPPEDWINDOW);

	//try {
	//	Socket socket = Socket(SocketType::CONNECT_SOCKET, "55555", IpAddress::LOCALHOST);
	//	socket.SendTest();
	//}
	//catch (const SocketException &e) {
	//	std::cerr << e.what() << std::endl;
	//	system("pause");
	//}

	// message loop
	OpenClipboard(mainWindow.Window());
	auto cd = GetClipboardData(CF_TEXT);
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	CloseClipboard();
	system("pause");
	return 0;
}
