
#ifndef UNICODE
#define UNICODE
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <iostream>
#include "WinapiHelpers.hh"
#include "ClientConfig.hh"
#include "Exceptions.hh"
#include "StringTools.hh"
#include "IpAddress.hh"
#include "ClientWindow.hh"
#include "Console.hh"
#include "Tools.hh"
#pragma comment(lib, "Ws2_32.lib")

using namespace  std::string_literals;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, int nCmdShow) try{
#ifdef _DEBUG
	Console::CreateConsole();
#endif

	ClientWindow mainWindow{};
	mainWindow.Create(L"", WS_OVERLAPPEDWINDOW);

	//try {
	//	Socket socket = Socket(SocketType::CONNECT_SOCKET, "55555", IpAddress::LOCALHOST);
	//	socket.SendTest();
	//}
	//catch (const SocketException &e) {
	//	std::cerr << e.what() << std::endl;
	//	system("pause");
	//}
	//std::cout << mainWindow.ReadClipboardText() << std::endl;
	// message loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	system("pause");
	return 0;
}catch(const Exception& ex) {
	Tools::PrintDebugMessage(ex.what());
	system("pause");
}
