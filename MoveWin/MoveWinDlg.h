
// MoveWinDlg.h : header file
//

#pragma once

#include <vector>
#include "WindowHWND.h"

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

// CMoveWinDlg dialog
class CMoveWinDlg : public CDialogEx
{
// Construction
public:
	CMoveWinDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOVEWIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Member Variables
private:
	std::vector<WindowHWND> mWindowHWNDs;
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	void RefreshAllWindowTitles(const TCHAR* strFilter);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void textSearchFilter_OnChange();
	afx_msg void OnBnClickedButtonMoveSelectedWindow();
};
