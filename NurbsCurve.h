#pragma once
#include "lib3d\object3d.h"
class C_NurbsCurve3D;
#include "lib3d\Transform.h"
class CNurbsCurve :public CObject3d
{
public:

	//the pointer of the curve
	C_NurbsCurve3D *pCurve;
	int m_showControlPoints;
public:
	/*提供的函数*/
	CNurbsCurve();
	~CNurbsCurve(void);

	CTransform	 m_Transform;
	int glDraw();
	void glDrawControlPoints();
	int m_Show;
	double c_r,c_g,c_b;
};
