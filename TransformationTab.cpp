// TransformationTab.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "TransformationTab.h"
#include "FormView3.h"

// TransformationTab dialog

IMPLEMENT_DYNAMIC(TransformationTab, CPropertyPage)

TransformationTab::TransformationTab()
	: CPropertyPage(TransformationTab::IDD)
{

}

TransformationTab::~TransformationTab()
{

}

void TransformationTab::DoDataExchange(CDataExchange* pDX)
{

	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TransformationTAB, m_tab);
}

void TransformationTab::OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	p_FormView3->UpdateSliderPosition();
	*pResult = 0;
}
BEGIN_MESSAGE_MAP(TransformationTab, CPropertyPage)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_THEMECHANGED, IDC_Mobius_Alpha, &TransformationTab::OnNMThemeChangedMobiusAlpha)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TransformationTAB, &TransformationTab::OnTcnSelchangeTransformationtab)
END_MESSAGE_MAP()


// TransformationTab message handlers

void TransformationTab::OnNMThemeChangedMobiusAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	p_FormView3->UpdateSliderPosition();
	*pResult = 0;
}
void TransformationTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	p_FormView3->UpdateSliderPosition();

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
BOOL TransformationTab::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//初始化m_tab控件上的属性页标签
	m_tab.InsertItem(0,_T("莫比乌斯法"));//Mobius
	m_tab.InsertItem(1,_T("组合变换法"));//Composite
	m_tab.InsertItem(2,_T("双线性法"));//Bilinear
	m_tab.InsertItem(3,_T("二次变换法"));//Quadratic
	m_tab.InsertItem(4,_T("一般双线性法"));//GeneralBilinear

	//创建属性页;
	mobiustab.Create(IDD_MobiusTransTab,GetDlgItem(IDC_TransformationTAB));
	bilineartab.Create(IDD_BilinearTransTab,GetDlgItem(IDC_TransformationTAB));
	quadratictab.Create(IDD_QuadraticTransTab,GetDlgItem(IDC_TransformationTAB));
	generalbilineartab.Create(IDD_GeneralBilinearTransTab,GetDlgItem(IDC_TransformationTAB));
	compositemobiustab.Create(IDD_CompositeMobiusTransTab,GetDlgItem(IDC_TransformationTAB));

	// TODO:  Add extra initialization here
	//获取TAB的客户端矩形框，从而设置各属性页在TAB上的物理位置
	CRect rs;
	m_tab.GetClientRect(&rs);
	rs.top+=20;
	rs.bottom-=4;
	rs.left+=4;
	rs.right-=4;

	//设置属性页的大小和位置
	mobiustab.MoveWindow(&rs);
	bilineartab.MoveWindow(&rs);
	quadratictab.MoveWindow(&rs);
	generalbilineartab.MoveWindow(&rs);
	compositemobiustab.MoveWindow(&rs);

	//默认第一页显示
	mobiustab.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void TransformationTab::OnTcnSelchangeTransformationtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	switch(m_tab.GetCurSel())
	{
	case 0:
		mobiustab.ShowWindow(TRUE);
		compositemobiustab.ShowWindow(FALSE);
		bilineartab.ShowWindow(FALSE);
		quadratictab.ShowWindow(FALSE);
		generalbilineartab.ShowWindow(FALSE);
		break;
	case 1:
		mobiustab.ShowWindow(FALSE);
		compositemobiustab.ShowWindow(TRUE);
		bilineartab.ShowWindow(FALSE);
		quadratictab.ShowWindow(FALSE);
		generalbilineartab.ShowWindow(FALSE);
		break;
	case 2:
		mobiustab.ShowWindow(FALSE);
		compositemobiustab.ShowWindow(FALSE);
		bilineartab.ShowWindow(TRUE);
		quadratictab.ShowWindow(FALSE);
		generalbilineartab.ShowWindow(FALSE);
		break;
	case 3:
		mobiustab.ShowWindow(FALSE);
		compositemobiustab.ShowWindow(FALSE);
		bilineartab.ShowWindow(FALSE);
		quadratictab.ShowWindow(TRUE);
		generalbilineartab.ShowWindow(FALSE);
		break;
	case 4:
		mobiustab.ShowWindow(FALSE);
		compositemobiustab.ShowWindow(FALSE);
		bilineartab.ShowWindow(FALSE);
		quadratictab.ShowWindow(FALSE);
		generalbilineartab.ShowWindow(TRUE);
		break;
	default: ;
	}
	*pResult = 0;
}
