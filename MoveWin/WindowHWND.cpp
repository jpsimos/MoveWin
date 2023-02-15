
/*

	WindowHWND -- A nice window wrapper for some basic Windows API calls dealing with dialogs and windows.
	Author Jacob Psimos 2016 updated 2023
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
	CONST size_t windowTextLength = GetWindowTextLength(hwnd);
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
		nullTitle << L"NULL (PID: " << (INT)mProcessId << L")";
		mWindowTitle = nullTitle.str();
	}
}

WindowHWND::WindowHWND(const WindowHWND& original)
{
	mWindowTitle = original.mWindowTitle;
	mWidth = original.mWidth;
	mHeight = original.mHeight;
	mXPos = original.mXPos;
	mYPos = original.mYPos;
	mHwndPtr = original.mHwndPtr;
	mProcessId = original.mProcessId;
}

BOOL WindowHWND::operator>(CONST WindowHWND& other)
{
	return (0 < mWindowTitle.compare(other.mWindowTitle));
}

BOOL WindowHWND::operator>=(const WindowHWND& other)
{
	return (0 <= mWindowTitle.compare(other.mWindowTitle));
}

BOOL WindowHWND::operator<(const WindowHWND& other)
{
	return (0 > mWindowTitle.compare(other.mWindowTitle));
}

BOOL WindowHWND::operator<=(const WindowHWND& other)
{
	return (mWindowTitle.compare(other.mWindowTitle) <= 0);
}

BOOL WindowHWND::operator==(const WindowHWND& other)
{
	BOOL equalityAndCondition = Hashify() + other.Hashify();
	return (equalityAndCondition == (BOOL)mWindowTitle.compare(other.mWindowTitle));
}

INT WindowHWND::Hashify() CONST
{
	INT customIdentificationHash = mHeight + mWidth + mXPos + mYPos;

	for (std::wstring::const_iterator i = mWindowTitle.begin(); i < mWindowTitle.end(); i++)
	{
		customIdentificationHash += (INT)(*i);
	}

	customIdentificationHash = ~customIdentificationHash;

	return customIdentificationHash;
}

VOID WindowHWND::SetLocation(INT x, INT y) {
	SetWindowPos(mHwndPtr, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

std::wstring WindowHWND::getTitle() CONST{
	return mWindowTitle;
}

INT WindowHWND::getWidth() CONST{
	return mWidth;
}

INT WindowHWND::getHeight() CONST{
	return mHeight;
}

INT WindowHWND::getLocationX() CONST{
	return mXPos;
}

INT WindowHWND::getLocationY() CONST{
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

std::shared_ptr<std::vector<WindowHWND>> WindowHWND::getHWNDS() {
	std::shared_ptr<std::vector<WindowHWND>> result(new std::vector <WindowHWND>);
	WNDENUMPROC ptr = reinterpret_cast<WNDENUMPROC>(EnumWindowsProc);
	BOOL loopComplete = EnumWindows(ptr, (LPARAM)&result);
	std::sort(result.get()->begin(), result.get()->end());
	return result;
}

BOOL WindowHWND::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	WindowHWND wnd(hwnd);
	if(wnd.getTitle().find(L"NULL", 0) != 0)
		((std::shared_ptr<std::vector<WindowHWND>>*)lParam)->get()->push_back(wnd);
	return TRUE;
}

