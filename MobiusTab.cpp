// MobiusTab.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "MobiusTab.h"
#include "FormView3.h"
#include <vector>
#include <algorithm>
#include "lib3D\base3d.h"
#include "Nurbs3D.h"
#include "mobiustransformation.h"
#include "Mytreeview.h"
// MobiusTab dialog

IMPLEMENT_DYNAMIC(MobiusTab, CPropertyPage)

MobiusTab::MobiusTab()
	: CPropertyPage(MobiusTab::IDD)
{
	m_Mobiustransformation_Alpha = 50;
	m_Mobiustransformation_Beta = 50;
}

MobiusTab::~MobiusTab()
{
}

void MobiusTab::DoDataExchange(CDataExchange* pDX)
{
	DDX_Slider(pDX, IDC_Mobius_Alpha, m_Mobiustransformation_Alpha);
	DDX_Slider(pDX, IDC_Mobius_Beta, m_Mobiustransformation_Beta);
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MobiusTab, CPropertyPage)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void MobiusTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
}

// MobiusTab message handlers
