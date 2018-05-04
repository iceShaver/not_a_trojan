#include "ClientHandler.h"



ClientHandler::ClientHandler(SOCKET clientSocket)
	: clientSocket(clientSocket)
{
	char buf[256];
	int iResult = recv(clientSocket, buf, 256, NULL);
	buf[iResult] = 0;
	MessageBox(NULL, buf, "Wiadomosc", MB_OK);
}

ClientHandler::~ClientHandler()
{
}
