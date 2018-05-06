#include "ServerWindow.h"
#include <iostream>

PCWSTR ServerWindow::ClassName() const { return L"SampleWindowClass"; }

LRESULT ServerWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(this->hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(this->hwnd, &ps);
		}
		return 0;
	default:
		return DefWindowProc(this->hwnd, uMsg, wParam, lParam);
	}
}

void ServerWindow::EmergencyExit(std::string message) {
	std::cerr << "EmergencyExit called!\n" << message << std::endl;
	system("pause");
	SendMessage(this->hwnd, WM_CLOSE, NULL, NULL);
}

