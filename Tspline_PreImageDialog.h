#pragma once


// Tspline_PreImageDialog dialog
class Tspline;
class Tspline_PreImageDialog : public CDialog
{
	DECLARE_DYNAMIC(Tspline_PreImageDialog)

public:
	Tspline_PreImageDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~Tspline_PreImageDialog();

	Tspline* pTspline;
	int xmax,ymax;
// Dialog Data
	enum { IDD = IDD_TSPLINE_PREIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
