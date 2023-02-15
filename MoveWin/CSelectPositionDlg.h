#pragma once

typedef struct DimensionSummation
{
    int minimumTop;
    int maximumBottom;
    int totalWidth;
    int leftmostPosition;
    RECT summation;
}DimensionSummation;

class CSelectPositionDlg :
    public CDialogEx
{
public:
    CSelectPositionDlg(CWnd* pParent = nullptr);

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SELECTPOSITION_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
public:
    int mSelectedX;
    int mSelectedY;
private:
    DimensionSummation mMonitorDimensions;
public:
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

