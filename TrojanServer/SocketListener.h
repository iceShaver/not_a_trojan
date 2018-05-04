#pragma once
#include "ServerWindow.h"

class SocketListener
{
public:
	explicit SocketListener(ServerWindow *pMainWindow);
	SocketListener(const SocketListener&) = delete;
	SocketListener(SocketListener&&) = default;
	~SocketListener();
	void BlockingListen();
	static void StartListening(ServerWindow *pMainWindow);
private:
	template<typename T>
	T recv();
	ServerWindow * pMainWindow;
	SOCKET listenSocket;
};

