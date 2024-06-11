#pragma once
#include "transformation.h"

class MobiusTransformation :
	public Transformation
{
public:
	double m_Mobiustransformation_Alpha;
	double m_Mobiustransformation_Beta;

	MobiusTransformation(void);
	~MobiusTransformation(void);
	int Evaluation(double s,double t,double &u,double &v);
	int InverseEvaluation(double u,double v,double &s,double &t);
	Transformation* copy();
};
