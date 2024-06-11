#pragma once
#include "energy.h"

class ClosenessEnergy :	public Energy
{
public:
	ClosenessEnergy(real_1d_array x1);
	~ClosenessEnergy(void);

private:
	real_1d_array x;

public:
	/*3X3��B-spline*/
	virtual void getBSplineGrad33(double &J,real_1d_array &grad);
	/*1X1��*/
	virtual void getGrad11(double &J,real_1d_array &grad);
	/*2X2��*/
	virtual void getGrad22(double &J,real_1d_array &grad);
	/*3X3��*/
	virtual void getGrad33(double &J,std::vector<double> &grad,std::vector<double> &x);
	/*For surface 2X2*/
	static int getIndex22(int i,int j,int k);
	/*For surface 3X3*/
	static int getIndex33(int i,int j,int k);
	/*For B-Spline Surface 3X3*/
	static int getIndexBSpline33(int i,int j,int k);
	/*For B-Spline Surface 3X3,ȡ�õڣ�i,j����Bezier�ڵģ�m,n����������surface�е�λ��,Bezier�Ǵ�0��ʼ������*/
	static int getIndexForIJBezierMNInBSpline(int i,int j,int m,int n,int k);
private:
	//���������
	int c(int i,int n);
	/*For surface 2X2����������a=k*(pi1j1-pi2j2)*(pi3j3-pi4j4),������2*a'*/
	std::vector<double> getDerivative22(double k,const real_1d_array &x,int i1,int j1,int i2,int j2,int i3,int j3,int i4,int j4);
	/*For surface 3X3, ��������a=k*(pi1j1-pi2j2)*(pi3j3-pi4j4),������2*a'*/
	std::vector<double> getDerivative33(double k,std::vector<double> &x,int i1,int j1,int i2,int j2,int i3,int j3,int i4,int j4);

};
