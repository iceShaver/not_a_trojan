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
	virtual ~BaseWindow();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	explicit BaseWindow(HINSTANCE hInstance = NULL) : hwnd(nullptr), hInstance(hInstance) {}

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
	HWND Window()const { return hwnd; }

protected:
	virtual PCWSTR ClassName()const = 0;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND hwnd;
	HINSTANCE hInstance;
};

template <class DERIVED_TYPE>
BaseWindow<DERIVED_TYPE>::~BaseWindow() {
	::PostMessage(this->hwnd, WM_CLOSE, 0, 0);
}

template <class DERIVED_TYPE>
LRESULT BaseWindow<DERIVED_TYPE>::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	DERIVED_TYPE* pThis = NULL;
	if (uMsg == WM_NCCREATE) {
		auto pCreateStruct = (CREATESTRUCT*)lParam;
		pThis = (DERIVED_TYPE*)pCreateStruct->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		pThis->hwnd = hwnd;
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
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(230));
	RegisterClass(&wc);
	hwnd = CreateWindowEx(
		dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(nullptr),
		this
	);
	if(!hwnd) {
		throw WindowCreationException("Error has occured while creating new window: " + Winapi::GetErrorMessage());
	}
	return (hwnd ? TRUE : FALSE);
}

template <class DERIVED_TYPE>
void BaseWindow<DERIVED_TYPE>::Show(int nCmdShow) const { ShowWindow(hwnd, nCmdShow); }
