#pragma once

#include "OpenMesh.h"
#include "ap.h"
// Actions_Tspline dialog
class Tspline;
class CNurbsSuface;
class C_NurbsSurf;
class CMesh3d;
class Actions_Tspline : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_Tspline)

public:
	Actions_Tspline();
	virtual ~Actions_Tspline();

	//
	void ValidateRefinementMatrix(Tspline* T_current,Tspline* T_target,alglib::real_2d_array &M_refinement_matrix);

	//
	void Generate_Tspline_Least_Square_Fitting_Homogeneity(CMesh3d* pMesh1,CMesh3d* pMesh2,Tspline* pSur);

	//Least Square Tspline
	void Generate_Tspline_Least_Square_Fitting(CMesh3d* pMesh1,CMesh3d* pMesh2,Tspline* pSur);

	//
	void LeastSquareFitting_Tspline(Tspline* T_current,Tspline* T_target);

	//
	void LabelFourCornerPoints(Tspline* T_mesh);

	//
	BOOL SetFaceOffendingFlag(Tspline* T_mesh,TsplineMesh::FaceHandle fh);

	//
	void CollectandSplitOppositeFace_Horizontal(Tspline* T_current,TsplineMesh::VertexHandle fromvh,TsplineMesh::VertexHandle tovh,TsplineMesh::VertexHandle insertvh,TsplineMesh::FaceHandle fhandle);

	//
	void SplitHorizontal_Topology(Tspline* T_current,TsplineMesh::FaceHandle handle,int v0_index,int v1_index,int umin_index,int umax_index,int vmin_index,int vmax_index,TsplineMesh::VertexHandle vh1,TsplineMesh::VertexHandle vh2,BOOL findflag1,BOOL findflag2);

	//
	void CollectandSplitOppositeFace_Vertical(Tspline* T_current,TsplineMesh::VertexHandle vh1,TsplineMesh::VertexHandle vh2,TsplineMesh::VertexHandle insertvh,TsplineMesh::FaceHandle fhandle);

	//
	void Compute_RefinementMatrix(Tspline* T_current,Tspline* T_target,alglib::real_2d_array &M_refinement_matrix);

	//
	void SplitHorizontal(Tspline* T_current,TsplineMesh::FaceHandle handle,int uknot_num_inface,int vknot_num_inface,int vmin_index,int vmax_index,double umin,double umax,double vmin,double vmax,int umin_index,int umax_index);

	//
	void SplitVertical(Tspline* T_current,TsplineMesh::FaceHandle handle,int uknot_num_inface,int vknot_num_inface,int umin_index,int umax_index,double umin,double umax,double vmin,double vmax,int vmin_index,int vmax_index);

	//
	void SplitVertical_Topology(Tspline* T_current,TsplineMesh::FaceHandle handle,int u0_index,int u1_index,int umin_index,int umax_index,int vmin_index,int vmax_index,TsplineMesh::VertexHandle vh1,TsplineMesh::VertexHandle vh2,BOOL findflag1,BOOL findflag2);

	//
	void SetOffendingFaceFlag(Tspline* T_target,Tspline* T_current);

	//Get the fitting error
	double GetFittingError(Tspline* T_target,Tspline* T_current,alglib::real_2d_array &M_refinement_matrix,double TOL);

	//Compute the position
	void ComputePosition(Tspline* T_target,Tspline* T_current,alglib::real_2d_array &M_refinement_matrix,int index);

	//Compute the weights
	void ComputeWeights(Tspline* T_target,Tspline* T_current,alglib::real_2d_array &M_refinement_matrix);

	//bezier
	Tspline* OverSimplifyTspline(Tspline* pSurface,CMesh3d* pMesh3D,CMesh3d* pMesh2D,CNurbsSuface* pOriginalSurface);
	
	//LeastSquareFit
	double LeastSquareFit(Tspline* T_current,Tspline* T_target,double TOL, BOOL fitting = FALSE);

	//Get Face Error
	double GetFaceError(Tspline* T_target,Tspline* T_current,TsplineMesh::FaceHandle handle);

	//Subdivide the face
	void SubdivideFace(Tspline* T_current,TsplineMesh::FaceHandle handle);

	//
	C_NurbsSurf* Generate_NURBS_Least_Square_Fitting(CMesh3d* pMesh1,CMesh3d* pMesh2,CNurbsSuface* pSur);

// Dialog Data
	enum { IDD = IDD_TSPLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedTsplineGen();
	afx_msg void OnBnClickedTsplineSimplification();
};
