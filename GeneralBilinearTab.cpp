// GeneralBilinearTab.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "GeneralBilinearTab.h"
#include "FormView3.h"
#include <vector>
#include <algorithm>
#include "lib3D\base3d.h"
#include "Nurbs3D.h"
#include "GeneralBilineartransformation.h"
#include "Mytreeview.h"
// GeneralBilinearTab dialog

IMPLEMENT_DYNAMIC(GeneralBilinearTab, CPropertyPage)

GeneralBilinearTab::GeneralBilinearTab()
	: CPropertyPage(GeneralBilinearTab::IDD)
{
	/*m_e00 = 50;
	m_e01 = 50;
	m_e10 = 50;
	m_e11 = 50;
	m_f00 = 50;
	m_f01 = 50;
	m_f10 = 50;
	m_f11 = 50;
	m_g00 = 50;
	m_g01 = 50;
	m_g10 = 50;
	m_g11 = 50;
	m_h00 = 50;
	m_h01 = 50;
	m_h10 = 50;
	m_h11 = 50;*/
	/*m_e00 = 33;
	m_e01 = 50;
	m_e10 = 54;
	m_e11 = 39;
	m_f00 = 66;
	m_f01 = 36;
	m_f10 = 50;
	m_f11 = 50;
	m_g00 = 44;
	m_g01 = 54;
	m_g10 = 43;
	m_g11 = 63;
	m_h00 = 50;
	m_h01 = 50;
	m_h10 = 50;
	m_h11 = 50;*/

	m_e00 = 28;
	m_e01 = 83;
	m_e10 = 62;
	m_e11 = 20;
	m_f00 = 51;
	m_f01 = 36;
	m_f10 = 50;
	m_f11 = 50;
	m_g00 = 68;
	m_g01 = 54;
	m_g10 = 29;
	m_g11 = 85;
	m_h00 = 50;
	m_h01 = 50;
	m_h10 = 50;
	m_h11 = 50;
		
	/*m_e00 = 78;
	m_e01 = 57;
	m_e10 = 39;
	m_e11 = 36;
	m_f00 = 33;
	m_f01 = 31;
	m_f10 = 35;
	m_f11 = 72;
	m_g00 = 50;
	m_g01 = 24;
	m_g10 = 82;
	m_g11 = 50;
	m_h00 = 100;
	m_h01 = 47;
	m_h10 = 92;
	m_h11 = 49;*/
}

GeneralBilinearTab::~GeneralBilinearTab()
{
}

void GeneralBilinearTab::DoDataExchange(CDataExchange* pDX)
{
	DDX_Slider(pDX, IDC_e00, m_e00);
	DDX_Slider(pDX, IDC_e01, m_e01);
	DDX_Slider(pDX, IDC_e10, m_e10);
	DDX_Slider(pDX, IDC_e11, m_e11);
	DDX_Slider(pDX, IDC_f00, m_f00);
	DDX_Slider(pDX, IDC_f01, m_f01);
	DDX_Slider(pDX, IDC_f10, m_f10);
	DDX_Slider(pDX, IDC_f11, m_f11);
	DDX_Slider(pDX, IDC_g00, m_g00);
	DDX_Slider(pDX, IDC_g01, m_g01);
	DDX_Slider(pDX, IDC_g10, m_g10);
	DDX_Slider(pDX, IDC_g11, m_g11);
	DDX_Slider(pDX, IDC_h00, m_h00);
	DDX_Slider(pDX, IDC_h01, m_h01);
	DDX_Slider(pDX, IDC_h10, m_h10);
	DDX_Slider(pDX, IDC_h11, m_h11);
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GeneralBilinearTab, CPropertyPage)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void GeneralBilinearTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData(TRUE);

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_SceneGraph.NbObject() == 0)
		return;

	CTreeCtrl* pTreeControl = (CTreeCtrl*)p_FormView3->objTab.GetDlgItem(IDC_Objects);

	vector<HTREEITEM>::iterator treeitem;
	treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),pTreeControl->GetSelectedItem());

	int index = treeitem - p_FormView3->m_ArraySurfaceItems.begin();
	if(index < 0 || index >= pDoc->m_SceneGraph.NbObject())
		index = 0;

	if(pDoc->m_SceneGraph.GetAt(index)->GetType() != TYPE_NURBSSURFACE)
		return;

	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(index);
	pSurf->m_Modified = TRUE;
	GeneralBilinearTransformation* generalbilineartrans;
	if(!pSurf->ptransformation)
	{
		generalbilineartrans = new GeneralBilinearTransformation();
		pSurf->ptransformation = generalbilineartrans;
	}else 
	{
		if(pSurf->ptransformation->m_ReparameterizationType != generalbilinear)
		{
			delete pSurf->ptransformation;
			generalbilineartrans = new GeneralBilinearTransformation();
			pSurf->ptransformation = generalbilineartrans;

		}else
		{
			generalbilineartrans = (GeneralBilinearTransformation*)pSurf->ptransformation;
		}
	}

	generalbilineartrans->m_ReparameterizationType = generalbilinear;
	generalbilineartrans->umin_Reparameterization = 0;
	generalbilineartrans->umax_Reparameterization = 1;
	generalbilineartrans->vmin_Reparameterization = 0;
	generalbilineartrans->vmax_Reparameterization = 1;

	generalbilineartrans->e00 = m_e00 / 100.0 - 0.5;
	generalbilineartrans->e01 = m_e01 / 100.0 - 0.5;
	generalbilineartrans->e10 = m_e10 / 100.0 + 0.5;
	generalbilineartrans->e11 = m_e11 / 100.0 + 0.5;
	generalbilineartrans->f00 = m_f00 / 100.0 + 0.5;
	generalbilineartrans->f01 = m_f01 / 100.0 + 0.5;
	generalbilineartrans->f10 = m_f10 / 100.0 + 0.5;
	generalbilineartrans->f11 = m_f11 / 100.0 + 0.5;
	generalbilineartrans->g00 = m_g00 / 100.0 - 0.5;
	generalbilineartrans->g01 = m_g01 / 100.0 + 0.5;
	generalbilineartrans->g10 = m_g10 / 100.0 - 0.5;
	generalbilineartrans->g11 = m_g11 / 100.0 + 0.5;
	generalbilineartrans->h00 = m_h00 / 100.0 + 0.5;
	generalbilineartrans->h01 = m_h01 / 100.0 + 0.5;
	generalbilineartrans->h10 = m_h10 / 100.0 + 0.5;
	generalbilineartrans->h11 = m_h11 / 100.0 + 0.5;

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
// GeneralBilinearTab message handlers
