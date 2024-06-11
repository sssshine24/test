// ompositeMobiusTab.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "CompositeMobiusTab.h"
#include "FormView3.h"
#include <vector>
#include <algorithm>
#include "lib3D\base3d.h"
#include "Nurbs3D.h"
#include "mobiustransformation.h"
#include "Mytreeview.h"
#include "CompositeMobiusTransformation.h"

// CompositeMobiusTab dialog

IMPLEMENT_DYNAMIC(CompositeMobiusTab, CPropertyPage)

CompositeMobiusTab::CompositeMobiusTab()
	: CPropertyPage(CompositeMobiusTab::IDD)
{
	ptransformation = NULL;
}

CompositeMobiusTab::~CompositeMobiusTab()
{
}

void CompositeMobiusTab::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_COMPOSITE_UKNOT, m_Slide_Uknot);
	DDX_Control(pDX, IDC_SLIDER_COMPOSITE_VKNOT, m_Slide_Vknot);
	DDX_Control(pDX, IDC_SLIDER_COMPOSITE_SKNOT, m_Slide_Sknot);
	DDX_Control(pDX, IDC_SLIDER_COMPOSITE_TKNOT, m_Slide_Tknot);
	DDX_Control(pDX, IDC_SLIDER_COMPOSITE_ALPHA, m_Slide_Alpha);
	DDX_Control(pDX, IDC_SLIDER_COMPOSITE_BETA, m_Slide_Beta);
	DDX_Control(pDX, IDC_EDIT_COMPOSITE_UKNOT, m_Text_Uknot);
	DDX_Control(pDX, IDC_EDIT_COMPOSITE_VKNOT, m_Text_Vknot);
	DDX_Control(pDX, IDC_EDIT_COMPOSITE_SKNOT, m_Text_Sknot);
	DDX_Control(pDX, IDC_EDIT_COMPOSITE_TKNOT, m_Text_Tknot);
	DDX_Control(pDX, IDC_EDIT_COMPOSITE_ALPHA, m_Text_Alpha);
	DDX_Control(pDX, IDC_EDIT_COMPOSITE_BETA, m_Text_Beta);
	DDX_Control(pDX, IDC_LABEL_COMPOSITE_UKNOT, m_Static_Uknot);
	DDX_Control(pDX, IDC_LABEL_COMPOSITE_VKNOT, m_Static_Vknot);
	DDX_Control(pDX, IDC_LABEL_COMPOSITE_SKNOT, m_Static_Sknot);
	DDX_Control(pDX, IDC_LABEL_COMPOSITE_TKNOT, m_Static_Tknot);
	DDX_Control(pDX, IDC_LABEL_COMPOSITE_ALPHA, m_Static_Alpha);
	DDX_Control(pDX, IDC_LABEL_COMPOSITE_BETA, m_Static_Beta);
	DDX_Control(pDX, IDC_SPIN__COMPOSITE_UKNOT, m_Spin_Uknot);
	DDX_Control(pDX, IDC_SPIN__COMPOSITE_VKNOT, m_Spin_Vknot);
	DDX_Control(pDX, IDC_SPIN__COMPOSITE_SKNOT, m_Spin_Sknot);
	DDX_Control(pDX, IDC_SPIN__COMPOSITE_TKNOT, m_Spin_Tknot);
	DDX_Control(pDX, IDC_SPIN__COMPOSITE_ALPHA, m_Spin_Alpha);
	DDX_Control(pDX, IDC_SPIN__COMPOSITE_BETA, m_Spin_Beta);
}


BEGIN_MESSAGE_MAP(CompositeMobiusTab, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_COMPOSITE_MOBIUS_UPDATE, &CompositeMobiusTab::OnBnClickedButtonCompositeMobiusUpdate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN__COMPOSITE_UKNOT, &CompositeMobiusTab::OnDeltaposSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN__COMPOSITE_VKNOT, &CompositeMobiusTab::OnDeltaposSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN__COMPOSITE_SKNOT, &CompositeMobiusTab::OnDeltaposSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN__COMPOSITE_TKNOT, &CompositeMobiusTab::OnDeltaposSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN__COMPOSITE_ALPHA, &CompositeMobiusTab::OnDeltaposSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN__COMPOSITE_BETA, &CompositeMobiusTab::OnDeltaposSpin)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SLIDER_COMPOSITE_UKNOT, &CompositeMobiusTab::OnNMThemeChangedSliderCompositeUknot)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_COMPOSITE_UKNOT, &CompositeMobiusTab::OnNMReleasedcaptureSliderCompositeUknot)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_GENERATE_COMPOSITESURFACE, &CompositeMobiusTab::OnBnClickedButtonGenerateCompositesurface)
END_MESSAGE_MAP()


void CompositeMobiusTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//p_FormView3->UpdateSliderPosition();
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	
	UpdateSliderPosition(pScrollBar->GetSafeHwnd());

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
// CompositeMobiusTab message handlers

void CompositeMobiusTab::OnBnClickedButtonCompositeMobiusUpdate()
{
	// TODO: Add your control notification handler code here
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
	ptransformation = pSurf->ptransformation;

	if(ptransformation->m_ReparameterizationType != compositemobius)
		return;

	CompositeMobiusTransformation *pCompositeMobiusTrans = (CompositeMobiusTransformation*)ptransformation;

	//Update the limits of the edits.
	m_Spin_Uknot.SetRange(0,pCompositeMobiusTrans->uknot.size() - 1);
	m_Spin_Vknot.SetRange(0,pCompositeMobiusTrans->vknot.size() - 1);
	m_Spin_Sknot.SetRange(0,pCompositeMobiusTrans->sknot.size() - 1);
	m_Spin_Tknot.SetRange(0,pCompositeMobiusTrans->tknot.size() - 1);
	m_Spin_Alpha.SetRange(0,pCompositeMobiusTrans->alpha.size() - 1);
	m_Spin_Beta.SetRange(0,pCompositeMobiusTrans->beta.size() - 1);

	//update the range of slider.
	m_Slide_Uknot.SetRange(1,99);
	m_Slide_Vknot.SetRange(1,99);
	m_Slide_Sknot.SetRange(1,99);
	m_Slide_Tknot.SetRange(1,99);
	m_Slide_Alpha.SetRange(1,99);
	m_Slide_Beta.SetRange(1,99);

	//set the value of slider
	m_Slide_Uknot.SetPos(pCompositeMobiusTrans->uknot[0] * 100);
	m_Slide_Vknot.SetPos(pCompositeMobiusTrans->vknot[0] * 100);
	m_Slide_Sknot.SetPos(pCompositeMobiusTrans->sknot[0] * 100);
	m_Slide_Tknot.SetPos(pCompositeMobiusTrans->tknot[0] * 100);
	m_Slide_Alpha.SetPos(pCompositeMobiusTrans->alpha[0] * 100);
	m_Slide_Beta.SetPos(pCompositeMobiusTrans->beta[0] * 100);

	//set value of text
	char message[100];
	sprintf(message,"%5.2f",pCompositeMobiusTrans->uknot[0]);
	m_Text_Uknot.SetWindowText(message);
	sprintf(message,"%5.2f",pCompositeMobiusTrans->vknot[0]);
	m_Text_Vknot.SetWindowText(message);
	sprintf(message,"%5.2f",pCompositeMobiusTrans->sknot[0]);
	m_Text_Sknot.SetWindowText(message);
	sprintf(message,"%5.2f",pCompositeMobiusTrans->tknot[0]);
	m_Text_Tknot.SetWindowText(message);
	sprintf(message,"%5.2f",pCompositeMobiusTrans->alpha[0]);
	m_Text_Alpha.SetWindowText(message);
	sprintf(message,"%5.2f",pCompositeMobiusTrans->beta[0]);
	m_Text_Beta.SetWindowText(message);
}

void CompositeMobiusTab::UpdateSliderPosition(HWND hwndFrom)
{
	CompositeMobiusTransformation *pCompositeMobiusTrans = (CompositeMobiusTransformation*)ptransformation;

	double tolerance = 1e-5;
	char message[100];
	if(hwndFrom == m_Slide_Uknot.m_hWnd)
	{
		int pos = m_Slide_Uknot.GetPos();
		double uknot = pos / 100.0;
		double preuknot = 0,nextuknot = 1;
		if(m_Spin_Uknot.GetPos() > 0)
			preuknot = pCompositeMobiusTrans->uknot[m_Spin_Uknot.GetPos() - 1];
		if(m_Spin_Uknot.GetPos() < pCompositeMobiusTrans->uknot.size() - 1)
			nextuknot = pCompositeMobiusTrans->uknot[m_Spin_Uknot.GetPos() + 1];

		if(uknot > nextuknot)
			uknot = nextuknot - tolerance;
		if(uknot < preuknot)
			uknot = preuknot + tolerance;

		m_Slide_Uknot.SetPos(uknot * 100.0);
		pCompositeMobiusTrans->uknot[m_Spin_Uknot.GetPos()] = uknot;
		sprintf(message,"%f",uknot);
		m_Text_Uknot.SetWindowText(message);
	}else
	if(hwndFrom == m_Slide_Vknot.m_hWnd)
	{
		int pos = m_Slide_Vknot.GetPos();
		double vknot = pos / 100.0;
		double prevknot = 0,nextvknot = 1;
		if(m_Spin_Vknot.GetPos() > 0)
			prevknot = pCompositeMobiusTrans->vknot[m_Spin_Vknot.GetPos() - 1];
		if(m_Spin_Vknot.GetPos() < pCompositeMobiusTrans->vknot.size() - 1)
			nextvknot = pCompositeMobiusTrans->vknot[m_Spin_Vknot.GetPos() + 1];

		if(vknot > nextvknot)
			vknot = nextvknot - tolerance;
		if(vknot < prevknot)
			vknot = prevknot + tolerance;

		m_Slide_Vknot.SetPos(vknot * 100.0);
		pCompositeMobiusTrans->vknot[m_Spin_Vknot.GetPos()] = vknot;
		sprintf(message,"%f",vknot);
		m_Text_Vknot.SetWindowText(message);
	}else
	if(hwndFrom == m_Slide_Sknot.m_hWnd)
	{
		int pos = m_Slide_Sknot.GetPos();
		double sknot = pos / 100.0;
		double presknot = 0,nextsknot = 1;
		if(m_Spin_Sknot.GetPos() > 0)
			presknot = pCompositeMobiusTrans->sknot[m_Spin_Sknot.GetPos() - 1];
		if(m_Spin_Sknot.GetPos() < pCompositeMobiusTrans->sknot.size() - 1)
			nextsknot = pCompositeMobiusTrans->sknot[m_Spin_Sknot.GetPos() + 1];

		if(sknot > nextsknot)
			sknot = nextsknot - tolerance;
		if(sknot < presknot)
			sknot = presknot + tolerance;

		m_Slide_Sknot.SetPos(sknot * 100.0);
		pCompositeMobiusTrans->sknot[m_Spin_Sknot.GetPos()] = sknot;
		sprintf(message,"%f",sknot);
		m_Text_Sknot.SetWindowText(message);
	}else
	if(hwndFrom == m_Slide_Tknot.m_hWnd)
	{
		int pos = m_Slide_Tknot.GetPos();
		double tknot = pos / 100.0;
		double pretknot = 0,nexttknot = 1;
		if(m_Spin_Tknot.GetPos() > 0)
			pretknot = pCompositeMobiusTrans->tknot[m_Spin_Tknot.GetPos() - 1];
		if(m_Spin_Tknot.GetPos() < pCompositeMobiusTrans->tknot.size() - 1)
			nexttknot = pCompositeMobiusTrans->tknot[m_Spin_Tknot.GetPos() + 1];

		if(tknot > nexttknot)
			tknot = nexttknot - tolerance;
		if(tknot < pretknot)
			tknot = pretknot + tolerance;

		m_Slide_Tknot.SetPos(tknot * 100.0);
		pCompositeMobiusTrans->tknot[m_Spin_Tknot.GetPos()] = tknot;
		sprintf(message,"%f",tknot);
		m_Text_Tknot.SetWindowText(message);
	}else
	if(hwndFrom == m_Slide_Alpha.m_hWnd)
	{
		int pos = m_Slide_Alpha.GetPos();
		double alpha = pos / 100.0;

		m_Slide_Alpha.SetPos(alpha * 100.0);
		pCompositeMobiusTrans->alpha[m_Spin_Alpha.GetPos()] = alpha;
		sprintf(message,"%f",alpha);
		m_Text_Alpha.SetWindowText(message);
	}else
	if(hwndFrom == m_Slide_Beta.m_hWnd)
	{
		int pos = m_Slide_Beta.GetPos();
		double beta = pos / 100.0;

		m_Slide_Beta.SetPos(beta * 100.0);
		pCompositeMobiusTrans->beta[m_Spin_Beta.GetPos()] = beta;
		sprintf(message,"%f",beta);
		m_Text_Beta.SetWindowText(message);
	}
}
void CompositeMobiusTab::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	//spin change
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CompositeMobiusTransformation *pCompositeMobiusTrans = (CompositeMobiusTransformation*)ptransformation;
	char message[100];
	if(pNMUpDown->iPos + pNMUpDown->iDelta < 0)
		return;

	if(pNMHDR->hwndFrom == m_Spin_Uknot.m_hWnd)
	{
		if(pNMUpDown->iPos + pNMUpDown->iDelta >=  pCompositeMobiusTrans->uknot.size())
			return;
		double uknot = pCompositeMobiusTrans->uknot[pNMUpDown->iPos + pNMUpDown->iDelta];
		m_Slide_Uknot.SetPos(uknot * 100);
		sprintf(message,"%f",uknot);
		m_Text_Uknot.SetWindowText(message);
	}else
		if(pNMHDR->hwndFrom == m_Spin_Vknot.m_hWnd)
		{
			if(pNMUpDown->iPos + pNMUpDown->iDelta >=  pCompositeMobiusTrans->vknot.size())
				return;
			double vknot = pCompositeMobiusTrans->vknot[pNMUpDown->iPos + pNMUpDown->iDelta];
			m_Slide_Vknot.SetPos(vknot * 100);
			sprintf(message,"%f",vknot);
			m_Text_Vknot.SetWindowText(message);
		}else
			if(pNMHDR->hwndFrom == m_Spin_Sknot.m_hWnd)
			{
				if(pNMUpDown->iPos + pNMUpDown->iDelta >=  pCompositeMobiusTrans->sknot.size())
					return;
				double sknot = pCompositeMobiusTrans->sknot[pNMUpDown->iPos + pNMUpDown->iDelta];
				m_Slide_Sknot.SetPos(sknot * 100);
				sprintf(message,"%f",sknot);
				m_Text_Sknot.SetWindowText(message);
			}else
				if(pNMHDR->hwndFrom == m_Spin_Tknot.m_hWnd)
				{
					if(pNMUpDown->iPos + pNMUpDown->iDelta >=  pCompositeMobiusTrans->tknot.size())
						return;
					double tknot = pCompositeMobiusTrans->tknot[pNMUpDown->iPos + pNMUpDown->iDelta];
					m_Slide_Tknot.SetPos(tknot * 100);
					sprintf(message,"%f",tknot);
					m_Text_Tknot.SetWindowText(message);
				}else
					if(pNMHDR->hwndFrom == m_Spin_Alpha.m_hWnd)
					{
						if(pNMUpDown->iPos + pNMUpDown->iDelta >=  pCompositeMobiusTrans->alpha.size())
							return;
						double alpha = pCompositeMobiusTrans->alpha[pNMUpDown->iPos + pNMUpDown->iDelta];
						m_Slide_Alpha.SetPos(alpha * 100);
						sprintf(message,"%f",alpha);
						m_Text_Alpha.SetWindowText(message);
					}else
						if(pNMHDR->hwndFrom == m_Spin_Beta.m_hWnd)
						{
							if(pNMUpDown->iPos + pNMUpDown->iDelta >=  pCompositeMobiusTrans->beta.size())
								return;
							double beta = pCompositeMobiusTrans->beta[pNMUpDown->iPos + pNMUpDown->iDelta];
							m_Slide_Beta.SetPos(beta * 100);
							sprintf(message,"%f",beta);
							m_Text_Beta.SetWindowText(message);
						}
	*pResult = 0;
}

void CompositeMobiusTab::OnNMThemeChangedSliderCompositeUknot(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CompositeMobiusTab::OnNMReleasedcaptureSliderCompositeUknot(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CompositeMobiusTransformation *pCompositeMobiusTrans = (CompositeMobiusTransformation*)ptransformation;
	if(pNMUpDown->iPos + pNMUpDown->iDelta < 0)
		return;
	double tolerance = 1e-5;
	char message[100];
	if(pNMHDR->hwndFrom == m_Slide_Uknot.m_hWnd)
	{
		int pos = m_Slide_Uknot.GetPos();
		double uknot = pos / 100.0;
		double preuknot = 0,nextuknot = 1;
		if(m_Spin_Uknot.GetPos() > 0)
			preuknot = pCompositeMobiusTrans->uknot[m_Spin_Uknot.GetPos() - 1];
		if(m_Spin_Uknot.GetPos() < pCompositeMobiusTrans->uknot.size() - 1)
			nextuknot = pCompositeMobiusTrans->uknot[m_Spin_Uknot.GetPos() + 1];

		if(uknot > nextuknot)
			uknot = nextuknot - tolerance;
		if(uknot < preuknot)
			uknot = preuknot + tolerance;

		m_Slide_Uknot.SetPos(uknot * 100.0);
		pCompositeMobiusTrans->uknot[m_Spin_Uknot.GetPos()] = uknot;
		sprintf(message,"%f",uknot);
		m_Text_Uknot.SetWindowText(message);
	}else
	if(pNMHDR->hwndFrom == m_Spin_Vknot.m_hWnd)
	{
		double vknot = pCompositeMobiusTrans->vknot[pNMUpDown->iPos + pNMUpDown->iDelta];
		m_Slide_Vknot.SetPos(vknot * 100);
		sprintf(message,"%f",vknot);
		m_Text_Vknot.SetWindowText(message);
	}else
	if(pNMHDR->hwndFrom == m_Spin_Sknot.m_hWnd)
	{
		double sknot = pCompositeMobiusTrans->sknot[pNMUpDown->iPos + pNMUpDown->iDelta];
		m_Slide_Sknot.SetPos(sknot * 100);
		sprintf(message,"%f",sknot);
		m_Text_Sknot.SetWindowText(message);
	}else
	if(pNMHDR->hwndFrom == m_Spin_Tknot.m_hWnd)
	{
		double tknot = pCompositeMobiusTrans->tknot[pNMUpDown->iPos + pNMUpDown->iDelta];
		m_Slide_Tknot.SetPos(tknot * 100);
		sprintf(message,"%f",tknot);
		m_Text_Tknot.SetWindowText(message);
	}else
	if(pNMHDR->hwndFrom == m_Spin_Alpha.m_hWnd)
	{
		double alpha = pCompositeMobiusTrans->alpha[pNMUpDown->iPos + pNMUpDown->iDelta];
		m_Slide_Alpha.SetPos(alpha * 100);
		sprintf(message,"%f",alpha);
		m_Text_Alpha.SetWindowText(message);
	}else
	if(pNMHDR->hwndFrom == m_Spin_Beta.m_hWnd)
	{
		double beta = pCompositeMobiusTrans->beta[pNMUpDown->iPos + pNMUpDown->iDelta];
		m_Slide_Beta.SetPos(beta * 100);
		sprintf(message,"%f",beta);
		m_Text_Beta.SetWindowText(message);
	}
}

void CompositeMobiusTab::OnBnClickedButtonGenerateCompositesurface()
{
	// TODO: Add your control notification handler code here
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
	C_NurbsSurf* pSurface = pSurf->Converte_C_NurbsSurface();
	C_NurbsSurf* pNewSurface = pSurface->GetCompositeMobiusTransformedSurface((CompositeMobiusTransformation*)ptransformation);
	CNurbsSuface* pNewCNSurface = pNewSurface->GetCNurbsSurface();
	pNewCNSurface->SetTransform(*pSurf->GetTransform());
	pNewCNSurface->filename = "CompositeOptimizedSurface";
	pDoc->m_SceneGraph.Add(pNewCNSurface);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
