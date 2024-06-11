#include "stdafx.h"
#include "ParseTspline.h"
#include "lib3d\Base3d.h"
#include <fstream>
#include "Nurbs3D.h"
#include "C_ParseNurbs.h"
#include "GeneralBilinearTransformation.h"
#include "CompositeMobiusTransformation.h"
#include "BsplineTransformation.h"
#include "Tspline.h"
CParseTspline::CParseTspline(void)
{
}

CParseTspline::~CParseTspline(void)
{
}
void CParseTspline::Free(void)
{

}
int CParseTspline::Run(char *filename,CSceneGraph3d *pSceneGraph)
{
	char s[100];

	std::ifstream file(filename);
	Tspline* pSurf = new Tspline();
	pSurf->Read(file,filename);
	
	int m_type;
	file >> m_type;
	switch(m_type)
	{
	case generalbilinear:
		{
			GeneralBilinearTransformation *pTrans = new GeneralBilinearTransformation();
			pSurf->ptransformation = pTrans;
			pTrans->m_ReparameterizationType = generalbilinear;
			pTrans->Read(file);
			break;
		}
	case compositemobius:
		{
			CompositeMobiusTransformation *pTrans = new CompositeMobiusTransformation();
			pSurf->ptransformation = pTrans;
			pTrans->m_ReparameterizationType = compositemobius;
			pTrans->Read(file);
			break;
		}
	case Bspline:
		{
			BsplineTransformation* pTrans = new BsplineTransformation();
			pSurf->ptransformation = pTrans;
			pTrans->m_ReparameterizationType = Bspline;
			pTrans->Read(file);
			break;
		}
	}

	//Get Bounding box
	Vector3D m_lower,m_upper;
	pSurf->GetBoundingBox(m_lower,m_upper);

	Vector3D m_center;
	m_center = m_lower + m_upper;
	m_center /= -2;

	float length = m_upper.x - m_lower.x;
	if(length < m_upper.y - m_lower.y)
		length = m_upper.y - m_lower.y;
	if(length < m_upper.z - m_lower.z)
		length = m_upper.z - m_lower.z;

	length=length/2;

	CTransform transform;

	//translation
	CVector3d translation(m_center[0],m_center[1],m_center[2]);
	transform.SetTranslation(&translation);

	//scale
	CVector3d scale(1/length,1/length,1/length);
	transform.SetScale(&scale);

	if(pSceneGraph->NbObject() >= 1)
	{
		if(pSceneGraph->GetAt(0)->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pSurface = (CNurbsSuface*)pSceneGraph->GetAt(0);
			pSurf->SetTransform(*pSurface->GetTransform());
		}
		else
		if(pSceneGraph->GetAt(0)->GetType() == TYPE_MESH3D)
		{
			CMesh3d* pSurface = (CMesh3d*)pSceneGraph->GetAt(0);
			pSurf->SetTransform(*pSurface->GetTransform());
		}
	}else
		pSurf->SetTransform(transform);
	pSceneGraph->Add(pSurf);
	return 1;
}