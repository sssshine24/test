#pragma once
#include "ap.h"
#include <vector>

using namespace alglib;
class Energy
{
public:
	Energy(const real_1d_array &x){};

	/*3X3½×B-spline*/
	virtual void getBSplineGrad33(double &J,real_1d_array &grad)=0;

	/*1X1½×*/
	virtual void getGrad11(double&J,real_1d_array &grad)=0;

	/*2X2½×*/
	virtual void getGrad22(double &J,real_1d_array &grad)=0;

	/*3X3*/
	virtual void getGrad33(double &J,std::vector<double> &grad,std::vector<double> &x)=0;

	/*For surface of degree m * n*/
	static int getIndex(int m, int n, int i, int j, int k);

	//compute the derivative
	std::vector<double> getDerivative(int i, int j, double k,std::vector<double> &x,int i1,int j1,int i2,int j2,int i3,int j3,int i4,int j4);

	//
	std::vector<double> getDerivative(int i, int j, double k,std::vector<double> &x,int i1,int j1,int i2,int j2);
};