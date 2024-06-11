#include "stdafx.h"
#include "Utility.h"
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
#include "Point3D.h"
Utility::Utility(void)
{
}

Utility::~Utility(void)
{
}

double Utility::ComputeBasis(int i,int p, double u,const std::vector<double> &m_knotArray)
{
	double ui = m_knotArray[i];
	double uia1 = m_knotArray[i + 1];
	if(p == 0)
	{
		if(u >= ui && u <= uia1)
			return 1;
		else
			return 0;
	}else
	{
		double denorm1 = (m_knotArray[i + p]- ui);
		double denorm2 = (m_knotArray[i + p + 1] - uia1);

		double term1,term2;
		if(fabs(denorm1) < 1e-8)
		{
			term1 = 0;
		}else
		{
			term1 = (u -  ui) * ComputeBasis(i,p-1,u,m_knotArray)  /  denorm1;
		}
		if(fabs(denorm2) < 1e-8)
		{
			term2 = 0;
		}else
		{
			term2 = (m_knotArray[i + p + 1] - u) * ComputeBasis(i + 1,p - 1,u,m_knotArray) / denorm2;
		}

		double u1 =  term1 + term2;
		return u1;
	}
}

C_NurbsCurve3D* Utility::FittingBsplineCurve(const std::vector<double> &m_knotVector,const std::vector<Point3D> &m_PointArray,const std::vector<double> &m_ParaArray)
{
	std::vector<double> m_WeightArray;
	std::vector<double> m_ConstrainedParaArray;
	std::vector<Point3D> m_ConstrainedPointArray;
	for(int i = 0; i < m_PointArray.size(); i++)
		m_WeightArray.push_back(1);
	return FittingBsplineCurve_Constrained_Weighted(m_knotVector,m_PointArray,m_ParaArray,m_WeightArray,m_ConstrainedParaArray,m_ConstrainedPointArray);
}

C_NurbsCurve3D* Utility::FittingBsplineCurve_Constrained_Weighted(const std::vector<double> &m_knotVector,const std::vector<Point3D> &m_PointArray,const std::vector<double> &m_ParaArray,
																			  const std::vector<double> &m_WeightArray,const std::vector<double> &m_ConstrainedParaArray,const std::vector<Point3D> &m_ConstrainedPointArray)
{
	//------------------------------------------------------------------------------
	//Input:
	//m_knotVector stores the knot values of the curve.
	//Fitting a Bspline curve from Qis in m_PointArray.
	//The parameters of the given points are in the array m_ParaArray.
	//m_WeightArray: the weights of the given points in m_PointArray.
	//m_ConstrainedParaArray: the parameters of the constrained points
	//m_ConstrainedPointArray: the 3D constrained points on the curve.
	//Output:
	// We will generate a degree-3 Bspline least square fitting curve 
	//--------------------------------------------------------------------------------

	int order = 4;
	int ctrlnum = m_knotVector.size() - order;

	//First generate the curve.
	double* knot = new double[m_knotVector.size()];
	for(int i = 0; i < m_knotVector.size(); i++)
		knot[i] =  m_knotVector[i];
	Point3D* pControlPoint = new Point3D[ctrlnum];
	double* pWeight = new double[ctrlnum];

	C_NurbsCurve3D* pNurbsCurve = new C_NurbsCurve3D(ctrlnum,order,pControlPoint,knot,pWeight);
	
	Point3D startpoint,endpoint;
	startpoint = m_PointArray[0];
	endpoint = m_PointArray[m_PointArray.size() - 1];

	//Set the start and end point
	pNurbsCurve->SetControlPoint(0,startpoint);
	pNurbsCurve->SetControlPoint(ctrlnum - 1,endpoint);
	pNurbsCurve->SetWeight(0,1);
	pNurbsCurve->SetWeight(ctrlnum - 1,1);

	//solve the other points using a least square method in NURBS book with weights and linear constraints.
	std::vector<double> coefficient;
	std::vector<double> barray_x;
	std::vector<double> barray_y;
	std::vector<double> barray_z;
	std::vector<std::vector<double>> coefficientArray;
	double c;
	for(int l = 1; l < ctrlnum  - 1; l++)
	{
		//compute the l derivative
		coefficient.clear();
		for(int i = 1; i < ctrlnum  - 1;i++)
		{
			c = 0;
			for(int k = 1; k < m_PointArray.size() - 1; k++)
			{
				double t = m_ParaArray[k];
				
				double val;

				double ubas = pNurbsCurve->ComputeBasis(l,t);
				double ubas1 = pNurbsCurve->ComputeBasis(i,t);
				c = c + ubas  * ubas1 * m_WeightArray[k];
				
			}
			coefficient.push_back(c);
		}
		//linear constraints
		
		for(int i = 0; i < m_ConstrainedParaArray.size(); i++)
		{
			c = pNurbsCurve->ComputeBasis(l,m_ConstrainedParaArray[i]);
			coefficient.push_back(c);
		}
		coefficientArray.push_back(coefficient);
		
		//x
		double c = 0;
		for(int k = 1; k < m_PointArray.size() - 1; k++)
		{
			double t = m_ParaArray[k];
			Point3D point = m_PointArray[k];

			double ubas = pNurbsCurve->ComputeBasis(l,t);
			double ubas1 = pNurbsCurve->ComputeBasis(0,t);
			double ubas2 = pNurbsCurve->ComputeBasis(ctrlnum - 1,t);
			c = c + ubas *(point.x - ubas1 * startpoint.x - ubas2 * endpoint.x)  * m_WeightArray[k];
		}
		barray_x.push_back(c);

		//y
		c = 0;
		for(int k = 1; k < m_PointArray.size() - 1; k++)
		{
			double t = m_ParaArray[k];
			Point3D point = m_PointArray[k];

			double ubas = pNurbsCurve->ComputeBasis(l,t);
			double ubas1 = pNurbsCurve->ComputeBasis(0,t);
			double ubas2 = pNurbsCurve->ComputeBasis(ctrlnum - 1,t);
			c = c + ubas *(point.y - ubas1 * startpoint.y - ubas2 * endpoint.y)  * m_WeightArray[k];
		}
		barray_y.push_back(c);

		//z
		c = 0;
		for(int k = 1; k < m_PointArray.size() - 1; k++)
		{
			double t = m_ParaArray[k];
			Point3D point = m_PointArray[k];

			double ubas = pNurbsCurve->ComputeBasis(l,t);
			double ubas1 = pNurbsCurve->ComputeBasis(0,t);
			double ubas2 = pNurbsCurve->ComputeBasis(ctrlnum - 1,t);
			c = c + ubas *(point.z - ubas1 * startpoint.z - ubas2 * endpoint.z)  * m_WeightArray[k];
		}
		barray_z.push_back(c);
	}
	
	//Linear constraints.

	for(int l = 0; l < m_ConstrainedParaArray.size(); l++)
	{
		coefficient.clear();
		for(int i = 1; i < ctrlnum - 1; i++)
		{
			double c;
			double t = m_ConstrainedParaArray[l];
			c = pNurbsCurve->ComputeBasis(i,t);
			coefficient.push_back(c);
		}

		for(int i = 0; i < m_ConstrainedParaArray.size(); i++)
			coefficient.push_back(0);

		coefficientArray.push_back(coefficient);

		//x
		Point3D point = m_ConstrainedPointArray[l];
		double c;
		c = point.x - startpoint.x * pNurbsCurve->ComputeBasis(0,m_ConstrainedParaArray[l]) - endpoint.x * pNurbsCurve->ComputeBasis(ctrlnum - 1,m_ConstrainedParaArray[l]);
		barray_x.push_back(c);

		//y;
		c = point.y - startpoint.y * pNurbsCurve->ComputeBasis(0,m_ConstrainedParaArray[l]) - endpoint.y * pNurbsCurve->ComputeBasis(ctrlnum - 1,m_ConstrainedParaArray[l]);
		barray_y.push_back(c);

		//z
		c = point.z - startpoint.z * pNurbsCurve->ComputeBasis(0,m_ConstrainedParaArray[l]) - endpoint.z * pNurbsCurve->ComputeBasis(ctrlnum - 1,m_ConstrainedParaArray[l]);
		barray_z.push_back(c);
	}

	TRACE("\nCoefficients_1\n");
	for(int i = 0; i < coefficientArray.size(); i++)
	{
		TRACE("\n");
		for(int j = 0; j < coefficientArray[i].size(); j++)
		{
			TRACE("%5.1f    ",coefficientArray[i][j]);
		}
	}
	TRACE("\nbarray_1\n");
	for(int i = 0; i < barray_x.size(); i++)
		TRACE(" %5.1f ",barray_x[i]);
	std::vector<double> xarray,yarray,zarray;
	p_FormView3->SolveLinearSystem(coefficientArray,barray_x,xarray);
	p_FormView3->SolveLinearSystem(coefficientArray,barray_y,yarray);
	p_FormView3->SolveLinearSystem(coefficientArray,barray_z,zarray);
	TRACE("\nxarray\n");
	for(int i = 0; i < xarray.size(); i++)
		TRACE(" %5.1f ",xarray[i]);
	for(int i = 1; i < ctrlnum  - 1; i++)
	{
		Point3D point;
		point.x = xarray[i - 1];
		point.y = yarray[i - 1];
		point.z = zarray[i - 1];

		pNurbsCurve->SetControlPoint(i,point);
		pNurbsCurve->SetWeight(i,1);
	}
	return pNurbsCurve;
}

