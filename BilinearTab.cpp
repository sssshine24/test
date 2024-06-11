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

	UpdateData(TRUE);

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_SceneGraph.NbObject() == 0)
		return;

	CTreeCtrl* pTreeControl = (CTreeCtrl*)p_FormView3->objTab.GetDlgItem(IDC_Objects);

	std::vector<HTREEITEM>::iterator treeitem;
	treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),pTreeControl->GetSelectedItem());

	int index = treeitem - p_FormView3->m_ArraySurfaceItems.begin();
	if(index < 0 || index >= pDoc->m_SceneGraph.NbObject())
		index = 0;

	if(pDoc->m_SceneGraph.GetAt(index)->GetType() != TYPE_NURBSSURFACE)
		return;

	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(index);
	pSurf->m_Modified = TRUE;
	BilinearTransformation* bilineartrans;
	if(!pSurf->ptransformation)
	{
		bilineartrans = new BilinearTransformation();
		pSurf->ptransformation = bilineartrans;
	}else 
	{
		if(pSurf->ptransformation->m_ReparameterizationType != bilinear)
		{
			delete pSurf->ptransformation;
			bilineartrans = new BilinearTransformation();
			pSurf->ptransformation = bilineartrans;
		}else
		{
			bilineartrans = (BilinearTransformation*)pSurf->ptransformation;
		}
	}

	bilineartrans->m_ReparameterizationType = bilinear;
	bilineartrans->umin_Reparameterization = 0;
	bilineartrans->umax_Reparameterization = 1;
	bilineartrans->vmin_Reparameterization = 0;
	bilineartrans->vmax_Reparameterization = 1;
	bilineartrans->m_Bilinear_Alpha1 = m_Bilinear_Alpha1 / 100.0;
	bilineartrans->m_Bilinear_Alpha2 = m_Bilinear_Alpha2 / 100.0;
	bilineartrans->m_Bilinear_Beta1 = m_Bilinear_Beta1 / 100.0;
	bilineartrans->m_Bilinear_Beta2 = m_Bilinear_Beta2 / 100.0;

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
// BilinearTab message handlers
