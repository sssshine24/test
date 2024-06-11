#pragma once


// GeneralBilinearTab dialog

class GeneralBilinearTab : public CPropertyPage
{
	DECLARE_DYNAMIC(GeneralBilinearTab)

public:
	int m_e00,m_e01,m_e10,m_e11,m_f00,m_f01,m_f10,m_f11;
	int m_g00,m_g01,m_g10,m_g11,m_h00,m_h01,m_h10,m_h11;

	GeneralBilinearTab();
	virtual ~GeneralBilinearTab();

// Dialog Data
	enum { IDD = IDD_GeneralBilinearTransTab };

protected:
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
