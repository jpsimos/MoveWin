
// MoveWinDlg.cpp : implementation file
//

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

// CAboutDlg dialog used for App About
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMoveWinDlg dialog


CMoveWinDlg::CMoveWinDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOVEWIN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMoveWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMoveWinDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMoveWinDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMoveWinDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_TXTSEARCHFILTER, &CMoveWinDlg::textSearchFilter_OnChange)
	ON_BN_CLICKED(IDC_BUTTONMOVEWINDOW, &CMoveWinDlg::OnBnClickedButtonMoveSelectedWindow)
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CMoveWinDlg message handlers

BOOL CMoveWinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, TRUE);		// Set small icon

	RefreshAllWindowTitles(NULL);
	
	CEdit* txtfilter = (CEdit*)GetDlgItem(IDC_TXTSEARCHFILTER);
	txtfilter->SetFocus();

	return FALSE;
}

void CMoveWinDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CMoveWinDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMoveWinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMoveWinDlg::RefreshAllWindowTitles(const TCHAR* strFilter)
{
	CListBox* lstWindowHWNDS = static_cast<CListBox*>(GetDlgItem(IDC_WINDOWTITLELIST));
	pWindowHWNDs = WindowHWND::getHWNDS();
	lstWindowHWNDS->ResetContent();
	for (std::vector<WindowHWND>::const_iterator i = pWindowHWNDs.get()->begin(); i != pWindowHWNDs.get()->end(); i++) {
		if (strFilter != NULL) {
			if ((*i).getTitle().find(strFilter) == std::wstring::npos)
				continue;
		}
		lstWindowHWNDS->AddString((*i).getTitle().c_str());
	}
}

void CMoveWinDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CMoveWinDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CMoveWinDlg::textSearchFilter_OnChange()
{
	CEdit* txtSearchFilter = static_cast<CEdit*>(GetDlgItem(IDC_TXTSEARCHFILTER));
	std::unique_ptr<TCHAR> strSearchFilter(new TCHAR[100 + 2]);
	int strSearchFilterLength = txtSearchFilter->GetWindowText(strSearchFilter.get(), 100 + 1);
	RefreshAllWindowTitles(strSearchFilterLength > 0 ? strSearchFilter.get() : NULL);
	CListBox* lstWindows = static_cast<CListBox*>(GetDlgItem(IDC_WINDOWTITLELIST));
	
}


void CMoveWinDlg::OnBnClickedButtonMoveSelectedWindow()
{
	CListBox* lstWindows = static_cast<CListBox*>(GetDlgItem(IDC_WINDOWTITLELIST));
	int selectedIndex = lstWindows->GetCurSel();
	std::unique_ptr<TCHAR> selectedText;
	bool selectedLocation = false;
	int selectedX;
	int selectedY;
	if (selectedIndex > -1) {
		const int selectedTextLength = lstWindows->GetTextLen(selectedIndex);
		selectedText.reset(new TCHAR[selectedTextLength + 2]);
		lstWindows->GetText(selectedIndex, selectedText.get());
		for (std::vector<WindowHWND>::iterator i = pWindowHWNDs.get()->begin(); i != pWindowHWNDs.get()->end(); i++) {
			if (i->getTitle() == reinterpret_cast<const wchar_t*>(selectedText.get())) {
				if (!selectedLocation) {
					CSelectPositionDlg dlg;
					ShowWindow(SW_HIDE);
					dlg.DoModal();
					ShowWindow(SW_SHOW);
					selectedX = dlg.mSelectedX;
					selectedY = dlg.mSelectedY;
					selectedLocation = true;
				}
				i->SetLocation(selectedX, selectedY);
			}
		}
	}
}


void CMoveWinDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);
}


void CMoveWinDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	static bool alreadyFocusedOnSearch = false;
	if (!alreadyFocusedOnSearch)
	{
		CEdit* txtfilter = (CEdit*)this->GetDlgItem(IDC_TXTSEARCHFILTER);
		txtfilter->SetFocus();
		Invalidate();
		alreadyFocusedOnSearch = true;
	}
}

