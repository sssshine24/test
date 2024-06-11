#include "stdafx.h"
#include "resource.h"
#include "math.h"
#include "GeneralBilinearTransformation.h"
#include "lib3d\Mesh3d.h"
#include <fstream>
#include "FormView3.h"
#include "Nurbs3D.h"
GeneralBilinearTransformation::GeneralBilinearTransformation(void)
{
	e00 = 0;
	e01 = 0;
	e10 = 1;
	e11 = 1;
	f00 = 1;
	f01 = 1;
	f10 = 1;
	f11 = 1;
	g00 = 0;
	g01 = 1;
	g10 = 0;
	g11 = 1;
	h00 = 1;
	h01 = 1;
	h10 = 1;
	h11 = 1;
	label = 0;

	umin = 0;
	umax = 1;
	vmin = 0;
	vmax = 1;

	pNewSurface = NULL;
	pSurface = NULL;
}
double GeneralBilinearTransformation::Comput_u_s(double s,double t)
{
	//
	double numer = -e10*f00+e00*f10-e10*SQR(t)*f00+2*e10*f00*t-e10*f01*t+e10*SQR(t)*f01-e11*t*f00+e11*SQR(t)*f00-e11*SQR(t)*f01-2*e00*f10*t+e00*f11*t+e00*SQR(t)*f10-e00*SQR(t)*f11+e01*t*f10-e01*SQR(t)*f10+e01*SQR(t)*f11;
	numer = -numer;
	double denominator = SQR(f00-f00*t-f00*s+f00*s*t+f01*t-f01*t*s+f10*s-f10*s*t+f11*s*t);
	double val = numer / denominator;
	return val;
}
double GeneralBilinearTransformation::Comput_u_t(double s,double t)
{
	double numer = 2*e01*f00*s-e01*f10*s-e01*SQR(s)*f00+e01*SQR(s)*f10-e11*s*f00+e11*SQR(s)*f00-e11*SQR(s)*f10-2*e00*f01*s+e00*f11*s+e00*SQR(s)*f01-e00*SQR(s)*f11+e10*s*f01-e10*SQR(s)*f01+e10*SQR(s)*f11-e01*f00+e00*f01;
	numer = -numer;
	double denominator = SQR(f00-f00*t-f00*s+f00*s*t+f01*t-f01*t*s+f10*s-f10*s*t+f11*s*t);
	double val = numer / denominator;
	return val;
}
double GeneralBilinearTransformation::Comput_v_s(double s,double t)
{
	double numer = 2*g10*h00*t-g10*h01*t-g10*SQR(t)*h00+g10*SQR(t)*h01-g11*t*h00+g11*SQR(t)*h00-g11*SQR(t)*h01-2*g00*h10*t+g00*h11*t+g00*SQR(t)*h10-g00*SQR(t)*h11+g01*t*h10-g01*SQR(t)*h10+g01*SQR(t)*h11-g10*h00+g00*h10;
	numer = -numer;
	double denominator = SQR(h00-h00*t-h00*s+h00*s*t+h01*t-h01*t*s+h10*s-h10*s*t+h11*s*t);
	double val = numer / denominator;
	return val;
}
double GeneralBilinearTransformation::Comput_v_t(double s,double t)
{
	double numer = -g01*h00+g00*h01+g01*SQR(s)*h10+2*g01*h00*s+g11*SQR(s)*h00-2*g00*h01*s+g00*h11*s-g01*SQR(s)*h00-g11*s*h00-g01*h10*s-g11*SQR(s)*h10+g00*SQR(s)*h01-g00*SQR(s)*h11+g10*s*h01-g10*SQR(s)*h01+g10*SQR(s)*h11;
	numer = -numer;
	double denominator = SQR(h00-h00*t-h00*s+h00*s*t+h01*t-h01*t*s+h10*s-h10*s*t+h11*s*t);
	double val = numer / denominator;
	return val;
}
GeneralBilinearTransformation::~GeneralBilinearTransformation(void)
{
}
void GeneralBilinearTransformation::SetInverseEvaluationLabel(CMesh3d* pMeshuv,CMesh3d* pMeshst)
{
	double u,v;
	double a,b,c;
	double s,t;
	for(int i = 0; i < pMeshuv->NbVertex(); i++)
	{
		u = pMeshuv->GetVertex(i)->x();
		v = pMeshuv->GetVertex(i)->y();
		s = pMeshst->GetVertex(i)->x();
		t = pMeshst->GetVertex(i)->y();

		a = u*f00*g11+u*f01*g10-u*f01*g11-e10*v*h01-e11*v*h00+e11*v*h01-u*f10*g01-u*f11*g00+u*f11*g01+e00*v*h11+e01*v*h10-e01*v*h11+u*f00*v*h10-u*f10*v*h00-e00*g11-e01*g10+e01*g11+e10*g01+e11*g00-e11*g01-e00*v*h10-u*f00*g10+e10*v*h00+u*f10*g00-e10*g00+e00*g10+u*f10*v*h01+u*f11*v*h00-u*f11*v*h01-u*f00*v*h11-u*f01*v*h10+u*f01*v*h11;
		if(fabs(a) > 1e-5)
			break;
	}
	b = -2*e00*g10+2*e10*g00-e10*g01-u*f10*v*h01-u*f00*g11-e11*g00-u*f11*v*h00-u*f01*g10+e00*g11-2*e10*v*h00+e10*v*h01+e01*g10+u*f10*g01+e11*v*h00+u*f01*v*h10-2*u*f10*g00+u*f00*v*h11+2*u*f00*g10+2*e00*v*h10+u*f11*g00-2*u*f00*v*h10-e01*v*h10-e00*v*h11+2*u*f10*v*h00;
	c = -u*f10*v*h00+e10*v*h00+u*f00*v*h10+u*f10*g00-e10*g00+e00*g10-e00*v*h10-u*f00*g10;
	//a * t^2 + b * t + c = 0
	double t1 = (-b + sqrt(b * b - 4 * a * c))/(a * 2);
	double t2 = (-b - sqrt(b * b - 4 * a * c))/(a * 2);
	double s1 = (-e00+e00*t1-e01*t1+u*f00-u*f00*t1+u*f01*t1)/(-e00+e00*t1-e01*t1+e10-e10*t1+e11*t1+u*f00-u*f00*t1+u*f01*t1-u*f10+u*f10*t1-u*f11*t1);
	double s2 = (-e00+e00*t2-e01*t2+u*f00-u*f00*t2+u*f01*t2)/(-e00+e00*t2-e01*t2+e10-e10*t2+e11*t2+u*f00-u*f00*t2+u*f01*t2-u*f10+u*f10*t2-u*f11*t2);

	if(SQR(s - s1) + SQR(t - t1) < SQR(s - s2) + SQR(t - t2))
		label = 1;
	else
		label = -1;
}
int GeneralBilinearTransformation::InverseEvaluation(double u,double v,double &s,double &t)
{
	//Given the (u,v), get the (s,t) 
	double a = u*f00*g11+u*f01*g10-u*f01*g11-e10*v*h01-e11*v*h00+e11*v*h01-u*f10*g01-u*f11*g00+u*f11*g01+e00*v*h11+e01*v*h10-e01*v*h11+u*f00*v*h10-u*f10*v*h00-e00*g11-e01*g10+e01*g11+e10*g01+e11*g00-e11*g01-e00*v*h10-u*f00*g10+e10*v*h00+u*f10*g00-e10*g00+e00*g10+u*f10*v*h01+u*f11*v*h00-u*f11*v*h01-u*f00*v*h11-u*f01*v*h10+u*f01*v*h11;
	double b = -2*e00*g10+2*e10*g00-e10*g01-u*f10*v*h01-u*f00*g11-e11*g00-u*f11*v*h00-u*f01*g10+e00*g11-2*e10*v*h00+e10*v*h01+e01*g10+u*f10*g01+e11*v*h00+u*f01*v*h10-2*u*f10*g00+u*f00*v*h11+2*u*f00*g10+2*e00*v*h10+u*f11*g00-2*u*f00*v*h10-e01*v*h10-e00*v*h11+2*u*f10*v*h00;
	double c = -u*f10*v*h00+e10*v*h00+u*f00*v*h10+u*f10*g00-e10*g00+e00*g10-e00*v*h10-u*f00*g10;
	//a * t^2 + b * t + c = 0
	double val = b * b - 4 * a * c;
	if( val < 0 )
		val = 0;
	if(fabs(a) < 1e-8)
		t = -c / b;
	else
		t = (-b + sqrt(val))/(a * 2);
	double t1;
	if(fabs(a) > 1e-8)
		t1 = (-b - sqrt(val))/(a * 2);
	else
		t1 = t;
	/*if(t < vmin_Reparameterization || t > vmax_Reparameterization)
	{
		t = t1;
	}else
	{
		TRACE("%f %f %f %f\n",e00,e01,e10,e11);
		TRACE("%f %f %f %f\n",f00,f01,f10,f11);
		TRACE("%f %f %f %f\n",g00,g01,g10,g11);
		TRACE("%f %f %f %f\n",h00,h01,h10,h11);
		TRACE("%f %f\n",u,v);
		ASSERT(t1 < vmin_Reparameterization || t1 > vmax_Reparameterization);
	}*/

	s = (-e00+e00*t-e01*t+u*f00-u*f00*t+u*f01*t)/(-e00+e00*t-e01*t+e10-e10*t+e11*t+u*f00-u*f00*t+u*f01*t-u*f10+u*f10*t-u*f11*t);
	double s1 = (-e00+e00*t1-e01*t1+u*f00-u*f00*t1+u*f01*t1)/(-e00+e00*t1-e01*t1+e10-e10*t1+e11*t1+u*f00-u*f00*t1+u*f01*t1-u*f10+u*f10*t1-u*f11*t1);
	if(label == -1)
	{
		s = s1;
		t = t1;
	}else if (label == 0)
	{
		//must set label (call ) before calling this function
		ASSERT(0);
	}

	return 1;
}

int GeneralBilinearTransformation::Evaluation(double s,double t,double &u,double &v)
{
	double denorm1 = f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t;
	double denorm2 = h00 * (1 - s) * (1 - t) + h01 * (1 - s) * t + h10 * s * (1 - t) + h11 * s * t;
	if(fabs(denorm1) > 1e-5 && fabs(denorm2) > 1e-5)
	{
		u = (e00 * (1 - s) * (1 - t) + e01 * (1 - s) * t + e10 * s * (1 - t) + e11 * s * t) / 
			(f00 * (1 - s) * (1 - t) + f01 * (1 - s) * t + f10 * s * (1 - t) + f11 * s * t);
		v = (g00 * (1 - s) * (1 - t) + g01 * (1 - s) * t + g10 * s * (1 - t) + g11 * s * t) / 
			(h00 * (1 - s) * (1 - t) + h01 * (1 - s) * t + h10 * s * (1 - t) + h11 * s * t);

		//test
		/*if(u < 0)
			u = 0;
		if(u > 1)
			u = 1;
		if(v < 0)
			v = 0;
		if(v > 1)
			v = 1;*/
		return 1;
	}
	else
	{
		//the degenerate case
		u = v = -1;
		ASSERT(0);
		return 0;
	}
	return 1;
}
void GeneralBilinearTransformation::Trace()
{
	TRACE("generalbilinear\n");
	TRACE("%f %f %f %f\n",e00,e01,e10,e11);
	TRACE("%f %f %f %f\n",f00,f01,f10,f11);
	TRACE("%f %f %f %f\n",g00,g01,g10,g11);
	TRACE("%f %f %f %f\n",h00,h01,h10,h11);
}
int GeneralBilinearTransformation::IsValid()
{
	//check whether the value is right.
	int m = 1000;
	int n = 1000;
	double u,v;
	for(int i = 0; i <= m; i++)
		for(int j = 0; j <= n; j++)
		{
			u = umin + i * (umax - umin) / m;
			v = vmin + j * (vmax - vmin) / n;

			double a = u*f00*g11+u*f01*g10-u*f01*g11-e10*v*h01-e11*v*h00+e11*v*h01-u*f10*g01-u*f11*g00+u*f11*g01+e00*v*h11+e01*v*h10-e01*v*h11+u*f00*v*h10-u*f10*v*h00-e00*g11-e01*g10+e01*g11+e10*g01+e11*g00-e11*g01-e00*v*h10-u*f00*g10+e10*v*h00+u*f10*g00-e10*g00+e00*g10+u*f10*v*h01+u*f11*v*h00-u*f11*v*h01-u*f00*v*h11-u*f01*v*h10+u*f01*v*h11;
			double b = -2*e00*g10+2*e10*g00-e10*g01-u*f10*v*h01-u*f00*g11-e11*g00-u*f11*v*h00-u*f01*g10+e00*g11-2*e10*v*h00+e10*v*h01+e01*g10+u*f10*g01+e11*v*h00+u*f01*v*h10-2*u*f10*g00+u*f00*v*h11+2*u*f00*g10+2*e00*v*h10+u*f11*g00-2*u*f00*v*h10-e01*v*h10-e00*v*h11+2*u*f10*v*h00;
			double c = -u*f10*v*h00+e10*v*h00+u*f00*v*h10+u*f10*g00-e10*g00+e00*g10-e00*v*h10-u*f00*g10;

			if(b * b - 4 * a * c < 0)
				return 0;
		}
	return 1;
}
int GeneralBilinearTransformation::ComputeGeneralBilinearTransformationFromSevenPoints()
{
	double u1 = p1.x;
	double v1 = p1.y;
	double u2 = p2.x;
	double v2 = p2.y;
	double u3 = p3.x;
	double v3 = p3.y;
	double u4 = p4.x;
	double v4 = p4.y;
	double u5 = p5.x;
	double v5 = p5.y;
	double u6 = p6.x;
	double v6 = p6.y;
	double u7 = p7.x;
	double v7 = p7.y;

	double denom = (-SQR(u1-u2)*SQR(u6-u7)*(2*SQR(u2-u5)+2*SQR(u3-u5)+4*SQR(u4-u5)-SQR(u2-u3))-SQR(u5-u7)*SQR(u2+u3-2*u4)*(SQR(u1-u6)+SQR(u2-u6)));
	double TOL = 1e-3;
	if(fabs(denom) > TOL)
	{
		//non degenerate case
		e00 = (u1 * (u1 - u2) * (u2 - u6) * (u5 - u6) * (u5 - u7) * SQR(u2 + u3 - 2 * u4)) / denom;
		e01 = (-(u2 + u3) * SQR(u1 - u2) * (u6 - u7) * (u5 - u6) * (-u5 + u4) * (-2 * u4 + u3 + u2)) / denom;
		e10 = (-u2 * SQR(-2 * u4 + u3 + u2) * (u1 - u2) * (u1 - u6) * (u5 - u6) * (u5 - u7)) / denom;
		e11 = (u4 * SQR(u1 - u2) * (u6 - u7) * (u5 - u6) * (u2 + u3 - 2 * u5) * (u2 + u3 - 2 * u4)) / denom;
		f00 = (SQR(u2 + u3 - 2 * u4) * (u2 - u6) * (u5 - u6) * (u5 - u7) * (u1 - u2)) / denom;
		f01 = (-2 * SQR(u1 - u2) * (u6 - u7) * (u5 - u6) * (u4 - u5) * (u2 + u3 - 2 * u4)) / denom;
		f10 = (-SQR(u2 + u3 - 2 * u4) * (u1 - u2) * (u1 - u6) * (u5 - u6) * (u5 - u7)) / denom;
		f11 = (SQR(u1 - u2) * (u6 - u7) * (u5 - u6) * (u2 + u3 - 2 * u5) * (u2 + u3 - 2 * u4)) / denom;
	}else
	{

	}
	IsValid();
	return 1;
}
int GeneralBilinearTransformation::SetSevenPoints(Point2D p1,Point2D p2,Point2D p3,Point2D p4,Point2D p5,Point2D p6,Point2D p7)
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;
	this->p5 = p5;
	this->p6 = p6;
	this->p7 = p7;
	ComputeGeneralBilinearTransformationFromSevenPoints();
	return 1;
}
double GeneralBilinearTransformation::GetCoefficient(int index)
{
	switch(index)
	{
	case 0:
		return e00;
	case 1:
		return e01;
	case 2:
		return e10;
	case 3:
		return e11;
	case 4:
		return f00;
	case 5:
		return f01;
	case 6:
		return f10;
	case 7:
		return f11;
	case 8:
		return g00;
	case 9:
		return g01;
	case 10:
		return g10;
	case 11:
		return g11;
	case 12:
		return h00;
	case 13:
		return h01;
	case 14:
		return h10;
	case 15:
		return h11;
	}
}
void GeneralBilinearTransformation::SetCoefficient(int index,double val)
{
	switch(index)
	{
	case 0:
		e00 = val;
		break;
	case 1:
		e01 = val;
		break;
	case 2:
		e10 = val;
		break;
	case 3:
		e11 = val;
		break;
	case 4:
		f00 = val;
		break;
	case 5:
		f01 = val;
		break;
	case 6:
		f10 = val;
		break;
	case 7:
		f11 = val;
		break;
	case 8:
		g00 = val;
		break;
	case 9:
		g01 = val;
		break;
	case 10:
		g10 = val;
		break;
	case 11:
		g11 = val;
		break;
	case 12:
		h00 = val;
		break;
	case 13:
		h01 = val;
		break;
	case 14:
		h10 = val;
		break;
	case 15:
		h11 = val;
		break;
	}
}
void GeneralBilinearTransformation::UpdateBoundingBox()
{
	//update the umax_Reparameterization & others
	umin_Reparameterization = vmin_Reparameterization = 1e10;
	umax_Reparameterization = vmax_Reparameterization = -1e10;
	int m = 10;
	int n = 10;
	double u,v;
	for(int i = 0; i <= m; i++)
		for(int j = 0; j <= n; j++)
		{
			u = umin + i * (umax - umin) / m;
			v = vmin + j * (vmax - vmin) / n;
			double s,t;
			InverseEvaluation(u,v,s,t);
			if(umin_Reparameterization > s)
				umin_Reparameterization = s;

			if(umax_Reparameterization < s)
				umax_Reparameterization = s;

			if(vmin_Reparameterization > t)
				vmin_Reparameterization = t;

			if(vmax_Reparameterization < t)
				vmax_Reparameterization = t;
		}
}
int GeneralBilinearTransformation::SetReparameterizationCoefficients(std::vector<double> &c)
{
	e00 = c[0];
	e01 = c[1];
	e10 = c[2];
	e11 = c[3];
	f00 = c[4];
	f01 = c[5];
	f10 = c[6];
	f11 = c[7];
	g00 = c[8];
	g01 = c[9];
	g10 = c[10];
	g11 = c[11];
	h00 = c[12];
	h01 = c[13];
	h10 = c[14];
	h11 = c[15];
	return 1;
}
Transformation* GeneralBilinearTransformation::copy()
{
	GeneralBilinearTransformation* pNewTrans = new GeneralBilinearTransformation();
	pNewTrans->m_ReparameterizationType = m_ReparameterizationType;
	pNewTrans->e00 = e00;
	pNewTrans->e01 = e01;
	pNewTrans->e10 = e10;
	pNewTrans->e11 = e11;
	pNewTrans->f00 = f00;
	pNewTrans->f01 = f01;
	pNewTrans->f10 = f10;
	pNewTrans->f11 = f11;
	pNewTrans->g00 = g00;
	pNewTrans->g01 = g01;
	pNewTrans->g10 = g10;
	pNewTrans->g11 = g11;
	pNewTrans->h00 = h00;
	pNewTrans->h01 = h01;
	pNewTrans->h10 = h10;
	pNewTrans->h11 = h11;
	pNewTrans->umax_Reparameterization = umax_Reparameterization;
	pNewTrans->umin_Reparameterization = umin_Reparameterization;
	pNewTrans->vmin_Reparameterization = vmin_Reparameterization;
	pNewTrans->vmax_Reparameterization = vmax_Reparameterization;
	pNewTrans->umin = umin;
	pNewTrans->umax = umax;
	pNewTrans->vmin = vmin;
	pNewTrans->vmax = vmax;
	pNewTrans->label = label;
	return (Transformation*)pNewTrans;
}
void GeneralBilinearTransformation::Save(std::ofstream &file)
{
	Transformation::Save(file);

	file << label <<"\n";
	file << e00  <<"\n";
	file << e01  <<"\n";
	file << e10  <<"\n";
	file << e11  <<"\n";
	file << f00  <<"\n";
	file << f01  <<"\n";
	file << f10  <<"\n";
	file << f11  <<"\n";
	file << g00  <<"\n";
	file << g01  <<"\n";
	file << g10  <<"\n";
	file << g11  <<"\n";
	file << h00  <<"\n";
	file << h01  <<"\n";
	file << h10  <<"\n";
	file << h11  <<"\n";
}
void GeneralBilinearTransformation::Read(std::ifstream &file)
{
	Transformation::Read(file);

	file >> label;
	file >> e00;
	file >> e01;
	file >> e10;
	file >> e11;
	file >> f00;
	file >> f01;
	file >> f10;
	file >> f11;
	file >> g00;
	file >> g01;
	file >> g10;
	file >> g11;
	file >> h00;
	file >> h01;
	file >> h10;
	file >> h11;
}
void GeneralBilinearTransformation::AddCoefficient(double s,double t,std::vector<double> &coefficient,std::vector<std::vector<double>> &coefficients)
{
	double sarray[7],tarray[7];
	 sarray[0] = pow(1 - s,6);
	 sarray[1] = 6 * pow(1 - s, 5) * s;
	 sarray[2] = 15 * pow(1 - s, 4) * s * s;
	 sarray[3] = 20 * pow(1 - s, 3) * pow(s,3);
	 sarray[4] = 15 * pow(1 - s, 2) * pow(s,4);
	 sarray[5] = 6 * pow(1 - s,1) * pow(s,5);
	 sarray[6] = pow(s,6);

	 tarray[0] = pow(1 - t, 6);
	 tarray[1] = 6 * pow(1 - t, 5) * t;
	 tarray[2] = 15 * pow(1 - t, 4) * t * t;
	 tarray[3] = 20 * pow(1 - t, 3) * pow(t,3);
	 tarray[4] = 15 * pow(1 - t, 2) * pow(t,4);
	 tarray[5] = 6 * pow(1 - t,1) * pow(t,5);
	 tarray[6] = pow(t,6);

	double denom = 0;
	for(int i = 0; i <= 6;i++)
		for(int j = 0; j <= 6; j++)
		{
			denom += sarray[i] * tarray[j] * pNewSurface->GetWeight(i,j);
		}

	coefficient.clear();
	for(int i = 0; i <= 6; i++)
		for(int j = 0; j <= 6; j++)
		{
			if(j == 0 && i == 0 || j == 6 && i == 0 || j == 0 && i == 6 || j == 6 && i == 6)
				continue;
			coefficient.push_back(sarray[j] * tarray[i] * pNewSurface->GetWeight(j,i) / denom);
		}

	coefficients.push_back(coefficient);
}
void GeneralBilinearTransformation::solveControlPoints()
{

	Vector3D p00 = pSurface->GetControlPoint(0,0);
	Vector3D p10 = pSurface->GetControlPoint(1,0);
	Vector3D p20 = pSurface->GetControlPoint(2,0);
	Vector3D p30 = pSurface->GetControlPoint(3,0);
	Vector3D p01 = pSurface->GetControlPoint(0,1);
	Vector3D p11 = pSurface->GetControlPoint(1,1);
	Vector3D p21 = pSurface->GetControlPoint(2,1);
	Vector3D p31 = pSurface->GetControlPoint(3,1);
	Vector3D p02 = pSurface->GetControlPoint(0,2);
	Vector3D p12 = pSurface->GetControlPoint(1,2);
	Vector3D p22 = pSurface->GetControlPoint(2,2);
	Vector3D p32 = pSurface->GetControlPoint(3,2);
	Vector3D p03 = pSurface->GetControlPoint(0,3);
	Vector3D p13 = pSurface->GetControlPoint(1,3);
	Vector3D p23 = pSurface->GetControlPoint(2,3);
	Vector3D p33 = pSurface->GetControlPoint(3,3);

	Vector3D q00 = (p00*pow(f00,3)*pow(h00,2)*g00 * (-3)+
		           p00*pow(f00,3)*h00*pow(g00,2) * 3-p00*pow(f00,2)*e00*pow(h00,3)*3
				   +p00*pow(f00,2)*e00*pow(g00,3)*3 + p00*f00*pow(e00,2)*pow(h00,3)*3-
				   p00*f00*pow(e00,2)*pow(g00,3)*3+3*p00*pow(e00,3)*pow(h00,2)*g00-3*p00*pow(e00,3)*h00*pow(g00,2)+3*p10*e00*pow(f00,2)*pow(h00,3)-3*p10*e00*pow(f00,2)*pow(g00,3)-6*p10*pow(e00,2)*f00*pow(h00,3)+6*p10*pow(e00,2)*f00*pow(g00,3)-9*p10*pow(e00,3)*pow(h00,2)*g00+9*p10*pow(e00,3)*h00*pow(g00,2)+3*p20*pow(e00,2)*f00*pow(h00,3)-3*p20*pow(e00,2)*f00*pow(g00,3)+9*p20*pow(e00,3)*pow(h00,2)*g00-9*p20*pow(e00,3)*h00*pow(g00,2)-3*p30*pow(e00,3)*pow(h00,2)*g00+3*p30*pow(e00,3)*h00*pow(g00,2)+3*p01*g00*pow(f00,3)*pow(h00,2)-6*p01*pow(g00,2)*pow(f00,3)*h00-9*p01*pow(g00,3)*pow(f00,2)*e00+9*p01*pow(g00,3)*f00*pow(e00,2)-3*p01*g00*pow(e00,3)*pow(h00,2)+6*p01*pow(g00,2)*pow(e00,3)*h00+9*p11*e00*pow(g00,3)*pow(f00,2)-18*p11*pow(e00,2)*pow(g00,3)*f00+9*p11*pow(e00,3)*g00*pow(h00,2)-18*p11*pow(e00,3)*pow(g00,2)*h00+9*p21*pow(e00,2)*pow(g00,3)*f00-9*p21*pow(e00,3)*g00*pow(h00,2)+18*p21*pow(e00,3)*pow(g00,2)*h00+3*p31*pow(e00,3)*g00*pow(h00,2)-6*p31*pow(e00,3)*pow(g00,2)*h00+3*p02*pow(g00,2)*pow(f00,3)*h00+9*p02*pow(g00,3)*pow(f00,2)*e00-9*p02*pow(g00,3)*f00*pow(e00,2)-3*p02*pow(g00,2)*pow(e00,3)*h00-9*p12*e00*pow(g00,3)*pow(f00,2)+18*p12*pow(e00,2)*pow(g00,3)*f00+9*p12*pow(e00,3)*pow(g00,2)*h00-9*p22*pow(e00,2)*pow(g00,3)*f00-9*p22*pow(e00,3)*pow(g00,2)*h00+3*p32*pow(e00,3)*pow(g00,2)*h00-3*p03*pow(g00,3)*pow(f00,2)*e00+3*p03*pow(g00,3)*f00*pow(e00,2)+3*p13*e00*pow(g00,3)*pow(f00,2)-6*p13*pow(e00,2)*pow(g00,3)*f00+3*p23*pow(e00,2)*pow(g00,3)*f00+p00*pow(f00,3)*pow(h00,3)-p00*pow(f00,3)*pow(g00,3)-p00*pow(e00,3)*pow(h00,3)+p00*pow(e00,3)*pow(g00,3)+3*p10*pow(e00,3)*pow(h00,3)-3*p10*pow(e00,3)*pow(g00,3)-3*p20*pow(e00,3)*pow(h00,3)+3*p20*pow(e00,3)*pow(g00,3)+p30*pow(e00,3)*pow(h00,3)-p30*pow(e00,3)*pow(g00,3)+3*p01*pow(g00,3)*pow(f00,3)-3*p01*pow(g00,3)*pow(e00,3)+9*p11*pow(e00,3)*pow(g00,3)-9*p21*pow(e00,3)*pow(g00,3)+3*p31*pow(e00,3)*pow(g00,3)-3*p02*pow(g00,3)*pow(f00,3)+3*p02*pow(g00,3)*pow(e00,3)-9*p12*pow(e00,3)*pow(g00,3)+9*p22*pow(e00,3)*pow(g00,3)-3*p32*pow(e00,3)*pow(g00,3)+p03*pow(g00,3)*pow(f00,3)-p03*pow(g00,3)*pow(e00,3)+3*p13*pow(e00,3)*pow(g00,3)-3*p23*pow(e00,3)*pow(g00,3)+p33*pow(e00,3)*pow(g00,3)+9*p00*pow(f00,2)*e00*pow(h00,2)*g00-9*p00*pow(f00,2)*e00*h00*pow(g00,2)-9*p00*f00*pow(e00,2)*pow(h00,2)*g00+9*p00*f00*pow(e00,2)*h00*pow(g00,2)-9*p10*e00*pow(f00,2)*pow(h00,2)*g00+9*p10*e00*pow(f00,2)*h00*pow(g00,2)+18*p10*pow(e00,2)*f00*pow(h00,2)*g00-18*p10*pow(e00,2)*f00*h00*pow(g00,2)-9*p20*pow(e00,2)*f00*pow(h00,2)*g00+9*p20*pow(e00,2)*f00*h00*pow(g00,2)-9*p01*g00*pow(f00,2)*e00*pow(h00,2)+18*p01*pow(g00,2)*pow(f00,2)*e00*h00+9*p01*g00*f00*pow(e00,2)*pow(h00,2)-18*p01*pow(g00,2)*f00*pow(e00,2)*h00+9*p11*e00*g00*pow(f00,2)*pow(h00,2)-18*p11*e00*pow(g00,2)*pow(f00,2)*h00-18*p11*pow(e00,2)*g00*f00*pow(h00,2)+36*p11*pow(e00,2)*pow(g00,2)*f00*h00+9*p21*pow(e00,2)*g00*f00*pow(h00,2)-18*p21*pow(e00,2)*pow(g00,2)*f00*h00-9*p02*pow(g00,2)*pow(f00,2)*e00*h00+9*p02*pow(g00,2)*f00*pow(e00,2)*h00+9*p12*e00*pow(g00,2)*pow(f00,2)*h00-18*p12*pow(e00,2)*pow(g00,2)*f00*h00+9*p22*pow(e00,2)*pow(g00,2)*f00*h00)/(pow(f00,3)*pow(h00,3));

	Vector3D q60 = (p00*pow(f10,3)*pow(h10,3)-p00*pow(f10,3)*pow(g10,3)-p00*pow(e10,3)*pow(h10,3)+p00*pow(e10,3)*pow(g10,3)+3*p10*pow(e10,3)*pow(h10,3)-3*p10*pow(e10,3)*pow(g10,3)-3*p20*pow(e10,3)*pow(h10,3)+3*p20*pow(e10,3)*pow(g10,3)+p30*pow(e10,3)*pow(h10,3)-p30*pow(e10,3)*pow(g10,3)+3*p01*pow(g10,3)*pow(f10,3)-3*p01*pow(g10,3)*pow(e10,3)+9*p11*pow(e10,3)*pow(g10,3)-9*p21*pow(e10,3)*pow(g10,3)+3*p31*pow(e10,3)*pow(g10,3)-3*p02*pow(g10,3)*pow(f10,3)+3*p02*pow(g10,3)*pow(e10,3)-9*p12*pow(e10,3)*pow(g10,3)+9*p22*pow(e10,3)*pow(g10,3)-3*p32*pow(e10,3)*pow(g10,3)+p03*pow(g10,3)*pow(f10,3)-p03*pow(g10,3)*pow(e10,3)+3*p13*pow(e10,3)*pow(g10,3)-3*p23*pow(e10,3)*pow(g10,3)+p33*pow(e10,3)*pow(g10,3)+9*p00*pow(f10,2)*e10*pow(h10,2)*g10-9*p00*pow(f10,2)*e10*h10*pow(g10,2)-9*p00*f10*pow(e10,2)*pow(h10,2)*g10+9*p00*f10*pow(e10,2)*h10*pow(g10,2)-9*p10*e10*pow(f10,2)*pow(h10,2)*g10+9*p10*e10*pow(f10,2)*h10*pow(g10,2)+18*p10*pow(e10,2)*f10*pow(h10,2)*g10-18*p10*pow(e10,2)*f10*h10*pow(g10,2)-9*p20*pow(e10,2)*f10*pow(h10,2)*g10+9*p20*pow(e10,2)*f10*h10*pow(g10,2)-9*p01*g10*pow(f10,2)*e10*pow(h10,2)+18*p01*pow(g10,2)*pow(f10,2)*e10*h10+9*p01*g10*f10*pow(e10,2)*pow(h10,2)-18*p01*pow(g10,2)*f10*pow(e10,2)*h10+9*p11*e10*g10*pow(f10,2)*pow(h10,2)-18*p11*e10*pow(g10,2)*pow(f10,2)*h10-18*p11*pow(e10,2)*g10*f10*pow(h10,2)+36*p11*pow(e10,2)*pow(g10,2)*f10*h10+9*p21*pow(e10,2)*g10*f10*pow(h10,2)-18*p21*pow(e10,2)*pow(g10,2)*f10*h10-9*p02*pow(g10,2)*pow(f10,2)*e10*h10+9*p02*pow(g10,2)*f10*pow(e10,2)*h10+9*p12*e10*pow(g10,2)*pow(f10,2)*h10-18*p12*pow(e10,2)*pow(g10,2)*f10*h10+9*p22*pow(e10,2)*pow(g10,2)*f10*h10-9*p22*pow(e10,2)*pow(g10,3)*f10-9*p22*pow(e10,3)*pow(g10,2)*h10+3*p32*pow(e10,3)*pow(g10,2)*h10-3*p03*pow(g10,3)*pow(f10,2)*e10+3*p03*pow(g10,3)*f10*pow(e10,2)+3*p13*e10*pow(g10,3)*pow(f10,2)-6*p13*pow(e10,2)*pow(g10,3)*f10+3*p23*pow(e10,2)*pow(g10,3)*f10-3*p00*pow(f10,3)*pow(h10,2)*g10+3*p00*pow(f10,3)*h10*pow(g10,2)-3*p00*pow(f10,2)*e10*pow(h10,3)+3*p00*pow(f10,2)*e10*pow(g10,3)+3*p00*f10*pow(e10,2)*pow(h10,3)-3*p00*f10*pow(e10,2)*pow(g10,3)+3*p00*pow(e10,3)*pow(h10,2)*g10-3*p00*pow(e10,3)*h10*pow(g10,2)+3*p10*e10*pow(f10,2)*pow(h10,3)-3*p10*e10*pow(f10,2)*pow(g10,3)-6*p10*pow(e10,2)*f10*pow(h10,3)+6*p10*pow(e10,2)*f10*pow(g10,3)-9*p10*pow(e10,3)*pow(h10,2)*g10+9*p10*pow(e10,3)*h10*pow(g10,2)+3*p20*pow(e10,2)*f10*pow(h10,3)-3*p20*pow(e10,2)*f10*pow(g10,3)+9*p20*pow(e10,3)*pow(h10,2)*g10-9*p20*pow(e10,3)*h10*pow(g10,2)-3*p30*pow(e10,3)*pow(h10,2)*g10+3*p30*pow(e10,3)*h10*pow(g10,2)+3*p01*g10*pow(f10,3)*pow(h10,2)-6*p01*pow(g10,2)*pow(f10,3)*h10-9*p01*pow(g10,3)*pow(f10,2)*e10+9*p01*pow(g10,3)*f10*pow(e10,2)-3*p01*g10*pow(e10,3)*pow(h10,2)+6*p01*pow(g10,2)*pow(e10,3)*h10+9*p11*e10*pow(g10,3)*pow(f10,2)-18*p11*pow(e10,2)*pow(g10,3)*f10+9*p11*pow(e10,3)*g10*pow(h10,2)-18*p11*pow(e10,3)*pow(g10,2)*h10+9*p21*pow(e10,2)*pow(g10,3)*f10-9*p21*pow(e10,3)*g10*pow(h10,2)+18*p21*pow(e10,3)*pow(g10,2)*h10+3*p31*pow(e10,3)*g10*pow(h10,2)-6*p31*pow(e10,3)*pow(g10,2)*h10+3*p02*pow(g10,2)*pow(f10,3)*h10+9*p02*pow(g10,3)*pow(f10,2)*e10-9*p02*pow(g10,3)*f10*pow(e10,2)-3*p02*pow(g10,2)*pow(e10,3)*h10-9*p12*e10*pow(g10,3)*pow(f10,2)+18*p12*pow(e10,2)*pow(g10,3)*f10+9*p12*pow(e10,3)*pow(g10,2)*h10)/(pow(f10,3)*pow(h10,3));

	Vector3D q66 = (p00*pow(f11,3)*pow(h11,3)-p00*pow(f11,3)*pow(g11,3)-p00*pow(e11,3)*pow(h11,3)+p00*pow(e11,3)*pow(g11,3)+3*p10*pow(e11,3)*pow(h11,3)-3*p10*pow(e11,3)*pow(g11,3)-3*p20*pow(e11,3)*pow(h11,3)+3*p20*pow(e11,3)*pow(g11,3)+p30*pow(e11,3)*pow(h11,3)-p30*pow(e11,3)*pow(g11,3)+3*p01*pow(g11,3)*pow(f11,3)-3*p01*pow(g11,3)*pow(e11,3)+9*p11*pow(e11,3)*pow(g11,3)-9*p21*pow(e11,3)*pow(g11,3)+3*p31*pow(e11,3)*pow(g11,3)-3*p02*pow(g11,3)*pow(f11,3)+3*p02*pow(g11,3)*pow(e11,3)-9*p12*pow(e11,3)*pow(g11,3)+9*p22*pow(e11,3)*pow(g11,3)-3*p32*pow(e11,3)*pow(g11,3)+p03*pow(g11,3)*pow(f11,3)-p03*pow(g11,3)*pow(e11,3)+3*p13*pow(e11,3)*pow(g11,3)-3*p23*pow(e11,3)*pow(g11,3)+p33*pow(e11,3)*pow(g11,3)+9*p00*pow(f11,2)*e11*pow(h11,2)*g11-9*p00*pow(f11,2)*e11*h11*pow(g11,2)-9*p00*f11*pow(e11,2)*pow(h11,2)*g11+9*p00*f11*pow(e11,2)*h11*pow(g11,2)-9*p10*e11*pow(f11,2)*pow(h11,2)*g11+9*p10*e11*pow(f11,2)*h11*pow(g11,2)+18*p10*pow(e11,2)*f11*pow(h11,2)*g11-18*p10*pow(e11,2)*f11*h11*pow(g11,2)-9*p20*pow(e11,2)*f11*pow(h11,2)*g11+9*p20*pow(e11,2)*f11*h11*pow(g11,2)-9*p01*g11*pow(f11,2)*e11*pow(h11,2)+18*p01*pow(g11,2)*pow(f11,2)*e11*h11+9*p01*g11*f11*pow(e11,2)*pow(h11,2)-18*p01*pow(g11,2)*f11*pow(e11,2)*h11+9*p11*e11*g11*pow(f11,2)*pow(h11,2)-18*p11*e11*pow(g11,2)*pow(f11,2)*h11-18*p11*pow(e11,2)*g11*f11*pow(h11,2)+36*p11*pow(e11,2)*pow(g11,2)*f11*h11+9*p21*pow(e11,2)*g11*f11*pow(h11,2)-18*p21*pow(e11,2)*pow(g11,2)*f11*h11-9*p02*pow(g11,2)*pow(f11,2)*e11*h11+9*p02*pow(g11,2)*f11*pow(e11,2)*h11+9*p12*e11*pow(g11,2)*pow(f11,2)*h11-18*p12*pow(e11,2)*pow(g11,2)*f11*h11+9*p22*pow(e11,2)*pow(g11,2)*f11*h11+9*p01*pow(g11,3)*f11*pow(e11,2)-3*p01*g11*pow(e11,3)*pow(h11,2)+6*p01*pow(g11,2)*pow(e11,3)*h11+9*p11*e11*pow(g11,3)*pow(f11,2)-18*p11*pow(e11,2)*pow(g11,3)*f11+9*p11*pow(e11,3)*g11*pow(h11,2)-18*p11*pow(e11,3)*pow(g11,2)*h11+9*p21*pow(e11,2)*pow(g11,3)*f11-9*p21*pow(e11,3)*g11*pow(h11,2)+18*p21*pow(e11,3)*pow(g11,2)*h11+3*p31*pow(e11,3)*g11*pow(h11,2)-6*p31*pow(e11,3)*pow(g11,2)*h11+3*p02*pow(g11,2)*pow(f11,3)*h11+9*p02*pow(g11,3)*pow(f11,2)*e11-9*p02*pow(g11,3)*f11*pow(e11,2)-3*p02*pow(g11,2)*pow(e11,3)*h11-9*p12*e11*pow(g11,3)*pow(f11,2)+18*p12*pow(e11,2)*pow(g11,3)*f11+9*p12*pow(e11,3)*pow(g11,2)*h11-9*p22*pow(e11,2)*pow(g11,3)*f11-9*p22*pow(e11,3)*pow(g11,2)*h11+3*p32*pow(e11,3)*pow(g11,2)*h11-3*p03*pow(g11,3)*pow(f11,2)*e11+3*p03*pow(g11,3)*f11*pow(e11,2)+3*p13*e11*pow(g11,3)*pow(f11,2)-6*p13*pow(e11,2)*pow(g11,3)*f11+3*p23*pow(e11,2)*pow(g11,3)*f11-3*p00*pow(f11,3)*pow(h11,2)*g11+3*p00*pow(f11,3)*h11*pow(g11,2)-3*p00*pow(f11,2)*e11*pow(h11,3)+3*p00*pow(f11,2)*e11*pow(g11,3)+3*p00*f11*pow(e11,2)*pow(h11,3)-3*p00*f11*pow(e11,2)*pow(g11,3)+3*p00*pow(e11,3)*pow(h11,2)*g11-3*p00*pow(e11,3)*h11*pow(g11,2)+3*p10*e11*pow(f11,2)*pow(h11,3)-3*p10*e11*pow(f11,2)*pow(g11,3)-6*p10*pow(e11,2)*f11*pow(h11,3)+6*p10*pow(e11,2)*f11*pow(g11,3)-9*p10*pow(e11,3)*pow(h11,2)*g11+9*p10*pow(e11,3)*h11*pow(g11,2)+3*p20*pow(e11,2)*f11*pow(h11,3)-3*p20*pow(e11,2)*f11*pow(g11,3)+9*p20*pow(e11,3)*pow(h11,2)*g11-9*p20*pow(e11,3)*h11*pow(g11,2)-3*p30*pow(e11,3)*pow(h11,2)*g11+3*p30*pow(e11,3)*h11*pow(g11,2)+3*p01*g11*pow(f11,3)*pow(h11,2)-6*p01*pow(g11,2)*pow(f11,3)*h11-9*p01*pow(g11,3)*pow(f11,2)*e11)/(pow(f11,3)*pow(h11,3));

	Vector3D q06 = (3*p10*pow(e01,3)*pow(h01,3)-3*p10*pow(e01,3)*pow(g01,3)-3*p20*pow(e01,3)*pow(h01,3)+3*p20*pow(e01,3)*pow(g01,3)+p30*pow(e01,3)*pow(h01,3)-p30*pow(e01,3)*pow(g01,3)+3*p01*pow(g01,3)*pow(f01,3)-3*p01*pow(g01,3)*pow(e01,3)+9*p11*pow(e01,3)*pow(g01,3)-9*p21*pow(e01,3)*pow(g01,3)+3*p31*pow(e01,3)*pow(g01,3)-3*p02*pow(g01,3)*pow(f01,3)+3*p02*pow(g01,3)*pow(e01,3)-9*p12*pow(e01,3)*pow(g01,3)+9*p22*pow(e01,3)*pow(g01,3)-3*p32*pow(e01,3)*pow(g01,3)+p03*pow(g01,3)*pow(f01,3)-p03*pow(g01,3)*pow(e01,3)+3*p13*pow(e01,3)*pow(g01,3)-3*p23*pow(e01,3)*pow(g01,3)+p00*pow(f01,3)*pow(h01,3)-p00*pow(f01,3)*pow(g01,3)-p00*pow(e01,3)*pow(h01,3)+p00*pow(e01,3)*pow(g01,3)+p33*pow(e01,3)*pow(g01,3)+9*p00*pow(f01,2)*e01*pow(h01,2)*g01-9*p00*pow(f01,2)*e01*h01*pow(g01,2)-9*p00*f01*pow(e01,2)*pow(h01,2)*g01+9*p00*f01*pow(e01,2)*h01*pow(g01,2)-9*p10*e01*pow(f01,2)*pow(h01,2)*g01+9*p10*e01*pow(f01,2)*h01*pow(g01,2)+18*p10*pow(e01,2)*f01*pow(h01,2)*g01-18*p10*pow(e01,2)*f01*h01*pow(g01,2)-9*p20*pow(e01,2)*f01*pow(h01,2)*g01+9*p20*pow(e01,2)*f01*h01*pow(g01,2)-9*p01*g01*pow(f01,2)*e01*pow(h01,2)+18*p01*pow(g01,2)*pow(f01,2)*e01*h01+9*p01*g01*f01*pow(e01,2)*pow(h01,2)-18*p01*pow(g01,2)*f01*pow(e01,2)*h01+9*p11*e01*g01*pow(f01,2)*pow(h01,2)-18*p11*e01*pow(g01,2)*pow(f01,2)*h01-18*p11*pow(e01,2)*g01*f01*pow(h01,2)+36*p11*pow(e01,2)*pow(g01,2)*f01*h01+9*p21*pow(e01,2)*g01*f01*pow(h01,2)-18*p21*pow(e01,2)*pow(g01,2)*f01*h01-9*p02*pow(g01,2)*pow(f01,2)*e01*h01+9*p02*pow(g01,2)*f01*pow(e01,2)*h01+9*p12*e01*pow(g01,2)*pow(f01,2)*h01-18*p12*pow(e01,2)*pow(g01,2)*f01*h01+9*p22*pow(e01,2)*pow(g01,2)*f01*h01-3*p00*pow(f01,3)*pow(h01,2)*g01+3*p00*pow(f01,3)*h01*pow(g01,2)-3*p00*pow(f01,2)*e01*pow(h01,3)+3*p00*pow(f01,2)*e01*pow(g01,3)+3*p00*f01*pow(e01,2)*pow(h01,3)-3*p00*f01*pow(e01,2)*pow(g01,3)+3*p00*pow(e01,3)*pow(h01,2)*g01-3*p00*pow(e01,3)*h01*pow(g01,2)+3*p10*e01*pow(f01,2)*pow(h01,3)-3*p10*e01*pow(f01,2)*pow(g01,3)-6*p10*pow(e01,2)*f01*pow(h01,3)+6*p10*pow(e01,2)*f01*pow(g01,3)-9*p10*pow(e01,3)*pow(h01,2)*g01+9*p10*pow(e01,3)*h01*pow(g01,2)+3*p20*pow(e01,2)*f01*pow(h01,3)-3*p20*pow(e01,2)*f01*pow(g01,3)+9*p20*pow(e01,3)*pow(h01,2)*g01-9*p20*pow(e01,3)*h01*pow(g01,2)-3*p30*pow(e01,3)*pow(h01,2)*g01+3*p30*pow(e01,3)*h01*pow(g01,2)+3*p01*g01*pow(f01,3)*pow(h01,2)-6*p01*pow(g01,2)*pow(f01,3)*h01-9*p01*pow(g01,3)*pow(f01,2)*e01+9*p01*pow(g01,3)*f01*pow(e01,2)-3*p01*g01*pow(e01,3)*pow(h01,2)+6*p01*pow(g01,2)*pow(e01,3)*h01+9*p11*e01*pow(g01,3)*pow(f01,2)-18*p11*pow(e01,2)*pow(g01,3)*f01+9*p11*pow(e01,3)*g01*pow(h01,2)-18*p11*pow(e01,3)*pow(g01,2)*h01+9*p21*pow(e01,2)*pow(g01,3)*f01-9*p21*pow(e01,3)*g01*pow(h01,2)+18*p21*pow(e01,3)*pow(g01,2)*h01+3*p31*pow(e01,3)*g01*pow(h01,2)-6*p31*pow(e01,3)*pow(g01,2)*h01+3*p02*pow(g01,2)*pow(f01,3)*h01+9*p02*pow(g01,3)*pow(f01,2)*e01-9*p02*pow(g01,3)*f01*pow(e01,2)-3*p02*pow(g01,2)*pow(e01,3)*h01-9*p12*e01*pow(g01,3)*pow(f01,2)+18*p12*pow(e01,2)*pow(g01,3)*f01+9*p12*pow(e01,3)*pow(g01,2)*h01-9*p22*pow(e01,2)*pow(g01,3)*f01-9*p22*pow(e01,3)*pow(g01,2)*h01+3*p32*pow(e01,3)*pow(g01,2)*h01-3*p03*pow(g01,3)*pow(f01,2)*e01+3*p03*pow(g01,3)*f01*pow(e01,2)+3*p13*e01*pow(g01,3)*pow(f01,2)-6*p13*pow(e01,2)*pow(g01,3)*f01+3*p23*pow(e01,2)*pow(g01,3)*f01)/(pow(f01,3)*pow(h01,3));
	std::vector<double> coefficient;
	std::vector<std::vector<double>> coefficients;

	for(int i = 0; i <= 6; i++)
		for(int j = 0; j <= 6; j++)
		{
			if(j == 0 && i == 0 || j == 6 && i == 0 || j == 0 && i == 6 || j == 6 && i == 6)
				continue;

			AddCoefficient((j + 0.0)/6,(0.0 + i)/6,coefficient,coefficients);
		}
	



	double u,v;
	std::vector<double> bx,by,bz;
	Vector3D vec;
	for(int i = 0; i <= 6; i++)
		for(int j = 0; j <= 6; j++)
		{
			if(j == 0 && i == 0 || j == 6 && i == 0 || j == 0 && i == 6 || j == 6 && i == 6)
				continue;

			Evaluation((j + 0.0)/6,(0.0 + i)/6,u,v);
			double s = (j + 0.0)/6;
			double t = (0.0 + i)/6;
			double sarray[7],tarray[7];
			sarray[0] = pow(1 - s,6);
			sarray[1] = 6 * pow(1 - s, 5) * s;
			sarray[2] = 15 * pow(1 - s, 4) * s * s;
			sarray[3] = 20 * pow(1 - s, 3) * pow(s,3);
			sarray[4] = 15 * pow(1 - s, 2) * pow(s,4);
			sarray[5] = 6 * pow(1 - s,1) * pow(s,5);
			sarray[6] = pow(s,6);

			tarray[0] = pow(1 - t, 6);
			tarray[1] = 6 * pow(1 - t, 5) * t;
			tarray[2] = 15 * pow(1 - t, 4) * t * t;
			tarray[3] = 20 * pow(1 - t, 3) * pow(t,3);
			tarray[4] = 15 * pow(1 - t, 2) * pow(t,4);
			tarray[5] = 6 * pow(1 - t,1) * pow(t,5);
			tarray[6] = pow(t,6);

			double denom = 0;
			for(int i = 0; i <= 6;i++)
				for(int j = 0; j <= 6; j++)
				{
					denom += sarray[i] * tarray[j] * pNewSurface->GetWeight(i,j);
				}

			vec = pSurface->ComputPosition(s,t) - ZeroP3D;
			vec = vec - q00 * sarray[0] * tarray[0] * pNewSurface->GetWeight(0,0) / denom;
			vec = vec - q60 * sarray[6] * tarray[0] * pNewSurface->GetWeight(6,0) / denom;
			vec = vec - q06 * sarray[0] * tarray[6] * pNewSurface->GetWeight(0,6) / denom;
			vec = vec - q66 * sarray[6] * tarray[6] * pNewSurface->GetWeight(6,6) / denom;
			bx.push_back(vec.x);
			by.push_back(vec.y);
			bz.push_back(vec.z);
		}
	
	std::vector<double> xarray,yarray,zarray;
	p_FormView3->SolveLinearSystem(coefficients,bx,xarray);
	p_FormView3->SolveLinearSystem(coefficients,by,yarray);
	p_FormView3->SolveLinearSystem(coefficients,bz,zarray);

	pNewSurface->SetControlPoint(0,0,q00);
	pNewSurface->SetControlPoint(6,0,q60);
	pNewSurface->SetControlPoint(0,6,q06);
	pNewSurface->SetControlPoint(6,6,q66);

	int index = 0;
	for(int i = 0; i <= 6; i++)
		for(int j = 0; j <= 6; j++)
		{
			if(j == 0 && i == 0 || j == 6 && i == 0 || j == 0 && i == 6 || j == 6 && i == 6)
				continue;

			Vector3D point;
			point.x = xarray[index];
			point.y = yarray[index];
			point.z = zarray[index];
			index++;
			pNewSurface->SetControlPoint(j,i,point);
		}

	/*double w00 = GetWeight_33(0,0);
	double w10 = GetWeight_33(1,0);
	double w20 = GetWeight_33(2,0);
	double w30 = GetWeight_33(3,0);
	double w01 = GetWeight_33(0,1);
	double w11 = GetWeight_33(1,1);
	double w21 = GetWeight_33(2,1);
	double w31 = GetWeight_33(3,1);
	double w02 = GetWeight_33(0,2);
	double w12 = GetWeight_33(1,2);
	double w22 = GetWeight_33(2,2);
	double w32 = GetWeight_33(3,2);
	double w03 = GetWeight_33(0,3);
	double w13 = GetWeight_33(1,3);
	double w23 = GetWeight_33(2,3);
	double w33 = GetWeight_33(3,3);

	double w = w00 + w10 + w20 + w30;
	w += w01 + w11 + w21 + w31;
	w += w02 + w12 + w22 + w32;
	w += w03 + w13 + w23 + w33;

	w00 = w00 / w;
	w10 = w10 / w;
	w20 = w20 / w;
	w30 = w30 / w;

	w01 = w01 / w;
	w11 = w11 / w;
	w21 = w21 / w;
	w31 = w31 / w;

	w02 = w02 / w;
	w12 = w12 / w;
	w22 = w22 / w;
	w32 = w32 / w;

	w03 = w03 / w;
	w13 = w13 / w;
	w23 = w23 / w;
	w33 = w33 / w;
	std::vector<double> coefficient;
	std::vector<std::vector<double>> coefficients;

	AddCoefficient(0,0,coefficient,coefficients,w00,w10,w20,w30,w01,w11,w21,w31,w02,w12,w22,w32,w03,w13,w23,w33);
	AddCoefficient(1,0,coefficient,coefficients,w00,w10,w20,w30,w01,w11,w21,w31,w02,w12,w22,w32,w03,w13,w23,w33);
	AddCoefficient(0,1,coefficient,coefficients,w00,w10,w20,w30,w01,w11,w21,w31,w02,w12,w22,w32,w03,w13,w23,w33);
	AddCoefficient(1,1,coefficient,coefficients,w00,w10,w20,w30,w01,w11,w21,w31,w02,w12,w22,w32,w03,w13,w23,w33);
	AddCoefficient(0.5,0.5,coefficient,coefficients,w00,w10,w20,w30,w01,w11,w21,w31,w02,w12,w22,w32,w03,w13,w23,w33);
	AddCoefficient(1.0/3,0.5,coefficient,coefficients,w00,w10,w20,w30,w01,w11,w21,w31,w02,w12,w22,w32,w03,w13,w23,w33);
	AddCoefficient(0.5,1.0/3,coefficient,coefficients,w00,w10,w20,w30,w01,w11,w21,w31,w02,w12,w22,w32,w03,w13,w23,w33);
	AddCoefficient(1.0/3,1.0/3,coefficient,coefficients,w00,w10,w20,w30,w01,w11,w21,w31,w02,w12,w22,w32,w03,w13,w23,w33);

	double u,v;
	std::vector<double> bx,by,bz;
	Vector3D v;
	Evaluation(0,0,u,v);
	v = pSurface->ComputPosition(u,v);
	bx.push_back(v.x);
	by.push_back(v.y);
	bz.push_back(v.z);

	Evaluation(1,0,u,v);
	v = pSurface->ComputPosition(u,v);
	bx.push_back(v.x);
	by.push_back(v.y);
	bz.push_back(v.z);	

	Evaluation(0,1,u,v);
	v = pSurface->ComputPosition(u,v);
	bx.push_back(v.x);
	by.push_back(v.y);
	bz.push_back(v.z);	

	Evaluation(1,1,u,v);
	v = pSurface->ComputPosition(u,v);
	bx.push_back(v.x);
	by.push_back(v.y);
	bz.push_back(v.z);	

	Evaluation(0.5,0.5,u,v);
	v = pSurface->ComputPosition(u,v);
	bx.push_back(v.x);
	by.push_back(v.y);
	bz.push_back(v.z);	

	Evaluation(1.0/3,0.5,u,v);
	v = pSurface->ComputPosition(u,v);
	bx.push_back(v.x);
	by.push_back(v.y);
	bz.push_back(v.z);	

	Evaluation(0.5,1.0/3,u,v);
	v = pSurface->ComputPosition(u,v);
	bx.push_back(v.x);
	by.push_back(v.y);
	bz.push_back(v.z);	

	Evaluation(1.0/3,1.0/3,u,v);
	v = pSurface->ComputPosition(u,v);
	bx.push_back(v.x);
	by.push_back(v.y);
	bz.push_back(v.z);

	std::vector<double> xarray,yarray,zarray;
	p_FormView3->SolveLinearSystem(coefficients,bx,xarray);
	p_FormView3->SolveLinearSystem(coefficients,by,yarray);
	p_FormView3->SolveLinearSystem(coefficients,bz,zarray);

	pSurface*/
}
void GeneralBilinearTransformation::AddWeightCoefficient(double s,double t,std::vector<std::vector<double>>& coefficients,
														 std::vector<double> &b)
{
	double s0 = pow(1 - s,6);
	double s1 = 6 * pow(1 - s, 5) * s;
	double s2 = 15 * pow(1 - s, 4) * s * s;
	double s3 = 20 * pow(1 - s, 3) * pow(s,3);
	double s4 = 15 * pow(1 - s, 2) * pow(s,4);
	double s5 = 6 * pow(1 - s,1) * pow(s,5);
	double s6 = pow(s,6);

	double t0 = pow(1 - t, 6);
	double t1 = 6 * pow(1 - t, 5) * t;
	double t2 = 15 * pow(1 - t, 4) * t * t;
	double t3 = 20 * pow(1 - t, 3) * pow(t,3);
	double t4 = 15 * pow(1 - t, 2) * pow(t,4);
	double t5 = 6 * pow(1 - t,1) * pow(t,5);
	double t6 = pow(t,6);

	double w00 = pow(f00,3) * pow(h00,3);
	double w60 = pow(f10,3) * pow(h10,3);
	double w06 = pow(f01,3) * pow(h01,3);
	double w66 = pow(f11,3) * pow(h11,3);

	std::vector<double> coefficient;
	coefficient.clear();

	//coefficient.push_back(s0 * t0 * w00);
	coefficient.push_back(s1 * t0);
	coefficient.push_back(s2 * t0);
	coefficient.push_back(s3 * t0);
	coefficient.push_back(s4 * t0);
	coefficient.push_back(s5 * t0);
	//coefficient.push_back(s6 * t0);

	coefficient.push_back(s0 * t1);
	coefficient.push_back(s1 * t1);
	coefficient.push_back(s2 * t1);
	coefficient.push_back(s3 * t1);
	coefficient.push_back(s4 * t1);
	coefficient.push_back(s5 * t1);
	coefficient.push_back(s6 * t1);

	coefficient.push_back(s0 * t2);
	coefficient.push_back(s1 * t2);
	coefficient.push_back(s2 * t2);
	coefficient.push_back(s3 * t2);
	coefficient.push_back(s4 * t2);
	coefficient.push_back(s5 * t2);
	coefficient.push_back(s6 * t2);

	coefficient.push_back(s0 * t3);
	coefficient.push_back(s1 * t3);
	coefficient.push_back(s2 * t3);
	coefficient.push_back(s3 * t3);
	coefficient.push_back(s4 * t3);
	coefficient.push_back(s5 * t3);
	coefficient.push_back(s6 * t3);

	coefficient.push_back(s0 * t4);
	coefficient.push_back(s1 * t4);
	coefficient.push_back(s2 * t4);
	coefficient.push_back(s3 * t4);
	coefficient.push_back(s4 * t4);
	coefficient.push_back(s5 * t4);
	coefficient.push_back(s6 * t4);

	coefficient.push_back(s0 * t5);
	coefficient.push_back(s1 * t5);
	coefficient.push_back(s2 * t5);
	coefficient.push_back(s3 * t5);
	coefficient.push_back(s4 * t5);
	coefficient.push_back(s5 * t5);
	coefficient.push_back(s6 * t5);

	//coefficient.push_back(s0 * t6);
	coefficient.push_back(s1 * t6);
	coefficient.push_back(s2 * t6);
	coefficient.push_back(s3 * t6);
	coefficient.push_back(s4 * t6);
	coefficient.push_back(s5 * t6);
	//coefficient.push_back(s6 * t6);

	coefficients.push_back(coefficient);

	double weight = pow(f00-f00*t-f00*s+f00*s*t+f01*t-f01*t*s+f10*s-f10*s*t+f11*s*t,3)*
		            pow(h00-h00*t-h00*s+h00*s*t+h01*t-h01*t*s+h10*s-h10*s*t+h11*s*t,3);
	b.push_back(weight - pow(1 - s, 6) * pow(1 - t, 6) * w00  
		- pow(1 - s, 6) * pow(t, 6) * w06 
		- pow(s, 6) * pow(1 - t, 6) * w60 
		-  pow(s, 6) * pow(t, 6) * w66);
}
void GeneralBilinearTransformation::GetWeight_33()
{
	if(!pNewSurface)
	{
		C_NurbsSurf* pSurf = new C_NurbsSurf(7,7,7,7);
		for(int i = 0; i < 7; i++)
		{
			pSurf->SetUKnot(i,0);
			pSurf->SetUKnot(i + 7,1);
			pSurf->SetVKnot(i,0);
			pSurf->SetVKnot(i + 7,1);
		}
		pNewSurface = pSurf->GetCNurbsSurface();
	}
	double s,t;
	std::vector<std::vector<double>> coefficients;
	std::vector<double> b;

	double w00 = pow(f00,3) * pow(h00,3);
	double w60 = pow(f10,3) * pow(h10,3);
	double w06 = pow(f01,3) * pow(h01,3);
	double w66 = pow(f11,3) * pow(h11,3);

	for(int i = 0; i <= 6; i++)
		for(int j = 0; j <= 6; j++)
		{
			if(j == 0 && i == 0 || j == 6 && i == 0 || j == 0 && i == 6 || j == 6 && i == 6)
				continue;

			AddWeightCoefficient( (j + 0.0)/6,(0.0 + i)/6,coefficients,b);
		}

	std::vector<double> weight;
	p_FormView3->SolveLinearSystem(coefficients,b,weight);
	pNewSurface->SetWeight(0,0,w00);
	pNewSurface->SetWeight(6,0,w60);
	pNewSurface->SetWeight(0,6,w06);
	pNewSurface->SetWeight(6,6,w66);

	int index = 0;
	for(int i = 0; i <= 6; i++)
		for(int j = 0; j <= 6; j++)
		{
			if(j == 0 && i == 0 || j == 6 && i == 0 || j == 0 && i == 6 || j == 6 && i == 6)
				continue;

			pNewSurface->SetWeight(j,i,weight[index++]);
		}
}