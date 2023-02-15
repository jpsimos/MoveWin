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

BOOL Grab(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM arg)
{
	BOOL happy;
	DimensionSummation* dimensions = (DimensionSummation*)arg;

	if ((happy = (NULL != dimensions && NULL != rect)))
	{

		if (rect->left < dimensions->leftmostPosition)
			dimensions->leftmostPosition = rect->left;

		if (rect->bottom > dimensions->maximumBottom)
			dimensions->maximumBottom = rect->bottom;

		if (rect->top < dimensions->minimumTop)
			dimensions->minimumTop = rect->top;

		dimensions->totalWidth += (rect->right - rect->left);

		dimensions->summation.top += rect->top;
		dimensions->summation.left += rect->left;
		dimensions->summation.bottom += rect->bottom;
		dimensions->summation.right += rect->right;
	}
	return happy;
}

BOOL CSelectPositionDlg::OnInitDialog()
{
	
	BOOL result;
	CRect rect;

	if ((result = CDialogEx::OnInitDialog())) {

		ZeroMemory(&mMonitorDimensions, sizeof(mMonitorDimensions));

		if (TRUE == EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)Grab, (LPARAM)&mMonitorDimensions))
		{
			SetWindowPos(GetParent(), mMonitorDimensions.leftmostPosition, mMonitorDimensions.minimumTop, 
				mMonitorDimensions.totalWidth, mMonitorDimensions.maximumBottom, SWP_SHOWWINDOW);
			SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
			::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(255, 255, 255), 0x7F, LWA_COLORKEY | LWA_ALPHA);
		}

	}
	return result;
}


BOOL CSelectPositionDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CSelectPositionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	mSelectedX = mMonitorDimensions.leftmostPosition + point.x + 5;
	mSelectedY = mMonitorDimensions.maximumBottom - (mMonitorDimensions.maximumBottom - point.y) + 5;
	CDialogEx::OnOK();
}
