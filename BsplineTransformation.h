#pragma once
#include "transformation.h"
class CMesh3d;
class C_NurbsSurf;
class BsplineTransformation : public Transformation
{
public:
	BsplineTransformation(void);
	~BsplineTransformation(void);

	C_NurbsSurf* pNurbsSurf;
	//evaluation
	int Evaluation(double s,double t,double &u,double &v);

	//inverse evaluation
	int InverseEvaluation(double u,double v,double &s,double &t);

	double Comput_u_s(double s,double t);

	double Comput_u_t(double s,double t);

	double Comput_v_s(double s,double t);

	double Comput_v_t(double s,double t);

	 Transformation* copy();

	 virtual void Read(std::ifstream &file);

	 virtual void Save(std::ofstream &file);
};
