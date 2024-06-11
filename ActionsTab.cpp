// Actions.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "ActionsTab.h"


// Actions dialog

IMPLEMENT_DYNAMIC(ActionsTab, CPropertyPage)

ActionsTab::ActionsTab()
	: CPropertyPage(ActionsTab::IDD)
{

}

ActionsTab::~ActionsTab()
{
}

void ActionsTab::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(ActionsTab, CPropertyPage)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &ActionsTab::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// Actions message handlers

BOOL ActionsTab::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//初始化m_tab控件上的属性页标签
	m_tab.InsertItem(0, _T("OCC"));
	m_tab.InsertItem(1, _T("ACIS"));
	//m_tab.InsertItem(0, _T("Bool Operations"));
	//m_tab.InsertItem(1, _T("Tooth"));
	//m_tab.InsertItem(2, _T("EdgeSwap"));
	//m_tab.InsertItem(3, _T("GeoMap"));//GeoMap
	//m_tab.InsertItem(0, _T("设计图优化"));//GeoMap
	//m_tab.InsertItem(4, _T("General"));
	//m_tab.InsertItem(5, _T("Render"));
	//m_tab.InsertItem(6, _T("Area"));
	//m_tab.InsertItem(7, _T("GBilinear"));
	//m_tab.InsertItem(8, _T("Bilinear"));
	//m_tab.InsertItem(9, _T("LeastSquare"));
	//m_tab.InsertItem(10, _T("Tspline"));
	//m_tab.InsertItem(11, _T("LandMark"));
	//m_tab.InsertItem(12, _T("RicciFlow"));
	//m_tab.InsertItem(13, _T("Harmonic"));
	//m_tab.InsertItem(14, _T("Levy"));
	//m_tab.InsertItem(15, _T("Teichmuller"));
	//m_tab.InsertItem(16, _T("Constrained Registration"));
	//m_tab.InsertItem(17, _T("Dynamic Quasiconformal"));//Dynamic Quasiconformal
	//m_tab.InsertItem(1, _T("动态拟共形映射"));//Dynamic Quasiconformal
	//m_tab.InsertItem(18, _T("DataAnalysis"));//DataAnalysis
	//m_tab.InsertItem(2, _T("数据分析"));//DataAnalysis
	//m_tab.InsertItem(19, _T("GeoMap2"));//GeoMap2
	//m_tab.InsertItem(3, _T("设计图分析"));//GeoMap2

	//创建属性页;
	occtab.Create(IDD_DIALOG_OCC, GetDlgItem(IDC_TAB1));
	acistab.Create(IDD_DIALOG_ACIS, GetDlgItem(IDC_TAB1));
	//rendertab.Create(IDD_ActionRender, GetDlgItem(IDC_TAB1));
	//areapreservingtab.Create(IDD_AreaPreserving, GetDlgItem(IDC_TAB1));
	//generalbilineartab.Create(IDD_ActionsGeneralBilinear, GetDlgItem(IDC_TAB1));
	//bilineartab.Create(IDD_ActionsBilinear, GetDlgItem(IDC_TAB1));
	//leastsquaretab.Create(IDD_LeastSquareMethod, GetDlgItem(IDC_TAB1));
	//tsplinetab.Create(IDD_TSPLINE, GetDlgItem(IDC_TAB1));
	//landmartab.Create(IDD_LANDMARK, GetDlgItem(IDC_TAB1));
	//ricciflowtab.Create(IDD_RicciFlow,GetDlgItem(IDC_TAB1));
	//harmonictab.Create(IDD_Harmonic, GetDlgItem(IDC_TAB1));
	//polyominotab.Create(IDD_POLYOMINO, GetDlgItem(IDC_TAB1));
	//polyominotab2.Create(IDD_POLYOMINO2, GetDlgItem(IDC_TAB1));
	//levyleastsquaretab.Create(IDD_LEVY_LEASTSQUARE, GetDlgItem(IDC_TAB1));
	///teichumullertab.Create(IDD_TeichmullerMap, GetDlgItem(IDC_TAB1));
	//ConstrainedRegistrationtab.Create(IDD_DIALOG_ConstrainedRegistration, GetDlgItem(IDC_TAB1));
	//DynamicQuasitab.Create(IDD_DIALOG_DynamicQuasi, GetDlgItem(IDC_TAB1));
	//DataAnalysistab.Create(IDD_DIALOG_DATA_ANALYSIS, GetDlgItem(IDC_TAB1));
	//EdgeSwaptab.Create(IDD_DIALOG_EDGESWAP, GetDlgItem(IDC_TAB1));
	//toothtab.Create(IDD_DIALOG_TOOTH, GetDlgItem(IDC_TAB1));
	//booloperationtab.Create(IDD_DIALOG_BOOLOPERATION, GetDlgItem(IDC_TAB1));
	//获取TAB的客户端矩形框，从而设置各属性页在TAB上的物理位置
	CRect rs;
	m_tab.GetClientRect(&rs);
	rs.top += 20;
	rs.bottom -= 4;
	rs.left += 4;
	rs.right -= 4;

	//设置属性页的大小和位置
	acistab.MoveWindow(&rs);
	occtab.MoveWindow(&rs);
	//generaltab.MoveWindow(&rs);
	//generalbilineartab.MoveWindow(&rs);
	//bilineartab.MoveWindow(&rs);
	//rendertab.MoveWindow(&rs);
	//leastsquaretab.MoveWindow(&rs);
	//tsplinetab.MoveWindow(&rs);
	//areapreservingtab.MoveWindow(&rs);
	//landmartab.MoveWindow(&rs);
	//ricciflowtab.MoveWindow(&rs);
	//harmonictab.MoveWindow(&rs);
	//polyominotab.MoveWindow(&rs);
	//polyominotab2.MoveWindow(&rs);
	//levyleastsquaretab.MoveWindow(&rs);
	//teichumullertab.MoveWindow(&rs);
	//ConstrainedRegistrationtab.MoveWindow(&rs);
	//DynamicQuasitab.MoveWindow(&rs);
	//DataAnalysistab.MoveWindow(&rs);
	//EdgeSwaptab.MoveWindow(&rs);
	//toothtab.MoveWindow(&rs);
	//booloperationtab.MoveWindow(&rs);
	//show the first page
	//booloperationtab.ShowWindow(TRUE);
	occtab.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ActionsTab::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	switch (m_tab.GetCurSel())
	{
	case 0:
		occtab.ShowWindow(TRUE);
		acistab.ShowWindow(FALSE);
		//bilineartab.ShowWindow(FALSE);
		//rendertab.ShowWindow(FALSE);
		//areapreservingtab.ShowWindow(FALSE);
		//generaltab.ShowWindow(FALSE);
		//generalbilineartab.ShowWindow(FALSE);
		//leastsquaretab.ShowWindow(FALSE);
		//tsplinetab.ShowWindow(FALSE);
		//landmartab.ShowWindow(FALSE);
		////ricciflowtab.ShowWindow(FALSE);
		//harmonictab.ShowWindow(FALSE);
		//polyominotab.ShowWindow(FALSE);
		//polyominotab2.ShowWindow(FALSE);
		//levyleastsquaretab.ShowWindow(FALSE);
		//teichumullertab.ShowWindow(FALSE);
		//ConstrainedRegistrationtab.ShowWindow(FALSE);
		//DynamicQuasitab.ShowWindow(FALSE);
		//DataAnalysistab.ShowWindow(FALSE);
		//EdgeSwaptab.ShowWindow(FALSE);
		//toothtab.ShowWindow(FALSE);
		//booloperationtab.ShowWindow(TRUE);
		break;
	case 1:
		occtab.ShowWindow(FALSE);
		acistab.ShowWindow(TRUE);
		//bilineartab.ShowWindow(FALSE);
		//rendertab.ShowWindow(FALSE);
		//areapreservingtab.ShowWindow(FALSE);
		//generaltab.ShowWindow(FALSE);
		//generalbilineartab.ShowWindow(FALSE);
		//leastsquaretab.ShowWindow(FALSE);
		//tsplinetab.ShowWindow(FALSE);
		//landmartab.ShowWindow(FALSE);
		////ricciflowtab.ShowWindow(FALSE);
		//harmonictab.ShowWindow(FALSE);
		//polyominotab.ShowWindow(FALSE);
		//polyominotab2.ShowWindow(FALSE);
		//levyleastsquaretab.ShowWindow(FALSE);
		//teichumullertab.ShowWindow(FALSE);
		//ConstrainedRegistrationtab.ShowWindow(FALSE);
		//DynamicQuasitab.ShowWindow(FALSE);
		//DataAnalysistab.ShowWindow(FALSE);
		//EdgeSwaptab.ShowWindow(FALSE);
		//toothtab.ShowWindow(TRUE);
		//booloperationtab.ShowWindow(FALSE);
		break;

	default:;
	}
}
