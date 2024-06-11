#pragma once


// MobiusTab dialog

class MobiusTab : public CPropertyPage
{
	DECLARE_DYNAMIC(MobiusTab)

public:
	int     m_Mobiustransformation_Alpha,m_Mobiustransformation_Beta;

	MobiusTab();
	virtual ~MobiusTab();

// Dialog Data
	enum { IDD = IDD_MobiusTransTab };

protected:
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
