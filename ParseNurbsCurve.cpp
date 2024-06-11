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
	//这个函数用来真正的读入数据，并且运用读入的数据构造一个CNurbsface()
	//CNurbsCurve* pCurve=new CNurbsCurve();
	//std::ifstream file(filename);	//将整个文件读入file中
	//file>>pCurve->numConPoint;			//读入控制定点数
	//file>>pCurve->degree;					//读入度数
	//int numOfCon;
	/*pCurve->kont=new GLfloat[pCurve->numConPoint+pCurve->degree+1];	//初始化knotVector
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