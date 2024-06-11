#pragma once


// Action_Levy_Leastsquare dialog
class CMesh3d_OpenMesh;
class Action_Levy_Leastsquare : public CPropertyPage
{
	DECLARE_DYNAMIC(Action_Levy_Leastsquare)

public:
	Action_Levy_Leastsquare();
	virtual ~Action_Levy_Leastsquare();

// Dialog Data
	enum { IDD = IDD_LEVY_LEASTSQUARE };
public:
	static double ComputeProjection(CMesh3d_OpenMesh* pMesh3D, OpenMesh::FaceHandle fh, OpenMesh::VertexHandle vh, int flag);

	//
	static double ComputeCoefficient(CMesh3d_OpenMesh* pMesh3D,OpenMesh::FaceHandle fh,OpenMesh::VertexHandle vh,int flag);

	//
	void Compute_Boundary_Vertices_Parameter_Positions(CMesh3d_OpenMesh* pMesh3D);

	//
	void Compute_Linear_Equations(CMesh3d_OpenMesh* pMesh3D);

	//
	void GenerateParameterDomain(CMesh3d_OpenMesh* pMesh3D);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLevyLeastSquare();
	virtual BOOL OnInitDialog();
};
