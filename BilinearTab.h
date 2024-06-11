#pragma once


// BilinearTab dialog

class BilinearTab : public CPropertyPage
{
	DECLARE_DYNAMIC(BilinearTab)

public:
	int     m_Bilinear_Alpha1,m_Bilinear_Alpha2,m_Bilinear_Beta1,m_Bilinear_Beta2;
	BilinearTab();
	virtual ~BilinearTab();

// Dialog Data
	enum { IDD = IDD_BilinearTransTab };

protected:
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
