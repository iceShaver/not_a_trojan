#include "TrojanServer.hh"
#include "WinapiHelpers.hh"
#include <iostream>

//TrojanServer::TrojanServer(){
//	this->mainWindowHandle = CreateWindow(
//		nullptr,				// lpClassName
//		Config::WND_TITLE,	// lpWindowName
//		WS_OVERLAPPEDWINDOW,	// dwStyle
//		CW_USEDEFAULT,		// x
//		0,						// y
//		CW_USEDEFAULT,		// nWidth
//		0,						// nHeight
//		nullptr,				// hwndParent
//		nullptr,				// hMenu
//		nullptr,				// hInstance
//		nullptr					// lpParam
//	);
//	std::cout << Winapi::getErrorMessage() << std::endl;
//	ShowWindow(this->mainWindowHandle, SW_SHOW);
//	this->run();
//}


TrojanServer::TrojanServer(HINSTANCE hInstance, int nCmdShow) :
	serverState(ServerState::UNINITIALIZED),
	mainWindowHandle(nullptr),
	hInstance(hInstance),
	nCmdShow(nCmdShow) {
	registerWindowClass();
}

TrojanServer::~TrojanServer()
{

}

void TrojanServer::registerWindowClass() {
	
}

void TrojanServer::run() {
	createWindow();
	serverState = ServerState::RUNNING;
	while (serverState != ServerState::TERMINATED) {

	}
}

void TrojanServer::emergecyExit() {
	exit(1);
}

void TrojanServer::createWindow() {

}
