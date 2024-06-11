#pragma once
#include <afxdlgs.h>
class Action_Tooth :
	public CPropertyPage
{
public:
	Action_Tooth();
	~Action_Tooth();
	//
	void Meshsmooth(CMesh3d* pMesh);

	//
	int GetPointProjectionOnMesh(double x, double y, double z, CMesh3d* pMesh, CMesh3d_OpenMesh* pOpenMesh);

	//
	int GetProjectionOnEdge(CMesh3d* pMesh, int vindex1, int vindex2, int &sharpedgeindex, double &t1, double &t2);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonToothBoundarydetection();
	afx_msg void OnBnClickedButtonToothMeshsmooth();
	afx_msg void OnBnClickedButtonToothLoadpath();
	afx_msg void OnBnClickedButtonToothDeletesharpedge();
	afx_msg void OnBnClickedButtonToothAddsharpedge2();
	afx_msg void OnBnClickedButtonToothClearsharpedgeselection();
	afx_msg void OnBnClickedButtonToothSavepath();
	afx_msg void OnBnClickedButtonToothClearsharpvertexselectio();
	afx_msg void OnBnClickedButtonToothDeletesharpedgeTwopoints();
	afx_msg void OnBnClickedButtonToothSwapedge();
	afx_msg void OnBnClickedButtonToothSwapedgeSharpedge();
	afx_msg void OnBnClickedButtonToothPathsmooth();
	afx_msg void OnBnClickedButtonToothPathprojection();
	afx_msg void OnBnClickedButtonCheckteeth();
};

