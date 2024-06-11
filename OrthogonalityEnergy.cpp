#include "stdafx.h"
#include"OrthogonalityEnergy.h"
extern int uBezierNum;
extern int vBezierNum;

OrthogonalityEnergy::OrthogonalityEnergy(const real_1d_array & x):Energy(x)
{
	this->x=x;
	
}
void OrthogonalityEnergy::getGrad11(double &J,real_1d_array &grad)
{
	double a,b,c,d;
	Vector3D p00,p10,p01,p11;

	for(int i=0;i<3;i++)
	{
		p00[i]=x[i];
		p10[i]=x[i+3];
		p01[i]=x[i+6];
		p11[i]=x[i+9];
	}
	a = (p10 - p00) * (p01 - p00);
	b = (p10 - p00) * (p11 - p10);
	c = (p11 - p01) * (p01 - p00);
	d = (p11 - p01) * (p11 - p10);

	J=a*a+b*b+c*c+d*d;

	double aa00[3],aa10[3],aa01[3],aa11[3];		//aax00,aay00,aaz00
	for (int i=0;i<3;i++)
	{
		aa00[i]=2*p00[i]-p10[i]-p01[i];
		aa10[i]=-p00[i]+p01[i];
		aa01[i]=-p00[i]+p10[i];
		aa11[i]=0;
	}

	double ab00[3],ab10[3],ab01[3],ab11[3];		//abx00,aby00,abz00
	for (int i=0;i<3;i++)
	{
		ab00[i]=-p11[i]+p10[i];
		ab10[i]=p11[i]-2*p10[i]+p00[i];
		ab01[i]=0;
		ab11[i]=-p00[i]+p10[i];
	}

	double ac00[3],ac10[3],ac01[3],ac11[3];		//acx00,acy00,acz00
	for (int i=0;i<3;i++)
	{
		ac00[i]=-p11[i]+p01[i];
		ac10[i]=0;
		ac01[i]=p00[i]-2*p01[i]+p11[i];
		ac11[i]=-p00[i]+p01[i];
	}

	double ad00[3],ad10[3],ad01[3],ad11[3];		//acx00,acy00,acz00
	for (int i=0;i<3;i++)
	{
		ad00[i]=0;
		ad10[i]=-p11[i]+p01[i];
		ad01[i]=-p11[i]+p10[i];
		ad11[i]=2*p11[i]-p10[i]-p01[i];
	}

	grad[0]=2*a*aa00[0]+2*b*ab00[0]+2*c*ac00[0]+2*d*ad00[0];//x00
	grad[1]=2*a*aa00[1]+2*b*ab00[1]+2*c*ac00[1]+2*d*ad00[1];//y00
	grad[2]=2*a*aa00[2]+2*b*ab00[2]+2*c*ac00[2]+2*d*ad00[2];//z00

	grad[3]=2*a*aa10[0]+2*b*ab10[0]+2*c*ac10[0]+2*d*ad10[0];//x10
	grad[4]=2*a*aa10[1]+2*b*ab10[1]+2*c*ac10[1]+2*d*ad10[1];//y10
	grad[5]=2*a*aa10[2]+2*b*ab10[2]+2*c*ac10[2]+2*d*ad10[2];//z10

	grad[6]=2*a*aa01[0]+2*b*ab01[0]+2*c*ac01[0]+2*d*ad01[0];//x01
	grad[7]=2*a*aa01[1]+2*b*ab01[1]+2*c*ac01[1]+2*d*ad01[1];//y01
	grad[8]=2*a*aa01[2]+2*b*ab01[2]+2*c*ac01[2]+2*d*ad01[2];//z01

	grad[9]=2*a*aa11[0]+2*b*ab11[0]+2*c*ac11[0]+2*d*ad11[0];//x11
	grad[10]=2*a*aa11[1]+2*b*ab11[1]+2*c*ac11[1]+2*d*ad11[1];//y11
	grad[11]=2*a*aa11[2]+2*b*ab11[2]+2*c*ac11[2]+2*d*ad11[2];//z11

}


void OrthogonalityEnergy::getGrad22(double &J, alglib::real_1d_array &grad)
{

	double a[16];

	Vector3D p00(x[0],x[1],x[2]),p10(x[3],x[4],x[5]),p20(x[6],x[7],x[8]);
	Vector3D p01(x[9],x[10],x[11]),p11(x[12],x[13],x[14]),p21(x[15],x[16],x[17]);
	Vector3D p02(x[18],x[19],x[20]),p12(x[21],x[22],x[23]),p22(x[24],x[25],x[26]);
	
	
	a[0]=      4*((p10-p00)*(p01-p00));
	a[1]=(4.0/3)*((p20-p10)*(p01-p00))+(8.0/3)*((p10-p00)*(p11-p10));
	a[2]=(4.0/3)*((p10-p00)*(p21-p20))+(8.0/3)*((p20-p10)*(p11-p10));
	a[3]=      4*((p20-p10)*(p21-p20));
	a[4]=(4.0/3)*((p10-p00)*(p02-p01))+(8.0/3)*((p11-p01)*(p01-p00));
	a[5]=(4.0/9)*((p20-p10)*(p02-p01))+(8.0/9)*((p10-p00)*(p12-p11))+(8.0/9)*((p21-p11)*(p01-p00))+(16.0/9)*((p11-p01)*(p11-p10));
	a[6]=(4.0/9)*((p10-p00)*(p22-p21))+(8.0/9)*((p20-p10)*(p12-p11))+(8.0/9)*((p11-p01)*(p21-p20))+(16.0/9)*((p21-p11)*(p11-p10));
	a[7]=(4.0/3)*((p20-p10)*(p22-p21))+(8.0/3)*((p21-p11)*(p21-p20));
	a[8]=(4.0/3)*((p12-p02)*(p01-p00))+(8.0/3)*((p11-p01)*(p02-p01));
	a[9]=(4.0/9)*((p22-p12)*(p01-p00))+(8.0/9)*((p12-p02)*(p11-p10))+(8.0/9)*((p21-p11)*(p02-p01))+(16.0/9)*((p11-p01)*(p12-p11));
   a[10]=(4.0/9)*((p12-p02)*(p21-p20))+(8.0/9)*((p11-p01)*(p22-p21))+(8.0/9)*((p22-p12)*(p11-p10))+(16.0/9)*((p21-p11)*(p12-p11));
   a[11]=(4.0/3)*((p22-p12)*(p21-p20))+(8.0/3)*((p21-p11)*(p22-p21));
   a[12]=      4*((p12-p02)*(p02-p01));
   a[13]=(4.0/3)*((p22-p12)*(p02-p01))+(8.0/3)*((p12-p02)*(p12-p11));
   a[14]=(4.0/3)*((p12-p02)*(p22-p21))+(8.0/3)*((p22-p12)*(p12-p11));
   a[15]=    4*((p22-p12)*(p22-p21));


   J=0;
   for(int i=0;i<16;i++)
		J+=a[i]*a[i];

   std::vector<double> gradient;
   
   for(int i = 0; i < 27; i++)
	   gradient.push_back(0);

   std::vector<double> stdx;
   for(int i = 0; i < 27; i++)
	   stdx.push_back(x[i]);

	gradient=gradient+getDerivative(2, 2, 4,stdx,		1,0,	0,0,	0,1,	0,0)*a[0];
	gradient=gradient+getDerivative(2, 2, 4.0/3,stdx,	2,0,	1,0,	0,1,	0,0)*a[1]
	                 +getDerivative(2, 2, 8.0/3,stdx,	1,0,	0,0,	1,1,	1,0)*a[1];
	gradient=gradient+getDerivative(2, 2, 4.0/3,stdx,	1,0,	0,0,	2,1,	2,0)*a[2]
	                 +getDerivative(2, 2, 8.0/3,stdx,	2,0,	1,0,	1,1,	1,0)*a[2];
	gradient=gradient+getDerivative(2, 2, 4,stdx,		2,0,	1,0,	2,1,	2,0)*a[3];

	gradient=gradient+getDerivative(2, 2, 4.0/3,stdx,	1,0,	0,0,	0,2,	0,1)*a[4]
	                 +getDerivative(2, 2, 8.0/3,stdx,	1,1,	0,1,	0,1,	0,0)*a[4];
	gradient=gradient+getDerivative(2, 2, 8.0/9,stdx,	1,0,	0,0,	1,2,	1,1)*a[5]
	                 +getDerivative(2, 2, 4.0/9,stdx,	2,0,	1,0,	0,2,	0,1)*a[5]
					 +getDerivative(2, 2, 16.0/9,stdx,	1,1,	0,1,	1,1,	1,0)*a[5]
					 +getDerivative(2, 2, 8.0/9,stdx,	2,1,	1,1,	0,1,	0,0)*a[5];
	gradient=gradient+getDerivative(2, 2, 4.0/9,stdx,	1,0,	0,0,	2,2,	2,1)*a[6]
	                 +getDerivative(2, 2, 8.0/9,stdx,	2,0,	1,0,	1,2,	1,1)*a[6]
					 +getDerivative(2, 2, 8.0/9,stdx,	1,1,	0,1,	2,1,	2,0)*a[6]
					 +getDerivative(2, 2, 16.0/9,stdx,	2,1,	1,1,	1,1,	1,0)*a[6];
	gradient=gradient+getDerivative(2, 2, 4.0/3,stdx,	2,0,	1,0,	2,2,	2,1)*a[7]
	                 +getDerivative(2, 2, 8.0/3,stdx,	2,1,	1,1,	2,1,	2,0)*a[7];

	gradient=gradient+getDerivative(2, 2, 8.0/3,stdx,	1,1,	0,1,	0,2,	0,1)*a[8]
	                 +getDerivative(2, 2, 4.0/3,stdx,	1,2,	0,2,	0,1,	0,0)*a[8];
	gradient=gradient+getDerivative(2, 2, 16.0/9,stdx,	1,1,	0,1,	1,2,	1,1)*a[9]
	                 +getDerivative(2, 2, 8.0/9,stdx,	2,1,	1,1,	0,2,	0,1)*a[9]
					 +getDerivative(2, 2, 8.0/9,stdx,	1,2,	0,2,	1,1,	1,0)*a[9]
					 +getDerivative(2, 2, 4.0/9,stdx,	2,2,	1,2,	0,1,	0,0)*a[9];
	gradient=gradient+getDerivative(2, 2, 8.0/9,stdx,	1,1,	0,1,	2,2,	2,1)*a[10]
	                 +getDerivative(2, 2, 16.0/9,stdx,	2,1,	1,1,	1,2,	1,1)*a[10]
					 +getDerivative(2, 2, 4.0/9,stdx,	1,2,	0,2,	2,1,	2,0)*a[10]
					 +getDerivative(2, 2, 8.0/9,stdx,	2,2,	1,2,	1,1,	1,0)*a[10];
	gradient=gradient+getDerivative(2, 2, 8.0/3,stdx,	2,1,	1,1,	2,2,	2,1)*a[11]
	                 +getDerivative(2, 2, 4.0/3,stdx,	2,2,	1,2,	2,1,	2,0)*a[11];

	gradient=gradient+getDerivative(2, 2, 4,stdx,		1,2,	0,2,	0,2,	0,1)*a[12];
	gradient=gradient+getDerivative(2, 2, 8.0/3,stdx,	1,2,	0,2,	1,2,	1,1)*a[13]
	                 +getDerivative(2, 2, 4.0/3,stdx,	2,2,	1,2,	0,2,	0,1)*a[13];
	gradient=gradient+getDerivative(2, 2, 4.0/3,stdx,	1,2,	0,2,	2,2,	2,1)*a[14]
	                 +getDerivative(2, 2, 8.0/3,stdx,	2,2,	1,2,	1,2,	1,1)*a[14];
	gradient=gradient+getDerivative(2, 2, 4,stdx,		2,2,	1,2,	2,2,	2,1)*a[15];

	for(int i=0;i<27;i++)
		grad[i]=gradient[i];


}
int OrthogonalityEnergy::c(int i,int n)
{
	if(i==0)
		return 1;
	if(i==n)
		return 1;
	int nn=1;
	for(int j=1;j<=n;j++)
		nn*=j;
	int ni=1;
	for(int j=1;j<=n-i;j++)
		ni*=j;
	int ii=1;
	for(int j=1;j<=i;j++)
		ii*=j;

	return nn/(ni*ii);
	
}
int OrthogonalityEnergy::getIndex(int m, int n, int i,int j,int k)//返回点pij的下标
{
	return Energy::getIndex(m,n,i,j,k);
}
int OrthogonalityEnergy::getIndexBSpline33(int i, int j, int k)
{
	return (j*(uBezierNum*3+1)+i)*3+k;
}
int OrthogonalityEnergy::getIndexForIJBezierMNInBSpline(int i,int j,int m,int n,int k)
{
	return getIndexBSpline33(i*4+m,j*4+n,k);
}
void OrthogonalityEnergy::getBSplineGrad33(double &J, alglib::real_1d_array &grad)
{
	for(int i=0;i<grad.length();i++)
		grad[i]=0;
	//gradient.reserve()
	J=0;
	for(int i=0;i<uBezierNum;i++)
		for(int j=0;j<vBezierNum;j++)
		{
			std::vector<double> y;
			for(int i2=0;i2<48;i2++)
				y.push_back(0);
			for(int i1=0;i1<4;i1++)
				for(int j1=0;j1<4;j1++)
					for(int k=0;k<3;k++)
					{
						int i2=getIndex(3,3,i1,j1,k);
						int j2=getIndexBSpline33(3*i+i1,3*j+j1,k);
						y[getIndex(3,3,i1,j1,k)]=this->x[getIndexBSpline33(3*i+i1,3*j+j1,k)];
					}

			double J2=0;
			std::vector<double> g;
			getGrad33(J2,g,y);
			TRACE("J2:%f\n",J2);
			J+=J2;
			for(int i1=0;i1<4;i1++)
				for(int j1=0;j1<4;j1++)
					for(int k=0;k<3;k++)
					{
						grad[getIndexBSpline33(3*i+i1,3*j+j1,k)]+=g[getIndex(3,3,i1,j1,k)];
					}

		}
}

void OrthogonalityEnergy::getGrad33(double &J, std::vector<double> &grad,std::vector<double> &x)
{
	Vector3D p00(x[0],x[1],x[2]),	p10(x[3],x[4],x[5]),	p20(x[6],x[7],x[8]),	p30(x[9],x[10],x[11]);
	Vector3D p01(x[12],x[13],x[14]),p11(x[15],x[16],x[17]),	p21(x[18],x[19],x[20]),	p31(x[21],x[22],x[23]);
	Vector3D p02(x[24],x[25],x[26]),p12(x[27],x[28],x[29]),	p22(x[30],x[31],x[32]),	p32(x[33],x[34],x[35]);
	Vector3D p03(x[36],x[37],x[38]),p13(x[39],x[40],x[41]),	p23(x[42],x[43],x[44]),	p33(x[45],x[46],x[47]);

	double a[36];
	
	a[0]=9*((p00-p10)*(p00-p01));					//5050

	a[1]= (18.0/5*(p20-p10))*(p01-p00)+				//4150
		  (27.0/5*(p10-p00))*(p11-p10);

	a[2]=(9.0/10*(p30-p20))*(p01-p00)+				//3250
		(54.0/10*(p20-p10))*(p11-p10)+
		(27.0/10*(p10-p00))*(p21-p20);

	a[3]=(27.0/10*(p30-p20))*(p11-p10)+				//2350
		 (54.0/10*(p20-p10))*(p21-p20)+
		 (9.0/10*(p10-p00))*(p31-p30);

	a[4]=(27.0/5*(p30-p20))*(p21-p20)+				//1450
		 (18.0/5*(p20-p10))*(p31-p30);

	a[5]=9*(p30-p20)*(p31-p30);						//0550



	a[6]=(27.0/5*(p11-p01))*(p01-p00)+				//5041
		 (18.0/5*(p10-p00))*(p02-p01);

	a[7]=(54.0/25*(p21-p11))*(p01-p00)+				//4141
		 (36.0/25*(p20-p10))*(p02-p01)+
		(81.0/25*(p11-p01))*(p11-p10)+
		(54.0/25*(p10-p00))*(p12-p11);

	a[8]=(27.0/50*(p31-p21))*(p01-p00)+				//3241
		 (18.0/50*(p30-p20))*(p02-p01)+
		(162.0/50*(p21-p11))*(p11-p10)+
		(108.0/50*(p20-p10))*(p12-p11)+
		 (81.0/50*(p11-p01))*(p21-p20)+
		 (54.0/50*(p10-p00))*(p22-p21);

	a[9]=(81.0/50*(p31-p21))*(p11-p10)+				//2341
		 (54.0/50*(p30-p20))*(p12-p11)+
		(162.0/50*(p21-p11))*(p21-p20)+
		(108.0/50*(p20-p10))*(p22-p21)+
		 (27.0/50*(p11-p01))*(p31-p30)+
		(18.0/50*(p10-p00))*(p32-p31);

	a[10]=(81.0/25*(p31-p21))*(p21-p20)+			//1441
		  (54.0/25*(p30-p20))*(p22-p21)+
		  (54.0/25*(p21-p11))*(p31-p30)+
		  (36.0/25*(p20-p10))*(p32-p31);

	a[11]=(27.0/5*(p31-p21))*(p31-p30)+				//0541
		  (18.0/5*(p30-p20))*(p32-p31);



	a[12]=(27.0/10*(p12-p02))*(p01-p00)+			//5032
		  (54.0/10*(p11-p01))*(p02-p01)+
		   (9.0/10*(p10-p00))*(p03-p02);

	a[13]=(54.0/50*(p22-p12))*(p01-p00)+			//4132
		 (108.0/50*(p21-p11))*(p02-p01)+
		  (18.0/50*(p20-p10))*(p03-p02)+
		  (81.0/50*(p12-p02))*(p11-p10)+
		 (162.0/50*(p11-p01))*(p12-p11)+
		  (27.0/50*(p10-p00))*(p13-p12);

	a[14]=(27.0/100*(p32-p22))*(p01-p00)+			//3232
		  (54.0/100*(p31-p21))*(p02-p01)+
		   (9.0/100*(p30-p20))*(p03-p02)+
		 (162.0/100*(p22-p12))*(p11-p10)+
		 (324.0/100*(p21-p11))*(p12-p11)+
		  (54.0/100*(p20-p10))*(p13-p12)+
		  (81.0/100*(p12-p02))*(p21-p20)+
		 (162.0/100*(p11-p01))*(p22-p21)+
		  (27.0/100*(p10-p00))*(p23-p22);

	a[15]=(81.0/100*(p32-p22))*(p11-p10)+			//2332
		 (162.0/100*(p31-p21))*(p12-p11)+
		  (27.0/100*(p30-p20))*(p13-p12)+
		 (162.0/100*(p22-p12))*(p21-p20)+
		 (324.0/100*(p21-p11))*(p22-p21)+
		  (54.0/100*(p20-p10))*(p23-p22)+
		  (27.0/100*(p12-p02))*(p31-p30)+
		  (54.0/100*(p11-p01))*(p32-p31)+
		   (9.0/100*(p10-p00))*(p33-p32);

	a[16]=(81.0/50*(p32-p22))*(p21-p20)+             //1432
		 (162.0/50*(p31-p21))*(p22-p21)+
		  (27.0/50*(p30-p20))*(p23-p22)+
		  (54.0/50*(p22-p12))*(p31-p30)+
		 (108.0/50*(p21-p11))*(p32-p31)+
		  (18.0/50*(p20-p10))*(p33-p32);

	a[17]=(27.0/10*(p32-p22))*(p31-p30)+            //0532
		  (54.0/10*(p31-p21))*(p32-p31)+
		   (9.0/10*(p30-p20))*(p33-p32);


	a[18]=(9.0/10*(p13-p03))*(p01-p00)+             //5023
		 (54.0/10*(p12-p02))*(p02-p01)+
		 (27.0/10*(p11-p01))*(p03-p02);


	a[19]=(18.0/50*(p23-p13))*(p01-p00)+            //4123
		 (108.0/50*(p22-p12))*(p02-p01)+
		  (54.0/50*(p21-p11))*(p03-p02)+
		  (27.0/50*(p13-p03))*(p11-p10)+
		 (162.0/50*(p12-p02))*(p12-p11)+
		  (81.0/50*(p11-p01))*(p13-p12);

	a[20]=(9.0/100*(p33-p23))*(p01-p00)+            //3223
	  	 (54.0/100*(p32-p22))*(p02-p01)+
		 (27.0/100*(p31-p21))*(p03-p02)+
		 (54.0/100*(p23-p13))*(p11-p10)+
		(324.0/100*(p22-p12))*(p12-p11)+
		(162.0/100*(p21-p11))*(p13-p12)+
		 (27.0/100*(p13-p03))*(p21-p20)+
		(162.0/100*(p12-p02))*(p22-p21)+
		 (81.0/100*(p11-p01))*(p23-p22);

	a[21]=(27.0/100*(p33-p23))*(p11-p10)+         //2323
		 (162.0/100*(p32-p22))*(p12-p11)+
		  (81.0/100*(p31-p21))*(p13-p12)+
		  (54.0/100*(p23-p13))*(p21-p20)+
		 (324.0/100*(p22-p12))*(p22-p21)+
		 (162.0/100*(p21-p11))*(p23-p22)+
		   (9.0/100*(p13-p03))*(p31-p30)+
		  (54.0/100*(p12-p02))*(p32-p31)+
		  (27.0/100*(p11-p01))*(p33-p32);

	a[22]=(27.0/50*(p33-p23))*(p21-p20)+          //1423
		 (162.0/50*(p32-p22))*(p22-p21)+
		  (81.0/50*(p31-p21))*(p23-p22)+
		  (18.0/50*(p23-p13))*(p31-p30)+
		 (108.0/50*(p22-p12))*(p32-p31)+
		  (54.0/50*(p21-p11))*(p33-p32);

	a[23]=(9.0/10*(p33-p23))*(p31-p30)+          //0523
		 (54.0/10*(p32-p22))*(p32-p31)+
		 (27.0/10*(p31-p21))*(p33-p32);

	a[24]=(18.0/5*(p13-p03))*(p02-p01)+          //5014
		  (27.0/5*(p12-p02))*(p03-p02);

	a[25]=(36.0/25*(p23-p13))*(p02-p01)+		//4114
		  (54.0/25*(p22-p12))*(p03-p02)+
		  (54.0/25*(p13-p03))*(p12-p11)+
		  (81.0/25*(p12-p02))*(p13-p12);

	a[26]=(18.0/50*(p33-p23))*(p02-p01)+		//3214
		  (27.0/50*(p32-p22))*(p03-p02)+
		 (108.0/50*(p23-p13))*(p12-p11)+
		 (162.0/50*(p22-p12))*(p13-p12)+
		  (54.0/50*(p13-p03))*(p22-p21)+
		  (81.0/50*(p12-p02))*(p23-p22);

	a[27]=(54.0/50*(p33-p23))*(p12-p11)+		//2314
		  (81.0/50*(p32-p22))*(p13-p12)+
		 (108.0/50*(p23-p13))*(p22-p21)+
		 (162.0/50*(p22-p12))*(p23-p22)+
		  (18.0/50*(p13-p03))*(p32-p31)+
		  (27.0/50*(p12-p02))*(p33-p32);


	a[28]=(54.0/25*(p33-p23))*(p22-p21)+		//1414	
		(81.0/25*(p32-p22))*(p23-p22)+
		(36.0/25*(p23-p13))*(p32-p31)+
		(54.0/25*(p22-p12))*(p33-p32);


	a[29]=(18.0/5*(p33-p23))*(p32-p31)+			//0514
		  (27.0/5*(p32-p22))*(p33-p32);

	a[30]=9*(p13-p03)*(p03-p02);				//5005

	a[31]=(18.0/5*(p23-p13))*(p03-p02)+			//4105
		  (27.0/5*(p13-p03))*(p13-p12);

	a[32]=(9.0/10*(p33-p23))*(p03-p02)+			//3205
		 (54.0/10*(p23-p13))*(p13-p12)+
		 (27.0/10*(p13-p03))*(p23-p22);

	a[33]=(27.0/10*(p33-p23))*(p13-p12)+		//2305
		  (54.0/10*(p23-p13))*(p23-p22)+
		  (9.0/10*(p13-p03))*(p33-p32);

	a[34]=(27.0/5*(p33-p23))*(p23-p22)+			//1405
		  (18.0/5*(p23-p13))*(p33-p32);

	a[35]=9*(p33-p23)*(p33-p32);				//0505

	J=0;
	for(int i=0;i<36;i++)
		J+=a[i]*a[i];

/************************************************************************/
/*                   grad                                                   */
/************************************************************************/

	std::vector<double> gradient;

	for(int i = 0; i < 48; i++)
		gradient.push_back(0);

	gradient=gradient+getDerivative(3, 3, 9,x,	0,0,	1,0,	0,0,	0,1)*a[0];//5050

	gradient=gradient+getDerivative(3, 3, 18.0/5,x,	2,0,	1,0,	0,1,	0,0)*a[1]
					 +getDerivative(3, 3, 27.0/5,x,	1,0,	0,0,	1,1,	1,0)*a[1];//4150

	gradient=gradient+getDerivative(3, 3, 9.0/10,x,		3,0,	2,0,	0,1,	0,0)*a[2]
					 +getDerivative(3, 3, 54.0/10,x,	2,0,	1,0,	1,1,	1,0)*a[2]
					 +getDerivative(3, 3, 27.0/10,x,	1,0,	0,0,	2,1,	2,0)*a[2];//3250

	gradient=gradient+getDerivative(3, 3, 27.0/10,x,	3,0,	2,0,	1,1,	1,0)*a[3]
					 +getDerivative(3, 3, 54.0/10,x,	2,0,	1,0,	2,1,	2,0)*a[3]
					 +getDerivative(3, 3, 9.0/10,x,		1,0,	0,0,	3,1,	3,0)*a[3];//2350

	gradient=gradient+getDerivative(3, 3, 27.0/5,x,		3,0,	2,0,	2,1,	2,0)*a[4]
					 +getDerivative(3, 3, 18.0/5,x,		2,0,	1,0,	3,1,	3,0)*a[4];//1450


	gradient=gradient+getDerivative(3, 3, 9,x,		3,0,	2,0,	3,1,	3,0)*a[5];//0550

	gradient=gradient+getDerivative(3, 3, 27.0/5,x,	1,1,	0,1,	0,1,	0,0)*a[6]
					 +getDerivative(3, 3, 18.0/5,x,	1,0,	0,0,	0,2,	0,1)*a[6];//5041

	gradient=gradient+getDerivative(3, 3, 54.0/25,x,	2,1,	1,1,	0,1,	0,0)*a[7]
					 +getDerivative(3, 3, 36.0/25,x,	2,0,	1,0,	0,2,	0,1)*a[7]
					 +getDerivative(3, 3, 81.0/25,x,	1,1,	0,1,	1,1,	1,0)*a[7]
					 +getDerivative(3, 3, 54.0/25,x,	1,0,	0,0,	1,2,	1,1)*a[7];//4141


	gradient=gradient+getDerivative(3, 3, 27.0/50,x,	3,1,	2,1,	0,1,	0,0)*a[8]
					 +getDerivative(3, 3, 18.0/50,x,	3,0,	2,0,	0,2,	0,1)*a[8]
					 +getDerivative(3, 3, 162.0/50,x,	2,1,	1,1,	1,1,	1,0)*a[8]
					 +getDerivative(3, 3, 108.0/50,x,	2,0,	1,0,	1,2,	1,1)*a[8]
					 +getDerivative(3, 3, 81.0/50,x,	1,1,	0,1,	2,1,	2,0)*a[8]
					 +getDerivative(3, 3, 54.0/50,x,	1,0,	0,0,	2,2,	2,1)*a[8];//3241


	gradient=gradient+getDerivative(3, 3, 81.0/50,x,	3,1,	2,1,	1,1,	1,0)*a[9]
					 +getDerivative(3, 3, 54.0/50,x,	3,0,	2,0,	1,2,	1,1)*a[9]
					 +getDerivative(3, 3, 162.0/50,x,	2,1,	1,1,	2,1,	2,0)*a[9]
					 +getDerivative(3, 3, 108.0/50,x,	2,0,	1,0,	2,2,	2,1)*a[9]
					 +getDerivative(3, 3, 27.0/50,x,	1,1,	0,1,	3,1,	3,0)*a[9]
					 +getDerivative(3, 3, 18.0/50,x,	1,0,	0,0,	3,2,	3,1)*a[9];//2341


	gradient=gradient+getDerivative(3, 3, 81.0/25,x,	3,1,	2,1,	2,1,	2,0)*a[10]
					 +getDerivative(3, 3, 54.0/25,x,	3,0,	2,0,	2,2,	2,1)*a[10]
					 +getDerivative(3, 3, 54.0/25,x,	2,1,	1,1,	3,1,	3,0)*a[10]
					 +getDerivative(3, 3, 36.0/25,x,	2,0,	1,0,	3,2,	3,1)*a[10];//1441

	gradient=gradient+getDerivative(3, 3, 27.0/5,x,	3,1,	2,1,	3,1,	3,0)*a[11]
					 +getDerivative(3, 3, 18.0/5,x,	3,0,	2,0,	3,2,	3,1)*a[11];//0541

	gradient=gradient+getDerivative(3, 3, 27.0/10,x,	1,2,	0,2,	0,1,	0,0)*a[12]
					 +getDerivative(3, 3, 54.0/10,x,	1,1,	0,1,	0,2,	0,1)*a[12]
					 +getDerivative(3, 3, 9.0/10,x,		1,0,	0,0,	0,3,	0,2)*a[12];//5032

	gradient=gradient+getDerivative(3, 3, 54.0/50,x,	2,2,	1,2,	0,1,	0,0)*a[13]
					 +getDerivative(3, 3, 108.0/50,x,	2,1,	1,1,	0,2,	0,1)*a[13]
					 +getDerivative(3, 3, 18.0/50,x,	2,0,	1,0,	0,3,	0,2)*a[13]
					 +getDerivative(3, 3, 81.0/50,x,	1,2,	0,2,	1,1,	1,0)*a[13]
					 +getDerivative(3, 3, 162.0/50,x,	1,1,	0,1,	1,2,	1,1)*a[13]
					 +getDerivative(3, 3, 27.0/50,x,	1,0,	0,0,	1,3,	1,2)*a[13];//4132

	gradient=gradient+getDerivative(3, 3, 27.0/100,x,	3,2,	2,2,	0,1,	0,0)*a[14]
					 +getDerivative(3, 3, 54.0/100,x,	3,1,	2,1,	0,2,	0,1)*a[14]
					 +getDerivative(3, 3, 9.0/100,x,	3,0,	2,0,	0,3,	0,2)*a[14]
					 +getDerivative(3, 3, 162.0/100,x,	2,2,	1,2,	1,1,	1,0)*a[14]
					 +getDerivative(3, 3, 324.0/100,x,	2,1,	1,1,	1,2,	1,1)*a[14]
					 +getDerivative(3, 3, 54.0/100,x,	2,0,	1,0,	1,3,	1,2)*a[14]
					 +getDerivative(3, 3, 81.0/100,x,	1,2,	0,2,	2,1,	2,0)*a[14]
					 +getDerivative(3, 3, 162.0/100,x,	1,1,	0,1,	2,2,	2,1)*a[14]
					 +getDerivative(3, 3, 27.0/100,x,	1,0,	0,0,	2,3,	2,2)*a[14];//3232

	gradient=gradient+getDerivative(3, 3, 81.0/100,x,	3,2,	2,2,	1,1,	1,0)*a[15]
					 +getDerivative(3, 3, 162.0/100,x,	3,1,	2,1,	1,2,	1,1)*a[15]
					 +getDerivative(3, 3, 27.0/100,x,	3,0,	2,0,	1,3,	1,2)*a[15]
					 +getDerivative(3, 3, 162.0/100,x,	2,2,	1,2,	2,1,	2,0)*a[15]
					 +getDerivative(3, 3, 324.0/100,x,	2,1,	1,1,	2,2,	2,1)*a[15]
					 +getDerivative(3, 3, 54.0/100,x,	2,0,	1,0,	2,3,	2,2)*a[15]
					 +getDerivative(3, 3, 27.0/100,x,	1,2,	0,2,	3,1,	3,0)*a[15]
					 +getDerivative(3, 3, 54.0/100,x,	1,1,	0,1,	3,2,	3,1)*a[15]
					 +getDerivative(3, 3, 9.0/100,x,	1,0,	0,0,	3,3,	3,2)*a[15];//2332

	gradient=gradient+getDerivative(3, 3, 81.0/50,x,	3,2,	2,2,	2,1,	2,0)*a[16]
					 +getDerivative(3, 3, 162.0/50,x,	3,1,	2,1,	2,2,	2,1)*a[16]
					 +getDerivative(3, 3, 27.0/50,x,	3,0,	2,0,	2,3,	2,2)*a[16]
					 +getDerivative(3, 3, 54.0/50,x,	2,2,	1,2,	3,1,	3,0)*a[16]
					 +getDerivative(3, 3, 108.0/50,x,	2,1,	1,1,	3,2,	3,1)*a[16]
					 +getDerivative(3, 3, 18.0/50,x,	2,0,	1,0,	3,3,	3,2)*a[16];//1432

	gradient=gradient+getDerivative(3, 3, 27.0/10,x,	3,2,	2,2,	3,1,	3,0)*a[17]
					 +getDerivative(3, 3, 54.0/10,x,	3,1,	2,1,	3,2,	3,1)*a[17]
					 +getDerivative(3, 3, 9.0/10,x,		3,0,	2,0,	3,3,	3,2)*a[17];//0532

	gradient=gradient+getDerivative(3, 3, 9.0/10,x,		1,3,	0,3,	0,1,	0,0)*a[18]
					 +getDerivative(3, 3, 54.0/10,x,	1,2,	0,2,	0,2,	0,1)*a[18]
					 +getDerivative(3, 3, 27.0/10,x,	1,1,	0,1,	0,3,	0,2)*a[18];//5023

	gradient=gradient+getDerivative(3, 3, 18.0/50,x,	2,3,	1,3,	0,1,	0,0)*a[19]
					 +getDerivative(3, 3, 108.0/50,x,	2,2,	1,2,	0,2,	0,1)*a[19]
					 +getDerivative(3, 3, 54.0/50,x,	2,1,	1,1,	0,3,	0,2)*a[19]
					 +getDerivative(3, 3, 27.0/50,x,	1,3,	0,3,	1,1,	1,0)*a[19]
					 +getDerivative(3, 3, 162.0/50,x,	1,2,	0,2,	1,2,	1,1)*a[19]
					 +getDerivative(3, 3, 81.0/50,x,	1,1,	0,1,	1,3,	1,2)*a[19];//4123
	
	gradient=gradient+getDerivative(3, 3, 9.0/100,x,	3,3,	2,3,	0,1,	0,0)*a[20]
					 +getDerivative(3, 3, 54.0/100,x,	3,2,	2,2,	0,2,	0,1)*a[20]
					 +getDerivative(3, 3, 27.0/100,x,	3,1,	2,1,	0,3,	0,2)*a[20]
					 +getDerivative(3, 3, 54.0/100,x,	2,3,	1,3,	1,1,	1,0)*a[20]
					 +getDerivative(3, 3, 324.0/100,x,	2,2,	1,2,	1,2,	1,1)*a[20]
					 +getDerivative(3, 3, 162.0/100,x,	2,1,	1,1,	1,3,	1,2)*a[20]
					 +getDerivative(3, 3, 27.0/100,x,	1,3,	0,3,	2,1,	2,0)*a[20]
					 +getDerivative(3, 3, 162.0/100,x,	1,2,	0,2,	2,2,	2,1)*a[20]
					 +getDerivative(3, 3, 81.0/100,x,	1,1,	0,1,	2,3,	2,2)*a[20];//3223

	gradient=gradient+getDerivative(3, 3, 27.0/100,x,	3,3,	2,3,	1,1,	1,0)*a[21]
					 +getDerivative(3, 3, 162.0/100,x,	3,2,	2,2,	1,2,	1,1)*a[21]
					 +getDerivative(3, 3, 81.0/100,x,	3,1,	2,1,	1,3,	1,2)*a[21]
					 +getDerivative(3, 3, 54.0/100,x,	2,3,	1,3,	2,1,	2,0)*a[21]
					 +getDerivative(3, 3, 324.0/100,x,	2,2,	1,2,	2,2,	2,1)*a[21]
					 +getDerivative(3, 3, 162.0/100,x,	2,1,	1,1,	2,3,	2,2)*a[21]
					 +getDerivative(3, 3, 9.0/100,x,	1,3,	0,3,	3,1,	3,0)*a[21]
					 +getDerivative(3, 3, 54.0/100,x,	1,2,	0,2,	3,2,	3,1)*a[21]
					 +getDerivative(3, 3, 27.0/100,x,	1,1,	0,1,	3,3,	3,2)*a[21];//2323

	gradient=gradient+getDerivative(3, 3, 27.0/50,x,	3,3,	2,3,	2,1,	2,0)*a[22]
					 +getDerivative(3, 3, 162.0/50,x,	3,2,	2,2,	2,2,	2,1)*a[22]
					 +getDerivative(3, 3, 81.0/50,x,	3,1,	2,1,	2,3,	2,2)*a[22]
					 +getDerivative(3, 3, 18.0/50,x,	2,3,	1,3,	3,1,	3,0)*a[22]
					 +getDerivative(3, 3, 108.0/50,x,	2,2,	1,2,	3,2,	3,1)*a[22]
					 +getDerivative(3, 3, 54.0/50,x,	2,1,	1,1,	3,3,	3,2)*a[22];//1423

	gradient=gradient+getDerivative(3, 3, 9.0/10,x,		3,3,	2,3,	3,1,	3,0)*a[23]
					 +getDerivative(3, 3, 54.0/10,x,	3,2,	2,2,	3,2,	3,1)*a[23]
					 +getDerivative(3, 3, 27.0/10,x,	3,1,	2,1,	3,3,	3,2)*a[23];//0523
					
	gradient=gradient+getDerivative(3, 3, 18.0/5,x,		1,3,	0,3,	0,2,	0,1)*a[24]
					 +getDerivative(3, 3, 27.0/5,x,		1,2,	0,2,	0,3,	0,2)*a[24];//5014

	gradient=gradient+getDerivative(3, 3, 36.0/25,x,	2,3,	1,3,	0,2,	0,1)*a[25]
					 +getDerivative(3, 3, 54.0/25,x,	2,2,	1,2,	0,3,	0,2)*a[25]
				     +getDerivative(3, 3, 54.0/25,x,	1,3,	0,3,	1,2,	1,1)*a[25]
					 +getDerivative(3, 3, 81.0/25,x,	1,2,	0,2,	1,3,	1,2)*a[25];//4114

	gradient=gradient+getDerivative(3, 3, 18.0/50,x,	3,3,	2,3,	0,2,	0,1)*a[26]
					 +getDerivative(3, 3, 27.0/50,x,	3,2,	2,2,	0,3,	0,2)*a[26]
					 +getDerivative(3, 3, 108.0/50,x,	2,3,	1,3,	1,2,	1,1)*a[26]
					 +getDerivative(3, 3, 162.0/50,x,	2,2,	1,2,	1,3,	1,2)*a[26]
					 +getDerivative(3, 3, 54.0/50,x,	1,3,	0,3,	2,2,	2,1)*a[26]
					 +getDerivative(3, 3, 81.0/50,x,	1,2,	0,2,	2,3,	2,2)*a[26];//3214


	gradient=gradient+getDerivative(3, 3, 54.0/50,x,	3,3,	2,3,	1,2,	1,1)*a[27]
					 +getDerivative(3, 3, 81.0/50,x,	3,2,	2,2,	1,3,	1,2)*a[27]
					 +getDerivative(3, 3, 108.0/50,x,	2,3,	1,3,	2,2,	2,1)*a[27]
					 +getDerivative(3, 3, 162.0/50,x,	2,2,	1,2,	2,3,	2,2)*a[27]
					 +getDerivative(3, 3, 18.0/50,x,	1,3,	0,3,	3,2,	3,1)*a[27]
					 +getDerivative(3, 3, 27.0/50,x,	1,2,	0,2,	3,3,	3,2)*a[27];//2314

	gradient=gradient+getDerivative(3, 3, 54.0/25,x,	3,3,	2,3,	2,2,	2,1)*a[28]
					 +getDerivative(3, 3, 81.0/25,x,	3,2,	2,2,	2,3,	2,2)*a[28]
					 +getDerivative(3, 3, 36.0/25,x,	2,3,	1,3,	3,2,	3,1)*a[28]
					 +getDerivative(3, 3, 54.0/25,x,	2,2,	1,2,	3,3,	3,2)*a[28];//1414

	gradient=gradient+getDerivative(3, 3, 18.0/5,x,	3,3,	2,3,	3,2,	3,1)*a[29]
					 +getDerivative(3, 3, 27.0/5,x,	3,2,	2,2,	3,3,	3,2)*a[29];//0514

	gradient=gradient+getDerivative(3, 3, 9.0/1,x,	1,3,	0,3,	0,3,	0,2)*a[30];//5005

	gradient=gradient+getDerivative(3, 3, 18.0/5,x,	2,3,	1,3,	0,3,	0,2)*a[31]
					 +getDerivative(3, 3, 27.0/5,x,	1,3,	0,3,	1,3,	1,2)*a[31];//4105

	gradient=gradient+getDerivative(3, 3, 9.0/10,x,		3,3,	2,3,	0,3,	0,2)*a[32]
					 +getDerivative(3, 3, 54.0/10,x,	2,3,	1,3,	1,3,	1,2)*a[32]
					 +getDerivative(3, 3, 27.0/10,x,	1,3,	0,3,	2,3,	2,2)*a[32];//3205

	gradient=gradient+getDerivative(3, 3, 27.0/10,x,	3,3,	2,3,	1,3,	1,2)*a[33]
					 +getDerivative(3, 3, 54.0/10,x,	2,3,	1,3,	2,3,	2,2)*a[33]
					 +getDerivative(3, 3, 9.0/10,x,		1,3,	0,3,	3,3,	3,2)*a[33];//2305

	gradient=gradient+getDerivative(3, 3, 27.0/5,x,		3,3,	2,3,	2,3,	2,2)*a[34]
					 +getDerivative(3, 3, 18.0/5,x,		2,3,	1,3,	3,3,	3,2)*a[34];//1405

	gradient=gradient+getDerivative(3, 3, 9.0/1,x,		3,3,	2,3,	3,3,	3,2)*a[35];//0505

	for(int i=0;i<gradient.size();i++)
	{
		grad.push_back(gradient[i]);
	}

}