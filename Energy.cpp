#include "stdafx.h"
#include "energy.h"

//surface of degree m * n
int Energy::getIndex(int m, int n, int i, int j, int k)
{
	return (j * (m + 1) + i) * 3 + k;
}

std::vector<double> Energy::getDerivative(int i, int j, double k,std::vector<double> &x,int i1,int j1,int i2,int j2,int i3,int j3,int i4,int j4)
{

	std::vector<double> gradient;
	gradient.clear();

	int num = (i + 1) * (j + 1) * 3;
	for(int i = 0; i < num; i++)
		gradient.push_back(0);

	gradient[getIndex(i,j,i1,j1,0)] += 2 * k * (x[getIndex(i,j,i3,j3,0)] - x[getIndex(i,j,i4,j4,0)]);//x
	gradient[getIndex(i,j,i1,j1,1)] += 2 * k * (x[getIndex(i,j,i3,j3,1)] - x[getIndex(i,j,i4,j4,1)]);//y
	gradient[getIndex(i,j,i1,j1,2)] += 2 * k * (x[getIndex(i,j,i3,j3,2)] - x[getIndex(i,j,i4,j4,2)]);//z

	gradient[getIndex(i,j,i2,j2,0)] += 2 * k * (x[getIndex(i,j,i4,j4,0)] - x[getIndex(i,j,i3,j3,0)]);//x
	gradient[getIndex(i,j,i2,j2,1)] += 2 * k * (x[getIndex(i,j,i4,j4,1)] - x[getIndex(i,j,i3,j3,1)]);//y
	gradient[getIndex(i,j,i2,j2,2)] += 2 * k * (x[getIndex(i,j,i4,j4,2)] - x[getIndex(i,j,i3,j3,2)]);//z

	gradient[getIndex(i,j,i3,j3,0)] += 2 * k * (x[getIndex(i,j,i1,j1,0)] - x[getIndex(i,j,i2,j2,0)]);//x
	gradient[getIndex(i,j,i3,j3,1)] += 2 * k * (x[getIndex(i,j,i1,j1,1)] - x[getIndex(i,j,i2,j2,1)]);//y
	gradient[getIndex(i,j,i3,j3,2)] += 2 * k * (x[getIndex(i,j,i1,j1,2)] - x[getIndex(i,j,i2,j2,2)]);//z

	gradient[getIndex(i,j,i4,j4,0)] += 2 * k * (x[getIndex(i,j,i2,j2,0)] - x[getIndex(i,j,i1,j1,0)]);//x
	gradient[getIndex(i,j,i4,j4,1)] += 2 * k * (x[getIndex(i,j,i2,j2,1)] - x[getIndex(i,j,i1,j1,1)]);//y
	gradient[getIndex(i,j,i4,j4,2)] += 2 * k * (x[getIndex(i,j,i2,j2,2)] - x[getIndex(i,j,i1,j1,2)]);//z

	return gradient;

}

std::vector<double> Energy::getDerivative(int i, int j, double k,std::vector<double> &x,int i1,int j1,int i2,int j2)
{

	std::vector<double> gradient;
	gradient.clear();

	int num = (i + 1) * (j + 1) * 3;
	for(int i = 0; i < num; i++)
		gradient.push_back(0);

	gradient[getIndex(i,j,i1,j1,0)] +=  k * x[getIndex(i,j,i2,j2,0)];//x
	gradient[getIndex(i,j,i1,j1,1)] += k * x[getIndex(i,j,i2,j2,1)];//y
	gradient[getIndex(i,j,i1,j1,2)] += k * x[getIndex(i,j,i2,j2,2)];//z

	gradient[getIndex(i,j,i2,j2,0)] += k * x[getIndex(i,j,i1,j1,0)];//x
	gradient[getIndex(i,j,i2,j2,1)] += k * x[getIndex(i,j,i1,j1,1)];//y
	gradient[getIndex(i,j,i2,j2,2)] += k * x[getIndex(i,j,i1,j1,2)];//z

	return gradient;

}