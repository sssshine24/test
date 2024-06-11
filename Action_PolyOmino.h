#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ap.h"

// Action_PolyOmino dialog
class CMesh3d;
class GeoMapLandMark;
class CVertex3d;
class KDTree;
class MySparseMatrix;
class CMesh3d_OpenMesh;
class Shape_Polygon;
class countryinfo
{
public:
	int index;
	CString contryname;
	long population;
	std::vector<int> districtarray;
};
class Action_PolyOmino : public CPropertyPage
{
	DECLARE_DYNAMIC(Action_PolyOmino)

public:
	Action_PolyOmino();
	virtual ~Action_PolyOmino();

// Dialog Data
	enum { IDD = IDD_POLYOMINO };

public:
	Shape_Polygon* pPolygon;
	KDTree* pKdTree;
	std::vector<std::vector<int>> m_HorizontalSegments,m_VerticalSegments; //On a Same Horizontal line or a Vertical Line
	std::vector<std::vector<int>> m_HorizontalContinuousSegments,m_VerticalContinuousSegments;
	std::vector<countryinfo> m_ArrayCountryInfo;
	double horizontalscaleratio,verticalscaleratio;
	std::vector<std::vector<int>> m_ArrayPointConnectedSelectEdgeIndex;
	std::vector<int> m_VertexIndexArray;
	std::vector<int> m_EdgeLandmarkIndexArray;
	int  m_InitialIndex;

	//
	int m_LatestWorldMapIndex;
	int m_LatestLandmarkIndex;
	int m_LatestSelectedLandmarkIndex;

	//
	BOOL m_IsMergeHorizontal;
	int  m_MergeIndex;
	BOOL IsUniformGridChanged;

	BOOL m_SplitLandmarkFirstRun;
	BOOL m_SloppingedgesubdivisionFirstRun;
	BOOL m_MergeOneEdgeWithSlopingFirstRun;
	BOOL m_MergeSmallEdgesFirstRun;

	int m_SplitAngle;
	int m_MergeAngle;
	int m_EdgeSubdivisionNum;
	BOOL m_bSplitDirection;
	int m_initialfacenum;

	double maxPointDeviations;

	//deleted and insert edge array
	std::vector<int> m_DeletedEdgeArray;
	std::vector<std::vector<int>> m_InsertEdgeArray;
	std::vector<std::vector<int>> m_ConstraintsArray;
	std::vector<std::vector<int>> m_MergeConstraintsSave;

	//
	CMesh3d* pMesh;  //The current mesh
	CMesh3d* pFinalRectilinearMesh; //final rectilinear mesh
	CMesh3d* pUniformGridMesh; //Uniform Grid Mesh

	//
	void Initialization_KDTree_World();

	//
	void Initialization_KDTree_China();

	//
	void KdtreeWorld();

	//
	void KdtreeChina();

	//
	void MergeHorizontalVerticalSegments();

	//
	int IsSameLandmark(GeoMapLandMark* pLandmark1, GeoMapLandMark* pLandmark2);

	//
	void DeleteZeroLandmarks(CMesh3d* pMesh);

	//
	void SearchSharpEdgeWithEndPoints(std::vector<std::pair<int, int>> &m_ArraySharpEdges, std::vector<int> &newlandmark, std::vector<std::vector<std::pair<int, int>>>& m_CornerPointArray);

	//
	static void SolveSpareseLinearSystem_matlab_FAST(MySparseMatrix *A, std::vector<double> &B, std::vector<double> &X);

	//
	void SolveCNN_matlab();

	//
	double SolveCNNClassification_matlab_SelfTest(std::vector<std::vector<double>> &traindata, std::vector<int> &trainlabel);

	//
	void SaveMatrix(std::vector<std::vector<double>> &vec, int m1, int m2, int n, char* filename);

	//
	void Loadworldpopulation();

	//
	void DetectInvalidSharpEdges();

	//
	void AdjustHorizontalandVerticalGroupofLinePositions_3base();

	//
	void OnBnClickedButtonLoadNanjizhoumap_Triangulation();

	//
	void SaveCurrentRectilinearWorldMap();

	//
	void SubdivideEdgeSloppingVertical(CMesh3d* pMesh, int m_Edge_Subdivision_Num);

	//
	void SubdivideEdgesSloppingHorizontal(CMesh3d* pMesh, int m_Edge_Subdivision_Num);

	//
	void SubdivideMeshEdge(CMesh3d* pMesh, int m_Edge_Subdivision_Num);

	//
	void CurveStraight_Harmonic();

	//
	void CurveStraight_Holomorphic();

	//
	void GetLatestWorldMap();

	//
	void SaveCurrentWorldMap();

	//
	void GetLatestLandmarkIndex();

	//
	void GetLatestLandmark();

	//
	void SaveCurrentLandmak();

	//
	void GetLatestSelectedLandmark();

	//
	void SaveCurrentSelectedLandmark();

	//
	void GetLatestWorldMapIndex();

	//
	void GetLatestSelectedLandmarkIndex();

	//
	void OnlyKeepSelectedLandmarks(CMesh3d* pMesh, CMesh3d_OpenMesh* pOpenMesh);

	//
	int DeleteDuplicateLandmarks(CMesh3d* pMesh);

	//
	int DeleteDuplicateVertices_Mesh3d(CMesh3d* pMesh);

	//
	void SaveSelectedGeoMap();

	//
	void LoadSelectedSharpVertices(CString FileName);

	//
	void SaveSelectedSharpVertices(CString FileName);

	//
	void LoadSelectedLandmarks(CString FileName);

	//
	void SaveSelectedLandmarks(CString FileName);

	//
	void DeleteDuplicateSharpEdges();

	//
	void OnBnClickedButtonLoadworldmap_Triangulation();

	//
	CMesh3d* ExportMesh3D(Shape_Polygon* pPolygon);

	//
	void SetPolygonRingIndex(Shape_Polygon* pPolygon);

	//
	void ComputePolygonRingArea(Shape_Polygon* pPolygon);

	//
	void DeleteSmallPolygon(Shape_Polygon* pPolygon);

	//
	void LoadNanJingZhouRings();

	//
	void LoadDeleteRings();

	//
	void LoadShape_Polygon();

	//
	void AssignLandmarkToSharpEdges();

	//
	int DeleteDuplicateVertices_Poly();

	//
	void SetCurrentMesh(int ChangeMeshType_to_Mesh3D = 0);

	//
	void Compute_Boundary_Vertices_Parameter_Positions_TutteEmbedding_FixOuterBoundary(CMesh3d* pMesh3D, std::vector<int> &m_boundaryRingVertices, std::vector<std::pair<int, int>>& m_BoundaryVertices, std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex, std::vector<double> &boundaryedgelength, BOOL IsUniform);

	//
	void Initialization_KDTree1();

	//
	void SaveMatrix(MySparseMatrix* ma,char* filename);

	//
	static void SolveSpareseLinearSystem_matlab_quadprog(MySparseMatrix *A, std::vector<double> &B, MySparseMatrix *Acon, std::vector<double> &Bcon, std::vector<double> &X);

	//
	static void SolveSpareseLinearSystem_matlab(MySparseMatrix *A, std::vector<double> &B,std::vector<double> &X);

	//
	static void SolveCNNClassification_matlab(std::vector<std::vector<double>> &traindata, std::vector<int> &trainlabel, std::vector<std::vector<double>> &testdata, std::vector<int> &testlabel);

	//
	static void SolveSVMClassification_matlab(std::vector<std::vector<double>> data, std::vector<int> label);

	//
	static void SolveSVMClassification_matlab(std::vector<std::vector<double>> &traindata, std::vector<int> &trainlabel, std::vector<std::vector<double>> &testdata, std::vector<int> &testlabel);

	//
	static void SolveLinearSystem_matlab(std::vector<std::vector<double>> &A, std::vector<double> &B,std::vector<double> &X);

	//
	void Loadlandmark(CMesh3d_OpenMesh*pMesh, CString FileName);

	//
	void Loadlandmark(CMesh3d*pMesh, CString FileName);

	//
	void Initialization_KDTree_WorldMap();

	//
	void Initialization_KDTree();

	//
	void PointSearchTest();

	//
	void GetPointandEdgeCorrespondence_China();

	//
	void GetPointandEdgeCorrespondence();

	//
	void AdaptKdTreeEdges();

	//
	void ComputeKdTreeNodeDenotedAreas();

	//
	void ComputeKdTreeLeafLocatedStates();

	//
	double GetLargestPointDeviations_China();

	//
	double GetLargestPointDeviations();

	//
	void ConstructKdTreeFromPoints();

	//
	void ConstructKdTreeFromEdges_China();

	//
	void ConstructKdTreeFromEdges();

	//
	void SimplifyLandmarks();

	//
	void DetectandSplitSharpEdges(CMesh3d* pMesh1,std::vector<int> &m_ArrayOutputVerticesIndex);

	//
	void GeneratePSLGFile(std::vector<int>& m_ArrayOutputVerticesIndex,char* outputfile);

	//
	int EdgeInLandmark(CVertex3d* pVertex1,CVertex3d* pVertex2);

	//
	void GetFacesDifferenceMaximalandMinimalDensityPerCell_Array(double &maximaldensityeachcell,double &minimaldensityeachcell,double &maximalstatepopulation,double &minimalstatepopulation);

	//
	void ComputetheDifference(int index1,int index2);

	//
	void PlotFaceCellByDifference_Array(int colornum,double maximaldensityeachcell,double minimaldensityeachcell,double maximalstatepopulation,double minimalstatepopulation);

	//
	void PlotFaceCellsBySeveralColorsFromTopLeft_Array(int index,int colornum,double maximaldensityeachcell,double minimaldensityeachcell,double &maximalstatepopulation,double &minimalstatepopulation);

	//
	void LoadTargetFaceAreaArray(int index);

	//
	void LoadStateName(CString FileName);

	//
	void GetSplitEdgeEndPoints_JianZhi(std::vector<int> &edgeset,std::vector<double> &splitposition);

	//
	double GetConnectedEdgeEndPoints(std::vector<int> &edgeset,int &startvindex,int &endvindex);

	//
	int IsEdgesConnected(int edgeindex1,int edgeindex2);

	//
	void GetConnectedEdgeSet(std::vector<int> &edgeset,std::vector<int> &edgeset1);

	//
	void UniformgridProgessiveMeshOneIteration();

	//
	void SetStatesArray();

	//
	void PlotFaceCellsBySeveralColorsFromTopLeft(int colornum,double maximaldensityeachcell,double minimaldensityeachcell);

	//
	void PlotFaceCellsbyContinuousColor(double maximaldensityeachcell);

	//
	void PlotFaceCellsbySingleColorFromCenter(double maximaldensityeachcell);

	//
	void GetFacesMaximalandMinimalDensityPerCell_Array(double &maximaldensityeachcell,double &minimaldensityeachcell,double &maximalstatepopulation,double &minimalstatepopulation);

	//
	void GetStateMaximalandMinimalDensityPerCell_Array(double &maximaldensityeachcell, double &minimaldensityeachcell, double &maximalstatepopulation, double &minimalstatepopulation);

	//
	void GetFacesMaximalandMinimalDensityPerCell(double &maximaldensityeachcell,double &minimaldensityeachcell);

	//
	void AssignUniformCellstoFaces();

	//
	void ComputeFacesFourElementArray(std::vector<int> &m_ArrayFacePoints,std::vector<int> &m_ArrayFaceEdges);

	//
	void TraceFaces(std::vector<int> &m_ArrayFacePoints,std::vector<int> &m_ArrayFaceEdges);

	//
	void AdjustHorizontalandVerticalGroupofLinePositions(); 

	//
	int InDistrict(CMesh3d* pMesh,double x,double y);

	//
	void LoadTargetFaceArea(CString FileName);

	//
	void InsertSavedEdgeSet();

	//
	void UpdateMergeConstraints_Save(std::vector<int> edgeset,int newindex);

	//
	int SameCoordinate(CVertex3d* pVertex1,CVertex3d* pVertex2);

	//
	int InsertNewSelectedEdge_Save(std::pair<int,int> newedge,int index,int isVertical,int ischeck);

	//
	void LoadData1(CString FileName,int &iterationnum);

	//
	void UpdateMergeConstraints(std::vector<int> edgeset,int newindex);

	//
	BOOL EnlargeInterval(std::vector<double> &xinterval,std::vector<double> &yinterval,int minimalindex,BOOL isVertical);

	//
	BOOL HasMergeConstraints(int minimalindex,BOOL isVertical);

	//
	void MergeInterval(std::vector<double> &xinterval,std::vector<double> &yinterval,int minimalindex,BOOL isVertical);

	//
	void LoadMergeConstraints(CString filename);

	//
	void SaveMergeConstraints(CString filename);

	//
	void SaveMatrix(int **vec,int m,int n,char* filename);

	//
	void SaveMatrix(std::vector<std::vector<double>> &vec,int m,int n,char* filename);

	//
	void SaveMatrix(double **vec,int m,int n,char* filename);

	//
	void SaveVector(std::vector<int> vec, int num, char* filename);

	//
	void SaveVector(std::vector<double> vec,int num,char* filename);

	//
	void SaveVector(double *vec,int num,char* filename);

	//
	void LoadFaceArea(CString FileName);

	//
	void LinearOptimization(std::vector<double> &areaarray);

	//
	double ComputeFaceArea(CMesh3d* pMesh,std::vector<double> &areaarray);
	//static void ComputeDataGradient(const real_1d_array &x, double &func,  real_1d_array &grad, void *ptr);
	//
	double ComputeYGradient(std::vector<double> &xinterval,std::vector<double> &yinterval,int index);
	
	//
	 double ComputeXGradient(std::vector<double> &xinterval,std::vector<double> &yinterval,int index);

	//
	double ComputeFaceArea(std::vector<double> &xinterval,std::vector<double> &yinterval,std::vector<double> &areaarray);

	//
	int IsHorizontalEdge(int edgeindex);

	//
	int RemoveOuterFace(std::vector<std::vector<int>>& m_ArrayFacePointsArray,std::vector<std::vector<int>>& m_ArrayFaceEdgesArray);

	//
	int TraceFace(int edgeindex1,int direction,std::vector<std::vector<int>>& m_ArrayEdge_flags,std::vector<int>& m_ArrayPoints,std::vector<int>& m_ArrayEdges);

	//
	void LoadData(CString FileName,int &iterationnum);

	//
	int GetDirectionVector(int index,int direction,Vector3D& edgevec1);

	//
	int SetSharpEdgeFlag();

	//
	int GetConnectedDirectedSegment(int segmentindex,int vindex,int& nextvindex,int passingedge);

	//
	int GetConnectedUnDegenerateEdgeNum(int vindex);

	//
	int IsDegenerateEdge(int segmentindex);

	//
	int HandleIntersecting_AditionalFace_IdenticalPoint(int index, int isVertical, int ischeck,int edgeindex1,int edgeindex2,int vindex1,int vindex2,std::vector<int>& removeedgeset);

	//
	int OnSameEdge(int index1,int index2);

	//Obj
	int SaveObjMesh(int iterationnum);

	//
	int ComputeFaceboundingbox(std::vector<std::vector<int>>& m_ArrayFaceArray,int index,Point3D& lower,Point3D& upper);

	//
	int RemoveOuterFace(std::vector<std::vector<int>>& m_ArrayFaceArray);

	//
	int TraceFace(int edgeindex1,int direction,std::vector<std::vector<int>>& m_ArrayEdge_flags,std::vector<int>& m_ArrayPoints);

	//
	int CheckVerticalEdgeIntersection_EdgeArray(std::vector<int> edgeset,int i);

	//
	int CheckVerticalEdgeIntersection();

	//
	int CheckHorizontalEdgeIntersection();

	//
	int CheckHorizontalEdgeIntersection_EdgeArray(std::vector<int> edgeset,int i);

	//
	int DeleteEdgesWithDegreeoneEndPoint();

	//
	void InitializeHorizontalandVerticalSegmentArray();

	//
	void GenerateUniformGridMesh();

	//
	int HandleIntersecting_General(int index, int isVertical, int ischeck,int edgeindex1,int edgeindex2);

	//
	int HandleIntersecting_General_NOJudgement(int index, int isVertical, int ischeck,int edgeindex1,int edgeindex2);

	//
	int HandleIntersecting_AditionalFace(int index, int isVertical, int ischeck,int edgeindex1,int edgeindex2);

	//
	int DetectandHandleHorizontalIntersectingEdges_EdgeArray(std::vector<int> edgeset,int i);

	//
	int DetectandHandleVerticalIntersectingEdges_EdgeArray(std::vector<int> edgeset,int i);

	//
	int HandleIntersecting_innerholes(int index, int isVertical, int ischeck, int firstvindex,int nextvindex,int edgeindex1,int edgeindex2);

	//
	int HandleIntersecting_innerholes_onediretion(int index, int isVertical, int ischeck,int firstvindex,int nextvindex,int edgeindex1,int edgeindex2);

	//
	int GetConnectedDirectedSegmentArray(int edgeindex1,int edgeindex2,int firstvindex,int nextvindex,std::vector<int>& m_EdgeArray);

	//
	int GetConnectedDirectedSegment(int segmentindex,int vindex,int& nextvindex);

	//
	int JudgewhetherTwoedgesOntheSameFace(int edgeindex1,int edgeindex2);


	//
	int ComputeFaceNum();

	//
	void SaveDebugInformation(char *filename);

	//
	int DeleteDuplicateEdges();

	//
	void SaveLandmark(CString FileName);

	//
	int InsertNewSelectedEdge(std::pair<int,int> newedge,int index,int isVertical,int ischeck);

	//
	void UpdatePointConnectedSelectedEdgeIndex();

	//
	int DeleteInnerHoles();

	//
	int DeleteUnusedVertices();

	//
	int DeleteDuplicateVertices();

	void Compute_Linear_Equations_TutteEmbedding_ConvexSubdivision(CMesh3d* pMesh3D,std::vector<std::pair<int,int>>& m_BoundaryVertices,std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex,std::vector<std::pair<int,std::pair<double,double>>> &m_InteriorPointsArray,BOOL IsUniform);

	//
	void Harmonic_ConvexSubdivision(CMesh3d* pMesh3D,std::vector<int> &m_boundaryRingVertices,std::vector<std::pair<int,int>>& m_BoundaryVertices,std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex,std::vector<std::pair<int,std::pair<double,double>>> &m_InteriorPointsArray,BOOL IsUniform);

	//
	double ComputeEdgeLength(CMesh3d* pMesh3D,std::vector<int> &landmarkset);

	//
	void Compute_Boundary_Vertices_Parameter_Positions_TutteEmbedding(CMesh3d* pMesh3D,std::vector<int> &m_boundaryRingVertices,std::vector<std::pair<int,int>>& m_BoundaryVertices,std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex,std::vector<double> &boundaryedgelength,BOOL IsUniform);

	//
	void Compute_Linear_Equations_TutteEmbedding(CMesh3d* pMesh3D,std::vector<std::pair<int,int>>& m_BoundaryVertices,std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex,std::vector<std::pair<int,std::pair<double,double>>> &m_InteriorPointsArray,BOOL IsUniform);
	//
	int MergeConnectedSegments();

	//
	int DetectandHandleEdgeWithZeroLength();

	//
	void DetectandHandleVerticalIntersectingEdges();

	//
	void DetectandHandleHorizontalIntersectingEdges();

	//
	int MergeTwoSegments(int index,int index1,int index2,int IsHorizontal);

	//
	void IsValidMesh();

	//
	void RemoveEdgeSet(std::vector<int>& m_EdgeArray);

	//
	void CheckandFixTopologyValidation(int iterationnum);

	//
	int GetConnectedSegment_SameStyle(int segmentindex,int vindex,int& nextvindex);

	//
	int GetConnectedSegment(int segmentindex,int vindex,int& nextvindex);

	//
	int GetSegmentStyle(int index);

	//
	int SearchSamePositionVertices(CMesh3d* pMesh3d,std::vector<int> changedvertices,int index);

	//
	int	 UniformSimplificationOneIteration();

	//
	void Loadlandmark(CString FileName);

	//
	void LoadSelectedPoints(CString FileName);

	//
	void DetectIntersectLandmarks();

	//
	void DetectSelfIntersectLandmarks();

	//
	void MergeSeletedlandmarks();

	//
	void InsertLandMark(std::vector<int> &landmarkpoints,int style);

	//
	void GenerateLandMarks_KeepSharpEdgesConsistent();

	//
	void GenerateLandMarks();

	//
	void SplitLandmark();

	//
	void Sloppingedgesubdivision();

	//
	void MergeConnectedLandMarkswithSameStyle();

	//
	void MergeThreeConnectedLandmarks();

	//
	double GetLandMarkLength(std::vector<GeoMapLandMark*> &m_newlandmarks, int index);

	//
	BOOL MergeTwoLandmarks(std::vector<int> &landmark1,std::vector<int> &landmark2);

	//
	void MergeSmallEdges();

	//
	void DeleteVertexConnectedLandmarks();

	//TJunction Rectification
	void TJuctionRectification();

	//
	void MergeOneEdgeWithSloping();

	//
	void SaveGeoMap();

	//
	double GetAngleBetweenTwoLandmarks(int index);

	//
	void ExtendSmall();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedPolyominoExportlandmark();
	afx_msg void OnBnClickedGeneratelandmarks();
	afx_msg void OnBnClickedPolyominoRectification();
	afx_msg void OnBnClickedPolyominoMergeslope();
	afx_msg void OnBnClickedPolyominoDeletelandmark();
	afx_msg void OnBnClickedPolyominoMergesamestyle();
	afx_msg void OnBnClickedPolyominoThreeconnected();
	afx_msg void OnBnClickedPolyominoMergesmall();
	afx_msg void OnBnClickedPolyominoSavelandmark();
	afx_msg void OnBnClickedPolyominoExtendsmall();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedPolyominoCurvestraight();
	afx_msg void OnBnClickedOminoSetcurrentmesh();
	afx_msg void OnBnClickedOminoSplitlandmark();
	CSliderCtrl m_SplitAngleSlider;
	CButton m_SplitDirection;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSplitdirectioncheck1();
	CSliderCtrl m_OminoMergeAngle;
	afx_msg void OnBnClickedPolyominoSloppingedgesubdivision();
	CSliderCtrl m_EdgeSubdivision;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedPolyominoDeleteselectedlandmarks();
	afx_msg void OnBnClickedOminoLoadusamap();
	afx_msg void OnBnClickedPolyominoMergeseletedlandmarks();
	afx_msg void OnBnClickedPolyominoClearlandmarkselection();
	afx_msg void OnBnClickedPolyominoInverseseletedlandmarkstyle();
	afx_msg void OnBnClickedPolyominoSaveLandmark();
	afx_msg void OnBnClickedPolyominoLoadlandmark();
	afx_msg void OnBnClickedPolyominoSmoothlandmarks();
	afx_msg void OnBnClickedPolyominoSmoothselectedlandmarks();
	afx_msg void OnBnClickedPolyominoTriangulate();
	afx_msg void OnBnClickedPolyominoSplitlandmarks();
	afx_msg void OnBnClickedPolyominoClearvertexselection();
	afx_msg void OnBnClickedPolyominoCheckinvalidedges();
	afx_msg void OnBnClickedPolyominoAddlandmark();
	afx_msg void OnBnClickedPolyominoBackuplandmarks();
	afx_msg void OnBnClickedPolyominoSetauxillarylandmark();
	afx_msg void OnBnClickedPolyominoAuxillaryvisiable();
	CSliderCtrl m_MergeSmallLandmarkLength;
	CSliderCtrl m_MergeSmallLandmarkAngle;
	afx_msg void OnBnClickedPolyominoDeleteauxillarylandmarks();
	afx_msg void OnBnClickedPolyominoFixpoints();
	afx_msg void OnBnClickedPolyominoUnfixpoints();
	afx_msg void OnBnClickedPolyominoCopyfixpoints();
	afx_msg void OnBnClickedOminoLoadusastreighten();
	afx_msg void OnBnClickedOminoLoadGeorgiaStreighten();
	afx_msg void OnBnClickedUniformgridInitialization();
	afx_msg void OnBnClickedUniformgridProgessiveMeshIteration();
	afx_msg void OnBnClickedPolyominoShortestpath();
	afx_msg void OnBnClickedPolyominoSaveMLandmark();
  afx_msg void OnBnClickedPolyominoTuteembedding();
  afx_msg void OnBnClickedOminoLoaddebuginformation();
  afx_msg void OnBnClickedPolyominoAddvertexselection();
  afx_msg void OnBnClickedPolyominoAddedgeselection();
  afx_msg void OnBnClickedOminoDatadrivenmorph();
  afx_msg void OnBnClickedOminoDatadrivenmorphoptimization();
  afx_msg void OnBnClickedPolyominoSetArea();
  afx_msg void OnBnClickedOminoSavefacearea();
  afx_msg void OnBnClickedOminoLoadfacearea();
  afx_msg void OnBnClickedOminoAddmergeconstraints();
  afx_msg void OnBnClickedOminoSavemergeconstraints();
  afx_msg void OnBnClickedOminoLoadmergeconstraints();
  afx_msg void OnBnClickedOminoLoaddata();
  afx_msg void OnBnClickedOminoLoadgeorgiamap();
  afx_msg void OnBnClickedOminoUniformgrid();
  afx_msg void OnBnClickedOminoJianzhi();
  afx_msg void OnBnClickedPolyominoChecktriangulation();
  afx_msg void OnBnClickedOminoLoadillinoismap();
  afx_msg void OnBnClickedOminoLoadillinoisstreighten();
  afx_msg void OnBnClickedPolyominoInverselandmarkselection();
  afx_msg void OnBnClickedPolyominoInversealllandmarkStyle();
  afx_msg void OnBnClickedButtonKdtree();
  afx_msg void OnBnClickedButtonKdtreeIteration();
  afx_msg void OnBnClickedPolyominoOpenmesh();
  afx_msg void OnBnClickedButtonLoadworldmap();
  afx_msg void OnBnClickedButtonSavesimplifiedmfile();
  afx_msg void OnBnClickedButtonSaveselectedPoints();
  afx_msg void OnBnClickedButtonLoadselectedPoints();
  afx_msg void OnBnClickedPolyominoCurvestraightSelected();
  afx_msg void OnBnClickedButtonSubdivideEdge();
  afx_msg void OnBnClickedButtonSearchLandmark();
  afx_msg void OnBnClickedButtonSubdivideEdgeSloppingHorizontal();
  afx_msg void OnBnClickedButtonSubdivideEdgeSloppingVertical();
  afx_msg void OnBnClickedButtonSearchLandmark2();
  afx_msg void OnBnClickedPolyominoMeshFixvertices();
  afx_msg void OnBnClickedPolyominoMeshFixvertices2();
  afx_msg void OnBnClickedPolyominoMeshFixvertices3();
  afx_msg void OnBnClickedPolyominoMeshFixvertices4();
  afx_msg void OnBnClickedPolyominoMeshSharpevertexSave();
  afx_msg void OnBnClickedPolyominoMeshSharpevertexLoad();
  afx_msg void OnBnClickedButtonSelectlandmarksTwopoints();
  afx_msg void OnBnClickedButtonSaveselectedlandmark();
  afx_msg void OnBnClickedButtonLoadselectedlandmark();
  afx_msg void OnBnClickedButtonAddlandmarks();
  afx_msg void OnBnClickedButtonOnepointlandmark();
  afx_msg void OnBnClickedButtonDetectOpenlandmark();
  afx_msg void OnBnClickedButtonDeleteDuplicateLandmark();
  afx_msg void OnBnClickedButtonSubdividesharpeedge();
};
