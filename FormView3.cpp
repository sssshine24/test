// FormView3.cpp : implementation file

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "MyTreeView.h"
#include "FormView3.h"
#include "MainFrm.h"
#include "Lib3D/Mesh3d.h"
#include "Lib3D/Vertex3d.h"
#include "SDIViewSwitchDoc.h"
#include <cmath>
#include <fstream>
#include <gl/gl.h>
#include <gl/glu.h>
#include "C_NurbsSurf.h"
#include "math.h"
#include "Nurbs3D.h"
#include <iostream>
#include "convexFunction.h"
#include "lib3d/Base3d.h"
#include <algorithm>
#include "ShowSurfaceParameterDomain.h"
#include "SurfaceInformation.h"
#include "RenderMode.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"
#include "ProjectToParameterDomain.h"
#include <atlimage.h>
#include "urveintersection.h"
#include "GeneralBilinearTransformation.h"
#include <sstream> 
#include "OrthogonalityEnergy.h"
#include "mobiustransformation.h"
#include "ShowTransformedParameterDomain.h"
#include "quadratictransformation.h"
#include "Tspline.h"
#include "Tspline_PreImageDialog.h"
#include "Disline2D.h"
#include "NurbsCurve.h"
#include "Lib3D/Mesh3d_OpenMesh.h"
#include "string.h"
#include "engine.h"
#include "matrix.h"
#include <strstream>
#include "MeshSetColor.h"
#include "MySparseMatrix.h"
#include "kdtree.h"
#include "BilinearTransformation.h"
#include "Shape_Polygon.h"
//#include <openglut.h>
#define SHOWTRACEINFORMATION
#include <gl/glut.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define	FOVY	150.0	
#define ZNEAR	1.0
#define	ZFAR	2000.0
/////////////////////////////////////////////////////////////////////////////

string DoubleToString(double d);
int uBezierNum;
int vBezierNum;
C_NurbsSurf* pCurOptimizationSurf;
C_NurbsSurf* pOriginalOptimizationSurf;

void AFXAPI DDV_MinString(CDataExchange* pDX, CString const& str)
{
	//a very simple custom DDV routine:
	if (str.IsEmpty())
	{
		if (!pDX->m_bSaveAndValidate)
		{
			return;
		}
		AfxMessageBox("String needs to have at least one character.");
		pDX->Fail();
	}
}
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CFormView3, CColorFormView)

CFormView3::CFormView3()
	: CColorFormView(CFormView3::IDD, RGB(255, 255, 255)) //a green color
{
	//{{AFX_DATA_INIT(CFormView3)
	//}}AFX_DATA_INIT

	//The following color will be used to "override" the default edit color that 
	//is coded in the CColorFormView class that this formview is derived from:
	m_crGreen = RGB(255, 255, 255); //very light green for edit windows
	m_pEditBkBrush = new CBrush(m_crGreen);
	type = 2;
	m_KnotPosition = 50;
	p_FormView3 = this;
	m_bInitialized = FALSE;
	currentindex = 0;
}
CFormView3* p_FormView3;
/////////////////////////////////////////////////////////////////////////////
CFormView3::~CFormView3()
{
	TRACE("destructing CFormView3\n");
}
/////////////////////////////////////////////////////////////////////////////
void CFormView3::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CFormView3)
	//}}AFX_DATA_MAP
	//
	//DDX_Slider(pDX, IDC_SLIDERCHANGEKNOT, m_KnotPosition);
	//DDX_CBString(pDX,IDC_COMBOSELECT,m_ComboSelectContent);
	//DDX_CBIndex(pDX,IDC_COMBOSELECT,m_ComboSelectIndex);

	//My own very simple custom DDV that just checks that someone has entered 
	//something in the edit control. I could do this for all of the edit controls,
	//but I don't want to bother the user in filling out the entire form. Note that I
	//put the line DDX_Text(pDX, IDC_EDIT_NAME, m_strName) last in the data map above. 
	//The reason is that when the validation fails, focus will be returned to the last 
	//control that was "exchanged". If this were a "real" app, you'd probably want to
	//combine DDV_MaxChars with DDV_MinString to create one DDV routine.
	//this must come after the DDX_Text for m_strName
	//and not before another DDX_...
	//DDX_Control(pDX, IDC_COMBOSELECT, m_SelectComboBox);
	DDX_Control(pDX, IDC_TAB2, m_tab);
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CFormView3, CColorFormView)
	//{{AFX_MSG_MAP(CFormView3)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_CANCELMODE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_ITEMCHANGED, IDC_Objects, OnTvnItemChangedObjects)
	ON_BN_CLICKED(IDC_CheckAreaPreserving, OnBnClickedCheckareapreserving)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, OnReleasedcaptureSlider)
	ON_BN_CLICKED(IDC_AREAPRESERVE, OnBnClickedAreapreserve)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SLIDER1, OnNMThemeChangedSlider)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SLIDER2, OnNMThemeChangedSlider)
	ON_BN_CLICKED(IDC_ShowConvexFunction, OnBnClickedShowconvexfunction)
	ON_NOTIFY(NM_THEMECHANGED, IDC_Mobius_Alpha, OnNMThemeChangedMobiusAlpha)
	ON_NOTIFY(NM_THEMECHANGED, IDC_Mobius_Beta, OnNMThemeChangedMobiusBeta)
	ON_BN_CLICKED(IDC_ShowCoordinateAxis_Object, OnBnClickedShowcoordinateaxis)
	ON_BN_CLICKED(ID_SURFACE_SHOWPARAMETER, OnShowSurfaceParameter)
	ON_BN_CLICKED(ID_SURFACE_ONLYSHOWME, OnShowOnlyMe)
	ON_BN_CLICKED(ID_SURFACE_SHOWGENERALBILINEARISOPARAMETERCURVES, OnSurfaceShowIsoParameterCurvesGeneralBilinear)
	ON_BN_CLICKED(ID_SURFACE_SHOWISOPARAMETERCURVES, OnSurfaceShowIsoParameterCurves)
	ON_BN_CLICKED(ID_SURFACE_SHOWISOLINE, OnSurfaceShowIsoLines)
	ON_BN_CLICKED(ID_SURFACE_TRANSFORMATIONOFMOBIUS, OnSurfaceShowIsoParameterCurves_mobius)
	ON_BN_CLICKED(ID_SURFACE_TRANSFORMATIONOFMOBIUSANDBILINEAR, OnSurfaceShowIsoParameterCurves_mb)
	ON_BN_CLICKED(ID_SURFACE_TRANSFORMATIONOFMOBIUSANDCUBIC, OnSurfaceShowIsoParameterCurves_mc)
	ON_BN_CLICKED(ID_SURFACE_SHOWSUBDIVISIONCURVES, OnSurfaceShowSubdivisionCurves)
	ON_BN_CLICKED(ID_SURFACE_SHOWSURFACE, OnSurfaceShowSurface)
	ON_BN_CLICKED(ID_SURFACE_SHOWOPTIMALISOPARAMETERCURVES, OnSurfaceShowOptimalIsoParameterCurves)
	ON_BN_CLICKED(ID_SURFACE_SHOWCONTROLPOINTNETWORK, OnSurfaceShowcontrolpointnetwork)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWCONTROLPOINTNETWORK, OnUpdateSurfaceShowcontrolpointnetwork)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWCOLORBOUNDARY, &CFormView3::OnUpdateSurfaceShowcolorboundary)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWGENERALBILINEARISOPARAMETERCURVES, &CFormView3::OnUpdateSurfaceShowisoparametercurvesGeneralBilinear)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWISOPARAMETERCURVES, &CFormView3::OnUpdateSurfaceShowisoparametercurves)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_TRANSFORMATIONOFMOBIUS, &CFormView3::OnUpdateSurfaceTransformationofmobius)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWISOLINE, &CFormView3::OnUpdateSurfaceISOlines)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_TRANSFORMATIONOFMOBIUSANDBILINEAR, &CFormView3::OnUpdateSurfaceTransformationofmobiusandbilinear)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_TRANSFORMATIONOFMOBIUSANDCUBIC, &CFormView3::OnUpdateSurfaceTransformationofmobiusandcubic)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWSUBDIVISIONCURVES, &CFormView3::OnUpdateSurfaceShowsubdivisioncurves)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWOPTIMALISOPARAMETERCURVES, &CFormView3::OnUpdateSurfaceShowoptimalisoparametercurves)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWSURFACE, &CFormView3::OnUpdateSurfaceShowsurface)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWSUBSURFACECOORDINATE, &CFormView3::OnUpdateSurfaceShowsubsurfacecoordinate)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWBILINEARCOEFFICIENTS, &CFormView3::OnUpdateSurfaceShowbilinearcoefficients)
	ON_BN_CLICKED(IDC_ADDKNOT, &CFormView3::OnBnClickedAddknot)
	ON_BN_CLICKED(IDC_BILINEAR_PARAMETERIZE, &CFormView3::OnBnClickedBilinearparameterize)
	ON_BN_CLICKED(IDC_Quadratic_Reparameterization, &CFormView3::OnBnClickedQuadraticparameterize)
	ON_BN_CLICKED(ID_SURFACE_DELETE, OnSurfaceDelete)
	ON_BN_CLICKED(ID_SURFACE_SHOWSUBSURFACECOORDINATE, &CFormView3::OnSurfaceShowsubsurfacecoordinate)
	ON_BN_CLICKED(ID_SURFACE_SHOWCOLORBOUNDARY, &CFormView3::OnSurfaceShowColorBoundary)
	ON_BN_CLICKED(ID_SURFACE_SUBDIVIDE, &CFormView3::OnSurfaceSubdivide)
	ON_BN_CLICKED(ID_SURFACE_SHOWBILINEARCOEFFICIENTS, &CFormView3::OnSurfaceShowbilinearcoefficients)
	ON_BN_CLICKED(ID_SURFACE_SHOWSURFACEINFORMATION, &CFormView3::OnSurfaceShowsurfaceinformation)
	ON_BN_CLICKED(ID_RENDER_SETRENDERMODE, &CFormView3::OnRenderSetrendermode)
	ON_BN_CLICKED(IDC_BUTTON3, &CFormView3::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CFormView3::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CFormView3::OnBnClickedButton5)
	ON_BN_CLICKED(ID_SURFACE_POVRAY, &CFormView3::OnSurfacePovray)
	ON_BN_CLICKED(ID_MESH3D_NORMALIZE, &CFormView3::OnSurfaceMesh3dNormalize)
	ON_BN_CLICKED(IDC_ProjectToParameterDomain, &CFormView3::OnBnClickedProjecttoparameterdomain)
	ON_BN_CLICKED(ID_SURFACE_NORMALIZE, &CFormView3::OnSurfaceNormalize)
	ON_BN_CLICKED(ID_SURFACE_SAVEAS, &CFormView3::OnSurfaceSaveas)
	ON_BN_CLICKED(IDC_ReadViewPoint, &CFormView3::OnBnClickedReadviewpoint)
	ON_BN_CLICKED(IDC_FaceExample, &CFormView3::OnBnClickedFaceexample)
	ON_BN_CLICKED(IDC_FaceExample2, &CFormView3::OnBnClickedFaceexample2)
	ON_BN_CLICKED(IDC_BezierExampleStep1, &CFormView3::OnBnClickedBezierexamplestep1)
	ON_COMMAND(ID_SAVEASTMFILE, &CFormView3::OnSaveastmfile)
	ON_COMMAND(ID_MESH_SETCOLORRS, &CFormView3::OnMeshSetColor)
	ON_COMMAND(ID_MESH_SAVEASMFILE, &CFormView3::OnSaveasMeshMfile)
	ON_BN_CLICKED(IDC_VENUS_STEP1, &CFormView3::OnBnClickedVenusStep1)
	ON_BN_CLICKED(IDC_VENUS_STEP2, &CFormView3::OnBnClickedVenusStep2)
	ON_BN_CLICKED(IDC_VENUS_STEP3, &CFormView3::OnBnClickedVenusStep3)
	ON_BN_CLICKED(IDC_READBSFFILE, &CFormView3::OnBnClickedReadbsffile)
	ON_COMMAND(ID_SURFACE_SHOWOPTIMALAREAPRESERVING, &CFormView3::OnSurfaceShowoptimalareapreserving)
	ON_COMMAND(ID_SURFACE_SHOWOPTIMALAREAPRESERVINGMODIFIED, &CFormView3::OnSurfaceShowoptimalareapreservingmodified)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWOPTIMALAREAPRESERVING, &CFormView3::OnUpdateSurfaceShowoptimalareapreserving)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWOPTIMALAREAPRESERVINGMODIFIED, &CFormView3::OnUpdateSurfaceShowoptimalareapreservingmodified)
	ON_BN_CLICKED(IDC_BONESTEP1, &CFormView3::OnBnClickedBonestep1)
	ON_BN_CLICKED(IDC_BONESTEP2, &CFormView3::OnBnClickedBonestep2)
	ON_BN_CLICKED(IDC_BUTTON6, &CFormView3::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CFormView3::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_FACE2STEP1, &CFormView3::OnBnClickedFace2step1)
	ON_BN_CLICKED(IDC_FACE2STEP2, &CFormView3::OnBnClickedFace2step2)
	ON_BN_CLICKED(IDC_MeshAnimation, &CFormView3::OnBnClickedMeshanimation)
	ON_BN_CLICKED(IDC_FinalMesh, &CFormView3::OnBnClickedFinalmesh)
	ON_BN_CLICKED(ID_MESH_GENERATEBSPINESURFACE, &CFormView3::OnBnClickedBsplineSurface)
	ON_BN_CLICKED(ID_BsplineFITTINGERROR, GetBsplineFittingError)
	ON_COMMAND(ID_SURFACE_SHOWCOLORORTHOGONALITY, &CFormView3::OnSurfaceShowcolororthogonality)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWCOLORORTHOGONALITY, &CFormView3::OnUpdateSurfaceShowcolororthogonality)
	//ON_BN_CLICKED(ID_MESH_GENERATEOPTIMALBILINEARTRANSFORMATION,
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CFormView3::OnTcnSelchangeTab2)
	ON_COMMAND(ID_SURFACE_SHOWTRANSFORMEDPARAMETERDOMAIN, &CFormView3::OnSurfaceShowtransformedparameterdomain)
	ON_COMMAND(ID_MESH_GENERATEQUADMESH, &CFormView3::OnMeshGeneratequadmesh)
	ON_COMMAND(ID_SURFACE_CONVERTTOTSPLINE, &CFormView3::OnSurfaceConverttotspline)
	ON_COMMAND(ID_TSPLINE_PLOTPREIMAGE, &CFormView3::OnTsplinePlotpreimage)
	ON_COMMAND(ID_TSPLINE_SAVEAS, &CFormView3::OnTsplineSaveAs)
	ON_COMMAND(ID_MESH_SHOWEDGE, &CFormView3::OnMeshShowedge)
	ON_COMMAND(ID_MESH_SHOWLANDMARKEDGE, &CFormView3::OnMeshShowLandmarkEdge)
	ON_COMMAND(IDC_Omino_SetCurrentMesh_Menu, &CFormView3::OnMeshSetCurrentMesh)
	ON_COMMAND(ID_MESH_SHOWSELECTEDLANDMARK, &CFormView3::OnMeshShowSelectedLandmarkEdge)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSELECTEDLANDMARK, &CFormView3::OnUpdateMeshShowSelectedlandmarkEdge)
	ON_COMMAND(ID_MESH_SHOWMERGECONSTRAINTS, &CFormView3::OnMeshShowMergeConstraints)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWMERGECONSTRAINTS, &CFormView3::OnUpdateMeshShowMergeConstraints)
	ON_COMMAND(ID_MESH_SHOWLANDMARKVERTEX, &CFormView3::OnMeshShowLandmarkVertex)
	ON_COMMAND(ID_MESH_SHOWSHARPEDGE, &CFormView3::OnMeshShowSharpEdge)
	ON_COMMAND(ID_POLYGON_SHOWPOLYGONLINE, &CFormView3::OnPolygonShowRingEdge)
	ON_COMMAND(ID_POLYGON_DELETE, &CFormView3::OnPolygonDelete)
	ON_COMMAND(ID_POLYGON_SHOWPOLYGONINNERRINGS, &CFormView3::OnPolygonShowInnerRingEdge)
	ON_COMMAND(ID_POLYGON_SHOWPOLYGONINDEX, &CFormView3::OnPolygonShowRingIndex)
	ON_COMMAND(ID_MESH_SHOWSELECTEDSHARPEDGE, &CFormView3::OnMeshShowSelectedSharpEdge)
	ON_COMMAND(ID_MESH_SHOWFACE, &CFormView3::OnMeshShowface)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWEDGE, &CFormView3::OnUpdateMeshShowEdge)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSHARPEDGE, &CFormView3::OnUpdateMeshShowSharpedge)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_SHOWPOLYGONLINE, &CFormView3::OnUpdatePolygonShowRingEdge)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_SHOWPOLYGONINNERRINGS, &CFormView3::OnUpdatePolygonShowInnerRingEdge)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_SHOWPOLYGONINDEX, &CFormView3::OnUpdatePolygonShowRingIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWLANDMARKEDGE, &CFormView3::OnUpdateMeshShowlandmarkEdge)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSELECTEDSHARPEDGE, &CFormView3::OnUpdateMeshShowSelectedSharpedge)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWLANDMARKVERTEX, &CFormView3::OnUpdateMeshShowlandmarkVertex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWFACE, &CFormView3::OnUpdateMeshShowface)
	ON_COMMAND(ID_MESH_SHOWVERTEX, &CFormView3::OnMeshShowvertex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWVERTEX, &CFormView3::OnUpdateMeshShowvertex)
	ON_COMMAND(ID_MESH_SHOWSELECTEDVERTEX, &CFormView3::OnMeshShowSelectedvertex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSELECTEDVERTEX, &CFormView3::OnUpdateMeshShowSelectedvertex)
	ON_COMMAND(ID_MESH_SHOWPOINTS_ONSURFACE, &CFormView3::OnMeshShowPointsOnSurface)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWPOINTS_ONSURFACE, &CFormView3::OnUpdateMeshShowPointsOnSurface)
	ON_COMMAND(ID_MESH_SHOWSELECTEDEDGE, &CFormView3::OnMeshShowSelectedEdge)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSELECTEDEDGE, &CFormView3::OnUpdateMeshShowSelectedEdge)
	ON_COMMAND(ID_MESH_SHOWSELECTEDVERTEXINDEX, &CFormView3::OnMeshShowSelectedVertexIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSELECTEDVERTEX, &CFormView3::OnUpdateMeshShowSelectedVertexIndex)
	ON_COMMAND(ID_MESH_SHOWSHARPSELECTEDVERTEX, &CFormView3::OnMeshShowSharpSelectedvertex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSHARPSELECTEDVERTEX, &CFormView3::OnUpdateMeshShowSharpSelectedvertex)
	ON_COMMAND(ID_MESH_SHOWFEATUREVERTEX, &CFormView3::OnMeshShowFeaturevertex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWFEATUREVERTEX, &CFormView3::OnUpdateMeshShowFeaturevertex)
	ON_COMMAND(ID_MESH_SHOWFEATUREVERTEXINDEX, &CFormView3::OnMeshShowFeaturevertexIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWFEATUREVERTEXINDEX, &CFormView3::OnUpdateMeshShowFeaturevertexIndex)
	ON_COMMAND(ID_MESH_SHOWSELECTEDFACE, &CFormView3::OnMeshShowSelectedface)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSELECTEDFACE, &CFormView3::OnUpdateMeshShowSelectedface)
	ON_COMMAND(ID_MESH_SHOWVERTEXINDEX, &CFormView3::OnMeshShowvertexIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWVERTEXINDEX, &CFormView3::OnUpdateMeshShowvertexIndex)
	ON_COMMAND(ID_MESH_SHOWEDGEINDEX, &CFormView3::OnMeshShowedgeIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWEDGEINDEX, &CFormView3::OnUpdateMeshShowedgeIndex)
	ON_COMMAND(ID_MESH_SHOWFACEINDEX, &CFormView3::OnMeshShowFaceIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWFACEINDEX, &CFormView3::OnUpdateMeshShowFaceIndex)
	ON_COMMAND(ID_MESH_SHOWDISTRICTINDEX, &CFormView3::OnMeshShowDistrictIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWDISTRICTINDEX, &CFormView3::OnUpdateMeshShowDistrictIndex)
	ON_COMMAND(ID_MESH_SHOWSTATENAME, &CFormView3::OnMeshShowStateName)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSTATENAME, &CFormView3::OnUpdateMeshShowStateName)
	ON_COMMAND(ID_MESH_SHOWJIANZHI, &CFormView3::OnMeshShowJianZhi)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWJIANZHI, &CFormView3::OnUpdateMeshShowJianZhi)
	ON_COMMAND(ID_MESH_SHOWUNIFORMGRID, &CFormView3::OnMeshShowUniformGrid)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWUNIFORMGRID, &CFormView3::OnUpdateMeshShowUniformGrid)
	ON_COMMAND(ID_ShowPuzzleGraph, &CFormView3::OnShowPuzzleGraph)
	ON_UPDATE_COMMAND_UI(ID_ShowPuzzleGraph, &CFormView3::OnUpdateShowPuzzleGraph)
	ON_COMMAND(ID_MESH_SHOWVERTEXMUVECTORFIELD, &CFormView3::OnShowVertexMUVectorField)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWVERTEXMUVECTORFIELD, &CFormView3::OnUpdateShowVertexMUVectorField)
	ON_COMMAND(ID_MESH_SHOWVECTORDISPLACEMENT, &CFormView3::OnShowVertexDisplacement)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWVECTORDISPLACEMENT, &CFormView3::OnUpdateShowVertexDisplacement)
	ON_COMMAND(ID_MESH_SHOWVERTEXMU, &CFormView3::OnShowVertexMU)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWVERTEXMU, &CFormView3::OnUpdateShowVertexMU)
	ON_COMMAND(ID_MESH_SHOWTRIANGLEMU, &CFormView3::OnShowTriangleMU)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWTRIANGLEMU, &CFormView3::OnUpdateShowTriangleMU)
	ON_COMMAND(ID_MESH_SHOWFACEAREA, &CFormView3::OnMeshShowFaceArea)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWFACEAREA, &CFormView3::OnUpdateMeshShowFaceArea)
	ON_COMMAND(ID_MESH_SHOWXYINDEX, &CFormView3::OnMeshShowFaceXYIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWXYINDEX, &CFormView3::OnUpdateMeshShowFaceXYIndex)
	ON_COMMAND(ID_MESH_SHOWCELLXYINDEX, &CFormView3::OnMeshShowCellXYIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWCELLXYINDEX, &CFormView3::OnUpdateMeshShowCellXYIndex)
	ON_COMMAND(ID_MESH_SHOWTargetFaceArea, &CFormView3::OnMeshShowTargetFaceArea)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWTargetFaceArea, &CFormView3::OnUpdateMeshShowTargetFaceArea)
	ON_COMMAND(ID_MESH_SHOWAREADIFFERENCE, &CFormView3::OnMeshShowDifferenceFaceArea)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWAREADIFFERENCE, &CFormView3::OnUpdateMeshShowDifferenceFaceArea)
	ON_COMMAND(ID_MESH_SHOWSHARPEDGEINDEX, &CFormView3::OnMeshShowSharpEdgeIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSHARPEDGEINDEX, &CFormView3::OnUpdateMeshShowSharpEdgeIndex)
	ON_COMMAND(ID_MESH_SHOWLANDMARKINDEX, &CFormView3::OnMeshShowLandmarkIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWLANDMARKINDEX, &CFormView3::OnUpdateMeshShowLandmarkIndex)
	ON_COMMAND(ID_MESH_SHOWLANDMARKENDVERTEXINDEX, &CFormView3::OnMeshShowLandmarkVertexIndex)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWLANDMARKENDVERTEXINDEX, &CFormView3::OnUpdateMeshShowLandmarkVertexIndex)
	ON_COMMAND(ID_MESH_SHOWANCHORPOINTS, &CFormView3::OnMeshShowAnchorPonits)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWANCHORPOINTS, &CFormView3::OnUpdateMeshShowAnchorPonits)
	ON_COMMAND(ID_CURVE_SHOWCONTROLPOINTS, &CFormView3::OnCurveShowcontrolpoints)
	ON_UPDATE_COMMAND_UI(ID_CURVE_SHOWCONTROLPOINTS, &CFormView3::OnUpdateCurveShowcontrolpoints)
	ON_COMMAND(ID_SURFACE_COMPUTECONFORMALENERGY, &CFormView3::OnSurfaceComputeconformalenergy)
	ON_COMMAND(ID_ACTIONS_KILLCURRENTT, &CFormView3::OnActionsKillcurrentt)
	ON_COMMAND(ID_MESH_SHOWMESHINFORMATION, &CFormView3::OnMeshShowmeshinformation)
	ON_COMMAND(ID_MESH_EXTRACTPARAMETERDOMAIN, &CFormView3::OnMeshExtractparameterdomain)
	ON_COMMAND(ID_MENU_MESH_ZOOMALL, &CFormView3::OnMeshMenuZoomAll)
	ON_COMMAND(ID_MENU_ZOOM_CURRENTMESH_TOFIT, &CFormView3::OnMeshZoomCurrentMeshToFit)
	ON_COMMAND(ID_MESH_ONLYMOVEME, &CFormView3::OnMeshOnlyMoveMe)
	ON_COMMAND(ID_ZoomAllToFitCurrentMesh, &CFormView3::OnMeshZoomAllToFitCurrentMesh)
	ON_COMMAND(ID_MESH_SHOWRECTILINEARGRID, &CFormView3::OnMeshShowrectilineargrid)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWRECTILINEARGRID, &CFormView3::OnUpdateMeshShowrectilineargrid)
	ON_COMMAND(ID_MESH_SHOWSHARPEDGEBASENUM, &CFormView3::OnMeshShowsharpedgebasenum)
	ON_UPDATE_COMMAND_UI(ID_MESH_SHOWSHARPEDGEBASENUM, &CFormView3::OnUpdateMeshShowsharpedgebasenum)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CFormView3 diagnostics
#ifdef _DEBUG
void CFormView3::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormView3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

extern CMyTreeView* pTreeView;
/////////////////////////////////////////////////////////////////////////////
void CFormView3::OnMeshMenuZoomAll()
{
	pTreeView->OnZoomall();
}
void CFormView3::OnMeshOnlyMoveMe()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	for (int i = 0; i < num; i++)
		pDoc->m_SceneGraph.GetAt(i)->m_IsMove = 0;

	pDoc->m_SceneGraph.GetAt(currentindex)->m_IsMove = 1;

	pTreeView->m_xTranslation = pDoc->m_SceneGraph.GetAt(currentindex)->m_xTranslation;
	pTreeView->m_yTranslation = pDoc->m_SceneGraph.GetAt(currentindex)->m_yTranslation;
	pTreeView->m_zTranslation = pDoc->m_SceneGraph.GetAt(currentindex)->m_zTranslation;
	pTreeView->m_xRotation = pDoc->m_SceneGraph.GetAt(currentindex)->m_xRotation;
	pTreeView->m_yRotation = pDoc->m_SceneGraph.GetAt(currentindex)->m_yRotation;
	pTreeView->m_zRotation = pDoc->m_SceneGraph.GetAt(currentindex)->m_zRotation;
	pTreeView->m_xScaling = pDoc->m_SceneGraph.GetAt(currentindex)->m_xScaling;
	pTreeView->m_yScaling = pDoc->m_SceneGraph.GetAt(currentindex)->m_yScaling;
	pTreeView->m_zScaling = pDoc->m_SceneGraph.GetAt(currentindex)->m_zScaling;
}
void CFormView3::OnMeshZoomCurrentMeshToFit()
{
	pTreeView->OnZoomToFitCurrentMesh();
}
void CFormView3::OnMeshZoomAllToFitCurrentMesh()
{
	pTreeView->OnZoomAllToFitCurrentMesh();
}
HBRUSH CFormView3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{   //set color, and background text color for edit controls in this formview:
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(m_crGreen);
		return *m_pEditBkBrush;
	default:
		return CColorFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}
void CFormView3::UpdateSliderPosition()
{
	transformationTab.mobiustab.UpdateData(TRUE);
	transformationTab.bilineartab.UpdateData(TRUE);
	transformationTab.quadratictab.UpdateData(TRUE);
	transformationTab.generalbilineartab.UpdateData(TRUE);
	transformationTab.UpdateData(TRUE);
	objTab.UpdateData(TRUE);

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_Disc1 = objTab.m_slider1;
	pDoc->m_Disc2 = objTab.m_slider2;

	//if(pDoc->m_SceneGraph.NbObject() == 0)
	//	return;

	//CTreeCtrl* pTreeControl = (CTreeCtrl*)objTab.GetDlgItem(IDC_Objects);

	//vector<HTREEITEM>::iterator treeitem;
	//treeitem = find(m_ArraySurfaceItems.begin(),m_ArraySurfaceItems.end(),pTreeControl->GetSelectedItem());

	//int index = treeitem - m_ArraySurfaceItems.begin();
	//if(index < 0 || index >= pDoc->m_SceneGraph.NbObject())
	//	index = 0;

	//if(pDoc->m_SceneGraph.GetAt(index)->GetType() != TYPE_NURBSSURFACE)
	//	return;

	//CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(index);


	//pSurf->reparameterization_coefficients.clear();

	/*int m_Mobiustransformation_Alpha = pSurf->reparameterization_coefficients[0] * 100;
	int m_Mobiustransformation_Beta = pSurf->reparameterization_coefficients[1] * 100;
	int m_Bilinear_Alpha1 = pSurf->reparameterization_coefficients[2] * 100;
	int m_Bilinear_Alpha2 = pSurf->reparameterization_coefficients[3] * 100;
	int m_Bilinear_Beta1 = pSurf->reparameterization_coefficients[4] * 100;
	int m_Bilinear_Beta2 = pSurf->reparameterization_coefficients[5] * 100;
	int m_Quadratic_Alpha1 = pSurf->reparameterization_coefficients[6] * 100;
	int m_Quadratic_Alpha2 = pSurf->reparameterization_coefficients[7] * 100;
	int m_Quadratic_Alpha3 = pSurf->reparameterization_coefficients[8] * 100;
	int m_Quadratic_Beta1 = pSurf->reparameterization_coefficients[9] * 100;
	int m_Quadratic_Beta2 = pSurf->reparameterization_coefficients[10] * 100;
	int m_Quadratic_Beta3 = pSurf->reparameterization_coefficients[11] * 100;
	int m_e00 = pSurf->reparameterization_coefficients[12] * 100 + 50;
	int m_e01 = pSurf->reparameterization_coefficients[13] * 100 + 50;
	int m_e10 = pSurf->reparameterization_coefficients[14] * 100 - 50;
	int m_e11 = pSurf->reparameterization_coefficients[15] * 100 - 50;
	int m_f00 = pSurf->reparameterization_coefficients[16] * 100 - 50;
	int m_f01 = pSurf->reparameterization_coefficients[17] * 100 - 50;
	int m_f10 = pSurf->reparameterization_coefficients[18] * 100 - 50;
	int m_f11 = pSurf->reparameterization_coefficients[19] * 100 - 50;
	int m_g00 = pSurf->reparameterization_coefficients[20] * 100 + 50;
	int m_g01 = pSurf->reparameterization_coefficients[21] * 100 - 50;
	int m_g10 = pSurf->reparameterization_coefficients[22] * 100 + 50;
	int m_g11 = pSurf->reparameterization_coefficients[23] * 100 - 50;
	int m_h00 = pSurf->reparameterization_coefficients[24] * 100 - 50;
	int m_h01 = pSurf->reparameterization_coefficients[25] * 100 - 50;
	int m_h10 = pSurf->reparameterization_coefficients[26] * 100 - 50;
	int m_h11 = pSurf->reparameterization_coefficients[27] * 100 - 50;

	if(m_Mobiustransformation_Alpha != transformationTab.mobiustab.m_Mobiustransformation_Alpha ||
		m_Mobiustransformation_Beta != transformationTab.mobiustab.m_Mobiustransformation_Beta)
	{
		pSurf->reparameterization_coefficients[0] = transformationTab.mobiustab.m_Mobiustransformation_Alpha / 100.0;
		pSurf->reparameterization_coefficients[1] = transformationTab.mobiustab.m_Mobiustransformation_Beta / 100.0;
		pSurf->m_ReparameterizationType = mobius;
	}else
	if( m_Bilinear_Alpha1 != transformationTab.bilineartab.m_Bilinear_Alpha1 ||
	   m_Bilinear_Alpha2 != transformationTab.bilineartab.m_Bilinear_Alpha2 ||
	   m_Bilinear_Beta1 != transformationTab.bilineartab.m_Bilinear_Beta1 ||
	   m_Bilinear_Beta2 != transformationTab.bilineartab.m_Bilinear_Beta2)
	{
		pSurf->reparameterization_coefficients[2] = transformationTab.bilineartab.m_Bilinear_Alpha1 / 100.0;
		pSurf->reparameterization_coefficients[3] = transformationTab.bilineartab.m_Bilinear_Alpha2 / 100.0;
		pSurf->reparameterization_coefficients[4] = transformationTab.bilineartab.m_Bilinear_Beta1 / 100.0;
		pSurf->reparameterization_coefficients[5] = transformationTab.bilineartab.m_Bilinear_Beta2 / 100.0;

		pSurf->m_ReparameterizationType = bilinear;
	}else
	if(m_Quadratic_Alpha1 != transformationTab.quadratictab.m_Quadratic_Alpha1 ||
	   m_Quadratic_Alpha2 != transformationTab.quadratictab.m_Quadratic_Alpha2 ||
	   m_Quadratic_Alpha3 != transformationTab.quadratictab.m_Quadratic_Alpha3 ||
	   m_Quadratic_Beta1 != transformationTab.quadratictab.m_Quadratic_Beta1 ||
	   m_Quadratic_Beta2 != transformationTab.quadratictab.m_Quadratic_Beta2 ||
	   m_Quadratic_Beta3 != transformationTab.quadratictab.m_Quadratic_Beta3)
	{
		pSurf->reparameterization_coefficients[6] = transformationTab.quadratictab.m_Quadratic_Alpha1 / 100.0;
		pSurf->reparameterization_coefficients[7] = transformationTab.quadratictab.m_Quadratic_Alpha2 / 100.0;
		pSurf->reparameterization_coefficients[8] = transformationTab.quadratictab.m_Quadratic_Alpha3 / 100.0;
		pSurf->reparameterization_coefficients[9] = transformationTab.quadratictab.m_Quadratic_Beta1 / 100.0;
		pSurf->reparameterization_coefficients[10] = transformationTab.quadratictab.m_Quadratic_Beta2 / 100.0;
		pSurf->reparameterization_coefficients[11] = transformationTab.quadratictab.m_Quadratic_Beta3 / 100.0;

		pSurf->m_ReparameterizationType = quadratic;
	}else
	if(m_e00 != transformationTab.generalbilineartab.m_e00 ||
	   m_e01 != transformationTab.generalbilineartab.m_e01 ||
	   m_e10 != transformationTab.generalbilineartab.m_e10 ||
	   m_e11 != transformationTab.generalbilineartab.m_e11 ||
	   m_f00 != transformationTab.generalbilineartab.m_f00 ||
	   m_f01 != transformationTab.generalbilineartab.m_f01 ||
	   m_f10 != transformationTab.generalbilineartab.m_f10 ||
	   m_f11 != transformationTab.generalbilineartab.m_f11 ||
	   m_g00 != transformationTab.generalbilineartab.m_g00 ||
	   m_g01 != transformationTab.generalbilineartab.m_g01 ||
	   m_g10 != transformationTab.generalbilineartab.m_g10 ||
	   m_g11 != transformationTab.generalbilineartab.m_g11 ||
	   m_h00 != transformationTab.generalbilineartab.m_h00 ||
	   m_h01 != transformationTab.generalbilineartab.m_h01 ||
	   m_h10 != transformationTab.generalbilineartab.m_h10 ||
	   m_h11 != transformationTab.generalbilineartab.m_h11)
	{
		pSurf->reparameterization_coefficients[12] = transformationTab.generalbilineartab.m_e00 / 100.0 - 0.5;
		pSurf->reparameterization_coefficients[13] = transformationTab.generalbilineartab.m_e01 / 100.0 - 0.5;
		pSurf->reparameterization_coefficients[14] = transformationTab.generalbilineartab.m_e10 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[15] = transformationTab.generalbilineartab.m_e11 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[16] = transformationTab.generalbilineartab.m_f00 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[17] = transformationTab.generalbilineartab.m_f01 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[18] = transformationTab.generalbilineartab.m_f10 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[19] = transformationTab.generalbilineartab.m_f11 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[20] = transformationTab.generalbilineartab.m_g00 / 100.0 - 0.5;
		pSurf->reparameterization_coefficients[21] = transformationTab.generalbilineartab.m_g01 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[22] = transformationTab.generalbilineartab.m_g10 / 100.0 - 0.5;
		pSurf->reparameterization_coefficients[23] = transformationTab.generalbilineartab.m_g11 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[24] = transformationTab.generalbilineartab.m_h00 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[25] = transformationTab.generalbilineartab.m_h01 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[26] = transformationTab.generalbilineartab.m_h10 / 100.0 + 0.5;
		pSurf->reparameterization_coefficients[27] = transformationTab.generalbilineartab.m_h11 / 100.0 + 0.5;

		pSurf->m_ReparameterizationType = generalbilinear;
		pSurf->m_Modified = TRUE;
	}*/

	/*if(index == 0 && pDoc->m_SceneGraph.NbObject() == bilinear)
	{
		CNurbsSuface* pSurf1 = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(1);
		pSurf1->m_ReparameterizationType = pSurf->m_ReparameterizationType;
		if(pSurf->m_ReparameterizationType == bilinear)
		{
			pSurf1->reparameterization_coefficients[2] = pSurf->reparameterization_coefficients[2] * 2 - pSurf->reparameterization_coefficients[3];
			pSurf1->reparameterization_coefficients[3] = pSurf->reparameterization_coefficients[2];
			pSurf1->reparameterization_coefficients[4] = 100 - pSurf->reparameterization_coefficients[4];
			pSurf1->reparameterization_coefficients[5] = 100 - pSurf->reparameterization_coefficients[5];
		}else
		if(pSurf->m_ReparameterizationType == quadratic)
		{
			pSurf1->reparameterization_coefficients[11] = 100 -  pSurf->reparameterization_coefficients[11];
			pSurf1->reparameterization_coefficients[9] = 100 -  pSurf->reparameterization_coefficients[9];
			pSurf1->reparameterization_coefficients[10] = 100 -  pSurf->reparameterization_coefficients[10];
			pSurf1->reparameterization_coefficients[6] = pSurf->reparameterization_coefficients[8];
			pSurf1->reparameterization_coefficients[7] = pSurf->reparameterization_coefficients[8] * 2 - pSurf->reparameterization_coefficients[7];
		}
	}*/

	UpdateShowView();
}

void CFormView3::UpdateShowView()
{
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
void CFormView3::OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here

	UpdateSliderPosition();
	*pResult = 0;
}

void CFormView3::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (nFlags & MK_CONTROL)
	{
		m_bPicking = TRUE;

		HWND hWnd = GetSafeHwnd();
		HDC hDC = ::GetDC(hWnd);

		wglMakeCurrent(hDC, m_hRC);

		wglMakeCurrent(NULL, NULL);
	}
	else if (nFlags & MK_LBUTTON)
	{
		m_bDragging = TRUE;
	}
	CColorFormView::OnLButtonDown(nFlags, point);
}
void CFormView3::OnInitialUpdate()
{
	//
	if (m_bInitialized)
		return;
	m_bInitialized = true;

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	//CString strTemp;
	//((CComboBoxEx  *)GetDlgItem(IDC_COMBOSELECT))->ResetContent(); //消除现有所有内容
	//m_SelectComboBox.AddString("Points");
	//((CComboBoxEx *)GetDlgItem(IDC_COMBOSELECT))->AddString("Control Point");
	//((CComboBoxEx *)GetDlgItem(IDC_COMBOSELECT))->AddString("Control Point1");
	/*m_ComboSelectContent = "Select; Control Points;";
	m_ComboSelectIndex = 0;
	COMBOBOXEXITEM     cbi;
	CString            str;
	int                nItem;
	int i=0;
	cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY |
		CBEIF_SELECTEDIMAGE | CBEIF_TEXT;

	cbi.iItem = i;
	str.Format(_T("Fix One Corner"));
	cbi.pszText = (LPTSTR)(LPCTSTR)str;
	cbi.cchTextMax = str.GetLength();
	cbi.iImage = 0;
	cbi.iSelectedImage = 1;
	cbi.iOverlay = 2;
	cbi.iIndent = (i & 0x03);   //Set indentation according
	//to item position
	int ItemNumber=((CComboBoxEx *)GetDlgItem(IDC_COMBOSELECT))->GetCount();
	for(int i=0;i<ItemNumber;i++)
		((CComboBoxEx *)GetDlgItem(IDC_COMBOSELECT))->DeleteItem(0);
	nItem = ((CComboBoxEx *)GetDlgItem(IDC_COMBOSELECT))->InsertItem(&cbi);

	cbi.iItem = ++i;
	str.Format(_T("Fix Three Corner"));
	cbi.pszText = (LPTSTR)(LPCTSTR)str;
	cbi.cchTextMax = str.GetLength();
	nItem = ((CComboBoxEx *)GetDlgItem(IDC_COMBOSELECT))->InsertItem(&cbi);

	cbi.iItem = ++i;
	str.Format(_T("Fix Four Corner"));
	cbi.pszText = (LPTSTR)(LPCTSTR)str;
	cbi.cchTextMax = str.GetLength();
	nItem = ((CComboBoxEx *)GetDlgItem(IDC_COMBOSELECT))->InsertItem(&cbi);


	cbi.iItem = ++i;
	str.Format(_T("Fix Zero Corner"));
	cbi.pszText = (LPTSTR)(LPCTSTR)str;
	cbi.cchTextMax = str.GetLength();
	nItem = ((CComboBoxEx *)GetDlgItem(IDC_COMBOSELECT))->InsertItem(&cbi);

	ASSERT(nItem == i);*/


	//UpdateData(FALSE);	
	CColorFormView::OnInitialUpdate();

	//初始化m_tab控件上的属性页标签
	m_tab.InsertItem(0, _T("objectViewer"));//objectViewer
	m_tab.InsertItem(1, _T("Transformations"));//Transformations
	m_tab.InsertItem(2, _T("Actions"));//Actions

	//创建属性页;
	objTab.Create(IDD_objViewerTab, GetDlgItem(IDC_TAB2));
	transformationTab.Create(IDD_TransformationsTAB, GetDlgItem(IDC_TAB2));
	actionTab.Create(IDD_ActionsTab, GetDlgItem(IDC_TAB2));

	//获取TAB的客户端矩形框，从而设置各属性页在TAB上的物理位置
	CRect rs;
	m_tab.GetClientRect(&rs);
	rs.top += 20;
	rs.bottom -= 4;
	rs.left += 4;
	rs.right -= 4;

	//设置属性页的大小和位置
	objTab.MoveWindow(&rs);
	transformationTab.MoveWindow(&rs);
	actionTab.MoveWindow(&rs);

	//默认第一页显示
	objTab.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
}

void CFormView3::DrawScene(CDC* pdc, GLenum mode)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num != 1)
		return;
}
void CFormView3::SamplingMesh()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	Point3D lower, upper;
	pMesh1->ComputeBoundingBox(lower, upper);
	double minu = lower.x;
	double minv = lower.y;

	double maxu = upper.x;
	double maxv = upper.y;

	//for
}
/*void CFormView3::GenerateFittingCurve(std::vector<Point3D> &m_Array,std::vector<Point2D> &m_para,C_NurbsSurf* pSurf)
{
	int m = m_Array.size();
	int u_order = 4;
	int uknotnum = 9;
	int n = u_order + uknotnum;

	double** N = new double[m-1][n-1];
	for(int j  =1; j <= m-1; j++)
		for(int i = 1; i <= n-1; i++)
		{
			pSurf->ComputeUBasis(i,m_para[i])
		}
}*/
void CFormView3::GetBsplineFittingError()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() != 3)
		return;

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(2);
	C_NurbsSurf* pSurface1 = pSurface->Converte_C_NurbsSurface();
	double umin = pSurface1->GetUKnot(0);
	double umax = pSurface1->GetUKnot(pSurface1->GetCtrlNumberU() + pSurface1->GetOrderU() - 1);

	double vmin = pSurface1->GetVKnot(0);
	double vmax = pSurface1->GetVKnot(pSurface1->GetCtrlNumberV() + pSurface1->GetOrderV() - 1);

	Point3D lower, upper, point;
	pMesh2->ComputeBoundingBox(lower, upper);

	double dis = 0;
	for (int i = 0; i < pMesh2->NbVertex(); i++)
	{
		double x1 = pMesh1->GetVertex(i)->x();
		double y1 = pMesh1->GetVertex(i)->y();
		double z1 = pMesh1->GetVertex(i)->z();

		double x2 = pMesh2->GetVertex(i)->x();
		double y2 = pMesh2->GetVertex(i)->y();
		double z2 = pMesh2->GetVertex(i)->z();

		x2 = umin + (x2 - lower.x) * (umax - umin) / (upper.x - lower.x);
		y2 = vmin + (y2 - lower.y) * (vmax - vmin) / (upper.y - lower.y);

		point = pSurface1->ComputPosition(x2, y2);
		dis = dis + sqrt(SQR(point.x - x1) + SQR(point.y - y1) + SQR(point.z - z1));
	}
	dis = dis / pMesh2->NbVertex();
	char message[80];
	sprintf(message, "error: %f", dis);
	AfxMessageBox(message);
}

void CFormView3::OnBnClickedBsplineSurface()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() != 2)
		return;
	//The 3D triangle mesh
	//pMesh2 the parameterization
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);

	Point3D lower, upper;
	pMesh2->ComputeBoundingBox(lower, upper);
	double length = upper.x - lower.x;
	if (length < upper.y - lower.y)
		length = upper.y - lower.y;

	Point3D p00, pm0, p0n, pmn;
	int index00, indexm0, index0n, indexmn;
	pMesh2->GetFourCornerPoints(index00, indexm0, index0n, indexmn);
	p00[0] = pMesh1->GetVertex(index00)->x();
	p00[1] = pMesh1->GetVertex(index00)->y();
	p00[2] = pMesh1->GetVertex(index00)->z();

	pm0[0] = pMesh1->GetVertex(indexm0)->x();
	pm0[1] = pMesh1->GetVertex(indexm0)->y();
	pm0[2] = pMesh1->GetVertex(indexm0)->z();

	p0n[0] = pMesh1->GetVertex(index0n)->x();
	p0n[1] = pMesh1->GetVertex(index0n)->y();
	p0n[2] = pMesh1->GetVertex(index0n)->z();

	pmn[0] = pMesh1->GetVertex(indexmn)->x();
	pmn[1] = pMesh1->GetVertex(indexmn)->y();
	pmn[2] = pMesh1->GetVertex(indexmn)->z();

	int vnum = pMesh2->NbVertex();
	//vnum = 100;
	//degree + 1
	int u_order = 4;
	int v_order = 4;

	//int uknotnum = 9;
	//int vknotnum = 9;
	//
	/*int uknotnum = 32;
	int vknotnum = 31;

	double uknot[40] = {0, 0, 0, 0, 0.05, 0.1, 0.15, 0.2, 0.22, 0.24,
					  0.26, 0.28, 0.3, 0.32, 0.34, 0.36, 0.38, 0.4,0.45,0.5,
					  0.55,0.6,0.65,0.7,0.72,0.74,0.76,0.78,0.8,0.82,
					  0.84,0.86,0.88,0.9,0.92,0.95,1,1,1,1};
	double vknot[39] = {0,0,0,0,0.05,0.1,0.15,0.2,0.25,0.3,
					  0.32,0.34,0.36,0.38,0.4,0.42,0.44,0.46,0.48,0.5,
					  0.52,0.54,0.56,0.58,0.6,0.62,0.64,0.66,0.68,0.7,
					  0.75,0.8,0.85,0.9,0.95,1,1,1,1};*/

	int uknotnum = 31;
	int vknotnum = 31;

	//double uknot[21] = {0, 0, 0, 0, 0.05,0.1,0.15,0.2, 0.3, 0.4,0.5,0.6,0.65,0.7,0.75,0.8,0.9,1,1,1,1};
	//double vknot[22] = {0, 0, 0, 0, 0.1, 0.2, 0.25,0.3, 0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.8,0.9,1,1,1,1};

	/*int uknotnum = 17;
	int vknotnum = 15;

	double uknot[25] = {-0.66,  -0.66,  -0.66,  -0.66,  -0.56,  -0.46,  -0.36,  -0.26, -0.23, -0.2,
						 -0.16,  -0.06,  0.04, 0.07, 0.1, 0.14, 0.17, 0.2, 0.24, 0.27,
						 0.3,  0.34,  0.34,  0.34,  0.34};
	double vknot[23] = { -0.35,  -0.35,  -0.35,  -0.35,  -0.26,  -0.16,  -0.07,  0.03, 0.06, 0.09,
					   0.12, 0.15, 0.18, 0.21, 0.24, 0.27, 0.31,  0.40,  0.49,  0.59,
					   0.59,  0.59,  0.59 };*/

					   /*int uknotnum = 17;
					   int vknotnum = 14;
					   double uknot[25] = {-0.64,  -0.64,  -0.64,  -0.64, -0.61, -0.58, -0.54, -0.51, -0.48, -0.44,
											-0.34,  -0.24,  -0.14, -0.11, -0.08, -0.04, -0.01, 0.02, 0.06,  0.16,
											0.26,  0.36,  0.36,  0.36,  0.36};
					   double vknot[22] = { -0.21,  -0.21,  -0.21,  -0.21,  -0.11,  -0.02, 0.025, 0.07, 0.12, 0.17,
											 0.22, 0.26, 0.31, 0.36, 0.41, 0.45,  0.55,  0.64,  0.73,  0.73,
											 0.73,  0.73};
				   */
	double* uknot = new double[u_order * 2 + uknotnum];
	double* vknot = new double[v_order * 2 + vknotnum];

	for (int i = 0; i < u_order; i++)
	{
		uknot[i] = lower.x;
		uknot[u_order + uknotnum + i] = upper.x;
	}

	for (int i = 0; i < v_order; i++)
	{
		vknot[i] = lower.y;
		vknot[v_order + vknotnum + i] = upper.y;
	}

	for (int i = 1; i <= uknotnum; i++)
	{
		uknot[u_order + i - 1] = (upper.x - lower.x) * i / (uknotnum + 1) + lower.x;
	}

	for (int i = 1; i <= vknotnum; i++)
	{
		vknot[v_order + i - 1] = (upper.y - lower.y) * i / (vknotnum + 1) + lower.y;
	}

	//
	TRACE("\n uknot ");
	for (int i = 0; i < u_order * 2 + uknotnum; i++)
	{
		TRACE("%f ", uknot[i]);
	}
	TRACE("\n vknot ");
	for (int i = 0; i < v_order * 2 + vknotnum; i++)
	{
		TRACE("%f ", vknot[i]);
	}

	Point3D* pControlPoint = new Point3D[(u_order + uknotnum) * (v_order + vknotnum)];

	double* pWeight = new double[(u_order + uknotnum) * (v_order + vknotnum)];

	C_NurbsSurf* pNurbsSurface = new C_NurbsSurf(u_order + uknotnum, v_order + vknotnum, u_order, v_order, uknot, vknot, pControlPoint, pWeight);

	double c;
	std::vector<std::vector<double>> coeffcients;

	for (int j1 = 0; j1 < v_order + vknotnum; j1++)
		for (int i1 = 0; i1 < u_order + uknotnum; i1++)
		{
			//the four corners
			if (i1 == 0 && j1 == 0)
				continue;
			if (i1 == u_order + uknotnum - 1 && j1 == 0)
				continue;
			if (i1 == 0 && j1 == v_order + vknotnum - 1)
				continue;
			if (i1 == u_order + uknotnum - 1 && j1 == v_order + vknotnum - 1)
				continue;

			std::vector<double> coeff;
			for (int j = 0; j < v_order + vknotnum; j++)
				for (int i = 0; i < u_order + uknotnum; i++)
				{
					//the four corners
					if (i == 0 && j == 0)
						continue;
					if (i == u_order + uknotnum - 1 && j == 0)
						continue;
					if (i == 0 && j == v_order + vknotnum - 1)
						continue;
					if (i == u_order + uknotnum - 1 && j == v_order + vknotnum - 1)
						continue;

					c = 0;
					for (int k = 0; k < vnum; k++)
					{
						CVertex3d* pVertex = pMesh2->GetVertex(k);
						double u = pVertex->x();
						double v = pVertex->y();

						//u = (u - lower.x) / length;
						//v = (v - lower.y) / length;

						//u = (u - lower.x) / (upper.x - lower.x);
						//v = (v - lower.y) / (upper.y - lower.y);
						if (u >= pNurbsSurface->GetUKnot(i) && u <= pNurbsSurface->GetUKnot(i + u_order - 1) &&
							v >= pNurbsSurface->GetVKnot(j) && v <= pNurbsSurface->GetVKnot(j + v_order - 1) &&
							u >= pNurbsSurface->GetUKnot(i1) && u <= pNurbsSurface->GetUKnot(i1 + u_order - 1) &&
							v >= pNurbsSurface->GetVKnot(j1) && v <= pNurbsSurface->GetVKnot(j1 + v_order - 1))
						{
							int ij = 0;
						}

						double ubas = pNurbsSurface->ComputeUBasis(i, u);
						double vbas = pNurbsSurface->ComputeVBasis(j, v);

						double ubas1 = pNurbsSurface->ComputeUBasis(i1, u);
						double vbas1 = pNurbsSurface->ComputeVBasis(j1, v);
						c = c + ubas * vbas * ubas1 * vbas1;
					}
					coeff.push_back(c);
				}
			c = 0;
			for (int k = 0; k < vnum; k++)
			{
				CVertex3d* pVertex = pMesh2->GetVertex(k);
				CVertex3d* pVertex1 = pMesh1->GetVertex(k);
				double u = pVertex->x();
				double v = pVertex->y();

				//u = (u - lower.x) / length;
				//v = (v - lower.y) / length;

				//u = (u - lower.x) / (upper.x - lower.x);
				//v = (v - lower.y) / (upper.y - lower.y);

				double ubas = pNurbsSurface->ComputeUBasis(i1, u);
				double vbas = pNurbsSurface->ComputeVBasis(j1, v);

				double ubas0 = pNurbsSurface->ComputeUBasis(0, u);
				double vbas0 = pNurbsSurface->ComputeVBasis(0, v);

				double ubasm = pNurbsSurface->ComputeUBasis(u_order + uknotnum - 1, u);
				double vbasn = pNurbsSurface->ComputeVBasis(v_order + vknotnum - 1, v);

				c = c + ubas * vbas * (pVertex1->x() - (ubas0 * vbas0 * p00.x + ubasm * vbas0 * pm0.x + ubas0 * vbasn * p0n.x + ubasm * vbasn * pmn.x));
			}
			coeff.push_back(c);
			c = 0;
			for (int k = 0; k < vnum; k++)
			{
				CVertex3d* pVertex = pMesh2->GetVertex(k);
				CVertex3d* pVertex1 = pMesh1->GetVertex(k);
				double u = pVertex->x();
				double v = pVertex->y();

				double ubas = pNurbsSurface->ComputeUBasis(i1, u);
				double vbas = pNurbsSurface->ComputeVBasis(j1, v);

				double ubas0 = pNurbsSurface->ComputeUBasis(0, u);
				double vbas0 = pNurbsSurface->ComputeVBasis(0, v);

				double ubasm = pNurbsSurface->ComputeUBasis(u_order + uknotnum - 1, u);
				double vbasn = pNurbsSurface->ComputeVBasis(v_order + vknotnum - 1, v);

				c = c + ubas * vbas * (pVertex1->y() - (ubas0 * vbas0 * p00.y + ubasm * vbas0 * pm0.y + ubas0 * vbasn * p0n.y + ubasm * vbasn * pmn.y));
			}
			coeff.push_back(c);
			c = 0;
			for (int k = 0; k < vnum; k++)
			{
				CVertex3d* pVertex = pMesh2->GetVertex(k);
				CVertex3d* pVertex1 = pMesh1->GetVertex(k);
				double u = pVertex->x();
				double v = pVertex->y();

				//u = (u - lower.x) / length;
				//v = (v - lower.y) / length;

				//u = (u - lower.x) / (upper.x - lower.x);
				//v = (v - lower.y) / (upper.y - lower.y);

				double ubas = pNurbsSurface->ComputeUBasis(i1, u);
				double vbas = pNurbsSurface->ComputeVBasis(j1, v);

				double ubas0 = pNurbsSurface->ComputeUBasis(0, u);
				double vbas0 = pNurbsSurface->ComputeVBasis(0, v);

				double ubasm = pNurbsSurface->ComputeUBasis(u_order + uknotnum - 1, u);
				double vbasn = pNurbsSurface->ComputeVBasis(v_order + vknotnum - 1, v);

				c = c + ubas * vbas * (pVertex1->z() - (ubas0 * vbas0 * p00.z + ubasm * vbas0 * pm0.z + ubas0 * vbasn * p0n.z + ubasm * vbasn * pmn.z));
			}
			coeff.push_back(c);
			coeffcients.push_back(coeff);
		}
	//Trace
	/*TRACE("\nCoefficients");
	for(int i = 0; i < coeffcients.size(); i++)
	{
		TRACE("\n");
		for(int j = 0; j < coeffcients[i].size(); j++)
		{
			TRACE("%5.2f  ",coeffcients[i][j]);
		}
	}
	TRACE("\n");
	for(int j = 0; j < coeffcients.size(); j++)
	{
		TRACE("%5.2f  ",coeffcients[j][coeffcients.size()]);
	}
	TRACE("\n");
	for(int j = 0; j < coeffcients.size(); j++)
	{
		TRACE("%5.2f  ",coeffcients[j][coeffcients.size() + 1]);
	}*/
	//compute the matrix.

	using namespace alglib;

	//get the control points
	string str = "[";
	string str1 = "[";
	string str2 = "[";
	string str3 = "[";
	for (int i = 0; i < coeffcients.size(); i++)
	{
		str3 += "[";
		for (int j = 0; j < coeffcients.size(); j++)
		{
			str3 += DoubleToString(coeffcients[i][j]);
			if (j < coeffcients[i].size() - 4)
				str3 += ",";
		}
		str3 += "]";
		if (i < coeffcients.size() - 1)
			str3 += ",";
		str += DoubleToString(coeffcients[i][coeffcients.size()]);
		str1 += DoubleToString(coeffcients[i][coeffcients.size() + 1]);
		str2 += DoubleToString(coeffcients[i][coeffcients.size() + 2]);
		if (i < coeffcients.size() - 1)
		{
			str += ",";
			str1 += ",";
			str2 += ",";
		}
		else
		{
			str += "]";
			str1 += "]";
			str2 += "]";
		}
	}
	str3 += "]";

	real_1d_array	b(str.c_str());
	real_1d_array	b1(str1.c_str());
	real_1d_array	b2(str2.c_str());
	real_2d_array 	A(str3.c_str());
	real_1d_array   x;
	real_1d_array   y;
	real_1d_array   z;
	int64_t val;
	densesolverreport rep;

	//TRACE("\n %s \n",str3);
	//TRACE("\n %s \n",str);
	//TRACE("\n %s \n",str1);
	//TRACE("\n %s \n",str2);
	rmatrixsolve(A, coeffcients.size(), b, val, rep, x);
	rmatrixsolve(A, coeffcients.size(), b1, val, rep, y);
	rmatrixsolve(A, coeffcients.size(), b2, val, rep, z);
	/*TRACE("x: \n");
	for(int i = 0; i < coeffcients.size(); i++)
		TRACE("%f ",x[i]);
	TRACE("y: \n");
	for(int i = 0; i < coeffcients.size(); i++)
		TRACE("%f ",y[i]);
	TRACE("z: \n");
	for(int i = 0; i < coeffcients.size(); i++)
		TRACE("%f ",z[i]);*/

	int num = 0;

	//four coner points
	pNurbsSurface->SetControlPoint(0, 0, p00);
	pNurbsSurface->SetControlPoint(u_order + uknotnum - 1, 0, pm0);
	pNurbsSurface->SetControlPoint(0, v_order + vknotnum - 1, p0n);
	pNurbsSurface->SetControlPoint(u_order + uknotnum - 1, v_order + vknotnum - 1, pmn);
	pNurbsSurface->SetWeight(0, 0, 1);
	pNurbsSurface->SetWeight(u_order + uknotnum - 1, 0, 1);
	pNurbsSurface->SetWeight(0, v_order + vknotnum - 1, 1);
	pNurbsSurface->SetWeight(u_order + uknotnum - 1, v_order + vknotnum - 1, 1);

	for (int j1 = 0; j1 < v_order + vknotnum; j1++)
		for (int i1 = 0; i1 < u_order + uknotnum; i1++)
		{
			//the four corners
			if (i1 == 0 && j1 == 0)
				continue;
			if (i1 == u_order + uknotnum - 1 && j1 == 0)
				continue;
			if (i1 == 0 && j1 == v_order + vknotnum - 1)
				continue;
			if (i1 == u_order + uknotnum - 1 && j1 == v_order + vknotnum - 1)
				continue;

			Point3D point;
			point[0] = x[num];
			point[1] = y[num];
			point[2] = z[num];
			num++;
			pNurbsSurface->SetControlPoint(i1, j1, point);
			Point3D point1 = pNurbsSurface->GetControlPoint(i1, j1);
			pNurbsSurface->SetWeight(i1, j1, 1);
		}
	CNurbsSuface* pNurbs = pNurbsSurface->GetCNurbsSurface();
	pNurbs->SetTransform(*pMesh1->GetTransform());
	pDoc->m_SceneGraph.Add(pNurbs);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	/*Point3D* pCurveCtrl = new Point3D[u_order + uknotnum];

	for(int i = 0; i < u_order + uknotnum; i++)
	{
		pCurveCtrl[i].setValue(0,0,0);
	}
	double* pCurveWeight = new double[(u_order + uknotnum)];
	for(int i = 0; i < u_order + uknotnum; i++)
	{
		pCurveWeight[i] = 1;
	}
	pCurveCtrl[1].setValue(1,1,1);
	C_NurbsCurve3D* pNurbs =new C_NurbsCurve3D(u_order + uknotnum,u_order,pCurveCtrl,uknot,pCurveWeight);

	Point3D pp = pNurbs->ComputPosition(0.03);
	double u1 = pNurbsSurface->ComputeUBasis(1,0.03);*/

	//pNurbsSurface->ComputPosition()
}
void CFormView3::SetVaseModification()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	double a, b;
	//a = pSurf->m_Mobiustransformation_Alpha / 100.0;
	//b = pSurf->m_Mobiustransformation_Beta / 100.0;

	//a = 0.24;
	//b = 0.22;
	a = 0.25;
	b = 0.78;

	pSurf->uarraymodified.clear();
	pSurf->varraymodified.clear();

	double u, v, s, t;
	for (int i = 0; i < pSurf->uarray.size(); i++)
	{
		u = pSurf->uarray[i];
		s = u * a / (1 - a + 2 * a * u - u);
		pSurf->uarraymodified.push_back(s);
	}

	for (int i = 0; i < pSurf->varray.size(); i++)
	{
		v = pSurf->varray[i];
		t = v * b / (1 - b + 2 * b * v - v);
		pSurf->varraymodified.push_back(t);
	}

	int num1 = pSurf->uarraymodified.size();
	Point3D m_pointarray[10];

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->uarraymodified[i];
		p[1] = pSurf->uarraymodified[i];
		p[2] = pSurf->uarraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	double* knot;
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pUInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->varraymodified[i];
		p[1] = pSurf->varraymodified[i];
		p[2] = pSurf->varraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pVInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < pSurf->pUInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pUInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}
	for (int i = 0; i < pSurf->pVInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pVInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}
}
void CFormView3::SetBoneModification()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	double a, b;
	//a = pSurf->m_Mobiustransformation_Alpha / 100.0;
	//b = pSurf->m_Mobiustransformation_Beta / 100.0;

	//a = 0.24;
	//b = 0.22;
	a = 0.34;
	b = 0.38;

	pSurf->uarraymodified.clear();
	pSurf->varraymodified.clear();

	double u, v, s, t;
	for (int i = 0; i < pSurf->uarray.size(); i++)
	{
		u = pSurf->uarray[i];
		s = u * a / (1 - a + 2 * a * u - u);
		pSurf->uarraymodified.push_back(s);
	}

	for (int i = 0; i < pSurf->varray.size(); i++)
	{
		v = pSurf->varray[i];
		t = v * b / (1 - b + 2 * b * v - v);
		pSurf->varraymodified.push_back(t);
	}

	int num1 = pSurf->uarraymodified.size();
	Point3D m_pointarray[10];

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->uarraymodified[i];
		p[1] = pSurf->uarraymodified[i];
		p[2] = pSurf->uarraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	double* knot;
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pUInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->varraymodified[i];
		p[1] = pSurf->varraymodified[i];
		p[2] = pSurf->varraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pVInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < pSurf->pUInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pUInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}
	for (int i = 0; i < pSurf->pVInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pVInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}
}

void CFormView3::SetCartopModification()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	double a, b;
	//a = pSurf->m_Mobiustransformation_Alpha / 100.0;
	//b = pSurf->m_Mobiustransformation_Beta / 100.0;

	a = 0.30;
	b = 0.40;
	//a = 0.60;
	//b = 0.37;

	pSurf->uarraymodified.clear();
	pSurf->varraymodified.clear();

	double u, v, s, t;
	for (int i = 0; i < pSurf->uarray.size(); i++)
	{
		u = pSurf->uarray[i];
		s = u * a / (1 - a + 2 * a * u - u);
		pSurf->uarraymodified.push_back(s);
	}

	for (int i = 0; i < pSurf->varray.size(); i++)
	{
		v = pSurf->varray[i];
		t = v * b / (1 - b + 2 * b * v - v);
		pSurf->varraymodified.push_back(t);
	}

	int num1 = pSurf->uarraymodified.size();
	Point3D m_pointarray[10];

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->uarraymodified[i];
		p[1] = pSurf->uarraymodified[i];
		p[2] = pSurf->uarraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	double* knot;
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pUInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->varraymodified[i];
		p[1] = pSurf->varraymodified[i];
		p[2] = pSurf->varraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pVInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < pSurf->pUInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pUInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}
	for (int i = 0; i < pSurf->pVInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pVInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}
}
void CFormView3::SetFace2Modification()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	double a, b;
	//a = pSurf->m_Mobiustransformation_Alpha / 100.0;
	//b = pSurf->m_Mobiustransformation_Beta / 100.0;

	//a = 0.24;
	//b = 0.22;
	a = 0.60;
	b = 0.37;

	pSurf->uarraymodified.clear();
	pSurf->varraymodified.clear();

	double u, v, s, t;
	for (int i = 0; i < pSurf->uarray.size(); i++)
	{
		u = pSurf->uarray[i];
		s = u * a / (1 - a + 2 * a * u - u);
		pSurf->uarraymodified.push_back(s);
	}

	for (int i = 0; i < pSurf->varray.size(); i++)
	{
		v = pSurf->varray[i];
		t = v * b / (1 - b + 2 * b * v - v);
		pSurf->varraymodified.push_back(t);
	}

	int num1 = pSurf->uarraymodified.size();
	Point3D m_pointarray[10];

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->uarraymodified[i];
		p[1] = pSurf->uarraymodified[i];
		p[2] = pSurf->uarraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	double* knot;
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pUInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->varraymodified[i];
		p[1] = pSurf->varraymodified[i];
		p[2] = pSurf->varraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pVInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < pSurf->pUInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pUInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}
	for (int i = 0; i < pSurf->pVInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pVInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}

}
void CFormView3::SetFaceModification()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	double a, b;
	//a = pSurf->m_Mobiustransformation_Alpha / 100.0;
	//b = pSurf->m_Mobiustransformation_Beta / 100.0;

	//a = 0.24;
	//b = 0.22;
	a = 0.60;
	b = 0.37;

	pSurf->uarraymodified.clear();
	pSurf->varraymodified.clear();

	double u, v, s, t;
	for (int i = 0; i < pSurf->uarray.size(); i++)
	{
		u = pSurf->uarray[i];
		s = u * a / (1 - a + 2 * a * u - u);
		pSurf->uarraymodified.push_back(s);
	}

	for (int i = 0; i < pSurf->varray.size(); i++)
	{
		v = pSurf->varray[i];
		t = v * b / (1 - b + 2 * b * v - v);
		pSurf->varraymodified.push_back(t);
	}

	int num1 = pSurf->uarraymodified.size();
	Point3D m_pointarray[10];

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->uarraymodified[i];
		p[1] = pSurf->uarraymodified[i];
		p[2] = pSurf->uarraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	double* knot;
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pUInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pSurf->varraymodified[i];
		p[1] = pSurf->varraymodified[i];
		p[2] = pSurf->varraymodified[i];
		TRACE(" (%f %f) \n", p[0], p[1]);
		m_pointarray[i] = p;
	}
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pSurf->pVInterOptiModifyCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < pSurf->pUInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pUInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}
	for (int i = 0; i < pSurf->pVInterOptiModifyCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pSurf->pVInterOptiModifyCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}

}
void CFormView3::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateSliderPosition();

	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFormView3::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO scrollinfo;
	//垂直滚动条响应事件
	int np;
	GetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);

	switch (nSBCode)
	{
	case SB_BOTTOM:
		ScrollWindow(0, (scrollinfo.nPos - scrollinfo.nMax) * 10);
		scrollinfo.nPos = scrollinfo.nMax;
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		break;
	case SB_TOP:
		ScrollWindow(0, (scrollinfo.nPos - scrollinfo.nMin) * 10);
		scrollinfo.nPos = scrollinfo.nMin;
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		break;
	case SB_LINEUP:
		scrollinfo.nPos -= 1;
		if (scrollinfo.nPos) {
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		ScrollWindow(0, 15);
		break;
	case SB_LINEDOWN:
		scrollinfo.nPos += 1;
		if (scrollinfo.nPos > scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		ScrollWindow(0, -15);
		break;
	case SB_PAGEUP:
		scrollinfo.nPos -= 5;
		if (scrollinfo.nPos)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		ScrollWindow(0, 10 * 5);
		break;
	case SB_PAGEDOWN:
		scrollinfo.nPos += 5;
		if (scrollinfo.nPos > scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		ScrollWindow(0, -10 * 5);
		break;
	case SB_ENDSCROLL:
		break;
	case SB_THUMBPOSITION:
		break;
	case SB_THUMBTRACK:
		//	CRect rect;
		//GetWindowRect(&rect);
		//rect.Height();//高度
		//np=100*nPos/rect.Height();
		ScrollWindow(0, (scrollinfo.nPos - nPos));
		scrollinfo.nPos = nPos;
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		break;
	}
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CFormView3::OnBnClickedCheckareapreserving()
{
	// TODO: Add your control notification handler code here
	//Check the area preserving of the surface.
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num != 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();
	Vector3D uvec = pSurf->ComputUDerivative1(0, 0);
	Vector3D vvec = pSurf->ComputVDerivative1(0, 0);

	Vector3D zvec = uvec ^ vvec;
	double A = Length(zvec);
	BOOL IsAreaPreserving = TRUE;
	double maxlen = -10;
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
		{
			Vector3D uvec1 = pSurf->ComputUDerivative1(i * 0.01, j * 0.01);
			Vector3D vvec1 = pSurf->ComputVDerivative1(i * 0.01, j * 0.01);

			Vector3D zvec1 = uvec1 ^ vvec1;
			double len = Length(zvec1);
			if (fabs(A - len) > maxlen)
				maxlen = fabs(A - len);

			if (fabs(A - len) > 1e-8)
			{
				//AfxMessageBox("It is not a Area-Preserving parameterization!!!!");
				IsAreaPreserving = FALSE;
				//goto ExitLoop;
			}
		}
ExitLoop:
	char message[100];

	if (IsAreaPreserving)
	{
		sprintf(message, "It is a Area-Preserving parameterization. Maximal deviation: %f", maxlen);
		AfxMessageBox(message);
	}
	else
	{
		sprintf(message, "It is not a Area-Preserving parameterization. Maximal deviation: %f", maxlen);
		AfxMessageBox(message);
	}
	delete pSurf;
}

Vector3D CFormView3::GetSurfaceQVector(C_NurbsSurf* pNurbsSurf, int i, int j)
{
	Vector3D v1 = pNurbsSurf->GetControlPoint(i + 1, j) - ZeroP3D;
	Vector3D v2 = pNurbsSurf->GetControlPoint(i, j) - ZeroP3D;
	Vector3D v3 = v1 - v2;
	return v3;
}
Vector3D CFormView3::GetSurfaceRVector(C_NurbsSurf* pNurbsSurf, int i, int j)
{
	Vector3D v1 = pNurbsSurf->GetControlPoint(i, j + 1) - ZeroP3D;
	Vector3D v2 = pNurbsSurf->GetControlPoint(i, j) - ZeroP3D;
	Vector3D v3 = v1 - v2;
	return v3;
}
double CFormView3::Compute_Coefficients_Cij_NumericalMethod_FixedSampling_FaceSurface(CNurbsSuface* pNurbsSurf, int i, int j, int m, int n)
{
	//Convert CNurbsSurface* to C_NurbsSurf*
	C_NurbsSurf* pSurf = pNurbsSurf->Converte_C_NurbsSurface();

	double h1 = 1.0 / (m * 2);
	double g1 = 1.0 / (n * 2);
	double val = 0;
	int Ai, Aj;
	double term1, term2;
	for (int ii = 0; ii <= m * 2; ii++)
	{
		for (int jj = 0; jj <= n * 2; jj++)
		{
			if (ii == 0 || ii == m * 2)
				Ai = 1;
			else
				Ai = 2;

			if (jj == 0 || jj == n * 2)
				Aj = 1;
			else
				Aj = 2;

			double ui = ii * h1;
			double vj = jj * g1;

			Point3D pp1 = pNurbsSurf->pUInterOptiModifyCurve->ComputPosition(ui);
			Vector3D vv1 = pNurbsSurf->pUInterOptiModifyCurve->ComputDerivative1(ui);
			double uu = pp1[0];
			Point3D pp2 = pNurbsSurf->pVInterOptiModifyCurve->ComputPosition(vj);
			Vector3D vv2 = pNurbsSurf->pVInterOptiModifyCurve->ComputDerivative1(vj);
			double vv = pp2[0];

			Vector3D xu = pSurf->ComputUDerivative1(uu, vv) * vv1[0];
			Vector3D xv = pSurf->ComputVDerivative1(uu, vv) * vv2[0];

			Vector3D xn = xu ^ xv;
			double len = xn * xn;
			double term11 = 2 * pow(1 - ui, 2 - i) * pow(ui, i) / (Factorial(2 - i) * Factorial(i));
			double term21 = 2 * pow(1 - vj, 2 - j) * pow(vj, j) / (Factorial(2 - j) * Factorial(j));

			val += Ai * Aj * len * term11 * term21;
		}
	}
	val = val * h1 * g1 / 4;
	return val;
}
double CFormView3::Compute_Coefficients_Cij_NumericalMethod_FixedSampling(C_NurbsSurf* pNurbsSurf, int i, int j, int m, int n)
{
	double h1 = 1.0 / (m * 2);
	double g1 = 1.0 / (n * 2);
	double val = 0;
	int Ai, Aj;
	double term1, term2;
	for (int ii = 0; ii <= m * 2; ii++)
	{
		for (int jj = 0; jj <= n * 2; jj++)
		{
			if (ii == 0 || ii == m * 2)
				Ai = 1;
			else
				Ai = 2;

			if (jj == 0 || jj == n * 2)
				Aj = 1;
			else
				Aj = 2;

			double ui = ii * h1;
			double vj = jj * g1;

			Vector3D xu = pNurbsSurf->ComputUDerivative1(ui, vj);
			Vector3D xv = pNurbsSurf->ComputVDerivative1(ui, vj);

			Vector3D xn = xu ^ xv;
			double len = xn * xn;
			double term11 = 2 * pow(1 - ui, 2 - i) * pow(ui, i) / (Factorial(2 - i) * Factorial(i));
			double term21 = 2 * pow(1 - vj, 2 - j) * pow(vj, j) / (Factorial(2 - j) * Factorial(j));

			val += Ai * Aj * len * term11 * term21;
		}
	}
	val = val * h1 * g1 / 4;
	return val;
}
double CFormView3::Compute_Coefficients_Cij_NumericalMethod_FaceSurface(CNurbsSuface* pNurbsSurf, int i, int j)
{
	int m = 10;
	int n = 10;
	double tol = 1e-4;
	//double val1 = Compute_Coefficients_Cij_NumericalMethod_FixedSampling_FaceSurface(pNurbsSurf,i,j,m,n);
	double val2 = Compute_Coefficients_Cij_NumericalMethod_FixedSampling_FaceSurface(pNurbsSurf, i, j, m * 4, n * 4);
	/*while(fabs(val1 - val2) > tol)
	{
		m = m * 2;
		n = n * 2;
		val1 = val2;
		val2 = Compute_Coefficients_Cij_NumericalMethod_FixedSampling_FaceSurface(pNurbsSurf,i,j,m * 2,n * 2);
	}*/
	return val2;
}
double CFormView3::Compute_Coefficients_Cij_NumericalMethod(C_NurbsSurf* pNurbsSurf, int i, int j)
{
	int m = 100;
	int n = 100;
	double tol = 1e-4;
	double val1 = Compute_Coefficients_Cij_NumericalMethod_FixedSampling(pNurbsSurf, i, j, m, n);
	/*double val2 = Compute_Coefficients_Cij_NumericalMethod_FixedSampling(pNurbsSurf,i,j,m * 2,n * 2);
	while(fabs(val1 - val2) > tol)
	{
		m = m * 2;
		n = n * 2;
		val1 = val2;
		val2 = Compute_Coefficients_Cij_NumericalMethod_FixedSampling(pNurbsSurf,i,j,m * 2,n * 2);
	}
	return val2;*/
	return val1;
}

double CFormView3::Compute_Coefficients_Cij(C_NurbsSurf* pNurbsSurf, int i, int j)
{
	double scale = 1.0;
	if (i == 1)
		scale = scale * 2;
	if (j == 1)
		scale = scale * 2;

	int m = pNurbsSurf->GetDegreeU();
	int n = pNurbsSurf->GetDegreeV();

	double val = Factorial(m) * Factorial(m) * Factorial(m) * Factorial(m) * Factorial(n) * Factorial(n) * Factorial(n) * Factorial(n);
	val = val / (Factorial(4 * m + 1) * Factorial(4 * n + 1));

	double interterm1 = 0;
	for (int i1 = 0; i1 <= m - 1; i1++)
		for (int j1 = 0; j1 <= n; j1++)
			for (int k1 = 0; k1 <= m - 1; k1++)
				for (int l1 = 0; l1 <= n; l1++)
					for (int i2 = 0; i2 <= m; i2++)
						for (int j2 = 0; j2 <= n - 1; j2++)
							for (int k2 = 0; k2 <= m; k2++)
								for (int l2 = 0; l2 <= n - 1; l2++)
								{
									double numer1 = Factorial(4 * m - i1 - i2 - k1 - k2 - i);
									double numer2 = Factorial(i1 + i2 + k1 + k2 + i);
									double numer3 = Factorial(4 * n - j1 - j2 - l1 - l2 - j);
									double numer4 = Factorial(j1 + j2 + l1 + l2 + j);
									double numer = numer1 * numer2 * numer3 * numer4;
									double deno = Factorial(m - 1 - i1) * Factorial(i1) * Factorial(m - 1 - k1) * Factorial(k1) * Factorial(n - j1) * Factorial(j1) * Factorial(n - l1) * Factorial(l1);
									deno *= Factorial(n - 1 - j2) * Factorial(j2) * Factorial(n - 1 - l2) * Factorial(l2) * Factorial(m - i2) * Factorial(i2) * Factorial(m - k2) * Factorial(k2);

									double t1 = GetSurfaceQVector(pNurbsSurf, i1, j1) * GetSurfaceQVector(pNurbsSurf, k1, l1);
									double t2 = GetSurfaceRVector(pNurbsSurf, i2, j2) * GetSurfaceRVector(pNurbsSurf, k2, l2);

									interterm1 = interterm1 + numer * t1 * t2 / deno;
								}

	double interterm2 = 0;
	for (int i1 = 0; i1 <= m - 1; i1++)
		for (int j1 = 0; j1 <= n; j1++)
			for (int k1 = 0; k1 <= m; k1++)
				for (int l1 = 0; l1 <= n - 1; l1++)
					for (int i2 = 0; i2 <= m - 1; i2++)
						for (int j2 = 0; j2 <= n; j2++)
							for (int k2 = 0; k2 <= m; k2++)
								for (int l2 = 0; l2 <= n - 1; l2++)
								{
									double numer = Factorial(4 * m - i1 - i2 - k1 - k2 - i) * Factorial(i1 + i2 + k1 + k2 + i) * Factorial(4 * n - j1 - j2 - l1 - l2 - j) * Factorial(j1 + j2 + l1 + l2 + j);
									double deno = Factorial(m - 1 - i1) * Factorial(i1) * Factorial(m - k1) * Factorial(k1) * Factorial(n - j1) * Factorial(j1) * Factorial(n - 1 - l1) * Factorial(l1);
									deno *= Factorial(m - 1 - i2) * Factorial(i2) * Factorial(m - k2) * Factorial(k2) * Factorial(n - j2) * Factorial(j2) * Factorial(n - 1 - l2) * Factorial(l2);

									double t1 = GetSurfaceQVector(pNurbsSurf, i1, j1) * GetSurfaceRVector(pNurbsSurf, k1, l1);
									double t2 = GetSurfaceQVector(pNurbsSurf, i2, j2) * GetSurfaceRVector(pNurbsSurf, k2, l2);

									interterm2 = interterm2 + numer * t1 * t2 / deno;
								}
	double retval = (interterm1 - interterm2) * val * scale;
	return retval;
}
double CFormView3::Factorial(double n, double i)
{
	double val = Factorial((int)n) / (Factorial((int)(n - i)) * Factorial((int)i));
	return val;
}
double CFormView3::Factorial(int n)
{
	double rev = 1.0;
	for (int i = 1; i <= n; i++)
		rev *= i;
	return rev;
}

double CFormView3::ComputeFixPoints(double c00, double c01, double c02, double c10, double c12, double c20, double c21, double c22, double b, double& a)
{
	double term1_numer = c00 * (1 - b) * (1 - b) + c01 * (1 - b) * b + c02 * b * b;
	double term1_deno = c20 * (1 - b) * (1 - b) + c21 * (1 - b) * b + c22 * b * b;
	a = sqrt(term1_numer / term1_deno) / (1 + sqrt(term1_numer / term1_deno));

	double term2_numer = c00 * (1 - a) * (1 - a) + c10 * a * (1 - a) + c20 * a * a;
	double term2_deno = c02 * (1 - a) * (1 - a) + c12 * a * (1 - a) + c22 * a * a;
	double newb = sqrt(term2_numer / term2_deno) / (1 + sqrt(term2_numer / term2_deno));
	return newb;
}
void CFormView3::OutPutCompositeEnergyMesh(std::vector<std::vector<std::vector<double>>>& coefficientarray, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	int samplesm = 100;
	int samplesn = 100;
	outfile << "# " << (samplesm - 1) * (samplesn - 1) << "vertices, " << (samplesm - 2) * (samplesn - 2) << "faces" << endl;

	int M = coefficientarray.size();
	int N = coefficientarray[0].size();

	std::vector<double> alphaarray;
	std::vector<double> betaarray;
	alphaarray.reserve(M);
	betaarray.reserve(N);

	std::vector<int> flagarray;
	std::vector<int> indexarray;

	double maxenergy = -1e10;
	//for(j = 1; j <= N; j++)
	{
		//for(i = 1; i <= M; i++)
		{
			i = 0;
			j = 0;
			for (int i = 0; i < M; i++)
				alphaarray.push_back(0.5);
			for (int i = 0; i < N; i++)
				betaarray.push_back(0.5);
			for (int k = 1; k < samplesm; k++)
			{
				for (int l = 1; l < samplesn; l++)
				{
					alphaarray[i] = (double)k / samplesm;
					betaarray[j] = (double)l / samplesm;
					//
					double z = 0;
					for (int ii = 0; ii < M; ii++)
						for (int jj = 0; jj < N; jj++)
						{
							z += ((1 - alphaarray[i]) * (1 - alphaarray[i]) * (1 - betaarray[j]) * (1 - betaarray[j]) * coefficientarray[i][j][0]//c00
								+ (1 - alphaarray[i]) * (1 - alphaarray[i]) * (1 - betaarray[j]) * betaarray[j] * coefficientarray[i][j][1] //c01
								+ (1 - alphaarray[i]) * (1 - alphaarray[i]) * betaarray[j] * betaarray[j] * coefficientarray[i][j][2]//c02  
								+ (1 - alphaarray[i]) * alphaarray[i] * (1 - betaarray[j]) * (1 - betaarray[j]) * coefficientarray[i][j][3] //c10 
								+ (1 - alphaarray[i]) * alphaarray[i] * (1 - betaarray[j]) * betaarray[j] * coefficientarray[i][j][4] + //c11
								(1 - alphaarray[i]) * alphaarray[i] * betaarray[j] * betaarray[j] * coefficientarray[i][j][5]//c12 
								+ alphaarray[i] * alphaarray[i] * (1 - betaarray[j]) * (1 - betaarray[j]) * coefficientarray[i][j][6] + //c20
								alphaarray[i] * alphaarray[i] * (1 - betaarray[j]) * betaarray[j] * coefficientarray[i][j][7]//c21  
								+ alphaarray[i] * alphaarray[i] * betaarray[j] * betaarray[j] * coefficientarray[i][j][8]//c22
								) / (alphaarray[i] * (1 - alphaarray[i]) * betaarray[j] * (1 - betaarray[j]));
						}

					//double z = -6*b*b*b+8*b*b*b*b-6*a*a*a*a*a+2*b*b-6*b*b*b*b*b+2*a*a+18*b*b*b*b*b*a+18*a*a*a*a*a*b+6*b*b*b*b*b*b*a*a-18*a*a*b*b*b-24*a*b*b*b*b-18*a*a*a*a*a*b*b+24*a*a*a*a*b*b+24*a*a*b*b*b*b-6*a*b*b-6*a*a*b+18*a*b*b*b-6*b*b*b*b*b*b*a-18*a*a*a*b*b-6*a*a*a*a*a*a*b+18*a*a*a*b+2*a*a*a*a*a*a+8*a*a*a*a-24*a*a*a*a*b+12*a*a*b*b-18*b*b*b*b*b*a*a-6*a*a*a+2*b*b*b*b*b*b+6*a*a*a*a*a*a*b*b;

					//if(z > 25)
					//{
					//	z = 1.0;
					//}else
					//	z = z / 25;
					if (z > 2000)
						flagarray.push_back(0);
					else
					{
						flagarray.push_back(1);
						outfile << "v " << alphaarray[i] * 10 << " " << betaarray[j] * 10 << " " << z / 10.0 << endl;
					}

					if (z > maxenergy)
						maxenergy = z;
					//if(z > 0.05e8)
					//	z = 0.05e8;

					//if(z > 1)
					//	z = 10000000;

				}
			}

		}
	}

	int currentindex = 0;
	for (int i = 0; i < flagarray.size(); i++)
	{
		if (flagarray[i])
			currentindex++;
		indexarray.push_back(currentindex);
	}
	for (j = 0; j < samplesn - 2; j++)
	{
		for (i = 0; i < samplesm - 2; i++)
		{
			//
			if (flagarray[(samplesn - 1) * j + i + 1] && flagarray[(samplesn - 1) * j + i + 2] && flagarray[(samplesn - 1) * j + i + 2 + samplesn - 1]
				&& flagarray[(samplesn - 1) * j + i + 1 + samplesn - 1])
			{
				outfile << "f " << indexarray[(samplesn - 1) * j + i + 1] << " " << indexarray[(samplesn - 1) * j + i + 2] << " " << indexarray[(samplesn - 1) * j + i + 2 + samplesn - 1] << endl;
				outfile << "f " << indexarray[(samplesn - 1) * j + i + 1] << " " << indexarray[(samplesn - 1) * j + i + 2 + samplesn - 1] << " " << indexarray[(samplesn - 1) * j + i + 1 + samplesn - 1] << endl;
			}
		}
	}

	outfile.close();
}
void CFormView3::OutPutEnergyMesh(double c00, double c01, double c02,
	double c10, double c11, double c12,
	double c20, double c21, double c22,
	char* filename)
{
	std::ofstream outfile(filename);
	char c1 = 13;
	char c2 = 10;
	CString s1;
	int i, j;
	int samplesm = 100;
	int samplesn = 100;
	outfile << "# " << (samplesm - 1) * (samplesn - 1) << "vertices, " << (samplesm - 2) * (samplesn - 2) << "faces" << endl;

	for (j = 1; j <= samplesn - 1; j++)
	{
		for (i = 1; i <= samplesm - 1; i++)
		{	//
			double a = (double)i / samplesm;
			double b = (double)j / samplesn;

			/*double z = SQR(1 - a) * SQR(1 - b) * c00;
			z += SQR(1 - a) * b * (1 - b) * c01;
			z += SQR(1 - a) * SQR(b) * c02;
			z += a * (1 - a) * SQR(1 - b) * c10;
			z += a * (1 - a) * b * (1 - b) * c11;
			z += a * (1 - a) * SQR(b) * c12;
			z += SQR(a) * SQR (1 - b) * c20;
			z += SQR(a) * b * (1 - b) * c21;
			z += SQR(a) * SQR(b) * c22;

			z = z / (a * (1 - a) * b * (1 - b));*/
			//double c00 = 0;
			//double c01 = 0;
			//double c02 = 0;
			//double c10 = 0;
			//double c11 = 0;
			//double c12 = 0;
			//double c20 = 0;
			//double c21 = 0;
			//double c22 = 1;
			double z = ((1 - a) * (1 - a) * (1 - b) * (1 - b) * c00
				+ (1 - a) * (1 - a) * (1 - b) * b * c01
				+ (1 - a) * (1 - a) * b * b * c02
				+ (1 - a) * a * (1 - b) * (1 - b) * c10
				+ (1 - a) * a * (1 - b) * b * c11 +
				(1 - a) * a * b * b * c12
				+ a * a * (1 - b) * (1 - b) * c20 +
				a * a * (1 - b) * b * c21
				+ a * a * b * b * c22
				) / (a * (1 - a) * b * (1 - b));

			//double z = -6*b*b*b+8*b*b*b*b-6*a*a*a*a*a+2*b*b-6*b*b*b*b*b+2*a*a+18*b*b*b*b*b*a+18*a*a*a*a*a*b+6*b*b*b*b*b*b*a*a-18*a*a*b*b*b-24*a*b*b*b*b-18*a*a*a*a*a*b*b+24*a*a*a*a*b*b+24*a*a*b*b*b*b-6*a*b*b-6*a*a*b+18*a*b*b*b-6*b*b*b*b*b*b*a-18*a*a*a*b*b-6*a*a*a*a*a*a*b+18*a*a*a*b+2*a*a*a*a*a*a+8*a*a*a*a-24*a*a*a*a*b+12*a*a*b*b-18*b*b*b*b*b*a*a-6*a*a*a+2*b*b*b*b*b*b+6*a*a*a*a*a*a*b*b;

			if (z > 10)
			{
				z = 10;
			}
			//if(z > 0.05e8)
			//	z = 0.05e8;

			//if(z > 1)
			//	z = 10000000;
			double m = 1.0 / a - 1;
			double n = 1.0 / b - 1;
			outfile << "v " << m * 10 << " " << n * 10 << " " << z / 1 << endl;
		}
	}

	for (j = 0; j < samplesn - 2; j++)
	{
		for (i = 0; i < samplesm - 2; i++)
		{
			//
			outfile << "f " << (samplesn - 1) * j + i + 1 << " " << (samplesn - 1) * j + i + 2 << " " << (samplesn - 1) * j + i + 2 + samplesn - 1 << endl;
			outfile << "f " << (samplesn - 1) * j + i + 1 << " " << (samplesn - 1) * j + i + 2 + samplesn - 1 << " " << (samplesn - 1) * j + i + 1 + samplesn - 1 << endl;
		}
	}

	outfile.close();
}

double CFormView3::Compute_Area_Sampling(C_NurbsSurf* pNurbsSurf, double u, double v, int m, int n)
{
	//Compute the Area
	double h1 = u / (m * 2);
	double g1 = v / (n * 2);
	double val = 0;
	int Ai, Aj;
	double term1, term2;
	for (int ii = 0; ii <= m * 2; ii++)
	{
		for (int jj = 0; jj <= n * 2; jj++)
		{
			if (ii == 0 || ii == m * 2)
				Ai = 1;
			else
				Ai = 2;

			if (jj == 0 || jj == n * 2)
				Aj = 1;
			else
				Aj = 2;

			double ui = ii * h1;
			double vj = jj * g1;

			Vector3D xu = pNurbsSurf->ComputUDerivative1(ui, vj);
			Vector3D xv = pNurbsSurf->ComputVDerivative1(ui, vj);

			Vector3D xn = xu ^ xv;
			double len = Length(xn);

			val += Ai * Aj * len;
		}
	}
	val = val * h1 * g1 / 4;
	return val;
}
double CFormView3::Compute_Area(C_NurbsSurf* pNurbsSurf, double startu, double startv, double endu, double endv)
{
	//compute the area from (startu,startv)->(endu,endv)
	return Compute_Area(pNurbsSurf, endu, endv) - Compute_Area(pNurbsSurf, startu, endv) - Compute_Area(pNurbsSurf, endu, startv) + Compute_Area(pNurbsSurf, startu, startv);
}
double CFormView3::Compute_Area(C_NurbsSurf* pNurbsSurf, double u, double v)
{
	int m = 100;
	int n = 100;
	double tol = 1e-4;
	double val1 = Compute_Area_Sampling(pNurbsSurf, u, v, m, n);
	/*double val2 = Compute_Area_Sampling(pNurbsSurf,u,v,m * 2,n * 2);
	int MAX_ITERATION = 5;
	int iterations = 0;
	while(fabs(val1 - val2) > tol)
	{
		m = m * 2;
		n = n * 2;
		val1 = val2;
		val2 = Compute_Area_Sampling(pNurbsSurf,u,v,m * 2,n * 2);
		iterations++;
		if(iterations > MAX_ITERATION)
			break;
	}
	return val2;*/
	return val1;
}
double CFormView3::Compute_V_Value_GivenArea(C_NurbsSurf* pNurbsSurf, double area)
{
	//compute the V value for given area using the divide and conquer method
	double lowerv = 0;
	double upperv = 1.0;
	double lowerarea = 0;
	double upperarea = Compute_Area(pNurbsSurf, 1, 1);
	double TOL = 1e-5;
	double v;
	while (1)
	{
		v = (lowerv + upperv) / 2;
		double currentarea = Compute_Area(pNurbsSurf, 1, v);
		if (currentarea < area)
		{
			lowerv = v;
			lowerarea = currentarea;
		}
		else
		{
			upperv = v;
			upperarea = currentarea;
		}

		if (fabs(lowerv - upperv) < TOL)
			return v;
	}
}
double CFormView3::Compute_U_Value_GivenArea(C_NurbsSurf* pNurbsSurf, double area)
{
	//compute the U value for given area using the divide and conquer method
	double loweru = 0;
	double upperu = 1.0;
	double lowerarea = 0;
	double upperarea = Compute_Area(pNurbsSurf, 1, 1);
	double TOL = 1e-5;
	double u;
	while (1)
	{
		u = (loweru + upperu) / 2;
		double currentarea = Compute_Area(pNurbsSurf, u, 1);
		if (currentarea < area)
		{
			loweru = u;
			lowerarea = currentarea;
		}
		else
		{
			upperu = u;
			upperarea = currentarea;
		}

		if (fabs(loweru - upperu) < TOL)
			return u;
	}
}
void CFormView3::Compute_Optimal_AreaPreserving_IsoParameterCurves_NumericalMethod(C_NurbsSurf* pNurbsSurf)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num < 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	//first check whether we have computed the values.
	CString arefilename = pNSurf->filename;
	arefilename.Replace(".sur", ".are");
	CFileFind find;
	if (!find.FindFile(arefilename))
	{
		C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();

		//first Compute the total Area of the surface
		double area = Compute_Area(pSurf, 1, 1);

		for (int i = 0; i <= pDoc->m_Disc1; i++)
		{
			double v = Compute_V_Value_GivenArea(pSurf, area * i / pDoc->m_Disc1);
			pNSurf->varray.push_back(v);
			TRACE(" %f ", v);
		}

		TRACE("\n");
		//
		for (int i = 0; i <= pDoc->m_Disc1; i++)
		{
			double u = Compute_U_Value_GivenArea(pSurf, area * i / pDoc->m_Disc1);
			pNSurf->uarray.push_back(u);
			TRACE(" %f ", u);
		}

		std::ofstream file(arefilename);

		file << pNSurf->uarray.size() << "\n";
		for (int i = 0; i < pNSurf->uarray.size(); i++)
			file << pNSurf->uarray[i] << " ";
		file << "\n";
		file << pNSurf->varray.size() << "\n";
		for (int i = 0; i < pNSurf->varray.size(); i++)
			file << pNSurf->varray[i] << " ";
		file.close();
	}
	else
	{
		//read the values.
		std::ifstream file(arefilename);
		int m, n;
		double val;
		pNSurf->uarray.clear();
		pNSurf->varray.clear();
		file >> m;
		for (int i = 0; i < m; i++)
		{
			file >> val;
			pNSurf->uarray.push_back(val);
		}

		file >> n;
		for (int i = 0; i < n; i++)
		{
			file >> val;
			pNSurf->varray.push_back(val);
		}
		file.close();
	}
	find.Close();

	//interpolation
	int num1 = pNSurf->uarray.size();
	Point3D m_pointarray[10];

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pNSurf->uarray[i];
		p[1] = pNSurf->uarray[i];
		p[2] = pNSurf->uarray[i];
		TRACE(" (%f %f %f) \n", p[0], p[1], p[2]);
		m_pointarray[i] = p;
	}
	double* knot;
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	for (int i = 0; i < num1 + 6; i++)
		TRACE(" %f ", knot[i]);
	TRACE("\n");

	pNSurf->pUInterOptiCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);
	for (int i = 0; i < num1 + 6; i++)
	{
		Point3D pp = pNSurf->pUInterOptiCurve->ComputPosition(knot[i]);
		TRACE("(%f %f %f) \n", pp[0], pp[1], pp[2]);
	}

	for (int i = 0; i < num1; i++)
	{
		Point3D p;
		p[0] = pNSurf->varray[i];
		p[1] = pNSurf->varray[i];
		p[2] = pNSurf->varray[i];
		TRACE(" (%f %f %f) \n", p[0], p[1], p[2]);
		m_pointarray[i] = p;
	}
	GetUniformInterpolatKnot(num1, m_pointarray, knot);
	pNSurf->pVInterOptiCurve = new C_NurbsCurve3D(num1, 4, m_pointarray, knot);

	for (int i = 0; i < pNSurf->pUInterOptiCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pNSurf->pUInterOptiCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}
	for (int i = 0; i < pNSurf->pVInterOptiCurve->GetCtrlNumber(); i++)
	{
		Point3D pp = pNSurf->pVInterOptiCurve->GetControlPoint(i);
		TRACE(" (%f %f %f) \n", pp.x, pp.y, pp.z);
	}

	//pNSurf->m_showAreaPreservingIsoParameterCurves = TRUE;
	//pDoc->UpdateAllViews(NULL);
}
void CFormView3::Compute_Optimal_Mobiustransformation_NumericalMethod_FaceSurface(CNurbsSuface* pNurbsSurf)
{
	C_NurbsSurf* pSurf = pNurbsSurf->Converte_C_NurbsSurface();
	double c00 = Compute_Coefficients_Cij_NumericalMethod_FaceSurface(pNurbsSurf, 0, 0);
	double c01 = Compute_Coefficients_Cij_NumericalMethod_FaceSurface(pNurbsSurf, 0, 1);
	double c02 = Compute_Coefficients_Cij_NumericalMethod_FaceSurface(pNurbsSurf, 0, 2);
	double c10 = Compute_Coefficients_Cij_NumericalMethod_FaceSurface(pNurbsSurf, 1, 0);
	double c11 = Compute_Coefficients_Cij_NumericalMethod_FaceSurface(pNurbsSurf, 1, 1);
	double c12 = Compute_Coefficients_Cij_NumericalMethod_FaceSurface(pNurbsSurf, 1, 2);
	double c20 = Compute_Coefficients_Cij_NumericalMethod_FaceSurface(pNurbsSurf, 2, 0);
	double c21 = Compute_Coefficients_Cij_NumericalMethod_FaceSurface(pNurbsSurf, 2, 1);
	double c22 = Compute_Coefficients_Cij_NumericalMethod_FaceSurface(pNurbsSurf, 2, 2);

	OutPutEnergyMesh(c00, c01, c02, c10, c11, c12, c20, c21, c22, "d:\\energy.obj");

	TRACE("\n c00: %f", c00);
	TRACE(" c01: %f", c01);
	TRACE(" c02: %f", c02);
	TRACE(" c10: %f", c10);
	TRACE(" c11: %f", c11);
	TRACE(" c12: %f", c12);
	TRACE(" c20: %f", c20);
	TRACE(" c21: %f", c21);
	TRACE(" c22: %f\n", c22);

	//the algorithm to compute the optimal coefficient.
	double b[100];
	double a[100];
	b[0] = 0.5;
	a[0] = 0.5;
	int index;
	for (int i = 1; i < 100; i++)
	{
		b[i] = ComputeFixPoints(c00, c01, c02, c10, c12, c20, c21, c22, b[i - 1], a[i]);
		if (fabs(b[i] - b[i - 1]) < 1e-8 && fabs(a[i] - a[i - 1]) < 1e-8)
		{
			char message[100];
			index = i;
			//sprintf(message,"%d Interations. a: %f, b: %f", i,a[i],b[i]);
			//AfxMessageBox(message);
			break;
		}
	}

	double a1[100];
	double b1[100];
	for (int ii = 1; ii < 100; ii++)
		for (int jj = 1; jj < 100; jj++)
		{
			a1[0] = ii * 0.01;
			b1[0] = jj * 0.01;

			for (int i = 1; i < 100; i++)
			{
				b1[i] = ComputeFixPoints(c00, c01, c02, c10, c12, c20, c21, c22, b1[i - 1], a1[i]);
				if (fabs(b1[i] - b1[i - 1]) < 1e-8 && fabs(a1[i] - a1[i - 1]) < 1e-8)
				{
					char message[100];
					if (fabs(b1[i] - b[index]) > 1e-8 || fabs(a1[i] - a[index]) > 1e-8)
					{
						AfxMessageBox("Multi stationary  points.");
						goto ExitFlag;
					}

				}
			}

		}
ExitFlag:
	char message[100];
	sprintf(message, "%d Interations. a: %f, b: %f", index, a[index], b[index]);
	AfxMessageBox(message);
	C_NurbsSurf* newSurf = pSurf->GetMobiusTransformedSurface(a[index], b[index]);
	CNurbsSuface* pNewS = newSurf->GetCNurbsSurface();

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	pNewS->m_Transform.Copy(pNurbs->m_Transform);
	//pDoc->m_SceneGraph.RemoveAt(0);
	pDoc->m_SceneGraph.Add(pNewS);
	pDoc->UpdateTreeControl();
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
void CFormView3::Compute_Optimal_Mobiustransformation_NumericalMethod(C_NurbsSurf* pNurbsSurf)
{
	double c00 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 0, 0);
	double c01 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 0, 1);
	double c02 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 0, 2);
	double c10 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 1, 0);
	double c11 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 1, 1);
	double c12 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 1, 2);
	double c20 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 2, 0);
	double c21 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 2, 1);
	double c22 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 2, 2);

	OutPutEnergyMesh(c00, c01, c02, c10, c11, c12, c20, c21, c22, "d:\\energy.obj");

	TRACE("\n c00: %f", c00);
	TRACE(" c01: %f", c01);
	TRACE(" c02: %f", c02);
	TRACE(" c10: %f", c10);
	TRACE(" c11: %f", c11);
	TRACE(" c12: %f", c12);
	TRACE(" c20: %f", c20);
	TRACE(" c21: %f", c21);
	TRACE(" c22: %f\n", c22);

	//the algorithm to compute the optimal coefficient.
	double b[100];
	double a[100];
	b[0] = 0.5;
	a[0] = 0.5;
	int index;
	for (int i = 1; i < 100; i++)
	{
		b[i] = ComputeFixPoints(c00, c01, c02, c10, c12, c20, c21, c22, b[i - 1], a[i]);
		if (fabs(b[i] - b[i - 1]) < 1e-8 && fabs(a[i] - a[i - 1]) < 1e-8)
		{
			char message[100];
			index = i;
			//sprintf(message,"%d Interations. a: %f, b: %f", i,a[i],b[i]);
			//AfxMessageBox(message);
			break;
		}
	}

	double a1[100];
	double b1[100];
	for (int ii = 1; ii < 100; ii++)
		for (int jj = 1; jj < 100; jj++)
		{
			a1[0] = ii * 0.01;
			b1[0] = jj * 0.01;

			for (int i = 1; i < 100; i++)
			{
				b1[i] = ComputeFixPoints(c00, c01, c02, c10, c12, c20, c21, c22, b1[i - 1], a1[i]);
				if (fabs(b1[i] - b1[i - 1]) < 1e-8 && fabs(a1[i] - a1[i - 1]) < 1e-8)
				{
					char message[100];
					if (fabs(b1[i] - b[index]) > 1e-8 || fabs(a1[i] - a[index]) > 1e-8)
					{
						AfxMessageBox("Multi stationary  points.");
						goto ExitFlag;
					}

				}
			}

		}
ExitFlag:
	char message[100];
	sprintf(message, "%d Interations. a: %f, b: %f", index, a[index], b[index]);
	AfxMessageBox(message);
	C_NurbsSurf* newSurf = pNurbsSurf->GetMobiusTransformedSurface(a[index], b[index]);
	CNurbsSuface* pNewS = newSurf->GetCNurbsSurface();

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	pNewS->m_Transform.Copy(pNurbs->m_Transform);
	//pDoc->m_SceneGraph.RemoveAt(0);
	pDoc->m_SceneGraph.Add(pNewS);
	pDoc->UpdateTreeControl();
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
double CFormView3::Compute_F(double c00, double c01, double c02,
	double c10, double c11, double c12,
	double c20, double c21, double c22, double m, double n)
{
	double a = 1.0 / (1 + m);
	double b = 1.0 / (1 + n);

	double F = 0;
	F = c00 * (1 - a) * (1 - b) / (a * b) +
		c01 * (1 - a) / a +
		c02 * (1 - a) * b / (a * (1 - b)) +
		c10 * (1 - b) / b +
		c11 +
		c12 * b / (1 - b) +
		c20 * a * (1 - b) / ((1 - a) * b) +
		c21 * a / (1 - a) +
		c22 * a * b / ((1 - a) * (1 - b));
	return F;

}
double CFormView3::Compute_F_1(double c00, double c01, double c02,
	double c10, double c11, double c12,
	double c20, double c21, double c22, double a, double b)
{
	double F = 0;
	F = c00 * (1 - a) * (1 - b) / (a * b) +
		c01 * (1 - a) / a +
		c02 * (1 - a) * b / (a * (1 - b)) +
		c10 * (1 - b) / b +
		c11 +
		c12 * b / (1 - b) +
		c20 * a * (1 - b) / ((1 - a) * b) +
		c21 * a / (1 - a) +
		c22 * a * b / ((1 - a) * (1 - b));
	return F;

}
void CFormView3::Compute_Optimal_Mobiustransformation(C_NurbsSurf* pNurbsSurf)
{
	/*double c00 = Compute_Coefficients_Cij(pNurbsSurf,0,0);
	double c01 = Compute_Coefficients_Cij(pNurbsSurf,0,1);
	double c02 = Compute_Coefficients_Cij(pNurbsSurf,0,2);
	double c10 = Compute_Coefficients_Cij(pNurbsSurf,1,0);
	double c11 = Compute_Coefficients_Cij(pNurbsSurf,1,1);
	double c12 = Compute_Coefficients_Cij(pNurbsSurf,1,2);
	double c20 = Compute_Coefficients_Cij(pNurbsSurf,2,0);
	double c21 = Compute_Coefficients_Cij(pNurbsSurf,2,1);
	double c22 = Compute_Coefficients_Cij(pNurbsSurf,2,2);*/

	double c00 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 0, 0);
	double c01 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 0, 1);
	double c02 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 0, 2);
	double c10 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 1, 0);
	double c11 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 1, 1);
	double c12 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 1, 2);
	double c20 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 2, 0);
	double c21 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 2, 1);
	double c22 = Compute_Coefficients_Cij_NumericalMethod(pNurbsSurf, 2, 2);

	double area = c00 + c01 + c02 + c10 + c11 + c12 + c20 + c21 + c22;
	OutPutEnergyMesh(c00, c01, c02, c10, c11, c12, c20, c21, c22, "d:\\energy.obj");

	//double tes1 = 
	TRACE("\n c00: %f", c00 / area);
	TRACE(" c01: %f", c01 / area);
	TRACE(" c02: %f", c02 / area);
	TRACE(" c10: %f", c10 / area);
	TRACE(" c11: %f", c11 / area);
	TRACE(" c12: %f", c12 / area);
	TRACE(" c20: %f", c20 / area);
	TRACE(" c21: %f", c21 / area);
	TRACE(" c22: %f\n", c22 / area);

	//double test = c00 * c22 - c02 * c20;

	/*double m1 = 1,m2 = 2,n1 = 1,n2 = 5;
	double D00 = c00 * ((m1 - m2) * (n1 - n2))/4;
	double D02 = c02 * (1.0/2) * (n2 - n1) * (-m2 * n1 + m1*n2) / (n1 * n2 * (n1 + n2));
	double D12 = c12 * (1.0/2) * (n1 - n2) * (n1 - n2) / (n1 * n2 * (n1 + n2));
	double D20 = c20 * (1.0/2) * (m1 - m2) * (-m2 * n1 + m1 * n2) / (m1 * m2 *(m1 + m2));
	double D21 = c21 * (1.0/2) * (m1 - m2) * (m1 - m2) / (m1 * m2 * (m1 + m2));
	double D22 = c22 * (1.0/(2*m1*n1)+1/(2*m2*n2)-4/((m1+m2)*(n1+n2)));
	double D = D00 + D02 + D12 + D20 + D21 +D22;

	double D_10 = Compute_F(c00,c01,c02,c10,c11,c12,c20,c21,c22,m1,n1);
	double D_11 = Compute_F(c00,c01,c02,c10,c11,c12,c20,c21,c22,m2,n2);
	double a1_ = 1.0 / (1 + m1);
	double b1_ = 1.0 / (1 + n1);
	double a2_ = 1.0 / (1 + m2);
	double b2_ = 1.0 / (1 + n2);
	double middlea = (a1_ + a2_) / 2;
	double middleb = (b1_ + b2_) / 2;
	double middlem = 1.0 / middlea - 1;
	double middlen = 1.0 / middleb - 1;
	double D_12 =  Compute_F(c00,c01,c02,c10,c11,c12,c20,c21,c22,(m1+m2)/2,(n1+n2)/2);
	double D_123 =  Compute_F(c00,c01,c02,c10,c11,c12,c20,c21,c22,middlem,middlen);
	double D1 = (D_10 + D_11)/2 - D_12;
	double D1_ = (D_10 + D_11)/2 - D_123;*/
	/*double a1_ = 0.01;
	double b1_ = 0.01;
	double a2_ = 0.36;
	double b2_ = 0.04;
	for(int ii = 1; ii < 100; ii++)
		for(int jj = 1; jj < 100; jj++)
			for(int kk = 1; kk < 100; kk++)
				for(int ll = 1; ll < 100; ll++)
				{
					double a1,b1,a2,b2;
					a1 = ii * 0.01;
					b1 = jj * 0.01;
					a2 = kk * 0.01;
					b2 = ll * 0.01;
					double f11 = Compute_F_1(c00,c01,c02,c10,c11,c12,c20,c21,c22,a1,b1);
					double f12 = Compute_F_1(c00,c01,c02,c10,c11,c12,c20,c21,c22,a2,b2);
					double f13 = Compute_F_1(c00,c01,c02,c10,c11,c12,c20,c21,c22,(a1 + a2)/2,(b1 + b2)/2);


					double D1 = (f11 + f12)/2 - f13;
					if(D1 < 0)
					{
						AfxMessageBox("negative");
						return;
					}
				}
				AfxMessageBox("positive");
				return;*/
				/*double min = 1e10;
				double mina1,minb1,mina2,minb2;
				for(int ii = 1; ii < 100; ii++)
					for(int jj = 1; jj < 100; jj++)
						for(int kk = 1; kk < 100; kk++)
							for(int ll = 1; ll < 100; ll++)
							{
								double a1,b1,a2,b2;
								a1 = ii * 0.01;
								b1 = jj * 0.01;
								a2 = kk * 0.01;
								b2 = ll * 0.01;
								double eqn11 = (a1)*(1 - b1)/((1 - a1)*(b1));
								double eqn12 = (a2)*(1 - b2)/((1 - a2)*(b2));

								double c1 = (a1+a2)*(1.0/2);
								double d1 = (b1+b2)*(1.0/2);

								double eqn13 = (c1)*(1 - d1)/((1 - c1)*(d1));

								double eqn14 = (eqn11+eqn12)*(1.0/2)-eqn13;
								if(eqn14 < min)
								{
									min = eqn14;
									mina1 = a1;
									minb1 = b1;
									mina2 = a2;
									minb2 = b2;
								}
							}
				char message1[100];
				sprintf(message1,"a1%f,b1%f,a2%f,b2%f",mina1,minb1,mina2,minb2);
				AfxMessageBox(message1);
				return;*/
				//the algorithm to compute the optimal coefficient.
	double b[100];
	double a[100];
	b[0] = 0.5;
	a[0] = 0.5;
	int index;
	for (int i = 1; i < 100; i++)
	{
		b[i] = ComputeFixPoints(c00, c01, c02, c10, c12, c20, c21, c22, b[i - 1], a[i]);
		if (fabs(b[i] - b[i - 1]) < 1e-8 && fabs(a[i] - a[i - 1]) < 1e-8)
		{
			char message[100];
			index = i;
			//sprintf(message,"%d Interations. a: %f, b: %f", i,a[i],b[i]);
			//AfxMessageBox(message);
			break;
		}
	}

	double a1[100];
	double b1[100];
	for (int ii = 1; ii < 100; ii++)
		for (int jj = 1; jj < 100; jj++)
		{
			a1[0] = ii * 0.01;
			b1[0] = jj * 0.01;

			for (int i = 1; i < 100; i++)
			{
				b1[i] = ComputeFixPoints(c00, c01, c02, c10, c12, c20, c21, c22, b1[i - 1], a1[i]);
				if (fabs(b1[i] - b1[i - 1]) < 1e-8 && fabs(a1[i] - a1[i - 1]) < 1e-8)
				{
					char message[100];
					if (fabs(b1[i] - b[index]) > 1e-8 || fabs(a1[i] - a[index]) > 1e-8)
					{
						AfxMessageBox("Multi stationary  points.");
						goto ExitFlag;
					}

				}
			}

		}
ExitFlag:
	char message[100];
	sprintf(message, "%d Interations. a: %f, b: %f", index, a[index], b[index]);
	TRACE("%s", message);
	//AfxMessageBox(message);
	C_NurbsSurf* newSurf = pNurbsSurf->GetMobiusTransformedSurface(a[index], b[index]);
	CNurbsSuface* pNewS = newSurf->GetCNurbsSurface();

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	pNewS->m_Transform.Copy(pNurbs->m_Transform);
	//pDoc->m_SceneGraph.RemoveAt(0);
	pNewS->filename = "Optimized surface";
	pDoc->m_SceneGraph.Add(pNewS);
	pDoc->UpdateTreeControl();
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
void CFormView3::OnBnClickedAreapreserve()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num != 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();

	//Step 1: first compute the cij coefficients
	//Compute_Coefficients_Cij(pSurf,0,0);

	//Step 2: Compute the optimal mobius transformation coefficents a & b
	Compute_Optimal_Mobiustransformation(pSurf);
}

void CFormView3::OnNMThemeChangedSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	UpdateSliderPosition();
	*pResult = 0;
}
extern CString m_FileTextureName;
extern int m_ShowTexture;

void CFormView3::OnBnClickedShowconvexfunction()
{
	// TODO: Add your control notification handler code here
	ConvexFunction dialog;
	dialog.DoModal();
}

void CFormView3::OnNMThemeChangedMobiusAlpha(NMHDR* pNMHDR, LRESULT* pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CFormView3::OnNMThemeChangedMobiusBeta(NMHDR* pNMHDR, LRESULT* pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
extern int m_ShowCoordinateAxis;
void CFormView3::OnBnClickedShowcoordinateaxis()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	m_ShowCoordinateAxis = !m_ShowCoordinateAxis;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}

void CFormView3::UpdateTreeControl()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	//ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	CTreeCtrl* pTreeControl = (CTreeCtrl*)objTab.GetDlgItem(IDC_Objects);
	pTreeControl->DeleteAllItems();
	m_ArraySurfaceItems.clear();
	char name[200];
	for (int i = 0; i < num; i++)
	{
		int type = pDoc->m_SceneGraph.GetAt(i)->GetType();
		if (type == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(i);
			CString path = pNurbsSurface->filename;
			path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
			sprintf(name, "%s", path);
			HTREEITEM item = pTreeControl->InsertItem(name, NULL, NULL);
			m_ArraySurfaceItems.push_back(item);
			pTreeControl->SetCheck(item, pNurbsSurface->m_Show);
		}
		else
			if (type == TYPE_MESH3D)
			{
				CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(i);
				CString path = pMesh3D->filename;
				path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
				sprintf(name, "%s", path);
				CString cname(name);
				pMesh3D->SetName(cname);
				HTREEITEM item = pTreeControl->InsertItem(name, NULL, NULL);
				m_ArraySurfaceItems.push_back(item);
				pTreeControl->SetCheck(item, pMesh3D->m_Show);
			}
			else
				if (type == TYPE_TSPLINE)
				{
					Tspline* pNurbsSurface = (Tspline*)pDoc->m_SceneGraph.GetAt(i);
					CString path = pNurbsSurface->filename;
					path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
					sprintf(name, "%s", path);
					HTREEITEM item = pTreeControl->InsertItem(name, NULL, NULL);
					m_ArraySurfaceItems.push_back(item);
					pTreeControl->SetCheck(item, pNurbsSurface->m_Show);
				}
				else
					if (type == TYPE_DISLINE2D)
					{
						CDisline2D* pDisline = (CDisline2D*)pDoc->m_SceneGraph.GetAt(i);
						pDisline->filename = "Disline2D";
						sprintf(name, "Disline2D");
						HTREEITEM item = pTreeControl->InsertItem(name, NULL, NULL);
						m_ArraySurfaceItems.push_back(item);
						pTreeControl->SetCheck(item, pDisline->m_Show);
					}
					else
						if (type == TYPE_NURBSCURVE3D)
						{
							CNurbsCurve* pCurve = (CNurbsCurve*)pDoc->m_SceneGraph.GetAt(i);
							pCurve->filename = "NurbsCurve3D";
							sprintf(name, "NurbsCurve3D");
							HTREEITEM item = pTreeControl->InsertItem(name, NULL, NULL);
							m_ArraySurfaceItems.push_back(item);
							pTreeControl->SetCheck(item, pCurve->m_Show);
						}
						else
							if (type == TYPE_MESH3D_OPENMESH)
							{
								CMesh3d_OpenMesh* pMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(i);
								//pMesh->filename = "Mesh3D";
								//sprintf(name,"Mesh3D");
								HTREEITEM item = pTreeControl->InsertItem(pMesh->filename, NULL, NULL);
								m_ArraySurfaceItems.push_back(item);
								pTreeControl->SetCheck(item, pMesh->m_Show);
							}
							else
								if (type == TYPE_POLYGON)
								{
									Shape_Polygon* pPolygon = (Shape_Polygon*)pDoc->m_SceneGraph.GetAt(i);
									HTREEITEM item = pTreeControl->InsertItem(pPolygon->filename, NULL, NULL);
									m_ArraySurfaceItems.push_back(item);
									pTreeControl->SetCheck(item, pPolygon->m_Show);
								}
	}
}


void CFormView3::OnTvnItemChangedObjects(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CFormView3::OnShowSurfaceParameter()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	CShowSurfaceParameterDomain dialog;
	dialog.pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	if (dialog.DoModal() == IDOK)
	{
		int ii = 0;
	}
}
void CFormView3::OnSurfaceDelete()
{
	//删除三维空间中的相应曲面
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	//	ASSERT_VALID(pDoc);
	pDoc->m_SceneGraph.RemoveAt(currentindex);

	UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnShowOnlyMe()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	for (int ii = 0; ii < pDoc->m_SceneGraph.NbObject(); ii++)
	{
		if (ii == currentindex)
		{
			if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(ii);
				pMesh->m_Show = 1;
			}
			else if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
			{
				CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
				pNurbs->m_Show = 1;
			}
		}
		else
		{
			if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(ii);
				pMesh->m_Show = 0;
			}
			else if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
			{
				CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
				pNurbs->m_Show = 0;
			}

		}

	}
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnSurfaceShowIsoParameterCurvesGeneralBilinear()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pNurbs->m_showIsoParameterCurvesGeneralBilinear = !pNurbs->m_showIsoParameterCurvesGeneralBilinear;
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnSurfaceShowIsoParameterCurves()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		pNurbs->m_showIsoParameterCurves = !pNurbs->m_showIsoParameterCurves;
	}
	else
	{
		Tspline* pNurbs = (Tspline*)pDoc->m_SceneGraph.GetAt(currentindex);
		pNurbs->m_showIsoParameterCurves = !pNurbs->m_showIsoParameterCurves;
	}
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnSurfaceShowIsoLines()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pNurbs->m_showOptimalIsoParameterCurves_demo10 = !pNurbs->m_showOptimalIsoParameterCurves_demo10;
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnSurfaceShowIsoParameterCurves_mobius()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pNurbs->m_showOptimalIsoParameterCurves_demo11 = !pNurbs->m_showOptimalIsoParameterCurves_demo11;
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnSurfaceShowIsoParameterCurves_mb()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pNurbs->m_showOptimalIsoParameterCurves_demo12 = !pNurbs->m_showOptimalIsoParameterCurves_demo12;
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnSurfaceShowIsoParameterCurves_mc()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pNurbs->m_showOptimalIsoParameterCurves_demo13 = !pNurbs->m_showOptimalIsoParameterCurves_demo13;
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnSurfaceShowColorBoundary()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pNurbs->m_showColorBoundary = !pNurbs->m_showColorBoundary;
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnUpdateSurfaceShowisoparametercurves(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pNurbs->m_showIsoParameterCurves);
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_TSPLINE)
		{
			Tspline* pNurbs = (Tspline*)pDoc->m_SceneGraph.GetAt(currentindex);
			pCmdUI->SetCheck(pNurbs->m_showIsoParameterCurves);
		}

}
void CFormView3::OnUpdateSurfaceShowisoparametercurvesGeneralBilinear(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showIsoParameterCurvesGeneralBilinear);
}
void CFormView3::OnUpdateSurfaceShowOptimalIsoParameterCurves(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showOptimalIsoParameterCurves);
}
void CFormView3::OnSurfaceShowOptimalIsoParameterCurves()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		pNurbs->m_showOptimalIsoParameterCurves = !pNurbs->m_showOptimalIsoParameterCurves;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_TSPLINE)
		{
			Tspline* pNurbs = (Tspline*)pDoc->m_SceneGraph.GetAt(currentindex);
			pNurbs->m_showOptimalIsoParameterCurves = !pNurbs->m_showOptimalIsoParameterCurves;
		}

	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnSurfaceShowSubdivisionCurves()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pNurbs->m_showSubdivisionCurves = !pNurbs->m_showSubdivisionCurves;
	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnUpdateSurfaceShowSubdivisionCurves(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showSubdivisionCurves);
}

void CFormView3::OnSurfaceShowSurface()
{
	// TODO: Add your command handler code here
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		pNurbs->m_showSurface = !pNurbs->m_showSurface;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_TSPLINE)
		{
			Tspline* pNurbs = (Tspline*)pDoc->m_SceneGraph.GetAt(currentindex);
			pNurbs->m_showSurface = !pNurbs->m_showSurface;
		}

	UpdateShowView();
}
void CFormView3::OnSurfaceShowcontrolpointnetwork()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		pNurbs->m_showControlPointsNetwork = !pNurbs->m_showControlPointsNetwork;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_TSPLINE)
		{
			Tspline* pNurbs = (Tspline*)pDoc->m_SceneGraph.GetAt(currentindex);
			pNurbs->m_showControlPointsNetwork = !pNurbs->m_showControlPointsNetwork;
		}

	UpdateShowView();
}

void CFormView3::OnUpdateSurfaceShowcontrolpointnetwork(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pNurbs->m_showControlPointsNetwork);
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_TSPLINE)
		{
			Tspline* pNurbs = (Tspline*)pDoc->m_SceneGraph.GetAt(currentindex);
			pCmdUI->SetCheck(pNurbs->m_showControlPointsNetwork);
		}
}

void CFormView3::OnUpdateSurfaceShowcolorboundary(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showColorBoundary);
}

void CFormView3::OnUpdateSurfaceTransformationofmobius(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showOptimalIsoParameterCurves_demo11);
}
void CFormView3::OnUpdateSurfaceISOlines(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showOptimalIsoParameterCurves_demo10);
}
void CFormView3::OnUpdateSurfaceTransformationofmobiusandbilinear(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showOptimalIsoParameterCurves_demo12);
}
void CFormView3::OnUpdateSurfaceTransformationofmobiusandcubic(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showOptimalIsoParameterCurves_demo13);
}
void CFormView3::OnUpdateSurfaceShowsubdivisioncurves(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showSubdivisionCurves);
}

void CFormView3::OnUpdateSurfaceShowoptimalisoparametercurves(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showOptimalIsoParameterCurves);
}

void CFormView3::OnUpdateSurfaceShowsurface(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pNurbs->m_showSurface);
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_TSPLINE)
		{
			Tspline* pNurbs = (Tspline*)pDoc->m_SceneGraph.GetAt(currentindex);
			pCmdUI->SetCheck(pNurbs->m_showSurface);
		}
}

void CFormView3::OnBnClickedAddknot()
{
	// TODO: Add your control notification handler code here
	//Add knot 0.5.
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pNurbsSurf = pNurbs->Converte_C_NurbsSurface();
	pNurbsSurf->InsertMultiUKnot(3, 0.5);

	//output the information of the surface
	TRACE("%d %d \n", pNurbsSurf->GetCtrlNumberU(), pNurbsSurf->GetCtrlNumberV());
	TRACE("%d %d \n", pNurbsSurf->GetDegreeU(), pNurbsSurf->GetDegreeV());
	for (int i = 0; i < pNurbsSurf->GetCtrlNumberU() + pNurbsSurf->GetDegreeU() + 1; i++)
	{
		TRACE("%f ", pNurbsSurf->GetUKnot(i));
	}
	TRACE("\n");
	for (int i = 0; i < pNurbsSurf->GetCtrlNumberV() + pNurbsSurf->GetDegreeV() + 1; i++)
	{
		TRACE("%f ", pNurbsSurf->GetVKnot(i));
	}
	TRACE("\n");
	for (int i = 0; i < pNurbsSurf->GetCtrlNumberU(); i++)
		for (int j = 0; j < pNurbsSurf->GetCtrlNumberV(); j++)
		{
			Point3D point = pNurbsSurf->GetControlPoint(i, j);
			TRACE("%f %f %f 1 \n", point.x, point.y, point.z);
		}
}
void CFormView3::OnBnClickedQuadraticparameterize()
{
	//First Construct the date Structure for the NURBS surface.
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	CFileDialog dialog(TRUE);
	CString ReparaFileName;
	if (dialog.DoModal())
	{
		ReparaFileName = dialog.GetPathName();
	}
	else
		return;

	std::ifstream file(ReparaFileName);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* poriginalSurf = pNurbs->Converte_C_NurbsSurface();

	//Insert internal knot
	poriginalSurf->SubdividSurface();

	//first read the knot and order num of the reparameterized surface;
	int controlnumu, controlnumv, orderu, orderv;
	file >> controlnumu >> controlnumv >> orderu >> orderv;

	C_NurbsSurf* pNewSurf = new C_NurbsSurf(controlnumu, controlnumv, orderu, orderv);

	//read the knot.
	int uknotnum, vknotnum;
	file >> uknotnum >> vknotnum;
	for (int i = 0; i < uknotnum; i++)
	{
		double u;
		file >> u;
		pNewSurf->SetUKnot(i, u);
	}

	for (int i = 0; i < vknotnum; i++)
	{
		double v;
		file >> v;
		pNewSurf->SetVKnot(i, v);
	}

	//Read the reparameterization coefficients.
	pNurbs->ReadReparameterizationCoefficients(file);


	//Perform the bilinear reparameterization for each sub surface.
	int num = 0;
	for (int i = 0; i < pNurbs->alphaarraynum2; i++)
		for (int j = 0; j < pNurbs->betaarraynum2; j++)
		{
			//construct the surface.
			C_NurbsSurf* psurf = new C_NurbsSurf(4, 4, 4, 4);
			for (int ii = i * 4; ii < (i + 1) * 4; ii++)
				for (int jj = j * 4; jj < (j + 1) * 4; jj++)
				{
					for (int ij = 0; ij < 4; ij++)
					{
						psurf->SetUKnot(ij, 0);
						psurf->SetVKnot(ij, 0);
						psurf->SetUKnot(ij + 4, 1);
						psurf->SetVKnot(ij + 4, 1);
					}

					Point3D point = poriginalSurf->GetControlPoint(ii, jj);
					TRACE("%d %d: Point: %f, %f, %f \n", ii, jj, point.x, point.y, point.z);
					psurf->SetControlPoint(ii - i * 4, jj - j * 4, point);
					psurf->SetWeight(ii - i * 4, jj - j * 4, 1);
				}

			//pNurbs->alphaarraynum1 * i + j * 2
			//pNurbs->betaarraynum1 * j + i * 2
			//
			TRACE("Reparameteriztion:a %f %f %f \n", pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j * 2], pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j * 2 + 1], pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j * 2 + 2]);
			TRACE("Reparameteriztion:b %f %f %f \n", pNurbs->betaarray[pNurbs->betaarraynum1 * j + i * 2], pNurbs->betaarray[pNurbs->betaarraynum1 * j + i * 2 + 1], pNurbs->betaarray[pNurbs->betaarraynum1 * j + i * 2 + 2]);
			TRACE("---------------Reparameterization Surface------------\n");
			//reparameterize
			C_NurbsSurf* pRepararizeSurface = psurf->QuadraticTransformedSurface(pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j * 2], pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j * 2 + 1], pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j * 2 + 2],
				pNurbs->betaarray[pNurbs->betaarraynum1 * j + i * 2], pNurbs->betaarray[pNurbs->betaarraynum1 * j + i * 2 + 1], pNurbs->betaarray[pNurbs->betaarraynum1 * j + i * 2 + 2]);

			//delete psurf;
			for (int ii = i * 10; ii < (i + 1) * 10; ii++)
				for (int jj = j * 10; jj < (j + 1) * 10; jj++)
				{
					Point3D p = pRepararizeSurface->GetControlPoint(ii - i * 10, jj - j * 10);
					double w = pRepararizeSurface->GetWeight(ii - i * 10, jj - j * 10);
					pNewSurf->SetControlPoint(ii, jj, p);
					TRACE("Point %d %d: %f %f %f Weight %f\n", ii, jj, p.x, p.y, p.z, w);
					pNewSurf->SetWeight(ii, jj, w);
				}
			//delete pRepararizeSurface;
				//delete pReparaterizedSurface;
			//CNurbsSuface* pNurbsSurface1 = psurf->GetCNurbsSurface();
			//pNurbsSurface1->SetTransform(*pNurbs->GetTransform());
			//pDoc->m_SceneGraph.Add(pNurbsSurface1);
			//if(pDoc->m_SceneGraph.NbObject() == 170)
			//{
			//	int ii = 0;
			//}
			//CNurbsSuface* pNurbsSurface2 = pRepararizeSurface->GetCNurbsSurface();
			//pNurbsSurface2->SetTransform(*pNurbs->GetTransform());
			//pDoc->m_SceneGraph.Add(pNurbsSurface2);
			//num++;
			//if(num == 2)
			//	goto label;
		}
label:
	//Add the surface to the array.
	CNurbsSuface* pNurbsSurface = pNewSurf->GetCNurbsSurface();
	pNurbsSurface->SetTransform(*pNurbs->GetTransform());
	pDoc->m_SceneGraph.Add(pNurbsSurface);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnBnClickedBilinearparameterize()
{
	// TODO: Add your control notification handler code here
	//First Construct the date Structure for the NURBS surface.
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	CFileDialog dialog(TRUE);
	CString ReparaFileName;
	if (dialog.DoModal())
	{
		ReparaFileName = dialog.GetPathName();
	}
	else
		return;

	std::ifstream file(ReparaFileName);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* poriginalSurf = pNurbs->Converte_C_NurbsSurface();

	//Insert internal knot
	poriginalSurf->SubdividSurface();

	//first read the knot and order num of the reparameterized surface;
	int controlnumu, controlnumv, orderu, orderv;
	file >> controlnumu >> controlnumv >> orderu >> orderv;

	C_NurbsSurf* pNewSurf = new C_NurbsSurf(controlnumu, controlnumv, orderu, orderv);

	//read the knot.
	int uknotnum, vknotnum;
	file >> uknotnum >> vknotnum;
	for (int i = 0; i < uknotnum; i++)
	{
		double u;
		file >> u;
		pNewSurf->SetUKnot(i, u);
	}

	for (int i = 0; i < vknotnum; i++)
	{
		double v;
		file >> v;
		pNewSurf->SetVKnot(i, v);
	}

	//Read the reparameterization coefficients.
	pNurbs->ReadReparameterizationCoefficients(file);

	//Perform the bilinear reparameterization for each sub surface.
	int num = 0;
	for (int i = 0; i < pNurbs->alphaarraynum2; i++)
		for (int j = 0; j < pNurbs->betaarraynum2; j++)
		{
			//construct the surface.
			C_NurbsSurf* psurf = new C_NurbsSurf(4, 4, 4, 4);
			for (int ii = i * 4; ii < (i + 1) * 4; ii++)
				for (int jj = j * 4; jj < (j + 1) * 4; jj++)
				{
					for (int ij = 0; ij < 4; ij++)
					{
						psurf->SetUKnot(ij, 0);
						psurf->SetVKnot(ij, 0);
						psurf->SetUKnot(ij + 4, 1);
						psurf->SetVKnot(ij + 4, 1);
					}

					Point3D point = poriginalSurf->GetControlPoint(ii, jj);
					TRACE("%d %d: Point: %f, %f, %f \n", ii, jj, point.x, point.y, point.z);
					psurf->SetControlPoint(ii - i * 4, jj - j * 4, point);
					psurf->SetWeight(ii - i * 4, jj - j * 4, 1);
				}
			//pNurbs->alphaarraynum1 * i + j
			//pNurbs->betaarraynum1 * j + i
			TRACE("Reparameteriztion: %f %f %f %f \n", pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j], pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j + 1], pNurbs->betaarray[pNurbs->betaarraynum1 * j + i], pNurbs->betaarray[pNurbs->betaarraynum1 * j + i + 1]);
			TRACE("---------------Reparameterization Surface------------\n");
			//reparameterize
			C_NurbsSurf* pReparaterizedSurface = psurf->GetBilinearTransformedSurface(pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j], pNurbs->alphaarray[pNurbs->alphaarraynum1 * i + j + 1], pNurbs->betaarray[pNurbs->betaarraynum1 * j + i], pNurbs->betaarray[pNurbs->betaarraynum1 * j + i + 1]);
			//delete psurf;
			for (int ii = i * 7; ii < (i + 1) * 7; ii++)
				for (int jj = j * 7; jj < (j + 1) * 7; jj++)
				{
					Point3D p = pReparaterizedSurface->GetControlPoint(ii - i * 7, jj - j * 7);
					double w = pReparaterizedSurface->GetWeight(ii - i * 7, jj - j * 7);
					pNewSurf->SetControlPoint(ii, jj, p);
					TRACE("Point %d %d: %f %f %f Weight %f\n", ii, jj, p.x, p.y, p.z, w);
					pNewSurf->SetWeight(ii, jj, w);
				}
			//delete pReparaterizedSurface;
			//CNurbsSuface* pNurbsSurface1 = psurf->GetCNurbsSurface();
			//pNurbsSurface1->SetTransform(*pNurbs->GetTransform());
			//pDoc->m_SceneGraph.Add(pNurbsSurface1);
			//CNurbsSuface* pNurbsSurface2 = pReparaterizedSurface->GetCNurbsSurface();
			//pNurbsSurface2->SetTransform(*pNurbs->GetTransform());
			//pDoc->m_SceneGraph.Add(pNurbsSurface2);
			//num++;
			//if(num == 18)
			//	goto label;
		}
label:
	//Add the surface to the array.
	CNurbsSuface* pNurbsSurface = pNewSurf->GetCNurbsSurface();
	pNurbsSurface->SetTransform(*pNurbs->GetTransform());
	pDoc->m_SceneGraph.Add(pNurbsSurface);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnSurfaceShowsubsurfacecoordinate()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pNurbs->m_showSubSurfaceCoordinate = !pNurbs->m_showSubSurfaceCoordinate;
	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnUpdateSurfaceShowsubsurfacecoordinate(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showSubSurfaceCoordinate);
}

void CFormView3::OnSurfaceSubdivide()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	if (currentindex < 0 || currentindex >= pDoc->m_SceneGraph.NbObject())
		return;

	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	C_NurbsSurf* poriginalSurf = pNurbs->Converte_C_NurbsSurface();

	//Insert internal knot
	poriginalSurf->SubdividSurface();
	CNurbsSuface* pNurbsSurf = poriginalSurf->GetCNurbsSurface();
	delete pNurbs;
	pDoc->m_SceneGraph.RemoveAt(currentindex);
	pDoc->m_SceneGraph.Add(pNurbsSurf);
	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnSurfaceShowbilinearcoefficients()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pNurbs->m_showBilinearCoefficients = !pNurbs->m_showBilinearCoefficients;
	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnUpdateSurfaceShowbilinearcoefficients(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	pCmdUI->SetCheck(pNurbs->m_showBilinearCoefficients);
}

void CFormView3::OnSurfaceShowsurfaceinformation()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);

	SurfaceInformation dialog;
	dialog.pSurface = pNurbs;
	dialog.DoModal();
}

void CFormView3::OnRenderSetrendermode()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	RenderMode dialog;
	dialog.m_UsingOpenglNurbsRender = 1 - pDoc->m_UsingOpenglNurbsRender;
	dialog.m_Tessellation = pDoc->m_Tessellation;
	if (dialog.DoModal() == IDOK)
	{
		pDoc->m_UsingOpenglNurbsRender = 1 - dialog.m_UsingOpenglNurbsRender;
		pDoc->m_Tessellation = dialog.m_Tessellation;
		for (int i = 0; i < pDoc->m_SceneGraph.NbObject(); i++)
		{
			if (pDoc->m_SceneGraph.GetAt(i)->GetType() == TYPE_NURBSSURFACE)
			{
				CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(i);
				pSurface->m_ListDone = FALSE;
			}
			if (pDoc->m_SceneGraph.GetAt(i)->GetType() == TYPE_TSPLINE)
			{
				Tspline* pSurface = (Tspline*)pDoc->m_SceneGraph.GetAt(i);
				pSurface->m_ListDone = FALSE;
			}
		}
		pDoc->UpdateAllViews(NULL);
	}
}
using namespace alglib;
std::vector<std::vector<double>> MeshArray;//(p00x p00y p00z) (p10x p10y p10z) (p01x p01y p01z) (p11x p11y p11z)
void function11_func(const real_1d_array& x, double& func, real_1d_array& grad, void* ptr)
{

	//排列顺序为p00x，p00y,p00z,p10x,p10y,p10z.......

	OrthogonalityEnergy OrthEnergy(x);
	std::vector<double> mesh;
	for (int ii = 0; ii < 12; ii++)
		mesh.push_back(x[ii]);

	MeshArray.push_back(mesh);
	OrthEnergy.getGrad11(func, grad);//func,Grad

	TRACE("%s :grad\n", grad.tostring(5).c_str());
	TRACE("%s :x\n", x.tostring(5).c_str());
	TRACE("%f :J\n\n\n", func);

}
string DoubleToString(double d)
{
	//这个函数的作用是将double 转换为string。但double仅能为6位
	using namespace std;
	string str;
	stringstream ss;
	//ss<<d;
	//ss>>str;
	//ss.clear();//清空流
	//return str;
	char message[100];
	sprintf(message, "%f", d);
	str = message;
	return str;
}
std::vector<double> CFormView3::MatrixVectorMultiplication(std::vector<std::vector<double>>& A, std::vector<double>& B)
{
	//A *ｂ
	//if(A.size() < 1)
	//	return;

	int m = A.size();
	int n = A[0].size();

	real_2d_array a;
	a.setlength(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			a(i, j) = A[i][j];
		}
	real_1d_array b;
	b.setlength(n);
	for (int i = 0; i < n; i++)
		b(i) = B[i];
	double threshold = 0;
	int info;
	densesolverlsreport rep;
	real_1d_array x;
	x.setlength(m);

	rmatrixmv(m, n, a, 0, 0, 0, b, 0, x, 0);
	std::vector<double> X;
	X.resize(x.length());

	for (int i = 0; i < x.length(); i++)
	{
		X[i] = x(i);
	}
	return X;
}

void CFormView3::SolveLinearSystem(std::vector<std::vector<double>>& A, std::vector<double>& B, std::vector<double>& X)
{
	//solve the matrix 
	//A * x = B

	if (A.size() < 1)
		return;

	int m = A.size();
	int n = A[0].size();

	real_2d_array a;
	a.setlength(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			a(i, j) = A[i][j];
		}
	real_1d_array b;
	b.setlength(n);
	for (int i = 0; i < n; i++)
		b(i) = B[i];
	double threshold = 0;
	int64_t info;
	densesolverlsreport rep;
	real_1d_array x;

	alglib::rmatrixsolvels(a, m, n, b, threshold, info, rep, x);
	X.resize(x.length(), 0);

	for (int i = 0; i < x.length(); i++)
	{
		X[i] = x(i);
	}

	/*a(0,0) = 3;
	a(0,1) = 2;
	a(1,0) = 6;
	a(1,1) = 7;
	int nrows = 2;
	int ncols = 2;
	real_1d_array b;
	b.setlength(2);
	b(0) = 1;
	b(1) = 2;
	double threshold = 0;
	int info;
	densesolverlsreport rep;
	real_1d_array x;

	alglib::rmatrixsolvels(a,nrows,ncols,b,threshold,info,rep,x);
	for(int i = 0; i < x.length(); i++)
	{
		TRACE(" %f ",x(i));
	}*/
}

void CFormView3::OptimizationTest11(C_NurbsSurf* pSurf)
{

	using namespace alglib;
	//这是已经写好了的
	//get the control points
	Vector3D p00 = pSurf->GetControlPoint(0, 0) - ZeroP3D;
	Vector3D p10 = pSurf->GetControlPoint(1, 0) - ZeroP3D;
	Vector3D p01 = pSurf->GetControlPoint(0, 1) - ZeroP3D;
	Vector3D p11 = pSurf->GetControlPoint(1, 1) - ZeroP3D;


	string str = "[ " + DoubleToString(p00.x) + " , " + DoubleToString(p00.y) + " , " + DoubleToString(p00.z) + " , "
		+ DoubleToString(p10.x) + " , " + DoubleToString(p10.y) + " , " + DoubleToString(p10.z) + " , "
		+ DoubleToString(p01.x) + " , " + DoubleToString(p01.y) + " , " + DoubleToString(p01.z) + " , "
		+ DoubleToString(p11.x) + " , " + DoubleToString(p11.y) + " , " + DoubleToString(p11.z) + " ] ";

	Vector3D N = (p10 - p00) ^ (p01 - p00);
	N.SetUnit();

	double k = p00.x * N.x + p00.y * N.y + p00.z * N.z;//常数

	string str2 = "[[0,0,0,0,0,0," + DoubleToString(N.x) + "," + DoubleToString(N.y) + "," + DoubleToString(N.z) + ",0,0,0," + DoubleToString(k) + "]]";
	real_1d_array	x(str.c_str());
	real_2d_array 	c(str2.c_str());
	integer_1d_array ct("[0]");//负数表示小于等于，0表示等于，整数表示大于等于
	double epsg = 0.0000000001;
	double epsf = 0;
	double epsx = 0;
	ae_int_t maxits = 0;
	minbleicstate state;
	minbleicreport rep;

	minbleiccreate(x, state);
	minbleicsetlc(state, c, ct);
	minbleicsetcond(state, epsg, epsf, epsx, maxits);
	alglib::minbleicoptimize(state, function11_func);
	minbleicresults(state, x, rep);

	//Now the results are stored in the X

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num < 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurf2 = pNSurf->Converte_C_NurbsSurface();
	C_NurbsSurf* pNewSurf = (C_NurbsSurf*)pSurf2->Copy();

	//make the control points and show the new mesh
	Point3D Pp00(x[0], x[1], x[2]);
	Point3D Pp10(x[3], x[4], x[5]);
	Point3D Pp01(x[6], x[7], x[8]);
	Point3D Pp11(x[9], x[10], x[11]);
	pNewSurf->SetControlPoint(0, 0, Pp00);
	pNewSurf->SetControlPoint(1, 0, Pp10);
	pNewSurf->SetControlPoint(0, 1, Pp01);
	pNewSurf->SetControlPoint(1, 1, Pp11);

	CNurbsSuface* pNewNSurf = pNewSurf->GetCNurbsSurface();
	pNewNSurf->SetTransform(*pNSurf->GetTransform());
	pNewNSurf->filename = "Optimized Surface";
	pDoc->m_SceneGraph.Add(pNewNSurf);
	pDoc->UpdateTreeControl();
}

void function22_func(const real_1d_array& x, double& func, real_1d_array& grad, void* ptr)
{

	//排列顺序为p00x，p00y,p00z,p10x,p10y,p10z.......

	OrthogonalityEnergy OrthEnergy(x);

	std::vector<double> mesh;
	for (int ii = 0; ii < 27; ii++)
		mesh.push_back(x[ii]);

	MeshArray.push_back(mesh);

	OrthEnergy.getGrad22(func, grad);//func,Grad
	int ItemSel = ((CComboBoxEx*)p_FormView3->GetDlgItem(IDC_COMBOSELECT))->GetCurSel();
	switch (ItemSel)
	{
	case 0:
		//Fix One Corner
		for (int i = 0; i < 6; i++)
			grad[i] = 0;
		break;
	case 1:
		//Fix Three Corner
		grad[0] = grad[1] = grad[2] = 0;
		grad[6] = grad[7] = grad[8] = 0;
		//grad[18]=grad[19]=grad[20]=0;
		grad[24] = grad[25] = grad[26] = 0;
		break;
	case 2:
		//Fix Four Corner
		grad[0] = grad[1] = grad[2] = 0;
		grad[6] = grad[7] = grad[8] = 0;
		grad[18] = grad[19] = grad[20] = 0;
		grad[24] = grad[25] = grad[26] = 0;
		break;
	case 3:
		//Fix Zero Corner
		break;
	}


	TRACE("%s :grad\n", grad.tostring(5).c_str());
	TRACE("%s :x\n", x.tostring(5).c_str());
	TRACE("%f :J\n", func);
	TRACE("-----------------------------------------------------------------------------------------------------------------------------------\n");

}

void CFormView3::OptimizationTest22(C_NurbsSurf* pSurf)
{
	using namespace alglib;

	//get the control points
	Vector3D pArray[3][3];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			pArray[i][j] = pSurf->GetControlPoint(i, j) - ZeroP3D;

	string str = "[";
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				if (i == 2 && j == 2 && k == 2)
					str += DoubleToString(pArray[j][i][k]) + "]";
				else
					str += DoubleToString(pArray[j][i][k]) + ",";

	Vector3D N = (pArray[1][0] - pArray[0][0]) ^ (pArray[0][1] - pArray[0][0]);
	N.SetUnit();
	double k = pArray[0][0].x * N.x + pArray[0][0].y * N.y + pArray[0][0].z * N.z;

	string str2;

	int ItemSel = ((CComboBoxEx*)p_FormView3->GetDlgItem(IDC_COMBOSELECT))->GetCurSel();
	switch (ItemSel)
	{
	case 0:
		//Fix One Corner
		str2 = "[[0,0,0,	0,0,0,	0,0,0," + DoubleToString(N.x) + "," + DoubleToString(N.y) + "," + DoubleToString(N.z) + ",0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0," + DoubleToString(k) + "]]";
		break;
	case 1:
	case 2:
		//Fix Four Corner
		str2 = "[[0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0, 0]]";
		break;
	}

	real_1d_array	x(str.c_str());
	real_2d_array 	c(str2.c_str());
	integer_1d_array ct("[0]");//负数表示小于等于，0表示等于，整数表示大于等于
	double epsg = 0;
	double epsf = 0;
	double epsx = 0;
	ae_int_t maxits = 0;
	minbleicstate state;
	minbleicreport rep;

	minbleiccreate(x, state);
	minbleicsetlc(state, c, ct);
	minbleicsetcond(state, epsg, epsf, epsx, maxits);
	alglib::minbleicoptimize(state, function22_func);
	minbleicresults(state, x, rep);

	//Now the results are stored in the X

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num < 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurf2 = pNSurf->Converte_C_NurbsSurface();
	C_NurbsSurf* pNewSurf = (C_NurbsSurf*)pSurf2->Copy();

	//make the control points and show the new mesh

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			Point3D point;
			point.x = x[OrthogonalityEnergy::getIndex(2, 2, i, j, 0)];
			point.y = x[OrthogonalityEnergy::getIndex(2, 2, i, j, 1)];
			point.z = x[OrthogonalityEnergy::getIndex(2, 2, i, j, 2)];
			pNewSurf->SetControlPoint(i, j, point);
		}

	CNurbsSuface* pNewNSurf = pNewSurf->GetCNurbsSurface();
	pNewNSurf->SetTransform(*pNSurf->GetTransform());
	pDoc->m_SceneGraph.Add(pNewNSurf);
	pDoc->UpdateTreeControl();
}

void function33_func(const real_1d_array& x, double& func, real_1d_array& grad, void* ptr)
{

	//排列顺序为p00x，p00y,p00z,p10x,p10y,p10z.......

	OrthogonalityEnergy OrthEnergy(x);

	std::vector<double> mesh;
	for (int ii = 0; ii < x.length(); ii++)
		mesh.push_back(x[ii]);

	MeshArray.push_back(mesh);

	//Set the optimization mesh
	for (int i = 0; i < pOriginalOptimizationSurf->GetCtrlNumberV(); i++)
		for (int j = 0; j < pOriginalOptimizationSurf->GetCtrlNumberU(); j++)
		{
			int index = OrthogonalityEnergy::getIndexBSpline33(j, i, 0);
			Point3D point;
			point.x = x[index];
			point.y = x[index + 1];
			point.z = x[index + 2];
			pCurOptimizationSurf->SetControlPoint(j, i, point);
		}

	OrthEnergy.getBSplineGrad33(func, grad);//func,Grad
	int ItemSel = 0;
	switch (ItemSel)
	{
	case 0:
		//Fix One Corner
		for (int i = 0; i < 3; i++)
			grad[i] = 0;
		break;
	case 1:
		//Fix Three Corner
		grad[0] = grad[1] = grad[2] = 0;
		grad[uBezierNum * 9] = grad[uBezierNum * 9 + 1] = grad[uBezierNum * 9 + 2] = 0;
		//grad[9]=grad[10]=grad[11]=0;
		grad[36 * uBezierNum * vBezierNum] = grad[36 * uBezierNum * vBezierNum + 1] = grad[36 * uBezierNum * vBezierNum + 2] = 0;
		break;
	case 2:
		//Fix Four Corner
		grad[0] = grad[1] = grad[2] = 0;
		grad[uBezierNum * 9] = grad[uBezierNum * 9 + 1] = grad[uBezierNum * 9 + 2] = 0;
		grad[(3 * uBezierNum + 1) * 3 * (vBezierNum * 3)] = grad[(3 * uBezierNum + 1) * 3 * (vBezierNum * 3) + 1] = grad[(3 * uBezierNum + 1) * 3 * (vBezierNum * 3) + 2] = 0;
		grad[x.length() - 3] = grad[x.length() - 2] = grad[x.length() - 1] = 0;
		break;
	case 3:
		//Fix Zero Corner
		break;
	}

#ifdef SHOWTRACEINFORMATION
	TRACE("%s :grad\n", grad.tostring(5).c_str());
	TRACE("%s :x\n", x.tostring(5).c_str());
	TRACE("%f :J\n", func);
	TRACE("-----------------------------------------------------------------------------------------------------------------------------------\n");
#endif

}
void CFormView3::setLinearConstraint(int uConNum, int vConNum, const std::vector<std::vector<Vector3D>>& pArray, std::string& staments, std::string& relation)
{
	int coutOfStatemet = 0;

	staments = "[[ ";
	//计算竖着的
	for (int i = 1; i < uConNum - 1; i++)//第一行与最后一行不取
	{
		for (int j = 0; j < vConNum; j++)
		{

			if (i % 3 != 0)//非2边点
				break;
			Vector3D point = pArray[i][j];
			Vector3D point2 = pArray[i - 1][j];
			Vector3D point3 = pArray[i + 1][j];
			double sum = sqrt((point2 - point) * (point2 - point)) + sqrt((point3 - point) * (point3 - point));
			double k2 = sqrt((point2 - point) * (point2 - point)) / sum;
			double k3 = 1 - k2;

			string temp = "";
			for (int j3 = 0; j3 < vConNum; j3++)
				for (int i3 = 0; i3 < uConNum; i3++)
				{
					if (j3 == j && i3 == i - 1)
						temp += DoubleToString(k3) + ",0,0, ";
					else if (j3 == j && i3 == i)
						temp += "-1,0,0, ";
					else if (j3 == j && i3 == i + 1)
						temp += DoubleToString(k2) + ",0,0, ";
					else
						temp += "0,0,0, ";
				}
			TRACE("(%d,%d): %s\n", i, j, temp.c_str());
			staments += temp;
			staments.push_back('0');
			staments.push_back(']');
			coutOfStatemet++;
			staments += ",[";
			temp = "";
			for (int j3 = 0; j3 < vConNum; j3++)
				for (int i3 = 0; i3 < uConNum; i3++)
				{
					if (j3 == j && i3 == i - 1)
						temp += "0," + DoubleToString(k3) + ",0, ";
					else if (j3 == j && i3 == i)
						temp += "0,-1,0, ";
					else if (j3 == j && i3 == i + 1)
						temp += "0," + DoubleToString(k2) + ",0, ";
					else
						temp += "0,0,0, ";
				}
			TRACE("(%d,%d): %s\n", i, j, temp.c_str());
			staments += temp;
			staments.push_back('0');
			staments.push_back(']');
			coutOfStatemet++;
			staments += ",[";
			temp = "";
			for (int j3 = 0; j3 < vConNum; j3++)
				for (int i3 = 0; i3 < uConNum; i3++)
				{
					if (j3 == j && i3 == i - 1)
						temp += "0,0," + DoubleToString(k3) + ", ";
					else if (j3 == j && i3 == i)
						temp += "0,0,-1, ";
					else if (j3 == j && i3 == i + 1)
						temp += "0,0," + DoubleToString(k2) + ", ";
					else
						temp += "0,0,0, ";
				}
			TRACE("(%d,%d): %s\n", i, j, temp.c_str());
			staments += temp;
			staments.push_back('0');
			staments.push_back(']');
			coutOfStatemet++;
			staments += ",[";


		}
	}

	//计算横着的
	for (int i = 0; i < uConNum; i++)
	{
		for (int j = 1; j < vConNum - 1; j++)
		{
			if (j % 3 != 0)//非2边
				break;

			Vector3D point = pArray[i][j];
			Vector3D point2 = pArray[i][j - 1];
			Vector3D point3 = pArray[i][j + 1];
			double sum = sqrt((point2 - point) * (point2 - point)) + sqrt((point3 - point) * (point3 - point));
			double k2 = sqrt((point2 - point) * (point2 - point)) / sum;
			double k3 = 1 - k2;
			string temp = "";
			for (int j3 = 0; j3 < vConNum; j3++)
				for (int i3 = 0; i3 < uConNum; i3++)
				{
					if (j3 == j - 1 && i3 == i)
						temp += DoubleToString(k3) + ",0,0, ";
					else if (j3 == j && i3 == i)
						temp += "-1,0,0, ";
					else if (j3 == j + 1 && i3 == i)
						temp += DoubleToString(k2) + ",0,0, ";
					else
						temp += "0,0,0, ";
				}
			TRACE("(%d,%d): %s\n", i, j, temp.c_str());
			staments += temp;
			staments.push_back('0');
			staments.push_back(']');
			coutOfStatemet++;
			staments += ",[";
			temp = "";
			for (int j3 = 0; j3 < vConNum; j3++)
				for (int i3 = 0; i3 < uConNum; i3++)
				{
					if (j3 == j - 1 && i3 == i)
						temp += "0," + DoubleToString(k3) + ",0, ";
					else if (j3 == j && i3 == i)
						temp += "0,-1,0, ";
					else if (j3 == j + 1 && i3 == i)
						temp += "0," + DoubleToString(k2) + ",0, ";
					else
						temp += "0,0,0, ";
				}
			TRACE("(%d,%d): %s\n", i, j, temp.c_str());
			staments += temp;
			staments.push_back('0');
			staments.push_back(']');
			coutOfStatemet++;
			staments += ",[";
			temp = "";
			for (int j3 = 0; j3 < vConNum; j3++)
				for (int i3 = 0; i3 < uConNum; i3++)
				{
					if (j3 == j - 1 && i3 == i)
						temp += "0,0," + DoubleToString(k3) + ", ";
					else if (j3 == j && i3 == i)
						temp += "0,0,-1, ";
					else if (j3 == j + 1 && i3 == i)
						temp += "0,0," + DoubleToString(k2) + ", ";
					else
						temp += "0,0,0, ";
				}
			TRACE("(%d,%d): %s\n", i, j, temp.c_str());
			staments += temp;
			staments.push_back('0');
			staments.push_back(']');
			coutOfStatemet++;
			staments += ",[";
		}
	}

	staments[staments.length() - 2] = ' ';
	staments[staments.length() - 1] = ']';
	relation = "[ ";
	for (int i = 0; i < coutOfStatemet; i++)
		relation += "0,";
	relation[relation.length() - 1] = ']';

	if (coutOfStatemet == 0)//如果是0，那么说明这是一个Bezier,上面的无效
	{
		staments = "[[";
		for (int j = 0; j < vConNum; j++)
			for (int i = 0; i < uConNum; i++)
			{
				staments += "0,0,0,";
			}
		staments += "0]]";
		relation = "[0]";

	}
}
double CFormView3::ComputeEquiareality_NumericalMethod(C_NurbsSurf* pSurface)
{
	double A = Compute_Area(pSurface, 1, 1);
	int i, j;
	int m = 500;
	int n = 500;

	double h1 = 1.0 / (m * 2);
	double g1 = 1.0 / (n * 2);
	double val = 0;
	int Ai, Aj;
	double term1, term2;
	for (int ii = 0; ii <= m * 2; ii++)
	{
		for (int jj = 0; jj <= n * 2; jj++)
		{
			if (ii == 0 || ii == m * 2)
				Ai = 1;
			else
				Ai = 2;

			if (jj == 0 || jj == n * 2)
				Aj = 1;
			else
				Aj = 2;

			double ui = ii * h1;
			double vj = jj * g1;

			Vector3D xu = pSurface->ComputUDerivative1(ui, vj);
			Vector3D xv = pSurface->ComputVDerivative1(ui, vj);

			Vector3D xn = xu ^ xv;
			double len = SQR(Length(xn) - A);
			//double term11 = 2 * pow(1 - ui,2 - i) * pow(ui, i) / (Factorial(2 - i) * Factorial(i));
			//double term21 = 2 * pow(1 - vj,2 - j) * pow(vj, j) / (Factorial(2 - j) * Factorial(j));

			val += Ai * Aj * len;
		}
	}
	val = val * h1 * g1 / 4;

	return val;
}
void CFormView3::OptimizationTestBSpline33(C_NurbsSurf* pSurf)
{
	int uConNum = pSurf->GetCtrlNumberU();
	int vConNum = pSurf->GetCtrlNumberV();
	uBezierNum = (uConNum - 1) / 3;
	vBezierNum = (vConNum - 1) / 3;

	using namespace alglib;

	//get the control points
	std::vector<std::vector<Vector3D>> pArray;
	for (int i = 0; i < pSurf->GetCtrlNumberU(); i++)
	{
		std::vector<Vector3D> uArray;
		for (int j = 0; j < pSurf->GetCtrlNumberV(); j++)
			uArray.push_back(pSurf->GetControlPoint(i, j) - ZeroP3D);

		pArray.push_back(uArray);
	}

	string str = "[";
	for (int i = 0; i < pSurf->GetCtrlNumberV(); i++)
		for (int j = 0; j < pSurf->GetCtrlNumberU(); j++)
			for (int k = 0; k < 3; k++)
				if (i == pSurf->GetCtrlNumberV() - 1 && j == pSurf->GetCtrlNumberU() - 1 && k == 2)
					str += DoubleToString(pArray[j][i][k]) + "]";
				else
					str += DoubleToString(pArray[j][i][k]) + ",";

	// 	Vector3D N=(pArray[1][0]-pArray[0][0])^(pArray[0][1]-pArray[0][0]);
	// 	N.SetUnit();
	// 	double k=pArray[0][0].x*N.x+pArray[0][0].y*N.y+pArray[0][0].z*N.z;

	string str3, str4;
	setLinearConstraint(uConNum, vConNum, pArray, str3, str4);

	real_1d_array	x(str.c_str());
	real_2d_array 	c(str3.c_str());
	integer_1d_array ct(str4.c_str());//负数表示小于等于，0表示等于，整数表示大于等于
	double epsg = 0;
	double epsf = 0;
	double epsx = 0;
	ae_int_t maxits = 0;
	minbleicstate state;
	minbleicreport rep;

	minbleiccreate(x, state);
	minbleicsetlc(state, c, ct);
	minbleicsetcond(state, epsg, epsf, epsx, maxits);
	alglib::minbleicoptimize(state, function33_func);
	minbleicresults(state, x, rep);

	//Now the results are stored in the X

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num < 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurf2 = pNSurf->Converte_C_NurbsSurface();
	C_NurbsSurf* pNewSurf = (C_NurbsSurf*)pSurf2->Copy();

	//make the control points and show the new mesh

	for (int i = 0; i < uConNum; i++)
		for (int j = 0; j < vConNum; j++)
		{
			Point3D point;
			point.x = x[OrthogonalityEnergy::getIndexBSpline33(i, j, 0)];
			point.y = x[OrthogonalityEnergy::getIndexBSpline33(i, j, 1)];
			point.z = x[OrthogonalityEnergy::getIndexBSpline33(i, j, 2)];
			pNewSurf->SetControlPoint(i, j, point);
		}

	CNurbsSuface* pNewNSurf = pNewSurf->GetCNurbsSurface();
	pNewNSurf->SetTransform(*pNSurf->GetTransform());
	pNewNSurf->filename = "Optimized Surface";
	pDoc->m_SceneGraph.Add(pNewNSurf);
	pDoc->UpdateTreeControl();


}
void CFormView3::OptimizationTest33(C_NurbsSurf* pSurf, int i, int j)
{
	using namespace alglib;

	//get the control points
	Vector3D pArray[4][4];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			pArray[i][j] = pSurf->GetControlPoint(i, j) - ZeroP3D;

	string str = "[";
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 3; k++)
				if (i == 3 && j == 3 && k == 2)
					str += DoubleToString(pArray[j][i][k]) + "]";
				else
					str += DoubleToString(pArray[j][i][k]) + ",";

	Vector3D N = (pArray[1][0] - pArray[0][0]) ^ (pArray[0][1] - pArray[0][0]);
	N.SetUnit();
	double k = pArray[0][0].x * N.x + pArray[0][0].y * N.y + pArray[0][0].z * N.z;

	string str2;

	int ItemSel = ((CComboBoxEx*)p_FormView3->GetDlgItem(IDC_COMBOSELECT))->GetCurSel();
	switch (ItemSel)
	{
	case 0:
		//Fix One Corner
		str2 = "[  [0,0,0,	0,0,0,	0,0,0,	0,0,0,"
			+ DoubleToString(N.x) + "," + DoubleToString(N.y) + "," + DoubleToString(N.z) + ",0,0,0,	0,0,0,	0,0,0,"
			+ "0,0,0,	0,0,0,	0,0,0,	0,0,0,"
			+ "0,0,0,	0,0,0,	0,0,0,	0,0,0,"
			+ DoubleToString(k) + "]]";
		break;
	case 1:
	case 2:
		//Fix Four Corner
		str2 = "[[0,0,0,	0,0,0,	0,0,0,	0,0,0,";
		str2 += "0,0,0,	0,0,0,	0,0,0,	0,0,0,";
		str2 += "0,0,0,	0,0,0,	0,0,0,	0,0,0,";
		str2 += "0,0,0,	0,0,0,	0,0,0,	0,0,0,";
		str2 += "0]]";
		break;
	}

	real_1d_array	x(str.c_str());
	real_2d_array 	c(str2.c_str());
	integer_1d_array ct("[0]");//负数表示小于等于，0表示等于，整数表示大于等于
	double epsg = 0;
	double epsf = 0;
	double epsx = 0;
	ae_int_t maxits = 0;
	minbleicstate state;
	minbleicreport rep;

	minbleiccreate(x, state);
	minbleicsetlc(state, c, ct);
	minbleicsetcond(state, epsg, epsf, epsx, maxits);
	alglib::minbleicoptimize(state, function33_func);
	minbleicresults(state, x, rep);

	//Now the results are stored in the X

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num < 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurf2 = pNSurf->Converte_C_NurbsSurface();
	C_NurbsSurf* pNewSurf = (C_NurbsSurf*)pSurf2->Copy();

	//make the control points and show the new mesh

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			Point3D point;
			point.x = x[OrthogonalityEnergy::getIndex(3, 3, i, j, 0)];
			point.y = x[OrthogonalityEnergy::getIndex(3, 3, i, j, 1)];
			point.z = x[OrthogonalityEnergy::getIndex(3, 3, i, j, 2)];
			pNewSurf->SetControlPoint(i, j, point);
		}

	CNurbsSuface* pNewNSurf = pNewSurf->GetCNurbsSurface();
	pNewNSurf->SetTransform(*pNSurf->GetTransform());
	pDoc->m_SceneGraph.Add(pNewNSurf);
	pDoc->UpdateTreeControl();
}


void CFormView3::BilinearOptimizationTest()
{
	//GeneralBilinearTransformation dialog;
	//dialog.DoModal();
}
void CFormView3::GenerateQuadraticSurfaceFromBilinearSurface(C_NurbsSurf* pSurf)
{
	Vector3D p00 = pSurf->GetControlPoint(0, 0) - ZeroP3D;
	Vector3D p10 = pSurf->GetControlPoint(1, 0) - ZeroP3D;
	Vector3D p01 = pSurf->GetControlPoint(0, 1) - ZeroP3D;
	Vector3D p11 = pSurf->GetControlPoint(1, 1) - ZeroP3D;

}
void CFormView3::GenerateGeneralBilinearSurface(C_NurbsSurf* pNurbsSurf)
{

}
void CFormView3::OnGeneralBilinearTransformation()
{

	int m = 1;
	int n = 2;
	int k1 = 0;
	int k2 = 1;

	TRACE("Q%d,%d=", k1, k2);
	double kk = Factorial(m + n, k1) * Factorial(m + n, k2);
	for (int i1 = 0; i1 <= m; i1++)
		for (int j1 = 0; j1 <= n; j1++)
		{
			TRACE("(");
			int flag = 0;
			for (int p1 = 0; p1 <= m - i1; p1++)
			{
				if (p1 > k1)
					continue;
				if (p1 > k2)
					continue;

				for (int p2 = 0; p2 <= m - i1 - p1; p2++)
				{
					if (p2 > k1 - p1)
						continue;

					for (int p3 = 0; p3 <= m - i1 - p1 - p2; p3++)
					{
						if (p3 > k2 - p1)
							continue;

						for (int p5 = 0; p5 <= i1; p5++)
						{
							if (p5 > k1 - p1 - p2)
								continue;
							if (p5 > k2 - p1 - p3)
								continue;

							for (int p6 = 0; p6 <= i1 - p5; p6++)
							{
								if (p6 > k1 - p1 - p2 - p5)
									continue;

								for (int p7 = 0; p7 <= i1 - p5 - p6; p7++)
								{
									if (p7 > k2 - p1 - p3 - p5)
										continue;

									for (int p9 = 0; p9 <= n - j1; p9++)
									{
										if (p9 > k1 - p1 - p2 - p5 - p6)
											continue;

										if (p9 > k2 - p1 - p3 - p5 - p7)
											continue;

										for (int p10 = 0; p10 <= n - j1 - p9; p10++)
										{
											if (p10 > k1 - p1 - p2 - p5 - p6 - p9)
												continue;

											for (int p11 = 0; p11 <= n - j1 - p9 - p10; p11++)
											{
												if (p11 > k2 - p1 - p3 - p5 - p7 - p9)
													continue;

												for (int p13 = 0; p13 <= j1; p13++)
												{
													if (p13 > k1 - p1 - p2 - p5 - p6 - p9 - p10)
														continue;
													if (p13 > k2 - p1 - p3 - p5 - p7 - p9 - p11)
														continue;
													if (p13 < k1 + k2 - j1 - 2 * p1 - p2 - p3 - 2 * p5 - p6 - p7 - 2 * p9 - p10 - p11)
														continue;

													double kk1 = Factorial(m, i1) * Factorial(n, j1) * Factorial(m - i1, p1) * Factorial(m - i1 - p1, p2) *
														Factorial(m - i1 - p1 - p2, p3) * Factorial(i1, p5) * Factorial(i1 - p5, p6) * Factorial(i1 - p5 - p6, p7) *
														Factorial(n - j1, p9) * Factorial(n - j1 - p9, p10) * Factorial(n - j1 - p9 - p10, p11) *
														Factorial(j1, p13);

													double k = kk1 / kk;

													/*if(flag)
														TRACE("   +   ");
													if(int(kk1) != 1)
														TRACE("%d",int(kk1));
													if(p1 > 0)
														if(p1 > 1)
															TRACE("(f11-e11)^%d",p1);
														else
															TRACE("(f11-e11)");
													if(p2 > 0)
														if(p2 > 1)
															TRACE("(f10-e10)^%d",p2);
														else
															TRACE("(f10-e10)");
													if(p3 > 0)
														if(p3 > 1)
															TRACE("(f01-e01)^%d",p3);
														else
															TRACE("(f01-e01)");
													if(m-i1-p1-p2-p3 > 0)
														if(m-i1-p1-p2-p3 > 1)
															TRACE("(f00-e00)^%d",m-i1-p1-p2-p3);
														else
															TRACE("(f00-e00)");
													if(p5 > 0)
														if(p5 > 1)
															TRACE("e11^%d",p5);
														else
															TRACE("e11");
													if(p6 > 0)
														if(p6 > 1)
															TRACE("e10^%d",p6);
														else
															TRACE("e10");
													if(p7 > 0)
														if(p7 > 1)
															TRACE("e01^%d",p7);
														else
															TRACE("e01");
													if(i1 - p5 - p6 - p7 > 0)
														if(i1 - p5 - p6 - p7 > 1)
															TRACE("e00^%d",i1 - p5 - p6 - p7);
														else
															TRACE("e00");
													if(p9 > 0)
														if(p9 > 1)
															TRACE("(h11-g11)^%d",p9);
														else
															TRACE("(h11-g11)");
													if(p10 > 0)
														if(p10 > 1)
															TRACE("(h10-g10)^%d",p10);
														else
															TRACE("(h10-g10)");
													if(p11 > 0)
														if(p11 > 1)
															TRACE("(h01-g01)^%d",p11);
														else
															TRACE("(h01-g01)");
													if(n - j1 - p9 - p10 - p11 > 0)
														if(n - j1 - p9 - p10 - p11 > 1)
															TRACE("(h00-g00)^%d",n-j1-p9-p10-p11);
														else
															TRACE("(h00-g00)");
													if(p13 > 0)
														if(p13 > 1)
															TRACE("g11^%d",p13);
														else
															TRACE("g11");
													if(k1- p1 - p2 - p5 - p6 - p9 - p10 - p13 > 0)
														if(k1- p1 - p2 - p5 - p6 - p9 - p10 - p13 > 1)
															TRACE("g10^%d",k1- p1 - p2 - p5 - p6 - p9 - p10 - p13);
														else
															TRACE("g10");
													if(k2 - p1 - p3 - p5 - p7 - p9 - p11 - p13 > 0)
														if(k2 - p1 - p3 - p5 - p7 - p9 - p11 - p13 > 1)
															TRACE("g01^%d",k2 - p1 - p3 - p5 - p7 - p9 - p11 - p13);
														else
															TRACE("g01");
													if(j1 - p13 - k1 - k2 + p1 * 2 + p2 + p3 + p5 * 2 + p6 + p7 + p9 * 2 + p10 + p11 + p13 * 2  > 0)
														if(j1 - p13 - k1 - k2 + p1 * 2 + p2 + p3 + p5 * 2 + p6 + p7 + p9 * 2 + p10 + p11 + p13 * 2  > 1)
															TRACE("g00^%d",j1 - p13 - k1 - k2 + p1 * 2 + p2 + p3 + p5 * 2 + p6 + p7 + p9 * 2 + p10 + p11 + p13 * 2 );
														else
															TRACE("g00");*/

															//kk1 kk p1 p2 p3 m
													TRACE("\n %d %d %d %d %d %d  %d %d %d %d  %d %d %d %d  %d %d %d %d", int(kk1), int(kk), p1, p2, p3, m - i1 - p1 - p2 - p3, p5, p6, p7, i1 - p5 - p6 - p7, p9, p10, p11, n - j1 - p9 - p10 - p11, p13,
														k1 - p1 - p2 - p5 - p6 - p9 - p10 - p13, k2 - p1 - p3 - p5 - p7 - p9 - p11 - p13,
														j1 - p13 - k1 - k2 + p1 * 2 + p2 + p3 + p5 * 2 + p6 + p7 + p9 * 2 + p10 + p11 + p13 * 2);
													flag = 1;
												}
											}
										}
									}
								}
							}
						}
					}

				}
			}
			TRACE(")/%d P%d,%d \n", int(kk), i1, j1);
		}


}


void CFormView3::GenerateBilinearTransformation()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num != 1)
		return;

	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pNurbsSurf = pNurbs->Converte_C_NurbsSurface();

	double a1 = 0.3;
	double a2 = 0.7;
	double b1 = 0.2;
	double b2 = 0.8;

	double e00 = 0;
	double e01 = 0;
	double e10 = 1 - a2;
	double e11 = 1 - a1;
	double g00 = 0;
	double g01 = 0;
	double g10 = 1 - b2;
	double g11 = 1 - b1;
	double f00 = a2;
	double f01 = a1;
	double f10 = 1 - a2;
	double f11 = 1 - a1;
	double h00 = b2;
	double h01 = b1;
	double h10 = 1 - b2;
	double h11 = 1 - b1;

	C_NurbsSurf* pNewSurface_ = pNurbsSurf->GetGeneralBilinearTransformedSurface(
		e00, e01, e10, e11, f00, f01, f10, f11,
		g00, g01, g10, g11, h00, h01, h10, h11, 0, 1, 0, 1);

	CNurbsSuface* pNewNSurface = pNewSurface_->GetCNurbsSurface();
	pNewNSurface->SetTransform(*pNurbs->GetTransform());
	pDoc->m_SceneGraph.Add(pNewNSurface);

	C_NurbsSurf* pNewSurface_1 = pNurbsSurf->GetBilinearTransformedSurface(a1, a2, b1, b2);
	CNurbsSuface* pNewNSurface1 = pNewSurface_1->GetCNurbsSurface();
	pNewNSurface1->SetTransform(*pNurbs->GetTransform());
	pDoc->m_SceneGraph.Add(pNewNSurface1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);

}
void CFormView3::ComputeBoundaryRatio()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num != 1)
		return;

	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pNurbsSurf = pNurbs->Converte_C_NurbsSurface();

	double len1 = 0;
	for (int i = 0; i < 10000; i++)
	{
		double s1 = i / 10000.0;
		double s2 = (i + 1) / 10000.0;

		Point3D p1 = pNurbsSurf->ComputPosition(s1, 0);
		Point3D p2 = pNurbsSurf->ComputPosition(s2, 0);
		len1 += Length(p1 - p2);
	}

	double len2 = 0;
	for (int i = 0; i < 10000; i++)
	{
		double s1 = i / 10000.0;
		double s2 = (i + 1) / 10000.0;

		Point3D p1 = pNurbsSurf->ComputPosition(0, s1);
		Point3D p2 = pNurbsSurf->ComputPosition(0, s2);
		len2 += Length(p1 - p2);
	}
	double ratio = len1 / len2;

	for (int i = 0; i < pNurbsSurf->GetCtrlNumberU() + pNurbsSurf->GetOrderU(); i++)
	{
		double uknot = pNurbsSurf->GetUKnot(i);
		TRACE(" %f ", uknot * ratio);
	}
}
void CFormView3::OnBnClickedButton3()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();
	if (num < 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();
	//pSurf->InsertMultiUKnot(1,0.90625);
	//Point3D p = pSurf->ComputPosition(0.90625,0);
	//int index;
	//for(int i = 0; i < pSurf->GetCtrlNumberU(); i++)
	//{
	//	if(Length(p - pSurf->GetControlPoint(i,0)) < 1e-5)
	//	{
	//		index = i;
	//	}
	//}
	//CNurbsSuface* pSurf1 = pSurf->GetCNurbsSurface();
	//pSurf1->SetTransform(*pNSurf->GetTransform());
	//pDoc->m_SceneGraph.Add(pSurf1);

	//GenerateBilinearTransformation();
	//return;

	//GenerateOptimalGeneralBilinearTransformation();
	//return;

	//SolveLinearSystem();
	//return ;
	//GeneralBilinearTransformationDlg dlg;
	//dlg.DoModal();
	//return;
	//OnGeneralBilinearTransformation();
	//return;
	//pDoc->OnGenerateNewSurface();
	//pDoc->UpdateTreeControl();
	//return;

	//int num=pDoc->m_SceneGraph.NbObject();
	//if(num < 1)
	//	return;

	//CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	//C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();
	/*C_NurbsSurf* pTemp = pSurf->GetGeneralBilinearTransformedSurface(1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1);;
	CNurbsSuface* pTempNurbs = pTemp->GetCNurbsSurface();
	pTempNurbs->SetTransform(*pNSurf->GetTransform());
	pDoc->m_SceneGraph.Add(pTempNurbs);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	return;*/

	//初始化全局变量
	pOriginalOptimizationSurf = pSurf;
	pCurOptimizationSurf = (C_NurbsSurf*)pSurf->Copy();

	// 进行优化
	if (pSurf->GetDegreeU() == 1 && pSurf->GetDegreeV() == 1)
		OptimizationTest11(pSurf);
	else if (pSurf->GetDegreeU() == 2 && pSurf->GetDegreeV() == 2)
		OptimizationTest22(pSurf);
	else if (pSurf->GetDegreeU() == 3 && pSurf->GetDegreeV() == 3)
		OptimizationTestBSpline33(pSurf);

	//初始化uv的控制顶点个数
	uConNum = pSurf->GetCtrlNumberU();
	vConNum = pSurf->GetCtrlNumberV();

	return;
	/*
	Compute_Optimal_AreaPreserving_IsoParameterCurves_NumericalMethod(pSurf);
	SetVaseModification();
	Compute_Optimal_Mobiustransformation_NumericalMethod_FaceSurface(pNSurf);
	*/
}

void CFormView3::OnBnClickedButton3_()
{
	//计算三种变换的能量最小值，并显示变换后的等参线
	//1 计算mobius变换,求得能量最小值
	//根据能量最小值是的a,b取值分别显示等参线
	//打开一个同样的曲面
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	pNurbs->m_showOptimalIsoParameterCurves_demo11 = !pNurbs->m_showOptimalIsoParameterCurves_demo11;
	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnBnClickedButton4()
{
	//计算三种变换的能量最小值，并显示变换后的等参线
	//2 计算mobius+bilinear变换,求得能量最小值
	//根据能量最小值是的a,b取值分别显示等参线
	//打开一个同样的曲面
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	pNurbs->m_showOptimalIsoParameterCurves_demo12 = !pNurbs->m_showOptimalIsoParameterCurves_demo12;
	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnBnClickedButton5()
{
	//计算三种变换的能量最小值，并显示变换后的等参线
	//3 计算mobius+cubic变换,求得能量最小值
	//根据能量最小值是的a,b取值分别显示等参线
	//打开一个同样的曲面
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	pNurbs->m_showOptimalIsoParameterCurves_demo13 = !pNurbs->m_showOptimalIsoParameterCurves_demo13;
	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OutPutViewPointConfigFile(CObject3d* pSurface, char* filename)
{
	std::ofstream file(filename);

	//m_SetCameraMode = 1;

	GLdouble* modelview = pSurface->modelview;
	TRACE("%f %f %f %f\n", modelview[0], modelview[4], modelview[8], modelview[12]);
	TRACE("%f %f %f %f\n", modelview[1], modelview[5], modelview[9], modelview[13]);
	TRACE("%f %f %f %f\n", modelview[2], modelview[6], modelview[10], modelview[14]);
	TRACE("%f %f %f %f\n", modelview[3], modelview[7], modelview[11], modelview[15]);

	double eyex = -(modelview[12] * modelview[0] + modelview[13] * modelview[1] + modelview[14] * modelview[2]);
	double eyey = -(modelview[12] * modelview[4] + modelview[13] * modelview[5] + modelview[14] * modelview[6]);
	double eyez = -(modelview[12] * modelview[8] + modelview[13] * modelview[9] + modelview[14] * modelview[10]);

	double centerx = eyex - modelview[2];
	double centery = eyey - modelview[6];
	double centerz = eyez - modelview[10];

	double upx = modelview[1];
	double upy = modelview[5];
	double upz = modelview[9];

	/*pTreeView->g_eye[0] = eyex;
	pTreeView->g_eye[1] = eyey;
	pTreeView->g_eye[2] = eyez;

	pTreeView->g_look[0] = centerx;
	pTreeView->g_look[1] = centery;
	pTreeView->g_look[2] = centerz;

	pTreeView->g_dir[0] = upx;
	pTreeView->g_dir[1] = upy;
	pTreeView->g_dir[2] = upz;*/

	file << "\n/* scene viewpoint */ \n";
	file << "\n";
	file << "camera \n";
	file << "{\n";
	file << " perspective\n";
	file << " angle 25\n";
	file << " location <" << eyex << ", " << eyey << ", " << -eyez << ">\n";
	file << " sky <" << upx << ", " << upy << ", " << -upz << ">\n";
	file << " look_at <" << centerx << ", " << centery << ", " << -centerz << ">\n";
	file << " right x*image_width/image_height\n";
	file << "}\n";
	file << "background\n";
	file << "{\n";
	file << " color <1.0, 1.0, 1.0>\n";
	file << "}\n";
	file.close();
}

void CFormView3::OutPutLightConfigFile(CObject3d* pSurface, char* filename)
{
	std::ofstream file(filename);

	file << "/* scene light */\n";
	file << "light_source\n";
	file << "{\n";
	file << "  <0,0,-150>\n";
	file << "color rgb<1.0, 1.0, 1.0>\n";
	file << "}\n";
	file.close();
}
void CFormView3::ExtractSurface_Sub()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();
	if (num < 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();
	pSurf->InsertMultiUKnot(1, 0.1875);

	TRACE("10 10 \n");
	TRACE("3 3 \n");
	for (int i = 0; i < 14; i++)
		TRACE(" %f ", pSurf->GetUKnot(i));
	TRACE("\n");
	for (int i = 0; i < 14; i++)
		TRACE(" %f ", pSurf->GetVKnot(i));
	TRACE("\n");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Point3D point = pSurf->GetControlPoint(i, j);
			TRACE("%f %f %f 1 \n", point.x, point.y, point.z);
		}
	}
}
void CFormView3::ExtractSurface()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();
	if (num < 1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();
	pSurf->InsertMultiUKnot(1, 0.90625);
	Point3D p = pSurf->ComputPosition(0.90625, 0);
	int index;
	for (int i = 0; i < pSurf->GetCtrlNumberU(); i++)
	{
		if (Length(p - pSurf->GetControlPoint(i, 0)) < 1e-5)
		{
			index = i;
		}
	}
	CNurbsSuface* pSurf1 = pSurf->GetCNurbsSurface();
	pSurf1->SetTransform(*pNSurf->GetTransform());
	pDoc->m_SceneGraph.Add(pSurf1);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);

}
void CFormView3::GenerateNewSurface(CNurbsSuface* pSurface)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int leftnum = 4;
	int rightnum = 2;
	int topnum = 5;
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	double uknot1 = pNurbsSurface->GetUKnot(leftnum + 3);
	double uknot2 = pNurbsSurface->GetUKnot(pSurface->m_uknotcount - rightnum - 4);
	double vknot1 = pNurbsSurface->GetVKnot(pSurface->m_vknotcount - topnum - 4);
	pNurbsSurface->InsertMultiUKnot(3, uknot1);
	pNurbsSurface->InsertMultiUKnot(3, uknot2);
	pNurbsSurface->InsertMultiVKnot(3, vknot1);

	int uknotcount = pSurface->m_uknotcount - (leftnum + rightnum);
	int vknotcount = pSurface->m_vknotcount - (topnum);
	C_NurbsSurf* pNewSurface = new C_NurbsSurf(uknotcount - 4, vknotcount - 4, 4, 4);
	//Set uknot
	for (int i = 0; i < uknotcount; i++)
	{
		pNewSurface->SetUKnot(i, pNurbsSurface->GetUKnot(i + 3 + leftnum));
	}
	for (int i = 0; i < vknotcount; i++)
	{
		pNewSurface->SetVKnot(i, pNurbsSurface->GetVKnot(i));
	}

	//Set control point
	for (int i = 0; i < uknotcount - 4; i++)
		for (int j = 0; j < vknotcount - 4; j++)
		{
			pNewSurface->SetControlPoint(i, j, pNurbsSurface->GetControlPoint(i + 3 + leftnum, j));
			pNewSurface->SetWeight(i, j, 1);
		}
	CNurbsSuface* pNSurf = pNewSurface->GetCNurbsSurface();
	pNSurf->SetTransform(*pSurface->GetTransform());
	pDoc->m_SceneGraph.Add(pNSurf);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::SoomthFreeformsSurface(CNurbsSuface* pSurface)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();

	int index = pSurf->GetCtrlNumberV() - 1;
	for (int i = 0; i < pSurf->GetCtrlNumberU(); i++)
	{
		Vector3D v1 = pSurf->GetControlPoint(i, 0) - ZeroP3D;
		Vector3D v2 = pSurf->GetControlPoint(i, index) - ZeroP3D;

		Vector3D v3 = pSurf->GetControlPoint(i, 1) - ZeroP3D;
		Vector3D v4 = pSurf->GetControlPoint(i, index - 1) - ZeroP3D;
		Vector3D v5 = (v3 + v4) / 2;
		Point3D pp;
		pp.x = v5.x;
		pp.y = v5.y;
		pp.z = v5.z;
		pSurf->SetControlPoint(i, 0, pp);
		pSurf->SetControlPoint(i, index, pp);
	}
	CNurbsSuface* pNnewsurf = pSurf->GetCNurbsSurface();
	pNnewsurf->SetTransform(*pNSurf->GetTransform());

	pDoc->m_SceneGraph.Add(pNnewsurf);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::TrimmingFreeformSurface()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();
	int numu = 31;
	int numv = 32;
	TRACE("31 32 \n");
	TRACE("3  3 \n");
	TRACE("0 0 0 0");
	for (int i = 1; i <= 27; i++)
		TRACE(" %f ", i / 28.0);
	TRACE("1 1 1 1 \n");

	TRACE("0 0 0 0");
	for (int i = 1; i <= 28; i++)
		TRACE(" %f ", i / 29.0);
	TRACE("1 1 1 1 \n");

	for (int i = 0; i < 31; i++)
	{
		Point3D p1 = pSurf->GetControlPoint(i, 30);
		Point3D p2 = pSurf->GetControlPoint(i, 32);
		Point3D p3 = (p1 + p2) / 2;
		TRACE("%f %f %f 1 \n", p3.x, p3.y, p3.z);
		for (int j = 1; j < 31; j++)
		{
			Point3D p = pSurf->GetControlPoint(i, j);
			TRACE("%f %f %f 1 \n", p.x, p.y, p.z);
		}

		TRACE("%f %f %f 1 \n", p3.x, p3.y, p3.z);
	}
}
void CFormView3::ExtendSurface(CNurbsSuface* pNSurf)
{
	for (int i = 0; i < 4; i++)
	{
		Vector3D p0 = pNSurf->GetControlPoint(i, 0);
		Vector3D p1 = pNSurf->GetControlPoint(i, 1);
		Vector3D p2 = pNSurf->GetControlPoint(i, 2);
		Vector3D p3 = pNSurf->GetControlPoint(i, 3);


		//Vector3D q0 = p0;
		//Vector3D q1 = (p0 + p1) / 2;
		//Vector3D q2 = (p0 + p1 * 2 + p2) / 4;
		//Vector3D q3 = (p0 + p1 * 3 + p2 * 3 + p3) / 8;
		Vector3D q0 = p0;
		Vector3D q1 = p1 * 2 - p0;
		Vector3D q2 = p0 - p1 * 4 + p2 * 4;
		Vector3D q3 = -p0 + p1 * 6 - p2 * 12 + p3 * 8;

		pNSurf->SetControlPoint(i, 0, q0);
		pNSurf->SetControlPoint(i, 1, q1);
		pNSurf->SetControlPoint(i, 2, q2);
		pNSurf->SetControlPoint(i, 3, q3);
	}
}
void CFormView3::SetBunnyGeneralTransformation1()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	CNurbsSuface* pNurbsSurface;

	for (int ii = 0; ii < num; ii++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
			BilinearTransformation* generaltrans = new BilinearTransformation();
			generaltrans->m_Bilinear_Alpha1 = 0.3;
			generaltrans->m_Bilinear_Alpha2 = 0.6;
			generaltrans->m_Bilinear_Beta1 = 0.7;
			generaltrans->m_Bilinear_Beta2 = 0.6;

			pNurbsSurface->ptransformation = generaltrans;
			//generaltrans->UpdateBoundingBox();
		}
	}
}
void CFormView3::SetBunnyGeneralTransformation()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	CNurbsSuface* pNurbsSurface;

	for (int ii = 0; ii < num; ii++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
			GeneralBilinearTransformation* generaltrans = new GeneralBilinearTransformation();
			generaltrans->umin = 0;
			generaltrans->umax = 1;
			generaltrans->vmin = 0;
			generaltrans->vmax = 1;
			generaltrans->umin_Reparameterization = -0.26039901375770569;
			generaltrans->umax_Reparameterization = 0.34060800075531006;
			generaltrans->vmin_Reparameterization = -0.37038001418113686;
			generaltrans->vmax_Reparameterization = 1.1398600339889526;

			generaltrans->m_ReparameterizationType = generalbilinear;

			generaltrans->e00 = 0.12255526498437488;
			generaltrans->e01 = 1.1239473926876469;
			generaltrans->e10 = -0.88874107087198551;
			generaltrans->e11 = 0.31987433267009380;
			generaltrans->f00 = 1;
			generaltrans->f01 = 1;
			generaltrans->f10 = 1;
			generaltrans->f11 = 1;
			generaltrans->g00 = 0.55639158926720100;
			generaltrans->g01 = 0.55053449283471723;
			generaltrans->g10 = -1.2991611407558747;
			generaltrans->g11 = -1.0930369065399548;
			generaltrans->h00 = 1;
			generaltrans->h01 = 1;
			generaltrans->h10 = 1;
			generaltrans->h11 = 1;
			generaltrans->label = -1;

			pNurbsSurface->ptransformation = generaltrans;
			//generaltrans->UpdateBoundingBox();
		}
	}
}
struct comp
{
	comp(int const& s) : _s(s) { }

	bool operator () (std::pair<int, int> const& p)
	{
		return (p.first == _s);
	}

	int _s;
};
void CFormView3::ExportPolyomino(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	std::pair<int, int> pair;
	/*for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		for(int j = 0; j < pLandmark->landmark_points.size() - 1; j++)
		{
			int index1 = pLandmark->landmark_points[j];
			int index2 = pLandmark->landmark_points[j + 1];
			CVertex3d* pStartVertex = pMesh->GetVertex(index1);
			CVertex3d* pEndVertex = pMesh->GetVertex(index2);

			double x1 = pStartVertex->x();
			double y1 = pStartVertex->y();
			double z1 = pStartVertex->z();

			double x2 = pEndVertex->x();
			double y2 = pEndVertex->y();
			double z2 = pEndVertex->z();

			if(pLandmark->style == 0)
			{
				//Horizontal
				outfile << "cylinder {<" << x1 << " , " << y1 << " , " << -z1 << " >,<" << x2 << " , " << y2 << " , " << -z2 << " > ,r_hedge texture { hedge_tex"<<"} }" << endl;
			}
			else
			{
				//Vertical
				outfile << "cylinder {<" << x1 << " , " << y1 << " , " << -z1 << " >,<" << x2 << " , " << y2 << " , " << -z2 << " > ,r_vedge texture { vedge_tex"<<"} }" << endl;
			}
		}

	}*/
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		int startindex = pair.first;
		int endindex = pair.second;
		CVertex3d* pStartVertex = pMesh->GetVertex(startindex);
		CVertex3d* pEndVertex = pMesh->GetVertex(endindex);

		double x1 = pStartVertex->x();
		double y1 = pStartVertex->y();
		double z1 = pStartVertex->z();

		double x2 = pEndVertex->x();
		double y2 = pEndVertex->y();
		double z2 = pEndVertex->z();

		double dis = Distance(pStartVertex, pEndVertex);
		if (dis < 1e-6)
		{
			continue;
		}
		if (fabs(y1 - y2) < 1e-6)
		{
			//Horizontal
			outfile << "cylinder {<" << x1 << " , " << y1 << " , " << -z1 << " >,<" << x2 << " , " << y2 << " , " << -z2 << " > ,r_hedge texture { hedge_tex" << "} }" << endl;
		}
		else
		{
			//Vertical 
			outfile << "cylinder {<" << x1 << " , " << y1 << " , " << -z1 << " >,<" << x2 << " , " << y2 << " , " << -z2 << " > ,r_vedge texture { vedge_tex" << "} }" << endl;
		}
	}
	for (int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();
		outfile << "sphere {<" << x << " , " << y << " , " << -z << " >,r_vertex texture { vertex_tex } }" << endl;
	}
	outfile.close();
}
void CFormView3::ExPortGraph_Line_SelectedEdges(CMesh3d* pMesh, char* filename)
{
	std::pair<int, int> pair;
	std::ofstream outfile(filename);
	//export the graph for wei zeng
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		double x1 = pMesh->GetVertex(pair.first)->x();
		double y1 = pMesh->GetVertex(pair.first)->y();
		double z1 = pMesh->GetVertex(pair.first)->z();
		double x2 = pMesh->GetVertex(pair.second)->x();
		double y2 = pMesh->GetVertex(pair.second)->y();
		double z2 = pMesh->GetVertex(pair.second)->z();

		outfile << "cylinder {<" << x1 << " , " << y1 << " , " << -z1 << " >,<" << x2 << " , " << y2 << " , " << -z2 << " > ,r_edge texture { edge_tex" << "} }" << endl;
	}
	outfile.close();
}
void CFormView3::ExPortGraph_Line()
{
	//export the graph for wei zeng
	char s[100];
	long index;

	std::ifstream file("D:\\papers\\2014\\cvpr\\pov\\texture\\2713_1\\graph_lines\\2377_1.graph_lines_3d.m");
	std::vector<std::pair<int, int>> m_ArrayPoints;
	std::vector<Vector3D> m_ArrayPointsLoc;
	Vector3D v;
	std::vector<std::vector<int>> m_ArrayFaces;
	std::vector<int> farray;
	file >> s >> index;
	strlwr(s);
	std::pair<int, int> p;
	int cindex = 0;
	while (!file.eof())
	{
		if (strcmp(s, "vertex") == 0)
		{
			p.first = index;
			p.second = cindex++;
			file >> v.x >> v.y >> v.z;
			m_ArrayPointsLoc.push_back(v);
			m_ArrayPoints.push_back(p);

			file >> s >> index;
			strlwr(s);
		}
		else
			if (strcmp(s, "line") == 0)
			{
				farray.clear();
				while (!file.eof())
				{
					file >> s;
					strlwr(s);
					if (strcmp(s, "line") == 0)
					{
						m_ArrayFaces.push_back(farray);
						farray.clear();
						file >> index;
						break;
					}
					int findex = atoi(s);


					std::vector<std::pair<int, int>>::iterator mii = std::find_if(m_ArrayPoints.begin(), m_ArrayPoints.end(), comp(findex));
					if (mii != m_ArrayPoints.end())
					{
						findex = (*mii).second;
						farray.push_back(findex);
					}

				}
				if (farray.size() != 0)
					m_ArrayFaces.push_back(farray);

			}
	}
	file.close();


	std::ofstream outfile("D:\\edges.inc");
	int colorindex = 0;
	for (int i = 0; i < m_ArrayFaces.size(); i++)
	{
		for (int j = 0; j < m_ArrayFaces[i].size(); j++)
		{
			int index0 = m_ArrayFaces[i][(j - 1 + m_ArrayFaces[i].size()) % m_ArrayFaces[i].size()];
			int index1 = m_ArrayFaces[i][j];
			int index2 = m_ArrayFaces[i][(j + 1) % m_ArrayFaces[i].size()];
			int index3 = m_ArrayFaces[i][(j + 2) % m_ArrayFaces[i].size()];

			Vector3D vv0 = m_ArrayPointsLoc[index0];
			Vector3D vv1 = m_ArrayPointsLoc[index1];
			Vector3D vv2 = m_ArrayPointsLoc[index2];
			Vector3D vv3 = m_ArrayPointsLoc[index3];

			//smooth it
			//Vector3D vv1s = vv0 * 0.3 + vv1 * 0.4 + vv2 * 0.3;
			//Vector3D vv2s = vv1 * 0.3 + vv2 * 0.4 + vv3 * 0.3;
			Vector3D vv1s = vv1;
			Vector3D vv2s = vv2;
			outfile << "cylinder {<" << vv1s[0] << " , " << vv1s[1] << " , " << -vv1s[2] << " >,<" << vv2s[0] << " , " << vv2s[1] << " , " << -vv2s[2] << " > ,r_edge texture { edge_tex" << "} }" << endl;
		}
		colorindex++;
		colorindex = colorindex % 10;
	}

	outfile.close();

	std::ofstream outfile2("D:\\vertices1.inc");
	/*for(int i = 0; i < m_ArrayFaces.size(); i++)
	{
		for(int j = 0; j < m_ArrayFaces[i].size(); j++)
		{
			int index1 = m_ArrayFaces[i][j];

			Vector3D vv1 = m_ArrayPointsLoc[index1];

			outfile2 << "sphere {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,r_vertex1 texture { vertex_tex1 } }" << endl;
			//outfile1 << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}*/
	for (int i = 64; i < m_ArrayPointsLoc.size(); i++)
	{
		Vector3D vv1 = m_ArrayPointsLoc[i];
		outfile2 << "sphere {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,r_vertex1 texture { vertex_tex1 } }" << endl;
	}
	outfile2.close();
	std::ofstream outfile1("D:\\vertices.inc");
	for (int i = 0; i < 64; i++)
	{
		Vector3D vv1 = m_ArrayPointsLoc[i];
		outfile1 << "sphere {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,r_vertex texture { vertex_tex } }" << endl;
	}

	outfile1.close();
}
void CFormView3::ExPortGraph()
{
	//export the graph for wei zeng
	char s[100];
	long index;

	std::ifstream file("D:\\papers\\2014\\cvpr\\pov\\texture\\2357_white_1.graph_3d.m");
	std::vector<std::pair<int, int>> m_ArrayPoints;
	std::vector<Vector3D> m_ArrayPointsLoc;
	Vector3D v;
	std::vector<std::vector<int>> m_ArrayFaces;
	std::vector<int> farray;
	file >> s >> index;
	strlwr(s);
	std::pair<int, int> p;
	int cindex = 0;
	while (!file.eof())
	{
		if (strcmp(s, "vertex") == 0)
		{
			p.first = index;
			p.second = cindex++;
			file >> v.x >> v.y >> v.z;
			m_ArrayPointsLoc.push_back(v);
			m_ArrayPoints.push_back(p);

			file >> s >> index;
			strlwr(s);
		}
		else
			if (strcmp(s, "face") == 0)
			{
				farray.clear();
				while (!file.eof())
				{
					file >> s;
					strlwr(s);
					if (strcmp(s, "face") == 0)
					{
						m_ArrayFaces.push_back(farray);
						farray.clear();
						file >> index;
						break;
					}
					int findex = atoi(s);


					std::vector<std::pair<int, int>>::iterator mii = std::find_if(m_ArrayPoints.begin(), m_ArrayPoints.end(), comp(findex));
					if (mii != m_ArrayPoints.end())
					{
						findex = (*mii).second;
						farray.push_back(findex);
					}

				}
				if (farray.size() != 0)
					m_ArrayFaces.push_back(farray);

			}
	}
	file.close();


	std::ofstream outfile("D:\\edges.inc");
	for (int i = 0; i < m_ArrayFaces.size(); i++)
	{
		for (int j = 0; j < m_ArrayFaces[i].size(); j++)
		{
			int index0 = m_ArrayFaces[i][(j - 1 + m_ArrayFaces[i].size()) % m_ArrayFaces[i].size()];
			int index1 = m_ArrayFaces[i][j];
			int index2 = m_ArrayFaces[i][(j + 1) % m_ArrayFaces[i].size()];
			int index3 = m_ArrayFaces[i][(j + 2) % m_ArrayFaces[i].size()];

			Vector3D vv0 = m_ArrayPointsLoc[index0];
			Vector3D vv1 = m_ArrayPointsLoc[index1];
			Vector3D vv2 = m_ArrayPointsLoc[index2];
			Vector3D vv3 = m_ArrayPointsLoc[index3];

			//smooth it
			//Vector3D vv1s = vv0 * 0.3 + vv1 * 0.4 + vv2 * 0.3;
			//Vector3D vv2s = vv1 * 0.3 + vv2 * 0.4 + vv3 * 0.3;
			Vector3D vv1s = vv1;
			Vector3D vv2s = vv2;
			outfile << "cylinder {<" << vv1s[0] << " , " << vv1s[1] << " , " << -vv1s[2] << " >,<" << vv2s[0] << " , " << vv2s[1] << " , " << -vv2s[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	outfile.close();

	std::ofstream outfile2("D:\\vertices1.inc");
	/*for(int i = 0; i < m_ArrayFaces.size(); i++)
	{
		for(int j = 0; j < m_ArrayFaces[i].size(); j++)
		{
			int index1 = m_ArrayFaces[i][j];

			Vector3D vv1 = m_ArrayPointsLoc[index1];

			outfile2 << "sphere {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,r_vertex1 texture { vertex_tex1 } }" << endl;
			//outfile1 << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}*/
	for (int i = 64; i < m_ArrayPointsLoc.size(); i++)
	{
		Vector3D vv1 = m_ArrayPointsLoc[i];
		outfile2 << "sphere {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,r_vertex1 texture { vertex_tex1 } }" << endl;
	}
	outfile2.close();
	std::ofstream outfile1("D:\\vertices.inc");
	for (int i = 0; i < 64; i++)
	{
		Vector3D vv1 = m_ArrayPointsLoc[i];
		outfile1 << "sphere {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,r_vertex texture { vertex_tex } }" << endl;
	}

	outfile1.close();
}
void CFormView3::OnSurfaceMesh3dNormalize()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph[currentindex];
	pMesh->Normalize();
	pDoc->UpdateAllViews(NULL);
}
void* my_calloc(size_t n, size_t size)
{
	void* ptr;
	if ((n <= 0) || (size <= 0))
	{
		return NULL;
	}
	else
	{
		ptr = calloc(n, size);
		return(ptr);
	}
}
void my_free(void* ptr)
{
	if (ptr == NULL)
	{
		return;
	}
	else
	{
		free(ptr);
	}
}
void* my_realloc(void* ptr, size_t size)
{
	if (size == 0)
	{
		free(ptr);
	}
	if (size <= 0)
	{
		return NULL;
	}
	if (ptr == NULL)
	{
		ptr = malloc(size);
	}
	else
	{
		ptr = realloc(ptr, size);
	}
	return(ptr);
}
void* my_malloc(size_t size)
{
	void* ptr;
	if (size <= 0)
		return NULL;
	else
	{
		ptr = malloc(size);
		return ptr;
	}
}
void CFormView3::MatlabTest()
{
	MySparseMatrix A;
	A.m = A.n = 2;
	A.AddItem(0, 0, 1);
	A.AddItem(0, 1, 2);
	A.AddItem(1, 0, 3);
	A.AddItem(1, 1, 5);
	std::vector<double> b, x;
	b.push_back(1);
	b.push_back(2);
	Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A, b, x);
	return;
	/*const int N=20;
	double x[N],y[N];
	for (int i=0;i<N; i++)
	{
		x[i]=i+1;
		y[i]=x[i]*x[i];

	}
	double x0[3];
	double A[2][3];
	x0[0] = x0[1] = x0[2] = 10;
	A[0][0] = -1;
	A[0][1] = -2;
	A[0][2] = -2;

	A[1][0] = 1;
	A[1][1] = 2;
	A[1][2] = 2;
	double b[2];
	b[0] = 0;
	b[1] = 72;


	Engine *ep;     //定义MATLAB引擎指针变量
	if(!(ep=engOpen(NULL))) //打开MATLAB引擎
	{
		cout<<"can't start MATLAB engine!"<<endl;
		exit(1);
	}

	mxArray *x0_ = mxCreateDoubleMatrix(1,3,mxREAL);
	memcpy(mxGetPr(x0_),x0,3*sizeof(double));
	engPutVariable(ep,"x0",x0_);
	mxArray *A_ = mxCreateDoubleMatrix(2,3,mxREAL);
	memcpy(mxGetPr(A_),A,6*sizeof(double));
	double* AA = mxGetPr(A_);

	mxArray *b_ = mxCreateDoubleMatrix(1,2,mxREAL);
	memcpy(mxGetPr(b_),b,2*sizeof(double));
	engPutVariable(ep,"b",b_);

	engEvalString(ep,"cd('C:\\Users\\Administrator\\Documents\\MATLAB')");
	//engEvalString(ep,"y = myfun(x0);");
	engEvalString(ep,"[x,fval]=fmincon(@myfun,x0,A,b)");
	mxArray *xx = engGetVariable(ep,"x");
	double *xval = mxGetPr(xx);
	TRACE("%f,%f,%f",xval[0],xval[1],xval[2]);*/
	//char *x;
	//mxArray* pa;
	//mxSetAllocFcns(my_calloc,my_free,my_realloc,my_malloc);
	//pa = mxCreateString("asdfasdf");
	//x = (char*) mxCalloc(255,sizeof(char));
	//mxGetString(pa,x,255);
	//mclInitializeApplication(NULL,0);
	//利用MATLAB API mxCreateDoubleMatrix函数生成矩阵，即申请空间，MATLAB引擎中使用mxArray类型数据

	//mxArray *xx = mxCreateDoubleMatrix(1,N,mxREAL);//1行N列，mxREAL为实双精度矩阵
	//mxArray *yy = mxCreateDoubleMatrix(1,N,mxREAL);
	//mxGetPr获取指向输入、输出矩阵数据的指针
	//memcpy(mxGetPr(xx),x,N*sizeof(double));//将数组 x 复制到 mxarray 数组 xx 中，即给xx数组赋值
	//memcpy(mxGetPr(yy),y,N*sizeof(double));

	//engPutVariable(ep,"xx",xx);//将 mxArray 数组 xx 写入到 Matlab 工作空间，命名为 xx 
	//engPutVariable(ep,"yy",yy);

	//engEvalString(ep,"plot(xx,yy)");//通过引擎调用MATLAB中plot(x,y)函数，绘制函数曲线
	//mxDestroyArray(xx); //释放内存
	//mxDestroyArray(yy);//释放内存

	//cout<<"press any key to exit!"<<endl;
	//cin.get();
	//engClose(ep);
	//engEvalString(ep,"rand('seed',0);x=rand(100,1)*4-2;y=rand(100,1)*4-2;");       //如果不用“\”连接下一行，编译会报错。  
	//engEvalString(ep,"z=100*(y.^3-x.^2);");
	//engEvalString(ep,"ti=0:0.05:2;");
	//engEvalString(ep,"[XI,YI]=meshgrid(ti,ti);");
	//engEvalString(ep,"ZI=griddata(x,y,z,XI,YI);");
	//engEvalString(ep,"surf(XI,YI,ZI);title('数据插值三维曲面图');xlabel('x');ylabel('y');zlabel('z');grid on");
	//MessageBox(NULL,"插值计算完毕","数据插值",MB_OK);
	return;
}
void CFormView3::OutPutPovrayPuzzle(CMesh3d* pMesh)
{
	std::ofstream outfile("D:\\puzzle.inc");

	int nstatesize = pMesh->m_ArrayStates.size();
	std::vector<std::vector<int>> m_ArrayConnectionMatrix;
	m_ArrayConnectionMatrix.resize(pMesh->m_ArrayStates.size());
	for (int i = 0; i < nstatesize; i++)
		m_ArrayConnectionMatrix[i].resize(nstatesize);

	//Initialization
	for (int i = 0; i < nstatesize; i++)
		for (int j = 0; j < nstatesize; j++)
		{
			m_ArrayConnectionMatrix[i][j] = 0;
		}

	for (int i = 0; i < nstatesize; i++)
		for (int j = 0; j < nstatesize; j++)
		{
			if (i == j)
				continue;

			if (m_ArrayConnectionMatrix[i][j])
				continue;

			if (pMesh->IsStateAdjacent(i, j))
			{
				m_ArrayConnectionMatrix[i][j] = 1;
				m_ArrayConnectionMatrix[j][i] = 1;
			}
		}
	std::vector<std::vector<double>> m_Color3ElementsArray;
	const int Azure = 0; //tianlan
	const int Red = 1;
	const int Yellow = 2;
	const int Orange = 3;
	const int Rose_Carmine = 4; //meihong
	std::vector<int> m_ColorArray;
	m_ColorArray.push_back(Azure); //0x3299CC
	m_ColorArray.push_back(Red);  //0x ff0000
	m_ColorArray.push_back(Yellow); //0xFFFF00
	m_ColorArray.push_back(Orange); //
	m_ColorArray.push_back(Rose_Carmine); //

	m_Color3ElementsArray.resize(m_ColorArray.size());

	m_Color3ElementsArray[0].push_back(78 / 255.0);
	m_Color3ElementsArray[0].push_back(204 / 255.0);
	m_Color3ElementsArray[0].push_back(236 / 255.0);

	m_Color3ElementsArray[1].push_back(255 / 255.0);
	m_Color3ElementsArray[1].push_back(0 / 255.0);
	m_Color3ElementsArray[1].push_back(0 / 255.0);

	m_Color3ElementsArray[2].push_back(255 / 255.0);
	m_Color3ElementsArray[2].push_back(255 / 255.0);
	m_Color3ElementsArray[2].push_back(0 / 255.0);

	m_Color3ElementsArray[3].push_back(255 / 255.0);
	m_Color3ElementsArray[3].push_back(96 / 255.0);
	m_Color3ElementsArray[3].push_back(0 / 255.0);

	m_Color3ElementsArray[4].push_back(232 / 255.0);
	m_Color3ElementsArray[4].push_back(29 / 255.0);
	m_Color3ElementsArray[4].push_back(123 / 255.0);

	std::vector<int> m_StateColoring;
	m_StateColoring.resize(nstatesize);

	for (int i = 0; i < nstatesize; i++)
		m_StateColoring[i] = -1;

	//coloring
	int k;
	for (int i = 0; i < nstatesize; i++)
	{
		for (int j = 0; j < m_ColorArray.size(); j++)
		{
			for (k = 0; k < nstatesize; k++)
			{
				if (m_ArrayConnectionMatrix[i][k] && m_StateColoring[k] == j)
					break;
			}
			if (k == nstatesize)
			{
				m_StateColoring[i] = m_ColorArray[j];
				break;
			}
		}
	}
	//int nstatesize = pMesh->m_ArrayStates.size();
	double r, g, b;
	for (int i = 0; i < nstatesize; i++)
	{
		double x = 0;
		double y = 0;
		double z = 0;
		int num = 0;
		int statecells;
		//color
		r = m_Color3ElementsArray[m_StateColoring[i]][0];
		g = m_Color3ElementsArray[m_StateColoring[i]][1];
		b = m_Color3ElementsArray[m_StateColoring[i]][2];
		double x1, y1, z1, x2, y2, z2;
		for (int j = 0; j < pMesh->m_ArrayStates[i].size(); j++)
		{
			std::vector<std::pair<int, int>> cells;
			cells = pMesh->m_ArrayUniformCells[pMesh->m_ArrayStates[i][j]];
			std::pair<int, int> pair;
			for (int k = 0; k < cells.size(); k++)
			{
				pair = cells[k];
				x1 = pMesh->xinterval[0] + pMesh->baseinterval * pair.first + pMesh->baseinterval * 0.1;
				y1 = pMesh->yinterval[0] + pMesh->baseinterval * pair.second + pMesh->baseinterval * 0.1;
				z1 = 0;

				x2 = pMesh->xinterval[0] + pMesh->baseinterval * pair.first + pMesh->baseinterval * 0.9;
				y2 = pMesh->yinterval[0] + pMesh->baseinterval * pair.second + pMesh->baseinterval * 0.9;
				z2 = z1 + pMesh->baseinterval;

				outfile << "object{Round_Box(<" << x1 << " , " << y1 << " , " << -z1 << " >,<" << x2 << " , " << y2 << " , " << -z2 << " > ,0.25,0) texture { hedge_tex" << m_StateColoring[i] << "} }" << endl;
			}
		}
	}
}
void CFormView3::ExportConstraints()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	std::ofstream outfile("D:\\constraints.txt");
	if (num != 2)
		return;
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	//CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);

	for (int i = 0; i < pMesh1->NbVertex(); i++)
	{
		pMesh1->GetVertex(i)->SetFlag(0);
	}
	for (int i = 0; i < pMesh1->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh1->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		if (pVertex->GetFlag())
			continue;
		if (fabs(y) < 1e-5)
		{
			outfile << i << " " << x << " " << y << "\n";
			pVertex->SetFlag(1);
		}
		else
			if (fabs(x) < 1e-5)
			{
				outfile << i << " " << x << " " << y << "\n";
				pVertex->SetFlag(1);
			}
			else
				if (fabs(x - 1) < 1e-5)
				{
					outfile << i << " " << x << " " << y << "\n";
					pVertex->SetFlag(1);
				}
				else
					if (fabs(y - 0.656463) < 1e-5)
					{
						outfile << i << " " << x << " " << y << "\n";
						pVertex->SetFlag(1);
					}
	}
	for (int i = 0; i < pMesh1->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh1->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size(); j++)
		{
			int index = pLandmark->landmark_points[j];
			//if(index)
			//CVertex3d* pVertex2 = pMesh2->GetVertex(index);
			CVertex3d* pVertex1 = pMesh1->GetVertex(index);
			if (pVertex1->GetFlag())
				continue;
			outfile << index << " " << pVertex1->x() << " " << pVertex1->y() << "\n";
			pVertex1->SetFlag(1);
		}
	}
	outfile << -1;
	outfile.close();
	//for()
}
void CFormView3::AssignLandmark()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num != 2)
		return;
	if (pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
		CMesh3d_OpenMesh* pMesh2 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(1);
		pMesh2->m_ArraySharpEdges = pMesh1->m_ArraySharpEdges;
		pMesh2->m_LandMarkArray = pMesh1->m_LandMarkArray;
	}
	else if (pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh1 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(0);
		CMesh3d_OpenMesh* pMesh2 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(1);
		pMesh2->m_ArraySharpEdges = pMesh1->m_ArraySharpEdges;
		pMesh2->m_LandMarkArray = pMesh1->m_LandMarkArray;
	}
}
void CFormView3::OnCDT_PSLG()
{
	std::ofstream outfile("D:\\papers\\code\\Triangle\\Geo.poly");
	int n = 20;
	int num = n * 4;
	outfile << num << " 2 0 1" << "\n";

	//Bottom
	int index = 0;
	for (int i = 0; i <= n; i++)
	{
		outfile << index++ << " " << (i + 0.0) / n << " " << 0 << " 0 " << "\n";
	}

	//Right
	for (int i = 1; i <= n; i++)
	{
		outfile << index++ << " " << 1 << " " << (i + 0.0) / n << " 0 " << "\n";
	}

	//Top
	for (int i = n - 1; i >= 0; i--)
	{
		outfile << index++ << " " << (i + 0.0) / n << " " << 1 << " 0 " << "\n";
	}

	//left
	for (int i = n - 1; i > 0; i--)
	{
		outfile << index++ << " " << 0 << " " << (i + 0.0) / n << " 0 " << "\n";
	}

	outfile << (n) * 4 << " 1 " << "\n";
	index = 0;
	for (int i = 0; i < (n) * 4; i++)
	{
		outfile << i << " " << i << " " << (i + 1) % num << " 1 \n";
	}

	outfile << " 0 " << "\n";
	outfile.close();

	char step1[200];
	SetCurrentDirectory("D:\\papers\\code\\triangle");
	//sprintf(step1,"triangle.exe -pq33.8 Geo");
	sprintf(step1, "triangle.exe -p Geo");
	system(step1);

	char resultmeshname[100];
	sprintf(resultmeshname, "D:\\papers\\code\\triangle\\Geo.1.ele");
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument(resultmeshname);
}
void CFormView3::ProjectTriangulationOnSurfaces()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pNURBS = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh3d5 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	CMesh3d* pMesh3d3copy = new CMesh3d();
	pMesh3d3copy->Copy(pMesh3d5);
	Point3D lower, upper;
	pMesh3d5->ComputeBoundingBox(lower, upper);
	double startu = pNURBS->m_uknot[0];
	double endu = pNURBS->m_uknot[pNURBS->m_uknotcount - 1];
	double startv = pNURBS->m_vknot[0];
	double endv = pNURBS->m_vknot[pNURBS->m_vknotcount - 1];
	CVertex3d* pVertex;
	for (int i = 0; i < pMesh3d3copy->NbVertex(); i++)
	{
		pVertex = pMesh3d3copy->GetVertex(i);
		double s = pVertex->x();
		double t = pVertex->y();

		s = startu + s * (endu - startu);
		s = (pNURBS->ptransformation->umax_Reparameterization - pNURBS->ptransformation->umin_Reparameterization) * s + pNURBS->ptransformation->umin_Reparameterization;
		t = (pNURBS->ptransformation->vmax_Reparameterization - pNURBS->ptransformation->vmin_Reparameterization) * t + pNURBS->ptransformation->vmin_Reparameterization;

		Point3D point = pNURBS->ComputPosition(s, t);
		pVertex->Set(point.x, point.y, point.z);
	}
	pDoc->m_SceneGraph.Add(pMesh3d3copy);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
double CFormView3::ComputeMinimalAngle()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CMesh3d* pMesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	double minangle = 1e10;
	for (int i = 0; i < pMesh3d->NbFace(); i++)
	{
		CFace3d* pFace3d = pMesh3d->GetFace(i);
		CVertex3d* pVertex1 = pFace3d->v1();
		CVertex3d* pVertex2 = pFace3d->v2();
		CVertex3d* pVertex3 = pFace3d->v3();
		double a = Distance(pVertex1, pVertex2);
		double b = Distance(pVertex2, pVertex3);
		double c = Distance(pVertex3, pVertex1);

		double anglec = acos((SQR(a) + SQR(b) - SQR(c)) / (2 * a * b));
		double anglea = acos((SQR(b) + SQR(c) - SQR(a)) / (2 * b * c));
		double angleb = acos((SQR(a) + SQR(c) - SQR(b)) / (2 * a * c));
		if (anglea > 1e-5)
			if (anglea < minangle)
				minangle = anglea;
		if (anglea > 1e-5)
			if (angleb < minangle)
				minangle = angleb;
		if (anglea > 1e-5)
			if (anglec < minangle)
				minangle = anglec;
	}
	return minangle * 180 / 3.1415926;
}
void CFormView3::GetCorrespondingLocation()
{
	double x = 0.51;
	double y = 0.54;
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	double newx, newy;
	for (int i = 0; i < pMesh1->NbFace(); i++)
	{
		CFace3d* pFace = pMesh1->GetFace(i);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();
		double x1 = pVertex1->x();
		double y1 = pVertex1->y();
		double x2 = pVertex2->x();
		double y2 = pVertex2->y();
		double x3 = pVertex3->x();
		double y3 = pVertex3->y();
		BOOL b = pTreeView->JudgeWhetherPointLiesInsideTriangle(x, y, pVertex1->x(), pVertex1->y(), pVertex2->x(), pVertex2->y(), pVertex3->x(), pVertex3->y());
		if (b)
		{
			double l1 = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
			double l2 = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
			double l3 = 1 - l1 - l2;
			CFace3d* pFace1 = pMesh2->GetFace(i);
			CVertex3d* pVertex11 = pFace1->v1();
			CVertex3d* pVertex21 = pFace1->v2();
			CVertex3d* pVertex31 = pFace1->v3();
			double x11 = pVertex11->x();
			double y11 = pVertex11->y();
			double x21 = pVertex21->x();
			double y21 = pVertex21->y();
			double x31 = pVertex31->x();
			double y31 = pVertex31->y();

			newx = l1 * x11 + l2 * x21 + l3 * x31;
			newy = l1 * y11 + l2 * y21 + l3 * y31;
			break;
		}
	}
}
void CFormView3::ChangeColors()
{
	std::ifstream infile("D:\\1.txt");
	std::ofstream outfile("D:\\2.txt");
	char message[80];
	double r, g, b;
	CImage m_HotTextureImage;
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\summer.PNG");
	int index = 0;
	while (!infile.eof())
	{
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> r;
		infile >> message;
		infile >> g;
		infile >> message;
		infile >> b;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		infile >> message;
		double mindis = 1e10;
		int minindex;
		for (int i = 0; i <= 100; i++)
		{
			int x = i * 0.01 * m_HotTextureImage.GetWidth();
			if (x == m_HotTextureImage.GetWidth())
				x--;
			COLORREF color = m_HotTextureImage.GetPixel(x, 0);

			double r1 = GetRValue(color) / 256.0;
			double g1 = GetGValue(color) / 256.0;
			double b1 = GetBValue(color) / 256.0;
			double dis = SQR(r - r1) + SQR(g - g1) + SQR(b - b1);
			if (dis < mindis)
			{
				mindis = dis;
				minindex = i;
			}
		}
		double val;
		if (minindex < 33)
		{
			val = 0.04 + (0.21 - 0.04) * minindex / 33.0;
		}
		else if (minindex < 66)
		{
			val = 0.21 + (1.07 - 0.21) * (minindex - 33) / 33.0;
		}
		else
		{
			val = 1.07 + (9.31 - 1.07) * (minindex - 66) / 33.0;
		}

		if (val < 1.2)
		{
			minindex = val * 33 / 1.2;
		}
		else if (val < 5.98)
		{
			minindex = 33 + (val - 1.2) * 33 / (5.98 - 1.2);
		}
		else
		{
			minindex = 66 + (val - 5.98) * 33 / (38.8 - 5.98);
		}
		COLORREF color = m_HotTextureImage.GetPixel(minindex * 0.01 * m_HotTextureImage.GetWidth(), 0);
		double r1 = GetRValue(color) / 256.0;
		double g1 = GetGValue(color) / 256.0;
		double b1 = GetBValue(color) / 256.0;

		outfile << "#declare state_tex" << index++ << " = texture { pigment { rgb < " << r1 << "," << g1 << "," << b1 << "> } finish { ambient thres_ambient diffuse thres_diffuse phong thres_phong  }};" << std::endl;
	}
	infile.close();
	outfile.close();
	/*CImage m_HotTextureImage;
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\summer.PNG");

	double maxcolor = -1;
	double mincolor = 1e10;
	for (int i = 0; i < m_ArrayStates.size(); i++)
	{
		if (m_ArrayStatesColor[i][0] > maxcolor)
			maxcolor = m_ArrayStatesColor[i][0];
		if (m_ArrayStatesColor[i][0] < mincolor)
			mincolor = m_ArrayStatesColor[i][0];
		int x = m_ArrayStatesColor[i][0] * m_HotTextureImage.GetWidth();
		if (x == m_HotTextureImage.GetWidth())
			x--;
		COLORREF color = m_HotTextureImage.GetPixel(x, 0);
		double r = GetRValue(color) / 256.0;
		double g = GetGValue(color) / 256.0;
		double b = GetBValue(color) / 256.0;
		outfile << "#declare state_tex" << i << " = texture { pigment { rgb < " << r << "," << g << "," << b << "> } finish { ambient thres_ambient diffuse thres_diffuse phong thres_phong  }};" << std::endl;
		*/
}

void CFormView3::TessellateBunnySurfaces()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	CNurbsSuface* pNurbsSurface, * pNurbsSurface1;
	MyMesh* pMesh = new MyMesh();

	std::vector<Point3D> m_ArrayPointArray;
	//Add vertex
	int i;
	for (int ii = 0; ii < num; ii++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
			Point3D p00 = pNurbsSurface->ComputPosition(0, 0);
			Point3D p10 = pNurbsSurface->ComputPosition(1, 0);
			Point3D p01 = pNurbsSurface->ComputPosition(0, 1);
			Point3D p11 = pNurbsSurface->ComputPosition(1, 1);

			for (i = 0; i < m_ArrayPointArray.size(); i++)
			{
				double dis = Distance(m_ArrayPointArray[i], p00);
				if (dis < 1e-8)
				{
					pNurbsSurface->m_ArrayHalfEdgeStructure.push_back(i);
					break;
				}
			}
			if (i == m_ArrayPointArray.size())
			{
				m_ArrayPointArray.push_back(p00);
				pNurbsSurface->m_ArrayHalfEdgeStructure.push_back(i);
			}
			for (i = 0; i < m_ArrayPointArray.size(); i++)
			{
				double dis = Distance(m_ArrayPointArray[i], p10);
				if (dis < 1e-8)
				{
					pNurbsSurface->m_ArrayHalfEdgeStructure.push_back(i);
					break;
				}
			}
			if (i == m_ArrayPointArray.size())
			{
				m_ArrayPointArray.push_back(p10);
				pNurbsSurface->m_ArrayHalfEdgeStructure.push_back(i);
			}
			for (i = 0; i < m_ArrayPointArray.size(); i++)
			{
				double dis = Distance(m_ArrayPointArray[i], p11);
				if (dis < 1e-8)
				{
					pNurbsSurface->m_ArrayHalfEdgeStructure.push_back(i);
					break;
				}
			}
			if (i == m_ArrayPointArray.size())
			{
				m_ArrayPointArray.push_back(p11);
				pNurbsSurface->m_ArrayHalfEdgeStructure.push_back(i);
			}

			for (i = 0; i < m_ArrayPointArray.size(); i++)
			{
				double dis = Distance(m_ArrayPointArray[i], p01);
				if (dis < 1e-8)
				{
					pNurbsSurface->m_ArrayHalfEdgeStructure.push_back(i);
					break;
				}
			}
			if (i == m_ArrayPointArray.size())
			{
				m_ArrayPointArray.push_back(p01);
				pNurbsSurface->m_ArrayHalfEdgeStructure.push_back(i);
			}
		}
	}

	//add vertex
	for (i = 0; i < m_ArrayPointArray.size(); i++)
	{
		Point3D point = m_ArrayPointArray[i];
		pMesh->add_vertex(MyMesh::Point(point.x, point.y, point.z));
	}

	//add faces.
	for (int ii = 0; ii < num; ii++)
	{
		std::vector<MyMesh::VertexHandle>  face_vhandles;
		pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
		face_vhandles.clear();
		face_vhandles.push_back(pMesh->vertex_handle(pNurbsSurface->m_ArrayHalfEdgeStructure[0]));
		face_vhandles.push_back(pMesh->vertex_handle(pNurbsSurface->m_ArrayHalfEdgeStructure[1]));
		face_vhandles.push_back(pMesh->vertex_handle(pNurbsSurface->m_ArrayHalfEdgeStructure[2]));
		face_vhandles.push_back(pMesh->vertex_handle(pNurbsSurface->m_ArrayHalfEdgeStructure[3]));

		pMesh->add_face(face_vhandles);
	}
	CMesh3d_OpenMesh* pMesh_OpenMesh = new CMesh3d_OpenMesh();
	pMesh_OpenMesh->pmesh = pMesh;
	pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	pMesh_OpenMesh->m_Transform.Copy(pNurbsSurface->GetTransform());
	pDoc->m_SceneGraph.Add(pMesh_OpenMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	//return;
	//set transformations
	SetBunnyGeneralTransformation2();

	double edgetolerance, facetolerance;
	edgetolerance = 0.001;
	facetolerance = 0.001;

	EPROP_EDGE_INTERINFO eprop_interinfo;
	FPROP_FACE_INTERINFO fprop_interinfo;
	HPROP_EDGE_INTERINFO hprop_interinfo;

	//Edge intermediate info to store the tessellation points
	if (!pMesh_OpenMesh->pmesh->get_property_handle(eprop_interinfo, PROP_EDGE_INTERMEDIATEINFO))
		pMesh_OpenMesh->pmesh->add_property(eprop_interinfo, PROP_EDGE_INTERMEDIATEINFO);
	//Face intermediate info to store the tessellation points
	if (!pMesh_OpenMesh->pmesh->get_property_handle(fprop_interinfo, PROP_FACE_INTERMEDIATEINFO))
		pMesh_OpenMesh->pmesh->add_property(fprop_interinfo, PROP_FACE_INTERMEDIATEINFO);
	//Half Edge intermediate info
	if (!pMesh_OpenMesh->pmesh->get_property_handle(hprop_interinfo, PROP_HEDGE_INTERMEDIATEINFO))
		pMesh_OpenMesh->pmesh->add_property(hprop_interinfo, PROP_HEDGE_INTERMEDIATEINFO);

	std::vector<Vector3D> m_ArrayEdgeTessellationPoints;
	std::vector<Vector2D> m_ArrayEdge2DPoints, m_ArrayEdge2DPoints1;
	//First Tessellate the Edges under the tolerance
	for (MyMesh::EdgeIter e_it = pMesh_OpenMesh->pmesh->edges_begin(); e_it != pMesh_OpenMesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		MyMesh::HalfedgeHandle hh = pMesh_OpenMesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pMesh_OpenMesh->pmesh->halfedge_handle(eh, 1);
		MyMesh::VertexHandle vh2 = pMesh_OpenMesh->pmesh->to_vertex_handle(hh);
		MyMesh::VertexHandle vh1 = pMesh_OpenMesh->pmesh->from_vertex_handle(hh);

		OpenMesh::Vec3d v1 = pMesh_OpenMesh->pmesh->point(vh1);
		OpenMesh::Vec3d v2 = pMesh_OpenMesh->pmesh->point(vh2);

		MyMesh::FaceHandle fh = pMesh_OpenMesh->pmesh->face_handle(hh);
		MyMesh::FaceHandle fh1 = pMesh_OpenMesh->pmesh->face_handle(hh1);
		int fidx = fh.idx();
		int fidx1 = fh1.idx();
		pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(fidx);
		pNurbsSurface1 = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(fidx1);
		//pNurbsSurface->m_ArrayHalfEdgeStructure[]
		Vector2D pv1 = GetSurfaceParameter(v1, pNurbsSurface);
		Vector2D pv2 = GetSurfaceParameter(v2, pNurbsSurface);
		Vector2D pv3 = GetSurfaceParameter(v1, pNurbsSurface1);
		Vector2D pv4 = GetSurfaceParameter(v2, pNurbsSurface1);
		m_ArrayEdgeTessellationPoints.clear();
		m_ArrayEdge2DPoints.clear();
		m_ArrayEdge2DPoints1.clear();

		//put the points into the edges.
		TessellateEdge(pNurbsSurface, v1, v2, pv1, pv2, pv3, pv4, m_ArrayEdgeTessellationPoints, edgetolerance, m_ArrayEdge2DPoints, m_ArrayEdge2DPoints1);
		std::vector<double> m_ArrayEdgeTessellationPointscoordiantes;
		for (int ii = 0; ii < m_ArrayEdgeTessellationPoints.size(); ii++)
		{
			m_ArrayEdgeTessellationPointscoordiantes.push_back(m_ArrayEdgeTessellationPoints[ii][0]);
			m_ArrayEdgeTessellationPointscoordiantes.push_back(m_ArrayEdgeTessellationPoints[ii][1]);
			m_ArrayEdgeTessellationPointscoordiantes.push_back(m_ArrayEdgeTessellationPoints[ii][2]);
		}
		pMesh_OpenMesh->pmesh->property(eprop_interinfo, eh) = m_ArrayEdgeTessellationPointscoordiantes;
		std::vector<double> m_ArrayEdgeTessellationPoints2Dcoordiantes, m_ArrayEdgeTessellationPoints2Dcoordiantes1;
		for (int ii = 0; ii < m_ArrayEdge2DPoints.size(); ii++)
		{
			m_ArrayEdgeTessellationPoints2Dcoordiantes.push_back(m_ArrayEdge2DPoints[ii][0]);
			m_ArrayEdgeTessellationPoints2Dcoordiantes.push_back(m_ArrayEdge2DPoints[ii][1]);
		}
		pMesh_OpenMesh->pmesh->property(hprop_interinfo, hh) = m_ArrayEdgeTessellationPoints2Dcoordiantes;
		for (int ii = m_ArrayEdge2DPoints1.size() - 1; ii >= 0; ii--)
		{
			m_ArrayEdgeTessellationPoints2Dcoordiantes1.push_back(m_ArrayEdge2DPoints1[ii][0]);
			m_ArrayEdgeTessellationPoints2Dcoordiantes1.push_back(m_ArrayEdge2DPoints1[ii][1]);
		}
		pMesh_OpenMesh->pmesh->property(hprop_interinfo, hh1) = m_ArrayEdgeTessellationPoints2Dcoordiantes1;
		////Vector2D middle = (pv1 + pv2) / 2;
		//Point3D middlep = pNurbsSurface->ComputPosition(middle);
	}
	//output the edges.
	std::ofstream outfileedge("d:\\1.inc");
	for (MyMesh::EdgeIter e_it = pMesh_OpenMesh->pmesh->edges_begin(); e_it != pMesh_OpenMesh->pmesh->edges_end(); ++e_it)
	{
		std::vector<double> Points;
		Points = pMesh_OpenMesh->pmesh->property(eprop_interinfo, e_it.handle());

		int num = Points.size() / 3;
		for (int ij = 0; ij < num - 1; ij++)
		{
			outfileedge << "cylinder {<" << Points[ij * 3] << " , " << Points[ij * 3 + 1] << " , " << -Points[ij * 3 + 2] << " >,<" << Points[(ij + 1) * 3] << " , " << Points[(ij + 1) * 3 + 1] << " , " << -Points[(ij + 1) * 3 + 2] << " > ,r_edge_ texture { edge_tex_ } }" << endl;
		}
	}
	outfileedge.close();
	//return;
	std::vector<double> m_ArrayFaceTessellationPointscoordiantes;
	for (int ii = 0; ii < num; ii++)
	{
		pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
		m_ArrayFaceTessellationPointscoordiantes.clear();

		//Second Tessellate the faces under the tolerance
		TessellateSurface_Kdtree(pNurbsSurface, facetolerance, m_ArrayFaceTessellationPointscoordiantes);
		pMesh_OpenMesh->pmesh->property(fprop_interinfo, pMesh_OpenMesh->pmesh->face_handle(ii)) = m_ArrayFaceTessellationPointscoordiantes;
	}

	std::vector<double> edgepointsarray, edgepoints;
	std::ofstream outfile("D:\\mesh.obj");
	int pointsnum = 0;
	std::vector<CMesh3d*> m_ArrayMesh3d;
	//Tessellate the face.
	for (int ii = 0; ii < num; ii++)
	{
		pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);

		std::vector<double> m_ArrayFaceTessellationPointscoordiantes;

		MyMesh::FaceHandle fh = pMesh_OpenMesh->pmesh->face_handle(ii);
		m_ArrayFaceTessellationPointscoordiantes = pMesh_OpenMesh->pmesh->property(fprop_interinfo, fh);

		edgepointsarray.clear();
		//remove the same end points!
		for (MyMesh::FaceHalfedgeIter fh_it = pMesh_OpenMesh->pmesh->fh_iter(fh); fh_it; ++fh_it)
		{
			edgepoints = pMesh_OpenMesh->pmesh->property(hprop_interinfo, fh_it.handle());
			/*for (int ij = 0; ij < 3; ij++)
			{
				if (std::find(edgepointsarray.begin(), edgepointsarray.end(), edgepoints[0]) != edgepointsarray.end())
				{
					edgepoints.erase(edgepoints.begin());
				}
			}
			for (int ij = 0; ij < 3; ij++)
			{
				if (std::find(edgepointsarray.begin(), edgepointsarray.end(), edgepoints[edgepoints.size() - 1]) != edgepointsarray.end())
					edgepoints.erase(edgepoints.begin() + edgepoints.size() - 1);
			}*/
			if (edgepointsarray.size() > 0)
			{
				//if (fabs(edgepoints[0] - edgepointsarray[edgepointsarray.size() - 2]) > 1e-8 ||
				//	fabs(edgepoints[1] - edgepointsarray[edgepointsarray.size() - 1]) > 1e-8)
				//{
				//	std::vector<double> edgepoints_;
				//	for (int ij = edgepoints.size() - 1; ij >= 0; ij--)
				//	{
				//		edgepoints_.push_back(edgepoints[ij]);
				//	}
				//	edgepoints = edgepoints_;
				//}

				if (fabs(edgepoints[0] - edgepointsarray[edgepointsarray.size() - 2]) > 1e-8 ||
					fabs(edgepoints[1] - edgepointsarray[edgepointsarray.size() - 1]) > 1e-8)
					ASSERT(0);
			}
			edgepointsarray.insert(edgepointsarray.end(), edgepoints.begin(), edgepoints.end());
		}
		//
		RemoveDuplicatePoints(edgepointsarray);
		//Tessellate the points array.
		//output the .polygon file
		OnCDT_PSLG_POINTS(m_ArrayFaceTessellationPointscoordiantes, edgepointsarray);
		CMesh3d* pMesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
		pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);
		m_ArrayMesh3d.push_back(pMesh3d);
	}
	for (int ii = 0; ii < m_ArrayMesh3d.size(); ii++)
	{
		pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
		CMesh3d* pMesh3d = m_ArrayMesh3d[ii];
		for (int i = 0; i < pMesh3d->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3d->GetVertex(i);
			Point3D point = pNurbsSurface->ComputPosition(pVertex->x(), pVertex->y());
			outfile << "v " << point[0] << " " << point[1] << " " << point[2] << "\n";
		}
	}
	for (int ii = 0; ii < m_ArrayMesh3d.size(); ii++)
	{
		CMesh3d* pMesh3d = m_ArrayMesh3d[ii];
		pMesh3d->SetVertexFlagAsItsIndex();
		for (int i = 0; i < pMesh3d->NbFace(); i++)
		{
			CFace3d* pFace = pMesh3d->GetFace(i);
			outfile << "f " << pFace->v1()->GetFlag() + 1 + pointsnum << " " << pFace->v2()->GetFlag() + 1 + pointsnum << " " << pFace->v3()->GetFlag() + 1 + pointsnum << "\n";
		}
		pointsnum += pMesh3d->NbVertex();
	}
	outfile.close();
}
void CFormView3::RemoveDuplicatePoints(std::vector<double>& edgepointsarray)
{
	std::vector<Vector2D> pointsarray;
	Vector2D vv;
	for (int i = 0; i < edgepointsarray.size(); i = i + 2)
	{
		vv.SetValue(edgepointsarray[i], edgepointsarray[i + 1]);
		pointsarray.push_back(vv);
	}
	for (int i = 1; i < pointsarray.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (Distance(pointsarray[i], pointsarray[j]) < 1e-8)
			{
				pointsarray.erase(pointsarray.begin() + i);
				i--;
				break;
			}
		}
	}
	edgepointsarray.clear();
	for (int i = 0; i < pointsarray.size(); i++)
	{
		edgepointsarray.push_back(pointsarray[i][0]);
		edgepointsarray.push_back(pointsarray[i][1]);
	}
}
void CFormView3::TessellateSurface_Kdtree(CNurbsSuface* pNurbsSurface, double facetolerance, std::vector<double>& m_ArrayFaceTessellationPointscoordiantes)
{
	KDTree kdtree1;
	kdtree1.pNurbsSurface = pNurbsSurface;
	kdtree1.maxDeviation = facetolerance;
	kdtree1.ConstructionKdTree_NURBSSurface(0, 1, 0, 1, 0, NULL);

	//
	kdtree1.Collect_Points_NURBS(kdtree1.proot, m_ArrayFaceTessellationPointscoordiantes);
}
void CFormView3::TessellateEdge(CNurbsSuface* pNurbsSurface, OpenMesh::Vec3d v1, OpenMesh::Vec3d v2, Vector2D pv1, Vector2D pv2, Vector2D pv3, Vector2D pv4, std::vector<Vector3D>& m_ArrayEdgeTessellationPoints, double edgetolerance, std::vector<Vector2D>& m_ArrayEdge2DPoints, std::vector<Vector2D>& m_ArrayEdge2DPoints1)
{
	Vector3D v1_(v1[0], v1[1], v1[2]);
	Vector3D v2_(v2[0], v2[1], v2[2]);

	m_ArrayEdgeTessellationPoints.push_back(v1_);
	m_ArrayEdgeTessellationPoints.push_back(v2_);
	//std::vector<Vector2D> m_ArrayParameterPoints;
	m_ArrayEdge2DPoints.push_back(pv1);
	m_ArrayEdge2DPoints.push_back(pv2);
	m_ArrayEdge2DPoints1.push_back(pv3);
	m_ArrayEdge2DPoints1.push_back(pv4);
	int i;
	double dis;
	int flag = 1;
	while (flag)
	{
		flag = 0;
		for (i = 0; i < m_ArrayEdgeTessellationPoints.size() - 1; i++)
		{
			Vector2D vv1 = m_ArrayEdge2DPoints[i];
			Vector2D vv2 = m_ArrayEdge2DPoints[i + 1];
			Vector2D vv3 = m_ArrayEdge2DPoints1[i];
			Vector2D vv4 = m_ArrayEdge2DPoints1[i + 1];

			Vector2D middle = (vv1 + vv2) / 2;
			Vector2D middle1 = (vv3 + vv4) / 2;
			Vector3D pp1 = pNurbsSurface->ComputPosition(middle[0], middle[1]) - ZeroP3D;

			Vector3D vv1_ = m_ArrayEdgeTessellationPoints[i];
			Vector3D vv2_ = m_ArrayEdgeTessellationPoints[i + 1];
			Vector3D pp1_ = (vv1_ + vv2_) / 2;
			dis = Distance(pp1, pp1_);
			if (dis > edgetolerance)
			{
				flag = 1;
				m_ArrayEdgeTessellationPoints.insert(m_ArrayEdgeTessellationPoints.begin() + i + 1, pp1);
				m_ArrayEdge2DPoints.insert(m_ArrayEdge2DPoints.begin() + i + 1, middle);
				m_ArrayEdge2DPoints1.insert(m_ArrayEdge2DPoints1.begin() + i + 1, middle1);
				i = i - 1;
			}
		}
	}
}
Vector2D CFormView3::GetSurfaceParameter(OpenMesh::Vec3d v1, CNurbsSuface* pNurbsSurface)
{
	Vector3D p00, p10, p01, p11;
	p00 = pNurbsSurface->ComputPosition(0, 0) - ZeroP3D;
	p10 = pNurbsSurface->ComputPosition(1, 0) - ZeroP3D;
	p01 = pNurbsSurface->ComputPosition(0, 1) - ZeroP3D;
	p11 = pNurbsSurface->ComputPosition(1, 1) - ZeroP3D;
	Vector3D vv(v1[0], v1[1], v1[2]);
	Vector2D result;
	if (Distance(p00, vv) < 1e-8)
	{
		result.SetValue(0, 0);
	}
	else if (Distance(p10, vv) < 1e-8)
	{
		result.SetValue(1, 0);
	}
	else if (Distance(p01, vv) < 1e-8)
	{
		result.SetValue(0, 1);
	}
	else if (Distance(p11, vv) < 1e-8)
	{
		result.SetValue(1, 1);
	}
	else
		ASSERT(0);
	return result;
}
void CFormView3::OnCDT_PSLG_POINTS(std::vector<double>& m_ArrayFaceTessellationPointscoordiantes, std::vector<double>& edgepointsarray)
{
	std::ofstream outfile("D:\\papers\\code\\Triangle\\Geo.poly");
	//int n = 20;
	int num = edgepointsarray.size() + m_ArrayFaceTessellationPointscoordiantes.size();
	outfile << num / 2 << " 2 0 1" << "\n";
	int index = 0;
	for (int i = 0; i < edgepointsarray.size(); i = i + 2)
	{
		outfile << index++ << " " << edgepointsarray[i] << " " << edgepointsarray[i + 1] << " 1 " << "\n";
	}

	for (int i = 0; i < m_ArrayFaceTessellationPointscoordiantes.size(); i = i + 2)
	{
		outfile << index++ << " " << m_ArrayFaceTessellationPointscoordiantes[i] << " " << m_ArrayFaceTessellationPointscoordiantes[i + 1] << " 0 " << "\n";
	}
	outfile << edgepointsarray.size() / 2 << " 1\n";
	for (int i = 0; i < edgepointsarray.size() / 2; i++)
	{
		outfile << i << " " << (i) % (edgepointsarray.size() / 2) << " " << (i + 1) % (edgepointsarray.size() / 2) << "\n";
	}
	outfile << " 0 " << "\n";
	outfile.close();

	char step1[200];
	SetCurrentDirectory("D:\\papers\\code\\triangle");
	//cf;
	sprintf(step1, "triangle.exe -p Geo");
	system(step1);

	char resultmeshname[100];
	sprintf(resultmeshname, "D:\\papers\\code\\triangle\\Geo.1.ele");
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument(resultmeshname);
}
void CFormView3::SetBunnyGeneralTransformation2()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject() - 1;

	CMesh3d_OpenMesh* pMesh_OpenMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	HPROP_EDGE_TRANSFORMATIONINFO hprop_transformationinfo;

	//face selected flag
	if (!pMesh_OpenMesh->pmesh->get_property_handle(hprop_transformationinfo, PROP_HEDGE_TRANSFORMATIONINFO))
		pMesh_OpenMesh->pmesh->add_property(hprop_transformationinfo, PROP_HEDGE_TRANSFORMATIONINFO);

	for (MyMesh::EdgeIter e_it = pMesh_OpenMesh->pmesh->edges_begin(); e_it != pMesh_OpenMesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		MyMesh::HalfedgeHandle hh = pMesh_OpenMesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pMesh_OpenMesh->pmesh->halfedge_handle(eh, 1);

		pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, hh) = 0.6;
		pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, hh1) = 0.4;
	}

	CNurbsSuface* pNurbsSurface, * pNurbsSurface1;

	for (int ii = 0; ii < num; ii++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);

			MyMesh::FaceHandle fh = pMesh_OpenMesh->pmesh->face_handle(ii);
			BilinearTransformation* generaltrans = new BilinearTransformation();
			//generaltrans->m_Bilinear_Alpha1 = 0.3;
			//generaltrans->m_Bilinear_Alpha2 = 0.6;
			//generaltrans->m_Bilinear_Beta1 = 0.7;
			//generaltrans->m_Bilinear_Beta2 = 0.6;
			for (MyMesh::FaceHalfedgeIter fh_it = pMesh_OpenMesh->pmesh->fh_iter(fh); fh_it; ++fh_it)
			{
				MyMesh::VertexHandle vh2 = pMesh_OpenMesh->pmesh->to_vertex_handle(fh_it.handle());
				MyMesh::VertexHandle vh1 = pMesh_OpenMesh->pmesh->from_vertex_handle(fh_it.handle());

				OpenMesh::Vec3d v1 = pMesh_OpenMesh->pmesh->point(vh1);
				OpenMesh::Vec3d v2 = pMesh_OpenMesh->pmesh->point(vh2);

				Vector2D v1_para = GetSurfaceParameter(v1, pNurbsSurface);
				Vector2D v2_para = GetSurfaceParameter(v2, pNurbsSurface);

				if (fabs(v1_para.x - v2_para.x) < 1e-8)
				{
					if (v1_para.x < 0.5)
					{
						if (v1_para.y > v2_para.y)
						{
							generaltrans->m_Bilinear_Beta2 = 1 - pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
						else
						{
							generaltrans->m_Bilinear_Beta2 = pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
					}
					else
					{
						if (v1_para.y < v2_para.y)
						{
							generaltrans->m_Bilinear_Beta1 = pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
						else
						{
							generaltrans->m_Bilinear_Beta1 = 1 - pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
					}
				}
				else
				{
					if (v1_para.y < 0.5)
					{
						if (v1_para.x < v2_para.x)
						{
							generaltrans->m_Bilinear_Alpha2 = 1 - pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
						else
						{
							generaltrans->m_Bilinear_Alpha2 = pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
					}
					else
					{
						if (v1_para.x > v2_para.x)
						{
							generaltrans->m_Bilinear_Alpha1 = pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
						else
						{
							generaltrans->m_Bilinear_Alpha1 = 1 - pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
					}
				}
			}
			pNurbsSurface->ptransformation = generaltrans;
			//generaltrans->UpdateBoundingBox();
		}
	}

	//check consistency
	for (MyMesh::EdgeIter e_it = pMesh_OpenMesh->pmesh->edges_begin(); e_it != pMesh_OpenMesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		MyMesh::HalfedgeHandle hh = pMesh_OpenMesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pMesh_OpenMesh->pmesh->halfedge_handle(eh, 1);

		MyMesh::FaceHandle fh = pMesh_OpenMesh->pmesh->face_handle(hh);
		MyMesh::FaceHandle fh1 = pMesh_OpenMesh->pmesh->face_handle(hh1);

		pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(fh.idx());
		pNurbsSurface1 = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(fh1.idx());

		MyMesh::VertexHandle vh2 = pMesh_OpenMesh->pmesh->to_vertex_handle(hh);
		MyMesh::VertexHandle vh1 = pMesh_OpenMesh->pmesh->from_vertex_handle(hh);

		OpenMesh::Vec3d v1 = pMesh_OpenMesh->pmesh->point(vh1);
		OpenMesh::Vec3d v2 = pMesh_OpenMesh->pmesh->point(vh2);

		Vector2D v1_para = GetSurfaceParameter(v1, pNurbsSurface);
		Vector2D v2_para = GetSurfaceParameter(v2, pNurbsSurface);
		Vector2D middle = (v1_para + v2_para) / 2;

		Vector2D v3_para = GetSurfaceParameter(v1, pNurbsSurface1);
		Vector2D v4_para = GetSurfaceParameter(v2, pNurbsSurface1);
		Vector2D middle_ = (v3_para + v4_para) / 2;

		Vector3D point = pNurbsSurface->ComputPosition(middle[0], middle[1]) - ZeroP3D;
		Vector3D point_ = pNurbsSurface1->ComputPosition(middle_[0], middle_[1]) - ZeroP3D;
		if (Distance(point, point_) > 1e-6)
		{
			pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, hh1) = 1 - pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, hh1);
		}
	}

	for (int ii = 0; ii < num; ii++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);

			MyMesh::FaceHandle fh = pMesh_OpenMesh->pmesh->face_handle(ii);
			BilinearTransformation* generaltrans = new BilinearTransformation();
			//generaltrans->m_Bilinear_Alpha1 = 0.3;
			//generaltrans->m_Bilinear_Alpha2 = 0.6;
			//generaltrans->m_Bilinear_Beta1 = 0.7;
			//generaltrans->m_Bilinear_Beta2 = 0.6;
			for (MyMesh::FaceHalfedgeIter fh_it = pMesh_OpenMesh->pmesh->fh_iter(fh); fh_it; ++fh_it)
			{
				MyMesh::VertexHandle vh2 = pMesh_OpenMesh->pmesh->to_vertex_handle(fh_it.handle());
				MyMesh::VertexHandle vh1 = pMesh_OpenMesh->pmesh->from_vertex_handle(fh_it.handle());

				OpenMesh::Vec3d v1 = pMesh_OpenMesh->pmesh->point(vh1);
				OpenMesh::Vec3d v2 = pMesh_OpenMesh->pmesh->point(vh2);

				Vector2D v1_para = GetSurfaceParameter(v1, pNurbsSurface);
				Vector2D v2_para = GetSurfaceParameter(v2, pNurbsSurface);

				if (fabs(v1_para.x - v2_para.x) < 1e-8)
				{
					if (v1_para.x < 0.5)
					{
						if (v1_para.y > v2_para.y)
						{
							generaltrans->m_Bilinear_Beta2 = 1 - pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
						else
						{
							generaltrans->m_Bilinear_Beta2 = pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
					}
					else
					{
						if (v1_para.y < v2_para.y)
						{
							generaltrans->m_Bilinear_Beta1 = pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
						else
						{
							generaltrans->m_Bilinear_Beta1 = 1 - pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
					}
				}
				else
				{
					if (v1_para.y < 0.5)
					{
						if (v1_para.x < v2_para.x)
						{
							generaltrans->m_Bilinear_Alpha2 = 1 - pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
						else
						{
							generaltrans->m_Bilinear_Alpha2 = pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
					}
					else
					{
						if (v1_para.x > v2_para.x)
						{
							generaltrans->m_Bilinear_Alpha1 = pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
						else
						{
							generaltrans->m_Bilinear_Alpha1 = 1 - pMesh_OpenMesh->pmesh->property(hprop_transformationinfo, fh_it.handle());
						}
					}
				}
			}
			pNurbsSurface->ptransformation = generaltrans;
			//generaltrans->UpdateBoundingBox();
		}
	}
}
void CFormView3::OutPutCircleTessellations()
{
	std::ofstream outfile("D:\\CircleTessallations.obj");
	CString s1;
	int i, j;
	int samplesm = 1001;
	int samplesn = 300;

	outfile << "# " << (samplesm) * (samplesn)+1 << "vertices, " << samplesm * (2 * samplesn - 1) << "faces" << endl;
	int index00, indexm0, indexmn, index0n;
	int num = 1;
	double angle;
	outfile << "v 0 0 0 " << endl;
	for (i = 0; i < samplesm; i++)
	{
		for (j = 1; j <= samplesn; j++)
		{
			//
			num++;
			angle = (i + 0.0) * PI * 2 / samplesm;
			double x = cos(angle);
			double y = sin(angle);

			double s = x * j / samplesn;
			double t = y * j / samplesn;

			outfile << "v " << s << " " << t << " " << 0 << endl;
		}
	}

	int index1, index2, index3, index4;
	for (j = 1; j <= samplesm; j++)
	{
		for (i = 1; i < samplesn; i++)
		{
			//
			index1 = 1 + (j - 1) * samplesn + i;
			index2 = 1 + (j % samplesm) * samplesn + i;
			index3 = 1 + (j - 1) * samplesn + i + 1;
			index4 = 1 + (j % samplesm) * samplesn + i + 1;

			outfile << "f " << index1 << " " << index4 << " " << index2 << endl;
			outfile << "f " << index1 << " " << index3 << " " << index4 << endl;
		}
	}
	for (j = 1; j <= samplesm; j++)
	{
		index1 = 1;
		index2 = 1 + (j - 1) * samplesn + 1;
		index3 = 1 + (j % samplesm) * samplesn + 1;
		outfile << "f " << index1 << " " << index2 << " " << index3 << endl;
	}

	//int i1 = samplesm * 1 / 8;
	//int i2 = samplesm * 3 / 8;
	//int i3 = samplesm * 5 / 8;
	//int i4 = samplesm * 7 / 8;
	double ss1 = 1.935;
	double ss2 = 1;
	int i1 = samplesm * ss1 / ((ss1 + ss2) * 4);
	int i2 = samplesm * (ss1 + ss2 * 2) / ((ss1 + ss2) * 4);
	int i3 = samplesm * (ss1 * 3 + ss2 * 2) / ((ss1 + ss2) * 4);
	int i4 = samplesm * (ss1 * 3 + ss2 * 4) / ((ss1 + ss2) * 4);

	outfile << "Edge " << 1 + (i1 % samplesm) * samplesn << " " << 1 + ((i1) % samplesm) * samplesn + samplesn << " {sharp} " << endl;
	outfile << "Edge " << 1 + ((i1 + 2) % samplesm) * samplesn << " " << 1 + ((i1) % samplesm) * samplesn + samplesn << " {sharp} " << endl;
	outfile << "Edge " << 1 + ((i1) % samplesm) * samplesn + samplesn << " " << ((i1) % samplesm) * samplesn + samplesn << " {sharp} " << endl;

	outfile << "Edge " << 1 + (i2 % samplesm) * samplesn << " " << 1 + ((i2) % samplesm) * samplesn + samplesn << " {sharp} " << endl;
	outfile << "Edge " << 1 + ((i2 + 2) % samplesm) * samplesn << " " << 1 + ((i2) % samplesm) * samplesn + samplesn << " {sharp} " << endl;
	outfile << "Edge " << 1 + ((i2) % samplesm) * samplesn + samplesn << " " << ((i2) % samplesm) * samplesn + samplesn << " {sharp} " << endl;

	outfile << "Edge " << 1 + (i3 % samplesm) * samplesn << " " << 1 + ((i3) % samplesm) * samplesn + samplesn << " {sharp} " << endl;
	outfile << "Edge " << 1 + ((i3 + 2) % samplesm) * samplesn << " " << 1 + ((i3) % samplesm) * samplesn + samplesn << " {sharp} " << endl;
	outfile << "Edge " << 1 + ((i3) % samplesm) * samplesn + samplesn << " " << ((i3) % samplesm) * samplesn + samplesn << " {sharp} " << endl;

	outfile << "Edge " << 1 + (i4 % samplesm) * samplesn << " " << 1 + ((i4) % samplesm) * samplesn + samplesn << " {sharp} " << endl;
	outfile << "Edge " << 1 + ((i4 + 2) % samplesm) * samplesn << " " << 1 + ((i4) % samplesm) * samplesn + samplesn << " {sharp} " << endl;
	outfile << "Edge " << 1 + ((i4) % samplesm) * samplesn + samplesn << " " << ((i4) % samplesm) * samplesn + samplesn << " {sharp} " << endl;

	outfile.close();
}
void CFormView3::RotateMesh(CMesh3d* pMesh)
{
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();

		double len = sqrt(x * x + y * y);
		if (len < 1e-8)
		{
			pVertex->Set(x, y, 0);
		}
		else
		{
			double angle = acos(x / len);
			if (y < 0)
				angle = PI * 2 - angle;
			angle += PI;
			x = cos(angle) * len;
			y = sin(angle) * len;
			pVertex->Set(x, y, 0);
		}
	}
	pMesh->m_Modified = 1;
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::CheckIsolatedVertices(CMesh3d* pMesh)
{
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->m_Flag = 0;
	}
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace3d = pMesh->GetFace(i);
		pFace3d->v1()->m_Flag = 1;
		pFace3d->v2()->m_Flag = 1;
		pFace3d->v3()->m_Flag = 1;
	}
	TRACE(" \n\n ");
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		if (pVertex->m_Flag == 0)
		{
			pMesh->m_ArrayVertex.RemoveAt(i);
			i--;
		}
	}
}
void CFormView3::SetMeshTextureColor(CMesh3d* pMesh)
{
	CImage m_TextureImage;
	//m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\F0001_FE01WH_F3D_textureImage1.png");
	//m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\AN\\F0001_AN01WH_F3D_textureImage1.png"); 
	//m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\F0001_SU02WH_F3D_fixed_textureImage1.png");
	//m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\DI\\F0001_DI01WH_F3DImage1.png");
	//m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\HA\\F0001_HA01WH_F3D_textureImage1.png");
	//m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\NE\\F0001_NE00WH_F3DImage1.png");
	//m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\SA\\F0001_SA01WH_F3D_textureImage1.png");
	//m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\F0001_NE00WH_F3DImage1.png");
	//m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\M0004_NE00WH_F3DImage1.png");
	m_TextureImage.Load("D:\\papers\\code\\data\\CVPR2017\\example2\\Newfolder\\11_curve_1_subject_each_expression\\11_curve_1_subject_each_expression\\neutralexample\\F0006_NE00WH_F3D.png");
	double u, v;
	double x, y;
	CVertex3d* pVertex;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		pVertex = pMesh->GetVertex(i);
		if (pVertex->u > 0 && pVertex->u < 1)
		{
			x = m_TextureImage.GetWidth() * (pVertex->u);
			y = m_TextureImage.GetHeight() * (1 - pVertex->v);
			COLORREF color = m_TextureImage.GetPixel(x, y);
			pVertex->SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
		}
	}
	pMesh->m_ColorBinding = COLOR_PER_VERTEX;
	pMesh->m_Modified = 1;
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::GenerateMatrix()
{
	std::ofstream ofile("C:\\MATLAB701\\work\\A.txt");
	std::ifstream ifile("C:\\MATLAB701\\work\\signature.txt");
	double A[13][44];

	double B[13][13];
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 44; j++)
		{
			ifile >> A[i][j];
		}
	}
	for (int i = 0; i < 6; i++)
	{
		if (i == 4)
			continue;
		for (int j = 0; j < 44; j++)
		{
			A[i][j] = A[i][j] - A[4][j];
		}
	}
	for (int j = 0; j < 44; j++)
	{
		A[4][j] = 0;
	}
	for (int i = 6; i < 11; i++)
	{
		if (i == 9)
			continue;
		for (int j = 0; j < 44; j++)
		{
			A[i][j] = A[i][j] - A[9][j];
		}
	}
	for (int j = 0; j < 44; j++)
	{
		A[9][j] = 0;
	}

	double dis = 0;
	for (int i = 0; i < 11; i++)
		for (int j = 0; j < 11; j++)
		{
			dis = 0;
			for (int k = 0; k < 44; k++)
			{
				dis += SQR(A[i][k] - A[j][k]);
			}
			dis = sqrt(dis);
			B[i][j] = dis;
		}

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			ofile << B[i][j] << " ";
		}
		ofile << "\n";
	}
	ofile.close();
}
void CFormView3::GenerateMatrix1()
{
	std::ofstream ofile("C:\\MATLAB701\\work\\A1.txt");
	std::ifstream ifile("C:\\MATLAB701\\work\\signature1.txt");
	double A[11][11];

	double B[11][11];
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			ifile >> A[i][j];
		}
	}
	for (int i = 0; i < 6; i++)
	{
		if (i == 4)
			continue;
		for (int j = 0; j < 11; j++)
		{
			A[i][j] = A[i][j] - A[4][j];
		}
	}
	for (int j = 0; j < 11; j++)
	{
		A[4][j] = 0;
	}
	for (int i = 6; i < 11; i++)
	{
		if (i == 9)
			continue;
		for (int j = 0; j < 11; j++)
		{
			A[i][j] = A[i][j] - A[9][j];
		}
	}
	for (int j = 0; j < 11; j++)
	{
		A[9][j] = 0;
	}

	double dis = 0;
	for (int i = 0; i < 11; i++)
		for (int j = 0; j < 11; j++)
		{
			dis = 0;
			for (int k = 0; k < 11; k++)
			{
				dis += SQR(A[i][k] - A[j][k]);
			}
			dis = sqrt(dis);
			B[i][j] = dis;
		}

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			ofile << B[i][j] << " ";
		}
		ofile << "\n";
	}
	ofile.close();
}
void CFormView3::Outputtestpuzzle(CMesh3d* pMesh)
{
	std::ofstream file1("D:\\plotpuzzle.inc");
	std::ofstream file2("D:\\plotpuzzleedges.inc");
	pMesh->plotPolyominoType1(1, 3, 3, file1, file2);
}
void CFormView3::AllNumbersAddOne()
{
	std::ifstream infile("D:\\111.txt");
	std::ofstream outfile("D:\\112.txt");
	int num;
	int num1 = 0;
	while (!infile.eof())
	{
		num1++;
		infile >> num;
		num++;
		outfile << num << " ";
		if (num1 % 3 == 0)
			outfile << "\n";
	}
	infile.close();
	outfile.close();
}
void CFormView3::ReadPoints(CMesh3d* pMesh)
{
	std::ifstream infile("E:\\workinsdu\\牙齿项目\\data\\Gumline_M&Pts\\VS_.pts");
	double data;
	while (!infile.eof())
	{
		infile >> data;
		pMesh->m_ArrayPointCoordinates.push_back(data);
	}
	infile.close();

	CVertex3d* pNewVertex = new CVertex3d();
	double dis;
	CVertex3d* pVertex;
	int minindex1;
	double minlen11;
	double dd;
	double mindd;
	int minindex2;
	for (int j = 0; j < pMesh->m_ArrayPointCoordinates.size(); j = j + 3)
	{
		pNewVertex->Set(pMesh->m_ArrayPointCoordinates[j], pMesh->m_ArrayPointCoordinates[j + 1], pMesh->m_ArrayPointCoordinates[j + 2]);
		minlen11 = 1e10;
		for (int i = 0; i < pMesh->NbVertex(); i++)
		{
			pVertex = pMesh->GetVertex(i);
			dis = Distance(pVertex, pNewVertex);
			if (dis < minlen11)
			{
				minlen11 = dis;
				minindex1 = i;
			}
		}

		CVertex3d* pNewVertex1 = pMesh->GetVertex(minindex1);
		mindd = 1e10;
		for (int i = 0; i < pNewVertex1->NbFaceNeighbor(); i++)
		{
			CFace3d* pFace = pNewVertex1->GetFaceNeighbor(i);
			Vector3D v1(pFace->v1()->x(), pFace->v1()->y(), pFace->v1()->z());
			Vector3D v2(pFace->v2()->x(), pFace->v2()->y(), pFace->v2()->z());
			Vector3D v3(pFace->v3()->x(), pFace->v3()->y(), pFace->v3()->z());

			Vector3D normal = (v2 - v1) ^ (v3 - v1);
			normal.SetUnit();

			Vector3D v(pMesh->m_ArrayPointCoordinates[j], pMesh->m_ArrayPointCoordinates[j + 1], pMesh->m_ArrayPointCoordinates[j + 2]);
			dd = (v - v1) * normal;
			if (dd < mindd)
			{
				mindd = dd;
				minindex2 = i;
			}

		}
		CFace3d* pFace1 = pNewVertex1->GetFaceNeighbor(minindex2);
		Vector3D v1(pFace1->v1()->x(), pFace1->v1()->y(), pFace1->v1()->z());
		Vector3D v2(pFace1->v2()->x(), pFace1->v2()->y(), pFace1->v2()->z());
		Vector3D v3(pFace1->v3()->x(), pFace1->v3()->y(), pFace1->v3()->z());

		Vector3D normal = (v2 - v1) ^ (v3 - v1);
		normal.SetUnit();

		Vector3D v(pMesh->m_ArrayPointCoordinates[j], pMesh->m_ArrayPointCoordinates[j + 1], pMesh->m_ArrayPointCoordinates[j + 2]);
		dd = (v - v1) * normal;

		v = v - dd * normal;
		pMesh->m_ArrayPointCoordinates[j] = v[0];
		pMesh->m_ArrayPointCoordinates[j + 1] = v[1];
		pMesh->m_ArrayPointCoordinates[j + 2] = v[2];
	}
	//ceshi
	/*CVertex3d* pNewVertex1 = pMesh->GetVertex(37233);
	//double dd;
	//double mindd = 1e10;
	for (int i = 0; i < pNewVertex1->NbFaceNeighbor(); i++)
	{
		CFace3d* pFace = pNewVertex1->GetFaceNeighbor(i);
		Vector3D v1(pFace->v1()->x(), pFace->v1()->y(), pFace->v1()->z());
		Vector3D v2(pFace->v2()->x(), pFace->v2()->y(), pFace->v2()->z());
		Vector3D v3(pFace->v3()->x(), pFace->v3()->y(), pFace->v3()->z());

		Vector3D normal = (v2 - v1) ^ (v3 - v1);
		normal.SetUnit();

		Vector3D v(pMesh->m_ArrayPointCoordinates[54], pMesh->m_ArrayPointCoordinates[55], pMesh->m_ArrayPointCoordinates[56]);
		dd = (v - v1) * normal;
		if (dd < mindd)
			mindd = dd;
	}*/


	//
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		pVertex = pMesh->GetVertex(i);
		pVertex->minimallength = 1e10;

		for (int j = 0; j < pMesh->m_ArrayPointCoordinates.size(); j = j + 3)
		{
			pNewVertex->Set(pMesh->m_ArrayPointCoordinates[j], pMesh->m_ArrayPointCoordinates[j + 1], pMesh->m_ArrayPointCoordinates[j + 2]);
			dis = Distance(pVertex, pNewVertex);
			if (dis < pVertex->minimallength)
			{
				minindex1 = j;
				pVertex->minimallength = dis;
			}
		}

		Vector3D v(pVertex->x(), pVertex->y(), pVertex->z());
		Vector3D vv(pMesh->m_ArrayPointCoordinates[minindex1], pMesh->m_ArrayPointCoordinates[minindex1 + 1], pMesh->m_ArrayPointCoordinates[minindex1 + 2]);
		Vector3D prevv, nextvv;
		int n = pMesh->m_ArrayPointCoordinates.size();
		prevv.SetValue(pMesh->m_ArrayPointCoordinates[(minindex1 - 3 + n) % n], pMesh->m_ArrayPointCoordinates[(minindex1 - 2 + n) % n], pMesh->m_ArrayPointCoordinates[(minindex1 - 1 + n) % n]);
		nextvv.SetValue(pMesh->m_ArrayPointCoordinates[(minindex1 + 3 + n) % n], pMesh->m_ArrayPointCoordinates[(minindex1 + 4 + n) % n], pMesh->m_ArrayPointCoordinates[(minindex1 + 5 + n) % n]);

		Vector3D vv1 = v - vv;
		Vector3D vv2 = prevv - vv;
		Vector3D vv3 = nextvv - vv;
		vv1.SetUnit();
		vv2.SetUnit();
		vv3.SetUnit();
		double n1 = vv1 * vv2;
		double n2 = vv1 * vv3;
		if (n1 > n2)
		{
			double nn = Length(v - vv);
			double nn1 = ((v - vv) * vv2);
			double nn2 = sqrt(nn * nn - nn1 * nn1);
			pVertex->minimallength = nn2;
			pVertex->m_ArrayPointOnSurface.push_back((minindex1 - 3 + n) % n);
			pVertex->m_ArrayPointOnSurface.push_back(minindex1);
		}
		else
		{
			double nn = Length(v - vv);
			double nn1 = ((v - vv) * vv3);
			double nn2 = sqrt(nn * nn - nn1 * nn1);
			pVertex->minimallength = nn2;
			pVertex->m_ArrayPointOnSurface.push_back((minindex1 + 3 + n) % n);
			pVertex->m_ArrayPointOnSurface.push_back(minindex1);
		}
	}

	int minindex;
	double minimallength = 1e10;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->m_Flag = i;
		//pVertex->minimallength = 1e10;

		if (pVertex->minimallength < minimallength)
		{
			minimallength = pVertex->minimallength;
			minindex = i;
		}
	}

	int currentpointonsurface_index;
	int nextpointonsurface_index;
	double minimaldis11 = 1e10;
	CVertex3d* pStartVertex1 = pMesh->GetVertex(minindex);
	for (int j = 0; j < pMesh->m_ArrayPointCoordinates.size(); j = j + 3)
	{
		pNewVertex->Set(pMesh->m_ArrayPointCoordinates[j], pMesh->m_ArrayPointCoordinates[j + 1], pMesh->m_ArrayPointCoordinates[j + 2]);
		dis = Distance(pStartVertex1, pNewVertex);
		if (dis < minimaldis11)
		{
			currentpointonsurface_index = j;
			minimaldis11 = dis;
			//pVertex->minimallength = dis;
		}
	}

	CVertex3d* preVertex = NULL;
	CVertex3d* pAdjVertex;
	int startvindex = minindex;
	int num = 0;
	while (1)
	{
		CVertex3d* pStartVertex = pMesh->GetVertex(minindex);
		//pStartVertex->m_Flag = 1;

		double minlength = 1e10;
		int minvindex;
		double tol = 0.1;
		double tol1 = 0.1;
		while (1)
		{
			for (int i = 0; i < pStartVertex->NbVertexNeighbor(); i++)
			{
				pAdjVertex = pStartVertex->GetVertexNeighbor(i);

				if (pAdjVertex == preVertex)
					continue;

				if (preVertex)
				{
					Vector3D prevv(preVertex->x(), preVertex->y(), preVertex->z());
					Vector3D vv(pStartVertex->x(), pStartVertex->y(), pStartVertex->z());
					Vector3D nextvv(pAdjVertex->x(), pAdjVertex->y(), pAdjVertex->z());
					Vector3D vv1 = vv - prevv;
					Vector3D vv2 = nextvv - vv;
					vv1.SetUnit();
					vv2.SetUnit();
					double val = vv1 * vv2;
					if (val < tol)
						continue;

					if (pAdjVertex->m_ArrayPointOnSurface.size() != 2)
						continue;

					nextpointonsurface_index = currentpointonsurface_index + 3;
					if (pAdjVertex->m_ArrayPointOnSurface[0] != currentpointonsurface_index &&
						pAdjVertex->m_ArrayPointOnSurface[1] != currentpointonsurface_index &&
						pAdjVertex->m_ArrayPointOnSurface[0] != nextpointonsurface_index &&
						pAdjVertex->m_ArrayPointOnSurface[1] != nextpointonsurface_index)
						continue;

					if (pAdjVertex->m_ArrayPointOnSurface[0] == currentpointonsurface_index + 3 &&
						pAdjVertex->m_ArrayPointOnSurface[1] == currentpointonsurface_index + 6 ||
						pAdjVertex->m_ArrayPointOnSurface[1] == currentpointonsurface_index + 3 &&
						pAdjVertex->m_ArrayPointOnSurface[0] == currentpointonsurface_index + 6)
						currentpointonsurface_index = currentpointonsurface_index + 3;

					/*if (pAdjVertex->m_ArrayPointOnSurface[0] == currentpointonsurface_index)
					{
						nextpointonsurface_index = pAdjVertex->m_ArrayPointOnSurface[1];
					}
					else
					{
						nextpointonsurface_index = pAdjVertex->m_ArrayPointOnSurface[0];
					}*/

					Vector3D curentvv(pMesh->m_ArrayPointCoordinates[currentpointonsurface_index], pMesh->m_ArrayPointCoordinates[currentpointonsurface_index + 1], pMesh->m_ArrayPointCoordinates[currentpointonsurface_index + 2]);
					Vector3D nextvv1(pMesh->m_ArrayPointCoordinates[(currentpointonsurface_index + 3) % pMesh->m_ArrayPointCoordinates.size()],
						pMesh->m_ArrayPointCoordinates[(currentpointonsurface_index + 3 + 1) % pMesh->m_ArrayPointCoordinates.size()], pMesh->m_ArrayPointCoordinates[(currentpointonsurface_index + 3 + 2) % pMesh->m_ArrayPointCoordinates.size()]);

					Vector3D vv3 = nextvv1 - curentvv;
					vv3.SetUnit();
					val = vv3 * vv2;
					if (val < tol1)
						continue;
				}


				if (pAdjVertex->minimallength < minlength)
				{
					minlength = pAdjVertex->minimallength;
					minvindex = pAdjVertex->m_Flag;
				}
			}

			if (minvindex != minindex)
				break;

			tol = tol - 0.01;
		}

		pMesh->m_ArraySharpEdges.push_back(std::make_pair(minindex, minvindex));
		if (minvindex != startvindex)
			pMesh->GetVertex(minvindex)->minimallength = 1e10;
		preVertex = pStartVertex;
		minindex = minvindex;
		if (minvindex == startvindex)
			break;
		num++;
		//if (num > 1000)
		//	break;
	}
}
void CFormView3::ModifyMesh(CMesh3d* pMesh)
{

	CVertex3d* pVertex1 = pMesh->GetVertex(67811);
	CVertex3d* pVertex2 = pMesh->GetVertex(150539);

	double dx = pVertex1->x() - pVertex2->x();
	double dy = pVertex1->y() - pVertex2->y();
	double dz = pVertex1->z() - pVertex2->z();

	CMesh3d_OpenMesh* pOpenMesh = pMesh->Convert_OpenMesh();
	std::vector<int> m_ArrayFaces1, m_ArrayFaces2;
	m_ArrayFaces1.push_back(67811);
	m_ArrayFaces2.push_back(150539);
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_vflag;

	//Vertex Flag
	if (!pOpenMesh->pmesh->get_property_handle(vprop_vertex_vflag, PROP_VERTEX_SELECTED))
		pOpenMesh->pmesh->add_property(vprop_vertex_vflag, PROP_VERTEX_SELECTED);

	for (MyMesh::VertexIter v_it = pOpenMesh->pmesh->vertices_begin(); v_it != pOpenMesh->pmesh->vertices_end(); ++v_it)
	{
		pOpenMesh->pmesh->property(vprop_vertex_vflag, v_it.handle()) = 0;
	}

	pOpenMesh->pmesh->property(vprop_vertex_vflag, pOpenMesh->pmesh->vertex_handle(67811)) = 1;

	pOpenMesh->pmesh->property(vprop_vertex_vflag, pOpenMesh->pmesh->vertex_handle(150539)) = 1;

	for (int i = 0; i < m_ArrayFaces1.size(); i++)
	{
		for (MyMesh::VertexVertexIter vv_it = pOpenMesh->pmesh->vv_iter(pOpenMesh->pmesh->vertex_handle(m_ArrayFaces1[i])); vv_it; ++vv_it)
		{
			if (pOpenMesh->pmesh->property(vprop_vertex_vflag, vv_it.handle()) == 0)
			{
				pOpenMesh->pmesh->property(vprop_vertex_vflag, vv_it.handle()) = 1;
				m_ArrayFaces1.push_back(vv_it.handle().idx());
			}
		}
	}
	for (int i = 0; i < m_ArrayFaces2.size(); i++)
	{
		for (MyMesh::VertexVertexIter vv_it = pOpenMesh->pmesh->vv_iter(pOpenMesh->pmesh->vertex_handle(m_ArrayFaces2[i])); vv_it; ++vv_it)
		{
			if (pOpenMesh->pmesh->property(vprop_vertex_vflag, vv_it.handle()) == 0)
			{
				pOpenMesh->pmesh->property(vprop_vertex_vflag, vv_it.handle()) = 2;
				m_ArrayFaces2.push_back(vv_it.handle().idx());
			}
		}
	}

	int size1 = m_ArrayFaces1.size();
	int size2 = m_ArrayFaces2.size();
	int size = pOpenMesh->pmesh->n_vertices();

	for (int i = 0; i < m_ArrayFaces2.size(); i++)
	{
		CVertex3d* pVertex2 = pMesh->GetVertex(m_ArrayFaces2[i]);
		double x = pVertex2->x();
		double y = pVertex2->y();
		double z = pVertex2->z();
		pVertex2->Set(x + dx, y + dy, z + dz);
	}
	pMesh->SetModified(1);

	pDoc->m_SceneGraph.Add(pOpenMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::GenerateHalfSphere()
{
	//x = a + Rcosu
	//y = b + Rsinucosv
	//z = c + Rsinusinv
	CMesh3d* pMesh = new CMesh3d();
	int numu, numv;
	numu = 100;
	numv = 100;
	double u, v;
	double x, y, z;
	double R = 10;
	CVertex3d* pVertex;
	std::vector<std::vector<CVertex3d*>> m_ArrayVertex;
	m_ArrayVertex.resize(numu + 1);
	for (int i = 0; i <= numu; i++)
	{
		m_ArrayVertex[i].resize(numv + 1);
	}
	for (int i = 0; i <= numu; i++)
	{
		for (int j = 0; j <= numv; j++)
		{
			u = PI * 2 * i * (1.0 / numu);
			v = PI * j * (1.0 / numv);
			x = R * cos(v);
			y = R * sin(v) * cos(u);
			z = R * sin(v) * sin(u) - 22;
			//x = u;
			//y = v;
			//z = 0;
			pVertex = new CVertex3d(x, y, z);
			pMesh->AddVertex(pVertex);
			m_ArrayVertex[i][j] = pVertex;
		}
	}
	CFace3d* pFace1, * pFace2;
	for (int i = 0; i < numu; i++)
	{
		for (int j = 0; j < numv; j++)
		{
			//(i,j) (i + 1,j) (i + 1, j+1)
			//(i,j) (i + 1,j +1) (i, j+1)
			pFace1 = new CFace3d(m_ArrayVertex[i][j], m_ArrayVertex[i + 1][j], m_ArrayVertex[i + 1][j + 1]);
			pFace2 = new CFace3d(m_ArrayVertex[i][j], m_ArrayVertex[i + 1][j + 1], m_ArrayVertex[i][j + 1]);
			pMesh->AddFace(pFace1);
			pMesh->AddFace(pFace2);
		}
	}
	pDoc->m_SceneGraph.Add(pMesh);
	pMesh->CalculateNormalPerFace();
	pMesh->CalculateNormalPerVertex();
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::GenerateSphereParaterdomain(CMesh3d* pMesh)
{
	//x = a + Rcosu
	//y = b + Rsinucosv
	//z = c + Rsinusinv

	Vector3D m_X(-1, 0, 0);
	Vector3D m_Z(0, 0, 1);
	Vector3D m_Y;
	m_Y = m_Z ^ m_X;

}
void CFormView3::OnSurfacePovray()
{
	//GenerateHalfSphere();
	//return;
	//AllNumbersAddOne();
	//return;
	//GenerateMatrix1();
	//return;
	//OutPutCircleTessellations();
	//return;
	//SetBunnyGeneralTransformation1();
	//OutPutMesh_Dotted_OneEdge_PovRay(0, 0, 0.7, 0.6, 55, "D:\\1.inc", "DOTTED_EDGE_TEX");
	//OutPutMesh_Dotted_OneEdge_PovRay(1, 0, 0.7, 0.6, 40, "D:\\2.inc", "DOTTED_EDGE_TEX");
	//OutPutMesh_Dotted_OneEdge_PovRay(1, 1, 0.7, 0.6, 30, "D:\\3.inc", "DOTTED_EDGE_TEX");
	//return;
	//TessellateBunnySurfaces();
	//return;
	//return;
	//ChangeColors();
	//return;
	//double angle = ComputeMinimalAngle();
	//TRACE("\nMinimal Angle: %f\n",angle);
	//return;
	//ProjectTriangulationOnSurfaces();
	//return;
	//OnCDT_PSLG();
	//return;
	//OutPutSTLMesh("D:\\mesh.stl",FALSE);
	//return;
	//OutPutMeshFaces_PovRay(10,10,"D:\\mesh.inc");
	//OutPutSurfaceBoundaryEdges_PovRay("D:\\meshiso.inc");
	//GeneratetThreeMesh_PovRay();
	//return;
	//return;
	//AssignLandmark();
	//return;
	//ExportConstraints();
	//return;
	//ExPortGraph();
	//ExPortGraph_Line();
	//ExtractSurface();
	//ExtractSurface_Sub();
	//return;
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	//TrimmingFreeformSurface();
	//return;
	//ComputeBoundaryRatio();

	//output the viewpoint config file.
	OutPutViewPointConfigFile(pDoc->m_SceneGraph.GetAt(currentindex), "D:\\viewpoint_config.inc");

	//output
	OutPutLightConfigFile(pDoc->m_SceneGraph.GetAt(currentindex), "D:\\light_config.inc");

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();
		int samplesm = 2;
		int samplesn = 2;
		CheckAreaPreserving(pSurf);
		((GeneralBilinearTransformation*)pNSurf->ptransformation)->pSurface = pNSurf;
		((GeneralBilinearTransformation*)pNSurf->ptransformation)->GetWeight_33();
		((GeneralBilinearTransformation*)pNSurf->ptransformation)->solveControlPoints();
		((GeneralBilinearTransformation*)pNSurf->ptransformation)->pNewSurface->SetTransform(*pNSurf->GetTransform());
		pDoc->m_SceneGraph.Add(((GeneralBilinearTransformation*)pNSurf->ptransformation)->pNewSurface);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		//return;
		ExtendSurface(pNSurf);
		//return;
		double val = ComputeEquiareality_NumericalMethod(pNSurf->GetTransformedSurface());
		char message[100];
		//sprintf(message,"%f",val);
		//AfxMessageBox(message);
		//return;
		//return;
		//SoomthFreeformsSurface(pNSurf);
		//return;
		//output four specific points on the surface.
		//OutputSpecificPointsOntheSurface(pSurf,"D:\\points.inc");

		//output 
		//OutputSubObjMesh(pNSurf,samplesm,samplesn,"D:\\meshsub.obj");
		//return;
		//output the mesh
		//OutPutSTLMesh("D:\\mesh.stl",FALSE);
		//SetBunnyGeneralTransformation();
		//OutPutConformity_PovRay(50,50,"D:\\meshconformality.inc");
		//OutPutSurfaceBoundaryEdges_PovRay("D:\\meshiso.inc");
		OutPutMeshFaces_PovRay(400, 400, "D:\\mesh.inc");
		//return;

		OutPutObjMesh(pNSurf, 100, 100, "D:\\mesh.obj", FALSE);

		//OutPutObjMesh(pNSurf,samplesm,samplesn,"D:\\parameterdomain.obj",TRUE);

		//OutPutControlNetwork_PovRay(pSurf,"D:\\controlnetwork.inc");

		//OutPutControlPoints_PovRay(pSurf,"D:\\controlpoints.inc");

		//OutPutMeshFaces_PovRay(pNSurf,500,500,"D:\\mesh.inc");

		OutPutMeshEdges_PovRay(pNSurf, 10, 10, "D:\\meshiso.inc");

		//OutPutMeshEdges_PovRay(pNSurf,10,10,"D:\\meshiso.inc");

		//OutPutTexture_PovRay(pNSurf,400,400,"D:\\surfacetexture.inc");

		//OutPutConformity_PovRay(pNSurf,500,500,"D:\\conformity.inc");
		//OutPutAreaFactor_PovRay(pSurf,1000,1000,"D:\\areapreserving.inc");
		//OutPutAreaFactor_Saved_PovRay(pSurf, 100, 100, "D:\\areapreservingoptimized.inc");
		OutPutAreaFactor_Savedmodified_PovRay(pSurf, 100, 100, "D:\\areapreservingmodified.inc");
		return;
		//return;
		//output the M file
		//OutPutMMesh(pNSurf,samplesm,samplesn,"D:\\papers\\conformalbin\\bin\\mesh.m");




		//return;
		//GenerateNewSurface(pNSurf);

		//return;

		//output the parameter domain
		//OutPutObjMesh(pNSurf,samplesm,samplesn,"D:\\parameterdomain.obj",TRUE);

		//output the mesh obj surface
		//OutPutObjMesh(pNSurf,samplesm,samplesn,"D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bezier\\facebad3dmesh.obj");

		//output the parameter domain
		//OutPutObjMesh(pNSurf,samplesm,samplesn,"D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\originalparameter.obj",TRUE);	

		//OutPutMesh_OneEdge_PovRay(pNSurf,0,0,1,0,300,"D:\\e0.inc","edge_tex21");
		//OutPutMesh_OneEdge_PovRay(pNSurf,1,0,1,1,300,"D:\\e1.inc","edge_tex21");
		//OutPutMesh_OneEdge_PovRay(pNSurf,1,1,0,1,300,"D:\\e2.inc","edge_tex21");
		//OutPutMesh_OneEdge_PovRay(pNSurf,0,1,0,0,300,"D:\\e3.inc","edge_tex21");

		return;


		//OutPutMesh_OneEdge_PovRay(pNSurf,0,0,1,0,300,"D:\\1_.inc","edge_tex11");
		//OutPutMesh_OneEdge_PovRay(pNSurf,1,0,1,0.5,300,"D:\\2_.inc","edge_tex12");
		//OutPutMesh_OneEdge_PovRay(pNSurf,1,0.5,1,1,300,"D:\\3_.inc","edge_tex13");
		//OutPutMesh_OneEdge_PovRay(pNSurf,1,1,0,1,300,"D:\\4_.inc","edge_tex14");
		//OutPutMesh_OneEdge_PovRay(pNSurf,0,1,0,0.5,300,"D:\\5_.inc","edge_tex15");
		//OutPutMesh_OneEdge_PovRay(pNSurf,0,0.5,0,0,300,"D:\\6_.inc","edge_tex16");
		//OutPutMesh_OneEdge_PovRay(pNSurf,0,0.5,1,0.5,300,"D:\\7_.inc","edge_tex17");
		//OutPutMesh_OneEdge_PovRay(pNSurf,1.0/8,0,1.0/8,1,300,"D:\\8.inc","edge_tex7");
		return;



		//OutPutGeneralBilinearTransformedEdges_PovRay(pNSurf,10,10,"D:\\transformedmeshiso.inc");

		//OutPutGeneralBilinearTransformedEdges_obj(pNSurf,10,10,"D:\\transformedmeshiso.obj");

		//OutPutQuadMeshEdges_PovRay(pNSurf,10,10,"D:\\meshisoquadmesh.inc");

		//OutPutQuadMeshEdges_PovRay(pNSurf,10,10,"D:\\meshiso.inc");

		//OutPutMeshSavedEdges_PovRay(pNSurf,samplesm,samplesn,"D:\\savedmeshiso.inc");

		return;

		OutPutTexture_Optimization_PovRay(pSurf, samplesm, samplesn, "d:\\surfaceoptimaltexture.inc");

		OutPutTexture_Optimization_Moddified_PovRay(pSurf, samplesm, samplesn, "d:\\surfaceoptimalmodifiedtexture.inc");

		OutPutMeshSavedEdgesModifided_PovRay(pNSurf, samplesm, samplesn, "D:\\savedmeshisomodified.inc");


	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);


		//ModifyMesh(pMesh);
		//ReadPoints(pMesh);
		//return;
		//OutPutMeshTexture("D:\\meshtexture.inc");
		//Outputtestpuzzle(pMesh);
		//pMesh->PlotPolyominoes_world();
		//return;
		//CheckIsolatedVertices(pMesh);
		//SetMeshTextureColor(pMesh);
		//RotateMesh(pMesh);
		//return;
		//OutPutMeshTexture_PovRay(pMesh, "D:\\meshtexture.inc");
		//return;
		//GetCorrespondingLocation();
		//return;
		//OutPutPovrayPuzzle(pMesh);
		//return;
		//ExportPolyomino(pMesh,"D:\\polyomino.inc");
		//OutPutSharpEdges_PovRay(pMesh, "D:\\mesh_sharpedges.inc");
		//OutPutSharpVertices_PovRay(pMesh, "D:\\mesh_sharpvertices.inc");
		//OutPutSharpVertices_PovRay_Anchor(pMesh, "D:\\mesh_sharpvertices.inc");
	//	OutPutSharpVertices_PovRay1(pMesh, "D:\\mesh_sharpvertices1.inc");
		//OutPutSharpVertices_PovRay2(pMesh, "D:\\mesh_sharpvertices2.inc");

		OutPutMeshFaces_PovRay_Smooth(pMesh, "D:\\meshsmooth.inc");
		//OutPutMeshFaces_PovRay_Smooth_Anchor(pMesh, "D:\\meshsmooth.inc");
		OutPutMeshEdges_PovRay(pMesh, "D:\\mesh_edges.inc");
		//OutPutMeshEdges_PovRay_Anchor(pMesh, "D:\\mesh_edges.inc");
		//OutPutSharpEdges_PovRay(pMesh,"D:\\mesh_sharpedges.inc");
		//OutPutLandmarkEdges_PovRay_1(pMesh,"D:\\mesh_landmarkedges_usa.inc");
		//OutPutMeshVertices_PovRay(pMesh, "D:\\vertices.inc");
		//pMesh->SaveOBJFile("d:\\1.obj");
		//pMesh->SaveBoundingBoxOBJFile("d:\\b1.obj");
		//OutPutMu_PovRay(pMesh, "D:\\meshmu.inc");
		return;
		//pMesh->SaveMFile();
		//return; 
		//

		//ExPortGraph_Line_SelectedEdges(pMesh,"D:\\edges.inc");
		//OutPutMeshFaces_PovRay(pMesh,"D:\\mesh.inc");
		//
		outPutMeshBoundaryEdges_PovRay(pMesh, "D:\\meshboundary.inc");
		//return;
		//outPutMeshBoundaryEdges_PovRay_bone(pMesh,"D:\\meshboundary.inc");
		//OutPutMeshEdges_PovRay(pMesh,"D:\\mesh_edges.inc");
		//OutPutChainEdges_PovRay(pMesh,"D:\\mesh_chainedges.inc");
		//OutPutChainEndVertexs_PovRay(pMesh,"D:\\mesh_chainendvertexs.inc");
		//OutPutChainEdges_PovRay_two(pMesh,"D:\\mesh_chainendvertexs.inc");
		//OutPutMeshTexture_PovRay(pMesh,"D:\\meshtexture.inc");

		//OutPutMeshQuadEdges_PovRay(pMesh,"D:\\mesh_quadedges.inc");
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh_OpenMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);

		OutPutMeshFaces_PovRay_Smooth_nonflip(pMesh_OpenMesh);

		CMesh3d* pMesh = pMesh_OpenMesh->ConvertMesh3d();
		pMesh->m_LandMarkArray = pMesh_OpenMesh->m_LandMarkArray;
		OutPutMeshFaces_PovRay_Smooth(pMesh, "D:\\meshsmooth.inc");
		OutPutMeshEdges_PovRay(pMesh, "D:\\mesh_edges.inc");
		//OutPutSharpEdges_PovRay(pMesh,"D:\\mesh_sharpedges.inc");
		OutPutLandmarkEdges_PovRay(pMesh, "D:\\mesh_landmarkedges.inc");
		OutPutMeshVertices_PovRay(pMesh, "D:\\vertices.inc");
	}

	pDoc->UpdateAllViews(NULL);
}


void CFormView3::OutPutMeshSavedEdgesModifided_PovRay(CNurbsSuface* pNurbsSurface, int samplesm, int samplesn, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	//Vector3D vv1 = pNurbsSurface->ComputPosition(0,0) - ZeroP3D;
	//Vector3D vv2 = pNurbsSurface->ComputPosition(1,1) - ZeroP3D;

	//outfile << "sphere {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,r_edge texture { edge_tex } }" << endl;
	//outfile << "sphere {<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " >,r_edge texture { edge_tex } }" << endl;

	samplesm = samplesm * 30;
	for (j = 0; j < pNurbsSurface->varraymodified.size(); j++)
	{
		for (i = 0; i < samplesm; i++)
		{	//
			double s1 = (double)i / samplesm;
			double s2 = (double)(i + 1) / samplesm;
			double t = pNurbsSurface->varraymodified[j];


			double u1 = s1;
			double v1 = t;

			double u2 = s2;
			double v2 = t;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1, v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2, v2) - ZeroP3D);

			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	samplesm = samplesm / 30;
	samplesn = samplesn * 30;

	for (i = 0; i < pNurbsSurface->uarraymodified.size(); i++)
	{
		for (j = 0; j < samplesn; j++)
		{	//
			double s = pNurbsSurface->uarraymodified[i];
			double t1 = (double)j / samplesn;
			double t2 = (double)(j + 1) / samplesn;

			double u1 = s;
			double v1 = t1;

			double u2 = s;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1, v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2, v2) - ZeroP3D);

			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	outfile.close();
}
void CFormView3::OutPutControlNetwork_PovRay(C_NurbsSurf* pNurbsSurface, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	for (int i = 0; i < pNurbsSurface->GetCtrlNumberU(); i++)
		for (int j = 0; j < pNurbsSurface->GetCtrlNumberV() - 1; j++)
		{
			Vector3D vv1 = pNurbsSurface->GetControlPoint(i, j) - ZeroP3D;
			Vector3D vv2 = pNurbsSurface->GetControlPoint(i, j + 1) - ZeroP3D;

			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex_net } }" << endl;
		}
	for (int i = 0; i < pNurbsSurface->GetCtrlNumberV(); i++)
		for (int j = 0; j < pNurbsSurface->GetCtrlNumberU() - 1; j++)
		{
			Vector3D vv1 = pNurbsSurface->GetControlPoint(j, i) - ZeroP3D;
			Vector3D vv2 = pNurbsSurface->GetControlPoint(j + 1, i) - ZeroP3D;

			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex_net } }" << endl;
		}
	outfile.close();
}
void CFormView3::OutPutControlPoints_PovRay(C_NurbsSurf* pNurbsSurface, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	for (int i = 0; i < pNurbsSurface->GetCtrlNumberU(); i++)
		for (int j = 0; j < pNurbsSurface->GetCtrlNumberV(); j++)
		{
			Vector3D v1 = pNurbsSurface->GetControlPoint(i, j) - ZeroP3D;

			outfile << "sphere{<" << v1.x << " , " << v1.y << " , " << -v1.z << ">,spherer texture { point_tex } }" << endl;
		}

	outfile.close();
}
void CFormView3::OutPutMeshQuadEdges_PovRay(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	for (int i = 0; i < pMesh->NbFace(); i = i + 2)
	{
		CFace3d* pFace1 = pMesh->GetFace(i);
		CFace3d* pFace2 = pMesh->GetFace(i + 1);
		CVertex3d* pVertexArray[6];

		pVertexArray[0] = pFace1->v1();
		pVertexArray[1] = pFace1->v2();
		pVertexArray[2] = pFace1->v3();
		pVertexArray[3] = pFace2->v1();
		pVertexArray[4] = pFace2->v2();
		pVertexArray[5] = pFace2->v3();

		int index1 = -1;
		int index2 = -1;
		for (int i = 0; i < 3; i++)
		{
			if (pVertexArray[i] != pVertexArray[3] && pVertexArray[i] != pVertexArray[4] && pVertexArray[i] != pVertexArray[5])
			{
				index1 = i;
				break;
			}
		}
		for (int i = 3; i < 6; i++)
		{
			if (pVertexArray[i] != pVertexArray[0] && pVertexArray[i] != pVertexArray[1] && pVertexArray[i] != pVertexArray[2])
			{
				index2 = i;
				break;
			}
		}
		outfile << "cylinder {<" << pVertexArray[index1]->x() << " , " << pVertexArray[index1]->y() << " , " << -pVertexArray[index1]->z() << " >,<" << pVertexArray[(index1 + 1) % 3]->x() << " , " << pVertexArray[(index1 + 1) % 3]->y() << " , " << -pVertexArray[(index1 + 1) % 3]->z() << " > ,r_edge texture { edge_tex } }" << endl;
		outfile << "cylinder {<" << pVertexArray[(index1 + 1) % 3]->x() << " , " << pVertexArray[(index1 + 1) % 3]->y() << " , " << -pVertexArray[(index1 + 1) % 3]->z() << " >,<" << pVertexArray[index2]->x() << " , " << pVertexArray[index2]->y() << " , " << -pVertexArray[index2]->z() << " > ,r_edge texture { edge_tex } }" << endl;
		outfile << "cylinder {<" << pVertexArray[index2]->x() << " , " << pVertexArray[index2]->y() << " , " << -pVertexArray[index2]->z() << " >,<" << pVertexArray[(index1 + 2) % 3]->x() << " , " << pVertexArray[(index1 + 2) % 3]->y() << " , " << -pVertexArray[(index1 + 2) % 3]->z() << " > ,r_edge texture { edge_tex } }" << endl;
		outfile << "cylinder {<" << pVertexArray[index1]->x() << " , " << pVertexArray[index1]->y() << " , " << -pVertexArray[index1]->z() << " >,<" << pVertexArray[(index1 + 2) % 3]->x() << " , " << pVertexArray[(index1 + 2) % 3]->y() << " , " << -pVertexArray[(index1 + 2) % 3]->z() << " > ,r_edge texture { edge_tex } }" << endl;
	}
	outfile.close();
}
void CFormView3::OutPutChainEndVertexs_PovRay(CMesh3d* pMesh, char* filename)
{
	/*std::vector<int> m_ArrayEnd;
	int ii;
	for(int i = 0; i < pMesh->m_ArraySeletedEdges.size(); i++)
	{
		std::pair<int,int> pair;
		pair = pMesh->m_ArraySeletedEdges[i];

		for(ii = 0; ii < pMesh->m_ArraySeletedEdges.size(); ii++)
		{
			if(ii == i)
				continue;
			std::pair<int,int> pair1 = pMesh->m_ArraySeletedEdges[ii];
			if(pair1.first == pair.first || pair1.second == pair.first)
				break;
		}
		if(ii == pMesh->m_ArraySeletedEdges.size())
		{
			m_ArrayEnd.push_back(pair.first);
		}
		for(ii = 0; ii < pMesh->m_ArraySeletedEdges.size(); ii++)
		{
			if(ii == i)
				continue;
			std::pair<int,int> pair1 = pMesh->m_ArraySeletedEdges[ii];
			if(pair1.first == pair.second || pair1.second == pair.second)
				break;
		}
		if(ii == pMesh->m_ArraySeletedEdges.size())
		{
			m_ArrayEnd.push_back(pair.second);
		}
	}*/

	std::ofstream outfile(filename);
	//5762 5235 4954 837  
	//4628 9614
	//3540 14088
	//10286 16267 811 3405 10694 3141
	pMesh->m_LandMarkEndPoints.push_back(10286);
	pMesh->m_LandMarkEndPoints.push_back(16267);
	pMesh->m_LandMarkEndPoints.push_back(811);
	pMesh->m_LandMarkEndPoints.push_back(3405);
	pMesh->m_LandMarkEndPoints.push_back(10694);
	pMesh->m_LandMarkEndPoints.push_back(3141);
	for (int i = 0; i < pMesh->m_LandMarkEndPoints.size(); i++)
		//for(int i = 0; i < m_ArrayEnd.size(); i++)
	{
		int index = pMesh->m_LandMarkEndPoints[i];
		CVertex3d* pVertex = pMesh->GetVertex(index);
		outfile << "sphere{<" << pVertex->x() << " , " << pVertex->y() << " , " << -pVertex->z() << ">,spherer texture { point_tex }}" << endl;
	}
	outfile.close();
}

void CFormView3::outPutMeshBoundaryEdges_PovRay_bone(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);

	int m_array[65] = {
		25,29,33,37,41,45,49,53,57,61,
		65,69,73,77,81,85,89,93,97,101,
		105,109,113,117,121,125,129,133,137,141,
		145,149,153,157,161,164,168,172,176,180,
		184,188,192,196,200,204,208,212,216,219,
		222,225,228,231,234,237,240,243,245,247,
		249,251,253,255,257 };

	int flag1, flag2, flag3;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		Vector3D v1(pVertex1->x(), pVertex1->y(), pVertex1->z());
		Vector3D v2(pVertex2->x(), pVertex2->y(), pVertex2->z());
		Vector3D v3(pVertex3->x(), pVertex3->y(), pVertex3->z());

		flag1 = 0;
		flag2 = 0;
		flag3 = 0;

		for (int ii = 0; ii < 65; ii++)
		{
			CVertex3d* pVertex4 = pMesh->GetVertex(m_array[ii]);
			Vector3D v4(pVertex4->x(), pVertex4->y(), pVertex4->z());

			if (Length(v1 - v4) < 1e-5)
				flag1 = 1;
			if (Length(v2 - v4) < 1e-5)
				flag2 = 1;
			if (Length(v3 - v4) < 1e-5)
				flag3 = 1;
		}

		if (pVertex1->IsOnBoundary() && pVertex2->IsOnBoundary() && !flag1 && !flag2)
		{
			outfile << "cylinder {<" << pVertex1->x() << "," << pVertex1->y() << "," << -pVertex1->z() << " >,<" << pVertex2->x() << "," << pVertex2->y() << "," << -pVertex2->z() << " > ,r_edge texture { boundaryedge_tex } }" << endl;
		}
		if (pVertex2->IsOnBoundary() && pVertex3->IsOnBoundary() && !flag2 && !flag3)
		{
			outfile << "cylinder {<" << pVertex2->x() << "," << pVertex2->y() << "," << -pVertex2->z() << " >,<" << pVertex3->x() << "," << pVertex3->y() << "," << -pVertex3->z() << " > ,r_edge texture { boundaryedge_tex } }" << endl;
		}
		if (pVertex3->IsOnBoundary() && pVertex1->IsOnBoundary() && !flag1 && !flag3)
		{
			outfile << "cylinder {<" << pVertex1->x() << "," << pVertex1->y() << "," << -pVertex1->z() << " >,<" << pVertex3->x() << "," << pVertex3->y() << "," << -pVertex3->z() << " > ,r_edge texture { boundaryedge_tex } }" << endl;
		}
	}
	outfile.close();
}


void CFormView3::outPutMeshBoundaryEdges_PovRay(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);


	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();


		if (pVertex1->IsOnBoundary() && pVertex2->IsOnBoundary())
		{
			outfile << "cylinder {<" << pVertex1->x() << "," << pVertex1->y() << "," << -pVertex1->z() << " >,<" << pVertex2->x() << "," << pVertex2->y() << "," << -pVertex2->z() << " > ,r_edge texture { boundaryedge_tex } }" << endl;
		}
		if (pVertex2->IsOnBoundary() && pVertex3->IsOnBoundary())
		{
			outfile << "cylinder {<" << pVertex2->x() << "," << pVertex2->y() << "," << -pVertex2->z() << " >,<" << pVertex3->x() << "," << pVertex3->y() << "," << -pVertex3->z() << " > ,r_edge texture { boundaryedge_tex } }" << endl;
		}
		if (pVertex3->IsOnBoundary() && pVertex1->IsOnBoundary())
		{
			outfile << "cylinder {<" << pVertex1->x() << "," << pVertex1->y() << "," << -pVertex1->z() << " >,<" << pVertex3->x() << "," << pVertex3->y() << "," << -pVertex3->z() << " > ,r_edge texture { boundaryedge_tex } }" << endl;
		}
	}
	outfile.close();
}

void CFormView3::OutPutChainEdges_PovRay(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int, int> pair;
		pair = pMesh->m_ArraySharpEdges[i];
		//if(pair.first == 224 || pair.second == 224)
		//	continue;
		//if(pair.first == 14869 || pair.second == 14869)
		//	continue;
		//if(pair.first == 147 || pair.second == 147)
		//	continue;
		//if(pair.first == 6198 || pair.second == 6198)
		//	continue;
		if (pair.first == 4645 || pair.second == 4645)
			continue;
		if (pair.first == 138 || pair.second == 138)
			continue;
		if (pair.first == 251 || pair.second == 251)
			continue;
		if (pair.first == 43 || pair.second == 43)
			continue;
		CVertex3d* pVertex1 = pMesh->m_ArrayVertex[pair.first];
		CVertex3d* pVertex2 = pMesh->m_ArrayVertex[pair.second];

		outfile << "cylinder {<" << pVertex1->x() << "," << pVertex1->y() << "," << -pVertex1->z() << " >,<" << pVertex2->x() << "," << pVertex2->y() << "," << -pVertex2->z() << " > ,r_edge texture { edge_tex } }" << endl;
	}
	outfile.close();
}
void CFormView3::OutPutSharpEdges_PovRay(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		CVertex3d* pStartVertex = pMesh->GetVertex(pair.first);
		CVertex3d* pEndVertex = pMesh->GetVertex(pair.second);
		double x1 = pStartVertex->x();
		double y1 = pStartVertex->y();
		double z1 = pStartVertex->z();
		double x2 = pEndVertex->x();
		double y2 = pEndVertex->y();
		double z2 = pEndVertex->z();
		if (SQR(x1 - x2) + SQR(y1 - y2) + SQR(z1 - z2) > 1e-6)
			outfile << "cylinder {<" << pStartVertex->x() << " , " << pStartVertex->y() << " , " << -pStartVertex->z() << " >,<" << pEndVertex->x() << " , " << pEndVertex->y() << " , " << -pEndVertex->z() << " > ,r_sharpedge texture { sharpedge_tex } }" << endl;
	}
	outfile.close();
}
void CFormView3::OutPutLandmarkEdges_PovRay_1(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size() - 1; j++)
		{
			int index1 = pLandmark->landmark_points[j];
			int index2 = pLandmark->landmark_points[j + 1];
			CVertex3d* pStartVertex = pMesh->GetVertex(index1);
			CVertex3d* pEndVertex = pMesh->GetVertex(index2);
			double x1 = pStartVertex->x();
			double y1 = pStartVertex->y();
			double z1 = pStartVertex->z();
			double x2 = pEndVertex->x();
			double y2 = pEndVertex->y();
			double z2 = pEndVertex->z();

			if (SQR(x1 - x2) + SQR(y1 - y2) + SQR(z1 - z2) > 1e-8)
				outfile << "cylinder {<" << x1 << ", " << y1 << ", " << -z1 << " >,<" << x2 << ", " << y2 << " , " << -z2 << " > ,r_sharpedge texture { sharpedge_tex" << pLandmark->style << " } }" << endl;
		}
	}
	outfile.close();
}
void CFormView3::OutPutLandmarkEdges_PovRay(CMesh3d* pMesh, char* filename)
{
	/*Vector3D topleft(0.08769, 0.859124, 0);
	Vector3D topright(0.461452, 0.91231, 0);
	Vector3D bottomleft(0.29944, 0.090761, 0);
	Vector3D bottomright(0.840848, 0.170680, 0);

	Vector3D center = (topleft + topright + bottomleft + bottomright) / 4;
	Vector3D topleft1(0.699278, 0.257253, 0);
	Vector3D topright1(0.744299, 0.263527, 0);
	Vector3D bottomleft1(0.724801, 0.164877, 0);
	Vector3D bottomright1(0.788265, 0.174461, 0);
	Vector3D center1 = (topleft1 + topright1 + bottomleft1 + bottomright1) / 4;

	Vector3D offset = (center - center1);
	Vector3D newbottomright1 = bottomright1 + offset;
	double scalex = (bottomright.x - center.x) / (newbottomright1.x - center.x);
	double scaley = (bottomright.y - center.y) / (newbottomright1.y - center.y);

	Vector3D newbottomleft1 = bottomleft1 + offset;
	newbottomleft1.x = (newbottomleft1.x - center.x) * scalex + center.x;
	newbottomleft1.y = (newbottomleft1.y - center.y) * scaley + center.y;
	double theta = -0 * 3.1415926 / 180;
	double costheta = cos(theta);
	double sintheta = sin(theta);*/
	/*Vector3D topleft(0.598145, 0.41411, 0);
	Vector3D topright(0.644261, 0.417258, 0);
	Vector3D bottomleft(0.631057, 0.294717, 0);
	Vector3D bottomright(0.649537, 0.307091, 0);

	Vector3D center = (topleft + topright + bottomleft + bottomright) / 4;
	Vector3D topleft1(0.152041, 0.89979, 0);
	Vector3D topright1(0.473518, 0.926115, 0);
	Vector3D bottomleft1(0.385156, 0.073147, 0);
	Vector3D bottomright1(0.513238, 0.166069, 0);
	Vector3D center1 = (topleft1 + topright1 + bottomleft1 + bottomright1) / 4;

	Vector3D offset = (center - center1);
	Vector3D newbottomright1 = bottomright1 + offset;
	double scalex = (bottomright.x - center.x) / (newbottomright1.x - center.x);
	double scaley = (bottomright.y - center.y) / (newbottomright1.y - center.y);

	Vector3D newbottomleft1 = bottomleft1 + offset;
	newbottomleft1.x = (newbottomleft1.x - center.x) * scalex + center.x;
	newbottomleft1.y = (newbottomleft1.y - center.y) * scaley + center.y;
	double theta = -0 * 3.1415926 / 180;
	double costheta = cos(theta);
	double sintheta = sin(theta);*/
	Vector3D topleft(0.599635, 0.43786, 0);
	Vector3D topright(0.650303, 0.437696, 0);
	Vector3D bottomleft(0.626127, 0.316449, 0);
	Vector3D bottomright(0.64844, 0.326624, 0);

	Vector3D center = (topleft + bottomright) / 2;
	Vector3D topleft1(0.174541, 0.913336, 0);
	Vector3D topright1(0.521944, 0.913336, 0);
	Vector3D bottomleft1(0.351795, 0.082826, 0);
	Vector3D bottomright1(0.506593, 0.156064, 0);
	Vector3D center1 = (topleft1 + bottomright1) / 2;

	Vector3D offset = (center - center1);
	Vector3D newbottomright1 = bottomright1 + offset;
	double scalex = (bottomright.x - center.x) / (newbottomright1.x - center.x);
	double scaley = (bottomright.y - center.y) / (newbottomright1.y - center.y);
	scalex = scalex * 0.98;
	scaley = scaley * 0.98;
	Vector3D newbottomleft1 = bottomleft1 + offset;
	newbottomleft1.x = (newbottomleft1.x - center.x) * scalex + center.x;
	newbottomleft1.y = (newbottomleft1.y - center.y) * scaley + center.y;
	double theta = -0 * 3.1415926 / 180;
	double costheta = cos(theta);
	double sintheta = sin(theta);
	std::ofstream outfile(filename);
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size() - 1; j++)
		{
			int index1 = pLandmark->landmark_points[j];
			int index2 = pLandmark->landmark_points[j + 1];
			CVertex3d* pStartVertex = pMesh->GetVertex(index1);
			CVertex3d* pEndVertex = pMesh->GetVertex(index2);
			double x1 = pStartVertex->x();
			double y1 = pStartVertex->y();
			double z1 = pStartVertex->z();
			double x2 = pEndVertex->x();
			double y2 = pEndVertex->y();
			double z2 = pEndVertex->z();

			x1 = x1 + offset.x;
			y1 = y1 + offset.y;
			x1 = (x1 - center.x) * scalex + center.x;
			y1 = (y1 - center.y) * scaley + center.y;

			x1 = (x1 - center.x) * costheta - (y1 - center.y) * sintheta + center.x;
			y1 = (x1 - center.x) * sintheta + (y1 - center.y) * costheta + center.y;

			x2 = x2 + offset.x;
			y2 = y2 + offset.y;
			x2 = (x2 - center.x) * scalex + center.x;
			y2 = (y2 - center.y) * scaley + center.y;

			x2 = (x2 - center.x) * costheta - (y2 - center.y) * sintheta + center.x;
			y2 = (x2 - center.x) * sintheta + (y2 - center.y) * costheta + center.y;

			outfile << "cylinder {<" << x1 << "+xoffset , " << y1 << "+yoffset , " << -z1 << " >,<" << x2 << "+xoffset , " << y2 << " +yoffset, " << -z2 << " > ,r_sharpedge texture { sharpedge_tex" << pLandmark->style << " } }" << endl;

			//if(fabs(y1 - y2) < fabs(x1 - x2))
			//	outfile << "cylinder {<" << x1 << " , " << (y1 + y2) / 2  << " , " << -z1 << " >,<" << x2 << " , " << (y1 + y2) / 2 << " , " << -z2 << " > ,r_horizontal_sharpedge texture { horizontal_sharpedge_tex } }" << endl;
			//else
			//	outfile << "cylinder {<" << (x1 + x2) / 2 << " , " << y1 << " , " << -z1 << " >,<" << (x1 + x2) / 2 << " , " << y2 << " , " << -z2 << " > ,r_vertical_sharpedge texture { vertical_sharpedge_tex } }" << endl;
			//::glVertex3f(pMesh->m_ArrayVertex[index1]->x(),m_ArrayVertex[index1]->y(),m_ArrayVertex[index1]->z());
			//::glVertex3f(pMesh->m_ArrayVertex[index2]->x(),m_ArrayVertex[index2]->y(),m_ArrayVertex[index2]->z());
		}
		//pLandmark->landmark_points[0]
		//CVertex3d* pStartVertex = pMesh->GetVertex(pair.first);
		//CVertex3d* pEndVertex = pMesh->GetVertex(pair.second);
		//double x1 = pStartVertex->x();
		//double y1 = pStartVertex->y();
		//double z1 = pStartVertex->z();
		//double x2 = pEndVertex->x();
		//double y2 = pEndVertex->y();
		//double z2 = pEndVertex->z();
		//if(SQR(x1 - x2) + SQR(y1 - y2) + SQR(z1 - z2)  > 1e-6)
		//	outfile << "cylinder {<" << pStartVertex->x() << " , " << pStartVertex->y() << " , " << -pStartVertex->z() << " >,<" << pEndVertex->x() << " , " << pEndVertex->y() << " , " << -pEndVertex->z() << " > ,r_sharpedge texture { sharpedge_tex } }" << endl;
	}
	outfile.close();
}
void CFormView3::OutPutSharpVertices_PovRay1(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	std::pair<int, int> pair;
	std::vector<int> m_ArrayFlag;
	m_ArrayFlag.resize(pMesh->m_ArrayVertex.GetSize(), 0);

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		if (!pMesh->m_LandMarkArray[i]->isVisible)
			continue;

		int index1 = pMesh->m_LandMarkArray[i]->landmark_points[0];
		int index2 = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
		m_ArrayFlag[index1]++;
		m_ArrayFlag[index2]++;
		//::glVertex3f(pMesh->m_ArrayVertex[index1]->x(),pMesh->m_ArrayVertex[index1]->y(),pMesh->m_ArrayVertex[index1]->z());
		//::glVertex3f(pMesh->m_ArrayVertex[index2]->x(),pMesh->m_ArrayVertex[index2]->y(),pMesh->m_ArrayVertex[index2]->z());
	}

	for (int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		if (m_ArrayFlag[i] > 2)
		{
			CVertex3d* pVertex = pMesh->m_ArrayVertex[i];
			outfile << "sphere{<" << pVertex->x() << " , " << pVertex->y() << " , " << -pVertex->z() << ">,spherer texture { vertex_tex }}" << endl;
		}
	}
	/*for(int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		CVertex3d* pStartVertex = pMesh->GetVertex(pair.first);
		CVertex3d* pEndVertex = pMesh->GetVertex(pair.second);

		outfile << "sphere{<" <<pStartVertex->x()<<" , "<<pStartVertex->y()<<" , " << -pStartVertex->z()<<">,spherer texture { vertex_tex }}"<<endl;
		outfile << "sphere{<" <<pEndVertex->x()<<" , "<<pEndVertex->y()<<" , " << -pEndVertex->z()<<">,spherer texture { vertex_tex }}"<<endl;
		//outfile << "cylinder {<" << pStartVertex->x() << " , " << pStartVertex->y() << " , " << -pStartVertex->z() << " >,<" << pEndVertex->x() << " , " << pEndVertex->y() << " , " << -pEndVertex->z() << " > ,r_sharpedge texture { sharpedge_tex } }" << endl;
	}*/
	outfile.close();
}
void CFormView3::OutPutSharpVertices_PovRay2(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		if (!pLandmark->isVisible)
			continue;

		for (int j = 0; j < pLandmark->landmark_points.size(); j++)
		{
			int index = pLandmark->landmark_points[j];
			outfile << "sphere{<" << pMesh->m_ArrayVertex[index]->x() << " , " << pMesh->m_ArrayVertex[index]->y() << " , " << -pMesh->m_ArrayVertex[index]->z() << ">,spherer texture { vertex_tex }}" << endl;
		}
	}
	/*for(int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
	pair = pMesh->m_ArraySharpEdges[i];
	CVertex3d* pStartVertex = pMesh->GetVertex(pair.first);
	CVertex3d* pEndVertex = pMesh->GetVertex(pair.second);

	outfile << "sphere{<" <<pStartVertex->x()<<" , "<<pStartVertex->y()<<" , " << -pStartVertex->z()<<">,spherer texture { vertex_tex }}"<<endl;
	outfile << "sphere{<" <<pEndVertex->x()<<" , "<<pEndVertex->y()<<" , " << -pEndVertex->z()<<">,spherer texture { vertex_tex }}"<<endl;
	//outfile << "cylinder {<" << pStartVertex->x() << " , " << pStartVertex->y() << " , " << -pStartVertex->z() << " >,<" << pEndVertex->x() << " , " << pEndVertex->y() << " , " << -pEndVertex->z() << " > ,r_sharpedge texture { sharpedge_tex } }" << endl;
	}*/
	outfile.close();
}
void CFormView3::OutPutSharpVertices_PovRay(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		if (!pMesh->m_LandMarkArray[i]->isVisible)
			continue;

		int index1 = pMesh->m_LandMarkArray[i]->landmark_points[0];
		int index2 = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
		//::glVertex3f(pMesh->m_ArrayVertex[index1]->x(),pMesh->m_ArrayVertex[index1]->y(),pMesh->m_ArrayVertex[index1]->z());
		//::glVertex3f(pMesh->m_ArrayVertex[index2]->x(),pMesh->m_ArrayVertex[index2]->y(),pMesh->m_ArrayVertex[index2]->z());
		outfile << "sphere{<" << pMesh->m_ArrayVertex[index1]->x() << " , " << pMesh->m_ArrayVertex[index1]->y() << " , " << -pMesh->m_ArrayVertex[index1]->z() << ">,spherer1 texture { vertex_tex1 }}" << endl;
		outfile << "sphere{<" << pMesh->m_ArrayVertex[index2]->x() << " , " << pMesh->m_ArrayVertex[index2]->y() << " , " << -pMesh->m_ArrayVertex[index2]->z() << ">,spherer1 texture { vertex_tex1 }}" << endl;
	}
	/*for(int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		CVertex3d* pStartVertex = pMesh->GetVertex(pair.first);
		CVertex3d* pEndVertex = pMesh->GetVertex(pair.second);

		outfile << "sphere{<" <<pStartVertex->x()<<" , "<<pStartVertex->y()<<" , " << -pStartVertex->z()<<">,spherer texture { vertex_tex }}"<<endl;
		outfile << "sphere{<" <<pEndVertex->x()<<" , "<<pEndVertex->y()<<" , " << -pEndVertex->z()<<">,spherer texture { vertex_tex }}"<<endl;
		//outfile << "cylinder {<" << pStartVertex->x() << " , " << pStartVertex->y() << " , " << -pStartVertex->z() << " >,<" << pEndVertex->x() << " , " << pEndVertex->y() << " , " << -pEndVertex->z() << " > ,r_sharpedge texture { sharpedge_tex } }" << endl;
	}*/
	outfile.close();
}
void CFormView3::OutPutMeshPolyOminoEdges_PovRay(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	std::pair<int, int> pair;
	std::vector<double> m_ArrayVerticalInnerIntervals, m_ArrayHorizontalInnerIntervals;
	double TOL = 1e-8;
	for (int i = 0; i < pMesh->m_ArrayFaceEdgesArray.size(); i++)
	{
		std::vector<int> edgeset = pMesh->m_ArrayFaceEdgesArray[i];
		for (int jj = 0; jj < edgeset.size(); jj++)
		{
			pair = pMesh->m_ArraySharpEdges[edgeset[jj]];
			CVertex3d* pVertex1 = pMesh->GetVertex(pair.first);
			CVertex3d* pVertex2 = pMesh->GetVertex(pair.second);

			double x1 = pVertex1->x();
			double y1 = pVertex1->y();
			double x2 = pVertex2->x();
			double y2 = pVertex2->y();

			double dis = Distance(pVertex1, pVertex2);
			if (dis < 1e-4)
			{
				int ii = 0;
			}
			if (fabs(y1 - y2) < 1e-8)
				outfile << "cylinder {<" << x1 << " , " << y1 << " , " << 0 << " >,<" << x2 << " , " << y2 << " , " << 0 << " > ,r_horizontal_sharpedge texture { horizontal_sharpedge_tex } }" << endl;
			else
				outfile << "cylinder {<" << x1 << " , " << y1 << " , " << 0 << " >,<" << x2 << " , " << y2 << " , " << 0 << " > ,r_vertical_sharpedge texture { vertical_sharpedge_tex } }" << endl;

		}
	}
	for (int i = 0; i < pMesh->m_ArrayFaceAreaArray.size(); i++)
	{
		std::vector<int> pointset = pMesh->m_ArrayFaceAreaArray[i];
		std::vector<int> edgeset = pMesh->m_ArrayFaceEdgesArray[i];
		int size = pointset.size() / 4;
		double area = 0;
		for (int j = 0; j < size; j++)
		{
			int index1 = pointset[4 * j];
			int index2 = pointset[4 * j + 1];
			int index3 = pointset[4 * j + 2];
			int index4 = pointset[4 * j + 3];
			double TOL = 1e-8;
			for (double x = pMesh->xinterval[index1]; x < pMesh->xinterval[index2] - TOL; x += pMesh->baseinterval)
			{
				double y1, y2;
				y1 = pMesh->yinterval[index3] < pMesh->yinterval[index4] ? pMesh->yinterval[index3] : pMesh->yinterval[index4];
				y2 = pMesh->yinterval[index3] > pMesh->yinterval[index4] ? pMesh->yinterval[index3] : pMesh->yinterval[index4];
				m_ArrayVerticalInnerIntervals.push_back(x);
				m_ArrayVerticalInnerIntervals.push_back(y1);
				m_ArrayVerticalInnerIntervals.push_back(y2);
				//::glVertex3f(x,pMesh->yinterval[index3],0);
				//::glVertex3f(x,pMesh->yinterval[index4],0);
			}
			for (double y = pMesh->yinterval[index3]; y < pMesh->yinterval[index4] - TOL; y += pMesh->baseinterval)
			{
				//::glVertex3f(pMesh->xinterval[index1],y,0);
				//::glVertex3f(pMesh->xinterval[index2],y,0);
				m_ArrayHorizontalInnerIntervals.push_back(y);
				double x1, x2;
				x1 = pMesh->xinterval[index1] < pMesh->xinterval[index2] ? pMesh->xinterval[index1] : pMesh->xinterval[index2];
				x2 = pMesh->xinterval[index1] > pMesh->xinterval[index2] ? pMesh->xinterval[index1] : pMesh->xinterval[index2];
				m_ArrayHorizontalInnerIntervals.push_back(x1);
				m_ArrayHorizontalInnerIntervals.push_back(x2);
			}
			//area += (xinterval[index2] - xinterval[index1]) * (yinterval[index4] - yinterval[index3]);
		}

		for (int j = 0; j < m_ArrayVerticalInnerIntervals.size(); j = j + 3)
		{
			double x = m_ArrayVerticalInnerIntervals[j];
			double y1 = m_ArrayVerticalInnerIntervals[j + 1];
			double y2 = m_ArrayVerticalInnerIntervals[j + 2];
			for (int jj = 0; jj < edgeset.size(); jj++)
			{
				pair = pMesh->m_ArraySharpEdges[jj];
				CVertex3d* pVertex1 = pMesh->GetVertex(pair.first);
				CVertex3d* pVertex2 = pMesh->GetVertex(pair.second);

				double x1 = pVertex1->x();
				double y3 = pVertex1->y();
				double x2 = pVertex2->x();
				double y4 = pVertex2->y();


				if (fabs(x1 - x2) < 1e-8 && fabs(x1 - x) < 1e-8)
				{
					double y5 = y3 < y4 ? y3 : y4;
					double y6 = y3 > y4 ? y3 : y4;
					if (y5 < y1 - TOL && y6 > y1 + TOL && y6 < y2 - TOL)
					{
						m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
						m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
						m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
						m_ArrayVerticalInnerIntervals.push_back(x);
						m_ArrayVerticalInnerIntervals.push_back(y1);
						m_ArrayVerticalInnerIntervals.push_back(y6);
						j = j - 3;
						break;
					}
					else
						if (y5 < y1 + TOL && y6 > y2 - TOL)
						{
							m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
							m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
							m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
							j = j - 3;
							break;
						}
						else
							if (y5 > y1 + TOL && y5 < y2 - TOL && y6 > y2 - TOL)
							{
								m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
								m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
								m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
								m_ArrayVerticalInnerIntervals.push_back(x);
								m_ArrayVerticalInnerIntervals.push_back(y1);
								m_ArrayVerticalInnerIntervals.push_back(y5);
								j = j - 3;
								break;
							}
							else
								if (y5 > y1 + TOL && y5 < y2 - TOL && y6 > y1 + TOL && y6 < y2 - TOL)
								{
									m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
									m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
									m_ArrayVerticalInnerIntervals.erase(m_ArrayVerticalInnerIntervals.begin() + j);
									m_ArrayVerticalInnerIntervals.push_back(x);
									m_ArrayVerticalInnerIntervals.push_back(y1);
									m_ArrayVerticalInnerIntervals.push_back(y5);
									m_ArrayVerticalInnerIntervals.push_back(x);
									m_ArrayVerticalInnerIntervals.push_back(y6);
									m_ArrayVerticalInnerIntervals.push_back(y2);
									j = j - 3;
									break;
								}
				}
			}
		}
		for (int j = 0; j < m_ArrayHorizontalInnerIntervals.size(); j = j + 3)
		{
			double y = m_ArrayHorizontalInnerIntervals[j];
			double x1 = m_ArrayHorizontalInnerIntervals[j + 1];
			double x2 = m_ArrayHorizontalInnerIntervals[j + 2];
			for (int jj = 0; jj < edgeset.size(); jj++)
			{
				pair = pMesh->m_ArraySharpEdges[jj];
				CVertex3d* pVertex1 = pMesh->GetVertex(pair.first);
				CVertex3d* pVertex2 = pMesh->GetVertex(pair.second);

				double x3 = pVertex1->x();
				double y3 = pVertex1->y();
				double x4 = pVertex2->x();
				double y4 = pVertex2->y();


				if (fabs(y3 - y4) < 1e-8 && fabs(y3 - y) < 1e-8)
				{
					double x5 = x3 < x4 ? x3 : x4;
					double x6 = x3 > x4 ? x3 : x4;
					if (x5 < x1 - TOL && x6 > x1 + TOL && x6 < x2 - TOL)
					{
						m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
						m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
						m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
						m_ArrayHorizontalInnerIntervals.push_back(y);
						m_ArrayHorizontalInnerIntervals.push_back(x1);
						m_ArrayHorizontalInnerIntervals.push_back(x6);
						j = j - 3;
						break;
					}
					else
						if (x5 < x1 + TOL && x6 > x2 - TOL)
						{
							m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
							m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
							m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
							j = j - 3;
							break;
						}
						else
							if (x5 > x1 + TOL && x5 < x2 - TOL && x6 > x2 - TOL)
							{
								m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
								m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
								m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
								m_ArrayHorizontalInnerIntervals.push_back(y);
								m_ArrayHorizontalInnerIntervals.push_back(x1);
								m_ArrayHorizontalInnerIntervals.push_back(x5);
								j = j - 3;
								break;
							}
							else
								if (x5 > x1 + TOL && x5 < x2 - TOL && x6 > x1 + TOL && x6 < x2 - TOL)
								{
									m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
									m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
									m_ArrayHorizontalInnerIntervals.erase(m_ArrayHorizontalInnerIntervals.begin() + j);
									m_ArrayHorizontalInnerIntervals.push_back(y);
									m_ArrayHorizontalInnerIntervals.push_back(x1);
									m_ArrayHorizontalInnerIntervals.push_back(x5);
									m_ArrayHorizontalInnerIntervals.push_back(y);
									m_ArrayHorizontalInnerIntervals.push_back(x6);
									m_ArrayHorizontalInnerIntervals.push_back(x2);
									j = j - 3;
									break;
								}
				}
			}
		}

		//save to the file
		for (int jj = 0; jj < m_ArrayVerticalInnerIntervals.size(); jj = jj + 3)
		{
			double x = m_ArrayVerticalInnerIntervals[jj];
			double y1 = m_ArrayVerticalInnerIntervals[jj + 1];
			double y2 = m_ArrayVerticalInnerIntervals[jj + 2];
			double dis = fabs(y1 - y2);
			if (dis < 1e-5)
			{
				int ii = 0;
			}
			outfile << "cylinder {<" << x << " , " << y1 << " , " << 0 << " >,<" << x << " , " << y2 << " , " << 0 << " > ,r_edge2 texture { edge_tex2 } }" << endl;
		}
		for (int jj = 0; jj < m_ArrayHorizontalInnerIntervals.size(); jj = jj + 3)
		{
			double y = m_ArrayHorizontalInnerIntervals[jj];
			double x1 = m_ArrayHorizontalInnerIntervals[jj + 1];
			double x2 = m_ArrayHorizontalInnerIntervals[jj + 2];
			double dis = fabs(x1 - x2);
			if (dis < 1e-5)
			{
				int ii = 0;
			}
			outfile << "cylinder {<" << x1 << " , " << y << " , " << 0 << " >,<" << x2 << " , " << y << " , " << 0 << " > ,r_edge2 texture { edge_tex2 } }" << endl;
		}
		m_ArrayVerticalInnerIntervals.clear();
		m_ArrayHorizontalInnerIntervals.clear();
	}
	outfile.close();
}
void CFormView3::OutPutSharpVertices_PovRay_Anchor(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);

	for (int i = 0; i < pMesh->m_ArrayAnchorPoints.size(); i++)
	{
		outfile << "sphere{<" << pMesh->m_ArrayVertex[pMesh->m_ArrayAnchorPoints[i]]->x() << " , " << pMesh->m_ArrayVertex[pMesh->m_ArrayAnchorPoints[i]]->y() << " , " << -pMesh->m_ArrayVertex[pMesh->m_ArrayAnchorPoints[i]]->z() << ">,spherer1 texture { vertex_tex1 }}" << endl;
	}

	outfile.close();
}
void CFormView3::OutPutMeshEdges_PovRay_Anchor(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	//pMesh->BuildAdjacency();
	//pMesh->CalculateNormalPerFace();
	//pMesh->CalculateNormalPerVertex();
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		pMesh->GetVertex(i)->m_OriginalIndex = i;
	}

	int facenum = 0;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;

		if (index1 || index2 || index3)
			continue;
		facenum++;
	}
	int faceindex = 0;

	std::vector<std::pair<int, int>> m_ArrayEdges;
	std::pair<int, int> pair, pair1;
	faceindex = 0;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;

		if (index1 || index2 || index3)
			continue;

		pair.first = pFace->v1()->m_OriginalIndex;
		pair.second = pFace->v2()->m_OriginalIndex;

		pair1.first = pFace->v2()->m_OriginalIndex;
		pair1.second = pFace->v1()->m_OriginalIndex;
		if (std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair) == m_ArrayEdges.end() &&
			std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair1) == m_ArrayEdges.end())
		{
			m_ArrayEdges.push_back(pair);
		}

		pair.first = pFace->v2()->m_OriginalIndex;
		pair.second = pFace->v3()->m_OriginalIndex;

		pair1.first = pFace->v3()->m_OriginalIndex;
		pair1.second = pFace->v2()->m_OriginalIndex;
		if (std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair) == m_ArrayEdges.end() &&
			std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair1) == m_ArrayEdges.end())
		{
			m_ArrayEdges.push_back(pair);
		}

		pair.first = pFace->v3()->m_OriginalIndex;
		pair.second = pFace->v1()->m_OriginalIndex;

		pair1.first = pFace->v1()->m_OriginalIndex;
		pair1.second = pFace->v3()->m_OriginalIndex;
		if (std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair) == m_ArrayEdges.end() &&
			std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair1) == m_ArrayEdges.end())
		{
			m_ArrayEdges.push_back(pair);
		}
		//outfile << "<" << pFace->v1()->m_OriginalIndex << "," << pFace->v2()->m_OriginalIndex << "," << pFace->v3()->m_OriginalIndex << ">," << pFace->v1()->m_OriginalIndex << "," << pFace->v2()->m_OriginalIndex << "," << pFace->v3()->m_OriginalIndex;
	}

	for (int i = 0; i < m_ArrayEdges.size(); i++)
	{
		pair = m_ArrayEdges[i];
		CVertex3d* v1 = pMesh->GetVertex(pair.first);
		CVertex3d* v2 = pMesh->GetVertex(pair.second);
		outfile << "cylinder {<" << v1->x() << " , " << v1->y() << " , " << -v1->z() << " >,<" << v2->x() << " , " << v2->y() << " , " << -v2->z() << " > ,r_edge texture { edge_tex } }" << endl;
		//outfile << "< " << pair.first << ", " << pair.second << ", 0 >";
	}

	/*outfile << "\n #declare jn = " << pMesh->m_ArrayAnchorPoints.size() << ";\n";
	outfile << "#declare junction_list = array[" << pMesh->m_ArrayAnchorPoints.size() << "]\n{";
	for (int i = 0; i < pMesh->m_ArrayAnchorPoints.size(); i++)
	{
		outfile << "< " << pMesh->m_ArrayAnchorPoints[i] << ", " << 0 << ", 0 >";
		if (i < pMesh->m_ArrayAnchorPoints.size() - 1)
		{
			outfile << ",";
		}
		else
		{
			outfile << "}";
		}
	}*/
	outfile.close();
}
void CFormView3::OutPutMeshEdges_PovRay(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	int num = 0;
	CMesh3d_OpenMesh* pMesh_OpenMesh = pMesh->Convert_OpenMesh();
	for (MyMesh::EdgeIter e_it = pMesh_OpenMesh->pmesh->edges_begin(); e_it != pMesh_OpenMesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		MyMesh::HalfedgeHandle hh = pMesh_OpenMesh->pmesh->halfedge_handle(eh, 0);

		MyMesh::VertexHandle vh1 = pMesh_OpenMesh->pmesh->to_vertex_handle(hh);
		MyMesh::VertexHandle vh2 = pMesh_OpenMesh->pmesh->from_vertex_handle(hh);

		OpenMesh::Vec3d v1 = pMesh_OpenMesh->pmesh->point(vh1);
		OpenMesh::Vec3d v2 = pMesh_OpenMesh->pmesh->point(vh2);

		outfile << "cylinder {<" << v1[0] << " , " << v1[1] << " , " << -v1[2] << " >,<" << v2[0] << " , " << v2[1] << " , " << -v2[2] << " > ,r_edge texture { edge_tex } }" << endl;
	}

	/*for(j = 0; j < pMesh->NbFace(); j++)
	{
		CFace3d* pFace = pMesh->GetFace(j);

	   // num = 0 ;
		//if(pFace->v1()->x() == pFace->v2()->x())
		//	num++;
		//if(pFace->v1()->y() == pFace->v2()->y())
		//	num++;
		//if(pFace->v1()->z() == pFace->v2()->z())
		//	num++;
		//if(num == 2)
			outfile << "cylinder {<" << pFace->v1()->x() << " , " << pFace->v1()->y() << " , " << -pFace->v1()->z() << " >,<" << pFace->v2()->x() << " , " << pFace->v2()->y() << " , " << -pFace->v2()->z() << " > ,r_edge texture { edge_tex } }" << endl;
		//num = 0 ;
		//if(pFace->v2()->x() == pFace->v3()->x())
		//	num++;
		//if(pFace->v2()->y() == pFace->v3()->y())
		//	num++;
		//if(pFace->v2()->z() == pFace->v3()->z())
		//	num++;
		//if(num == 2)
			outfile << "cylinder {<" << pFace->v2()->x() << " , " << pFace->v2()->y() << " , " << -pFace->v2()->z() << " >,<" << pFace->v3()->x() << " , " << pFace->v3()->y() << " , " << -pFace->v3()->z() << " > ,r_edge texture { edge_tex } }" << endl;
		//num = 0 ;
		//if(pFace->v1()->x() == pFace->v3()->x())
		//	num++;
		//if(pFace->v1()->y() == pFace->v3()->y())
		//	num++;
		//if(pFace->v1()->z() == pFace->v3()->z())
		//	num++;
		//if(num == 2)
			outfile << "cylinder {<" << pFace->v3()->x() << " , " << pFace->v3()->y() << " , " << -pFace->v3()->z() << " >,<" << pFace->v1()->x() << " , " << pFace->v1()->y() << " , " << -pFace->v1()->z() << " > ,r_edge texture { edge_tex } }" << endl;
	}*/
	outfile.close();
}
void CFormView3::OutPutMeshSavedEdges_PovRay(CNurbsSuface* pNurbsSurface, int samplesm, int samplesn, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	samplesm = samplesm * 30;
	for (j = 0; j < pNurbsSurface->varray.size(); j++)
	{
		for (i = 0; i < samplesm; i++)
		{	//
			double s1 = (double)i / samplesm;
			double s2 = (double)(i + 1) / samplesm;
			double t = pNurbsSurface->varray[j];


			double u1 = s1;
			double v1 = t;

			double u2 = s2;
			double v2 = t;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1, v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2, v2) - ZeroP3D);

			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	samplesm = samplesm / 30;
	samplesn = samplesn * 30;

	for (i = 0; i < pNurbsSurface->uarray.size(); i++)
	{
		for (j = 0; j < samplesn; j++)
		{	//
			double s = pNurbsSurface->uarray[i];
			double t1 = (double)j / samplesn;
			double t2 = (double)(j + 1) / samplesn;

			double u1 = s;
			double v1 = t1;

			double u2 = s;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1, v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2, v2) - ZeroP3D);

			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	outfile.close();
}
void CFormView3::OutPutQuadMeshEdges_PovRay(CNurbsSuface* pSurface, int samplesm, int samplesn, char* filename)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	double s1, s2, t1, t2;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	//samplesm = samplesm * 30;
	for (j = 0; j <= samplesn; j++)
	{
		for (i = 0; i < samplesm; i++)
		{	//
			s1 = (double)i / samplesm;
			s2 = (double)(i + 1) / samplesm;
			t1 = t2 = (double)j / samplesn;

			if (pSurface->ptransformation)
			{

				Transformation* pTrans = (Transformation*)pSurface->ptransformation;
				s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
				s2 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s2;
				t1 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t1;
				t2 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t2;
				double u, v;
				pTrans->Evaluation(s1, t1, u, v);
				s1 = u;
				t1 = v;
				pTrans->Evaluation(s2, t2, u, v);
				s2 = u;
				t2 = v;

			}
			else
			{
				double umin = pNurbsSurface->GetUKnot(0);
				double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

				double vmin = pNurbsSurface->GetVKnot(0);
				double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

				s1 = umin + (umax - umin) * s1;
				s2 = umin + (umax - umin) * s2;
				t1 = vmin + (vmax - vmin) * t1;
				t2 = vmin + (vmax - vmin) * t2;
			}

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1, v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2, v2) - ZeroP3D);

			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	//samplesm = samplesm / 30;
	//samplesn = samplesn * 30;

	for (i = 0; i <= samplesm; i++)
	{
		for (j = 0; j < samplesn; j++)
		{	//
			s1 = s2 = (double)i / samplesm;
			t1 = (double)j / samplesn;
			t2 = (double)(j + 1) / samplesn;

			double umin = pNurbsSurface->GetUKnot(0);
			double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

			double vmin = pNurbsSurface->GetVKnot(0);
			double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

			if (pSurface->ptransformation)
			{

				Transformation* pTrans = (Transformation*)pSurface->ptransformation;
				s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
				s2 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s2;
				t1 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t1;
				t2 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t2;
				double u, v;
				pTrans->Evaluation(s1, t1, u, v);
				s1 = u;
				t1 = v;
				pTrans->Evaluation(s2, t2, u, v);
				s2 = u;
				t2 = v;

			}
			else
			{
				s1 = umin + (umax - umin) * s1;
				s2 = umin + (umax - umin) * s2;
				t1 = vmin + (vmax - vmin) * t1;
				t2 = vmin + (vmax - vmin) * t2;
			}

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1, v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2, v2) - ZeroP3D);

			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	outfile.close();
}
void CFormView3::OutPutGeneralBilinearTransformedEdges_obj(CNurbsSuface* pSurface, int samplesm, int samplesn, char* filename)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	double s1, s2, t1, t2;
	int i, j;
	if (!pSurface->ptransformation)
		return;
	if (pSurface->ptransformation->m_ReparameterizationType != generalbilinear)
		return;

	for (j = 0; j <= samplesn; j++)
	{
		for (i = 0; i <= samplesm; i++)
		{	//
			s1 = (double)i / samplesm;
			t1 = (double)j / samplesn;

			Transformation* pTrans = (Transformation*)pSurface->ptransformation;
			s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
			t1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * t1;

			double u, v;
			pTrans->Evaluation(s1, t1, u, v);

			//if(Length(vv1 - vv2) > 1e-5)
			outfile << "v " << u << "  " << v << "  " << 0 << endl;
		}
	}

	for (j = 0; j < samplesn; j++)
	{
		for (i = 0; i < samplesm; i++)
		{
			//
			//outfile << "f " << (samplesn + 1) * j + i + 1 <<" " << (samplesn + 1) * j + i + 2 << " "<< (samplesn + 1) * j + i + 2 + samplesn + 1 << " "<< (samplesn + 1) * j + i + 1 + samplesn + 1 << endl;
			int index1 = (samplesn + 1) * j + i + 1;
			int index2 = (samplesn + 1) * j + i + 2;
			int index3 = (samplesn + 1) * j + i + 1 + samplesn + 1;
			int index4 = (samplesn + 1) * j + i + 2 + samplesn + 1;
			{
				outfile << "f " << index1 << " " << index2 << " " << index3 << endl;
				outfile << "f " << index2 << " " << index4 << " " << index3 << endl;
			}

		}
	}


	outfile.close();
}
void CFormView3::OutPutGeneralBilinearTransformedEdges_PovRay(CNurbsSuface* pSurface, int samplesm, int samplesn, char* filename)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	double s1, s2, t1, t2;
	int i, j;
	if (!pSurface->ptransformation)
		return;
	if (pSurface->ptransformation->m_ReparameterizationType != generalbilinear)
		return;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	samplesm = samplesm * 10;
	for (j = 0; j <= samplesn; j++)
	{
		for (i = 0; i < samplesm; i++)
		{	//
			s1 = (double)i / samplesm;
			s2 = (double)(i + 1) / samplesm;
			t1 = t2 = (double)j / samplesn;

			Transformation* pTrans = (Transformation*)pSurface->ptransformation;
			s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
			s2 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s2;
			t1 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t1;
			t2 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t2;
			double u, v;
			pTrans->Evaluation(s1, t1, u, v);

			s1 = u;
			t1 = v;
			pTrans->Evaluation(s2, t2, u, v);
			s2 = u;
			t2 = v;

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			//if(Length(vv1 - vv2) > 1e-5)
			outfile << "cylinder {<" << u1 << " , " << v1 << " , " << 0 << " >,<" << u2 << " , " << v2 << " , " << 0 << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	samplesm = samplesm / 10;
	samplesn = samplesn * 10;

	for (i = 0; i <= samplesm; i++)
	{
		for (j = 0; j < samplesn; j++)
		{	//
			s1 = s2 = (double)i / samplesm;
			t1 = (double)j / samplesn;
			t2 = (double)(j + 1) / samplesn;

			Transformation* pTrans = (Transformation*)pSurface->ptransformation;
			s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
			s2 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s2;
			t1 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t1;
			t2 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t2;
			double u, v;
			pTrans->Evaluation(s1, t1, u, v);
			s1 = u;
			t1 = v;
			pTrans->Evaluation(s2, t2, u, v);
			s2 = u;
			t2 = v;

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			//if(Length(vv1 - vv2) > 1e-5)
			outfile << "cylinder {<" << u1 << " , " << v1 << " , " << 0 << " >,<" << u2 << " , " << v2 << " , " << 0 << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	outfile.close();
}
void CFormView3::OutPutMesh_OneEdge_PovRay(CNurbsSuface* pSurface, double u0, double v0, double u1, double v1, int samples, char* filename, char* colortex)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	double s1, s2, t1, t2;
	double p1, p2;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	//samples = samples * 300;
	for (i = 0; i < samples; i = i + 6)
	{
		p1 = (double)i / samples;
		p2 = (double)(i + 2) / samples;

		s1 = u0 + (u1 - u0) * p1;
		t1 = v0 + (v1 - v0) * p1;

		s2 = u0 + (u1 - u0) * p2;
		t2 = v0 + (v1 - v0) * p2;
		if (pSurface->ptransformation)
		{

			Transformation* pTrans = (Transformation*)pSurface->ptransformation;
			s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
			s2 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s2;
			t1 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t1;
			t2 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t2;
			double u, v;
			//pTrans->Evaluation(s1,t1,u,v);

			//s1 = u;
			//t1 = v;
			//pTrans->Evaluation(s2,t2,u,v);
			//s2 = u;
			//t2 = v;

				/*if(s1 < pTrans->umin)
					continue;
				if(t1 < pTrans->vmin)
					continue;
				if(s1 > pTrans->umax)
					continue;
				if(t1 > pTrans->vmax)
					continue;
				if(s2 < pTrans->umin)
					continue;
				if(t2 < pTrans->vmin)
					continue;
				if(s2 > pTrans->umax)
					continue;
				if(t2 > pTrans->vmax)
					continue;*/

		}
		else
		{
			double umin = pNurbsSurface->GetUKnot(0);
			double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

			double vmin = pNurbsSurface->GetVKnot(0);
			double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

			s1 = umin + (umax - umin) * s1;
			s2 = umin + (umax - umin) * s2;
			t1 = vmin + (vmax - vmin) * t1;
			t2 = vmin + (vmax - vmin) * t2;
		}

		double u1 = s1;
		double v1 = t1;

		double u2 = s2;
		double v2 = t2;

		//pSurface->ptransformation
		//Vector3D vv1 = (pNurbsSurface->ComputPosition(u1,v1) - ZeroP3D);
		//Vector3D vv2 = (pNurbsSurface->ComputPosition(u2,v2) - ZeroP3D);
		Vector3D vv1 = (pSurface->ComputPosition(u1, v1) - ZeroP3D);
		Vector3D vv2 = (pSurface->ComputPosition(u2, v2) - ZeroP3D);

		//if(Length(vv1 - vv2) > 1e-5)
		outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { " << colortex << " } }" << endl;

	}
	outfile.close();
}
void CFormView3::OutPutMesh_Dotted_OneEdge_PovRay(double u0, double v0, double u1, double v1, int samples, char* filename, char* colortex)
{
	std::ofstream outfile(filename);
	double s1, s2, t1, t2;
	double p1, p2;
	int i, j;
	for (i = 0; i < samples; i = i + 6)
	{
		p1 = (double)i / samples;
		p2 = (double)(i + 2) / samples;

		s1 = u0 + (u1 - u0) * p1;
		t1 = v0 + (v1 - v0) * p1;

		s2 = u0 + (u1 - u0) * p2;
		t2 = v0 + (v1 - v0) * p2;

		outfile << "cylinder {<" << s1 << " , " << t1 << " , " << 0 << " >,<" << s2 << " , " << t2 << " , " << 0 << " > ,r_edge texture { " << colortex << " } }" << endl;
	}
	outfile.close();
}
void CFormView3::OutPutSurfaceBoundaryEdges_PovRay(char* filename)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	std::ofstream outfile(filename);
	int i, j;
	int samplesm = 300;
	CNurbsSuface* pNurbsSurface;
	double s1, s2, t1, t2;
	for (int ii = 0; ii < num; ii++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
			for (i = 0; i < samplesm; i++)
			{
				s1 = (double)i / samplesm;
				s2 = (double)(i + 1) / samplesm;

				Vector3D vv1 = (pNurbsSurface->ComputPosition(s1, 0) - ZeroP3D);
				Vector3D vv2 = (pNurbsSurface->ComputPosition(s2, 0) - ZeroP3D);

				outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
			}
			for (i = 0; i < samplesm; i++)
			{
				s1 = (double)i / samplesm;
				s2 = (double)(i + 1) / samplesm;

				Vector3D vv1 = (pNurbsSurface->ComputPosition(s1, 1) - ZeroP3D);
				Vector3D vv2 = (pNurbsSurface->ComputPosition(s2, 1) - ZeroP3D);

				outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
			}
			for (i = 0; i < samplesm; i++)
			{
				s1 = (double)i / samplesm;
				s2 = (double)(i + 1) / samplesm;

				Vector3D vv1 = (pNurbsSurface->ComputPosition(0, s1) - ZeroP3D);
				Vector3D vv2 = (pNurbsSurface->ComputPosition(0, s2) - ZeroP3D);

				outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
			}
			for (i = 0; i < samplesm; i++)
			{
				s1 = (double)i / samplesm;
				s2 = (double)(i + 1) / samplesm;

				Vector3D vv1 = (pNurbsSurface->ComputPosition(1, s1) - ZeroP3D);
				Vector3D vv2 = (pNurbsSurface->ComputPosition(1, s2) - ZeroP3D);

				outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
			}
		}
	}
	outfile.close();
}
void CFormView3::OutPutMeshEdges_PovRay(CNurbsSuface* pSurface, int samplesm, int samplesn, char* filename)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	double s1, s2, t1, t2;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	samplesm = samplesm * 300;
	for (j = 0; j <= samplesn; j++)
	{
		//if(j == 0 || j == samplesn)
		//	continue;

		for (i = 0; i < samplesm; i++)
		{	//
			s1 = (double)i / samplesm;
			s2 = (double)(i + 1) / samplesm;
			t1 = t2 = (double)j / samplesn;

			if (pSurface->ptransformation)
			{
				Transformation* pTrans = (Transformation*)pSurface->ptransformation;
				s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
				s2 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s2;
				t1 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t1;
				t2 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t2;
				double u, v;
				//pTrans->Evaluation(s1,t1,u,v);
				//((GeneralBilinearTransformation*)pTrans)->label = -1;
				//s1 = u;
				//t1 = v;
				//pTrans->Evaluation(s2,t2,u,v);
				//s2 = u;
				//t2 = v;

					/*if(s1 < pTrans->umin)
						continue;
					if(t1 < pTrans->vmin)
						continue;
					if(s1 > pTrans->umax)
						continue;
					if(t1 > pTrans->vmax)
						continue;
					if(s2 < pTrans->umin)
						continue;
					if(t2 < pTrans->vmin)
						continue;
					if(s2 > pTrans->umax)
						continue;
					if(t2 > pTrans->vmax)
						continue;*/

			}
			else
			{
				double umin = pNurbsSurface->GetUKnot(0);
				double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

				double vmin = pNurbsSurface->GetVKnot(0);
				double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

				s1 = umin + (umax - umin) * s1;
				s2 = umin + (umax - umin) * s2;
				t1 = vmin + (vmax - vmin) * t1;
				t2 = vmin + (vmax - vmin) * t2;
			}

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			/*if(u1 > 1)
				u1 = 1;
			if(v1 > 1)
				v1 = 1;
			if(u2 > 1)
				u2 = 1;
			if(v2 > 1)
				v2 = 1;
			if(u1 < 0)
				u1 = 0;
			if(v1 < 0)
				v1 = 0;
			if(u2 < 0)
				u2 = 0;
			if(v2 > 0)
				v2 = 0;*/
				//Vector3D vv1 = (pNurbsSurface->ComputPosition(u1,v1) - ZeroP3D);
				//Vector3D vv2 = (pNurbsSurface->ComputPosition(u2,v2) - ZeroP3D);
			Vector3D vv1 = (pSurface->ComputPosition(u1, v1) - ZeroP3D);
			Vector3D vv2 = (pSurface->ComputPosition(u2, v2) - ZeroP3D);

			//vv1.x = u1;
			//vv1.y = v1;
			//vv1.z = 0;
			//vv2.x = u2;
			//vv2.y = v2;
			//vv2.z = 0;
			//CString s1;
			//std::ofstream s1outfile(s1);
			//CString s2;
			//std::ofstream s2outfile(s2);


			char s1[150];
			ostrstream s1outfile(s1, 150);
			char s2[150];
			ostrstream s2outfile(s2, 150);
			memset(s1, 0, 150);
			memset(s2, 0, 150);

			s1outfile << vv1[0] << " , " << vv1[1] << " , " << -vv1[2];
			s2outfile << vv2[0] << " , " << vv2[1] << " , " << -vv2[2];
			if (strcmp(s1, s2) != 0)
				//if(Length(vv1 - vv2) > 1e-5)
				outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	samplesm = samplesm / 300;
	samplesn = samplesn * 300;

	for (i = 0; i <= samplesm; i++)
	{
		//if(i == 0 || i == samplesm || i == samplesm / 2)
		//	continue;

		for (j = 0; j < samplesn; j++)
		{	//
			s1 = s2 = (double)i / samplesm;
			t1 = (double)j / samplesn;
			t2 = (double)(j + 1) / samplesn;

			double umin = pNurbsSurface->GetUKnot(0);
			double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

			double vmin = pNurbsSurface->GetVKnot(0);
			double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

			if (pSurface->ptransformation)
			{

				Transformation* pTrans = (Transformation*)pSurface->ptransformation;
				s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
				s2 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s2;
				t1 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t1;
				t2 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t2;
				double u, v;
				//pTrans->Evaluation(s1,t1,u,v);
				//s1 = u;
				//t1 = v;
				//pTrans->Evaluation(s2,t2,u,v);
				//s2 = u;
				//t2 = v;

				/*if(s1 < pTrans->umin)
					continue;
				if(t1 < pTrans->vmin)
					continue;
				if(s1 > pTrans->umax)
					continue;
				if(t1 > pTrans->vmax)
					continue;
				if(s2 < pTrans->umin)
					continue;
				if(t2 < pTrans->vmin)
					continue;
				if(s2 > pTrans->umax)
					continue;
				if(t2 > pTrans->vmax)
					continue;*/


			}
			else
			{
				s1 = umin + (umax - umin) * s1;
				s2 = umin + (umax - umin) * s2;
				t1 = vmin + (vmax - vmin) * t1;
				t2 = vmin + (vmax - vmin) * t2;
			}

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			/*if(u1 > 1)
				u1 = 1;
			if(v1 > 1)
				v1 = 1;
			if(u2 > 1)
				u2 = 1;
			if(v2 > 1)
				v2 = 1;
			if(u1 < 0)
				u1 = 0;
			if(v1 < 0)
				v1 = 0;
			if(u2 < 0)
				u2 = 0;
			if(v2 > 0)
				v2 = 0;*/
				//Vector3D vv1 = (pNurbsSurface->ComputPosition(u1,v1) - ZeroP3D);
				//Vector3D vv2 = (pNurbsSurface->ComputPosition(u2,v2) - ZeroP3D);

			Vector3D vv1 = (pSurface->ComputPosition(u1, v1) - ZeroP3D);
			Vector3D vv2 = (pSurface->ComputPosition(u2, v2) - ZeroP3D);

			//vv1.x = u1;
			//vv1.y = v1;
			//vv1.z = 0;
			//vv2.x = u2;
			//vv2.y = v2;
			//vv2.z = 0;

			char s1[150];
			ostrstream s1outfile(s1, 150);
			char s2[150];
			ostrstream s2outfile(s2, 150);
			memset(s1, 0, 150);
			memset(s2, 0, 150);

			s1outfile << vv1[0] << " , " << vv1[1] << " , " << -vv1[2];
			s2outfile << vv2[0] << " , " << vv2[1] << " , " << -vv2[2];
			if (strcmp(s1, s2) != 0)
				//if(Length(vv1 - vv2) > 1e-5)
				outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	outfile.close();
}

void CFormView3::OutPutMMesh(CNurbsSuface* pSurface, int samplesm, int samplesn, char* filename, BOOL parameterdomain)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	double minu = -0.0;
	double maxu = 1.0;
	double minv = -0.0;
	double maxv = 1.0;

	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	int index00, indexm0, indexmn, index0n;
	int num = 1;
	for (j = 0; j <= samplesn; j++)
	{
		for (i = 0; i <= samplesm; i++)
		{	//

			if (i == 0 && j == 0)
				index00 = num;
			if (i == samplesm && j == 0)
				indexm0 = num;
			if (i == samplesm && j == samplesn)
				indexmn = num;
			if (i == 0 && j == samplesn)
				index0n = num;


			double s = minu + (double)i * (maxu - minu) / samplesm;
			double t = minv + (double)j * (maxv - minv) / samplesn;

			double umin = pNurbsSurface->GetUKnot(0);
			double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

			double vmin = pNurbsSurface->GetVKnot(0);
			double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

			s = umin + (umax - umin) * s;
			t = vmin + (vmax - vmin) * t;
			double u, v;
			if (pSurface->ptransformation)
			{
				pSurface->ptransformation->Evaluation(s, t, u, v);
			}
			else
			{
				u = s;
				v = t;
			}

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u, v) - ZeroP3D);
			Vector3D xu = pNurbsSurface->ComputUDerivative1(u, v);
			Vector3D xv = pNurbsSurface->ComputVDerivative1(u, v);
			Vector3D normal = xu ^ xv;
			normal.SetUnit();
			//vv1 = vv1;
			if (parameterdomain)
				outfile << "Vertex " << num << " " << u << " " << v << " " << 0 << endl;
			else
				outfile << "Vertex " << num << " " << vv1[0] << " " << vv1[1] << " " << vv1[2] << " {normal=(" << normal.x << " " << normal.y << " " << normal.z << ")}" << endl;

			num++;
		}
	}
	num = 1;
	std::vector<int> m_ArraySelectedEdges;
	for (j = 0; j < samplesn; j++)
	{
		for (i = 0; i < samplesm; i++)
		{
			//
			//outfile << "f " << (samplesn + 1) * j + i + 1 <<" " << (samplesn + 1) * j + i + 2 << " "<< (samplesn + 1) * j + i + 2 + samplesn + 1 << " "<< (samplesn + 1) * j + i + 1 + samplesn + 1 << endl;
			int index1 = (samplesn + 1) * j + i + 1;
			int index2 = (samplesn + 1) * j + i + 2;
			int index3 = (samplesn + 1) * j + i + 1 + samplesn + 1;
			int index4 = (samplesn + 1) * j + i + 2 + samplesn + 1;
			if (index1 == index00 || index1 == indexm0 || index1 == indexmn || index1 == index0n ||
				index4 == index00 || index4 == indexm0 || index4 == indexmn || index4 == index0n)
			{
				outfile << "Face " << num << " " << index1 << " " << index2 << " " << index4 << endl;
				outfile << "Face " << num + 1 << " " << index1 << " " << index4 << " " << index3 << endl;
				if (index1 == index00 || index1 == indexm0 || index1 == indexmn || index1 == index0n)
				{
					TRACE("\n Edge %d %d {sharp}", index1, index2);
					TRACE("\n Edge %d %d {sharp}", index1, index4);
					TRACE("\n Edge %d %d {sharp}", index1, index3);
					m_ArraySelectedEdges.push_back(index1);
					m_ArraySelectedEdges.push_back(index2);
					m_ArraySelectedEdges.push_back(index1);
					m_ArraySelectedEdges.push_back(index4);
					m_ArraySelectedEdges.push_back(index1);
					m_ArraySelectedEdges.push_back(index3);
				}
				else
				{
					TRACE("\n Edge %d %d {sharp}", index2, index4);
					TRACE("\n Edge %d %d {sharp}", index1, index4);
					TRACE("\n Edge %d %d {sharp}", index3, index4);
					m_ArraySelectedEdges.push_back(index2);
					m_ArraySelectedEdges.push_back(index4);
					m_ArraySelectedEdges.push_back(index1);
					m_ArraySelectedEdges.push_back(index4);
					m_ArraySelectedEdges.push_back(index3);
					m_ArraySelectedEdges.push_back(index4);
				}
			}
			else if (index2 == index00 || index2 == indexm0 || index2 == indexmn || index2 == index0n ||
				index3 == index00 || index3 == indexm0 || index3 == indexmn || index3 == index0n)
			{
				outfile << "Face " << num << " " << index1 << " " << index2 << " " << index3 << endl;
				outfile << "Face " << num + 1 << " " << index2 << " " << index4 << " " << index3 << endl;
				if (index2 == index00 || index2 == indexm0 || index2 == indexmn || index2 == index0n)
				{
					TRACE("\n Edge %d %d {sharp}", index2, index1);
					TRACE("\n Edge %d %d {sharp}", index2, index3);
					TRACE("\n Edge %d %d {sharp}", index2, index4);
					m_ArraySelectedEdges.push_back(index2);
					m_ArraySelectedEdges.push_back(index1);
					m_ArraySelectedEdges.push_back(index2);
					m_ArraySelectedEdges.push_back(index3);
					m_ArraySelectedEdges.push_back(index2);
					m_ArraySelectedEdges.push_back(index4);
				}
				else
				{
					TRACE("\n Edge %d %d {sharp}", index3, index1);
					TRACE("\n Edge %d %d {sharp}", index3, index2);
					TRACE("\n Edge %d %d {sharp}", index3, index4);
					m_ArraySelectedEdges.push_back(index3);
					m_ArraySelectedEdges.push_back(index1);
					m_ArraySelectedEdges.push_back(index3);
					m_ArraySelectedEdges.push_back(index2);
					m_ArraySelectedEdges.push_back(index3);
					m_ArraySelectedEdges.push_back(index4);
				}

			}
			else
			{
				outfile << "Face " << num << " " << index1 << " " << index2 << " " << index3 << endl;
				outfile << "Face " << num + 1 << " " << index2 << " " << index4 << " " << index3 << endl;
			}
			num = num + 2;
		}
	}
	for (int i = 0; i < m_ArraySelectedEdges.size() / 2; i++)
	{
		outfile << "Edge " << m_ArraySelectedEdges[i * 2] << " " << m_ArraySelectedEdges[i * 2 + 1] << " {sharp} " << endl;
	}
	outfile.close();
}
void CFormView3::OutputSubObjMesh(CNurbsSuface* pSurface, int samplesm, int samplesn, char* filename, BOOL parameterdomain)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	double minu = -0.0;
	double maxu = 1.0;
	double minv = -0.0;
	double maxv = 1.0;

	outfile << "# " << (samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn << "faces" << endl;
	int index00, indexm0, indexmn, index0n;
	int num = 1;
	for (j = 0; j <= samplesn; j++)
	{
		for (i = 0; i <= samplesm; i++)
		{	//

			if (i == 0 && j == 0)
				index00 = num;
			if (i == samplesm && j == 0)
				indexm0 = num;
			if (i == samplesm && j == samplesn)
				indexmn = num;
			if (i == 0 && j == samplesn)
				index0n = num;
			num++;

			double s = minu + (double)i * (maxu - minu) / samplesm;
			double t = minv + (double)j * (maxv - minv) / samplesn;


			double u, v;
			if (pSurface->ptransformation)
			{
				s = s * (pSurface->ptransformation->umax_Reparameterization - pSurface->ptransformation->umin_Reparameterization) + pSurface->ptransformation->umin_Reparameterization;
				t = t * (pSurface->ptransformation->vmax_Reparameterization - pSurface->ptransformation->vmin_Reparameterization) + pSurface->ptransformation->vmin_Reparameterization;

				pSurface->ptransformation->Evaluation(s, t, u, v);
			}
			else
			{
				double umin = pNurbsSurface->GetUKnot(0);
				double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

				double vmin = pNurbsSurface->GetVKnot(0);
				double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

				s = umin + (umax - umin) * s;
				t = vmin + (vmax - vmin) * t;

				u = s;
				v = t;
			}

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u, v) - ZeroP3D);
			//vv1 = vv1;
			if (parameterdomain)
				outfile << "v " << u << " " << v << " " << 0 << endl;
			else
				outfile << "v " << vv1[0] << " " << vv1[1] << " " << vv1[2] << endl;
		}
	}

	for (j = 0; j < samplesn; j++)
	{
		for (i = 1; i < samplesm - 1; i++)
		{
			//
			//outfile << "f " << (samplesn + 1) * j + i + 1 <<" " << (samplesn + 1) * j + i + 2 << " "<< (samplesn + 1) * j + i + 2 + samplesn + 1 << " "<< (samplesn + 1) * j + i + 1 + samplesn + 1 << endl;
			int index1 = (samplesn + 1) * j + i + 1;
			int index2 = (samplesn + 1) * j + i + 2;
			int index3 = (samplesn + 1) * j + i + 1 + samplesn + 1;
			int index4 = (samplesn + 1) * j + i + 2 + samplesn + 1;
			if (index1 == index00 || index1 == indexm0 || index1 == indexmn || index1 == index0n ||
				index4 == index00 || index4 == indexm0 || index4 == indexmn || index4 == index0n)
			{
				outfile << "f " << index1 << " " << index2 << " " << index4 << endl;
				outfile << "f " << index1 << " " << index4 << " " << index3 << endl;
				if (index1 == index00 || index1 == indexm0 || index1 == indexmn || index1 == index0n)
				{
					TRACE("\n Edge %d %d {sharp}", index1, index2);
					TRACE("\n Edge %d %d {sharp}", index1, index4);
					TRACE("\n Edge %d %d {sharp}", index1, index3);
				}
				else
				{
					TRACE("\n Edge %d %d {sharp}", index2, index4);
					TRACE("\n Edge %d %d {sharp}", index1, index4);
					TRACE("\n Edge %d %d {sharp}", index3, index4);
				}
			}
			else if (index2 == index00 || index2 == indexm0 || index2 == indexmn || index2 == index0n ||
				index3 == index00 || index3 == indexm0 || index3 == indexmn || index3 == index0n)
			{
				outfile << "f " << index1 << " " << index2 << " " << index3 << endl;
				outfile << "f " << index2 << " " << index4 << " " << index3 << endl;
				if (index2 == index00 || index2 == indexm0 || index2 == indexmn || index2 == index0n)
				{
					TRACE("\n Edge %d %d {sharp}", index2, index1);
					TRACE("\n Edge %d %d {sharp}", index2, index3);
					TRACE("\n Edge %d %d {sharp}", index2, index4);
				}
				else
				{
					TRACE("\n Edge %d %d {sharp}", index3, index1);
					TRACE("\n Edge %d %d {sharp}", index3, index2);
					TRACE("\n Edge %d %d {sharp}", index3, index4);
				}

			}
			else
			{
				outfile << "f " << index1 << " " << index2 << " " << index3 << endl;
				outfile << "f " << index2 << " " << index4 << " " << index3 << endl;
			}

		}
	}

	outfile.close();
}

void CFormView3::OutPutSTLMesh(char* filename, BOOL parameterdomain)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int samplesm = 10;
	int samplesn = 10;

	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	double minu = -0.0;
	double maxu = 1.0;
	double minv = -0.0;
	double maxv = 1.0;

	outfile << "solid WRAP" << "\n";
	for (int ii = 0; ii < num; ii++)
	{
		CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
		C_NurbsSurf* pNurbsSurface = pNSurf->Converte_C_NurbsSurface();
		int num = 1;
		for (j = 0; j < samplesn; j++)
		{
			for (i = 0; i < samplesm; i++)
			{	//
				double s0 = minu + (double)i * (maxu - minu) / samplesm;
				double s1 = minu + (double)(i + 1) * (maxu - minu) / samplesm;
				double t0 = minv + (double)j * (maxv - minv) / samplesn;
				double t1 = minv + (double)(j + 1) * (maxv - minv) / samplesn;

				Vector3D v00 = (pNurbsSurface->ComputPosition(s0, t0) - ZeroP3D);
				Vector3D v01 = (pNurbsSurface->ComputPosition(s0, t1) - ZeroP3D);
				Vector3D v10 = (pNurbsSurface->ComputPosition(s1, t0) - ZeroP3D);
				Vector3D v11 = (pNurbsSurface->ComputPosition(s1, t1) - ZeroP3D);

				Vector3D n1 = (v10 - v00) ^ (v01 - v00);
				Vector3D n2 = (v10 - v01) ^ (v11 - v01);
				n1.SetUnit();
				n2.SetUnit();
				if (ii == 51)
				{
					n1 = n1 * (-1);
					n2 = n2 * (-1);
					Vector3D vv = v10;
					v10 = v01;
					v01 = vv;
				}
				outfile << "facet normal" << " " << n1.x << " " << n1.y << " " << n1.z << "\n";
				outfile << "outer loop" << "\n";
				outfile << "vertex" << " " << v00.x << " " << v00.y << " " << v00.z << "\n";
				outfile << "vertex" << " " << v10.x << " " << v10.y << " " << v10.z << "\n";
				outfile << "vertex" << " " << v01.x << " " << v01.y << " " << v01.z << "\n";
				outfile << "endloop" << "\n";
				outfile << "endfacet" << "\n";

				outfile << "facet normal" << " " << n2.x << " " << n2.y << " " << n2.z << "\n";
				outfile << "outer loop" << "\n";
				outfile << "vertex" << " " << v01.x << " " << v01.y << " " << v01.z << "\n";
				outfile << "vertex" << " " << v10.x << " " << v10.y << " " << v10.z << "\n";
				outfile << "vertex" << " " << v11.x << " " << v11.y << " " << v11.z << "\n";
				outfile << "endloop" << "\n";
				outfile << "endfacet" << "\n";
			}
		}
	}
	outfile << "endsolid WRAP" << "\n";

	outfile.close();
}

void CFormView3::GeneratetThreeMesh_PovRay()
{
	std::ifstream file("e:\\data.txt");

	double x, y, z, data;
	std::vector<double> m_Arraydata;
	double minz = 1e10;
	double maxz = -1e10;
	while (!file.eof())
	{
		file >> x;
		file >> y;
		file >> z;
		file >> data;

		if (minz > data)
			minz = data;
		if (maxz < data)
			maxz = data;

		m_Arraydata.push_back(x);
		m_Arraydata.push_back(y);
		m_Arraydata.push_back(z);
		m_Arraydata.push_back(data);
	}

	double dataarray[101][101];
	for (int i = 0; i < 101; i++)
		for (int j = 0; j < 101; j++)
		{
			dataarray[i][j] = -1;
		}
	for (int i = 0; i < m_Arraydata.size(); i = i + 4)
	{
		int index1 = int(m_Arraydata[i] * 100 + 0.5);
		int index2 = int(m_Arraydata[i + 1] * 100 + 0.5);
		dataarray[index1][index2] = m_Arraydata[i + 3];
	}

	std::ofstream outfile("D:\\mesh1.stl");
	Vector3D v00, v10, v11, v01;
	int times = 10;
	int k1, k2;
	int bakj;
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
		{
			//k1 = 100 - i - j;
			//k2 = 100 - i - j - 1;
			//bakj = i;
			//i = k;
			//i,j,data; i+1,j,data;i+1,j+1,data;i,j+1,data
			v00[0] = i * 0.01;
			v00[1] = j * 0.01;
			//v00[1] = j * 0.01;
			v00[2] = dataarray[i][j];

			if (v00[2] < 0 && 100 - i - j >= 0)
				TRACE("No data:(%f %f %f) \n", i * 0.01, j * 0.01, (100 - i - j) * 0.01);

			v10[0] = (i + 1) * 0.01;
			v10[1] = j * 0.01;
			v10[2] = dataarray[i + 1][j];

			v11[0] = (i + 1) * 0.01;
			v11[1] = (j + 1) * 0.01;
			v11[2] = dataarray[i + 1][j + 1];

			v01[0] = (i) * 0.01;
			v01[1] = (j + 1) * 0.01;
			v01[2] = dataarray[i][j + 1];

			if (v00[2] > 0 && v10[2] > 0 && v11[2] > 0)
			{
				Vector3D n = (v10 - v00) ^ (v11 - v10);
				n.SetUnit();
				outfile << "facet normal" << " " << n.x << " " << n.y << " " << n.z << "\n";
				outfile << "outer loop" << "\n";
				outfile << "vertex" << " " << v00.x << " " << v00.y << " " << v00.z * times << "\n";
				outfile << "vertex" << " " << v10.x << " " << v10.y << " " << v10.z * times << "\n";
				outfile << "vertex" << " " << v11.x << " " << v11.y << " " << v11.z * times << "\n";
				outfile << "endloop" << "\n";
				outfile << "endfacet" << "\n";
			}

			if (v00[2] > 0 && v11[2] > 0 && v01[2] > 0)
			{
				Vector3D n = (v11 - v00) ^ (v01 - v11);
				n.SetUnit();
				outfile << "facet normal" << " " << n.x << " " << n.y << " " << n.z << "\n";
				outfile << "outer loop" << "\n";
				outfile << "vertex" << " " << v00.x << " " << v00.y << " " << v00.z * times << "\n";
				outfile << "vertex" << " " << v11.x << " " << v11.y << " " << v11.z * times << "\n";
				outfile << "vertex" << " " << v01.x << " " << v01.y << " " << v01.z * times << "\n";
				outfile << "endloop" << "\n";
				outfile << "endfacet" << "\n";
			}
			i = bakj;
		}
	outfile.close();
	/*std::ofstream outfile("D:\\mesh3.stl");
	Vector3D v00, v10, v11, v01;
	int times = 10;
	int k1, k2;
	int bakj;
	for (int i = 0; i < 100; i++)
		for (int k = 0; k < 100; k++)
		{
			int j = 100 - i - k;
			//k1 = 100 - i - j;
			//k2 = 100 - i - j - 1;
			//bakj = i;
			//i = k;
			//i,j,data; i+1,j,data;i+1,j+1,data;i,j+1,data
			v00[0] = i * 0.01;
			v00[1] = k * 0.01;
			//v00[1] = j * 0.01;
			v00[2] = dataarray[i][j];

			if (v00[2] < 0 && 100 - i - j >= 0)
				TRACE("No data:(%f %f %f) \n", i * 0.01, j * 0.01, (100 - i - j) * 0.01);

			v10[0] = (i + 1) * 0.01;
			v10[1] = k * 0.01;
			v10[2] = dataarray[i + 1][100 - i - 1 - k];

			v11[0] = (i + 1) * 0.01;
			v11[1] = (k + 1) * 0.01;
			v11[2] = dataarray[i + 1][98 - i - k];

			v01[0] = (i) * 0.01;
			v01[1] = (k + 1) * 0.01;
			v01[2] = dataarray[i][99 - i - k];

			if (v00[2] > 0 && v10[2] > 0 && v11[2] > 0)
			{
				Vector3D n = (v10 - v00) ^ (v11 - v10);
				n.SetUnit();
				outfile << "facet normal" << " " << n.x << " " << n.y << " " << n.z << "\n";
				outfile << "outer loop" << "\n";
				outfile << "vertex" << " " << v00.x << " " << v00.y << " " << v00.z * times << "\n";
				outfile << "vertex" << " " << v10.x << " " << v10.y << " " << v10.z  * times << "\n";
				outfile << "vertex" << " " << v11.x << " " << v11.y << " " << v11.z  * times << "\n";
				outfile << "endloop" << "\n";
				outfile << "endfacet" << "\n";
			}

			if (v00[2] > 0 && v11[2] > 0 && v01[2] > 0)
			{
				Vector3D n = (v11 - v00) ^ (v01 - v11);
				n.SetUnit();
				outfile << "facet normal" << " " << n.x << " " << n.y << " " << n.z << "\n";
				outfile << "outer loop" << "\n";
				outfile << "vertex" << " " << v00.x << " " << v00.y << " " << v00.z  * times << "\n";
				outfile << "vertex" << " " << v11.x << " " << v11.y << " " << v11.z  * times << "\n";
				outfile << "vertex" << " " << v01.x << " " << v01.y << " " << v01.z  * times << "\n";
				outfile << "endloop" << "\n";
				outfile << "endfacet" << "\n";
			}
			//i = bakj;
		}
	outfile.close();*/
}
void CFormView3::OutPutObjMesh(CNurbsSuface* pSurface, int samplesm, int samplesn, char* filename, BOOL parameterdomain)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	double minu = -0.0;
	double maxu = 1.0;
	double minv = -0.0;
	double maxv = 1.0;

	outfile << "# " << (samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn << "faces" << endl;
	int index00, indexm0, indexmn, index0n;
	int num = 1;
	for (j = 0; j <= samplesn; j++)
	{
		for (i = 0; i <= samplesm; i++)
		{	//

			if (i == 0 && j == 0)
				index00 = num;
			if (i == samplesm && j == 0)
				indexm0 = num;
			if (i == samplesm && j == samplesn)
				indexmn = num;
			if (i == 0 && j == samplesn)
				index0n = num;
			num++;

			double s = minu + (double)i * (maxu - minu) / samplesm;
			double t = minv + (double)j * (maxv - minv) / samplesn;


			double u, v;
			if (pSurface->ptransformation)
			{
				s = s * (pSurface->ptransformation->umax_Reparameterization - pSurface->ptransformation->umin_Reparameterization) + pSurface->ptransformation->umin_Reparameterization;
				t = t * (pSurface->ptransformation->vmax_Reparameterization - pSurface->ptransformation->vmin_Reparameterization) + pSurface->ptransformation->vmin_Reparameterization;

				pSurface->ptransformation->Evaluation(s, t, u, v);
			}
			else
			{
				double umin = pNurbsSurface->GetUKnot(0);
				double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

				double vmin = pNurbsSurface->GetVKnot(0);
				double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

				s = umin + (umax - umin) * s;
				t = vmin + (vmax - vmin) * t;

				u = s;
				v = t;
			}

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u, v) - ZeroP3D);
			//vv1 = vv1;
			if (parameterdomain)
				outfile << "v " << u << " " << v << " " << 0 << endl;
			else
				outfile << "v " << vv1[0] << " " << vv1[1] << " " << vv1[2] << endl;
		}
	}

	for (j = 0; j < samplesn; j++)
	{
		for (i = 0; i < samplesm; i++)
		{
			//
			//outfile << "f " << (samplesn + 1) * j + i + 1 <<" " << (samplesn + 1) * j + i + 2 << " "<< (samplesn + 1) * j + i + 2 + samplesn + 1 << " "<< (samplesn + 1) * j + i + 1 + samplesn + 1 << endl;
			int index1 = (samplesm + 1) * j + i + 1;
			int index2 = (samplesm + 1) * j + i + 2;
			int index3 = (samplesm + 1) * j + i + 1 + samplesm + 1;
			int index4 = (samplesm + 1) * j + i + 2 + samplesm + 1;
			if (index1 == index00 || index1 == indexm0 || index1 == indexmn || index1 == index0n ||
				index4 == index00 || index4 == indexm0 || index4 == indexmn || index4 == index0n)
			{
				outfile << "f " << index1 << " " << index2 << " " << index4 << endl;
				outfile << "f " << index1 << " " << index4 << " " << index3 << endl;
				if (index1 == index00 || index1 == indexm0 || index1 == indexmn || index1 == index0n)
				{
					TRACE("\n Edge %d %d {sharp}", index1, index2);
					TRACE("\n Edge %d %d {sharp}", index1, index4);
					TRACE("\n Edge %d %d {sharp}", index1, index3);
				}
				else
				{
					TRACE("\n Edge %d %d {sharp}", index2, index4);
					TRACE("\n Edge %d %d {sharp}", index1, index4);
					TRACE("\n Edge %d %d {sharp}", index3, index4);
				}
			}
			else if (index2 == index00 || index2 == indexm0 || index2 == indexmn || index2 == index0n ||
				index3 == index00 || index3 == indexm0 || index3 == indexmn || index3 == index0n)
			{
				outfile << "f " << index1 << " " << index2 << " " << index3 << endl;
				outfile << "f " << index2 << " " << index4 << " " << index3 << endl;
				if (index2 == index00 || index2 == indexm0 || index2 == indexmn || index2 == index0n)
				{
					TRACE("\n Edge %d %d {sharp}", index2, index1);
					TRACE("\n Edge %d %d {sharp}", index2, index3);
					TRACE("\n Edge %d %d {sharp}", index2, index4);
				}
				else
				{
					TRACE("\n Edge %d %d {sharp}", index3, index1);
					TRACE("\n Edge %d %d {sharp}", index3, index2);
					TRACE("\n Edge %d %d {sharp}", index3, index4);
				}

			}
			else
			{
				outfile << "f " << index1 << " " << index2 << " " << index3 << endl;
				outfile << "f " << index2 << " " << index4 << " " << index3 << endl;
			}

		}
	}

	outfile.close();
}

void CFormView3::OutputSpecificPointsOntheSurface(C_NurbsSurf* pNurbsSurface, char* filename)
{
	//output four points on the surface
	std::ofstream outfile(filename);

	Vector3D v1 = pNurbsSurface->ComputPosition(0.35, 0.1) - ZeroP3D;
	Vector3D v2 = pNurbsSurface->ComputPosition(0.35, 0.9) - ZeroP3D;
	Vector3D v3 = pNurbsSurface->ComputPosition(0.7, 0.1) - ZeroP3D;
	Vector3D v4 = pNurbsSurface->ComputPosition(0.7, 0.9) - ZeroP3D;

	outfile << "sphere{<" << v1.x << " , " << v1.y << " , " << -v1.z << ">,spherer}" << endl;
	outfile << "sphere{<" << v2.x << " , " << v2.y << " , " << -v2.z << ">,spherer}" << endl;
	outfile << "sphere{<" << v3.x << " , " << v3.y << " , " << -v3.z << ">,spherer}" << endl;
	outfile << "sphere{<" << v4.x << " , " << v4.y << " , " << -v4.z << ">,spherer}" << endl;
}
void CFormView3::OutPutMeshFaces_PovRay_Smooth_nonflip(CMesh3d_OpenMesh* pMesh)
{
	FPROP_FACE_SELECTED_FLAG fprop_selectedflag;

	//face selected flag
	if (!pMesh->pmesh->get_property_handle(fprop_selectedflag, PROP_FACE_SELECTED))
		pMesh->pmesh->add_property(fprop_selectedflag, PROP_FACE_SELECTED);

	std::ofstream outfile1("D:\\face1.inc");
	std::ofstream outfile2("D:\\face2.inc");
	std::ofstream outfile3("D:\\face3.inc");

	int nvertices = pMesh->pmesh->n_vertices();
	outfile1 << "mesh2 {" << endl;
	outfile1 << "vertex_vectors {" << endl;
	outfile1 << nvertices << "," << endl;

	outfile2 << "mesh2 {" << endl;
	outfile2 << "vertex_vectors {" << endl;
	outfile2 << nvertices << "," << endl;

	outfile3 << "mesh2 {" << endl;
	outfile3 << "vertex_vectors {" << endl;
	outfile3 << nvertices << "," << endl;

	int i = 0;
	for (MyMesh::VertexIter v_it = pMesh->pmesh->vertices_begin(); v_it != pMesh->pmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d v1 = pMesh->pmesh->point(v_it.handle());

		double x = v1[0];
		double y = v1[1];
		double z = v1[2];

		outfile1 << "<" << x << "," << y << "," << -z << ">";
		outfile2 << "<" << x << "," << y << "," << -z << ">";
		outfile3 << "<" << x << "," << y << "," << -z << ">";
		if (i < nvertices - 1)
		{
			outfile1 << "," << endl;
			outfile2 << "," << endl;
			outfile3 << "," << endl;
		}
		else
		{
			outfile1 << "}" << endl;
			outfile2 << "}" << endl;
			outfile3 << "}" << endl;
		}
		i++;
	}

	outfile1 << "normal_vectors {" << endl;
	outfile1 << nvertices << "," << endl;
	outfile2 << "normal_vectors {" << endl;
	outfile2 << nvertices << "," << endl;
	outfile3 << "normal_vectors {" << endl;
	outfile3 << nvertices << "," << endl;
	for (int i = 0; i < nvertices; i++)
	{
		outfile1 << "<" << 0 << "," << 0 << "," << -1 << ">";
		outfile2 << "<" << 0 << "," << 0 << "," << -1 << ">";
		outfile3 << "<" << 0 << "," << 0 << "," << -1 << ">";
		if (i < nvertices - 1)
		{
			outfile1 << "," << endl;
			outfile2 << "," << endl;
			outfile3 << "," << endl;
		}
		else
		{
			outfile1 << "}" << endl;
			outfile2 << "}" << endl;
			outfile3 << "}" << endl;
		}
	}

	//texture
	outfile1 << "texture_list {" << endl;
	outfile1 << nvertices << "," << endl;
	outfile2 << "texture_list {" << endl;
	outfile2 << nvertices << "," << endl;
	outfile3 << "texture_list {" << endl;
	outfile3 << nvertices << "," << endl;
	for (int i = 0; i < nvertices; i++)
	{
		//outfile << "texture {pigment{rgbt<" << r << "," << g << "," << b <<", thres_trans > } }" << endl;	
		outfile1 << "texture {mesh_tex1}" << endl;
		outfile2 << "texture {mesh_tex2}" << endl;
		outfile3 << "texture {mesh_tex3}" << endl;
	}
	outfile1 << "}" << endl;
	outfile1 << "face_indices {" << endl;
	outfile2 << "}" << endl;
	outfile2 << "face_indices {" << endl;
	outfile3 << "}" << endl;
	outfile3 << "face_indices {" << endl;

	int facenum1 = 0;
	int facenum2 = 0;
	int facenum3 = 0;
	int flag;
	std::vector<int> vhandleidxs;
	for (MyMesh::FaceIter f_it = pMesh->pmesh->faces_begin(); f_it != pMesh->pmesh->faces_end(); ++f_it)
	{
		flag = pMesh->pmesh->property(fprop_selectedflag, f_it.handle());

		if (flag == 0)
		{
			facenum1++;
		}
		else if (flag == 1)
		{
			facenum2++;
		}
		else if (flag == 2)
		{
			facenum3++;
		}
	}
	outfile1 << facenum1 << "," << endl;
	outfile2 << facenum2 << "," << endl;
	outfile3 << facenum3 << "," << endl;
	int faceindex1 = 0;
	int faceindex2 = 0;
	int faceindex3 = 0;
	//std::vector<int> vhandleidxs;
	for (MyMesh::FaceIter f_it = pMesh->pmesh->faces_begin(); f_it != pMesh->pmesh->faces_end(); ++f_it)
	{
		flag = pMesh->pmesh->property(fprop_selectedflag, f_it.handle());
		vhandleidxs.clear();
		for (MyMesh::FaceVertexIter fv_it = pMesh->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			vhandleidxs.push_back(fv_it.handle().idx());
		}
		if (flag == 0)
		{
			outfile1 << "<" << vhandleidxs[0] << "," << vhandleidxs[1] << "," << vhandleidxs[2] << ">," << vhandleidxs[0] << "," << vhandleidxs[1] << "," << vhandleidxs[2];
			if (faceindex1 < facenum1 - 1)
				outfile1 << "," << endl;
			else
				outfile1 << "}" << endl;
			faceindex1++;
		}
		else if (flag == 1)
		{
			outfile2 << "<" << vhandleidxs[0] << "," << vhandleidxs[1] << "," << vhandleidxs[2] << ">," << vhandleidxs[0] << "," << vhandleidxs[1] << "," << vhandleidxs[2];
			if (faceindex2 < facenum2 - 1)
				outfile2 << "," << endl;
			else
				outfile2 << "}" << endl;
			faceindex2++;
		}
		else if (flag == 2)
		{
			outfile3 << "<" << vhandleidxs[0] << "," << vhandleidxs[1] << "," << vhandleidxs[2] << ">," << vhandleidxs[0] << "," << vhandleidxs[1] << "," << vhandleidxs[2];
			if (faceindex3 < facenum3 - 1)
				outfile3 << "," << endl;
			else
				outfile3 << "}" << endl;
			faceindex3++;
		}
	}

	outfile1 << "}" << endl;
	outfile2 << "}" << endl;
	outfile3 << "}" << endl;

	outfile1.close();
	outfile2.close();
	outfile3.close();
}
void CFormView3::OutPutMeshFaces_PovRay_Smooth_Anchor(CMesh3d* pMesh, char* filename)
{
	//pMesh->CalculateNormalPerFace();
	//pMesh->CalculateNormalPerVertex();

	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	//samplesm = samplesm * 10;
	/*for(int i = 0; i < pMesh->NbFace(); i++)
	{
	double r,g,b;
	CFace3d* pFace = pMesh->GetFace(i);
	CVertex3d* pVertex1 = pFace->v1();
	CVertex3d* pVertex2 = pFace->v2();
	CVertex3d* pVertex3 = pFace->v3();

	CColor *color1 = pVertex1->GetColor();
	CColor *color2 = pVertex2->GetColor();
	CColor *color3 = pVertex3->GetColor();

	r = (color1->r() + color2->r() + color3->r())  / (255.0 * 3);
	g = (color1->g() + color2->g() + color3->g()) / (255.0 * 3);
	b = (color1->b() + color2->b() + color3->b()) / (255.0 * 3);

	outfile <<"#declare T"<<i<<" "<<"= texture { pigment{ rgb<"<<r<<","<<g<<","<<b<<">}}" <<endl;
	}*/
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		pMesh->GetVertex(i)->m_OriginalIndex = i;
	}
	outfile << "mesh2 {" << endl;

	outfile << "vertex_vectors {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		outfile << "<" << x << "," << y << "," << -z << ">";
		if (i < pMesh->NbVertex() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;

	}
	outfile << "normal_vectors {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		CVector3d* pVector = pVertex->GetNormal();
		double x = pVector->x();
		double y = pVector->y();
		double z = pVector->z();
		outfile << "<" << x << "," << y << "," << -z << ">";
		if (i < pMesh->NbVertex() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
	}

	//texture
	outfile << "texture_list {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		CColor* color1 = pVertex->GetColor();
		double r, g, b;
		r = (color1->r()) / (255.0);
		g = (color1->g()) / (255.0);
		b = (color1->b()) / (255.0);

		outfile << "texture {pigment{rgbt<" << r << "," << g << "," << b << ", thres_trans > } }" << endl;
		//outfile << "texture {mesh_tex}" << endl;	
	}
	outfile << "}" << endl;

	outfile << "face_indices {" << endl;
	int facenum = 0;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		/*int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;

		if (index1 || index2 || index3)
			continue;*/

		facenum++;
	}
	outfile << facenum << "," << endl;
	int faceindex = 0;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		/*int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;

		if (index1 || index2 || index3)
			continue;*/

		outfile << "<" << pFace->v1()->m_OriginalIndex << "," << pFace->v2()->m_OriginalIndex << "," << pFace->v3()->m_OriginalIndex << ">," << pFace->v1()->m_OriginalIndex << "," << pFace->v2()->m_OriginalIndex << "," << pFace->v3()->m_OriginalIndex;

		if (faceindex < facenum - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
		faceindex++;
	}
	/*for(int i = 0; i < pMesh->NbFace(); i++)
	{
	CFace3d* pFace = pMesh->GetFace(i);
	CVertex3d* pVertex1 = pFace->v1();

	CVertex3d* pVertex2 = pFace->v2();
	CVertex3d* pVertex3 = pFace->v3();

	outfile << "smooth_triangle { < " << pVertex1->x() << "," << pVertex1->y() << " , " << -pVertex1->z() << " >,<" << pVertex1->GetNormal()->x()<< "," << pVertex1->GetNormal()->y() << "," << -pVertex1->GetNormal()->z() << ">,<";
	outfile << pVertex2->x() << "," << pVertex2->y() << "," << -pVertex2->z() << ">,<" << pVertex2->GetNormal()->x() << "," << pVertex2->GetNormal()->y() << "," << -pVertex2->GetNormal()->z() << ">,<";
	outfile << pVertex3->x() << "," << pVertex3->y() << "," << -pVertex3->z() << ">,<" << pVertex3->GetNormal()->x() << "," << pVertex3->GetNormal()->y() << "," << -pVertex3->GetNormal()->z() << "> texture{T"<<i<<"}}"<<endl;
	}*/

	//outfile << "texture {" << endl;
	//outfile << "samplesmesh_tex" << endl;
	//outfile << "}" << endl;
	outfile << "}" << endl;
	outfile.close();
}
void CFormView3::OutPutMeshFaces_PovRay_Smooth(CMesh3d* pMesh, char* filename)
{
	/*for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->m_Coord[2] = pVertex->m_Coord[2] * 10;
	}*/
	pMesh->CalculateNormalPerFace();
	pMesh->CalculateNormalPerVertex();

	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	//samplesm = samplesm * 10;
	double minz = 1e10;
	double maxz = -1e10;

	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex1 = pMesh->GetVertex(i);
		double z = pVertex1->z();
		if (z < minz)
			minz = z;
		if (z > maxz)
			maxz = z;
	}
	//maxz = 2;
	CImage m_HotTextureImage;
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");
	//int x = i * 0.01 * m_HotTextureImage.GetWidth();
	//if (x == m_HotTextureImage.GetWidth())
	//	x--;
	//COLORREF color = m_HotTextureImage.GetPixel(x, 0);

	/*for(int i = 0; i < pMesh->NbFace(); i++)
	{
		double r,g,b;
		CFace3d* pFace = pMesh->GetFace(i);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		double z = (pVertex1->z() + pVertex2->z() + pVertex3->z()) / 3;
		double t = (z - minz) / (maxz - minz);
		int pos = t * m_HotTextureImage.GetWidth() + 0.5;
		if (pos == m_HotTextureImage.GetWidth())
			pos--;

		COLORREF color = m_HotTextureImage.GetPixel(pos, 0);

		r = GetRValue(color) / 256.0;
		g = GetGValue(color) / 256.0;
		b = GetBValue(color) / 256.0;*/

		//r = color
		//CColor *color1 = pVertex1->GetColor();
		//CColor *color2 = pVertex2->GetColor();
		//CColor *color3 = pVertex3->GetColor();

		//r = (color1->r() + color2->r() + color3->r())  / (255.0 * 3);
		//g = (color1->g() + color2->g() + color3->g()) / (255.0 * 3);
		//b = (color1->b() + color2->b() + color3->b()) / (255.0 * 3);

		//outfile <<"#declare T"<<i<<" "<<"= texture { pigment{ rgb<"<<r<<","<<g<<","<<b<<">}}" <<endl;
	//}
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		pMesh->GetVertex(i)->m_Flag = i;
	}
	outfile << "mesh2 {" << endl;

	outfile << "vertex_vectors {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		outfile << "<" << x << "," << y << "," << -z << ">";
		if (i < pMesh->NbVertex() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;

	}
	outfile << "normal_vectors {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		CVector3d* pVector = pVertex->GetNormal();
		double x = pVector->x();
		double y = pVector->y();
		double z = pVector->z();
		outfile << "<" << x << "," << y << "," << -z << ">";
		if (i < pMesh->NbVertex() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
	}

	//texture
	outfile << "texture_list {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);

		double z = pVertex->z();
		double t = (z - minz) / (maxz - minz);
		int pos = t * m_HotTextureImage.GetWidth() + 0.5;
		if (pos >= m_HotTextureImage.GetWidth())
			pos = m_HotTextureImage.GetWidth() - 1;

		COLORREF color = m_HotTextureImage.GetPixel(pos, 0);

		double r, g, b;
		r = GetRValue(color) / 256.0;
		g = GetGValue(color) / 256.0;
		b = GetBValue(color) / 256.0;

		outfile << "texture {pigment{rgbt<" << r << "," << g << "," << b << ", thres_trans > } }" << endl;
		//outfile << "texture {mesh_tex}" << endl;	
	}
	outfile << "}" << endl;

	outfile << "face_indices {" << endl;
	outfile << pMesh->NbFace() << "," << endl;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;

		outfile << "<" << index1 << "," << index2 << "," << index3 << ">," << index1 << "," << index2 << "," << index3;

		if (i < pMesh->NbFace() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
	}
	/*outfile << "mesh {" << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->m_Coord[2] = pVertex->m_Coord[2] * 10;
	}
	for(int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		CVertex3d* pVertex1 = pFace->v1();

		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		outfile << "smooth_triangle { < " << pVertex1->x() << "," << pVertex1->y() << " , " << -pVertex1->z() << " >,<" << pVertex1->GetNormal()->x()<< "," << pVertex1->GetNormal()->y() << "," << -pVertex1->GetNormal()->z() << ">,<";
		outfile << pVertex2->x() << "," << pVertex2->y() << "," << -pVertex2->z() << ">,<" << pVertex2->GetNormal()->x() << "," << pVertex2->GetNormal()->y() << "," << -pVertex2->GetNormal()->z() << ">,<";
		//outfile << pVertex3->x() << "," << pVertex3->y() << "," << -pVertex3->z() << ">,<" << pVertex3->GetNormal()->x() << "," << pVertex3->GetNormal()->y() << "," << -pVertex3->GetNormal()->z() << "> texture{T"<<i<<"}}"<<endl;
		outfile << pVertex3->x() << "," << pVertex3->y() << "," << -pVertex3->z() << ">,<" << pVertex3->GetNormal()->x() << "," << pVertex3->GetNormal()->y() << "," << -pVertex3->GetNormal()->z() << "> }" << endl;
	}

	outfile << "texture {" << endl;
	outfile << "samplesmesh_tex" << endl;
	outfile << "}" << endl;*/
	outfile << "}" << endl;
	outfile.close();
}

void CFormView3::OutPutMeshFaces_PovRay_Smooth_flags(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	pMesh->BuildAdjacency();
	pMesh->CalculateNormalPerFace();
	pMesh->CalculateNormalPerVertex();
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		pMesh->GetVertex(i)->m_OriginalIndex = i;
	}
	outfile << "#include \"face.obj_POV_mat.inc\"" << endl;
	outfile << "#declare face_obj_material_0_= mesh2 {" << endl;

	outfile << "vertex_vectors {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		outfile << "<" << x << "," << y << "," << -z << ">";
		if (i < pMesh->NbVertex() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;

	}
	outfile << "normal_vectors {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		CVector3d* pVector = pVertex->GetNormal();
		double x = pVector->x();
		double y = pVector->y();
		double z = pVector->z();
		outfile << "<" << x << "," << y << "," << -z << ">";
		if (i < pMesh->NbVertex() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
	}

	//texture
	outfile << "texture_list {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		CColor* color1 = pVertex->GetColor();
		double r, g, b;
		r = (color1->r()) / (255.0);
		g = (color1->g()) / (255.0);
		b = (color1->b()) / (255.0);

		outfile << "texture {pigment{rgbt<" << r << "," << g << "," << b << ", thres_trans > } }" << endl;
		//outfile << "texture {mesh_tex}" << endl;
	}
	outfile << "}" << endl;

	outfile << "face_indices {" << endl;
	int facenum = 0;
	int scalenum = 3;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;
		//CVertex3d* pVertex1 = pMesh->GetVertex(4109);
		//CVertex3d* pVertex2 = pMesh->GetVertex(4901);
		//double dis = Distance(pVertex1, pVertex2) * scalenum;
		if (index1 || index2 || index3)
			continue;

		//if (Distance(pFace->v1(), pFace->v2()) > dis || Distance(pFace->v2(), pFace->v3()) > dis || Distance(pFace->v1(), pFace->v3()) > dis)
		//	continue;
		facenum++;
	}
	outfile << facenum << "," << endl;
	int faceindex = 0;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;
		CVertex3d* pVertex1 = pMesh->GetVertex(4109);
		CVertex3d* pVertex2 = pMesh->GetVertex(4901);
		double dis = Distance(pVertex1, pVertex2) * scalenum;
		if (index1 || index2 || index3)
			continue;

		if (Distance(pFace->v1(), pFace->v2()) > dis || Distance(pFace->v2(), pFace->v3()) > dis || Distance(pFace->v1(), pFace->v3()) > dis)
			continue;

		outfile << "<" << pFace->v1()->m_OriginalIndex << "," << pFace->v2()->m_OriginalIndex << "," << pFace->v3()->m_OriginalIndex << ">," << pFace->v1()->m_OriginalIndex << "," << pFace->v2()->m_OriginalIndex << "," << pFace->v3()->m_OriginalIndex;

		if (faceindex < facenum - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
		faceindex++;
	}
	/*outfile << "uv_indices {" << endl;
	outfile << facenum << "," << endl;
	faceindex = 0;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;

		if (index1 || index2 || index3)
			continue;

		outfile << "<" << pFace->v1()->m_OriginalIndex << "," << pFace->v2()->m_OriginalIndex << "," << pFace->v3()->m_OriginalIndex << ">";

		if (faceindex < facenum - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
		faceindex++;
	}
	outfile << "}" << endl;*/
	outfile << "inside_vector <0, 0, 1> }" << endl;

	outfile << "#declare face_ = object{ face_obj_material_0_  material{ material_0_ } hollow }" << endl;

	std::vector<std::pair<int, int>> m_ArrayEdges;
	std::pair<int, int> pair, pair1;
	faceindex = 0;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;
		CVertex3d* pVertex1 = pMesh->GetVertex(4109);
		CVertex3d* pVertex2 = pMesh->GetVertex(4901);
		double dis = Distance(pVertex1, pVertex2) * scalenum;
		if (index1 || index2 || index3)
			continue;

		if (Distance(pFace->v1(), pFace->v2()) > dis || Distance(pFace->v2(), pFace->v3()) > dis || Distance(pFace->v1(), pFace->v3()) > dis)
			continue;

		pair.first = pFace->v1()->m_OriginalIndex;
		pair.second = pFace->v2()->m_OriginalIndex;

		pair1.first = pFace->v2()->m_OriginalIndex;
		pair1.second = pFace->v1()->m_OriginalIndex;
		if (std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair) == m_ArrayEdges.end() &&
			std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair1) == m_ArrayEdges.end())
		{
			m_ArrayEdges.push_back(pair);
		}

		pair.first = pFace->v2()->m_OriginalIndex;
		pair.second = pFace->v3()->m_OriginalIndex;

		pair1.first = pFace->v3()->m_OriginalIndex;
		pair1.second = pFace->v2()->m_OriginalIndex;
		if (std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair) == m_ArrayEdges.end() &&
			std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair1) == m_ArrayEdges.end())
		{
			m_ArrayEdges.push_back(pair);
		}

		pair.first = pFace->v3()->m_OriginalIndex;
		pair.second = pFace->v1()->m_OriginalIndex;

		pair1.first = pFace->v1()->m_OriginalIndex;
		pair1.second = pFace->v3()->m_OriginalIndex;
		if (std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair) == m_ArrayEdges.end() &&
			std::find(m_ArrayEdges.begin(), m_ArrayEdges.end(), pair1) == m_ArrayEdges.end())
		{
			m_ArrayEdges.push_back(pair);
		}
		//outfile << "<" << pFace->v1()->m_OriginalIndex << "," << pFace->v2()->m_OriginalIndex << "," << pFace->v3()->m_OriginalIndex << ">," << pFace->v1()->m_OriginalIndex << "," << pFace->v2()->m_OriginalIndex << "," << pFace->v3()->m_OriginalIndex;
	}
	outfile << "#declare vn = " << pMesh->NbVertex() << ";\n";
	outfile << "#declare vertex_list = array[" << pMesh->NbVertex() << "]\n{";
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		outfile << "<" << pMesh->GetVertex(i)->x() << "," << pMesh->GetVertex(i)->y() << "," << -pMesh->GetVertex(i)->z() << ">";

		if (i < pMesh->NbVertex() - 1)
		{
			outfile << ",";
		}
		else
		{
			outfile << "}\n";
		}
	}
	outfile << "#declare en = " << m_ArrayEdges.size() << ";\n";
	outfile << "#declare edge_list = array[" << m_ArrayEdges.size() << "]\n{";
	for (int i = 0; i < m_ArrayEdges.size(); i++)
	{
		pair = m_ArrayEdges[i];
		outfile << "< " << pair.first << ", " << pair.second << ", 0 >";
		if (i < m_ArrayEdges.size() - 1)
		{
			outfile << ",";
		}
		else
		{
			outfile << "}";
		}
	}

	outfile << "\n #declare jn = " << pMesh->m_ArrayAnchorPoints.size() << ";\n";
	outfile << "#declare junction_list = array[" << pMesh->m_ArrayAnchorPoints.size() << "]\n{";
	for (int i = 0; i < pMesh->m_ArrayAnchorPoints.size(); i++)
	{
		outfile << "< " << pMesh->m_ArrayAnchorPoints[i] << ", " << 0 << ", 0 >";
		if (i < pMesh->m_ArrayAnchorPoints.size() - 1)
		{
			outfile << ",";
		}
		else
		{
			outfile << "}";
		}
	}
	outfile.close();
}
void CFormView3::OutPutMeshVertices_PovRay(CMesh3d* pMesh, char* filename)
{
	std::ofstream outfile2(filename);

	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* v1 = pMesh->GetVertex(i);
		outfile2 << "sphere {<" << v1->x() << " , " << v1->y() << " , " << -v1->z() << " >,r_vertex1 texture { vertex_tex1 } }" << endl;
	}
	outfile2.close();
}
void CFormView3::OutPutMeshFaces_PovRay(CMesh3d* pMesh, char* filename)
{
	//pMesh->CalculateNormalPerFace();
	//pMesh->CalculateNormalPerVertex();

	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	//samplesm = samplesm * 10;
	/*for(int i = 0; i < pMesh->NbFace(); i++)
	{
		double r,g,b;
		CFace3d* pFace = pMesh->GetFace(i);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		CColor *color1 = pVertex1->GetColor();
		CColor *color2 = pVertex2->GetColor();
		CColor *color3 = pVertex3->GetColor();

		r = (color1->r() + color2->r() + color3->r())  / (255.0 * 3);
		g = (color1->g() + color2->g() + color3->g()) / (255.0 * 3);
		b = (color1->b() + color2->b() + color3->b()) / (255.0 * 3);

		outfile <<"#declare T"<<i<<" "<<"= texture { pigment{ rgb<"<<r<<","<<g<<","<<b<<">}}" <<endl;
	}*/
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		double r, g, b;
		CFace3d* pFace = pMesh->GetFace(i);

		CColor* color1 = pFace->GetColor();

		r = (color1->r());
		g = (color1->g());
		b = (color1->b());

		outfile << "#declare T" << i << " " << "= texture { pigment{ rgbt<" << r << "," << g << "," << b << ",tr> / 255} finish { ambient thres_ambient diffuse thres_diffuse phong thres_phong  }}" << endl;
	}
	outfile << "mesh {" << endl;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		outfile << "smooth_triangle { < " << pVertex1->x() << "," << pVertex1->y() << " , " << -pVertex1->z() << " >,<" << pVertex1->GetNormal()->x() << "," << pVertex1->GetNormal()->y() << "," << -pVertex1->GetNormal()->z() << ">,<";
		outfile << pVertex2->x() << "," << pVertex2->y() << "," << -pVertex2->z() << ">,<" << pVertex2->GetNormal()->x() << "," << pVertex2->GetNormal()->y() << "," << -pVertex2->GetNormal()->z() << ">,<";
		outfile << pVertex3->x() << "," << pVertex3->y() << "," << -pVertex3->z() << ">,<" << pVertex3->GetNormal()->x() << "," << pVertex3->GetNormal()->y() << "," << -pVertex3->GetNormal()->z() << "> texture{T" << i << "}}" << endl;
		//outfile << pVertex3->x() << "," << pVertex3->y() << "," << -pVertex3->z() << ">,<" << pVertex3->GetNormal()->x() << "," << pVertex3->GetNormal()->y() << "," << -pVertex3->GetNormal()->z() <<">}"<<endl;
	}

	//outfile << "texture {" << endl;
	//outfile << "samplesmesh_tex" << endl;
	//outfile << "}" << endl;
	outfile << "}" << endl;
	outfile.close();
}
void CFormView3::OutPutMeshFaces_PovRay(int samplesm, int samplesn, char* filename)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	std::ofstream outfile(filename);
	CString s1;
	int i, j;

	outfile << "mesh {" << endl;
	CNurbsSuface* pNurbsSurface;
	for (int ii = 0; ii < num; ii++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
			for (j = 0; j < samplesn; j++)
			{
				for (i = 0; i < samplesm; i++)
				{	//
					double s1 = (double)i / samplesm;
					double s2 = (double)(i + 1) / samplesm;
					double t1 = (double)j / samplesn;
					double t2 = (double)(j + 1) / samplesn;

					double umin, umax, vmin, vmax;
					if (pNurbsSurface->ptransformation)
					{
						umin = pNurbsSurface->ptransformation->umin_Reparameterization;
						umax = pNurbsSurface->ptransformation->umax_Reparameterization;

						vmin = pNurbsSurface->ptransformation->vmin_Reparameterization;
						vmax = pNurbsSurface->ptransformation->vmax_Reparameterization;
					}
					else
					{
						umin = pNurbsSurface->Converte_C_NurbsSurface()->GetUKnot(0);
						umax = pNurbsSurface->Converte_C_NurbsSurface()->GetUKnot(pNurbsSurface->Converte_C_NurbsSurface()->GetCtrlNumberU() + pNurbsSurface->Converte_C_NurbsSurface()->GetOrderU() - 1);

						vmin = pNurbsSurface->Converte_C_NurbsSurface()->GetVKnot(0);
						vmax = pNurbsSurface->Converte_C_NurbsSurface()->GetVKnot(pNurbsSurface->Converte_C_NurbsSurface()->GetCtrlNumberV() + pNurbsSurface->Converte_C_NurbsSurface()->GetOrderV() - 1);
					}

					s1 = umin + (umax - umin) * s1;
					s2 = umin + (umax - umin) * s2;
					t1 = vmin + (vmax - vmin) * t1;
					t2 = vmin + (vmax - vmin) * t2;

					double u1 = s1;
					double v1 = t1;

					double u2 = s2;
					double v2 = t2;

					Vector3D vv1 = (pNurbsSurface->ComputPosition(u1, v1) - ZeroP3D);

					Vector3D vv2 = (pNurbsSurface->ComputPosition(u2, v1) - ZeroP3D);

					Vector3D vv3 = (pNurbsSurface->ComputPosition(u2, v2) - ZeroP3D);

					Vector3D vv4 = (pNurbsSurface->ComputPosition(u1, v2) - ZeroP3D);

					Vector3D n1 = (vv2 - vv1) ^ (vv3 - vv1);
					n1.SetUnit();
					Vector3D n2 = (vv3 - vv1) ^ (vv4 - vv1);
					n2.SetUnit();

					outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
					outfile << vv2[0] << "," << vv2[1] << "," << -vv2[2] << ">,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
					outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">}" << endl;

					outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n2[0] << "," << n2[1] << "," << -n2[2] << ">,<";
					outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n2[0] << "," << n2[1] << "," << -n2[2] << ">,<";
					outfile << vv4[0] << "," << vv4[1] << "," << -vv4[2] << ">,<" << n2[0] << "," << n2[1] << "," << -n2[2] << ">}" << endl;
				}
			}
		}
	}

	outfile << "texture {" << endl;
	outfile << "samplesmesh_tex" << endl;
	outfile << "}" << endl;
	outfile << "}" << endl;
	outfile.close();
}
void CFormView3::OutPutMeshFaces_PovRay(CNurbsSuface* pNurbsSurface, int samplesm, int samplesn, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;

	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	//samplesm = samplesm * 10;
	outfile << "mesh {" << endl;
	for (j = 0; j < samplesn; j++)
	{
		for (i = 0; i < samplesm; i++)
		{	//
			double s1 = (double)i / samplesm;
			double s2 = (double)(i + 1) / samplesm;
			double t1 = (double)j / samplesn;
			double t2 = (double)(j + 1) / samplesn;

			double umin, umax, vmin, vmax;
			if (pNurbsSurface->ptransformation == NULL)
			{
				umin = pNurbsSurface->m_uknot[0];
				umax = pNurbsSurface->m_uknot[pNurbsSurface->m_uknotcount - 1];
				vmin = pNurbsSurface->m_vknot[0];
				vmax = pNurbsSurface->m_vknot[pNurbsSurface->m_vknotcount - 1];
			}
			else
			{
				umin = pNurbsSurface->ptransformation->umin_Reparameterization;
				umax = pNurbsSurface->ptransformation->umax_Reparameterization;
				vmin = pNurbsSurface->ptransformation->vmin_Reparameterization;
				vmax = pNurbsSurface->ptransformation->vmax_Reparameterization;
			}

			//double umin = pNurbsSurface->Converte_C_NurbsSurface()->GetUKnot(0);
			//double umax = pNurbsSurface->Converte_C_NurbsSurface()->GetUKnot(pNurbsSurface->Converte_C_NurbsSurface()->GetCtrlNumberU() + pNurbsSurface->Converte_C_NurbsSurface()->GetOrderU() - 1);

			//double vmin = pNurbsSurface->Converte_C_NurbsSurface()->GetVKnot(0);
			//double vmax = pNurbsSurface->Converte_C_NurbsSurface()->GetVKnot(pNurbsSurface->Converte_C_NurbsSurface()->GetCtrlNumberV() + pNurbsSurface->Converte_C_NurbsSurface()->GetOrderV() - 1);

			s1 = umin + (umax - umin) * s1;
			s2 = umin + (umax - umin) * s2;
			t1 = vmin + (vmax - vmin) * t1;
			t2 = vmin + (vmax - vmin) * t2;

			//double u1 = s1;
			double u1 = s1;
			double v1 = t1;

			//double u2 = s2;
			double u2 = s2;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1, v1) - ZeroP3D);
			//Vector3D v1_ = pNurbsSurface->ComputUDerivative1(u1,v1);
			//Vector3D v2_ = pNurbsSurface->ComputVDerivative1(u1,v1);
			//Vector3D n1 = v1_ ^ v2_;
			//n1.SetUnit();

			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2, v1) - ZeroP3D);
			//v1_ = pNurbsSurface->ComputUDerivative1(u2,v1);
			//v2_ = pNurbsSurface->ComputVDerivative1(u2,v1);
			//Vector3D n2 = v1_ ^ v2_;
			//n2.SetUnit();

			Vector3D vv3 = (pNurbsSurface->ComputPosition(u2, v2) - ZeroP3D);
			//v1_ = pNurbsSurface->ComputUDerivative1(u2,v2);
			//v2_ = pNurbsSurface->ComputVDerivative1(u2,v2);
			//Vector3D n3 = v1_ ^ v2_;
			//n3.SetUnit();

			Vector3D vv4 = (pNurbsSurface->ComputPosition(u1, v2) - ZeroP3D);
			//v1_ = pNurbsSurface->ComputUDerivative1(u1,v2);
			//v2_ = pNurbsSurface->ComputVDerivative1(u1,v2);
			//Vector3D n4 = v1_ ^ v2_;
			//n4.SetUnit();

			//n1 = n1 * (-1);
			//n2 = n2 * (-1);
			//n3 = n3 * (-1);
			//n4 = n4 * (-1);
			Vector3D n1 = (vv2 - vv1) ^ (vv3 - vv1);
			n1.SetUnit();
			Vector3D n2 = (vv3 - vv1) ^ (vv4 - vv1);
			n2.SetUnit();

			outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
			outfile << vv2[0] << "," << vv2[1] << "," << -vv2[2] << ">,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
			outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">}" << endl;

			outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n2[0] << "," << n2[1] << "," << -n2[2] << ">,<";
			outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n2[0] << "," << n2[1] << "," << -n2[2] << ">,<";
			outfile << vv4[0] << "," << vv4[1] << "," << -vv4[2] << ">,<" << n2[0] << "," << n2[1] << "," << -n2[2] << ">}" << endl;
		}
	}
	outfile << "texture {" << endl;
	outfile << "samplesmesh_tex" << endl;
	outfile << "}" << endl;
	outfile << "}" << endl;
	outfile.close();
}
void CFormView3::OutPutMeshFaces_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm, int samplesn, char* filename)
{
	std::ofstream outfile(filename);
	CString s1;
	int i, j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	//samplesm = samplesm * 10;
	outfile << "mesh {" << endl;
	for (j = 0; j < samplesn; j++)
	{
		for (i = 0; i < samplesm; i++)
		{	//
			double s1 = (double)i / samplesm;
			double s2 = (double)(i + 1) / samplesm;
			double t1 = (double)j / samplesn;
			double t2 = (double)(j + 1) / samplesn;

			double umin = pNurbsSurface->GetUKnot(0);
			double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

			double vmin = pNurbsSurface->GetVKnot(0);
			double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

			s1 = umin + (umax - umin) * s1;
			s2 = umin + (umax - umin) * s2;
			t1 = vmin + (vmax - vmin) * t1;
			t2 = vmin + (vmax - vmin) * t2;

			//double u1 = s1;
			double u1 = s1;
			double v1 = t1;

			//double u2 = s2;
			double u2 = s2;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1, v1) - ZeroP3D);
			Vector3D v1_ = pNurbsSurface->ComputUDerivative1(u1, v1);
			Vector3D v2_ = pNurbsSurface->ComputVDerivative1(u1, v1);
			Vector3D n1 = v1_ ^ v2_;
			n1.SetUnit();

			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2, v1) - ZeroP3D);
			v1_ = pNurbsSurface->ComputUDerivative1(u2, v1);
			v2_ = pNurbsSurface->ComputVDerivative1(u2, v1);
			Vector3D n2 = v1_ ^ v2_;
			n2.SetUnit();

			Vector3D vv3 = (pNurbsSurface->ComputPosition(u2, v2) - ZeroP3D);
			v1_ = pNurbsSurface->ComputUDerivative1(u2, v2);
			v2_ = pNurbsSurface->ComputVDerivative1(u2, v2);
			Vector3D n3 = v1_ ^ v2_;
			n3.SetUnit();

			Vector3D vv4 = (pNurbsSurface->ComputPosition(u1, v2) - ZeroP3D);
			v1_ = pNurbsSurface->ComputUDerivative1(u1, v2);
			v2_ = pNurbsSurface->ComputVDerivative1(u1, v2);
			Vector3D n4 = v1_ ^ v2_;
			n4.SetUnit();

			//n1 = n1 * (-1);
			//n2 = n2 * (-1);
			//n3 = n3 * (-1);
			//n4 = n4 * (-1);

			outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
			outfile << vv2[0] << "," << vv2[1] << "," << -vv2[2] << ">,<" << n2[0] << "," << n2[1] << "," << -n2[2] << ">,<";
			outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n3[0] << "," << n3[1] << "," << -n3[2] << ">}" << endl;

			outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
			outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n3[0] << "," << n3[1] << "," << -n3[2] << ">,<";
			outfile << vv4[0] << "," << vv4[1] << "," << -vv4[2] << ">,<" << n4[0] << "," << n4[1] << "," << -n4[2] << ">}" << endl;
		}
	}
	outfile << "texture {" << endl;
	outfile << "samplesmesh_tex" << endl;
	outfile << "}" << endl;
	outfile << "}" << endl;
	outfile.close();
}
void CFormView3::OnSurfaceNormalize()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();

		//Get the transform
		CVector3d* pVec = pNSurf->GetTransform()->GetScale();
		double scale = (*pVec)[0];

		CNurbsSuface* pNNewSurf = pSurf->GetCNurbsSurface();
		for (int i = 0; i < pNNewSurf->GetCtrlNumberU(); i++)
			for (int j = 0; j < pNNewSurf->GetCtrlNumberV(); j++)
			{
				Vector3D p = pNNewSurf->GetControlPoint(i, j);
				p = p * scale;
				pNNewSurf->SetControlPoint(i, j, p);
			}
		pNNewSurf->m_Transform.Copy(pNSurf->m_Transform);
		pDoc->m_SceneGraph.Add(pNNewSurf);
	}

	pDoc->UpdateTreeControl();
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}

	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnSurfaceSaveas()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pSur = pSurface->Converte_C_NurbsSurface();
		/*if(pSurface->ptransformation)
		{
			if(pSurface->ptransformation->m_ReparameterizationType == mobius)
			{
				MobiusTransformation* mobiustrans = (MobiusTransformation*)pSurface->ptransformation;
				pSur = pSur->GetMobiusTransformedSurface( mobiustrans->m_Mobiustransformation_Alpha,
					mobiustrans->m_Mobiustransformation_Beta);
				pSurface = pSur->GetCNurbsSurface();
			}
		}*/
		CFileDialog dlg(FALSE);
		if (dlg.DoModal())
		{
			m_FileTextureName = dlg.GetPathName();

			std::ofstream file(m_FileTextureName);

			pSurface->Save(file);

			if (pSurface->ptransformation)
			{
				pSurface->ptransformation->Save(file);
			}

			file.close();
		}
	}
	else
	{
		CFileDialog dlg(FALSE);
		if (dlg.DoModal())
		{
			CString m_FileName = dlg.GetPathName();
			char  filename[100];
			sprintf(filename, "%s", m_FileName);
			if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
				pMesh3D->SaveOBJFile(filename);
			}
			else
				if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
				{
					CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
					pMesh3D->ConvertMesh3d()->SaveOBJFile(filename);
				}
		}
	}
}

void CFormView3::OnBnClickedProjecttoparameterdomain()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CDC* pdc = pTreeView->GetDC();
	//ProjectToParameterDomain dlg;

	//SwapBuffers(pdc->GetSafeHdc());

	CImage m_Image;
	m_Image.Load("E:\\download\\colorbar.bmp");
	int width = m_Image.GetWidth();
	int height = m_Image.GetHeight();
	COLORREF color;
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			color = m_Image.GetPixel(i, j);
			//if(color != 0xffffff)
			//{
			//	TRACE("\n point: %d %d \n",i,j);
			//}
			//color = pdc->GetPixel(x,y);
			//color = image1.GetPixel(x,y);
			pdc->SetPixel(i, j, color);
		}
	return;
	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	//double x00 = 695;
	//double y00 = 347;
	//double x33 = 841.5;
	//double y33 = 788;
	 //double x00 = 89;
	 //double y00 = 203;
	 //double x33 = 1510;
	 //double y33 = 1024;
	double x00 = 798;
	double y00 = 1123.6;
	double x33 = 739.5;
	double y33 = 68.5;

	Vector3D p = pSurface->ComputPosition(0, 0) - ZeroP3D;
	double out[3];
	Vector3D proPoint;
	gluProject(p[0], p[1], p[2], pSurface->modelview, pSurface->projection, pSurface->m_viewPort, &(out[0]), &(out[1]), &(out[2]));

	double nx00 = out[0];
	double ny00 = proPoint.y = pSurface->m_viewPort[3] - out[1];

	p = pSurface->ComputPosition(1, 1) - ZeroP3D;
	gluProject(p[0], p[1], p[2], pSurface->modelview, pSurface->projection, pSurface->m_viewPort, &(out[0]), &(out[1]), &(out[2]));

	double nx33 = out[0];
	double ny33 = proPoint.y = pSurface->m_viewPort[3] - out[1];

	double xscale = (x33 - x00) / (nx33 - nx00);
	double yscale = (y33 - y00) / (ny33 - ny00);

	int resolution = 1500;
	int offset = 100;
	COLORREF red1 = 0x00ff0000;
	for (int i = 0; i <= resolution; i++)
		for (int j = 0; j <= resolution; j++)
		{
			double u = (i + 0.0) / resolution;
			double v = (j + 0.0) / resolution;

			//Have to modify this place for the face surface.
			pSurface->GetInterpolationModifiedUV(u, v);

			Vector3D p = pSurface->ComputPosition(u, v) - ZeroP3D;
			double out[3];
			Vector3D proPoint;
			gluProject(p[0], p[1], p[2], pSurface->modelview, pSurface->projection, pSurface->m_viewPort, &(out[0]), &(out[1]), &(out[2]));

			double x = out[0];
			double y = pSurface->m_viewPort[3] - out[1];
			//proPoint.z = out[2];
			color = m_Image.GetPixel((x - nx00) * xscale + x00 + 0.5, (y - ny00) * yscale + y00 + 0.5);
			//color = pdc->GetPixel(x,y);
			//color = image1.GetPixel(x,y);
			if ((x - nx00) * xscale + x00 < width && (y - ny00) * yscale + y00 < height)
				pdc->SetPixel((x - nx00) * xscale + x00 + 0.5, (y - ny00) * yscale + y00 + 0.5, red1);

			pdc->SetPixel(i + offset, resolution - j + offset, color);
			//COLORREF color = pdc->GetPixel(proPoint.x,proPoint.y);
			//pdc->SetPixel(proPoint.x,proPoint.y,color);
			//unsigned char pixel[3];
			//glReadPixels(proPoint.x, proPoint.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
			//dlg.m_colorarray[i][j] = pixel[0]*256*256+pixel[1]*256+pixel[2];
		}
}
void CFormView3::ReadViewPointFromFile(char* viewfile)
{
	ifstream file(viewfile);
	char message[80];

	double angle;
	char c;
	double eyex, eyey, eyez;
	double centerx, centery, centerz;
	double upx, upy, upz;
	while (!file.eof())
	{
		file >> message;
		if (strcmp(message, "location") == 0)
		{
			file >> c;
			while (c != '<')
				file >> c;
			file >> eyex;
			file >> c;
			while (c != ',')
				file >> c;
			file >> eyey;
			file >> c;
			while (c != ',')
				file >> c;
			file >> eyez;
		}
		else
			if (strcmp(message, "sky") == 0)
			{
				file >> c;
				while (c != '<')
					file >> c;
				file >> upx;
				file >> c;
				while (c != ',')
					file >> c;
				file >> upy;
				file >> c;
				while (c != ',')
					file >> c;
				file >> upz;
			}
			else
				if (strcmp(message, "look_at") == 0)
				{
					file >> c;
					while (c != '<')
						file >> c;
					file >> centerx;
					file >> c;
					while (c != ',')
						file >> c;
					file >> centery;
					file >> c;
					while (c != ',')
						file >> c;
					file >> centerz;
				}
	}
	file.close();
	eyez = -eyez;
	upz = -upz;
	centerz = -centerz;

	pTreeView->g_eye[0] = eyex;
	pTreeView->g_eye[1] = eyey;
	pTreeView->g_eye[2] = eyez;

	pTreeView->g_look[0] = centerx;
	pTreeView->g_look[1] = centery;
	pTreeView->g_look[2] = centerz;

	pTreeView->g_dir[0] = upx;
	pTreeView->g_dir[1] = upy;
	pTreeView->g_dir[2] = upz;

	m_SetCameraMode = 1;
}
void CFormView3::OnBnClickedReadviewpoint()
{
	// TODO: Add your control notification handler code here
	//Read View Point.
	CFileDialog dialog(TRUE);
	CString viewfile;
	if (dialog.DoModal() == IDOK)
	{
		viewfile = dialog.GetPathName();
		ReadViewPointFromFile((char*)LPCTSTR(viewfile));
	}
	pTreeView->Invalidate(TRUE);
}
void CFormView3::OutPutMeshTexture_PovRay(CMesh3d* pMesh, char* filename)
{

	//CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	//ASSERT_VALID(pDoc);
	//CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);

	std::ofstream outfile(filename);

	double minu = 1e10;
	double minv = 1e10;
	double maxu = -1e10;
	double maxv = -1e10;

	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->SetFlag(i);
	}

	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		if (minu > pVertex->u)
			minu = pVertex->u;
		if (minv > pVertex->v)
			minv = pVertex->v;
		if (maxu < pVertex->u)
			maxu = pVertex->u;
		if (maxv < pVertex->v)
			maxv = pVertex->v;
	}
	outfile << "vertex_vectors {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		outfile << "<" << x << "," << y << "," << -z << ">";
		if (i < pMesh->NbVertex() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;

	}
	outfile << "uv_vectors {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	double maxlength = maxu - minu;
	if (maxlength < maxv - minv)
		maxlength = maxv - minv;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);

		double u = pVertex->u;
		double v = pVertex->v;

		u = (u - minu) / maxlength;
		v = (v - minv) / maxlength;
		//u = u + 10 - ((int)(u + 10));
		//v = v + 10 - ((int)(v + 10));
		//if(u > 1.0)
		//	u = u - 1.0;
		//if(v > 1.0)
		//	v = v - 1.0;
		//u = u - minu;
		//v = v - minv;
		//u = u / 2;
		//v = v / 2;
		if (i < pMesh->NbVertex() - 1)
			outfile << " < " << u << " , " << v << ">, " << endl;
		else
			outfile << " < " << u << " , " << v << ">} " << endl;
	}

	outfile << "normal_vectors {" << endl;
	outfile << pMesh->NbVertex() << "," << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		CVector3d* pVector = pVertex->GetNormal();
		double x = pVector->x();
		double y = pVector->y();
		double z = pVector->z();
		outfile << "<" << x << "," << y << "," << -z << ">";
		if (i < pMesh->NbVertex() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
	}
	outfile << "face_indices {" << endl;
	outfile << pMesh->NbFace() << "," << endl;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;

		CVertex3d* pVertex1 = pMesh->GetVertex(index1);
		CVertex3d* pVertex2 = pMesh->GetVertex(index2);
		CVertex3d* pVertex3 = pMesh->GetVertex(index3);

		outfile << "<" << index1 << "," << index2 << "," << index3 << ">";

		if (i < pMesh->NbFace() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
	}
	outfile.close();
}
void CFormView3::OutPutTexture_PovRay(CNurbsSuface* pSurface, int samplesm, int samplesn, char* filename)
{
	C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();
	Transformation* pTrans = pSurface->ptransformation;
	//C_NurbsSurf* pNurbsSurface1 = ComputeQuadraticSuraceFromAnyDegreeSurface(pNurbsSurface,a1,a2,b1,b2);
	std::ofstream outfile(filename);
	char c1 = 13;
	char c2 = 10;
	//
	samplesm = 300;
	samplesn = 300;
	double minangle = 0;
	double maxangle = 30;
	int i, j;
	outfile << "vertex_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	std::vector<int> m_ArrayLabels;
	int ii = 0;
	int label;
	for (i = 0; i <= samplesm; i++)
		for (j = 0; j <= samplesn; j++)
		{
			double s = ((double)i) / samplesm;
			double t = ((double)j) / samplesn;

			label = 1;
			if (pSurface->ptransformation)
			{

				Transformation* pTrans = (Transformation*)pSurface->ptransformation;
				s = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s;
				t = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t;
				double u, v;
				//pTrans->Evaluation(s,t,u,v);
				double minu = pNurbsSurface1->GetUKnot(0);
				double maxu = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetDegreeU());

				double minv = pNurbsSurface1->GetVKnot(0);
				double maxv = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetDegreeV());

				pSurface->ptransformation->Evaluation(s, t, u, v);

				if (u > maxu)
					u = maxu;
				if (u < minu)
					u = minu;
				if (v > maxv)
					v = maxv;
				if (v < minv)
					v = minv;
				s = u;
				t = v;
				if (pTrans->m_ReparameterizationType == generalbilinear)
				{
					GeneralBilinearTransformation* pTran1 = (GeneralBilinearTransformation*)pTrans;
					/*if(u < pTran1->umin)
						label = 0;
					if(v < pTran1->vmin)
						label = 0;
					if(u > pTran1->umax)
						label = 0;
					if(v > pTran1->vmax)
						label = 0;
					if(u < pTran1->umin)
						label = 0;
					if(v < pTran1->vmin)
						label = 0;
					if(u > pTran1->umax)
						label = 0;
					if(v > pTran1->vmax)
						label = 0;*/
				}

				//s = u;
				//t = v;

			}
			else
			{
				double umin = pNurbsSurface1->GetUKnot(0);
				double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

				double vmin = pNurbsSurface1->GetVKnot(0);
				double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

				s = umin + (umax - umin) * s;
				t = vmin + (vmax - vmin) * t;
			}

			double u, v;
			u = s;
			v = t;

			//if(u > 1)
			//	u = 1;
			//if(v > 1)
			//	v = 1;

			m_ArrayLabels.push_back(label);
			if (label)
			{
				Point3D p1 = pNurbsSurface1->ComputPosition(u, v);
				outfile << "<" << p1[0] << " , " << p1[1] << " , " << -p1[2] << " >, " << endl;
			}
			else
			{
				outfile << "<" << 0 << " , " << 0 << " , " << 0 << " >, " << endl;
			}

		}
	outfile << " }" << endl;

	ii = 0;
	outfile << "uv_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	double maxangle_real = -1e10;
	for (i = 0; i <= samplesm; i++)
		for (j = 0; j <= samplesn; j++)
		{
			double u = ((double)i) / samplesm;
			double v = ((double)j) / samplesn;


			if (pSurface->ptransformation)
			{
				double length = pTrans->umax_Reparameterization - pTrans->umin_Reparameterization;
				if (length < pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization)
					length = pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization;

				u = (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * u / length;
				v = (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * v / length;
			}
			else
			{
				double umin = pNurbsSurface1->GetUKnot(0);
				double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

				double vmin = pNurbsSurface1->GetVKnot(0);
				double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

				double length = umax - umin;
				if (length < vmax - vmin)
					length = vmax - vmin;

				u = (umax - umin) * u / length;
				v = (vmax - vmin) * v / length;
			}
			//if(u > 1)
			//	u = 1;
			//if(v > 1)
			//	v = 1;
			if (m_ArrayLabels[ii])
			{
				//outfile << " < " << u * 8.0  / (9 * 2)  << " , " << v * 6.0 / (9 * 2) << ">, " <<endl;
				outfile << " < " << u << " , " << v << ">, " << endl;
			}
			else
			{
				outfile << " < " << 0 << " , " << 0 << ">, " << endl;
			}
			ii++;

		}
	outfile << " }" << endl;

	ii = 0;
	//normal
	outfile << "normal_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	for (i = 0; i <= samplesm; i++)
		for (j = 0; j <= samplesn; j++)
		{
			if (m_ArrayLabels[ii])
			{
				double s = ((double)i) / samplesm;
				double t = ((double)j) / samplesn;
				double u, v;

				if (pSurface->ptransformation)
				{

					Transformation* pTrans = (Transformation*)pSurface->ptransformation;
					s = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s;
					t = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t;
					double u, v;
					pTrans->Evaluation(s, t, u, v);


					double minu = pNurbsSurface1->GetUKnot(0);
					double maxu = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetDegreeU());

					double minv = pNurbsSurface1->GetVKnot(0);
					double maxv = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetDegreeV());

					//pSurface->ptransformation->Evaluation(s, t, u, v);

					if (u > maxu)
						u = maxu;
					if (u < minu)
						u = minu;
					if (v > maxv)
						v = maxv;
					if (v < minv)
						v = minv;

					s = u;
					t = v;

				}
				else
				{
					double umin = pNurbsSurface1->GetUKnot(0);
					double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

					double vmin = pNurbsSurface1->GetVKnot(0);
					double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

					s = umin + (umax - umin) * s;
					t = vmin + (vmax - vmin) * t;
				}

				u = s;
				v = t;

				/*if(u > 1)
					u = 1;
				if(v > 1)
					v = 1;
				if(u < 0)
					u = 0;
				if(v < 0)
					v = 0;*/
				Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u, v);
				Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u, v);
				v1.SetUnit();
				v2.SetUnit();

				Vector3D n = v1 ^ v2;
				outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >," << endl;
			}
			else
			{
				outfile << " < " << 0 << " , " << 0 << " , " << 0 << " >," << endl;
			}
			ii++;

		}
	outfile << " }" << endl;

	ii = 0;
	//face_indices {
	for (j = 0; j < samplesn; j++)
		for (i = 0; i < samplesm; i++)
		{
			double u = ((double)i) / samplesm;
			double v = ((double)j) / samplesn;

			int n1 = j * (samplesm + 1) + i;
			int n2 = j * (samplesm + 1) + i + 1;
			int n3 = (j + 1) * (samplesm + 1) + i;
			int n4 = (j + 1) * (samplesm + 1) + i + 1;

			if (m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
			{
				ii++;
			}
			if (m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
			{
				ii++;
			}
		}

	outfile << "face_indices {" << endl;
	outfile << ii << endl;
	for (j = 0; j < samplesn; j++)
		for (i = 0; i < samplesm; i++)
		{
			double u = ((double)i) / samplesm;
			double v = ((double)j) / samplesn;

			int n1 = j * (samplesm + 1) + i;
			int n2 = j * (samplesm + 1) + i + 1;
			int n3 = (j + 1) * (samplesm + 1) + i;
			int n4 = (j + 1) * (samplesm + 1) + i + 1;

			if (m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
			{
				outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >," << endl;
				ii++;
			}
			if (m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
			{
				outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >," << endl;
				ii++;
			}
		}
	outfile << " }" << endl;

	outfile.close();
}

void CFormView3::OutPutTexture_Optimization_Moddified_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm, int samplesn, char* filename)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();

		//double A = Compute_Area(pNurbsSurface1,1,1);
		std::ofstream outfile(filename);

		double minangle = 0;
		double maxangle = 50;
		int i, j;
		outfile << "vertex_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D p1 = pSurface->pUInterOptiModifyCurve->ComputPosition(u);
				Point3D p2 = pSurface->pVInterOptiModifyCurve->ComputPosition(v);
				u = p1[0];
				v = p2[0];

				Point3D pp1 = pNurbsSurface1->ComputPosition(u, v);
				outfile << "<" << pp1[0] << " , " << pp1[1] << " , " << -pp1[2] << " >, " << endl;
			}
		outfile << " }" << endl;


		outfile << "uv_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		double maxangle_real = -1e10;
		double uscale, vscale;
		double energy[101][101], energy1[101][101];
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				//Point3D p1 = pSurface->pUInterOptiModifyCurve->ComputPosition(u);
				//Point3D p2 = pSurface->pVInterOptiModifyCurve->ComputPosition(v);

				//double u_ori = p1[0];
				//double v_ori = p2[0];

				outfile << " < " << u << " , " << v << ">, " << endl;
			}
		outfile << " }" << endl;


		//normal
		outfile << "normal_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D pp1 = pSurface->pUInterOptiModifyCurve->ComputPosition(u);
				Point3D pp2 = pSurface->pVInterOptiModifyCurve->ComputPosition(v);

				u = pp1[0];
				v = pp2[0];
				Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u, v);
				Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u, v);
				v1.SetUnit();
				v2.SetUnit();

				Vector3D n = v1 ^ v2;
				outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >," << endl;
			}
		outfile << " }" << endl;

		//face_indices {
		outfile << "face_indices {" << endl;
		outfile << (samplesm) * (samplesn) * 2 << endl;
		for (j = 0; j < samplesn; j++)
			for (i = 0; i < samplesm; i++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				int n1 = j * (samplesm + 1) + i;
				int n2 = j * (samplesm + 1) + i + 1;
				int n3 = (j + 1) * (samplesm + 1) + i;
				int n4 = (j + 1) * (samplesm + 1) + i + 1;

				outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >," << endl;
				outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >," << endl;
			}
		outfile << " }" << endl;

		outfile.close();
	}
}
void CFormView3::OutPutTexture_Optimization_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm, int samplesn, char* filename)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();

		//double A = Compute_Area(pNurbsSurface1,1,1);
		std::ofstream outfile(filename);

		double minangle = 0;
		double maxangle = 50;
		int i, j;
		outfile << "vertex_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D p1 = pSurface->pUInterOptiCurve->ComputPosition(u);
				Point3D p2 = pSurface->pVInterOptiCurve->ComputPosition(v);
				u = p1[0];
				v = p2[0];

				Point3D pp1 = pNurbsSurface1->ComputPosition(u, v);
				outfile << "<" << pp1[0] << " , " << pp1[1] << " , " << -pp1[2] << " >, " << endl;
			}
		outfile << " }" << endl;


		outfile << "uv_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		double maxangle_real = -1e10;
		double uscale, vscale;
		double energy[101][101], energy1[101][101];
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				//Point3D p1 = pSurface->pUInterOptiCurve->ComputPosition(u);
				//Point3D p2 = pSurface->pVInterOptiCurve->ComputPosition(v);

				//double u_ori = p1[0];
				//double v_ori = p2[0];

				outfile << " < " << u << " , " << v << ">, " << endl;
			}
		outfile << " }" << endl;


		//normal
		outfile << "normal_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D pp1 = pSurface->pUInterOptiCurve->ComputPosition(u);
				Point3D pp2 = pSurface->pVInterOptiCurve->ComputPosition(v);

				u = pp1[0];
				v = pp2[0];
				Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u, v);
				Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u, v);
				v1.SetUnit();
				v2.SetUnit();

				Vector3D n = v1 ^ v2;
				outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >," << endl;
			}
		outfile << " }" << endl;

		//face_indices {
		outfile << "face_indices {" << endl;
		outfile << (samplesm) * (samplesn) * 2 << endl;
		for (j = 0; j < samplesn; j++)
			for (i = 0; i < samplesm; i++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				int n1 = j * (samplesm + 1) + i;
				int n2 = j * (samplesm + 1) + i + 1;
				int n3 = (j + 1) * (samplesm + 1) + i;
				int n4 = (j + 1) * (samplesm + 1) + i + 1;

				outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >," << endl;
				outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >," << endl;
			}
		outfile << " }" << endl;

		outfile.close();
	}
}
void CFormView3::OutPutAreaFactor_Saved_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm, int samplesn, char* filename)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();

		double A = Compute_Area(pNurbsSurface1, 1, 1);
		std::ofstream outfile(filename);

		double minangle = 0;
		double maxangle = 50;
		int i, j;
		outfile << "vertex_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D p1 = pSurface->pUInterOptiCurve->ComputPosition(u);
				Point3D p2 = pSurface->pVInterOptiCurve->ComputPosition(v);
				u = p1[0];
				v = p2[0];

				Point3D pp1 = pNurbsSurface1->ComputPosition(u, v);
				outfile << "<" << pp1[0] << " , " << pp1[1] << " , " << -pp1[2] << " >, " << endl;
			}
		outfile << " }" << endl;


		outfile << "uv_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		double maxangle_real = -1e10;
		double uscale, vscale;
		double energy[101][101], energy1[101][101];
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D p1 = pSurface->pUInterOptiCurve->ComputPosition(u);
				Point3D p2 = pSurface->pVInterOptiCurve->ComputPosition(v);

				double u_ori = p1[0];
				double v_ori = p2[0];

				Vector3D vv1 = pSurface->pUInterOptiCurve->ComputDerivative1(u);
				Vector3D vv2 = pSurface->pVInterOptiCurve->ComputDerivative1(v);
				Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u_ori, v_ori) * vv1[0];
				Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u_ori, v_ori) * vv2[0];

				Vector3D v3 = v1 ^ v2;
				double val = fabs(Length(v3) - A);
				//val = val / (3.8140685402725407 * 0.8);
				//val = val / (5.4781895116862112 * 0.8);
				//val = val / (6.1330763503449450);
				val = val / (0.024885715408693884 * 6);
				if (val >= 1)
					val = 0.99;
				if (val > maxangle_real)
					maxangle_real = val;
				energy[i][j] = val;
			}
		/*				for(int ii = 0; ii <= 2; ii++)
						{
							for(i = 0; i <= samplesm; i++)
								for(j = 0; j <= samplesn; j++)
								{
									int num = 0;
									double sum = 0;
									if(i - 1 >= 0 && j - 1 >= 0)
									{
										num++;
										sum += energy[i][j];
									}
									if( j - 1 >= 0)
									{
										num++;
										sum += energy[i][j - 1];
									}
									if( j - 1 >= 0 && i + 1 <= samplesm)
									{
										num++;
										sum += energy[i + 1][j - 1];
									}
									if( i - 1 >= 0 )
									{
										num++;
										sum += energy[i - 1][j];
									}

									num++;
									sum += energy[i][j];

									if( i + 1 <= samplesm )
									{
										num++;
										sum += energy[i + 1][j];
									}
									if( i - 1 >= 0 && j + 1 <= samplesn)
									{
										num++;
										sum += energy[i - 1][j + 1];
									}
									if( j + 1 <= samplesn)
									{
										num++;
										sum += energy[i][j + 1];
									}
									if( i + 1 <= samplesm && j + 1 <= samplesn)
									{
										num++;
										sum += energy[i + 1][j + 1];
									}
									energy1[i][j] = sum / num;
								}
								for(i = 0; i <= samplesm; i++)
									for(j = 0; j <= samplesn; j++)
									{
										energy[i][j] = energy1[i][j];
									}
						}*/


		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double val = energy[i][j];
				outfile << " < 0 " << " , " << val << ">, " << endl;
			}
		outfile << " }" << endl;

		//normal
		outfile << "normal_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D pp1 = pSurface->pUInterOptiCurve->ComputPosition(u);
				Point3D pp2 = pSurface->pVInterOptiCurve->ComputPosition(v);

				u = pp1[0];
				v = pp2[0];
				Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u, v);
				Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u, v);
				v1.SetUnit();
				v2.SetUnit();

				Vector3D n = v1 ^ v2;
				outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >," << endl;
			}
		outfile << " }" << endl;

		//face_indices {
		outfile << "face_indices {" << endl;
		outfile << (samplesm) * (samplesn) * 2 << endl;
		for (j = 0; j < samplesn; j++)
			for (i = 0; i < samplesm; i++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				int n1 = j * (samplesm + 1) + i;
				int n2 = j * (samplesm + 1) + i + 1;
				int n3 = (j + 1) * (samplesm + 1) + i;
				int n4 = (j + 1) * (samplesm + 1) + i + 1;

				outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >," << endl;
				outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >," << endl;
			}
		outfile << " }" << endl;

		outfile.close();
	}
}
void CFormView3::OutPutAreaFactor_Savedmodified_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm, int samplesn, char* filename)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();

		double A = Compute_Area(pNurbsSurface1, 1, 1);
		std::ofstream outfile(filename);

		double minangle = 0;
		double maxangle = 50;
		int i, j;
		outfile << "vertex_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D pp1 = pSurface->pUInterOptiModifyCurve->ComputPosition(u);
				Point3D pp2 = pSurface->pVInterOptiModifyCurve->ComputPosition(v);
				u = pp1[0];
				v = pp2[0];

				Point3D p1 = pNurbsSurface1->ComputPosition(u, v);
				outfile << "<" << p1[0] << " , " << p1[1] << " , " << -p1[2] << " >, " << endl;
			}
		outfile << " }" << endl;


		outfile << "uv_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		double maxangle_real = -1e10;
		double uscale, vscale;
		double energy[101][101], energy1[101][101];
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D pp1 = pSurface->pUInterOptiModifyCurve->ComputPosition(u);
				Point3D pp2 = pSurface->pVInterOptiModifyCurve->ComputPosition(v);

				double u_1 = pp1[0];
				double v_1 = pp2[0];

				Vector3D vv1 = pSurface->pUInterOptiModifyCurve->ComputDerivative1(u);
				Vector3D vv2 = pSurface->pVInterOptiModifyCurve->ComputDerivative1(v);
				Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u_1, v_1) * vv1[0];
				Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u_1, v_1) * vv2[0];

				Vector3D v3 = v1 ^ v2;
				double val = fabs(Length(v3) - A);
				//val = val / (3.8140685402725407 * 0.8);
				//val = val / (5.4781895116862112 * 0.8);
				//val = val / (6.1330763503449450);
				val = val / (0.024885715408693884 * 6);
				if (val >= 1)
					val = 0.99;
				if (val > maxangle_real)
					maxangle_real = val;
				energy[i][j] = val;
			}
		/*for(int ii = 0; ii <= 2; ii++)
		{
			for(i = 0; i <= samplesm; i++)
				for(j = 0; j <= samplesn; j++)
				{
					int num = 0;
					double sum = 0;
					if(i - 1 >= 0 && j - 1 >= 0)
					{
						num++;
						sum += energy[i][j];
					}
					if( j - 1 >= 0)
					{
						num++;
						sum += energy[i][j - 1];
					}
					if( j - 1 >= 0 && i + 1 <= samplesm)
					{
						num++;
						sum += energy[i + 1][j - 1];
					}
					if( i - 1 >= 0 )
					{
						num++;
						sum += energy[i - 1][j];
					}

					num++;
					sum += energy[i][j];

					if( i + 1 <= samplesm )
					{
						num++;
						sum += energy[i + 1][j];
					}
					if( i - 1 >= 0 && j + 1 <= samplesn)
					{
						num++;
						sum += energy[i - 1][j + 1];
					}
					if( j + 1 <= samplesn)
					{
						num++;
						sum += energy[i][j + 1];
					}
					if( i + 1 <= samplesm && j + 1 <= samplesn)
					{
						num++;
						sum += energy[i + 1][j + 1];
					}
					energy1[i][j] = sum / num;
				}
				for(i = 0; i <= samplesm; i++)
					for(j = 0; j <= samplesn; j++)
					{
						energy[i][j] = energy1[i][j];
					}
		}*/


		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double val = energy[i][j];
				outfile << " < 0 " << " , " << val << ">, " << endl;
			}
		outfile << " }" << endl;

		//normal
		outfile << "normal_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Point3D pp1 = pSurface->pUInterOptiModifyCurve->ComputPosition(u);
				Point3D pp2 = pSurface->pVInterOptiModifyCurve->ComputPosition(v);

				u = pp1[0];
				v = pp2[0];

				Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u, v);
				Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u, v);
				v1.SetUnit();
				v2.SetUnit();

				Vector3D n = v1 ^ v2;
				outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >," << endl;
			}
		outfile << " }" << endl;

		//face_indices {
		outfile << "face_indices {" << endl;
		outfile << (samplesm) * (samplesn) * 2 << endl;
		for (j = 0; j < samplesn; j++)
			for (i = 0; i < samplesm; i++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				int n1 = j * (samplesm + 1) + i;
				int n2 = j * (samplesm + 1) + i + 1;
				int n3 = (j + 1) * (samplesm + 1) + i;
				int n4 = (j + 1) * (samplesm + 1) + i + 1;

				outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >," << endl;
				outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >," << endl;
			}
		outfile << " }" << endl;

		outfile.close();
	}
}
void CFormView3::OutPutMu_PovRay(CMesh3d* pMesh, char* filename)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CMesh3d* pMesh_ = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);

	std::ofstream outfile(filename);
	//outfile << "mesh2 {" << endl;
	outfile << "vertex_vectors {" << endl;
	outfile << pMesh->NbVertex() << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->m_Flag = i;
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		outfile << "<" << x << " , " << y << " , " << -z << " >, " << endl;
	}
	outfile << " }" << endl;

	double maxmu = -1e10;
	outfile << "uv_vectors {" << endl;
	outfile << pMesh->NbVertex() << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh_->GetVertex(i);
		double mu = sqrt(pVertex->mux * pVertex->mux + pVertex->muy * pVertex->muy);
		//if(mu > maxmu)
		//	maxmu = mu;
		//mu = mu / 0.64771474646320204;
		if (mu > 1)
			mu = 1;
		outfile << " < 0 " << " , " << mu << ">, " << endl;
	}
	outfile << " }" << endl;

	outfile << "normal_vectors {" << endl;
	outfile << pMesh->NbVertex() << endl;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->GetNormal()->x();
		double y = pVertex->GetNormal()->y();
		double z = pVertex->GetNormal()->z();

		outfile << " < " << x << " , " << y << " , " << -z << " >," << endl;
	}
	outfile << " }" << endl;

	outfile << "face_indices {" << endl;
	outfile << pMesh->NbFace() << "," << endl;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		int index1 = pFace->v1()->m_Flag;
		int index2 = pFace->v2()->m_Flag;
		int index3 = pFace->v3()->m_Flag;

		outfile << "<" << index1 << "," << index2 << "," << index3 << ">";

		if (i < pMesh->NbFace() - 1)
			outfile << "," << endl;
		else
			outfile << "}" << endl;
	}
	//outfile << "}" << endl;
	outfile.close();
}
void CFormView3::OutPutConformity_PovRay(int samplesm, int samplesn, char* filename)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	std::ofstream outfile(filename);
	int i, j;

	CNurbsSuface* pNurbsSurface;
	double s1, s2, t1, t2;
	double u, v, s, t;
	std::vector<int> m_ArrayLabels;
	int label;
	outfile << "vertex_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) * num << endl;
	for (int ii = 0; ii < num; ii++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii);
			C_NurbsSurf* pNurbsSurface1 = pNurbsSurface->Converte_C_NurbsSurface();
			Transformation* pTrans = pNurbsSurface->ptransformation;

			for (i = 0; i <= samplesm; i++)
				for (j = 0; j <= samplesn; j++)
				{
					s = ((double)i) / samplesm;
					t = ((double)j) / samplesn;

					label = 1;
					{
						double umin = pNurbsSurface1->GetUKnot(0);
						double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

						double vmin = pNurbsSurface1->GetVKnot(0);
						double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

						s = umin + (umax - umin) * s;
						t = vmin + (vmax - vmin) * t;
					}

					double u, v;
					u = s;
					v = t;

					m_ArrayLabels.push_back(label);
					if (label)
					{
						Point3D p1 = pNurbsSurface1->ComputPosition(u, v);
						outfile << "<" << p1[0] << " , " << p1[1] << " , " << -p1[2] << " >, " << endl;
					}
					else
					{
						outfile << "<" << 0 << " , " << 0 << " , " << 0 << " >, " << endl;
					}
				}
		}
	}
	outfile << " }" << endl;

	int ii = -1;
	outfile << "uv_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) * num << endl;
	double maxangle_real = -1e10;
	for (int ii1 = 0; ii1 < num; ii1++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii1)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii1);
			C_NurbsSurf* pNurbsSurface1 = pNurbsSurface->Converte_C_NurbsSurface();
			Transformation* pTrans = pNurbsSurface->ptransformation;

			for (i = 0; i <= samplesm; i++)
				for (j = 0; j <= samplesn; j++)
				{
					double s = ((double)i) / samplesm;
					double t = ((double)j) / samplesn;

					ii++;
					if (!m_ArrayLabels[ii])
					{
						outfile << " < 0 " << " , " << 0 << ">, " << endl;
					}
					else
					{
						if (pNurbsSurface->ptransformation)
						{
							Transformation* pTrans = (Transformation*)pNurbsSurface->ptransformation;
							double umin = pNurbsSurface1->GetUKnot(0);
							double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

							double vmin = pNurbsSurface1->GetVKnot(0);
							double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

							u = umin + (umax - umin) * s;
							v = vmin + (vmax - vmin) * t;

							pTrans->InverseEvaluation(u, v, s, t);

						}
						else
						{
							double umin = pNurbsSurface1->GetUKnot(0);
							double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

							double vmin = pNurbsSurface1->GetVKnot(0);
							double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

							s = umin + (umax - umin) * s;
							t = vmin + (vmax - vmin) * t;


							u = s;
							v = t;
						}


						Vector3D Xu = pNurbsSurface1->ComputUDerivative1(u, v);
						Vector3D Xv = pNurbsSurface1->ComputVDerivative1(u, v);
						Vector3D N = Xu ^ Xv;
						N.SetUnit();

						double val;
						if (pNurbsSurface->ptransformation)
						{
							double us = pTrans->Comput_u_s(s, t);
							double vs = pTrans->Comput_v_s(s, t);
							double ut = pTrans->Comput_u_t(s, t);
							double vt = pTrans->Comput_v_t(s, t);

							Vector3D Xs = Xu * us + Xv * vs;
							Vector3D Xt = Xu * ut + Xv * vt;

							Vector3D newvec = N ^ Xs;
							val = (newvec - Xt) * (newvec - Xt);
						}
						else
						{
							Vector3D newvec = N ^ Xu;
							val = (newvec - Xv) * (newvec - Xv);
						}

						//newvec.SetUnit();
						//Xt.SetUnit();
						//double cosval = newvec * Xt;
						//if(cosval < 0 )
						//{
						//	int ij = 0;
						//}
						//cartop 0.17209753561068819 * 2
						//198249.64033109313 * 1.2
						//198249.64033109313 * 0.1
						//bunny maximal value 0.0015512831460528663
						val = val / (0.0015512831460528663 * 4);
						if (val >= 0.05)
							val = 0.05;
						if (val > maxangle_real)
							maxangle_real = val;

						outfile << " < 0 " << " , " << val << ">, " << endl;
					}
				}
		}
	}
	outfile << " }" << endl;

	//normal
	ii = -1;
	outfile << "normal_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) * num << endl;
	for (int ii1 = 0; ii1 < num; ii1++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii1)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii1);
			C_NurbsSurf* pNurbsSurface1 = pNurbsSurface->Converte_C_NurbsSurface();
			Transformation* pTrans = pNurbsSurface->ptransformation;
			for (i = 0; i <= samplesm; i++)
				for (j = 0; j <= samplesn; j++)
				{
					double s = ((double)i) / samplesm;
					double t = ((double)j) / samplesn;

					ii++;
					if (!m_ArrayLabels[ii])
					{
						outfile << " < " << 0 << " , " << 0 << " , " << 0 << " >," << endl;
					}
					else
					{
						if (pNurbsSurface->ptransformation)
						{

							Transformation* pTrans = (Transformation*)pNurbsSurface->ptransformation;
							s = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s;
							t = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t;
							double u, v;
							pTrans->Evaluation(s, t, u, v);

							if (pTrans->m_ReparameterizationType == generalbilinear)
							{
								GeneralBilinearTransformation* pTran1 = (GeneralBilinearTransformation*)pTrans;
								if (u < pTran1->umin)
									label = 0;
								if (v < pTran1->vmin)
									label = 0;
								if (u > pTran1->umax)
									label = 0;
								if (v > pTran1->vmax)
									label = 0;
								if (u < pTran1->umin)
									label = 0;
								if (v < pTran1->vmin)
									label = 0;
								if (u > pTran1->umax)
									label = 0;
								if (v > pTran1->vmax)
									label = 0;
							}

							s = u;
							t = v;

						}
						else
						{
							double umin = pNurbsSurface1->GetUKnot(0);
							double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

							double vmin = pNurbsSurface1->GetVKnot(0);
							double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

							s = umin + (umax - umin) * s;
							t = vmin + (vmax - vmin) * t;
							u = s;
							v = t;
						}


						Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u, v);
						Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u, v);


						Vector3D Xu = pNurbsSurface1->ComputUDerivative1(u, v);
						Vector3D Xv = pNurbsSurface1->ComputVDerivative1(u, v);
						Vector3D n = Xu ^ Xv;
						n.SetUnit();

						outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >," << endl;
					}

				}
		}
	}
	outfile << " }" << endl;

	ii = 0;
	//face_indices {
	for (int ii1 = 0; ii1 < num; ii1++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii1)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii1);
			C_NurbsSurf* pNurbsSurface1 = pNurbsSurface->Converte_C_NurbsSurface();
			Transformation* pTrans = pNurbsSurface->ptransformation;

			for (j = 0; j < samplesn; j++)
				for (i = 0; i < samplesm; i++)
				{
					double u = ((double)i) / samplesm;
					double v = ((double)j) / samplesn;

					int n1 = j * (samplesm + 1) + i + (samplesm + 1) * (samplesn + 1) * ii1;
					int n2 = j * (samplesm + 1) + i + 1 + (samplesm + 1) * (samplesn + 1) * ii1;
					int n3 = (j + 1) * (samplesm + 1) + i + (samplesm + 1) * (samplesn + 1) * ii1;
					int n4 = (j + 1) * (samplesm + 1) + i + 1 + (samplesm + 1) * (samplesn + 1) * ii1;

					if (m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
					{
						ii++;
					}
					if (m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
					{
						ii++;
					}
				}
		}
	}

	//face_indices {
	outfile << "face_indices {" << endl;
	outfile << ii << endl;
	for (int ii1 = 0; ii1 < num; ii1++)
	{
		if (pDoc->m_SceneGraph.GetAt(ii1)->GetType() == TYPE_NURBSSURFACE)
		{
			pNurbsSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(ii1);
			C_NurbsSurf* pNurbsSurface1 = pNurbsSurface->Converte_C_NurbsSurface();
			Transformation* pTrans = pNurbsSurface->ptransformation;

			for (j = 0; j < samplesn; j++)
				for (i = 0; i < samplesm; i++)
				{
					double u = ((double)i) / samplesm;
					double v = ((double)j) / samplesn;

					int n1 = j * (samplesm + 1) + i + (samplesm + 1) * (samplesn + 1) * ii1;
					int n2 = j * (samplesm + 1) + i + 1 + (samplesm + 1) * (samplesn + 1) * ii1;
					int n3 = (j + 1) * (samplesm + 1) + i + (samplesm + 1) * (samplesn + 1) * ii1;
					int n4 = (j + 1) * (samplesm + 1) + i + 1 + (samplesm + 1) * (samplesn + 1) * ii1;

					if (m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
					{
						outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >," << endl;
					}
					if (m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
					{
						outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >," << endl;
					}
				}
		}
	}
	outfile << " }" << endl;
	outfile.close();
}
void CFormView3::OutPutConformity_PovRay(CNurbsSuface* pSurface, int samplesm, int samplesn, char* filename)
{
	C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();
	Transformation* pTrans = pSurface->ptransformation;
	std::ofstream outfile(filename);

	int i, j;
	double u, v, s, t;
	std::vector<int> m_ArrayLabels;
	int label;
	outfile << "vertex_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	int vindex = 1;
	for (i = 0; i <= samplesm; i++)
		for (j = 0; j <= samplesn; j++)
		{
			s = ((double)i) / samplesm;
			t = ((double)j) / samplesn;

			if (vindex == 40600)
			{
				int ij = 0;
			}
			label = 1;
			if (pSurface->ptransformation)
			{

				Transformation* pTrans = (Transformation*)pSurface->ptransformation;
				s = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s;
				t = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t;
				double u, v;
				pTrans->Evaluation(s, t, u, v);

				if (pTrans->m_ReparameterizationType == generalbilinear)
				{
					GeneralBilinearTransformation* pTran1 = (GeneralBilinearTransformation*)pTrans;
					if (u < pTran1->umin)
						label = 0;
					if (v < pTran1->vmin)
						label = 0;
					if (u > pTran1->umax)
						label = 0;
					if (v > pTran1->vmax)
						label = 0;
					if (u < pTran1->umin)
						label = 0;
					if (v < pTran1->vmin)
						label = 0;
					if (u > pTran1->umax)
						label = 0;
					if (v > pTran1->vmax)
						label = 0;
				}

				s = u;
				t = v;

			}
			else
			{
				double umin = pNurbsSurface1->GetUKnot(0);
				double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

				double vmin = pNurbsSurface1->GetVKnot(0);
				double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

				s = umin + (umax - umin) * s;
				t = vmin + (vmax - vmin) * t;
			}

			double u, v;
			u = s;
			v = t;

			//if(u > 1)
			//	u = 1;
			//if(v > 1)
			//	v = 1;
			//if(u < 0)
			//	u = 0;
			//if(v < 0)
			//	v = 0;

			m_ArrayLabels.push_back(label);
			if (label)
			{
				Point3D p1 = pNurbsSurface1->ComputPosition(u, v);
				outfile << "<" << p1[0] << " , " << p1[1] << " , " << -p1[2] << " >, " << endl;
			}
			else
			{
				outfile << "<" << 0 << " , " << 0 << " , " << 0 << " >, " << endl;
			}
			vindex++;
		}
	outfile << " }" << endl;

	int ii = -1;
	outfile << "uv_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	double maxangle_real = -1e10;
	for (i = 0; i <= samplesm; i++)
		for (j = 0; j <= samplesn; j++)
		{
			double s = ((double)i) / samplesm;
			double t = ((double)j) / samplesn;

			ii++;
			if (!m_ArrayLabels[ii])
			{
				outfile << " < 0 " << " , " << 0 << ">, " << endl;
			}
			else
			{
				if (pSurface->ptransformation)
				{

					Transformation* pTrans = (Transformation*)pSurface->ptransformation;
					s = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s;
					t = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t;
					pTrans->Evaluation(s, t, u, v);

					if (pTrans->m_ReparameterizationType == generalbilinear)
					{
						GeneralBilinearTransformation* pTran1 = (GeneralBilinearTransformation*)pTrans;
						if (u < pTran1->umin)
							label = 0;
						if (v < pTran1->vmin)
							label = 0;
						if (u > pTran1->umax)
							label = 0;
						if (v > pTran1->vmax)
							label = 0;
						if (u < pTran1->umin)
							label = 0;
						if (v < pTran1->vmin)
							label = 0;
						if (u > pTran1->umax)
							label = 0;
						if (v > pTran1->vmax)
							label = 0;
					}

					//s = u;
					//t = v;

				}
				else
				{
					double umin = pNurbsSurface1->GetUKnot(0);
					double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

					double vmin = pNurbsSurface1->GetVKnot(0);
					double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

					s = umin + (umax - umin) * s;
					t = vmin + (vmax - vmin) * t;
					u = s;
					v = t;
				}

				//if(u > 1)
				//	u = 1;
				//if(v > 1)
				//	v = 1;
				//if(u < 0)
				//	u = 0;
				//if(v < 0)
				//	v = 0;
				Vector3D Xu = pNurbsSurface1->ComputUDerivative1(u, v);
				Vector3D Xv = pNurbsSurface1->ComputVDerivative1(u, v);
				Vector3D N = Xu ^ Xv;
				N.SetUnit();

				double val;
				if (pSurface->ptransformation)
				{
					double us = pTrans->Comput_u_s(s, t);
					double vs = pTrans->Comput_v_s(s, t);
					double ut = pTrans->Comput_u_t(s, t);
					double vt = pTrans->Comput_v_t(s, t);

					Vector3D Xs = Xu * us + Xv * vs;
					Vector3D Xt = Xu * ut + Xv * vt;

					Vector3D newvec = N ^ Xs;
					val = (newvec - Xt) * (newvec - Xt);
				}
				else
				{
					Vector3D newvec = N ^ Xu;
					val = (newvec - Xv) * (newvec - Xv);
				}

				//newvec.SetUnit();
				//Xt.SetUnit();
				//double cosval = newvec * Xt;
				//if(cosval < 0 )
				//{
				//	int ij = 0;
				//}
				//cartop 0.17209753561068819 * 2
				//198249.64033109313 * 1.2
				//198249.64033109313 * 0.1
				//val = val / (41650752.483175509 * 2);
				//val = val / 8.5895957922795958;
				//val = val / 1.4324368664210838;
				val = val / 1244284509.3515666;
				//if(val >= 1)
				//	val = 0.99;
				//if(val >= 0.41)
				//	val = 0.41;
				//val = val / 4;
				if (val >= 0.02)
					val = 0.02;
				if (val > maxangle_real)
					maxangle_real = val;

				outfile << " < 0 " << " , " << val << ">, " << endl;
			}


		}
	outfile << " }" << endl;

	//normal
	ii = -1;
	outfile << "normal_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	for (i = 0; i <= samplesm; i++)
		for (j = 0; j <= samplesn; j++)
		{
			double s = ((double)i) / samplesm;
			double t = ((double)j) / samplesn;

			ii++;
			if (!m_ArrayLabels[ii])
			{
				outfile << " < " << 0 << " , " << 0 << " , " << 0 << " >," << endl;
			}
			else
			{
				if (pSurface->ptransformation)
				{

					Transformation* pTrans = (Transformation*)pSurface->ptransformation;
					s = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s;
					t = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t;
					double u, v;
					pTrans->Evaluation(s, t, u, v);

					if (pTrans->m_ReparameterizationType == generalbilinear)
					{
						GeneralBilinearTransformation* pTran1 = (GeneralBilinearTransformation*)pTrans;
						if (u < pTran1->umin)
							label = 0;
						if (v < pTran1->vmin)
							label = 0;
						if (u > pTran1->umax)
							label = 0;
						if (v > pTran1->vmax)
							label = 0;
						if (u < pTran1->umin)
							label = 0;
						if (v < pTran1->vmin)
							label = 0;
						if (u > pTran1->umax)
							label = 0;
						if (v > pTran1->vmax)
							label = 0;
					}

					s = u;
					t = v;

				}
				else
				{
					double umin = pNurbsSurface1->GetUKnot(0);
					double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

					double vmin = pNurbsSurface1->GetVKnot(0);
					double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

					s = umin + (umax - umin) * s;
					t = vmin + (vmax - vmin) * t;
					u = s;
					v = t;
				}


				Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u, v);
				Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u, v);


				Vector3D Xu = pNurbsSurface1->ComputUDerivative1(u, v);
				Vector3D Xv = pNurbsSurface1->ComputVDerivative1(u, v);
				Vector3D n = Xu ^ Xv;
				n.SetUnit();

				outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >," << endl;
			}

		}
	outfile << " }" << endl;

	ii = 0;
	//face_indices {
	for (j = 0; j < samplesn; j++)
		for (i = 0; i < samplesm; i++)
		{
			double u = ((double)i) / samplesm;
			double v = ((double)j) / samplesn;

			int n1 = j * (samplesm + 1) + i;
			int n2 = j * (samplesm + 1) + i + 1;
			int n3 = (j + 1) * (samplesm + 1) + i;
			int n4 = (j + 1) * (samplesm + 1) + i + 1;

			if (m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
			{
				ii++;
			}
			if (m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
			{
				ii++;
			}
		}

	//face_indices {
	outfile << "face_indices {" << endl;
	outfile << ii << endl;
	for (j = 0; j < samplesn; j++)
		for (i = 0; i < samplesm; i++)
		{
			double u = ((double)i) / samplesm;
			double v = ((double)j) / samplesn;

			int n1 = j * (samplesm + 1) + i;
			int n2 = j * (samplesm + 1) + i + 1;
			int n3 = (j + 1) * (samplesm + 1) + i;
			int n4 = (j + 1) * (samplesm + 1) + i + 1;

			if (m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
			{
				outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >," << endl;
			}
			if (m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
			{
				outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >," << endl;
			}
		}
	outfile << " }" << endl;

	outfile.close();
}
void CFormView3::CheckAreaPreserving(C_NurbsSurf* pNurbsSurface)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	double maxdeviation = -1;
	double u1, v1;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();

		double len = Length(pNurbsSurface1->GetParaNormal(0, 0, FALSE));

		for (int i = 0; i <= 1000; i++)
			for (int j = 0; j <= 1000; j++)
			{
				double u = i * 0.001;
				double v = j * 0.001;

				double len1 = Length(pNurbsSurface1->GetParaNormal(u, v, FALSE));

				if (fabs(len - len1) > maxdeviation)
				{
					maxdeviation = fabs(len - len1);
					u1 = u;
					v1 = v;
				}
			}
	}
}
void CFormView3::OutPutAreaFactor_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm, int samplesn, char* filename)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();
		//Point3D pmin, pmax;
		//pNurbsSurface1->ComputeControlPointBoundingBox(pmin, pmax);
		double A = Compute_Area(pNurbsSurface1, 1, 1);
		std::ofstream outfile(filename);

		double minangle = 0;
		double maxangle = 50;
		int i, j;
		outfile << "vertex_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				Point3D p1;
				double u, v, s, t;
				double umin, umax, vmin, vmax;
				if (pSurface->ptransformation)
				{
					umin = pSurface->ptransformation->umin_Reparameterization;
					umax = pSurface->ptransformation->umax_Reparameterization;

					vmin = pSurface->ptransformation->vmin_Reparameterization;
					vmax = pSurface->ptransformation->vmax_Reparameterization;

					double minu = pNurbsSurface1->GetUKnot(0);
					double maxu = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetDegreeU());

					double minv = pNurbsSurface1->GetVKnot(0);
					double maxv = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetDegreeV());


					s = umin + (umax - umin) * (((double)i) / samplesm);
					t = vmin + (vmax - vmin) * (((double)j) / samplesn);

					pSurface->ptransformation->Evaluation(s, t, u, v);

					if (u > maxu)
						u = maxu;
					if (u < minu)
						u = minu;
					if (v > maxv)
						v = maxv;
					if (v < minv)
						v = minv;
					p1 = pNurbsSurface1->ComputPosition(u, v);

					if (_isnan(p1.x) || _isnan(p1.y) || _isnan(p1.z))
						ASSERT(0);
				}
				else
				{
					p1 = pNurbsSurface1->ComputPosition(((double)i) / samplesm, ((double)j) / samplesn);
				}
				outfile << "<" << p1[0] << " , " << p1[1] << " , " << -p1[2] << " >, " << endl;
			}
		outfile << " }" << endl;


		outfile << "uv_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		double maxangle_real = -1e10;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				Vector3D v1, v2;
				double s, t;
				double umin, umax, vmin, vmax;
				if (pSurface->ptransformation)
				{
					umin = pSurface->ptransformation->umin_Reparameterization;
					umax = pSurface->ptransformation->umax_Reparameterization;

					vmin = pSurface->ptransformation->vmin_Reparameterization;
					vmax = pSurface->ptransformation->vmax_Reparameterization;

					s = umin + (umax - umin) * (((double)i) / samplesm);
					t = vmin + (vmax - vmin) * (((double)j) / samplesn);

					//double s, t;
					//s = ((double)i) / samplesm;
					//t = ((double)j) / samplesn;
					//s = u;
					//t = v;
					pSurface->ptransformation->Evaluation(s, t, u, v);

					double minu = pNurbsSurface1->GetUKnot(0);
					double maxu = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetDegreeU());

					double minv = pNurbsSurface1->GetVKnot(0);
					double maxv = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetDegreeV());

					if (u > maxu)
						u = maxu;
					if (u < minu)
						u = minu;
					if (v > maxv)
						v = maxv;
					if (v < minv)
						v = minv;
					//pSurface->ptransformation->Evaluation( u, v, s, t);

					Vector3D Xu = pNurbsSurface1->ComputUDerivative1(u, v);
					Vector3D Xv = pNurbsSurface1->ComputVDerivative1(u, v);

					double us = pSurface->ptransformation->Comput_u_s(s, t);
					double vs = pSurface->ptransformation->Comput_v_s(s, t);
					double ut = pSurface->ptransformation->Comput_u_t(s, t);
					double vt = pSurface->ptransformation->Comput_v_t(s, t);

					Vector3D Xs = Xu * us + Xv * vs;
					Vector3D Xt = Xu * ut + Xv * vt;

					v1 = Xs;
					v2 = Xt;
				}
				else
				{
					v1 = pNurbsSurface1->ComputUDerivative1(u, v);
					v2 = pNurbsSurface1->ComputVDerivative1(u, v);
				}


				Vector3D v3 = v1 ^ v2;
				double val = fabs(Length(v3) - A) / A;
				//sophie 8.222
				//cartop 0.97 
				//venus 1.8
				//dixing2.56 
				//bezier 1.8
				val = val / (1);
				//if(val >= 0.01)
				//	val = 0.01;
				//if(val > maxangle_real)
					//maxangle_real = val;

				outfile << " < 0 " << " , " << val << ">, " << endl;
			}
		outfile << " }" << endl;

		//normal
		outfile << "normal_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		for (i = 0; i <= samplesm; i++)
			for (j = 0; j <= samplesn; j++)
			{
				double umin, umax, vmin, vmax, s, t, u, v;
				if (pSurface->ptransformation)
				{
					umin = pSurface->ptransformation->umin_Reparameterization;
					umax = pSurface->ptransformation->umax_Reparameterization;

					vmin = pSurface->ptransformation->vmin_Reparameterization;
					vmax = pSurface->ptransformation->vmax_Reparameterization;

					s = umin + (umax - umin) * (((double)i) / samplesm);
					t = vmin + (vmax - vmin) * (((double)j) / samplesn);

					pSurface->ptransformation->Evaluation(s, t, u, v);

					double minu = pNurbsSurface1->GetUKnot(0);
					double maxu = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetDegreeU());

					double minv = pNurbsSurface1->GetVKnot(0);
					double maxv = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetDegreeV());

					if (u > maxu)
						u = maxu;
					if (u < minu)
						u = minu;
					if (v > maxv)
						v = maxv;
					if (v < minv)
						v = minv;
				}
				else
				{
					u = ((double)i) / samplesm;
					v = ((double)j) / samplesn;
				}

				Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u, v);
				Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u, v);
				v1.SetUnit();
				v2.SetUnit();

				Vector3D n = v1 ^ v2;
				outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >," << endl;
			}
		outfile << " }" << endl;

		//face_indices {
		outfile << "face_indices {" << endl;
		outfile << (samplesm) * (samplesn) * 2 << endl;
		for (j = 0; j < samplesn; j++)
			for (i = 0; i < samplesm; i++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				int n1 = j * (samplesm + 1) + i;
				int n2 = j * (samplesm + 1) + i + 1;
				int n3 = (j + 1) * (samplesm + 1) + i;
				int n4 = (j + 1) * (samplesm + 1) + i + 1;

				outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >," << endl;
				outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >," << endl;
			}
		outfile << " }" << endl;

		outfile.close();
	}
}
void CFormView3::OnBnClickedFaceexample()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers2012\\SVNNurbs\\data\\face.sur");
	ReadViewPointFromFile("D:\\papers2012\\AreaPreservingBezierusingmobiustransformations\\Povray\\Example2\\originalpara\\viewpoint_config.inc");
	pTreeView->Invalidate(TRUE);
}


void CFormView3::OnBnClickedFaceexample2()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	Compute_Optimal_AreaPreserving_IsoParameterCurves_NumericalMethod(pSurf->Converte_C_NurbsSurface());
	SetFaceModification();
	//OnBnClickedProjecttoparameterdomain();
}

void CFormView3::OnBnClickedBezierexamplestep1()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\data\\BezierMobius.sur");
	//ReadViewPointFromFile("D:\\papers\\2013\\AreaPreservingBezierusingmobiustransformations\\Povray\\bezier2\\originalpara\\viewpoint_config.inc");
	pTreeView->Invalidate(TRUE);
}
void CFormView3::OnSaveasMeshMfile()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CFileDialog dlg(FALSE);
		if (dlg.DoModal())
		{
			m_FileTextureName = dlg.GetPathName();

			pMesh3D->SaveMFile((LPTSTR)(LPCTSTR)m_FileTextureName);
		}
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pOpenMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		CFileDialog dlg(FALSE);
		if (dlg.DoModal())
		{
			m_FileTextureName = dlg.GetPathName();

			pOpenMesh3D->SaveMFile((LPTSTR)(LPCTSTR)m_FileTextureName);
		}
	}
}
void CFormView3::OnMeshSetColor()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		MeshSetColor dlg;
		dlg.pMesh = pMesh3D;
		dlg.DoModal();
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		MeshSetColor dlg;
		dlg.pMesh_Openmesh = pMesh3D;
		dlg.DoModal();
	}
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnSaveastmfile()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CFileDialog dlg(FALSE);
		if (dlg.DoModal())
		{
			m_FileTextureName = dlg.GetPathName();

			std::ofstream file(m_FileTextureName);

			file << "#Mesh \n";
			file << pMesh3D->NbVertex() << "  " << pMesh3D->NbFace() << "\n";

			//Vertices
			for (int i = 0; i < pMesh3D->NbVertex(); i++)
			{
				pMesh3D->GetVertex(i)->m_Flag = i;
				file << pMesh3D->GetVertex(i)->m_Coord[0] << " " << pMesh3D->GetVertex(i)->m_Coord[1] << " " << pMesh3D->GetVertex(i)->m_Coord[2] << " \n ";
			}

			//faces
			for (int i = 0; i < pMesh3D->NbFace(); i++)
			{
				file << pMesh3D->GetFace(i)->v1()->m_Flag << " " << pMesh3D->GetFace(i)->v2()->m_Flag << " " << pMesh3D->GetFace(i)->v3()->m_Flag << " \n ";
			}

			file.close();
		}
	}
}

void CFormView3::OnBnClickedVenusStep1()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers2012\\SVNNurbs\\data\\venus.sur");
	ReadViewPointFromFile("D:\\papers2012\\AreaPreservingBezierusingmobiustransformations\\Povray\\venus\\originalpara\\viewpoint_config.inc");
	pTreeView->Invalidate(TRUE);
}

void CFormView3::OnBnClickedVenusStep2()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	Compute_Optimal_AreaPreserving_IsoParameterCurves_NumericalMethod(pSurf->Converte_C_NurbsSurface());
	SetFaceModification();
}

void CFormView3::OnBnClickedVenusStep3()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CDC* pdc = pTreeView->GetDC();
	//ProjectToParameterDomain dlg;
	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	//SwapBuffers(pdc->GetSafeHdc());

	CImage m_Image;
	m_Image.Load("D:\\papers2012\\AreaPreservingBezierusingmobiustransformations\\Povray\\venus\\originalpara\\scene.bmp");
	int width = m_Image.GetWidth();
	int height = m_Image.GetHeight();
	COLORREF color;
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			color = m_Image.GetPixel(i, j);
			//if(color != 0xffffff)
			//{
			//	TRACE("\n point: %d %d \n",i,j);
			//}
			//color = pdc->GetPixel(x,y);
			//color = image1.GetPixel(x,y);
			pdc->SetPixel(i, j, color);
		}
	//double x00 = 695;
	//double y00 = 347;
	//double x33 = 841.5;
	//double y33 = 788;
	//double x00 = 89;
	//double y00 = 203;
	//double x33 = 1510;
	//double y33 = 1024;
	double x00 = 947;
	double y00 = 214;
	double x33 = 622;
	double y33 = 991;

	Vector3D p = pSurface->ComputPosition(0.35, 0.1) - ZeroP3D;
	double out[3];
	Vector3D proPoint;
	gluProject(p[0], p[1], p[2], pSurface->modelview, pSurface->projection, pSurface->m_viewPort, &(out[0]), &(out[1]), &(out[2]));

	double nx00 = out[0];
	double ny00 = proPoint.y = pSurface->m_viewPort[3] - out[1];

	p = pSurface->ComputPosition(0.7, 0.9) - ZeroP3D;
	gluProject(p[0], p[1], p[2], pSurface->modelview, pSurface->projection, pSurface->m_viewPort, &(out[0]), &(out[1]), &(out[2]));

	double nx33 = out[0];
	double ny33 = proPoint.y = pSurface->m_viewPort[3] - out[1];

	double xscale = (x33 - x00) / (nx33 - nx00);
	double yscale = (y33 - y00) / (ny33 - ny00);

	int resolution = 1500;
	int offset = 100;
	COLORREF red1 = 0x00ff0000;
	COLORREF black = 0x00000000;
	for (int i = 0; i <= resolution; i++)
		for (int j = 0; j <= resolution; j++)
		{
			double u = (i + 0.0) / resolution;
			double v = (j + 0.0) / resolution;

			//Have to modify this place for the face surface.
			pSurface->GetInterpolationModifiedUV(u, v);

			Vector3D p = pSurface->ComputPosition(u, v) - ZeroP3D;
			double out[3];
			Vector3D proPoint;
			gluProject(p[0], p[1], p[2], pSurface->modelview, pSurface->projection, pSurface->m_viewPort, &(out[0]), &(out[1]), &(out[2]));

			double x = out[0];
			double y = pSurface->m_viewPort[3] - out[1];

			//Get the depth of the current pixel
			double zdepth;
			glReadPixels(x + 0.5, y + 0.5, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zdepth);
			if (out[2] > zdepth)
				color = black;
			else
				color = m_Image.GetPixel((x - nx00) * xscale + x00 + 0.5, (y - ny00) * yscale + y00 + 0.5);

			//proPoint.z = out[2];
			//color = pdc->GetPixel(x,y);
			//color = image1.GetPixel(x,y);
			if ((x - nx00) * xscale + x00 < width && (y - ny00) * yscale + y00 < height)
				pdc->SetPixel((x - nx00) * xscale + x00 + 0.5, (y - ny00) * yscale + y00 + 0.5, red1);

			pdc->SetPixel(i + offset, resolution - j + offset, color);
			//COLORREF color = pdc->GetPixel(proPoint.x,proPoint.y);
			//pdc->SetPixel(proPoint.x,proPoint.y,color);
			//unsigned char pixel[3];
			//glReadPixels(proPoint.x, proPoint.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
			//dlg.m_colorarray[i][j] = pixel[0]*256*256+pixel[1]*256+pixel[2];
		}
}
void CFormView3::ReadBSFFormat()
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal())
	{
		CString m_FileName = dlg.GetPathName();

		std::ifstream inputfile(m_FileName);
		m_FileName.Replace(".bsf", ".sur");
		std::ofstream outputfile(m_FileName);

		char message[100];
		//BSF2
		inputfile >> message;

		//orderu & orderv.
		int orderu, orderv;
		inputfile >> orderu;
		inputfile >> orderv;

		//control number
		int unumber, vnumber;
		inputfile >> unumber;
		inputfile >> vnumber;
		if (unumber < 0)
			unumber = -unumber;
		if (vnumber < 0)
			vnumber = -vnumber;

		outputfile << unumber << "  " << vnumber << endl;

		outputfile << orderu - 1 << "  " << orderv - 1 << endl;

		//knot
		std::vector<double> uknot, vknot;
		double val;
		for (int i = 0; i < orderu + unumber; i++)
		{
			inputfile >> val;
			uknot.push_back(val);
		}

		for (int i = 0; i < orderv + vnumber; i++)
		{
			inputfile >> val;
			vknot.push_back(val);
		}


		/*for(int i1 = 0; i1 < orderu + unumber; i1++)
			for(int i2 = 0; i2 < orderu + unumber - 1; i2++)
			{
				if(uknot[i2] > uknot[i2 + 1])
				{
					double temp = uknot[i2];
					uknot[i2] = uknot[i2 + 1];
					uknot[i2 + 1] = temp;
				}
			}

		for(int i1 = 0; i1 < orderv + vnumber; i1++)
			for(int i2 = 0; i2 < orderv + vnumber - 1; i2++)
			{
				if(vknot[i2] > vknot[i2 + 1])
				{
						double temp = vknot[i2];
						vknot[i2] = vknot[i2 + 1];
						vknot[i2 + 1] = temp;
				}
			}*/


		for (int i = 0; i < orderu + unumber; i++)
		{
			uknot[i] = uknot[i] / uknot[orderu + unumber - 1];
		}

		for (int i = 0; i < orderv + vnumber; i++)
		{
			vknot[i] = vknot[i] / vknot[orderv + vnumber - 1];
		}

		for (int i = 0; i < orderu + unumber; i++)
		{
			outputfile << uknot[i] << " ";
		}
		outputfile << endl;

		for (int i = 0; i < orderv + vnumber; i++)
		{
			outputfile << vknot[i] << " ";
		}
		outputfile << endl;


		//control point
		double x[100][100], y[100][100], z[100][100];
		for (int i = 0; i < vnumber; i++)
			for (int j = 0; j < unumber; j++)
			{
				inputfile >> x[j][i] >> y[j][i] >> z[j][i];
			}
		for (int i = 0; i < unumber; i++)
			for (int j = 0; j < vnumber; j++)
			{

				outputfile << x[i][j] << " " << y[i][j] << " " << z[i][j] << " 1" << endl;
			}
		inputfile.close();
		outputfile.close();
	}
}
void CFormView3::OnBnClickedReadbsffile()
{
	// TODO: Add your control notification handler code here
	ReadBSFFormat();
}

void CFormView3::OnSurfaceShowoptimalareapreserving()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pSur = pSurface->Converte_C_NurbsSurface();
		pSurface->m_showAreaPreservingIsoParameterCurves = !pSurface->m_showAreaPreservingIsoParameterCurves;
		pTreeView->Invalidate(TRUE);
	}
}

void CFormView3::OnSurfaceShowoptimalareapreservingmodified()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pSur = pSurface->Converte_C_NurbsSurface();
		pSurface->m_showAreaPreservingIsoParameterCurves_Modified = !pSurface->m_showAreaPreservingIsoParameterCurves_Modified;
		pTreeView->Invalidate(TRUE);
	}
}

void CFormView3::OnUpdateSurfaceShowoptimalareapreserving(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pSur = pSurface->Converte_C_NurbsSurface();
		pCmdUI->SetCheck(pSurface->m_showAreaPreservingIsoParameterCurves);
	}
}

void CFormView3::OnUpdateSurfaceShowoptimalareapreservingmodified(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_NurbsSurf* pSur = pSurface->Converte_C_NurbsSurface();
		pCmdUI->SetCheck(pSurface->m_showAreaPreservingIsoParameterCurves_Modified);
	}
}

void CFormView3::OnBnClickedBonestep1()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers2012\\SVNNurbs\\data\\yukun\\ballJoint1.0.sur");
	ReadViewPointFromFile("D:\\papers2012\\AreaPreservingBezierusingmobiustransformations\\Povray\\example3\\originalpara\\viewpoint_config.inc");
	pTreeView->Invalidate(TRUE);
}

void CFormView3::OnBnClickedBonestep2()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	Compute_Optimal_AreaPreserving_IsoParameterCurves_NumericalMethod(pSurf->Converte_C_NurbsSurface());
	SetBoneModification();
}

void CFormView3::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers2012\\SVNNurbs\\data\\yukun\\screwdriver2.sur");
	ReadViewPointFromFile("D:\\papers2012\\AreaPreservingBezierusingmobiustransformations\\Povray\\example4\\originalpara\\viewpoint_config.inc");
	pTreeView->Invalidate(TRUE);
}

void CFormView3::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	Compute_Optimal_AreaPreserving_IsoParameterCurves_NumericalMethod(pSurf->Converte_C_NurbsSurface());
	SetVaseModification();
}

void CFormView3::OnBnClickedFace2step1()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\code\\SVNNurbs\\data\\yukun\\face2.fs.sur");
	ReadViewPointFromFile("D:\\papers\\2013\\AreaPreservingBezierusingmobiustransformations\\Povray\\example4\\originalpara\\viewpoint_config.inc");
	pTreeView->Invalidate(TRUE);
}

void CFormView3::OnBnClickedFace2step2()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	Compute_Optimal_AreaPreserving_IsoParameterCurves_NumericalMethod(pSurf->Converte_C_NurbsSurface());
	SetFace2Modification();
}

int ConNow = 0;
void CFormView3::OnBnClickedMeshanimation()
{
	//动态的显示优化过程，每点击一次按钮，进行一步
	if (ConNow >= MeshArray.size())
		return;

	//show the count of steps and current step 
	char message[30];
	wsprintf(message, "%s%d%s%d%s", "需要", MeshArray.size(), "步，当前为第", ConNow + 1, "步");
	AfxGetMainWnd()->SetWindowText(message);

	//delete other meshes.
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	for (int i = 1; i < num; i++)
	{
		pDoc->m_SceneGraph.RemoveAt(1);
	}
	//change the mesh
	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();

	//get the control points of mesh
	std::vector<double> mesh = MeshArray[ConNow];

	if (pSurf->GetDegreeU() == 2 && pSurf->GetDegreeV() == 2)//2X2
	{
		for (int i = 0; i < pSurf->GetCtrlNumberU(); i++)
			for (int j = 0; j < pSurf->GetCtrlNumberV(); j++)
			{
				Point3D point;
				point.x = mesh[OrthogonalityEnergy::getIndex(2, 2, i, j, 0)];
				point.y = mesh[OrthogonalityEnergy::getIndex(2, 2, i, j, 1)];
				point.z = mesh[OrthogonalityEnergy::getIndex(2, 2, i, j, 2)];
				pSurf->SetControlPoint(i, j, point);
			}

		ConNow++;

	}
	else if (pSurf->GetDegreeU() == 3 && pSurf->GetDegreeV() == 3)//3X3
	{
		for (int i = 0; i < pSurf->GetCtrlNumberU(); i++)
			for (int j = 0; j < pSurf->GetCtrlNumberV(); j++)
			{
				Point3D point;
				point.x = mesh[OrthogonalityEnergy::getIndexBSpline33(i, j, 0)];
				point.y = mesh[OrthogonalityEnergy::getIndexBSpline33(i, j, 1)];
				point.z = mesh[OrthogonalityEnergy::getIndexBSpline33(i, j, 2)];
				pSurf->SetControlPoint(i, j, point);
			}

		ConNow++;;
	}
	//show the new mesh
	CNurbsSuface* pNewNSurf = pSurf->GetCNurbsSurface();
	pNewNSurf->SetTransform(*pNSurf->GetTransform());
	pNewNSurf->m_showIsoParameterCurves = pNSurf->m_showIsoParameterCurves;
	pDoc->m_SceneGraph.RemoveAt(0);
	pDoc->m_SceneGraph.Add(pNewNSurf);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);


}

void CFormView3::OnBnClickedFinalmesh()
{
	//change the title of window
	char message[30];
	wsprintf(message, "最后一步");
	AfxGetMainWnd()->SetWindowText(message);


	//delete other meshes.
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	for (int i = 1; i < num; i++)
	{
		pDoc->m_SceneGraph.RemoveAt(1);
	}
	//change the mesh
	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();

	//get the control points of mesh
	std::vector<double> mesh = MeshArray[MeshArray.size() - 1];

	if (pSurf->GetDegreeU() == 2 && pSurf->GetDegreeV() == 2)//2X2
	{
		for (int i = 0; i < pSurf->GetCtrlNumberU(); i++)
			for (int j = 0; j < pSurf->GetCtrlNumberV(); j++)
			{
				Point3D point;
				point.x = mesh[OrthogonalityEnergy::getIndex(2, 2, i, j, 0)];
				point.y = mesh[OrthogonalityEnergy::getIndex(2, 2, i, j, 1)];
				point.z = mesh[OrthogonalityEnergy::getIndex(2, 2, i, j, 2)];
				pSurf->SetControlPoint(i, j, point);
			}

	}
	else if (pSurf->GetDegreeU() == 3 && pSurf->GetDegreeV() == 3)//3X3
	{
		for (int i = 0; i < pSurf->GetCtrlNumberU(); i++)
			for (int j = 0; j < pSurf->GetCtrlNumberV(); j++)
			{
				Point3D point;
				point.x = mesh[OrthogonalityEnergy::getIndexBSpline33(i, j, 0)];
				point.y = mesh[OrthogonalityEnergy::getIndexBSpline33(i, j, 1)];
				point.z = mesh[OrthogonalityEnergy::getIndexBSpline33(i, j, 2)];
				pSurf->SetControlPoint(i, j, point);
			}
	}
	//show the new mesh
	CNurbsSuface* pNewNSurf = pSurf->GetCNurbsSurface();
	pNewNSurf->SetTransform(*pNSurf->GetTransform());
	pNewNSurf->m_showIsoParameterCurves = pNSurf->m_showIsoParameterCurves;
	pDoc->m_SceneGraph.RemoveAt(0);
	pDoc->m_SceneGraph.Add(pNewNSurf);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);

}

void CFormView3::OnSurfaceShowcolororthogonality()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		pSurface->m_showColorOrthogonality = !pSurface->m_showColorOrthogonality;
		pSurface->m_Modified = TRUE;
		pDoc->UpdateAllViews(NULL);
	}
}


void CFormView3::OnUpdateSurfaceShowcolororthogonality(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pSurface->m_showColorOrthogonality);
	}
}

void CFormView3::OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	//根据当前TAB上的选择来显示属性页
	switch (m_tab.GetCurSel())
	{
	case 0:
		objTab.ShowWindow(TRUE);
		transformationTab.ShowWindow(FALSE);
		actionTab.ShowWindow(FALSE);
		break;
	case 1:
		objTab.ShowWindow(FALSE);
		transformationTab.ShowWindow(TRUE);
		actionTab.ShowWindow(FALSE);
		break;
	case 2:
		objTab.ShowWindow(FALSE);
		transformationTab.ShowWindow(FALSE);
		actionTab.ShowWindow(TRUE);
		break;

	default:;
	}
	*pResult = 0;
}

void CFormView3::OnSurfaceShowtransformedparameterdomain()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		C_ShowTransformedParameterDomain dlg;
		dlg.pSurface = pSurface;
		dlg.DoModal();
	}
}

void CFormView3::OnMeshGeneratequadmesh()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (currentindex < 0 || currentindex >= num)
		return;

	int samplesm = 14;
	int samplesn = 14;
	char filename[100];
	sprintf(filename, "D:\\meshquad.obj");
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);

		std::ofstream outfile(filename);
		int i, j;

		outfile << "# " << (samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn << "faces" << endl;
		for (i = 0; i < pMesh->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh->GetVertex(i);
			outfile << "v " << pVertex->x() << " " << pVertex->y() << " " << pVertex->z() << endl;
		}

		for (j = 0; j < samplesn; j++)
		{
			for (i = 0; i < samplesm; i++)
			{
				//
			//outfile << "f " << (samplesn + 1) * j + i + 1 <<" " << (samplesn + 1) * j + i + 2 << " "<< (samplesn + 1) * j + i + 2 + samplesn + 1 << " "<< (samplesn + 1) * j + i + 1 + samplesn + 1 << endl;
				int index1 = (samplesn + 1) * j + i + 1;
				int index2 = (samplesn + 1) * j + i + 2;
				int index3 = (samplesn + 1) * j + i + 1 + samplesn + 1;
				int index4 = (samplesn + 1) * j + i + 2 + samplesn + 1;
				outfile << "f " << index1 << " " << index2 << " " << index3 << " " << index4 << endl;
			}
		}
		outfile.close();
	}
	//inc
	sprintf(filename, "D:\\meshquad.inc");
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);

		std::ofstream outfile1(filename);
		int i, j;

		for (j = 0; j < samplesn; j++)
		{
			for (i = 0; i < samplesm; i++)
			{
				//
				//outfile << "f " << (samplesn + 1) * j + i + 1 <<" " << (samplesn + 1) * j + i + 2 << " "<< (samplesn + 1) * j + i + 2 + samplesn + 1 << " "<< (samplesn + 1) * j + i + 1 + samplesn + 1 << endl;
				int index1 = (samplesn + 1) * j + i + 1;
				int index2 = (samplesn + 1) * j + i + 2;
				int index3 = (samplesn + 1) * j + i + 1 + samplesn + 1;
				int index4 = (samplesn + 1) * j + i + 2 + samplesn + 1;

				CVertex3d* pVertex1 = pMesh->GetVertex(index1 - 1);
				CVertex3d* pVertex2 = pMesh->GetVertex(index2 - 1);
				CVertex3d* pVertex3 = pMesh->GetVertex(index3 - 1);
				CVertex3d* pVertex4 = pMesh->GetVertex(index4 - 1);

				outfile1 << "cylinder {<" << pVertex1->x() << " , " << pVertex1->y() << " , " << -pVertex1->z() << " >,<" << pVertex2->x() << " , " << pVertex2->y() << " , " << -pVertex2->z() << " > ,r_edge texture { edge_tex } }" << endl;
				outfile1 << "cylinder {<" << pVertex2->x() << " , " << pVertex2->y() << " , " << -pVertex2->z() << " >,<" << pVertex4->x() << " , " << pVertex4->y() << " , " << -pVertex4->z() << " > ,r_edge texture { edge_tex } }" << endl;
				outfile1 << "cylinder {<" << pVertex4->x() << " , " << pVertex4->y() << " , " << -pVertex4->z() << " >,<" << pVertex3->x() << " , " << pVertex3->y() << " , " << -pVertex3->z() << " > ,r_edge texture { edge_tex } }" << endl;
				outfile1 << "cylinder {<" << pVertex3->x() << " , " << pVertex3->y() << " , " << -pVertex3->z() << " >,<" << pVertex1->x() << " , " << pVertex1->y() << " , " << -pVertex1->z() << " > ,r_edge texture { edge_tex } }" << endl;

			}
		}
		outfile1.close();
	}
}


void CFormView3::OnSurfaceConverttotspline()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		Tspline* ptspline = new Tspline;
		std::vector<int> m_uknotindex, m_vknotindex;
		ptspline->BuildFromNURBS(pSurface, m_uknotindex, m_vknotindex);
		ptspline->filename = "Tspline";
		ptspline->SetTransform(*pSurface->GetTransform());
		pDoc->m_SceneGraph.Add(ptspline);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
}
void CFormView3::OnTsplineSaveAs()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_TSPLINE)
	{
		Tspline* pSurface = (Tspline*)pDoc->m_SceneGraph.GetAt(currentindex);
		/*if(pSurface->ptransformation)
		{
			if(pSurface->ptransformation->m_ReparameterizationType == mobius)
			{
				MobiusTransformation* mobiustrans = (MobiusTransformation*)pSurface->ptransformation;
				pSur = pSur->GetMobiusTransformedSurface( mobiustrans->m_Mobiustransformation_Alpha,
					mobiustrans->m_Mobiustransformation_Beta);
				pSurface = pSur->GetCNurbsSurface();
			}
		}*/
		CFileDialog dlg(FALSE);
		if (dlg.DoModal())
		{
			CString m_FileName = dlg.GetPathName();

			std::ofstream file(m_FileName);

			pSurface->Save(file, m_FileName);

			if (pSurface->ptransformation)
			{
				pSurface->ptransformation->Save(file);
			}

			file.close();
		}
	}
}
void CFormView3::OnTsplinePlotpreimage()
{
	// TODO: Add your command handler code here

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_TSPLINE)
	{
		Tspline* pSurface = (Tspline*)pDoc->m_SceneGraph.GetAt(currentindex);
		Tspline_PreImageDialog dialog;
		dialog.pTspline = pSurface;
		dialog.DoModal();
	}
}
void CFormView3::OnMeshShowface()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showFace = !pMesh3D->m_showFace;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
		{
			CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
			pMesh3D->m_showFace = !pMesh3D->m_showFace;
		}

	//update right view
	UpdateShowView();
}
extern Action_PolyOmino* pPolyOmino;
void CFormView3::OnMeshSetCurrentMesh()
{
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
}
void CFormView3::OnMeshShowLandmarkEdge()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showLandmarkEdge = !pMesh3D->m_showLandmarkEdge;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showLandmarkEdge = !pMesh3D->m_showLandmarkEdge;
	}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnMeshShowMergeConstraints()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showMergeConstraints = !pMesh3D->m_showMergeConstraints;
	}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnMeshShowSelectedLandmarkEdge()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedLandmarkEdge = !pMesh3D->m_showSelectedLandmarkEdge;
	}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnMeshShowLandmarkVertex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showLandmarkVertex = !pMesh3D->m_showLandmarkVertex;
	}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnMeshShowSelectedSharpEdge()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedSharpEdge = !pMesh3D->m_showSelectedSharpEdge;
	}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnPolygonShowRingIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_POLYGON)
	{
		Shape_Polygon* pMesh3D = (Shape_Polygon*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showPolygonRingIndex = !pMesh3D->m_showPolygonRingIndex;
	}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnPolygonDelete()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	//	ASSERT_VALID(pDoc);
	pDoc->m_SceneGraph.RemoveAt(currentindex);

	UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void CFormView3::OnPolygonShowInnerRingEdge()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_POLYGON)
	{
		Shape_Polygon* pMesh3D = (Shape_Polygon*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showPolygonInnerRing = !pMesh3D->m_showPolygonInnerRing;
	}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnPolygonShowRingEdge()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_POLYGON)
	{
		Shape_Polygon* pMesh3D = (Shape_Polygon*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showPolygonRingEdge = !pMesh3D->m_showPolygonRingEdge;
	}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnMeshShowSharpEdge()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSharpEdge = !pMesh3D->m_showSharpEdge;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSharpEdge = !pMesh3D->m_showSharpEdge;
	}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnMeshShowedge()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showEdge = !pMesh3D->m_showEdge;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
		{
			CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
			pMesh3D->m_showEdge = !pMesh3D->m_showEdge;
		}

	//Update right view;
	UpdateShowView();
}
void CFormView3::OnUpdateMeshShowface(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFace);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFace);
	}
}
void CFormView3::OnUpdateMeshShowSelectedlandmarkEdge(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedLandmarkEdge);
	}
}
void CFormView3::OnUpdateMeshShowMergeConstraints(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showMergeConstraints);
	}
}
void CFormView3::OnUpdateMeshShowlandmarkEdge(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showLandmarkEdge);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showLandmarkEdge);
	}
}
void CFormView3::OnUpdateMeshShowlandmarkVertex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showLandmarkVertex);
	}
}

void CFormView3::OnUpdateMeshShowSelectedSharpedge(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedSharpEdge);
	}
}
void CFormView3::OnUpdatePolygonShowRingIndex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_POLYGON)
	{
		Shape_Polygon* pMesh3D = (Shape_Polygon*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showPolygonRingIndex);
	}
}
void CFormView3::OnUpdatePolygonShowInnerRingEdge(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_POLYGON)
	{
		Shape_Polygon* pMesh3D = (Shape_Polygon*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showPolygonInnerRing);
	}
}
void CFormView3::OnUpdatePolygonShowRingEdge(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_POLYGON)
	{
		Shape_Polygon* pMesh3D = (Shape_Polygon*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showPolygonRingEdge);
	}
}
void CFormView3::OnUpdateMeshShowSharpedge(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSharpEdge);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSharpEdge);
	}
}
void CFormView3::OnUpdateMeshShowEdge(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showEdge);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showEdge);
	}
}
void CFormView3::OnMeshShowSelectedVertexIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedVertexIndex = !pMesh3D->m_showSelectedVertexIndex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedVertexIndex = !pMesh3D->m_showSelectedVertexIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowSelectedEdge()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedEdge = !pMesh3D->m_showSelectedEdge;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedEdge = !pMesh3D->m_showSelectedEdge;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowPointsOnSurface()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showPointsOnSurface = !pMesh3D->m_showPointsOnSurface;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showPointsOnSurface = !pMesh3D->m_showPointsOnSurface;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowSelectedvertex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedVertex = !pMesh3D->m_showSelectedVertex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedVertex = !pMesh3D->m_showSelectedVertex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowFeaturevertexIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showFeatureVertexIndex = !pMesh3D->m_showFeatureVertexIndex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showFeatureVertexIndex = !pMesh3D->m_showFeatureVertexIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowFeaturevertex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showFeatureVertex = !pMesh3D->m_showFeatureVertex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showFeatureVertex = !pMesh3D->m_showFeatureVertex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowSharpSelectedvertex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedSharpVertex = !pMesh3D->m_showSelectedSharpVertex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedSharpVertex = !pMesh3D->m_showSelectedSharpVertex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowSelectedface()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedFace = !pMesh3D->m_showSelectedFace;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSelectedFace = !pMesh3D->m_showSelectedFace;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowvertex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showVertex = !pMesh3D->m_showVertex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showVertex = !pMesh3D->m_showVertex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowSharpEdgeIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSharpEdgeIndex = !pMesh3D->m_showSharpEdgeIndex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSharpEdgeIndex = !pMesh3D->m_showSharpEdgeIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowAnchorPonits()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showAnchorPoints = !pMesh3D->m_showAnchorPoints;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowLandmarkVertexIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showLandmarkVertexIndex = !pMesh3D->m_showLandmarkVertexIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowLandmarkIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showLandmarkIndex = !pMesh3D->m_showLandmarkIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowFaceIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showFaceIndex = !pMesh3D->m_showFaceIndex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showFaceIndex = !pMesh3D->m_showFaceIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowedgeIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showEdgeIndex = !pMesh3D->m_showEdgeIndex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showEdgeIndex = !pMesh3D->m_showEdgeIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowvertexIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showVertexIndex = !pMesh3D->m_showVertexIndex;
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showVertexIndex = !pMesh3D->m_showVertexIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowUniformGrid()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showUniformGrid = !pMesh3D->m_showUniformGrid;
	}

	UpdateShowView();
}
void CFormView3::OnShowVertexDisplacement()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showVertexDisplacement = !pMesh3D->m_showVertexDisplacement;
	}

	UpdateShowView();
}
void CFormView3::OnShowVertexMUVectorField()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showVertexMUVectorField = !pMesh3D->m_showVertexMUVectorField;
	}

	UpdateShowView();
}
void CFormView3::OnUpdateShowVertexDisplacement(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showVertexDisplacement);
	}
}
void CFormView3::OnShowVertexMU()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showVertexMUMou = !pMesh3D->m_showVertexMUMou;
	}

	UpdateShowView();
}
void CFormView3::OnUpdateShowVertexMUVectorField(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showVertexMUVectorField);
	}
}
void CFormView3::OnUpdateShowVertexMU(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showVertexMUMou);
	}
}
void CFormView3::OnUpdateShowTriangleMU(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showTriangleMU);
	}
}
void CFormView3::OnShowTriangleMU()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showTriangleMU = !pMesh3D->m_showTriangleMU;
	}

	UpdateShowView();
}
void CFormView3::OnShowPuzzleGraph()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showPuzzleGraph = !pMesh3D->m_showPuzzleGraph;
	}

	UpdateShowView();
}
void CFormView3::OnUpdateMeshShowLandmarkVertexIndex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showLandmarkVertexIndex);
	}
}
void CFormView3::OnUpdateMeshShowAnchorPonits(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showAnchorPoints);
	}
}
void CFormView3::OnUpdateMeshShowLandmarkIndex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showLandmarkIndex);
	}
}
void CFormView3::OnUpdateMeshShowSharpEdgeIndex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSharpEdgeIndex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSharpEdgeIndex);
	}
}
void CFormView3::OnMeshShowFaceArea()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showFaceArea = !pMesh3D->m_showFaceArea;
	}

	UpdateShowView();
}
void CFormView3::OnUpdateMeshShowFaceArea(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFaceArea);
	}
}
void CFormView3::OnUpdateMeshShowFaceIndex(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFaceIndex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFaceIndex);
	}
}
void CFormView3::OnUpdateMeshShowedgeIndex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showEdgeIndex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showEdgeIndex);
	}
}
void CFormView3::OnUpdateMeshShowvertexIndex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showVertexIndex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showVertexIndex);
	}
}
void CFormView3::OnUpdateMeshShowUniformGrid(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showUniformGrid);
	}
}
void CFormView3::OnUpdateShowPuzzleGraph(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showPuzzleGraph);
	}
}
void CFormView3::OnUpdateMeshShowvertex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showVertex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showVertex);
	}
}
void CFormView3::OnUpdateMeshShowFeaturevertex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFeatureVertex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFeatureVertex);
	}
}
void CFormView3::OnUpdateMeshShowFeaturevertexIndex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFeatureVertexIndex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFeatureVertexIndex);
	}
}
void CFormView3::OnUpdateMeshShowSharpSelectedvertex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedSharpVertex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedSharpVertex);
	}
}
void CFormView3::OnUpdateMeshShowSelectedVertexIndex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedVertexIndex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedVertexIndex);
	}
}
void CFormView3::OnUpdateMeshShowSelectedEdge(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedEdge);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedEdge);
	}
}
void CFormView3::OnUpdateMeshShowPointsOnSurface(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showPointsOnSurface);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showPointsOnSurface);
	}
}
void CFormView3::OnUpdateMeshShowSelectedvertex(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedVertex);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedVertex);
	}
}
void CFormView3::OnUpdateMeshShowSelectedface(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedFace);
	}
	else if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSelectedFace);
	}
}
void CFormView3::OnCurveShowcontrolpoints()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSCURVE3D)
	{
		CNurbsCurve* pNurbs = (CNurbsCurve*)pDoc->m_SceneGraph.GetAt(currentindex);
		pNurbs->m_showControlPoints = !pNurbs->m_showControlPoints;
	}

	pDoc->UpdateAllViews(NULL);
}

void CFormView3::OnUpdateCurveShowcontrolpoints(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSCURVE3D)
	{
		CNurbsCurve* pNurbs = (CNurbsCurve*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pNurbs->m_showControlPoints);
	}
}

void CFormView3::OnSurfaceComputeconformalenergy()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		double energy = ComputeConformalEnergy(pSurface);
		char message[80];
		sprintf(message, "The Energy is %5.6f", energy);
		AfxMessageBox(message);
	}
}

void CFormView3::OnActionsKillcurrentt()
{
	// TODO: Add your command handler code here
	::TerminateThread(g_pThread, 0);
}
void CFormView3::UpdateReadTree()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	CTreeCtrl* pTreeControl = (CTreeCtrl*)objTab.GetDlgItem(IDC_Objects);
	pTreeControl->DeleteAllItems();
	m_ArraySurfaceItems.clear();
	//char name[200];
	for (int i = 0; i < num; i++)
	{
		int type = pDoc->m_SceneGraph.GetAt(i)->GetType();
		if (type == TYPE_MESH3D)
		{
			CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(i);
			CString cname = pMesh3D->GetName();
			HTREEITEM item = pTreeControl->InsertItem(cname, NULL, NULL);
			m_ArraySurfaceItems.push_back(item);
			pTreeControl->SetCheck(item, pMesh3D->m_Show);
		}
	}
}
#define MAX_CHAR        128
void CFormView3::drawOpenglString(double x, double y, double z, const char* str)
{
	//glPushMatrix();
	//glLoadIdentity();
	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos3f(x, y, z);
	for (; *str != '\0'; ++str)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *str);
	//glPopMatrix();
}
void CFormView3::OnMeshShowTargetFaceArea()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showTargetFaceArea = !pMesh3D->m_showTargetFaceArea;
	}

	UpdateShowView();
}
void CFormView3::OnUpdateMeshShowTargetFaceArea(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showTargetFaceArea);
	}
}
void CFormView3::OnMeshShowDifferenceFaceArea()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showDifferenceFaceArea = !pMesh3D->m_showDifferenceFaceArea;
	}

	UpdateShowView();
}
void CFormView3::OnUpdateMeshShowDifferenceFaceArea(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showDifferenceFaceArea);
	}
}
void CFormView3::OnMeshShowFaceXYIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showFaceXYIndex = !pMesh3D->m_showFaceXYIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowCellXYIndex()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showCellXYIndex = !pMesh3D->m_showCellXYIndex;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowDistrictIndex()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showDistrictIndex = !pMesh3D->m_showDistrictIndex;
	}

	UpdateShowView();
}

void CFormView3::OnMeshShowStateName()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showStateName = !pMesh3D->m_showStateName;
	}

	UpdateShowView();
}
void CFormView3::OnMeshShowJianZhi()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showJianZhi = !pMesh3D->m_showJianZhi;
	}

	UpdateShowView();
}
void CFormView3::OnUpdateMeshShowJianZhi(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showJianZhi);
	}
}
void CFormView3::OnUpdateMeshShowDistrictIndex(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showDistrictIndex);
	}
}
void CFormView3::OnUpdateMeshShowStateName(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showStateName);
	}
}
void CFormView3::OnUpdateMeshShowFaceXYIndex(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showFaceXYIndex);
	}
}
void CFormView3::OnUpdateMeshShowCellXYIndex(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showCellXYIndex);
	}
}

void CFormView3::OnMeshShowmeshinformation()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		char message[280];
		sprintf(message, "Mesh: %d vertices, %d faces", pMesh3D->NbVertex(), pMesh3D->NbFace());
		AfxMessageBox(message);
	}
}


void CFormView3::OnMeshExtractparameterdomain()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CMesh3d* pMesh3D_new = new CMesh3d();
		pMesh3D_new->Copy(pMesh3D);

		for (int i = 0; i < pMesh3D->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3D->GetVertex(i);
			CVertex3d* pVertex1 = pMesh3D_new->GetVertex(i);

			pVertex1->Set(pVertex->u, pVertex->v, 0);
		}
		pDoc->m_SceneGraph.Add(pMesh3D_new);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
}


void CFormView3::OnMeshShowrectilineargrid()
{
	// TODO: Add your command handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showRectilinearGrid = !pMesh3D->m_showRectilinearGrid;
	}

	UpdateShowView();
}


void CFormView3::OnUpdateMeshShowrectilineargrid(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showRectilinearGrid);
	}
}

void CFormView3::OnUpdateMeshShowsharpedgebasenum(CCmdUI* pCmdUI)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pCmdUI->SetCheck(pMesh3D->m_showSharpEdgeBaseNum);
	}
}
void CFormView3::OnMeshShowsharpedgebasenum()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		pMesh3D->m_showSharpEdgeBaseNum = !pMesh3D->m_showSharpEdgeBaseNum;
	}

	UpdateShowView();
}
