#include "stdafx.h"
#include "MobiusTransformation.h"

MobiusTransformation::MobiusTransformation(void)
{
	m_Mobiustransformation_Alpha = 0.5;
	m_Mobiustransformation_Beta = 0.5;
}

MobiusTransformation::~MobiusTransformation(void)
{
}
int MobiusTransformation::Evaluation(double s,double t,double &u,double &v)
{
	double a = m_Mobiustransformation_Alpha;
	double b = m_Mobiustransformation_Beta;

	u = (a - 1) * s / (2 * a * s - s - a);
	v = (b - 1) * t / (2 * b * t - t - b);
	return 1;
}
int MobiusTransformation::InverseEvaluation(double u,double v,double &s,double &t)
{
	return 1;
}
Transformation* MobiusTransformation::copy()
{
	MobiusTransformation* pNewTrans = new MobiusTransformation();
	pNewTrans->m_ReparameterizationType = m_ReparameterizationType;
	pNewTrans->m_Mobiustransformation_Alpha = m_Mobiustransformation_Alpha;
	pNewTrans->m_Mobiustransformation_Beta = m_Mobiustransformation_Beta;
	pNewTrans->umax_Reparameterization = umax_Reparameterization;
	pNewTrans->umin_Reparameterization = umin_Reparameterization;
	pNewTrans->vmin_Reparameterization = vmin_Reparameterization;
	pNewTrans->vmax_Reparameterization = vmax_Reparameterization;
	return (Transformation*)pNewTrans;
}