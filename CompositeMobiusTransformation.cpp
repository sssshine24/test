#include "stdafx.h"
#include "CompositeMobiusTransformation.h"
#include <fstream>
CompositeMobiusTransformation::CompositeMobiusTransformation(void)
{
}

CompositeMobiusTransformation::~CompositeMobiusTransformation(void)
{

}
int CompositeMobiusTransformation::Evaluation(double s,double t,double &u,double &v)
{
	int sindex,tindex;
	for(int i = 0; i < M ; i++)
		if(s >= sknot[i] && s <= sknot[i + 1])
		{
			sindex = i;	
			break;
		}
	for(int i = 0; i < N ; i++)
		if(t >= tknot[i] && t <= tknot[i + 1])
		{
			tindex = i;
			break;
		}
	//
	double sba = (s -  sknot[sindex]) / (sknot[sindex + 1] - sknot[sindex]);
	double tba = (t -  tknot[tindex]) / (tknot[tindex + 1] - tknot[tindex]);

	u = uknot[sindex] + (uknot[sindex + 1] - uknot[sindex]) * (1 - alpha[sindex]) * sba / (alpha[sindex] * (1 - sba) + (1 - alpha[sindex]) * sba);
	v = vknot[tindex] + (vknot[tindex + 1] - vknot[tindex]) * (1 - beta[tindex]) * tba / (beta[tindex] * (1 - tba) + (1 - beta[tindex]) * tba);	
	return 1;
}
int CompositeMobiusTransformation::InverseEvaluation(double u,double v,double &s,double &t)
{
	return 1;
}
Transformation* CompositeMobiusTransformation::copy()
{
	return NULL;
}
void CompositeMobiusTransformation::Save(std::ofstream &file)
{
	Transformation::Save(file);
	file << M << " " << N << "\n";
	for(int i = 0; i <= M; i++)
		file << uknot[i] << " ";
	file << "\n";
	for(int i = 0; i <= N; i++)
		file << vknot[i] << " ";
	file << "\n";
	for(int i = 0; i <= M; i++)
		file << sknot[i] << " ";
	file << "\n";
	for(int i = 0; i <= N; i++)
		file << tknot[i] << " ";
	file << "\n";

	for(int i = 0; i <= M - 1; i++)
		file << alpha[i] << " ";
	file << "\n";
	for(int i = 0; i <= N - 1; i++)
		file << beta[i] << " ";
	file << "\n";

}
void CompositeMobiusTransformation::Read(std::ifstream &file)
{
	if(file.eof())
	{
		return;
	}
	Transformation::Read(file);
	uknot.clear();
	vknot.clear();
	sknot.clear();
	tknot.clear();
	alpha.clear();
	beta.clear();

	file >> M;
	file >> N;
	double temp;
	for(int i = 0; i <= M; i++)
	{
		file >> temp;
		uknot.push_back(temp);
	}
	for(int i = 0; i <= N; i++)
	{
		file >> temp;
		vknot.push_back(temp);
	}
	for(int i = 0; i <= M; i++)
	{
		file >> temp;
		sknot.push_back(temp);
	}
	for(int i = 0; i <= N; i++)
	{
		file >> temp;
		tknot.push_back(temp);
	}
	for(int i = 0; i <= M - 1; i++)
	{
		file >> temp;
		alpha.push_back(temp);
	}
	for(int i = 0; i <= N - 1; i++)
	{
		file >> temp;
		beta.push_back(temp);
	}
}