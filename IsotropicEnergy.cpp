#include "stdafx.h"
#include "IsotropicEnergy.h"

IsotropicEnergy::IsotropicEnergy(const real_1d_array & x):Energy(x)
{
	this->x = x;
}

IsotropicEnergy::~IsotropicEnergy(void)
{
}

void IsotropicEnergy::getBSplineGrad33(double &J,real_1d_array &grad)
{

}
int IsotropicEnergy::getIndex(int m, int n, int i, int j, int k)
{
	return Energy::getIndex(m,n,i,j,k);
}

void IsotropicEnergy::getGrad11(double &J,real_1d_array &grad)
{
	//
	double a[3][3];
	Vector3D p00,p10,p01,p11;

	for(int i=0;i<3;i++)
	{
		p00[i]=x[i];
		p10[i]=x[i+3];
		p01[i]=x[i+6];
		p11[i]=x[i+9];
	}

	//the coefficient of term B0^2(u)B0^2(v)
	//-2*p00*p01+p01^2+2*p00*p10-p10^2
	a[0][0] = -2 * p00 * p01 + 
		           p01 * p01 + 
			   2 * p00 * p10 - 
			       p10 * p10;

	//the coefficient of term B1^2(u)B0^2(v)
	//p00^2-3*p00*p01+p01^2+p10*p01+p00*p11-p10*p11
	a[1][0] =   p00 * p00
		   -3 * p00 * p01 
		      + p01 * p01    
			  + p10 * p01
			  + p00 * p11
			  - p10 * p11;

	//the coefficient of term B2^2(u)B0^2(v)
	//p00^2-2*p00*p01+2*p01*p11-p11^2
	a[2][0] =   p00 * p00
	      - 2 * p00 * p01
		   +2 * p01 * p11
		    -   p11 * p11;	

	//the coefficient of term B0^2(u)B1^2(v)
	//-p00^2+3*p00*p10-p10^2-p10*p01+p01*p11-p00*p11
	a[0][1] = -p00 * p00 
		  +3 * p00 * p10
		      -p10 * p10
			  -p10 * p01
			  +p01 * p11
			  -p00 * p11;

	//the coefficient of term B1^2(u)B1^2(v)
	//p00*p10-p00*p01+p01*p11-p10*p11
	a[1][1] = p00 * p10 - p00 * p01 + p01 * p11- p10 * p11;

	//the coefficient of term B2^2(u)B1^2(v)
	//p00*p10-p10*p01-p01^2+3*p01*p11-p11^2-p00*p11
	a[2][1] = p00 * p10 
		    - p10 * p01
			- p01 * p01
		   +3*p01 * p11
		     -p11 * p11
			 -p00 * p11;

	//the coefficient of term B0^2(u)B2^2(v)
	//-p00^2+2*p00*p10-2*p10*p11+p11^2
	a[0][2] = -p00 * p00 + 2 * p00 * p10 - 2 * p10 * p11 + p11 * p11;

	//the coefficient of term B1^2(u)B2^2(v)
	//p10^2-p00*p01+p10*p01-3*p10*p11+p11^2+p00*p11
	a[1][2] = p10 * p10 - 
		      p00 * p01 + 
			  p10 * p01 
		   -3*p10 * p11
		     +p11 * p11 
			 +p00 * p11;

	//the coefficient of term B2^2(u)B2^2(v)
	//p10^2-p01^2+2*p01*p11-2*p10*p11
	a[2][2] = p10 * p10 - p01 * p01 + 2 * p01 * p11- 2 * p10 * p11;

	J = 0;
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			J += SQR(a[i][j]);


	std::vector<double> gradient;

	for(int i = 0; i < 12; i++)
		gradient.push_back(0);

	std::vector<double> stdx;
	for(int i = 0; i < 12; i++)
		stdx.push_back(x[i]);

	//-2*p00*p01+p01^2+2*p00*p10-p10^2
	gradient = gradient + getDerivative(1, 1, -2, stdx, 0,0,	0,1)*a[0][0]
	                    + getDerivative(1, 1,  1, stdx, 0,1,	0,1)*a[0][0]
	                    + getDerivative(1, 1,  2, stdx, 0,0,	1,0)*a[0][0]
						+ getDerivative(1, 1, -1, stdx, 1,0,	1,0)*a[0][0];

	//p00^2-3*p00*p01+p01^2+p10*p01+p00*p11-p10*p11
	gradient = gradient + getDerivative(1, 1,  1, stdx, 0,0,	0,0)*a[1][0]
						+ getDerivative(1, 1, -3, stdx, 0,0,	0,1)*a[1][0]
						+ getDerivative(1, 1,  1, stdx, 0,1,	0,1)*a[1][0]
						+ getDerivative(1, 1,  1, stdx, 1,0,	0,1)*a[1][0]
						+ getDerivative(1, 1,  1, stdx, 0,0,	1,1)*a[1][0]
						+ getDerivative(1, 1, -1, stdx, 1,0,	1,1)*a[1][0];

	//p00^2-2*p00*p01+2*p01*p11-p11^2
	gradient = gradient + getDerivative(1, 1,  1, stdx, 0,0,	0,0)*a[2][0]
						+ getDerivative(1, 1, -2, stdx, 0,0,	0,1)*a[2][0]
						+ getDerivative(1, 1,  2, stdx, 0,1,	1,1)*a[2][0]
						+ getDerivative(1, 1, -1, stdx, 1,1,	1,1)*a[2][0];

	//-p00^2+3*p00*p10-p10^2-p10*p01+p01*p11-p00*p11
	gradient = gradient + getDerivative(1, 1, -1, stdx, 0,0,	0,0)*a[0][1]
						+ getDerivative(1, 1,  3, stdx, 0,0,	1,0)*a[0][1]
						+ getDerivative(1, 1, -1, stdx, 1,0,	1,0)*a[0][1]
						+ getDerivative(1, 1, -1, stdx, 1,0,	0,1)*a[0][1]
						+ getDerivative(1, 1,  1, stdx, 0,1,	1,1)*a[0][1]
						+ getDerivative(1, 1, -1, stdx, 0,0,	1,1)*a[0][1];

	//p00*p10-p00*p01+p01*p11-p10*p11
	gradient = gradient + getDerivative(1, 1,  1, stdx, 0,0,	1,0)*a[1][1]
						+ getDerivative(1, 1, -1, stdx, 0,0,	0,1)*a[1][1]
						+ getDerivative(1, 1,  1, stdx, 0,1,	1,1)*a[1][1]
						+ getDerivative(1, 1, -1, stdx, 1,0,	1,1)*a[1][1];

	//p00*p10-p10*p01-p01^2+3*p01*p11-p11^2-p00*p11
	gradient = gradient + getDerivative(1, 1,  1, stdx, 0,0,	1,0)*a[2][1]
						+ getDerivative(1, 1, -1, stdx, 1,0,	0,1)*a[2][1]
						+ getDerivative(1, 1, -1, stdx, 0,1,	0,1)*a[2][1]
						+ getDerivative(1, 1,  3, stdx, 0,1,	1,1)*a[2][1]
						+ getDerivative(1, 1, -1, stdx, 1,1,	1,1)*a[2][1]
						+ getDerivative(1, 1, -1, stdx, 0,0,	1,1)*a[2][1];

	//-p00^2+2*p00*p10-2*p10*p11+p11^2
	gradient = gradient + getDerivative(1, 1, -1, stdx, 0,0,	0,0)*a[0][2]
						+ getDerivative(1, 1,  2, stdx, 0,0,	1,0)*a[0][2]
						+ getDerivative(1, 1, -2, stdx, 1,0,	1,1)*a[0][2]
						+ getDerivative(1, 1,  1, stdx, 1,1,	1,1)*a[0][2];
	
	//p10^2-p00*p01+p10*p01-3*p10*p11+p11^2+p00*p11
	gradient = gradient + getDerivative(1, 1,  1, stdx, 1,0,	1,0)*a[1][2]
						+ getDerivative(1, 1, -1, stdx, 0,0,	0,1)*a[1][2]
						+ getDerivative(1, 1,  1, stdx, 1,0,	0,1)*a[1][2]
						+ getDerivative(1, 1, -3, stdx, 1,0,	1,1)*a[1][2]
						+ getDerivative(1, 1,  1, stdx, 1,1,	1,1)*a[1][2]
						+ getDerivative(1, 1,  1, stdx, 0,0,	1,1)*a[1][2];

	//p10^2-p01^2+2*p01*p11-2*p10*p11
	gradient = gradient + getDerivative(1, 1,  1, stdx, 1,0,	1,0)*a[2][2]
						+ getDerivative(1, 1, -1, stdx, 0,1,	0,1)*a[2][2]
						+ getDerivative(1, 1,  2, stdx, 0,1,	1,1)*a[2][2]
						+ getDerivative(1, 1, -2, stdx, 1,0,	1,1)*a[2][2];

	for(int i = 0; i < 12; i++)
	{
		grad[i] = gradient[i] * 2;
		TRACE("%f ",grad[i]);
	}
	TRACE("\n");
}
void IsotropicEnergy::getGrad22(double &J,real_1d_array &grad)
{
	double a[5][5];

	Vector3D p00(x[0],x[1],x[2]),p10(x[3],x[4],x[5]),p20(x[6],x[7],x[8]);
	Vector3D p01(x[9],x[10],x[11]),p11(x[12],x[13],x[14]),p21(x[15],x[16],x[17]);
	Vector3D p02(x[18],x[19],x[20]),p12(x[21],x[22],x[23]),p22(x[24],x[25],x[26]);

	//the coefficient of term B0^4(u)B0^4(v)
	//(-2*p00+2*p10)^2-(-2*p00+2*p01)^2
	a[0][0] = (-2*p00+2*p10) * (-2*p00+2*p10)
		     -(-2*p00+2*p01) * (-2*p00+2*p01);

	//the coefficient of term B1^4(u)B0^4(v)
	//4*p00*p11+4*p10*p01-4*p01*p11+2*p00^2-6*p00*p10+2*p10*p20-2*p00*p20
	a[1][0] =   4*p00*p11+4*p10*p01-4*p01*p11+2*p00*p00-6*p00*p10+2*p10*p20-2*p00*p20;

	//the coefficient of term B2^4(u)B0^4(v)
	//(16/3)*p10*p11-(8/3)*p11*p11+(2/3)*p00^2+(4/3)*p00*p10-4*p10^2+(2/3)*p20*p20+(4/3)*p20*p01+(4/3)*p10*p20-4*p00*p20+(4/3)*p00*p21-(4/3)*p01*p21
	a[2][0] =  (16.0/3)*p10*p11-(8.0/3)*p11*p11+(2.0/3)*p00*p00+(4.0/3)*p00*p10-4*p10*p10+(2.0/3)*p20*p20+(4.0/3)*p20*p01+(4.0/3)*p10*p20-4*p00*p20+(4.0/3)*p00*p21-(4.0/3)*p01*p21;	

	//the coefficient of term B3^4(u)B0^4(v)
	//2*p00*p10+4*p10*p21+2*p20*p20+4*p20*p11-4*p11*p21-6*p10*p20-2*p00*p20
	a[3][0] =  2*p00*p10+4*p10*p21+2*p20*p20+4*p20*p11-4*p11*p21-6*p10*p20-2*p00*p20;	
	
	//the coefficient of term B4^4(u)B0^4(v)
	//(-2*p10+2*p20)^2-(-2*p20+2*p21)^2
	a[4][0] = (-2*p10+2*p20) * (-2*p10+2*p20) - (-2*p20+2*p21) * (-2*p20+2*p21);	

	//the coefficient of term B0^4(u)B1^4(v)
	//-2*p01*p02-4*p00*p11+2*p00*p02-4*p10*p01+4*p10*p11-2*p00^2+6*p00*p01
	a[0][1] =-2*p01*p02-4*p00*p11+2*p00*p02-4*p10*p01+4*p10*p11-2*p00*p00+6*p00*p01;

	//the coefficient of term B1^4(u)B1^4(v)
	//p00*p12+p10*p02-p01*p12-p11*p02-2*p00*p10+2*p00*p01+p10*p21+p20*p11-p00*p21-p20*p01
	a[1][1] = p00*p12+p10*p02-p01*p12-p11*p02-2*p00*p10+2*p00*p01+p10*p21+p20*p11-p00*p21-p20*p01;

	//the coefficient of term B2^4(u)B1^4(v)
	//(2/3)*p00*p11+(2/3)*p10*p01+(4/3)*p10*p12-(4/3)*p11*p12-(4/3)*p10*p10+(2/3)*p00*p01+(2/3)*p10*p21+(2/3)*p20*p11+(2/3)*p20*p21+(1/3)*p00*p22+(1/3)*p20*p02-(2/3)*p00*p20-p00*p21-p20*p01-(1/3)*p01*p22-(1/3)*p21*p02
	a[2][1] = (2.0/3)*p00*p11+(2.0/3)*p10*p01+(4.0/3)*p10*p12-(4.0/3)*p11*p12-(4.0/3)*p10*p10+(2.0/3)*p00*p01+(2.0/3)*p10*p21+(2.0/3)*p20*p11+(2.0/3)*p20*p21+(1.0/3)*p00*p22+(1.0/3)*p20*p02-(2.0/3)*p00*p20-p00*p21-p20*p01-(1.0/3)*p01*p22-(1.0/3)*p21*p02;


	a[3][1] = p00*p11+p10*p01+p10*p22+p20*p12-p11*p22-p21*p12-2*p10*p20+2*p20*p21-p00*p21-p20*p01;

	a[4][1] = -2*p21*p22+4*p10*p11-4*p10*p21-2*p20*p20-4*p20*p11+2*p20*p22+6*p20*p21;

	//the coefficient of term B0^4(u)B2^4(v)
	//-(2/3)*p02*p02-(4/3)*p01*p02+4*p00*p02-(4/3)*p00*p12-(4/3)*p10*p02+(4/3)*p10*p12-(16/3)*p01*p11+(8/3)*p11*p11-(2/3)*p00^2-(4/3)*p00*p01+4*p01*p01
	a[0][2] = -(2.0/3)*p02*p02-(4.0/3)*p01*p02+4*p00*p02-(4.0/3)*p00*p12-(4.0/3)*p10*p02+(4.0/3)*p10*p12-(16.0/3)*p01*p11+(8.0/3)*p11*p11-(2.0/3)*p00*p00-(4.0/3)*p00*p01+4*p01*p01;

	//the coefficient of term B1^4(u)B2^4(v)
	//-(2/3)*p02*p12-(2/3)*p00*p11+(2/3)*p00*p02+p00*p12-(2/3)*p10*p01+p10*p02-(2/3)*p01*p12-(2/3)*p11*p02-(2/3)*p00*p10+(4/3)*p01*p01+(1/3)*p10*p22+(1/3)*p20*p12+(4/3)*p11*p21-(1/3)*p00*p22-(1/3)*p20*p02-(4/3)*p01*p21
	a[1][2] = -(2.0/3)*p02*p12-(2.0/3)*p00*p11+(2.0/3)*p00*p02+p00*p12-(2.0/3)*p10*p01+p10*p02-(2.0/3)*p01*p12-(2.0/3)*p11*p02-(2.0/3)*p00*p10+(4.0/3)*p01*p01+(1.0/3)*p10*p22+(1.0/3)*p20*p12+(4.0/3)*p11*p21-(1.0/3)*p00*p22-(1.0/3)*p20*p02-(4.0/3)*p01*p21;

	//the coefficient of term B2^4(u)B2^4(v)
	//-(4/9)*p12*p12+(2/9)*p00*p02+(2/9)*p00*p12-(8/9)*p10*p11+(2/9)*p10*p02+(4/3)*p10*p12+(8/9)*p01*p11-(8/9)*p11*p12-(4/9)*p10*p10+(4/9)*p01*p01+(2/9)*p10*p22+(2/9)*p20*p12+(2/9)*p20*p22+(8/9)*p11*p21-(2/9)*p20*p01+(4/9)*p21*p21-(2/9)*p00*p20-(2/9)*p00*p21-(2/9)*p21*p02-(4/3)*p01*p21-(2/9)*p02*p22-(2/9)*p01*p22
	a[2][2] = -(4.0/9)*p12*p12+(2.0/9)*p00*p02+(2.0/9)*p00*p12-(8.0/9)*p10*p11+(2.0/9)*p10*p02+(4.0/3)*p10*p12+(8.0/9)*p01*p11-(8.0/9)*p11*p12-(4.0/9)*p10*p10+(4.0/9)*p01*p01+(2.0/9)*p10*p22+(2.0/9)*p20*p12+(2.0/9)*p20*p22+(8.0/9)*p11*p21-(2.0/9)*p20*p01+(4.0/9)*p21*p21-(2.0/9)*p00*p20-(2.0/9)*p00*p21-(2.0/9)*p21*p02-(4.0/3)*p01*p21-(2.0/9)*p02*p22-(2.0/9)*p01*p22;

	a[3][2] = -(2.0/3)*p12*p22+(1.0/3)*p00*p12+(1.0/3)*p10*p02+(4.0/3)*p01*p11-(2.0/3)*p10*p21+p10*p22-(2.0/3)*p20*p11+p20*p12+(2.0/3)*p20*p22-(2.0/3)*p11*p22-(2.0/3)*p21*p12-(2.0/3)*p10*p20+(4.0/3)*p21*p21-(1.0/3)*p00*p22-(1.0/3)*p20*p02-(4.0/3)*p01*p21;

	a[4][2] = -(4.0/3)*p21*p22+(4.0/3)*p10*p12+(8.0/3)*p11*p11-(2.0/3)*p22*p22-(4.0/3)*p10*p22-(2.0/3)*p20*p20-(4.0/3)*p20*p12+4*p20*p22-(16.0/3)*p11*p21-(4.0/3)*p20*p21+4*p21*p21;

	a[0][3] = -2*p02*p02+6*p01*p02+2*p00*p02-4*p01*p12-4*p11*p02+4*p11*p12-2*p00*p01;

	a[1][3] = -2*p02*p12+2*p01*p02-p00*p11+p00*p12-p10*p01+p10*p02+p11*p22+p21*p12-p01*p22-p21*p02;

	a[2][3] = -(4.0/3)*p12*p12+(2.0/3)*p01*p02+(2.0/3)*p21*p22-(4.0/3)*p10*p11+(4.0/3)*p10*p12+(2.0/3)*p01*p12+(2.0/3)*p11*p02+(2.0/3)*p11*p22+(2.0/3)*p21*p12+(1.0/3)*p00*p22+(1.0/3)*p20*p02-(1.0/3)*p00*p21-(1.0/3)*p20*p01-(2.0/3)*p02*p22-p01*p22-p21*p02;

	a[3][3] = -2*p12*p22+2*p21*p22+p01*p12+p11*p02-p10*p21+p10*p22-p20*p11+p20*p12-p01*p22-p21*p02;

	a[4][3] = 6*p21*p22+4*p11*p12-2*p22*p22+2*p20*p22-4*p11*p22-4*p21*p12-2*p20*p21;

	a[0][4] = (-2*p02+2*p12) * (-2*p02+2*p12) -(-2*p01+2*p02) * (-2*p01+2*p02);

	a[1][4] = 2*p02*p02-6*p02*p12+2*p12*p22-4*p01*p11+4*p01*p12+4*p11*p02-2*p02*p22;

	a[2][4] = (2.0/3)*p02*p02+(4.0/3)*p02*p12-4*p12*p12+(4.0/3)*p12*p22-(8.0/3)*p11*p11+(16.0/3)*p11*p12+(2.0/3)*p22*p22+(4/3)*p21*p02-(4.0/3)*p01*p21-4*p02*p22+(4.0/3)*p01*p22;

	a[3][4] = 2*p02*p12-6*p12*p22+2*p22*p22-4*p11*p21+4*p11*p22+4*p21*p12-2*p02*p22;

	a[4][4] = (-2*p12+2*p22) * (-2*p12+2*p22) - (-2*p21+2*p22) * (-2*p21+2*p22);

	J = 0;
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
			J += SQR(a[i][j]);


	std::vector<double> gradient;

	for(int i = 0; i < 27; i++)
		gradient.push_back(0);

	std::vector<double> stdx;
	for(int i = 0; i < 27; i++)
		stdx.push_back(x[i]);

	//g[0][0] -8*p00*p10+4*p10^2+8*p00*p01-4*p01^2
	gradient = gradient + getDerivative(2, 2, -8, stdx, 0,0,	1,0)*a[0][0]
						+ getDerivative(2, 2,  4, stdx, 1,0,	1,0)*a[0][0]
						+ getDerivative(2, 2,  8, stdx, 0,0,	0,1)*a[0][0]
						+ getDerivative(2, 2, -4, stdx, 0,1,	0,1)*a[0][0];

	//4*p00*p11+4*p10*p01-4*p01*p11+2*p00*p00-6*p00*p10+2*p10*p20-2*p00*p20
	//g[1][0]
	gradient = gradient + getDerivative(2, 2,  4, stdx, 0,0,	1,1)*a[1][0]
						+ getDerivative(2, 2,  4, stdx, 1,0,	0,1)*a[1][0]
						+ getDerivative(2, 2, -4, stdx, 0,1,	1,1)*a[1][0]
						+ getDerivative(2, 2,  2, stdx, 0,0,	0,0)*a[1][0]
						+ getDerivative(2, 2, -6, stdx, 0,0,	1,0)*a[1][0]
						+ getDerivative(2, 2,  2, stdx, 1,0,	2,0)*a[1][0]
						+ getDerivative(2, 2, -2, stdx, 0,0,	2,0)*a[1][0];

	//(16.0/3)*p10*p11-(8.0/3)*p11*p11+(2.0/3)*p00*p00+(4.0/3)*p00*p10-4*p10*p10+(2.0/3)*p20*p20+(4.0/3)*p20*p01+(4.0/3)*p10*p20-4*p00*p20+(4.0/3)*p00*p21-(4.0/3)*p01*p21
	//g[2][0]
	gradient = gradient + getDerivative(2, 2,  (16.0/3), stdx, 1,0, 1,1)*a[2][0]
						+ getDerivative(2, 2,  -(8.0/3), stdx, 1,1, 1,1)*a[2][0]
						+ getDerivative(2, 2,  (2.0/3),  stdx, 0,0, 0,0)*a[2][0]
						+ getDerivative(2, 2,  (4.0/3),  stdx, 0,0, 1,0)*a[2][0]
						+ getDerivative(2, 2,  -4,       stdx, 1,0, 1,0)*a[2][0]
						+ getDerivative(2, 2,  (2.0/3),  stdx, 2,0, 2,0)*a[2][0]
						+ getDerivative(2, 2,  (4.0/3),  stdx, 2,0, 0,1)*a[2][0]
						+ getDerivative(2, 2,  (4.0/3),  stdx, 1,0, 2,0)*a[2][0]
						+ getDerivative(2, 2,  -4,       stdx, 0,0, 2,0)*a[2][0]
						+ getDerivative(2, 2,  (4.0/3),  stdx, 0,0, 2,1)*a[2][0]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 0,1, 2,1)*a[2][0];

	//2*p00*p10+4*p10*p21+2*p20*p20+4*p20*p11-4*p11*p21-6*p10*p20-2*p00*p20
	//[3][0]
	gradient = gradient + getDerivative(2, 2,  2, stdx, 0,0, 1,0)*a[3][0]
						+ getDerivative(2, 2,  4, stdx, 1,0, 2,1)*a[3][0]
						+ getDerivative(2, 2,  2, stdx, 2,0, 2,0)*a[3][0]
						+ getDerivative(2, 2,  4, stdx, 2,0, 1,1)*a[3][0]
						+ getDerivative(2, 2,  -4, stdx, 1,1, 2,1)*a[3][0]
						+ getDerivative(2, 2,  -6, stdx, 1,0, 2,0)*a[3][0]
						+ getDerivative(2, 2,  -2, stdx, 0,0, 2,0)*a[3][0];

	//4*p10*p10-8*p10*p20+8*p20*p21-4*p21*p21
	//[4][0]
	gradient = gradient + getDerivative(2, 2,  4, stdx, 1,0, 1,0)*a[4][0]
						+ getDerivative(2, 2, -8, stdx, 1,0, 2,0)*a[4][0]
						+ getDerivative(2, 2,  8, stdx, 2,0, 2,1)*a[4][0]
						+ getDerivative(2, 2, -4, stdx, 2,1, 2,1)*a[4][0];

	//-2*p01*p02-4*p00*p11+2*p00*p02-4*p10*p01+4*p10*p11-2*p00*p00+6*p00*p01
	//[0][1]
	gradient = gradient + getDerivative(2, 2,  -2, stdx, 0,1, 0,2)*a[0][1]
						+ getDerivative(2, 2,  -4, stdx, 0,0, 1,1)*a[0][1]
						+ getDerivative(2, 2,  2,  stdx, 0,0, 0,2)*a[0][1]
						+ getDerivative(2, 2,  -4, stdx, 1,0, 0,1)*a[0][1]
						+ getDerivative(2, 2,  4,  stdx, 1,0, 1,1)*a[0][1]
						+ getDerivative(2, 2,  -2, stdx, 0,0, 0,0)*a[0][1]
						+ getDerivative(2, 2,  6,  stdx, 0,0, 0,1)*a[0][1];

	//[1][1]
	//
	gradient = gradient + getDerivative(2, 2,  1, stdx, 0,0, 1,2)*a[1][1]
						+ getDerivative(2, 2,  1, stdx, 1,0, 0,2)*a[1][1]
						+ getDerivative(2, 2,  -1, stdx, 0,1, 1,2)*a[1][1]
						+ getDerivative(2, 2,  -1, stdx, 1,1, 0,2)*a[1][1]
						+ getDerivative(2, 2,  -2, stdx, 0,0, 1,0)*a[1][1]
						+ getDerivative(2, 2,  2, stdx, 0,0, 0,1)*a[1][1]
						+ getDerivative(2, 2,  1, stdx, 1,0, 2,1)*a[1][1]
						+ getDerivative(2, 2,  1, stdx, 2,0, 1,1)*a[1][1]
						+ getDerivative(2, 2,  -1, stdx, 0,0, 2,1)*a[1][1]
						+ getDerivative(2, 2,  -1, stdx, 2,0, 0,1)*a[1][1];

	//
    //[2][1]
	gradient = gradient + getDerivative(2, 2,  (2.0/3), stdx, 0,0, 1,1)*a[2][1]
						+ getDerivative(2, 2,  (2.0/3), stdx, 1,0, 0,1)*a[2][1]
						+ getDerivative(2, 2,  (4.0/3), stdx, 1,0, 1,2)*a[2][1]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 1,1, 1,2)*a[2][1]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 1,0, 1,0)*a[2][1]
						+ getDerivative(2, 2,  (2.0/3), stdx, 0,0, 0,1)*a[2][1]
						+ getDerivative(2, 2,  (2.0/3), stdx, 1,0, 2,1)*a[2][1]
						+ getDerivative(2, 2,  (2.0/3), stdx, 2,0, 1,1)*a[2][1]
						+ getDerivative(2, 2,  (2.0/3), stdx, 2,0, 2,1)*a[2][1]
						+ getDerivative(2, 2,  (1.0/3), stdx, 0,0, 2,2)*a[2][1]
						+ getDerivative(2, 2,  (1.0/3), stdx, 2,0, 0,2)*a[2][1]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 0,0, 2,0)*a[2][1]
						+ getDerivative(2, 2,  -1, stdx, 0,0, 2,1)*a[2][1]
						+ getDerivative(2, 2,  -1, stdx, 2,0, 0,1)*a[2][1]
						+ getDerivative(2, 2,  -(1.0/3), stdx, 0,1, 2,2)*a[2][1]
						+ getDerivative(2, 2,  -(1.0/3), stdx, 2,1, 0,2)*a[2][1];


	//[3][1]
	gradient = gradient + getDerivative(2, 2,  1, stdx, 0,0, 1,1)*a[3][1]
						+ getDerivative(2, 2,  1, stdx, 1,0, 0,1)*a[3][1]
						+ getDerivative(2, 2,  1, stdx, 1,0, 2,2)*a[3][1]
						+ getDerivative(2, 2,  1, stdx, 2,0, 1,2)*a[3][1]
						+ getDerivative(2, 2,  -1, stdx, 1,1, 2,2)*a[3][1]
						+ getDerivative(2, 2,  -1, stdx, 2,1, 1,2)*a[3][1]
						+ getDerivative(2, 2,  -2, stdx, 1,0, 2,0)*a[3][1]
						+ getDerivative(2, 2,  2, stdx, 2,0, 2,1)*a[3][1]
						+ getDerivative(2, 2,  -1, stdx, 0,0, 2,1)*a[3][1]
						+ getDerivative(2, 2,  -1, stdx, 2,0, 0,1)*a[3][1];

	//[4][1]
	gradient = gradient + getDerivative(2, 2,  -2, stdx, 2,1, 2,2)*a[4][1]
						+ getDerivative(2, 2,  4,  stdx, 1,0, 1,1)*a[4][1]
						+ getDerivative(2, 2,  -4, stdx, 1,0, 2,1)*a[4][1]
						+ getDerivative(2, 2,  -2, stdx, 2,0, 2,0)*a[4][1]
						+ getDerivative(2, 2,  -4, stdx, 2,0, 1,1)*a[4][1]
						+ getDerivative(2, 2,  2,  stdx, 2,0, 2,2)*a[4][1]
						+ getDerivative(2, 2,  6,  stdx, 2,0, 2,1)*a[4][1];

	//[0][2]
	//
	gradient = gradient + getDerivative(2, 2,  -(2.0/3), stdx, 0,2, 0,2)*a[0][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 0,1, 0,2)*a[0][2]
						+ getDerivative(2, 2,  4,        stdx, 0,0, 0,2)*a[0][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 0,0, 1,2)*a[0][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 1,0, 0,2)*a[0][2]
						+ getDerivative(2, 2,  (4.0/3),  stdx, 1,0, 1,2)*a[0][2]
						+ getDerivative(2, 2,  -(16.0/3),stdx, 0,1, 1,1)*a[0][2]
						+ getDerivative(2, 2,  (8.0/3),  stdx, 1,1, 1,1)*a[0][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 0,0 ,0,0)*a[0][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 0,0, 0,1)*a[0][2]
						+ getDerivative(2, 2,  4,        stdx, 0,1, 0,1)*a[0][2];

	//[1][2]
	gradient = gradient + getDerivative(2, 2,  -(2.0/3), stdx, 0,2, 1,2)*a[1][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 0,0, 1,1)*a[1][2]
						+ getDerivative(2, 2,  (2.0/3), stdx, 0,0, 0,2)*a[1][2]
						+ getDerivative(2, 2,  1, stdx, 0,0, 1,2)*a[1][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 1,0, 0,1)*a[1][2]
						+ getDerivative(2, 2,  1, stdx, 1,0, 0,2)*a[1][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 0,1, 1,2)*a[1][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 1,1, 0,2)*a[1][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 0,0, 1,0)*a[1][2]
						+ getDerivative(2, 2,  (4.0/3), stdx, 0,1, 0,1)*a[1][2]
						+ getDerivative(2, 2,  (1.0/3), stdx, 1,0 ,2,2)*a[1][2]
						+ getDerivative(2, 2,  (1.0/3), stdx, 2,0 ,1,2)*a[1][2]
						+ getDerivative(2, 2,  (4.0/3), stdx, 1,1, 2,1)*a[1][2]
						+ getDerivative(2, 2,  -(1.0/3), stdx, 0,0, 2,2)*a[1][2]
						+ getDerivative(2, 2,  -(1.0/3), stdx, 2,0, 0,2)*a[1][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 0,1 ,2,1)*a[1][2];
	//[2][2]
	gradient = gradient + getDerivative(2, 2,  -(4.0/9), stdx, 1,2, 1,2)*a[2][2]
						+ getDerivative(2, 2,  (2.0/9), stdx, 0,0, 0,2)*a[2][2]
						+ getDerivative(2, 2,  (2.0/9), stdx, 0,0, 1,2)*a[2][2]
						+ getDerivative(2, 2,  -(8.0/9), stdx, 1,0 ,1,1)*a[2][2]
						+ getDerivative(2, 2,  (2.0/9), stdx, 1,0 ,0,2)*a[2][2]
						+ getDerivative(2, 2,  (4.0/3), stdx, 1,0 ,1,2)*a[2][2]
						+ getDerivative(2, 2,  (8.0/9), stdx, 0,1, 1,1)*a[2][2]
						+ getDerivative(2, 2,  -(8.0/9), stdx, 1,1 ,1,2)*a[2][2]
						+ getDerivative(2, 2,  -(4.0/9), stdx, 1,0, 1,0)*a[2][2]
						+ getDerivative(2, 2,  (4.0/9), stdx, 0,1 ,0,1)*a[2][2]
						+ getDerivative(2, 2,  (2.0/9), stdx, 1,0, 2,2)*a[2][2]
						+ getDerivative(2, 2,  (2.0/9), stdx, 2,0 ,1,2)*a[2][2]
						+ getDerivative(2, 2,  (2.0/9), stdx, 2,0, 2,2)*a[2][2]
						+ getDerivative(2, 2,  (8.0/9), stdx, 1,1, 2,1)*a[2][2]
						+ getDerivative(2, 2,  -(2.0/9), stdx, 2,0, 0,1)*a[2][2]
						+ getDerivative(2, 2,  (4.0/9), stdx, 2,1, 2,1)*a[2][2]
						+ getDerivative(2, 2,  -(2.0/9), stdx, 0,0, 2,0)*a[2][2]
						+ getDerivative(2, 2,  -(2.0/9), stdx, 0,0, 2,1)*a[2][2]
						+ getDerivative(2, 2,  -(2.0/9), stdx, 2,1, 0,2)*a[2][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 0,1, 2,1)*a[2][2]
						+ getDerivative(2, 2,  -(2.0/9), stdx, 0,2, 2,2)*a[2][2]
						+ getDerivative(2, 2,  -(2.0/9), stdx, 0,1, 2,2)*a[2][2];

		//[3][2]
		gradient = gradient + getDerivative(2, 2,  -(2.0/3), stdx, 1,2, 2,2)*a[3][2]
						+ getDerivative(2, 2,  (1.0/3), stdx, 0,0, 1,2)*a[3][2]
						+ getDerivative(2, 2,  (1.0/3), stdx, 1,0, 0,2)*a[3][2]
						+ getDerivative(2, 2,  (4.0/3), stdx, 0,1, 1,1)*a[3][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 1,0, 2,1)*a[3][2]
						+ getDerivative(2, 2,  1, stdx, 1,0, 2,2)*a[3][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 2,0, 1,1)*a[3][2]
						+ getDerivative(2, 2,  1, stdx, 2,0, 1,2)*a[3][2]
						+ getDerivative(2, 2,  (2.0/3), stdx, 2,0, 2,2)*a[3][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 1,1, 2,2)*a[3][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 2,1, 1,2)*a[3][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 1,0, 2,0)*a[3][2]
						+ getDerivative(2, 2,  (4.0/3), stdx, 2,1, 2,1)*a[3][2]
						+ getDerivative(2, 2,  -(1.0/3), stdx, 0,0, 2,2)*a[3][2]
						+ getDerivative(2, 2,  -(1.0/3), stdx, 2,0, 0,2)*a[3][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 0,1, 2,1)*a[3][2];

		//[4][2]
		gradient = gradient + getDerivative(2, 2,  -(4.0/3), stdx, 2,1, 2,2)*a[4][2]
						+ getDerivative(2, 2,  (4.0/3), stdx, 1,0, 1,2)*a[4][2]
						+ getDerivative(2, 2,  (8.0/3), stdx, 1,1, 1,1)*a[4][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 2,2, 2,2)*a[4][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 1,0, 2,2)*a[4][2]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 2,0, 2,0)*a[4][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 2,0, 1,2)*a[4][2]
						+ getDerivative(2, 2,  4, stdx, 2,0, 2,2)*a[4][2]
						+ getDerivative(2, 2,  -(16.0/3), stdx, 1,1, 2,1)*a[4][2]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 2,0, 2,1)*a[4][2]
						+ getDerivative(2, 2,  4, stdx, 2,1, 2,1)*a[4][2];

		//[0][3]
		gradient = gradient + getDerivative(2, 2,  -2, stdx, 0,2, 0,2)*a[0][3]
						+ getDerivative(2, 2,  6, stdx, 0,1, 0,2)*a[0][3]
						+ getDerivative(2, 2,  2, stdx, 0,0, 0,2)*a[0][3]
						+ getDerivative(2, 2,  -4, stdx, 0,1, 1,2)*a[0][3]
						+ getDerivative(2, 2,  -4, stdx, 1,1, 0,2)*a[0][3]
						+ getDerivative(2, 2,  4, stdx, 1,1, 1,2)*a[0][3]
						+ getDerivative(2, 2,  -2, stdx, 0,0, 0,1)*a[0][3];

		//[1][3]
		gradient = gradient + getDerivative(2, 2,  -2, stdx, 0,2, 1,2)*a[1][3]
						+ getDerivative(2, 2,  2, stdx, 0,1, 0,2)*a[1][3]
						+ getDerivative(2, 2,  -1, stdx, 0,0, 1,1)*a[1][3]
						+ getDerivative(2, 2,  1, stdx, 0,0, 1,2)*a[1][3]
						+ getDerivative(2, 2,  -1, stdx, 1,0, 0,1)*a[1][3]
						+ getDerivative(2, 2,  1, stdx, 1,0, 0,2)*a[1][3]
						+ getDerivative(2, 2,  1, stdx, 1,1, 2,2)*a[1][3]
						+ getDerivative(2, 2,  1, stdx, 2,1, 1,2)*a[1][3]
						+ getDerivative(2, 2,  -1, stdx, 0,1, 2,2)*a[1][3]
						+ getDerivative(2, 2,  -1, stdx, 2,1, 0,2)*a[1][3];

		//[2][3]
		gradient = gradient + getDerivative(2, 2,  -(4.0/3), stdx, 1,2, 1,2)*a[2][3]
						+ getDerivative(2, 2,  (2.0/3), stdx, 0,1, 0,2)*a[2][3]
						+ getDerivative(2, 2,  (2.0/3), stdx, 2,1, 2,2)*a[2][3]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 1,0, 1,1)*a[2][3]
						+ getDerivative(2, 2,  (4.0/3), stdx, 1,0, 1,2)*a[2][3]
						+ getDerivative(2, 2,  (2.0/3), stdx, 0,1, 1,2)*a[2][3]
						+ getDerivative(2, 2,  (2.0/3), stdx, 1,1, 0,2)*a[2][3]
						+ getDerivative(2, 2,  (2.0/3), stdx, 1,1, 2,2)*a[2][3]
						+ getDerivative(2, 2,  (2.0/3), stdx, 2,1, 1,2)*a[2][3]
						+ getDerivative(2, 2,  (1.0/3), stdx, 0,0, 2,2)*a[2][3]
						+ getDerivative(2, 2,  (1.0/3), stdx, 2,0, 0,2)*a[2][3]
						+ getDerivative(2, 2,  -(1.0/3), stdx, 0,0, 2,1)*a[2][3]
						+ getDerivative(2, 2,  -(1.0/3), stdx, 2,0, 0,1)*a[2][3]
						+ getDerivative(2, 2,  -(2.0/3), stdx, 0,2, 2,2)*a[2][3]
						+ getDerivative(2, 2,  -1, stdx, 0,1, 2,2)*a[2][3]
						+ getDerivative(2, 2,  -1, stdx, 2,1, 0,2)*a[2][3];

		//[3][3]
		gradient = gradient + getDerivative(2, 2,  -2, stdx, 1,2, 2,2)*a[3][3]
						+ getDerivative(2, 2,  2, stdx, 2,1, 2,2)*a[3][3]
						+ getDerivative(2, 2,  1, stdx, 0,1, 1,2)*a[3][3]
						+ getDerivative(2, 2,  1, stdx, 1,1, 0,2)*a[3][3]
						+ getDerivative(2, 2,  -1, stdx, 1,0, 2,1)*a[3][3]
						+ getDerivative(2, 2,  1, stdx, 1,0, 2,2)*a[3][3]
						+ getDerivative(2, 2,  -1, stdx, 2,0, 1,1)*a[3][3]
						+ getDerivative(2, 2,  1, stdx, 2,0, 1,2)*a[3][3]
						+ getDerivative(2, 2,  -1, stdx, 0,1, 2,2)*a[3][3]
						+ getDerivative(2, 2,  -1, stdx, 2,1, 0,2)*a[3][3];

		//[4][3]
		gradient = gradient + getDerivative(2, 2,  6, stdx, 2,1, 2,2)*a[4][3]
						+ getDerivative(2, 2,  4, stdx, 1,1, 1,2)*a[4][3]
						+ getDerivative(2, 2,  -2, stdx, 2,2, 2,2)*a[4][3]
						+ getDerivative(2, 2,  2, stdx, 2,0, 2,2)*a[4][3]
						+ getDerivative(2, 2,  -4, stdx, 1,1, 2,2)*a[4][3]
						+ getDerivative(2, 2,  -4, stdx, 2,1, 1,2)*a[4][3]
						+ getDerivative(2, 2,  -2, stdx, 2,0, 2,1)*a[4][3];

		//[0][4]
		gradient = gradient + getDerivative(2, 2,  -8, stdx, 0,2, 1,2)*a[0][4]
						+ getDerivative(2, 2,  4, stdx, 1,2, 1,2)*a[0][4]
						+ getDerivative(2, 2,  -4, stdx, 0,1, 0,1)*a[0][4]
						+ getDerivative(2, 2,  8, stdx, 0,1, 0,2)*a[0][4];

		//[1][4]
		gradient = gradient + getDerivative(2, 2,  2, stdx, 0,2, 0,2)*a[1][4]
						+ getDerivative(2, 2,  -6, stdx, 0,2, 1,2)*a[1][4]
						+ getDerivative(2, 2,  2, stdx, 1,2, 2,2)*a[1][4]
						+ getDerivative(2, 2,  -4, stdx, 0,1, 1,1)*a[1][4]
						+ getDerivative(2, 2,  4, stdx, 0,1, 1,2)*a[1][4]
						+ getDerivative(2, 2,  4, stdx, 1,1, 0,2)*a[1][4]
						+ getDerivative(2, 2,  -2, stdx, 0,2, 2,2)*a[1][4];

		//[2][4]
		gradient = gradient + getDerivative(2, 2,  (2.0/3), stdx, 0,2, 0,2)*a[2][4]
						+ getDerivative(2, 2,  (4.0/3), stdx, 0,2, 1,2)*a[2][4]
						+ getDerivative(2, 2,  -4, stdx, 1,2, 1,2)*a[2][4]
						+ getDerivative(2, 2,  (4.0/3), stdx, 1,2, 2,2)*a[2][4]
						+ getDerivative(2, 2,  -(8.0/3), stdx, 1,1, 1,1)*a[2][4]
						+ getDerivative(2, 2,  (16.0/3), stdx, 1,1, 1,2)*a[2][4]
						+ getDerivative(2, 2,  (2.0/3), stdx, 2,2, 2,2)*a[2][4]
						+ getDerivative(2, 2,  (4/3), stdx, 2,1, 0,2)*a[2][4]
						+ getDerivative(2, 2,  -(4.0/3), stdx, 0,1, 2,1)*a[2][4]
						+ getDerivative(2, 2,  -4, stdx, 0,2, 2,2)*a[2][4]
						+ getDerivative(2, 2,  (4.0/3), stdx, 0,1, 2,2)*a[2][4];

		//[3][4]
		gradient = gradient + getDerivative(2, 2,  2, stdx, 0,2, 1,2)*a[3][4]
						+ getDerivative(2, 2,  -6, stdx, 1,2, 2,2)*a[3][4]
						+ getDerivative(2, 2,  2, stdx, 2,2, 2,2)*a[3][4]
						+ getDerivative(2, 2,  -4, stdx, 1,1, 2,1)*a[3][4]
						+ getDerivative(2, 2,  4, stdx, 1,1, 2,2)*a[3][4]
						+ getDerivative(2, 2,  4, stdx, 2,1, 1,2)*a[3][4]
						+ getDerivative(2, 2,  -2, stdx, 0,2, 2,2)*a[3][4];

		//[4][4]
		gradient = gradient + getDerivative(2, 2,  4, stdx, 1,2, 1,2)*a[4][4]
						+ getDerivative(2, 2,  -8, stdx, 1,2, 2,2)*a[4][4]
						+ getDerivative(2, 2,  -4, stdx, 2,1, 2,1)*a[4][4]
						+ getDerivative(2, 2,  8, stdx, 2,1, 2,2)*a[4][4];


	for(int i = 0; i < 27; i++)
	{
		grad[i] = gradient[i] * 2;
		//TRACE("%f ",grad[i]);
	}
	//TRACE("\n");
}
void IsotropicEnergy::getGrad33(double &J,std::vector<double> &grad,std::vector<double> &x)
{

}