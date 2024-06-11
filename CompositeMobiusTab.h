#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CompositeMobiusTab dialog
class Transformation;
class CompositeMobiusTab : public CPropertyPage
{
	DECLARE_DYNAMIC(CompositeMobiusTab)

public:
	CompositeMobiusTab();
	virtual ~CompositeMobiusTab();

	Transformation* ptransformation;

	void UpdateSliderPosition(HWND hwndFrom);

// Dialog Data
	enum { IDD = IDD_CompositeMobiusTransTab };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCompositeMobiusUpdate();
	CSliderCtrl m_Slide_Uknot;
	CSliderCtrl m_Slide_Vknot;
	CSliderCtrl m_Slide_Sknot;
	CSliderCtrl m_Slide_Tknot;
	CSliderCtrl m_Slide_Alpha;
	CSliderCtrl m_Slide_Beta;
	CEdit m_Text_Uknot;
	CEdit m_Text_Vknot;
	CEdit m_Text_Sknot;
	CEdit m_Text_Tknot;
	CEdit m_Text_Alpha;
	CEdit m_Text_Beta;
	CStatic m_Static_Uknot;
	CStatic m_Static_Vknot;
	CStatic m_Static_Sknot;
	CStatic m_Static_Tknot;
	CStatic m_Static_Alpha;
	CStatic m_Static_Beta;
	CSpinButtonCtrl m_Spin_Uknot;
	CSpinButtonCtrl m_Spin_Vknot;
	CSpinButtonCtrl m_Spin_Sknot;
	CSpinButtonCtrl m_Spin_Tknot;
	CSpinButtonCtrl m_Spin_Alpha;
	CSpinButtonCtrl m_Spin_Beta;
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedSliderCompositeUknot(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderCompositeUknot(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonGenerateCompositesurface();
};
