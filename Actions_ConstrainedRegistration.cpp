// Actions_ConstrainedRegistration.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Actions_ConstrainedRegistration.h"
#include "afxdialogex.h"
#include "lib3d/Base3d.h"
#include "Lib3D/Mesh3d_OpenMesh.h"
#include "Action_PolyOmino.h"
#include "FormView3.h"
#include "MyTreeView.h"
#include "Actions_HarmonicMapping.h"
#include <atlimage.h>
#include "Actions_DynamicQuasiconformal.h"
// Actions_ConstrainedRegistration dialog

IMPLEMENT_DYNAMIC(Actions_ConstrainedRegistration, CPropertyPage)
Actions_ConstrainedRegistration* pConstrainedRegistration;
Actions_ConstrainedRegistration::Actions_ConstrainedRegistration()
	: CPropertyPage(IDD_DIALOG_ConstrainedRegistration)
{
	pMesh = pMesh_UV = NULL;
	pConstrainedRegistration = this;
}

Actions_ConstrainedRegistration::~Actions_ConstrainedRegistration()
{
}

void Actions_ConstrainedRegistration::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Actions_ConstrainedRegistration, CPropertyPage)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadFace, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadface)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConstrainedHarmonic, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonic)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadFace2, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadface2)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadSophia, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophia)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadSophia2, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophia2)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ChangeLandmarkIndex, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationChangelandmarkindex)
	ON_BN_CLICKED(IDC_BUTTON_SetMeshUV_Mesh, &Actions_ConstrainedRegistration::OnBnClickedButtonSetmeshuvMesh)
	ON_BN_CLICKED(IDC_BUTTON_SetMeshUV_UV, &Actions_ConstrainedRegistration::OnBnClickedButtonSetmeshuvUv)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_SetMeshUV, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetmeshuv)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadSophia_Holomorphic, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiaHolomorphic)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalParameterization_Quad, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalparameterizationQuad)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_CopyVertexColor, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyvertexcolor)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalParameterization_Circle, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalparameterizationCircle)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircle)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadBrain, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadbrain)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_Brain_ConformalParameterization_Circle, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationBrainConformalparameterizationCircle)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ExtractBoundaryEdges, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationExtractboundaryedges)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConstrainedHarmonic_Sophia, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonicSophia)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadSophiaStraightening, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiastraightening)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadSophia_SearchStep, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiaSearchstep)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_CopyVertexColor_DifferentTopology, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyvertexcolorDifferenttopology)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadSophia_InitialRegistration_Harmonic, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiaInitialregistrationHarmonic)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_CopyUV_DifferentTopology, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyuvDifferenttopology)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_SetReferenceMesh, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetreferencemesh)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadBrain2, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadbrain2)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_CopyUV_DifferentTopology2, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyuvDifferenttopology2)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_TargetVertexColor_MU, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationTargetvertexcolorMu)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadSophia_ConformalCircle, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiaConformalcircle)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_MapRectangleToCircle, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationMaprectangletocircle)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalParameterization_Circle_Holomorphic, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalparameterizationCircleHolomorphic)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Holomorphic, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleHolomorphic)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadExample2_Budata, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadExample2_Holomorphic, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Holomorphic)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Rectangle_Holomorphic_Budata, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningRectangleHolomorphicBudata)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadExample2_Straightening, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Straightening)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConstrainedHarmonic_Budata, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonicBudata)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConstrainedHarmonic_Budata_BijectiveRegistration, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonicBudataBijectiveregistration)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadExample2_Registration_Flip, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2RegistrationFlip)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Budata_AN, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataAn)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Budata_DI, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataDi)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Budata_FE, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataFe)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Budata_HA, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataHa)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Budata_NE, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataNe)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Budata_SA, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataSa)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Budata_SU, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataSu)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadExample2_Budata2, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata2)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadFeaturePoints, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadfeaturepoints)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadFeaturePoints_Landmark, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadfeaturepointsLandmark)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadExample2_Budata3, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata3)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Budata_AN2, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataAn2)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Budata_AN3, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataAn3)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConstrainedHarmonic_Sophia2, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonicSophia2)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadExample2_Budata4, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata4)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_LoadExample2_Budata5, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata5)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_ConformalStraightening_Circle_Brain_, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBrain)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_MeshNormalization, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationMeshnormalization)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_DeteleteFeaturePoints, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationDeteletefeaturepoints)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_Brain_ConformalParameterization_Circle2, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationBrainConformalparameterizationCircle2)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_1627brain_strainghtening, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistration1627brainstrainghtening)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_CopyImagetoSourceMesh, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyimagetosourcemesh)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_SetNormalAsColor, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetnormalascolor)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_SetCurvaturelAsColor, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetcurvaturelascolor)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_SetGaussCurvaturelAsColor, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetgausscurvaturelascolor)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_CopyImagetoMesh, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyimagetomesh)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_SmoothGaussCurvature, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSmoothgausscurvature)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_DetectTeetchLine, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationDetectteetchline)
	ON_BN_CLICKED(IDC_ConstrainedRegistration_SharpenGaussCurvature, &Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSharpengausscurvature)
END_MESSAGE_MAP()


// Actions_ConstrainedRegistration message handlers

extern Action_PolyOmino* pPolyOmino;
extern CMyTreeView* pTreeView;
extern Actions_DynamicQuasiconformal* pDynamicQuasiconformal;
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadface()
{
	// TODO: Add your control notification handler code here
	//D:\papers\weidoc\Surface Mapper\sophie_different_expressions\Sophie.remesh.m
	//D : \papers\weidoc\Surface Mapper\sophie_different_expressions\Sophie_smile.remesh.m
	//D:\papers\2013\WeiCVPR\Fig.6 - BUFace\results
	//D:\papers\code\data\CVPR2017\results
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\results\\F0002_NE00BL_F3D_0_me.m");
	p_FormView3->currentindex = 0;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\results\\F0002_NE00BL_F3D_0_me.txt");

	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7435);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7283);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7289);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7673);

	//set uv
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\results\\F0006_NE00WH_F3D_0_me.m");
	p_FormView3->currentindex = 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\results\\F0006_NE00WH_F3D_0_me.txt");
	//pPolyOmino->LoadSelectedPoints("D:\\papers\\weidoc\\Surface Mapper\\sophie_different_expressions\\sophie.remesh.markers.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(6722);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(6824);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(6886);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(6683);
	//set uv
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	//pDoc->OnOpenDocument("D:\\papers\\weidoc\\Surface Mapper\\sophie_different_expressions\\Sophie_smile.remesh.m");
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

extern Actions_HarmonicMapping* pHarmonicMapping;
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonic()
{
	// TODO: Add your control notification handler code here
	//First map the two models with constraints to the plane
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	CMesh3d* pMesh3 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(2);
	CMesh3d* pMesh4 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(3);
	CMesh3d_OpenMesh* pOpenMesh1 = pMesh1->Convert_OpenMesh();
	pOpenMesh1->m_LandMarkArray = pMesh1->m_LandMarkArray;
	CMesh3d_OpenMesh* pOpenMesh2 = pMesh2->Convert_OpenMesh();
	pOpenMesh2->m_LandMarkArray = pMesh2->m_LandMarkArray;

	//
	pOpenMesh1->pBoundaryMeshReference = pMesh3->Convert_OpenMesh();
	pOpenMesh2->pBoundaryMeshReference = pMesh4->Convert_OpenMesh();

	//First Map The meshes to 2D
	MapTo2D(pOpenMesh1, pOpenMesh2);

	//Read the 2D mapping Result

	CMesh3d_OpenMesh* pOpenMesh3 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(2);
	CMesh3d_OpenMesh* pOpenMesh4 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(3);

	//Copy FixArray
	pOpenMesh3->m_FixVerticesArray = pOpenMesh1->m_FixVerticesArray;
	pOpenMesh4->m_FixVerticesArray = pOpenMesh2->m_FixVerticesArray;
	pOpenMesh3->m_FixVerticesArray_uv = pOpenMesh1->m_FixVerticesArray_uv;
	pOpenMesh4->m_FixVerticesArray_uv = pOpenMesh2->m_FixVerticesArray_uv;

	//copy
	pOpenMesh3 = pOpenMesh3->copy();
	pOpenMesh4 = pOpenMesh4->copy();

	//Compute bijective mapping between pOpenmesh3 and pOpenmesh4
	Bijective2DMapping(pOpenMesh3, pOpenMesh4);
}
void Actions_ConstrainedRegistration::Bijective2DMapping_Initialize(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2)
{
	//Set the uv of the end points of the landmark
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pOpenMesh1->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenMesh1->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

std::vector<double> uv1, uv2;

//pOpenMesh1->m_FixVerticesArray.clear();
//Step 1: Compute a constrained harmonic mapping between the two meshes
//four boundary vertices
//pOpenMesh1->m_FixVerticesArray.push_back(7435);
uv1.clear();
uv1.push_back(pOpenMesh1->m_FixVerticesArray_uv[0]);
uv1.push_back(pOpenMesh1->m_FixVerticesArray_uv[1]);
//pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(7435)) = uv1;
pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(pOpenMesh1->m_FixVerticesArray[0])) = uv1;

//pOpenMesh1->m_FixVerticesArray.push_back(7283);
uv1.clear();
uv1.push_back(pOpenMesh1->m_FixVerticesArray_uv[2]);
uv1.push_back(pOpenMesh1->m_FixVerticesArray_uv[3]);
//pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(7283)) = uv1;
pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(pOpenMesh1->m_FixVerticesArray[1])) = uv1;

//pOpenMesh1->m_FixVerticesArray.push_back(7289);
uv1.clear();
uv1.push_back(pOpenMesh1->m_FixVerticesArray_uv[4]);
uv1.push_back(pOpenMesh1->m_FixVerticesArray_uv[5]);
//pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(7289)) = uv1;
pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(pOpenMesh1->m_FixVerticesArray[2])) = uv1;
//pOpenMesh1->m_FixVerticesArray.push_back(7673);
uv1.clear();
uv1.push_back(pOpenMesh1->m_FixVerticesArray_uv[6]);
uv1.push_back(pOpenMesh1->m_FixVerticesArray_uv[7]);
//pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(7673)) = uv1;
pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(pOpenMesh1->m_FixVerticesArray[3])) = uv1;
//initialize the selected flag
for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
{
	pOpenMesh1->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
}

for (int i = 0; i < pOpenMesh1->m_LandMarkArray.size(); i++)
{
	GeoMapLandMark* pLandmark1 = pOpenMesh1->m_LandMarkArray[i];
	int start1 = pLandmark1->landmark_points[0];
	int end1 = pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1];

	GeoMapLandMark* pLandmark2 = pOpenMesh2->m_LandMarkArray[i];
	int start2 = pLandmark2->landmark_points[0];
	int end2 = pLandmark2->landmark_points[pLandmark2->landmark_points.size() - 1];

	//landmark not consistent!!!! 
	//one landmark start is the same as the end of the corresponding landmark!
	OpenMesh::Vec3d startpoint = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(start2));
	OpenMesh::Vec3d endpoint = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(end2));

	uv1.clear();
	uv2.clear();
	uv1.push_back(startpoint[0]);
	uv1.push_back(startpoint[1]);
	uv2.push_back(endpoint[0]);
	uv2.push_back(endpoint[1]);

	//
	pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(start1)) = uv1;
	pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(end1)) = uv2;

	//fix
	pOpenMesh1->pmesh->property(vprop_selectedflag, pOpenMesh1->pmesh->vertex_handle(start1)) = 1;
	pOpenMesh1->pmesh->property(vprop_selectedflag, pOpenMesh1->pmesh->vertex_handle(end1)) = 1;

	int startflag1 = pOpenMesh1->pmesh->is_boundary(pOpenMesh1->pmesh->vertex_handle(start1));
	if (startflag1)
	{
		if (std::find(pOpenMesh1->m_FixVerticesArray.begin(), pOpenMesh1->m_FixVerticesArray.end(), start1) == pOpenMesh1->m_FixVerticesArray.end())
			pOpenMesh1->m_FixVerticesArray.push_back(start1);
	}
	startflag1 = pOpenMesh1->pmesh->is_boundary(pOpenMesh1->pmesh->vertex_handle(end1));
	if (startflag1)
	{
		//pOpenMesh1->m_FixVerticesArray.push_back(end1);
		if (std::find(pOpenMesh1->m_FixVerticesArray.begin(), pOpenMesh1->m_FixVerticesArray.end(), end1) == pOpenMesh1->m_FixVerticesArray.end())
			pOpenMesh1->m_FixVerticesArray.push_back(end1);
	}
}

//initialzie arrayconstraints
/*pOpenMesh1->m_ArrayConstraints.clear();
std::pair<int, std::vector<double>> pair;
for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
{
	int boundaryflag = pOpenMesh1->pmesh->is_boundary(v_it.handle());
	if (boundaryflag)
		continue;
	int flag = pOpenMesh1->pmesh->property(vprop_selectedflag, v_it.handle());
	if (flag)
	{
		std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle());
		pair.first = v_it.handle().idx();
		pair.second = uv;
		pOpenMesh1->m_ArrayConstraints.push_back(pair);
	}
}*/
}
void Actions_ConstrainedRegistration::GetVerticesbetwwenTwoPoints(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2, std::vector<MyMesh::VertexHandle> &m_ArrarBoundaryVertices, MyMesh::VertexHandle m_startVertexHandle, MyMesh::VertexHandle m_endVertexHandle)
{
	m_ArrarBoundaryVertices.clear();
	MyMesh::HalfedgeHandle he1 = pOpenMesh1->pmesh->halfedge_handle(m_startVertexHandle);
	MyMesh::HalfedgeHandle he2;
	m_ArrarBoundaryVertices.push_back(m_startVertexHandle);
	MyMesh::VertexHandle prevertexhandle = m_startVertexHandle;
	MyMesh::VertexHandle nextvertexhandle;
	while (!pOpenMesh1->pmesh->is_boundary(he1))
	{
		he1 = pOpenMesh1->pmesh->prev_halfedge_handle(he1);
		he1 = pOpenMesh1->pmesh->opposite_halfedge_handle(he1);
	}
	he1 = pOpenMesh1->pmesh->prev_halfedge_handle(he1);
	while (1)
	{
		MyMesh::VertexHandle vh1 = pOpenMesh1->pmesh->from_vertex_handle(he1);
		MyMesh::VertexHandle vh2 = pOpenMesh1->pmesh->to_vertex_handle(he1);
		if (vh1 == prevertexhandle)
		{
			nextvertexhandle = vh2;
		}
		else
		{
			nextvertexhandle = vh1;
		}
		if (nextvertexhandle == m_endVertexHandle)
		{
			m_ArrarBoundaryVertices.push_back(nextvertexhandle);
			break;
		}

		m_ArrarBoundaryVertices.push_back(nextvertexhandle);
		prevertexhandle = nextvertexhandle;
		he1 = pOpenMesh1->pmesh->prev_halfedge_handle(he1);
	}
}
void Actions_ConstrainedRegistration::SetPointsUV(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2, std::vector<MyMesh::VertexHandle> &m_ArrarBoundaryVertices)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pOpenMesh1->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenMesh1->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);
	double toltallength = 0;
	for (int i = 0; i < m_ArrarBoundaryVertices.size() - 1; i++)
	{
		int startvertex = i;
		int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		OpenMesh::Vec3d startpt, endpt;
		startpt = pOpenMesh1->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
		endpt = pOpenMesh1->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
		OpenMesh::Vec3d edge = startpt - endpt;
		double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
		toltallength += edgelength;
	}

	double startangle, endangle;
	if (m_ArrarBoundaryVertices[0].idx() == pOpenMesh1->m_FixVerticesArray[0])
	{
		startangle = acos(pOpenMesh2->m_FixVerticesArray_uv[0]);
		if (pOpenMesh2->m_FixVerticesArray_uv[1] < 0)
		{
			startangle = PI * 2 - startangle;
		}
		endangle = acos(pOpenMesh2->m_FixVerticesArray_uv[2]);
		if (pOpenMesh2->m_FixVerticesArray_uv[3] < 0)
		{
			endangle = PI * 2 - endangle;
		}
		if (endangle < startangle)
			endangle += PI * 2;
	}
	else
	{
		startangle = acos(pOpenMesh2->m_FixVerticesArray_uv[2]);
		if (pOpenMesh2->m_FixVerticesArray_uv[3] < 0)
		{
			startangle = PI * 2 - startangle;
		}
		endangle = acos(pOpenMesh2->m_FixVerticesArray_uv[0]);
		if (pOpenMesh2->m_FixVerticesArray_uv[1] < 0)
		{
			endangle = PI * 2 - endangle;
		}
		if (endangle < startangle)
			endangle += PI * 2;
	}


	double accummlateedgelength = 0;
	double r;
	std::vector<double> uv;
	for (int i = 0; i < m_ArrarBoundaryVertices.size() - 1; i++)
	{
		if (i > 0)
		{
			int startvertex = i;
			int endvertex = i - 1;
			OpenMesh::Vec3d startpt, endpt;
			startpt = pOpenMesh1->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
			endpt = pOpenMesh1->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
			OpenMesh::Vec3d edge = startpt - endpt;
			double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
			accummlateedgelength += edgelength;
		}

		r = accummlateedgelength * (endangle - startangle) / toltallength + startangle;
		uv.clear();
		uv.push_back(cos(r));
		uv.push_back(sin(r));
		pOpenMesh1->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = uv;
	}
}

void Actions_ConstrainedRegistration::Bijective2DMapping_Initialize_Budata(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2)
{
	//Set the uv of the end points of the landmark
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pOpenMesh1->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenMesh1->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uv1, uv2;

	pOpenMesh1->m_FixVerticesArray.clear();
	pOpenMesh1->m_FixVerticesArray_uv.clear();
	pOpenMesh2->m_FixVerticesArray.clear();
	pOpenMesh2->m_FixVerticesArray_uv.clear();
	//Sophie two end points on the circles
	for (int i = 0; i < pOpenMesh1->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark1 = pOpenMesh1->m_LandMarkArray[i];
		GeoMapLandMark* pLandmark2 = pOpenMesh2->m_LandMarkArray[i];
		int start1 = pLandmark1->landmark_points[0];
		int end1 = pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1];
		int start2 = pLandmark2->landmark_points[0];
		int end2 = pLandmark2->landmark_points[pLandmark2->landmark_points.size() - 1];

		if (pOpenMesh1->pmesh->is_boundary(pOpenMesh1->pmesh->vertex_handle(start1)))
		{
			pOpenMesh1->m_FixVerticesArray.push_back(start1);
			OpenMesh::Vec3d point = pOpenMesh1->pmesh->point(pOpenMesh1->pmesh->vertex_handle(start1));
			pOpenMesh1->m_FixVerticesArray_uv.push_back(point[0]);
			pOpenMesh1->m_FixVerticesArray_uv.push_back(point[1]);
			pOpenMesh2->m_FixVerticesArray.push_back(start2);
			point = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(start2));
			pOpenMesh2->m_FixVerticesArray_uv.push_back(point[0]);
			pOpenMesh2->m_FixVerticesArray_uv.push_back(point[1]);
		}
		if (pOpenMesh1->pmesh->is_boundary(pOpenMesh1->pmesh->vertex_handle(end1)))
		{
			pOpenMesh1->m_FixVerticesArray.push_back(end1);
			OpenMesh::Vec3d point = pOpenMesh1->pmesh->point(pOpenMesh1->pmesh->vertex_handle(end1));
			pOpenMesh1->m_FixVerticesArray_uv.push_back(point[0]);
			pOpenMesh1->m_FixVerticesArray_uv.push_back(point[1]);
			pOpenMesh2->m_FixVerticesArray.push_back(end2);
			point = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(end2));
			pOpenMesh2->m_FixVerticesArray_uv.push_back(point[0]);
			pOpenMesh2->m_FixVerticesArray_uv.push_back(point[1]);
		}
	}
	//uv1.clear();
	//uv1.push_back(pOpenMesh2->m_FixVerticesArray_uv[0]);
	//uv1.push_back(pOpenMesh2->m_FixVerticesArray_uv[1]);
	//pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(pOpenMesh1->m_FixVerticesArray[0])) = uv1;

	//uv1.clear();
	//uv1.push_back(pOpenMesh2->m_FixVerticesArray_uv[2]);
	//uv1.push_back(pOpenMesh2->m_FixVerticesArray_uv[3]);
	//pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(pOpenMesh1->m_FixVerticesArray[1])) = uv1;

	//initialize the selected flag
	for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
	{
		pOpenMesh1->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}

	//First fix the boundaries by mapping the two vertices and interpolating the middle points.
	std::vector<MyMesh::VertexHandle> m_ArrarBoundaryVertices;
	MyMesh::VertexHandle m_startVertexHandle, m_endVertexHandle;
	m_startVertexHandle = MyMesh::VertexHandle(pOpenMesh1->m_FixVerticesArray[0]);
	m_endVertexHandle = MyMesh::VertexHandle(pOpenMesh1->m_FixVerticesArray[1]);

	//store all the vertices
	GetVerticesbetwwenTwoPoints(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices, m_startVertexHandle, m_endVertexHandle);

	//Set the uv between the two points
	SetPointsUV(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices);

	//store all the vertices
	GetVerticesbetwwenTwoPoints(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices, m_endVertexHandle, m_startVertexHandle);

	//Set the uv between the two points
	SetPointsUV(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices);

	//Set uv of landmark endpoints
	for (int i = 0; i < pOpenMesh1->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark1 = pOpenMesh1->m_LandMarkArray[i];
		int start1 = pLandmark1->landmark_points[0];
		int end1 = pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1];

		GeoMapLandMark* pLandmark2 = pOpenMesh2->m_LandMarkArray[i];
		int start2 = pLandmark2->landmark_points[0];
		int end2 = pLandmark2->landmark_points[pLandmark2->landmark_points.size() - 1];

		//landmark not consistent!!!! 
		//one landmark start is the same as the end of the corresponding landmark!
		OpenMesh::Vec3d startpoint = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(start2));
		OpenMesh::Vec3d endpoint = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(end2));

		uv1.clear();
		uv2.clear();
		uv1.push_back(startpoint[0]);
		uv1.push_back(startpoint[1]);
		uv2.push_back(endpoint[0]);
		uv2.push_back(endpoint[1]);

		//
		pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(start1)) = uv1;
		pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(end1)) = uv2;

		//fix
		pOpenMesh1->pmesh->property(vprop_selectedflag, pOpenMesh1->pmesh->vertex_handle(start1)) = 1;
		pOpenMesh1->pmesh->property(vprop_selectedflag, pOpenMesh1->pmesh->vertex_handle(end1)) = 1;
	}

	//initialzie arrayconstraints
	/*pOpenMesh1->m_ArrayConstraints.clear();
	std::pair<int, std::vector<double>> pair;
	for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
	{
	int boundaryflag = pOpenMesh1->pmesh->is_boundary(v_it.handle());
	if (boundaryflag)
	continue;
	int flag = pOpenMesh1->pmesh->property(vprop_selectedflag, v_it.handle());
	if (flag)
	{
	std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle());
	pair.first = v_it.handle().idx();
	pair.second = uv;
	pOpenMesh1->m_ArrayConstraints.push_back(pair);
	}
	}*/
}
void Actions_ConstrainedRegistration::Bijective2DMapping_Initialize_Budata1(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2)
{
	//Set the uv of the end points of the landmark
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pOpenMesh1->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenMesh1->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uv1, uv2;

	//initialize the selected flag
	for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
	{
		pOpenMesh1->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}

	//First fix the boundaries by mapping the two vertices and interpolating the middle points.
	std::vector<MyMesh::VertexHandle> m_ArrarBoundaryVertices;
	MyMesh::VertexHandle m_startVertexHandle, m_endVertexHandle;
	m_startVertexHandle = MyMesh::VertexHandle(pOpenMesh1->m_FixVerticesArray[0]);
	m_endVertexHandle = MyMesh::VertexHandle(pOpenMesh1->m_FixVerticesArray[0]);

	//store all the vertices
	GetVerticesbetwwenTwoPoints(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices, m_startVertexHandle, m_endVertexHandle);

	//Set the uv between the two points
	SetPointsUV(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices);

	//store all the vertices
	//GetVerticesbetwwenTwoPoints(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices, m_endVertexHandle, m_startVertexHandle);

	//Set the uv between the two points
	//SetPointsUV(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices);

	//Set uv of landmark endpoints
	for (int i = 0; i < pOpenMesh1->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark1 = pOpenMesh1->m_LandMarkArray[i];
		int start1 = pLandmark1->landmark_points[0];
		int end1 = pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1];

		GeoMapLandMark* pLandmark2 = pOpenMesh2->m_LandMarkArray[i];
		int start2 = pLandmark2->landmark_points[0];
		int end2 = pLandmark2->landmark_points[pLandmark2->landmark_points.size() - 1];

		//landmark not consistent!!!! 
		//one landmark start is the same as the end of the corresponding landmark!
		OpenMesh::Vec3d startpoint = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(start2));
		OpenMesh::Vec3d endpoint = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(end2));

		uv1.clear();
		uv2.clear();
		uv1.push_back(startpoint[0]);
		uv1.push_back(startpoint[1]);
		uv2.push_back(endpoint[0]);
		uv2.push_back(endpoint[1]);

		//
		pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(start1)) = uv1;
		pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(end1)) = uv2;

		//fix
		pOpenMesh1->pmesh->property(vprop_selectedflag, pOpenMesh1->pmesh->vertex_handle(start1)) = 1;
		pOpenMesh1->pmesh->property(vprop_selectedflag, pOpenMesh1->pmesh->vertex_handle(end1)) = 1;
	}
}
void Actions_ConstrainedRegistration::Bijective2DMapping_Initialize_Sophia(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2)
{
	//Set the uv of the end points of the landmark
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pOpenMesh1->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenMesh1->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uv1, uv2;

	pOpenMesh1->m_FixVerticesArray.clear();
	pOpenMesh1->m_FixVerticesArray_uv.clear();
	pOpenMesh2->m_FixVerticesArray.clear();
	pOpenMesh2->m_FixVerticesArray_uv.clear();
	//Sophie two end points on the circles
	for (int i = 0; i < pOpenMesh1->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark1 = pOpenMesh1->m_LandMarkArray[i];
		GeoMapLandMark* pLandmark2 = pOpenMesh2->m_LandMarkArray[i];
		int start1 = pLandmark1->landmark_points[0];
		int end1 = pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1];
		int start2 = pLandmark2->landmark_points[0];
		int end2 = pLandmark2->landmark_points[pLandmark2->landmark_points.size() - 1];

		if (pOpenMesh1->pmesh->is_boundary(pOpenMesh1->pmesh->vertex_handle(start1)))
		{
			pOpenMesh1->m_FixVerticesArray.push_back(start1);
			OpenMesh::Vec3d point = pOpenMesh1->pmesh->point(pOpenMesh1->pmesh->vertex_handle(start1));
			pOpenMesh1->m_FixVerticesArray_uv.push_back(point[0]);
			pOpenMesh1->m_FixVerticesArray_uv.push_back(point[1]);
			pOpenMesh2->m_FixVerticesArray.push_back(start2);
			point = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(start2));
			pOpenMesh2->m_FixVerticesArray_uv.push_back(point[0]);
			pOpenMesh2->m_FixVerticesArray_uv.push_back(point[1]);
		}
		if (pOpenMesh1->pmesh->is_boundary(pOpenMesh1->pmesh->vertex_handle(end1)))
		{
			pOpenMesh1->m_FixVerticesArray.push_back(end1);
			OpenMesh::Vec3d point = pOpenMesh1->pmesh->point(pOpenMesh1->pmesh->vertex_handle(end1));
			pOpenMesh1->m_FixVerticesArray_uv.push_back(point[0]);
			pOpenMesh1->m_FixVerticesArray_uv.push_back(point[1]);
			pOpenMesh2->m_FixVerticesArray.push_back(end2);
			point = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(end2));
			pOpenMesh2->m_FixVerticesArray_uv.push_back(point[0]);
			pOpenMesh2->m_FixVerticesArray_uv.push_back(point[1]);
		}
	}
	uv1.clear();
	uv1.push_back(pOpenMesh2->m_FixVerticesArray_uv[0]);
	uv1.push_back(pOpenMesh2->m_FixVerticesArray_uv[1]);
	//pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(7435)) = uv1;
	pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(pOpenMesh1->m_FixVerticesArray[0])) = uv1;

	//pOpenMesh1->m_FixVerticesArray.push_back(7283);
	uv1.clear();
	uv1.push_back(pOpenMesh2->m_FixVerticesArray_uv[2]);
	uv1.push_back(pOpenMesh2->m_FixVerticesArray_uv[3]);
	//pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(7283)) = uv1;
	pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(pOpenMesh1->m_FixVerticesArray[1])) = uv1;

	//initialize the selected flag
	for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
	{
		pOpenMesh1->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}

	//First fix the boundaries by mapping the two vertices and interpolating the middle points.
	std::vector<MyMesh::VertexHandle> m_ArrarBoundaryVertices;
	MyMesh::VertexHandle m_startVertexHandle, m_endVertexHandle;
	m_startVertexHandle = MyMesh::VertexHandle(pOpenMesh1->m_FixVerticesArray[0]);
	m_endVertexHandle = MyMesh::VertexHandle(pOpenMesh1->m_FixVerticesArray[1]);

	//store all the vertices
	GetVerticesbetwwenTwoPoints(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices,m_startVertexHandle, m_endVertexHandle);

	//Set the uv between the two points
	SetPointsUV(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices);

	//store all the vertices
	GetVerticesbetwwenTwoPoints(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices, m_endVertexHandle, m_startVertexHandle);

	//Set the uv between the two points
	SetPointsUV(pOpenMesh1, pOpenMesh2, m_ArrarBoundaryVertices);

	//Set uv of landmark endpoints
	for (int i = 0; i < pOpenMesh1->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark1 = pOpenMesh1->m_LandMarkArray[i];
		int start1 = pLandmark1->landmark_points[0];
		int end1 = pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1];

		GeoMapLandMark* pLandmark2 = pOpenMesh2->m_LandMarkArray[i];
		int start2 = pLandmark2->landmark_points[0];
		int end2 = pLandmark2->landmark_points[pLandmark2->landmark_points.size() - 1];

		//landmark not consistent!!!! 
		//one landmark start is the same as the end of the corresponding landmark!
		OpenMesh::Vec3d startpoint = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(start2));
		OpenMesh::Vec3d endpoint = pOpenMesh2->pmesh->point(pOpenMesh2->pmesh->vertex_handle(end2));

		uv1.clear();
		uv2.clear();
		uv1.push_back(startpoint[0]);
		uv1.push_back(startpoint[1]);
		uv2.push_back(endpoint[0]);
		uv2.push_back(endpoint[1]);

		//
		pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(start1)) = uv1;
		pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(end1)) = uv2;

		//fix
		pOpenMesh1->pmesh->property(vprop_selectedflag, pOpenMesh1->pmesh->vertex_handle(start1)) = 1;
		pOpenMesh1->pmesh->property(vprop_selectedflag, pOpenMesh1->pmesh->vertex_handle(end1)) = 1;
	}

	//initialzie arrayconstraints
	/*pOpenMesh1->m_ArrayConstraints.clear();
	std::pair<int, std::vector<double>> pair;
	for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
	{
	int boundaryflag = pOpenMesh1->pmesh->is_boundary(v_it.handle());
	if (boundaryflag)
	continue;
	int flag = pOpenMesh1->pmesh->property(vprop_selectedflag, v_it.handle());
	if (flag)
	{
	std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle());
	pair.first = v_it.handle().idx();
	pair.second = uv;
	pOpenMesh1->m_ArrayConstraints.push_back(pair);
	}
	}*/
}
void Actions_ConstrainedRegistration::Bijective2DMapping(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2)
{
	int flag;
	int iternum = 0;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenMesh1->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	char name[80];
	while (1)
	{
		//std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(0));
		//int isonboundary = pOpenMesh1->pmesh->is_boundary(pOpenMesh1->pmesh->vertex_handle(0));

		//
		Bijective2DMapping_Initialize(pOpenMesh1, pOpenMesh2);

		//uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(0));

		//Compute the constrained harmonic with fixing the landmark endpoints
		pHarmonicMapping->TwoDHarmonicStraightening_FixLandmarkEndPoints(pOpenMesh1);

		//uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(0));
		sprintf(name, "D:\\%d_initial2Dharmonic.obj",iternum);
		//Generate Parameter Domain
		pHarmonicMapping->GenerateParameterDomain(pOpenMesh1);
		pOpenMesh1->SaveOBJParameterDomainFile(name);

		//Judge whether the result has flips
		flag = pHarmonicMapping->IsMeshUVFlip(pOpenMesh1, iternum);

		if (!flag)
		{
			//Compute the CDT
			//pHarmonicMapping->CDT_New(pOpenMesh1);
			//
			for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
			{
				std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle());
				OpenMesh::Vec3d pos = pOpenMesh1->pmesh->point(v_it.handle());

				//swap the face position and mu vector
				double tempx = pos[0];
				double tempy = pos[1];
				pos[0] = uv[0];
				pos[1] = uv[1];
				pOpenMesh1->pmesh->set_point(v_it.handle(), pos);

				uv[0] = tempx;
				uv[1] = tempy;
				pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle()) = uv;
			}
			pHarmonicMapping->GenerateDuplicateMesh_LinearConstaints(pOpenMesh1, "CDT1");

			sprintf(name, "D:\\%d_finalharmonic.obj",iternum);
			pOpenMesh1->SaveOBJFile(name);

			//CDT_EdgeFlip(pMesh3D,iternum);
			//Constrained Delaunay Triangulation
			pHarmonicMapping->CDT_New(pOpenMesh1);
			//CDT_Direct(pMesh3D);
			pHarmonicMapping->GenerateDuplicateMesh_LinearConstaints(pOpenMesh1, "CDT2");

			sprintf(name, "D:\\%d_finalharmonic_CDT.obj",iternum);
			pOpenMesh1->SaveOBJFile(name);
			break;
		}

		//Move a small step along the target mu
		pOpenMesh1 = pHarmonicMapping->GetMeshfreeofFlip_quasiconformal(pOpenMesh1, iternum);
		iternum++;
	}

	//
	//pHarmonicMapping->GenerateParameterDomain(pOpenMesh1);
}
void Actions_ConstrainedRegistration::Bijective2DMapping_Budata(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2)
{
	int flag;
	int iternum = 0;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenMesh1->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	char name[80];
	while (1)
	{
		//std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(0));
		//int isonboundary = pOpenMesh1->pmesh->is_boundary(pOpenMesh1->pmesh->vertex_handle(0));

		//
		Bijective2DMapping_Initialize_Budata1(pOpenMesh1, pOpenMesh2);

		//uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(0));

		//Compute the constrained harmonic with fixing the landmark endpoints
		pHarmonicMapping->TwoDHarmonicStraightening_FixLandmarkEndPoints_Sophie(pOpenMesh1);

		//uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(0));
		sprintf(name, "D:\\%d_initial2Dharmonic.obj", iternum);
		//Generate Parameter Domain
		pHarmonicMapping->GenerateParameterDomain(pOpenMesh1);
		pOpenMesh1->SaveOBJParameterDomainFile(name);

		//Judge whether the result has flips
		flag = pHarmonicMapping->IsMeshUVFlip(pOpenMesh1, iternum);

		if (!flag)
		{
			//Compute the CDT
			//pHarmonicMapping->CDT_New(pOpenMesh1);
			//
			for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
			{
				std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle());
				OpenMesh::Vec3d pos = pOpenMesh1->pmesh->point(v_it.handle());

				//swap the face position and mu vector
				double tempx = pos[0];
				double tempy = pos[1];
				pos[0] = uv[0];
				pos[1] = uv[1];
				pOpenMesh1->pmesh->set_point(v_it.handle(), pos);

				uv[0] = tempx;
				uv[1] = tempy;
				pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle()) = uv;
			}
			pHarmonicMapping->GenerateDuplicateMesh_LinearConstaints(pOpenMesh1, "CDT1");

			sprintf(name, "D:\\%d_finalharmonic.obj", iternum);
			pOpenMesh1->SaveOBJFile(name);

			//CDT_EdgeFlip(pMesh3D,iternum);
			//Constrained Delaunay Triangulation
			//pHarmonicMapping->CDT_New(pOpenMesh1);
			//CDT_Direct(pMesh3D);
			//pHarmonicMapping->GenerateDuplicateMesh_LinearConstaints(pOpenMesh1, "CDT2");

			//sprintf(name, "D:\\%d_finalharmonic_CDT.obj", iternum);
			//pOpenMesh1->SaveOBJFile(name);
			break;
		}

		//Move a small step along the target mu
		pOpenMesh1 = pHarmonicMapping->GetMeshfreeofFlip_quasiconformal(pOpenMesh1, iternum);
		iternum++;
	}

	//
	//pHarmonicMapping->GenerateParameterDomain(pOpenMesh1);
}
void Actions_ConstrainedRegistration::Bijective2DMapping_Sophia(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2)
{
	int flag;
	int iternum = 0;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenMesh1->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	char name[80];
	while (1)
	{
		//std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(0));
		//int isonboundary = pOpenMesh1->pmesh->is_boundary(pOpenMesh1->pmesh->vertex_handle(0));

		//
		Bijective2DMapping_Initialize_Sophia(pOpenMesh1, pOpenMesh2);

		//uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(0));

		//Compute the constrained harmonic with fixing the landmark endpoints
		pHarmonicMapping->TwoDHarmonicStraightening_FixLandmarkEndPoints_Sophie(pOpenMesh1);

		//uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(0));
		sprintf(name, "D:\\%d_initial2Dharmonic.obj", iternum);
		//Generate Parameter Domain
		pHarmonicMapping->GenerateParameterDomain(pOpenMesh1);
		pOpenMesh1->SaveOBJParameterDomainFile(name);

		//Judge whether the result has flips
		flag = pHarmonicMapping->IsMeshUVFlip(pOpenMesh1, iternum);

		if (!flag)
		{
			//Compute the CDT
			//pHarmonicMapping->CDT_New(pOpenMesh1);
			//
			for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
			{
				std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle());
				OpenMesh::Vec3d pos = pOpenMesh1->pmesh->point(v_it.handle());

				//swap the face position and mu vector
				double tempx = pos[0];
				double tempy = pos[1];
				pos[0] = uv[0];
				pos[1] = uv[1];
				pOpenMesh1->pmesh->set_point(v_it.handle(), pos);

				uv[0] = tempx;
				uv[1] = tempy;
				pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle()) = uv;
			}
			pHarmonicMapping->GenerateDuplicateMesh_LinearConstaints(pOpenMesh1, "CDT1");

			sprintf(name, "D:\\%d_finalharmonic.obj", iternum);
			pOpenMesh1->SaveOBJFile(name);

			//CDT_EdgeFlip(pMesh3D,iternum);
			//Constrained Delaunay Triangulation
			pHarmonicMapping->CDT_New(pOpenMesh1);
			//CDT_Direct(pMesh3D);
			pHarmonicMapping->GenerateDuplicateMesh_LinearConstaints(pOpenMesh1, "CDT2");

			sprintf(name, "D:\\%d_finalharmonic_CDT.obj", iternum);
			pOpenMesh1->SaveOBJFile(name);
			break;
		}

		//Move a small step along the target mu
		pOpenMesh1 = pHarmonicMapping->GetMeshfreeofFlip_quasiconformal(pOpenMesh1, iternum);
		iternum++;
	}

	//
	//pHarmonicMapping->GenerateParameterDomain(pOpenMesh1);
}
void Actions_ConstrainedRegistration::MapTo2D(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2,int boundarytype)
{
	//Boundary type 0: quad, 1:unit circle

	//Convert the sharp edges to landmark
	//Harmonic with Straightening Constraints
	/*pOpenMesh1->m_FixVerticesArray.push_back(7435);
	pOpenMesh1->m_FixVerticesArray.push_back(7283);
	pOpenMesh1->m_FixVerticesArray.push_back(7289);
	pOpenMesh1->m_FixVerticesArray.push_back(7673);

	pOpenMesh2->m_FixVerticesArray.push_back(6722);
	pOpenMesh2->m_FixVerticesArray.push_back(6824);
	pOpenMesh2->m_FixVerticesArray.push_back(6886);
	pOpenMesh2->m_FixVerticesArray.push_back(6683);*/

	//
	if(pOpenMesh1)
		pHarmonicMapping->HarmonicStraightening(pOpenMesh1,boundarytype);

	//Harmonic with Straightening Constraints
	if(pOpenMesh2)
		pHarmonicMapping->HarmonicStraightening(pOpenMesh2,boundarytype);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadface2()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//D:\papers\weidoc\Surface Mapper\sophie_different_expressions\Sophie.remesh.m
	//D : \papers\weidoc\Surface Mapper\sophie_different_expressions\Sophie_smile.remesh.m
	//D:\papers\2013\WeiCVPR\Fig.6 - BUFace\results
	//D:\papers\code\data\CVPR2017\results
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\results\\F0002_NE00BL_F3D_0_me.m");
	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\results\\F0002_NE00BL_F3D_0_me_1.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7435);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7283);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7289);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7673);

	//set uv
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\results\\F0006_NE00WH_F3D_0_me.m");
	p_FormView3->currentindex = 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\results\\F0006_NE00WH_F3D_0_me_2.txt");
	//pPolyOmino->LoadSelectedPoints("D:\\papers\\weidoc\\Surface Mapper\\sophie_different_expressions\\sophie.remesh.markers.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(6722);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(6824);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(6886);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(6683);

	//set uv
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	//pDoc->OnOpenDocument("D:\\papers\\weidoc\\Surface Mapper\\sophie_different_expressions\\Sophie_smile.remesh.m");
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void Actions_ConstrainedRegistration::RotateMesh(CMesh3d* pMesh)
{
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();
		pVertex->Set(y, -x, z);
	}
}
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophia()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie.remesh.rotate7.m");
	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\sophie.remesh.rotate.landmark7.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(140);
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(19333);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(18013);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(12298);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(101);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.28865);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.28865);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	/*pPolyOmino->pMesh->m_FixVerticesArray.push_back(144);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(18017);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(12302);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(105);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(18895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(15756);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(15851);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(1160);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(3258);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(4505);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(9274);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(20335);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(4243);*/
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie_smile.remesh.rotate6.m");
	p_FormView3->currentindex = 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie_smile.remesh.rotate.landmark6.txt");
	//pPolyOmino->LoadSelectedPoints("D:\\papers\\weidoc\\Surface Mapper\\sophie_different_expressions\\sophie.remesh.markers.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7203);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(4645);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(12684);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(251);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.27377);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.27377);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	//RotateMesh(pPolyOmino->pMesh);

	/*pPolyOmino->pMesh->m_FixVerticesArray.push_back(7207);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(4649);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(12688);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(255);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(18250);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(12069);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(10859);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(13017);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(980);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(8680);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(3637);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(19834);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(1455);*/
	//pDoc->OnOpenDocument("D:\\papers\\weidoc\\Surface Mapper\\sophie_different_expressions\\Sophie_smile.remesh.m");
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophia2()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie.remesh.rotate14.m");
	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	/// <zlzhujie>
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\sophie.remesh.rotate.landmark14.txt");
	/// </summary>
	
	//for (int i = 17; i >= 5; i--)
	//	pPolyOmino->pMesh->m_LandMarkArray.erase(pPolyOmino->pMesh->m_LandMarkArray.begin() + i);
	/*for (int i = 0; i < 18; i++)
	{
		GeoMapLandMark* pLandmark = pPolyOmino->pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size() - 1; j++)
		{
			if (pLandmark->landmark_points[j] == pLandmark->landmark_points[j + 1])
			{
				int ij = 0;
			}
		}
	}*/
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(140);
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(19333);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(18013);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(12298);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(101);
	/*pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.28865);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.28865);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);*/
	/*pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.28865);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.28865);*/
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.499649);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.00258);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.500352);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.00377);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.500359);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.772237);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.499645);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.773424);
	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie_smile.remesh.rotate12.m");
	p_FormView3->currentindex = 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie_smile.remesh.rotate.landmark12.txt");
	//pPolyOmino->LoadSelectedPoints("D:\\papers\\weidoc\\Surface Mapper\\sophie_different_expressions\\sophie.remesh.markers.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7203);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(4645);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(12684);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(251);
	/*pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.27377);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.27377);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);*/
	/*pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.27377);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1.27377);*/
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.658971);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.731788);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.341025);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.733554);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.341032);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.05882);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.658968);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.060594);
	//RotateMesh(pPolyOmino->pMesh);

	//pDoc->OnOpenDocument("D:\\papers\\weidoc\\Surface Mapper\\sophie_different_expressions\\Sophie_smile.remesh.m");

	//Load the holomorphic results
	//OnBnClickedConstrainedregistrationLoadsophiaHolomorphic();

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationChangelandmarkindex()
{
	// TODO: Add your control notification handler code here
	CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	/*std::vector<GeoMapLandMark*> m_LandMarkArraybak;
	m_LandMarkArraybak.resize(18);
	m_LandMarkArraybak[0] = pMesh3D->m_LandMarkArray[3];
	m_LandMarkArraybak[1] = pMesh3D->m_LandMarkArray[4];
	m_LandMarkArraybak[2] = pMesh3D->m_LandMarkArray[5];
	m_LandMarkArraybak[3] = pMesh3D->m_LandMarkArray[6];
	m_LandMarkArraybak[4] = pMesh3D->m_LandMarkArray[1];
	m_LandMarkArraybak[5] = pMesh3D->m_LandMarkArray[2];
	m_LandMarkArraybak[6] = pMesh3D->m_LandMarkArray[7];
	m_LandMarkArraybak[7] = pMesh3D->m_LandMarkArray[0];
	m_LandMarkArraybak[8] = pMesh3D->m_LandMarkArray[15];
	m_LandMarkArraybak[10] = pMesh3D->m_LandMarkArray[14];
	m_LandMarkArraybak[9] = pMesh3D->m_LandMarkArray[13];
	m_LandMarkArraybak[11] = pMesh3D->m_LandMarkArray[12];
	m_LandMarkArraybak[12] = pMesh3D->m_LandMarkArray[11];
	m_LandMarkArraybak[13] = pMesh3D->m_LandMarkArray[16];
	m_LandMarkArraybak[14] = pMesh3D->m_LandMarkArray[9];
	m_LandMarkArraybak[15] = pMesh3D->m_LandMarkArray[17];
	m_LandMarkArraybak[16] = pMesh3D->m_LandMarkArray[10];
	m_LandMarkArraybak[17] = pMesh3D->m_LandMarkArray[8];
	pMesh3D->m_LandMarkArray = m_LandMarkArraybak;*/

	/*std::vector<GeoMapLandMark*> m_LandMarkArraybak;
	m_LandMarkArraybak.resize(18);
	m_LandMarkArraybak[0] = pMesh3D->m_LandMarkArray[16];
	m_LandMarkArraybak[1] = pMesh3D->m_LandMarkArray[17];
	m_LandMarkArraybak[2] = pMesh3D->m_LandMarkArray[15];
	m_LandMarkArraybak[3] = pMesh3D->m_LandMarkArray[3];
	m_LandMarkArraybak[4] = pMesh3D->m_LandMarkArray[1];
	m_LandMarkArraybak[5] = pMesh3D->m_LandMarkArray[0];
	m_LandMarkArraybak[6] = pMesh3D->m_LandMarkArray[8];
	m_LandMarkArraybak[7] = pMesh3D->m_LandMarkArray[2];
	m_LandMarkArraybak[8] = pMesh3D->m_LandMarkArray[10];
	m_LandMarkArraybak[10] = pMesh3D->m_LandMarkArray[9];
	m_LandMarkArraybak[9] = pMesh3D->m_LandMarkArray[12];
	m_LandMarkArraybak[11] = pMesh3D->m_LandMarkArray[11];
	m_LandMarkArraybak[12] = pMesh3D->m_LandMarkArray[7];
	m_LandMarkArraybak[13] = pMesh3D->m_LandMarkArray[14];
	m_LandMarkArraybak[14] = pMesh3D->m_LandMarkArray[6];
	m_LandMarkArraybak[15] = pMesh3D->m_LandMarkArray[13];
	m_LandMarkArraybak[17] = pMesh3D->m_LandMarkArray[4];
	m_LandMarkArraybak[16] = pMesh3D->m_LandMarkArray[5];
	pMesh3D->m_LandMarkArray = m_LandMarkArraybak;*/

	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	/*ReverseLandmark(pMesh3D->m_LandMarkArray[4]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[9]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[10]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[11]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[13]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[14]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[15]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[16]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[17]);*/
	ReverseLandmark(pMesh3D->m_LandMarkArray[4]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[5]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[6]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[10]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[11]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[14]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[15]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[16]);
	ReverseLandmark(pMesh3D->m_LandMarkArray[17]);
	pDoc->UpdateAllViews(NULL);
}
void Actions_ConstrainedRegistration::ReverseLandmark(GeoMapLandMark* pLandmark)
{
	int num = pLandmark->landmark_points.size();
	std::vector<int> landmark_points_;
	landmark_points_.resize(num);
	for (int i = 0; i < num; i++)
	{
		landmark_points_[i] = pLandmark->landmark_points[num - 1 - i];
	}
	pLandmark->landmark_points = landmark_points_;
}


void Actions_ConstrainedRegistration::OnBnClickedButtonSetmeshuvMesh()
{
	// TODO: Add your control notification handler code here
	pMesh = pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
	char message[80];
	sprintf(message, "%d", p_FormView3->currentindex);
	GetDlgItem(IDC_BUTTON_SetMeshUV_Mesh_Text)->SetWindowText(message);
}


void Actions_ConstrainedRegistration::OnBnClickedButtonSetmeshuvUv()
{
	// TODO: Add your control notification handler code here
	pMesh_UV = pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
	char message[80];
	sprintf(message, "%d", p_FormView3->currentindex);
	GetDlgItem(IDC_BUTTON_SetMeshUV_UV_Text)->SetWindowText(message);
}

extern char Post_Message[1000];
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetmeshuv()
{
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the parameterization!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d* pMeshCopy = NULL;
	CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();
	if (pMesh_UV->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh_UV = pMeshUVCopy = ((CMesh3d_OpenMesh*)pMesh_UV)->ConvertMesh3d();
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);
			CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			pVertex->u = pVertex_uv->x();
			pVertex->v = pVertex_uv->y();
		}
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		if (!pMeshUVCopy)
			delete pMeshUVCopy;
		return;
	}
	//sprintf(Post_Message, "Set parameterization Fail because of the mesh type is not mesh3d!");
	//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiaHolomorphic()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\holomorphic\\sophie_holomorphic.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\holomorphic\\sophiesmile_holomorphic.obj");
}
void Actions_ConstrainedRegistration::LoadSophiaStraighteningModel(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2)
{
	//Type_Parameterdomain 0 rectangle, 1 circle
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\source_straightening.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraightening_holomorphic\\sophie.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraightening_holomorphic\\sophie_rotate.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\Sophie.Source.landmark.txt");

		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(14668);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(15861);
	

	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\target_straightening.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraightening_holomorphic\\sophiesmile.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraightening_holomorphic\\sophiesmile_rotate.obj");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();


	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\Sophie.Target.landmark.txt");

	pPolyOmino->pMesh->m_FixVerticesArray.push_back(21348);

	//Convert to Openmesh
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	pOpenMesh1 = pMesh1->Convert_OpenMesh();
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2 = pMesh2->Convert_OpenMesh();

}
void Actions_ConstrainedRegistration::LoadBudataModel(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2, int Type_Parameterdomain, int HasLandmark)
{
	//Type_Parameterdomain 0 rectangle, 1 circle
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\fe\\F0001_FE01WH_F3D_texture.m");

	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	if (HasLandmark)
	{
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\fe\\F0001_FE01WH_F3D_landmark_changesequence.txt");
	}

	if (Type_Parameterdomain == 0)
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7454);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7525);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7613);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);

		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.999994);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.527311);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-1);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.194469);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(4.02148e-14);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.194498);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-3.27195e-13);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.527283);
	}
	else
	{
		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(14668);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(15861);
	}

	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\su\\F0001_SU02WH_F3D_fixed_texture1.m");

	p_FormView3->currentindex = 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	if (HasLandmark)
	{
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->pMesh->hasvt = 1;
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\su\\F0001_SU02WH_F3D_landmark_changesequence1.txt");
	}

	if (Type_Parameterdomain == 0)
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7020);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7405);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7106);

		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.991567);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.512348);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-1);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.159772);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(2e-06);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.224549);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-1e-06);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.452031);
	}
	else
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(21348);
	}

	//Convert to Openmesh
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	pOpenMesh1 = pMesh1->Convert_OpenMesh();
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2 = pMesh2->Convert_OpenMesh();
}
void Actions_ConstrainedRegistration::LoadSophiaModel(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2,int Type_Parameterdomain,int HasLandmark)
{
	//Type_Parameterdomain 0 rectangle, 1 circle
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie.remesh.rotate14.m");

	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	if (HasLandmark)
	{
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\sophie.remesh.rotate.landmark14.txt");
	}

	if (Type_Parameterdomain == 0)
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(140);
		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(19333);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(18013);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(12298);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(101);

		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.499649);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.00258);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.500352);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.00377);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.500359);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.772237);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.499645);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.773424);
	}
	else
	{
		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(14668);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(15861);
	}

	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie_smile.remesh.rotate12.m");

	p_FormView3->currentindex = 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	if (HasLandmark)
	{
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->pMesh->hasvt = 1;
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie_smile.remesh.rotate.landmark12.txt");
	}

	if (Type_Parameterdomain == 0)
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7203);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(4645);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(12684);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(251);

		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.658971);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.731788);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.341025);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.733554);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.341032);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.05882);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.658968);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.060594);
	}
	else
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(21348);
	}

	//Convert to Openmesh
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	pOpenMesh1 = pMesh1->Convert_OpenMesh();
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2 = pMesh2->Convert_OpenMesh();
}
void Actions_ConstrainedRegistration::LoadBrainModel(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2, int Type_Parameterdomain, int HasLandmark)
{
	//Type_Parameterdomain 0 rectangle, 1 circle
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\zengbrain\\E117_N4.left.inner.cortex.id.m");

	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	if (HasLandmark)
	{
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\zengbrain\\E117_N4.left.inner.cortex.id.txt");
	}

	if (Type_Parameterdomain == 0)
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(140);
		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(19333);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(18013);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(12298);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(101);

		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.499649);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.00258);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.500352);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.00377);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.500359);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.772237);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.499645);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.773424);
	}
	else
	{
		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(14668);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(66340);
	}

	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\zengbrain\\E117_N4.left.mid.cortex.id.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	if (HasLandmark)
	{
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->pMesh->hasvt = 1;
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\zengbrain\\E117_N4.left.mid.cortex.id.txt");
	}

	if (Type_Parameterdomain == 0)
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7203);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(4645);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(12684);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(251);

		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.658971);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.731788);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.341025);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.733554);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.341032);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.05882);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.658968);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.060594);
	}
	else
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(66340);
	}

	//Convert to Openmesh
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	pOpenMesh1 = pMesh1->Convert_OpenMesh();
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2 = pMesh2->Convert_OpenMesh();
}
void Actions_ConstrainedRegistration::LoadBrainModel_1(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2, int Type_Parameterdomain, int HasLandmark)
{
	//Type_Parameterdomain 0 rectangle, 1 circle
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\zengbrain\\E117_N4.left.inner.cortex.id.m");
	pDoc->OnOpenDocument("D:\\papers\\2019\\iccv2019\\data_brain\\L1554.m");

	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	if (HasLandmark)
	{
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->Loadlandmark("D:\\papers\\2019\\iccv2019\\data_brain\\L1554_landmark.txt");
	}

	if (Type_Parameterdomain == 0)
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(140);
		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(19333);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(18013);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(12298);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(101);

		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.499649);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.00258);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.500352);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.00377);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.500359);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.772237);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.499645);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.773424);
	}
	else
	{
		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(14668);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(2551);
	}

	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\2019\\iccv2019\\data_brain\\L1627.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	if (HasLandmark)
	{
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->pMesh->hasvt = 1;
		pPolyOmino->Loadlandmark("D:\\papers\\2019\\iccv2019\\data_brain\\L1627_landmark.txt");
	}

	if (Type_Parameterdomain == 0)
	{
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(7203);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(4645);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(12684);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(251);

		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.658971);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.731788);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.341025);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.733554);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.341032);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.05882);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0.658968);
		pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-0.060594);
	}
	else
	{
		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(4499);
		pPolyOmino->pMesh->m_FixVerticesArray.push_back(3855);
		//pPolyOmino->pMesh->m_FixVerticesArray.push_back(6354);
	}

	//Convert to Openmesh
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	pOpenMesh1 = pMesh1->Convert_OpenMesh();
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2 = pMesh2->Convert_OpenMesh();
}
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalparameterizationQuad()
{
	CMesh3d_OpenMesh* pSource_OpenMesh, *pTarget_Openmesh;
	//Load Sophia Model
	LoadSophiaModel(pSource_OpenMesh, pTarget_Openmesh,0,0);

	//Load the boundary vertices
	OnBnClickedConstrainedregistrationLoadsophiaHolomorphic();

	//First map the two models with constraints to the plane
	CMesh3d* pSourceParameterization_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	CMesh3d* pTargetParameterization_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	//
	pSource_OpenMesh->pBoundaryMeshReference = pSourceParameterization_Mesh3d->Convert_OpenMesh();
	pTarget_Openmesh->pBoundaryMeshReference = pTargetParameterization_Mesh3d->Convert_OpenMesh();

	//First Map The meshes to 2D quad
	MapTo2D(pSource_OpenMesh, pTarget_Openmesh,0);

	//
	LoadLandmarkOnParameterization(pDoc->m_SceneGraph.NbObject() - 2, pDoc->m_SceneGraph.NbObject() - 1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_ConstrainedRegistration::CopyImageColorToTargetMesh(CObject3d* pMesh)
{
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	
	CMesh3d* pMeshCopy = NULL;
	//CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();

	CImage m_TextureImage;

	m_TextureImage.Load("D:\\2.png");
	int x, y;
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		//CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);
			//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			//if (pVertex->x() > 0 && pVertex->x() < 1)
			{
				x = m_TextureImage.GetWidth() * (pVertex->x() + 1) / 2;
				y = m_TextureImage.GetHeight() * (pVertex->y() + 1) / 2;
				if (x >= m_TextureImage.GetWidth())
					x = m_TextureImage.GetWidth() - 1;
				if (y >= m_TextureImage.GetHeight())
					y = m_TextureImage.GetHeight() - 1;
				COLORREF color = m_TextureImage.GetPixel(x, y);
				pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
			}
			//pVertex_uv->SetColor(*pVertex->GetColor());
			//pVertex->u = pVertex_uv->x();
			//pVertex->v = pVertex_uv->y();
		}
		pMesh3d->m_Modified = 1;
		pMesh3d->m_ColorBinding = COLOR_PER_VERTEX;
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		//if (!pMeshUVCopy)
		//	delete pMeshUVCopy;
		return;
	}
}
void Actions_ConstrainedRegistration::CopyMeshVertexColor(CObject3d* pMesh, CObject3d* pMesh_UV)
{
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the target mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d* pMeshCopy = NULL;
	CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();
	if (pMesh_UV->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh_UV = pMeshUVCopy = ((CMesh3d_OpenMesh*)pMesh_UV)->ConvertMesh3d();
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);
			CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			pVertex_uv->SetColor(*pVertex->GetColor());
			//pVertex->u = pVertex_uv->x();
			//pVertex->v = pVertex_uv->y();
		}
		pMesh3dUV->m_Modified = 1;
		pMesh3dUV->m_ColorBinding = COLOR_PER_VERTEX;
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		if (!pMeshUVCopy)
			delete pMeshUVCopy;
		return;
	}
}
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyvertexcolor()
{
	// TODO: Add your control notification handler code here
	CopyMeshVertexColor(pMesh, pMesh_UV);
}

void Actions_ConstrainedRegistration::LoadLandmarkOnParameterization(int index1,int index2)
{
	//p_FormView3->currentindex = 4;
	p_FormView3->currentindex = index1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\sophie.remesh.rotate.landmark14.txt");

	//p_FormView3->currentindex = 5;
	p_FormView3->currentindex = index2;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie_smile.remesh.rotate.landmark12.txt");
}
void Actions_ConstrainedRegistration::LoadLandmarkOnParameterization_OnBrain(CMesh3d_OpenMesh* pMesh1, CMesh3d_OpenMesh* pMesh2)
{
	if (pMesh1)
	{
		CMesh3d* pMesh3d1 = pMesh1->ConvertMesh3d();
		pDoc->m_SceneGraph.Add(pMesh3d1);
		p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		pPolyOmino->SetCurrentMesh();
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\zengbrain\\E117_N4.left.inner.cortex.id.txt");
	}

	if (pMesh2)
	{
		CMesh3d* pMesh3d2 = pMesh2->ConvertMesh3d();
		pDoc->m_SceneGraph.Add(pMesh3d2);
		p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		pPolyOmino->SetCurrentMesh();
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\zengbrain\\E117_N4.left.mid.cortex.id.txt");
	}

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_ConstrainedRegistration::LoadLandmarkOnParameterization_OnBrain_1(CMesh3d_OpenMesh* pMesh1, CMesh3d_OpenMesh* pMesh2)
{
	if (pMesh1)
	{
		CMesh3d* pMesh3d1 = pMesh1->ConvertMesh3d();
		pDoc->m_SceneGraph.Add(pMesh3d1);
		p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		pPolyOmino->SetCurrentMesh();
		pPolyOmino->Loadlandmark("D:\\papers\\2019\\iccv2019\\data_brain\\L1554_landmark.txt");
	}

	if (pMesh2)
	{
		CMesh3d* pMesh3d2 = pMesh2->ConvertMesh3d();
		pDoc->m_SceneGraph.Add(pMesh3d2);
		p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		pPolyOmino->SetCurrentMesh();
		pPolyOmino->Loadlandmark("D:\\papers\\2019\\iccv2019\\data_brain\\L1627_landmark.txt");
	}

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalparameterizationCircle()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	LoadSophiaModel(pOpenMesh1, pOpenMesh2,1,0);

	//Map The meshes to 2D
	MapTo2D(pOpenMesh1, pOpenMesh2,1);

	//Load Landmark on the parameterization
	LoadLandmarkOnParameterization(pDoc->m_SceneGraph.NbObject() - 2, pDoc->m_SceneGraph.NbObject() - 1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircle()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1,1);

	//Map The meshes to 2D
	MapTo2D(pOpenMesh1, pOpenMesh2, 1);

	//Load Landmark on the parameterization
	LoadLandmarkOnParameterization(pDoc->m_SceneGraph.NbObject() - 2, pDoc->m_SceneGraph.NbObject() - 1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadbrain()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load Brain Model
	LoadBrainModel(pOpenMesh1, pOpenMesh2, 1, 1);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationBrainConformalparameterizationCircle()
{
	CMesh3d_OpenMesh* pOpenMesh1 = NULL, *pOpenMesh2 = NULL;

	//Load Brain Model
	LoadBrainModel(pOpenMesh1, pOpenMesh2, 1, 1);

	//Map The meshes to 2D
	MapTo2D(pOpenMesh1, pOpenMesh2, 1);

	//Load Landmark on the parameterization
	LoadLandmarkOnParameterization_OnBrain(pOpenMesh1, pOpenMesh2);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationExtractboundaryedges()
{
	//Convert to Openmesh
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	CMesh3d_OpenMesh* pOpenMesh1 = pMesh1->Convert_OpenMesh();
	for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
	{
		if (pOpenMesh1->pmesh->is_boundary(v_it.handle()))
			pMesh1->m_FixVerticesArray.push_back(v_it.handle().idx());
	}
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonicSophia()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);

	LoadSophiaStraighteningModel(pOpenMesh1, pOpenMesh2);

	//Compute bijective mapping between pOpenmesh3 and pOpenmesh4
	Bijective2DMapping_Sophia(pOpenMesh1, pOpenMesh2);
	//LoadLandmarkOnParameterization(pDoc->m_SceneGraph.NbObject() - 2, pDoc->m_SceneGraph.NbObject() - 1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiastraightening()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;
	LoadSophiaStraighteningModel(pOpenMesh1, pOpenMesh2);
}

void Actions_ConstrainedRegistration::LoadSophiaSearchStepResults()
{
	char message[180];
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\searchstep\\initial_result_flip.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showFace = FALSE;
	pPolyOmino->pMesh->m_showEdge = FALSE;
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\Sophie.Source.landmark.txt");

	for (int i = 0; i <= 5; i++)
	{
		sprintf(message, "D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\searchstep\\%d.obj", i);
		pDoc->OnOpenDocument(message);
		p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		pPolyOmino->OnBnClickedOminoSetcurrentmesh();
		pPolyOmino->pMesh->m_showFace = FALSE;
		pPolyOmino->pMesh->m_showEdge = FALSE;
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\Sophie.Source.landmark.txt");
		sprintf(message, "D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\searchstep\\%d_CDT.obj", i);
		pDoc->OnOpenDocument(message);
		p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		pPolyOmino->OnBnClickedOminoSetcurrentmesh();
		pPolyOmino->pMesh->m_showFace = FALSE;
		pPolyOmino->pMesh->m_showEdge = FALSE;
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\Sophie.Source.landmark.txt");
		sprintf(message, "D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\searchstep\\%d_CDT_result_flip.obj", i);
		pDoc->OnOpenDocument(message);
		p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		pPolyOmino->OnBnClickedOminoSetcurrentmesh();
		pPolyOmino->pMesh->m_showFace = FALSE;
		pPolyOmino->pMesh->m_showEdge = FALSE;
		pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
		pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
		pPolyOmino->pMesh->m_showFace = TRUE;
		pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\Sophie.Source.landmark.txt");
	}
	
	//finalresult cdt
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\searchstep\\5_CDT_result_flip_CDT.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showFace = FALSE;
	pPolyOmino->pMesh->m_showEdge = FALSE;
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\Sophie.Source.landmark.txt");
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiaSearchstep()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;
	LoadSophiaStraighteningModel(pOpenMesh1, pOpenMesh2);

	//result
	LoadSophiaSearchStepResults();
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyvertexcolorDifferenttopology()
{
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the target mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d* pMeshCopy = NULL;
	CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();
	if (pMesh_UV->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh_UV = pMeshUVCopy = ((CMesh3d_OpenMesh*)pMesh_UV)->ConvertMesh3d();
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		pMesh3d->BuildAdjacency();
		CColor color;
		int flag = 0;
		for (int i = 0; i < pMesh3dUV->NbVertex(); i++)
		{
			CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			flag = 0;
			for (int j = 0; j < pMesh3d->NbFace(); j++)
			{
				CFace3d* pFace3d = pMesh3d->GetFace(j);
				CVertex3d* pVertex1 = pFace3d->v1();
				CVertex3d* pVertex2 = pFace3d->v2();
				CVertex3d* pVertex3 = pFace3d->v3();
				BOOL b = pTreeView->JudgeWhetherPointLiesInsideTriangle(pVertex_uv->x(), pVertex_uv->y(), pVertex1->x(), pVertex1->y(), pVertex2->x(), pVertex2->y(), pVertex3->x(), pVertex3->y());
				if (b)
				{
					double totalarea = pTreeView->TriangleArea(pVertex1->x(), pVertex1->y(), pVertex2->x(), pVertex2->y(), pVertex3->x(), pVertex3->y());
					double area1 = pTreeView->TriangleArea(pVertex_uv->x(), pVertex_uv->y(), pVertex2->x(), pVertex2->y(), pVertex3->x(), pVertex3->y());
					double area2 = pTreeView->TriangleArea(pVertex1->x(), pVertex1->y(), pVertex_uv->x(), pVertex_uv->y(), pVertex3->x(), pVertex3->y());
					double area3 = pTreeView->TriangleArea(pVertex1->x(), pVertex1->y(), pVertex2->x(), pVertex2->y(), pVertex_uv->x(), pVertex_uv->y());
					area1 = area1 / totalarea;
					area2 = area2 / totalarea;
					area3 = area3 / totalarea;

					color.r(pVertex1->GetColor()->r() * area1 + pVertex2->GetColor()->r() * area2 + pVertex3->GetColor()->r() * area3);
					color.g(pVertex1->GetColor()->g() * area1 + pVertex2->GetColor()->g() * area2 + pVertex3->GetColor()->g() * area3);
					color.b(pVertex1->GetColor()->b() * area1 + pVertex2->GetColor()->b() * area2 + pVertex3->GetColor()->b() * area3);

					pVertex_uv->SetColor(color);
					flag = 1;
				}
			}
			if (!flag)
			{
				//first find the closest point.
				double mindis = 1e10;
				int    minindex;
				for (int j = 0; j < pMesh3d->NbVertex(); j++)
				{
					CVertex3d* pVertex = pMesh3d->GetVertex(j);
					double dis = Distance(pVertex, pVertex_uv);
					if (dis < mindis)
					{
						mindis = dis;
						minindex = j;
					}
				}

				if (mindis < 1e-5)
				{
					pVertex_uv->SetColor(*pMesh3d->GetVertex(minindex)->GetColor());
				}
				else
				{
					//on edges
					CVertex3d* pVertex = pMesh3d->GetVertex(minindex);
					int n_vneighbors = pVertex->NbVertexNeighbor();
					double vdis = Distance(pVertex, pVertex_uv);
					double mindis = 1e10;
					CVertex3d* pminVertex;
					for (int ii = 0; ii < n_vneighbors; ii++)
					{
						CVertex3d* pVertexNeighbor = pVertex->GetVertexNeighbor(ii);

						double dis = pTreeView->GetDistancebetweenPointandLineSegment(pVertex_uv->x(), pVertex_uv->y(), pVertex->x(), pVertex->y(), pVertexNeighbor->x(), pVertexNeighbor->y());
						if (dis < mindis)
						{
							mindis = dis;
							pminVertex = pVertexNeighbor;
						}
					}
					if (mindis < vdis)
					{
						double dis1 = Distance(pVertex_uv, pminVertex);
						double dis2 = Distance(pVertex_uv, pVertex);
						double dis = dis1 + dis2;
						dis1 = dis1 / dis;
						dis2 = dis2 / dis;
						color.r(pVertex->GetColor()->r() * dis1 + pminVertex->GetColor()->r() * dis2);
						color.g(pVertex->GetColor()->g() * dis1 + pminVertex->GetColor()->g() * dis2);
						color.b(pVertex->GetColor()->b() * dis1 + pminVertex->GetColor()->b() * dis2);

						pVertex_uv->SetColor(color);
					}
					else
					{
						pVertex_uv->SetColor(*pVertex->GetColor());
					}
				}
			}
		}
		pMesh3dUV->m_Modified = 1;
		pMesh3dUV->m_ColorBinding = COLOR_PER_VERTEX;
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		if (!pMeshUVCopy)
			delete pMeshUVCopy;

		pDoc->UpdateAllViews(NULL);
		return;
	}
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiaInitialregistrationHarmonic()
{
	// TODO: Add your control notification handler code here
	char message[180];
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\searchstep\\initial_result_flip.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showFace = FALSE;
	pPolyOmino->pMesh->m_showEdge = FALSE;
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\Sophie.Source.landmark.txt");
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyuvDifferenttopology()
{
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the target mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d* pMeshCopy = NULL;
	CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();
	if (pMesh_UV->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh_UV = pMeshUVCopy = ((CMesh3d_OpenMesh*)pMesh_UV)->ConvertMesh3d();
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		pMesh3d->BuildAdjacency();
		pMesh3d->SetVertexFlagAsItsIndex();
		pMesh3dUV->SetVertexFlagAsItsIndex();
		CColor color;
		int flag = 0;
		for (int i = 0; i < pMesh3dUV->NbVertex(); i++)
		{
			CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			int val = pVertex_uv->GetFlag();
			flag = 0;
			for (int j = 0; j < pMesh3d->NbFace(); j++)
			{
				CFace3d* pFace3d = pMesh3d->GetFace(j);
				CVertex3d* pVertex1 = pFace3d->v1();
				CVertex3d* pVertex2 = pFace3d->v2();
				CVertex3d* pVertex3 = pFace3d->v3();
				CVertex3d* pVertex_Ref1 = pMesh3d->pReferenceMesh->GetVertex(pVertex1->GetFlag());
				CVertex3d* pVertex_Ref2 = pMesh3d->pReferenceMesh->GetVertex(pVertex2->GetFlag());
				CVertex3d* pVertex_Ref3 = pMesh3d->pReferenceMesh->GetVertex(pVertex3->GetFlag());
				BOOL b = pTreeView->JudgeWhetherPointLiesInsideTriangle(pVertex_uv->x(), pVertex_uv->y(), pVertex1->x(), pVertex1->y(), pVertex2->x(), pVertex2->y(), pVertex3->x(), pVertex3->y());
				if (b)
				{
					double totalarea = pTreeView->TriangleArea(pVertex1->x(), pVertex1->y(), pVertex2->x(), pVertex2->y(), pVertex3->x(), pVertex3->y());
					double area1 = pTreeView->TriangleArea(pVertex_uv->x(), pVertex_uv->y(), pVertex2->x(), pVertex2->y(), pVertex3->x(), pVertex3->y());
					double area2 = pTreeView->TriangleArea(pVertex1->x(), pVertex1->y(), pVertex_uv->x(), pVertex_uv->y(), pVertex3->x(), pVertex3->y());
					double area3 = pTreeView->TriangleArea(pVertex1->x(), pVertex1->y(), pVertex2->x(), pVertex2->y(), pVertex_uv->x(), pVertex_uv->y());
					area1 = area1 / totalarea;
					area2 = area2 / totalarea;
					area3 = area3 / totalarea;

					double x = pVertex_Ref1->x() * area1 + pVertex_Ref2->x() * area2 + pVertex_Ref3->x() * area3;
					double y = pVertex_Ref1->y() * area1 + pVertex_Ref2->y() * area2 + pVertex_Ref3->y() * area3;
					double z = pVertex_Ref1->z() * area1 + pVertex_Ref2->z() * area2 + pVertex_Ref3->z() * area3;

					pVertex_uv->Set(x,y,z);
					flag = 1;
					break;
				}
			}
			if (!flag)
			{
				//first find the closest point.
				double mindis = 1e10;
				int    minindex;
				for (int j = 0; j < pMesh3d->NbVertex(); j++)
				{
					CVertex3d* pVertex = pMesh3d->GetVertex(j);
					double dis = Distance(pVertex, pVertex_uv);
					if (dis < mindis)
					{
						mindis = dis;
						minindex = j;
					}
				}

				if (mindis < 1e-5)
				{
					pVertex_uv->Set(pMesh3d->pReferenceMesh->GetVertex(minindex)->x(), pMesh3d->pReferenceMesh->GetVertex(minindex)->y(), pMesh3d->pReferenceMesh->GetVertex(minindex)->z());
				}
				else
				{
					//on edges
					CVertex3d* pVertex = pMesh3d->GetVertex(minindex);
					CVertex3d* pVertex_ref = pMesh3d->pReferenceMesh->GetVertex(pVertex->GetFlag());
					int n_vneighbors = pVertex->NbVertexNeighbor();
					double vdis = Distance(pVertex, pVertex_uv);
					double mindis = 1e10;
					CVertex3d* pminVertex,*pminVertex_ref;
					for (int ii = 0; ii < n_vneighbors; ii++)
					{
						CVertex3d* pVertexNeighbor = pVertex->GetVertexNeighbor(ii);

						double dis = pTreeView->GetDistancebetweenPointandLineSegment(pVertex_uv->x(), pVertex_uv->y(), pVertex->x(), pVertex->y(), pVertexNeighbor->x(), pVertexNeighbor->y());
						if (dis < mindis)
						{
							mindis = dis;
							pminVertex = pVertexNeighbor;
						}
					}
					pminVertex_ref = pMesh3d->pReferenceMesh->GetVertex(pminVertex->GetFlag());
					if (mindis < vdis)
					{
						double dis1 = Distance(pVertex_uv, pminVertex);
						double dis2 = Distance(pVertex_uv, pVertex);
						double dis = dis1 + dis2;
						dis1 = dis1 / dis;
						dis2 = dis2 / dis;

						double x = pVertex_ref->x() * dis1 + pminVertex_ref->x() * dis2;
						double y = pVertex_ref->y() * dis1 + pminVertex_ref->y() * dis2;
						double z = pVertex_ref->z() * dis1 + pminVertex_ref->z() * dis2;

						pVertex_uv->Set(x, y, z);
					}
					else
					{
						//pVertex_uv->SetColor(*pVertex->GetColor());
						pVertex_uv->Set(pVertex_ref->x(), pVertex_ref->y(), pVertex_ref->z());
					}
				}
			}
		}
		pMesh3dUV->m_Modified = 1;
		pMesh3dUV->m_ColorBinding = COLOR_PER_VERTEX;
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		if (!pMeshUVCopy)
			delete pMeshUVCopy;

		pDoc->UpdateAllViews(NULL);
		return;
	}
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetreferencemesh()
{
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the target mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d* pMeshCopy = NULL;
	CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();
	if (pMesh_UV->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh_UV = pMeshUVCopy = ((CMesh3d_OpenMesh*)pMesh_UV)->ConvertMesh3d();

	((CMesh3d* )pMesh)->pReferenceMesh = (CMesh3d*)pMesh_UV;
	sprintf(Post_Message, "Type: both mesh3d, Set Reference Mesh Successfully!");
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadbrain2()
{
	// TODO: Add your control notification handler code here
	char message[180];
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\Face\\ECCV14\\data_sophie\\sophie3lh1lv_b2e2mn.m");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->pMesh->m_showFace = FALSE;
	pPolyOmino->pMesh->m_showEdge = FALSE;
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\Face\\ECCV14\\data_sophie\\sophie3lh1lv_b2e2mn_landmark1.txt");

	pPolyOmino->pMesh->m_FixVerticesArray.push_back(16);
	//return;
	//
	CMesh3d_OpenMesh* pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\Face\\ECCV14\\data_sophie\\sophie3lh1lv_b2e2mn_landmark1.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyuvDifferenttopology2()
{
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the target mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d* pMeshCopy = NULL;
	CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();
	if (pMesh_UV->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh_UV = pMeshUVCopy = ((CMesh3d_OpenMesh*)pMesh_UV)->ConvertMesh3d();
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);
			CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			pVertex_uv->SetNormal(*pVertex->GetNormal());
			//pVertex_uv->SetColor(*pVertex->GetColor());
			//pVertex->u = pVertex_uv->x();
			//pVertex->v = pVertex_uv->y();
		}
		pMesh3dUV->m_Modified = 1;
		pMesh3dUV->m_ColorBinding = COLOR_PER_VERTEX;
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		if (!pMeshUVCopy)
			delete pMeshUVCopy;
		return;
	}
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationTargetvertexcolorMu()
{
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the target mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d_OpenMesh* pMeshCopy = ((CMesh3d*)pMesh)->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh_UVCopy = ((CMesh3d*)pMesh_UV)->Convert_OpenMesh();

	//
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Face mu
	if (!pMeshCopy->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMeshCopy->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex UV
	if (!pMeshCopy->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMeshCopy->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Set UV
	pHarmonicMapping->SetMeshParameterUV(pMeshCopy, pMesh_UVCopy);

	//Compute Vertex Mu
	//pHarmonicMapping->Compute_Mu(pMeshCopy);

	//Compute the Vertex Mu
	ComputeVertexMu(pMeshCopy);

	//Change the mu of the vertices near the boundary 
	ChangeVertexNearBoundaryMu(pMeshCopy);

	//Set Vertex Color
	SetVertexColorByMu((CMesh3d*)pMesh, (CMesh3d*)pMesh_UV, pMeshCopy);
}

void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationTargetvertexcolorMu_Registration()
{
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the target mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d_OpenMesh* pMeshCopy = ((CMesh3d*)pMesh)->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh_UVCopy = ((CMesh3d*)pMesh_UV)->Convert_OpenMesh();

	//
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Face mu
	if (!pMeshCopy->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMeshCopy->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex UV
	if (!pMeshCopy->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMeshCopy->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Set UV
	//pHarmonicMapping->SetMeshParameterUV(pMeshCopy, pMesh_UVCopy);

	//Compute Vertex Mu
	//pHarmonicMapping->Compute_Mu(pMeshCopy);

	//Compute the Vertex Mu
	ComputeVertexMu_Registration(pMeshCopy, pMesh_UVCopy);

	//Change the mu of the vertices near the boundary 
	//ChangeVertexNearBoundaryMu(pMeshCopy);

	//Set Vertex Color
	SetVertexColorByMu((CMesh3d*)pMesh, (CMesh3d*)pMesh_UV, pMeshCopy);
}
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationBothMeshAreaStretch_Registration()
{
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the target mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d_OpenMesh* pMeshCopy = ((CMesh3d*)pMesh)->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh_UVCopy = ((CMesh3d*)pMesh_UV)->Convert_OpenMesh();

	//
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Face mu
	if (!pMeshCopy->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMeshCopy->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex UV
	if (!pMeshCopy->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMeshCopy->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Set UV
	//pHarmonicMapping->SetMeshParameterUV(pMeshCopy, pMesh_UVCopy);

	//Compute Vertex Mu
	//pHarmonicMapping->Compute_Mu(pMeshCopy);

	//Compute the Vertex Mu
	//ComputeVertexMu(pMeshCopy);
	ComputeVertexAreaStretch_Registration(pMeshCopy, pMesh_UVCopy);

	//Change the mu of the vertices near the boundary 
	//ChangeVertexNearBoundaryMu(pMeshCopy);

	//Set Vertex Color
	SetVertexColorByMu((CMesh3d*)pMesh, (CMesh3d*)pMesh_UV, pMeshCopy);
}
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationBothMeshAreaStretch()
{
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the target mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d_OpenMesh* pMeshCopy = ((CMesh3d*)pMesh)->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh_UVCopy = ((CMesh3d*)pMesh_UV)->Convert_OpenMesh();

	//
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Face mu
	if (!pMeshCopy->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMeshCopy->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex UV
	if (!pMeshCopy->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMeshCopy->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Set UV
	pHarmonicMapping->SetMeshParameterUV(pMeshCopy, pMesh_UVCopy);

	//Compute Vertex Mu
	//pHarmonicMapping->Compute_Mu(pMeshCopy);

	//Compute the Vertex Mu
	//ComputeVertexMu(pMeshCopy);
	ComputeVertexAreaStretch(pMeshCopy);

	//Change the mu of the vertices near the boundary 
	//ChangeVertexNearBoundaryMu(pMeshCopy);

	//Set Vertex Color
	SetVertexColorByMu((CMesh3d*)pMesh, (CMesh3d*)pMesh_UV, pMeshCopy);
}
void Actions_ConstrainedRegistration::ChangeVertexNearBoundaryMu(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_vflag;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex Flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_vflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_vflag, PROP_VERTEX_SELECTED);

	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	std::vector<double> x, y, u, v;
	std::vector<double> m_mu;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_vflag, v_it.handle()) = 0;
	}
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
		{
			for (MyMesh::VertexVertexIter vv_it = pMesh3D->pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
			{
				if (!pMesh3D->pmesh->is_boundary(vv_it.handle()))
				{
					pMesh3D->pmesh->property(vprop_vertex_vflag, vv_it.handle()) = 1;
				}
			}
		}
	}
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->property(vprop_vertex_vflag, v_it.handle()))
		{
			m_mu = pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle());
			if (m_mu.size() > 0)
			{
				m_mu[0] = m_mu[0] * 1;
				m_mu[1] = m_mu[1] * 1;
				pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle()) = m_mu;
			}
		}
	}

}
double Actions_ConstrainedRegistration::GetFaceArea(CMesh3d_OpenMesh* pMesh3D,MyMesh::FaceHandle fh)
{
	std::vector<double> x, y;
	for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(fh); fv_it; ++fv_it)
	{
		OpenMesh::Vec3d pt;
		pt = pMesh3D->pmesh->point(fv_it.handle());
		x.push_back(pt[0]);
		y.push_back(pt[1]);
	}

	double area = pTreeView->TriangleArea(x[0], y[0], x[1], y[1], x[2], y[2]);
	return area;
}
void Actions_ConstrainedRegistration::ComputeVertexAreaStretch_Registration(CMesh3d_OpenMesh* pMesh3D, CMesh3d_OpenMesh* pMesh3D_uv)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;
	std::ofstream file("D:\\fenbu.txt");
	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	std::vector<double> x, y, u, v;
	double minfaimou = 1e10;
	double maxfaimou = -1e10;
	std::vector<int> m_tongji;
	m_tongji.resize(11, 0);
	std::vector<double> m_mu;
	std::vector<int> m_vindexarray;
	double fai_mou;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		x.clear();
		y.clear();
		u.clear();
		v.clear();
		m_vindexarray.clear();
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, fv_it.handle());
			OpenMesh::Vec3d pt;
			pt = pMesh3D->pmesh->point(fv_it.handle());
			x.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D, f_it.handle(), fv_it.handle(), 0));
			y.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D, f_it.handle(), fv_it.handle(), 1));
			//
			//x.push_back(pt[0]);
			//y.push_back(pt[1]);
			u.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D_uv, f_it.handle(), fv_it.handle(), 0));
			v.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D_uv, f_it.handle(), fv_it.handle(), 1));
			m_vindexarray.push_back(fv_it.handle().idx());
		}
		Vector3D v1, v2, v3;
		Vector3D v1_uv, v2_uv, v3_uv;
		v1[0] = x[0];
		v1[1] = y[0];
		v1[2] = 0;

		v2[0] = x[1];
		v2[1] = y[1];
		v2[2] = 0;

		v3[0] = x[2];
		v3[1] = y[2];
		v3[2] = 0;

		v1_uv[0] = u[0];
		v1_uv[1] = v[0];
		v1_uv[2] = 0;

		v2_uv[0] = u[1];
		v2_uv[1] = v[1];
		v2_uv[2] = 0;

		v3_uv[0] = u[2];
		v3_uv[1] = v[2];
		v3_uv[2] = 0;

		double trianglearea = Length((v2 - v1) ^ (v3 - v1));
		double trianglearea_uv = Length((v2_uv - v1_uv) ^ (v3_uv - v1_uv));
		double fai_real, fai_imaginary;
		if (trianglearea > trianglearea_uv)
			fai_real = trianglearea_uv / trianglearea;
		else
			fai_real = trianglearea / trianglearea_uv;

		fai_real = 1 - fai_real;
		fai_imaginary = 0;
		fai_mou = fai_real;
		m_mu.clear();
		m_mu.push_back(fai_real);
		m_mu.push_back(fai_imaginary);
		pMesh3D->pmesh->property(fprop_face_mu, f_it.handle()) = m_mu;

		file << fai_mou << "\n";
		if (fai_mou > maxfaimou)
			maxfaimou = fai_mou;
		if (fai_mou < minfaimou)
			minfaimou = fai_mou;
		if (fai_mou > 1)
			m_tongji[10]++;
		else if (fai_mou > 0.9)
			m_tongji[9]++;
		else if (fai_mou > 0.8)
			m_tongji[8]++;
		else if (fai_mou > 0.7)
			m_tongji[7]++;
		else if (fai_mou > 0.6)
			m_tongji[6]++;
		else if (fai_mou > 0.5)
			m_tongji[5]++;
		else if (fai_mou > 0.4)
			m_tongji[4]++;
		else if (fai_mou > 0.3)
			m_tongji[3]++;
		else if (fai_mou > 0.2)
			m_tongji[2]++;
		else if (fai_mou > 0.1)
			m_tongji[1]++;
		else
			m_tongji[0]++;
	}
	file.close();
	double mux, muy;
	int num;
	double totalmulength = 0;
	int vnum = 0;
	std::vector<double> muvec;
	double totalarea;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		mux = 0;
		muy = 0;
		num = 0;
		totalarea = 0;
		for (MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(v_it.handle()); vf_it; vf_it++)
		{
			std::vector<double> mu = pMesh3D->pmesh->property(fprop_face_mu, vf_it.handle());
			if (mu.size() == 2)
			{
				double trianglearea = GetFaceArea(pMesh3D, vf_it.handle());
				mux += mu[0] * trianglearea;
				muy += mu[1] * trianglearea;

				totalarea += trianglearea;
				num++;
			}
		}
		//mux /= num;
		//muy /= num;
		mux /= totalarea;
		muy /= totalarea;
		muvec.clear();
		muvec.push_back(mux);
		muvec.push_back(muy);
		totalmulength += sqrt(SQR(mux) + SQR(muy));
		pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle()) = muvec;
		vnum++;
	}
}
void Actions_ConstrainedRegistration::ComputeVertexAreaStretch(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;
	std::ofstream file("D:\\fenbu.txt");
	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	std::vector<double> x, y, u, v;
	double minfaimou = 1e10;
	double maxfaimou = -1e10;
	std::vector<int> m_tongji;
	m_tongji.resize(11, 0);
	std::vector<double> m_mu;
	std::vector<int> m_vindexarray;
	double fai_mou;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		x.clear();
		y.clear();
		u.clear();
		v.clear();
		m_vindexarray.clear();
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, fv_it.handle());
			OpenMesh::Vec3d pt;
			pt = pMesh3D->pmesh->point(fv_it.handle());
			x.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D, f_it.handle(), fv_it.handle(), 0));
			y.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D, f_it.handle(), fv_it.handle(), 1));
			//
			//x.push_back(pt[0]);
			//y.push_back(pt[1]);
			u.push_back(uv[0]);
			v.push_back(uv[1]);
			m_vindexarray.push_back(fv_it.handle().idx());
		}
		Vector3D v1, v2, v3;
		Vector3D v1_uv, v2_uv, v3_uv;
		v1[0] = x[0];
		v1[1] = y[0];
		v1[2] = 0;

		v2[0] = x[1];
		v2[1] = y[1];
		v2[2] = 0;

		v3[0] = x[2];
		v3[1] = y[2];
		v3[2] = 0;

		v1_uv[0] = u[0];
		v1_uv[1] = v[0];
		v1_uv[2] = 0;

		v2_uv[0] = u[1];
		v2_uv[1] = v[1];
		v2_uv[2] = 0;

		v3_uv[0] = u[2];
		v3_uv[1] = v[2];
		v3_uv[2] = 0;

		double trianglearea = Length((v2 - v1) ^ (v3 - v1));
		double trianglearea_uv = Length((v2_uv - v1_uv) ^ (v3_uv - v1_uv));
		double fai_real, fai_imaginary;
		if (trianglearea > trianglearea_uv)
			fai_real = trianglearea_uv / trianglearea;
		else
			fai_real = trianglearea / trianglearea_uv;

		fai_real = 1 - fai_real;
		fai_imaginary = 0;
		fai_mou = fai_real;
		m_mu.clear();
		m_mu.push_back(fai_real);
		m_mu.push_back(fai_imaginary);
		pMesh3D->pmesh->property(fprop_face_mu, f_it.handle()) = m_mu;

		file << fai_mou << "\n";

		if (fai_mou > maxfaimou)
			maxfaimou = fai_mou;
		if (fai_mou < minfaimou)
			minfaimou = fai_mou;
		if (fai_mou > 1)
			m_tongji[10]++;
		else if (fai_mou > 0.9)
			m_tongji[9]++;
		else if (fai_mou > 0.8)
			m_tongji[8]++;
		else if (fai_mou > 0.7)
			m_tongji[7]++;
		else if (fai_mou > 0.6)
			m_tongji[6]++;
		else if (fai_mou > 0.5)
			m_tongji[5]++;
		else if (fai_mou > 0.4)
			m_tongji[4]++;
		else if (fai_mou > 0.3)
			m_tongji[3]++;
		else if (fai_mou > 0.2)
			m_tongji[2]++;
		else if (fai_mou > 0.1)
			m_tongji[1]++;
		else
			m_tongji[0]++;
	}
	file.close();
	double mux, muy;
	int num;
	double totalmulength = 0;
	int vnum = 0;
	std::vector<double> muvec;
	double totalarea;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		mux = 0;
		muy = 0;
		num = 0;
		totalarea = 0;
		for (MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(v_it.handle()); vf_it; vf_it++)
		{
			std::vector<double> mu = pMesh3D->pmesh->property(fprop_face_mu, vf_it.handle());
			if (mu.size() == 2)
			{
				double trianglearea = GetFaceArea(pMesh3D, vf_it.handle());
				mux += mu[0] * trianglearea;
				muy += mu[1] * trianglearea;

				totalarea += trianglearea;
				num++;
			}
		}
		//mux /= num;
		//muy /= num;
		mux /= totalarea;
		muy /= totalarea;
		muvec.clear();
		muvec.push_back(mux);
		muvec.push_back(muy);
		totalmulength += sqrt(SQR(mux) + SQR(muy));
		pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle()) = muvec;
		vnum++;
	}
}
void Actions_ConstrainedRegistration::ComputeVertexMu_Registration(CMesh3d_OpenMesh* pMesh3D, CMesh3d_OpenMesh* pMesh3D_UV)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;
	std::ofstream file("D:\\fenbu.txt");

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	std::vector<double> x, y, u, v;
	double minfaimou = 1e10;
	double maxfaimou = -1e10;
	std::vector<int> m_tongji;
	m_tongji.resize(11, 0);
	std::vector<double> m_mu;
	std::vector<int> m_vindexarray;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		x.clear();
		y.clear();
		u.clear();
		v.clear();
		m_vindexarray.clear();
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, fv_it.handle());
			OpenMesh::Vec3d pt;
			pt = pMesh3D->pmesh->point(fv_it.handle());
			x.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D, f_it.handle(), fv_it.handle(), 0));
			y.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D, f_it.handle(), fv_it.handle(), 1));

			u.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D_UV, f_it.handle(), fv_it.handle(), 0));
			v.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D_UV, f_it.handle(), fv_it.handle(), 1));
			//
			//x.push_back(pt[0]);
			//y.push_back(pt[1]);
			//u.push_back(uv[0]);
			//v.push_back(uv[1]);
			m_vindexarray.push_back(fv_it.handle().idx());
		}/*
		 if(std::find(m_vindexarray.begin(),m_vindexarray.end(),52434) != m_vindexarray.end() &&
		 std::find(m_vindexarray.begin(),m_vindexarray.end(),52415) != m_vindexarray.end() &&
		 std::find(m_vindexarray.begin(),m_vindexarray.end(),1) != m_vindexarray.end())
		 {
		 int ij = 0;
		 }//*/
		double dT = (x[0] * y[1] - y[0] * x[1]) + (x[1] * y[2] - y[1] * x[2]) + (x[2] * y[0] - y[2] * x[0]);
		double partialu_partialx = (u[0] * (y[1] - y[2]) + u[1] * (y[2] - y[0]) + u[2] * (y[0] - y[1])) / dT;
		double partialu_partialy = (u[0] * (x[2] - x[1]) + u[1] * (x[0] - x[2]) + u[2] * (x[1] - x[0])) / dT;
		double partialv_partialx = (v[0] * (y[1] - y[2]) + v[1] * (y[2] - y[0]) + v[2] * (y[0] - y[1])) / dT;
		double partialv_partialy = (v[0] * (x[2] - x[1]) + v[1] * (x[0] - x[2]) + v[2] * (x[1] - x[0])) / dT;

		double partialdzbar_real = (partialu_partialx - partialv_partialy) / 2;
		double partialdzbar_imaginary = (partialv_partialx + partialu_partialy) / 2;

		double partialdz_real = (partialu_partialx + partialv_partialy) / 2;
		double partialdz_imaginary = (partialv_partialx - partialu_partialy) / 2;

		double denorminator = SQR(partialdz_real) + SQR(partialdz_imaginary);
		double fai_real = (partialdzbar_real * partialdz_real + partialdzbar_imaginary * partialdz_imaginary) / denorminator;
		double fai_imaginary = (partialdzbar_imaginary * partialdz_real - partialdzbar_real * partialdz_imaginary) / denorminator;
		double fai_mou = sqrt(SQR(fai_real) + SQR(fai_imaginary));
		m_mu.clear();
		m_mu.push_back(fai_real);
		m_mu.push_back(fai_imaginary);
		pMesh3D->pmesh->property(fprop_face_mu, f_it.handle()) = m_mu;

		if (fai_mou > 0.7)
			fai_mou = 0;
		file << fai_mou << "\n";

		if (fai_mou > maxfaimou)
			maxfaimou = fai_mou;
		if (fai_mou < minfaimou)
			minfaimou = fai_mou;
		if (fai_mou > 1)
			m_tongji[10]++;
		else if (fai_mou > 0.9)
			m_tongji[9]++;
		else if (fai_mou > 0.8)
			m_tongji[8]++;
		else if (fai_mou > 0.7)
			m_tongji[7]++;
		else if (fai_mou > 0.6)
			m_tongji[6]++;
		else if (fai_mou > 0.5)
			m_tongji[5]++;
		else if (fai_mou > 0.4)
			m_tongji[4]++;
		else if (fai_mou > 0.3)
			m_tongji[3]++;
		else if (fai_mou > 0.2)
			m_tongji[2]++;
		else if (fai_mou > 0.1)
			m_tongji[1]++;
		else
			m_tongji[0]++;
	}
	file.close();
	double mux, muy;
	int num;
	double totalmulength = 0;
	int vnum = 0;
	std::vector<double> muvec;
	double totalarea;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		mux = 0;
		muy = 0;
		num = 0;
		totalarea = 0;
		for (MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(v_it.handle()); vf_it; vf_it++)
		{
			std::vector<double> mu = pMesh3D->pmesh->property(fprop_face_mu, vf_it.handle());
			if (mu.size() == 2)
			{
				double trianglearea = GetFaceArea(pMesh3D, vf_it.handle());
				mux += mu[0] * trianglearea;
				muy += mu[1] * trianglearea;

				totalarea += trianglearea;
				num++;
			}
		}
		//mux /= num;
		//muy /= num;
		mux /= totalarea;
		muy /= totalarea;
		muvec.clear();
		muvec.push_back(mux);
		muvec.push_back(muy);
		totalmulength += sqrt(SQR(mux) + SQR(muy));
		pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle()) = muvec;
		vnum++;
	}
}
void Actions_ConstrainedRegistration::ComputeVertexMu(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;
	std::ofstream file("D:\\fenbu.txt");
	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	std::vector<double> x, y, u, v;
	double minfaimou = 1e10;
	double maxfaimou = -1e10;
	std::vector<int> m_tongji;
	m_tongji.resize(11, 0);
	std::vector<double> m_mu;
	std::vector<int> m_vindexarray;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		x.clear();
		y.clear();
		u.clear();
		v.clear();
		m_vindexarray.clear();
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, fv_it.handle());
			OpenMesh::Vec3d pt;
			pt = pMesh3D->pmesh->point(fv_it.handle());
			x.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D,f_it.handle(),fv_it.handle(),0));
			y.push_back(Action_Levy_Leastsquare::ComputeProjection(pMesh3D,f_it.handle(),fv_it.handle(),1));
			//
			//x.push_back(pt[0]);
			//y.push_back(pt[1]);
			u.push_back(uv[0]);
			v.push_back(uv[1]);
			m_vindexarray.push_back(fv_it.handle().idx());
		}/*
		 if(std::find(m_vindexarray.begin(),m_vindexarray.end(),52434) != m_vindexarray.end() &&
		 std::find(m_vindexarray.begin(),m_vindexarray.end(),52415) != m_vindexarray.end() &&
		 std::find(m_vindexarray.begin(),m_vindexarray.end(),1) != m_vindexarray.end())
		 {
		 int ij = 0;
		 }//*/
		double dT = (x[0] * y[1] - y[0] * x[1]) + (x[1] * y[2] - y[1] * x[2]) + (x[2] * y[0] - y[2] * x[0]);
		double partialu_partialx = (u[0] * (y[1] - y[2]) + u[1] * (y[2] - y[0]) + u[2] * (y[0] - y[1])) / dT;
		double partialu_partialy = (u[0] * (x[2] - x[1]) + u[1] * (x[0] - x[2]) + u[2] * (x[1] - x[0])) / dT;
		double partialv_partialx = (v[0] * (y[1] - y[2]) + v[1] * (y[2] - y[0]) + v[2] * (y[0] - y[1])) / dT;
		double partialv_partialy = (v[0] * (x[2] - x[1]) + v[1] * (x[0] - x[2]) + v[2] * (x[1] - x[0])) / dT;

		double partialdzbar_real = (partialu_partialx - partialv_partialy) / 2;
		double partialdzbar_imaginary = (partialv_partialx + partialu_partialy) / 2;

		double partialdz_real = (partialu_partialx + partialv_partialy) / 2;
		double partialdz_imaginary = (partialv_partialx - partialu_partialy) / 2;

		double denorminator = SQR(partialdz_real) + SQR(partialdz_imaginary);
		double fai_real = (partialdzbar_real * partialdz_real + partialdzbar_imaginary * partialdz_imaginary) / denorminator;
		double fai_imaginary = (partialdzbar_imaginary * partialdz_real - partialdzbar_real * partialdz_imaginary) / denorminator;
		double fai_mou = sqrt(SQR(fai_real) + SQR(fai_imaginary));
		m_mu.clear();
		m_mu.push_back(fai_real);
		m_mu.push_back(fai_imaginary);
		pMesh3D->pmesh->property(fprop_face_mu, f_it.handle()) = m_mu;
		if (fai_mou > 0.7)
			fai_mou = 0;
		file << fai_mou <<"\n";
		if (fai_mou > maxfaimou)
			maxfaimou = fai_mou;
		if (fai_mou < minfaimou)
			minfaimou = fai_mou;
		if(fai_mou > 1)
		m_tongji[10]++;
		else if(fai_mou > 0.9)
		m_tongji[9]++;
		else if(fai_mou > 0.8)
		m_tongji[8]++;
		else if(fai_mou > 0.7)
		m_tongji[7]++;
		else if(fai_mou > 0.6)
		m_tongji[6]++;
		else if(fai_mou > 0.5)
		m_tongji[5]++;
		else if(fai_mou > 0.4)
		m_tongji[4]++;
		else if(fai_mou > 0.3)
		m_tongji[3]++;
		else if(fai_mou > 0.2)
		m_tongji[2]++;
		else if(fai_mou > 0.1)
		m_tongji[1]++;
		else
		m_tongji[0]++;
	}
	file.close();
	double mux, muy;
	int num;
	double totalmulength = 0;
	int vnum = 0;
	std::vector<double> muvec;
	double totalarea;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		mux = 0;
		muy = 0;
		num = 0;
		totalarea = 0;
		for (MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(v_it.handle()); vf_it; vf_it++)
		{
			std::vector<double> mu = pMesh3D->pmesh->property(fprop_face_mu, vf_it.handle());
			if (mu.size() == 2)
			{
				double trianglearea = GetFaceArea(pMesh3D, vf_it.handle());
				mux += mu[0] * trianglearea;
				muy += mu[1] * trianglearea;

				totalarea += trianglearea;
				num++;
			}
		}
		//mux /= num;
		//muy /= num;
		mux /= totalarea;
		muy /= totalarea;
		muvec.clear();
		muvec.push_back(mux);
		muvec.push_back(muy);
		totalmulength += sqrt(SQR(mux) + SQR(muy));
		pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle()) = muvec;
		vnum++;
	}
}
double Actions_ConstrainedRegistration::Length(std::vector<double> mu)
{
	double len = 0;
	for (int i = 0; i < mu.size(); i++)
		len += SQR(mu[i]);
	len = sqrt(len);
	return len;
}
double Actions_ConstrainedRegistration::Length(Vector3D mu)
{
	double len = 0;
	for (int i = 0; i < 3; i++)
		len += SQR(mu[i]);
	len = sqrt(len);
	return len;
}
void Actions_ConstrainedRegistration::SetVertexColorByMu(CMesh3d* pMesh, CMesh3d* pMesh_UV,CMesh3d_OpenMesh* pMeshCopy)
{
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Vertex mu
	if (!pMeshCopy->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMeshCopy->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	std::vector<double> muvec;
	for (MyMesh::VertexIter v_it = pMeshCopy->pmesh->vertices_begin(); v_it != pMeshCopy->pmesh->vertices_end(); ++v_it)
	{
		muvec = pMeshCopy->pmesh->property(vprop_vertex_mu, v_it.handle());
		double len = Length(muvec);

		int x = len * m_TextureImage.GetWidth();
		if (x >= m_TextureImage.GetWidth())
			x = m_TextureImage.GetWidth() - 1;
		COLORREF color = m_TextureImage.GetPixel(x, 0);

		CVertex3d * pVertex = pMesh_UV->GetVertex(v_it.handle().idx());
		pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));

		CVertex3d * pVertex1 = pMesh->GetVertex(v_it.handle().idx());
		pVertex1->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));

		if (muvec.size() > 0)
		{
			pVertex->mux = muvec[0];
			pVertex->muy = muvec[1];
			pVertex1->mux = muvec[0];
			pVertex1->muy = muvec[1];
		}
		else
		{
			pVertex->mux = 0;
			pVertex->muy = 0;
			pVertex1->mux = 0;
			pVertex1->muy = 0;
		}
	}

	pMesh_UV->m_Modified = 1;
	pMesh->m_Modified = 1;
	pMesh_UV->m_ColorBinding = COLOR_PER_VERTEX;
	pMesh->m_ColorBinding = COLOR_PER_VERTEX;
}

void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadsophiaConformalcircle()
{
	// TODO: Add your control notification handler code here
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circleconformal\\sophie.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circleconformal\\sophiesmile.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circleconformal\\sophie_holomorphic_final_rotate.obj");
	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\sophie.remesh.rotate.landmark14.txt");
	
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circleconformal\\sophiesmile_holomorphic_final_rotate.obj");
	p_FormView3->currentindex = 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\sophie\\Sophie_smile.remesh.rotate.landmark12.txt");
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationMaprectangletocircle()
{
	// TODO: Add your control notification handler code here
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\holomorphic\\sophie_holomorphic.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circletorectangle\\0_776189_2_1\\dense2\\Rectangle.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circletorectangle\\0_776189_2_1\\dense2\\CircleTessallations.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\holomorphic\\sophiesmile_holomorphic.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circletorectangle\\0_792488_1_95\\dense\\Rectangle.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circletorectangle\\0_792488_1_95\\dense\\CircleTessallations.obj");
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	CMesh3d* pMesh3 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(2);

	pMesh1->GetVertex(7133)->SetFlag(1);
	pMesh1->GetVertex(18013)->SetFlag(1);
	//return;
	Point3D lower1, upper1;
	pMesh1->ComputeBoundingBox(lower1, upper1);

	Point3D lower2, upper2;
	pMesh2->ComputeBoundingBox(lower2, upper2);

	/*for (int i = 0; i < pMesh1->NbVertex(); i++)
	{
		//i = 15408;
		CVertex3d* pVertex = pMesh1->GetVertex(i);

		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		x = (x - lower1[0]) / (upper1[0] - lower1[0]);
		y = (y - lower1[1]) / (upper1[1] - lower1[1]);

		x = x * (upper2[0] - lower2[0]) + lower2[0];
		y = y * (upper2[1] - lower2[1]) + lower2[1];
		pVertex->Set(x, y, 0);
	}
	pMesh1->SetModified(1);
	pDoc->UpdateAllViews(NULL);
	return;*/
	BOOL b;
	for (int i = 0; i < pMesh1->NbVertex(); i++)
	{
		//i = 15408;
		CVertex3d* pVertex = pMesh1->GetVertex(i);

		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		x = (x - lower1[0]) / (upper1[0] - lower1[0]);
		y = (y - lower1[1]) / (upper1[1] - lower1[1]);

		x = x * (upper2[0] - lower2[0]) + lower2[0];
		y = y * (upper2[1] - lower2[1]) + lower2[1];

		//Search the triangle.
		
		pMesh2->SetVertexFlagAsItsIndex();
		double maxval = -1e10;
		int maxindex;
		BOOL b;
		b = FALSE;
		for (int j = 0; j < pMesh2->m_ArrayFace.GetSize(); j++)
		{
			CFace3d* pFace3d = pMesh2->GetFace(j);

			//BOOL b = pTreeView->JudgeWhetherPointLiesOnFace(pFace3d);

			b = pTreeView->JudgeWhetherPointLiesInsideTriangle(x, y, pFace3d->v1()->x(), pFace3d->v1()->y(), pFace3d->v2()->x(), pFace3d->v2()->y(), pFace3d->v3()->x(), pFace3d->v3()->y());
			//if (b)
			//	break;
			//double val = pTreeView->JudgeWhetherPointLiesInsideTriangle_double(x, y, pFace3d->v1()->x(), pFace3d->v1()->y(), pFace3d->v2()->x(), pFace3d->v2()->y(), pFace3d->v3()->x(), pFace3d->v3()->y());
			//if (val > maxval)
			//{
			//	maxval = val;
			//	maxindex = j;
			//}
			if (b)
			{
				//CFace3d* pFace3d = pMesh2->GetFace(maxindex);
				CFace3d* pFace3d = pMesh2->GetFace(j);
				//Compute the weight
				double w1 = pTreeView->TriangleArea(x, y, pFace3d->v2()->x(), pFace3d->v2()->y(), pFace3d->v3()->x(), pFace3d->v3()->y());
				double w2 = pTreeView->TriangleArea(x, y, pFace3d->v1()->x(), pFace3d->v1()->y(), pFace3d->v3()->x(), pFace3d->v3()->y());
				double w3 = pTreeView->TriangleArea(x, y, pFace3d->v1()->x(), pFace3d->v1()->y(), pFace3d->v2()->x(), pFace3d->v2()->y());

				double w = w1 + w2 + w3;
				w1 = w1 / w;
				w2 = w2 / w;
				w3 = w3 / w;

				CFace3d* pFace3d1 = pMesh3->GetFace(j);
				double newx = pFace3d1->v1()->x() * w1 + pFace3d1->v2()->x() * w2 + pFace3d1->v3()->x() * w3;
				double newy = pFace3d1->v1()->y() * w1 + pFace3d1->v2()->y() * w2 + pFace3d1->v3()->y() * w3;

				pVertex->Set(newx, newy, 0);
				break;
			}
		}

		//if (maxval > 0)
		
		//else
		if(!b)
		{
			int selectindex = -1;
			double mindis = 1e10;
			for (int i = 0; i < pMesh2->NbVertex(); i++)
			{
				CVertex3d* pVertex = pMesh2->GetVertex(i);
				double val = SQR(x - pVertex->x()) + SQR(y - pVertex->y());
				if ( val < mindis)
				{
					mindis = val;
					selectindex = i;
				}
			}
			//if (mindis < 1e-5)
			//{
				pVertex->Set(pMesh3->GetVertex(selectindex)->x(), pMesh3->GetVertex(selectindex)->y(), 0);
				//break;
			//}
			//else
			//	ASSERT(0);
		}
		
	}

	//Project to the unit circle
	CMesh3d_OpenMesh* pMesh1_Openmesh = pMesh1->Convert_OpenMesh();
	for (MyMesh::VertexIter v_it = pMesh1_Openmesh->pmesh->vertices_begin(); v_it != pMesh1_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh1_Openmesh->pmesh->is_boundary(v_it.handle()))
		{
			CVertex3d* pVertex = pMesh1->GetVertex(v_it.handle().idx());
			double x = pVertex->x();
			double y = pVertex->y();

			double len = sqrt(x * x + y * y);
			x = x / len;
			y = y / len;
			pVertex->Set(x, y, 0);
		}
	}
	
	pMesh1->SetModified(1);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalparameterizationCircleHolomorphic()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 0);

	//Load the parameterization reference
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circleconformal\\sophie_holomorphic.obj");

	CMesh3d* pMesh1_Circle_Holomorphic = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh1->pBoundaryMeshReference = pMesh1_Circle_Holomorphic->Convert_OpenMesh();

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circleconformal\\sophiesmile_holomorphic.obj");

	CMesh3d* pMesh2_Circle_Holomorphic = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2->pBoundaryMeshReference = pMesh2_Circle_Holomorphic->Convert_OpenMesh();

	//Map The meshes to 2D
	MapTo2D(pOpenMesh1, pOpenMesh2, 1);

	//Load Landmark on the parameterization
	LoadLandmarkOnParameterization(pDoc->m_SceneGraph.NbObject() - 2, pDoc->m_SceneGraph.NbObject() - 1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleHolomorphic()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	//Load the parameterization reference
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circleconformal\\sophie_holomorphic.obj");

	CMesh3d* pMesh1_Circle_Holomorphic = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh1->pBoundaryMeshReference = pMesh1_Circle_Holomorphic->Convert_OpenMesh();

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circleconformal\\sophiesmile_holomorphic.obj");

	CMesh3d* pMesh2_Circle_Holomorphic = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2->pBoundaryMeshReference = pMesh2_Circle_Holomorphic->Convert_OpenMesh();

	//Map The meshes to 2D
	MapTo2D(pOpenMesh1, pOpenMesh2, 1);

	//Load Landmark on the parameterization
	LoadLandmarkOnParameterization(pDoc->m_SceneGraph.NbObject() - 2, pDoc->m_SceneGraph.NbObject() - 1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata()
{
	// TODO: Add your control notification handler code here
	//Type_Parameterdomain 0 rectangle, 1 circle
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\fe\\F0001_FE01WH_F3D_texture.m");

	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\fe\\F0001_FE01WH_F3D_landmark_changesequence.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7454);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7525);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7613);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);

	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\su\\F0001_SU02WH_F3D_fixed_texture1.m");

	p_FormView3->currentindex = 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();


	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\su\\F0001_SU02WH_F3D_landmark_changesequence1.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7020);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7405);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7106);
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(21348);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Holomorphic()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\holomorphic\\fear_rotate.m");

	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\F0001_FE01WH_F3D_landmark_changesequence.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7454);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7525);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7613);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);

	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\holomorphic\\surprise_rotate.m");

	p_FormView3->currentindex = 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();


	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\F0001_SU02WH_F3D_landmark_changesequence1.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7019);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7317);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7402);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7105);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningRectangleHolomorphicBudata()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);
	LoadBudataModel(pOpenMesh1, pOpenMesh2, 0, 1);

	//Load the parameterization reference
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\holomorphic\\fear_rotate.m");

	CMesh3d* pMesh1_Circle_Holomorphic = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh1->pBoundaryMeshReference = pMesh1_Circle_Holomorphic->Convert_OpenMesh();

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\holomorphic\\surprise_rotate.m");

	CMesh3d* pMesh2_Circle_Holomorphic = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2->pBoundaryMeshReference = pMesh2_Circle_Holomorphic->Convert_OpenMesh();

	//Map The meshes to 2D
	MapTo2D(pOpenMesh1, pOpenMesh2, 0);

	//Load Landmark on the parameterization
	//LoadLandmarkOnParameterization(pDoc->m_SceneGraph.NbObject() - 2, pDoc->m_SceneGraph.NbObject() - 1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void Actions_ConstrainedRegistration::OnBnClickedLoadexample2Straightening_Constrainedregistration()
{
	CMesh3d* pMesh1, *pMesh2;
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\straighteningresult\\fear.obj");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\F0001_FE01WH_F3D_landmark_changesequence.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7454);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7525);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7613);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);

	pPolyOmino->pMesh->hasvt = 1;
	pMesh1 = pPolyOmino->pMesh;

	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\straighteningresult\\surprise.obj");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();


	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\F0001_SU02WH_F3D_landmark_changesequence1.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7019);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7317);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7402);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7105);
	pMesh2 = pPolyOmino->pMesh;

	//
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7019)->x());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7019)->y());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7317)->x());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7317)->y());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7402)->x());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7402)->y());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7105)->x());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7105)->y());
}
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Straightening()
{
	CMesh3d* pMesh1, *pMesh2;
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\straighteningresult\\fear.obj");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\F0001_FE01WH_F3D_landmark_changesequence.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7454);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7525);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7613);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);
	pMesh1 = pPolyOmino->pMesh;
	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\straighteningresult\\surprise.obj");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pMesh2 = pPolyOmino->pMesh;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\F0001_SU02WH_F3D_landmark_changesequence1.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7019);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7317);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7402);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7105);

	//
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7019)->x());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7019)->y());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7317)->x());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7317)->y());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7402)->x());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7402)->y());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7105)->x());
	pMesh1->m_FixVerticesArray_uv.push_back(pMesh2->GetVertex(7105)->y());
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonicBudata()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//LoadSophiaStraighteningModel(pOpenMesh1, pOpenMesh2);
	OnBnClickedLoadexample2Straightening_Constrainedregistration();

	//Convert to Openmesh
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	pOpenMesh1 = pMesh1->Convert_OpenMesh();
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2 = pMesh2->Convert_OpenMesh();

	//
	Bijective2DMapping_Initialize(pOpenMesh1, pOpenMesh2);

	pHarmonicMapping->TwoDHarmonicStraightening_FixLandmarkEndPoints(pOpenMesh1);

	pHarmonicMapping->GenerateParameterDomain(pOpenMesh1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonicBudataBijectiveregistration()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;
	//
	OnBnClickedConstrainedregistrationLoadexample2Straightening();

	//Convert to Openmesh
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	pOpenMesh1 = pMesh1->Convert_OpenMesh();
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2 = pMesh2->Convert_OpenMesh();

	//
	Bijective2DMapping(pOpenMesh1, pOpenMesh2);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2RegistrationFlip()
{
	// TODO: Add your control notification handler code here
	CMesh3d* pMesh1, *pMesh2;
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\initialregistration\\registration_flip.obj");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\F0001_FE01WH_F3D_landmark_changesequence.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7454);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7525);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7613);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);

	pPolyOmino->pMesh->hasvt = 1;
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataAn()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\AN\\F0001_AN01WH_F3D_texture.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\AN\\F0001_AN01WH_F3D.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7699);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\AN\\F0001_AN01WH_F3D.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataDi()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\DI\\F0001_DI01WH_F3D_texture.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\DI\\F0001_DI01WH_F3D.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7526);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\DI\\F0001_DI01WH_F3D.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataFe()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\FE\\F0001_FE01WH_F3D_texture.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\FE\\F0001_FE01WH_F3D.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7498);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\FE\\F0001_FE01WH_F3D.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataHa()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\HA\\F0001_HA01WH_F3D_texture.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\HA\\F0001_HA01WH_F3D.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(6945);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\HA\\F0001_HA01WH_F3D.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataNe()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\NE\\F0001_NE00WH_F3D_texture.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\NE\\F0001_NE00WH_F3D.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7560);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\NE\\F0001_NE00WH_F3D.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataSa()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\SA\\F0001_SA01WH_F3D_texture.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\SA\\F0001_SA01WH_F3D.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7082);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\SA\\F0001_SA01WH_F3D.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataSu()
{
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\SU\\F0001_SU02WH_F3D_fixed_texture.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\SU\\F0001_SU02WH_F3D.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7306);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\SU\\F0001_SU02WH_F3D.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata2()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//Type_Parameterdomain 0 rectangle, 1 circle
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\fe\\F0001_FE01WH_F3D_texture.m");

	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\fe\\F0001_FE01WH_F3D_landmark_changesequence.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7454);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7525);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7613);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);

	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\fe\\F0002_FE01BL_F3D.m");

	p_FormView3->currentindex = 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();


	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\fe\\F0001_SU02WH_F3D_landmark_changesequence1.txt");
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7020);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7405);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7106);
}

void Actions_ConstrainedRegistration::LoadFeaturePoints(CString FileName)
{
	std::ifstream file(FileName);
	int index;
	double x, y, z;
	//
	m_ArrayFeatureArrayPoints.clear();

	//for (int i = 0; i < pPolyOmino->pMesh->NbVertex(); i++)
	//	pPolyOmino->pMesh->GetVertex(i)->SetFlag(0);

	int vindex = 1;
	while (!file.eof())
	{
		file >> index;
		file >> x >> y >> z;
		m_ArrayFeatureArrayPoints.push_back(index);
		//pPolyOmino->pMesh->GetVertex(index)->SetFlag(vindex);
		pPolyOmino->pMesh->m_ArrayFeatureVertexIndex.push_back(index);
		vindex++;
	}
	file.close();
}
void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadfeaturepoints()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;

	LoadFeaturePoints(FileName);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadfeaturepointsLandmark()
{
	// TODO: Add your control notification handler code here
	std::vector<std::vector<int>> m_ArrayLandmarkPairs;
	std::vector<int> landmark;
	landmark.push_back(21);
	landmark.push_back(20);
	landmark.push_back(19);
	landmark.push_back(18);
	landmark.push_back(17);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(31);
	landmark.push_back(30);
	landmark.push_back(29);
	landmark.push_back(28);
	landmark.push_back(27);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(4);
	landmark.push_back(3);
	landmark.push_back(2);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(12);
	landmark.push_back(11);
	landmark.push_back(10);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(8);
	landmark.push_back(7);
	landmark.push_back(6);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(14);
	landmark.push_back(15);
	landmark.push_back(16);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(37);
	landmark.push_back(38);
	landmark.push_back(39);
	landmark.push_back(40);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(48);
	landmark.push_back(47);
	landmark.push_back(46);
	landmark.push_back(45);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(44);
	landmark.push_back(43);
	landmark.push_back(42);
	landmark.push_back(41);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(50);
	landmark.push_back(51);
	landmark.push_back(52);
	landmark.push_back(53);
	landmark.push_back(54);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(56);
	landmark.push_back(57);
	landmark.push_back(58);
	landmark.push_back(59);
	landmark.push_back(60);
	m_ArrayLandmarkPairs.push_back(landmark);
	
	std::vector<int> path;
	GeoMapLandMark* pLandmark;
	for (int i = 0; i < m_ArrayLandmarkPairs.size(); i++)
	{
		landmark = m_ArrayLandmarkPairs[i];
		pLandmark = new GeoMapLandMark();
		
		pLandmark->landmark_points.push_back(m_ArrayFeatureArrayPoints[landmark[0] - 1]);
		for (int j = 0; j < landmark.size() - 1; j++)
		{
			path.clear();
			int index1 = m_ArrayFeatureArrayPoints[landmark[j] - 1];
			int index2 = m_ArrayFeatureArrayPoints[landmark[j + 1] - 1];

			pPolyOmino->pMesh->ComputeShortestPath(index2, index1, path);

			for (int k = 1; k < path.size(); k++)
			{
				pLandmark->landmark_points.push_back(path[k]);
			}
		}
		pPolyOmino->pMesh->m_LandMarkArray.push_back(pLandmark);
	}
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata3()
{
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\F0001_NE00WH_F3D1.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\F0001_NE00WH_F3D2.txt");
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7454);
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7525);
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7613);
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7355);

	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\M0004_NE00WH_F3D3.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();


	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\M0004_NE00WH_F3D5.txt");
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7020);
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7320);
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7405);
	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7106);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7634);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataAn2()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\F0001_NE00WH_F3D1.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\F0001_NE00WH_F3D2.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7504);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\F0001_NE00WH_F3D2.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataAn3()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\M0004_NE00WH_F3D3.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\M0004_NE00WH_F3D5.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7411);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\M0004_NE00WH_F3D5.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConstrainedharmonicSophia2()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;

	//Type_Parameterdomain 0 rectangle, 1 circle
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\source_straightening.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraightening_holomorphic\\sophie.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\F0001_NE00WH_F3D_straightening.m");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\F0001_NE00WH_F3D2.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(14668);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7504);


	pPolyOmino->pMesh->hasvt = 1;
	//RotateMesh(pPolyOmino->pMesh);
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraighteningresult\\target_straightening.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\sophie\\circlestraightening_holomorphic\\sophiesmile.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\M0004_NE00WH_F3D_straightening.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();


	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\M0004_NE00WH_F3D5.txt");

	pPolyOmino->pMesh->m_FixVerticesArray.push_back(7411);

	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(0);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(1);
	pPolyOmino->pMesh->m_FixVerticesArray_uv.push_back(-1e-8);
	//Convert to Openmesh
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	pOpenMesh1 = pMesh1->Convert_OpenMesh();
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pOpenMesh2 = pMesh2->Convert_OpenMesh();

	//Compute bijective mapping between pOpenmesh3 and pOpenmesh4
	Bijective2DMapping_Budata(pOpenMesh1, pOpenMesh2);
	//LoadLandmarkOnParameterization(pDoc->m_SceneGraph.NbObject() - 2, pDoc->m_SceneGraph.NbObject() - 1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata4()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;
	//
	pDoc->OnOpenDocument(FileName);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFeatureVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pPolyOmino->pMesh->hasvt = 1;

	CString FilebndName,landmarkname,Mfile;
	CString extension = FileName;
	extension = extension.Left(FileName.ReverseFind('.'));
	extension.MakeLower();
	landmarkname = extension + ".txt";
	FilebndName = extension + ".bnd";
	Mfile = extension + "_para.m";
	LoadFeaturePoints(FilebndName);

	pPolyOmino->GenerateLandMarks();

	int index = m_ArrayFeatureArrayPoints[75];
	CMesh3d_OpenMesh* pOpenmesh = pPolyOmino->pMesh->Convert_OpenMesh();
	double x = pPolyOmino->pMesh->GetVertex(index)->x();
	double y = pPolyOmino->pMesh->GetVertex(index)->y();
	double z = pPolyOmino->pMesh->GetVertex(index)->z();
	double mindis = 1e10;
	int minindex;
	for (MyMesh::VertexIter v_it = pOpenmesh->pmesh->vertices_begin(); v_it != pOpenmesh->pmesh->vertices_end(); ++v_it)
	{
		if (pOpenmesh->pmesh->is_boundary(v_it.handle()))
		{
			OpenMesh::Vec3d startpoint = pOpenmesh->pmesh->point(v_it.handle());
			double dis = SQR(x - startpoint[0]) + SQR(y - startpoint[1]) + SQR(z - startpoint[2]);
			if (dis < mindis)
			{
				mindis = dis;
				minindex = v_it.handle().idx();
			}
		}
	}
	pOpenmesh->m_FixVerticesArray.push_back(minindex);

	CMesh3d* pOldMesh = pPolyOmino->pMesh;
	pHarmonicMapping->HarmonicStraightening_VerticalPoint(pOpenmesh, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_LandMarkArray = pOldMesh->m_LandMarkArray;
	pPolyOmino->SaveLandmark(landmarkname);
	char message[80];
	sprintf(message, "%s", Mfile);
	pPolyOmino->pMesh->SaveMFile(message);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	//pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\M0004_NE00WH_F3D5.txt");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationLoadexample2Budata5()
{
	// TODO: Add your control notification handler code here
	std::vector<std::vector<int>> m_ArrayLandmarkPairs;
	std::vector<int> landmark;
	landmark.push_back(21);
	landmark.push_back(20);
	landmark.push_back(19);
	landmark.push_back(18);
	landmark.push_back(17);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(31);
	landmark.push_back(30);
	landmark.push_back(29);
	landmark.push_back(28);
	landmark.push_back(27);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(4);
	landmark.push_back(3);
	landmark.push_back(2);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(12);
	landmark.push_back(11);
	landmark.push_back(10);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(8);
	landmark.push_back(7);
	landmark.push_back(6);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(14);
	landmark.push_back(15);
	landmark.push_back(16);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(37);
	landmark.push_back(38);
	landmark.push_back(39);
	landmark.push_back(40);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(48);
	landmark.push_back(47);
	landmark.push_back(46);
	landmark.push_back(45);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(44);
	landmark.push_back(43);
	landmark.push_back(42);
	landmark.push_back(41);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(50);
	landmark.push_back(51);
	landmark.push_back(52);
	landmark.push_back(53);
	landmark.push_back(54);
	m_ArrayLandmarkPairs.push_back(landmark);
	landmark.clear();
	landmark.push_back(56);
	landmark.push_back(57);
	landmark.push_back(58);
	landmark.push_back(59);
	landmark.push_back(60);
	m_ArrayLandmarkPairs.push_back(landmark);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	std::ofstream file("D:\\signature.txt");
	for (int i = 0; i < m_ArrayLandmarkPairs.size(); i++)
	{
		landmark = m_ArrayLandmarkPairs[i];
		int start = landmark[0];
		int end = landmark[landmark.size() - 1];

		double x1 = pPolyOmino->pMesh->GetVertex(start)->x();
		double y1 = pPolyOmino->pMesh->GetVertex(start)->y();
		double z1 = pPolyOmino->pMesh->GetVertex(start)->z();

		double x2 = pPolyOmino->pMesh->GetVertex(end)->x();
		double y2 = pPolyOmino->pMesh->GetVertex(end)->y();
		double z2 = pPolyOmino->pMesh->GetVertex(end)->z();

		double len = sqrt(SQR(x1 - x2) + SQR(y1 - y2));
		//file << x1 << " " << y1 << " " << x2 << " " << y2 << " ";
		file << len <<" ";
		//TRACE("%f %f   %f %f        ", x1, y1, x2, y2);
	}
	file.close();
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationConformalstraighteningCircleBrain()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pOpenMesh1, *pOpenMesh2;
	CMesh3d* pStartMesh, *pEndMesh;
	//Load sophia model
	//LoadSophiaModel(pOpenMesh1, pOpenMesh2, 1, 1);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\brain\\fromwei\\2713_0.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pStartMesh = pPolyOmino->pMesh;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\brain\\fromwei\\2713_0.txt");

	//pPolyOmino->pMesh->m_FixVerticesArray.push_back(7895);
	pPolyOmino->pMesh->m_FixVerticesArray.push_back(69568);

	pPolyOmino->pMesh->hasvt = 1;
	pOpenMesh1 = pPolyOmino->pMesh->Convert_OpenMesh();
	//Map The meshes to 2D
	//MapTo2D(pOpenMesh1, pOpenMesh2, 1);
	pHarmonicMapping->HarmonicStraightening(pOpenMesh1, 1);

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->SetCurrentMesh(1);
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = TRUE;
	pEndMesh = pPolyOmino->pMesh;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\brain\\fromwei\\2713_0.txt");

	pEndMesh->CopyVertexRegion(pStartMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationMeshnormalization()
{
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	//CMesh3d_OpenMesh* pOpenMesh1 = pMesh1->Convert_OpenMesh();
	CMesh3d* pMesh2 = new CMesh3d();
	pMesh2->Copy(pMesh1);

	double x0, y0, theta;
	//x0,y0 are the center, theta are the angles
	x0 = 0.040848;
	y0 = -0.1750;
	theta = 0;
	for (int i = 0; i < pMesh2->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh2->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		
		double a0 = (x - x0) * cos(theta) - sin(theta) * (y - y0);
		double b0 = (x - x0) * sin(theta) + cos(theta) * (y - y0);
		double a1 = 1 - x * x0 - y * y0;
		double b1 = x0 * y - x * y0;

		double x1 = (a0 * a1 - b0 * b1) / (SQR(a1) + SQR(b1));
		double y1 = (a0 * b1 + a1 * b0) / (SQR(a1) + SQR(b1));

		pVertex->Set(x1, y1, 0);
	}

	pDoc->m_SceneGraph.Add(pMesh2);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationDeteletefeaturepoints()
{
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	double maxz = -1e10;
	int maxindex;
	for (int i = 0; i < pMesh1->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh1->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();
		
		//find the maximal z
		if (z > maxz)
		{
			maxz = z;
			maxindex = i;
		}
	}
	pMesh1->m_ArraySharpVertexIndex.push_back(maxindex);

	//
	p_FormView3->currentindex = 0;
	pPolyOmino->SetCurrentMesh(1);

	pPolyOmino->GenerateLandMarks();
	std::pair<int, double> m_LandmarkEndPoint;
	std::vector<std::pair<int, double>> m_ArrayLandmarkEndPoints;
	for (int i = 0; i < pMesh1->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh1->m_LandMarkArray[i];
		m_LandmarkEndPoint.first = pLandmark->landmark_points[0];
		m_LandmarkEndPoint.second = pMesh1->GetVertex(m_LandmarkEndPoint.first)->y();
		m_ArrayLandmarkEndPoints.push_back(m_LandmarkEndPoint);

		m_LandmarkEndPoint.first = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
		m_LandmarkEndPoint.second = pMesh1->GetVertex(m_LandmarkEndPoint.first)->y();
		m_ArrayLandmarkEndPoints.push_back(m_LandmarkEndPoint);
	}
	struct sort_pred {
		bool operator()(const std::pair<int, double> &left, const std::pair<int, double> &right) {
			return left.second > right.second;
		}
	};

	std::sort(m_ArrayLandmarkEndPoints.begin(), m_ArrayLandmarkEndPoints.end(), sort_pred());
	std::vector<std::pair<int, double>> m_ArrayLandmarkEndPointsX;

	for (int j = 0; j < 4; j++)
	{
		m_LandmarkEndPoint = m_ArrayLandmarkEndPoints[j];
		m_LandmarkEndPoint.second = pMesh1->GetVertex(m_LandmarkEndPoint.first)->x();
		m_ArrayLandmarkEndPointsX.push_back(m_LandmarkEndPoint);
	}

	std::sort(m_ArrayLandmarkEndPointsX.begin(), m_ArrayLandmarkEndPointsX.end(), sort_pred());
	pMesh1->m_ArraySharpVertexIndex.push_back(m_ArrayLandmarkEndPointsX[1].first);
	pMesh1->m_ArraySharpVertexIndex.push_back(m_ArrayLandmarkEndPointsX[2].first);
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationBrainConformalparameterizationCircle2()
{
	CMesh3d_OpenMesh* pOpenMesh1 = NULL, *pOpenMesh2 = NULL;

	//Load Brain Model
	LoadBrainModel_1(pOpenMesh1, pOpenMesh2, 1, 1);

	//Map The meshes to 2D
	MapTo2D(pOpenMesh1, pOpenMesh2, 1);

	//Load Landmark on the parameterization
	LoadLandmarkOnParameterization_OnBrain_1(pOpenMesh1, pOpenMesh2);

	//return;
	//return;
	CMesh3d* pMesh3d1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 4);
	CMesh3d* pMesh3d2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 3);

	pMesh3d2->m_FixVerticesArray_uv.push_back(1);
	pMesh3d2->m_FixVerticesArray_uv.push_back(0);
	pMesh3d2->m_FixVerticesArray_uv.push_back(1);
	pMesh3d2->m_FixVerticesArray_uv.push_back(-1e-8);

	CMesh3d_OpenMesh* pMesh3d1_openmesh = pMesh3d1->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh3d2_openmesh = pMesh3d2->Convert_OpenMesh();

	Bijective2DMapping_Budata(pMesh3d1_openmesh, pMesh3d2_openmesh);

	//pDynamicQuasiconformal->pconvex_para_Openmesh  = pMesh3d1->Convert_OpenMesh();
	//pDynamicQuasiconformal->ptarget_para_Openmesh = pMesh3d2->Convert_OpenMesh();

	//pDynamicQuasiconformal->OnBnClickedDynamicDynamicharmonic();

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistration1627brainstrainghtening()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyimagetosourcemesh()
{
	CopyImageColorToTargetMesh(pMesh);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetnormalascolor()
{
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}

	CMesh3d* pMeshCopy = NULL;
	//CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();

	CImage m_TextureImage;

	//m_TextureImage.Load("D:\\1.png");
	double x, y,z;
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		//CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		pMesh3d->CalculateNormalPerVertex();
		for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);
			//x = pVertex->m_Normal.x();
			//y = pVertex->m_Normal.y();
			//z = pVertex->m_Normal.z();
			x = pVertex->m_Normal.x();
			y = pVertex->m_Normal.y();
			z = pVertex->m_Normal.z();
			//pVertex->SetColor((pVertex->x() + 1) * 255 / 2, (pVertex->y() + 1) * 255 / 2, (pVertex->z() + 1) * 255 / 2);
			pVertex->SetColor((x) * 255, (y) * 255, (z) * 255);
			//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			//if (pVertex->x() > 0 && pVertex->x() < 1)
			
			//pVertex_uv->SetColor(*pVertex->GetColor());
			//pVertex->u = pVertex_uv->x();
			//pVertex->v = pVertex_uv->y();
		}
		pMesh3d->m_Modified = 1;
		pMesh3d->m_ColorBinding = COLOR_PER_VERTEX;
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		//if (!pMeshUVCopy)
		//	delete pMeshUVCopy;
		return;
	}
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetcurvaturelascolor()
{
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}


	CMesh3d* pMeshCopy = NULL;
	//CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();


	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	double x, y, z;
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		CMesh3d_OpenMesh* pOpenmesh = pMesh3d->Convert_OpenMesh();
		pOpenmesh->CalculateLaplaceBeltramiOperator();
		pOpenmesh->CopyLaplaceOperator(pMesh3d);
		delete pOpenmesh;

		//pMesh3d->CalculateLaplaceBeltramiOperator();
		double minimalmeancurvature = pMesh3d->CalculateMinimalMeanCurvature();
		double maximalmeancurvature = pMesh3d->CalculateMaximalMeanCurvature();
		maximalmeancurvature = 20;
		//CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		//pMesh3d->CalculateNormalPerVertex();
		for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);
			//x = pVertex->m_Normal.x();
			//y = pVertex->m_Normal.y();
			//z = pVertex->m_Normal.z();
			double meanvaluecurvature = Length(pVertex->m_LaplaceOperatorVec) / 2;

			double ratio = (meanvaluecurvature - minimalmeancurvature) / (maximalmeancurvature - minimalmeancurvature);

			if (ratio > 1)
				ratio = 1;

			int x = ratio * m_TextureImage.GetWidth();
			if (x >= m_TextureImage.GetWidth())
				x = m_TextureImage.GetWidth() - 1;
			COLORREF color = m_TextureImage.GetPixel(x, 0);

			pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
			//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			//if (pVertex->x() > 0 && pVertex->x() < 1)

			//pVertex_uv->SetColor(*pVertex->GetColor());
			//pVertex->u = pVertex_uv->x();
			//pVertex->v = pVertex_uv->y();
		}
		pMesh3d->m_Modified = 1;
		pMesh3d->m_ColorBinding = COLOR_PER_VERTEX;
		pDoc->m_Light = 0;
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		//if (!pMeshUVCopy)
		//	delete pMeshUVCopy;
		return;
	}
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSetgausscurvaturelascolor()
{
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}

	CMesh3d* pMeshCopy = NULL;
	//CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();


	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	double x, y, z;
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		CMesh3d_OpenMesh* pOpenmesh = pMesh3d->Convert_OpenMesh();
		pOpenmesh->CalculateGaussCurvature();
		pOpenmesh->CopyGaussCurvature(pMesh3d);
		delete pOpenmesh;
		//pMesh3d->CalculateGaussCurvature();
		double minimalcurvature = pMesh3d->CalculateMinimalGaussCurvature();
		double maximalcurvature = pMesh3d->CalculateMaximalGaussCurvature();
		minimalcurvature = -4;
		maximalcurvature = 1;
		//CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		//pMesh3d->CalculateNormalPerVertex();
		for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);
			//x = pVertex->m_Normal.x();
			//y = pVertex->m_Normal.y();
			//z = pVertex->m_Normal.z();
			double curvature = pVertex->m_GaussCurvature;

			double ratio = (curvature - minimalcurvature) / (maximalcurvature - minimalcurvature);

			int x = ratio * m_TextureImage.GetWidth();
			if (x >= m_TextureImage.GetWidth())
				x = m_TextureImage.GetWidth() - 1;
			if (x < 0)
				x = 0;
			COLORREF color = m_TextureImage.GetPixel(x, 0);

			pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
			//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			//if (pVertex->x() > 0 && pVertex->x() < 1)

			//pVertex_uv->SetColor(*pVertex->GetColor());
			//pVertex->u = pVertex_uv->x();
			//pVertex->v = pVertex_uv->y();
		}
		pMesh3d->m_Modified = 1;
		pMesh3d->m_ColorBinding = COLOR_PER_VERTEX;
		pDoc->m_Light = 0;
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		//if (!pMeshUVCopy)
		//	delete pMeshUVCopy;
		return;
	}
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationCopyimagetomesh()
{
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}

	CMesh3d* pMeshCopy = NULL;
	//CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();


	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\2019\\iccv2019\\color_1.PNG");

	double x, y, z;
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		//pMesh3d->CalculateGaussCurvature();
		//double minimalcurvature = pMesh3d->CalculateMinimalGaussCurvature();
		//double maximalcurvature = pMesh3d->CalculateMaximalGaussCurvature();
		//CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
		//pMesh3d->CalculateNormalPerVertex();
		Point3D lower, upper;
		pMesh3d->ComputeBoundingBox(lower, upper);

		for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);

			//x = pVertex->m_Normal.x();
			//y = pVertex->m_Normal.y();
			//z = pVertex->m_Normal.z();
			//double curvature = pVertex->m_GaussCurvature;

			//double ratio = (curvature - minimalcurvature) / (maximalcurvature - minimalcurvature);

			//int x = ratio * m_TextureImage.GetWidth();
			//if (x >= m_TextureImage.GetWidth())
			//	x = m_TextureImage.GetWidth() - 1;
			//if (x < 0)
			//	x = 0;
			double xratio = (pVertex->x() - lower[0]) / (upper[0] - lower[0]);
			double yratio = (pVertex->y() - lower[1]) / (upper[1] - lower[1]);
			int x = xratio * m_TextureImage.GetWidth();
			int y = yratio * m_TextureImage.GetHeight();

			if (x >= m_TextureImage.GetWidth())
				x = m_TextureImage.GetWidth() - 1;
			if (y >= m_TextureImage.GetHeight())
				y = m_TextureImage.GetHeight() - 1;

			COLORREF color = m_TextureImage.GetPixel(x, y);

			pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
			//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			//if (pVertex->x() > 0 && pVertex->x() < 1)

			//pVertex_uv->SetColor(*pVertex->GetColor());
			//pVertex->u = pVertex_uv->x();
			//pVertex->v = pVertex_uv->y();
		}
		pMesh3d->m_Modified = 1;
		pMesh3d->m_ColorBinding = COLOR_PER_VERTEX;
		sprintf(Post_Message, "Type: both mesh3d, Set parameterization Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		//if (!pMeshUVCopy)
		//	delete pMeshUVCopy;
		return;
	}
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSmoothgausscurvature()
{
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	CMesh3d* pMeshCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();

	CMesh3d* pMesh3d = (CMesh3d*)pMesh;
	pMesh3d->SmoothGaussCurvature();

	double minimalcurvature = -4;
	double maximalcurvature = 1;
	//CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
	//pMesh3d->CalculateNormalPerVertex();
	for (int i = 0; i < pMesh3d->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3d->GetVertex(i);
		//x = pVertex->m_Normal.x();
		//y = pVertex->m_Normal.y();
		//z = pVertex->m_Normal.z();
		double curvature = pVertex->m_GaussCurvature;

		double ratio = (curvature - minimalcurvature) / (maximalcurvature - minimalcurvature);

		int x = ratio * m_TextureImage.GetWidth();
		if (x >= m_TextureImage.GetWidth())
			x = m_TextureImage.GetWidth() - 1;
		if (x < 0)
			x = 0;
		COLORREF color = m_TextureImage.GetPixel(x, 0);

		pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
		//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
		//if (pVertex->x() > 0 && pVertex->x() < 1)

		//pVertex_uv->SetColor(*pVertex->GetColor());
		//pVertex->u = pVertex_uv->x();
		//pVertex->v = pVertex_uv->y();
	}

	pMesh3d->m_Modified = 1;
	pMesh3d->m_ColorBinding = COLOR_PER_VERTEX;
	if (!pMeshCopy)
		delete pMeshCopy;
	pDoc->UpdateAllViews(NULL);
}
//double ComputeMinimalDistance_NearestPoint(CMesh3d* pMesh3d, int vstartindex1, m_TargetVertex);
void FindPoint(CMesh3d* pMesh3d, std::vector<int> m_TargetVertex)
{
	std::vector<int> m_anticlockwise;
	std::vector<int> m_clockwise;
	//int SHORTEST1 = pMesh3d->ComputeWeightedShortestPath(26506, 26462, m_TargetVertex);
	int vstartindex1, vstartindex2;
	std::vector<int> path, path2;
	vstartindex1 = 26550;
	vstartindex2 = 26561;

	pMesh3d->GetVertex(26550)->m_Flag = 0;
	pMesh3d->GetVertex(26561)->m_Flag = 0;

	pMesh3d->m_ArraySharpEdges.push_back(std::make_pair(vstartindex1, vstartindex2));

	m_anticlockwise.push_back(vstartindex1);
	m_clockwise.push_back(vstartindex2);

	auto it1 = std::find(m_TargetVertex.begin(), m_TargetVertex.end(), vstartindex1) ;
	if(it1 != m_TargetVertex.end())
		m_TargetVertex.erase(it1);
	auto it2 = std::find(m_TargetVertex.begin(), m_TargetVertex.end(), vstartindex2);
	if (it2 != m_TargetVertex.end())
		m_TargetVertex.erase(it2);
	int vendindex;
	int num;
	while (m_TargetVertex.size() > 1)
	{
		path.clear();
		path2.clear();

		num = pMesh3d->m_ArraySharpEdges.size();

		if (num == 473)
			return;
		//ComputeMinimalDistance_NearestPoint(vstartindex1, m_TargetVertex);
		int len1 = pMesh3d->ComputeNewWeightedShortestPath(vstartindex1, vendindex, path, m_TargetVertex);
		int len2 = pMesh3d->ComputeWeightedShortestPath(vstartindex2, vendindex, path2);
		if (len1 < len2)
		{
			for (int i = path.size() - 1; i >= 0; i--)
			{
				m_anticlockwise.push_back(path[i]);
				pMesh3d->GetVertex(path[i])->m_Flag = 0;

				if(i > 0)
					pMesh3d->m_ArraySharpEdges.push_back(std::make_pair(path[i], path[i - 1]));

			}
			auto it3 = std::find(m_TargetVertex.begin(), m_TargetVertex.end(), vendindex);
			if (it3 != m_TargetVertex.end())
			{
				m_TargetVertex.erase(it3);
				pMesh3d->GetVertex(vendindex)->m_Flag = 0;
			}

			vstartindex1 = m_anticlockwise[m_anticlockwise.size() - 1];
		}
		else
		{
			for (int i = path2.size() - 1; i >= 0; i--)
			{
				m_clockwise.push_back(path2[i]);
				pMesh3d->GetVertex(path2[i])->m_Flag = 0;

				if (i > 0)
					pMesh3d->m_ArraySharpEdges.push_back(std::make_pair(path2[i], path2[i - 1]));
			}
			auto it3 = std::find(m_TargetVertex.begin(), m_TargetVertex.end(), vendindex);
			if (it3 != m_TargetVertex.end())
			{
				m_TargetVertex.erase(it3);
				pMesh3d->GetVertex(vendindex)->m_Flag = 0;
			}

			vstartindex2 = m_clockwise[m_clockwise.size() - 1];
		}
	}
	for (int i = 0; i < m_anticlockwise.size() - 2;)
	{
		if (m_anticlockwise[i] == m_anticlockwise[i + 1])
			m_anticlockwise.erase(m_anticlockwise.begin() + i);
		else
			i++;
	}
}
void DeletePoint(CMesh3d* pMesh3d)
{
	std::string filename = pMesh3d->filename;
	filename.replace(filename.find("."), 2, ".txt");
	std::ifstream file(filename);
	std::string sl;
	file >> sl;
	int vdindex;
	int test = sl.find("deletepoints");
	if (test > -1)
	{
		while (!file.eof())
		{
			file >> vdindex;
			CVertex3d* pVertex = pMesh3d->GetVertex(vdindex);
			pVertex->m_Flag = 0;
		}
	}
	file.close();
}
void AddPoint(CMesh3d* pMesh3d)
{
	std::string filename = pMesh3d->filename;
	filename.replace(filename.find("."), 2, "a.txt");
	std::ifstream file(filename);
	std::string sl;
	file >> sl;
	int vdindex;
	int test = sl.find("addpoints");
	if (test > -1)
	{
		while (!file.eof())
		{
			file >> vdindex;
			CVertex3d* pVertex = pMesh3d->GetVertex(vdindex);
			pVertex->m_Flag = 1;
		}
	}
	file.close();
}

void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationDetectteetchline()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}

	CMesh3d* pMeshCopy = NULL;
	CMesh3d_OpenMesh* pMesh_Openmesh = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();
	else
		pMesh_Openmesh = ((CMesh3d*)pMesh)->Convert_OpenMesh();

	CMesh3d* pMesh3d = (CMesh3d*)pMesh;
	//pMesh3d->SmoothGaussCurvature();

	pMesh3d->CalculateGaussCurvature();
	int vindex, vindex1;
	double m_CurrentVertexCurvature;
	CVertex3d* pVertex, *pVertex1;
	std::vector<double> m_VNeighborGaussCurvature;
	std::vector<double> m_VNeighborMINIMALGaussCurvature;
	std::vector<double> m_VNeighborMAXIMALGaussCurvature;
	double TOL_VAL = 0.1;
	double TOL_MINIMALGAUSSCURVATURE = -3;
	int    MINIMALGAUSSCURVATURE_num = 1;
	double TOL_MAXIMALGAUSSCURVATURE = -1;
	int    MAXIMALGAUSSCURVATURE_num = 1;
	for (MyMesh::VertexIter v_it = pMesh_Openmesh->pmesh->vertices_begin(); v_it != pMesh_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		vindex = v_it.handle().idx();
		pVertex = pMesh3d->GetVertex(vindex);
		pVertex->m_Flag = 0;
		m_CurrentVertexCurvature = pVertex->m_GaussCurvature;
		m_VNeighborGaussCurvature.clear();

		if (m_CurrentVertexCurvature < -TOL_VAL || m_CurrentVertexCurvature > TOL_VAL)
			continue;

		for (MyMesh::VertexVertexIter vv_it = pMesh_Openmesh->pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
		{
			vindex1 = vv_it.handle().idx();
			CVertex3d* pVertex1 = pMesh3d->GetVertex(vindex1);
			m_VNeighborGaussCurvature.push_back(pVertex1->m_GaussCurvature);
		}
		m_VNeighborMINIMALGaussCurvature.clear();
		m_VNeighborMAXIMALGaussCurvature.clear();

		for (int i = 0; i < m_VNeighborGaussCurvature.size(); i++)
		{
			if (m_VNeighborGaussCurvature[i] < TOL_MINIMALGAUSSCURVATURE)
				m_VNeighborMINIMALGaussCurvature.push_back(m_VNeighborGaussCurvature[i]);
			else if (m_VNeighborGaussCurvature[i] < TOL_MAXIMALGAUSSCURVATURE)
				m_VNeighborMAXIMALGaussCurvature.push_back(m_VNeighborGaussCurvature[i]);
		}

		if (m_VNeighborMINIMALGaussCurvature.size() >= MINIMALGAUSSCURVATURE_num && m_VNeighborMAXIMALGaussCurvature.size() >= MAXIMALGAUSSCURVATURE_num)
			pVertex->m_Flag = 1;
	}*/
/*	for (int i = 0; i < pMesh3d->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3d->GetVertex(i);
		pVertex->m_Flag = 0;

		double curvature = pVertex->m_GaussCurvature;

		double ratio = (curvature - minimalcurvature) / (maximalcurvature - minimalcurvature);

		int x = ratio * m_TextureImage.GetWidth();
		if (x >= m_TextureImage.GetWidth())
			x = m_TextureImage.GetWidth() - 1;
		if (x < 0)
			x = 0;
		COLORREF color = m_TextureImage.GetPixel(x, 0);

		pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
		//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
		//if (pVertex->x() > 0 && pVertex->x() < 1)

		//pVertex_uv->SetColor(*pVertex->GetColor());
		//pVertex->u = pVertex_uv->x();
		//pVertex->v = pVertex_uv->y();
	}*/

	/*pMesh3d->m_Modified = 1;
	pMesh3d->m_ColorBinding = COLOR_PER_VERTEX;
	if (!pMeshCopy)
		delete pMeshCopy;
	pDoc->UpdateAllViews(NULL);*/

	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}

	CMesh3d* pMeshCopy = NULL;
	CMesh3d_OpenMesh* pMesh_Openmesh = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();
	else
		pMesh_Openmesh = ((CMesh3d*)pMesh)->Convert_OpenMesh();
	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");
	CMesh3d* pMesh3d = (CMesh3d*)pMesh;
	pMesh3d->CalculateGaussCurvature();
	pMesh3d->CalculateLaplaceBeltramiOperator();
	double minimalmeancurvature = pMesh3d->CalculateMinimalMeanCurvature();
	double maximalmeancurvature = pMesh3d->CalculateMaximalMeanCurvature();
	maximalmeancurvature = 2;
	std::vector<double> m_Gauss;
	std::vector<double> m_Mean;
	for (int i = 0; i < pMesh3d->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3d->GetVertex(i);
		double meancur = Length(pVertex->m_LaplaceOperatorVec) / 2;
		double curvature = pVertex->m_GaussCurvature;
		m_Gauss.push_back(curvature);
		m_Mean.push_back(meancur);
	}

	//pMesh3d->SmoothGaussCurvature();
	pMesh3d->SmoothGaussCurvature();
	pMesh3d->SmoothGaussCurvature();
	double minimalcurvature = -4;
	double maximalcurvature = 1;
	//CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
	//pMesh3d->CalculateNormalPerVertex();
	for (int i = 0; i < pMesh3d->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3d->GetVertex(i);
		//x = pVertex->m_Normal.x();
		//y = pVertex->m_Normal.y();
		//z = pVertex->m_Normal.z();
		double curvature = pVertex->m_GaussCurvature;

		double ratio = (curvature - minimalcurvature) / (maximalcurvature - minimalcurvature);

		int x = ratio * m_TextureImage.GetWidth();
		if (x >= m_TextureImage.GetWidth())
			x = m_TextureImage.GetWidth() - 1;
		if (x < 0)
			x = 0;
		COLORREF color = m_TextureImage.GetPixel(x, 0);

		pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
		//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
		//if (pVertex->x() > 0 && pVertex->x() < 1)

		//pVertex_uv->SetColor(*pVertex->GetColor());
		//pVertex->u = pVertex_uv->x();
		//pVertex->v = pVertex_uv->y();
	}
	//pMesh3d->CalculateGaussCurvature();
	int vindex, vindex1;
	double m_CurrentVertexCurvature;
	double m_CurrentMeanCurvature;
	CVertex3d* pVertex, *pVertex1;
	std::vector<double> m_VNeighborGaussCurvature;
	std::vector<double> m_VNeighborMeanCurvature;
	std::vector<double> m_VNeighborMINIMALGaussCurvature;
	std::vector<double> m_VNeighborMAXIMALGaussCurvature;
	std::vector<double> m_VNeighborSAMEGaussCurvature;
	std::vector<double> m_VNeighborGETMeanCurvature;
	std::vector<double> VMaxLength;
	std::vector<double> m_currentVertexLen;   //当前领边长度集合
	std::vector<int> m_TargetVertex;
	double TOL_Gauss = 2.0;
	double TOL_MAXVAL = -1.5;
	double TOL_MINVAL = -3.5;
	double TOL_MINIMALGAUSSCURVATURE = -3;
	int    MINIMALGAUSSCURVATURE_num = 1;
	double TOL_MAXIMALGAUSSCURVATURE = -2;
	int    MAXIMALGAUSSCURVATURE_num = 1;
	int    maxNUMOFSAMENEIGHBOR = 3;
	int    minNUMOFSAMENEIGHBOR = 1;
	for (MyMesh::VertexIter v_it = pMesh_Openmesh->pmesh->vertices_begin(); v_it != pMesh_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		vindex = v_it.handle().idx();
		pVertex = pMesh3d->GetVertex(vindex);
		pVertex->m_Flag = 0;
		m_CurrentVertexCurvature = pVertex->m_GaussCurvature;
		m_CurrentMeanCurvature = Length(pVertex->m_LaplaceOperatorVec) / 2;
		m_VNeighborGaussCurvature.clear();

		double test = m_Gauss[vindex];
		if (m_CurrentVertexCurvature < TOL_MINVAL || m_CurrentVertexCurvature > TOL_MAXVAL || m_Gauss[vindex] >= TOL_Gauss || m_Gauss[vindex] <= -TOL_Gauss)
		{
			VMaxLength.push_back(0);
			continue;
		}

		double maxlen = 0.0;

		m_currentVertexLen.clear();

		for (MyMesh::VertexVertexIter vv_it = pMesh_Openmesh->pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
		{
			vindex1 = vv_it.handle().idx();
			CVertex3d* pVertex1 = pMesh3d->GetVertex(vindex1);
			double currentlen = (pVertex->m_Coord[0] - pVertex1->m_Coord[0])*(pVertex->m_Coord[0] - pVertex1->m_Coord[0]) + (pVertex->m_Coord[1] - pVertex1->m_Coord[1])*(pVertex->m_Coord[1] - pVertex1->m_Coord[1]) + (pVertex->m_Coord[2] - pVertex1->m_Coord[2])*(pVertex->m_Coord[2] - pVertex1->m_Coord[2]);
			if (currentlen >= 0.08)
			{
				m_currentVertexLen.push_back(currentlen);
			}
			if (maxlen < currentlen)
			{
				maxlen = currentlen;
			}
			m_VNeighborGaussCurvature.push_back(pVertex1->m_GaussCurvature);
			//m_VNeighborMeanCurvature.push_back(Length(pVertex->m_LaplaceOperatorVec) / 2);
		}
		VMaxLength.push_back(maxlen);
		m_VNeighborMINIMALGaussCurvature.clear();
		m_VNeighborMAXIMALGaussCurvature.clear();
		m_VNeighborGETMeanCurvature.clear();
		m_VNeighborSAMEGaussCurvature.clear();
		for (int i = 0; i < m_VNeighborGaussCurvature.size(); i++)
		{
			if (m_VNeighborGaussCurvature[i] < TOL_MINIMALGAUSSCURVATURE)
				m_VNeighborMINIMALGaussCurvature.push_back(m_VNeighborGaussCurvature[i]);
			else if (m_VNeighborGaussCurvature[i] > TOL_MAXIMALGAUSSCURVATURE)
				m_VNeighborMAXIMALGaussCurvature.push_back(m_VNeighborGaussCurvature[i]);
			else
				m_VNeighborSAMEGaussCurvature.push_back(m_VNeighborGaussCurvature[i]);
			//if (m_VNeighborMeanCurvature[i] >= 1 && m_VNeighborMeanCurvature[i] <= 2) 
			//{
			//	m_VNeighborGETMeanCurvature.push_back(m_VNeighborGaussCurvature[i]);
			//}
		}
		if (VMaxLength[vindex] >= 4 || m_currentVertexLen.size() <= 1) {
			pVertex->m_Flag = 0;
			continue;
		}
		//if (m_VNeighborGETMeanCurvature.size() >= 3) {
			//pVertex->m_Flag = 1;
			//m_TargetVertex.push_back(vindex);
		//}
		if (m_VNeighborGaussCurvature.size() >= 5 && m_VNeighborGaussCurvature.size() < 7 && m_VNeighborSAMEGaussCurvature.size() >= minNUMOFSAMENEIGHBOR && m_VNeighborSAMEGaussCurvature.size() <= maxNUMOFSAMENEIGHBOR && m_VNeighborMINIMALGaussCurvature.size() >= MINIMALGAUSSCURVATURE_num && m_VNeighborMAXIMALGaussCurvature.size() >= MAXIMALGAUSSCURVATURE_num)
		{
			pVertex->m_Flag = 1;
			//m_TargetVertex.push_back(vindex);
		}
	}
	/*	for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);
			pVertex->m_Flag = 0;

			double curvature = pVertex->m_GaussCurvature;

			double ratio = (curvature - minimalcurvature) / (maximalcurvature - minimalcurvature);

			int x = ratio * m_TextureImage.GetWidth();
			if (x >= m_TextureImage.GetWidth())
				x = m_TextureImage.GetWidth() - 1;
			if (x < 0)
				x = 0;
			COLORREF color = m_TextureImage.GetPixel(x, 0);

			pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
			//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
			//if (pVertex->x() > 0 && pVertex->x() < 1)

			//pVertex_uv->SetColor(*pVertex->GetColor());
			//pVertex->u = pVertex_uv->x();
			//pVertex->v = pVertex_uv->y();
		}*/
		/***************
		std::vector<int> path;
		GeoMapLandMark* pLandmark;
		for (int i = 0; i < m_TargetVertex.size() - 1; i++)
		{
			path.clear();
			int index1 = m_TargetVertex[i];
			int index2 = m_TargetVertex[i + 1];
			pPolyOmino -> pMesh -> ComputeShortestPath(index2, index1, path);
			for (int k = 1; k < path.size(); k++)
			{
				pLandmark->landmark_points.push_back(path[k]);
			}
		}
		pPolyOmino->pMesh->m_LandMarkArray.push_back(pLandmark);
		********/
	pMesh3d->m_Modified = 1;
	pMesh3d->m_ColorBinding = COLOR_PER_VERTEX;
	if (!pMeshCopy)
		delete pMeshCopy;
	pDoc->UpdateAllViews(NULL);
	DeletePoint(pMesh3d);
	AddPoint(pMesh3d);
	//std::vector<int> test;
	for (int i = 0; i < pMesh3d->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3d->GetVertex(i);
		if (pVertex->m_Flag == 1)
		{
			m_TargetVertex.push_back(i);
			//test.push_back(i);
		}
	}
	//FindPoint(pMesh3d, m_TargetVertex);
}


void Actions_ConstrainedRegistration::OnBnClickedConstrainedregistrationSharpengausscurvature()
{
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the source mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	CMesh3d* pMeshCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();

	CMesh3d* pMesh3d = (CMesh3d*)pMesh;
	pMesh3d->SharpGaussCurvature();

	double minimalcurvature = -4;
	double maximalcurvature = 1;
	//CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;
	//pMesh3d->CalculateNormalPerVertex();
	for (int i = 0; i < pMesh3d->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3d->GetVertex(i);
		//x = pVertex->m_Normal.x();
		//y = pVertex->m_Normal.y();
		//z = pVertex->m_Normal.z();
		double curvature = pVertex->m_GaussCurvature;

		double ratio = (curvature - minimalcurvature) / (maximalcurvature - minimalcurvature);

		int x = ratio * m_TextureImage.GetWidth();
		if (x >= m_TextureImage.GetWidth())
			x = m_TextureImage.GetWidth() - 1;
		if (x < 0)
			x = 0;
		COLORREF color = m_TextureImage.GetPixel(x, 0);

		pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
		//CVertex3d* pVertex_uv = pMesh3dUV->GetVertex(i);
		//if (pVertex->x() > 0 && pVertex->x() < 1)

		//pVertex_uv->SetColor(*pVertex->GetColor());
		//pVertex->u = pVertex_uv->x();
		//pVertex->v = pVertex_uv->y();
	}

	pMesh3d->m_Modified = 1;
	pMesh3d->m_ColorBinding = COLOR_PER_VERTEX;
	if (!pMeshCopy)
		delete pMeshCopy;
	pDoc->UpdateAllViews(NULL);
}
