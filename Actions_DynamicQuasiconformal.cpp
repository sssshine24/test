// Actions_DynamicQuasiconformal.cpp : implementation file
//

#include "stdafx.h"
#include "Action_PolyOmino.h"
#include "FormView3.h"
#include "SDIViewSwitch.h"
#include "lib3d/Base3d.h"
#include "lib3d/Mesh3d_OpenMesh.h"
#include "Actions_DynamicQuasiconformal.h"
#include "afxdialogex.h"
#include "Actions_HarmonicMapping.h"

// Actions_DynamicQuasiconformal dialog
extern Actions_HarmonicMapping* pHarmonicMapping;
IMPLEMENT_DYNAMIC(Actions_DynamicQuasiconformal, CDialogEx)

Actions_DynamicQuasiconformal* pDynamicQuasiconformal;
extern Actions_DataAnalysis* pDataAnalysis;
Actions_DynamicQuasiconformal::Actions_DynamicQuasiconformal(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DynamicQuasi, pParent)
	, m_DynamicMappingValue(_T(""))
{
	p3DSurface_Mesh3d = NULL;
	pconvex_para_Mesh3d = NULL;
	ptarget_para_Mesh3d = NULL;
	p3DSurface_Openmesh = NULL;
	pconvex_para_Openmesh = NULL;
	ptarget_para_Openmesh = NULL;
	ptarget_para_Openmesh_Intermediate = NULL;
	selectvindex = -1;
	convexmesh_onlyshowboundary = 0;
	tarmesh_onlyshowboundary = 1;
	m_FirstConvexMeshPlot = 1;
	m_FirstTargetMeshPlot = 1;
	m_ShowMeshMapping = 0;
	m_show_target_intermediate = 0;
	pDynamicQuasiconformal = this;
}

Actions_DynamicQuasiconformal::~Actions_DynamicQuasiconformal()
{
}

void Actions_DynamicQuasiconformal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_DYNAMICQUASICONFORMAL_SLIDER, m_DynamicMappingSlider);
	DDX_Control(pDX, IDC_DYNAMICQUASICONFORMAL_SPIN, m_DynamicMappingSpin);
	DDX_Control(pDX, IDC_DYNAMICQUASI_EDIT, m_DynamicMappingEdit);
	DDX_Text(pDX, IDC_DYNAMICQUASI_EDIT, m_DynamicMappingValue);
	DDX_Control(pDX, IDC_DYNAMICQUASI_COMBO, m_ActionsCombo);
	DDX_Control(pDX, IDC_DynamicQuasi_ShowInitialConvexPara, m_ShowIntialConvexPara);
	DDX_Control(pDX, IDC_DynamicQuasi_ShowInitialTargetPara, m_ShowInitialTargetPara);
	DDX_Control(pDX, IDC_DynamicQuasi_ShowIntermediateTargetPara, m_ShowIntermediateTargetPara);
	DDX_Control(pDX, IDC_DynamicQuasi_ShowIndex, m_ShowIndex);
	DDX_Control(pDX, IDC_DynamicQuasi_ShowCircle, m_ShowCircle);
}


BEGIN_MESSAGE_MAP(Actions_DynamicQuasiconformal, CDialogEx)
	ON_BN_CLICKED(IDC_DYNAMIC_QUASI_LOAD3DSURFACE, &Actions_DynamicQuasiconformal::OnBnClickedDynamicQuasiLoad3dsurface)
	ON_BN_CLICKED(IDC_DYNAMIC_COMPUTEHARMONICENERGY, &Actions_DynamicQuasiconformal::OnBnClickedDynamicComputeharmonicenergy)
	ON_BN_CLICKED(IDC_DYNAMIC_CHECKDELAUNAY, &Actions_DynamicQuasiconformal::OnBnClickedDynamicCheckdelaunay)
	ON_BN_CLICKED(IDC_DYNAMIC_QUASI_LOADTWOTRIANGLES, &Actions_DynamicQuasiconformal::OnBnClickedDynamicQuasiLoadtwotriangles)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DYNAMIC_SETTARGETBOUNDARY, &Actions_DynamicQuasiconformal::OnBnClickedDynamicSettargetboundary)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_DYNAMIC_SAVETARGETPARAMESH, &Actions_DynamicQuasiconformal::OnBnClickedDynamicSavetargetparamesh)
	ON_BN_CLICKED(IDC_DYNAMIC_LOADTARGETPARAMESH, &Actions_DynamicQuasiconformal::OnBnClickedDynamicLoadtargetparamesh)
	ON_BN_CLICKED(IDC_DYNAMIC_DYNAMICHARMONIC, &Actions_DynamicQuasiconformal::OnBnClickedDynamicDynamicharmonic)
	ON_BN_CLICKED(IDC_DYNAMIC_DYNAMICHARMONIC_MAPPINGSEQUENCES, &Actions_DynamicQuasiconformal::OnBnClickedDynamicDynamicharmonicMappingsequences)
	ON_WM_HSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_DYNAMICQUASICONFORMAL_SPIN, &Actions_DynamicQuasiconformal::OnDeltaposDynamicquasiconformalSpin)
	ON_BN_CLICKED(IDC_DynamicQuasi_ShowInitialConvexPara, &Actions_DynamicQuasiconformal::OnBnClickedDynamicquasiShowinitialconvexpara)
	ON_BN_CLICKED(IDC_DynamicQuasi_ShowInitialTargetPara, &Actions_DynamicQuasiconformal::OnBnClickedDynamicquasiShowinitialtargetpara)
	ON_BN_CLICKED(IDC_DynamicQuasi_ShowIntermediateTargetPara, &Actions_DynamicQuasiconformal::OnBnClickedDynamicquasiShowintermediatetargetpara)
	ON_BN_CLICKED(IDC_DYNAMIC_OPTIMALHARMONIC, &Actions_DynamicQuasiconformal::OnBnClickedDynamicOptimalharmonic)
	ON_BN_CLICKED(IDD_DIALOG_DynamicQuasi_TargetPara_Static, &Actions_DynamicQuasiconformal::OnBnClickedDialogDynamicquasiTargetparaStatic)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR1, &Actions_DynamicQuasiconformal::OnNMThemeChangedScrollbar1)
	ON_BN_CLICKED(IDC_BUTTON15, &Actions_DynamicQuasiconformal::OnBnClickedButton15)
	ON_EN_CHANGE(IDC_DYNAMICQUASI_EDIT, &Actions_DynamicQuasiconformal::OnEnChangeDynamicquasiEdit)
END_MESSAGE_MAP()


// Actions_DynamicQuasiconformal message handlers

extern CFormView3 *p_FormView3;
extern Action_PolyOmino* pPolyOmino;
extern Actions_HarmonicMapping* pHarmonicMap;
void Actions_DynamicQuasiconformal::OnBnClickedDynamicQuasiLoad3dsurface()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\dynamicquasiconformal\\example1\\3Dface.obj");
	p3DSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p3DSurface_Openmesh = p3DSurface_Mesh3d->Convert_OpenMesh();

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\dynamicquasiconformal\\example1\\3Dface_para.obj");
	pconvex_para_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pconvex_para_Openmesh = pconvex_para_Mesh3d->Convert_OpenMesh();

	//pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);

	//p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	//pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pDoc->UpdateAllViews(NULL);
}
double Actions_DynamicQuasiconformal::GetUVHarmonicEnergy(CMesh3d_OpenMesh* pOpenmesh)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pOpenmesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenmesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	EPROP_EDGE_LAPLACE_ENERGY eprop_laplaceenergy;

	//Edge Laplace weight;
	if (!pOpenmesh->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pOpenmesh->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	//Edge laplace energy;
	if (!pOpenmesh->pmesh->get_property_handle(eprop_laplaceenergy, PROP_EDGE_LAPLACE_ENERGY))
		pOpenmesh->pmesh->add_property(eprop_laplaceenergy, PROP_EDGE_LAPLACE_ENERGY);

	double v1_v2, v1_v3, v2_v3, v1_v4, v2_v4;
	double angle1, angle2;
	double w;
	double totalenergy = 0;
	for (MyMesh::EdgeIter e_it = pOpenmesh->pmesh->edges_begin(); e_it != pOpenmesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		//
		angle1 = angle2 = 0;
		w = 0;

		MyMesh::HalfedgeHandle hh0 = pOpenmesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pOpenmesh->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pOpenmesh->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pOpenmesh->pmesh->from_vertex_handle(hh0);

		int vh1index = vh1.idx();
		int vh2index = vh2.idx();

		MyMesh::VertexHandle vh3, vh4;
		MyMesh::FaceHandle fh1 = pOpenmesh->pmesh->face_handle(hh0);
		MyMesh::FaceHandle fh2 = pOpenmesh->pmesh->face_handle(hh1);

		std::vector<double> uv1 = pOpenmesh->pmesh->property(vprop_vertex_uv, vh1);
		std::vector<double> uv2 = pOpenmesh->pmesh->property(vprop_vertex_uv, vh2);
		OpenMesh::Vec3d v1, v2;
		v1[0] = uv1[0];
		v1[1] = uv1[1];
		v1[2] = 0;

		v2[0] = uv2[0];
		v2[1] = uv2[1];
		v2[2] = 0;

		v1_v2 = Length(v1 - v2);

		if (fh1.is_valid())
		{
			for (MyMesh::FaceVertexIter fv_it = pOpenmesh->pmesh->fv_iter(fh1); fv_it; ++fv_it)
			{
				if (fv_it.handle().idx() != vh1.idx() && fv_it.handle().idx() != vh2.idx())
				{
					vh3 = fv_it.handle();
					break;
				}
			}
			std::vector<double> uv3 = pOpenmesh->pmesh->property(vprop_vertex_uv, vh3);
			//OpenMesh::Vec3d v3 = pOpenmesh->pmesh->point(vh3);
			OpenMesh::Vec3d v3;
			v3[0] = uv3[0];
			v3[1] = uv3[1];
			v3[2] = 0;
			v1_v3 = Length(v1 - v3);
			v2_v3 = Length(v2 - v3);
			angle1 = acos((SQR(v1_v3) + SQR(v2_v3) - SQR(v1_v2)) / (2 * v1_v3 * v2_v3));
			w += 1.0 / tan(angle1);
		}

		if (fh2.is_valid())
		{
			for (MyMesh::FaceVertexIter fv_it = pOpenmesh->pmesh->fv_iter(fh2); fv_it; ++fv_it)
			{
				if (fv_it.handle().idx() != vh1.idx() && fv_it.handle().idx() != vh2.idx())
				{
					vh4 = fv_it.handle();
					break;
				}
			}
			std::vector<double> uv4 = pOpenmesh->pmesh->property(vprop_vertex_uv, vh4);
			//OpenMesh::Vec3d v3 = pOpenmesh->pmesh->point(vh3);
			OpenMesh::Vec3d v4;
			v4[0] = uv4[0];
			v4[1] = uv4[1];
			v4[2] = 0;
			//OpenMesh::Vec3d v4 = pOpenmesh->pmesh->point(vh4);
			v1_v4 = Length(v1 - v4);
			v2_v4 = Length(v2 - v4);
			angle2 = acos((SQR(v1_v4) + SQR(v2_v4) - SQR(v1_v2)) / (2 * v1_v4 * v2_v4));
			w += 1.0 / tan(angle2);
		}

		pOpenmesh->pmesh->property(eprop_laplaceweight, eh) = w;
		pOpenmesh->pmesh->property(eprop_laplaceenergy, eh) = SQR(v1_v2) * w;
		totalenergy += SQR(v1_v2) * w;
	}
	totalenergy = totalenergy / 2;
	return totalenergy;
}
double Actions_DynamicQuasiconformal::GetHarmonicEnergy(CMesh3d_OpenMesh* pOpenmesh)
{
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	EPROP_EDGE_LAPLACE_ENERGY eprop_laplaceenergy;

	//Edge Laplace weight;
	if (!pOpenmesh->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pOpenmesh->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	//Edge laplace energy;
	if (!pOpenmesh->pmesh->get_property_handle(eprop_laplaceenergy, PROP_EDGE_LAPLACE_ENERGY))
		pOpenmesh->pmesh->add_property(eprop_laplaceenergy, PROP_EDGE_LAPLACE_ENERGY);

	double v1_v2, v1_v3, v2_v3, v1_v4, v2_v4;
	double angle1, angle2;
	double w;
	double totalenergy = 0;
	for (MyMesh::EdgeIter e_it = pOpenmesh->pmesh->edges_begin(); e_it != pOpenmesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		//
		angle1 = angle2 = 0;
		w = 0;

		MyMesh::HalfedgeHandle hh0 = pOpenmesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pOpenmesh->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pOpenmesh->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pOpenmesh->pmesh->from_vertex_handle(hh0);

		int vh1index = vh1.idx();
		int vh2index = vh2.idx();

		MyMesh::VertexHandle vh3, vh4;
		MyMesh::FaceHandle fh1 = pOpenmesh->pmesh->face_handle(hh0);
		MyMesh::FaceHandle fh2 = pOpenmesh->pmesh->face_handle(hh1);

		OpenMesh::Vec3d v1 = pOpenmesh->pmesh->point(vh1);
		OpenMesh::Vec3d v2 = pOpenmesh->pmesh->point(vh2);
		v1_v2 = Length(v1 - v2);

		if (fh1.is_valid())
		{
			for (MyMesh::FaceVertexIter fv_it = pOpenmesh->pmesh->fv_iter(fh1); fv_it; ++fv_it)
			{
				if (fv_it.handle().idx() != vh1.idx() && fv_it.handle().idx() != vh2.idx())
				{
					vh3 = fv_it.handle();
					break;
				}
			}
			OpenMesh::Vec3d v3 = pOpenmesh->pmesh->point(vh3);
			v1_v3 = Length(v1 - v3);
			v2_v3 = Length(v2 - v3);
			angle1 = acos((SQR(v1_v3) + SQR(v2_v3) - SQR(v1_v2)) / (2 * v1_v3 * v2_v3));
			w += 1.0 / tan(angle1);
		}


		if (fh2.is_valid())
		{
			for (MyMesh::FaceVertexIter fv_it = pOpenmesh->pmesh->fv_iter(fh2); fv_it; ++fv_it)
			{
				if (fv_it.handle().idx() != vh1.idx() && fv_it.handle().idx() != vh2.idx())
				{
					vh4 = fv_it.handle();
					break;
				}
			}
			OpenMesh::Vec3d v4 = pOpenmesh->pmesh->point(vh4);
			v1_v4 = Length(v1 - v4);
			v2_v4 = Length(v2 - v4);
			angle2 = acos((SQR(v1_v4) + SQR(v2_v4) - SQR(v1_v2)) / (2 * v1_v4 * v2_v4));
			w += 1.0 / tan(angle2);
		}

		pOpenmesh->pmesh->property(eprop_laplaceweight, eh) = w;
		pOpenmesh->pmesh->property(eprop_laplaceenergy, eh) = SQR(v1_v2) * w;
		totalenergy += SQR(v1_v2) * w;
	}
	totalenergy = totalenergy / 2;
	return totalenergy;
}
void Actions_DynamicQuasiconformal::OnBnClickedDynamicComputeharmonicenergy()
{
	// TODO: Add your control notification handler code here

	//if (pDoc->m_SceneGraph.NbObject() != 1)
	//	return;

	CMesh3d_OpenMesh* pOpenmesh;
	if (pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
		pOpenmesh = pMesh3d->Convert_OpenMesh();
	}
	else
		pOpenmesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);

	//Compute the harmonic energy
	double totalenergy = GetHarmonicEnergy(pOpenmesh);

	char Post_Message[1000];
	sprintf(Post_Message,"Total Harmonic Energy: %f", totalenergy);
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	AfxMessageBox(Post_Message);
	Invalidate(TRUE);
	//delete pOpenmesh;
}


void Actions_DynamicQuasiconformal::OnBnClickedDynamicCheckdelaunay()
{
	// TODO: Add your control notification handler code here

	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	EPROP_EDGE_LAPLACE_ENERGY eprop_laplaceenergy;

	if (pDoc->m_SceneGraph.NbObject() != 1)
		return;

	CMesh3d_OpenMesh* pOpenmesh;
	if (pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
		pOpenmesh = pMesh3d->Convert_OpenMesh();
	}
	else
		return;

	EPROP_EDGE_SELECTED_FLAG eprop_selectedflag;

	//Edge selected flag;
	if (!pOpenmesh->pmesh->get_property_handle(eprop_selectedflag, PROP_EDGE_SELECTED))
		pOpenmesh->pmesh->add_property(eprop_selectedflag, PROP_EDGE_SELECTED);

	std::vector<int> m_ArrayEdgeId;
	for (MyMesh::EdgeIter e_it = pOpenmesh->pmesh->edges_begin(); e_it != pOpenmesh->pmesh->edges_end(); ++e_it)
	{
		int flag = pOpenmesh->pmesh->property(eprop_selectedflag, e_it.handle());
		if (flag || pOpenmesh->pmesh->is_boundary(e_it.handle()))
			continue;

		MyMesh::EdgeHandle eh = e_it.handle();

		MyMesh::HalfedgeHandle hh0 = pOpenmesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pOpenmesh->pmesh->halfedge_handle(eh, 1);

		int vh1 = pOpenmesh->pmesh->to_vertex_handle(hh0).idx();
		int vh2 = pOpenmesh->pmesh->from_vertex_handle(hh0).idx();

		flag = pHarmonicMap->IsDelaunay(pOpenmesh, eh.idx());
		if (!flag)
		{
			//AfxMessageBox("Not Delaunay");
			//return;
			m_ArrayEdgeId.push_back(eh.idx());
			double previousval = GetHarmonicEnergy(pOpenmesh);
			pHarmonicMap->SwapAdjacentTriangles(pOpenmesh,eh.idx(), m_ArrayEdgeId);
			double currentval = GetHarmonicEnergy(pOpenmesh);
//			if (currentval > previousval)
//				ASSERT(0);
			TRACE("%f %f \n", previousval, currentval);
		}
	}
	pOpenmesh->pmesh->garbage_collection();
	CMesh3d* pNewMesh = pOpenmesh->ConvertMesh3d();
	pDoc->m_SceneGraph.Add(pNewMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	//AfxMessageBox("Delaunay");
	return;
}


void Actions_DynamicQuasiconformal::OnBnClickedDynamicQuasiLoadtwotriangles()
{
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\dynamicquasiconformal\\twotriangles.obj");
	//p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	//pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pDoc->UpdateAllViews(NULL);
}

void Actions_DynamicQuasiconformal::PlotConvexMeshPara(CPaintDC &dc, BOOL Is_Boundary)
{
	if (!pconvex_para_Openmesh)
		return;

	if (m_ShowMeshMapping)
		return;

	int left, top, right, bottom;

	//convex region
	CRect m_Region_ConvexRegion;
	CWnd*pCanvas = GetDlgItem(IDD_DIALOG_DynamicQuasi_ConvexPara_Static);
	pCanvas->GetWindowRect(&m_Region_ConvexRegion);
	ScreenToClient(&m_Region_ConvexRegion);

	left = m_Region_ConvexRegion.left + 10;
	top = m_Region_ConvexRegion.top + 20;
	right = m_Region_ConvexRegion.right - 10;
	bottom = m_Region_ConvexRegion.bottom - 10;

	if (m_FirstConvexMeshPlot)
	{
		m_FirstConvexMeshPlot = 0;

		meshleft_convexpara = 1e10;
		meshright_convexpara = -1e10;
		meshtop_convexpara = 1e10;
		meshbottom_convexpara = -1e10;

		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		dc.SelectObject(&pen);
		dc.SelectObject(&brush);
		for (MyMesh::VertexIter v_it = pconvex_para_Openmesh->pmesh->vertices_begin(); v_it != pconvex_para_Openmesh->pmesh->vertices_end(); ++v_it)
		{
			OpenMesh::Vec3d v1 = pconvex_para_Openmesh->pmesh->point(v_it.handle());
			if (v1[0] < meshleft_convexpara)
				meshleft_convexpara = v1[0];
			if (v1[0] > meshright_convexpara)
				meshright_convexpara = v1[0];

			if (v1[1] < meshtop_convexpara)
				meshtop_convexpara = v1[1];
			if (v1[1] > meshbottom_convexpara)
				meshbottom_convexpara = v1[1];
		}

		scale_convexpara = (right - left) / (meshright_convexpara - meshleft_convexpara);
		if ((bottom - top) / (meshbottom_convexpara - meshtop_convexpara) < scale_convexpara)
			scale_convexpara = (bottom - top) / (meshbottom_convexpara - meshtop_convexpara);
	}

	for (MyMesh::VertexIter v_it = pconvex_para_Openmesh->pmesh->vertices_begin(); v_it != pconvex_para_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		if (Is_Boundary && !pconvex_para_Openmesh->pmesh->is_boundary(v_it.handle()))
			continue;
		OpenMesh::Vec3d v1 = pconvex_para_Openmesh->pmesh->point(v_it.handle());
		//dc.Ellipse(left + (v1[0]) * scale_convexpara - 2, top + (v1[1]) * scale_convexpara - 2, left + (v1[0]) * scale_convexpara + 2, top + (v1[1]) * scale_convexpara + 2);
		dc.Ellipse(left + (v1[0]) * scale_convexpara - 2, bottom - (v1[1]) * scale_convexpara - 2, left + (v1[0]) * scale_convexpara + 2, bottom - (v1[1]) * scale_convexpara + 2);
		if (m_ShowIndex.GetCheck())
		{
			CString str;
			CRect rect;
			rect.left = left + (v1[0]) * scale_convexpara;
			rect.top = bottom - (v1[1]) * scale_convexpara;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;

			str.Format(_T("%d"), v_it.handle().idx());
			dc.DrawText(str, rect, DT_TOP | DT_LEFT);
		}
	}

	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen1);

	for (MyMesh::EdgeIter e_it = pconvex_para_Openmesh->pmesh->edges_begin(); e_it != pconvex_para_Openmesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();

		if (Is_Boundary && !pconvex_para_Openmesh->pmesh->is_boundary(eh))
			continue;

		MyMesh::HalfedgeHandle hh0 = pconvex_para_Openmesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pconvex_para_Openmesh->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pconvex_para_Openmesh->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pconvex_para_Openmesh->pmesh->from_vertex_handle(hh0);

		OpenMesh::Vec3d v1 = pconvex_para_Openmesh->pmesh->point(vh1);
		OpenMesh::Vec3d v2 = pconvex_para_Openmesh->pmesh->point(vh2);
		dc.MoveTo(left + (v1[0] - meshleft_convexpara) * scale_convexpara, bottom - (v1[1] - meshtop_convexpara) * scale_convexpara);
		dc.LineTo(left + (v2[0] - meshleft_convexpara) * scale_convexpara, bottom - (v2[1] - meshtop_convexpara) * scale_convexpara);

		if (m_ShowIndex.GetCheck())
		{
			CString str;
			CRect rect;
			rect.left = left + ((v1[0] + v2[0]) / 2 - meshleft_convexpara) * scale_convexpara;
			rect.top = bottom - ((v1[1] + v2[1]) / 2 - meshleft_convexpara) * scale_convexpara;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;

			str.Format(_T("%d"), eh.idx());
			dc.DrawText(str, rect, DT_TOP | DT_LEFT);
		}
	}
}
void Actions_DynamicQuasiconformal::PlotTargetIntermediatePara(CPaintDC &dc)
{
	if (!ptarget_para_Openmesh_Intermediate)
		return;

	int left, top, right, bottom;

	//convex region
	CRect m_Region_TargetRegion;
	CWnd*pCanvas = GetDlgItem(IDD_DIALOG_DynamicQuasi_TargetPara_Static);
	pCanvas->GetWindowRect(&m_Region_TargetRegion);
	ScreenToClient(&m_Region_TargetRegion);

	left = m_Region_TargetRegion.left + 10;
	top = m_Region_TargetRegion.top + 20;
	right = m_Region_TargetRegion.right - 10;
	bottom = m_Region_TargetRegion.bottom - 10;

	for (MyMesh::VertexIter v_it = ptarget_para_Openmesh_Intermediate->pmesh->vertices_begin(); v_it != ptarget_para_Openmesh_Intermediate->pmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d v1 = ptarget_para_Openmesh_Intermediate->pmesh->point(v_it.handle());
		dc.Ellipse(left + (v1[0]) * scale_targetpara - 2, bottom - (v1[1]) * scale_targetpara - 2, left + (v1[0]) * scale_targetpara + 2, bottom - (v1[1]) * scale_targetpara + 2);

		if (m_ShowIndex.GetCheck())
		{
			CString str;
			CRect rect;
			rect.left = left + (v1[0]) * scale_targetpara;
			rect.top = bottom - (v1[1]) * scale_targetpara;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;

			str.Format(_T("%d"), v_it.handle().idx());
			dc.DrawText(str, rect, DT_TOP | DT_LEFT);
		}
	}

	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	CPen pen2(PS_SOLID, 1, RGB(255, 0, 0));
	dc.SelectObject(&pen1);

	for (MyMesh::EdgeIter e_it = ptarget_para_Openmesh_Intermediate->pmesh->edges_begin(); e_it != ptarget_para_Openmesh_Intermediate->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();

		MyMesh::HalfedgeHandle hh0 = ptarget_para_Openmesh_Intermediate->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = ptarget_para_Openmesh_Intermediate->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = ptarget_para_Openmesh_Intermediate->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = ptarget_para_Openmesh_Intermediate->pmesh->from_vertex_handle(hh0);

		OpenMesh::Vec3d v1 = ptarget_para_Openmesh_Intermediate->pmesh->point(vh1);
		OpenMesh::Vec3d v2 = ptarget_para_Openmesh_Intermediate->pmesh->point(vh2);

		dc.SelectObject(&pen1);
		dc.SelectStockObject(NULL_BRUSH);
		if (m_ShowCircle.GetCheck())
		{
			if (!ptarget_para_Openmesh_Intermediate->pmesh->is_boundary(eh))
			{
				if (!pHarmonicMapping->IsDelaunay(ptarget_para_Openmesh_Intermediate, eh.idx()))
				{
					dc.SelectObject(&pen2);
					std::vector<int> oppsitevertices;
					pHarmonicMapping->GetTwoOppositePoints(ptarget_para_Openmesh_Intermediate, eh.idx(), oppsitevertices);

					OpenMesh::Vec3d v3 = ptarget_para_Openmesh_Intermediate->pmesh->point(ptarget_para_Openmesh_Intermediate->pmesh->vertex_handle(oppsitevertices[0]));
					OpenMesh::Vec3d v4 = ptarget_para_Openmesh_Intermediate->pmesh->point(ptarget_para_Openmesh_Intermediate->pmesh->vertex_handle(oppsitevertices[1]));

					double centerx, centery, radius;
					findCircle(v1[0], v1[1], v2[0], v2[1], v3[0], v3[1], centerx, centery, radius);
					double x0 = centerx - radius;
					double y0 = centery - radius;
					double x1 = centerx + radius;
					double y1 = centery + radius;
					x0 = left + (x0 - meshleft_targetpara) * scale_targetpara;
					x1 = left + (x1 - meshleft_targetpara) * scale_targetpara;

					y0 = bottom - (y0 - meshtop_targetpara) * scale_targetpara;
					y1 = bottom - (y1 - meshtop_targetpara) * scale_targetpara;
					dc.Ellipse(x0, y0, x1, y1);

					findCircle(v1[0], v1[1], v2[0], v2[1], v4[0], v4[1], centerx, centery, radius);

					x0 = centerx - radius;
					y0 = centery - radius;
					x1 = centerx + radius;
					y1 = centery + radius;
					x0 = left + (x0 - meshleft_targetpara) * scale_targetpara;
					x1 = left + (x1 - meshleft_targetpara) * scale_targetpara;

					y0 = bottom - (y0 - meshtop_targetpara) * scale_targetpara;
					y1 = bottom - (y1 - meshtop_targetpara) * scale_targetpara;

					dc.Ellipse(x0, y0, x1, y1);

					CString str;
					CRect rect;
					rect.left = left + ((v1[0] + v2[0]) / 2 - meshleft_targetpara) * scale_targetpara;
					rect.top = bottom - ((v1[1] + v2[1]) / 2 - meshtop_targetpara) * scale_targetpara;
					rect.right = rect.left + 30;
					rect.bottom = rect.top + 30;

					str.Format(_T("%d"), eh.idx());
					dc.DrawText(str, rect, DT_TOP | DT_LEFT);
				}
			}
		}
			
		dc.MoveTo(left + (v1[0] - meshleft_targetpara) * scale_targetpara, bottom - (v1[1] - meshtop_targetpara) * scale_targetpara);
		dc.LineTo(left + (v2[0] - meshleft_targetpara) * scale_targetpara, bottom - (v2[1] - meshtop_targetpara) * scale_targetpara);

		if (m_ShowIndex.GetCheck())
		{
			CString str;
			CRect rect;
			rect.left = left + ((v1[0] + v2[0]) / 2 - meshleft_targetpara) * scale_targetpara;
			rect.top = bottom - ((v1[1] + v2[1]) / 2 - meshleft_targetpara) * scale_targetpara;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;

			str.Format(_T("%d"), eh.idx());
			dc.DrawText(str, rect, DT_TOP | DT_LEFT);
		}
	}
}
void Actions_DynamicQuasiconformal::PlotTargetMeshPara(CPaintDC &dc, BOOL Is_Boundary)
{
	if (!ptarget_para_Openmesh)
		return;

	int left, top, right, bottom;

	//convex region
	CRect m_Region_TargetRegion;
	CWnd*pCanvas = GetDlgItem(IDD_DIALOG_DynamicQuasi_TargetPara_Static);
	pCanvas->GetWindowRect(&m_Region_TargetRegion);
	ScreenToClient(&m_Region_TargetRegion);

	left = m_Region_TargetRegion.left + 10;
	top = m_Region_TargetRegion.top + 20;
	right = m_Region_TargetRegion.right - 10;
	bottom = m_Region_TargetRegion.bottom - 10;

	if (m_FirstTargetMeshPlot)
	{
		m_FirstTargetMeshPlot = 0;

		meshleft_targetpara = 1e10;
		meshright_targetpara = -1e10;
		meshtop_targetpara = 1e10;
		meshbottom_targetpara = -1e10;

		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		dc.SelectObject(&pen);
		dc.SelectObject(&brush); 
		for (MyMesh::VertexIter v_it = ptarget_para_Openmesh->pmesh->vertices_begin(); v_it != ptarget_para_Openmesh->pmesh->vertices_end(); ++v_it)
		{
			OpenMesh::Vec3d v1 = ptarget_para_Openmesh->pmesh->point(v_it.handle());
			if (v1[0] < meshleft_targetpara)
				meshleft_targetpara = v1[0];
			if (v1[0] > meshright_targetpara)
				meshright_targetpara = v1[0];

			if (v1[1] < meshtop_targetpara)
				meshtop_targetpara = v1[1];
			if (v1[1] > meshbottom_targetpara)
				meshbottom_targetpara = v1[1];
		}

		scale_targetpara = (right - left) / (meshright_targetpara - meshleft_targetpara);
		if ((bottom - top) / (meshbottom_targetpara - meshtop_targetpara) < scale_targetpara)
			scale_targetpara = (bottom - top) / (meshbottom_targetpara - meshtop_targetpara);
	}


	for (MyMesh::VertexIter v_it = ptarget_para_Openmesh->pmesh->vertices_begin(); v_it != ptarget_para_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		if (Is_Boundary && !ptarget_para_Openmesh->pmesh->is_boundary(v_it.handle()))
			continue;
		OpenMesh::Vec3d v1 = ptarget_para_Openmesh->pmesh->point(v_it.handle());
		dc.Ellipse(left + (v1[0]) * scale_targetpara - 2, bottom - (v1[1]) * scale_targetpara - 2, left + (v1[0]) * scale_targetpara + 2, bottom - (v1[1]) * scale_targetpara + 2);

		if (m_ShowIndex.GetCheck())
		{
			CString str;
			CRect rect;
			rect.left = left + (v1[0]) * scale_targetpara;
			rect.top = bottom - (v1[1]) * scale_targetpara;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;

			str.Format(_T("%d"), v_it.handle().idx());
			dc.DrawText(str, rect, DT_TOP | DT_LEFT);
		}
	}

	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen1);

	for (MyMesh::EdgeIter e_it = ptarget_para_Openmesh->pmesh->edges_begin(); e_it != ptarget_para_Openmesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();

		if (Is_Boundary && !ptarget_para_Openmesh->pmesh->is_boundary(eh))
			continue;

		MyMesh::HalfedgeHandle hh0 = ptarget_para_Openmesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = ptarget_para_Openmesh->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = ptarget_para_Openmesh->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = ptarget_para_Openmesh->pmesh->from_vertex_handle(hh0);

		OpenMesh::Vec3d v1 = ptarget_para_Openmesh->pmesh->point(vh1);
		OpenMesh::Vec3d v2 = ptarget_para_Openmesh->pmesh->point(vh2);
		dc.MoveTo(left + (v1[0] - meshleft_targetpara) * scale_targetpara, bottom - (v1[1] - meshtop_targetpara) * scale_targetpara);
		dc.LineTo(left + (v2[0] - meshleft_targetpara) * scale_targetpara, bottom - (v2[1] - meshtop_targetpara) * scale_targetpara);

		if (m_ShowIndex.GetCheck())
		{
			CString str;
			CRect rect;
			rect.left = left + ((v1[0] + v2[0]) / 2 - meshleft_targetpara) * scale_targetpara;
			rect.top = bottom - ((v1[1] + v2[1]) / 2 - meshleft_targetpara) * scale_targetpara;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;

			str.Format(_T("%d"), eh.idx());
			dc.DrawText(str, rect, DT_TOP | DT_LEFT);
		}
	}
}
void Actions_DynamicQuasiconformal::PlotCurrentMeshMapping(CPaintDC &dc)
{
	if (!m_ShowMeshMapping)
		return;

	int left, top, right, bottom;

	m_CurrentMeshMapping = m_DynamicMappingSlider.GetPos() - 1;
	CMesh3d_OpenMesh* pMesh = m_ArrayMeshMappingSequences[m_CurrentMeshMapping]->pStartMesh;

	CPen pen1(PS_SOLID, 3, RGB(255, 0, 0));
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));

	//convex region
	CRect m_Region_ConvexRegion;
	CWnd*pCanvas = GetDlgItem(IDD_DIALOG_DynamicQuasi_ConvexPara_Static);
	pCanvas->GetWindowRect(&m_Region_ConvexRegion);
	ScreenToClient(&m_Region_ConvexRegion);

	left = m_Region_ConvexRegion.left + 10;
	top = m_Region_ConvexRegion.top + 20;
	right = m_Region_ConvexRegion.right - 10;
	bottom = m_Region_ConvexRegion.bottom - 10;

	for (MyMesh::VertexIter v_it = pMesh->pmesh->vertices_begin(); v_it != pMesh->pmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d v1 = pconvex_para_Openmesh->pmesh->point(v_it.handle());
		dc.Ellipse(left + (v1[0]) * scale_convexpara - 2, bottom - (v1[1]) * scale_convexpara - 2, left + (v1[0]) * scale_convexpara + 2, bottom - (v1[1]) * scale_convexpara + 2);
	}

	int nIndex = m_ActionsCombo.GetCurSel();
	if (nIndex == -1)
		return;

	CString strCBText;
	m_ActionsCombo.GetLBText(nIndex, strCBText);

	for (MyMesh::EdgeIter e_it = pMesh->pmesh->edges_begin(); e_it != pMesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		if (strCBText.Compare("IntialDelaunaySwap") == 0)
		{
			if (eh.idx() == m_ArrayMeshMappingSequences[m_CurrentMeshMapping]->m_StartEdgeIndex)
			{
				dc.SelectObject(&pen1);
			}
		}
		else
		{
			dc.SelectObject(&pen);
		}
		MyMesh::HalfedgeHandle hh0 = pMesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pMesh->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pMesh->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pMesh->pmesh->from_vertex_handle(hh0);

		OpenMesh::Vec3d v1 = pMesh->pmesh->point(vh1);
		OpenMesh::Vec3d v2 = pMesh->pmesh->point(vh2);
		dc.MoveTo(left + (v1[0] - meshleft_convexpara) * scale_convexpara, bottom - (v1[1] - meshtop_convexpara) * scale_convexpara);
		dc.LineTo(left + (v2[0] - meshleft_convexpara) * scale_convexpara, bottom - (v2[1] - meshtop_convexpara) * scale_convexpara);
	}
}
void Actions_DynamicQuasiconformal::OnPaint()
{
	 // device context for painting
	// TODO: Add your message handler code here
	// Do not call CPropertyPage::OnPaint() for painting messages
	//dc.MoveTo(100, 100);
	//dc.LineTo(1200, 1200);
	CPaintDC dc(this);
	//dc.MoveTo(m_Region_ConvexRegion.left + 10, m_Region_ConvexRegion.top + 10);
	//dc.LineTo(m_Region_ConvexRegion.right - 10, m_Region_ConvexRegion.bottom - 10);

	int nIndex = m_ActionsCombo.GetCurSel();
	CString strCBText;

	if (m_ShowIntialConvexPara.GetCheck())
		PlotConvexMeshPara(dc, convexmesh_onlyshowboundary);

	//target region
	if (m_ShowInitialTargetPara.GetCheck())
		PlotTargetMeshPara(dc, tarmesh_onlyshowboundary);

	//Plot
	if (nIndex > -1)
	{
		m_ActionsCombo.GetLBText(nIndex, strCBText);
		if (strCBText.Compare("MeshInterpolate") != 0)
		{
			PlotCurrentMeshMapping(dc);
		}
	}

	//
	if (m_ShowIntermediateTargetPara.GetCheck())
		PlotTargetIntermediatePara(dc);
	//dc.MoveTo(m_Region_TargetRegion.left, m_Region_TargetRegion.top);
	//dc.LineTo(m_Region_TargetRegion.right, m_Region_TargetRegion.bottom);
}

void Actions_DynamicQuasiconformal::OnBnClickedDynamicSettargetboundary()
{
	// TODO: Add your control notification handler code here
	ptarget_para_Openmesh = pconvex_para_Openmesh->copy();
	ptarget_para_Mesh3d = new CMesh3d(*pconvex_para_Mesh3d);
	pDoc->UpdateAllViews(NULL);
	//pconvex_para_Mesh3d->Copy(ptarget_para_Mesh3d);
}


void Actions_DynamicQuasiconformal::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int nIndex = m_ActionsCombo.GetCurSel();
	if (nIndex == -1)
		return;

	CString strCBText;
	m_ActionsCombo.GetLBText(nIndex, strCBText);
	if (strCBText.Compare("DragTargetPara") != 0)
		return;

	double mind = 1e10;
	selectvindex = -1;
	int left, top, right, bottom;

	//convex region
	CRect m_Region_TargetRegion;
	CWnd*pCanvas = GetDlgItem(IDD_DIALOG_DynamicQuasi_TargetPara_Static);
	pCanvas->GetWindowRect(&m_Region_TargetRegion);
	ScreenToClient(&m_Region_TargetRegion);

	left = m_Region_TargetRegion.left + 10;
	top = m_Region_TargetRegion.top + 20;
	right = m_Region_TargetRegion.right - 10;
	bottom = m_Region_TargetRegion.bottom - 10;

	double x = (point.x - left) / scale_targetpara + meshleft_targetpara;
	double y = (point.y - top) / scale_targetpara + meshtop_targetpara;

	for (MyMesh::VertexIter v_it = ptarget_para_Openmesh->pmesh->vertices_begin(); v_it != ptarget_para_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d v1 = ptarget_para_Openmesh->pmesh->point(v_it.handle());

		double d1 = pow(v1[0] - x, 2) + pow(v1[1] - y, 2);
		if (d1 < mind)
		{
			mind = d1;
			if (mind < 5)
			{
				selectvindex = v_it.handle().idx();
			}
		}

		//dc.Ellipse(left + (v1[0]) * scale - 2, top + (v1[1]) * scale - 2, left + (v1[0]) * scale + 2, top + (v1[1]) * scale + 2);
	}
	CString   str;
	//char message[80];
	//sprintf_s(message, "%f  %f", val1, val2);
	str.Format(_T("Selecte Vertex: %d"), selectvindex);
	AfxGetMainWnd()->SetWindowText(LPCTSTR(str));
	CDialogEx::OnLButtonDown(nFlags, point);
}


void Actions_DynamicQuasiconformal::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (selectvindex > -1)
	{
		int left, top, right, bottom;
		//convex region
		CRect m_Region_TargetRegion;
		CWnd*pCanvas = GetDlgItem(IDD_DIALOG_DynamicQuasi_TargetPara_Static);
		pCanvas->GetWindowRect(&m_Region_TargetRegion);
		ScreenToClient(&m_Region_TargetRegion);

		left = m_Region_TargetRegion.left + 10;
		top = m_Region_TargetRegion.top + 20;
		right = m_Region_TargetRegion.right - 10;
		bottom = m_Region_TargetRegion.bottom - 10;

		double x = (point.x - left) / scale_targetpara + meshleft_targetpara;
		double y = (point.y - top) / scale_targetpara + meshtop_targetpara;

		OpenMesh::Vec3d v1;
		v1[0] = x;
		v1[1] = y;
		v1[2] = 0;

		ptarget_para_Openmesh->pmesh->set_point(ptarget_para_Openmesh->pmesh->vertex_handle(selectvindex), v1);
		pDoc->UpdateAllViews(NULL);
		selectvindex = -1;
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void Actions_DynamicQuasiconformal::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (selectvindex > -1)
	{
		int left, top, right, bottom;

		//convex region
		CRect m_Region_TargetRegion;
		CWnd*pCanvas = GetDlgItem(IDD_DIALOG_DynamicQuasi_TargetPara_Static);
		pCanvas->GetWindowRect(&m_Region_TargetRegion);
		ScreenToClient(&m_Region_TargetRegion);

		left = m_Region_TargetRegion.left + 10;
		top = m_Region_TargetRegion.top + 20;
		right = m_Region_TargetRegion.right - 10;
		bottom = m_Region_TargetRegion.bottom - 10;

		double x = (point.x - left) / scale_targetpara + meshleft_targetpara;
		double y = (point.y - top) / scale_targetpara + meshtop_targetpara;

		OpenMesh::Vec3d v1;
		v1[0] = x;
		v1[1] = y;
		v1[2] = 0;

		ptarget_para_Openmesh->pmesh->set_point(ptarget_para_Openmesh->pmesh->vertex_handle(selectvindex), v1);
		pDoc->UpdateAllViews(NULL);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void Actions_DynamicQuasiconformal::OnBnClickedDynamicSavetargetparamesh()
{
	// TODO: Add your control notification handler code here
	if (ptarget_para_Openmesh)
	{
		CFileDialog dlg(FALSE);
		if (dlg.DoModal() == IDOK)
		{
			ptarget_para_Openmesh->SaveOBJFile((LPSTR)(LPCTSTR)dlg.GetPathName());
		}
	}
}


void Actions_DynamicQuasiconformal::OnBnClickedDynamicLoadtargetparamesh()
{
	if (ptarget_para_Openmesh)
	{
		//CFileDialog dlg(TRUE);
		//if (dlg.DoModal() == IDOK)
		{
			CSceneGraph3d m_SceneGraph1;
			//pDoc->Read_Mesh_OpenMesh((LPSTR)(LPCTSTR)dlg.GetPathName(), m_SceneGraph1);
			pDoc->Read_Mesh_OpenMesh("D:\\papers\\code\\data\\dynamicquasiconformal\\example1\\3Dface_targetpara.obj", m_SceneGraph1);
			if (m_SceneGraph1.NbObject() == 1)
			{
				delete ptarget_para_Openmesh;
				ptarget_para_Openmesh = (CMesh3d_OpenMesh*)m_SceneGraph1.GetAt(0);
				double energy = GetHarmonicEnergy(ptarget_para_Openmesh);
				m_SceneGraph1.RemoveAt(0);
				pDoc->UpdateTreeControl();
				pDoc->UpdateAllViews(NULL);
			}
			//ptarget_para_Openmesh->SaveOBJFile((LPSTR)(LPCTSTR)dlg.GetPathName());
		}
	}
}

void Actions_DynamicQuasiconformal::findCircle(double x1, double y1, double x2, double y2, double x3, double y3, double &centerx, double &centery, double &radius)
{

	double A1, A2, B1, B2, C1, C2, temp;
	A1 = x1 - x2;
	B1 = y1 - y2;
	C1 = (pow(x1, 2) - pow(x2, 2) + pow(y1, 2) - pow(y2, 2)) / 2;
	A2 = x3 - x2;
	B2 = y3 - y2;
	C2 = (pow(x3, 2) - pow(x2, 2) + pow(y3, 2) - pow(y2, 2)) / 2;
	//为了方便编写程序，令temp = A1*B2 - A2*B1  
	temp = A1*B2 - A2*B1;
	//定义一个圆的数据的结构体对象CD  
	//判断三点是否共线  
	if (temp == 0) {
		//共线则将第一个点pt1作为圆心  
		centerx = x1;
		centery = y1;
	}
	else {
		//不共线则求出圆心：  
		//center.x = (C1*B2 - C2*B1) / A1*B2 - A2*B1;  
		//center.y = (A1*C2 - A2*C1) / A1*B2 - A2*B1;  
		centerx = (C1*B2 - C2*B1) / temp;
		centery = (A1*C2 - A2*C1) / temp;
	}

	radius = sqrtf((centerx - x1)*(centerx - x1) + (centery - y1)*(centery - y1));
}
void Actions_DynamicQuasiconformal::OnBnClickedDynamicDynamicharmonic()
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pconvex_para_Openmesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pconvex_para_Openmesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	// TODO: Add your control notification handler code here
	//Harmonic mapping
	//((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_LAPLACE))->SetCheck(TRUE);
	pHarmonicMapping->HarmonicMappingLaplace(pconvex_para_Openmesh, ptarget_para_Openmesh);
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	OpenMesh::Vec3d v1;
	for (MyMesh::VertexIter v_it = pconvex_para_Openmesh->pmesh->vertices_begin(); v_it != pconvex_para_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		//OpenMesh::Vec3d v1 = ptarget_para_Openmesh->pmesh->point(v_it.handle());

		int boundaryflag = pconvex_para_Openmesh->pmesh->is_boundary(v_it.handle());

		if (boundaryflag)
			continue;

		uvvalues = pconvex_para_Openmesh->pmesh->property(vprop_vertex_uv, v_it.handle());
		v1[0] = uvvalues[0];
		v1[1] = uvvalues[1];
		v1[2] = 0;
		ptarget_para_Openmesh->pmesh->set_point(v_it.handle(), v1);
		//dc.Ellipse(left + (v1[0]) * scale - 2, top + (v1[1]) * scale - 2, left + (v1[0]) * scale + 2, top + (v1[1]) * scale + 2);
	}
	double energy = GetHarmonicEnergy(ptarget_para_Openmesh);
	pDoc->m_SceneGraph.Add(ptarget_para_Openmesh);
	pDoc->UpdateTreeControl();
	tarmesh_onlyshowboundary = 0;
	pDoc->UpdateAllViews(NULL);
}
double Actions_DynamicQuasiconformal::SearchSmallestStep(CMesh3d_OpenMesh* pCurrentMesh, int &edgeindex)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pCurrentMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pCurrentMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//
	double minstep = 1e10;
	double step;

	for (MyMesh::EdgeIter e_it = pCurrentMesh->pmesh->edges_begin(); e_it != pCurrentMesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();

		if (pCurrentMesh->pmesh->is_boundary(eh))
			continue;

		//IsDelaunay
		step = GetSmallestStep(pCurrentMesh, eh.idx());

		if (step < minstep)
		{
			minstep = step;
			edgeindex = eh.idx();
		}
	}
	return minstep;
}
double Actions_DynamicQuasiconformal::SearchSmallestStep_CDT(CMesh3d_OpenMesh* pCurrentMesh, int &edgeindex)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pCurrentMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pCurrentMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	EPROP_EDGE_CONSTRAINED_FLAG eprop_constrainedflag;

	//Edge selected flag;
	if (!pCurrentMesh->pmesh->get_property_handle(eprop_constrainedflag, PROP_EDGE_CONSTRAINED))
		pCurrentMesh->pmesh->add_property(eprop_constrainedflag, PROP_EDGE_CONSTRAINED);

	//
	double minstep = 1e10;
	double step;

	for (MyMesh::EdgeIter e_it = pCurrentMesh->pmesh->edges_begin(); e_it != pCurrentMesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();

		if (pCurrentMesh->pmesh->is_boundary(eh))
			continue;

		if (pCurrentMesh->pmesh->property(eprop_constrainedflag, eh) == 1)
			continue;

		MyMesh::HalfedgeHandle hh0 = pCurrentMesh->pmesh->halfedge_handle(eh, 0);

		int vh1 = pCurrentMesh->pmesh->to_vertex_handle(hh0).idx();
		int vh2 = pCurrentMesh->pmesh->from_vertex_handle(hh0).idx();

		if (pDataAnalysis->SearchMeshMappingSequencesForSpecificDiagonalSwap(vh1, vh2) > -1)
			continue;

		//IsDelaunay
		step = GetSmallestStep(pCurrentMesh, eh.idx());

		if (step < minstep)
		{
			minstep = step;
			edgeindex = eh.idx();
		}
	}
	return minstep;
}
double Actions_DynamicQuasiconformal::ComputeStepDelaunay(CMesh3d_OpenMesh* pMesh3D, int ehindex, double step1, double step2)
{
	double TOL = 0.0001;
	double value1 = ComputeSumOppositeAngle(pMesh3D, ehindex, step1);
	double value2 = ComputeSumOppositeAngle(pMesh3D, ehindex, step2);
	while (fabs(step1 - step2) > TOL)
	{
		double value3 = ComputeSumOppositeAngle(pMesh3D, ehindex, (step1 + step2) / 2);

		if (value3 > PI)
		{
			step2 = (step1 + step2) / 2;
		}
		else
		{
			step1 = (step1 + step2) / 2;
		}
	}
	return step1;
}
double Actions_DynamicQuasiconformal::GetSmallestStep(CMesh3d_OpenMesh* pMesh3D, int ehindex)
{
	int StepNum = 10;
	double totalangle;
	for (int i = 1; i <= StepNum; i++)
	{
		totalangle = ComputeSumOppositeAngle(pMesh3D, ehindex, (i + 0.0) / StepNum);
		if (totalangle > PI)
		{
			return ComputeStepDelaunay(pMesh3D, ehindex, (i - 1.0) / StepNum, (i - 0.0) / StepNum);
		}
	}
	return 1.0;
}
double Actions_DynamicQuasiconformal::ComputeSumOppositeAngle(CMesh3d_OpenMesh* pMesh3D, int ehindex, double t)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	MyMesh::EdgeHandle eh = pMesh3D->pmesh->edge_handle(ehindex);

	MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

	int vh1 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
	int vh2 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();

	std::vector<int> oppsitevertices;
	pHarmonicMapping->GetTwoOppositePoints(pMesh3D, ehindex, oppsitevertices);
	std::vector<double> uvvalues1, uvvalues2, uvvalues3, uvvalues4;
	if (oppsitevertices.size() == 2)
	{
		int vh3 = oppsitevertices[0];
		int vh4 = oppsitevertices[1];

		OpenMesh::Vec3d vv1 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh1));
		OpenMesh::Vec3d vv2 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh2));
		OpenMesh::Vec3d vv3 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh3));
		OpenMesh::Vec3d vv4 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh4));

		uvvalues1 = pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(vh1));
		uvvalues2 = pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(vh2));
		uvvalues3 = pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(vh3));
		uvvalues4 = pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(vh4));

		vv1[0] = vv1[0] * (1 - t) + uvvalues1[0] * t;
		vv1[1] = vv1[1] * (1 - t) + uvvalues1[1] * t;

		vv2[0] = vv2[0] * (1 - t) + uvvalues2[0] * t;
		vv2[1] = vv2[1] * (1 - t) + uvvalues2[1] * t;

		vv3[0] = vv3[0] * (1 - t) + uvvalues3[0] * t;
		vv3[1] = vv3[1] * (1 - t) + uvvalues3[1] * t;

		vv4[0] = vv4[0] * (1 - t) + uvvalues4[0] * t;
		vv4[1] = vv4[1] * (1 - t) + uvvalues4[1] * t;

		double e23 = Length(vv2 - vv3);
		double e13 = Length(vv1 - vv3);
		double e12 = Length(vv1 - vv2);
		double e24 = Length(vv2 - vv4);
		double e14 = Length(vv1 - vv4);

		double angle231 = acos((SQR(e23) + SQR(e13) - SQR(e12)) / (2 * e23 * e13));
		double angle241 = acos((SQR(e24) + SQR(e14) - SQR(e12)) / (2 * e24 * e14));

		return angle231 + angle241;
	}
	else
		ASSERT(0);
}

void Actions_DynamicQuasiconformal::GetHarmonicIterations(CMesh3d_OpenMesh* pCurrentMesh)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pCurrentMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pCurrentMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	int itenum = 0;
	while (1)
	{
		pDoc->m_SceneGraph.Add(pCurrentMesh->copy());
		//ptarget_para_Openmesh_Intermediate = pCurrentMesh->copy();
		ptarget_para_Openmesh_Intermediate = pCurrentMesh;

		//
		pHarmonicMapping->HarmonicMappingLaplace(pCurrentMesh, ptarget_para_Openmesh);

		double energy1 = GetUVHarmonicEnergy(pCurrentMesh);
		//ceshi
		/*if (itenum == 50)
		{
			//ptarget_para_Openmesh_Intermediate
			ptarget_para_Openmesh_current = pCurrentMesh;
			pHarmonicMapping->GenerateParameterDomain(pCurrentMesh);
			//pDoc->m_SceneGraph.Add(pCurrentMesh);
			m_DynamicMappingSlider.SetRange(0, 100);//设置滑动范围为1到20
			m_DynamicMappingSlider.SetTicFreq(1);//每1个单位画一刻度
			m_DynamicMappingSlider.SetPos(1);//设置滑块初始位置为1
			pDoc->UpdateTreeControl();
			return;
		}*/

		//the parameterization has flip? 
		if (!pHarmonicMapping->IsMeshUVFlip(pCurrentMesh, 0))
		{
			pDoc->m_SceneGraph.Add(pCurrentMesh);
			double energy = GetHarmonicEnergy(pCurrentMesh);
			double energy1 = GetUVHarmonicEnergy(pCurrentMesh);
			ptarget_para_Openmesh_current = pCurrentMesh;
			pHarmonicMapping->GenerateParameterDomain(pCurrentMesh);
			//pDoc->m_SceneGraph.Add(pCurrentMesh);
			m_DynamicMappingSlider.SetRange(0, 100);//设置滑动范围为1到20
			m_DynamicMappingSlider.SetTicFreq(1);//每1个单位画一刻度
			m_DynamicMappingSlider.SetPos(1);//设置滑块初始位置为1
			pDoc->UpdateTreeControl();
			break;
		}

		//Search the step
		double step;
		int edgeindex;
		step = SearchSmallestStep(pCurrentMesh, edgeindex);

		std::vector<double> uvvalues;
		uvvalues.resize(2);
		OpenMesh::Vec3d targetpoint, initialpoint;
		for (MyMesh::VertexIter v_it = pCurrentMesh->pmesh->vertices_begin(); v_it != pCurrentMesh->pmesh->vertices_end(); ++v_it)
		{
			initialpoint = pCurrentMesh->pmesh->point(v_it.handle());
			uvvalues = pCurrentMesh->pmesh->property(vprop_vertex_uv, v_it.handle());
			targetpoint[0] = initialpoint[0] * (1 - step) + uvvalues[0] * step;
			targetpoint[1] = initialpoint[1] * (1 - step) + uvvalues[1] * step;
			targetpoint[2] = 0;
			ptarget_para_Openmesh_Intermediate->pmesh->set_point(v_it.handle(), targetpoint);
		}

		for (MyMesh::VertexIter v_it = ptarget_para_Openmesh_Intermediate->pmesh->vertices_begin(); v_it != ptarget_para_Openmesh_Intermediate->pmesh->vertices_end(); ++v_it)
		{
			initialpoint = ptarget_para_Openmesh_Intermediate->pmesh->point(v_it.handle());

			uvvalues[0] = initialpoint[0];
			uvvalues[1] = initialpoint[1];
			pCurrentMesh->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
		}

		//Add to the mesh mapping array
		MeshMapping* pMeshMapping = new MeshMapping();
		pMeshMapping->pStartMesh = pCurrentMesh;
		pMeshMapping->m_IsEdgeSwap = 0;
		m_ArrayMeshMappingSequences.push_back(pMeshMapping);

		std::vector<int> m_ArrayEdgeId;

		//edgeswap
		pHarmonicMapping->SwapAdjacentTriangles(ptarget_para_Openmesh_Intermediate, edgeindex, m_ArrayEdgeId);
		ptarget_para_Openmesh_Intermediate->pmesh->garbage_collection();

		pCurrentMesh = ptarget_para_Openmesh_Intermediate;
		itenum++;
	}

	m_show_target_intermediate = 1;
	//tarmesh_onlyshowboundary = 0;
	pDoc->UpdateAllViews(NULL);

}
void Actions_DynamicQuasiconformal::GetCDTMesh_Simplify(CMesh3d_OpenMesh* pCurrentMesh)
{
	EPROP_EDGE_CONSTRAINED_FLAG eprop_constrainedflag;

	//Edge selected flag;
	if (!pCurrentMesh->pmesh->get_property_handle(eprop_constrainedflag, PROP_EDGE_CONSTRAINED))
		pCurrentMesh->pmesh->add_property(eprop_constrainedflag, PROP_EDGE_CONSTRAINED);

	for (MyMesh::EdgeIter e_it = pCurrentMesh->pmesh->edges_begin(); e_it != pCurrentMesh->pmesh->edges_end(); ++e_it)
	{
		pCurrentMesh->pmesh->property(eprop_constrainedflag, e_it.handle()) = 0;
	}

	for (int i = 0; i < pCurrentMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pCurrentMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size() - 1; j++)
		{
			int vindex1 = pLandmark->landmark_points[j];
			int vindex2 = pLandmark->landmark_points[j + 1];

			int edgehandleindex = pCurrentMesh->GetEdgeHandleIdx(vindex1, vindex2);
			pCurrentMesh->pmesh->property(eprop_constrainedflag, pCurrentMesh->pmesh->edge_handle(edgehandleindex)) = 1;
		}
	}


	//Obtain a CDT Initial Mesh
	std::vector<int> m_ArrayEdgeId;
	while (1)
	{
		m_ArrayEdgeId.clear();
		//Find all the unDelaunay edges
		for (MyMesh::EdgeIter e_it = pCurrentMesh->pmesh->edges_begin(); e_it != pCurrentMesh->pmesh->edges_end(); ++e_it)
		{
			MyMesh::EdgeHandle eh = e_it.handle();

			if (pCurrentMesh->pmesh->is_boundary(eh))
				continue;

			if (pCurrentMesh->pmesh->property(eprop_constrainedflag, eh) == 1)
				continue;

			//IsDelaunay
			if (pHarmonicMapping->IsDelaunay(pCurrentMesh, eh.idx()))
				continue;

			m_ArrayEdgeId.push_back(eh.idx());
			break;
		}

		if (m_ArrayEdgeId.size() == 0)
			break;

		MeshMapping* pMeshMapping = new MeshMapping();
		pMeshMapping->m_IsEdgeSwap = 1;
		pMeshMapping->pStartMesh = NULL;

		MyMesh::EdgeHandle eh = pCurrentMesh->pmesh->edge_handle(m_ArrayEdgeId[0]);

		MyMesh::HalfedgeHandle hh0 = pCurrentMesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pCurrentMesh->pmesh->halfedge_handle(eh, 1);

		int vh1 = pCurrentMesh->pmesh->to_vertex_handle(hh0).idx();
		int vh2 = pCurrentMesh->pmesh->from_vertex_handle(hh0).idx();

		pMeshMapping->m_SwapEdgeEndPonits.first = vh1;
		pMeshMapping->m_SwapEdgeEndPonits.second = vh2;

		//swap
		pHarmonicMapping->SwapAdjacentTriangles(pCurrentMesh, m_ArrayEdgeId[0], m_ArrayEdgeId);

		m_ArrayMeshMappingSequences.push_back(pMeshMapping);

		pCurrentMesh->pmesh->garbage_collection();
	}
}
void Actions_DynamicQuasiconformal::GetCDTMesh(CMesh3d_OpenMesh* pCurrentMesh)
{
	//Obtain a CDT Initial Mesh
	std::vector<int> m_ArrayEdgeId;
	while (1)
	{
		m_ArrayEdgeId.clear();
		//Find all the unDelaunay edges
		for (MyMesh::EdgeIter e_it = pCurrentMesh->pmesh->edges_begin(); e_it != pCurrentMesh->pmesh->edges_end(); ++e_it)
		{
			MyMesh::EdgeHandle eh = e_it.handle();

			if (pCurrentMesh->pmesh->is_boundary(eh))
				continue;

			//IsDelaunay
			if (pHarmonicMapping->IsDelaunay(pCurrentMesh, eh.idx()))
				continue;

			m_ArrayEdgeId.push_back(eh.idx());
			break;
		}

		if (m_ArrayEdgeId.size() == 0)
			break;

		MeshMapping* pMeshMapping = new MeshMapping();
		pMeshMapping->m_IsEdgeSwap = 1;
		pMeshMapping->pStartMesh = pCurrentMesh->copy();
		pMeshMapping->m_StartEdgeIndex = m_ArrayEdgeId[0];

		//swap
		pHarmonicMapping->SwapAdjacentTriangles(pCurrentMesh, m_ArrayEdgeId[0], m_ArrayEdgeId);

		m_ArrayMeshMappingSequences.push_back(pMeshMapping);

		pCurrentMesh->pmesh->garbage_collection();
	}
}
double Actions_DynamicQuasiconformal::MeshDifferences(CMesh3d_OpenMesh* pMesh1, CMesh3d_OpenMesh* pMesh2)
{
	double maxdis = -1;
	for (MyMesh::VertexIter v_it = pMesh1->pmesh->vertices_begin(); v_it != pMesh1->pmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d v1 = pMesh1->pmesh->point(v_it.handle());
		OpenMesh::Vec3d v2 = pMesh2->pmesh->point(v_it.handle());
		double d = Distance(v1, v2);
		if (d > maxdis)
			maxdis = d;
	}
	return maxdis;
}
void Actions_DynamicQuasiconformal::OnBnClickedDynamicDynamicharmonicMappingsequences()
{
	// TODO: Add your control notification handler code here
	//First make the initial mesh parameterization as CDT
	CMesh3d_OpenMesh* pCurrentMesh = pconvex_para_Openmesh;

	//Obtain CDT
	GetCDTMesh(pCurrentMesh);

	//double energy = GetHarmonicEnergy(pCurrentMesh);
	//pDoc->m_SceneGraph.Add(pCurrentMesh);
	//pDoc->UpdateTreeControl();
	//return;
	//
	GetHarmonicIterations(pCurrentMesh);

	//
	//double dis = MeshDifferences(ptarget_para_Openmesh, ptarget_para_Openmesh_Intermediate);
	
	//Compute the Mapping sequences
	//m_DynamicMappingValue.Format("%d", m_ArrayMeshMappingSequences.size());
	//m_DynamicMappingSlider.SetRange(1, m_ArrayMeshMappingSequences.size());//设置滑动范围为1到20
	m_DynamicMappingValue.Format("%d", 100);
	m_DynamicMappingSlider.SetRange(0, 100);//设置滑动范围为1到20
	m_DynamicMappingSlider.SetTicFreq(1);//每1个单位画一刻度
	m_DynamicMappingSlider.SetPos(1);//设置滑块初始位置为1
	m_DynamicMappingSpin.SetBuddy(GetDlgItem(IDC_SLIDER_DYNAMICQUASICONFORMAL_SLIDER));
	//m_DynamicMappingSpin.SetRange(1, m_ArrayMeshMappingSequences.size());
	UpdateData(FALSE);
	pDoc->UpdateAllViews(NULL);
}

void Actions_DynamicQuasiconformal::SetIntermediateMesh()
{
	int nIndex = m_ActionsCombo.GetCurSel();
	//int pos = m_DynamicMappingSlider.GetPos();
	double nPos1 = m_DynamicMappingSlider.GetPos();
	char message[80];
	sprintf(message, "%d", m_DynamicMappingSlider.GetPos());
	m_DynamicMappingEdit.SetWindowText(message);
	CString strCBText;
	if (nIndex > -1)
	{
		m_ActionsCombo.GetLBText(nIndex, strCBText);
		if (strCBText.Compare("MeshInterpolate") == 0)
		{
			VPROP_VERTEX_UV vprop_vertex_uv;

			//Vertex UV
			if (!ptarget_para_Openmesh_current->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
				ptarget_para_Openmesh_current->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

			double step = (nPos1) / 100.0;
			//
			std::vector<double> uvvalues;
			uvvalues.resize(2);
			OpenMesh::Vec3d targetpoint, initialpoint;
			for (MyMesh::VertexIter v_it = ptarget_para_Openmesh_current->pmesh->vertices_begin(); v_it != ptarget_para_Openmesh_current->pmesh->vertices_end(); ++v_it)
			{
				initialpoint = ptarget_para_Openmesh_current->pmesh->point(v_it.handle());
				uvvalues = ptarget_para_Openmesh_current->pmesh->property(vprop_vertex_uv, v_it.handle());
				targetpoint[0] = initialpoint[0] * (1 - step) + uvvalues[0] * step;
				targetpoint[1] = initialpoint[1] * (1 - step) + uvvalues[1] * step;
				targetpoint[2] = 0;
				ptarget_para_Openmesh_Intermediate->pmesh->set_point(v_it.handle(), targetpoint);
			}
		}
	}
}
void Actions_DynamicQuasiconformal::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	//MeshMapping* pMeshMapping = m_ArrayMeshMappingSequences[nPos];
	m_ShowMeshMapping = 1;
	//m_CurrentMeshMapping = nPos;

	//
	SetIntermediateMesh();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	pDoc->UpdateAllViews(NULL);
}


BOOL Actions_DynamicQuasiconformal::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	m_ActionsCombo.AddString("-------------");
	m_ActionsCombo.AddString("DragTargetPara");
	m_ActionsCombo.AddString("IntialDelaunaySwap");
	m_ActionsCombo.AddString("MeshInterpolate");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Actions_DynamicQuasiconformal::OnDeltaposDynamicquasiconformalSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	CString ss;
	int a = m_DynamicMappingSlider.GetPos();
	if (pNMUpDown->iDelta < 0) // 如果此值为负数 , 说明点击了Spin的往下的箭头
	{
		a--;
	}
	else if (pNMUpDown->iDelta > 0) // 如果此值为正数, 说明点击了Spin的往上的箭头
	{
		a++;
	}
	ss.Format(_T("%d"), a);
	SetDlgItemText(IDC_DYNAMICQUASI_EDIT, ss);
	m_DynamicMappingSlider.SetPos(a);
	m_ShowMeshMapping = 1;

	//
	SetIntermediateMesh();
	*pResult = 0;
	pDoc->UpdateAllViews(NULL);
}


void Actions_DynamicQuasiconformal::OnBnClickedDynamicquasiShowinitialconvexpara()
{
	// TODO: Add your control notification handler code here
	pDoc->UpdateAllViews(NULL);
}


void Actions_DynamicQuasiconformal::OnBnClickedDynamicquasiShowinitialtargetpara()
{
	// TODO: Add your control notification handler code here
	pDoc->UpdateAllViews(NULL);
}


void Actions_DynamicQuasiconformal::OnBnClickedDynamicquasiShowintermediatetargetpara()
{
	// TODO: Add your control notification handler code here
	pDoc->UpdateAllViews(NULL);
}


void Actions_DynamicQuasiconformal::OnBnClickedDynamicOptimalharmonic()
{
	// TODO: Add your control notification handler code here
	//Load the original surface
	OnBnClickedDynamicQuasiLoad3dsurface();

	OnBnClickedDynamicSettargetboundary();

	//Load the target surface
	OnBnClickedDynamicLoadtargetparamesh();
	//
	//pDoc->Read_Mesh_OpenMesh("D:\\papers\\code\\data\\dynamicquasiconformal\\example1\\3Dface_targetpara.obj", m_SceneGraph1);

	//
	//ptarget_para_Openmesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1)
}


void Actions_DynamicQuasiconformal::OnBnClickedDialogDynamicquasiTargetparaStatic()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Actions_DynamicQuasiconformal::OnNMThemeChangedScrollbar1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void Actions_DynamicQuasiconformal::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Actions_DynamicQuasiconformal::OnEnChangeDynamicquasiEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
