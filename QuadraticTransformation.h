#pragma once
#include "transformation.h"

class QuadraticTransformation :
	public Transformation
{
public:
	double m_Quadratic_Alpha1,m_Quadratic_Alpha2,m_Quadratic_Alpha3;
	double m_Quadratic_Beta1,m_Quadratic_Beta2,m_Quadratic_Beta3;

	QuadraticTransformation(void);
	~QuadraticTransformation(void);

	//Evaluation
	int Evaluation(double s,double t,double &u,double &v);

	//Evaluation for Point2D
	int Evaluation(Point2D p1,Point2D &p2);

	//
	int InverseEvaluation(double u,double v,double &s,double &t);

	Transformation* copy();

	double Comput_u_s(double s,double t);

	double Comput_u_t(double s,double t);

	double Comput_v_s(double s,double t);

	double Comput_v_t(double s,double t);
};
