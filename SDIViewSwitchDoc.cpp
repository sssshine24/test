// SDIViewSwitchDoc.cpp : implementation of the CSDIViewSwitchDoc class

#include "stdafx.h"
#include "lib3d\Base3d.h"
#include "SDIViewSwitch.h"
#include "SDIViewSwitchDoc.h"
#include "MainFrm.h"
#include "math.h"
#include "C_ParseNurbs.h"
#include <fstream>
#include "FormView3.h"
#include "Nurbs3d.h"
#include "ParseM.h"
#include "ParseNurbsCurve.h"
#include "MyTreeView.h"
#include "lib3d/mesh3d_openmesh.h"
#include "OpenMesh.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include "ParseTspline.h"
#include "stepInputAPI.h"
#include "ParsePOLY.h"
#include "CParseSTEP.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
double CSDIViewSwitchDoc::SWITCHSPLIT_FILE_SIGNATURE = 0x1234567890123; //static member variable

IMPLEMENT_DYNCREATE(CSDIViewSwitchDoc, CDocument)

BEGIN_MESSAGE_MAP(CSDIViewSwitchDoc, CDocument)
	//{{AFX_MSG_MAP(CSDIViewSwitchDoc)
	ON_COMMAND(ID_GL_ADD_WIREFRAME, OnGlAddWireframe)
	ON_UPDATE_COMMAND_UI(ID_GL_ADD_WIREFRAME, OnUpdateGlAddWireframe)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_GL_SMOOTH, OnGlSmooth)
	ON_UPDATE_COMMAND_UI(ID_GL_SMOOTH, OnUpdateGlSmooth)
	ON_COMMAND(ID_MESH_LOOP, OnMeshLoop)
	ON_UPDATE_COMMAND_UI(ID_MESH_LOOP, OnUpdateMeshLoop)
	ON_COMMAND(ID_GL_LIGHT, OnGlLight)
	ON_COMMAND(ID_GL_LINE, OnGlLine)
	ON_UPDATE_COMMAND_UI(ID_GL_LINE, OnUpdateGlLine)
	ON_UPDATE_COMMAND_UI(ID_GL_LIGHT, OnUpdateGlLight)
	ON_COMMAND(ID_GL_VERTEX, OnGlVertex)
	ON_UPDATE_COMMAND_UI(ID_GL_VERTEX, OnUpdateGlVertex)
	ON_COMMAND(ID_GL_FACE, OnGlFace)
	ON_UPDATE_COMMAND_UI(ID_GL_FACE, OnUpdateGlFace)
	ON_COMMAND(ID_MESH_COLOR_HEIGHT, OnMeshColorHeight)
	ON_BN_CLICKED(IDC_NavigateStartPosition, OnBnClickedNavigatestartposition)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CSDIViewSwitchDoc* pDoc;
extern int m_ShowCoordinateAxis;
extern CString m_FileTextureName;
CSDIViewSwitchDoc::CSDIViewSwitchDoc()
{
	TRACE("\n\n-------\n");
	int number = 25;
	while (number > 0)
	{
		TRACE("A%d\n", --number % 4);
		number /= 5;
	}
	TRACE("\n\n-------\n");
	//TRACE("%f,%f,%f,%f,%f,%f",a,a +l,a+2*l,a+3*l,a+4*l,b);

	pDoc = this;
	m_bAddpoint = 0;
	m_Arraynum = 0;
	m_nNumberOfRecords = 0;
	m_nRecordLength = sizeof(tagDOC_RECORD);
	m_nHeaderSize = sizeof(m_nNumberOfRecords) + sizeof(SWITCHSPLIT_FILE_SIGNATURE);
	m_DiscNum = 10;
	m_Reparemeterization = 0;
	m_AddWireframe = 1;
	m_Smooth = 1;
	m_PolygonOffset = -1.0f;
	m_Light = TRUE;
	m_Mode = GL_FILL;
	m_Disc1 = 8;
	m_Disc2 = 30;
	omega = 1;
	ShowTexture = 0;
	m_IsoParameterCurve = FALSE;
	m_ShowCoordinateAxis = 0;
	m_UsingOpenglNurbsRender = TRUE;
	m_Tessellation = 50;
	m_SetCameraMode = 0;

	m_FileTextureName = "D:\\papers\\code\\data\\texture\\circles_1.bmp";

	//
	//OnGenerateNewSurface();
}

/////////////////////////////////////////////////////////////////////////////
CSDIViewSwitchDoc::~CSDIViewSwitchDoc()
{
	TRACE("destructing CSDIViewSwitchDoc\n");
}
/////////////////////////////////////////////////////////////////////////////
//a virtual member function of CDocument: 
BOOL CSDIViewSwitchDoc::OnNewDocument()
{ 
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//This application uses a transaction method of reading and writing data from the document.
//If your app does not require this, it may be much easier to do just use the MFC framework's
//default way of doing this and implement the Serialize() function. 
void CSDIViewSwitchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

void CSDIViewSwitchDoc::OnGenerateBezier21Surface1()
{
	m_SceneGraph.Free();
	CNurbsSuface* pSurf=new CNurbsSuface();
	pSurf->m_uknot=new GLfloat[6];
	pSurf->m_vknot=new GLfloat[4];

	pSurf->m_udegree = 2;
	pSurf->m_vdegree = 1;

	pSurf->m_uknotcount = 6;
	pSurf->m_vknotcount = 4;

	for(int i = 0; i < 3; i++)
	{
		pSurf->m_uknot[i] = 0;
		pSurf->m_uknot[i + 3] = 1;
	}

	for(int i = 0; i < 2; i++)
	{
		pSurf->m_vknot[i] = 0;
		pSurf->m_vknot[i + 2] = 1;
	}
	//
	Vector3D p0(0,0,0);
	Vector3D p1(1,1,0);
	Vector3D p2(2,0,0);
	Vector3D p3(0,0,2);
	Vector3D p4(1,1,2);
	Vector3D p5(2,0,2);

	pSurf->m_ControlPoint[0][0][0] = p0[0];
	pSurf->m_ControlPoint[0][0][1] = p0[1];
	pSurf->m_ControlPoint[0][0][2] = p0[2];
	pSurf->m_ControlPoint[0][0][3] = 1;

	pSurf->m_ControlPoint[1][0][0] = p1[0];
	pSurf->m_ControlPoint[1][0][1] = p1[1];
	pSurf->m_ControlPoint[1][0][2] = p1[2];
	pSurf->m_ControlPoint[1][0][3] = 1;

	pSurf->m_ControlPoint[2][0][0] = p2[0];
	pSurf->m_ControlPoint[2][0][1] = p2[1];
	pSurf->m_ControlPoint[2][0][2] = p2[2];
	pSurf->m_ControlPoint[2][0][3] = 1;

	pSurf->m_ControlPoint[0][1][0] = p3[0];
	pSurf->m_ControlPoint[0][1][1] = p3[1];
	pSurf->m_ControlPoint[0][1][2] = p3[2];
	pSurf->m_ControlPoint[0][1][3] = 1;

	pSurf->m_ControlPoint[1][1][0] = p4[0];
	pSurf->m_ControlPoint[1][1][1] = p4[1];
	pSurf->m_ControlPoint[1][1][2] = p4[2];
	pSurf->m_ControlPoint[1][1][3] = 1;

	pSurf->m_ControlPoint[2][1][0] = p5[0];
	pSurf->m_ControlPoint[2][1][1] = p5[1];
	pSurf->m_ControlPoint[2][1][2] = p5[2];
	pSurf->m_ControlPoint[2][1][3] = 1;

	CVector3d m_lower,m_upper;
	m_lower.Set(pSurf->m_ControlPoint[0][0][0]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][1]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][2]/pSurf->m_ControlPoint[0][0][3]);
	m_upper.Set(pSurf->m_ControlPoint[0][0][0]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][1]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][2]/pSurf->m_ControlPoint[0][0][3]);

	CTransform transform;
	transform.Clear();
	for(int i=0;i<3;i++)
		for(int j=0;j<2;j++)
		{
			if(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3] < m_lower.x())
				m_lower.x(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3]);
			if(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3] < m_lower.y())
				m_lower.y(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3]) ;
			if(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3] < m_lower.z())
				m_lower.z(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3]) ;

			if(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3] > m_upper.x())
				m_upper.x(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3]);
			if(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3] > m_upper.y())
				m_upper.y(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3]) ;
			if(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3] > m_upper.z())
				m_upper.z(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3]) ;
		}

	CVector3d m_center;
	m_center=m_lower+m_upper;
	m_center/=-2;
	float length=m_upper.x()-m_lower.x();
	if(length<m_upper.y()-m_lower.y())
		length=m_upper.y()-m_lower.y();
	if(length<m_upper.z()-m_lower.z())
		length=m_upper.z()-m_lower.z();
	length=length/2;
	transform.SetScale(&m_center);
	m_center.Set(1/length,1/length,1/length);
	transform.SetScale(&m_center);
	pSurf->SetTransform(transform);
	m_SceneGraph.Add(pSurf);
}
void CSDIViewSwitchDoc::OnGenerateNewSurface1()
{
	m_SceneGraph.Free();
	CNurbsSuface* pSurf=new CNurbsSuface();
	pSurf->m_uknot=new GLfloat[6];
	pSurf->m_vknot=new GLfloat[6];

	pSurf->m_udegree = 2;
	pSurf->m_vdegree = 2;

	pSurf->m_uknotcount = 6;
	pSurf->m_vknotcount = 6;

	for(int i = 0; i < 3; i++)
	{
		pSurf->m_uknot[i] = 0;
		pSurf->m_uknot[i + 3] = 1;
		pSurf->m_vknot[i] = 0;
		pSurf->m_vknot[i + 3] = 1;
	}

	//
	Vector3D p00(3,4,1);
	Vector3D p20(10,1,-1);
	Vector3D p22(11,10,1);
	Vector3D p02(1,10,1);
	Vector3D D(0,0,1);
	//scaling the factor
	double len = Length(p00-p22);
	Vector3D direction = p02-p20;
	direction.SetUnit();
	p02 = p20 + direction * len;

	D = (p00+p22)/2 - (p20+p02)/2;
	//double d = -(D * (p00 + p00 - p20 - p02))/4;
	//D = D * d;
	//p22 = D*2 + (p20 + p02)-p00;


	double l1 = D * (p00 - p20) + D * (p00-p02) + D * D * 4;
	double l2 = (p02-p20)*(p02-p20);

	pSurf->m_ControlPoint[0][0][0] = p00[0];
	pSurf->m_ControlPoint[0][0][1] = p00[1];
	pSurf->m_ControlPoint[0][0][2] = p00[2];
	pSurf->m_ControlPoint[0][0][3] = 1;

	//
	pSurf->m_ControlPoint[2][0][0] = p20[0];
	pSurf->m_ControlPoint[2][0][1] = p20[1];
	pSurf->m_ControlPoint[2][0][2] = p20[2];
	pSurf->m_ControlPoint[2][0][3] = 1;

	//
	pSurf->m_ControlPoint[2][2][0] = p22[0];
	pSurf->m_ControlPoint[2][2][1] = p22[1];
	pSurf->m_ControlPoint[2][2][2] = p22[2];
	pSurf->m_ControlPoint[2][2][3] = 1;

	//
	pSurf->m_ControlPoint[0][2][0] = p02[0];
	pSurf->m_ControlPoint[0][2][1] = p02[1];
	pSurf->m_ControlPoint[0][2][2] = p02[2];
	pSurf->m_ControlPoint[0][2][3] = 1;

	Vector3D Z = D;
	Z.SetUnit();
	Vector3D X(2,4,0);
	X = X - (X * Z)*Z;
	X.SetUnit();
	Vector3D Y = Z ^ X;
	double a1 = ((p02-p00)*X)*2;
	double b1 = ((p02-p00)*Y)*2;
	double c1 = D*(p00 - p20 - D);
	double a2 = ((p20-p00)*X)*2;
	double b2 = ((p20-p00)*Y)*2;
	double c2 = D*(p00 - p02 - D);
	double c3 = -(D * D) /4;
	double x1 = 0;
	double y1 = (c1 - a1*x1)/b1;
	Vector3D D1=X*x1+Y*y1;
	double x2 = -(-y1*c2+c3*b2)/(-x1*b2+a2*y1);
	double y2 = (-x1*c2+a2*c3)/(-x1*b2+a2*y1);
	Vector3D D2=X*x2+Y*y2;
	Vector3D p10 = (p00 + p20)/2 - D1;
	Vector3D p01 = (p00 + p02)/2 - D2;
	Vector3D p21 = (p22 + p20)/2 - D2;
	Vector3D p12 = (p22 + p02)/2 - D1;
	Vector3D p11 = ((p10+p01+p21+p12)*2-(p00+p20+p02+p22))/4;

	double t1 = D1*D2+(D*D)/4;

	double tt = (p02-p00)*D1*2+D*(p20-p00)+D*D;
	double tt1 = (p20-p00)*D2*2+D*(p02-p00)+D*D;

	pSurf->m_ControlPoint[1][0][0] = p10[0];
	pSurf->m_ControlPoint[1][0][1] = p10[1];
	pSurf->m_ControlPoint[1][0][2] = p10[2];
	pSurf->m_ControlPoint[1][0][3] = 1;

	pSurf->m_ControlPoint[0][1][0] = p01[0];
	pSurf->m_ControlPoint[0][1][1] = p01[1];
	pSurf->m_ControlPoint[0][1][2] = p01[2];
	pSurf->m_ControlPoint[0][1][3] = 1;

	pSurf->m_ControlPoint[2][1][0] = p21[0];
	pSurf->m_ControlPoint[2][1][1] = p21[1];
	pSurf->m_ControlPoint[2][1][2] = p21[2];
	pSurf->m_ControlPoint[2][1][3] = 1;

	pSurf->m_ControlPoint[1][2][0] = p12[0];
	pSurf->m_ControlPoint[1][2][1] = p12[1];
	pSurf->m_ControlPoint[1][2][2] = p12[2];
	pSurf->m_ControlPoint[1][2][3] = 1;

	pSurf->m_ControlPoint[1][1][0] = p11[0];
	pSurf->m_ControlPoint[1][1][1] = p11[1];
	pSurf->m_ControlPoint[1][1][2] = p11[2];
	pSurf->m_ControlPoint[1][1][3] = 1;

	CVector3d m_lower,m_upper;
	m_lower.Set(pSurf->m_ControlPoint[0][0][0]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][1]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][2]/pSurf->m_ControlPoint[0][0][3]);
	m_upper.Set(pSurf->m_ControlPoint[0][0][0]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][1]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][2]/pSurf->m_ControlPoint[0][0][3]);

	CTransform transform;
	transform.Clear();
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			if(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3] < m_lower.x())
				m_lower.x(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3]);
			if(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3] < m_lower.y())
				m_lower.y(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3]) ;
			if(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3] < m_lower.z())
				m_lower.z(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3]) ;

			if(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3] > m_upper.x())
				m_upper.x(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3]);
			if(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3] > m_upper.y())
				m_upper.y(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3]) ;
			if(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3] > m_upper.z())
				m_upper.z(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3]) ;
		}

		CVector3d m_center;
		m_center=m_lower+m_upper;
		m_center/=-2;
		float length=m_upper.x()-m_lower.x();
		if(length<m_upper.y()-m_lower.y())
			length=m_upper.y()-m_lower.y();
		if(length<m_upper.z()-m_lower.z())
			length=m_upper.z()-m_lower.z();
		length=length/2;
		transform.SetScale(&m_center);
		m_center.Set(1/length,1/length,1/length);
		transform.SetScale(&m_center);
		pSurf->SetTransform(transform);
		m_SceneGraph.Add(pSurf);

		double *weight = new double[pSurf->GetCtrlNumberU() *  pSurf->GetCtrlNumberV()];
		double *uknot = new double[pSurf->m_uknotcount];
		double *vknot = new double[pSurf->m_vknotcount];

		int i,j;
		for(i = 0; i < pSurf->m_uknotcount; i++)
			uknot[i] = pSurf->m_uknot[i];

		for(i = 0; i < pSurf->m_vknotcount; i++)
			vknot[i] = pSurf->m_vknot[i];

		int pnum = 0;
		Point3D* pointarray = new Point3D[pSurf->GetCtrlNumberU() * pSurf->GetCtrlNumberV()];
		for(j = 0; j < pSurf->GetCtrlNumberV(); j++)
		{
			for(i = 0; i < pSurf->GetCtrlNumberU(); i++)
			{
				pointarray[pnum][0] = pSurf->m_ControlPoint[i][j][0] / pSurf->m_ControlPoint[i][j][3];
				pointarray[pnum][1] = pSurf->m_ControlPoint[i][j][1] / pSurf->m_ControlPoint[i][j][3];
				pointarray[pnum][2] = pSurf->m_ControlPoint[i][j][2] / pSurf->m_ControlPoint[i][j][3];
				weight[pnum] = pSurf->m_ControlPoint[i][j][3];
				pnum++;
			}
		}
		C_NurbsSurf* pOriginalNurbsSurface = new C_NurbsSurf(pSurf->GetCtrlNumberU(),pSurf->GetCtrlNumberV(),
			pSurf->m_udegree + 1,pSurf->m_vdegree + 1,uknot,vknot,pointarray,weight);
		Vector3D vv1 = pOriginalNurbsSurface->ComputUDerivative1(0.5,0.5);
		Vector3D vv2 = pOriginalNurbsSurface->ComputVDerivative1(0.5,0.5);
		vv1.SetUnit();
		vv2.SetUnit();
		double test = vv1 * vv2;
		p00 = pOriginalNurbsSurface->GetControlPoint(0,0)-ZeroP3D;
		p10 = pOriginalNurbsSurface->GetControlPoint(1,0)-ZeroP3D;
		p20 = pOriginalNurbsSurface->GetControlPoint(2,0)-ZeroP3D;

		p01 = pOriginalNurbsSurface->GetControlPoint(0,1)-ZeroP3D;
		p11 = pOriginalNurbsSurface->GetControlPoint(1,1)-ZeroP3D;
		p21 = pOriginalNurbsSurface->GetControlPoint(2,1)-ZeroP3D;

		p02 = pOriginalNurbsSurface->GetControlPoint(0,2)-ZeroP3D;
		p12 = pOriginalNurbsSurface->GetControlPoint(1,2)-ZeroP3D;
		p22 = pOriginalNurbsSurface->GetControlPoint(2,2)-ZeroP3D;

		//D4 = p20 + p00 - p10*2;
		//D5 = p00 + p11 - p01 - p10;
		Vector3D D3 = p00 + p02 - p01*2;

		//test = D5*D4;
		double A1 = (2*p00-4*p10+2*p20-4*p01+8*p11-4*p21+2*p02-4*p12+2*p22)*(2*p00-4*p10+2*p20-4*p01+8*p11-4*p21+2*p02-4*p12+2*p22);
		double A2 = -(12*(p00+p20-p21-2*p10+2*p11-p01))*(p00+p22-2*p10+p20-2*p01+4*p11-2*p21+p02-2*p12);
		double A3 = 4*p00*p22+48*p10*p10-48*p10*p20+48*p10*p01+12*p00*p00-48*p00*p10+24*p00*p20-24*p00*p01+48*p00*p11-24*p00*p21+4*p00*p02-8*p00*p12-96*p10*p11+48*p10*p21-8*p10*p02+16*p10*p12-8*p10*p22-24*p01*p20+8*p01*p01-32*p01*p11+16*p01*p21+12*p20*p20+48*p20*p11-24*p20*p21+4*p20*p02-8*p20*p12+4*p20*p22-32*p21*p11+8*p21*p21+32*p11*p11;
		double A4 = -(4*(p00-2*p10+p20))*(p00+p20-p21-2*p10+2*p11-p01);
		double A5 = -(12*(-2*p01-p12+p02+2*p11+p00-p10))*(p00+p22-2*p10+p20-2*p01+4*p11-2*p21+p02-2*p12);
		double A6 = 16*p00*p22+72*p10*p10-36*p10*p20+164*p10*p01+36*p00*p00-108*p00*p10+36*p00*p20-108*p00*p01+160*p00*p11-52*p00*p21+36*p00*p02-52*p00*p12-216*p10*p11+52*p10*p21-56*p10*p02+72*p10*p12-16*p10*p22-56*p01*p20+72*p01*p01-216*p01*p11+72*p01*p21-36*p01*p02+52*p01*p12-16*p01*p22+56*p20*p11+20*p20*p02-20*p20*p12-72*p21*p11-20*p21*p02+20*p21*p12+144*p11*p11+56*p11*p02-72*p11*p12+16*p11*p22;
		double A7 = -4*p00*p22-72*p10*p10+36*p10*p20-112*p10*p01-36*p00*p00+108*p00*p10-36*p00*p20+72*p00*p01-104*p00*p11+32*p00*p21-12*p00*p02+16*p00*p12+144*p10*p11-32*p10*p21+20*p10*p02-24*p10*p12+4*p10*p22+40*p01*p20-24*p01*p01+72*p01*p11-24*p01*p21-40*p20*p11-8*p20*p02+8*p20*p12+24*p21*p11-48*p11*p11;
		double A8 = 12*p00*p00-4*p00*p21-36*p00*p10-12*p00*p01+12*p00*p20+16*p00*p11+4*p10*p21+24*p10*p10+20*p10*p01-12*p10*p20-24*p10*p11+8*p20*p11-8*p01*p20;
		double A9 = 4*p00*p22+8*p10*p10+8*p12*p12-24*p12*p02+12*p02*p02+48*p10*p01+4*p22*p02+12*p00*p00-24*p00*p10+4*p00*p20
			-48*p00*p01+48*p00*p11-8*p00*p21+24*p00*p02-24*p00*p12-32*p10*p11-24*p10*p02+16*p10*p12-8*p01*p20+48*p01*p01
			-96*p01*p11+16*p01*p21-48*p01*p02+48*p01*p12-8*p01*p22+4*p20*p02-8*p21*p02+32*p11*p11+48*p11*p02-32*p11*p12;
		double A10 = -4*p00*p22-24*p10*p10-112*p10*p01-36*p00*p00+72*p00*p10-12*p00*p20+108*p00*p01
			-104*p00*p11+16*p00*p21-36*p00*p02+32*p00*p12+72*p10*p11+40*p10*p02-24*p10*p12+20*p01*p20
			-72*p01*p01+144*p01*p11-24*p01*p21+36*p01*p02-32*p01*p12+4*p01*p22-8*p20*p02+8*p21*p02
			-48*p11*p11-40*p11*p02+24*p11*p12;
		double A11 = 24*p10*p10+80*p10*p01+36*p00*p00-72*p00*p10+12*p00*p20-72*p00*p01
			+64*p00*p11-8*p00*p21+12*p00*p02-8*p00*p12-48*p10*p11-16*p10*p02+8*p10*p12
			-16*p01*p20+24*p01*p01-48*p01*p11+8*p01*p21+4*p20*p02+16*p11*p11;
		double A12 = -12*p00*p00-8*p00*p11+12*p00*p01+24*p00*p10+8*p10*p11-16*p10*p01-8*p10*p10-4*p00*p20+4*p01*p20;
		double A13 = -(4*(-2*p01-p12+p02+2*p11+p00-p10))*(p00-2*p01+p02);
		double A14 = 12*p00*p00+12*p00*p02-36*p00*p01+16*p00*p11+8*p11*p02-24*p01*p11-12*p01*p02
			+24*p01*p01-12*p00*p10-8*p10*p02+20*p10*p01-4*p00*p12+4*p01*p12;
		double A15 = -12*p00*p00-4*p00*p02+24*p00*p01+12*p00*p10+4*p10*p02
			-16*p10*p01-8*p00*p11+8*p01*p11-8*p01*p01;
		double A16 = (4*(p00-p10))*(p00-p01);

}
void CSDIViewSwitchDoc::OnGenerateNewSurface()
{
	m_SceneGraph.Free();
	CNurbsSuface* pSurf=new CNurbsSuface();
	pSurf->m_uknot=new GLfloat[6];
	pSurf->m_vknot=new GLfloat[6];

	pSurf->m_udegree = 2;
	pSurf->m_vdegree = 2;

	pSurf->m_uknotcount = 6;
	pSurf->m_vknotcount = 6;

	for(int i = 0; i < 3; i++)
	{
		pSurf->m_uknot[i] = 0;
		pSurf->m_uknot[i + 3] = 1;
		pSurf->m_vknot[i] = 0;
		pSurf->m_vknot[i + 3] = 1;
	}

	//
	Vector3D p00(3,-4,1);
	Vector3D p20(10,1,-1);
	Vector3D p22(11,10,1);
	Vector3D p02(1,10,-1);
	Vector3D D(0,0,1);
	//scaling the factor
	double len = Length(p00-p22);
	Vector3D direction = p02-p20;
	direction.SetUnit();
	p02 = p20 + direction * len;

	D = (p00+p22)/2 - (p20+p02)/2;
	//double d = -(D * (p00 + p00 - p20 - p02))/4;
	//D = D * d;
	//p22 = D*2 + (p20 + p02)-p00;


	double l1 = D * (p00 - p20) + D * (p00-p02) + D * D * 4;
	double l2 = (p02-p20)*(p02-p20);

	pSurf->m_ControlPoint[0][0][0] = p00[0];
	pSurf->m_ControlPoint[0][0][1] = p00[1];
	pSurf->m_ControlPoint[0][0][2] = p00[2];
	pSurf->m_ControlPoint[0][0][3] = 1;

	//
	pSurf->m_ControlPoint[2][0][0] = p20[0];
	pSurf->m_ControlPoint[2][0][1] = p20[1];
	pSurf->m_ControlPoint[2][0][2] = p20[2];
	pSurf->m_ControlPoint[2][0][3] = 1;

	//
	pSurf->m_ControlPoint[2][2][0] = p22[0];
	pSurf->m_ControlPoint[2][2][1] = p22[1];
	pSurf->m_ControlPoint[2][2][2] = p22[2];
	pSurf->m_ControlPoint[2][2][3] = 1;

	//
	pSurf->m_ControlPoint[0][2][0] = p02[0];
	pSurf->m_ControlPoint[0][2][1] = p02[1];
	pSurf->m_ControlPoint[0][2][2] = p02[2];
	pSurf->m_ControlPoint[0][2][3] = 1;

	Vector3D Z = D;
	Z.SetUnit();
	Vector3D X(2,4,0);
	X = X - (X * Z)*Z;
	X.SetUnit();
	Vector3D Y = Z ^ X;
	double a1 = ((p02-p00)*X)*2;
	double b1 = ((p02-p00)*Y)*2;
	double c1 = D*(p00 - p20 - D);
	double a2 = ((p20-p00)*X)*2;
	double b2 = ((p20-p00)*Y)*2;
	double c2 = D*(p00 - p02 - D);
	double c3 = -(D * D) /4;

	double x1 = 0;
	double y1 = (c1 - a1*x1)/b1;
	Vector3D D1=X*x1+Y*y1;
	double x2 = -(-y1*c2+c3*b2)/(-x1*b2+a2*y1);
	double y2 = (-x1*c2+a2*c3)/(-x1*b2+a2*y1);
	Vector3D D2=X*x2+Y*y2;
	Vector3D p10 = (p00 + p20)/2 - D1;
	Vector3D p01 = (p00 + p02)/2 - D2;
	Vector3D p21 = (p22 + p20)/2 - D2;
	Vector3D p12 = (p22 + p02)/2 - D1;
	Vector3D p11 = ((p10+p01+p21+p12)*2-(p00+p20+p02+p22))/4;

	double t1 = D1*D2+(D*D)/4;

	double tt = (p02-p00)*D1*2+D*(p20-p00)+D*D;
	double tt1 = (p20-p00)*D2*2+D*(p02-p00)+D*D;

	pSurf->m_ControlPoint[1][0][0] = p10[0];
	pSurf->m_ControlPoint[1][0][1] = p10[1];
	pSurf->m_ControlPoint[1][0][2] = p10[2];
	pSurf->m_ControlPoint[1][0][3] = 1;

	pSurf->m_ControlPoint[0][1][0] = p01[0];
	pSurf->m_ControlPoint[0][1][1] = p01[1];
	pSurf->m_ControlPoint[0][1][2] = p01[2];
	pSurf->m_ControlPoint[0][1][3] = 1;

	pSurf->m_ControlPoint[2][1][0] = p21[0];
	pSurf->m_ControlPoint[2][1][1] = p21[1];
	pSurf->m_ControlPoint[2][1][2] = p21[2];
	pSurf->m_ControlPoint[2][1][3] = 1;

	pSurf->m_ControlPoint[1][2][0] = p12[0];
	pSurf->m_ControlPoint[1][2][1] = p12[1];
	pSurf->m_ControlPoint[1][2][2] = p12[2];
	pSurf->m_ControlPoint[1][2][3] = 1;

	pSurf->m_ControlPoint[1][1][0] = p11[0];
	pSurf->m_ControlPoint[1][1][1] = p11[1];
	pSurf->m_ControlPoint[1][1][2] = p11[2];
	pSurf->m_ControlPoint[1][1][3] = 1;

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		{
			pSurf->m_ControlPoint[i][j][0] = pSurf->m_ControlPoint[i][j][0] * 0.1;
			pSurf->m_ControlPoint[i][j][1] = pSurf->m_ControlPoint[i][j][1] * 0.1;
			pSurf->m_ControlPoint[i][j][2] = pSurf->m_ControlPoint[i][j][2] * 0.1;
		}
	CVector3d m_lower,m_upper;
	m_lower.Set(pSurf->m_ControlPoint[0][0][0]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][1]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][2]/pSurf->m_ControlPoint[0][0][3]);
	m_upper.Set(pSurf->m_ControlPoint[0][0][0]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][1]/pSurf->m_ControlPoint[0][0][3],pSurf->m_ControlPoint[0][0][2]/pSurf->m_ControlPoint[0][0][3]);

	CTransform transform;
	transform.Clear();
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			if(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3] < m_lower.x())
				m_lower.x(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3]);
			if(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3] < m_lower.y())
				m_lower.y(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3]) ;
			if(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3] < m_lower.z())
				m_lower.z(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3]) ;

			if(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3] > m_upper.x())
				m_upper.x(pSurf->m_ControlPoint[i][j][0]/pSurf->m_ControlPoint[i][j][3]);
			if(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3] > m_upper.y())
				m_upper.y(pSurf->m_ControlPoint[i][j][1]/pSurf->m_ControlPoint[i][j][3]) ;
			if(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3] > m_upper.z())
				m_upper.z(pSurf->m_ControlPoint[i][j][2]/pSurf->m_ControlPoint[i][j][3]) ;
		}


		CVector3d m_center;
		m_center=m_lower+m_upper;
		//m_center/=-2;
		m_center = m_upper;
		float length=m_upper.x()-m_lower.x();
		if(length<m_upper.y()-m_lower.y())
			length=m_upper.y()-m_lower.y();
		if(length<m_upper.z()-m_lower.z())
			length=m_upper.z()-m_lower.z();
		//length=length/2;
		transform.SetTranslation(&m_center);
		m_center.Set(1/length,1/length,1/length);
		transform.SetScale(&m_center);
		pSurf->SetTransform(transform);
		m_SceneGraph.Add(pSurf);

		double *weight = new double[pSurf->GetCtrlNumberU() *  pSurf->GetCtrlNumberV()];
		double *uknot = new double[pSurf->m_uknotcount];
		double *vknot = new double[pSurf->m_vknotcount];

		int i,j;
		for(i = 0; i < pSurf->m_uknotcount; i++)
			uknot[i] = pSurf->m_uknot[i];

		for(i = 0; i < pSurf->m_vknotcount; i++)
			vknot[i] = pSurf->m_vknot[i];

		int pnum = 0;
		Point3D* pointarray = new Point3D[pSurf->GetCtrlNumberU() * pSurf->GetCtrlNumberV()];
		for(j = 0; j < pSurf->GetCtrlNumberV(); j++)
		{
			for(i = 0; i < pSurf->GetCtrlNumberU(); i++)
			{
				pointarray[pnum][0] = pSurf->m_ControlPoint[i][j][0] / pSurf->m_ControlPoint[i][j][3];
				pointarray[pnum][1] = pSurf->m_ControlPoint[i][j][1] / pSurf->m_ControlPoint[i][j][3];
				pointarray[pnum][2] = pSurf->m_ControlPoint[i][j][2] / pSurf->m_ControlPoint[i][j][3];
				weight[pnum] = pSurf->m_ControlPoint[i][j][3];
				pnum++;
			}
		}
		C_NurbsSurf* pOriginalNurbsSurface = new C_NurbsSurf(pSurf->GetCtrlNumberU(),pSurf->GetCtrlNumberV(),
			pSurf->m_udegree + 1,pSurf->m_vdegree + 1,uknot,vknot,pointarray,weight);
		Vector3D vv1 = pOriginalNurbsSurface->ComputUDerivative1(0.5,0.5);
		Vector3D vv2 = pOriginalNurbsSurface->ComputVDerivative1(0.5,0.5);
		vv1.SetUnit();
		vv2.SetUnit();
		double test = vv1 * vv2;
		p00 = pOriginalNurbsSurface->GetControlPoint(0,0)-ZeroP3D;
		p10 = pOriginalNurbsSurface->GetControlPoint(1,0)-ZeroP3D;
		p20 = pOriginalNurbsSurface->GetControlPoint(2,0)-ZeroP3D;

		p01 = pOriginalNurbsSurface->GetControlPoint(0,1)-ZeroP3D;
		p11 = pOriginalNurbsSurface->GetControlPoint(1,1)-ZeroP3D;
		p21 = pOriginalNurbsSurface->GetControlPoint(2,1)-ZeroP3D;

		p02 = pOriginalNurbsSurface->GetControlPoint(0,2)-ZeroP3D;
		p12 = pOriginalNurbsSurface->GetControlPoint(1,2)-ZeroP3D;
		p22 = pOriginalNurbsSurface->GetControlPoint(2,2)-ZeroP3D;

		std::ofstream outfile1("D:\\surface2_original.sur");
		outfile1 << " 3 3 " << endl;
		outfile1 << " 2 2 " << endl;
		outfile1 << " 0 0 0 1 1 1 "<<endl;
		outfile1 << " 0 0 0 1 1 1 "<<endl;
		outfile1 << p00[0] << " " << p00[1] << " " << p00[2] << " 1 " <<endl;
		outfile1 << p01[0] << " " << p01[1] << " " << p01[2] << " 1 " <<endl;
		outfile1 << p02[0] << " " << p02[1] << " " << p02[2] << " 1 " <<endl;

		outfile1 << p10[0] << " " << p10[1] << " " << p10[2] << " 1 " <<endl;
		outfile1 << p11[0] << " " << p11[1] << " " << p11[2] << " 1 " <<endl;
		outfile1 << p12[0] << " " << p12[1] << " " << p12[2] << " 1 " <<endl;

		outfile1 << p20[0] << " " << p20[1] << " " << p20[2] << " 1 " <<endl;
		outfile1 << p21[0] << " " << p21[1] << " " << p21[2] << " 1 " <<endl;
		outfile1 << p22[0] << " " << p22[1] << " " << p22[2] << " 1 " <<endl;
		outfile1.close();

		Vector3D m00 = p00;
		Vector3D m01 = p00/3 + p01 * 2 /3;
		Vector3D m02 = p01*2/3 + p02 / 3;
		Vector3D m03 = p02;

		Vector3D m10 = p10;
		Vector3D m11 = p10 / 3 + p11 * 2 / 3;
		Vector3D m12 = p11 * 2 / 3 + p12 / 3;
		Vector3D m13 = p12;

		Vector3D m20 = p20;
		Vector3D m21 = p20 / 3 + p21 * 2 / 3;
		Vector3D m22 = p21 * 2 / 3 + p22 / 3;
		Vector3D m23 = p22;

		Vector3D k00 = m00;
		Vector3D k10 = m00 / 3 + m10 * 2 / 3;
		Vector3D k20 = m10 * 2 / 3 + m20 / 3;
		Vector3D k30 = m20;

		Vector3D k01 = m01;
		Vector3D k11 = m01 / 3 + m11 * 2 / 3;
		Vector3D k21 = m11 * 2 / 3 + m21 / 3;
		Vector3D k31 = m21;

		Vector3D k02 = m02;
		Vector3D k12 = m02 / 3 + m12 * 2 / 3;
		Vector3D k22 = m12 * 2 / 3 + m22 / 3;
		Vector3D k32 = m22;

		Vector3D k03 = m03;
		Vector3D k13 = m03 / 3 + m13 * 2 / 3;
		Vector3D k23 = m13 * 2 / 3 + m23 / 3;
		Vector3D k33 = m23;

		//
		/*k10 = k00 + (k10 - k00) * 4;
		k20 = k30 + (k20 - k30) / 4.0;

		k11 = k01 + (k11 - k01) * 2;
		k21 = k31 + (k21 - k31) / 2.0;

		k12 = k02 + (k12 - k02) / 1.0;
		k22 = k32 + (k22 - k32) * 1.0;

		k13 = k03 + (k13 - k03) / 2.0;
		k23 = k33 + (k23 - k33) * 2.0;*/
		/*k10 = k00 + (k10 - k00) / 2;
		k20 = k30 + (k20 - k30) * 2.0;

		k11 = k01 + (k11 - k01) / 1.0;
		k21 = k31 + (k21 - k31) * 1.0;

		k12 = k02 + (k12 - k02) * 2.0;
		k22 = k32 + (k22 - k32) / 2.0;

		k13 = k03 + (k13 - k03) * 4.0;
		k23 = k33 + (k23 - k33) / 4.0;*/

		//
		/*k01 = k00 + (k01 - k00) * 4;
		k02 = k03 + (k02 - k03) / 4.0;

		k11 = k10 + (k11 - k10) * 4;
		k12 = k13 + (k12 - k13) / 4.0;

		k21 = k20 + (k21 - k20) * 4.0;
		k22 = k23 + (k22 - k23) / 4.0;

		k31 = k30 + (k31 - k30) * 4.0;
		k32 = k33 + (k32 - k33) / 4.0;*/
		/*k01 = k00 + (k01 - k00) / 4;
		k02 = k03 + (k02 - k03) * 4.0;

		k11 = k10 + (k11 - k10) / 4;
		k12 = k13 + (k12 - k13) * 4.0;

		k21 = k20 + (k21 - k20) / 4.0;
		k22 = k23 + (k22 - k23) * 4.0;

		k31 = k30 + (k31 - k30) / 4.0;
		k32 = k33 + (k32 - k33) * 4.0;*/

		std::ofstream outfile("D:\\surface.sur");
		outfile << " 4 4 " << endl;
		outfile << " 3 3 " << endl;
		outfile << " 0 0 0 0 1 1 1 1 "<<endl;
		outfile << " 0 0 0 0 1 1 1 1 "<<endl;
		outfile << k00[0] << " " << k00[1] << " " << k00[2] << " 1 " <<endl;
		outfile << k01[0] << " " << k01[1] << " " << k01[2] << " 1 " <<endl;
		outfile << k02[0] << " " << k02[1] << " " << k02[2] << " 1 " <<endl;
		outfile << k03[0] << " " << k03[1] << " " << k03[2] << " 1 " <<endl;

		outfile << k10[0] << " " << k10[1] << " " << k10[2] << " 1 " <<endl;
		outfile << k11[0] << " " << k11[1] << " " << k11[2] << " 1 " <<endl;
		outfile << k12[0] << " " << k12[1] << " " << k12[2] << " 1 " <<endl;
		outfile << k13[0] << " " << k13[1] << " " << k13[2] << " 1 " <<endl;

		outfile << k20[0] << " " << k20[1] << " " << k20[2] << " 1 " <<endl;
		outfile << k21[0] << " " << k21[1] << " " << k21[2] << " 1 " <<endl;
		outfile << k22[0] << " " << k22[1] << " " << k22[2] << " 1 " <<endl;
		outfile << k23[0] << " " << k23[1] << " " << k23[2] << " 1 " <<endl;

		outfile << k30[0] << " " << k30[1] << " " << k30[2] << " 1 " <<endl;
		outfile << k31[0] << " " << k31[1] << " " << k31[2] << " 1 " <<endl;
		outfile << k32[0] << " " << k32[1] << " " << k32[2] << " 1 " <<endl;
		outfile << k33[0] << " " << k33[1] << " " << k33[2] << " 1 " <<endl;
		outfile.close();

		//D4 = p20 + p00 - p10*2; 
		//D5 = p00 + p11 - p01 - p10;
		Vector3D D3 = p00 + p02 - p01*2;

		//test = D5*D4;
		double A1 = (2*p00-4*p10+2*p20-4*p01+8*p11-4*p21+2*p02-4*p12+2*p22)*(2*p00-4*p10+2*p20-4*p01+8*p11-4*p21+2*p02-4*p12+2*p22);
		double A2 = -(12*(p00+p20-p21-2*p10+2*p11-p01))*(p00+p22-2*p10+p20-2*p01+4*p11-2*p21+p02-2*p12);
		double A3 = 4*p00*p22+48*p10*p10-48*p10*p20+48*p10*p01+12*p00*p00-48*p00*p10+24*p00*p20-24*p00*p01+48*p00*p11-24*p00*p21+4*p00*p02-8*p00*p12-96*p10*p11+48*p10*p21-8*p10*p02+16*p10*p12-8*p10*p22-24*p01*p20+8*p01*p01-32*p01*p11+16*p01*p21+12*p20*p20+48*p20*p11-24*p20*p21+4*p20*p02-8*p20*p12+4*p20*p22-32*p21*p11+8*p21*p21+32*p11*p11;
		double A4 = -(4*(p00-2*p10+p20))*(p00+p20-p21-2*p10+2*p11-p01);
		double A5 = -(12*(-2*p01-p12+p02+2*p11+p00-p10))*(p00+p22-2*p10+p20-2*p01+4*p11-2*p21+p02-2*p12);
		double A6 = 16*p00*p22+72*p10*p10-36*p10*p20+164*p10*p01+36*p00*p00-108*p00*p10+36*p00*p20-108*p00*p01+160*p00*p11-52*p00*p21+36*p00*p02-52*p00*p12-216*p10*p11+52*p10*p21-56*p10*p02+72*p10*p12-16*p10*p22-56*p01*p20+72*p01*p01-216*p01*p11+72*p01*p21-36*p01*p02+52*p01*p12-16*p01*p22+56*p20*p11+20*p20*p02-20*p20*p12-72*p21*p11-20*p21*p02+20*p21*p12+144*p11*p11+56*p11*p02-72*p11*p12+16*p11*p22;
		double A7 = -4*p00*p22-72*p10*p10+36*p10*p20-112*p10*p01-36*p00*p00+108*p00*p10-36*p00*p20+72*p00*p01-104*p00*p11+32*p00*p21-12*p00*p02+16*p00*p12+144*p10*p11-32*p10*p21+20*p10*p02-24*p10*p12+4*p10*p22+40*p01*p20-24*p01*p01+72*p01*p11-24*p01*p21-40*p20*p11-8*p20*p02+8*p20*p12+24*p21*p11-48*p11*p11;
		double A8 = 12*p00*p00-4*p00*p21-36*p00*p10-12*p00*p01+12*p00*p20+16*p00*p11+4*p10*p21+24*p10*p10+20*p10*p01-12*p10*p20-24*p10*p11+8*p20*p11-8*p01*p20;
		double A9 = 4*p00*p22+8*p10*p10+8*p12*p12-24*p12*p02+12*p02*p02+48*p10*p01+4*p22*p02+12*p00*p00-24*p00*p10+4*p00*p20
			-48*p00*p01+48*p00*p11-8*p00*p21+24*p00*p02-24*p00*p12-32*p10*p11-24*p10*p02+16*p10*p12-8*p01*p20+48*p01*p01
			-96*p01*p11+16*p01*p21-48*p01*p02+48*p01*p12-8*p01*p22+4*p20*p02-8*p21*p02+32*p11*p11+48*p11*p02-32*p11*p12;
		double A10 = -4*p00*p22-24*p10*p10-112*p10*p01-36*p00*p00+72*p00*p10-12*p00*p20+108*p00*p01
			-104*p00*p11+16*p00*p21-36*p00*p02+32*p00*p12+72*p10*p11+40*p10*p02-24*p10*p12+20*p01*p20
			-72*p01*p01+144*p01*p11-24*p01*p21+36*p01*p02-32*p01*p12+4*p01*p22-8*p20*p02+8*p21*p02
			-48*p11*p11-40*p11*p02+24*p11*p12;
		double A11 = 24*p10*p10+80*p10*p01+36*p00*p00-72*p00*p10+12*p00*p20-72*p00*p01
			+64*p00*p11-8*p00*p21+12*p00*p02-8*p00*p12-48*p10*p11-16*p10*p02+8*p10*p12
			-16*p01*p20+24*p01*p01-48*p01*p11+8*p01*p21+4*p20*p02+16*p11*p11;
		double A12 = -12*p00*p00-8*p00*p11+12*p00*p01+24*p00*p10+8*p10*p11-16*p10*p01-8*p10*p10-4*p00*p20+4*p01*p20;
		double A13 = -(4*(-2*p01-p12+p02+2*p11+p00-p10))*(p00-2*p01+p02);
		double A14 = 12*p00*p00+12*p00*p02-36*p00*p01+16*p00*p11+8*p11*p02-24*p01*p11-12*p01*p02
			+24*p01*p01-12*p00*p10-8*p10*p02+20*p10*p01-4*p00*p12+4*p01*p12;
		double A15 = -12*p00*p00-4*p00*p02+24*p00*p01+12*p00*p10+4*p10*p02
			-16*p10*p01-8*p00*p11+8*p01*p11-8*p01*p01;
		double A16 = (4*(p00-p10))*(p00-p01);
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CSDIViewSwitchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSDIViewSwitchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::UpdateTreeControl()
{
	POSITION pos = pDoc->GetFirstViewPosition();
	CFormView3* pFormView;
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CFormView3)))
			pFormView = (CFormView3*)pView;
	}
	pFormView->UpdateTreeControl();
}

BOOL CSDIViewSwitchDoc::OnOpenDocument_OpenMesh(LPCTSTR lpszPathName)
{
	MyMesh mesh;

	if (!OpenMesh::IO::read_mesh(mesh, lpszPathName)) 
	{
		std::cerr << "read error\n";
		exit(1);
	}
}
void CSDIViewSwitchDoc::ZoomToFitCurrentMesh()
{
	m_SceneGraph.ZoomToFitCurrentMesh();
}
void CSDIViewSwitchDoc::ZoomAll()
{
	m_SceneGraph.ZoomAll();
}
void CSDIViewSwitchDoc::ZoomAllToFitCurrentMesh()
{
	m_SceneGraph.ZoomAllToFitCurrentMesh();
}
void CSDIViewSwitchDoc::ReadNurbsSet(LPCTSTR lpszPathName,CSceneGraph3d *pSceneGraph)
{
	std::ifstream file(lpszPathName);
	int surfacenum;
	CString path = lpszPathName;
	path = path.Left(path.ReverseFind('\\') + 1);

	file >> surfacenum;
	CString filename;
	char   name[80];
	for(int i = 0; i < surfacenum; i++)
	{
		file >> name;
		filename = name;
		filename = path + filename;

		TRACE("Nurbs type\n");
		CParseNurbs parser;
		if(parser.Run((LPSTR)(LPCSTR)filename,&m_SceneGraph))
		{
		}
	}
}
BOOL CSDIViewSwitchDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	//p_FormView3->MatlabTest();
	CString file = lpszPathName;
	//m_SceneGraph.Free();
	//Extension
	CString extension = lpszPathName;
	extension = extension.Right(extension.GetLength() - extension.ReverseFind('.'));
	extension.MakeLower();

	// Path "c:\path\file.wrl" -> c:\path
	CString path = lpszPathName;
	path = path.Left(path.ReverseFind('\\'));

	// CDocument
	SetCurrentDirectory(path);

	TRACE("\nOpening document\n");
	TRACE("File      : %s\n",lpszPathName);
	TRACE("Path      : %s\n",path);
	TRACE("Extension : %s\n",extension);

	if(extension == ".set")
	{
		//many surface
		TRACE("Nurbs Set\n");
		ReadNurbsSet((char *)lpszPathName,&m_SceneGraph);

	}else
	if(extension == ".sur")
	{
		//start to read the NURBS surface
		TRACE("Nurbs type\n");
		CParseNurbs parser;
		if(parser.Run((char *)lpszPathName,&m_SceneGraph))
		{
		}
	}else
	if(extension == ".tsp")
	{
		//start to read the NURBS surface
		TRACE("Tspline type\n");
		CParseTspline parser;
		if(parser.Run((char *)lpszPathName,&m_SceneGraph))
		{
		}
	}else
	if(extension == ".obj")
	{
		// Start reading obj file
		TRACE("obj type\n");
		// Parser OBJ
		CParseOBJ parser;
		if(parser.Run2((char *)lpszPathName,&m_SceneGraph))
		{
			m_SceneGraph.BuildAdjacency();
			m_SceneGraph.CalculateNormalPerFace();
			m_SceneGraph.CalculateNormalPerVertex();
		}
		//Read_Mesh_OpenMesh(lpszPathName,m_SceneGraph);
	}else
	if(extension == ".m")
	{
		// Start reading m file
		TRACE("m type\n");
		CParseM parser;
		if(parser.Run((char *)lpszPathName,&m_SceneGraph))
		{
			m_SceneGraph.BuildAdjacency();
			m_SceneGraph.CalculateNormalPerFace();
			m_SceneGraph.CalculateNormalPerVertex();
		}
	}
	else	
	if(extension == ".cur")
	{
		TRACE("Nurbs curve type\n");
		CParseNurbsCurve parser;

		//Read Curve
		parser.Run((char *)lpszPathName,&m_SceneGraph);	
	}else
	if(extension == ".poly")
	{
		CParsePOLY parser;

		//Read Curve
		parser.Run_PSLG((char *)lpszPathName,&m_SceneGraph);		
	}else
	if(extension == ".ele")
	{
		CParsePOLY parser;

		//Read Curve
		if(parser.Run_Element((char *)lpszPathName,&m_SceneGraph))
		{
			m_SceneGraph.BuildAdjacency();
			m_SceneGraph.CalculateNormalPerFace();
			m_SceneGraph.CalculateNormalPerVertex();
		}
	}else
	if (extension == ".step")
	{
		CParseSTEP parser;

		parser.Run_STEP((char*)lpszPathName, &m_SceneGraph);
	}

	UpdateTreeControl();

	//Redraw
	m_SceneGraph.SetModifyFlag(TRUE);
	return TRUE;
}

void CSDIViewSwitchDoc::Read_Mesh_OpenMesh(LPCTSTR lpszPathName,CSceneGraph3d& m_SceneGraph)
{
	CMesh3d_OpenMesh* mesh = new CMesh3d_OpenMesh();

	//texture
	mesh->pmesh->request_vertex_texcoords2D();
	mesh->pmesh->request_vertex_texcoords3D();

	// request vertex normals, so the mesh reader can use normal information  
	// if available  
	mesh->pmesh->request_vertex_normals();  
	// assure we have vertex normals  

	// Add face normals as default property
	mesh->pmesh->request_face_normals();

	if (!mesh->pmesh->has_vertex_normals())  
	{  
		std::cerr << "ERROR: Standard vertex property 'Normals' not available!\n";  
		return;  
	}  

	OpenMesh::IO::Options option;
	option += OpenMesh::IO::Options::VertexTexCoord;

	if (!OpenMesh::IO::read_mesh(*mesh->pmesh, lpszPathName,option)) 
	{
		std::cerr << "read error\n";
		exit(1);
	}

	mesh->pmesh->request_face_colors();

	// If the file did not provide vertex normals, then calculate them  
	if ( !option.check( OpenMesh::IO::Options::VertexNormal ) )  
	{  
		// let the mesh update the normals  
		mesh->pmesh->update_normals(); 
	}  

	mesh->pmesh->request_vertex_colors();
	mesh->pmesh->request_face_colors();

	OpenMesh::Vec3d pt;
	pt = mesh->pmesh->point(mesh->pmesh->vertices_begin());
	CVector3d m_lower,m_upper;
	m_lower.Set(pt[0],pt[1],pt[2]);
	m_upper.Set(pt[0],pt[1],pt[2]);
	CTransform transform;
	transform.Clear();
	double m_x,m_y,m_z;

	BOOL vertexcolorflag = option.check( OpenMesh::IO::Options::VertexColor);
	for (MyMesh::VertexIter v_it = mesh->pmesh->vertices_begin(); v_it != mesh->pmesh->vertices_end(); ++v_it)
	{
		if ( !vertexcolorflag )  
		{ 
			//if no vertex color in the file, then set the default color
			mesh->pmesh->set_color(v_it,MyMesh::Color(40,240,120));
		}

		pt = mesh->pmesh->point(v_it);
		
		m_x = pt[0];
		m_y = pt[1];
		m_z = pt[2];

		if(m_x < m_lower.x())
			m_lower.x(m_x) ;
		if(m_y < m_lower.y())
			m_lower.y(m_y) ;
		if(m_z < m_lower.z())
			m_lower.z(m_z);

		if(m_x > m_upper.x())
			m_upper.x(m_x) ;
		if(m_y > m_upper.y())
			m_upper.y(m_y) ;
		if(m_z > m_upper.z())
			m_upper.z(m_z) ;	
	}

	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation=m_lower+m_upper;
	m_translation/=-2;
	float length=m_upper.x()-m_lower.x();
	float length1 = m_upper.y() - m_lower.y();
	if(length<m_upper.y()-m_lower.y())
		length=m_upper.y()-m_lower.y();
	if(length<m_upper.z()-m_lower.z())
		length=m_upper.z()-m_lower.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1/length,1/length,1/length);
	transform.SetScale(&m_Scale);
	mesh->m_Transform.Copy(transform);
	for (MyMesh::FaceIter f_it = mesh->pmesh->faces_begin(); f_it != mesh->pmesh->faces_end(); ++f_it)
	{
		mesh->pmesh->set_color(f_it,MyMesh::Color(40,240,120));
	}
	m_SceneGraph.Add(mesh);

	//Delete Test
	/*MyMesh::VertexHandle vhandle[11];
	MyMesh::FaceHandle   fhandle[7];
	i = 0;
	for (MyMesh::FaceIter f_it = mesh->pmesh->faces_begin(); f_it != mesh->pmesh->faces_end(); ++f_it)
	{
		fhandle[i++] = f_it.handle();
	}
	i = 0;

	MyMesh* newmesh = new MyMesh();
	for (MyMesh::VertexIter v_it = mesh->pmesh->vertices_begin(); v_it != mesh->pmesh->vertices_end(); ++v_it)
	{
		MyMesh::Point point = mesh->pmesh->point(v_it.handle());
		vhandle[i++] = newmesh->add_vertex(MyMesh::Point(point[0], point[1],point[2]));
	}
	vhandle[9] = newmesh->add_vertex(MyMesh::Point(0.5, 0,  0));
	vhandle[10] = newmesh->add_vertex(MyMesh::Point(0.5, 1,  0));
	std::vector<MyMesh::VertexHandle>  tmp_face_vhandles;
	tmp_face_vhandles.push_back(vhandle[0]);
	tmp_face_vhandles.push_back(vhandle[9]);
	tmp_face_vhandles.push_back(vhandle[10]);
	tmp_face_vhandles.push_back(vhandle[3]);
	fhandle[0] = newmesh->add_face(tmp_face_vhandles);
	tmp_face_vhandles.clear();
	tmp_face_vhandles.push_back(vhandle[9]);
	tmp_face_vhandles.push_back(vhandle[1]);
	tmp_face_vhandles.push_back(vhandle[4]);
	tmp_face_vhandles.push_back(vhandle[10]);
	fhandle[1] = newmesh->add_face(tmp_face_vhandles);
	tmp_face_vhandles.clear();
	tmp_face_vhandles.push_back(vhandle[3]);
	tmp_face_vhandles.push_back(vhandle[10]);
	tmp_face_vhandles.push_back(vhandle[4]);
	tmp_face_vhandles.push_back(vhandle[7]);
	tmp_face_vhandles.push_back(vhandle[6]);
	fhandle[2] = newmesh->add_face(tmp_face_vhandles);
	tmp_face_vhandles.clear();
	tmp_face_vhandles.push_back(vhandle[1]);
	tmp_face_vhandles.push_back(vhandle[2]);
	tmp_face_vhandles.push_back(vhandle[5]);
	tmp_face_vhandles.push_back(vhandle[4]);
	fhandle[3] = newmesh->add_face(tmp_face_vhandles);
	tmp_face_vhandles.clear();
	tmp_face_vhandles.push_back(vhandle[4]);
	tmp_face_vhandles.push_back(vhandle[5]);
	tmp_face_vhandles.push_back(vhandle[8]);
	tmp_face_vhandles.push_back(vhandle[7]);
	fhandle[4] = newmesh->add_face(tmp_face_vhandles);
	for(MyMesh::VertexOHalfedgeIter voh_it = newmesh->voh_iter(vhandle[3]); voh_it; ++voh_it) 
	{
		// Iterate over all outgoing halfedges...
		MyMesh::VertexHandle fromvh = newmesh->from_vertex_handle(voh_it.handle());
		MyMesh::VertexHandle tovh = newmesh->to_vertex_handle(voh_it.handle());
		MyMesh::FaceHandle fh = newmesh->face_handle(voh_it.handle());
		MyMesh::FaceHandle fh1 = newmesh->opposite_face_handle(voh_it.handle());
		int tes = 1;
	}
	newmesh->garbage_collection();
	mesh->pmesh = newmesh;
	mesh->pmesh->update_normals(); */
	//mesh->pmesh
	/*vhandle[9] = mesh->mesh.add_vertex(MyMesh::Point(0.5, 0,  0));
	vhandle[10] = mesh->mesh.add_vertex(MyMesh::Point(0.5, 1,  0));

	mesh->mesh.delete_face(fhandle[0], FALSE);
	mesh->mesh.delete_face(fhandle[1], FALSE);
	mesh->mesh.delete_face(fhandle[2], FALSE);
	mesh->mesh.delete_face(fhandle[3], FALSE);
	mesh->mesh.garbage_collection();
	std::vector<MyMesh::VertexHandle>  tmp_face_vhandles;
	tmp_face_vhandles.push_back(vhandle[0]);
	tmp_face_vhandles.push_back(vhandle[9]);
	tmp_face_vhandles.push_back(vhandle[10]);
	tmp_face_vhandles.push_back(vhandle[3]);
	fhandle[4] = mesh->mesh.add_face(tmp_face_vhandles);
	tmp_face_vhandles.clear();
	tmp_face_vhandles.push_back(vhandle[9]);
	tmp_face_vhandles.push_back(vhandle[1]);
	tmp_face_vhandles.push_back(vhandle[4]);
	tmp_face_vhandles.push_back(vhandle[10]);
	fhandle[5] = mesh->mesh.add_face(tmp_face_vhandles);
	tmp_face_vhandles.clear();
	tmp_face_vhandles.push_back(vhandle[3]);
	tmp_face_vhandles.push_back(vhandle[10]);
	tmp_face_vhandles.push_back(vhandle[4]);
	tmp_face_vhandles.push_back(vhandle[7]);
	tmp_face_vhandles.push_back(vhandle[6]);
	//fhandle[6] = mesh->mesh.add_face(tmp_face_vhandles);
	//mesh->mesh.delete_vertex(vhandle[0],FALSE);
	mesh->mesh.garbage_collection();*/


	//int test = mesh->mesh.is_polymesh();
}
/////////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::DeleteContents() 
{
	//The file was reopened in the override of CDocument::OnOpenDocument.
	//So, now the file should be closed when the framework wants to
	//clear out the CDocument object for potential reuse.

	if (m_file.m_hFile != CFile::hFileNull) //CFile::hFileNull = -1
		m_file.Close();

	((CSDIViewSwitchApp*)AfxGetApp())->m_pDoc = NULL;
	m_nNumberOfRecords = 0;
}
/////////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::WriteHeader()
{
	m_file.Seek(0L, CFile::begin);
	//writing of the signature only really needs to be done the first time the header is written:
	m_file.Write( &SWITCHSPLIT_FILE_SIGNATURE, sizeof(double) );
	//WriteHeader is called on other occassions to update the number of records:
	m_file.Write(&m_nNumberOfRecords, sizeof(m_nNumberOfRecords));
}
/////////////////////////////////////////////////////////////////////////////////
//Checks if it is a file of the correct type, and reads in the number of records:
BOOL CSDIViewSwitchDoc::ReadHeader()
{   
	m_file.Seek(0L, CFile::begin);
	double fileSignature;
	m_file.Read( &fileSignature, sizeof(double) ); 
	m_file.Read( &m_nNumberOfRecords, sizeof(UINT) ); 
	return( fileSignature == SWITCHSPLIT_FILE_SIGNATURE );
}
/////////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::FileSeekRecord(UINT nRecord)
{
	m_file.Seek( m_nHeaderSize + nRecord*m_nRecordLength, CFile::begin );
}
/////////////////////////////////////////////////////////////////////////////////
bool CSDIViewSwitchDoc::GetRecord(UINT nRecordIndex, void* pRecord)
{
	FileSeekRecord(nRecordIndex);
	if( m_file.Read(pRecord, m_nRecordLength) < m_nRecordLength )
	{
		AfxMessageBox("There is data corruption");
		return false;
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::UpdateRecord(UINT nRecordIndex, void* pRecord)  
{
	FileSeekRecord(nRecordIndex);
	m_file.Write(pRecord, m_nRecordLength);
}
/////////////////////////////////////////////////////////////////////////////////
BOOL CSDIViewSwitchDoc::SaveModified() 
{   //It is needed to override this virtual CDocument function because if the document 
	//is modified, CDocument::SaveModified() will produce a sharing violation because 
	//the file is open already. Since we're saving data on a transaction basis, the 
	//data is already saved. All we need to do is close the file:
	DeleteContents(); 

	return TRUE; // keep going   
}
//***********************************************
// RenderScene
//***********************************************

void CSDIViewSwitchDoc::RenderScene()
{
	// Main drawing
	m_SceneGraph.glDraw();


	//m_SceneGraph.glDraw_Accelerate();
}

void CSDIViewSwitchDoc::SetTransform(float m_xTranslation, float m_yTranslation, float m_zTranslation,
	float m_xRotation, float m_yRotation, float m_zRotation,
	float m_xScaling, float m_yScaling, float m_zScaling)
{
	// Main drawing
	m_SceneGraph.SetTransform(m_xTranslation, m_yTranslation, m_zTranslation,
		m_xRotation, m_yRotation, m_zRotation,
		m_xScaling, m_yScaling, m_zScaling);
	//m_SceneGraph.glDraw_Accelerate();
}

void CSDIViewSwitchDoc::RenderSceneCDC()
{
	for(int i = 0; i < m_SceneGraph.NbObject(); i++)
	{
		CObject3d* pObject = m_SceneGraph.GetAt(i);
		if(pObject->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pNurbs = (CNurbsSuface*)pObject;
			pNurbs->ShowSubSurfaceCoordinate();
			pNurbs->ShowBilinearCoefficients();
		}
	}
}

void CSDIViewSwitchDoc::OnMeshColorHeight() 
{
	// Rainbow height ramp
	CColorRamp ramp;
	ramp.BuildRainbow();
	((CMesh3d *)m_SceneGraph.GetAt(0))->ColorHeight(&ramp);
	UpdateAllViews(NULL);
}
//****************************
// Mode face
//****************************
void CSDIViewSwitchDoc::OnGlFace() 
{
	m_Mode = GL_FILL;
	UpdateAllViews(NULL);
}
void CSDIViewSwitchDoc::OnUpdateGlFace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Mode == GL_FILL);
}
//****************************
// Mode vertex
//****************************
void CSDIViewSwitchDoc::OnGlVertex() 
{
	m_Mode = GL_POINT;
	UpdateAllViews(NULL);
}
void CSDIViewSwitchDoc::OnUpdateGlVertex(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Mode == GL_POINT);
}
//****************************
// Mode light
//****************************
void CSDIViewSwitchDoc::OnGlLight() 
{
	m_Light = !m_Light;
	UpdateAllViews(NULL);
}
void CSDIViewSwitchDoc::OnUpdateGlLight(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Light);
}
//****************************
// Mode line
//****************************
void CSDIViewSwitchDoc::OnGlLine() 
{
	m_Mode = GL_LINE;
	UpdateAllViews(NULL);
}
void CSDIViewSwitchDoc::OnUpdateGlLine(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Mode == GL_LINE);
}
//***********************************************
// Smooth subdivision
//***********************************************
void CSDIViewSwitchDoc::OnMeshLoop() 
{
	BeginWaitCursor();
	int NbObject = m_SceneGraph.NbObject();
	for(int i=0;i<NbObject;i++)
	{
		CObject3d *pObject3d = m_SceneGraph[i];
	  if(pObject3d->GetType() == TYPE_MESH3D)
		{
			CMesh3d *pMesh  = (CMesh3d *)pObject3d;
			pMesh->SubdivisionLoop();
		}
	}
	UpdateAllViews(NULL);
	EndWaitCursor();
}
void CSDIViewSwitchDoc::OnUpdateMeshLoop(CCmdUI* pCmdUI) 
{ 
	pCmdUI->Enable(m_SceneGraph.NbObject() > 0);
}
//***********************************************
// Smooth shading
//***********************************************
void CSDIViewSwitchDoc::OnGlSmooth() 
{
	m_Smooth = !m_Smooth;
	if(m_Smooth)
	{
		m_SceneGraph.SetNormalBinding(NORMAL_PER_VERTEX);
		::glShadeModel(GL_SMOOTH);
		m_SceneGraph.BuildAdjacency();
	}
	else
	{
		m_SceneGraph.SetNormalBinding(NORMAL_PER_FACE);
		::glShadeModel(GL_FLAT);
		m_SceneGraph.BuildAdjacency();
	}
	UpdateAllViews(NULL);
}

void CSDIViewSwitchDoc::OnUpdateGlSmooth(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Smooth);
}
/////////////////////////////////////////////////////////////////////////////////

void CSDIViewSwitchDoc::OnGlAddWireframe() 
{
	// TODO: Add your command handler code here
	m_AddWireframe = !m_AddWireframe;
	UpdateAllViews(NULL);
}

void CSDIViewSwitchDoc::OnUpdateGlAddWireframe(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_AddWireframe);
}

void CSDIViewSwitchDoc::OnFileClose() 
{
	// TODO: Add your command handler code here
	m_SceneGraph.Free();
	UpdateAllViews(NULL);
}

void CSDIViewSwitchDoc::OnBnClickedNavigatestartposition()
{
	// TODO: Add your control notification handler code here
	int ii = 0;
}
void CSDIViewSwitchDoc::UpdateReadTree()
{
	POSITION pos = pDoc->GetFirstViewPosition();
	CFormView3* pFormView;
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CFormView3)))
			pFormView = (CFormView3*)pView;
	}
	pFormView->UpdateReadTree();
}

void CSDIViewSwitchDoc::UpdateShowView()
{
	CMyTreeView* pTreeView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
		{
			pTreeView = (CMyTreeView*)pView;
			pTreeView->Invalidate(TRUE);
			break;
		}
	}
}

