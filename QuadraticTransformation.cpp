#include "stdafx.h"
#include "QuadraticTransformation.h"

QuadraticTransformation::QuadraticTransformation(void)
{
	m_Quadratic_Alpha1 = 0.5;
	m_Quadratic_Alpha2 = 0.5;
	m_Quadratic_Alpha3 = 0.5;
	m_Quadratic_Beta1 = 0.5;
	m_Quadratic_Beta2 = 0.5;
	m_Quadratic_Beta3 = 0.5;

}

QuadraticTransformation::~QuadraticTransformation(void)
{
}
int QuadraticTransformation::Evaluation(double s,double t,double &u,double &v)
{
	double a1 = m_Quadratic_Alpha1;
	double a2 = m_Quadratic_Alpha2;
	double a3 = m_Quadratic_Alpha3;

	double b1 = m_Quadratic_Beta1;
	double b2 = m_Quadratic_Beta2;
	double b3 = m_Quadratic_Beta3;

	s = (s - umin_Reparameterization)/(umax_Reparameterization - umin_Reparameterization);
	t = (t - vmin_Reparameterization)/(vmax_Reparameterization - vmin_Reparameterization);

	double a = a1 * (1 - t) * (1 - t) + a2 * 2 * t * (1 - t) + a3 * t * t;
	double b = b1 * (1 - s) * (1 - s) + b2 * 2 * s * (1 - s) + b3 * s * s;

	u = (a - 1) * s / (2 * a * s - s - a);
	v = (b - 1) * t / (2 * b * t - t - b);

	u = umin + (umax - umin) * u;
	v = vmin + (vmax - vmin) * v;
	return 1;
}
int QuadraticTransformation::Evaluation(Point2D p1,Point2D &p2)										
{
	double s = p1.x;
	double t = p1.y;
	double u,v;

	double a1 = m_Quadratic_Alpha1;
	double a2 = m_Quadratic_Alpha2;
	double a3 = m_Quadratic_Alpha3;

	double b1 = m_Quadratic_Beta1;
	double b2 = m_Quadratic_Beta2;
	double b3 = m_Quadratic_Beta3;

	s = (s - umin_Reparameterization)/(umax_Reparameterization - umin_Reparameterization);
	t = (t - vmin_Reparameterization)/(vmax_Reparameterization - vmin_Reparameterization);

	double a = a1 * (1 - t) * (1 - t) + a2 * 2 * t * (1 - t) + a3 * t * t;
	double b = b1 * (1 - s) * (1 - s) + b2 * 2 * s * (1 - s) + b3 * s * s;

	u = (a - 1) * s / (2 * a * s - s - a);
	v = (b - 1) * t / (2 * b * t - t - b);

	u = umin + (umax - umin) * u;
	v = vmin + (vmax - vmin) * v;
	p2.x = u;
	p2.y = v;
	return 1;
}

int QuadraticTransformation::InverseEvaluation(double u,double v,double &s,double &t)
{
	double a1 = m_Quadratic_Alpha1;
	double a2 = m_Quadratic_Alpha2;
	double a3 = m_Quadratic_Alpha3;

	double b1 = m_Quadratic_Beta1;
	double b2 = m_Quadratic_Beta2;
	double b3 = m_Quadratic_Beta3;


	int num = 200;
	double mindis = 1e10;
	int besti,bestj;
	for(int i = 0; i <= num; i++)
		for(int j = 0; j <= num; j++)
		{
			double s1 = i / (num + 0.0);
			double t1 = j / (num + 0.0);
			
			double u1,v1;
			s1 = s1 * (umax_Reparameterization - umin_Reparameterization) + umin_Reparameterization;
			t1 = t1 * (vmax_Reparameterization - vmin_Reparameterization) + vmin_Reparameterization;

			Evaluation(s1,t1,u1,v1);

			//u1 = u1 * (umax - umin) + umin;
			//v1 = v1 * (vmax - vmin) + vmin;

			double dis = SQR(u1 - u) + SQR(v1 - v);
			if(dis < mindis)
			{
				mindis = dis;
				besti = i;
				bestj = j;
			}
		}

	s = besti / (num + 0.0);
	t = bestj / (num + 0.0);

	s = umin_Reparameterization + s * (umax_Reparameterization - umin_Reparameterization);
	t = vmin_Reparameterization + t * (vmax_Reparameterization - vmin_Reparameterization);
	
	return 1;
}
Transformation* QuadraticTransformation::copy()
{
	QuadraticTransformation* pNewTrans = new QuadraticTransformation();
	pNewTrans->m_ReparameterizationType = m_ReparameterizationType;
	pNewTrans->m_Quadratic_Alpha1 = m_Quadratic_Alpha1;
	pNewTrans->m_Quadratic_Alpha2 = m_Quadratic_Alpha2;
	pNewTrans->m_Quadratic_Alpha3 = m_Quadratic_Alpha3;
	pNewTrans->m_Quadratic_Beta1 = m_Quadratic_Beta1;
	pNewTrans->m_Quadratic_Beta2 = m_Quadratic_Beta2;
	pNewTrans->m_Quadratic_Beta3 = m_Quadratic_Beta3;
	pNewTrans->umax_Reparameterization = umax_Reparameterization;
	pNewTrans->umin_Reparameterization = umin_Reparameterization;
	pNewTrans->vmin_Reparameterization = vmin_Reparameterization;
	pNewTrans->vmax_Reparameterization = vmax_Reparameterization;
	return (Transformation*)pNewTrans;
}
double QuadraticTransformation::Comput_u_s(double s,double t)
{
	double a1 = m_Quadratic_Alpha1;
	double a2 = m_Quadratic_Alpha2;
	double a3 = m_Quadratic_Alpha3;

	double b1 = m_Quadratic_Beta1;
	double b2 = m_Quadratic_Beta2;
	double b3 = m_Quadratic_Beta3;

	double val = -(a1-2*a1*t+a1*SQR(t)+2*t*a2-2*SQR(t)*a2+SQR(t)*a3-1)*(a1-2*a1*t+a1*SQR(t)+2*t*a2-2*SQR(t)*a2+SQR(t)*a3)/
	SQR(2*s*a1-4*s*a1*t+2*s*a1*SQR(t)+4*s*t*a2-4*s*SQR(t)*a2+2*s*SQR(t)*a3-s-a1+2*a1*t-a1*SQR(t)-2*t*a2+2*SQR(t)*a2-SQR(t)*a3);
	return val;
}

double QuadraticTransformation::Comput_u_t(double s,double t)
{
	double a1 = m_Quadratic_Alpha1;
	double a2 = m_Quadratic_Alpha2;
	double a3 = m_Quadratic_Alpha3;

	double b1 = m_Quadratic_Beta1;
	double b2 = m_Quadratic_Beta2;
	double b3 = m_Quadratic_Beta3;
	double val = 2*s*(s-1)*(-a1+a1*t-2*t*a2+a2+t*a3)/
		SQR(2*s*a1-4*s*a1*t+2*s*a1*SQR(t)+4*s*t*a2-4*s*SQR(t)*a2+2*s*SQR(t)*a3-s-a1+2*a1*t-a1*SQR(t)-2*t*a2+2*SQR(t)*a2-SQR(t)*a3);
	return val;
}

double QuadraticTransformation::Comput_v_s(double s,double t)
{
	double a1 = m_Quadratic_Alpha1;
	double a2 = m_Quadratic_Alpha2;
	double a3 = m_Quadratic_Alpha3;

	double b1 = m_Quadratic_Beta1;
	double b2 = m_Quadratic_Beta2;
	double b3 = m_Quadratic_Beta3;
	double val = 2*t*(t-1)*(-b1+b1*s-2*s*b2+b2+s*b3)/
		SQR(2*t*b1-4*t*b1*s+2*t*b1*SQR(s)+4*t*s*b2-4*t*SQR(s)*b2+2*t*SQR(s)*b3-t-b1+2*b1*s-b1*SQR(s)-2*s*b2+2*SQR(s)*b2-SQR(s)*b3);
	return val;
}

double QuadraticTransformation::Comput_v_t(double s,double t)
{
	double a1 = m_Quadratic_Alpha1;
	double a2 = m_Quadratic_Alpha2;
	double a3 = m_Quadratic_Alpha3;

	double b1 = m_Quadratic_Beta1;
	double b2 = m_Quadratic_Beta2;
	double b3 = m_Quadratic_Beta3;
	double val = -(b1-2*b1*s+b1*SQR(s)+2*s*b2-2*SQR(s)*b2+SQR(s)*b3-1)*(b1-2*b1*s+b1*SQR(s)+2*s*b2-2*SQR(s)*b2+SQR(s)*b3)/
		SQR(2*t*b1-4*t*b1*s+2*t*b1*SQR(s)+4*t*s*b2-4*t*SQR(s)*b2+2*t*SQR(s)*b3-t-b1+2*b1*s-b1*SQR(s)-2*s*b2+2*SQR(s)*b2-SQR(s)*b3);
	return val;
}