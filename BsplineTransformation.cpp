#include "stdafx.h"
#include "BsplineTransformation.h"
#include "C_NurbsSurf.h"
#include <sstream> 
#include <iostream>
#include <fstream>
BsplineTransformation::BsplineTransformation(void)
{
	pNurbsSurf = NULL;
}

BsplineTransformation::~BsplineTransformation(void)
{
}
int BsplineTransformation::Evaluation(double s,double t,double &u,double &v)
{
	Point3D point;
	point = pNurbsSurf->ComputPosition(s,t);
	if(u > 1)
		u = 1;
	if(u < 0)
		u = 0;
	if(v > 1)
		v = 1;
	if(v < 0)
		v = 0;
	u = point.x;
	v = point.y;
	return 1;
}
int BsplineTransformation::InverseEvaluation(double u,double v,double &s,double &t)
{
	return 1;
}
Transformation* BsplineTransformation::copy()
{
	BsplineTransformation* pNewTrans = new BsplineTransformation();
	C_NurbsSurf* pNewSurf = (C_NurbsSurf*)pNurbsSurf->Copy();
	pNewTrans->pNurbsSurf = pNewSurf;
	pNewTrans->m_ReparameterizationType = m_ReparameterizationType;
	pNewTrans->umin_Reparameterization = umin_Reparameterization;
	pNewTrans->umax_Reparameterization = umax_Reparameterization;
	pNewTrans->vmin_Reparameterization = vmin_Reparameterization;
	pNewTrans->vmax_Reparameterization = vmax_Reparameterization;
	pNewTrans->umin = umin;
	pNewTrans->umax = umax;
	pNewTrans->vmin = vmin;
	pNewTrans->vmax = vmax;
	return pNewTrans;
}

double BsplineTransformation::Comput_u_s(double s,double t)
{
	Vector3D udir = pNurbsSurf->ComputUDerivative1(s,t);

	return udir[0];
}

double BsplineTransformation::Comput_u_t(double s,double t)
{
	Vector3D vdir = pNurbsSurf->ComputVDerivative1(s,t);

	return vdir[0];
}

double BsplineTransformation::Comput_v_s(double s,double t)
{
	Vector3D udir = pNurbsSurf->ComputUDerivative1(s,t);

	return udir[1];
}

double BsplineTransformation::Comput_v_t(double s,double t)
{
	Vector3D vdir = pNurbsSurf->ComputVDerivative1(s,t);

	return vdir[1];
}
void BsplineTransformation::Save(std::ofstream &file)
{
	Transformation::Save(file);

	if(pNurbsSurf)
	{
		file << 1 << "\n";
		pNurbsSurf->Save(file);
	}else
	{
		file << 0 << "\n";
	}
}
void BsplineTransformation::Read(std::ifstream &file)
{
	Transformation::Read(file);
	int label;
	file >> label;
	if(label)
	{
		pNurbsSurf = new C_NurbsSurf();
		pNurbsSurf->Read(file);
	}
}