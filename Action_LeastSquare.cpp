// Actions_GeneralBilinear.cpp : implementation file
//
#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Action_LeastSquare.h"
#include "C_NurbsSurf.h"
#include "Nurbs3D.h"
#include "Lib3D/Base3d.h"
#include "Lib3D/SceneGraph3d.h"
#include "FormView3.h"
#include "GeneralBilinearTransformation.h"
#include "BilinearTransformation.h"
#include <algorithm>
#include <fstream>
#include "QuadraticTransformation.h"
#include "BsplineTransformation.h"
#include "optimization.h"
#include "IsotropicEnergy.h"
#include"OrthogonalityEnergy.h"
#include "disline2d.h"
#include "disline.h"
#include "NurbsCurve.h"
#include "Lib3D/Transform.h"
#include "utility.h"
#include <fstream>
#include "vectorfield.h"
#include "Nurbs3D.h"
// Actions_GeneralBilinear dialog
long BeginTime;
UINT WorkingThread_ComputeCoefficient_y(LPVOID pParam);
UINT WorkingThread_ComputeCoefficient_x_b(LPVOID pParam);
UINT WorkingThread_ComputeCoefficient_y_b(LPVOID pParam);


IMPLEMENT_DYNAMIC(Actions_LeastSquare, CPropertyPage)
#define L_SAMPLING 20
#define K_SAMPLING 20
HWND g_hwndMain;
CWinThread* g_pThread;
CMesh3d* g_poriginalmesh;
CMesh3d* g_pconformalmesh;
CNurbsSuface* g_pSurface;
Actions_LeastSquare::Actions_LeastSquare()
	: CPropertyPage(Actions_LeastSquare::IDD)
{

}

Actions_LeastSquare::~Actions_LeastSquare()
{
}

void Actions_LeastSquare::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Actions_LeastSquare, CPropertyPage)
	ON_BN_CLICKED(IDC_DerivativeVectorField, &Actions_LeastSquare::OnBnClickedDerivativevectorfield)
	ON_BN_CLICKED(IDC_BsplineFitting, &Actions_LeastSquare::OnBnClickedBsplinefitting)
	ON_BN_CLICKED(IDC_BsplineFittingOptimization, &Actions_LeastSquare::OnBnClickedBsplinefittingoptimization)
	ON_BN_CLICKED(IDC_ConformalityOpt11, &Actions_LeastSquare::OnBnClickedOrthogonalityopt)
	ON_BN_CLICKED(IDC_ConformalityOpt22, &Actions_LeastSquare::OnBnClickedConformalityopt22)
	ON_BN_CLICKED(IDC_CHECKCONFORMITY, &Actions_LeastSquare::OnBnClickedCheckconformity)
	ON_BN_CLICKED(IDC_LeastSquareMethod, &Actions_LeastSquare::OnBnClickedLeastsquaremethod)
	ON_BN_CLICKED(IDC_TEST, &Actions_LeastSquare::OnBnClickedTest)
	ON_BN_CLICKED(IDC_LeastSquareMethod_Seperate, &Actions_LeastSquare::OnBnClickedLeastsquaremethodSeperate)
	ON_BN_CLICKED(IDC_hierarchical_TESSELLATION, &Actions_LeastSquare::OnBnClickedhierarchicalTessellation)
END_MESSAGE_MAP()

int IsValidBsplineTransformation(BsplineTransformation* pBsplineTrans)
{
	int l = 100;
	int k = 100;
	double mins = pBsplineTrans->pNurbsSurf->GetUKnot(0);
	double maxs = pBsplineTrans->pNurbsSurf->GetUKnot(pBsplineTrans->pNurbsSurf->GetCtrlNumberU() + pBsplineTrans->pNurbsSurf->GetDegreeU());

	double mint= pBsplineTrans->pNurbsSurf->GetVKnot(0);
	double maxt = pBsplineTrans->pNurbsSurf->GetVKnot(pBsplineTrans->pNurbsSurf->GetCtrlNumberV() + pBsplineTrans->pNurbsSurf->GetDegreeV());
	double u,v;
	double sum = 0;
	std::vector<double> sarray;
	std::vector<double> tarray;
	for(int i = 0; i <= 2 * l; i++)
	{
		double s = mins + i * (maxs - mins)/ (2.0 * l);
		sarray.push_back(s);
	}
	for(int j = 0; j <= 2 * k; j++)
	{
		double t = mint + j * (maxt - mint)/ (2.0 * k);
		tarray.push_back(t);
	}

	double TOL = 0.01;
	for(int i = 0; i < sarray.size(); i++)
		for(int j = 0; j < tarray.size(); j++)
		{
			double u,v;
			pBsplineTrans->Evaluation(sarray[i],tarray[j],u,v);
			if(u < -TOL || u > 1 + TOL || v < -TOL || v > 1 + TOL)
			{
				return 0;
			}
		}
 		return 1;
}
void Actions_LeastSquare::OnBnClickedDerivativevectorfield()
{
	// TODO: Add your control notification handler code here

	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\face.sur");

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	CNurbsSuface* pNsurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pSurface = pNsurface->Converte_C_NurbsSurface();

	//Given a surface, compute the derivatives of the nested freeform surface in 2D
	std::vector<std::vector<Vector2D>> m_VectorArray;
	int samplingu = 10;
	int samplingv = 10;
	for(int i = 0; i <= samplingu; i++)
		for(int j = 0; j <= samplingv; j++)
		{
			double u = i * 1.0 / samplingu;
			double v = j * 1.0 / samplingv;

			Vector3D Xu = pSurface->ComputUDerivative1(u,v);
			Vector3D Xv = pSurface->ComputVDerivative1(u,v);

			double E = Xu * Xu;
			double F = Xu * Xv;
			double G = Xv * Xv;
			double denominator = sqrt(E * G - F * F);

			//2 * 2 Matrix 
			double c11 = -F / denominator;
			double c12 = -G / denominator;
			double c21 = E / denominator;
			double c22 = F / denominator;
			std::vector<Vector2D> vector_;

			Vector2D phi_s(1,0);
			Vector2D phi_t;
			phi_t.x = phi_s.x * c11 + phi_s.y * c12;
			phi_t.y = phi_s.x * c21 + phi_s.y * c22;

			Vector2D coordinate;
			coordinate.x = u;
			coordinate.y = v;

			vector_.push_back(coordinate);
			vector_.push_back(phi_s);
			vector_.push_back(phi_t);

			m_VectorArray.push_back(vector_);
		}
	VectorField dlg;
	dlg.m_VectorArray = m_VectorArray;
	dlg.DoModal();
}
void Actions_LeastSquare::GetVectorCoeffient(Vector3D newvec,Vector3D vx,Vector3D vy,double &x,double &y)
{
	double a1 = vx * vx;
	double b1 = vx * vy;
	double c1 = newvec * vx;

	double a2 = vx * vy;
	double b2 = vy * vy;
	double c2 = newvec * vy;

	x = -(b1*c2-c1*b2)/(a1*b2-a2*b1);
	y = (a1*c2-a2*c1)/(a1*b2-a2*b1);
}
UINT WorkingThread(LPVOID pParam)
{
	HWND hwnd = (HWND)pParam;

	PostMessage(hwnd, WM_MESSAGE, 0, (LPARAM)"Start...");
	for(int i = 10; i < 40; i = i + 4)
	{
		Actions_LeastSquare::BsplineSurfacefitting(i,i);
		char message[80];
		sprintf(message,"D:\\%dby%d.sur",i,i);
		std::ofstream file(message);

		g_pSurface->Save(file);

		if(g_pSurface->ptransformation)
		{
			g_pSurface->ptransformation->Save(file);
		}

		file.close();
	}
	//Actions_LeastSquare::BsplineTransformationOptimization();
	PostMessage(hwnd, WM_MESSAGE, 0, (LPARAM)"End.");
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	//g_pMesh->report();
	return 1;

}
UINT WorkingThread_LeastSquare_Seperate(LPVOID pParam)
{
	ThreadParam *pThreadParam = (ThreadParam *)pParam;
	HWND hwnd = (HWND)pThreadParam->hwnd;

	PostMessage(hwnd, WM_MESSAGE, 0, (LPARAM)"Start...");
	Actions_LeastSquare::BsplineTransformationOptimization_LeastSquare(pThreadParam->ns,pThreadParam->nt);
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	//g_pMesh->report();
	long EndTime = GetTickCount();
	CString str;
	//the Time
	str.Format("Running Time:%5.2f ms",(EndTime - BeginTime)/1000.0);
	AfxMessageBox(str);

	return 1;
}
UINT WorkingThread_LeastSquare(LPVOID pParam)
{
	HWND hwnd = (HWND)pParam;

	PostMessage(hwnd, WM_MESSAGE, 0, (LPARAM)"Start...");

	for(int ns = 11; ns <= 20; ns++)
		for(int nt = 11; nt <= 20; nt++)
		{
			ThreadParam* pThreadParam = new ThreadParam();
			pThreadParam->hwnd = (HWND)pParam;
			pThreadParam->ns = ns;
			pThreadParam->nt = nt;
			g_pThread = AfxBeginThread(WorkingThread_LeastSquare_Seperate, 
				(LPVOID)pThreadParam,
				THREAD_PRIORITY_NORMAL,
				0,
				CREATE_SUSPENDED);
			g_pThread->m_bAutoDelete = TRUE;
			g_pThread->ResumeThread();
			//Actions_LeastSquare::BsplineTransformationOptimization_LeastSquare(ns,nt);
		}
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	//g_pMesh->report();
	return 1;

}

void Actions_LeastSquare::OnBnClickedBsplinefitting()
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\face.sur");
	//return;

	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\face.sur");

	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\cartop\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\cartop\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\cartop\\cartop.sur");

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\terrain\\parameterdomain.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\terrain\\conformal.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\terrain\\normalizedterrain.sur");

	//pDoc->OnOpenDocument("G:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\cartop\\parameterdomain.obj");
	//pDoc->OnOpenDocument("G:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\cartop\\conformal.obj");
	//pDoc->OnOpenDocument("G:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\cartop\\cartop.sur");

	//pDoc->OnOpenDocument("parameterdomain.obj");
	//pDoc->OnOpenDocument("conformal.obj");
	//pDoc->OnOpenDocument("cartop.sur");

	//pDoc->OnOpenDocument("D:\\Release\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\Release\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\Release\\face.sur");

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_SceneGraph.NbObject() != 3)
		return;

	g_poriginalmesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	g_pconformalmesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	g_pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(2);

	g_pThread = AfxBeginThread(WorkingThread, 
		(LPVOID)g_hwndMain,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	g_pThread->m_bAutoDelete = TRUE;
	g_pThread->ResumeThread();

}
char Post_Message[10000];
void Actions_LeastSquare::BsplineSurfacefitting(int ns_ = 4,int nt_ = 4)
{
	//Set the input from the global values
	CMesh3d* poriginalmesh = g_poriginalmesh;
	CMesh3d* pconformalmesh = g_pconformalmesh;
	CNurbsSuface* pSurface = g_pSurface;

	int uinnerknotnum = ns_; //inner knot num
	int vinnerknotnum = nt_;

	//we use 3*3-degree Bspline surfaces for the fitting.
	//
	Point3D lower,upper;
	Point3D originallower,originalupper;
	pconformalmesh->ComputeBoundingBox(lower,upper);
	poriginalmesh->ComputeBoundingBox(originallower,originalupper);
	int u_order = 4;
	int v_order = 4;
	int uknotnum = uinnerknotnum;
	int vknotnum = vinnerknotnum;

	double *uknot = new double[u_order * 2 + uknotnum];
	double *vknot = new double[v_order * 2 + vknotnum];

	for(int i = 0; i < 4; i++)
		uknot[i] = lower.x;
	
	for(int i = 1; i <= uinnerknotnum; i++)
	{
		uknot[3 + i] = lower.x + ((double)i) * (upper.x - lower.x) / (uinnerknotnum + 1);
	}

	for(int i = 0; i < 4; i++)
		uknot[4 + uinnerknotnum + i] = upper.x;

	for(int i = 0; i < 4; i++)
		vknot[i] = lower.y;

	for(int i = 1; i <= vinnerknotnum; i++)
	{
		vknot[3 + i] = lower.y + ((double)i) * (upper.y - lower.y) / (vinnerknotnum + 1);
	}

	for(int i = 0; i < 4; i++)
		vknot[4 + vinnerknotnum + i] = upper.y;

	//
	TRACE("\n uknot ");
	for(int i = 0 ; i < u_order * 2 + uknotnum; i++)
	{
		TRACE("%f ",uknot[i]);
	}
	TRACE("\n vknot ");
	for(int i = 0 ; i < v_order * 2 + vknotnum; i++)
	{
		TRACE("%f ",vknot[i]);
	}

	Point3D* pControlPoint = new Point3D[(u_order + uknotnum) * (v_order + vknotnum)];

	double* pWeight = new double[(u_order + uknotnum) * (v_order + vknotnum)];

	C_NurbsSurf* pNurbsSurface = new C_NurbsSurf(u_order + uknotnum, v_order + vknotnum, u_order, v_order, uknot,vknot,pControlPoint,pWeight);

	//first fit the four boundary curves.
	std::vector<int> m_lowerindexArray,m_upperindexArray,m_leftindexArray,m_rightindexArray;
	//Set label for the vertex 
	//boundary vertex 1
	//nonboundary vertex 0

	for(int i = 0; i < poriginalmesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = poriginalmesh->GetVertex(i);
		CVertex3d* pVertex1 = pconformalmesh->GetVertex(i);
		pVertex1->SetFlag(0);
		double x = pVertex->x();
		double y = pVertex->y();
		if(fabs(pVertex->y() - originallower.y) < 1e-5)
		{
			m_lowerindexArray.push_back(i);
			pVertex1->SetFlag(1);
		}
		
		if(fabs(pVertex->y() - originalupper.y) < 1e-5)
		{
			m_upperindexArray.push_back(i);
			pVertex1->SetFlag(1);
		}
		
		if(fabs(pVertex->x() - originallower.x) < 1e-5)
		{
			m_leftindexArray.push_back(i);
			pVertex1->SetFlag(1);
		}
		
		if(fabs(pVertex->x() - originalupper.x) < 1e-5)
		{
			m_rightindexArray.push_back(i);
			pVertex1->SetFlag(1);
		}
	}

	/*C_Disline2D* plowerDisline1 = new C_Disline2D();
	C_Disline2D* pupperDisline1 = new C_Disline2D();
	C_Disline2D* pleftDisline1 = new C_Disline2D();
	C_Disline2D* prightDisline1 = new C_Disline2D();

	C_Disline2D* plowerDisline2 = new C_Disline2D();
	C_Disline2D* pupperDisline2 = new C_Disline2D();
	C_Disline2D* pleftDisline2 = new C_Disline2D();
	C_Disline2D* prightDisline2 = new C_Disline2D();


	for(int i = 0; i < m_lowerindexArray.size(); i++)
	{
		Point2D point;

		point.x = poriginalmesh->GetVertex(m_lowerindexArray[i])->x();
		point.y = poriginalmesh->GetVertex(m_lowerindexArray[i])->y();
		plowerDisline1->AddDispoint(new C_Point2D(point));

		point.x = pconformalmesh->GetVertex(m_lowerindexArray[i])->x();
		point.y = pconformalmesh->GetVertex(m_lowerindexArray[i])->y();
		plowerDisline2->AddDispoint(new C_Point2D(point));

	}

	CDisline2D* pLine_LowerDisline1 = new CDisline2D();
	pLine_LowerDisline1->pDisline2D = plowerDisline1;
	pLine_LowerDisline1->type = TYPE_DISLINE2D;
	pLine_LowerDisline1->m_Transform.Copy(*(pSurface->GetTransform()));
	pDoc->m_SceneGraph.Add(pLine_LowerDisline1);

	CDisline2D* pLine_LowerDisline2 = new CDisline2D();
	pLine_LowerDisline2->pDisline2D = plowerDisline2;
	pLine_LowerDisline2->type = TYPE_DISLINE2D;
	pLine_LowerDisline2->m_Transform.Copy(*(pSurface->GetTransform()));
	pDoc->m_SceneGraph.Add(pLine_LowerDisline2);

	//
	TRACE("\n");
	for (int i = 0; i < m_lowerindexArray.size(); i++)
	{
		TRACE(" (%5.2f,%5.2f) ",poriginalmesh->GetVertex(m_lowerindexArray[i])->x(),poriginalmesh->GetVertex(m_lowerindexArray[i])->y());
	}
	TRACE("\n");
	for (int i = 0; i < m_lowerindexArray.size(); i++)
	{
		TRACE(" (%5.2f,%5.2f) ",pconformalmesh->GetVertex(m_lowerindexArray[i])->x(),pconformalmesh->GetVertex(m_lowerindexArray[i])->y());
	}
	for(int i = 0; i < m_upperindexArray.size(); i++)
	{
		Point2D point;

		point.x = poriginalmesh->GetVertex(m_upperindexArray[i])->x();
		point.y = poriginalmesh->GetVertex(m_upperindexArray[i])->y();
		pupperDisline1->AddDispoint(new C_Point2D(point));

		point.x = pconformalmesh->GetVertex(m_upperindexArray[i])->x();
		point.y = pconformalmesh->GetVertex(m_upperindexArray[i])->y();
		pupperDisline2->AddDispoint(new C_Point2D(point));

	}

	CDisline2D* pLine_UpperDisline1 = new CDisline2D();
	pLine_UpperDisline1->pDisline2D = pupperDisline1;
	pLine_UpperDisline1->type = TYPE_DISLINE2D;
	pLine_UpperDisline1->m_Transform.Copy(*(pSurface->GetTransform()));
	pDoc->m_SceneGraph.Add(pLine_UpperDisline1);

	CDisline2D* pLine_UpperDisline2 = new CDisline2D();
	pLine_UpperDisline2->pDisline2D = pupperDisline2;
	pLine_UpperDisline2->type = TYPE_DISLINE2D;
	pLine_UpperDisline2->m_Transform.Copy(*(pSurface->GetTransform()));
	pDoc->m_SceneGraph.Add(pLine_UpperDisline2);

	for(int i = 0; i < m_leftindexArray.size(); i++)
	{
		Point2D point;

		point.x = poriginalmesh->GetVertex(m_leftindexArray[i])->x();
		point.y = poriginalmesh->GetVertex(m_leftindexArray[i])->y();
		pleftDisline1->AddDispoint(new C_Point2D(point));

		point.x = pconformalmesh->GetVertex(m_leftindexArray[i])->x();
		point.y = pconformalmesh->GetVertex(m_leftindexArray[i])->y();
		pleftDisline2->AddDispoint(new C_Point2D(point));

	}

	CDisline2D* pLine_LeftDisline1 = new CDisline2D();
	pLine_LeftDisline1->pDisline2D = pleftDisline1;
	pLine_LeftDisline1->type = TYPE_DISLINE2D;
	pLine_LeftDisline1->m_Transform.Copy(*(pSurface->GetTransform()));
	pDoc->m_SceneGraph.Add(pLine_LeftDisline1);

	CDisline2D* pLine_LeftDisline2 = new CDisline2D();
	pLine_LeftDisline2->pDisline2D = pleftDisline2;
	pLine_LeftDisline2->type = TYPE_DISLINE2D;
	pLine_LeftDisline2->m_Transform.Copy(*(pSurface->GetTransform()));
	pDoc->m_SceneGraph.Add(pLine_LeftDisline2);


	for(int i = 0; i < m_rightindexArray.size(); i++)
	{
		Point2D point;

		point.x = poriginalmesh->GetVertex(m_rightindexArray[i])->x();
		point.y = poriginalmesh->GetVertex(m_rightindexArray[i])->y();	
		prightDisline1->AddDispoint(new C_Point2D(point));


		point.x = pconformalmesh->GetVertex(m_rightindexArray[i])->x();
		point.y = pconformalmesh->GetVertex(m_rightindexArray[i])->y();	
		prightDisline2->AddDispoint(new C_Point2D(point));

	}

	CDisline2D* pLine_RightDisline1 = new CDisline2D();
	pLine_RightDisline1->pDisline2D = prightDisline1;
	pLine_RightDisline1->type = TYPE_DISLINE2D;
	pLine_RightDisline1->m_Transform.Copy(*(pSurface->GetTransform()));
	pDoc->m_SceneGraph.Add(pLine_RightDisline1);

	CDisline2D* pLine_RightDisline2 = new CDisline2D();
	pLine_RightDisline2->pDisline2D = prightDisline2;
	pLine_RightDisline2->type = TYPE_DISLINE2D;
	pLine_RightDisline2->m_Transform.Copy(*(pSurface->GetTransform()));
	pDoc->m_SceneGraph.Add(pLine_RightDisline2);*/

	
	//
	C_NurbsCurve3D* plowercurve = FittingBsplineCurve_Constrained(uknot,m_lowerindexArray,pconformalmesh,poriginalmesh,pNurbsSurface,0,0);

	/*CNurbsCurve* pCurve1 = new CNurbsCurve();
	pCurve1->pCurve = plowercurve;
	pCurve1->filename = "Derivative";
	pCurve1->type = TYPE_NURBSCURVE3D;
	pCurve1->m_Transform.Copy(*(pSurface->GetTransform()));
	pDoc->m_SceneGraph.Add(pCurve1);
	return;*/

	C_NurbsCurve3D* puppercurve = FittingBsplineCurve_Constrained(uknot,m_upperindexArray,pconformalmesh,poriginalmesh,pNurbsSurface,0,0);
	C_NurbsCurve3D* pleftcurve = FittingBsplineCurve_Constrained(vknot,m_leftindexArray,pconformalmesh,poriginalmesh,pNurbsSurface,1,0);
	C_NurbsCurve3D* prightcurve = FittingBsplineCurve_Constrained(vknot,m_rightindexArray,pconformalmesh,poriginalmesh,pNurbsSurface,1,0);

	//Set the weights
	for(int i = 0; i < u_order + uknotnum; i++)
		for(int j = 0; j < v_order + vknotnum; j++)
			pNurbsSurface->SetWeight(i,j,1);

	//Set the boundary control points
	for(int i = 0; i < u_order + uknotnum; i++)
	{
		Point3D point = plowercurve->GetControlPoint(i);
		Point3D point1 = puppercurve->GetControlPoint(i);

		pNurbsSurface->SetControlPoint(i,0,point);
		pNurbsSurface->SetControlPoint(i,v_order + vknotnum - 1,point1);
	}
	for(int i = 0; i < v_order + vknotnum; i++)
	{
		Point3D point = pleftcurve->GetControlPoint(i);
		Point3D point1 = prightcurve->GetControlPoint(i);

		pNurbsSurface->SetControlPoint(0,i,point);
		pNurbsSurface->SetControlPoint(u_order + uknotnum - 1,i,point1);
	}

	//compute the inner control points.
	//the control number
	int ns = u_order + uknotnum - 1;
	int nt = v_order + vknotnum - 1;

	//sampling density
	int du = 100;
	int dv = 100;

	std::vector<Point3D> m_ConstrainedParaArray;
	std::vector<Point3D> m_ConstrainedPointArray;

	//---------------------Insert the constraints------------------
	//insert the constrains
	//TRACE(" \n ");
	for(int i = du + 2; i <= du * 2; i++)
	{
		//the lower constraints
		Point3D point;
		point.x = pconformalmesh->m_ArrayVertex[i]->x();
		point.y = pconformalmesh->m_ArrayVertex[i]->y();
		point.z = 0;
		m_ConstrainedParaArray.push_back(point);
		point.x = poriginalmesh->m_ArrayVertex[i]->x();
		point.y = poriginalmesh->m_ArrayVertex[i]->y();
		point.z = 0;
		m_ConstrainedPointArray.push_back(point);
		//TRACE(" %d ",i);
	}
	//TRACE(" \n ");
	for(int i = (du  + 1) * (dv - 1) + 1; i <= (du + 1) * (dv )  - 2; i++)
	{
		//the upper constraints
		Point3D point;
		point.x = pconformalmesh->m_ArrayVertex[i]->x();
		point.y = pconformalmesh->m_ArrayVertex[i]->y();
		point.z = 0;
		m_ConstrainedParaArray.push_back(point);
		point.x = poriginalmesh->m_ArrayVertex[i]->x();
		point.y = poriginalmesh->m_ArrayVertex[i]->y();
		point.z = 0;
		m_ConstrainedPointArray.push_back(point);
		//TRACE(" %d ",i);
	}
	//TRACE(" \n ");
	for(int i = 2 * (du + 1) + 1; i < (du  + 1) * (dv - 1) + 1; i = i + du + 1)
	{
		//the upper constraints
		Point3D point;
		point.x = pconformalmesh->m_ArrayVertex[i]->x();
		point.y = pconformalmesh->m_ArrayVertex[i]->y();
		point.z = 0;
		m_ConstrainedParaArray.push_back(point);
		point.x = poriginalmesh->m_ArrayVertex[i]->x();
		point.y = poriginalmesh->m_ArrayVertex[i]->y();
		point.z = 0;
		m_ConstrainedPointArray.push_back(point);
		//TRACE(" %d ",i);
	}
	//TRACE(" \n ");
	for(int i = 3 * du + 1; i < (du + 1) * (dv )  - 2; i = i + du + 1)
	{
		//the upper constraints
		Point3D point;
		point.x = pconformalmesh->m_ArrayVertex[i]->x();
		point.y = pconformalmesh->m_ArrayVertex[i]->y();
		point.z = 0;
		m_ConstrainedParaArray.push_back(point);
		point.x = poriginalmesh->m_ArrayVertex[i]->x();
		point.y = poriginalmesh->m_ArrayVertex[i]->y();
		point.z = 0;
		m_ConstrainedPointArray.push_back(point);
		//TRACE(" %d ",i);
	}
	m_ConstrainedPointArray.clear();
	m_ConstrainedParaArray.clear();
	//--------------------end insert constraints-------------------------------------
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)"Start Fitting Least Square Surface...");
	double c;
	std::vector<std::vector<double>> coeffcients;
	std::vector<double> coeffiecient;
	std::vector<double> b1,b2;
	//char message[500];
	for(int c1 = 1; c1 < ns; c1++)
		for(int c2 = 1; c2 < nt; c2++)
		{
			//sprintf(Post_Message,"Fitting Least Square Surface Computing Coefficient %3.2f%%",(c1*nt + c2) * 100.0 /(ns*nt));
			//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

			coeffiecient.clear();
			for(int k = 1; k < ns; k++)
				for(int l = 1; l < nt; l++)
				{
					c = 0;
					for(int i = 0; i < pconformalmesh->NbVertex(); i++)
					{
						CVertex3d* pVertex = pconformalmesh->GetVertex(i);
						if(pVertex->GetFlag() == 1)
							continue;

						double sij = pVertex->x();
						double tij = pVertex->y();

						double ubas = pNurbsSurface->ComputeUBasis(c1,sij);
						double vbas = pNurbsSurface->ComputeVBasis(c2,tij);

						double ubas1 = pNurbsSurface->ComputeUBasis(k,sij);
						double vbas1 = pNurbsSurface->ComputeVBasis(l,tij);
						c = c + ubas * vbas * ubas1 * vbas1 ;
					}
					coeffiecient.push_back(c);
				}

				//lambda
				for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
				{
					Point3D para = m_ConstrainedParaArray[k];

					double sij = para.x;
					double tij = para.y;

					double ubas = pNurbsSurface->ComputeUBasis(c1,sij);
					double vbas = pNurbsSurface->ComputeVBasis(c2,tij);

					c = ubas * vbas;

					coeffiecient.push_back(c);
				}
			coeffcients.push_back(coeffiecient);
		}

		//compute the derivative of lambda
		for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
		{
			//sprintf(Post_Message,"Fitting Least Square Surface Constraint (%d)...",k);
			//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

			coeffiecient.clear();
			Point3D para = m_ConstrainedParaArray[k];

			double sij = para.x;
			double tij = para.y;
			for(int c1 = 1; c1 < ns; c1++)
				for(int c2 = 1; c2 < nt; c2++)
				{
					double ubas = pNurbsSurface->ComputeUBasis(c1,sij);
					double vbas = pNurbsSurface->ComputeVBasis(c2,tij);
					c = ubas * vbas;
					coeffiecient.push_back(c);
				}
			for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
				coeffiecient.push_back(0);
			coeffcients.push_back(coeffiecient);
		}

		//b1
		for(int c1 = 1; c1 < ns; c1++)
			for(int c2 = 1; c2 < nt; c2++)
			{
				//sprintf(Post_Message,"Fitting Least Square Surface right constant b1 (%d,%d)...",c1,c2);
				//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
				c = 0;
				for(int i = 0; i < pconformalmesh->NbVertex(); i++)
				{
					CVertex3d* pVertex = pconformalmesh->GetVertex(i);
					CVertex3d* pVertex1 = poriginalmesh->GetVertex(i);
					if(pVertex->GetFlag() == 1)
						continue;

					double sij = pVertex->x();
					double tij = pVertex->y();

					double ubas = pNurbsSurface->ComputeUBasis(c1,sij);
					double vbas = pNurbsSurface->ComputeVBasis(c2,tij);

					double Aij = pVertex1->x();
					for(int ii = 0; ii <= ns; ii++)
					{
						double ubas1 = pNurbsSurface->ComputeUBasis(ii,sij);
						double vbas1 = pNurbsSurface->ComputeVBasis(0,tij);
						Vector3D Qi0 = plowercurve->GetControlPoint(ii) - ZeroP3D;
						Aij = Aij - Qi0.x * (ubas1 * vbas1); 
					}

					for(int ii = 0; ii <= ns; ii++)
					{
						double ubas1 = pNurbsSurface->ComputeUBasis(ii,sij);
						double vbas1 = pNurbsSurface->ComputeVBasis(nt,tij);
						Vector3D Qi0 = puppercurve->GetControlPoint(ii) - ZeroP3D;
						Aij = Aij - Qi0.x * (ubas1 * vbas1); 
					}

					for(int j = 1; j < nt; j++)
					{
						double ubas1 = pNurbsSurface->ComputeUBasis(0,sij);
						double vbas1 = pNurbsSurface->ComputeVBasis(j,tij);
						Vector3D Qi0 = pleftcurve->GetControlPoint(j) - ZeroP3D;
						Aij = Aij - Qi0.x * (ubas1 * vbas1); 
					}

					for(int j = 1; j < nt; j++)
					{
						double ubas1 = pNurbsSurface->ComputeUBasis(ns,sij);
						double vbas1 = pNurbsSurface->ComputeVBasis(j,tij);
						Vector3D Qi0 = prightcurve->GetControlPoint(j) - ZeroP3D;
						Aij = Aij - Qi0.x * (ubas1 * vbas1); 
					}

					c = c + ubas * vbas * Aij ;
				}
				b1.push_back(c);
			}

			for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
			{
				//sprintf(Post_Message,"Fitting Least Square Surface right constrained constant b1 (%d)...",k);
				//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

				Vector3D R = m_ConstrainedPointArray[k] - ZeroP3D;
				Point3D para = m_ConstrainedParaArray[k];
				//bottom
				for(int i = 0; i <= ns; i++)
				{
					Vector3D point = pNurbsSurface->GetControlPoint(i,0) - ZeroP3D;
					double ubas = pNurbsSurface->ComputeUBasis(i,para.x);
					double vbas = pNurbsSurface->ComputeVBasis(0,para.y);
					R = R - point * ubas * vbas;
				}
				//top
				for(int i = 0; i <= ns; i++)
				{
					Vector3D point = pNurbsSurface->GetControlPoint(i,nt) - ZeroP3D;
					double ubas = pNurbsSurface->ComputeUBasis(i,para.x);
					double vbas = pNurbsSurface->ComputeVBasis(nt,para.y);
					R = R - point * ubas * vbas;
				}
				//left
				for(int i = 1; i < nt; i++)
				{
					Vector3D point = pNurbsSurface->GetControlPoint(0,i) - ZeroP3D;
					double ubas = pNurbsSurface->ComputeUBasis(0,para.x);
					double vbas = pNurbsSurface->ComputeVBasis(i,para.y);
					R = R - point * ubas * vbas;
				}
				//right
				for(int i = 1; i < nt; i++)
				{
					Vector3D point = pNurbsSurface->GetControlPoint(ns,i) - ZeroP3D;
					double ubas = pNurbsSurface->ComputeUBasis(ns,para.x);
					double vbas = pNurbsSurface->ComputeVBasis(i,para.y);
					R = R - point * ubas * vbas;
				}
				c = R.x;
				b1.push_back(c);
			}

		//b2
			for(int c1 = 1; c1 < ns; c1++)
				for(int c2 = 1; c2 < nt; c2++)
				{
					//sprintf(Post_Message,"Fitting Least Square Surface right constant b2(%d,%d)...",c1,c2);
					//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
					c = 0;
					for(int i = 0; i < pconformalmesh->NbVertex(); i++)
					{
						CVertex3d* pVertex = pconformalmesh->GetVertex(i);
						CVertex3d* pVertex1 = poriginalmesh->GetVertex(i);
						if(pVertex->GetFlag() == 1)
							continue;

						double sij = pVertex->x();
						double tij = pVertex->y();

						double ubas = pNurbsSurface->ComputeUBasis(c1,sij);
						double vbas = pNurbsSurface->ComputeVBasis(c2,tij);

						double Aij = pVertex1->y();
						for(int ii = 0; ii <= ns; ii++)
						{
							double ubas1 = pNurbsSurface->ComputeUBasis(ii,sij);
							double vbas1 = pNurbsSurface->ComputeVBasis(0,tij);
							Vector3D Qi0 = plowercurve->GetControlPoint(ii) - ZeroP3D;
							Aij = Aij - Qi0.y * (ubas1 * vbas1); 
						}

						for(int ii = 0; ii <= ns; ii++)
						{
							double ubas1 = pNurbsSurface->ComputeUBasis(ii,sij);
							double vbas1 = pNurbsSurface->ComputeVBasis(nt,tij);
							Vector3D Qi0 = puppercurve->GetControlPoint(ii) - ZeroP3D;
							Aij = Aij - Qi0.y * (ubas1 * vbas1); 
						}

						for(int j = 1; j < nt; j++)
						{
							double ubas1 = pNurbsSurface->ComputeUBasis(0,sij);
							double vbas1 = pNurbsSurface->ComputeVBasis(j,tij);
							Vector3D Qi0 = pleftcurve->GetControlPoint(j) - ZeroP3D;
							Aij = Aij - Qi0.y * (ubas1 * vbas1); 
						}

						for(int j = 1; j < nt; j++)
						{
							double ubas1 = pNurbsSurface->ComputeUBasis(ns,sij);
							double vbas1 = pNurbsSurface->ComputeVBasis(j,tij);
							Vector3D Qi0 = prightcurve->GetControlPoint(j) - ZeroP3D;
							Aij = Aij - Qi0.y * (ubas1 * vbas1); 
						}

						c = c + ubas * vbas * Aij ;
					}
					b2.push_back(c);
				}

				for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
				{
					//sprintf(Post_Message,"Fitting Least Square Surface right constrained constant b2(%d)...",k);
					//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

					Vector3D R = m_ConstrainedPointArray[k] - ZeroP3D;
					Point3D para = m_ConstrainedParaArray[k];
					//bottom
					for(int i = 0; i <= ns; i++)
					{
						Vector3D point = pNurbsSurface->GetControlPoint(i,0) - ZeroP3D;
						double ubas = pNurbsSurface->ComputeUBasis(i,para.x);
						double vbas = pNurbsSurface->ComputeVBasis(0,para.y);
						R = R - point * ubas * vbas;
					}
					//top
					for(int i = 0; i <= ns; i++)
					{
						Vector3D point = pNurbsSurface->GetControlPoint(i,nt) - ZeroP3D;
						double ubas = pNurbsSurface->ComputeUBasis(i,para.x);
						double vbas = pNurbsSurface->ComputeVBasis(nt,para.y);
						R = R - point * ubas * vbas;
					}
					//left
					for(int i = 1; i < nt; i++)
					{
						Vector3D point = pNurbsSurface->GetControlPoint(0,i) - ZeroP3D;
						double ubas = pNurbsSurface->ComputeUBasis(0,para.x);
						double vbas = pNurbsSurface->ComputeVBasis(i,para.y);
						R = R - point * ubas * vbas;
					}
					//right
					for(int i = 1; i < nt; i++)
					{
						Vector3D point = pNurbsSurface->GetControlPoint(ns,i) - ZeroP3D;
						double ubas = pNurbsSurface->ComputeUBasis(ns,para.x);
						double vbas = pNurbsSurface->ComputeVBasis(i,para.y);
						R = R - point * ubas * vbas;
					}
					c = R.y;
					b2.push_back(c);
				}

		//sprintf(Post_Message,"Fitting Least Square Surface solving the least square system...");
		//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		std::vector<double> xarray,yarray;
		p_FormView3->SolveLinearSystem(coeffcients,b1,xarray);
		p_FormView3->SolveLinearSystem(coeffcients,b2,yarray);

		//Set the control points.
		int num = 0;
		for(int c1 = 1; c1 < ns; c1++)
			for(int c2 = 1; c2 < nt; c2++)
			{
				Point3D point;
				point.x = xarray[num];
				point.y = yarray[num];
				point.z = 0;
				pNurbsSurface->SetControlPoint(c1,c2,point);
				pNurbsSurface->SetWeight(c1,c2,1);
				num++;
			}

	//Generate 
	BsplineTransformation* pTrans = new BsplineTransformation();
	pTrans->m_ReparameterizationType = Bspline;
	pTrans->umin_Reparameterization = lower.x;
	pTrans->vmin_Reparameterization = lower.y;
	pTrans->umax_Reparameterization = upper.x;
	pTrans->vmax_Reparameterization = upper.y;
	pTrans->umin = originallower.x;
	pTrans->vmin = originallower.y;
	pTrans->umax = originalupper.x;
	pTrans->vmax = originalupper.y;

	pTrans->pNurbsSurf = pNurbsSurface;
	pSurface->ptransformation = pTrans;
	
	//if(!IsValidBsplineTransformation(pTrans))
	//	AfxMessageBox("Transformation Not Valid, Please adjust the parameter");
	//CNurbsSuface* pNurbs1 = pNurbsSurface->GetCNurbsSurface();
	//pNurbs1->filename = "transformation";
	//pNurbs1->SetTransform(*pSurface->GetTransform());
	//pDoc->m_SceneGraph.Add(pNurbs1);
	//pDoc->UpdateTreeControl();
}

C_NurbsCurve3D* Actions_LeastSquare::FittingBsplineCurve_Constrained(double *knot,std::vector<int> &m_indexArray,CMesh3d* pconformalmesh,CMesh3d* poriginalmesh,C_NurbsSurf* pSurface,int flag,int CONSTRAINED_NUMBER)
{
	int knotnum;
	if(flag == 0)
		knotnum = pSurface->GetCtrlNumberU() + pSurface->GetOrderU();
	else
		knotnum = pSurface->GetCtrlNumberV() + pSurface->GetOrderV();

	std::vector<double> m_knotVector;
	std::vector<Point3D> m_PointArray;
	std::vector<double> m_ParaArray;
	std::vector<double> m_WeightArray;
	std::vector<double> m_ConstrainedParaArray;
	std::vector<Point3D> m_ConstrainedPointArray;

	for(int i = 0; i < knotnum; i++)
		m_knotVector.push_back(knot[i]);

	int NUM = 4;
	int BIG_WEIGHT = 1;
	int NORMALA_WEIGHT = 1;
	for(int i = 0; i < m_indexArray.size(); i++)
	{
		Point3D point;
		double para;
		point.x = poriginalmesh->GetVertex(m_indexArray[i])->x();
		point.y = poriginalmesh->GetVertex(m_indexArray[i])->y();
		point.z = poriginalmesh->GetVertex(m_indexArray[i])->z();
		if(flag == 0)
			para = pconformalmesh->GetVertex(m_indexArray[i])->x();
		else
			para = pconformalmesh->GetVertex(m_indexArray[i])->y();
		m_PointArray.push_back(point);
		m_ParaArray.push_back(para);

		if(i < NUM || i > m_indexArray.size() - 1 - NUM)
			m_WeightArray.push_back(BIG_WEIGHT);
		else
			m_WeightArray.push_back(NORMALA_WEIGHT);
	}
	
	for(int i = 0; i < CONSTRAINED_NUMBER; i++)
	{
		if(flag == 0)
		{
			m_ConstrainedParaArray.push_back(pconformalmesh->GetVertex(m_indexArray[i + 1])->x());
			m_ConstrainedParaArray.push_back(pconformalmesh->GetVertex(m_indexArray[m_indexArray.size() - 2 - i])->x());
		}else
		{
			m_ConstrainedParaArray.push_back(pconformalmesh->GetVertex(m_indexArray[i + 1])->y());
			m_ConstrainedParaArray.push_back(pconformalmesh->GetVertex(m_indexArray[m_indexArray.size() - 2 - i])->y());
		}

		Point3D point;
		point.x = poriginalmesh->GetVertex(m_indexArray[i + 1])->x();
		point.y = poriginalmesh->GetVertex(m_indexArray[i + 1])->y();
		point.z = poriginalmesh->GetVertex(m_indexArray[i + 1])->z();
		m_ConstrainedPointArray.push_back(point);

		point.x = poriginalmesh->GetVertex(m_indexArray[m_indexArray.size() - 2 - i])->x();
		point.y = poriginalmesh->GetVertex(m_indexArray[m_indexArray.size() - 2 - i])->y();
		point.z = poriginalmesh->GetVertex(m_indexArray[m_indexArray.size() - 2 - i])->z();
		m_ConstrainedPointArray.push_back(point);
	}

	C_NurbsCurve3D* pNurbsCurve = Utility::FittingBsplineCurve_Constrained_Weighted(m_knotVector,m_PointArray,m_ParaArray,m_WeightArray,m_ConstrainedParaArray,m_ConstrainedPointArray);

	if(flag == 0)
	{
		//top or bottom
		for(int i = 0; i < pNurbsCurve->GetCtrlNumber(); i++)
		{
			Point3D point = pNurbsCurve->GetControlPoint(i);
			//if(point.y < 0.5)
			//	point.y = 0;
			//else
			//	point.y = 1;
			pNurbsCurve->SetControlPoint(i,point);
		}
	}else
	{
		//left or right
		for(int i = 0; i < pNurbsCurve->GetCtrlNumber(); i++)
		{
			Point3D point = pNurbsCurve->GetControlPoint(i);
			//if(point.x < 0.5)
			//	point.x = 0;
			//else
			//	point.x = 1;
			pNurbsCurve->SetControlPoint(i,point);
		}
	}

	double umin = pNurbsCurve->GetKnot(0);
	double umax = pNurbsCurve->GetKnot(pNurbsCurve->GetCtrlNumber() + pNurbsCurve->GetOrder() - 1);
	for(int i = 0; i <= 200; i++)
	{
		double t = i / 200.0;
		t = umin + (umax - umin) * t;
		Point3D point = pNurbsCurve->ComputPosition(t);
		if(point.x < 0 || point.x > 1 || point.y < 0 || point.y > 1)
		{
			int mn = 0;
		}
	}
	return pNurbsCurve;
}
using namespace alglib;
extern std::vector<std::vector<double>> MeshArray;
double finalval;
void ConformalEnergy22(const real_1d_array &x, double &func,  real_1d_array &grad, void *ptr)
{
	//排列顺序为p00x，p00y,p00z,p10x,p10y,p10z.......
	OrthogonalityEnergy OrthEnergy(x);
	IsotropicEnergy IsoEnergy(x);

	double energyorth,energyiso;
	real_1d_array gradorth = grad;
	real_1d_array gradiso = grad;

	//std::vector<double> mesh;
	//for(int ii = 0; ii < 27; ii++)
	//	mesh.push_back(x[ii]);

	//MeshArray.push_back(mesh);

	OrthEnergy.getGrad22(energyorth,gradorth);//func,Grad
	IsoEnergy.getGrad22(energyiso,gradiso);

	double orthweight = 0.5;
	double isoweight = 0.5;
	func = energyorth * orthweight + energyiso * isoweight;
	finalval = func;
	int len1 = gradorth.length();
	int len2 = gradiso.length();
	for(int i = 0; i < grad.length(); i++)
	{
		grad[i] = gradorth[i] * orthweight + gradiso[i] * isoweight;
	}

	grad[0] = 0;
	grad[1] = 0;
	grad[2] = 0;
	grad[3] = 0;
	grad[4] = 0;
	grad[5] = 0;

	//TRACE("%s :grad\n", grad.tostring(5).c_str());
	//TRACE("%s :x\n", x.tostring(5).c_str());
	/*TRACE("%f :J\n\n\n", func);
	for(int i = 0; i < grad.length(); i++)
	{
		TRACE("%f ",grad[i]);
	}
	TRACE("\n");
	for(int i = 0; i < x.length(); i++)
	{
		TRACE("%f ",x[i]);
	}*/
}
void ConformalEnergy11(const real_1d_array &x, double &func,  real_1d_array &grad, void *ptr)
{

	//排列顺序为p00x，p00y,p00z,p10x,p10y,p10z.......

	OrthogonalityEnergy OrthEnergy(x);
	IsotropicEnergy IsoEnergy(x);

	double energyorth,energyiso;
	real_1d_array gradorth = grad;
	real_1d_array gradiso = grad;

	std::vector<double> mesh;
	for(int ii = 0; ii < 12; ii++)
		mesh.push_back(x[ii]);

	MeshArray.push_back(mesh);

	OrthEnergy.getGrad11(energyorth,gradorth);//func,Grad
	IsoEnergy.getGrad11(energyiso,gradiso);

	double orthweight = 0.5;
	double isoweight = 0.5;
	func = energyorth * orthweight + energyiso * isoweight;

	int len1 = gradorth.length();
	int len2 = gradiso.length();
	for(int i = 0; i < grad.length(); i++)
	{
		grad[i] = gradorth[i] * orthweight + gradiso[i] * isoweight;
	}

	grad[0] = 0;
	grad[1] = 0;
	grad[2] = 0;
	grad[6] = 0;
	grad[7] = 0;
	grad[8] = 0;

	//TRACE("%s :grad\n", grad.tostring(5).c_str());
	//TRACE("%s :x\n", x.tostring(5).c_str());
	TRACE("%f :J\n\n\n", func);
	for(int i = 0; i < grad.length(); i++)
	{
		TRACE("%f ",grad[i]);
	}
}
void ConformalEnergy3D(const real_1d_array &x, double &func, void *ptr)
{
	//
	// this callback calculates f(x0,x1) = 100*(x0+3)^4 + (x1-3)^4
	//
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	//if(pDoc->m_SceneGraph.NbObject() != 4)
	//	return;

	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pNSurface = pSurface->Converte_C_NurbsSurface();

	int num = 0;
	for(int i = 0; i < pNSurface->GetCtrlNumberU(); i++)
		for(int j = 0; j < pNSurface->GetCtrlNumberV(); j++)
		{
			if(i == 0 && j == 0)
				continue;

			Point3D point;
			point.x = x[num * 3];
			point.y = x[num * 3 + 1];
			point.z = x[num * 3 + 2];

			TRACE("%f %f %f \n",point.x,point.y,point.z);

			pNSurface->SetControlPoint(i,j,point);
			num++;
		}

		int samplingu = 100;
		int samplingv = 100;
		double u,v;
		double sum = 0;
		for(int i = 0; i <= samplingu; i++)
			for(int j = 0; j <= samplingv; j++)
			{
				double s = i * 1.0 / samplingu;
				double t = j * 1.0 / samplingv;

				
				Vector3D udir = pNSurface->ComputUDerivative1(s,t);
				Vector3D vdir = pNSurface->ComputVDerivative1(s,t);

				Vector3D N = udir ^ vdir;
				N.SetUnit();

				double len = Length(N ^ udir - vdir);
				sum += len * len;
			}
		func = sum;
		TRACE("%f \n",sum);
}

double ComputeEnergy(C_NurbsSurf* pNSurface,BsplineTransformation* pBsplineTrans,double sij,double tij)
{
	//compute sij,tij.
	double uij,vij;
	if(pBsplineTrans)
		pBsplineTrans->Evaluation(sij,tij,uij,vij);
	else
	{
		uij = sij;
		vij = tij;
	}

	//N 
	Vector3D Xu = pNSurface->ComputUDerivative1(uij,vij);
	Vector3D Xv = pNSurface->ComputVDerivative1(uij,vij);
	Vector3D N = (Xu^Xv);
	N.SetUnit();

	double u_s = 0;
	double v_s = 0;
	double u_t = 0;
	double v_t = 0;

	if(pBsplineTrans)
	{
		int m = pBsplineTrans->pNurbsSurf->GetCtrlNumberU();
		int n = pBsplineTrans->pNurbsSurf->GetCtrlNumberV();

		for(int i2 = 0; i2 < m; i2++)
		{
			for(int j2 = 0; j2 < n; j2++)
			{
				u_s += pBsplineTrans->pNurbsSurf->ComputeUBasis_PartialU(i2,sij) * pBsplineTrans->pNurbsSurf->ComputeVBasis(j2,tij) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).x;
				v_s += pBsplineTrans->pNurbsSurf->ComputeUBasis_PartialU(i2,sij) * pBsplineTrans->pNurbsSurf->ComputeVBasis(j2,tij) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).y;
				u_t += pBsplineTrans->pNurbsSurf->ComputeUBasis(i2,sij) * pBsplineTrans->pNurbsSurf->ComputeVBasis_PartialV(j2,tij) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).x;
				v_t += pBsplineTrans->pNurbsSurf->ComputeUBasis(i2,sij) * pBsplineTrans->pNurbsSurf->ComputeVBasis_PartialV(j2,tij) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).y;

			}
		}
	}
	else
	{
		u_s = 1;
		v_s = 0;
		u_t = 0;
		v_t = 1;
	}

	//(N * Xs - Xt)^2
	Vector3D vec = N ^ (Xu * u_s  + Xv * v_s) - (Xu * u_t + Xv * v_t);

	double val = vec * vec;
	return val;
}
//Relax X_u and X_v, Fix N
std::vector<double> ComputeGradient_(C_NurbsSurf* pNSurface,BsplineTransformation* pBsplineTrans,double s,double t)
{
	std::vector<double> grad;

	//compute sij,tij.
	double u,v;

	if(fabs(s + 0.99999898672103882 ) < 1e-7 && fabs(t + 1.0160000324249268) < 1e-7)
	{
		int ij = 0;
	}
	pBsplineTrans->Evaluation(s,t,u,v);

	//suppose N is unchanged during the each iteration 
	Vector3D Xu = pNSurface->ComputUDerivative1(u,v);
	Vector3D Xv = pNSurface->ComputVDerivative1(u,v);
	Vector3D Xuu = pNSurface->ComputUDerivative2(u,v);
	Vector3D Xvv = pNSurface->ComputVDerivative2(u,v);
	Vector3D Xuv = pNSurface->ComputUVDerivative2(u,v);
	Vector3D N = Xu ^ Xv;
	N.SetUnit();
	Vector3D N_Xu = N ^ Xu;
	Vector3D N_Xv = N ^ Xv;
	Vector3D N_Xuu = N ^ Xuu;
	Vector3D N_Xvv = N ^ Xvv;
	Vector3D N_Xuv = N ^ Xuv;

	int m = pBsplineTrans->pNurbsSurf->GetCtrlNumberU();
	int n = pBsplineTrans->pNurbsSurf->GetCtrlNumberV();
	for(int j = 1; j < n - 1; j++)
	{
		for(int i = 1;i < m - 1; i++)
		{
			double u_s_Qijx = pBsplineTrans->pNurbsSurf->ComputeUBasis_PartialU(i,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis(j,t);
			double v_s_Qijy = u_s_Qijx;

			double u_t_Qijx = pBsplineTrans->pNurbsSurf->ComputeUBasis(i,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis_PartialV(j,t);
			double v_t_Qijy = u_t_Qijx;

			double u_Qijx = pBsplineTrans->pNurbsSurf->ComputeUBasis(i,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis(j,t);
			double v_Qijy =  u_Qijx;

			double u_s = 0;
			double v_s = 0;
			double u_t = 0;
			double v_t = 0;
			for(int i2 = 0; i2 < m; i2++)
			{
				for(int j2 = 0; j2 < n; j2++)
				{
					u_s += pBsplineTrans->pNurbsSurf->ComputeUBasis_PartialU(i2,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis(j2,t) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).x;
					v_s += pBsplineTrans->pNurbsSurf->ComputeUBasis_PartialU(i2,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis(j2,t) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).y;
					u_t += pBsplineTrans->pNurbsSurf->ComputeUBasis(i2,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis_PartialV(j2,t) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).x;
					v_t += pBsplineTrans->pNurbsSurf->ComputeUBasis(i2,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis_PartialV(j2,t) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).y;

				}
			}

			//W
			Vector3D res_x_part_1 = N_Xu * u_s + N_Xv * v_s - (Xu * u_t + Xv * v_t);
			Vector3D res_y_part_1 = N_Xu * u_s + N_Xv * v_s - (Xu * u_t + Xv * v_t);

			//W'
			Vector3D res_x_part_2 = N_Xuu * u_Qijx * u_s + N_Xu * u_s_Qijx + N_Xuv * u_Qijx * v_s - Xuu * u_Qijx * u_t - Xu * u_t_Qijx - Xuv * u_Qijx * v_t; 
			Vector3D res_y_part_2 = N_Xuv * v_Qijy * u_s + N_Xvv * v_Qijy * v_s + N_Xv * v_s_Qijy - Xuv * v_Qijy * u_t - Xvv * v_Qijy * v_t - Xv * v_t_Qijy;

			double res_x = res_x_part_1 * res_x_part_2 * 2;
			double res_y = res_y_part_1 * res_y_part_2 * 2;

			grad.push_back(res_x);
			grad.push_back(res_y);
		}
	}
	return grad;
}
std::vector<double> ComputeGradient(C_NurbsSurf* pNSurface,BsplineTransformation* pBsplineTrans,double s,double t)
{
	return ComputeGradient_(pNSurface,pBsplineTrans,s,t);
	//
	std::vector<double> grad;

	//compute sij,tij.
	double u,v;
	pBsplineTrans->Evaluation(s,t,u,v);

	//suppose N is unchanged during the each iteration 
	Vector3D Xu = pNSurface->ComputUDerivative1(u,v);
	Vector3D Xv = pNSurface->ComputVDerivative1(u,v);
	Vector3D N = (Xu^Xv);
	N.SetUnit();
	Vector3D N_Xu = N ^ Xu;
	Vector3D N_Xv = N ^ Xv;

	int m = pBsplineTrans->pNurbsSurf->GetCtrlNumberU();
	int n = pBsplineTrans->pNurbsSurf->GetCtrlNumberV();
	for(int j = 1; j < n - 1; j++)
	{
		for(int i = 1;i < m - 1; i++)
		{
			double u_s_Qijx = pBsplineTrans->pNurbsSurf->ComputeUBasis_PartialU(i,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis(j,t);
			double v_s_Qijy = u_s_Qijx;

			double u_t_Qijx = pBsplineTrans->pNurbsSurf->ComputeUBasis(i,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis_PartialV(j,t);
			double v_t_Qijy = u_t_Qijx;

			Vector3D res_x_part_1 = N_Xu * u_s_Qijx - Xu * u_t_Qijx;
			Vector3D res_y_part_1 = N_Xv * v_s_Qijy - Xv * v_t_Qijy;

			double u_s = 0;
			double v_s = 0;
			double u_t = 0;
			double v_t = 0;
			for(int i2 = 0; i2 < m; i2++)
			{
				for(int j2 = 0; j2 < n; j2++)
				{
					u_s += pBsplineTrans->pNurbsSurf->ComputeUBasis_PartialU(i2,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis(j2,t) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).x;
					v_s += pBsplineTrans->pNurbsSurf->ComputeUBasis_PartialU(i2,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis(j2,t) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).y;
					u_t += pBsplineTrans->pNurbsSurf->ComputeUBasis(i2,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis_PartialV(j2,t) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).x;
					v_t += pBsplineTrans->pNurbsSurf->ComputeUBasis(i2,s) * pBsplineTrans->pNurbsSurf->ComputeVBasis_PartialV(j2,t) * pBsplineTrans->pNurbsSurf->GetControlPoint(i2,j2).y;

				}
			}

			Vector3D res_x_part_2 = N_Xu * u_s  + N_Xv * v_s - (Xu * u_t + Xv * v_t);
			Vector3D res_y_part_2 = res_x_part_2;

			double res_x = res_x_part_1 * res_x_part_2 * 2;
			double res_y = res_y_part_1 * res_y_part_2 * 2;

			grad.push_back(res_x);
			grad.push_back(res_y);
		}
	}
	return grad;
}
std::vector<double> ComputeConformalGradient(CNurbsSuface* pSurface,int dimension)
{
	C_NurbsSurf* pNSurface = pSurface->Converte_C_NurbsSurface();
	BsplineTransformation* pBsplineTrans = (BsplineTransformation*)pSurface->ptransformation;

	int l = L_SAMPLING;
	int k = K_SAMPLING;
	double mins = pBsplineTrans->pNurbsSurf->GetUKnot(0);
	double maxs = pBsplineTrans->pNurbsSurf->GetUKnot(pBsplineTrans->pNurbsSurf->GetCtrlNumberU() + pBsplineTrans->pNurbsSurf->GetDegreeU());

	double mint= pBsplineTrans->pNurbsSurf->GetVKnot(0);
	double maxt = pBsplineTrans->pNurbsSurf->GetVKnot(pBsplineTrans->pNurbsSurf->GetCtrlNumberV() + pBsplineTrans->pNurbsSurf->GetDegreeV());
	double u,v;
	double sum = 0;
	std::vector<double> sarray;
	std::vector<double> tarray;
	for(int i = 0; i <= 2 * l; i++)
	{
		double s = mins + i * (maxs - mins)/ (2.0 * l);
		sarray.push_back(s);
	}
	for(int j = 0; j <= 2 * k; j++)
	{
		double t = mint + j * (maxt - mint)/ (2.0 * k);
		tarray.push_back(t);
	}

	std::vector<double> J1gradient,J2gradient,J3gradient,J4gradient;
	for(int i = 0; i < dimension; i++)
	{
		J1gradient.push_back(0);
		J2gradient.push_back(0);
		J3gradient.push_back(0);
		J4gradient.push_back(0);
	}

	//compute the gradient.

	std::vector<double> temp;
	J1gradient = J1gradient + ComputeGradient(pNSurface,pBsplineTrans,sarray[0],tarray[0]);
	for(int i = 1; i <= l - 1; i++)
	{
		J1gradient += 2 * ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * i],tarray[0]);
	}
	for(int i = 1; i<= l; i++)
		J1gradient += 4 *ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * i -1],tarray[0]);
	J1gradient += ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * l],tarray[0]);

	for(int j = 1; j<= k - 1; j++)
		J2gradient += ComputeGradient(pNSurface,pBsplineTrans,sarray[0],tarray[2 * j]);
	for(int j = 1; j <= k - 1; j++)
		for(int i= 1; i <= l - 1; i++)
			J2gradient += 2 * ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * i],tarray[2 * j]);
	for(int j = 1; j <= k - 1; j++)
		for(int i = 1; i<= l; i++)
			J2gradient += 4 * ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * i -1],tarray[2 * j]);
	for(int j = 1; j<= k - 1; j++)
		J2gradient += ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * l],tarray[2 * j]);

	for(int j = 1; j<= k; j++)
		J3gradient += ComputeGradient(pNSurface,pBsplineTrans,sarray[0],tarray[2 * j - 1]);
	for(int j = 1; j <= k; j++)
		for(int i= 1; i <= l - 1; i++)
			J3gradient += 2 * ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * i],tarray[2 * j - 1]);
	for(int j = 1; j <= k; j++)
		for(int i = 1; i<= l; i++)
			J3gradient += 4 * ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * i -1],tarray[2 * j - 1]);
	for(int j = 1; j<= k ; j++)
		J3gradient += ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * l],tarray[2 * j - 1]);

	J4gradient +=ComputeGradient(pNSurface,pBsplineTrans,sarray[0],tarray[2 * k]);
	for(int i = 1; i <= l - 1; i++)
		J4gradient += 2 * ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * i],tarray[2 * k]);
	for(int i = 1; i<= l; i++)
		J4gradient += 4 *ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * i -1],tarray[2 * k]);
	J4gradient += ComputeGradient(pNSurface,pBsplineTrans,sarray[2 * l],tarray[2 * k]);

	std::vector<double> gradient;
	gradient = (J1gradient + 2 * J2gradient + 4 * J3gradient + J4gradient)/(9 * l * k);
	return gradient;
}
double ComputeConformalEnergy(CNurbsSuface* pSurface)
{

	C_NurbsSurf* pNSurface = pSurface->Converte_C_NurbsSurface();
	BsplineTransformation* pBsplineTrans = (BsplineTransformation*)pSurface->ptransformation;

	int l = L_SAMPLING;
	int k = K_SAMPLING;

	double mins,maxs,mint,maxt;
	if(pBsplineTrans)
	{
		mins = pBsplineTrans->pNurbsSurf->GetUKnot(0);
		maxs = pBsplineTrans->pNurbsSurf->GetUKnot(pBsplineTrans->pNurbsSurf->GetCtrlNumberU() + pBsplineTrans->pNurbsSurf->GetDegreeU());

		mint= pBsplineTrans->pNurbsSurf->GetVKnot(0);
		maxt = pBsplineTrans->pNurbsSurf->GetVKnot(pBsplineTrans->pNurbsSurf->GetCtrlNumberV() + pBsplineTrans->pNurbsSurf->GetDegreeV());
	}else
	{
		mins = pNSurface->GetUKnot(0);
		maxs = pNSurface->GetUKnot(pNSurface->GetCtrlNumberU() - 1);
		mint = pNSurface->GetVKnot(0);
		maxt = pNSurface->GetVKnot(pNSurface->GetCtrlNumberV() - 1);
	}

	double u,v;
	double sum = 0;
	std::vector<double> sarray;
	std::vector<double> tarray;
	for(int i = 0; i <= 2 * l; i++)
	{
		double s = mins + i * (maxs - mins)/ (2.0 * l);
		sarray.push_back(s);
	}
	for(int j = 0; j <= 2 * k; j++)
	{
		double t = mint + j * (maxt - mint)/ (2.0 * k);
		tarray.push_back(t);
	}

	double J1 = 0;
	double J2 = 0;
	double J3 = 0;
	double J4 = 0;

	J1 += ComputeEnergy(pNSurface,pBsplineTrans,sarray[0],tarray[0]);
	for(int i = 1; i <= l - 1; i++)
		J1 += 2 * ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * i],tarray[0]);
	for(int i = 1; i<= l; i++)
		J1 += 4 *ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * i -1],tarray[0]);
	J1 += ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * l],tarray[0]);

	for(int j = 1; j<= k - 1; j++)
		J2 += ComputeEnergy(pNSurface,pBsplineTrans,sarray[0],tarray[2 * j]);
	for(int j = 1; j <= k - 1; j++)
		for(int i= 1; i <= l - 1; i++)
			J2 += 2 * ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * i],tarray[2 * j]);
	for(int j = 1; j <= k - 1; j++)
		for(int i = 1; i<= l; i++)
			J2 += 4 * ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * i -1],tarray[2 * j]);
	for(int j = 1; j<= k - 1; j++)
		J2 += ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * l],tarray[2 * j]);

	for(int j = 1; j<= k; j++)
		J3 += ComputeEnergy(pNSurface,pBsplineTrans,sarray[0],tarray[2 * j - 1]);
	for(int j = 1; j <= k; j++)
		for(int i= 1; i <= l - 1; i++)
			J3 += 2 * ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * i],tarray[2 * j - 1]);
	for(int j = 1; j <= k; j++)
		for(int i = 1; i<= l; i++)
			J3 += 4 * ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * i -1],tarray[2 * j - 1]);
	for(int j = 1; j<= k ; j++)
		J3 += ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * l],tarray[2 * j - 1]);

	J4 +=ComputeEnergy(pNSurface,pBsplineTrans,sarray[0],tarray[2 * k]);
	for(int i = 1; i <= l - 1; i++)
		J4 += 2 * ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * i],tarray[2 * k]);
	for(int i = 1; i<= l; i++)
		J4 += 4 *ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * i -1],tarray[2 * k]);
	J4 += ComputeEnergy(pNSurface,pBsplineTrans,sarray[2 * l],tarray[2 * k]);

	double func = (J1 + 2 * J2 + 4 * J3 + J4)/(9 * l * k);
	return func;
}

int iterationnum;
std::ofstream file;
void ConformalEnergy(const real_1d_array &x, double &func,  real_1d_array &grad, void *ptr)
{
	//CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	//ASSERT_VALID(pDoc);
	TRACE("---------------------------------------------------------------------\nx is :");
	for(int i = 0; i< x.length(); i++)
		TRACE("%5.2f\t", x[i]);
	TRACE("----------------------------------------------------------------------\n");
	iterationnum++;
	//char message[500];
	//sprintf(Post_Message,"Iteration: %d",iterationnum);
	//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

	CNurbsSuface* pSurface = ((CNurbsSuface*)pDoc->m_SceneGraph.GetAt(4))->copy();
	//CNurbsSuface* pSurface = ((CNurbsSuface*)pDoc->m_SceneGraph.GetAt(4));
	C_NurbsSurf* pNSurface = pSurface->Converte_C_NurbsSurface();
	BsplineTransformation* pBsplineTrans = (BsplineTransformation*)pSurface->ptransformation;

	//Set the transformation
	int num = 0;
	for(int j = 1; j < pBsplineTrans->pNurbsSurf->GetCtrlNumberV() - 1; j++)
		for(int i = 1; i < pBsplineTrans->pNurbsSurf->GetCtrlNumberU() - 1; i++)
		{
			Point3D point;
			point.x = x[num * 2];
			point.y = x[num * 2 + 1];
			point.z = 0;

			pBsplineTrans->pNurbsSurf->SetControlPoint(i,j,point);
			num++;
		}

	//not valid
	if(!IsValidBsplineTransformation(pBsplineTrans))
	{
		func = 1e10;
		return;
	}

	CNurbsSuface* pSurface1 = ((C_NurbsSurf*)((C_NurbsSurf*)(pBsplineTrans->pNurbsSurf))->Copy())->GetCNurbsSurface();
	pSurface1->SetTransform(*pSurface->GetTransform());

	//energy
	func = ComputeConformalEnergy(pSurface);
	
	//sprintf(Post_Message,"Iteration: %d, function : %f",iterationnum,func);
	//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

	//gradient
	std::vector<double> gradient = ComputeConformalGradient(pSurface,x.length());

	TRACE("iteration is : %d\n",iterationnum);

	//save information to file
	file <<endl;
	file << " ----------------------------------------------- " <<endl;
	file << "Iteration: " << iterationnum <<"  ";
	file << "Function: " << func << endl;
	file << "Gradient: ";
	for(int i = 0; i < gradient.size(); i++)
		file << gradient[i] << "  ";
	file <<endl;
	file << "current x is :";
	for(int i = 0; i< x.length(); i++)
		file << x[i] << "  ";
	file <<endl << "------------------------------------------------" << endl;

	sprintf(Post_Message,"Iteration%d_Surface%5.6f",iterationnum,func);
	pSurface->filename = Post_Message;
	pDoc->m_SceneGraph.Add(pSurface);

	sprintf(Post_Message,"Iteration%d_Transfomation%5.6f",iterationnum,func);
	pSurface1->filename = Post_Message;
	pDoc->m_SceneGraph.Add(pSurface1);

	//TRACE(" \n func = %5.6f",func);

	for(int i = 0; i < gradient.size(); i++)
	{
		grad[i] = gradient[i];
	}

	//::PostMessage(g_hwndMain, WM_UPDATE_TREECONTROL, 0, 0);
}
UINT Actions_LeastSquare::ComputeCoefficient_y(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,double u,double v,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,std::vector<double>* c,int c1,int c2,std::vector<double> *Jcoeff,double scalingnumber)
{

	ThreadParam_ComputeCoefficient* pThreadParam = new ThreadParam_ComputeCoefficient();
	pThreadParam->pSurface = pSurface;
	pThreadParam->pNurbs = pNurbs;
	pThreadParam->u = u;
	pThreadParam->v = v;
	pThreadParam->poriginalmesh = poriginalmesh;
	pThreadParam->pconformalmesh = pconformalmesh;
	pThreadParam->c = c;
	pThreadParam->c1 = c1;
	pThreadParam->c2 = c2;
	pThreadParam->Jcoeff = Jcoeff;
	pThreadParam->scalingnumber = scalingnumber;

	g_pThread = AfxBeginThread(WorkingThread_ComputeCoefficient_y, 
		(LPVOID)pThreadParam,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	g_pThread->m_bAutoDelete = TRUE;
	g_pThread->ResumeThread();
	return 1;
}
UINT Actions_LeastSquare::ComputeCoefficient_y_b(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,double u,double v,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double* c,int c1,int c2,double* Jcoeff,double scalingnumber)
{
	ThreadParam_ComputeCoefficient_b* pThreadParam = new ThreadParam_ComputeCoefficient_b();
	pThreadParam->pSurface = pSurface;
	pThreadParam->pNurbs = pNurbs;
	pThreadParam->u = u;
	pThreadParam->v = v;
	pThreadParam->poriginalmesh = poriginalmesh;
	pThreadParam->pconformalmesh = pconformalmesh;
	pThreadParam->c = c;
	pThreadParam->c1 = c1;
	pThreadParam->c2 = c2;
	pThreadParam->Jcoeff = Jcoeff;
	pThreadParam->scalingnumber = scalingnumber;

	g_pThread = AfxBeginThread(WorkingThread_ComputeCoefficient_y_b, 
		(LPVOID)pThreadParam,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	g_pThread->m_bAutoDelete = TRUE;
	g_pThread->ResumeThread();
	return 1;
}
void Actions_LeastSquare::FindCorrespondingPoint(CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double u,double v,double &s,double &t)
{
	Vector3D p(u,v,0);

	//Set Index for the two meshes.
	poriginalmesh->SetFlagOnVertices_ArrayIndex();
	pconformalmesh->SetFlagOnVertices_ArrayIndex();

	//Given a (u,v) point in the original mesh, find the corresponding point (s,t) in the conformal mesh
	for(int i = 0; i < poriginalmesh->NbFace(); i++)
	{
		CFace3d* pFace = poriginalmesh->GetFace(i);
		CVertex3d* pVertex1_originalmesh = pFace->v1();
		CVertex3d* pVertex2_originalmesh = pFace->v2();
		CVertex3d* pVertex3_originalmesh = pFace->v3();

		Vector3D v1_originalmesh(pVertex1_originalmesh->x(),pVertex1_originalmesh->y(),0);
		Vector3D v2_originalmesh(pVertex2_originalmesh->x(),pVertex2_originalmesh->y(),0);
		Vector3D v3_originalmesh(pVertex3_originalmesh->x(),pVertex3_originalmesh->y(),0);

		BOOL inTriangle = poriginalmesh->JudeWhetherPointLocatedInTriangle(u,v,pVertex1_originalmesh->x(),pVertex1_originalmesh->y(),
			pVertex2_originalmesh->x(),pVertex2_originalmesh->y(),pVertex3_originalmesh->x(),pVertex3_originalmesh->y());

		//Judge whether the normals are in the same directions.
		Vector3D v1v2 = v2_originalmesh - v1_originalmesh;
		Vector3D v2p = p - v2_originalmesh;
		Vector3D n1 = v1v2 ^ v2p;

		Vector3D v2v3 = v3_originalmesh - v2_originalmesh;
		Vector3D v3p = p - v3_originalmesh;
		Vector3D n2 = v2v3 ^ v3p;

		Vector3D v3v1 = v1_originalmesh - v3_originalmesh;
		Vector3D v1p = p - v1_originalmesh;
		Vector3D n3 = v3v1 ^ v1p;
		
		int index1 = pVertex1_originalmesh->GetFlag();
		int index2 = pVertex2_originalmesh->GetFlag();
		int index3 = pVertex3_originalmesh->GetFlag();

		Vector3D n = v1v2 ^ v2v3;
		if(n1[2] >= 0 && n2[2] >= 0 && n3[2] >= 0 ||
		   n1[2] <= 0 && n2[2] <= 0 && n3[2] <= 0)
		//if(inTriangle)
		{
			double v1_weight = n2[2] / n[2];
			double v2_weight = n3[2] / n[2];
			double v3_weight = n1[2] / n[2];
			
			CVertex3d* pVertex1_conformalmesh = pconformalmesh->GetVertex(index1);
			CVertex3d* pVertex2_conformalmesh = pconformalmesh->GetVertex(index2);
			CVertex3d* pVertex3_conformalmesh = pconformalmesh->GetVertex(index3);

			Vector3D v1_conformalmesh(pVertex1_conformalmesh->x(),pVertex1_conformalmesh->y(),0);
			Vector3D v2_conformalmesh(pVertex2_conformalmesh->x(),pVertex2_conformalmesh->y(),0);
			Vector3D v3_conformalmesh(pVertex3_conformalmesh->x(),pVertex3_conformalmesh->y(),0);

			Vector3D point = v1_conformalmesh * v1_weight + v2_conformalmesh * v2_weight + v3_conformalmesh * v3_weight;
			s = point.x;
			t = point.y;
			return;
		}
	}
}
UINT Actions_LeastSquare::ComputeCoefficient_x_b(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,double u,double v,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double* c,int c1,int c2,double* Jcoeff,double scalingnumber)
{
	ThreadParam_ComputeCoefficient_b* pThreadParam = new ThreadParam_ComputeCoefficient_b();
	pThreadParam->pSurface = pSurface;
	pThreadParam->pNurbs = pNurbs;
	pThreadParam->u = u;
	pThreadParam->v = v;
	pThreadParam->poriginalmesh = poriginalmesh;
	pThreadParam->pconformalmesh = pconformalmesh;
	pThreadParam->c = c;
	pThreadParam->c1 = c1;
	pThreadParam->c2 = c2;
	pThreadParam->Jcoeff = Jcoeff;
	pThreadParam->scalingnumber = scalingnumber;

	g_pThread = AfxBeginThread(WorkingThread_ComputeCoefficient_x_b, 
		(LPVOID)pThreadParam,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	g_pThread->m_bAutoDelete = TRUE;
	g_pThread->ResumeThread();
	return 1;
}
CRITICAL_SECTION g_csA;
UINT WorkingThread_ComputeCoefficient_y_b(LPVOID pParam)
{
	EnterCriticalSection(&g_csA);
	Actions_LeastSquare::ComputeCoefficient_y_b_threadnumber++;
	LeaveCriticalSection(&g_csA);
	TRACE("Thread Number:%d\n",Actions_LeastSquare::ComputeCoefficient_y_b_threadnumber);
	ThreadParam_ComputeCoefficient_b* pThreadParam = (ThreadParam_ComputeCoefficient_b*)pParam;
	C_NurbsSurf* pSurface = pThreadParam->pSurface;
	C_NurbsSurf* pNurbs = pThreadParam->pNurbs;
	double u = pThreadParam->u;
	double v = pThreadParam->v;
	CMesh3d* poriginalmesh = pThreadParam->poriginalmesh;
	CMesh3d* pconformalmesh = pThreadParam->pconformalmesh;
	double* c = pThreadParam->c;
	int c1 = pThreadParam->c1;
	int c2 = pThreadParam->c2;
	double* Jcoeff = pThreadParam->Jcoeff;
	double scalingnumber = pThreadParam->scalingnumber;
	
	int ns = pNurbs->GetCtrlNumberU();
	int nt = pNurbs->GetCtrlNumberV();

	/*double mindis = 1e10;
	int minindex;
	for(int i = 0; i < poriginalmesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = poriginalmesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();

		double dis = SQR(u - x) + SQR(v - y);
		if(dis < mindis)
		{
			mindis = dis;
			minindex = i;
		}
	}
	CVertex3d* pVertex1 = pconformalmesh->GetVertex(minindex);
	CVertex3d* pVertex2 = poriginalmesh->GetVertex(minindex);

	double s = pVertex1->x();
	double t = pVertex1->y();*/
	double s,t;
	Actions_LeastSquare::FindCorrespondingPoint(poriginalmesh,pconformalmesh,u,v,s,t);

	Vector3D Xu = pSurface->ComputUDerivative1(u,v);
	Vector3D Xv = pSurface->ComputVDerivative1(u,v);
	double E = Xu * Xu;
	double F = Xu * Xv;
	double G = Xv * Xv;

	double denorm = sqrt(E * G - F * F);
	double m11 = -F / denorm;
	double m12 = -G / denorm;
	double m21 = E / denorm;
	double m22 = F / denorm;
	//char message[500];
	std::vector<double> coeff;

	Point3D p00 = pNurbs->GetControlPoint(0,0);
	Point3D pm0 = pNurbs->GetControlPoint(ns - 1,0);
	Point3D p0n = pNurbs->GetControlPoint(0,nt - 1);
	Point3D pmn = pNurbs->GetControlPoint(ns - 1, nt - 1);

	//should modify this part of code
	//-m12 * partial_c1_s * c2_t
	double term1 = -m12 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
	//phi_tx
	double term21 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < nt; j++)
		{
			term21 += -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(j,t) * p00.x;
			term21 += -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(j,t) * pmn.x;
		}
	}else
	{
		term21 = -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(0,t) * p00.x 
				 -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(0,t) * pm0.x
			     -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * p0n.x
			     -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * pmn.x;
	}

	double term22 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < nt; j++)
		{
			term22 += m11 * (pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(j,t) * p00.x);
			term22 += m11 * (pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(j,t) * pmn.x);
		}
	}else
	{
		term22 = m11 * (pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(0,t) * p00.x + 
		         pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(0,t) * pm0.x + 
		         pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(nt - 1,t) * p0n.x + 
		         pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.x);
	}

	double term23 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < ns; j++)
		{
			term23 += m12 * (pNurbs->ComputeUBasis_PartialU(j,s) * pNurbs->ComputeVBasis(0,t) * p00.y);
			term23 += m12 * (pNurbs->ComputeUBasis_PartialU(j,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.y);
		}
	}else
	{
		term23 = m12 * (pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(0,t) * p00.y + 
						pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(0,t) * pm0.y + 
						pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(nt - 1,t) * p0n.y + 
						pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.y);
	}


	double term3 = pNurbs->ComputeUBasis(c1,s) * pNurbs->ComputeVBasis_PartialV(c2,t) - 
		           m22 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
	double term41 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < ns; j++)
		{
			term41 += -pNurbs->ComputeUBasis(j,s) * pNurbs->ComputeVBasis_PartialV(0,t) * p00.y;
			term41 += -pNurbs->ComputeUBasis(j,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * pmn.y;
		}
	}else
	{
		term41 = -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(0,t) * p00.y 
				 -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(0,t) * pm0.y
				 -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * p0n.y
				 -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * pmn.y;
	}


	double term42 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < nt; j++)
		{
			term42 += m21 * ( pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(j,t) * p00.x);
			term42 += m21 * ( pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(j,t) * pmn.x);
		}
	}else
	{
		term42 = m21 * ( pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(0,t) * p00.x + 
				pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(0,t) * pm0.x + 
				pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(nt - 1,t) * p0n.x + 
				pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.x);
	}

	double term43 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < ns; j++)
		{
			term43 += m22 * ( pNurbs->ComputeUBasis_PartialU(j,s) * pNurbs->ComputeVBasis(0,t) * p00.y); 
			term43 += m22 * ( pNurbs->ComputeUBasis_PartialU(j,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.y); 
		}
	}else
	{
		term43 = m22 * ( pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(0,t) * p00.y + 
				pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(0,t) * pm0.y + 
				pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(nt - 1,t) * p0n.y + 
				pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.y);
	}

	double val = term1 * (term21 + term22 + term23) + term3 * (term41 + term42 + term43);
	
	//Lock
	EnterCriticalSection(&g_csA);
	*Jcoeff= *Jcoeff + val * scalingnumber;
	Actions_LeastSquare::ComputeCoefficient_y_b_threadnumber--;
	LeaveCriticalSection(&g_csA);
	return 1;
	
	//return val;

}
UINT WorkingThread_ComputeCoefficient_x_b(LPVOID pParam)
{
	EnterCriticalSection(&g_csA);
	Actions_LeastSquare::ComputeCoefficient_x_b_threadnumber++;
	LeaveCriticalSection(&g_csA);
	TRACE("Thread Number:%d\n",Actions_LeastSquare::ComputeCoefficient_x_b_threadnumber);
	ThreadParam_ComputeCoefficient_b* pThreadParam = (ThreadParam_ComputeCoefficient_b*)pParam;
	C_NurbsSurf* pSurface = pThreadParam->pSurface;
	C_NurbsSurf* pNurbs = pThreadParam->pNurbs;
	double u = pThreadParam->u;
	double v = pThreadParam->v;
	CMesh3d* poriginalmesh = pThreadParam->poriginalmesh;
	CMesh3d* pconformalmesh = pThreadParam->pconformalmesh;
	double* c = pThreadParam->c;
	int c1 = pThreadParam->c1;
	int c2 = pThreadParam->c2;
	double* Jcoeff = pThreadParam->Jcoeff;
	double scalingnumber = pThreadParam->scalingnumber;
	
	int ns = pNurbs->GetCtrlNumberU();
	int nt = pNurbs->GetCtrlNumberV();


	/*double mindis = 1e10;
	int minindex;
	for(int i = 0; i < poriginalmesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = poriginalmesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();

		double dis = SQR(u - x) + SQR(v - y);
		if(dis < mindis)
		{
			mindis = dis;
			minindex = i;
		}
	}
	CVertex3d* pVertex1 = pconformalmesh->GetVertex(minindex);
	CVertex3d* pVertex2 = poriginalmesh->GetVertex(minindex);*/
	double s,t;
	Actions_LeastSquare::FindCorrespondingPoint(poriginalmesh,pconformalmesh,u,v,s,t);

	//double s = pVertex1->x();
	//double t = pVertex1->y();

	Vector3D Xu = pSurface->ComputUDerivative1(u,v);
	Vector3D Xv = pSurface->ComputVDerivative1(u,v);
	double E = Xu * Xu;
	double F = Xu * Xv;
	double G = Xv * Xv;

	double denorm = sqrt(E * G - F * F);
	double m11 = -F / denorm;
	double m12 = -G / denorm;
	double m21 = E / denorm;
	double m22 = F / denorm;
	//char message[500];
	std::vector<double> coeff;

	Point3D p00 = pNurbs->GetControlPoint(0,0);
	Point3D pm0 = pNurbs->GetControlPoint(ns - 1,0);
	Point3D p0n = pNurbs->GetControlPoint(0,nt - 1);
	Point3D pmn = pNurbs->GetControlPoint(ns - 1, nt - 1);
	double term1 = pNurbs->ComputeUBasis(c1,s) * pNurbs->ComputeVBasis_PartialV(c2,t) - 
				   m11 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
	double term21 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < nt; j++)
		{
			term21 += -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(j,t) * p00.x;
			term21 += -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(j,t) * pmn.x;
		}
	}else
	{
		term21 = -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(0,t) * p00.x 
			     -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(0,t) * pm0.x
				 -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * p0n.x
				 -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * pmn.x;
	}
	
	double term22 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < nt; j++)
		{
			term22 += m11 * (pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(j,t) * p00.x);
			term22 += m11 * (pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(j,t) * pmn.x);
		}
	}else
	{
		term22 = m11 * (pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(0,t) * p00.x + 
			     pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(0,t) * pm0.x + 
				 pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(nt - 1,t) * p0n.x + 
			     pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.x);
	}

	double term23 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < ns; j++)
		{
			term23 += m12 * (pNurbs->ComputeUBasis_PartialU(j,s) * pNurbs->ComputeVBasis(0,t) * p00.y);
			term23 += m12 * (pNurbs->ComputeUBasis_PartialU(j,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.y);
		}
	}else
	{
		term23 = m12 * (pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(0,t) * p00.y + 
						pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(0,t) * pm0.y + 
						pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(nt - 1,t) * p0n.y + 
						pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.y);
	}

	double term3 = -m21 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
	double term41 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < ns; j++)
		{
			term41 += -pNurbs->ComputeUBasis(j,s) * pNurbs->ComputeVBasis_PartialV(0,t) * p00.y;
			term41 += -pNurbs->ComputeUBasis(j,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * pmn.y;
		}
	}else
	{
		term41 = -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(0,t) * p00.y 
				 -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(0,t) * pm0.y
				 -pNurbs->ComputeUBasis(0,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * p0n.y
				 -pNurbs->ComputeUBasis(ns - 1,s) * pNurbs->ComputeVBasis_PartialV(nt - 1,t) * pmn.y;
	}

	double term42 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < nt; j++)
		{
			term42 += m21 * ( pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(j,t) * p00.x);
			term42 += m21 * ( pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(j,t) * pmn.x);
		}
	}else
	{
		term42 = m21 * ( pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(0,t) * p00.x + 
						 pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(0,t) * pm0.x + 
						 pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(nt - 1,t) * p0n.x + 
						 pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.x);
	}

	double term43 = 0;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		for(int j = 0; j < ns; j++)
		{
			term43 += m22 * ( pNurbs->ComputeUBasis_PartialU(j,s) * pNurbs->ComputeVBasis(0,t) * p00.y);
			term43 += m22 * ( pNurbs->ComputeUBasis_PartialU(j,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.y);
		}
	}else
	{
		term43 = m22 * ( pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(0,t) * p00.y + 
						 pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(0,t) * pm0.y + 
						 pNurbs->ComputeUBasis_PartialU(0,s) * pNurbs->ComputeVBasis(nt - 1,t) * p0n.y + 
						 pNurbs->ComputeUBasis_PartialU(ns - 1,s) * pNurbs->ComputeVBasis(nt - 1,t) * pmn.y);
	}

	double val = term1 * (term21 + term22 + term23) + term3 * (term41 + term42 + term43);
	
	//Lock
	EnterCriticalSection(&g_csA);
	*Jcoeff= *Jcoeff + val * scalingnumber;
	Actions_LeastSquare::ComputeCoefficient_x_b_threadnumber--;
	LeaveCriticalSection(&g_csA);
	return 1;
}
UINT WorkingThread_ComputeCoefficient_y(LPVOID pParam)
{
	EnterCriticalSection(&g_csA);
	Actions_LeastSquare::ComputeCoefficient_y_threadnumber++;
	LeaveCriticalSection(&g_csA);
	TRACE("Thread Number:%d\n",Actions_LeastSquare::ComputeCoefficient_y_threadnumber);
	ThreadParam_ComputeCoefficient* pThreadParam = (ThreadParam_ComputeCoefficient*)pParam;
	C_NurbsSurf* pSurface = pThreadParam->pSurface;
	C_NurbsSurf* pNurbs = pThreadParam->pNurbs;
	double u = pThreadParam->u;
	double v = pThreadParam->v;
	CMesh3d* poriginalmesh = pThreadParam->poriginalmesh;
	CMesh3d* pconformalmesh = pThreadParam->pconformalmesh;
	std::vector<double>* c = pThreadParam->c;
	int c1 = pThreadParam->c1;
	int c2 = pThreadParam->c2;
	std::vector<double>* Jcoeff = pThreadParam->Jcoeff;
	double scalingnumber = pThreadParam->scalingnumber;

	//modify
	int ns = pNurbs->GetCtrlNumberU();
	int nt = pNurbs->GetCtrlNumberV();

	/*double mindis = 1e10;
	int minindex;
	for(int i = 0; i < poriginalmesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = poriginalmesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();

		double dis = SQR(u - x) + SQR(v - y);
		if(dis < mindis)
		{
			mindis = dis;
			minindex = i;
		}
	}
	CVertex3d* pVertex1 = pconformalmesh->GetVertex(minindex);
	CVertex3d* pVertex2 = poriginalmesh->GetVertex(minindex);

	double s = pVertex1->x();
	double t = pVertex1->y();*/

	double s,t;
	Actions_LeastSquare::FindCorrespondingPoint(poriginalmesh,pconformalmesh,u,v,s,t);

	Vector3D Xu = pSurface->ComputUDerivative1(u,v);
	Vector3D Xv = pSurface->ComputVDerivative1(u,v);
	double E = Xu * Xu;
	double F = Xu * Xv;
	double G = Xv * Xv;

	double denorm = sqrt(E * G - F * F);
	double m11 = -F / denorm;
	double m12 = -G / denorm;
	double m21 = E / denorm;
	double m22 = F / denorm;

	//char message[500];
	std::vector<double> coeff;

	int c1lower,c1upper,c2lower,c2upper;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		c1lower = 1;
		c1upper = ns - 1;
		c2lower = 1;
		c2upper = nt - 1;
	}
	else
	{
		c1lower = 0;
		c1upper = ns;
		c2lower = 0;
		c2upper = nt;
	}

	//x
	for(int j = 0; j < nt; j++)
		for(int i = c1lower; i < c1upper; i++)
		{
			//sprintf(Post_Message,"Computing Coefficient y %3.2f%%",(j) * 100.0 /(nt));
			//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
			if(!Actions_LeastSquare::m_FixBoundary)
				if(i == 0 && j == 0 || i == 0 && j == nt - 1 || i == ns - 1 && j == 0 || i == ns - 1 && j == nt - 1)
					continue;

			double term1 = -m12 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
			double term2 = pNurbs->ComputeUBasis(i,s) * pNurbs->ComputeVBasis_PartialV(j,t) - 
				m11 * (pNurbs->ComputeUBasis_PartialU(i,s) * pNurbs->ComputeVBasis(j,t));
			double term3 = pNurbs->ComputeUBasis(c1,s) * pNurbs->ComputeVBasis_PartialV(c2,t) - 
				m22 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
			double term4 = -m21 * pNurbs->ComputeUBasis_PartialU(i,s) * pNurbs->ComputeVBasis(j,t);
			double val = term1 * term2 + term3 * term4;
			coeff.push_back(val);
		}
		//y
		for(int j = c2lower; j < c2upper; j++)
			for(int i = 0; i < ns; i++)
			{
				//sprintf(Post_Message,"Computing Coefficient y %3.2f%%",(j) * 100.0 /(nt));
				//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
				if(!Actions_LeastSquare::m_FixBoundary)
					if(i == 0 && j == 0 || i == 0 && j == nt - 1 || i == ns - 1 && j == 0 || i == ns - 1 && j == nt - 1)
						continue;

				double term1 = -m12 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
				double term2 = -m12 * pNurbs->ComputeUBasis_PartialU(i,s) * pNurbs->ComputeVBasis(j,t);
				double term3 = pNurbs->ComputeUBasis(c1,s) * pNurbs->ComputeVBasis_PartialV(c2,t) - 
							   m22 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
				double term4 = pNurbs->ComputeUBasis(i,s) * pNurbs->ComputeVBasis_PartialV(j,t) - 
							   m22 * pNurbs->ComputeUBasis_PartialU(i,s) * pNurbs->ComputeVBasis(j,t);
				double val = term1 * term2 + term3 * term4;
				coeff.push_back(val);
			}

			int num = c->size() - coeff.size();
			for(int i = 0; i < num; i++)
				coeff.push_back(0);

			//Lock
			EnterCriticalSection(&g_csA);
			*Jcoeff = *Jcoeff + coeff * scalingnumber;
			Actions_LeastSquare::ComputeCoefficient_y_threadnumber--;
			LeaveCriticalSection(&g_csA);
			return 1;
}
UINT WorkingThread_ComputeCoefficient_x(LPVOID pParam)
{
	EnterCriticalSection(&g_csA);
	Actions_LeastSquare::ComputeCoefficient_x_threadnumber++;
	LeaveCriticalSection(&g_csA);
	TRACE("Thread Number:%d\n",Actions_LeastSquare::ComputeCoefficient_x_threadnumber);
	ThreadParam_ComputeCoefficient* pThreadParam = (ThreadParam_ComputeCoefficient*)pParam;
	C_NurbsSurf* pSurface = pThreadParam->pSurface;
	C_NurbsSurf* pNurbs = pThreadParam->pNurbs;
	double u = pThreadParam->u;
	double v = pThreadParam->v;
	CMesh3d* poriginalmesh = pThreadParam->poriginalmesh;
	CMesh3d* pconformalmesh = pThreadParam->pconformalmesh;
	std::vector<double>* c = pThreadParam->c;
	int c1 = pThreadParam->c1;
	int c2 = pThreadParam->c2;
	std::vector<double>* Jcoeff = pThreadParam->Jcoeff;
	double scalingnumber = pThreadParam->scalingnumber;

	int ns = pNurbs->GetCtrlNumberU();
	int nt = pNurbs->GetCtrlNumberV();

	/*double mindis = 1e10;
	int minindex;
	for(int i = 0; i < poriginalmesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = poriginalmesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();

		double dis = SQR(u - x) + SQR(v - y);
		if(dis < mindis)
		{
			mindis = dis;
			minindex = i;
		}
	}
	CVertex3d* pVertex1 = pconformalmesh->GetVertex(minindex);
	CVertex3d* pVertex2 = poriginalmesh->GetVertex(minindex);

	double s = pVertex1->x();
	double t = pVertex1->y();*/

	double s,t;
	Actions_LeastSquare::FindCorrespondingPoint(poriginalmesh,pconformalmesh,u,v,s,t);

	Vector3D Xu = pSurface->ComputUDerivative1(u,v);
	Vector3D Xv = pSurface->ComputVDerivative1(u,v);
	double E = Xu * Xu;
	double F = Xu * Xv;
	double G = Xv * Xv;

	double denorm = sqrt(E * G - F * F);
	double m11 = -F / denorm;
	double m12 = -G / denorm;
	double m21 = E / denorm;
	double m22 = F / denorm;
	//char message[500];
	std::vector<double> coeff;

	int c1lower,c1upper,c2lower,c2upper;
	if(Actions_LeastSquare::m_FixBoundary)
	{
		c1lower = 1;
		c1upper = ns - 1;
		c2lower = 1;
		c2upper = nt - 1;
	}
	else
	{
		c1lower = 0;
		c1upper = ns;
		c2lower = 0;
		c2upper = nt;
	}
	//x
	for(int j = 0; j < nt; j++)
		for(int i = c1lower; i < c1upper; i++)
		{
			//sprintf(Post_Message,"Computing Coefficient X %3.2f%%",(j) * 100.0 /(nt - 1));
			//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

			if(!Actions_LeastSquare::m_FixBoundary)
				if(i == 0 && j == 0 || i == 0 && j == nt - 1 || i == ns - 1 && j == 0 || i == ns - 1 && j == nt - 1)
					continue;
			
			double term1 = pNurbs->ComputeUBasis(c1,s) * pNurbs->ComputeVBasis_PartialV(c2,t) - 
							m11 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
			double term2 = pNurbs->ComputeUBasis(i,s) * pNurbs->ComputeVBasis_PartialV(j,t) - 
							m11 * (pNurbs->ComputeUBasis_PartialU(i,s) * pNurbs->ComputeVBasis(j,t));
			double term3 = m21 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
			double term4 = m21 * pNurbs->ComputeUBasis_PartialU(i,s) * pNurbs->ComputeVBasis(j,t);
			double val = term1 * term2 + term3 * term4;
			coeff.push_back(val);
		}
	//y
	for(int j = c2lower; j < c2upper; j++)
		for(int i = 0; i < ns; i++)
		{
			//sprintf(Post_Message,"Computing Coefficient X %3.2f%%",(j) * 100.0 /(c2upper - c2lower - 1));
			//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
			if(!Actions_LeastSquare::m_FixBoundary)
				if(i == 0 && j == 0 || i == 0 && j == nt - 1 || i == ns - 1 && j == 0 || i == ns - 1 && j == nt - 1)
					continue;

			double term1 = pNurbs->ComputeUBasis(c1,s) * pNurbs->ComputeVBasis_PartialV(c2,t) - 
							m11 * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
			double term2 = -m12 * pNurbs->ComputeUBasis_PartialU(i,s) * pNurbs->ComputeVBasis(j,t);
			double term3 = (-m21) * pNurbs->ComputeUBasis_PartialU(c1,s) * pNurbs->ComputeVBasis(c2,t);
			double term4 = pNurbs->ComputeUBasis(i,s) * pNurbs->ComputeVBasis_PartialV(j,t) - m22 * 
							pNurbs->ComputeUBasis_PartialU(i,s) * pNurbs->ComputeVBasis(j,t);
			double val = term1 * term2 + term3 * term4;
			coeff.push_back(val);
		}
	
	int num = c->size() - coeff.size();
	for(int i = 0; i < num; i++)
		coeff.push_back(0);

	//sprintf(Post_Message,"Computing Coefficient X Finish");
	//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

	//Lock
	EnterCriticalSection(&g_csA);
	*Jcoeff = *Jcoeff + coeff * scalingnumber;
		Actions_LeastSquare::ComputeCoefficient_x_threadnumber--;
	LeaveCriticalSection(&g_csA);
	return 1;
}
UINT Actions_LeastSquare::ComputeCoefficient_x(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,double u,double v,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,std::vector<double>* c,int c1,int c2,std::vector<double> *Jcoeff,double scalingnumber)
{

	ThreadParam_ComputeCoefficient* pThreadParam = new ThreadParam_ComputeCoefficient();
	pThreadParam->pSurface = pSurface;
	pThreadParam->pNurbs = pNurbs;
	pThreadParam->u = u;
	pThreadParam->v = v;
	pThreadParam->poriginalmesh = poriginalmesh;
	pThreadParam->pconformalmesh = pconformalmesh;
	pThreadParam->c = c;
	pThreadParam->c1 = c1;
	pThreadParam->c2 = c2;
	pThreadParam->Jcoeff = Jcoeff;
	pThreadParam->scalingnumber = scalingnumber;

	g_pThread = AfxBeginThread(WorkingThread_ComputeCoefficient_x, 
		(LPVOID)pThreadParam,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	g_pThread->m_bAutoDelete = TRUE;
	g_pThread->ResumeThread();
	return 1;
}

void Actions_LeastSquare::ComputerConformalityCoefficient_CompositeSimpson_yderivative(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,int l,int k,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,std::vector<double>& c,int c1,int c2,double weight)
{
	sprintf(Post_Message,"Compute Conformality Coefficient Composite Simpson yderivative...");
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	InitializeCriticalSection(&g_csA);
	Actions_LeastSquare::ComputeCoefficient_y_threadnumber = 0;

	double minu = pNurbs->GetControlPoint(0,0).x;
	double maxu = pNurbs->GetControlPoint(pNurbs->GetCtrlNumberU() - 1,0).x;

	double minv= pNurbs->GetControlPoint(0,0).y;
	double maxv = pNurbs->GetControlPoint(0,pNurbs->GetCtrlNumberV() - 1).y;

	double u,v;
	double sum = 0;
	std::vector<double> uarray;
	std::vector<double> varray;
	for(int i = 0; i <= 2 * l; i++)
	{
		double u = minu + i * (maxu - minu)/ (2.0 * l);
		uarray.push_back(u);
	}
	for(int j = 0; j <= 2 * k; j++)
	{
		double v = minv + j * (maxv - minv)/ (2.0 * k);
		varray.push_back(v);
	}

	//compute the coefficient.
	std::vector<double> temp;
	std::vector<double> Jcoeff;
	Jcoeff.clear();

	for(int i = 0; i < c.size(); i++)
	{
		Jcoeff.push_back(0);
	}

	double scalingnumber = 1.0 / (9 * l * k);
	ComputeCoefficient_y(pSurface,pNurbs,uarray[0],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	for(int i = 1; i <= l - 1; i++)
	{
		ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * i],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
	}
	for(int i = 1; i<= l; i++)
	{
		ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * i -1],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
	}

	ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * l],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	scalingnumber = 2.0 / (9 * l * k);
	for(int j = 1; j<= k - 1; j++)
	{
		ComputeCoefficient_y(pSurface,pNurbs,uarray[0],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}
	for(int j = 1; j <= k - 1; j++)
		for(int i= 1; i <= l - 1; i++)
		{
			ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * i],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
		}
	for(int j = 1; j <= k - 1; j++)
		for(int i = 1; i<= l; i++)
		{
			ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * i -1],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
		}
	for(int j = 1; j<= k - 1; j++)
	{
		ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * l],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}

	scalingnumber = 4.0 / (9 * l * k);
	for(int j = 1; j<= k; j++)
	{
		ComputeCoefficient_y(pSurface,pNurbs,uarray[0],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}
	for(int j = 1; j <= k; j++)
		for(int i= 1; i <= l - 1; i++)
		{
			ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * i],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
		}
	for(int j = 1; j <= k; j++)
		for(int i = 1; i<= l; i++)
		{
			ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * i -1],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
		}
	for(int j = 1; j<= k ; j++)
	{
		ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * l],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}

	scalingnumber = 1.0 / (9 * l * k);
	ComputeCoefficient_y(pSurface,pNurbs,uarray[0],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	for(int i = 1; i <= l - 1; i++)
	{
		ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * i],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
	}
	for(int i = 1; i<= l; i++)
	{
		ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * i -1],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
	}

	ComputeCoefficient_y(pSurface,pNurbs,uarray[2 * l],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	while(Actions_LeastSquare::ComputeCoefficient_y_threadnumber > 0)
	{
		Sleep(100);
	}
	c = c + Jcoeff * weight;
	DeleteCriticalSection(&g_csA);
}
void Actions_LeastSquare::ComputerConformalityCoefficient_CompositeSimpson_xderivative_b(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,int l,int k,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double& c,int c1,int c2,double weight)
{
	sprintf(Post_Message,"Compute Conformality Coefficient Composite Simpson xderivative_b...");
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	InitializeCriticalSection(&g_csA);
	double minu = pNurbs->GetControlPoint(0,0).x;
	double maxu = pNurbs->GetControlPoint(pNurbs->GetCtrlNumberU() - 1,0).x;

	double minv= pNurbs->GetControlPoint(0,0).y;
	double maxv = pNurbs->GetControlPoint(0,pNurbs->GetCtrlNumberV() - 1).y;

	double u,v;
	std::vector<double> uarray;
	std::vector<double> varray;
	for(int i = 0; i <= 2 * l; i++)
	{
		double u = minu + i * (maxu - minu)/ (2.0 * l);
		uarray.push_back(u);
	}
	for(int j = 0; j <= 2 * k; j++)
	{
		double v = minv + j * (maxv - minv)/ (2.0 * k);
		varray.push_back(v);
	}

	//compute the coefficient.
	double Jcoeff = 0;

	double scalingnumber = 1.0 / (9 * l * k);

	ComputeCoefficient_x_b(pSurface,pNurbs,uarray[0],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	for(int i = 1; i <= l - 1; i++)
	{
		ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * i],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
	}
	for(int i = 1; i<= l; i++)
	{
		ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * i -1],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
	}

	ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * l],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	scalingnumber = 2.0 / (9 * l * k);
	for(int j = 1; j<= k - 1; j++)
	{
		ComputeCoefficient_x_b(pSurface,pNurbs,uarray[0],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	
	}
	for(int j = 1; j <= k - 1; j++)
		for(int i= 1; i <= l - 1; i++)
		{
			ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * i],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
		}
	for(int j = 1; j <= k - 1; j++)
		for(int i = 1; i<= l; i++)
		{
			ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * i -1],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
		}
	for(int j = 1; j<= k - 1; j++)
	{
		ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * l],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}

	scalingnumber = 4.0 / (9 * l * k);
	for(int j = 1; j<= k; j++)
	{
		ComputeCoefficient_x_b(pSurface,pNurbs,uarray[0],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}
	for(int j = 1; j <= k; j++)
		for(int i= 1; i <= l - 1; i++)
		{
			ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * i],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
		}
	for(int j = 1; j <= k; j++)
		for(int i = 1; i<= l; i++)
		{
			ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * i -1],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
		}
	for(int j = 1; j<= k ; j++)
	{
		ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * l],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}

	scalingnumber = 1.0 / (9 * l * k);
	ComputeCoefficient_x_b(pSurface,pNurbs,uarray[0],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	
	for(int i = 1; i <= l - 1; i++)
	{
		ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * i],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
	}
	for(int i = 1; i<= l; i++)
	{
		ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * i -1],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
	}

	ComputeCoefficient_x_b(pSurface,pNurbs,uarray[2 * l],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	
	while(Actions_LeastSquare::ComputeCoefficient_x_b_threadnumber > 0)
	{
		Sleep(100);
	}
	c = c + Jcoeff * weight;
	DeleteCriticalSection(&g_csA);
}
void Actions_LeastSquare::ComputerConformalityCoefficient_CompositeSimpson_yderivative_b(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,int l,int k,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,double& c,int c1,int c2,double weight)
{
	sprintf(Post_Message,"Compute Conformality Coefficient Composite Simpson yderivative_b...");
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	InitializeCriticalSection(&g_csA);

	double minu = pNurbs->GetControlPoint(0,0).x;
	double maxu = pNurbs->GetControlPoint(pNurbs->GetCtrlNumberU() - 1,0).x;

	double minv= pNurbs->GetControlPoint(0,0).y;
	double maxv = pNurbs->GetControlPoint(0,pNurbs->GetCtrlNumberV() - 1).y;

	double u,v;
	std::vector<double> uarray;
	std::vector<double> varray;
	for(int i = 0; i <= 2 * l; i++)
	{
		double u = minu + i * (maxu - minu)/ (2.0 * l);
		uarray.push_back(u);
	}
	for(int j = 0; j <= 2 * k; j++)
	{
		double v = minv + j * (maxv - minv)/ (2.0 * k);
		varray.push_back(v);
	}

	//compute the coefficient.
	double Jcoeff = 0;

	double scalingnumber = 1.0 / (9 * l * k);
	
	ComputeCoefficient_y_b(pSurface,pNurbs,uarray[0],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	for(int i = 1; i <= l - 1; i++)
	{
		ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * i],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
	}
	for(int i = 1; i<= l; i++)
	{
		ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * i -1],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
	}

	 ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * l],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	scalingnumber = 2.0 / (9 * l * k);
	for(int j = 1; j<= k - 1; j++)
	{
		ComputeCoefficient_y_b(pSurface,pNurbs,uarray[0],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}
	for(int j = 1; j <= k - 1; j++)
		for(int i= 1; i <= l - 1; i++)
		{
			ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * i],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
		}
	for(int j = 1; j <= k - 1; j++)
		for(int i = 1; i<= l; i++)
		{
			 ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * i -1],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
		}
	for(int j = 1; j<= k - 1; j++)
	{
		ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * l],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}
	scalingnumber = 4.0 / (9 * l * k);
	for(int j = 1; j<= k; j++)
	{
		ComputeCoefficient_y_b(pSurface,pNurbs,uarray[0],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}
	for(int j = 1; j <= k; j++)
		for(int i= 1; i <= l - 1; i++)
		{
			ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * i],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
		}
	for(int j = 1; j <= k; j++)
		for(int i = 1; i<= l; i++)
		{
			ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * i -1],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
		}
	for(int j = 1; j<= k ; j++)
	{
		ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * l],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}

	scalingnumber = 1.0 / (9 * l * k);
	ComputeCoefficient_y_b(pSurface,pNurbs,uarray[0],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	for(int i = 1; i <= l - 1; i++)
	{
		 ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * i],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
	}
	for(int i = 1; i<= l; i++)
	{
		ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * i -1],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
	}
	ComputeCoefficient_y_b(pSurface,pNurbs,uarray[2 * l],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	
	while(Actions_LeastSquare::ComputeCoefficient_y_b_threadnumber > 0)
	{
		Sleep(100);
	}
	c = c + Jcoeff * weight;
	DeleteCriticalSection(&g_csA);
}
void Actions_LeastSquare::ComputerConformalityCoefficient_CompositeSimpson_xderivative(C_NurbsSurf* pSurface,C_NurbsSurf* pNurbs,int l,int k,CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,std::vector<double>& c,int c1,int c2,double weight)
{
	Actions_LeastSquare::ComputeCoefficient_y_threadnumber = 0;

	sprintf(Post_Message,"Compute Conformality Coefficient Composite Simpson xderivative...");
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	InitializeCriticalSection(&g_csA);
	
	double minu = pNurbs->GetControlPoint(0,0).x;
	double maxu = pNurbs->GetControlPoint(pNurbs->GetCtrlNumberU() - 1,0).x;

	double minv= pNurbs->GetControlPoint(0,0).y;
	double maxv = pNurbs->GetControlPoint(0,pNurbs->GetCtrlNumberV() - 1).y;

	double u,v;
	std::vector<double> uarray;
	std::vector<double> varray;
	for(int i = 0; i <= 2 * l; i++)
	{
		double u = minu + i * (maxu - minu)/ (2.0 * l);
		uarray.push_back(u);
	}
	for(int j = 0; j <= 2 * k; j++)
	{
		double v = minv + j * (maxv - minv)/ (2.0 * k);
		varray.push_back(v);
	}

	//compute the coefficient.
	std::vector<double> temp;
	std::vector<double> Jcoeff;
	Jcoeff.clear();

	for(int i = 0; i < c.size(); i++)
	{
		Jcoeff.push_back(0);
	}

	double scalingnumber = 1.0 / (9 * l * k);
	ComputeCoefficient_x(pSurface,pNurbs,uarray[0],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	for(int i = 1; i <= l - 1; i++)
	{
		ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * i],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
	}
	for(int i = 1; i<= l; i++)
	{
		ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * i -1],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
	}
	ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * l],varray[0],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	scalingnumber = 2.0 / (9 * l * k);
	for(int j = 1; j<= k - 1; j++)
	{
		ComputeCoefficient_x(pSurface,pNurbs,uarray[0],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}
	for(int j = 1; j <= k - 1; j++)
		for(int i= 1; i <= l - 1; i++)
		{
			ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * i],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
		}
	for(int j = 1; j <= k - 1; j++)
		for(int i = 1; i<= l; i++)
		{
			ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * i -1],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
		}
	for(int j = 1; j<= k - 1; j++)
	{
		ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * l],varray[2 * j],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}

	scalingnumber = 4.0 / (9 * l * k);
	for(int j = 1; j<= k; j++)
	{
		ComputeCoefficient_x(pSurface,pNurbs,uarray[0],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}
	for(int j = 1; j <= k; j++)
		for(int i= 1; i <= l - 1; i++)
		{
			 ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * i],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
		}
	for(int j = 1; j <= k; j++)
		for(int i = 1; i<= l; i++)
		{
			ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * i -1],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
		}
	for(int j = 1; j<= k ; j++)
	{
		ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * l],varray[2 * j - 1],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);
	}

	scalingnumber = 1.0 / (9 * l * k);
	ComputeCoefficient_x(pSurface,pNurbs,uarray[0],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	for(int i = 1; i <= l - 1; i++)
	{
		ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * i],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 2);
	}
	for(int i = 1; i<= l; i++)
	{
		ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * i -1],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber * 4);
	}
	ComputeCoefficient_x(pSurface,pNurbs,uarray[2 * l],varray[2 * k],poriginalmesh,pconformalmesh,&c,c1,c2,&Jcoeff,scalingnumber);

	while(Actions_LeastSquare::ComputeCoefficient_x_threadnumber > 0)
	{
		Sleep(100);
	}
	c = c + Jcoeff * weight;
	DeleteCriticalSection(&g_csA);
}
void Actions_LeastSquare::ComputeDerivative_Y(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
											  CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
											  std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
											  int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight)
{
	//char message[500];
	double c;

	//if fix the boundary
	int c1lower,c1upper,c2lower,c2upper;
	if(m_FixBoundary)
	{
		c1lower = 1;
		c1upper = ns - 1;
		c2lower = 1;
		c2upper = nt - 1;
	}
	else
	{
		c1lower = 0;
		c1upper = ns;
		c2lower = 0;
		c2upper = nt;
	}

	//compute the partial derivative of y
	for(int c2 = c2lower; c2 < c2upper; c2++)
		for(int c1 = 0; c1 < ns; c1++)
		{
			sprintf(Post_Message,"Computing Coefficient Y %3.2f%%",(c2) * 100.0 /(c2upper - c2lower));
			::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

			if(!m_FixBoundary)
				if(c1 == 0 && c2 == 0 || c1 == ns - 1 && c2 == 0 || 
					c1 == 0 && c2 == nt - 1 || c1 == ns - 1 && c2 == nt - 1)
					continue;

			coeffiecient.clear();

			//for the x
			for(int l = 0; l < nt; l++)
				for(int k = c1lower; k < c1upper; k++)
				{
					if(!m_FixBoundary)
						if(k == 0 && l == 0 || k == ns - 1 && l == 0 || 
						   k == 0 && l == nt - 1 || k == ns - 1 && l == nt - 1)
							continue;

					c = 0;
					coeffiecient.push_back(c);
				}

			//for the y
			for(int l = c2lower; l < c2upper; l++)
				for(int k = 0; k < ns; k++)
				{
					if(!m_FixBoundary)
						if(k == 0 && l == 0 || k == ns - 1 && l == 0 || 
							k == 0 && l == nt - 1 || k == ns - 1 && l == nt - 1)
							continue;
					c = 0;
					int num = pconformalmesh->NbVertex();
					for(int i = 0; i < num; i++)
					{
						CVertex3d* pVertex = pconformalmesh->GetVertex(i);
						if(pVertex->GetFlag() == 1)
							continue;

						double sij = pVertex->x();
						double tij = pVertex->y();

						double ubas = pNurbs->ComputeUBasis(c1,sij);
						double vbas = pNurbs->ComputeVBasis(c2,tij);

						double ubas1 = pNurbs->ComputeUBasis(k,sij);
						double vbas1 = pNurbs->ComputeVBasis(l,tij);
						c = c + ubas * vbas * ubas1 * vbas1 * Position_Weight;
					}

					coeffiecient.push_back(c);
				}

				//lambda
				for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
				{
					c = 0;
					coeffiecient.push_back(c);
				}

				//lambda
				for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
				{
					Point3D para = m_ConstrainedParaArray[k];

					double sij = para.x;
					double tij = para.y;

					double ubas = pNurbs->ComputeUBasis(c1,sij);
					double vbas = pNurbs->ComputeVBasis(c2,tij);

					c = ubas * vbas;
					coeffiecient.push_back(c);
				}

				//the partial derivatives be set to zero.
				//low row
				/*for(int k = 0; k <= Derivative_Constraints; k++)
				{
					if(c2 == 0)
					{
						double s = lower.x + (upper.x - lower.x) * k / Derivative_Constraints;
						double t = lower.y;

						c = pNurbs->ComputeUBasis(c1,s);
						c = c * Derivative_Weight;
					}else
						c = 0;

					coeffiecient.push_back(c);
				}
				//top row
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					if(c2 == nt - 1)
					{
						double s = lower.x + (upper.x - lower.x) * k / Derivative_Constraints;
						double t = upper.y;

						c = pNurbs->ComputeUBasis(c1,s);
						c = c * Derivative_Weight;
					}else
						c = 0;

					coeffiecient.push_back(c);
				}*/
					/*//left column
					for(int k = 0; k <= Derivative_Constraints; k++)
					{
						coeffiecient.push_back(0);
					}
					//right column
					for(int k = 0; k <= Derivative_Constraints; k++)
					{
						coeffiecient.push_back(0);
					}*/
					
				if(m_AddConformality)
					ComputerConformalityCoefficient_CompositeSimpson_yderivative(pSurface->Converte_C_NurbsSurface(),pNurbs,l_num,k_num,poriginalmesh,pconformalmesh,coeffiecient,c1,c2,Conformality_Weight);

				coeffcients.push_back(coeffiecient);
		}
}
void Actions_LeastSquare::ComputeDerivative_Y_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
														  CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
														  std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
														  int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight)
{
	//char message[500];
	double c;
	//compute the derivative of lambda for the y
	for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
	{
		sprintf(Post_Message,"Fitting Least Square Surface Constraint (%d)...",k);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

		coeffiecient.clear();
		Point3D para = m_ConstrainedParaArray[k];

		double sij = para.x;
		double tij = para.y;
		//for the x
		for(int c2 = 0; c2 < nt; c2++)
			for(int c1 = 1; c1 < ns - 1; c1++)
			{
				coeffiecient.push_back(0);
			}
		//for the y
		for(int c2 = 1; c2 < nt - 1; c2++)
			for(int c1 = 0; c1 < ns; c1++)
			{
				double ubas = pNurbs->ComputeUBasis(c1,sij);
				double vbas = pNurbs->ComputeVBasis(c2,tij);
				c = ubas * vbas;
				coeffiecient.push_back(c);
			}

		for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
			coeffiecient.push_back(0);
		for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
			coeffiecient.push_back(0);

		/*//low
		for(int k = 0; k <= Derivative_Constraints; k++)
		{
			coeffiecient.push_back(0);
		}
		//top
		for(int k = 0; k <= Derivative_Constraints; k++)
		{
			coeffiecient.push_back(0);
		}
		//left
		for(int k = 0; k <= Derivative_Constraints; k++)
		{
			coeffiecient.push_back(0);
		}
		//right
		for(int k = 0; k <= Derivative_Constraints; k++)
		{
			coeffiecient.push_back(0);
		}*/
		coeffcients.push_back(coeffiecient);
	}
}
void Actions_LeastSquare::ComputeDerivative_X_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
														  CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
														  std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
														  int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight)
{
	//char message[500];
	double c;
	//compute the derivative of lambda for the x
	for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
	{
		sprintf(Post_Message,"Fitting Least Square Surface Constraint (%d)...",k);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

		coeffiecient.clear();
		Point3D para = m_ConstrainedParaArray[k];

		double sij = para.x;
		double tij = para.y;
		//for the x
		for(int c2 = 0; c2 < nt; c2++)
			for(int c1 = 1; c1 < ns - 1; c1++)
			{
				double ubas = pNurbs->ComputeUBasis(c1,sij);
				double vbas = pNurbs->ComputeVBasis(c2,tij);
				c = ubas * vbas;
				coeffiecient.push_back(c);
			}
		//for the y
		for(int c2 = 1; c2 < nt - 1; c2++)
			for(int c1 = 0; c1 < ns; c1++)
			{
				coeffiecient.push_back(0);
			}
		for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
			coeffiecient.push_back(0);
		for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
			coeffiecient.push_back(0);

		/*//low
		for(int k = 0; k <= Derivative_Constraints; k++)
		{
			coeffiecient.push_back(0);
		}
		//top
		for(int k = 0; k <= Derivative_Constraints; k++)
		{
			coeffiecient.push_back(0);
		}
		//left
		for(int k = 0; k <= Derivative_Constraints; k++)
		{
			coeffiecient.push_back(0);
		}
		//right
		for(int k = 0; k <= Derivative_Constraints; k++)
		{
			coeffiecient.push_back(0);
		}*/
		coeffcients.push_back(coeffiecient);
	}
}
void Actions_LeastSquare::Compute_b_y_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
												  CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
												  std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
												  int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
												  Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn)
{
	//char message[500];
	double c;
	for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
	{
		sprintf(Post_Message,"Fitting Least Square Surface right constrained constant b2(%d)...",k);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

		Vector3D R = m_ConstrainedPointArray[k] - ZeroP3D;
		Point3D para = m_ConstrainedParaArray[k];

		Vector3D point = p00 - ZeroP3D;
		double ubas = pNurbs->ComputeUBasis(0,para.x);
		double vbas = pNurbs->ComputeVBasis(0,para.y);
		R = R - point * ubas * vbas;

		point = pm0 - ZeroP3D;
		ubas = pNurbs->ComputeUBasis(ns - 1,para.x);
		vbas = pNurbs->ComputeVBasis(0,para.y);
		R = R - point * ubas * vbas;

		point = p0n - ZeroP3D;
		ubas = pNurbs->ComputeUBasis(0,para.x);
		vbas = pNurbs->ComputeVBasis(nt - 1,para.y);
		R = R - point * ubas * vbas;

		point = pmn - ZeroP3D;
		ubas = pNurbs->ComputeUBasis(ns - 1,para.x);
		vbas = pNurbs->ComputeVBasis(nt - 1,para.y);
		R = R - point * ubas * vbas;

		c = R.y;
		b1.push_back(c);
	}
}
void Actions_LeastSquare::Compute_b_Boundaries_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
														   CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
														   std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
														   int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
														   Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn)
{
	double c;
	//low
	for(int k = 0; k <= Derivative_Constraints; k++)
	{
		double s = lower.x + (upper.x - lower.x) * k / Derivative_Constraints;
		double t = lower.y;
		double ubas0 = pNurbs->ComputeUBasis(0,s);
		double ubasm = pNurbs->ComputeUBasis(ns - 1,s);

		c = lower.y - p00.y * ubas0 - pm0.y * ubasm;
		c = c * Derivative_Weight;
		b1.push_back(c);
	}
	//top
	for(int k = 0; k <= Derivative_Constraints; k++)
	{
		double s = lower.x + (upper.x - lower.x) * k / Derivative_Constraints;
		double t = upper.y;
		double ubas0 = pNurbs->ComputeUBasis(0,s);
		double ubasm = pNurbs->ComputeUBasis(ns - 1,s);

		c = upper.y - p0n.y * ubas0 - pmn.y * ubasm;
		c = c * Derivative_Weight;
		b1.push_back(c);
	}
	//left
	for(int k = 0; k <= Derivative_Constraints; k++)
	{
		double s = lower.x;
		double t = lower.y + (upper.y - lower.y) * k / Derivative_Constraints;;
		double vbas0 = pNurbs->ComputeVBasis(0,t);
		double vbasn = pNurbs->ComputeVBasis(nt - 1,t);

		c = lower.x - p00.x * vbas0 - p0n.x  * vbasn;
		c = c * Derivative_Weight;
		b1.push_back(c);
	}
	//right
	for(int k = 0; k <= Derivative_Constraints; k++)
	{
		double s = upper.x;
		double t = lower.y + (upper.y - lower.y) * k / Derivative_Constraints;;
		double vbas0 = pNurbs->ComputeVBasis(0,t);
		double vbasn = pNurbs->ComputeVBasis(nt - 1,t);

		c = upper.x - pm0.x * vbas0 - pmn.x  * vbasn;
		c = c * Derivative_Weight;
		b1.push_back(c);
	}
}
void Actions_LeastSquare::Compute_b_x_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
						CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
						std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
						int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
						Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn)
{
	//char message[500];
	double c;
	for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
	{
		sprintf(Post_Message,"Fitting Least Square Surface right constrained constant b1 (%d)...",k);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

		Vector3D R = m_ConstrainedPointArray[k] - ZeroP3D;
		Point3D para = m_ConstrainedParaArray[k];

		Vector3D point = p00 - ZeroP3D;
		double ubas = pNurbs->ComputeUBasis(0,para.x);
		double vbas = pNurbs->ComputeVBasis(0,para.y);
		R = R - point * ubas * vbas;

		point = pm0 - ZeroP3D;
		ubas = pNurbs->ComputeUBasis(ns - 1,para.x);
		vbas = pNurbs->ComputeVBasis(0,para.y);
		R = R - point * ubas * vbas;

		point = p0n - ZeroP3D;
		ubas = pNurbs->ComputeUBasis(0,para.x);
		vbas = pNurbs->ComputeVBasis(nt - 1,para.y);
		R = R - point * ubas * vbas;

		point = pmn - ZeroP3D;
		ubas = pNurbs->ComputeUBasis(ns - 1,para.x);
		vbas = pNurbs->ComputeVBasis(nt - 1,para.y);
		R = R - point * ubas * vbas;

		c = R.x;
		b1.push_back(c);
	}
}
void Actions_LeastSquare::Compute_b_y(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
									  CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
									  std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
									  int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
									  Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn)
{
	//char message[500];
	double c;

	//if fix the boundary
	int c1lower,c1upper,c2lower,c2upper;
	if(m_FixBoundary)
	{
		c1lower = 1;
		c1upper = ns - 1;
		c2lower = 1;
		c2upper = nt - 1;
	}
	else
	{
		c1lower = 0;
		c1upper = ns;
		c2lower = 0;
		c2upper = nt;
	}

	//y
	for(int c2 = c2lower; c2 < c2upper; c2++)
		for(int c1 = 0; c1 < ns; c1++)
		{
			sprintf(Post_Message,"Fitting Least Square Surface right constant b2(%d,%d)...",c1,c2);
			::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
			if(!m_FixBoundary)
				if(c1 == 0 && c2 == 0 || c1 == ns - 1 && c2 == 0 || 
					c1 == 0 && c2 == nt - 1 || c1 == ns - 1 && c2 == nt - 1)
					continue;

			c = 0;
			for(int i = 0; i < pconformalmesh->NbVertex(); i++)
			{
				CVertex3d* pVertex = pconformalmesh->GetVertex(i);
				CVertex3d* pVertex1 = poriginalmesh->GetVertex(i);
				if(pVertex->GetFlag() == 1)
					continue;

				double sij = pVertex->x();
				double tij = pVertex->y();

				double ubas = pNurbs->ComputeUBasis(c1,sij);
				double vbas = pNurbs->ComputeVBasis(c2,tij);

				double Aij = pVertex1->y();

				if(m_FixBoundary)
				{
					for(int ii = 0; ii < ns; ii++)
					{
						double ubas1 = pNurbs->ComputeUBasis(ii,sij);
						double vbas1 = pNurbs->ComputeVBasis(0,tij);
						Aij = Aij - p00.y * (ubas1 * vbas1); 
					}
					for(int ii = 0; ii < ns; ii++)
					{
						double ubas1 = pNurbs->ComputeUBasis(ii,sij);
						double vbas1 = pNurbs->ComputeVBasis(nt - 1,tij);
						Aij = Aij - pmn.y * (ubas1 * vbas1); 
					}
				}else
				{
					double ubas1 = pNurbs->ComputeUBasis(0,sij);
					double vbas1 = pNurbs->ComputeVBasis(0,tij);
					Aij = Aij - p00.y * (ubas1 * vbas1); 

					ubas1 = pNurbs->ComputeUBasis(ns - 1,sij);
					vbas1 = pNurbs->ComputeVBasis(0,tij);
					Aij = Aij - pm0.y * (ubas1 * vbas1); 

					ubas1 = pNurbs->ComputeUBasis(0,sij);
					vbas1 = pNurbs->ComputeVBasis(nt - 1,tij);
					Aij = Aij - p0n.y * (ubas1 * vbas1); 

					ubas1 = pNurbs->ComputeUBasis(ns - 1,sij);
					vbas1 = pNurbs->ComputeVBasis(nt - 1,tij);
					Aij = Aij - pmn.y * (ubas1 * vbas1); 
				}
				c = c + ubas * vbas * Aij * Position_Weight;
			}

		//conformality
		if(m_AddConformality)
			ComputerConformalityCoefficient_CompositeSimpson_yderivative_b(pSurface->Converte_C_NurbsSurface(),pNurbs,l_num,k_num,poriginalmesh,pconformalmesh,c,c1,c2,Conformality_Weight);

		b1.push_back(c);
		}
}
void Actions_LeastSquare::Compute_b_x(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
									  CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
									  std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
									  int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight,std::vector<double>& b1,
									  Point3D p00,Point3D pm0,Point3D p0n,Point3D pmn)
{
	//char message[500];
	double c;

	//if fix the boundary
	int c1lower,c1upper,c2lower,c2upper;
	if(m_FixBoundary)
	{
		c1lower = 1;
		c1upper = ns - 1;
		c2lower = 1;
		c2upper = nt - 1;
	}
	else
	{
		c1lower = 0;
		c1upper = ns;
		c2lower = 0;
		c2upper = nt;
	}

	//b
	//x
	for(int c2 = 0; c2 < nt; c2++)
		for(int c1 = c1lower; c1 < c1upper; c1++)
		{
			sprintf(Post_Message,"Fitting Least Square Surface right constant b1 (%d,%d)...",c1,c2);
			::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

			if(!m_FixBoundary)
				if(c1 == 0 && c2 == 0 || c1 == ns - 1 && c2 == 0 || 
					c1 == 0 && c2 == nt - 1 || c1 == ns - 1 && c2 == nt - 1)
					continue;

			c = 0;

			for(int i = 0; i < pconformalmesh->NbVertex(); i++)
			{
				CVertex3d* pVertex = pconformalmesh->GetVertex(i);
				CVertex3d* pVertex1 = poriginalmesh->GetVertex(i);
				if(pVertex->GetFlag() == 1)
					continue;

				double sij = pVertex->x();
				double tij = pVertex->y();

				double ubas = pNurbs->ComputeUBasis(c1,sij);
				double vbas = pNurbs->ComputeVBasis(c2,tij);

				double Aij = pVertex1->x();

				if(m_FixBoundary)
				{
					for(int ii = 0; ii < nt; ii++)
					{
						double ubas1 = pNurbs->ComputeUBasis(0,sij);
						double vbas1 = pNurbs->ComputeVBasis(ii,tij);
						Aij = Aij - p00.x * (ubas1 * vbas1); 
					}
					for(int ii = 0; ii < nt; ii++)
					{
						double ubas1 = pNurbs->ComputeUBasis(ns - 1,sij);
						double vbas1 = pNurbs->ComputeVBasis(ii,tij);
						Aij = Aij - pm0.x * (ubas1 * vbas1); 
					}
				}else
				{
					double ubas1 = pNurbs->ComputeUBasis(0,sij);
					double vbas1 = pNurbs->ComputeVBasis(0,tij);
					Aij = Aij - p00.x * (ubas1 * vbas1); 

					ubas1 = pNurbs->ComputeUBasis(ns - 1,sij);
					vbas1 = pNurbs->ComputeVBasis(0,tij);
					Aij = Aij - pm0.x * (ubas1 * vbas1); 

					ubas1 = pNurbs->ComputeUBasis(0,sij);
					vbas1 = pNurbs->ComputeVBasis(nt - 1,tij);
					Aij = Aij - p0n.x * (ubas1 * vbas1); 

					ubas1 = pNurbs->ComputeUBasis(ns - 1,sij);
					vbas1 = pNurbs->ComputeVBasis(nt - 1,tij);
					Aij = Aij - pmn.x * (ubas1 * vbas1); 
				}
				c = c + ubas * vbas * Aij * Position_Weight;
			}

		if(m_AddConformality)
			ComputerConformalityCoefficient_CompositeSimpson_xderivative_b(pSurface->Converte_C_NurbsSurface(),pNurbs,l_num,k_num,poriginalmesh,pconformalmesh,c,c1,c2,Conformality_Weight);
		b1.push_back(c);
	}
}
void Actions_LeastSquare::ComputeDerivative_right_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
															 CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
															 std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
															 int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight)
{
	double c;
	//right
	for(int k = 0; k <= Derivative_Constraints; k++)
	{
		coeffiecient.clear();
		double s = upper.x;
		double t = lower.y + (upper.y - lower.y) * k / Derivative_Constraints;

		for(int c2 = 0; c2 < nt; c2++)
			for(int c1 = 0; c1 < ns; c1++)
			{
				if(c1 == 0 && c2 == 0 || c1 == 0 && c2 == nt - 1|| c1 == ns - 1 && c2 == 0 || c1 == ns - 1 && c2 == nt - 1)
					continue;
				if(c1 == ns - 1)
				{
					c = pNurbs->ComputeVBasis(c2,t);
					c = c * Derivative_Weight;
				}else 
					c = 0;

				coeffiecient.push_back(c);
			}
			for(int c2 = 0; c2 < nt; c2++)
				for(int c1 = 0; c1 < ns; c1++)
				{
					if(c1 == 0 && c2 == 0 || c1 == 0 && c2 == nt - 1|| c1 == ns - 1 && c2 == 0 || c1 == ns - 1 && c2 == nt - 1)
						continue;
					coeffiecient.push_back(0);
				}
				for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
					coeffiecient.push_back(0);
				for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
					coeffiecient.push_back(0);

				//low
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//top
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//left
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//right
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
	}
}
void Actions_LeastSquare::ComputeDerivative_left_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
															CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
															std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
															int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight)
{
	double c;
	//left
	for(int k = 0; k <= Derivative_Constraints; k++)
	{
		coeffiecient.clear();
		double s = lower.x;
		double t = lower.y + (upper.y - lower.y) * k / Derivative_Constraints;

		for(int c2 = 0; c2 < nt; c2++)
			for(int c1 = 0; c1 < ns; c1++)
			{
				if(c1 == 0 && c2 == 0 || c1 == 0 && c2 == nt - 1|| c1 == ns - 1 && c2 == 0 || c1 == ns - 1 && c2 == nt - 1)
					continue;
				if(c1 == 0)
				{
					c = pNurbs->ComputeVBasis(c2,t);
					c = c * Derivative_Weight;
				}else
					c = 0;

				coeffiecient.push_back(c);
			}
			for(int c2 = 0; c2 < nt; c2++)
				for(int c1 = 0; c1 < ns; c1++)
				{
					if(c1 == 0 && c2 == 0 || c1 == 0 && c2 == nt - 1|| c1 == ns - 1 && c2 == 0 || c1 == ns - 1 && c2 == nt - 1)
						continue;
					coeffiecient.push_back(0);
				}
				for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
					coeffiecient.push_back(0);
				for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
					coeffiecient.push_back(0);

				//low
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//top
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//left
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//right
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
	}
}
void Actions_LeastSquare::ComputeDerivative_top_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
															CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
															std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
															int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight)
{
	double c;
	//top
	for(int k = 0; k <= Derivative_Constraints; k++)
	{
		coeffiecient.clear();
		double s = lower.x + (upper.x - lower.x) * k / Derivative_Constraints;
		double t = upper.y;

		for(int c2 = 0; c2 < nt; c2++)
			for(int c1 = 0; c1 < ns; c1++)
			{
				if(c1 == 0 && c2 == 0 || c1 == 0 && c2 == nt - 1|| c1 == ns - 1 && c2 == 0 || c1 == ns - 1 && c2 == nt - 1)
					continue;
				coeffiecient.push_back(0);
			}
			for(int c2 = 0; c2 < nt; c2++)
				for(int c1 = 0; c1 < ns; c1++)
				{
					if(c1 == 0 && c2 == 0 || c1 == 0 && c2 == nt - 1|| c1 == ns - 1 && c2 == 0 || c1 == ns - 1 && c2 == nt - 1)
						continue;

					if(c2 == nt - 1)
					{
						c = pNurbs->ComputeUBasis(c1,s);
						c = c * Derivative_Weight;
					}else
						c = 0;

					coeffiecient.push_back(c);
				}
				for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
					coeffiecient.push_back(0);
				for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
					coeffiecient.push_back(0);

				//low
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//top
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//left
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//right
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
	}
}
void Actions_LeastSquare::ComputeDerivative_low_Constrained(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
															CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
															std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
															int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight)
{
	double c;
	//low
	for(int k = 0; k <= Derivative_Constraints; k++)
	{
		coeffiecient.clear();
		double s = lower.x + (upper.x - lower.x) * k / Derivative_Constraints;
		double t = lower.y;

		for(int c2 = 0; c2 < nt; c2++)
			for(int c1 = 0; c1 < ns; c1++)
			{
				if(c1 == 0 && c2 == 0 || c1 == 0 && c2 == nt - 1|| c1 == ns - 1 && c2 == 0 || c1 == ns - 1 && c2 == nt - 1)
					continue;
				coeffiecient.push_back(0);
			}
			for(int c2 = 0; c2 < nt; c2++)
				for(int c1 = 0; c1 < ns; c1++)
				{
					if(c1 == 0 && c2 == 0 || c1 == 0 && c2 == nt - 1|| c1 == ns - 1 && c2 == 0 || c1 == ns - 1 && c2 == nt - 1)
						continue;
					if(c2 == 0)
					{
						c = pNurbs->ComputeUBasis(c1,s);
						c = c * Derivative_Weight;
					}else
						c = 0;

					coeffiecient.push_back(c);
				}
				for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
					coeffiecient.push_back(0);
				for(int k1 = 0; k1 < m_ConstrainedParaArray.size(); k1++)
					coeffiecient.push_back(0);

				//low
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//top
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//left
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
				//right
				for(int k = 0; k <= Derivative_Constraints; k++)
				{
					coeffiecient.push_back(0);
				}
	}
}
void Actions_LeastSquare::ComputeDerivative_X(int ns,int nt,std::vector<double>& coeffiecient,std::vector<std::vector<double>>& coeffcients,
											  CMesh3d* poriginalmesh,CMesh3d* pconformalmesh,C_NurbsSurf* pNurbs,CNurbsSuface* pSurface,double Position_Weight,
											  std::vector<Point3D>& m_ConstrainedParaArray,std::vector<Point3D>& m_ConstrainedPointArray,
											  int Derivative_Constraints,double Derivative_Weight,Point3D lower,Point3D upper,int l_num,int k_num,double Conformality_Weight)
{
	//char message[500];
	double c;

	//if fix the boundary
	int c1lower,c1upper,c2lower,c2upper;
	if(m_FixBoundary)
	{
		c1lower = 1;
		c1upper = ns - 1;
		c2lower = 1;
		c2upper = nt - 1;
	}
	else
	{
		c1lower = 0;
		c1upper = ns;
		c2lower = 0;
		c2upper = nt;
	}
	//compute the partial derivative of x
	for(int c2 = 0; c2 < nt; c2++)
		for(int c1 = c1lower; c1 < c1upper; c1++)
		{
			sprintf(Post_Message,"Computing Derivative X %3.2f%%",(c2) * 100.0 /(nt));
			::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
			
			if(!m_FixBoundary)
				if(c1 == 0 && c2 == 0 || c1 == ns - 1 && c2 == 0 ||
				   c1 == 0 && c2 == nt - 1 || c1 == ns - 1 && c2 == nt - 1)
				   continue;

			coeffiecient.clear();

			//x
			for(int l = 0; l < nt; l++)
				for(int k = c1lower; k < c1upper; k++)
				{
					//boundary
					if(!m_FixBoundary)
						if(k == 0 && l == 0 || k == ns - 1 && l == 0 || 
						   k == 0 && l == nt - 1 || k == ns - 1 && l == nt - 1)
							continue;

					c = 0;
					int num = pconformalmesh->NbVertex();
					for(int i = 0; i < num; i++)
					{
						CVertex3d* pVertex = pconformalmesh->GetVertex(i);
						if(pVertex->GetFlag() == 1)
							continue;

						double sij = pVertex->x();
						double tij = pVertex->y();

						double ubas = pNurbs->ComputeUBasis(c1,sij);
						double vbas = pNurbs->ComputeVBasis(c2,tij);

						double ubas1 = pNurbs->ComputeUBasis(k,sij);
						double vbas1 = pNurbs->ComputeVBasis(l,tij);
						c = c + ubas * vbas * ubas1 * vbas1 * Position_Weight;
					}

					coeffiecient.push_back(c);
				}


			//y
			for(int l = c2lower; l < c2upper; l++)
				for(int k = 0; k < ns; k++)
				{
					if(!m_FixBoundary)
						if(k == 0 && l == 0 || k == ns - 1 && l == 0 || 
							k == 0 && l == nt - 1 || k == ns - 1 && l == nt - 1)
							continue;

					c = 0;
					coeffiecient.push_back(c);
				}

			//lambda
			for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
			{
				Point3D para = m_ConstrainedParaArray[k];

				double sij = para.x;
				double tij = para.y;

				double ubas = pNurbs->ComputeUBasis(c1,sij);
				double vbas = pNurbs->ComputeVBasis(c2,tij);

				c = ubas * vbas;

				coeffiecient.push_back(c);
			}

			//lambda
			for(int k = 0; k < m_ConstrainedParaArray.size(); k++)
			{
				c = 0;
				coeffiecient.push_back(c);
			}

			/*//the partial derivatives be set to zero.
			//low row
			for(int k = 0; k <= Derivative_Constraints; k++)
				coeffiecient.push_back(0);

			//top row
			for(int k = 0; k <= Derivative_Constraints; k++)
				coeffiecient.push_back(0);*/

			/*//left column
			for(int k = 0; k <= Derivative_Constraints; k++)
			{
				if(c1 == 0)
				{
					double t = lower.y + (upper.y - lower.y) * k / Derivative_Constraints;
					double s = lower.x;

					c =  pNurbs->ComputeVBasis(c2,t);
					c = c * Derivative_Weight;
				}else
					c = 0;

				coeffiecient.push_back(c);
			}
			//right column
			for(int k = 0; k <= Derivative_Constraints; k++)
			{
				if(c1 == ns - 1)
				{
					double t = lower.y + (upper.y - lower.y) * k / Derivative_Constraints;
					double s = upper.x;

					c = pNurbs->ComputeVBasis(c2,t);
					c = c * Derivative_Weight;
				}else
					c = 0;

				coeffiecient.push_back(c);
			}*/

			//
			if(m_AddConformality)
				ComputerConformalityCoefficient_CompositeSimpson_xderivative(pSurface->Converte_C_NurbsSurface(),pNurbs,l_num,k_num,poriginalmesh,pconformalmesh,coeffiecient,c1,c2,Conformality_Weight);

			coeffcients.push_back(coeffiecient);
	}
}
void Actions_LeastSquare::BsplineTransformationOptimization_LeastSquare(int ns,int nt)
{
	iterationnum = 0;
	//file.open("D:\\Optimization.txt");

	//if(pDoc->m_SceneGraph.NbObject() != 3)
	//	return;

	//original uv sampling mesh
	CMesh3d* poriginalmesh = g_poriginalmesh;

	//conformal mesh in st domain
	CMesh3d* pconformalmesh = g_pconformalmesh;

	//the Freeform surface
	CNurbsSuface* pSurface = g_pSurface;

	//we use 3*3-degree Bspline surfaces for the fitting.
	//
	Point3D lower,upper;
	Point3D originallower,originalupper;
	pconformalmesh->ComputeBoundingBox(lower,upper);
	poriginalmesh->ComputeBoundingBox(originallower,originalupper);

	//
	int l_num = 3;
	int k_num = 3;

	//int ns = 11;
	//int nt = 11;
	int u_order = 4;
	int v_order = 4;
	int uknotnum = ns - u_order;
	int vknotnum = nt - v_order;

	double *uknot = new double[u_order * 2 + uknotnum];
	double *vknot = new double[v_order * 2 + vknotnum];

	for(int i = 0; i < 4; i++)
		uknot[i] = lower.x;

	for(int i = 1; i <= ns - u_order; i++)
	{
		uknot[3 + i] = lower.x + ((double)i) * (upper.x - lower.x) / (ns - u_order + 1);
	}

	for(int i = 0; i < 4; i++)
		uknot[4 + ns - u_order + i] = upper.x;

	for(int i = 0; i < 4; i++)
		vknot[i] = lower.y;

	for(int i = 1; i <= nt - v_order; i++)
	{
		vknot[3 + i] = lower.y + ((double)i) * (upper.y - lower.y) / (nt - v_order + 1);
	}

	for(int i = 0; i < 4; i++)
		vknot[4 + nt - v_order + i] = upper.y;

	//
	TRACE("\n uknot ");
	for(int i = 0 ; i < u_order * 2 + uknotnum; i++)
	{
		TRACE("%f ",uknot[i]);
	}
	TRACE("\n vknot ");
	for(int i = 0 ; i < v_order * 2 + vknotnum; i++)
	{
		TRACE("%f ",vknot[i]);
	}

	Point3D* pControlPoint = new Point3D[(u_order + uknotnum) * (v_order + vknotnum)];

	double* pWeight = new double[(u_order + uknotnum) * (v_order + vknotnum)];
	for(int i = 0; i < (u_order + uknotnum) * (v_order + vknotnum); i++)
		pWeight[i] = 1.0;

	C_NurbsSurf* pNurbsSurface = new C_NurbsSurf(u_order + uknotnum, v_order + vknotnum, u_order, v_order, uknot,vknot,pControlPoint,pWeight);

	C_NurbsSurf* pNurbs = pNurbsSurface;

	int num = 0;

	std::vector<Point3D> m_ConstrainedParaArray;
	std::vector<Point3D> m_ConstrainedPointArray;

	//fix the four corner points
	Point3D p00,pm0,p0n,pmn;
	int index00,indexm0,index0n,indexmn;
	pconformalmesh->GetFourCornerPoints(index00,indexm0,index0n,indexmn);
	p00[0] = poriginalmesh->GetVertex(index00)->x();
	p00[1] = poriginalmesh->GetVertex(index00)->y();
	p00[2] = poriginalmesh->GetVertex(index00)->z();

	pm0[0] = poriginalmesh->GetVertex(indexm0)->x();
	pm0[1] = poriginalmesh->GetVertex(indexm0)->y();
	pm0[2] = poriginalmesh->GetVertex(indexm0)->z();

	p0n[0] = poriginalmesh->GetVertex(index0n)->x();
	p0n[1] = poriginalmesh->GetVertex(index0n)->y();
	p0n[2] = poriginalmesh->GetVertex(index0n)->z();

	pmn[0] = poriginalmesh->GetVertex(indexmn)->x();
	pmn[1] = poriginalmesh->GetVertex(indexmn)->y();
	pmn[2] = poriginalmesh->GetVertex(indexmn)->z();

	//make the boundary a rectangle.
	pm0[0] = pmn[0];
	pm0[1] = p00[1];

	p0n[0] = p00[0];
	p0n[1] = pmn[1];

	pNurbs->SetControlPoint(0,0,p00);
	pNurbs->SetControlPoint(ns - 1,0,pm0);
	pNurbs->SetControlPoint(0,nt - 1,p0n);
	pNurbs->SetControlPoint(ns - 1,nt - 1,pmn);

	//the other control points are listed as Q10, Q20,...,Qi0, 
	//---------------------Insert the constraints------------------
	//insert the constrains
	//TRACE(" \n ");
	m_ConstrainedPointArray.clear();
	m_ConstrainedParaArray.clear();
	//--------------------end insert constraints-------------------------------------
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)"Start Optimizing Transformation Surface...");
	double c;
	std::vector<std::vector<double>> coeffcients;
	std::vector<double> coeffiecient;
	std::vector<double> b1,b2;
	//char message[500];
	double Derivative_Weight = 1.0;
	double Conformality_Weight = 1.0;
	double Position_Weight = 1.0;
	int Derivative_Constraints = 6;

	//the derivative of x
	ComputeDerivative_X(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight);

	//the derivative of y
	ComputeDerivative_Y(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight);

	//the derivative of x for constrained points
	ComputeDerivative_X_Constrained(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight);

	//the derivative of y for constrained points
	ComputeDerivative_Y_Constrained(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight);

	/*
	//low curve
	ComputeDerivative_low_Constrained(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight);

	//top curve
	ComputeDerivative_top_Constrained(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight);

	//left curve
	ComputeDerivative_left_Constrained(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight);
	
	//right curve
	ComputeDerivative_right_Constrained(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight);
	*/
	//b x
	Compute_b_x(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight,b1,p00,pm0,p0n,pmn);

	//b y
	Compute_b_y(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight,b1,p00,pm0,p0n,pmn);

	//b constrained x
	Compute_b_x_Constrained(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight,b1,p00,pm0,p0n,pmn);

	//b constrained y
	Compute_b_y_Constrained(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight,b1,p00,pm0,p0n,pmn);

	//b four boundary curves
	//Compute_b_Boundaries_Constrained(ns,nt,coeffiecient,coeffcients,poriginalmesh,pconformalmesh,pNurbs,pSurface,Position_Weight,m_ConstrainedParaArray,m_ConstrainedPointArray,Derivative_Constraints,Derivative_Weight,lower,upper,l_num,k_num,Conformality_Weight,b1,p00,pm0,p0n,pmn);
	
	sprintf(Post_Message,"Fitting Least Square Surface solving the least square system...");
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	std::vector<double> xarray,yarray;
	p_FormView3->SolveLinearSystem(coeffcients,b1,xarray);
	//p_FormView3->SolveLinearSystem(coeffcients,b2,yarray);

	/*TRACE("\n");
	for(int i = 0; i < coeffcients.size(); i++)
	{
		for(int j = 0; j < coeffcients[i].size(); j++)
		{
			TRACE(" %5.2f ",coeffcients[i][j]);
		}
			TRACE("\n");
	}
	for(int j = 0; j < b1.size(); j++)
	{
		TRACE(" %5.2f ",b1[j]);
	}
	TRACE("\n");
	for(int j = 0; j < b2.size(); j++)
	{
		TRACE(" %5.2f ",b2[j]);
	}
	TRACE("\n");*/
	int ctrlnum = ns * nt - 4;
	int offset = ns * nt - nt * 2;
	if(m_FixBoundary)
	{
		num = 0;
		for(int j = 0; j < nt; j++)
		{
			Point3D point = pNurbs->GetControlPoint(0,j);
			point.x = p00[0];
			pNurbs->SetControlPoint(0,j,point);

			point = pNurbs->GetControlPoint(ns - 1,j);
			point.x = pm0[0];
			pNurbs->SetControlPoint(ns - 1,j,point);
		}
		for(int j = 0; j < ns; j++)
		{
			Point3D point = pNurbs->GetControlPoint(j,0);
			point.y = p00[1];
			pNurbs->SetControlPoint(j,0,point);

			point = pNurbs->GetControlPoint(j,nt - 1);
			point.y = pmn[1];
			pNurbs->SetControlPoint(j,nt - 1,point);
		}
		for(int j = 0; j < nt; j++)
			for(int i = 1; i < ns - 1; i++)
			{

				Point3D point = pNurbs->GetControlPoint(i,j);
				point.x = xarray[num];
				pNurbs->SetControlPoint(i,j,point);
				num++;
			}
			for(int j = 1; j < nt - 1; j++)
				for(int i = 0; i < ns; i++)
				{
					Point3D point = pNurbs->GetControlPoint(i,j);
					point.y = xarray[num];
					pNurbs->SetControlPoint(i,j,point);
					num++;
				}
	}else
	{
		num = 0;
		for(int j = 0; j < nt; j++)
			for(int i = 0; i < ns; i++)
			{
				if(i == 0 && j == 0 || i == ns - 1 && j == 0 ||
				   i == 0 && j == nt - 1 || i == ns - 1 && j == nt - 1)
				   continue;

				Point3D point = pNurbs->GetControlPoint(i,j);
				point.x = xarray[num];
				point.y = xarray[ctrlnum + num];
				pNurbs->SetControlPoint(i,j,point);
				num++;
			}
	}

	//TRACE("%f,%f",x[0],x[1]);
	BsplineTransformation* pTrans = new BsplineTransformation();
	pTrans->m_ReparameterizationType = Bspline;
	pTrans->umin_Reparameterization = lower.x;
	pTrans->vmin_Reparameterization = lower.y;
	pTrans->umax_Reparameterization = upper.x;
	pTrans->vmax_Reparameterization = upper.y;
	pTrans->umin = originallower.x;
	pTrans->vmin = originallower.y;
	pTrans->umax = originalupper.x;
	pTrans->vmax = originalupper.y;

	pTrans->pNurbsSurf = pNurbs;

	CNurbsSuface* pNewSurface = pSurface->copy();
	sprintf(Post_Message,"Optimized Surface%d_%d",ns,nt);
	pNewSurface->filename = Post_Message;
	pNewSurface->SetTransform(*pSurface->GetTransform());
	pNewSurface->ptransformation = pTrans;
	pDoc->m_SceneGraph.Add(pNewSurface);

	CNurbsSuface* pNewTransformation = pNurbs->GetCNurbsSurface()->copy();
	sprintf(Post_Message,"Optimized Transformation%d_%d",ns,nt);
	pNewTransformation->filename = Post_Message;
	pNewTransformation->SetTransform(*pSurface->GetTransform());
	pDoc->m_SceneGraph.Add(pNewTransformation);
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)"End...");
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	::PostMessage(g_hwndMain, WM_UPDATE_TREECONTROL, 0, 0);
}
extern string DoubleToString(double d);
void Actions_LeastSquare::BsplineTransformationOptimization()
{
	iterationnum = 0;
	file.open("D:\\Optimization.txt");

	//if(pDoc->m_SceneGraph.NbObject() != 3)
	//	return;

	//original uv sampling mesh
	CMesh3d* poriginalmesh = g_poriginalmesh;

	//conformal mesh in st domain
	CMesh3d* pconformalmesh = g_pconformalmesh;

	//the Freeform surface
	CNurbsSuface* pSurface = g_pSurface;

	//if no initial value, exit
	if(!pSurface->ptransformation)
		return;

	CNurbsSuface* pTransform1 = ((BsplineTransformation*)pSurface->ptransformation)->pNurbsSurf->GetCNurbsSurface();
	pTransform1->filename = "initial Bspline transform";
	pTransform1->SetTransform(*pSurface->GetTransform());
	pDoc->m_SceneGraph.Add(pTransform1);

	//Generate a working copy
	CNurbsSuface* pNewSurface = pSurface->copy();
	pNewSurface->ptransformation = pSurface->ptransformation->copy();
	pNewSurface->filename = "Optimized surface";
	pDoc->m_SceneGraph.Add(pNewSurface);

	BsplineTransformation* pBsplineTrans = (BsplineTransformation*)pNewSurface->ptransformation;
	C_NurbsSurf* pNurbs = pBsplineTrans->pNurbsSurf;

	int num = 0;
	string str="[";
	int uctrlnum = pNurbs->GetCtrlNumberU();
	int vctrlnum = pNurbs->GetCtrlNumberV();

	//fix the four corner points
	//the other control points are listed as Q10, Q20,...,Qi0, 
	//first row then column
	for(int j = 1; j < pNurbs->GetCtrlNumberV() - 1; j++)
		for(int i = 1; i < pNurbs->GetCtrlNumberU() - 1; i++)
		{
			//only consider the inner control points

			Point3D point = pNurbs->GetControlPoint(i,j);

			if(num == (uctrlnum - 2) * (vctrlnum - 2) - 1)
			{
				str += DoubleToString(point.x) + "," + DoubleToString(point.y) + "]";
			}else
				str += DoubleToString(point.x) + "," + DoubleToString(point.y) + ",";

			num++;
		}
		real_1d_array x(str.c_str());

		TRACE("---------------------------------------------------------------------\n initial x is :");
		for(int i = 0; i< x.length(); i++)
			TRACE("%5.2f\t", x[i]);
		TRACE("----------------------------------------------------------------------\n");

		double epsg = 1.0;
		double epsf = 0;
		double epsx = 0;
		ae_int_t maxits = 2;
		minlbfgsstate state;
		minlbfgsreport rep;

		minlbfgscreate(1, x, state);
		minlbfgssetcond(state, epsg, epsf, epsx, maxits);
		alglib::minlbfgsoptimize(state, ConformalEnergy);
		minlbfgsresults(state, x, rep);

		TRACE("---------------------------------------------------------------------\nfinal x is :");
		for(int i = 0; i< x.length(); i++)
			TRACE("%5.2f\t", x[i]);
		TRACE("----------------------------------------------------------------------\n");

		file.close();
		num = 0;
		for(int j = 0; j < pNurbs->GetCtrlNumberV(); j++)
			for(int i = 0; i < pNurbs->GetCtrlNumberU(); i++)
			{
				if(i == 0 && j == 0)
					continue;
				if(i == pNurbs->GetCtrlNumberU() - 1 && j == 0)
					continue;
				if(i == pNurbs->GetCtrlNumberU() - 1 && j == pNurbs->GetCtrlNumberV() - 1)
					continue;
				if(i == 0 && j == pNurbs->GetCtrlNumberV() - 1)
					continue;

				Point3D point;
				point.x = x[num * 2];
				point.y = x[num * 2 + 1];
				point.z = 0;

				pNurbs->SetControlPoint(i,j,point);
				num++;
			}
			//TRACE("%f,%f",x[0],x[1]);

		CNurbsSuface* pTransform2 = pNurbs->GetCNurbsSurface();
		pTransform2->filename = "Optimized transformation";
		pTransform2->SetTransform(*pSurface->GetTransform());
		pDoc->m_SceneGraph.Add(pTransform2);

		TRACE("Initial conformal energy: %5.6f\tOptimized conformal energy%5.6f",ComputeConformalEnergy(pSurface),ComputeConformalEnergy(pNewSurface));
		//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)"End...");
		//pDoc->UpdateTreeControl();
		//pDoc->UpdateAllViews(NULL);
}
void Actions_LeastSquare::OnBnClickedBsplinefittingoptimization()
{
	// TODO: Add your control notification handler code here
	//numerical optimization.
	BsplineTransformationOptimization();
}

void Actions_LeastSquare::ConformalityOptimization22()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnOpenDocument("D:\\papers\\2013\\Conformal NURBS Surfaces\\data\\surface22new.sur");
	if(pDoc->m_SceneGraph.NbObject() != 1)
		return;

	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pNurbs = pSurface->Converte_C_NurbsSurface();

	int num = 0;
	string str="[";
	int uctrlnum = pNurbs->GetCtrlNumberU();
	int vctrlnum = pNurbs->GetCtrlNumberV();

	Vector3D p00 = pNurbs->GetControlPoint(0,0)- ZeroP3D;
	Vector3D p10 = pNurbs->GetControlPoint(1,0)- ZeroP3D;
	Vector3D p01 = pNurbs->GetControlPoint(0,1)- ZeroP3D;
	Vector3D p11 = pNurbs->GetControlPoint(1,1)- ZeroP3D;
	for(int j = 0; j < pNurbs->GetCtrlNumberV(); j++)
		for(int i = 0; i < pNurbs->GetCtrlNumberU(); i++)
		{
			Point3D point = pNurbs->GetControlPoint(i,j);

			TRACE("%f %f %f \n",point.x,point.y,point.z);

			if(num == uctrlnum * vctrlnum - 1)
			{
				str += DoubleToString(point.x) + "," + DoubleToString(point.y) + "," + DoubleToString(point.z) + "]";
			}else
			{
				str += DoubleToString(point.x) + "," + DoubleToString(point.y) + "," + DoubleToString(point.z) + ",";
			}
			num++;
		}

		Vector3D N=(p10-p00)^(p01-p00);
		N.SetUnit();

		double k=p00.x*N.x+p00.y*N.y+p00.z*N.z;//常数

		string str2="[[0,0,0,"+ DoubleToString(N.x)+","+DoubleToString(N.y)+","+DoubleToString(N.z)+",0,0,0,"
			          "0,0,0,0,0,0,0,0,0,"+ "0,0,0,0,0,0,0,0,0," + DoubleToString(k)+"]]";

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
		alglib::minbleicoptimize(state, ConformalEnergy22);
		minbleicresults(state, x, rep);

		char message[80];
		sprintf(message,"value = %f",finalval);
		AfxMessageBox(message);

		num = 0;
		for(int j = 0; j < pNurbs->GetCtrlNumberV(); j++)
			for(int i = 0; i < pNurbs->GetCtrlNumberU(); i++)
			{

				Point3D point;
				point.x = x[num * 3];
				point.y = x[num * 3 + 1];
				point.z = x[num * 3 + 2];

				pNurbs->SetControlPoint(i,j,point);
				num++;
			}
			//TRACE("%f,%f",x[0],x[1]);

			CNurbsSuface* pTransform2 = pNurbs->GetCNurbsSurface();
			pTransform2->filename = "Optimized transformation";
			pTransform2->SetTransform(*pSurface->GetTransform());
			pDoc->m_SceneGraph.Add(pTransform2);

			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);
}
void Actions_LeastSquare::ConformalityOptimization11()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnOpenDocument("D:\\papers\\2013\\AreaPreservingBezierusingmobiustransformations\\Programe\\data\\bilinearsuface.sur");
	if(pDoc->m_SceneGraph.NbObject() != 1)
		return;

	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pNurbs = pSurface->Converte_C_NurbsSurface();

	int num = 0;
	string str="[";
	int uctrlnum = pNurbs->GetCtrlNumberU();
	int vctrlnum = pNurbs->GetCtrlNumberV();

	Vector3D p00 = pNurbs->GetControlPoint(0,0)- ZeroP3D;
	Vector3D p10 = pNurbs->GetControlPoint(1,0)- ZeroP3D;
	Vector3D p01 = pNurbs->GetControlPoint(0,1)- ZeroP3D;
	Vector3D p11 = pNurbs->GetControlPoint(1,1)- ZeroP3D;
	for(int j = 0; j < pNurbs->GetCtrlNumberV(); j++)
		for(int i = 0; i < pNurbs->GetCtrlNumberU(); i++)
		{
			Point3D point = pNurbs->GetControlPoint(i,j);

			TRACE("%f %f %f \n",point.x,point.y,point.z);

			if(num == uctrlnum * vctrlnum - 1)
			{
				str += DoubleToString(point.x) + "," + DoubleToString(point.y) + "," + DoubleToString(point.z) + "]";
			}else
			{
				str += DoubleToString(point.x) + "," + DoubleToString(point.y) + "," + DoubleToString(point.z) + ",";
			}
			num++;
		}

		Vector3D N=(p10-p00)^(p01-p00);
		N.SetUnit();

		double k=p00.x*N.x+p00.y*N.y+p00.z*N.z;//常数

		string str2="[[0,0,0,0,0,0,"+ DoubleToString(N.x)+","+DoubleToString(N.y)+","+DoubleToString(N.z)+",0,0,0,"+DoubleToString(-k)+"]]";
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
		alglib::minbleicoptimize(state, ConformalEnergy11);
		minbleicresults(state, x, rep);

		num = 0;
		for(int j = 0; j < pNurbs->GetCtrlNumberV(); j++)
			for(int i = 0; i < pNurbs->GetCtrlNumberU(); i++)
			{

				Point3D point;
				point.x = x[num * 3];
				point.y = x[num * 3 + 1];
				point.z = x[num * 3 + 2];

				pNurbs->SetControlPoint(i,j,point);
				num++;
			}
			//TRACE("%f,%f",x[0],x[1]);

			CNurbsSuface* pTransform2 = pNurbs->GetCNurbsSurface();
			pTransform2->filename = "Optimized transformation";
			pTransform2->SetTransform(*pSurface->GetTransform());
			pDoc->m_SceneGraph.Add(pTransform2);

			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);
}
void Actions_LeastSquare::OnBnClickedOrthogonalityopt()
{
	// TODO: Add your control notification handler code here
	//p_FormView3->OnBnClickedButton3();
	//return;
	//no gradient
	ConformalityOptimization11();
}

void Actions_LeastSquare::OnBnClickedConformalityopt22()
{
	// TODO: Add your control notification handler code here
	ConformalityOptimization22();
}

void Actions_LeastSquare::OnBnClickedCheckconformity()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pNurbs = pSurface->Converte_C_NurbsSurface();
	int samplingnum = 100;
	for(int i = 0; i < samplingnum; i++)
		for(int j = 0; j < samplingnum; j++)
		{
			double u = i * 1.0 / samplingnum;
			double v = j * 1.0 / samplingnum;

			Vector3D xu = pNurbs->ComputUDerivative1(u,v);
			Vector3D xv = pNurbs->ComputVDerivative1(u,v);

			double length1 = Length(xu);
			double length2 = Length(xv);

			double length3 = xu * xv;

			Vector3D N = xu ^ xv;
			N.SetUnit();

			Vector3D testvec = N ^ xv + xu;
			double length = Length(testvec);

			if((length1 - length2) > 1e-4 || length3 > 1e-4)
			{
				char message[80];
				sprintf(message,"%f",length);
				AfxMessageBox(message);
				return;
			}
		}
	AfxMessageBox("Conformal");
}
int Actions_LeastSquare::m_AddConformality = 0;
int Actions_LeastSquare::m_FixBoundary = 0;
int Actions_LeastSquare::ComputeCoefficient_x_threadnumber = 0;
int Actions_LeastSquare::ComputeCoefficient_y_threadnumber = 0;
int Actions_LeastSquare::ComputeCoefficient_x_b_threadnumber = 0;
int Actions_LeastSquare::ComputeCoefficient_y_b_threadnumber = 0;
void Actions_LeastSquare::OnBnClickedLeastsquaremethod()
{
	// TODO: Add your control notification handler code here
	CButton *pButton = (CButton*)GetDlgItem(IDC_Conformality);
	m_AddConformality = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_CHECK_FIXBOUNDARY);
	m_FixBoundary = pButton->GetCheck();

	//Generate the least square method

	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\parameterdomain.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\conformal.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\face.sur");
	//return;
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\cartop\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\cartop\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\cartop\\cartop.sur");

	//pDoc->OnOpenDocument("parameterdomain.obj");
	//pDoc->OnOpenDocument("conformal.obj");
	//pDoc->OnOpenDocument("face.sur");

	//pDoc->OnOpenDocument("parameterdomain.obj");
	//pDoc->OnOpenDocument("conformal.obj");
	//pDoc->OnOpenDocument("cartop.sur");

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_SceneGraph.NbObject() != 3)
		return;

	g_poriginalmesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	g_pconformalmesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	g_pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(2);

	g_pThread = AfxBeginThread(WorkingThread_LeastSquare, 
		(LPVOID)g_hwndMain,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	g_pThread->m_bAutoDelete = TRUE;
	g_pThread->ResumeThread();

}

void Actions_LeastSquare::OnBnClickedTest()
{
	// TODO: Add your control notification handler code here
	double p0x = 0;
	double p0y = 0;
	double p1x = 1;
	double p1y = 0;
	double p2x = 0.5;
	double p2y = 1;
	for(int ii = 1; ii < 100; ii++)
		for(int jj = 1; jj < 100; jj++)
	{
		double angle = 3.1415926 * jj / 200; 
		double a = ii * 0.02;
		p2x = a * cos(angle);
		p2y = a * sin(angle);
		double x1 = p1x - p0x;
		double y1 = p1y - p0y;
		double x2 = p2x - p1x;
		double y2 = p2y - p1y;
		double x3 = p0x - p2x;
		double y3 = p0y - p2y;

		double ux = 0.2;
		double uy = 0.3;

		for(int i = 0; i <= 100; i++)
			for(int j = 0; j <= 100; j++)
			{
				ux = i * 0.01;
				uy = j * 0.01;
				double len = sqrt(ux * ux + uy * uy);

				ux = ux / len;
				uy = uy / len;
				if(len > 1.0)
					len = 1.0;
				ux = ux * len;
				uy = uy * len;

				double nx1 = x1 + ux * x1 + uy * y1;
				double ny1 = y1 + uy * x1 - ux * y1;
				double nx2 = x2 + ux * x2 + uy * y2;
				double ny2 = y2 + uy * x2 - ux * y2;
				double nx3 = x3 + ux * x3 + uy * y3;
				double ny3 = y3 + uy * x3 - ux * y3;

				double na = sqrt(nx1 * nx1 + ny1 * ny1);
				double nb = sqrt(nx2 * nx2 + ny2 * ny2);
				double nc = sqrt(nx3 * nx3 + ny3 * ny3);

				double t1 = na + nb - nc;
				double t2 = na + nc - nb;
				double t3 = nb + nc - na;
				double t4 = nc - (na - nb);
				double t5 = na - (nb - nc);
				double t6 = nb - (na - nc);
				double TOL = -1e-8;
				if(t1 < TOL || t2 < TOL || t3 < TOL || t4 < TOL || t5 < TOL || t6 < TOL) 
				{
					int ii = 0;
				}
			}
	}	
}

void Actions_LeastSquare::OnBnClickedLeastsquaremethodSeperate()
{
	// TODO: Add your control notification handler code here
	long BeginTime = GetTickCount();

	CButton *pButton = (CButton*)GetDlgItem(IDC_Conformality);
	m_AddConformality = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_CHECK_FIXBOUNDARY);
	m_FixBoundary = pButton->GetCheck();

	//Generate the least square method

	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	pDoc->OnOpenDocument("G:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\parameterdomain.obj");
	pDoc->OnOpenDocument("G:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\conformal.obj");
	pDoc->OnOpenDocument("G:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\face.sur");

	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\sparse\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\sparse\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\face\\sparse\\face.sur");

	//return;
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\cartop\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\cartop\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\Bsplinemethod\\cartop\\cartop.sur");

	//pDoc->OnOpenDocument("parameterdomain.obj");
	//pDoc->OnOpenDocument("conformal.obj");
	//pDoc->OnOpenDocument("face.sur");

	//pDoc->OnOpenDocument("parameterdomain.obj");
	//pDoc->OnOpenDocument("conformal.obj");
	//pDoc->OnOpenDocument("cartop.sur");

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_SceneGraph.NbObject() != 3)
		return;

	g_poriginalmesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	g_pconformalmesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	g_pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(2);
	ThreadParam* pThreadParam = new ThreadParam();
	pThreadParam->hwnd = g_hwndMain;
	pThreadParam->ns = GetDlgItemInt(IDC_FITTINGSURFACE_NS);
	pThreadParam->nt = GetDlgItemInt(IDC_FITTINGSURFACE_NT);
	g_pThread = AfxBeginThread(WorkingThread_LeastSquare_Seperate, 
		(LPVOID)pThreadParam,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	g_pThread->m_bAutoDelete = TRUE;
	g_pThread->ResumeThread();

}



void Actions_LeastSquare::OnBnClickedhierarchicalTessellation()
{
	// TODO: Add your control notification handler code here
	//first load the bunny model.
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\bunny\\bunny.set");
}
