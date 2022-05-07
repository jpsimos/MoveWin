#include "pch.h"
#include "framework.h"
#include "MoveWin.h"
#include "MoveWinDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>
#include <locale>
#include <codecvt>
#include "WindowHWND.h"
#include "CSelectPositionDlg.h"

CSelectPositionDlg::CSelectPositionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECTPOSITION_DIALOG, pParent)
{
	
}

BEGIN_MESSAGE_MAP(CSelectPositionDlg, CDialogEx)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CSelectPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	return CDialogEx::DoDataExchange(pDX);
}

BOOL CSelectPositionDlg::OnInitDialog()
{
	BOOL result;
	CRect rect;
	if ((result = CDialogEx::OnInitDialog())) {
		SetParent(GetDesktopWindow());
		//GetDesktopWindow()->GetWindowRect(&rect);
		rect.right = GetSystemMetrics(SM_CXMAXTRACK);
		rect.left = 0;
		rect.bottom = GetSystemMetrics(SM_CYMAXTRACK);
		rect.top = 0;
		SetWindowPos(&wndTopMost, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
		//SetBackgroundColor(RGB(1, 11, 21));
		::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(255, 255, 255), 0x7F, LWA_COLORKEY | LWA_ALPHA);
	}
	return result;
}


BOOL CSelectPositionDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CSelectPositionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CDialogEx::OnLButtonDown(nFlags, point);
	mSelectedX = point.x + 5;
	mSelectedY = point.y + 5;
	CDialogEx::OnOK();
}
