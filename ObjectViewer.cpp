// ObjectViewer.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "ObjectViewer.h"
#include "FormView3.h"
#include <algorithm>
#include "lib3d/Base3d.h"
#include "Nurbs3D.h"
#include "MyTreeView.h"
#include "Disline2D.h"
#include "NurbsCurve.h"
#include "lib3d/Mesh3d_OpenMesh.h"
#include "Tspline.h"
#include "MyTreeview.h"
#include "BsplineTransformation.h"
#include "Shape_Polygon.h"
// ObjectViewer dialog

IMPLEMENT_DYNAMIC(ObjectViewer, CPropertyPage)

ObjectViewer::ObjectViewer()
	: CPropertyPage(ObjectViewer::IDD)
{

}

ObjectViewer::~ObjectViewer()
{
}

void ObjectViewer::DoDataExchange(CDataExchange* pDX)
{
	DDX_Slider(pDX, IDC_SLIDER1, m_slider1);
	DDX_Slider(pDX, IDC_SLIDER2, m_slider2);
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDERMOVESPEED, m_MoveSpeedSlider);
	DDX_Control(pDX, IDC_SLIDERFOV, m_FovySlider);
}


BEGIN_MESSAGE_MAP(ObjectViewer, CPropertyPage)
	ON_BN_CLICKED(ID_IsoParametric_Line, &ObjectViewer::OnBnClickedIsoparametricLine)
	ON_NOTIFY(NM_RCLICK, IDC_Objects, &ObjectViewer::OnNMRClickObjects)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_CLICK, IDC_Objects, &ObjectViewer::OnNMClickObjects)
	ON_BN_CLICKED(IDC_Texture, &ObjectViewer::OnBnClickedTexture)
	ON_BN_CLICKED(IDC_Texture, OnBnClickedTexture)
	ON_BN_CLICKED(IDC_ShowTexture, OnBnClickedCheck1)
	ON_BN_CLICKED(ID_ZOOMALL, &ObjectViewer::OnBnClickedZoomall)
	ON_BN_CLICKED(ID_UPDATE_TREECONTROL, &ObjectViewer::OnBnClickedUpdateTreecontrol)
	ON_BN_CLICKED(ID_Extract_FreeformTransformation, &ObjectViewer::OnBnClickedExtractFreeformtransformation)
	ON_BN_CLICKED(ID_OPENMESH_CLEAR_VERTEX_SELECTION, &ObjectViewer::OnBnClickedOpenmeshClearVertexSelection)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERFOV, &ObjectViewer::OnNMCustomdrawSliderfov)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERMOVESPEED, &ObjectViewer::OnNMCustomdrawSlidermovespeed)
END_MESSAGE_MAP()


// ObjectViewer message handlers
extern CFormView3 *p_FormView3;
void ObjectViewer::OnBnClickedIsoparametricLine()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_IsoParameterCurve=!pDoc->m_IsoParameterCurve;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}

void ObjectViewer::OnNMRClickObjects(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	CTreeCtrl* pTreeControl = (CTreeCtrl*)GetDlgItem(IDC_Objects);

	CPoint screenPoint,clientpoint;
	UINT uFlag;
	GetCursorPos(&screenPoint);
	clientpoint = screenPoint;
	pTreeControl->ScreenToClient(&clientpoint);
	std::vector<HTREEITEM>::iterator treeitem;
	HTREEITEM hItem = pTreeControl->HitTest(clientpoint,&uFlag);
	if((hItem!= NULL) && ((TVHT_ONITEM & uFlag) && (TVHT_ONITEMLABEL & uFlag)))
	{
		treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),hItem);
		p_FormView3->currentindex = treeitem - p_FormView3->m_ArraySurfaceItems.begin();

		if(p_FormView3->currentindex >= pDoc->m_SceneGraph.NbObject() || p_FormView3->currentindex < 0)
		{
			p_FormView3->currentindex = -1;

		}
		else
		{
			if(pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_NURBSSURFACE)
			{
				CMenu menu;
				VERIFY( menu.LoadMenu( IDR_MENU1 ) );
				CMenu* popup = menu.GetSubMenu(0);
				ASSERT( popup != NULL );
				popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd() );
				popup->Detach();
			}else
				if(pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_MESH3D ||
					pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
				{
					CMenu menu;
					VERIFY( menu.LoadMenu( IDR_MENU3 ) );
					CMenu* popup = menu.GetSubMenu(0);
					ASSERT( popup != NULL );
					popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd() );
					popup->Detach();
				}
				else
			if(pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_TSPLINE)
			{
				CMenu menu;
				VERIFY( menu.LoadMenu( IDR_MENU4 ) );
				CMenu* popup = menu.GetSubMenu(0);
				ASSERT( popup != NULL );
				popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd() );
				popup->Detach();
			}else
			if(pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_NURBSCURVE3D){
				CMenu menu;
				VERIFY( menu.LoadMenu( IDR_MENU5 ) );
				CMenu* popup = menu.GetSubMenu(0);
				ASSERT( popup != NULL );
				popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd() );
				popup->Detach();	
			}else
				if (pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_POLYGON)
				{
					CMenu menu;
					VERIFY(menu.LoadMenu(IDR_MENU6));
					CMenu* popup = menu.GetSubMenu(0);
					ASSERT(popup != NULL);
					popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
					popup->Detach();
				}
		}

		//popup->GetSubMenu(1)->SetCheck

	}
}

BOOL ObjectViewer::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	m_MoveSpeedSlider.SetRange(0,100);
	m_MoveSpeedSlider.SetPos(50);
	m_FovySlider.SetRange(0,1000);
	m_FovySlider.SetPos(250);

	m_slider1 = pDoc->m_Disc1;
	m_slider2 = pDoc->m_Disc2;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
#define ZNEAR	0.1
#define	ZFAR	2000.0
extern CMyTreeView* pTreeView;
void ObjectViewer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar); 
	if(pSlider == &m_MoveSpeedSlider)
	{
		double scale = m_MoveSpeedSlider.GetPos() - 50;
		scale = (scale) * scale;
		if(m_MoveSpeedSlider.GetPos() > 50)
		{
			pTreeView->m_SpeedRotation = 1.0f * scale / (100.0 );
			pTreeView->m_SpeedTranslation = 1.0f * scale/ (100.0 );
		}else
		if(m_MoveSpeedSlider.GetPos() < 50)
		{
			pTreeView->m_SpeedRotation = 1.0f  / (100.0 * scale);
			pTreeView->m_SpeedTranslation = 1.0f / (100.0 * scale);
		}else
		{
			pTreeView->m_SpeedRotation = 1.0f  / (100.0);
			pTreeView->m_SpeedTranslation = 1.0f / (100.0);
		}

	}else if(pSlider == &m_FovySlider)
	{
		double scale = m_FovySlider.GetPos();
		pTreeView->FOVY = scale / 10;

		pTreeView->ChangeFov();
		//pTreeView->OnSize(0,rect.Width(),rect.Height());
		//gluPerspective(scale,aspect,ZNEAR,ZFAR);
		pTreeView->Invalidate(TRUE);
	}
	else
		p_FormView3->UpdateSliderPosition();

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
void ObjectViewer::OnNMClickObjects(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	CTreeCtrl* pTreeControl = (CTreeCtrl*)GetDlgItem(IDC_Objects);

	CPoint myPoint;
	UINT uFlag;
	GetCursorPos(&myPoint);
	pTreeControl->ScreenToClient(&myPoint);
	std::vector<HTREEITEM>::iterator treeitem;
	HTREEITEM hItem = pTreeControl->HitTest(myPoint,&uFlag);
	int index;
	if(!hItem)
		hItem = pTreeControl->GetSelectedItem();
	if((hItem!= NULL) && ((TVHT_ONITEM & uFlag) && !(TVHT_ONITEMBUTTON & uFlag) && !(TVHT_ONITEMLABEL & uFlag)))
	{
		treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),hItem);
		index = treeitem - p_FormView3->m_ArraySurfaceItems.begin();

		if(index >= pDoc->m_SceneGraph.NbObject())
			return;

		p_FormView3->currentindex = index;
		if(pDoc->m_SceneGraph.GetAt(index)->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(index);
			pNurbsSurface->m_Show = !pNurbsSurface->m_Show; 
		}else
		if(pDoc->m_SceneGraph.GetAt(index)->GetType() == TYPE_MESH3D)
		{
			CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(index);
			pMesh->m_Show = !pMesh->m_Show; 
		}else
		if(pDoc->m_SceneGraph.GetAt(index)->GetType() == TYPE_DISLINE2D)
		{
			CDisline2D* pDisline = (CDisline2D*)pDoc->m_SceneGraph.GetAt(index);
			pDisline->m_Show = !pDisline->m_Show;
		}else
		if(pDoc->m_SceneGraph.GetAt(index)->GetType() == TYPE_NURBSCURVE3D)
		{
			CNurbsCurve* pCurve = (CNurbsCurve*)pDoc->m_SceneGraph.GetAt(index);
			pCurve->m_Show = !pCurve->m_Show;
		}else
		if(pDoc->m_SceneGraph.GetAt(index)->GetType() == TYPE_NURBSCURVE3D)
		{
			CMesh3d_OpenMesh* pMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(index);
			pMesh->m_Show = !pMesh->m_Show;
		}else
		if(pDoc->m_SceneGraph.GetAt(index)->GetType() == TYPE_TSPLINE)
		{
			Tspline* pMesh = (Tspline*)pDoc->m_SceneGraph.GetAt(index);
			pMesh->m_Show = !pMesh->m_Show;
		}else
		if(pDoc->m_SceneGraph.GetAt(index)->GetType() == TYPE_MESH3D_OPENMESH)
		{
			CMesh3d_OpenMesh* pMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(index);
			pMesh->m_Show = !pMesh->m_Show;
		}else
		if (pDoc->m_SceneGraph.GetAt(index)->GetType() == TYPE_POLYGON)
		{
			Shape_Polygon* pPolygon = (Shape_Polygon*)pDoc->m_SceneGraph.GetAt(index);
			pPolygon->m_Show = !pPolygon->m_Show;
		}

		POSITION pos = pDoc->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
				pView->Invalidate(TRUE);
		}
	}else
	if(hItem)
	{
		treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),hItem);
		index = treeitem - p_FormView3->m_ArraySurfaceItems.begin();

		if(index >= pDoc->m_SceneGraph.NbObject())
			return;

		p_FormView3->currentindex = index;
	}

	*pResult = 0;
}
extern CString m_FileTextureName;
extern int m_ShowTexture;
void ObjectViewer::OnBnClickedTexture()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CButton* pCheckBox = (CButton*)GetDlgItem(IDC_ShowTexture);
	pDoc->ShowTexture = pCheckBox->GetCheck();
	m_ShowTexture = pDoc->ShowTexture;
	CFileDialog dialog(TRUE);
	if(dialog.DoModal())
	{
		m_FileTextureName=dialog.GetPathName();
		//m_texture1.SetWindowText(m_FileTextureName);
	}
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}

extern int m_ShowCoordinateAxis;
void ObjectViewer::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CButton* pCheckBox = (CButton*)GetDlgItem(IDC_ShowTexture);
	pDoc->ShowTexture = pCheckBox->GetCheck();
	m_ShowTexture = pDoc->ShowTexture;

	pCheckBox = (CButton*)GetDlgItem(IDC_ShowCoordinateAxis);
	pDoc->ShowCoordinateAxis = pCheckBox->GetCheck();
	m_ShowCoordinateAxis = pDoc->ShowCoordinateAxis;

	//modify the texture flag for all the objects
	for(int i = 0; i < pDoc->m_SceneGraph.NbObject(); i++)
	{
		pDoc->m_SceneGraph[i]->m_ListDone = 0;
	}

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
void ObjectViewer::OnBnClickedZoomall()
{
	// TODO: Add your control notification handler code here
	pTreeView->OnZoomall();
}

void ObjectViewer::OnBnClickedUpdateTreecontrol()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void ObjectViewer::OnBnClickedExtractFreeformtransformation()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if(p_FormView3->currentindex < 0 || p_FormView3->currentindex >= num)
		return;

	if(pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
		if(pNSurf->ptransformation)
		{
			if(pNSurf->ptransformation->m_ReparameterizationType == Bspline)
			{
				BsplineTransformation* pTrans = (BsplineTransformation*)pNSurf->ptransformation;
				C_NurbsSurf* pSurface = pTrans->pNurbsSurf;
				CNurbsSuface* pSurface1 = pSurface->GetCNurbsSurface();
				pSurface1->SetTransform(*pNSurf->GetTransform());
				pDoc->m_SceneGraph.Add(pSurface1);
				pDoc->UpdateTreeControl();
				pDoc->UpdateAllViews(NULL);
			}
		}
	}
}
void ObjectViewer::OnBnClickedOpenmeshClearVertexSelection()
{
	// TODO: Add your control notification handler code here
	if(pDoc->m_SceneGraph.NbObject() != 1)
		return;
	if(pDoc->m_SceneGraph.GetAt(0)->GetType() != TYPE_MESH3D_OPENMESH)
		return;
	CMesh3d_OpenMesh* pMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(0);
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;

	//Vertex selected flag;
	if (!pMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	for(MyMesh::VertexIter v_it=pMesh->pmesh->vertices_begin(); v_it!=pMesh->pmesh->vertices_end(); ++v_it)
	{
		pMesh->pmesh->property(vprop_selectedflag,v_it.handle()) = 0;
	}
	pDoc->UpdateAllViews(NULL);
}

void ObjectViewer::OnNMCustomdrawSliderfov(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void ObjectViewer::OnNMCustomdrawSlidermovespeed(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
