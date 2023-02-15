#ifndef __WINDOWHWND__H
#define __WINDOWHWND__H


/*

	A nice window wrapper for some basic Windows API calls dealing with dialogs and windows.

	Author Jacob Psimos 2016 updated 2023
*/

#include <vector>
#include <string>
#include <Windows.h>
#include <memory>

class WindowHWND {

public:
	WindowHWND(HWND);
	WindowHWND(CONST WindowHWND& original);
	BOOL operator>(CONST WindowHWND& other);
	BOOL operator>=(CONST WindowHWND& other);
	BOOL operator<(CONST WindowHWND& other);
	BOOL operator<=(CONST WindowHWND& other);
	BOOL operator==(CONST WindowHWND& other);
	INT Hashify() CONST;
public:
	VOID SetLocation(INT x, INT y);
	BOOL GetWindowDimensions();
	std::wstring getTitle() CONST;
	INT getWidth() CONST;
	INT getHeight() CONST;
	INT getLocationX() CONST;
	INT getLocationY() CONST;
	static std::shared_ptr<std::vector<WindowHWND>> getHWNDS();
private:
	std::wstring mWindowTitle;
	INT mWidth;
	INT mHeight;
	INT mXPos;
	INT mYPos;
	HWND mHwndPtr;
	DWORD mProcessId;
	static BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);
};



#endif