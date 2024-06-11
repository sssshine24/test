#pragma once
#include "afxcmn.h"


// Actions_General dialog

class Actions_General : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_General)

public:
	Actions_General();
	virtual ~Actions_General();

// Dialog Data
	enum { IDD = IDD_ActionsGeneral };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRotationParameterization();
	afx_msg void OnBnClickedStopsequence();
	afx_msg void OnBnClickedAnimationsequence();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m;
	afx_msg void OnNMCustomdrawTimeslider(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slider;
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedReadfile();
	afx_msg void OnBnClickedSavefile();
};
