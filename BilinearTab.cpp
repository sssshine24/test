// BilinearTab.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "BilinearTab.h"
#include "FormView3.h"
#include <vector>
#include <algorithm>
#include "lib3D\base3d.h"
#include "Nurbs3D.h"
#include "Bilineartransformation.h"
#include "Mytreeview.h"
// BilinearTab dialog

IMPLEMENT_DYNAMIC(BilinearTab, CPropertyPage)

BilinearTab::BilinearTab()
	: CPropertyPage(BilinearTab::IDD)
{
	m_Bilinear_Alpha1 = 50;
	m_Bilinear_Alpha2 = 50;
	m_Bilinear_Beta1 = 50;
	m_Bilinear_Beta2 = 50;
}

BilinearTab::~BilinearTab()
{
}

void BilinearTab::DoDataExchange(CDataExchange* pDX)
{
	DDX_Slider(pDX, IDC_Bilinear_Alpha1, m_Bilinear_Alpha1);
	DDX_Slider(pDX, IDC_Bilinear_Alpha2, m_Bilinear_Alpha2);
	DDX_Slider(pDX, IDC_Bilinear_Beta1, m_Bilinear_Beta1);
	DDX_Slider(pDX, IDC_Bilinear_Beta2, m_Bilinear_Beta2);
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BilinearTab, CPropertyPage)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void BilinearTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//p_FormView3->UpdateSliderPosition();

	
}
// BilinearTab message handlers
