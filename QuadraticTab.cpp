// QuadraticTab.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "QuadraticTab.h"
#include "FormView3.h"
#include <vector>
#include <algorithm>
#include "lib3D\base3d.h"
#include "Nurbs3D.h"
#include "Quadratictransformation.h"
#include "Mytreeview.h"
// QuadraticTab dialog

IMPLEMENT_DYNAMIC(QuadraticTab, CPropertyPage)

QuadraticTab::QuadraticTab()
	: CPropertyPage(QuadraticTab::IDD)
{
	/*m_Quadratic_Alpha1 = 50;
	m_Quadratic_Alpha2 = 50;
	m_Quadratic_Alpha3 = 50;

	m_Quadratic_Beta1 = 50;
	m_Quadratic_Beta2 = 50;
	m_Quadratic_Beta3 = 50;*/
	//face
	//m_Quadratic_Alpha1 = 29;
	//m_Quadratic_Alpha2 = 25;
	//m_Quadratic_Alpha3 = 46;
	//m_Quadratic_Beta1 = 74;
	//m_Quadratic_Beta2 = 0;
	//m_Quadratic_Beta3 = 36;
	//bone
	//m_Quadratic_Alpha1 = 26;
	//m_Quadratic_Alpha2 = 38;
	//m_Quadratic_Alpha3 = 26;
	//m_Quadratic_Beta1 = 90;
	//m_Quadratic_Beta2 = 18;
	//m_Quadratic_Beta3 = 54;
	//cartop
	//m_Quadratic_Alpha1 = 58;
	//m_Quadratic_Alpha2 = 49;
	//m_Quadratic_Alpha3 = 30;
	//m_Quadratic_Beta1 = 71;
	//m_Quadratic_Beta2 = 21;
	//m_Quadratic_Beta3 = 54;
	//venus sophia
	//m_Quadratic_Alpha1 = 62;
	//m_Quadratic_Alpha2 = 53;
	//m_Quadratic_Alpha3 = 28;
	//m_Quadratic_Beta1 = 54;
	//m_Quadratic_Beta2 = 21;
	//m_Quadratic_Beta3 = 54;

	//sophia
	//m_Quadratic_Alpha1 = 74;
	//m_Quadratic_Alpha2 = 53;
	//m_Quadratic_Alpha3 = 16;
	//m_Quadratic_Beta1 = 54;
	//m_Quadratic_Beta2 = 21;
	//m_Quadratic_Beta3 = 54;

	//terrain
	m_Quadratic_Alpha1 = 91;
	m_Quadratic_Alpha2 = 58;
	m_Quadratic_Alpha3 = 5;
	m_Quadratic_Beta1 = 49;
	m_Quadratic_Beta2 = 8;
	m_Quadratic_Beta3 = 49;
}

QuadraticTab::~QuadraticTab()
{
}

void QuadraticTab::DoDataExchange(CDataExchange* pDX)
{
	DDX_Slider(pDX, IDC_Quadratic_Alpha1, m_Quadratic_Alpha1);
	DDX_Slider(pDX, IDC_Quadratic_Alpha2, m_Quadratic_Alpha2);
	DDX_Slider(pDX, IDC_Quadratic_Alpha3, m_Quadratic_Alpha3);
	DDX_Slider(pDX, IDC_Quadratic_Beta1, m_Quadratic_Beta1);
	DDX_Slider(pDX, IDC_Quadratic_Beta2, m_Quadratic_Beta2);
	DDX_Slider(pDX, IDC_Quadratic_Beta3, m_Quadratic_Beta3);
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(QuadraticTab, CPropertyPage)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void QuadraticTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

}
// QuadraticTab message handlers
