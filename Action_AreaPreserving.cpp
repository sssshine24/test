// Action_AreaPreserving.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Action_AreaPreserving.h"
#include "FormView3.h"
#include "Nurbs3D.h"
#include "CompositeMobiusTransformation.h"
#include "MobiusTransformation.h"
#include "lib3d\Transform.h"
#include "Disline2D.h"
#include "disline.h"
#include "NurbsCurve.h"
#include "lib3d\Base3d.h"
// Action_AreaPreserving dialog

IMPLEMENT_DYNAMIC(Action_AreaPreserving, CPropertyPage)

Action_AreaPreserving::Action_AreaPreserving()
	: CPropertyPage(Action_AreaPreserving::IDD)
{

}

Action_AreaPreserving::~Action_AreaPreserving()
{
}

void Action_AreaPreserving::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Action_AreaPreserving, CPropertyPage)
	ON_BN_CLICKED(IDC_CARTOPSTEP1, &Action_AreaPreserving::OnBnClickedCartopstep1)
	ON_BN_CLICKED(IDC_CARTOPSTEP2, &Action_AreaPreserving::OnBnClickedCartopstep2)
	ON_BN_CLICKED(IDC_FACE2STEP1, &Action_AreaPreserving::OnBnClickedFace2step1)
	ON_BN_CLICKED(IDC_FACE2STEP2, &Action_AreaPreserving::OnBnClickedFace2step2)
	ON_BN_CLICKED(IDC_BezierExampleStep1, &Action_AreaPreserving::OnBnClickedBezierexamplestep1)
	ON_BN_CLICKED(IDC_COMPOSITEREPARABEZIER, &Action_AreaPreserving::OnBnClickedCompositereparabezier)
	ON_BN_CLICKED(IDC_FACECOMPOSITETRANSFORMATION, &Action_AreaPreserving::OnBnClickedFacecompositetransformation)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_BN_CLICKED(IDC_FaceExample, &Action_AreaPreserving::OnBnClickedFaceexample)
	ON_BN_CLICKED(IDC_BONESTEP1, &Action_AreaPreserving::OnBnClickedBonestep1)
	ON_BN_CLICKED(IDC_BUTTON6, &Action_AreaPreserving::OnBnClickedButton6)
END_MESSAGE_MAP()


// Action_AreaPreserving message handlers

void Action_AreaPreserving::OnBnClickedCartopstep1()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\cartop.sur");

	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	p_FormView3->Compute_Optimal_AreaPreserving_IsoParameterCurves_NumericalMethod(pSurf->Converte_C_NurbsSurface());

	//cartop
	p_FormView3->SetCartopModification();

	pDoc->UpdateAllViews(NULL);
}

void Action_AreaPreserving::OnBnClickedCartopstep2()
{
	// TODO: Add your control notification handler code here

}

void Action_AreaPreserving::OnBnClickedFace2step1()
{
	// TODO: Add your control notification handler code here
	p_FormView3->OnBnClickedFace2step1();

}

void Action_AreaPreserving::OnBnClickedFace2step2()
{
	// TODO: Add your control notification handler code here
	p_FormView3->OnBnClickedFace2step2();
}

void Action_AreaPreserving::OnBnClickedBezierexamplestep1()
{
	// TODO: Add your control notification handler code here
	//p_FormView3->OnBnClickedBezierexamplestep1();
	p_FormView3->OnBnClickedAreapreserve();
}
extern string DoubleToString(double d);
void Action_AreaPreserving::CheckMobius_CompositeMobius_Ability()
{
	double a = 0.75;
	MobiusTransformation* pTrans = new MobiusTransformation();
	pTrans->m_ReparameterizationType = mobius;
	pTrans->m_Mobiustransformation_Alpha = a;
	pTrans->m_Mobiustransformation_Beta = 0.5;

	CompositeMobiusTransformation* pCTrans = new CompositeMobiusTransformation();
	double s1 = 0.21;
	double u1;
	u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
	double u2 = (a - 1) * (s1 / 2) / (2 * a * (s1 / 2) - (s1 / 2) - a);
	double u3 = (a - 1) * ((s1 + 1) / 2) / (2 * a * ((s1 + 1) / 2) - ((s1 + 1) / 2) - a);
	double a1 = 1 - u2 / u1;
	double a2 = 1 - (u3 - u1) / (1 - u1);

	pCTrans->M = 2;
	pCTrans->N = 1;
	pCTrans->sknot.push_back(0);
	pCTrans->sknot.push_back(s1);
	pCTrans->sknot.push_back(1);
	pCTrans->tknot.push_back(0);
	pCTrans->tknot.push_back(1);
	pCTrans->uknot.push_back(0);
	pCTrans->uknot.push_back(u1);
	pCTrans->uknot.push_back(1);
	pCTrans->vknot.push_back(0);
	pCTrans->vknot.push_back(1);
	pCTrans->alpha.push_back(a1);
	pCTrans->alpha.push_back(a2);
	pCTrans->beta.push_back(0.5);

	double u_1,v_1,u_2,v_2;
	pTrans->Evaluation(0.1,0,u_1,v_1);
	pCTrans->Evaluation(0.1,0,u_2,v_2);
}
void Action_AreaPreserving::OnBnClickedCompositereparabezier()
{
	// TODO: Add your control notification handler code here
	//p_FormView3->OnBnClickedBezierexamplestep1();

	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num=pDoc->m_SceneGraph.NbObject();
	if(num!=1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();

	double val;
	//Compute_Optimal_Composite_Mobiustransformation(2,2,pSurf);
	//return;
	//Compute_Optimal_Composite_Mobiustransformation(4,4,pSurf);
	//Compute_Optimal_Composite_Mobiustransformation(8,8,pSurf);
	Compute_Optimal_Composite_Mobiustransformation(16,16,pSurf);
	return;
	
	//Step 1: first compute the cij coefficients
	//Compute_Coefficients_Cij(pSurf,0,0);
	
	//Step 2: Compute the optimal mobius transformation coefficents ai & bj

	std::vector<double> energy;
	std::vector<int> subdivision;
	int M = 1;
	int N = 1;
	val = Compute_Optimal_Composite_Mobiustransformation(M,N,pSurf);
	char message[1000];
	string str,str1;
	double val1;
	int MAX_INTERATIONS = 32;
	energy.push_back(val);
	subdivision.push_back(M);

	while(TRUE)
	{

		M = M * 2;
		N = N * 2;

		sprintf(message," %d * %d",M,N);
		str = message;
		str = "Processing" + str + "...";
		AfxGetMainWnd()->SetWindowText(str.c_str());
		val1 = Compute_Optimal_Composite_Mobiustransformation(M,N,pSurf);
		sprintf(message,"%d * %d, Energy: %f",M,N,val1);
		str = message;
		AfxGetMainWnd()->SetWindowText(str.c_str());
		AfxGetMainWnd()->SetWindowText(message);
		if(fabs(val - val1) < 1e-5)
			break;
		val = val1;
		if(M >= MAX_INTERATIONS || N >= MAX_INTERATIONS)
			break;

		energy.push_back(val);
		subdivision.push_back(M);

	}
	str = "";
	for(int i = 0; i < energy.size(); i++)
	{
		char message1[100];
		sprintf(message1," %d:",subdivision[i]);
		str1 = message1;
		str += str1 + "     " + DoubleToString(energy[i]) + ",";
	}
	sprintf(message,"%s",str.c_str());
	AfxMessageBox(message);	
}
int g_Mseg,g_Nseg;
std::vector<std::vector<std::vector<double>>> coefficientarray;
void CompositeAreaPreserving_grad(const real_1d_array &x, double &func, real_1d_array &grad, void *ptr)
{
	int M = g_Mseg;
	int N = g_Nseg;

	std::vector<double> alphaarray,betaarray;
	for(int i = 0; i < M; i++)
		alphaarray.push_back(x[i]);
	for(int j = 0; j < N; j++)
		betaarray.push_back(x[M + j]);

	double z = 0;
	for(int ii = 0; ii < M; ii++)
		for(int jj = 0; jj < N; jj++)
		{
			double a = alphaarray[ii];
			double b = betaarray[jj];

			double c00 = coefficientarray[ii][jj][0];
			double c01 = coefficientarray[ii][jj][1];
			double c02 = coefficientarray[ii][jj][2];
			double c10 = coefficientarray[ii][jj][3];
			double c11 = coefficientarray[ii][jj][4];
			double c12 = coefficientarray[ii][jj][5];
			double c20 = coefficientarray[ii][jj][6];
			double c21 = coefficientarray[ii][jj][7];
			double c22 = coefficientarray[ii][jj][8];

			z += ( (1 - a) * (1 - a) * (1 - b) * (1 - b) * c00
				+ (1 - a) * (1 - a) * (1 - b) * b * c01
				+ (1 - a) * (1 - a) * b * b * c02  
				+ (1 - a) * a * (1 - b) * (1 - b) * c10 
				+ (1 - a) * a * (1 - b) * b * c11
				+ (1 - a) * a * b * b * c12 
				+ a * a * (1 - b) * (1 - b) * c20
				+ a * a * (1 - b) * b * c21  
				+ a * a * b * b * c22) / (a * (1 - a) * b * (1 - b));
		}
		func = z;

		for(int i = 0; i < x.length(); i++)
			if(x[i] <= 0 || x[i] >= 1)
				func = 1e10;

		//compute the derivative for the alpha
		for(int i = 0; i < M; i++)
		{
			//for the alpha_i
			double a = alphaarray[i];
			double ad = 0;
			for(int j = 0; j < N; j++)
			{
				double b = betaarray[j];

				double c00 = coefficientarray[i][j][0];
				double c01 = coefficientarray[i][j][1];
				double c02 = coefficientarray[i][j][2];
				double c10 = coefficientarray[i][j][3];
				double c11 = coefficientarray[i][j][4];
				double c12 = coefficientarray[i][j][5];
				double c20 = coefficientarray[i][j][6];
				double c21 = coefficientarray[i][j][7];
				double c22 = coefficientarray[i][j][8];

				//the numer
				double numer = (1 - a) * (1 - a) * (1 - b) * (1 - b) * c00
					+ (1 - a) * (1 - a) * (1 - b) * b * c01
					+ (1 - a) * (1 - a) * b * b * c02  
					+ (1 - a) * a * (1 - b) * (1 - b) * c10 
					+ (1 - a) * a * (1 - b) * b * c11
					+ (1 - a) * a * b * b * c12 
					+ a * a * (1 - b) * (1 - b) * c20
					+ a * a * (1 - b) * b * c21  
					+ a * a * b * b * c22;

				//the denominator
				double denominator = a * (1 - a) * b * (1 - b);

				//compute the partial derivative of numer over alpha
				double partial_numer_over_a =   (-2) * (1 - a) * (1 - b) * (1 - b) * c00
					+ (-2) * (1 - a) * (1 - b) * b * c01
					+ (-2) * (1 - a) * b * b * c02  
					+ (1 - 2 * a) * (1 - b) * (1 - b) * c10 
					+ (1 - 2 * a) * (1 - b) * b * c11
					+ (1 - 2 * a) * b * b * c12 
					+ 2 * a * (1 - b) * (1 - b) * c20
					+ 2 * a * (1 - b) * b * c21  
					+ 2 * a * b * b * c22;

				//compute the partial derivative of denominator over alpha
				double partial_denorminator_over_a = (1 - 2 * a) * b * (1 - b);

				ad += (partial_numer_over_a * denominator - numer * partial_denorminator_over_a) 
					/ SQR(denominator);
			}
			grad[i] = ad;
		}

		//compute the derivative for the beta
		for(int i = 0; i < N; i++)
		{
			//for the alpha_i
			double b = betaarray[i];
			double bd = 0;
			for(int j = 0; j < M; j++)
			{
				double a = alphaarray[j];

				double c00 = coefficientarray[j][i][0];
				double c01 = coefficientarray[j][i][1];
				double c02 = coefficientarray[j][i][2];
				double c10 = coefficientarray[j][i][3];
				double c11 = coefficientarray[j][i][4];
				double c12 = coefficientarray[j][i][5];
				double c20 = coefficientarray[j][i][6];
				double c21 = coefficientarray[j][i][7];
				double c22 = coefficientarray[j][i][8];

				//the numer
				double numer = (1 - a) * (1 - a) * (1 - b) * (1 - b) * c00
					+ (1 - a) * (1 - a) * (1 - b) * b * c01
					+ (1 - a) * (1 - a) * b * b * c02  
					+ (1 - a) * a * (1 - b) * (1 - b) * c10 
					+ (1 - a) * a * (1 - b) * b * c11
					+ (1 - a) * a * b * b * c12 
					+ a * a * (1 - b) * (1 - b) * c20
					+ a * a * (1 - b) * b * c21  
					+ a * a * b * b * c22;

				//the denominator
				double denominator = a * (1 - a) * b * (1 - b);

				//compute the partial derivative of numer over alpha
				double partial_numer_over_b =   (1 - a) * (1 - a) * (-2) * (1 - b) * c00
					+ (1 - a) * (1 - a) * (1 - 2 * b) * c01
					+ (1 - a) * (1 - a) * 2 * b * c02  
					+ (1 - a) * a * (-2) * (1 - b) * c10 
					+ (1 - a) * a * (1 - 2 * b) * c11
					+ (1 - a) * a * 2 * b * c12 
					+ a * a * (-2) * (1 - b) * c20
					+ a * a * (1 - 2 * b) * c21  
					+ a * a * 2 * b * c22;

				double partial_denorminator_over_b = a * (1 - a) * (1 - 2 * b);

				bd += (partial_numer_over_b * denominator - numer * partial_denorminator_over_b) / SQR(denominator);
			}
			grad[M + i] = bd;
		}
		//for(int i = 0; i < x.length(); i++)
	//	TRACE(" %f ",x[i]);
	//	TRACE("\n");
	//	TRACE("fun = %f",func);
	//	TRACE("\n grad=[");
	//	for(int i = 0; i < grad.length(); i++)
	//		TRACE(" %f ",grad[i]);
	//	TRACE("]\n");
}
void Action_AreaPreserving::ComputeMaximal_Minimal_Pos_uv(std::vector<double> &m_Array_Zero_Derivatives,int SUBDIVISION_NUM, C_NurbsSurf* pNurbsSurf,int flag)
{
	//flag == 0 u, flag == 1 v
	C_Disline2D* pDisline = new C_Disline2D();

	for(int i = 0; i <= SUBDIVISION_NUM; i++)
	{
		double u = (double)i / SUBDIVISION_NUM;
		double area;
		Point2D point;
		if(flag == 0)
		{
			area = p_FormView3->Compute_Area(pNurbsSurf,u,1);
			point.x = u;
		}
		else
		{
			area = p_FormView3->Compute_Area(pNurbsSurf,1,u);
			point.x = u;
		}

		point.y = area;
		pDisline->AddDispoint(new C_Point2D(point));
	}

	CDisline2D* pLine = new CDisline2D();
	pLine->pDisline2D = pDisline;
	pLine->type = TYPE_DISLINE2D;
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	//pLine->m_Transform.Copy(*pNSurf->GetTransform());
	pDoc->m_SceneGraph.Add(pLine);

	//
	Point3D* pPointArray = new Point3D[SUBDIVISION_NUM + 1];
	for(int i = 0; i <= SUBDIVISION_NUM; i++)
	{
		pPointArray[i].x = pDisline->GetDispoint(i)->GetPoint().x;
		pPointArray[i].y = pDisline->GetDispoint(i)->GetPoint().y;
		pPointArray[i].z = 0;
	}
	C_NurbsCurve3D* pInterpolationCurve = new C_NurbsCurve3D(SUBDIVISION_NUM + 1,4,pPointArray);
	CNurbsCurve* pCurve = new CNurbsCurve();
	pCurve->pCurve = pInterpolationCurve;
	pCurve->filename = "Nurbs Curve";
	pCurve->type = TYPE_NURBSCURVE3D;
	pDoc->m_SceneGraph.Add(pCurve);

	//the curve derivatives
	for(int i = 0; i <= SUBDIVISION_NUM; i++)
	{
		Vector3D d1 = pInterpolationCurve->ComputDerivative1((double)i / SUBDIVISION_NUM);
		Vector3D pos = pInterpolationCurve->ComputPosition((double)i / SUBDIVISION_NUM) - ZeroP3D;
		pPointArray[i].x = pos.x;
		pPointArray[i].y = d1.y / d1.x;
		pPointArray[i].z = 0;
	}
	for(int i = 0; i <= SUBDIVISION_NUM; i++)
	{
		if(i >= 2)
		{
			if(pPointArray[i].y >= pPointArray[i - 1].y && pPointArray[i].y >= pPointArray[i - 2].y && pPointArray[i].y >= pPointArray[i + 1].y && pPointArray[i].y >= pPointArray[i + 2].y)
			{
				m_Array_Zero_Derivatives.push_back(pPointArray[i].x);
			}else
				if(pPointArray[i].y <= pPointArray[i - 1].y && pPointArray[i].y <= pPointArray[i - 2].y && pPointArray[i].y <= pPointArray[i + 1].y && pPointArray[i].y <= pPointArray[i + 2].y)
				{
					m_Array_Zero_Derivatives.push_back(pPointArray[i].x);
				}
		}
	}
	C_NurbsCurve3D* pInterpolationCurve1 = new C_NurbsCurve3D(SUBDIVISION_NUM + 1,4,pPointArray);
	CNurbsCurve* pCurve1 = new CNurbsCurve();
	pCurve1->pCurve = pInterpolationCurve1;
	pCurve1->filename = "Derivative";
	pCurve1->type = TYPE_NURBSCURVE3D;
	pDoc->m_SceneGraph.Add(pCurve1);
}
void Action_AreaPreserving::ComputeOptimalSubdivision(std::vector<double> &m_ArrayUsubdivision,std::vector<double> &m_ArrayVsubdivision, C_NurbsSurf* pNurbsSurf)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	//
	int SUBDIVISION_NUM = 100;
	
	//
	ComputeMaximal_Minimal_Pos_uv(m_ArrayUsubdivision,SUBDIVISION_NUM,pNurbsSurf,0);

	//
	ComputeMaximal_Minimal_Pos_uv(m_ArrayVsubdivision,SUBDIVISION_NUM,pNurbsSurf,1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);	
}
string DoubleToString(double d);
double Action_AreaPreserving::Compute_Optimal_Composite_Mobiustransformation(int Mseg,int Nseg,C_NurbsSurf* pNurbsSurf)
{
	coefficientarray.clear();

	//Set Global variable
	g_Mseg = Mseg;
	g_Nseg = Nseg;

	//segmentation
	int usegmentnum = Mseg;
	int vsegmentnum = Nseg;

	std::vector<double> uknot,vknot,sknot,tknot;
	/*uknot.reserve(usegmentnum + 1);
	vknot.reserve(vsegmentnum + 1);
	
	sknot.reserve(usegmentnum + 1);
	tknot.reserve(vsegmentnum + 1);

	uknot.push_back(0);
	vknot.push_back(0);

	//give a method to get the optimal subdivison of the u&v parameter intervals
	//ComputeOptimalSubdivision(uknot,vknot,pNurbsSurf);

	//uknot.push_back(1);
	//vknot.push_back(1);
	TRACE("\n");
	for(int i = 0; i < uknot.size(); i++)
	{
		TRACE(" %f ",uknot[i]);
	}
	TRACE("\n");
	for(int i = 0; i < vknot.size(); i++)
	{
		TRACE(" %f ",vknot[i]);
	}
	TRACE("\n");*/
	uknot.clear();
	vknot.clear();
	//set u & v knots
	for(int i = 0; i <= usegmentnum; i++)
	{
		uknot.push_back(i * 1.0 / usegmentnum);
	}
	for(int j = 0; j <= vsegmentnum; j++)
	{
		vknot.push_back(j * 1.0 / vsegmentnum);
	}
	usegmentnum = uknot.size() - 1;
	vsegmentnum = vknot.size() - 1;
	g_Mseg = Mseg = usegmentnum;
	g_Nseg = Nseg = vsegmentnum;

	//the area of the whole surface
	double A = p_FormView3->Compute_Area(pNurbsSurf,1,1);

	//set the s & t values.
	for(int i = 0; i <= usegmentnum; i++)
		sknot.push_back(p_FormView3->Compute_Area(pNurbsSurf,uknot[i],1) / A);

	for(int j = 0; j <= vsegmentnum; j++)
		tknot.push_back(p_FormView3->Compute_Area(pNurbsSurf,1,vknot[j]) / A);
	
	
	std::vector<std::vector<double>> columnsurfacecoefficient;
	std::vector<double> subsurfacecoefficient;
	for(int i = 0; i < usegmentnum; i++)
	{
		columnsurfacecoefficient.clear();
		for(int j = 0; j < vsegmentnum; j++)
		{
			subsurfacecoefficient.clear();
			C_NurbsSurf* newsurf = (C_NurbsSurf*)pNurbsSurf->Copy();
			double deltsi,delttj;
			deltsi = sknot[i + 1] - sknot[i];
			delttj = tknot[j + 1] - tknot[j];
			
			C_NurbsSurf* newsubsurf = newsurf->GetSubSurface(uknot[i],uknot[i+1],vknot[j],vknot[j+1]);
			double c00 = p_FormView3->Compute_Coefficients_Cij_NumericalMethod(newsubsurf,0,0);
			double c01 = p_FormView3->Compute_Coefficients_Cij_NumericalMethod(newsubsurf,0,1);
			double c02 = p_FormView3->Compute_Coefficients_Cij_NumericalMethod(newsubsurf,0,2);
			double c10 = p_FormView3->Compute_Coefficients_Cij_NumericalMethod(newsubsurf,1,0);
			double c11 = p_FormView3->Compute_Coefficients_Cij_NumericalMethod(newsubsurf,1,1);
			double c12= p_FormView3->Compute_Coefficients_Cij_NumericalMethod(newsubsurf,1,2);
			double c20 = p_FormView3->Compute_Coefficients_Cij_NumericalMethod(newsubsurf,2,0);
			double c21 = p_FormView3->Compute_Coefficients_Cij_NumericalMethod(newsubsurf,2,1);
			double c22 = p_FormView3->Compute_Coefficients_Cij_NumericalMethod(newsubsurf,2,2);
			subsurfacecoefficient.push_back(c00 / (deltsi * delttj));
			subsurfacecoefficient.push_back(c01 / (deltsi * delttj));
			subsurfacecoefficient.push_back(c02 / (deltsi * delttj));
			subsurfacecoefficient.push_back(c10 / (deltsi * delttj));
			subsurfacecoefficient.push_back(c11 / (deltsi * delttj));
			subsurfacecoefficient.push_back(c12 / (deltsi * delttj));
			subsurfacecoefficient.push_back(c20 / (deltsi * delttj));
			subsurfacecoefficient.push_back(c21 / (deltsi * delttj));
			subsurfacecoefficient.push_back(c22 / (deltsi * delttj));
			TRACE("%f %f %f %f %f %f %f %f %f\n",subsurfacecoefficient[0],subsurfacecoefficient[1],subsurfacecoefficient[2],subsurfacecoefficient[3],
				                                 subsurfacecoefficient[4],subsurfacecoefficient[5],subsurfacecoefficient[6],subsurfacecoefficient[7],
												 subsurfacecoefficient[8]);
			columnsurfacecoefficient.push_back(subsurfacecoefficient);
			delete newsurf;
			delete newsubsurf;
		}
		coefficientarray.push_back(columnsurfacecoefficient);
	}
	
	p_FormView3->OutPutCompositeEnergyMesh(coefficientarray,"d:\\energy.obj");
	//return -1;
	//M alpha N beta
	std::vector<double> alphaArray,betaarray;
	for(int i = 0; i < usegmentnum; i++)
		alphaArray.push_back(0.5);
	for(int i = 0; i < vsegmentnum; i++)
		betaarray.push_back(0.5);

	string str="[";
	for(int j = 0; j < usegmentnum; j++)
	{
		str += DoubleToString(alphaArray[j]) + ",";
	}
	for(int j = 0; j < vsegmentnum; j++)
	{
		if(j < vsegmentnum - 1)
			str += DoubleToString(betaarray[j]) + ",";
		else
			str += DoubleToString(betaarray[j]) + "]";
	}

	real_1d_array x(str.c_str());
	double epsg = 0.0000000001;
	double epsf = 0;
	double epsx = 0;
	ae_int_t maxits = 0;
	minlbfgsstate state;
	minlbfgsreport rep;

	minlbfgscreate(1, x, state);
	minlbfgssetcond(state, epsg, epsf, epsx, maxits);
	alglib::minlbfgsoptimize(state, CompositeAreaPreserving_grad);
	minlbfgsresults(state, x, rep);

	std::vector<double> alphaarray_,betaarray_;
	for(int i = 0; i < usegmentnum; i++)
		alphaarray_.push_back(x[i]);
	for(int j = 0; j < vsegmentnum; j++)
		betaarray_.push_back(x[usegmentnum + j]);

	double z = 0;
	for(int ii = 0; ii < usegmentnum; ii++)
		for(int jj = 0; jj < vsegmentnum; jj++)
		{
			double a = alphaarray_[ii];
			double b = betaarray_[jj];

			double c00 = coefficientarray[ii][jj][0];
			double c01 = coefficientarray[ii][jj][1];
			double c02 = coefficientarray[ii][jj][2];
			double c10 = coefficientarray[ii][jj][3];
			double c11 = coefficientarray[ii][jj][4];
			double c12 = coefficientarray[ii][jj][5];
			double c20 = coefficientarray[ii][jj][6];
			double c21 = coefficientarray[ii][jj][7];
			double c22 = coefficientarray[ii][jj][8];

			z += ( (1 - a) * (1 - a) * (1 - b) * (1 - b) * c00
				+ (1 - a) * (1 - a) * (1 - b) * b * c01
				+ (1 - a) * (1 - a) * b * b * c02  
				+ (1 - a) * a * (1 - b) * (1 - b) * c10 
				+ (1 - a) * a * (1 - b) * b * c11
				+ (1 - a) * a * b * b * c12 
				+ a * a * (1 - b) * (1 - b) * c20
				+ a * a * (1 - b) * b * c21  
				+ a * a * b * b * c22) / (a * (1 - a) * b * (1 - b));
		}
	TRACE("Segmentation: %d %d,Energy: %f",usegmentnum,vsegmentnum,z);

	TRACE("\n");
	for(int i = 0; i < x.length(); i++)
		TRACE(" %f ",x[i]);

	CompositeMobiusTransformation* pTrans = new CompositeMobiusTransformation();
	pTrans->m_ReparameterizationType = compositemobius;
	pTrans->M = usegmentnum;
	pTrans->N = vsegmentnum;
	for(int i = 0; i <= usegmentnum; i++)
		pTrans->uknot.push_back(uknot[i]);
	for(int i = 0; i <= vsegmentnum; i++)
		pTrans->vknot.push_back(vknot[i]);
	for(int i = 0; i <= usegmentnum; i++)
		pTrans->sknot.push_back(sknot[i]);
	for(int i = 0; i <= vsegmentnum; i++)
		pTrans->tknot.push_back(tknot[i]);
	for(int i = 0; i < usegmentnum; i++)
		pTrans->alpha.push_back(x[i]);
	for(int i = 0; i < vsegmentnum; i++)
		pTrans->beta.push_back(x[usegmentnum + i]);

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	CNurbsSuface* pSurface = pNurbsSurf->GetCNurbsSurface();
	pSurface->SetTransform(*pNSurf->GetTransform());
	pSurface->filename = "Composite Optimized Surface";
	pSurface->ptransformation = pTrans;
	pDoc->m_SceneGraph.Add(pSurface);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	return z;
}

void Action_AreaPreserving::GenerateFaceComposite_InitialSurface(C_NurbsSurf* pSurface)
{
	//
	int usegmentnum = 10;
	int vsegmentnum = 10;
	int Mseg,Nseg;

	std::vector<double> uknot,vknot;
	//set u & v knots
	for(int i = 0; i <= usegmentnum; i++)
	{
		uknot.push_back(i * 1.0 / usegmentnum);
	}
	for(int j = 0; j <= vsegmentnum; j++)
	{
		vknot.push_back(j * 1.0 / vsegmentnum);
	}
	usegmentnum = uknot.size() - 1;
	vsegmentnum = vknot.size() - 1;
	g_Mseg = Mseg = usegmentnum;
	g_Nseg = Nseg = vsegmentnum;

	std::vector<double> sknot,tknot;
	//set the s & t values.
	for(int i = 0; i <= usegmentnum; i++)
		sknot.push_back(uknot[i]);

	for(int j = 0; j <= vsegmentnum; j++)
		tknot.push_back(vknot[j]);

	std::vector<double> alphaArray,betaarray;
	for(int i = 0; i < usegmentnum; i++)
		alphaArray.push_back(0.5);
	for(int i = 0; i < vsegmentnum; i++)
		betaarray.push_back(0.5);

	CompositeMobiusTransformation* pTrans = new CompositeMobiusTransformation();
	pTrans->m_ReparameterizationType = compositemobius;
	pTrans->M = usegmentnum;
	pTrans->N = vsegmentnum;
	for(int i = 0; i <= usegmentnum; i++)
		pTrans->uknot.push_back(uknot[i]);
	for(int i = 0; i <= vsegmentnum; i++)
		pTrans->vknot.push_back(vknot[i]);
	for(int i = 0; i <= usegmentnum; i++)
		pTrans->sknot.push_back(sknot[i]);
	for(int i = 0; i <= vsegmentnum; i++)
		pTrans->tknot.push_back(tknot[i]);
	for(int i = 0; i < usegmentnum; i++)
		pTrans->alpha.push_back(alphaArray[i]);
	for(int i = 0; i < vsegmentnum; i++)
		pTrans->beta.push_back(betaarray[i]);

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	pNSurf->ptransformation = pTrans;
	pDoc->UpdateAllViews(NULL);
}
void Action_AreaPreserving::OnBnClickedFacecompositetransformation()
{
	// TODO: Add your control notification handler code here
	//Generate the initial surface.
	
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num=pDoc->m_SceneGraph.NbObject();
	if(num!=1)
		return;

	CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);

	C_NurbsSurf* pSurf = pNSurf->Converte_C_NurbsSurface();
	GenerateFaceComposite_InitialSurface(pSurf);
}


void Action_AreaPreserving::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CPropertyPage::OnPaint() for painting messages
	//dc.MoveTo(100, 100);
	//dc.LineTo(1200, 1200);

	/*CDC * dc1 = CWnd::GetDC();
	dc1->MoveTo(100, 100); dc1->LineTo(100, 400);
	dc1->LineTo(400, 400); dc1->LineTo(400, 100);
	dc1->LineTo(100, 100);
	dc1->DeleteDC();

	CClientDC dc(this);//设置当前客户区大小为设备上下文的大小
	dc.SetMapMode(MM_LOMETRIC);//设置逻辑坐标系:单位0.1mm.正Y轴向下
							   //原点屏幕左上角.
	CPen pen;
	pen.CreatePen(PS_DOT, 0, RGB(222, 222, 222));    //PS_SOLID---实线 
													 //DOT--点阵//1---画笔宽度,0为一个像素点,1则为两个像素点
													 //RGB=read&green&blue,数字范围0至255,随便调吧
	CPen* pOldPen = dc.SelectObject(&pen); //将pen选入DC,同时将被替换下来的画笔存入pOldPen中.以备使用后,恢复原始状态用.
	dc.MoveTo(100, -100);//移动到直线的起点坐标
	dc.LineTo(1000, -500);//画直线到直线的终点
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	pOldPen->DeleteObject();*/
}


void Action_AreaPreserving::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CPropertyPage::OnNcPaint() for painting messages
	//int i = 0;
}


void Action_AreaPreserving::OnBnClickedFaceexample()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Action_AreaPreserving::OnBnClickedBonestep1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Action_AreaPreserving::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
}
