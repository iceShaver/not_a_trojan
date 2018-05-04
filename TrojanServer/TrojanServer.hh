#ifndef TROJANSERVER_H
#define TROJANSERVER_H
#include "ServerState.hh"
#include "Windows.h"
class TrojanServer
{
public:
	
	TrojanServer(HINSTANCE hInstance, int nCmdShow);
	~TrojanServer();
	TrojanServer(const TrojanServer&) = delete;
	TrojanServer(TrojanServer&&) = default;
	void registerWindowClass();
	void run();
	void emergecyExit();
private:
	void createWindow();

	ServerState serverState;
	HWND mainWindowHandle;
	HINSTANCE hInstance;
	int nCmdShow;
};

#endif // TROJANSERVER_H
