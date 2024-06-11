#pragma once

// Actions_LeastSquare dialog
class CMesh3d;
class CNurbsSuface;
class C_NurbsCurve3D;
class C_NurbsSurf;
#include <algorithm>
#include "optimization.h"
#include "ap.h"
using namespace alglib;
class CMesh3d;
class ThreadParam
{
public:
	HWND hwnd;
	int ns,nt;
};
class ThreadParam_ComputeCoefficient
{
public:
	C_NurbsSurf* pSurface;
	C_NurbsSurf* pNurbs;
	double u;
	double v;
	CMesh3d* poriginalmesh;
	CMesh3d* pconformalmesh;
	std::vector<double>* c;
	int c1;
	int c2;
	std::vector<double>* Jcoeff;
	double scalingnumber;

};
class ThreadParam_ComputeCoefficient_b
{
public:
	C_NurbsSurf* pSurface;
	C_NurbsSurf* pNurbs;
	double u;
	double v;
	CMesh3d* poriginalmesh;
	CMesh3d* pconformalmesh;
	double* c;
	int c1;
	int c2;
	double* Jcoeff;
	double scalingnumber;

};
class Actions_LeastSquare : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_LeastSquare)

public:
	Actions_LeastSquare();
	virtual ~Actions_LeastSquare();
	
	static int m_AddConformality;
	static int m_FixBoundary;
	static int ComputeCoefficient_x_threadnumber;
	static int ComputeCoefficient_y_threadnumber;
	static int ComputeCoefficient_x_b_threadnumber;
	static int ComputeCoefficient_y_b_threadnumber;
	//Given the point (u,v), Find the corresponding (s,t).
	static void FindCorrespondingPoint(CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double u,double v,double &s,double &t);

	//b four boundaries
	static void Actions_LeastSquare::Compute_b_Boundaries_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
		Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn);

	//b x constrained
	static void Compute_b_x_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
		Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn);

	//b y constrained
	static void Compute_b_y_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
		Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn);

	//b y
	static void Compute_b_y(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
		Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn);

	//b x
	static void Compute_b_x(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
		Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn);

	//low
	static void ComputeDerivative_low_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight);

	//top
	static void ComputeDerivative_top_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight);

	//left
	static void ComputeDerivative_left_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight);

	//right
	static void ComputeDerivative_right_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight);

	//x derivative constrained
	static void ComputeDerivative_X_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight);

	//y derivative constrained
	static void ComputeDerivative_Y_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight);

	//y derivative
	static void ComputeDerivative_Y(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight);

	//x derivative
	static void ComputeDerivative_X(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
		CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
		std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
		int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight);

	//
	static UINT ComputeCoefficient_y(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,double u,double v,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,std::vector<double>* c,int c1,int c2,std::vector<double> *Jcoeff,double scalingnumber);

	//
	static UINT ComputeCoefficient_y_b(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,double u,double v,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double* c,int c1,int c2,double* Jcoeff,double scalingnumber);

	//
	static UINT ComputeCoefficient_x_b(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,double u,double v,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double* c,int c1,int c2,double* Jcoeff,double scalingnumber);

	//
	static UINT ComputeCoefficient_x(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,double u,double v,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,std::vector<double>* c,int c1,int c2,std::vector<double> *Jcoeff,double scalingnumber);

	//
	static void ComputerConformalityCoefficient_CompositeSimpson_yderivative_b(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,int l,int k,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double& c,int c1,int c2,double weight);

	//
	static void ComputerConformalityCoefficient_CompositeSimpson_xderivative_b(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,int l,int k,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double& c,int c1,int c2,double weight);

	//
	static void ComputerConformalityCoefficient_CompositeSimpson_yderivative(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,int l,int k,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,std::vector<double>& c,int c1,int c2,double weight);

	//
	static void ComputerConformalityCoefficient_CompositeSimpson_xderivative(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,int l,int k,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,std::vector<double>& c,int c1,int c2,double weight);

	//for Bezier surface of degree 2*2
	void ConformalityOptimization22();

	//for Bezier surface of degree 1*1
	void ConformalityOptimization11();

	//compute the (x,y) coefficient of newvec on the vectors vx and vy  
	void GetVectorCoeffient(Vector3D newvec,Vector3D vx,Vector3D vy,double &x,double &y);

	//Bspline fitting
	static void BsplineSurfacefitting(int ns_,int nt_);

	//Least Square method for optimization
	static void BsplineTransformationOptimization_LeastSquare(int ns,int nt);
	
	//
	static void BsplineTransformationOptimization();

	//Construct constrained Bspline curves
	static C_NurbsCurve3D* FittingBsplineCurve_Constrained(double *knot,std::vector<int> &m_indexArray,CMesh3d* pconformalmesh,CMesh3d* poriginalmesh,C_NurbsSurf* pSurface,int flag,int CONSTRAINED_NUMBER = 1);

// Dialog Data
	enum { IDD = IDD_LeastSquareMethod };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDerivativevectorfield();
    afx_msg void OnBnClickedBsplinefitting();
	afx_msg void OnBnClickedBsplinefittingoptimization();
	afx_msg void OnBnClickedOrthogonalityopt();
	afx_msg void OnBnClickedConformalityopt22();
	afx_msg void OnBnClickedCheckconformity();
	afx_msg void OnBnClickedLeastsquaremethod();
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedLeastsquaremethodSeperate();
	afx_msg void OnBnClickedhierarchicalTessellation();
};
