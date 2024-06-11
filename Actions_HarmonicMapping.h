#pragma once
#include "MySparseMatrix.h"
// Actions_HarmonicMapping dialog
#include "resource.h"
class CMesh3d_OpenMesh;
class Actions_HarmonicMapping : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_HarmonicMapping)

public:
	int iternum;
	Actions_HarmonicMapping();
	virtual ~Actions_HarmonicMapping();
	//
	int GetOneBoundaryVertex(CMesh3d_OpenMesh* pOpenMesh);

	//
	int IsValidSwapAdjacentTriangles(CMesh3d_OpenMesh* pMesh3D, int ehindex);

	//
	double GetAngle_ThreeVertices(CMesh3d_OpenMesh* pMesh3D, int vindex1, int vindex2, int vindex3);

	//
	Vector3D GetFaceNormal_THREEPOINTS(CMesh3d_OpenMesh* pMesh3D, int vh1, int vh2, int vh3);

	//
	Vector3D GetFaceNormal(CMesh3d_OpenMesh* pMesh3D, MyMesh::FaceHandle fh);

	//
	void HarmonicMappingLaplace(CMesh3d_OpenMesh* pOriginalMesh, CMesh3d_OpenMesh* pTargetMesh);

	//
	void RemoveLandmarksWithFixedVerticesandSetCorrespondingUVValue(CMesh3d_OpenMesh* pMesh3D, std::vector<std::vector<int>> &m_ArrayLandmarks, int ii);

	//
	void FindAdjacentFixPoints(CMesh3d_OpenMesh* pMesh3D, std::vector<std::vector<int>> &m_ArrayLandmarks, int findlandmarkindex, int vindex, std::vector<int> &m_AdjacentPointsArray);

	//
	void FixInnerPoints(CMesh3d_OpenMesh* pMesh3D);

	//
	void Compute_Linear_Equations_LinearConstraints_SingleVariable_Selected(CMesh3d_OpenMesh* pMesh3D, int muflag);

	//
	void Compute_Boundary_Vertices_Parameter_Positions_VerticalPoint(CMesh3d_OpenMesh* pMesh3D, int boundarytype);

	//
	void HarmonicStraightening_VerticalPoint(CMesh3d_OpenMesh* pOpenMesh, int boundarytype);

	//
	void Compute_Boundary_Vertices_Parameter_Positions_Circle_VerticalPoint(CMesh3d_OpenMesh* pMesh3D);

	//
	void TwoDHarmonicStraightening_FixLandmarkEndPoints_Sophie(CMesh3d_OpenMesh* pOpenMesh);

	//
	void Compute_Boundary_Vertices_Parameter_Positions_AdditionalFixPoints(CMesh3d_OpenMesh* pMesh3D);

	//
	void AddConstraints(CMesh3d_OpenMesh* pMesh3D);

	//
	CMesh3d_OpenMesh* GetMeshfreeofFlip_quasiconformal(CMesh3d_OpenMesh* pMesh3D, int iternum);

	//
	void Compute_Linear_Equations_WithStraighteningConstraints_FixEndPoints(CMesh3d_OpenMesh* pMesh3D);

	//
	void TwoDHarmonicStraightening_FixLandmarkEndPoints(CMesh3d_OpenMesh* pOpenMesh);

	//
	void HarmonicStraightening(CMesh3d_OpenMesh* pOpenMesh,int boundarytype);

	//
	void Compute_Linear_Equations_WithStraighteningConstraints(CMesh3d_OpenMesh* pMesh3D);

	//
	void ChangeVertexIndex();

	//
	static void SetBoundaryUV(CMesh3d_OpenMesh* pOpenMesh1);

	//
	static double GetEdgeLength(CMesh3d_OpenMesh* pOpenMesh1, int bindex1, int bindex3);

	//
	static BOOL IsFixed(CMesh3d_OpenMesh* pMesh3D, MyMesh::VertexHandle vh);

	//
	static BOOL IsFixed(CMesh3d_OpenMesh* pMesh3D, MyMesh::EdgeHandle eh);

	//
	static int  GetMeshInequalConstraintCount(CMesh3d_OpenMesh* pMesh3D);

	//
	static void ObtainConnectedLandmarks(CMesh3d_OpenMesh* pMesh3D, std::vector<std::vector<int>> &m_ArrayLandmarks, int ii);

	//
	static void ObtainConnectedLandmarks(CMesh3d_OpenMesh* pMesh3D, std::vector<std::vector<int>> &m_ArrayLandmarks);

	//
	static void Compute_Linear_Equations__LinearConstraints_PerVertex_SingleVariable(CMesh3d_OpenMesh* pMesh3D, OpenMesh::VertexHandle vh, int &rowindex, MySparseMatrix &A, int ii, std::vector<double> &b);

	//
	static void Compute_Linear_Equations_LinearConstraints_SingleVariable(CMesh3d_OpenMesh* pMesh3D, int muflag);

	//
	static void Compute_Constaints(CMesh3d_OpenMesh* pMesh3D, MySparseMatrix &Acon, std::vector<double>& bcon, int ii);

	//
	static int MeshUVLandmarkInvalid(CMesh3d_OpenMesh* pMesh3D);

	//
	int IsValidTriangle(CMesh3d_OpenMesh* pMesh3D, int facehandleindex);

	//
	static void CDT_Direct(CMesh3d_OpenMesh* pMesh3D);

	//
	static void InitializeMeshOriginalLocation(CMesh3d_OpenMesh* pMesh3D);

	//
	static void GenerateOriginalMesh_LinearConstaints(CMesh3d_OpenMesh* pMesh3D);

	//
	static void InitializeMeshInequalConstraintFlag(CMesh3d_OpenMesh* pMesh3D);

	//
	static void InitializeMeshFaceSelectedFlag(CMesh3d_OpenMesh* pMesh3D);

	//
	static void Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints_Test(CMesh3d_OpenMesh* pMesh3D);

	//
	static void ComputeDynamicHarmonic_LinearConstraints_Test(CMesh3d_OpenMesh* pMesh3D);

	//
	static double GetMaxLandmarkDeviation(CMesh3d_OpenMesh* pMesh3D);

	//
	static void CDT_New(CMesh3d_OpenMesh* pMesh3D);

	//
	int SwapAdjacentTriangles_Propogate(CMesh3d_OpenMesh* pMesh3D, int ehindex);

	//
	int SwapAdjacentTriangles(CMesh3d_OpenMesh* pMesh3D, int ehindex);

	//
	static int SwapAdjacentTriangles(CMesh3d_OpenMesh* pMesh3D,int ehindex,std::vector<int> &m_ArrayEdgeId);

	//
	static int IsDelaunay(CMesh3d_OpenMesh* pMesh3D,int ehindex);

	//
	double GetAdjacentTwoAnglesSum(CMesh3d_OpenMesh* pMesh3D, int ehindex, int vindex, double &angle1, double &angle2, int &ehindex1, int &ehindex2);

	//
	static void GetTwoOppositePoints(CMesh3d_OpenMesh* pMesh3D,int ehindex, std::vector<int> &oppsitevertices);

	//
	static int IsMeshUVFlip(CMesh3d_OpenMesh* pMesh3D, int iternum = 0, int showmessage = 0);

	//
	static int IsMeshOriginalFlip(CMesh3d_OpenMesh* pMesh3D);

	//
	static int IsMeshFlip(CMesh3d_OpenMesh* pMesh3D);

	//
	static CMesh3d_OpenMesh* GetMeshfreeofFlip(CMesh3d_OpenMesh* pMesh3D,int iternum);

	//
	static int Compute_DynamicHarmonic_Gradient(CMesh3d_OpenMesh* pMesh3D,OpenMesh::VertexHandle vh,std::vector<double> &m_Position,int ii,std::vector<std::pair<int,int>> &m_VerticalConstraints,std::vector<std::pair<int,int>> &m_HorizontalConstraints,int m_interior_vertices_num,double &derivativeval);

	//
	static void ComputeDynamicHarmonic_LinearConstraints(CMesh3d_OpenMesh* pMesh3D);

	//
	static void GeneratePSLGFile(CMesh3d_OpenMesh* pMesh3D,std::vector<int>& m_ArrayOutputVerticesIndex,char* outputfile);

	//
	static void CDT_EdgeFlip(CMesh3d_OpenMesh* pMesh3D,int iternum);

	//
	static CMesh3d_OpenMesh* CDT(CMesh3d_OpenMesh* pMesh3D,int iternum);

	//
	static int Subdivide_Triangle_Mulargerthan1(CMesh3d_OpenMesh* pMesh3D);

	//
	static void AverageNeibouringVertexMuVector_TwoRing(CMesh3d_OpenMesh* pMesh3D,std::vector<double> &vec,OpenMesh::VertexHandle vh);

	//
	static void SaveParameterDomainMesh(CMesh3d_OpenMesh* pMesh3D,char *filename);

	//
	static void GetMaxMinmuLength_Vertex(CMesh3d_OpenMesh* pMesh3D,double &maxmulength,double &minmulength,double &averagemulength,double &standarddeviation);

	//
	static void AverageNeibouringVertexMuVector_OneRing(CMesh3d_OpenMesh* pMesh3D,std::vector<double> &vec,OpenMesh::VertexHandle vh);

	//
	static void ModifyVertexMubyStep(CMesh3d_OpenMesh* pMesh3D,double step);

	//
	static void Compute_Linear_Equations__LinearConstraints_ComputeEdgeWeight(CMesh3d_OpenMesh* pMesh3D,int muflag);

	//
	static void Compute_Linear_Equations__LinearConstraints_Constraints(MySparseMatrix &A,std::vector<double> &b,int &rowindex,int ii,std::vector<std::pair<int,int>> &m_VerticalConstraints,std::vector<std::pair<int,int>> &m_HorizontalConstraints);

	//
	static void Compute_Linear_Equations__LinearConstraints_PerVertex(CMesh3d_OpenMesh* pMesh3D,OpenMesh::VertexHandle vh,int &rowindex,MySparseMatrix &A,double &bval, int ii,std::vector<std::pair<int,int>> &m_VerticalConstraints,std::vector<std::pair<int,int>> &m_HorizontalConstraints,int m_interior_vertices_num,std::vector<double> &b);

	//
	static void IncreaseItem(std::vector<std::pair<int,double>> &m_ArrayElements,int index,double val);

	//
	void GenerateIntermediateMesh_();

	//
	void SetMeshParameterUV(CMesh3d_OpenMesh* pMesh,CMesh3d_OpenMesh* pParameter);

	//
	void OptimizeUsingConstraints();

	//
	void GenerateIntermediateMesh();

	//
	static void LaplaceSmoothMeshMu(CMesh3d_OpenMesh* pMesh3D);

	//
	static void ModifyFaceMubyStep(CMesh3d_OpenMesh* pMesh3D,double step);

	//
	static double SearchForOptimalStep(CMesh3d_OpenMesh* pMesh3D);

	//
	static double ComputeAngleFromDistortedTriangle(OpenMesh::Vec3d v1,OpenMesh::Vec3d v2,OpenMesh::Vec3d v3,std::vector<double> &v1_mu,std::vector<double> &v2_mu,std::vector<double> &v3_mu);

	//
	static void InitilizeVertexUV(CMesh3d_OpenMesh* pMesh3D);

	//
	static void GetMaxMinmuLength(CMesh3d_OpenMesh* pMesh3D,double &maxmulength,double &minmulength,double &averagemulength,double &standarddeviation);

	//
	static void SetZeroMu(CMesh3d_OpenMesh* pMesh3D);

	//
	static void InitializeMeshMu(CMesh3d_OpenMesh* pMesh3D);

	//
	double ComputeNewEdgeLength_FromMu(OpenMesh::Vec3d v1,OpenMesh::Vec3d v2,std::vector<double> v1_mu,std::vector<double> v2_mu);

	//
	static BOOL IsFaceFixed(CMesh3d_OpenMesh* pMesh3D,OpenMesh::FaceHandle fh);

	//
	static double GetAverageMeshMuModule(CMesh3d_OpenMesh* pMesh3D);

	//
	static void AverageMeshMu(CMesh3d_OpenMesh* pMesh3D);

	//
	static double ComputeNewEdgeLength(double x, double y,double a,double b);

	//
	static void Compute_Mu(CMesh3d_OpenMesh* pMesh3D);

	//
	static void ComputeHarmonic_LinearConstraints(CMesh3d_OpenMesh* pMesh3D);

	//
	static void GenerateParameterDomain_LinearConstaints(CMesh3d_OpenMesh* pMesh3D, char* filename = NULL);

	//
	static void GenerateDuplicateMesh_LinearConstaints(CMesh3d_OpenMesh* pMesh3D, char* filename = NULL);

	//
	static void Compute_LinearConstraints_Dynamic_Harmonic(CMesh3d_OpenMesh* pMesh3D,int muflag);

	//
	static void Compute_Linear_Equations_LinearConstraints(CMesh3d_OpenMesh* pMesh3D,int muflag = 0);

	//
	static void Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(CMesh3d_OpenMesh* pMesh3D);

	//
	void Compute_Boundary_Vertices_Parameter_Positions_Circle(CMesh3d_OpenMesh* pMesh3D);

	//
	void Compute_Boundary_Vertices_Parameter_Positions_Quad(CMesh3d_OpenMesh* pMesh3D);

	//
	 void Compute_Boundary_Vertices_Parameter_Positions(CMesh3d_OpenMesh* pMesh3D, int boundarytype);

	//
	 void Compute_Linear_Equations(CMesh3d_OpenMesh* pMesh3D);

	//
	 void GenerateParameterDomain(CMesh3d_OpenMesh* pMesh3D, char* filename = NULL);

// Dialog Data
	enum { IDD = IDD_Harmonic };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedHarmonic();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLoadExamplewithSingulairty();
	afx_msg void OnBnClickedButtonLoadtest();
	afx_msg void OnBnClickedButtonDynamicHarmonic();
	afx_msg void OnBnClickedButtonDynamicharmonicInitialize();
	afx_msg void OnBnClickedButtonDynamicharmonicIteration();
	afx_msg void OnBnClickedButtonDynamicharmonicFreeofflip();
	afx_msg void OnBnClickedButtonMeshSwapsequence();
	afx_msg void OnBnClickedButtonMeshModifytopology();
	afx_msg void OnBnClickedButtonGenerategeogiageboundary();
	afx_msg void OnBnClickedButtonGenerategeogiagehybrid();
	afx_msg void OnBnClickedButtonStreightenlandmark();
	afx_msg void OnBnClickedButtonStreightenlandmark2();
	afx_msg void OnBnClickedButtonStreightenlandmark3();
	afx_msg void OnBnClickedButtonStreightenlandmark4();
	afx_msg void OnBnClickedButtonStreightenlandmark5();
	afx_msg void OnBnClickedButtonStreightenlandmark6();
	afx_msg void OnBnClickedButtonStreightenlandmark7();
	afx_msg void OnBnClickedButtonStreightenlandmark8();
	afx_msg void OnBnClickedButtonStreightenlandmark9();
	afx_msg void OnBnClickedButtonDynamicharmonic2();
	afx_msg void OnBnClickedButtonDynamicharmonic3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButtonUshape();
	afx_msg void OnBnClickedButtonUshapeTriagulated();
	afx_msg void OnBnClickedButtonHarmonicOriginal();
	afx_msg void OnBnClickedButtonFacemodel();
	afx_msg void OnBnClickedButtonHarmonicStraightening();
	afx_msg void OnBnClickedButtonExample1();
	afx_msg void OnBnClickedButtonHarmonicHvStraightening();
	afx_msg void OnBnClickedButtonExample2();
};
double Distance(OpenMesh::Vec3d v1, OpenMesh::Vec3d v2);