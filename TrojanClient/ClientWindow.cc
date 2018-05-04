#include "ClientWindow.hh"
#include <iostream>

LRESULT ClientWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_COPY:
		std::cout << "clipboard update" << std::endl;
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(m_hwnd, &ps);
	}

	return 0;
	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}
}
