#pragma once
#include "BaseWindow.hh"
#include <string>
class ServerWindow : public BaseWindow<ServerWindow>
{	
public:
	PCWSTR ClassName() const override;

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void EmergencyExit(std::string message);
};

