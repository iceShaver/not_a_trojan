
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
#include "WinapiHelpers.hh"
#include "ServerWindow.hh"
#include "ServerConfig.hh"
#include "SocketListener.hh"
#include <future>
#include "Console.hh"
#include "Tools.hh"
#include "Thread.hh"

#pragma comment(lib, "Ws2_32.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, int nCmdShow)try {
#ifdef _DEBUG
	Console::CreateConsole();
#endif // _DEBUG
	auto mainWindow = ServerWindow();
	if (!mainWindow.Create(ServerConfig::WND_TITLE, WS_OVERLAPPEDWINDOW)) {
		throw WindowCreationException(Winapi::GetErrorMessage());
	}
	mainWindow.Show(nCmdShow);

	// listen for clients
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
catch (const std::exception & ex) {
	Tools::PrintDebugMessage(ex.what());
	system("pause");
}
