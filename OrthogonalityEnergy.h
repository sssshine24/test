#pragma once
#include "Energy.h"
#include "Point3D.h"
#include <sstream> 
#include <vector>
using namespace alglib;
class OrthogonalityEnergy:public Energy
{
private:
	real_1d_array x;
	
public:
	OrthogonalityEnergy(const real_1d_array &x);
	/*3X3阶B-spline*/
	virtual void getBSplineGrad33(double &J,real_1d_array &grad);
	/*1X1阶*/
	virtual void getGrad11(double &J,real_1d_array &grad);
	/*2X2阶*/
	virtual void getGrad22(double &J,real_1d_array &grad);
	/*3X3阶*/
	virtual void getGrad33(double &J,std::vector<double> &grad,std::vector<double> &x);

	/*For surface of degree m * n*/
	static int getIndex(int m, int n, int i, int j, int k);

	/*For B-Spline Surface 3X3*/
	static int getIndexBSpline33(int i,int j,int k);
	/*For B-Spline Surface 3X3,取得第（i,j）个Bezier内的（m,n）点在整个surface中的位置,Bezier是从0开始计数的*/
	static int getIndexForIJBezierMNInBSpline(int i,int j,int m,int n,int k);
private:
	//计算组合数
	int c(int i,int n);
	
	//string vectorDoubleToString(std::vector<double> &d);

};