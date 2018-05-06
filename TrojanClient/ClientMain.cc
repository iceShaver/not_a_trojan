
#ifndef UNICODE
#define UNICODE
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <iostream>
#include "WinapiHelpers.hh"
#include "ClientConfig.hh"
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


	std::thread give_me_a_name([&]() {mainWindow.BlockingListen(); });

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}catch(const std::exception& ex) {
	Tools::PrintDebugMessage("Global exception: "s + ex.what());
	system("pause");
}
