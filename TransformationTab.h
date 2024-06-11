#pragma once
#include "afxcmn.h"
#include "MobiusTab.h"
#include "BilinearTab.h"
#include "QuadraticTab.h"
#include "GeneralBilinearTab.h"
#include "CompositeMobiusTab.h"

// TransformationTab dialog

class TransformationTab : public CPropertyPage
{
	DECLARE_DYNAMIC(TransformationTab)

public:
	TransformationTab();
	virtual ~TransformationTab();

// Dialog Data
	enum { IDD = IDD_TransformationsTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
public:

	afx_msg void OnNMThemeChangedMobiusAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tab;
	MobiusTab mobiustab;
	BilinearTab bilineartab;
	QuadraticTab quadratictab;
	GeneralBilinearTab generalbilineartab;
	CompositeMobiusTab compositemobiustab;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTransformationtab(NMHDR *pNMHDR, LRESULT *pResult);
};
