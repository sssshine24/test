#include "stdafx.h"
#include "ParseNurbsCurve.h"
#include "NurbsCurve.h"
#include <fstream>

CParseNurbsCurve::CParseNurbsCurve(void)
{
}

CParseNurbsCurve::~CParseNurbsCurve(void)
{
}
int CParseNurbsCurve::Run(char *filename, CSceneGraph3d *pSceneGraph)
{
	//����������������Ķ������ݣ��������ö�������ݹ���һ��CNurbsface()
	//CNurbsCurve* pCurve=new CNurbsCurve();
	//std::ifstream file(filename);	//�������ļ�����file��
	//file>>pCurve->numConPoint;			//������ƶ�����
	//file>>pCurve->degree;					//�������
	//int numOfCon;
	/*pCurve->kont=new GLfloat[pCurve->numConPoint+pCurve->degree+1];	//��ʼ��knotVector
	for(int i=0;i<pCurve->numConPoint+pCurve->degree+1;i++)
	{
		file>>pCurve->kont[i];
	}
	GLfloat (*conVertex4f)[4]=new GLfloat [numOfConPoints+degree+1][4];
	for(int i=0;i<numOfConPoints+degree+1;i++)
	{
		file>>conVertex4f[i][0]>>conVertex4f[i][1]>>conVertex4f[i][2]>>conVertex4f[i][3];
	}

*/
	return 1;
}