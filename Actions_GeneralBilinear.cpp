// Actions_GeneralBilinear.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Actions_GeneralBilinear.h"
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
#include "ShowTransformedParameterDomain.h"
// Actions_GeneralBilinear dialog

IMPLEMENT_DYNAMIC(Actions_GeneralBilinear, CPropertyPage)

Actions_GeneralBilinear::Actions_GeneralBilinear()
	: CPropertyPage(Actions_GeneralBilinear::IDD)
	, m_orthogonalityweight(0)
{

	m_orthogonalityweight = 50;
}

Actions_GeneralBilinear::~Actions_GeneralBilinear()
{
}

void Actions_GeneralBilinear::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_Orthogonality_Weight, m_orthogonalityweight);
}


BEGIN_MESSAGE_MAP(Actions_GeneralBilinear, CPropertyPage)
	ON_BN_CLICKED(IDC_Actions_GeneralBilinear_Bezier, &Actions_GeneralBilinear::OnBnClickedActionsBezier)
	ON_BN_CLICKED(IDC_IDC_Actions_GeneralBilinear_GenerateSubMesh, &Actions_GeneralBilinear::OnBnClickedIdcActionsGeneralbilinearGeneratesubmesh)
	ON_BN_CLICKED(IDC_Actions_GeneralBilinear_Optimization, &Actions_GeneralBilinear::OnBnClickedActionsGeneralbilinearOptimization)
	ON_BN_CLICKED(IDC_GENERALBILINEAR_POVRAY, &Actions_GeneralBilinear::OnBnClickedGeneralbilinearPovray)
	ON_BN_CLICKED(IDC_BUTTON_CARTOP, &Actions_GeneralBilinear::OnBnClickedButtonCartop)
	ON_BN_CLICKED(IDC_BUTTON_Face, &Actions_GeneralBilinear::OnBnClickedButtonFace)
	ON_BN_CLICKED(IDC_BUTTON_Bone, &Actions_GeneralBilinear::OnBnClickedButtonBone)
	ON_BN_CLICKED(IDC_BUTTON_Venus, &Actions_GeneralBilinear::OnBnClickedButtonVenus)
	ON_BN_CLICKED(IDC_BUTTON_Sophie, &Actions_GeneralBilinear::OnBnClickedButtonSophie)
	ON_BN_CLICKED(IDC_BUTTON_Terrain, &Actions_GeneralBilinear::OnBnClickedButtonTerrain)
	ON_BN_CLICKED(IDC_Actions_GeneralBilinear_Plot, &Actions_GeneralBilinear::OnBnClickedActionsGeneralbilinearPlot)
	ON_BN_CLICKED(IDC_BUTTON_GeneralBilinearInitialValue, &Actions_GeneralBilinear::OnBnClickedButtonGeneralbilinearinitialvalue)
END_MESSAGE_MAP()


// Actions_GeneralBilinear message handlers
//Get the initial general bilinear transformation
void Actions_GeneralBilinear::ComputeInitialGeneralBilinearTransformation(
	CMesh3d* pMesh1,CMesh3d* pMesh2,std::vector<std::vector<double>> &optinumx,int flag)
{
	//pMesh1 the obtained 2D conformal parameterization from 3D triangle mesh
	//pMesh2 the 2D uniform grid

	//flag = 0, compute the initial general bilinear transformation for u
	//flag = 1, compute the initial general bilinear transformation for v

	std::vector<std::vector<double>> A;
	std::vector<double> b;
	std::vector<double> temp;
	GeneralBilinearTransformation ptrans;
	double c1,c2,c3,c4,c5,c6,c7;

	int optimalindex = -1;
	double minenergy = 1e10;
	//std::vector<std::vector<double>> m_ArrayX;
	//0 f00 = 1; 1 f01 = 1; 2 f10 = 1; 3 f11 = 1;
	for(int index = 0; index < 4; index++)
	{
		A.clear();
		b.clear();
		temp.clear();

		//eij
		for(int ii = 0; ii <= 1; ii++)
			for(int jj = 0; jj <= 1; jj++)
			{
				c1 = 0;
				c2 = 0;
				c3 = 0;
				c4 = 0;
				c5 = 0;
				c6 = 0;
				c7 = 0;
				for(int i = 0; i < pMesh1->NbVertex(); i++)
				{
					CVertex3d* pVertex1 = pMesh2->GetVertex(i);
					CVertex3d* pVertex2 = pMesh1->GetVertex(i);
					if(pVertex1->GetFlag() == 1)
						continue;

					double u = pVertex1->x();
					double v = pVertex1->y();
					double z1 = pVertex1->z();

					double s = pVertex2->x();
					double t = pVertex2->y();
					double z2 = pVertex2->z();

					double v1,v2;
					if(ii == 0)
						v1 = 1 - s;
					else
						v1 = s;
					if(jj == 0)
						v2 = 1 - t;
					else
						v2 = t;

					c1 += (1 - s) * (1 - t) * v1 * v2;
					c2 += (1 - s) * t  * v1 * v2;
					c3 += (s) * (1 - t)  * v1 * v2;
					c4 += s * t  * v1 * v2;

					double value_;
					if(flag == 0)
						value_ = u;
					else if(flag == 1)
						value_ = v;

					if(index == 0)
					{
						c5 += (1 - s ) * t  * v1 * v2 * (-value_);
						c6 += (s) * (1 - t)  * v1 * v2 * (-value_);
						c7 += (s) * (t)  * v1 * v2 * (-value_);
					}else if(index == 1)
					{
						c5 += (1 - s ) * (1 - t)  * v1 * v2 * (-value_);
						c6 += (s) * (1 - t)  * v1 * v2 * (-value_);
						c7 += (s) * (t)  * v1 * v2 * (-value_);
					}else if(index == 2)
					{
						c5 += (1 - s ) * (1 - t)  * v1 * v2 * (-value_);
						c6 += (1 - s ) * t  * v1 * v2 * (-value_);
						c7 += (s) * (t)  * v1 * v2 * (-value_);
					}else if(index == 3)
					{
						c5 += (1 - s ) * (1 - t)  * v1 * v2 * (-value_);
						c6 += (1 - s ) * t  * v1 * v2 * (-value_);
						c7 += (s) * (1 - t)  * v1 * v2 * (-value_);
					}

				}
				temp.clear();
				//#ifdef _DEBUG
				//TRACE("%f %f %f %f %f %f %f",c1,c2,c3,c4,c5,c6,c7);
				//#endif
				temp.push_back(c1);
				temp.push_back(c2);
				temp.push_back(c3);
				temp.push_back(c4);
				temp.push_back(c5);
				temp.push_back(c6);
				temp.push_back(c7);
				A.push_back(temp);
			}

			//fij
			for(int ii = 0; ii <= 1; ii++)
				for(int jj = 0; jj <= 1; jj++)
				{
					if(index == 0)
					{
						if(ii == 0 && jj == 0)
							continue;
					}else if(index == 1)
					{
						if(ii == 0 && jj == 1)
							continue;
					}else if(index == 2)
					{
						if(ii == 1 && jj == 0)
							continue;
					}else if(index == 3)
					{
						if(ii == 1 && jj == 1)
							continue;
					}

					c1 = 0;
					c2 = 0;
					c3 = 0;
					c4 = 0;
					c5 = 0;
					c6 = 0;
					c7 = 0;
					for(int i = 0; i < pMesh1->NbVertex(); i++)
					{
						CVertex3d* pVertex1 = pMesh2->GetVertex(i);
						CVertex3d* pVertex2 = pMesh1->GetVertex(i);

						if(pVertex1->GetFlag() == 1)
							continue;

						double u = pVertex1->x();
						double v = pVertex1->y();
						double z1 = pVertex1->z();

						double s = pVertex2->x();
						double t = pVertex2->y();
						double z2 = pVertex2->z();

						double v1,v2;
						if(ii == 0)
							v1 = 1 - s;
						else
							v1 = s;
						if(jj == 0)
							v2 = 1 - t;
						else
							v2 = t;

						double value_;

						if(flag == 0)
							value_ = u;
						else if(flag == 1)
							value_ = v;

						c1 += (1 - s) * (1 - t) * v1 * v2 * (-value_);
						c2 += (1 - s) * t  * v1 * v2 * (-value_);
						c3 += (s) * (1 - t)  * v1 * v2 * (-value_);
						c4 += s * t  * v1 * v2 * (-value_);

						if(index == 0)
						{
							c5 += (1 - s ) * t  * v1 * v2 * (value_ * value_);
							c6 += (s) * (1 - t)  * v1 * v2 * (value_ * value_);
							c7 += (s) * (t)  * v1 * v2 * (value_ * value_);
						}else if(index == 1)
						{
							c5 += (1 - s ) * (1 - t) * v1 * v2 * (value_ * value_);
							c6 += (s) * (1 - t)  * v1 * v2 * (value_ * value_);
							c7 += (s) * (t)  * v1 * v2 * (value_ * value_);
						}else if(index == 2)
						{
							c5 += (1 - s ) * (1 - t) * v1 * v2 * (value_ * value_);
							c6 += (1 - s ) * t  * v1 * v2 * (value_ * value_);
							c7 += (s) * (t)  * v1 * v2 * (value_ * value_);
						}else if(index == 3)
						{
							c5 += (1 - s ) * (1 - t) * v1 * v2 * (value_ * value_);
							c6 += (1 - s ) * t  * v1 * v2 * (value_ * value_);
							c7 += (s) * (1 - t)  * v1 * v2 * (value_ * value_);
						}

					}
					temp.clear();
					//#ifdef _DEBUG
					//TRACE("%f %f %f %f %f %f %f",c1,c2,c3,c4,c5,c6,c7);
					//#endif
					temp.push_back(c1);
					temp.push_back(c2);
					temp.push_back(c3);
					temp.push_back(c4);
					temp.push_back(c5);
					temp.push_back(c6);
					temp.push_back(c7);
					A.push_back(temp);
				}

				//b

				c1 = 0;
				c2 = 0;
				c3 = 0;
				c4 = 0;
				c5 = 0;
				c6 = 0;
				c7 = 0;
				for(int i = 0; i < pMesh1->NbVertex(); i++)
				{
					CVertex3d* pVertex1 = pMesh2->GetVertex(i);
					CVertex3d* pVertex2 = pMesh1->GetVertex(i);
					if(pVertex1->GetFlag() == 1)
						continue;

					double u = pVertex1->x();
					double v = pVertex1->y();
					double z1 = pVertex1->z();

					double s = pVertex2->x();
					double t = pVertex2->y();
					double z2 = pVertex2->z();

					double v1,v2;

					if(index == 0)
					{
						v1 = 1 - s;
						v2 = 1 - t;
					}else if(index == 1)
					{
						v1 = 1 - s;
						v2 = t;
					}else if(index == 2)
					{
						v1 = s;
						v2 = 1 - t;
					}else if(index == 3)
					{
						v1 = s;
						v2 = t;
					}

					double value_;
					if(flag == 0)
						value_ = u;
					else if(flag == 1)
						value_ = v;

					c1 += (1 - s) * (1 - t) * v1 * v2 * (value_);
					c2 += (1 - s) * t  * v1 * v2 * (value_);
					c3 += (s) * (1 - t)  * v1 * v2 * (value_);
					c4 += s * t  * v1 * v2 * (value_);
					c5 += (1 - s ) * t  * v1 * v2 * (-value_ * value_);
					c6 += (s) * (1 - t)  * v1 * v2 * (-value_ * value_);
					c7 += (s) * (t)  * v1 * v2 * (-value_ * value_);
				}
				b.clear();
				//#ifdef _DEBUG
				//TRACE("%f %f %f %f %f %f %f",c1,c2,c3,c4,c5,c6,c7);
				//#endif
				b.push_back(c1);
				b.push_back(c2);
				b.push_back(c3);
				b.push_back(c4);
				b.push_back(c5);
				b.push_back(c6);
				b.push_back(c7);

				std::vector<double> x;
				x.clear();
				p_FormView3->SolveLinearSystem(A,b,x);

				double e00 = x[0];
				double e01 = x[1];
				double e10 = x[2];
				double e11 = x[3];

				double f00,f01,f10,f11;
				if(index == 0)
				{
					f00 = 1;
					f01 = x[4];
					f10 = x[5];
					f11 = x[6];
				}else if(index == 1)
				{
					f00 = x[4];
					f01 = 1;
					f10 = x[5];
					f11 = x[6];
				}else if(index == 2)
				{
					f00 = x[4];
					f01 = x[5];
					f10 = 1;
					f11 = x[6];			
				}else if(index == 3)
				{
					f00 = x[4];
					f01 = x[5];
					f10 = x[6];
					f11 = 1;	
				}
				std::vector<double> temparray;
				temparray.push_back(e00);
				temparray.push_back(e01);
				temparray.push_back(e10);
				temparray.push_back(e11);
				temparray.push_back(f00);
				temparray.push_back(f01);
				temparray.push_back(f10);
				temparray.push_back(f11);
 				optinumx.push_back(temparray);
				//compute the energy
				//

				/*double energy = 0;
				for(int i = 0; i < pMesh1->NbVertex(); i++)
				{
					CVertex3d* pVertex1 = pMesh2->GetVertex(i);
					CVertex3d* pVertex2 = pMesh1->GetVertex(i);

					double u = pVertex1->x();
					double v = pVertex1->y();
					double z1 = pVertex1->z();

					double s = pVertex2->x();
					double t = pVertex2->y();
					double z2 = pVertex2->z();

					double numer,denominator;
					numer = (e00 * (1 - s) * (1 - t) + e01 * (1- s) * t + e10 * s * (1 - t) + e11 * s * t);
					denominator = (f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);
					//if(fabs(denominator) < 1e-1)
					//{
					//	ASSERT(0);
					//}
					double u_ =  numer / denominator;
					energy += SQR(u - u_);
				}
				if(energy < minenergy)
				{
					minenergy = energy;
					optinumx = x;
					optimalindex = index;
				}*/
	}

	//Get the optimal general bilinear transformation.
	/*std::vector<double> x;
	for(int i = 0; i < 4 + optimalindex; i++)
		x.push_back(optinumx[i]);
	x.push_back(1.0);
	for(int i = 4 + optimalindex; i < 7; i++)
		x.push_back(optinumx[i]);
	optinumx = x;*/
	//#ifdef _DEBUG
	//TRACE("%f %f %f %f %f %f %f \n",x[0],x[1],x[2],x[3],x[4],x[5],x[7]);
	//#endif
	//return optimalindex;
}
int Actions_GeneralBilinear::CheckSingularLineInParameterDomain(double f00,double f01,double f10,double f11,double umin,double vmin,double umax,double vmax)
{
	double s,t;
	double value1;
	for(int i = 0; i < 10000; i++)
	{
		s = umin + (umax - umin) *  i * 0.0001;
		t = vmin;
		value1 = (f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);
		if(fabs(value1) < 1e-2)
		{
			return 0;
		}
		t = vmin + (vmax - vmin) * i * 0.0001;
		s = umax;
		value1 = (f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);
		if(fabs(value1) < 1e-2)
		{
			return 0;
		}
		t = vmax;
		s = umin + (umax - umin) *  i * 0.0001;
		value1 = (f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);
		if(fabs(value1) < 1e-2)
		{
			return 0;
		}
		t = vmin + (vmax - vmin) * i * 0.0001;
		s = umin;
		value1 = (f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);
		if(fabs(value1) < 1e-2)
		{
			return 0;
		}
	}
	return 1;
	/*s = umin;
	t = vmin;
	double value1 = (f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);
	s = umax;
	t = vmin;
	double value2 = (f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);
	s = umax;
	t = vmax;
	double value3 = (f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);
	s = umin; 
	t = vmax;
	double value4 = (f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);

	int sig1,sig2,sig3,sig4;
	if(value1 >= 0)
		sig1 = 1;
	else
		sig1 = -1;
	if(value2 >= 0)
		sig2 = 1;
	else
		sig2 = -1;
	if(value3 >= 0)
		sig3 = 1;
	else
		sig3 = -1;
	if(value4 >= 0)
		sig4 = 1;
	else
		sig4 = -1;
	if(sig1 * sig2 < -1 || sig2 * sig3 < -1 || sig3 * sig4 < -1 || sig4 * sig1 < -1)
		return 0;
	else
		return 1;*/
}
void Actions_GeneralBilinear::OnBnClickedActionsGeneralbilinearInitialvalue_Degenerate()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num=pDoc->m_SceneGraph.NbObject();
	if(num != 3)
		return;
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	//pMesh1 the 2D uniform grid
	//pMesh2 the obtained 2D conformal parameterization from 3D triangle mesh

	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(2);
	C_NurbsSurf* pNurbsSurf = pNurbs->Converte_C_NurbsSurface();
	Point3D lower,upper;
	pMesh2->ComputeBoundingBox(lower,upper);

	int index00,indexm0,index0n,indexmn;
	pMesh1->GetFourCornerPoints(index00,indexm0,index0n,indexmn);

	double s0 = pMesh2->GetVertex(index00)->x();
	double t0 = pMesh2->GetVertex(index00)->y();

	double u0 = pMesh1->GetVertex(index00)->x();
	double v0 = pMesh1->GetVertex(index00)->y();

	double s1 = pMesh2->GetVertex(indexm0)->x();
	double t1 = pMesh2->GetVertex(indexm0)->y();

	double u1 = pMesh1->GetVertex(indexm0)->x();
	double v1 = pMesh1->GetVertex(indexm0)->y();

	double s2 = pMesh2->GetVertex(index0n)->x();
	double t2 = pMesh2->GetVertex(index0n)->y();

	double u2 = pMesh1->GetVertex(index0n)->x();
	double v2 = pMesh1->GetVertex(index0n)->y();

	double s3 = pMesh2->GetVertex(indexmn)->x();
	double t3 = pMesh2->GetVertex(indexmn)->y();

	double u3 = pMesh1->GetVertex(indexmn)->x();
	double v3 = pMesh1->GetVertex(indexmn)->y();

	//
	double e00 = (s0*s3*t2*t3*u1-s1*s2*t1*t3*u0+s1*s2*t2*t3*u0+s1*s3*t1*t2*u0-s1*s3*t2*t3*u0+s0*s1*t0*t2*u3-s0*s1*t0*t3*u2-s0*s2*t0*t1*u3-s0*s1*t1*t2*u3+s0*s1*t1*t3*u2+s0*s2*t1*t2*u3-s0*s2*t2*t3*u1-s0*s3*t1*t3*u2-s0*s3*t0*t2*u1+s1*s2*t0*t1*u3-s1*s2*t0*t2*u3-s1*s3*t0*t1*u2+s1*s3*t0*t3*u2-s2*s3*t1*t2*u0+s2*s3*t1*t3*u0+s0*s2*t0*t3*u1+s0*s3*t0*t1*u2+s2*s3*t0*t2*u1-s2*s3*t0*t3*u1)/
		(s0*t3*s1*t1+s0*t1*s2*t2-s0*s2*t2*t3-s0*t1*s3*t3-s0*t2*s1*t1+s1*s2*t2*t3+s1*s3*t1*t2+s0*s3*t2*t3-s1*s2*t1*t3+s0*t2*t0*s1-s0*t3*t0*s1-s0*t1*t0*s2+s0*s2*t0*t3+s0*t1*t0*s3-s0*s3*t0*t2+s1*t1*t0*s2-s2*t2*t0*s1-s1*t1*t0*s3+s3*t3*t0*s1+s2*s3*t0*t2-s2*s3*t0*t3-s1*s3*t2*t3-s2*s3*t1*t2+s2*s3*t1*t3);
	double e01 = (s0*s3*t2*t3*u1-s1*s2*t1*t3*u0+s1*s2*t2*t3*u0+s1*s3*t1*t2*u0-s1*s3*t2*t3*u0-s0*s1*t1*t2*u3+s0*s1*t1*t3*u2+s0*s2*t1*t2*u3-s0*s2*t2*t3*u1-s0*s3*t1*t3*u2-s2*s3*t1*t2*u0+s2*s3*t1*t3*u0-s1*s2*t1*u3+s1*s2*t2*u3+s1*s3*t1*u2-s1*s3*t3*u2-s2*s3*t2*u1+s2*s3*t3*u1+s0*s2*t0*u3-s0*s2*t2*u3+s0*s3*t3*u2+s2*s3*t2*u0-s2*s3*t3*u0-s0*s3*t0*u2-s0*s3*t3*u1-s1*s3*t1*u0+s1*s3*t3*u0-s0*s1*t0*u3+s0*s1*t1*u3+s0*s3*t0*u1+s1*s2*t1*u0-s1*s2*t2*u0+s0*s1*t0*u2-s0*s1*t1*u2+s0*s2*t2*u1-s0*s2*t0*u1+s0*s1*t0*t2*u3-s0*s1*t0*t3*u2-s0*s2*t0*t1*u3-s0*s3*t0*t2*u1+s1*s2*t0*t1*u3-s1*s2*t0*t2*u3-s1*s3*t0*t1*u2+s1*s3*t0*t3*u2+s0*s2*t0*t3*u1+s0*s3*t0*t1*u2+s2*s3*t0*t2*u1-s2*s3*t0*t3*u1)/
		(s0*t3*s1*t1+s0*t1*s2*t2-s0*s2*t2*t3-s0*t1*s3*t3-s0*t2*s1*t1+s1*s2*t2*t3+s1*s3*t1*t2+s0*s3*t2*t3-s1*s2*t1*t3+s0*t2*t0*s1-s0*t3*t0*s1-s0*t1*t0*s2+s0*s2*t0*t3+s0*t1*t0*s3-s0*s3*t0*t2+s1*t1*t0*s2-s2*t2*t0*s1-s1*t1*t0*s3+s3*t3*t0*s1+s2*s3*t0*t2-s2*s3*t0*t3-s1*s3*t2*t3-s2*s3*t1*t2+s2*s3*t1*t3);
	double e10 = (s1*t1*t0*u2+s0*s3*t2*t3*u1-s1*s2*t1*t3*u0+s1*s2*t2*t3*u0+s1*s3*t1*t2*u0-s1*s3*t2*t3*u0-s0*s1*t1*t2*u3+s0*s1*t1*t3*u2+s0*s2*t1*t2*u3-s0*s2*t2*t3*u1-s0*s3*t1*t3*u2-s2*s3*t1*t2*u0+s2*s3*t1*t3*u0-s2*t2*t3*u0+s3*t2*t3*u0-s0*t0*t2*u3+s0*t0*t3*u2+s2*t0*t2*u3-s3*t0*t3*u2+s0*s1*t0*t2*u3-s0*s1*t0*t3*u2-s0*s2*t0*t1*u3-s0*s3*t0*t2*u1+s1*s2*t0*t1*u3-s1*s2*t0*t2*u3-s1*s3*t0*t1*u2+s1*s3*t0*t3*u2+s0*s2*t0*t3*u1+s0*s3*t0*t1*u2+s2*s3*t0*t2*u1-s2*s3*t0*t3*u1-s3*t2*t3*u1+s1*t1*t2*u3-s1*t1*t3*u2-s2*t1*t2*u3+s2*t2*t3*u1+s3*t1*t3*u2+s0*t1*t0*u3-s0*t3*t0*u1+t1*s2*t2*u0-s2*t2*t0*u1-t2*s1*t1*u0-s0*t1*t0*u2+s0*t2*t0*u1-t1*s3*t3*u0+s3*t3*t0*u1+t3*s1*t1*u0-s1*t1*t0*u3)/
		(s0*t3*s1*t1+s0*t1*s2*t2-s0*s2*t2*t3-s0*t1*s3*t3-s0*t2*s1*t1+s1*s2*t2*t3+s1*s3*t1*t2+s0*s3*t2*t3-s1*s2*t1*t3+s0*t2*t0*s1-s0*t3*t0*s1-s0*t1*t0*s2+s0*s2*t0*t3+s0*t1*t0*s3-s0*s3*t0*t2+s1*t1*t0*s2-s2*t2*t0*s1-s1*t1*t0*s3+s3*t3*t0*s1+s2*s3*t0*t2-s2*s3*t0*t3-s1*s3*t2*t3-s2*s3*t1*t2+s2*s3*t1*t3);
	double e11 = (s1*t1*t0*u2+s0*s3*t2*t3*u1-s1*s2*t1*t3*u0+s1*s2*t2*t3*u0+s1*s3*t1*t2*u0-s1*s3*t2*t3*u0-s0*s1*t1*t2*u3+s0*s1*t1*t3*u2+s0*s2*t1*t2*u3-s0*s2*t2*t3*u1-s0*s3*t1*t3*u2-s2*s3*t1*t2*u0+s2*s3*t1*t3*u0-s1*t2*u3+s1*t3*u2+s2*t1*u3+s3*t2*u1-s2*t3*u1-s3*t1*u2+u0*t2*s1-u0*t3*s1-u0*t1*s2+u0*s2*t3+u0*t1*s3-u0*s3*t2-s1*s2*t1*u3+s1*s2*t2*u3+s1*s3*t1*u2-s1*s3*t3*u2-s2*s3*t2*u1+s2*s3*t3*u1+s0*s2*t0*u3-s0*s2*t2*u3+s0*s3*t3*u2+s2*s3*t2*u0-s2*s3*t3*u0-s0*s3*t0*u2-s0*s3*t3*u1-s1*s3*t1*u0+s1*s3*t3*u0-s0*s1*t0*u3+s0*s1*t1*u3+s0*s3*t0*u1+s1*s2*t1*u0-s1*s2*t2*u0+s0*s1*t0*u2-s0*s1*t1*u2+s0*s2*t2*u1-s0*s2*t0*u1-s2*t2*t3*u0+s3*t2*t3*u0-s0*t0*t2*u3+s0*t0*t3*u2+s2*t0*t2*u3-s3*t0*t3*u2+s0*s1*t0*t2*u3-s0*s1*t0*t3*u2-s0*s2*t0*t1*u3-s0*s3*t0*t2*u1+s1*s2*t0*t1*u3-s1*s2*t0*t2*u3-s1*s3*t0*t1*u2+s1*s3*t0*t3*u2+s0*s2*t0*t3*u1+s0*s3*t0*t1*u2+s2*s3*t0*t2*u1-s2*s3*t0*t3*u1-s3*t2*t3*u1+s1*t1*t2*u3-s1*t1*t3*u2-s2*t1*t2*u3+s2*t2*t3*u1+s3*t1*t3*u2+s0*t1*u2-s0*t2*u1-s2*t0*u3+s3*t0*u2+s1*t0*u3-s3*t0*u1-s1*t0*u2+s2*t0*u1+s0*t2*u3-s0*t3*u2-s0*t1*u3+s0*t3*u1+s0*t1*t0*u3-s0*t3*t0*u1+t1*s2*t2*u0-s2*t2*t0*u1-t2*s1*t1*u0-s0*t1*t0*u2+s0*t2*t0*u1-t1*s3*t3*u0+s3*t3*t0*u1+t3*s1*t1*u0-s1*t1*t0*u3)/
		(s0*t3*s1*t1+s0*t1*s2*t2-s0*s2*t2*t3-s0*t1*s3*t3-s0*t2*s1*t1+s1*s2*t2*t3+s1*s3*t1*t2+s0*s3*t2*t3-s1*s2*t1*t3+s0*t2*t0*s1-s0*t3*t0*s1-s0*t1*t0*s2+s0*s2*t0*t3+s0*t1*t0*s3-s0*s3*t0*t2+s1*t1*t0*s2-s2*t2*t0*s1-s1*t1*t0*s3+s3*t3*t0*s1+s2*s3*t0*t2-s2*s3*t0*t3-s1*s3*t2*t3-s2*s3*t1*t2+s2*s3*t1*t3);

	double f00 = 1;
	double f01 = 1;
	double f10 = 1;
	double f11 = 1;

	double g00 = (s0*s3*t0*t1*v2-s0*s3*t0*t2*v1+s1*s2*t0*t1*v3-s1*s2*t0*t2*v3-s1*s3*t0*t1*v2+s1*s3*t0*t3*v2+s2*s3*t0*t2*v1-s0*s1*t1*t2*v3+s0*s1*t1*t3*v2+s0*s2*t1*t2*v3-s0*s2*t2*t3*v1-s0*s3*t1*t3*v2+s0*s3*t2*t3*v1-s1*s2*t1*t3*v0+s1*s2*t2*t3*v0+s1*s3*t1*t2*v0-s1*s3*t2*t3*v0-s2*s3*t1*t2*v0+s2*s3*t1*t3*v0+s0*s1*t0*t2*v3-s0*s1*t0*t3*v2-s2*s3*t0*t3*v1+s0*s2*t0*t3*v1-s0*s2*t0*t1*v3)/
		(s0*t3*s1*t1+s0*t1*s2*t2-s0*s2*t2*t3-s0*t1*s3*t3-s0*t2*s1*t1+s1*s2*t2*t3+s1*s3*t1*t2+s0*s3*t2*t3-s1*s2*t1*t3+s0*t2*t0*s1-s0*t3*t0*s1-s0*t1*t0*s2+s0*s2*t0*t3+s0*t1*t0*s3-s0*s3*t0*t2+s1*t1*t0*s2-s2*t2*t0*s1-s1*t1*t0*s3+s3*t3*t0*s1+s2*s3*t0*t2-s2*s3*t0*t3-s1*s3*t2*t3-s2*s3*t1*t2+s2*s3*t1*t3);
	double g01 = (-s0*s2*t2*t3*v1-s0*s3*t1*t3*v2+s0*s3*t2*t3*v1-s1*s2*t1*t3*v0+s1*s2*t2*t3*v0+s1*s3*t1*t2*v0-s1*s3*t2*t3*v0-s2*s3*t1*t2*v0+s2*s3*t1*t3*v0-s0*s1*t1*t2*v3+s0*s1*t1*t3*v2+s0*s2*t1*t2*v3+s0*s3*t0*t1*v2-s0*s3*t0*t2*v1+s1*s2*t0*t1*v3-s1*s2*t0*t2*v3-s1*s3*t0*t1*v2+s1*s3*t0*t3*v2+s2*s3*t0*t2*v1+s0*s1*t0*t2*v3-s0*s1*t0*t3*v2-s2*s3*t0*t3*v1+s0*s2*t0*t3*v1-s0*s2*t0*t1*v3-s1*s2*t1*v3+s1*s2*t2*v3+s1*s3*t1*v2-s1*s3*t3*v2-s2*s3*t2*v1+s2*s3*t3*v1-s0*s3*t0*v2-s0*s2*t2*v3+s0*s3*t3*v2+s2*s3*t2*v0-s2*s3*t3*v0+s0*s2*t0*v3+s0*s3*t0*v1+s0*s1*t1*v3-s0*s3*t3*v1-s1*s3*t1*v0+s1*s3*t3*v0-s0*s1*t0*v3-s0*s1*t1*v2+s0*s2*t2*v1+s1*s2*t1*v0-s1*s2*t2*v0+s0*s1*t0*v2-s0*s2*t0*v1)/
		(s0*t3*s1*t1+s0*t1*s2*t2-s0*s2*t2*t3-s0*t1*s3*t3-s0*t2*s1*t1+s1*s2*t2*t3+s1*s3*t1*t2+s0*s3*t2*t3-s1*s2*t1*t3+s0*t2*t0*s1-s0*t3*t0*s1-s0*t1*t0*s2+s0*s2*t0*t3+s0*t1*t0*s3-s0*s3*t0*t2+s1*t1*t0*s2-s2*t2*t0*s1-s1*t1*t0*s3+s3*t3*t0*s1+s2*s3*t0*t2-s2*s3*t0*t3-s1*s3*t2*t3-s2*s3*t1*t2+s2*s3*t1*t3);
	double g10 = (-s0*t3*t0*v1+s0*t1*t0*v3-s0*t1*t0*v2+s0*t2*t0*v1-s1*t1*t0*v3-t1*s3*t3*v0+s3*t3*t0*v1+t3*s1*t1*v0+s1*t1*t0*v2+t1*s2*t2*v0-s2*t2*t0*v1-t2*s1*t1*v0-s0*s2*t2*t3*v1-s0*s3*t1*t3*v2+s0*s3*t2*t3*v1-s1*s2*t1*t3*v0+s1*s2*t2*t3*v0+s1*s3*t1*t2*v0-s1*s3*t2*t3*v0-s2*s3*t1*t2*v0+s2*s3*t1*t3*v0-s0*s1*t1*t2*v3+s0*s1*t1*t3*v2+s0*s2*t1*t2*v3+s0*s3*t0*t1*v2-s0*s3*t0*t2*v1+s1*s2*t0*t1*v3-s1*s2*t0*t2*v3-s1*s3*t0*t1*v2+s1*s3*t0*t3*v2+s2*s3*t0*t2*v1+s0*s1*t0*t2*v3-s0*s1*t0*t3*v2-s2*s3*t0*t3*v1+s0*s2*t0*t3*v1-s0*s2*t0*t1*v3+s1*t1*t2*v3-s1*t1*t3*v2-s2*t1*t2*v3+s2*t2*t3*v1+s3*t1*t3*v2-s3*t2*t3*v1+s2*t0*t2*v3-s3*t0*t3*v2-s2*t2*t3*v0+s3*t2*t3*v0-s0*t0*t2*v3+s0*t0*t3*v2)/(s0*t3*s1*t1+s0*t1*s2*t2-s0*s2*t2*t3-s0*t1*s3*t3-s0*t2*s1*t1+s1*s2*t2*t3+s1*s3*t1*t2+s0*s3*t2*t3-s1*s2*t1*t3+s0*t2*t0*s1-s0*t3*t0*s1-s0*t1*t0*s2+s0*s2*t0*t3+s0*t1*t0*s3-s0*s3*t0*t2+s1*t1*t0*s2-s2*t2*t0*s1-s1*t1*t0*s3+s3*t3*t0*s1+s2*s3*t0*t2-s2*s3*t0*t3-s1*s3*t2*t3-s2*s3*t1*t2+s2*s3*t1*t3);
	double g11 = (v0*t2*s1-v0*t3*s1-v0*t1*s2+v0*s2*t3+v0*t1*s3-v0*s3*t2-s3*t1*v2+s3*t2*v1-s1*t2*v3+s1*t3*v2-s2*t3*v1+s2*t1*v3+s0*t2*v3-s0*t3*v2+s3*t0*v2-s2*t0*v3-s3*t0*v1+s1*t0*v3-s1*t0*v2+s2*t0*v1-s0*t1*v3+s0*t3*v1+s0*t1*v2-s0*t2*v1-s0*t3*t0*v1+s0*t1*t0*v3-s0*t1*t0*v2+s0*t2*t0*v1-s1*t1*t0*v3-t1*s3*t3*v0+s3*t3*t0*v1+t3*s1*t1*v0+s1*t1*t0*v2+t1*s2*t2*v0-s2*t2*t0*v1-t2*s1*t1*v0-s0*s2*t2*t3*v1-s0*s3*t1*t3*v2+s0*s3*t2*t3*v1-s1*s2*t1*t3*v0+s1*s2*t2*t3*v0+s1*s3*t1*t2*v0-s1*s3*t2*t3*v0-s2*s3*t1*t2*v0+s2*s3*t1*t3*v0-s0*s1*t1*t2*v3+s0*s1*t1*t3*v2+s0*s2*t1*t2*v3+s0*s3*t0*t1*v2-s0*s3*t0*t2*v1+s1*s2*t0*t1*v3-s1*s2*t0*t2*v3-s1*s3*t0*t1*v2+s1*s3*t0*t3*v2+s2*s3*t0*t2*v1+s0*s1*t0*t2*v3-s0*s1*t0*t3*v2-s2*s3*t0*t3*v1+s0*s2*t0*t3*v1-s0*s2*t0*t1*v3+s1*t1*t2*v3-s1*t1*t3*v2-s2*t1*t2*v3+s2*t2*t3*v1+s3*t1*t3*v2-s3*t2*t3*v1+s2*t0*t2*v3-s3*t0*t3*v2-s2*t2*t3*v0+s3*t2*t3*v0-s0*t0*t2*v3+s0*t0*t3*v2-s1*s2*t1*v3+s1*s2*t2*v3+s1*s3*t1*v2-s1*s3*t3*v2-s2*s3*t2*v1+s2*s3*t3*v1-s0*s3*t0*v2-s0*s2*t2*v3+s0*s3*t3*v2+s2*s3*t2*v0-s2*s3*t3*v0+s0*s2*t0*v3+s0*s3*t0*v1+s0*s1*t1*v3-s0*s3*t3*v1-s1*s3*t1*v0+s1*s3*t3*v0-s0*s1*t0*v3-s0*s1*t1*v2+s0*s2*t2*v1+s1*s2*t1*v0-s1*s2*t2*v0+s0*s1*t0*v2-s0*s2*t0*v1)/(s0*t3*s1*t1+s0*t1*s2*t2-s0*s2*t2*t3-s0*t1*s3*t3-s0*t2*s1*t1+s1*s2*t2*t3+s1*s3*t1*t2+s0*s3*t2*t3-s1*s2*t1*t3+s0*t2*t0*s1-s0*t3*t0*s1-s0*t1*t0*s2+s0*s2*t0*t3+s0*t1*t0*s3-s0*s3*t0*t2+s1*t1*t0*s2-s2*t2*t0*s1-s1*t1*t0*s3+s3*t3*t0*s1+s2*s3*t0*t2-s2*s3*t0*t3-s1*s3*t2*t3-s2*s3*t1*t2+s2*s3*t1*t3);

	double h00 = 1;
	double h01 = 1;
	double h10 = 1;
	double h11 = 1;

	CNurbsSuface* pNewNSurface = pNurbsSurf->GetCNurbsSurface();
	GeneralBilinearTransformation* generaltrans = new GeneralBilinearTransformation();
	generaltrans->umin = pNurbs->m_uknot[0];
	generaltrans->umax = pNurbs->m_uknot[pNurbs->m_uknotcount - 1];
	generaltrans->vmin = pNurbs->m_vknot[0];
	generaltrans->vmax = pNurbs->m_vknot[pNurbs->m_vknotcount - 1];
	generaltrans->umin_Reparameterization = lower.x;
	generaltrans->umax_Reparameterization = upper.x;
	generaltrans->vmin_Reparameterization = lower.y;
	generaltrans->vmax_Reparameterization = upper.y;

	generaltrans->m_ReparameterizationType = generalbilinear;

	generaltrans->e00 = e00;
	generaltrans->e01 = e01;
	generaltrans->e10 = e10;
	generaltrans->e11 = e11;
	generaltrans->f00 = f00;
	generaltrans->f01 = f01;
	generaltrans->f10 = f10;
	generaltrans->f11 = f11;
	generaltrans->g00 = g00;
	generaltrans->g01 = g01;
	generaltrans->g10 = g10;
	generaltrans->g11 = g11;
	generaltrans->h00 = h00;
	generaltrans->h01 = h01;
	generaltrans->h10 = h10;
	generaltrans->h11 = h11;
	generaltrans->SetInverseEvaluationLabel(pMesh1,pMesh2);
	pNewNSurface->ptransformation = generaltrans;
	generaltrans->UpdateBoundingBox();
	pNewNSurface->c_g = 1.0;
	pNewNSurface->SetTransform(*pNurbs->GetTransform());
	pNewNSurface->filename = "optimized surface";
	pDoc->m_SceneGraph.Add(pNewNSurface);

	CMesh3d* pMesh3 = new CMesh3d();
	pMesh3->Copy(pMesh2);
	double s,t,u,v;
	for(int i = 0; i < pMesh3->NbVertex(); i++)
	{
		s = pMesh2->GetVertex(i)->x();
		t = pMesh2->GetVertex(i)->y();

		//s = (generaltrans->umax_Reparameterization - generaltrans->umin_Reparameterization) * s + generaltrans->umin_Reparameterization;
		//t = (generaltrans->vmax_Reparameterization - generaltrans->vmin_Reparameterization) * t + generaltrans->vmin_Reparameterization;

		generaltrans->Evaluation(s,t,u,v);

		pMesh3->GetVertex(i)->x(u);
		pMesh3->GetVertex(i)->y(v);
	}
	pMesh3->filename = "mesh";
	pDoc->m_SceneGraph.Add(pMesh3);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);

}
void Actions_GeneralBilinear::OnBnClickedActionsGeneralbilinearInitialvalue()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num=pDoc->m_SceneGraph.NbObject();
	if(num != 3)
		return;

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(2);
	C_NurbsSurf* pNurbsSurf = pNurbs->Converte_C_NurbsSurface();
	Point3D lower,upper;

	//Compute a bounding box
	pMesh2->ComputeBoundingBox(lower,upper);

	std::vector<std::vector<double>> optinumx,optinumy;

	//pMesh1 the 2D uniform grid
	//pMesh2 the obtained 2D conformal parameterization from 3D triangle mesh

	ComputeInitialGeneralBilinearTransformation(pMesh2,pMesh1,optinumx,0);
	ComputeInitialGeneralBilinearTransformation(pMesh2,pMesh1,optinumy,1);

	//select a valid and best approximation
	GeneralBilinearTransformation pTrans;
	pTrans.m_ReparameterizationType = generalbilinear;
	int optimalx = -1;
	int optimaly = -1;
	double minenergy = 1e10;
	double currentenergy;
	double u,v;
	double minval = 1e10;
	for(int i = 0; i < optinumx.size(); i++)
		for(int j = 0; j < optinumy.size(); j++)
		{
			pTrans.e00 = optinumx[i][0];
			pTrans.e01 = optinumx[i][1];
			pTrans.e10 = optinumx[i][2];
			pTrans.e11 = optinumx[i][3];
			pTrans.f00 = optinumx[i][4];
			pTrans.f01 = optinumx[i][5];
			pTrans.f10 = optinumx[i][6];
			pTrans.f11 = optinumx[i][7];
			pTrans.g00 = optinumy[j][0];
			pTrans.g01 = optinumy[j][1];
			pTrans.g10 = optinumy[j][2];
			pTrans.g11 = optinumy[j][3];
			pTrans.h00 = optinumy[j][4];
			pTrans.h01 = optinumy[j][5];
			pTrans.h10 = optinumy[j][6];
			pTrans.h11 = optinumy[j][7];
			
			if(!pTrans.IsValid())
				continue;
			currentenergy = 0;
			for(int ii = 0; ii < pMesh2->NbVertex(); ii++)
			{
				CVertex3d* pVertex = pMesh2->GetVertex(ii);
				CVertex3d* pVertex1 = pMesh1->GetVertex(ii);
				double s = pVertex->x();
				double t = pVertex->y();

				pTrans.Evaluation(s,t,u,v);

				double norm1 = pTrans.e00 * (1 - s) * (1 - t) + pTrans.e01 * (1 - s) * t + pTrans.e10 * s * (1 - t) + pTrans.e11 * s * t;
				double norm2 = pTrans.g00 * (1 - s) * (1 - t) + pTrans.g01 * (1 - s) * t + pTrans.g10 * s * (1 - t) + pTrans.g11 * s * t;
				double denorm1 = pTrans.f00 * (1 - s) * (1 - t) + pTrans.f01 * (1 - s) * t + pTrans.f10 * s * (1 - t) + pTrans.f11 * s * t;
				double denorm2 = pTrans.h00 * (1 - s) * (1 - t) + pTrans.h01 * (1 - s) * t + pTrans.h10 * s * (1 - t) + pTrans.h11 * s * t;

				if(fabs(denorm1) < minval)
					minval = fabs(denorm1);
				if(fabs(denorm2) < minval)
					minval = fabs(denorm2);

				if(u > 10 || v > 10 || u < -10 || v < -10)
				{
					int ij = 0;
				}
				
				currentenergy += SQR(pVertex1->x() * denorm1 - norm1) + SQR(pVertex1->y() * denorm2 - norm2);
			}
			if(currentenergy < minenergy)
			{
				minenergy = currentenergy;
				optimalx = i;
				optimaly = j;
			}
		}
		currentenergy = 0;
		for(int ii = 0; ii < pMesh2->NbVertex(); ii++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(ii);
			CVertex3d* pVertex1 = pMesh1->GetVertex(ii);
			double s = pVertex->x();
			double t = pVertex->y();
			double u = s;
			double v = t;

			currentenergy += SQR(u - pVertex1->x()) + SQR(v - pVertex1->y());
		}
	if(optimalx == -1 || optimaly == -1)
		ASSERT(0);

	//optimalx = 3;
	//optimaly = 3;
	/*C_NurbsSurf* pNewSurface_ = pNurbsSurf->GetGeneralBilinearTransformedSurface(
	optinumx[0],optinumx[1],optinumx[2],optinumx[3],optinumx[4],optinumx[5],optinumx[6],optinumx[7],
	optinumy[0],optinumy[1],optinumy[2],optinumy[3],optinumy[4],optinumy[5],optinumy[6],optinumy[7],
	lower.x,upper.x,lower.y,upper.y);*/

	CNurbsSuface* pNewNSurface = pNurbsSurf->GetCNurbsSurface();
	GeneralBilinearTransformation* generaltrans = new GeneralBilinearTransformation();
	generaltrans->umin_Reparameterization = lower.x;
	generaltrans->umax_Reparameterization = upper.x;
	generaltrans->vmin_Reparameterization = lower.y;
	generaltrans->vmax_Reparameterization = upper.y;

	generaltrans->m_ReparameterizationType = generalbilinear;
	std::vector<double> c;
	c.insert(c.end(),optinumx[optimalx].begin(),optinumx[optimalx].end());
	c.insert(c.end(),optinumy[optimaly].begin(),optinumy[optimaly].end());
	generaltrans->SetReparameterizationCoefficients(c);
	/*generaltrans->e00 = 0;
	generaltrans->e01 = 0;
	generaltrans->e10 = 1;
	generaltrans->e11 = 1;
	generaltrans->f00 = 1;
	generaltrans->f01 = 1;
	generaltrans->f10 = 1;
	generaltrans->f11 = 1;
	generaltrans->g00 = 0;
	generaltrans->g01 = 1;
	generaltrans->g10 = 0;
	generaltrans->g11 = 1;
	generaltrans->h00 = 1;
	generaltrans->h01 = 1;
	generaltrans->h10 = 1;
	generaltrans->h11 = 1;*/
	generaltrans->SetInverseEvaluationLabel(pMesh1,pMesh2);
	TRACE("%f %f %f %f\n",c[0],c[1],c[2],c[3]);
	TRACE("%f %f %f %f\n",c[4],c[5],c[6],c[7]);
	TRACE("%f %f %f %f\n",c[8],c[9],c[10],c[11]);
	TRACE("%f %f %f %f\n",c[12],c[13],c[14],c[15]);
	pNewNSurface->ptransformation = generaltrans;
	generaltrans->UpdateBoundingBox();
	pNewNSurface->c_g = 1.0;
	pNewNSurface->SetTransform(*pNurbs->GetTransform());
	pDoc->m_SceneGraph.Add(pNewNSurface);

	CMesh3d* pMesh3 = new CMesh3d();
	pMesh3->Copy(pMesh2);
	double s,t;
	for(int i = 0; i < pMesh3->NbVertex(); i++)
	{
		s = pMesh1->GetVertex(i)->x();
		t = pMesh1->GetVertex(i)->y();

		s = (generaltrans->umax_Reparameterization - generaltrans->umin_Reparameterization) * s + generaltrans->umin_Reparameterization;
		t = (generaltrans->vmax_Reparameterization - generaltrans->vmin_Reparameterization) * t + generaltrans->vmin_Reparameterization;

		generaltrans->Evaluation(s,t,u,v);

		pMesh3->GetVertex(i)->x(u);
		pMesh3->GetVertex(i)->y(v);
	}
	pDoc->m_SceneGraph.Add(pMesh3);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_GeneralBilinear::OnBnClickedIdcActionsGeneralbilinearGeneratesubmesh()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	//if(num != 2)
	//	return;
	
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);

	int samplesn = 14;
	int samplesm = 14;
	double minu = - 0.2;
	double maxu = 1.2;
	double minv = -0.2;
	double maxv = 1.2;

	int i,j;
	int vnum = 0;
	double TOL = 1e-8;
	CColor color (255,0,0);
	for(j = 0; j <= samplesn; j++)
	{
		for(i = 0; i <= samplesm; i++)
		{	//
			double s = minu + (double)i * (maxu - minu) / samplesm;
			double t = minv + (double)j * (maxv - minv) / samplesn;

			if(s < -TOL || s > 1 + TOL || t < -TOL || t > TOL + 1)
			{
				pMesh1->GetVertex(vnum)->SetFlag(1);
				pMesh2->GetVertex(vnum)->SetFlag(1);
				pMesh1->GetVertex(vnum)->SetColor(color);
				pMesh2->GetVertex(vnum)->SetColor(color);
			}
			else
			{
				pMesh1->GetVertex(vnum)->SetFlag(0);
				pMesh2->GetVertex(vnum)->SetFlag(0);
			}
			vnum++;
		}
	}
	pMesh1->SetModified(TRUE);
	pMesh2->SetModified(TRUE);
	pMesh1->m_showVertex = TRUE;
	pMesh2->m_showVertex = TRUE;
}
void Actions_GeneralBilinear::SetConformalEnergyWeight(CNurbsSuface* pSurf)
{
	//Compute the current conformal energy
	if(!pSurf->ptransformation)
		return ;
	if(pSurf->ptransformation->m_ReparameterizationType != generalbilinear)
		return ;

	GeneralBilinearTransformation* pbilineartrans = (GeneralBilinearTransformation*)pSurf->ptransformation;
	int m = 10;
	int n = 10;
	C_NurbsSurf* pSurface = pSurf->Converte_C_NurbsSurface();
	//double orthogoanlityweight = m_orthogonalityweight * 0.01;
	//sample the points in the u,v space
	double s,t,u,v;
	double totalenergy = 0;
	double totalorthogonality = 0;
	double totaluniformity = 0;
	for(int i = 0; i <= m; i++)
		for(int j = 0; j <= n; j++)
		{
			u = i * 1.0 / m;
			v = j * 1.0 / n;

			u = u *(pbilineartrans->umax - pbilineartrans->umin) + pbilineartrans->umin;
			v = v * (pbilineartrans->vmax - pbilineartrans->vmin) + pbilineartrans->vmin;

			pbilineartrans->InverseEvaluation(u,v,s,t);
			Vector3D Xu = pSurface->ComputUDerivative1(u,v);
			Vector3D Xv = pSurface->ComputVDerivative1(u,v);
			double us = pbilineartrans->Comput_u_s(s,t);
			double vs = pbilineartrans->Comput_v_s(s,t);
			double ut = pbilineartrans->Comput_u_t(s,t);
			double vt = pbilineartrans->Comput_v_t(s,t);

			Vector3D Xs = Xu * us + Xv * vs;
			Vector3D Xt = Xu * ut + Xv * vt;

			double energyorthogonality = SQR(Xs * Xt);
			double energyuniformity = SQR(Xs * Xs - Xt * Xt);
			totalorthogonality += energyorthogonality;
			totaluniformity += energyuniformity;
			//double energy = energyorthogonality * orthogoanlityweight + energyuniformity * (1 - orthogoanlityweight);
			//totalenergy += energy;
		}
		//delete pSurface;
		//#ifdef _DEBUG
		//TRACE("Energy: %f", totalenergy);
		//#endif
		totalenergy = totalorthogonality + totaluniformity;
		m_orthogonalityweight = 100 - totalorthogonality * 100 / totalenergy;
}
double Actions_GeneralBilinear::ComputeConformalEnergy(CNurbsSuface* pSurf)
{	
	//Compute the current conformal energy
	if(!pSurf->ptransformation)
		return 0;
	double totalenergy;
	if(pSurf->ptransformation->m_ReparameterizationType == generalbilinear)
	{
		GeneralBilinearTransformation* pbilineartrans = (GeneralBilinearTransformation*)pSurf->ptransformation;
		int m = 10;
		int n = 10;
		double umin = pbilineartrans->umin;
		double umax = pbilineartrans->umax;
		double vmin = pbilineartrans->vmin;
		double vmax = pbilineartrans->vmax;

		C_NurbsSurf* pSurface = pSurf->Converte_C_NurbsSurface();
		double orthogoanlityweight = m_orthogonalityweight * 0.01;
		//sample the points in the u,v space
		double s,t,u,v;
		totalenergy = 0;
		for(int i = 0; i <= m; i++)
			for(int j = 0; j <= n; j++)
			{
				u = umin + i * (umax - umin) / m;
				v = vmin + j * (vmax - vmin) / n;

				pbilineartrans->InverseEvaluation(u,v,s,t);
				Vector3D Xu = pSurface->ComputUDerivative1(u,v);
				Vector3D Xv = pSurface->ComputVDerivative1(u,v);
				Vector3D N = Xu ^ Xv;
				N.SetUnit();

				double us = pbilineartrans->Comput_u_s(s,t);
				double vs = pbilineartrans->Comput_v_s(s,t);
				double ut = pbilineartrans->Comput_u_t(s,t);
				double vt = pbilineartrans->Comput_v_t(s,t);

				Vector3D Xs = Xu * us + Xv * vs;
				Vector3D Xt = Xu * ut + Xv * vt;

				Vector3D newvec = N ^ Xs;
				double val = (newvec - Xt) * (newvec - Xt);

				//double energyorthogonality = SQR(Xs * Xt);
				//double energyuniformity = SQR(Xs * Xs - Xt * Xt);
				//double energy = energyorthogonality * orthogoanlityweight + energyuniformity * (1 - orthogoanlityweight);
				totalenergy += val;
			}
			//delete pSurface;
			//#ifdef _DEBUG
			//TRACE("Energy: %f", totalenergy);
			//#endif
	}
	return totalenergy;
}
void Actions_GeneralBilinear::OnBnClickedActionsGeneralbilinearOptimization()
{
	// TODO: Add your control notification handler code here
	//using linear search methods
	//along each direction 
	//UpdateData(TRUE);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);

	if(pDoc->m_SceneGraph.NbObject() == 0)
		return;

	CTreeCtrl* pTreeControl = (CTreeCtrl*)p_FormView3->objTab.GetDlgItem(IDC_Objects);

	vector<HTREEITEM>::iterator treeitem;
	treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),pTreeControl->GetSelectedItem());

	//int index = treeitem - p_FormView3->m_ArraySurfaceItems.begin();
	//if(index < 0 || index >= pDoc->m_SceneGraph.NbObject())
	//	index = 0;
	int index = 3;

	if(pDoc->m_SceneGraph.GetAt(index)->GetType() != TYPE_NURBSSURFACE)
		return;

	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(index);
	SetConformalEnergyWeight(pSurf);
	UpdateData(FALSE);
	CNurbsSuface* pSurf1 = pSurf->copy();
	pDoc->m_SceneGraph.Add(pSurf1);
	//first fix two terms optimalxindex,optimalyindex to 1.0
	char message[100];
	sprintf(message,"Optimization Result %d",m_orthogonalityweight);
	pSurf1->name = message;
	//search e00.
	int iternum = 1000;
	double val1 = ComputeConformalEnergy(pSurf1);
	TRACE("\n Original Energy: %f \n",val1);
	double preval,newval;
	for(int ii = 0; ii < iternum; ii++)
	{
		preval = ComputeConformalEnergy(pSurf1);
		for(int i = 0; i < 16; i++)
		{
			if(i == optimalxindex + 4)
				continue;
			if(i == optimalyindex + 12)
				continue;
			double energy1 = ComputeConformalEnergy(pSurf1);
			int sign = LinearSearch(i,pSurf1);
			double energy2 = ComputeConformalEnergy(pSurf1);
			if(energy2 > energy1)
				ASSERT(0);
			if(sign == -1)
			{
				//error
				CNurbsSuface* pNSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
				CMesh3d* pMesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
				CMesh3d* pNewMesh3d = new CMesh3d();
				pNewMesh3d->Copy(pMesh3d);
				for(int i = 0; i < pNewMesh3d->NbVertex(); i++)
				{
					CVertex3d* pVertex = pNewMesh3d->GetVertex(i);
					GeneralBilinearTransformation *pTran = (GeneralBilinearTransformation*)pNSurf->ptransformation;
					double u,v;
					double s,t;
					s = pVertex->x();
					t = pVertex->y();
					s = -5 + 10 * s;
					t = -5 + 10 * t;
					s = (pTran->umax_Reparameterization - pTran->umin_Reparameterization) * s + pTran->umin_Reparameterization;
					t = (pTran->vmax_Reparameterization - pTran->vmin_Reparameterization) * t + pTran->vmin_Reparameterization;
					pTran->Evaluation(s,t,u,v);
					pVertex->x(u);
					pVertex->y(v);
				}
				pDoc->m_SceneGraph.Add(pNewMesh3d);
				p_FormView3->UpdateTreeControl();
				pDoc->UpdateAllViews(NULL);
				return;
			}
			
			//TRACE("New Energy: %f \n",val2);
			//return;
			//CNurbsSuface* pNewSurface = pSurf1->copy();
			//char message[20];
			//sprintf(message,"%d,%f",ii,val2);
			//pNewSurface->name = message;
			//pDoc->m_SceneGraph.Add(pNewSurface);
		}
		newval = ComputeConformalEnergy(pSurf1);
		TRACE("New Energy: %f \n",newval);
		if(fabs(newval - preval) < 1e-5)
			break;
	}
	/*((GeneralBilinearTransformation*)pSurf1->ptransformation)->label = -1;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->e00 = 0.55503119001345347;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->e01 = 0.17211707887510930;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->e10 = 1.3496867591384420;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->e11 = 1.1715772388223715;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->f00 = 1.0000000000000000;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->f01 = 0.96838906670380287;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->f10 = 0.76963309569731031;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->f11 = 0.37143491679153418;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->g00 = 0.39208716260504184;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->g01 = 0.86639091505216737;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->g10 = 1.3288590378464826;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->g11 = 2.3759976458765095;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->h00 = 1.0000000000000000;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->h01 = 0.82179701865316579;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->h10 = 0.95273554132737548;
	((GeneralBilinearTransformation*)pSurf1->ptransformation)->h11 = 1.1132974194920648;*/
	CMesh3d* pMesh3 = new CMesh3d();
	pMesh3->Copy(pMesh2);
	double s,t,u,v;
	for(int i = 0; i < pMesh3->NbVertex(); i++)
	{
		s = pMesh2->GetVertex(i)->x();
		t = pMesh2->GetVertex(i)->y();

		//s = (generaltrans->umax_Reparameterization - generaltrans->umin_Reparameterization) * s + generaltrans->umin_Reparameterization;
		//t = (generaltrans->vmax_Reparameterization - generaltrans->vmin_Reparameterization) * t + generaltrans->vmin_Reparameterization;

		pSurf1->ptransformation->Evaluation(s,t,u,v);

		pMesh3->GetVertex(i)->x(u);
		pMesh3->GetVertex(i)->y(v);
	}
	pDoc->m_SceneGraph.Add(pMesh3);

	((GeneralBilinearTransformation*)pSurf1->ptransformation)->UpdateBoundingBox();
	p_FormView3->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
int Actions_GeneralBilinear::LinearSearch_GradientSign(int index, CNurbsSuface* pSurf)
{
	double currentenergy = ComputeConformalEnergy(pSurf);
	GeneralBilinearTransformation* pTrans = (GeneralBilinearTransformation*)pSurf->ptransformation;

	double step = 1e-3;
	double originalval = pTrans->GetCoefficient(index);
	pTrans->SetCoefficient(index,originalval + step);
	double nextenergy,preenergy;
	if(pTrans->IsValid())
		nextenergy = ComputeConformalEnergy(pSurf);
	else
		nextenergy = 1e10;
	pTrans->SetCoefficient(index,originalval - step);
	if(pTrans->IsValid())
		preenergy = ComputeConformalEnergy(pSurf);
	else
		preenergy = 1e10;
	pTrans->SetCoefficient(index,originalval);
	if(currentenergy <= nextenergy && currentenergy <= preenergy)
	{
		return 0;

	}else
	if(currentenergy <= preenergy && currentenergy >= nextenergy)
	{
		return -1;
	}else
	if(currentenergy >= preenergy && currentenergy <= nextenergy)
	{
		return 1;
	}else
	{
		return 1;
	}
}
int Actions_GeneralBilinear::LinearSearch(int index,CNurbsSuface* pSurf)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	GeneralBilinearTransformation* pTrans = (GeneralBilinearTransformation*)pSurf->ptransformation;
	double oldvalue = pTrans->GetCoefficient(index);
	double currentenergy = ComputeConformalEnergy(pSurf);
	//TRACE("Value %f,Energy: %f\n",pTrans->GetCoefficient(index),currentenergy);
	int sign = LinearSearch_GradientSign(index,pSurf);
	double preval; 
	double nextval;
	double step = 0.1;
	if(sign == 0)
	{
		//get the local minimum
		return 1;
	}else if (sign == -1)
	{
		//the local minimum is the on the right
		preval = pTrans->GetCoefficient(index);
		double multiplier = 0.5;
		while(sign == -1)
		{
			multiplier = multiplier + 1;
			
			pTrans->SetCoefficient(index,preval + step * multiplier);
			/*if(index == 8)
			{
				CNurbsSuface* pNewSurface = pSurf->copy();
				char message[20];
				sprintf(message,"%f",step * multiplier);
				pNewSurface->name = message;
				pDoc->m_SceneGraph.Add(pNewSurface);
				
			}*/
			if(!pTrans->IsValid())
			{
				multiplier = multiplier / 2;
				CNurbsSuface* pNewSurface = pSurf->copy();
				pTrans->SetCoefficient(index,preval + step * multiplier);
				while(!pTrans->IsValid())
				{
					multiplier = multiplier / 2;
					if(multiplier < 0.1)
						multiplier = 0;
					pTrans->SetCoefficient(index,preval + step * multiplier);
				}
				double newenergy = ComputeConformalEnergy(pSurf);
				if(newenergy > currentenergy)
				{
					pTrans->SetCoefficient(index,oldvalue);
				}
				return 2;
				//((GeneralBilinearTransformation*)pNewSurface->ptransformation)->UpdateBoundingBox();
				//((GeneralBilinearTransformation*)pNewSurface->ptransformation)->Trace();
				//pDoc->m_SceneGraph.Add(pNewSurface);
				
				//pTrans->SetCoefficient(index,preval + step * multiplier);
				//pDoc->m_SceneGraph.Add(pSurf->copy());
				//pDoc->UpdateTreeControl();
				//pDoc->UpdateAllViews(NULL);
				//return -1;
				//break;
			}
			double currentenergy1 = ComputeConformalEnergy(pSurf);
			//TRACE("Sign: -1,Value %f,Energy: %f\n",pTrans->GetCoefficient(index),currentenergy1);
			sign = LinearSearch_GradientSign(index,pSurf);
		}
		if(fabs(multiplier - 1) < 1e-1)
		{
			nextval = preval + step * multiplier;
		}else
		{
			nextval = preval + step * multiplier;
			preval = preval + step * multiplier / 2.0;
		}

	}else if (sign == 1)
	{
		//the local minimum is the on the left
		nextval = pTrans->GetCoefficient(index);
		double multiplier = 0.5;
		while(sign == 1)
		{
			multiplier = multiplier + 1;
			pTrans->SetCoefficient(index,nextval - step * multiplier);
			if(!pTrans->IsValid())
			{
				multiplier = multiplier / 2;
				pTrans->SetCoefficient(index,nextval - step * multiplier);
				while(!pTrans->IsValid())
				{
					multiplier = multiplier / 2;
					if(multiplier < 0.1)
						multiplier = 0;
					pTrans->SetCoefficient(index,nextval - step * multiplier);
				}
				double newenergy = ComputeConformalEnergy(pSurf);
				if(newenergy > currentenergy)
				{
					pTrans->SetCoefficient(index,oldvalue);
				}
				return 2;
			}
			double currentenergy1 = ComputeConformalEnergy(pSurf);
			//TRACE("Sign: 1, Value: %f, Energy: %f\n",pTrans->GetCoefficient(index),currentenergy1);
			sign = LinearSearch_GradientSign(index,pSurf);
		}
		if(fabs(multiplier - 1) < 1e-1)
		{
			preval = nextval - step * multiplier;
		}else
		{
			preval = nextval - step * multiplier;
			nextval = nextval - step * multiplier / 2.0;
		}
	}else if(sign == -2)
	{
		//the current coefficient is near the singular point
		int ii = pTrans->IsValid();
		return 1;
	}
	pTrans->SetCoefficient(index,(preval + nextval) / 2);
	sign = LinearSearch_GradientSign(index,pSurf);
	while(sign != 0)
	{
		if(sign == -1)
		{
			preval = pTrans->GetCoefficient(index);
		}else
		{
			nextval = pTrans->GetCoefficient(index);
		}
		pTrans->SetCoefficient(index,(preval + nextval) / 2);
		if(!pTrans->IsValid())
		{
			int ij = 0;
		}
		double currentenergy1 = ComputeConformalEnergy(pSurf);
		//TRACE("Value: %f, Energy: %f\n",pTrans->GetCoefficient(index),currentenergy1);
		if(fabs(preval - nextval) < 1e-5)
			break;
		sign = LinearSearch_GradientSign(index,pSurf);
	}
	double newenergy = ComputeConformalEnergy(pSurf);
	if(newenergy > currentenergy)
	{
		pTrans->SetCoefficient(index,oldvalue);
	}
	return 1;
}

void Actions_GeneralBilinear::OutPutMeshFaces_PovRay_Terrain_Original(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	CString s1;
	int i,j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	//samplesm = samplesm * 10;
	outfile << "mesh {" << endl;
	for(j = 0; j < samplesn; j++)
	{
		for(i = 0; i < samplesm; i++)
		{	//
			double s1 = (double)i / samplesm;
			double s2 = (double)(i + 1) / samplesm;
			double t1 = (double)j / samplesn;
			double t2 = (double)(j + 1) / samplesn;

			double u1,v1,u2,v2,u3,v3,u4,v4;
			if(!pSurface->ptransformation)
			{
				double umin = pNurbsSurface->GetUKnot(0);
				double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

				double vmin = pNurbsSurface->GetVKnot(0);
				double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

				s1 = umin + (umax - umin) * s1;
				s2 = umin + (umax - umin) * s2;
				t1 = vmin + (vmax - vmin) * t1;
				t2 = vmin + (vmax - vmin) * t2;

				u1 = s1;
				v1 = t1;

				u2 = s2;
				v2 = t2;

				Vector3D vv1 = (pNurbsSurface->ComputPosition(u1,v1) - ZeroP3D);
				Vector3D v1_ = pNurbsSurface->ComputUDerivative1(u1,v1);
				Vector3D v2_ = pNurbsSurface->ComputVDerivative1(u1,v1);
				Vector3D n1 = v1_ ^ v2_;
				n1.SetUnit();

				Vector3D vv2 = (pNurbsSurface->ComputPosition(u2,v1) - ZeroP3D);
				v1_ = pNurbsSurface->ComputUDerivative1(u2,v1);
				v2_ = pNurbsSurface->ComputVDerivative1(u2,v1);
				Vector3D n2 = v1_ ^ v2_;
				n2.SetUnit();

				Vector3D vv3 = (pNurbsSurface->ComputPosition(u2,v2) - ZeroP3D);
				v1_ = pNurbsSurface->ComputUDerivative1(u2,v2);
				v2_ = pNurbsSurface->ComputVDerivative1(u2,v2);
				Vector3D n3 = v1_ ^ v2_;
				n3.SetUnit();

				Vector3D vv4 = (pNurbsSurface->ComputPosition(u1,v2) - ZeroP3D);
				v1_ = pNurbsSurface->ComputUDerivative1(u1,v2);
				v2_ = pNurbsSurface->ComputVDerivative1(u1,v2);
				Vector3D n4 = v1_ ^ v2_;
				n4.SetUnit();

				//n1 = n1 * (-1);
				//n2 = n2 * (-1);
				//n3 = n3 * (-1);
				//n4 = n4 * (-1);

				outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
				outfile << vv2[0] << "," << vv2[1] << "," << -vv2[2] << ">,<" << n2[0] << "," << n2[1] << "," << -n2[2] << ">,<";
				outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n3[0] << "," << n3[1] << "," << -n3[2] << ">}"<<endl;

				outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
				outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n3[0] << "," << n3[1] << "," << -n3[2] << ">,<";
				outfile << vv4[0] << "," << vv4[1] << "," << -vv4[2] << ">,<" << n4[0] << "," << n4[1] << "," << -n4[2] << ">}"<<endl;
			}else
			{
				s1 = -0.855068 + (-0.0486115 + 0.855068) * s1;
				t1 = -1.24888 + (-0.201405 + 1.24888) * t1;
				s2 = -0.855068 + (-0.0486115 + 0.855068) * s2;
				t2 = -1.24888 + (-0.201405 + 1.24888) * t2;


				//if(s1 < -0.855068 || s1 > -0.0486115)
				//	continue;
				//if(t1 < -1.24888 || t1 > -0.201405)
				//	continue;
				//if(s2 < -0.855068 || s2 > -0.0486115)
				//	continue;
				//if(t2 < -1.24888 || t2 > -0.201405)
				//	continue;

				pSurface->ptransformation->Evaluation(s1,t1,u1,v1);
				pSurface->ptransformation->Evaluation(s2,t1,u2,v2);
				pSurface->ptransformation->Evaluation(s2,t2,u3,v3);
				pSurface->ptransformation->Evaluation(s1,t2,u4,v4);

				Vector3D vv1 = (pNurbsSurface->ComputPosition(u1,v1) - ZeroP3D);
				Vector3D v1_ = pNurbsSurface->ComputUDerivative1(u1,v1);
				Vector3D v2_ = pNurbsSurface->ComputVDerivative1(u1,v1);
				Vector3D n1 = v1_ ^ v2_;
				n1.SetUnit();

				Vector3D vv2 = (pNurbsSurface->ComputPosition(u2,v2) - ZeroP3D);
				v1_ = pNurbsSurface->ComputUDerivative1(u2,v2);
				v2_ = pNurbsSurface->ComputVDerivative1(u2,v2);
				Vector3D n2 = v1_ ^ v2_;
				n2.SetUnit();

				Vector3D vv3 = (pNurbsSurface->ComputPosition(u3,v3) - ZeroP3D);
				v1_ = pNurbsSurface->ComputUDerivative1(u3,v3);
				v2_ = pNurbsSurface->ComputVDerivative1(u3,v3);
				Vector3D n3 = v1_ ^ v2_;
				n3.SetUnit();

				Vector3D vv4 = (pNurbsSurface->ComputPosition(u4,v4) - ZeroP3D);
				v1_ = pNurbsSurface->ComputUDerivative1(u4,v4);
				v2_ = pNurbsSurface->ComputVDerivative1(u4,v4);
				Vector3D n4 = v1_ ^ v2_;
				n4.SetUnit();

				//n1 = n1 * (-1);
				//n2 = n2 * (-1);
				//n3 = n3 * (-1);
				//n4 = n4 * (-1);

				outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
				outfile << vv2[0] << "," << vv2[1] << "," << -vv2[2] << ">,<" << n2[0] << "," << n2[1] << "," << -n2[2] << ">,<";
				outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n3[0] << "," << n3[1] << "," << -n3[2] << ">}"<<endl;

				outfile << "smooth_triangle { < " << vv1[0] << "," << vv1[1] << " , " << -vv1[2] << " >,<" << n1[0] << "," << n1[1] << "," << -n1[2] << ">,<";
				outfile << vv3[0] << "," << vv3[1] << "," << -vv3[2] << ">,<" << n3[0] << "," << n3[1] << "," << -n3[2] << ">,<";
				outfile << vv4[0] << "," << vv4[1] << "," << -vv4[2] << ">,<" << n4[0] << "," << n4[1] << "," << -n4[2] << ">}"<<endl;
			}


		}
	}
	outfile << "texture {" << endl;
	outfile << "samplesmesh_tex" << endl;
	outfile << "}" << endl;
	outfile << "}" << endl;
	outfile.close();
}

void Actions_GeneralBilinear::OutPutConformity_PovRay_result(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename)
{
	C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();
	QuadraticTransformation* pTrans = (QuadraticTransformation*)pSurface->ptransformation;

	std::ofstream outfile(filename);

	int i,j;
	double u,v,s,t;
	std::vector<int> m_ArrayLabels;
	int label;
	outfile << "vertex_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	for(i = 0; i <= samplesm; i++)
		for(j = 0; j <= samplesn; j++)
		{
			s = ((double)i) / samplesm;
			t = ((double)j) / samplesn;

			label = 1;

			//Transformation *pTrans = (Transformation*)pSurface->ptransformation;
			s = pTrans->umin + (pTrans->umax - pTrans->umin) * s;
			t = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t;

			double u,v;
			double s_1,t_1;
			pTrans->InverseEvaluation(s,t,s_1,t_1);

			if(s_1 < -0.855068 || s_1 > -0.0486115)
			{
				label = 0;
				//continue;
			}
			if(t_1 < -1.24888 || t_1 > -0.201405)
			{
				label = 0;
				//continue;
			}

			u = s;
			v = t;

			m_ArrayLabels.push_back(label);
			if(label)
			{
				Point3D p1 = pNurbsSurface1->ComputPosition(u,v);
				outfile << "<" << p1[0] <<" , " << p1[1] << " , "<< -p1[2] << " >, " <<endl;
			}else
			{
				outfile << "<" << 0 <<" , " << 0 << " , "<< 0 << " >, " <<endl;
			}
		}
		outfile << " }" << endl;

		int ii = -1;
		outfile << "uv_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		double maxangle_real = -1e10;
		for(i = 0; i <= samplesm; i++)
			for(j = 0; j <= samplesn; j++)
			{
				double s = ((double)i) / samplesm;
				double t = ((double)j) / samplesn;

				ii++;
				if(!m_ArrayLabels[ii])
				{
					outfile << " < 0 " << " , " << 0  << ">, " <<endl;
				}else
				{
					s = pTrans->umin + (pTrans->umax - pTrans->umin) * s;
					t = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t;

					u = s;
					v = t;
					
					Vector3D Xu = pNurbsSurface1->ComputUDerivative1(u,v);
					Vector3D Xv = pNurbsSurface1->ComputVDerivative1(u,v);
					Vector3D N = Xu ^ Xv;
					N.SetUnit();

					double val;

					Vector3D newvec = N ^ Xu;
					val = (newvec - Xv) * (newvec - Xv);

					//newvec.SetUnit();
					//Xt.SetUnit();
					//double cosval = newvec * Xt;
					//if(cosval < 0 )
					//{
					//	int ij = 0;
					//}
					//cartop 0.17209753561068819 * 2
					//198249.64033109313 * 1.2
					//198249.64033109313 * 0.1
					val = val / (4831942.3566562058 * 3);
					if(val >= 1)
						val = 0.99;
					if(val > maxangle_real)
						maxangle_real = val;

					outfile << " < 0 " << " , " << val  << ">, " <<endl;
				}


			}
			outfile << " }" << endl;

			//normal
			ii = -1;
			outfile << "normal_vectors {" << endl;
			outfile << (samplesm + 1) * (samplesn + 1) << endl;
			for(i = 0; i <= samplesm; i++)
				for(j = 0; j <= samplesn; j++)
				{
					double s = ((double)i) / samplesm;
					double t = ((double)j) / samplesn;

					ii++;
					if(!m_ArrayLabels[ii])
					{
						outfile << " < " << 0 << " , " <<0 << " , " << 0 << " >,"<<endl;
					}
					else
					{
						s = pTrans->umin + (pTrans->umax - pTrans->umin) * s;
						t = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t;
						u = s;
						v = t;

						Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u,v);
						Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u,v);


						Vector3D Xu = pNurbsSurface1->ComputUDerivative1(u,v);
						Vector3D Xv = pNurbsSurface1->ComputVDerivative1(u,v);
						Vector3D n = Xu ^ Xv;
						n.SetUnit();

						outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >,"<<endl;
					}

				}
				outfile << " }" << endl;

				ii = 0;
				//face_indices {
				for(j = 0; j < samplesn; j++)
					for(i = 0; i < samplesm; i++)
					{
						double u = ((double)i) / samplesm;
						double v = ((double)j) / samplesn;

						int n1 = j * (samplesm + 1) + i;
						int n2 = j * (samplesm + 1) + i + 1;
						int n3 = (j + 1)* (samplesm + 1) + i;
						int n4 = (j + 1)* (samplesm + 1) + i + 1;

						if(m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
						{
							ii++;
						}
						if(m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
						{
							ii++;
						}
					}

					//face_indices {
					outfile << "face_indices {" << endl;
					outfile << ii << endl;
					for(j = 0; j < samplesn; j++)
						for(i = 0; i < samplesm; i++)
						{
							double u = ((double)i) / samplesm;
							double v = ((double)j) / samplesn;

							int n1 = j * (samplesm + 1) + i;
							int n2 = j * (samplesm + 1) + i + 1;
							int n3 = (j + 1)* (samplesm + 1) + i;
							int n4 = (j + 1)* (samplesm + 1) + i + 1;

							if(m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
							{
								outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >,"<<endl;
							}
							if(m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
							{
								outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >,"<<endl;
							}
						}
						outfile << " }" << endl;

						outfile.close();
}

void Actions_GeneralBilinear::OutPutConformity_PovRay_original(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename)
{
	C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();
	QuadraticTransformation* pTrans = (QuadraticTransformation*)pSurface->ptransformation;

	std::ofstream outfile(filename);

	int i,j;
	double u,v,s,t;
	std::vector<int> m_ArrayLabels;
	int label;
	outfile << "vertex_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	for(i = 0; i <= samplesm; i++)
		for(j = 0; j <= samplesn; j++)
		{
			s = ((double)i) / samplesm;
			t = ((double)j) / samplesn;

			label = 1;

			s = -0.855068 + (-0.0486115 + 0.855068) * s;
			t = -1.24888 + (-0.201405 + 1.24888) * t;

			//s = pTrans->umin + (pTrans->umax - pTrans->umin) * s;
			//t = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t;

			double u,v;
			double s_1,t_1;
			pTrans->Evaluation(s,t,u,v);

			m_ArrayLabels.push_back(label);
			if(label)
			{
				Point3D p1 = pNurbsSurface1->ComputPosition(u,v);
				outfile << "<" << p1[0] <<" , " << p1[1] << " , "<< -p1[2] << " >, " <<endl;
			}else
			{
				outfile << "<" << 0 <<" , " << 0 << " , "<< 0 << " >, " <<endl;
			}
		}
		outfile << " }" << endl;

		int ii = -1;
		outfile << "uv_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		double maxangle_real = -1e10;
		for(i = 0; i <= samplesm; i++)
			for(j = 0; j <= samplesn; j++)
			{
				double s = ((double)i) / samplesm;
				double t = ((double)j) / samplesn;

				ii++;
				if(!m_ArrayLabels[ii])
				{
					outfile << " < 0 " << " , " << 0  << ">, " <<endl;
				}else
				{
					s = -0.855068 + (-0.0486115 + 0.855068) * s;
					t = -1.24888 + (-0.201405 + 1.24888) * t;

					pTrans->Evaluation(s,t,u,v);

					Vector3D Xu = pNurbsSurface1->ComputUDerivative1(u,v);
					Vector3D Xv = pNurbsSurface1->ComputVDerivative1(u,v);
					Vector3D N = Xu ^ Xv;
					N.SetUnit();

					double val;

					double u_s = pTrans->Comput_u_s(s,t);
					double u_t = pTrans->Comput_u_t(s,t);
					double v_s = pTrans->Comput_v_s(s,t);
					double v_t = pTrans->Comput_v_t(s,t);

					Vector3D Xs = Xu * u_s + Xv * v_s;
					Vector3D Xt = Xu * u_t + Xv * v_t;

					Vector3D newvec = N ^ Xs;
					val = (newvec - Xt) * (newvec - Xt);

					//newvec.SetUnit();
					//Xt.SetUnit();
					//double cosval = newvec * Xt;
					//if(cosval < 0 )
					//{
					//	int ij = 0;
					//}
					//cartop 0.17209753561068819 * 2
					//198249.64033109313 * 1.2
					//198249.64033109313 * 0.1
					val = val / (4831942.3566562058 * 5);
					if(val >= 1)
						val = 0.99;
					if(val > maxangle_real)
						maxangle_real = val;

					outfile << " < 0 " << " , " << val  << ">, " <<endl;
				}


			}
			outfile << " }" << endl;

			//normal
			ii = -1;
			outfile << "normal_vectors {" << endl;
			outfile << (samplesm + 1) * (samplesn + 1) << endl;
			for(i = 0; i <= samplesm; i++)
				for(j = 0; j <= samplesn; j++)
				{
					double s = ((double)i) / samplesm;
					double t = ((double)j) / samplesn;

					ii++;
					if(!m_ArrayLabels[ii])
					{
						outfile << " < " << 0 << " , " <<0 << " , " << 0 << " >,"<<endl;
					}
					else
					{
						s = -0.855068 + (-0.0486115 + 0.855068) * s;
						t = -1.24888 + (-0.201405 + 1.24888) * t;

						pTrans->Evaluation(s,t,u,v);

						Vector3D Xu = pNurbsSurface1->ComputUDerivative1(u,v);
						Vector3D Xv = pNurbsSurface1->ComputVDerivative1(u,v);
						Vector3D n = Xu ^ Xv;
						n.SetUnit();

						outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >,"<<endl;
					}

				}
				outfile << " }" << endl;

				ii = 0;
				//face_indices {
				for(j = 0; j < samplesn; j++)
					for(i = 0; i < samplesm; i++)
					{
						double u = ((double)i) / samplesm;
						double v = ((double)j) / samplesn;

						int n1 = j * (samplesm + 1) + i;
						int n2 = j * (samplesm + 1) + i + 1;
						int n3 = (j + 1)* (samplesm + 1) + i;
						int n4 = (j + 1)* (samplesm + 1) + i + 1;

						if(m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
						{
							ii++;
						}
						if(m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
						{
							ii++;
						}
					}

					//face_indices {
					outfile << "face_indices {" << endl;
					outfile << ii << endl;
					for(j = 0; j < samplesn; j++)
						for(i = 0; i < samplesm; i++)
						{
							double u = ((double)i) / samplesm;
							double v = ((double)j) / samplesn;

							int n1 = j * (samplesm + 1) + i;
							int n2 = j * (samplesm + 1) + i + 1;
							int n3 = (j + 1)* (samplesm + 1) + i;
							int n4 = (j + 1)* (samplesm + 1) + i + 1;

							if(m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
							{
								outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >,"<<endl;
							}
							if(m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
							{
								outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >,"<<endl;
							}
						}
						outfile << " }" << endl;

						outfile.close();
}

void Actions_GeneralBilinear::OutPutTexture_PovRay_Terrain_Result(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename)
{
	C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();
	Transformation* pTrans = pSurface->ptransformation;
	//C_NurbsSurf* pNurbsSurface1 = ComputeQuadraticSuraceFromAnyDegreeSurface(pNurbsSurface,a1,a2,b1,b2);
	std::ofstream outfile(filename);
	char c1=13;
	char c2=10;
	//
	//samplesm = 300;
	//samplesn = 300;
	double minangle = 0;
	double maxangle = 30;
	int i,j;
	outfile << "vertex_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	std::vector<int> m_ArrayLabels;
	int ii = 0;
	int label;
	for(i = 0; i <= samplesm; i++)
		for(j = 0; j <= samplesn; j++)
		{
			double s = ((double)i) / samplesm;
			double t = ((double)j) / samplesn;

			label = 1;

			s = pTrans->umin + (pTrans->umax - pTrans->umin) * s;
			t = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t;

			double s_1,t_1,s_2,t_2;
			pTrans->InverseEvaluation(s,t,s_1,t_1);

			if(s_1 < -0.855068 || s_1 > -0.0486115)
			{
				label = 0;
				//continue;
			}
			if(t_1 < -1.24888 || t_1 > -0.201405)
			{
				label = 0;
				//continue;
			}

			//s = -0.855068 + (-0.0486115 + 0.855068) * s;
			//t = -1.24888 + (-0.201405 + 1.24888) * t;

			double u,v;
			u = s;
			v = t;

			m_ArrayLabels.push_back(label);
			if(label)
			{
				Point3D p1 = pNurbsSurface1->ComputPosition(u,v);
				outfile << "<" << p1[0] <<" , " << p1[1] << " , "<< -p1[2] << " >, " <<endl;
			}else
			{
				outfile << "<" << 0 <<" , " <<0 << " , "<< 0 << " >, " <<endl;
			}

		}
		outfile << " }" << endl;

		ii = 0;
		outfile << "uv_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		double maxangle_real = -1e10;
		for(i = 0; i <= samplesm; i++)
			for(j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				if(m_ArrayLabels[ii])
				{
					outfile << " < " << u << " , " << v << ">, " <<endl;
				}
				else
				{
					outfile << " < " << 0 << " , " << 0 << ">, " <<endl;
				}
				ii++;

			}
			outfile << " }" << endl;

			ii = 0;
			//normal
			outfile << "normal_vectors {" << endl;
			outfile << (samplesm + 1) * (samplesn + 1) << endl;
			for(i = 0; i <= samplesm; i++)
				for(j = 0; j <= samplesn; j++)
				{
					if(m_ArrayLabels[ii])
					{
						double s = ((double)i) / samplesm;
						double t = ((double)j) / samplesn;
						double u,v;


						s = pTrans->umin + (pTrans->umax - pTrans->umin) * s;
						t = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t;
						
						u = s;
						v = t;

						Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u,v);
						Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u,v);
						v1.SetUnit();
						v2.SetUnit();

						Vector3D n = v1 ^ v2;
						outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >,"<<endl;
					}else
					{
						outfile << " < " << 0 << " , " << 0 << " , " << 0 << " >,"<<endl;
					}
					ii++;

				}
				outfile << " }" << endl;

				ii = 0;
				//face_indices {
				for(j = 0; j < samplesn; j++)
					for(i = 0; i < samplesm; i++)
					{
						double u = ((double)i) / samplesm;
						double v = ((double)j) / samplesn;

						int n1 = j * (samplesm + 1) + i;
						int n2 = j * (samplesm + 1) + i + 1;
						int n3 = (j + 1)* (samplesm + 1) + i;
						int n4 = (j + 1)* (samplesm + 1) + i + 1;

						if(m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
						{
							ii++;
						}
						if(m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
						{
							ii++;
						}
					}

					outfile << "face_indices {" << endl;
					outfile << ii << endl;
					for(j = 0; j < samplesn; j++)
						for(i = 0; i < samplesm; i++)
						{
							double u = ((double)i) / samplesm;
							double v = ((double)j) / samplesn;

							int n1 = j * (samplesm + 1) + i;
							int n2 = j * (samplesm + 1) + i + 1;
							int n3 = (j + 1)* (samplesm + 1) + i;
							int n4 = (j + 1)* (samplesm + 1) + i + 1;

							if(m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
							{
								outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >,"<<endl;
								ii++;
							}
							if(m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
							{
								outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >,"<<endl;
								ii++;
							}
						}
						outfile << " }" << endl;

						outfile.close();
}
void Actions_GeneralBilinear::OutPutTexture_PovRay_Terrain_Original(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename)
{
	C_NurbsSurf* pNurbsSurface1 = pSurface->Converte_C_NurbsSurface();
	Transformation* pTrans = pSurface->ptransformation;
	//C_NurbsSurf* pNurbsSurface1 = ComputeQuadraticSuraceFromAnyDegreeSurface(pNurbsSurface,a1,a2,b1,b2);
	std::ofstream outfile(filename);
	char c1=13;
	char c2=10;
	//
	//samplesm = 300;
	//samplesn = 300;
	double minangle = 0;
	double maxangle = 30;
	int i,j;
	outfile << "vertex_vectors {" << endl;
	outfile << (samplesm + 1) * (samplesn + 1) << endl;
	std::vector<int> m_ArrayLabels;
	int ii = 0;
	int label;
	for(i = 0; i <= samplesm; i++)
		for(j = 0; j <= samplesn; j++)
		{
			double s = ((double)i) / samplesm;
			double t = ((double)j) / samplesn;

			label = 1;
			if(pSurface->ptransformation)
			{

				Transformation *pTrans = (Transformation*)pSurface->ptransformation;

				s = -0.855068 + (-0.0486115 + 0.855068) * s;
				t = -1.24888 + (-0.201405 + 1.24888) * t;

				double u,v;
				pTrans->Evaluation(s,t,u,v);

				if(pTrans->m_ReparameterizationType == generalbilinear)
				{
					GeneralBilinearTransformation* pTran1 = (GeneralBilinearTransformation*)pTrans;
					if(u < pTran1->umin)
						label = 0;
					if(v < pTran1->vmin)
						label = 0;
					if(u > pTran1->umax)
						label = 0;
					if(v > pTran1->vmax)
						label = 0;
					if(u < pTran1->umin)
						label = 0;
					if(v < pTran1->vmin)
						label = 0;
					if(u > pTran1->umax)
						label = 0;
					if(v > pTran1->vmax)
						label = 0;
				}

				s = u;
				t = v;

			}else
			{
				double umin = pNurbsSurface1->GetUKnot(0);
				double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

				double vmin = pNurbsSurface1->GetVKnot(0);
				double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

				s = umin + (umax - umin) * s;
				t = vmin + (vmax - vmin) * t;
			}

			double u,v;
			u = s;
			v = t;

			m_ArrayLabels.push_back(label);
			if(label)
			{
				Point3D p1 = pNurbsSurface1->ComputPosition(u,v);
				outfile << "<" << p1[0] <<" , " << p1[1] << " , "<< -p1[2] << " >, " <<endl;
			}else
			{
				outfile << "<" << 0 <<" , " <<0 << " , "<< 0 << " >, " <<endl;
			}

		}
		outfile << " }" << endl;

		ii = 0;
		outfile << "uv_vectors {" << endl;
		outfile << (samplesm + 1) * (samplesn + 1) << endl;
		double maxangle_real = -1e10;
		for(i = 0; i <= samplesm; i++)
			for(j = 0; j <= samplesn; j++)
			{
				double u = ((double)i) / samplesm;
				double v = ((double)j) / samplesn;

				if(m_ArrayLabels[ii])
				{
					outfile << " < " << u << " , " << v << ">, " <<endl;
				}
				else
				{
					outfile << " < " << 0 << " , " << 0 << ">, " <<endl;
				}
				ii++;

			}
			outfile << " }" << endl;

			ii = 0;
			//normal
			outfile << "normal_vectors {" << endl;
			outfile << (samplesm + 1) * (samplesn + 1) << endl;
			for(i = 0; i <= samplesm; i++)
				for(j = 0; j <= samplesn; j++)
				{
					if(m_ArrayLabels[ii])
					{
						double s = ((double)i) / samplesm;
						double t = ((double)j) / samplesn;
						double u,v;

						if(pSurface->ptransformation)
						{

							Transformation *pTrans = (Transformation*)pSurface->ptransformation;
							s = -0.855068 + (-0.0486115 + 0.855068) * s;
							t = -1.24888 + (-0.201405 + 1.24888) * t;
							double u,v;
							pTrans->Evaluation(s,t,u,v);

							s = u;
							t = v;

						}else
						{
							double umin = pNurbsSurface1->GetUKnot(0);
							double umax = pNurbsSurface1->GetUKnot(pNurbsSurface1->GetCtrlNumberU() + pNurbsSurface1->GetOrderU() - 1);

							double vmin = pNurbsSurface1->GetVKnot(0);
							double vmax = pNurbsSurface1->GetVKnot(pNurbsSurface1->GetCtrlNumberV() + pNurbsSurface1->GetOrderV() - 1);

							s = umin + (umax - umin) * s;
							t = vmin + (vmax - vmin) * t;
						}

						u = s;
						v = t;

						Vector3D v1 = pNurbsSurface1->ComputUDerivative1(u,v);
						Vector3D v2 = pNurbsSurface1->ComputVDerivative1(u,v);
						v1.SetUnit();
						v2.SetUnit();

						Vector3D n = v1 ^ v2;
						outfile << " < " << n[0] << " , " << n[1] << " , " << -n[2] << " >,"<<endl;
					}else
					{
						outfile << " < " << 0 << " , " << 0 << " , " << 0 << " >,"<<endl;
					}
					ii++;

				}
				outfile << " }" << endl;

				ii = 0;
				//face_indices {
				for(j = 0; j < samplesn; j++)
					for(i = 0; i < samplesm; i++)
					{
						double u = ((double)i) / samplesm;
						double v = ((double)j) / samplesn;

						int n1 = j * (samplesm + 1) + i;
						int n2 = j * (samplesm + 1) + i + 1;
						int n3 = (j + 1)* (samplesm + 1) + i;
						int n4 = (j + 1)* (samplesm + 1) + i + 1;

						if(m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
						{
							ii++;
						}
						if(m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
						{
							ii++;
						}
					}

					outfile << "face_indices {" << endl;
					outfile << ii << endl;
					for(j = 0; j < samplesn; j++)
						for(i = 0; i < samplesm; i++)
						{
							double u = ((double)i) / samplesm;
							double v = ((double)j) / samplesn;

							int n1 = j * (samplesm + 1) + i;
							int n2 = j * (samplesm + 1) + i + 1;
							int n3 = (j + 1)* (samplesm + 1) + i;
							int n4 = (j + 1)* (samplesm + 1) + i + 1;

							if(m_ArrayLabels[n1] && m_ArrayLabels[n2] && m_ArrayLabels[n4])
							{
								outfile << " < " << n1 << " , " << n2 << " , " << n4 << " >,"<<endl;
								ii++;
							}
							if(m_ArrayLabels[n1] && m_ArrayLabels[n3] && m_ArrayLabels[n4])
							{
								outfile << " < " << n1 << " , " << n4 << " , " << n3 << " >,"<<endl;
								ii++;
							}
						}
						outfile << " }" << endl;

						outfile.close();
}

void Actions_GeneralBilinear::OutPutMeshEdges_PovRay_Terrain_Result(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	double s1,s2,t1,t2;
	QuadraticTransformation* pTrans = (QuadraticTransformation*)pSurface->ptransformation;
	int i,j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	samplesm = samplesm * 600;
	for(j = 0; j <= samplesn; j++)
	{
		for(i = 0; i < samplesm; i++)
		{	//
			s1 = (double)i / samplesm;
			s2 = (double)(i + 1) / samplesm;
			t1 = t2 = (double)j / samplesn;

			s1 = pTrans->umin + (pTrans->umax - pTrans->umin) * s1;
			t1 = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t1;
			s2 = pTrans->umin + (pTrans->umax - pTrans->umin) * s2;
			t2 = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t2;

			double s_1,t_1,s_2,t_2;
			pTrans->InverseEvaluation(s1,t1,s_1,t_1);
			pTrans->InverseEvaluation(s2,t2,s_2,t_2);

			if(s_1 < -0.855068 || s_1 > -0.0486115)
				continue;
			if(t_1 < -1.24888 || t_1 > -0.201405)
				continue;
			if(s_2 < -0.855068 || s_2 > -0.0486115)
				continue;
			if(t_2 < -1.24888 || t_2 > -0.201405)
				continue;
			//if(pSurface->ptransformation)
			//{

				/*if(s1 < -0.855068 || s1 > -0.0486115)
					continue;
				if(t1 < -1.24888 || t1 > -0.201405)
					continue;
				if(s2 < -0.855068 || s2 > -0.0486115)
					continue;
				if(t2 < -1.24888 || t2 > -0.201405)
					continue;*/

				//s1 = -0.855068 + (-0.0486115 + 0.855068) * s1;
				//s2 = -0.855068 + (-0.0486115 + 0.855068) * s2;
				//t1 = -1.24888 + (-0.201405 + 1.24888) * t1;
				//t2 = -1.24888 + (-0.201405 + 1.24888) * t2;


				double u,v;
				//pTrans->Evaluation(s1,t1,u,v);

				//s1 = u;
				//t1 = v;
				//pTrans->Evaluation(s2,t2,u,v);
				//s2 = u;
				//t2 = v;

			//}

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1,v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2,v2) - ZeroP3D);

			//if(Length(vv1 - vv2) > 1e-5)
			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	samplesm = samplesm / 600;
	samplesn = samplesn * 600;

	for(i = 0; i <= samplesm; i++)
	{
		for(j = 0; j < samplesn; j++)
		{	//
			s1 = s2 = (double)i / samplesm;
			t1 = (double)j / samplesn;
			t2 = (double)(j + 1) / samplesn;

			s1 = pTrans->umin + (pTrans->umax - pTrans->umin) * s1;
			t1 = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t1;
			s2 = pTrans->umin + (pTrans->umax - pTrans->umin) * s2;
			t2 = pTrans->vmin + (pTrans->vmax - pTrans->vmin) * t2;

			double s_1,t_1,s_2,t_2;
			pTrans->InverseEvaluation(s1,t1,s_1,t_1);
			pTrans->InverseEvaluation(s2,t2,s_2,t_2);

			if(s_1 < -0.855068 || s_1 > -0.0486115)
				continue;
			if(t_1 < -1.24888 || t_1 > -0.201405)
				continue;
			if(s_2 < -0.855068 || s_2 > -0.0486115)
				continue;
			if(t_2 < -1.24888 || t_2 > -0.201405)
				continue;

			//double umin = pNurbsSurface->GetUKnot(0);
			//double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

			//double vmin = pNurbsSurface->GetVKnot(0);
			//double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);


				//Transformation *pTrans = (Transformation*)pSurface->ptransformation;
				//s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
				//s2 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s2;
				//t1 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t1;
				//t2 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t2;

				//s1 = -0.855068 + (-0.0486115 + 0.855068) * s1;
				//s2 = -0.855068 + (-0.0486115 + 0.855068) * s2;
				//t1 = -1.24888 + (-0.201405 + 1.24888) * t1;
				//t2 = -1.24888 + (-0.201405 + 1.24888) * t2;

				double u,v;
				//pTrans->Evaluation(s1,t1,u,v);
				//s1 = u;
				//t1 = v;
				//pTrans->Evaluation(s2,t2,u,v);
				//s2 = u;
				//t2 = v;

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1,v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2,v2) - ZeroP3D);

			//if(Length(vv1 - vv2) > 1e-5)
			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	outfile.close();
}
void Actions_GeneralBilinear::OutPutMeshEdges_PovRay_Terrain_Original(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename)
{
	C_NurbsSurf* pNurbsSurface = pSurface->Converte_C_NurbsSurface();
	std::ofstream outfile(filename);
	double s1,s2,t1,t2;
	int i,j;
	//outfile << "# " <<(samplesm + 1) * (samplesn + 1) << "vertices, " << samplesm * samplesn <<"faces" << endl;
	samplesm = samplesm * 300;
	for(j = 0; j <= samplesn; j++)
	{
		for(i = 0; i < samplesm; i++)
		{	//
			s1 = (double)i / samplesm;
			s2 = (double)(i + 1) / samplesm;
			t1 = t2 = (double)j / samplesn;

			if(pSurface->ptransformation)
			{

				Transformation *pTrans = (Transformation*)pSurface->ptransformation;

				/*if(s1 < -0.855068 || s1 > -0.0486115)
					continue;
				if(t1 < -1.24888 || t1 > -0.201405)
					continue;
				if(s2 < -0.855068 || s2 > -0.0486115)
					continue;
				if(t2 < -1.24888 || t2 > -0.201405)
					continue;*/

				s1 = -0.855068 + (-0.0486115 + 0.855068) * s1;
				s2 = -0.855068 + (-0.0486115 + 0.855068) * s2;
				t1 = -1.24888 + (-0.201405 + 1.24888) * t1;
				t2 = -1.24888 + (-0.201405 + 1.24888) * t2;


				double u,v;
				pTrans->Evaluation(s1,t1,u,v);

				s1 = u;
				t1 = v;
				pTrans->Evaluation(s2,t2,u,v);
				s2 = u;
				t2 = v;

				if(s1 < pTrans->umin)
					continue;
				if(t1 < pTrans->vmin)
					continue;
				if(s1 > pTrans->umax)
					continue;
				if(t1 > pTrans->vmax)
					continue;
				if(s2 < pTrans->umin)
					continue;
				if(t2 < pTrans->vmin)
					continue;
				if(s2 > pTrans->umax)
					continue;
				if(t2 > pTrans->vmax)
					continue;

			}else
			{
				double umin = pNurbsSurface->GetUKnot(0);
				double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

				double vmin = pNurbsSurface->GetVKnot(0);
				double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

				s1 = umin + (umax - umin) * s1;
				s2 = umin + (umax - umin) * s2;
				t1 = vmin + (vmax - vmin) * t1;
				t2 = vmin + (vmax - vmin) * t2;
			}

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1,v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2,v2) - ZeroP3D);

			//if(Length(vv1 - vv2) > 1e-5)
			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	samplesm = samplesm / 300;
	samplesn = samplesn * 300;

	for(i = 0; i <= samplesm; i++)
	{
		for(j = 0; j < samplesn; j++)
		{	//
			s1 = s2 = (double)i / samplesm;
			t1 = (double)j / samplesn;
			t2 = (double)(j + 1) / samplesn;

			double umin = pNurbsSurface->GetUKnot(0);
			double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

			double vmin = pNurbsSurface->GetVKnot(0);
			double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

			if(pSurface->ptransformation)
			{

				Transformation *pTrans = (Transformation*)pSurface->ptransformation;
				//s1 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s1;
				//s2 = pTrans->umin_Reparameterization + (pTrans->umax_Reparameterization - pTrans->umin_Reparameterization) * s2;
				//t1 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t1;
				//t2 = pTrans->vmin_Reparameterization + (pTrans->vmax_Reparameterization - pTrans->vmin_Reparameterization) * t2;

				s1 = -0.855068 + (-0.0486115 + 0.855068) * s1;
				s2 = -0.855068 + (-0.0486115 + 0.855068) * s2;
				t1 = -1.24888 + (-0.201405 + 1.24888) * t1;
				t2 = -1.24888 + (-0.201405 + 1.24888) * t2;

				double u,v;
				pTrans->Evaluation(s1,t1,u,v);
				s1 = u;
				t1 = v;
				pTrans->Evaluation(s2,t2,u,v);
				s2 = u;
				t2 = v;

				if(s1 < pTrans->umin)
					continue;
				if(t1 < pTrans->vmin)
					continue;
				if(s1 > pTrans->umax)
					continue;
				if(t1 > pTrans->vmax)
					continue;
				if(s2 < pTrans->umin)
					continue;
				if(t2 < pTrans->vmin)
					continue;
				if(s2 > pTrans->umax)
					continue;
				if(t2 > pTrans->vmax)
					continue;


			}else
			{
				s1 = umin + (umax - umin) * s1;
				s2 = umin + (umax - umin) * s2;
				t1 = vmin + (vmax - vmin) * t1;
				t2 = vmin + (vmax - vmin) * t2;
			}

			double u1 = s1;
			double v1 = t1;

			double u2 = s2;
			double v2 = t2;

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u1,v1) - ZeroP3D);
			Vector3D vv2 = (pNurbsSurface->ComputPosition(u2,v2) - ZeroP3D);

			//if(Length(vv1 - vv2) > 1e-5)
			outfile << "cylinder {<" << vv1[0] << " , " << vv1[1] << " , " << -vv1[2] << " >,<" << vv2[0] << " , " << vv2[1] << " , " << -vv2[2] << " > ,r_edge texture { edge_tex } }" << endl;
		}
	}

	outfile.close();
}
void Actions_GeneralBilinear::OnBnClickedGeneralbilinearPovray()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	//first read the sur
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\terrain\\good\\terrain.sur");

	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	QuadraticTransformation* pTrans = new QuadraticTransformation;

	pTrans->m_Quadratic_Alpha1 = 0.91;
	pTrans->m_Quadratic_Alpha2 = 0.58;
	pTrans->m_Quadratic_Alpha3 = 0.05;
	pTrans->m_Quadratic_Beta1 = 0.49;
	pTrans->m_Quadratic_Beta2 = 0.08;
	pTrans->m_Quadratic_Beta3 = 0.49;
	pTrans->m_ReparameterizationType = quadratic;
	pTrans->umin_Reparameterization = pSurface->m_uknot[0];
	pTrans->umax_Reparameterization = pSurface->m_uknot[pSurface->m_uknotcount - 1];
	pTrans->vmin_Reparameterization = pSurface->m_vknot[0];
	pTrans->vmax_Reparameterization = pSurface->m_vknot[pSurface->m_vknotcount - 1];

	pTrans->umin = pSurface->m_uknot[0];
	pTrans->umax = pSurface->m_uknot[pSurface->m_uknotcount - 1];
	pTrans->vmin = pSurface->m_vknot[0];
	pTrans->vmax = pSurface->m_vknot[pSurface->m_vknotcount - 1];
	pSurface->ptransformation = pTrans;

	//The original result
	OutPutMeshFaces_PovRay_Terrain_Original(pSurface,200,200,"D:\\Terrainmesh.inc");

	OutPutMeshEdges_PovRay_Terrain_Original(pSurface,10,10,"D:\\meshterrainiso.inc");

	OutPutTexture_PovRay_Terrain_Original(pSurface,500,500,"D:\\terraintexture.inc");

	//The optimal result
	OutPutMeshEdges_PovRay_Terrain_Result(pSurface,10,10,"D:\\meshterrainiso_result.inc");

	OutPutTexture_PovRay_Terrain_Result(pSurface,500,500,"D:\\terraintexture_result.inc");

	OutPutConformity_PovRay_original(pSurface,500,500,"D:\\terrainconformity_original.inc");

	OutPutConformity_PovRay_result(pSurface,500,500,"D:\\terrainconformity_result.inc");

}
void Actions_GeneralBilinear::OnBnClickedActionsBezier()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\originalparameter.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1.sur");
	pDoc->OnOpenDocument("D:\\papers\\code\\SVNNurbs\\data\\Bezier\\parameterdomain1.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\SVNNurbs\\data\\Bezier\\conformal2.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\SVNNurbs\\data\\Bezier\\example1.sur");
	//Initial value
	OnBnClickedActionsGeneralbilinearInitialvalue();
	//OnBnClickedActionsGeneralbilinearInitialvalue_Degenerate();
}
void Actions_GeneralBilinear::OnBnClickedButtonCartop()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\cartop\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\cartop\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\cartop\\cartopbad.sur");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\cartop\\good\\parameterdomain.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\cartop\\good\\conformal.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\cartop\\good\\cartop.sur");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\cartop\\new\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\cartop\\new\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\cartop\\new\\newcartop.sur");
	//Initial value
	//Initial value
	//OnBnClickedActionsGeneralbilinearInitialvalue();
	OnBnClickedActionsGeneralbilinearInitialvalue_Degenerate();
}

void Actions_GeneralBilinear::OnBnClickedButtonFace()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\originalparameter.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\facebadparameterization.sur");

	
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\good\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\good\\meshpara.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\good\\face.sur");

	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\example1_face\\good\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\example1_face\\good\\meshpara.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\example1_face\\good\\face.sur");
	//Initial value
	//OnBnClickedActionsGeneralbilinearInitialvalue();
	//OnBnClickedActionsGeneralbilinearInitialvalue_Degenerate();
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\parameterdomain.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\conformal.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\face.sur");
	//OnBnClickedActionsGeneralbilinearInitialvalue_Degenerate();
	OnBnClickedActionsGeneralbilinearInitialvalue();
}

void Actions_GeneralBilinear::OnBnClickedButtonBone()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\originalparameter.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\facebadparameterization.sur");
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\bone\\parameterdomain.obj");
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\bone\\conformal.obj");
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\bone\\bone.sur");
	//Initial value
	OnBnClickedActionsGeneralbilinearInitialvalue();
}

void Actions_GeneralBilinear::OnBnClickedButtonVenus()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\originalparameter.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\facebadparameterization.sur");
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\venus\\parameterdomain.obj");
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\venus\\conformal.obj");
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\venus\\venus.sur");
	//Initial value
	OnBnClickedActionsGeneralbilinearInitialvalue();
}

void Actions_GeneralBilinear::OnBnClickedButtonSophie()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\originalparameter.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1_face\\facebadparameterization.sur");
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\sophie\\parameterdomain.obj");
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\sophie\\conformal.obj");
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\sophie\\sophieconformal.sur");
	//Initial value
	//OnBnClickedActionsGeneralbilinearInitialvalue();
	OnBnClickedActionsGeneralbilinearInitialvalue_Degenerate();
}

void Actions_GeneralBilinear::OnBnClickedButtonTerrain()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\example1_face\\originalparameter.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\example1_face\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\example1_face\\facebadparameterization.sur");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\terrain\\good\\parameterdomain.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\terrain\\good\\conformal.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\terrain\\good\\terrain.sur");

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\terrain\\example3\\parameterdomain.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\terrain\\example3\\conformal.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\terrain\\example3\\terrain.sur");

	//Initial value
	OnBnClickedActionsGeneralbilinearInitialvalue();
	//OnBnClickedActionsGeneralbilinearInitialvalue_Degenerate();
}

void Actions_GeneralBilinear::OnBnClickedActionsGeneralbilinearPlot()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	((GeneralBilinearTransformation*)pSurface->ptransformation)->label = -1;
	C_ShowTransformedParameterDomain dlg;
	dlg.pSurface = pSurface;
	dlg.DoModal();
}

void Actions_GeneralBilinear::OnBnClickedButtonGeneralbilinearinitialvalue()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\parameterdomain.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\conformal.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Bsplinemethod\\face\\face.sur");
	//OnBnClickedActionsGeneralbilinearInitialvalue_Degenerate();
	OnBnClickedActionsGeneralbilinearInitialvalue();
}
