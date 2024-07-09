#if !defined(AFX_FORMVIEW3_H__CB97F4D2_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
#define AFX_FORMVIEW3_H__CB97F4D2_1752_11D2_B134_00C04FB9CA2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FormView3.h : header file
//
#include "ColorFormView.h"
#include <vector>
#include <math.h>
/////////////////////////////////////////////////////////////////////////////
// CFormView3 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "lib3d/mesh3d.h"
#include "afxcmn.h"
#include "ap.h"
#include "TransformationTab.h"
#include "ObjectViewer.h"
#include "MobiusTab.h"
#include "BilinearTab.h"
#include "QuadraticTab.h"
#include "GeneralBilinearTab.h"
#include "ActionsTab.h"
class C_NurbsSurf;
class CNurbsSuface;
class C_NurbsCurve3D;
using namespace alglib;
class CFormView3 : public CColorFormView
{
protected:
	CFormView3();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormView3)

// Form Data
public:
	BOOL m_bInitialized;
	CTabCtrl m_tab;
	ObjectViewer objTab;
	TransformationTab transformationTab;
	ActionsTab actionTab;

	BOOL m_bDragging;
	GLuint m_nHits;
	BOOL m_bPicking;
	GLdouble m_dDragStart[2];
	GLint m_viewPort [4];
	GLuint m_selectBuf [512];
	double m_dMatrix[16];

	int currentindex; //tree index

	//{{AFX_DATA(CFormView3)
	enum { IDD = IDD_FORMVIEW3 };

	//}}AFX_DATA

// Attributes
public:
	double type;
    CBrush* m_pEditBkBrush;
	COLORREF m_crGreen;
	HGLRC m_hRC;

	int     m_KnotPosition;
	CString m_ComboSelectContent;
	int     m_ComboSelectIndex;

	//std::vector<HTREEITEM> m_ArraySurfaceItems;
	//std::map<HTREEITEM, CObject3d*> m_ArrayTreeItemObjectMap;
	CComboBoxEx m_SelectComboBox;

/*//优化的工具类
public :
	OrthogonalityEnergy OrhtEnergy;*/
// Operations
public:
	//
	void GenerateHalfSphere();

	//
	void GenerateSphereParaterdomain(CMesh3d* pMesh);

	//
	void GeneratetThreeMesh_PovRay();

	//
	void OutPutSharpVertices_PovRay_Anchor(CMesh3d* pMesh, char* filename);

	//
	void OutPutMeshEdges_PovRay_Anchor(CMesh3d* pMesh, char* filename);

	//
	void OutPutMeshFaces_PovRay_Smooth_Anchor(CMesh3d* pMesh, char* filename);

	//
	void OutPutMeshFaces_PovRay_Smooth_flags(CMesh3d* pMesh, char* filename);

	//
	void AllNumbersAddOne();

	//
	void Outputtestpuzzle(CMesh3d* pMesh);

	//
	void GenerateMatrix1();

	//
	void GenerateMatrix();

	//
	void CheckIsolatedVertices(CMesh3d* pMesh);

	//
	void SetMeshTextureColor(CMesh3d* pMesh);

	//
	void RotateMesh(CMesh3d* pMesh);

	//
	void OnUpdateMeshShowFaceIndex(CCmdUI *pCmdUI);

	//
	void OnMeshShowFaceIndex();

	//
	void OutPutCircleTessellations();

	//
	void OutPutMesh_Dotted_OneEdge_PovRay(double u0, double v0, double u1, double v1, int samples, char* filename, char *colortex);

	//
	void OutPutLandmarkEdges_PovRay_1(CMesh3d* pMesh, char* filename);

	//
	void SetBunnyGeneralTransformation2();

	//
	void SetBunnyGeneralTransformation1();

	//
	void RemoveDuplicatePoints(std::vector<double>& edgepointsarray);

	//
	void OnCDT_PSLG_POINTS(std::vector<double> &m_ArrayFaceTessellationPointscoordiantes, std::vector<double> &edgepointsarray);

	//
	void TessellateSurface_Kdtree(CNurbsSuface* pNurbsSurface, double facetolerance, std::vector<double> &m_ArrayFaceTessellationPointscoordiantes);

	//
	void TessellateEdge(CNurbsSuface* pNurbsSurface, OpenMesh::Vec3d v1, OpenMesh::Vec3d v2, Vector2D pv1, Vector2D pv2, Vector2D pv3, Vector2D pv4, std::vector<Vector3D>  &m_ArrayEdgeTessellationPoints, double edgetolerance, std::vector<Vector2D> &m_ArrayEdge2DPoints, std::vector<Vector2D> &m_ArrayEdge2DPoints1);

	//
	Vector2D GetSurfaceParameter(OpenMesh::Vec3d v1, CNurbsSuface* pNurbsSurface);

	//
	void TessellateBunnySurfaces();

	//
	void OutPutSharpVertices_PovRay2(CMesh3d* pMesh, char* filename);

	//
	void ChangeColors();

	//
	void GetCorrespondingLocation();

	//
	void OutPutMeshFaces_PovRay_Smooth_nonflip(CMesh3d_OpenMesh* pMesh);

	//
	double ComputeMinimalAngle();

	//
	void ProjectTriangulationOnSurfaces();

	//
	void OnCDT_PSLG();

	//
	void AssignLandmark();

	//
	void ExportConstraints();

	//
	void SolveLinearSystem_matlab(std::vector<std::vector<double>> &A, std::vector<double> &B,std::vector<double> &X);

	//
	std::vector<double> MatrixVectorMultiplication(std::vector<std::vector<double>> &A, std::vector<double> &B);

	//
	void OutPutPovrayPuzzle(CMesh3d* pMesh);

	//
	void OnUpdateMeshShowStateName(CCmdUI *pCmdUI);

	//
	void OnMeshShowStateName();

	//
	void OutPutMeshPolyOminoEdges_PovRay(CMesh3d* pMesh, char* filename);

	//
	void OutPutSharpVertices_PovRay1(CMesh3d* pMesh, char* filename);

	//
	void OutPutLandmarkEdges_PovRay(CMesh3d* pMesh, char* filename);

	//
	void ExportPolyomino(CMesh3d* pMesh, char* filename);

	//
	void MatlabTest();

	//
	void drawOpenglString(double x,double y, double z,const char* str) ;

	//Save As
	void OnTsplineSaveAs();

	//
	void ExtractSurface_Sub();

	void OutPutMeshVertices_PovRay(CMesh3d* pMesh, char* filename);

	void ExPortGraph_Line_SelectedEdges(CMesh3d* pMesh, char* filename);

	void ExPortGraph_Line();

	//export the graph for wei zeng
	void ExPortGraph();

	//
	void SetBunnyGeneralTransformation();

	//
	void ExtendSurface(CNurbsSuface* pNSurf);

	//
	void UpdateReadTree();

	//Update Right View
	void UpdateShowView();

	//
	void OutPutMeshFaces_PovRay(int samplesm,int samplesn,char* filename);

	//
	void OutPutMeshFaces_PovRay(CNurbsSuface* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//
	double ComputeEquiareality_NumericalMethod(C_NurbsSurf* pSurface);

	//
	void SoomthFreeformsSurface(CNurbsSuface* pSurface);

	//
	void OutPutCompositeEnergyMesh(std::vector<std::vector<std::vector<double>>> &coefficientarray,char* filename);

	//
	void OutPutMu_PovRay(CMesh3d* pMesh,char* filename);

	//
	void TrimmingFreeformSurface();

	//for the bone model
	void outPutMeshBoundaryEdges_PovRay_bone(CMesh3d* pMesh, char* filename);

	//
	void OutPutSurfaceBoundaryEdges_PovRay(char* filename);

	//
	void outPutMeshBoundaryEdges_PovRay(CMesh3d* pMesh, char* filename);

	//
	void OutPutMeshFaces_PovRay_Smooth(CMesh3d* pMesh, char* filename);

	void ExtractSurface();
	//
	void ComputeBoundaryRatio();

	//Get a sub Nurbs Surface 
	void GenerateNewSurface(CNurbsSuface* pSurface);

	//output Quad Mesh
	void OutPutQuadMeshEdges_PovRay(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//Generate bilinear transformation
	void GenerateBilinearTransformation();

	void SolveLinearSystem(std::vector<std::vector<double>> &A, std::vector<double> &B,std::vector<double> &X);

	void GenerateGeneralBilinearSurface(C_NurbsSurf* pNurbsSurf);

	//output the general bilinear information
	void OnGeneralBilinearTransformation();

	//Generate Quadratic Surface from Bilinear Surface
	void GenerateQuadraticSurfaceFromBilinearSurface(C_NurbsSurf* pSurf);

	//modify the optimal parameterization
	void SetFaceModification();

	//
	void OnTvnItemChangedObjects(NMHDR *pNMHDR, LRESULT *pResult);

	void UpdateTreeControl();

	void StartPicking(float xWin, float yWin) ;

	void StopPicking();

	void DrawScene(CDC* pdc, GLenum mode);

	void ProcessHits();

	static double Factorial(double n, double i);

	void AddItemToListView(UINT nRecordIndex);

	void ClearForm();

	void SetFace2Modification();

	//Read View Point from File
	void ReadViewPointFromFile(char* viewfile);

	//surface uv control number
	int uConNum,vConNum;

	//set the constraints 
	void setLinearConstraint(int uConNum,int vConNum,const std::vector<std::vector<Vector3D>> &pArray,std::string &staments,std::string &relation);

	//OPtimization Test for B-spline Surface 3X3
	void OptimizationTestBSpline33(C_NurbsSurf* pSurf);

	//Optimization Test for surface of degree 1X1
	void OptimizationTest11(C_NurbsSurf* pSurf);

	//Optimization Test for surface of degree 2X2
	void OptimizationTest22(C_NurbsSurf* pSurf);

	//Optimization Test for surface of degree 3X3
	void OptimizationTest33(C_NurbsSurf* pSurf,int i,int j);

	//output view point config file
	void OutPutViewPointConfigFile(CObject3d* pSurface,char* filename);

	//output light config file
	void OutPutLightConfigFile(CObject3d* pSurface,char* filename);

	static double Factorial(int n);

	//Vase Modification
	void SetVaseModification();

	//Bone Modification
	void SetBoneModification();

	void OutPutConformity_PovRay(int samplesm,int samplesn,char* filename);

	//output the conformity
	void OutPutConformity_PovRay(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//output the M file
	void OutPutMMesh(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename,BOOL parameterdomain = FALSE);

	//output the optimal modified texture.
	void OutPutTexture_Optimization_Moddified_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//output the optimal texture.
	void OutPutTexture_Optimization_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//output texture.
	void OutPutTexture_PovRay(CNurbsSuface* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//output texture.
	void OutPutMeshTexture_PovRay(CMesh3d* pMesh,char* filename);

	//output mesh save modified edges
	void OutPutMeshSavedEdgesModifided_PovRay(CNurbsSuface* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//output mesh save edges
	void OutPutMeshSavedEdges_PovRay(CNurbsSuface* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//output obj files for the transformed iso-parameter curves.
	void OutPutGeneralBilinearTransformedEdges_obj(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//output transformed iso-parameter curves.
	void OutPutGeneralBilinearTransformedEdges_PovRay(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//output mesh iso-parameter curves.
	void OutPutMeshEdges_PovRay(CNurbsSuface* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//output mesh quad edges for CMesh3d
	void OutPutMeshQuadEdges_PovRay(CMesh3d* pMesh, char* filename);

	//
	void OutPutSharpVertices_PovRay(CMesh3d* pMesh, char* filename);

	//
	void OutPutSharpEdges_PovRay(CMesh3d* pMesh, char* filename);

	//output mesh edges for CMesh3d
	void OutPutMeshEdges_PovRay(CMesh3d* pMesh, char* filename);

	//output four points on the surface
	void OutputSpecificPointsOntheSurface(C_NurbsSurf* pNurbsSurface, char* filename);

	//output povray for end vertexes
	void OutPutChainEndVertexs_PovRay(CMesh3d* pMesh, char* filename);

	//output one povray edge
	void OutPutMesh_OneEdge_PovRay(CNurbsSuface* pSurface, double u0,double v0,double u1,double v1,int samples,char* filename,char *colortex);

	//output povray chain edges
	void OutPutChainEdges_PovRay(CMesh3d* pMesh, char* filename);

	//output the povray mesh file for NURBS surface
	void OutPutMeshFaces_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//output the povray mesh file for Mesh surface
	void OutPutMeshFaces_PovRay(CMesh3d* pMesh, char* filename);

	//
	void OutputSubObjMesh(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename,BOOL parameterdomain = FALSE);

	//Output the obj files 
	void OutPutObjMesh(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename,BOOL parameterdomain = FALSE);

	//Output the stl files
	void OutPutSTLMesh(char* filename,BOOL parameterdomain = FALSE);

	//Compute the v value for the given area
	double Compute_V_Value_GivenArea(C_NurbsSurf* pNurbsSurf,double area);

	//Compute the u value for the given area
	double Compute_U_Value_GivenArea(C_NurbsSurf* pNurbsSurf,double area);

	//Compute the area from (startu,startv)->(endu,endv) using the numerical method
	double Compute_Area(C_NurbsSurf* pNurbsSurf,double startu,double startv,double endu,double endv);

	//Compute the area from (0,0)->(u,v) using the numerical method
	double Compute_Area(C_NurbsSurf* pNurbsSurf,double u,double v);

	//Compute the area from (0,0)->(u,v) with the sampling density m*n
	double Compute_Area_Sampling(C_NurbsSurf* pNurbsSurf,double u,double v,int m, int n);

	//Compute the cij using numerical method for face surface
	double Compute_Coefficients_Cij_NumericalMethod_FaceSurface(CNurbsSuface* pNurbsSurf,int i,int j);

	//compute the coefficients for area preserving
	double Compute_Coefficients_Cij(C_NurbsSurf* pNurbsSurf,int i,int j);

	//compute the coefficients for area preserving by numerical method
	double Compute_Coefficients_Cij_NumericalMethod(C_NurbsSurf* pNurbsSurf,int i,int j);

	//compute the coefficients for given sampling for face surface
	double Compute_Coefficients_Cij_NumericalMethod_FixedSampling_FaceSurface(CNurbsSuface* pNurbsSurf,int i,int j,int m,int n);

	//compute the coefficients for given sampling
	double Compute_Coefficients_Cij_NumericalMethod_FixedSampling(C_NurbsSurf* pNurbsSurf,int i,int j,int m,int n);

	//optimal mobius transformation.
	void Compute_Optimal_Mobiustransformation(C_NurbsSurf* pNurbsSurf);

	//Optimal Area Preserving Iso Parameter Curves.
	void Compute_Optimal_AreaPreserving_IsoParameterCurves_NumericalMethod(C_NurbsSurf* pNurbsSurf);

	//compute the optimal mobius transformation for the face surface (special)
	void Compute_Optimal_Mobiustransformation_NumericalMethod_FaceSurface(CNurbsSuface* pNurbsSurf);

	//optimal mobius transformation by numerical method.
	void Compute_Optimal_Mobiustransformation_NumericalMethod(C_NurbsSurf* pNurbsSurf);

	//compute the area factor for saved modified mesh
	void OutPutAreaFactor_Savedmodified_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//compute the area factor for saved  mesh
	void OutPutAreaFactor_Saved_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//Check Area Preserving
	void CheckAreaPreserving(C_NurbsSurf* pNurbsSurface);

	//output the area-preserving energy
	void OutPutAreaFactor_PovRay(C_NurbsSurf* pNurbsSurface, int samplesm,int samplesn,char* filename);

	//output the control network
	void OutPutControlNetwork_PovRay(C_NurbsSurf* pNurbsSurface, char* filename);

	//output the points of the control network
	void OutPutControlPoints_PovRay(C_NurbsSurf* pNurbsSurface, char* filename);

	//Read BSF file format from yukun lai
	void ReadBSFFormat();

	//Bilinear Optimization 
	void BilinearOptimizationTest();

	//Get Fitting Error
	void GetBsplineFittingError();

	//Sampling Mesh
	void SamplingMesh();

	Vector3D GetSurfaceQVector(C_NurbsSurf* pNurbsSurf,int i,int j);

	Vector3D GetSurfaceRVector(C_NurbsSurf* pNurbsSurf,int i,int j);

	double ComputeFixPoints(double c00,double c01,double c02,double c10,double c12,double c20,double c21,double c22,double b,double &a);

	void OutPutEnergyMesh(double c00,double c01,double c02,
		double c10,double c11,double c12,
		double c20,double c21,double c22,
		char* filename);

	void OnInitialUpdate();

	void UpdateSliderPosition();

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	void SetCartopModification();

	void ReadPoints(CMesh3d* pMesh);

	void ModifyMesh(CMesh3d* pMesh);

	double Compute_F(double c00,double c01,double c02,
		double c10,double c11,double c12,
		double c20,double c21,double c22,double m,double n);

	double Compute_F_1(double c00,double c01,double c02,
		double c10,double c11,double c12,
		double c20,double c21,double c22,double a,double b);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormView3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormView3();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormView3)
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSurfaceDelete();
	afx_msg void OnShowSurfaceParameter();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedZoomall();
	afx_msg void OnBnClickedDeleteAllObjects();
	afx_msg void OnBnClickedCheckareapreserving();
	afx_msg void OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedAreapreserve();
	afx_msg void OnNMThemeChangedSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedShowconvexfunction();
	afx_msg void OnNMThemeChangedMobiusAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedMobiusBeta(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedShowcoordinateaxis();
	afx_msg void OnShowOnlyMe();
	afx_msg void OnSurfaceShowIsoParameterCurves();
	afx_msg void OnSurfaceShowIsoParameterCurvesGeneralBilinear();
	afx_msg void OnSurfaceShowIsoLines();
	afx_msg void OnSurfaceShowIsoParameterCurves_mobius();
	afx_msg void OnSurfaceShowIsoParameterCurves_mb();
	afx_msg void OnSurfaceShowIsoParameterCurves_mc();
	afx_msg void OnSurfaceShowColorBoundary();
	afx_msg void OnUpdateSurfaceTransformationofmobius(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceISOlines(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceTransformationofmobiusandbilinear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceTransformationofmobiusandcubic(CCmdUI *pCmdUI);
	afx_msg void OnSurfaceShowOptimalIsoParameterCurves();
	afx_msg void OnUpdateSurfaceShowOptimalIsoParameterCurves(CCmdUI *pCmdUI);
	afx_msg void OnSurfaceShowSubdivisionCurves();
	afx_msg void OnUpdateSurfaceShowSubdivisionCurves(CCmdUI *pCmdUI);
	afx_msg void OnSurfaceShowSurface();
	afx_msg void OnSurfaceShowcontrolpointnetwork();
	afx_msg void OnUpdateSurfaceShowcontrolpointnetwork(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceShowcolorboundary(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceShowisoparametercurves(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceShowisoparametercurvesGeneralBilinear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceShowsubdivisioncurves(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceShowoptimalisoparametercurves(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceShowsubsurfacecoordinate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceShowsurface(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedAddknot();
	afx_msg void OnSurfaceShowsubsurfacecoordinate();
	afx_msg void OnSurfaceSubdivide();
	afx_msg void OnSurfaceShowbilinearcoefficients();
	afx_msg void OnUpdateSurfaceShowbilinearcoefficients(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedBilinearparameterize();
	afx_msg void OnBnClickedQuadraticparameterize();
	afx_msg void OnSurfaceShowsurfaceinformation();
	afx_msg void OnRenderSetrendermode();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton3_();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnSurfacePovray();
	afx_msg void OnSurfaceMesh3dNormalize();
	afx_msg void OnSurfaceNormalize();
	afx_msg void OnSurfaceSaveas();
	afx_msg void OnBnClickedProjecttoparameterdomain();
	afx_msg void OnBnClickedReadviewpoint();
	afx_msg void OnBnClickedFaceexample();
	afx_msg void OnBnClickedFaceexample2();
	afx_msg void OnBnClickedBezierexamplestep1();
	afx_msg void OnSaveastmfile();
	afx_msg void OnMeshSetColor();
	afx_msg void OnSaveasMeshMfile();
	afx_msg void OnBnClickedVenusStep1();
	afx_msg void OnBnClickedVenusStep2();
	afx_msg void OnBnClickedVenusStep3();
	afx_msg void OnBnClickedReadbsffile();
	afx_msg void OnSurfaceShowoptimalareapreserving();
	afx_msg void OnSurfaceShowoptimalareapreservingmodified();
	afx_msg void OnUpdateSurfaceShowoptimalareapreserving(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSurfaceShowoptimalareapreservingmodified(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedBonestep1();
	afx_msg void OnBnClickedBonestep2();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedFace2step1();
	afx_msg void OnBnClickedFace2step2();
	afx_msg void OnBnClickedMeshanimation();
	afx_msg void OnBnClickedFinalmesh();
	afx_msg void OnSurfaceShowcolororthogonality();
	afx_msg void OnUpdateSurfaceShowcolororthogonality(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedBsplineSurface();
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSurfaceShowtransformedparameterdomain();
	afx_msg void OnMeshGeneratequadmesh();
	afx_msg void OnSurfaceConverttotspline();
	afx_msg void OnTsplinePlotpreimage();
	afx_msg void OnMeshShowedge();
	afx_msg void OnMeshShowSharpEdge();
	afx_msg void OnPolygonShowRingEdge();
	afx_msg void OnPolygonDelete();
	afx_msg void OnPolygonShowInnerRingEdge();
	afx_msg void OnPolygonShowRingIndex();
	afx_msg void OnMeshShowSharpEdgeIndex();
	afx_msg void OnMeshShowLandmarkEdge();
	afx_msg void OnMeshShowSelectedLandmarkEdge();
	afx_msg void OnMeshShowLandmarkVertex();
	afx_msg void OnMeshShowface();
	afx_msg void OnUpdateMeshShowEdge(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowSharpedge(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePolygonShowRingEdge(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePolygonShowInnerRingEdge(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePolygonShowRingIndex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowlandmarkEdge(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowSelectedlandmarkEdge(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowlandmarkVertex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowface(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowvertex();
	afx_msg void OnMeshShowvertexIndex();
	afx_msg void OnUpdateMeshShowvertex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowSharpEdgeIndex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowvertexIndex(CCmdUI *pCmdUI);
	afx_msg void OnCurveShowcontrolpoints();
	afx_msg void OnUpdateCurveShowcontrolpoints(CCmdUI *pCmdUI);
	afx_msg void OnSurfaceComputeconformalenergy();
	afx_msg void OnActionsKillcurrentt();
	afx_msg void OnMeshShowSelectedSharpEdge();
	afx_msg void OnUpdateMeshShowSelectedSharpedge(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowSelectedvertex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowPointsOnSurface(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowSelectedEdge(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowSelectedVertexIndex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowSharpSelectedvertex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowSelectedface(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowPointsOnSurface();
	afx_msg void OnMeshShowSelectedvertex();
	afx_msg void OnMeshShowSelectedEdge();
	afx_msg void OnMeshShowSelectedVertexIndex();
	afx_msg void OnMeshShowSharpSelectedvertex();
	afx_msg void OnMeshShowSelectedface();
	afx_msg void OnMeshShowFaceArea();
	afx_msg void OnUpdateMeshShowFaceArea(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowTargetFaceArea();
	afx_msg void OnUpdateMeshShowTargetFaceArea(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowDifferenceFaceArea();
	afx_msg void OnUpdateMeshShowDifferenceFaceArea(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowFaceXYIndex();
	afx_msg void OnUpdateMeshShowFaceXYIndex(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowCellXYIndex();
	afx_msg void OnUpdateMeshShowCellXYIndex(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowLandmarkIndex();
	afx_msg void OnMeshShowLandmarkVertexIndex();
	afx_msg void OnUpdateMeshShowLandmarkVertexIndex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowLandmarkIndex(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowMergeConstraints();
	afx_msg void OnUpdateMeshShowMergeConstraints(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowUniformGrid(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowPuzzleGraph(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowUniformGrid();
	afx_msg void OnShowPuzzleGraph();
	afx_msg void OnMeshShowDistrictIndex();
	afx_msg void OnUpdateMeshShowDistrictIndex(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowJianZhi();
	afx_msg void OnUpdateMeshShowJianZhi(CCmdUI *pCmdUI);
	afx_msg void OnShowTriangleMU();
	afx_msg void OnShowVertexMU();
	afx_msg void OnUpdateShowVertexMU(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowTriangleMU(CCmdUI *pCmdUI);
	afx_msg void OnShowVertexMUVectorField();
	afx_msg void OnUpdateShowVertexMUVectorField(CCmdUI *pCmdUI);
	afx_msg void OnShowVertexDisplacement();
	afx_msg void OnUpdateShowVertexDisplacement(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowFeaturevertex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowFeaturevertexIndex(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowmeshinformation();
	afx_msg void OnMeshExtractparameterdomain();
	afx_msg void OnMeshSetCurrentMesh();
	afx_msg void OnMeshMenuZoomAll();
	afx_msg void OnMeshZoomCurrentMeshToFit();
	afx_msg void OnMeshOnlyMoveMe();
	afx_msg void OnMeshZoomAllToFitCurrentMesh();
	afx_msg void OnMeshShowFeaturevertex();
	afx_msg void OnMeshShowFeaturevertexIndex();
	afx_msg void OnMeshShowrectilineargrid();
	afx_msg void OnUpdateMeshShowrectilineargrid(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeshShowsharpedgebasenum(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowsharpedgebasenum();
	afx_msg void OnUpdateMeshShowedgeIndex(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowAnchorPonits();
	afx_msg void OnUpdateMeshShowAnchorPonits(CCmdUI *pCmdUI);
	afx_msg void OnMeshShowedgeIndex();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMVIEW3_H__CB97F4D2_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
