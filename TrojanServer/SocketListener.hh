#pragma once
#include "ServerWindow.hh"

class SocketListener
{
public:
	explicit SocketListener(ServerWindow *pMainWindow);
	SocketListener(const SocketListener&) = delete;
	SocketListener(SocketListener&&) = default;
	~SocketListener();
	void BlockingListen();
private:
	ServerWindow * pMainWindow;
	SOCKET listenSocket;
};



