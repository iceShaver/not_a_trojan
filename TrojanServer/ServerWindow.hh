#pragma once
#include "BaseWindow.hh"
#include <string>
class ServerWindow : public BaseWindow<ServerWindow>
{	
public:

	ServerWindow();

	PCWSTR ClassName() const override;

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
private:

};

