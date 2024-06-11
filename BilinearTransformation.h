#pragma once
#include "transformation.h"

class BilinearTransformation :
	public Transformation
{
public:
	double m_Bilinear_Alpha1,m_Bilinear_Alpha2;
	double m_Bilinear_Beta1,m_Bilinear_Beta2;
	BilinearTransformation(void);
	~BilinearTransformation(void);
	int Evaluation(double s,double t,double &u,double &v);
	int InverseEvaluation(double u,double v,double &s,double &t);
	Transformation* copy();

	double Comput_u_s(double s,double t);

	double Comput_u_t(double s,double t);

	double Comput_v_s(double s,double t);

	double Comput_v_t(double s,double t);
};
