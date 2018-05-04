#pragma once
#include "BaseWindow.hh"

class ClientWindow : public BaseWindow<ClientWindow> {
public:
	PCWSTR ClassName() const override { return L"ClientWindowClass"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};
