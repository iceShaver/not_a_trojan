#pragma once
#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)
#endif
#ifndef UNICODE
#define UNICODE
#endif//UNICODE
#include <Windows.h>
#include "Exceptions.hh"
#include "WinapiHelpers.hh"


template <class DERIVED_TYPE>
class BaseWindow {
public:
	virtual ~BaseWindow() = default;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	BaseWindow() : m_hwnd(nullptr) {}

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = nullptr,
		HMENU hMenu = nullptr);

	void Show(int nCmdShow) const;
	HWND Window()const { return m_hwnd; }

protected:
	virtual PCWSTR ClassName()const = 0;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};

template <class DERIVED_TYPE>
LRESULT BaseWindow<DERIVED_TYPE>::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	DERIVED_TYPE* pThis = NULL;
	if (uMsg == WM_NCCREATE) {
		auto pCreateStruct = (CREATESTRUCT*)lParam;
		pThis = (DERIVED_TYPE*)pCreateStruct->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		pThis->m_hwnd = hwnd;
	}
	else {
		pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}
	if (pThis) {
		return pThis->HandleMessage(uMsg, wParam, lParam);
	}
	else {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

template <class DERIVED_TYPE>
BOOL BaseWindow<DERIVED_TYPE>::Create(
	PCWSTR lpWindowName,
	DWORD dwStyle,
	DWORD dwExStyle,
	int x, int y, int nWidth, int nHeight,
	HWND hWndParent,
	HMENU hMenu)
{

	WNDCLASS wc = {};
	wc.lpfnWndProc = DERIVED_TYPE::WndProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = ClassName();
	RegisterClass(&wc);
	m_hwnd = CreateWindowEx(
		dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(nullptr),
		this
	);
	if(!m_hwnd) {
		throw WindowCreationException("Error has occured while creating new window: " + Winapi::getErrorMessage());
	}
	return (m_hwnd ? TRUE : FALSE);
}

template <class DERIVED_TYPE>
void BaseWindow<DERIVED_TYPE>::Show(int nCmdShow) const { ShowWindow(m_hwnd, nCmdShow); }
