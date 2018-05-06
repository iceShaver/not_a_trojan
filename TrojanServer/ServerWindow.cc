#include "ServerWindow.hh"
#include <iostream>
#include "Button.hh"

ServerWindow::ServerWindow() {
	
}
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