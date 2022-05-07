#ifndef __WINDOWHWND__H
#define __WINDOWHWND__H


/*

	A nice windows wrapper for some basic Windows API calls dealing with dialogs and windows.

	Author Jacob Psimos 2016 updated 2022
*/

#include <vector>
#include <string>
#include <Windows.h>

class WindowHWND {

public:
	WindowHWND(HWND);
	void SetLocation(int x, int y);
	BOOL GetWindowDimensions();
	std::wstring getTitle() const;
	int getWidth() const;
	int getHeight() const;
	int getLocationX() const;
	int getLocationY() const;
	static std::vector<WindowHWND> getHWNDS();
private:
	std::wstring mWindowTitle;
	int mWidth;
	int mHeight;
	int mXPos;
	int mYPos;
	HWND mHwndPtr;
	DWORD mProcessId;
	static BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);
};



#endif