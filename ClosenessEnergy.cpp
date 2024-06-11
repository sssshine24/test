#include "stdafx.h"
#include "ClosenessEnergy.h"
#include "C_NurbsSurf.h"
extern C_NurbsSurf* pCurOptimizationSurf;
extern C_NurbsSurf* pOriginalOptimizationSurf;
#define USAMPLINGNUM 10
#define VSAMPLINGNUM 10
ClosenessEnergy::ClosenessEnergy(real_1d_array x1):Energy(x1)
{
	x=x1;
}

ClosenessEnergy::~ClosenessEnergy(void)
{
}
void ClosenessEnergy::getGrad11(double &J,real_1d_array &grad)
{
}
void ClosenessEnergy::getGrad22(double &J,real_1d_array &grad)
{
}
void ClosenessEnergy::getBSplineGrad33(double &J,real_1d_array &grad)
{
}
 void ClosenessEnergy::getGrad33(double &J,std::vector<double> &grad,std::vector<double> &x)
{
	Point2D paraarray[VSAMPLINGNUM][USAMPLINGNUM];
	Point3D pointarray[VSAMPLINGNUM][USAMPLINGNUM];
	Point2D closestparaarray[VSAMPLINGNUM][USAMPLINGNUM];
	Point3D closestpointarray[VSAMPLINGNUM][USAMPLINGNUM];
	Point2D para;
	for(int i = 0; i <= VSAMPLINGNUM; i++)
		for(int j = 0; j <= USAMPLINGNUM; j++)
		{
			double u = j / (0.0 + USAMPLINGNUM);
			double v = i / (0.0 + VSAMPLINGNUM);

			paraarray[i][j].x = u;
			paraarray[i][j].y = v;

			Point3D point = pCurOptimizationSurf->ComputPosition(u,v);
			pointarray[i][j] = point;
			closestpointarray[i][j] = pOriginalOptimizationSurf->GetClosestPoint(point,para);
			closestparaarray[i][j] = para;
		}

	J = 0;
	for(int i = 0; i <= VSAMPLINGNUM; i++)
		for(int j = 0; j <= USAMPLINGNUM; j++)
		{
			J += (pointarray[i][j] - closestpointarray[i][j]) * (pointarray[i][j] - closestpointarray[i][j]);
		}
}