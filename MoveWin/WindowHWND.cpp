
/*

	WindowHWND -- A nice windows wrapper for some basic Windows API calls dealing with dialogs and windows.
	Author Jacob Psimos 2016 updated 2022
*/

#include "pch.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <locale>
#include <Windows.h>
#include "WindowHWND.h"

WindowHWND::WindowHWND(HWND hwnd) {
	const size_t windowTextLength = GetWindowTextLength(hwnd);
	std::unique_ptr<TCHAR> buffer(new TCHAR[windowTextLength + 2]);
	mHwndPtr = hwnd;
	mProcessId = GetWindowThreadProcessId(hwnd, NULL);
	if (windowTextLength > 0) {
		GetWindowText(hwnd, (LPWSTR)buffer.get(), windowTextLength + 1);
		mWindowTitle = buffer.get();
		GetWindowDimensions();
	}
	else {
		std::wstringstream nullTitle;
		nullTitle << L"NULL (PID: " << (unsigned int)mProcessId << L")";
		mWindowTitle = nullTitle.str();
	}
}

void WindowHWND::SetLocation(int x, int y) {
	SetWindowPos(mHwndPtr, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

std::wstring WindowHWND::getTitle() const{
	return mWindowTitle;
}

int WindowHWND::getWidth() const{
	return mWidth;
}

int WindowHWND::getHeight() const{
	return mHeight;
}

int WindowHWND::getLocationX() const{
	return mXPos;
}

int WindowHWND::getLocationY() const{
	return mYPos;
}

BOOL WindowHWND::GetWindowDimensions() {
	RECT rect;
	BOOL result;

	if (!GetWindowRect(mHwndPtr, &rect)) {
		mHeight = -1;
		mWidth = -1;
		mXPos = -1;
		mYPos = -1;
		result = FALSE;
	}
	else {
		mWidth = (rect.right - rect.left);
		mHeight = (rect.bottom - rect.top);
		mXPos = rect.left;
		mYPos = rect.top;
		result = TRUE;
	}

	return result;
}

/* ---------- STATIC CLASS METHODS --------------- */

std::vector<WindowHWND> WindowHWND::getHWNDS() {
	std::vector<WindowHWND> result;
	WNDENUMPROC ptr = reinterpret_cast<WNDENUMPROC>(EnumWindowsProc);
	bool loopComplete = EnumWindows(ptr, (LPARAM)&result);
	return result;
}

BOOL WindowHWND::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	std::vector<WindowHWND>* ptrLPARAM = (std::vector<WindowHWND>*)lParam;
	WindowHWND wnd(hwnd);
	ptrLPARAM->push_back(wnd);
	return TRUE;
}

