#pragma once
#include "transformation.h"

class CompositeMobiusTransformation :
	public Transformation
{
public:
	//setmentation
	int M,N;
	std::vector<double> uknot;
	std::vector<double> vknot;
	std::vector<double> sknot;
	std::vector<double> tknot;

	std::vector<double> alpha;
	std::vector<double> beta;

	CompositeMobiusTransformation(void);
	~CompositeMobiusTransformation(void);

	int Evaluation(double s,double t,double &u,double &v);
	int InverseEvaluation(double u,double v,double &s,double &t);
	void Save(std::ofstream &file);
	void Read(std::ifstream &file);
	Transformation* copy();
};
