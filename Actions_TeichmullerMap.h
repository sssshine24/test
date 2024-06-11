#pragma once


// Actions_TeichmullerMap dialog
class CMesh3d_OpenMesh;
class Actions_TeichmullerMap : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_TeichmullerMap)

public:
	Actions_TeichmullerMap();
	virtual ~Actions_TeichmullerMap();

// Dialog Data
	enum { IDD = IDD_TeichmullerMap };

public:
	//
	void Compute_Linear_Equations_Registration(CMesh3d_OpenMesh* pMesh3D);

	//
	void ScaleMu(CMesh3d_OpenMesh* pMesh3D, double scale);

	//
	static void IncreaseItem(std::vector<std::pair<int,double>> &m_ArrayElements,int index,double val);

	//
	static void Compute_Linear_Equations_PerVertex(CMesh3d_OpenMesh* pMesh3D,OpenMesh::VertexHandle vh,int rowindex,MySparseMatrix &A,double &bval, int ii);

	//
	static void Intilization_Linear_Equation(CMesh3d_OpenMesh* pMesh3D,int &m_interior_vertices_num);

	//
	static double SearchForOptimalStep_MultiThread(CMesh3d_OpenMesh* pMesh3D);

	//
	static UINT SearchForOptimalStep_Iteration(LPVOID lpParam);

	//
	CMesh3d_OpenMesh* ComputeOptimalTeichurmullerMap(CMesh3d_OpenMesh* pOpenMesh,char* filename);

	//
	static void ModifyFaceMubyStep(CMesh3d_OpenMesh* pMesh3D,double step);

	//
	static double SearchForOptimalStep(CMesh3d_OpenMesh* pMesh3D);

	//
	static void SetZeroMu(CMesh3d_OpenMesh* pMesh3D);

	//
	void LoadConstraints(CMesh3d_OpenMesh* pMesh3D,char* filename);

	//
	void LoadConstraints();

	//
	void SetMeshMu(CMesh3d_OpenMesh* pMesh3D,double mumou);

	//
	static BOOL IsFaceFixed(CMesh3d_OpenMesh* pMesh3D,OpenMesh::FaceHandle fh);

	//
	static double GetAverageMeshMuModule(CMesh3d_OpenMesh* pMesh3D);

	//
	void InitilizeVertexUV(CMesh3d_OpenMesh* pMesh3D);

	//
	void CheckBoundaryConditions(CMesh3d_OpenMesh* pMesh3D);

	//
	static void GetMaxMinmuLength(CMesh3d_OpenMesh* pMesh3D,double &maxmulength,double &minmulength,double &averagemulength,double &standarddeviation);

	//
	void AverageMeshMu(CMesh3d_OpenMesh* pMesh3D,double step = 0.1);

	//
	void LaplaceSmoothMeshMu(CMesh3d_OpenMesh* pMesh3D);

	//
	static void ComputeMu(CMesh3d_OpenMesh* pMesh3D);

	//
	void UpdateMeshMu(CMesh3d_OpenMesh* pMesh3D);

	//
	void CheckEquations(CMesh3d_OpenMesh* pMesh3D);

	//
	void InitilizeFaceMU(CMesh3d_OpenMesh* pMesh3D);

	//
	void Compute_Boundary_Vertices_Parameter_Positions(CMesh3d_OpenMesh* pMesh3D);

	//
	CMesh3d_OpenMesh* GenerateParameterDomainMesh(CMesh3d_OpenMesh* pMesh3D);

	//
	void AddConstraints(CMesh3d_OpenMesh* pMesh3D);

	//
	static void Compute_Linear_Equations(CMesh3d_OpenMesh* pMesh3D);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTeichmuller();
};
