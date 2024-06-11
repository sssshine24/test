#pragma once


// QuadraticTab dialog

class QuadraticTab : public CPropertyPage
{
	DECLARE_DYNAMIC(QuadraticTab)

public:
	int     m_Quadratic_Alpha1,m_Quadratic_Alpha2,m_Quadratic_Alpha3,m_Quadratic_Beta1,m_Quadratic_Beta2,m_Quadratic_Beta3;
	QuadraticTab();
	virtual ~QuadraticTab();

// Dialog Data
	enum { IDD = IDD_QuadraticTransTab };

protected:
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
