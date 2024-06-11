#include "stdafx.h"
#include "lib3d/mesh3d_openmesh.h"
#include "OpenMesh.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include "Tspline.h"
#include "disline.h"
#include "InterFunction.h"
#include "math.h"
#include "Nurbs3D.h"
#include "C_NurbsSurf.h"
#include "SDIViewSwitchDoc.h"
#include "lib3d/Base3d.h"
#include <windows.h>
#include "MyTreeView.h"
#include <iostream>
#include <fstream>
#include "resource.h"
#include <atlimage.h>
#include <algorithm>

#define SET_PERS(PH) pmesh->property(PH).set_persistent(true); 

Tspline::Tspline(void)
{
	m_ShowTexture = FALSE;
	//preimage = NULL;
	m_Show = TRUE;
	type = TYPE_TSPLINE;
	m_showControlPointsNetwork = FALSE;

	m_showSurface = TRUE;
	m_showControlPointsNetwork = FALSE;
	m_showIsoParameterCurves = FALSE;
	m_showOptimalIsoParameterCurves = FALSE;

	ptransformation = NULL;
	pmesh = NULL;

	//udegree and vdegree
	udegree = vdegree = 3;
}

Tspline::~Tspline(void)
{
}
extern CSDIViewSwitchDoc* pDoc;
void Tspline::BuildTriangulationList()
{
	if(!m_Modified && m_ListDone)
		return;

	// Erase last list
	::glDeleteLists(m_ListOpenGL,1);

	m_ListOpenGL = ::glGenLists(1);
	if(m_ListOpenGL == 0)
	{
		TRACE("CMesh3d::BuildList : unable to build DrawList\n");
		return;
	}
	::glNewList(m_ListOpenGL,GL_COMPILE_AND_EXECUTE);

	int unum = pDoc->m_Tessellation;
	int vnum = pDoc->m_Tessellation;

	//C_NurbsSurf* pNurbsSurf = Converte_C_NurbsSurface();
	::glBegin(GL_TRIANGLES);
	for(int i = 0; i < unum; i++)
		for(int j = 0; j < vnum; j++)
		{
			double u0 = umin + (umax - umin) * (i + 0.0) / unum;
			double v0 = vmin + (vmax - vmin) * (j + 0.0) / vnum;
			double u1 = umin + (umax - umin) * (i + 1.0) / unum;
			double v1 = vmin + (vmax - vmin) * (j + 1.0) / vnum;

			Point3D p00 = ComputePosition(u0,v0);
			Point3D p10 = ComputePosition(u1,v0);
			Point3D p01 = ComputePosition(u0,v1);
			Point3D p11 = ComputePosition(u1,v1);

			//the first triangle p00 p10 p11
			Vector3D vn00 = (p10 - p00) ^ (p11 - p00);
			vn00.SetUnit();
			Vector3D vn10 = vn00;
			Vector3D vn01 = vn00;
			Vector3D vn11 = vn00;
			::glNormal3f(vn00.x,vn00.y,vn00.z);

			if(m_ShowTexture)
				::glTexCoord2f(u0,v0);

			::glVertex3f(p00.x,p00.y,p00.z);


			::glNormal3f(vn10.x,vn10.y,vn10.z);
			if(m_ShowTexture)
				::glTexCoord2f(u1,v0);

			::glVertex3f(p10.x,p10.y,p10.z);


			::glNormal3f(vn11.x,vn11.y,vn11.z);
			if(m_ShowTexture)
				::glTexCoord2f(u1,v1);

			::glVertex3f(p11.x,p11.y,p11.z);

			//the second triangle p00 p11 p01

			::glNormal3f(vn00.x,vn00.y,vn00.z);
			if(m_ShowTexture)
				::glTexCoord2f(u0,v0);

			::glVertex3f(p00.x,p00.y,p00.z);


			::glNormal3f(vn11.x,vn11.y,vn11.z);
			if(m_ShowTexture)
				::glTexCoord2f(u1,v1);
			::glVertex3f(p11.x,p11.y,p11.z);


			::glNormal3f(vn01.x,vn01.y,vn01.z);
			if(m_ShowTexture)
				::glTexCoord2f(u0,v1);

			::glVertex3f(p01.x,p01.y,p01.z);
		}
		::glEnd();
	::glEndList();

	m_ListDone = 1;
	m_Modified = 0;
}
void Tspline::glRenderTriangulation()
{
	if(!m_ListDone || m_Modified)
		BuildTriangulationList();

	// Search for a new list
	if(::glIsList(m_ListOpenGL) == GL_TRUE)
	{
		::glCallList(m_ListOpenGL);
		return;
	}
	else
	{
		TRACE(" CMesh3d::Draw : unable to draw list %d\n",m_ListOpenGL);
		return;
	}
}
int Tspline::glDraw()
{
	if(!m_Show)
		return 0;
	glFrontFace(GL_CW);
	//------------
	// Material

	//
	glEnable(GL_POINT_SMOOTH); // 启用反走样
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND); // 启动融合
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 产生融合因子
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);// 权衡图像质量与绘制速度 


	//construct texture
	GLubyte* pTextureBits;
	GLint width,height;
	
	//---------------
	//This is a for Bezier surface;

	//
	GLboolean re1=glIsEnabled (GL_MAP2_VERTEX_4);
	

	//::glDisable(GL_LIGHTING);
	glEnable(GL_MAP2_VERTEX_4);
	//glColor3f(0.0,0.0,1.0);

	glMapGrid2f(30,0.0,1.0,30,0.0,1.0);
	::glPushMatrix();

	if(!m_SetCameraMode)
	{
		// Position / translation / scaling
		glScalef(m_Transform.GetScale()->x(),
			m_Transform.GetScale()->y(),
			m_Transform.GetScale()->z());

		glRotatef(m_Transform.GetValueRotation(),
			m_Transform.GetRotation()->x(),
			m_Transform.GetRotation()->y(),
			m_Transform.GetRotation()->z());

		glTranslatef(m_Transform.GetTranslation()->x(),
			m_Transform.GetTranslation()->y(),
			m_Transform.GetTranslation()->z());
	}

	//store the model view & projection view.
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);    
	glGetDoublev(GL_PROJECTION_MATRIX, projection);  
	glGetIntegerv(GL_VIEWPORT,m_viewPort);

	if(m_showSurface)
		glRenderTriangulation();

	if(m_showControlPointsNetwork)
		DrawControlPointsNetwork();

	if(m_showIsoParameterCurves)
		DrawIsoParameterCurves();

	::glPopMatrix();
	glFrontFace(GL_CCW);

	glDisable( GL_MAP2_VERTEX_4 ); 
	glDisable( GL_MAP2_TEXTURE_COORD_2 );
	glDisable( GL_TEXTURE_2D );

	return 1;
}
double TsplineVertex3D::GetSknot(int index)
{
	return sknotarray[index];
}
double TsplineVertex3D::GetTknot(int index)
{
	return tknotarray[index];
}
double Tspline::ComputeBasis_U(std::vector<double> &uknotarray, int i, int p,double u)
{
	double ui = uknotarray[i];
	double uia1 = uknotarray[i + 1];
	if(p == 0)
	{
		if(u >= ui && u <= uia1)
			return 1;
		else
			return 0;
	}else
	{
		double denorm1 = (uknotarray[i + p]- ui);
		double denorm2 = (uknotarray[i + p + 1] - uia1);

		double term1,term2;
		if(fabs(denorm1) < 1e-8)
		{
			term1 = 0;
		}else
			term1 = (u -  ui) * ComputeBasis_U(uknotarray,i,p-1,u)  /  denorm1;

		if(fabs(denorm2) < 1e-8)
			term2 = 0;
		else
		    term2 = (uknotarray[i + p + 1] - u) * ComputeBasis_U(uknotarray,i + 1,p - 1,u) / denorm2;

		double u1 = term1  + term2;
		return u1;
	}
}
double Tspline::ComputeBasis_U(std::vector<double> &uknotarray, double u)
{
	int udegree = (uknotarray.size() + 1) / 2;
	return ComputeBasis_U(uknotarray, 0, udegree,u);
}

double Tspline::ComputeBasis_V(std::vector<double> &vknotarray, int i, int p,double v)
{
	double vi = vknotarray[i];
	double via1 = vknotarray[i + 1];
	if(p == 0)
	{
		if(v >= vi && v <= via1)
			return 1;
		else
			return 0;
	}else
	{
		double denorm1 = (vknotarray[i + p]- vi);
		double denorm2 = (vknotarray[i + p + 1] - via1);

		double term1,term2;
		if(fabs(denorm1) < 1e-8)
		{
			term1 = 0;
		}else
			term1 = (v -  vi) * ComputeBasis_V(vknotarray,i,p-1,v)  /  denorm1;

		if(fabs(denorm2) < 1e-8)
			term2 = 0;
		else
			term2 = (vknotarray[i + p + 1] - v) * ComputeBasis_V(vknotarray,i + 1,p - 1,v) / denorm2;

		double v1 = term1 + term2;
		return v1;
	}
}

//Compute the Basis
double Tspline::ComputeBasis(TsplineMesh::VertexHandle vh,double u,double v)
{
	VPROP_VERTEX_UKNOTS vprop_uknots;
	VPROP_VERTEX_VKNOTS vprop_vknots;

	std::vector<double> uknots,vknots;

	//U knots
	if (!pmesh->get_property_handle(vprop_uknots, PROP_VERTEX_UKNOTS))
		pmesh->add_property(vprop_uknots, PROP_VERTEX_UKNOTS);

	//V knots
	if (!pmesh->get_property_handle(vprop_vknots, PROP_VERTEX_VKNOTS))
		pmesh->add_property(vprop_vknots, PROP_VERTEX_VKNOTS);

	//Get the knots
	uknots = pmesh->property(vprop_uknots,vh);
	vknots = pmesh->property(vprop_vknots,vh);

	double basis = ComputeBasis_U(uknots,u) * ComputeBasis_V(vknots,v);
	return basis;
}

//
double Tspline::ComputeBasis_V(std::vector<double> &vknotarray,double v)
{
	int vdegree = (vknotarray.size() + 1) / 2;
	return ComputeBasis_V(vknotarray,0, vdegree,v);
}
void Tspline::copySetting(Tspline* T)
{
	this->m_ListDone = T->m_ListDone;
	this->m_showSurface = T->m_showSurface;
	this->m_showControlPointsNetwork = T->m_showControlPointsNetwork;
	this->m_showIsoParameterCurves = T->m_showIsoParameterCurves;
	this->m_showOptimalIsoParameterCurves = T->m_showOptimalIsoParameterCurves;
	this->udegree = T->udegree;
	this->vdegree = T->vdegree;
	this->umin = T->umin;
	this->umax = T->umax;
	this->vmin = T->vmin;
	this->vmax = T->vmax;
	this->m_Show = T->m_Show;
	this->m_Transform.Copy(&T->m_Transform);
	this->m_ListOpenGL = T->m_ListOpenGL;
	this->m_ListDone = T->m_ListDone;
	this->m_Modified = T->m_Modified;
	this->m_ShowTexture = T->m_ShowTexture;
	if(T->ptransformation)
		this->ptransformation = T->ptransformation->copy();
}
Tspline* Tspline::copy()
{
	Tspline* p = new Tspline();
	p->m_ListDone = this->m_ListDone;
	p->m_showSurface = this->m_showSurface;
	p->m_showControlPointsNetwork = this->m_showControlPointsNetwork;
	p->m_showIsoParameterCurves = this->m_showIsoParameterCurves;
	p->m_showOptimalIsoParameterCurves = this->m_showOptimalIsoParameterCurves;
	p->udegree = this->udegree;
	p->vdegree = this->vdegree;
	p->umin = this->umin;
	p->umax = this->umax;
	p->vmin = this->vmin;
	p->vmax = this->vmax;
	p->m_Show = this->m_Show;
	p->m_Transform.Copy(&this->m_Transform);
	p->m_ListOpenGL = this->m_ListOpenGL;
	p->m_ListDone = this->m_ListDone;
	p->m_Modified = this->m_Modified;
	p->m_ShowTexture = this->m_ShowTexture;
	if(this->ptransformation)
		p->ptransformation = this->ptransformation->copy();
	p->pmesh = new TsplineMesh(*this->pmesh);
	return p;
}
Point3D Tspline::ComputePosition(double u,double v)
{
	VPROP_VERTEX_UV_COORDINATE vprop_2DCoordinate;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag;
	VPROP_VERTEX_UKNOTS vprop_uknots;
	VPROP_VERTEX_VKNOTS vprop_vknots;
	VPROP_VERTEX_WEIGHT vprop_vweights;

	//vertex UV Coordinates
	if (!pmesh->get_property_handle(vprop_2DCoordinate, PROP_VERTEX_UV_COORDINATE))
		pmesh->add_property(vprop_2DCoordinate, PROP_VERTEX_UV_COORDINATE);

	//vertex imagination flag
	if (!pmesh->get_property_handle(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG))
		pmesh->add_property(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG);

	//U knots
	if (!pmesh->get_property_handle(vprop_uknots, PROP_VERTEX_UKNOTS))
		pmesh->add_property(vprop_uknots, PROP_VERTEX_UKNOTS);

	//V knots
	if (!pmesh->get_property_handle(vprop_vknots, PROP_VERTEX_VKNOTS))
		pmesh->add_property(vprop_vknots, PROP_VERTEX_VKNOTS);

	//Vertex weight
	if (!pmesh->get_property_handle(vprop_vweights, PROP_VERTEX_WEIGHT))
		pmesh->add_property(vprop_vweights, PROP_VERTEX_WEIGHT);


	std::vector<double> uknots,vknots;
	Vector4D m_vertex(0,0,0,0);
	Vector4D m_currentvertex;
	for( TsplineMesh::VertexIter v_iter = pmesh->vertices_begin(); v_iter!=pmesh->vertices_end(); ++v_iter ) 
	{
		TsplineMesh::VertexHandle vh = v_iter.handle();
		
		//ignore imagination vertex
		int flag = pmesh->property(vprop_IMGFlag,vh);
		if(flag)
			continue;

		//vertex position
		OpenMesh::Vec3d pt;
		pt = pmesh->point (vh);
		
		//Get the knots
		uknots = pmesh->property(vprop_uknots,vh);
		vknots = pmesh->property(vprop_vknots,vh);

		//weight
		double weight = pmesh->property(vprop_vweights,vh);

		m_currentvertex[0] = pt[0] * weight;
		m_currentvertex[1] = pt[1] * weight;
		m_currentvertex[2] = pt[2] * weight;
		m_currentvertex[3] = weight;

		double ubasis,vbasis;
		ubasis = ComputeBasis_U(uknots,u);
		vbasis = ComputeBasis_V(vknots,v);
		m_vertex += m_currentvertex * ubasis * vbasis;
	}

	Point3D point;
	point[0] = m_vertex[0] / m_vertex[3];
	point[1] = m_vertex[1] / m_vertex[3];
	point[2] = m_vertex[2] / m_vertex[3];
	return point;
}
int indexmap(int i,int j, int m, int n)
{
	return m * j + i;
}
void Tspline::BuildFromNURBS(CNurbsSuface* pNurbsSurface,std::vector<int> &m_uknotindex,std::vector<int> &m_vknotindex)
{
	//generate an openmesh for the Tspline 
	if(pmesh)
		delete pmesh;

	pmesh = new TsplineMesh();
	VPROP_VERTEX_UV_COORDINATE vprop_2DCoordinate;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag;
	VPROP_VERTEX_UKNOTS vprop_uknots;
	VPROP_VERTEX_VKNOTS vprop_vknots;
	VPROP_VERTEX_WEIGHT vprop_vweights;
	VPROP_VERTEX_UV_PLOTCOORDINATE vprop_2DPlotCoordinate;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_2DCoordinate_Index;

	//vertex normal
	pmesh->request_vertex_normals(); 

	//vertex UV Coordinate Index
	if (!pmesh->get_property_handle(vprop_2DCoordinate_Index, PROP_VERTEX_UV_COORDINATE_INDEX))
		pmesh->add_property(vprop_2DCoordinate_Index, PROP_VERTEX_UV_COORDINATE_INDEX);

	if (!pmesh->get_property_handle(vprop_2DCoordinate, PROP_VERTEX_UV_COORDINATE))
		pmesh->add_property(vprop_2DCoordinate, PROP_VERTEX_UV_COORDINATE);

	//vertex UV PlotCoordinates
	if (!pmesh->get_property_handle(vprop_2DPlotCoordinate, PROP_VERTEX_UV_PLOTCOORDINATE))
		pmesh->add_property(vprop_2DPlotCoordinate, PROP_VERTEX_UV_PLOTCOORDINATE);
	
	//vertex imagination flag
	if (!pmesh->get_property_handle(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG))
		pmesh->add_property(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG);

	//U knots
	if (!pmesh->get_property_handle(vprop_uknots, PROP_VERTEX_UKNOTS))
		pmesh->add_property(vprop_uknots, PROP_VERTEX_UKNOTS);

	//V knots
	if (!pmesh->get_property_handle(vprop_vknots, PROP_VERTEX_VKNOTS))
		pmesh->add_property(vprop_vknots, PROP_VERTEX_VKNOTS);

	//Vertex weight
	if (!pmesh->get_property_handle(vprop_vweights, PROP_VERTEX_WEIGHT))
		pmesh->add_property(vprop_vweights, PROP_VERTEX_WEIGHT);

	int uknotnum = pNurbsSurface->m_uknotcount;
	int vknotnum = pNurbsSurface->m_vknotcount;

	C_NurbsSurf* pSurface = pNurbsSurface->Converte_C_NurbsSurface();

	//degree
	udegree = pSurface->GetDegreeU();
	vdegree = pSurface->GetDegreeV();

	umin = pSurface->GetUKnot(pSurface->GetDegreeU() - 1);
	umax = pSurface->GetUKnot(uknotnum  - pSurface->GetDegreeU());

	vmin = pSurface->GetVKnot(pSurface->GetDegreeV() - 1);
	vmax = pSurface->GetVKnot(vknotnum  - pSurface->GetDegreeV());

	std::vector<std::vector<TsplineMesh::VertexHandle>> vhandlearray; 
	std::vector<TsplineMesh::VertexHandle> rowvhandlearray;
	Vector2D uv_coordinate,uv_plotcoordinate;

	std::vector<double> uknots,vknots;

	//build the mesh
	int IMGFlag;

	std::vector<int> coordinateindex;

	//add the vertices
	for(int j = 0; j < vknotnum; j++)
	{
		//First clear the array
		rowvhandlearray.clear();

		for(int i = 0; i < uknotnum; i++)
		{
			coordinateindex.clear();
				
			if(m_uknotindex.size() > 0 && m_vknotindex.size() > 0)
			{
				coordinateindex.push_back(m_uknotindex[i]);
				coordinateindex.push_back(m_vknotindex[j]);
			}else
			{
				coordinateindex.push_back(i);
				coordinateindex.push_back(j);
			}

			double uknot = pNurbsSurface->m_uknot[i];
			double vknot = pNurbsSurface->m_vknot[j];

			uv_plotcoordinate.x = i;
			uv_plotcoordinate.y = vknotnum - 1 -j;

			//add the vertex and set the position
			int uindex = i - pSurface->GetDegreeU() + 1;
			int vindex = j - pSurface->GetDegreeV() + 1;
			Point3D p;
			double w;

			//set the initial value
			p.setValue(0,0,0);
			w = 0;

			//Imagination flag
			IMGFlag = 0;
			if(uindex >= 0 && uindex < pNurbsSurface->GetCtrlNumberU() && 
			   vindex >= 0 && vindex < pNurbsSurface->GetCtrlNumberV())
			{
				p = pNurbsSurface->GetControlPoint(uindex,vindex);
				w = pNurbsSurface->GetWeight(uindex,vindex);
			}
			else
			{
				IMGFlag = 1;
			}
			
			TsplineMesh::VertexHandle vh = pmesh->add_vertex(MyMesh::Point(p.x, p.y, p.z));

			//set the imagination flag
			pmesh->property(vprop_IMGFlag,vh) = IMGFlag;

			//add the knot information
			uv_coordinate.x = uknot;
			uv_coordinate.y = vknot;
			pmesh->property(vprop_2DCoordinate,vh) = uv_coordinate;

			//coordinate index
			pmesh->property(vprop_2DCoordinate_Index,vh) = coordinateindex;

			//set the plot coordinates
			pmesh->property(vprop_2DPlotCoordinate,vh) = uv_plotcoordinate;

			//add the weight property
			pmesh->property(vprop_vweights,vh) = w;

			if (!IMGFlag)
			{
				uknots.clear();
				vknots.clear();
				uknots.push_back(pNurbsSurface->m_uknot[i - 2]);
				uknots.push_back(pNurbsSurface->m_uknot[i - 1]);
				uknots.push_back(pNurbsSurface->m_uknot[i]);
				uknots.push_back(pNurbsSurface->m_uknot[i + 1]);
				uknots.push_back(pNurbsSurface->m_uknot[i + 2]);
				vknots.push_back(pNurbsSurface->m_vknot[j - 2]);
				vknots.push_back(pNurbsSurface->m_vknot[j - 1]);
				vknots.push_back(pNurbsSurface->m_vknot[j]);
				vknots.push_back(pNurbsSurface->m_vknot[j + 1]);
				vknots.push_back(pNurbsSurface->m_vknot[j + 2]);
				pmesh->property(vprop_uknots,vh) = uknots;
				pmesh->property(vprop_vknots,vh) = vknots;
			}
			
			rowvhandlearray.push_back(vh);
		}
		vhandlearray.push_back(rowvhandlearray);
	}

	std::vector<TsplineMesh::VertexHandle> faceVertexHandles;

	//add the faces
	int facenum = 0;
	for(int j = 0; j < vknotnum - 1; j++)
		for(int i = 0; i < uknotnum - 1; i++)
		{
			//
			//int vIMG_Flag1 = pmesh->property(vprop_IMGFlag,vhandlearray[j][i]);
			//int vIMG_Flag2 = pmesh->property(vprop_IMGFlag,vhandlearray[j][i + 1]);	
			//int vIMG_Flag3 = pmesh->property(vprop_IMGFlag,vhandlearray[j + 1][i]);	
			//int vIMG_Flag4 = pmesh->property(vprop_IMGFlag,vhandlearray[j + 1][i + 1]);

			//if(vIMG_Flag1 || vIMG_Flag2 || vIMG_Flag3 || vIMG_Flag4)
			//	continue;

			faceVertexHandles.clear();
			faceVertexHandles.push_back( vhandlearray[j + 1][i]);
			faceVertexHandles.push_back( vhandlearray[j][i]);
			faceVertexHandles.push_back( vhandlearray[j][i + 1]);
			faceVertexHandles.push_back( vhandlearray[j + 1][i + 1]);

			pmesh->add_face( faceVertexHandles );
			facenum++;
		}
}
struct sort_pred {
	bool operator()(const std::pair<double,double> &left, const std::pair<double,double> &right) {
		return left.second < right.second;
	}
};

void Tspline::SetUVKnotsVector()
{
	//Set UV Knot Vector From the preimage structure

	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag;
	VPROP_VERTEX_UV_COORDINATE vprop_UV_Coordinate;
	VPROP_VERTEX_UV_PLOTCOORDINATE vprop_UV_PlotCoordinate;
	VPROP_VERTEX_UKNOTS vprop_Vertex_Uknots;
	VPROP_VERTEX_VKNOTS vprop_Vertex_Vknots;

	//vertex imagination flag
	if (!pmesh->get_property_handle(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG))
		pmesh->add_property(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG);

	//vertex UV vector
	if (!pmesh->get_property_handle(vprop_UV_Coordinate, PROP_VERTEX_UV_COORDINATE))
		pmesh->add_property(vprop_UV_Coordinate, PROP_VERTEX_UV_COORDINATE);

	//vertex Plot UV vector
	if (!pmesh->get_property_handle(vprop_UV_PlotCoordinate, PROP_VERTEX_UV_PLOTCOORDINATE))
		pmesh->add_property(vprop_UV_PlotCoordinate, PROP_VERTEX_UV_PLOTCOORDINATE);

	//UKnot vector
	if (!pmesh->get_property_handle(vprop_Vertex_Uknots, PROP_VERTEX_UKNOTS))
		pmesh->add_property(vprop_Vertex_Uknots, PROP_VERTEX_UKNOTS);

	//VKnot vector
	if (!pmesh->get_property_handle(vprop_Vertex_Vknots, PROP_VERTEX_VKNOTS))
		pmesh->add_property(vprop_Vertex_Vknots, PROP_VERTEX_VKNOTS);

	std::vector<std::pair<double,double>>::iterator fiterator;

	int flag;
	for( TsplineMesh::VertexIter v_iter = pmesh->vertices_begin(); v_iter!=pmesh->vertices_end(); ++v_iter ) 
	{
		flag = pmesh->property(vprop_IMGFlag,v_iter.handle());
		if(flag)
			continue;

		TRACE("%d \n",v_iter.handle().idx());
		if(v_iter.handle().idx() == 66)
		{
			int ii = 0;
		}
		Vector2D uv = pmesh->property(vprop_UV_Coordinate,v_iter.handle());
		Vector2D plotuv = pmesh->property(vprop_UV_PlotCoordinate,v_iter.handle());

		//uv & plot coordinates
		std::vector<std::pair<double,double>> uknotvector;
		std::vector<std::pair<double,double>> vknotvector;

		uknotvector.clear();
		vknotvector.clear();

		//Set U vectors
		for( TsplineMesh::EdgeIter e_iter = pmesh->edges_begin(); e_iter!=pmesh->edges_end(); ++e_iter ) 
		{
		
			TsplineMesh::HalfedgeHandle hd = pmesh->halfedge_handle(e_iter.handle(),0);
			if(pmesh->is_valid_handle(hd))
			{
				TsplineMesh::VertexHandle fromvh = pmesh->from_vertex_handle(hd);
				TsplineMesh::VertexHandle tovh = pmesh->to_vertex_handle(hd);

				Vector2D para1 = pmesh->property(vprop_UV_Coordinate,fromvh);
				Vector2D para2 = pmesh->property(vprop_UV_Coordinate,tovh);

				Vector2D plotpara1 = pmesh->property(vprop_UV_PlotCoordinate,fromvh);
				Vector2D plotpara2 = pmesh->property(vprop_UV_PlotCoordinate,tovh);

				// u knot vectors
				if(fabs(plotpara1.x - plotpara2.x) < 1e-8)
				{
					//a vertical edge
					if(plotuv.y >= plotpara1.y && plotuv.y <= plotpara2.y || plotuv.y >= plotpara2.y && plotuv.y <= plotpara1.y)
					{
						std::pair<double,double> pair;
						pair.first = para1.x;
						pair.second = plotpara1.x;

						fiterator = std::find (uknotvector.begin(), uknotvector.end(),pair);
						if(fiterator == uknotvector.end())
							uknotvector.push_back(pair);
					}
				}

				//v knot vectors
				if(fabs(plotpara1.y - plotpara2.y) < 1e-8)
				{
					//a vertical edge
					if(plotuv.x >= plotpara1.x && plotuv.x <= plotpara2.x || plotuv.x >= plotpara2.x && plotuv.x <= plotpara1.x)
					{
						std::pair<double,double> pair;
						pair.first = para1.y;
						pair.second = plotpara1.y;

						fiterator = std::find (vknotvector.begin(), vknotvector.end(),pair);
						if(fiterator == vknotvector.end())
							vknotvector.push_back(pair);
					}
				}
			}	
		}
		std::sort (uknotvector.begin(), uknotvector.end(), sort_pred());
		std::sort (vknotvector.begin(), vknotvector.end(), sort_pred());
		
		//find the index
		int uindex,vindex;
		std::vector<std::pair<double,double>>::iterator uiterator,viterrator;
		std::pair<double,double> pairx,pairy;
		pairx.first = uv.x;
		pairx.second = plotuv.x;
		pairy.first = uv.y;
		pairy.second = plotuv.y;
		uiterator = std::find (uknotvector.begin(), uknotvector.end(),pairx);
		viterrator = std::find (vknotvector.begin(), vknotvector.end(),pairy);
		
		uindex = uiterator - uknotvector.begin();
		vindex = viterrator - vknotvector.begin();

		std::vector<double> unewknotvector;
		std::vector<double> vnewknotvector;
		unewknotvector.push_back(uknotvector[uindex - 2].first);
		unewknotvector.push_back(uknotvector[uindex - 1].first);
		unewknotvector.push_back(uknotvector[uindex].first);
		unewknotvector.push_back(uknotvector[uindex + 1].first);
		unewknotvector.push_back(uknotvector[uindex + 2].first);

		vnewknotvector.push_back(vknotvector[vindex + 2].first);
		vnewknotvector.push_back(vknotvector[vindex + 1].first);
		vnewknotvector.push_back(vknotvector[vindex].first);
		vnewknotvector.push_back(vknotvector[vindex - 1].first);
		vnewknotvector.push_back(vknotvector[vindex - 2].first);

		pmesh->property(vprop_Vertex_Uknots,v_iter.handle()) = unewknotvector;
		pmesh->property(vprop_Vertex_Vknots,v_iter.handle()) = vnewknotvector;
	}
}
void Tspline::GenerateSamplingMesh(CMesh3d* &pMesh3D,CMesh3d* &pMesh2D,int samplesm,int samplesn)
{
	pMesh3D = new CMesh3d();
	pMesh2D = new CMesh3d();

	int i,j;

	for(j = 0; j <= samplesn; j++)
	{
		for(i = 0; i <= samplesm; i++)
		{	//

			double s = umin + (double)i * (umax - umin) / samplesm;
			double t = vmin + (double)j * (vmax - vmin) / samplesn;

			Vector3D vv1 = (ComputePosition(s,t) - ZeroP3D);

			//
			CVertex3d* pVertex3D = new CVertex3d(vv1.x,vv1.y,vv1.z);
			CVertex3d* pVertex2D = new CVertex3d(s,t,0);

			pMesh3D->AddVertex(pVertex3D);
			pMesh2D->AddVertex(pVertex2D);
		}
	}

	for(j = 0; j < samplesn; j++)
	{
		for(i = 0; i < samplesm; i++)
		{
			//
			//outfile << "f " << (samplesn + 1) * j + i + 1 <<" " << (samplesn + 1) * j + i + 2 << " "<< (samplesn + 1) * j + i + 2 + samplesn + 1 << " "<< (samplesn + 1) * j + i + 1 + samplesn + 1 << endl;
			int index1 = (samplesn + 1) * j + i;
			int index2 = (samplesn + 1) * j + i + 1;
			int index3 = (samplesn + 1) * j + i + 1 + samplesn;
			int index4 = (samplesn + 1) * j + i + 2 + samplesn;
			pMesh3D->AddFace(new CFace3d(pMesh3D->GetVertex(index1),pMesh3D->GetVertex(index2),pMesh3D->GetVertex(index3)));
			pMesh3D->AddFace(new CFace3d(pMesh3D->GetVertex(index2),pMesh3D->GetVertex(index4),pMesh3D->GetVertex(index3)));
			pMesh2D->AddFace(new CFace3d(pMesh2D->GetVertex(index1),pMesh2D->GetVertex(index2),pMesh2D->GetVertex(index3)));
			pMesh2D->AddFace(new CFace3d(pMesh2D->GetVertex(index2),pMesh2D->GetVertex(index4),pMesh2D->GetVertex(index3)));
		}
	}

	pMesh3D->CalculateNormalPerFace();
	pMesh3D->CalculateNormalPerVertex();
}
void Tspline::DrawOriginalControlPointsNetwork()
{
	glPointSize(5.0);
	::glBegin(GL_POINTS);

	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag;

	//vertex imagination flag
	if (!pmesh->get_property_handle(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG))
		pmesh->add_property(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG);

	//vertices
	int ii = 0;
	int flag;
	for( TsplineMesh::VertexIter v_iter = pmesh->vertices_begin(); v_iter!=pmesh->vertices_end(); ++v_iter ) 
	{
		flag = pmesh->property(vprop_IMGFlag,v_iter.handle());
		if(flag)
			continue;

		TsplineMesh::Point pt = pmesh->point( v_iter );
		glLoadName(ii);
		ii++;
		glVertex3f(pt[0],pt[1],pt[2]);
	}
	::glEnd();

	//edges
	::glBegin(GL_LINES);
	for( TsplineMesh::EdgeIter e_iter = pmesh->edges_begin(); e_iter!=pmesh->edges_end(); ++e_iter ) 
	{
		TsplineMesh::HalfedgeHandle heHandle = pmesh->halfedge_handle( e_iter, 0 );
		flag = pmesh->property(vprop_IMGFlag,pmesh->from_vertex_handle( heHandle ));
		if(flag)
			continue;

		flag = pmesh->property(vprop_IMGFlag,pmesh->to_vertex_handle( heHandle ));
		if(flag)
			continue;
		TsplineMesh::Point pt1 = pmesh->point( pmesh->from_vertex_handle( heHandle ) );
		TsplineMesh::Point pt2 = pmesh->point( pmesh->to_vertex_handle( heHandle ) );

		glVertex3f(pt1[0],pt1[1],pt1[2]);
		glVertex3f(pt2[0],pt2[1],pt2[2]);
	}
	::glEnd();
}
void Tspline::DrawControlPointsNetwork()
{
	::glDisable(GL_LIGHTING);
	glEnable (GL_LINE_SMOOTH);     /* Enable Antialiased lines */
	glEnable (GL_LINE_STIPPLE);
	glColor3f(0,1,0);
	glLineWidth(1.0f);
	//
	if(ptransformation == NULL)
	{
		//draw the original control points
		DrawOriginalControlPointsNetwork();
	}
	::glEnable(GL_LIGHTING);
}

void Tspline::DrawIsoParameterCurves()
{
	//glDisable(GL_DEPTH_TEST); 
	::glDisable(GL_LIGHTING);
	glEnable (GL_LINE_SMOOTH);     /* Enable Antialiased lines */
	glEnable (GL_LINE_STIPPLE);

	glLineWidth(1.0f);
	//pDoc->m_Disc1 = 28;
	//pDoc->m_Disc2 = 30;
	for(int j=0;j<=pDoc->m_Disc1;j++)
	{
		for(int i=0;i<pDoc->m_Disc2;i++)
		{
			::glBegin(GL_LINES);

			double s1 = ((GLfloat)i)/pDoc->m_Disc2;
			double t1 = ((GLfloat)j)/pDoc->m_Disc1;
			double s2 = ((GLfloat)i+1)/pDoc->m_Disc2;
			double t2 = ((GLfloat)j)/pDoc->m_Disc1;
			double u1,v1,u2,v2;
			s1 = umin + (umax - umin) * s1;
			t1 = vmin + (vmax - vmin) * t1;
			s2 = umin + (umax - umin) * s2;
			t2 = vmin + (vmax - vmin) * t2;

			u1 = s1;
			v1 = t1;
			u2 = s2;
			v2 = t2;

			Point3D p1 = ComputePosition(u1,v1);
			Point3D p2 = ComputePosition(u2,v2);
			::glVertex3f(p1.x,p1.y,p1.z);
			::glVertex3f(p2.x,p2.y,p2.z);
			::glEnd();
		}
		for(int i=0;i<pDoc->m_Disc2;i++)
		{
			::glBegin(GL_LINES);
			double s1 = ((GLfloat)j)/pDoc->m_Disc1;
			double t1 = ((GLfloat)i)/pDoc->m_Disc2;
			double s2 = ((GLfloat)j)/pDoc->m_Disc1;
			double t2 = ((GLfloat)(i+1))/pDoc->m_Disc2;

			double u1,v1,u2,v2;

			s1 = umin + (umax - umin) * s1;
			t1 = vmin + (vmax - vmin) * t1;
			s2 = umin + (umax - umin) * s2;
			t2 = vmin + (vmax - vmin) * t2;

			u1 = s1;
			v1 = t1;
			u2 = s2;
			v2 = t2;

			Point3D p1 = ComputePosition(u1,v1);
			Point3D p2 = ComputePosition(u2,v2);
			::glVertex3f(p1.x,p1.y,p1.z);
			::glVertex3f(p2.x,p2.y,p2.z);
			::glEnd();
		}

	}

	::glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST); 
}
int Tspline::GetNon_ImaginationVertices()
{
	VPROP_VERTEX_IMG_FLAG  vprop_IMGFlag;;

	//Vertex offending
	if (!pmesh->get_property_handle(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG))
		pmesh->add_property(vprop_IMGFlag,PROP_VERTEX_IMG_FLAG);

	int num = 0;
	for( TsplineMesh::VertexIter v_iter = pmesh->vertices_begin(); v_iter!= pmesh->vertices_end(); ++v_iter ) 
	{
		int flag = pmesh->property(vprop_IMGFlag,v_iter.handle());
		if(!flag)
			num++;
	}
	return num;
}
void Tspline::Save(std::ofstream &file,CString filename)
{
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_uv_coordinate_index;
	VPROP_VERTEX_UV_COORDINATE vprop_uv_coordinate;
	VPROP_VERTEX_UKNOTS vprop_uknots;
	VPROP_VERTEX_VKNOTS vprop_vknots;
	VPROP_VERTEX_IMG_FLAG vprop_img_flag;
	VPROP_VERTEX_OFFENDING_FLAG vprop_offending_flag;
	VPROP_VERTEX_WEIGHT vprop_weight;
	FPROP_FACE_OFFENDING_FLAG fprop_offending_flag;
	FPROP_FACE_UK_BOUNDING_BOX fprop_uk_bounding_box;
	VPROP_VERTEX_TARGET_IN_CURRENT_INDEX vprop_target_in_current_index;
	VPROP_VERTEX_TARGET_IN_CURRENT_FLAG vprop_target_in_current_flag;
	VPROP_VERTEX_UV_PLOTCOORDINATE vprop_uv_plotcoordinate;
	VPROP_VERTEX_CORNER_LABEL vprop_corner_label;

	if (!pmesh->get_property_handle(vprop_uv_coordinate_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		pmesh->add_property(vprop_uv_coordinate_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	if (!pmesh->get_property_handle(vprop_uv_coordinate, PROP_VERTEX_UV_COORDINATE))
		pmesh->add_property(vprop_uv_coordinate, PROP_VERTEX_UV_COORDINATE);

	if (!pmesh->get_property_handle(vprop_uknots, PROP_VERTEX_UKNOTS))
		pmesh->add_property(vprop_uknots, PROP_VERTEX_UKNOTS);

	if (!pmesh->get_property_handle(vprop_vknots, PROP_VERTEX_VKNOTS))
		pmesh->add_property(vprop_vknots, PROP_VERTEX_VKNOTS);

	if (!pmesh->get_property_handle(vprop_img_flag, PROP_VERTEX_IMG_FLAG))
		pmesh->add_property(vprop_img_flag, PROP_VERTEX_IMG_FLAG);

	if (!pmesh->get_property_handle(vprop_offending_flag, PROP_VERTEX_OFFENDING_FLAG))
		pmesh->add_property(vprop_offending_flag, PROP_VERTEX_OFFENDING_FLAG);

	if (!pmesh->get_property_handle(vprop_weight, PROP_VERTEX_WEIGHT))
		pmesh->add_property(vprop_weight, PROP_VERTEX_WEIGHT);

	if (!pmesh->get_property_handle(fprop_offending_flag, PROP_FACE_OFFENDING_FLAG))
		pmesh->add_property(fprop_offending_flag, PROP_FACE_OFFENDING_FLAG);

	if (!pmesh->get_property_handle(fprop_uk_bounding_box, PROP_FACE_UV_BOUNDINGBOX))
		pmesh->add_property(fprop_uk_bounding_box, PROP_FACE_UV_BOUNDINGBOX);

	if (!pmesh->get_property_handle(vprop_target_in_current_index, PROP_VERTEX_TARGET_IN_CURRENT_INDEX))
		pmesh->add_property(vprop_target_in_current_index, PROP_VERTEX_TARGET_IN_CURRENT_INDEX);

	if (!pmesh->get_property_handle(vprop_target_in_current_flag, PROP_VERTEX_TARGET_IN_CURRENT_FLAG))
		pmesh->add_property(vprop_target_in_current_flag, PROP_VERTEX_TARGET_IN_CURRENT_FLAG);

	if (!pmesh->get_property_handle(vprop_uv_plotcoordinate, PROP_VERTEX_UV_PLOTCOORDINATE))
		pmesh->add_property(vprop_uv_plotcoordinate, PROP_VERTEX_UV_PLOTCOORDINATE);

	if (!pmesh->get_property_handle(vprop_corner_label, PROP_VERTEX_CORNER_LABEL))
		pmesh->add_property(vprop_corner_label, PROP_VERTEX_CORNER_LABEL);

	//pmesh->mproperty(vprop_uv_coordinate_index).set_persistent(true);
	//pmesh->mproperty(vprop_uknots).set_persistent(true);
	//pmesh->mproperty(vprop_vknots).set_persistent(true);
	//pmesh->mproperty(fprop_uk_bounding_box).set_persistent(true);
	SET_PERS(vprop_uv_coordinate_index);
	SET_PERS(vprop_uv_coordinate);
	SET_PERS(vprop_uknots);
	SET_PERS(vprop_vknots);
	SET_PERS(vprop_img_flag);
	SET_PERS(vprop_offending_flag);
	SET_PERS(vprop_weight);
	SET_PERS(fprop_offending_flag);
	SET_PERS(fprop_uk_bounding_box);
	SET_PERS(vprop_target_in_current_index);
	SET_PERS(vprop_target_in_current_flag);
	SET_PERS(vprop_uv_plotcoordinate);
	SET_PERS(vprop_corner_label);

	//Change the filename
	int index = filename.Find('.');
	//filename.Insert(index,'_');
	std::string omname = filename.Left(index) + ".om";
	std::string objname = filename.Left(index) + ".obj";
	//file << pmesh->n_vertices() << " "  << pmesh->n_faces() << "\n";

	//check
	std::vector<int> uvindex_;
	uvindex_.push_back(-1);
	uvindex_.push_back(-1);
	std::vector<double> uknots_;
	uknots_.push_back(-1);
	std::vector<double> vknots_;
	vknots_.push_back(-1);
	for( TsplineMesh::VertexIter v_iter = pmesh->vertices_begin(); v_iter!= pmesh->vertices_end(); ++v_iter ) 
	{
		std::vector<int> uvindex = pmesh->property(vprop_uv_coordinate_index,v_iter.handle());
		if(uvindex.size() == 0)
			pmesh->property(vprop_uv_coordinate_index,v_iter.handle()) = uvindex_;

		std::vector<double> uknots = pmesh->property(vprop_uknots,v_iter.handle());
		if(uknots.size() == 0)
			pmesh->property(vprop_uknots,v_iter.handle()) = uknots_;

		std::vector<double> vknots = pmesh->property(vprop_vknots,v_iter.handle());
		if(vknots.size() == 0)
			pmesh->property(vprop_vknots,v_iter.handle()) = vknots_;
	}
	std::vector<double> fb_;
	fb_.push_back(-1);
	for( TsplineMesh::FaceIter f_iter = pmesh->faces_begin(); f_iter!= pmesh->faces_end(); ++f_iter ) 
	{
		std::vector<double> fb = pmesh->property(fprop_uk_bounding_box,f_iter.handle());
		if(fb.size() == 0)
			pmesh->property(fprop_uk_bounding_box,f_iter.handle()) = fb_;		
	}

	//if (!OpenMesh::IO::write_mesh(*pmesh, objname)) 
	//{
	//	return;
	//}

	if (!OpenMesh::IO::write_mesh(*pmesh, omname)) 
	{
		return;
	}
	file << m_showSurface << "\n";
	file << m_showControlPointsNetwork << "\n";
	file << m_showIsoParameterCurves << "\n";
	file << m_showOptimalIsoParameterCurves << "\n";

	file << m_targetNurbsUKnots.size() << "\n";
	for(int i = 0; i < m_targetNurbsUKnots.size(); i++)
		file << m_targetNurbsUKnots[i] << " ";
	file << "\n";

	file << m_targetNurbsVKnots.size() << "\n";
	for(int i = 0; i < m_targetNurbsVKnots.size(); i++)
		file << m_targetNurbsVKnots[i] << " ";
	file << "\n";

	file << udegree << " " << vdegree << "\n";
	file << umin << " " << umax << " " << vmin << " " << vmax << "\n";
	file << m_Show << "\n";
	
	file << m_ListOpenGL << " " << m_ListDone << " " << m_Modified << " " << m_ShowTexture << "\n";

	m_Transform.Save(file);
	file.close();
}
//Get the bounding box
void Tspline::GetBoundingBox(Vector3D &lower, Vector3D &upper)
{
		int resolution = 100;
	int i,j;

	double umin,umax,vmin,vmax;
	if(ptransformation == NULL)
	{
		umin = this->umin;
		umax = this->umax;
		vmin = this->vmin;
		vmax = this->vmax;
	}else
	{
		umin = ptransformation->umin_Reparameterization;
		umax = ptransformation->umax_Reparameterization;
		vmin = ptransformation->vmin_Reparameterization;
		vmax = ptransformation->vmax_Reparameterization;
	}

	//Vector3D p = ComputPosition(0,0) - ZeroP3D;
	lower.x = 1e10;
	lower.y = 1e10;
	lower.z = 1e10;
	upper.x = -1e10;
	upper.y = -1e10;
	upper.z = -1e10;

	for(i = 0; i <= resolution; i++)
		for(j = 0; j <= resolution; j++)
		{
			double u = (i + 0.0) / resolution;
			double v = (j + 0.0) / resolution;

			u = umin + (umax - umin) * u;
			v = vmin + (vmax - vmin) * v;
			/*if(ptransformation)
			{
				double u1,v1;
				ptransformation->Evaluation(u,v,u1,v1);
				u = u1;
				v = v1;
			}*/

			Vector3D p = ComputePosition(u,v) - ZeroP3D;
			
			if(p[0] < lower[0])
				lower[0] = p[0];
			if(p[1] < lower[1])
				lower[1] = p[1];
			if(p[2] < lower[2])
				lower[2] = p[2];

			if(p[0] > upper[0])
				upper[0] = p[0];
			if(p[1] > upper[1])
				upper[1] = p[1];
			if(p[2] > upper[2])
				upper[2] = p[2];
		}
}
void Tspline::Read(std::ifstream &file,CString filename)
{
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_uv_coordinate_index;
	VPROP_VERTEX_UV_COORDINATE vprop_uv_coordinate;
	VPROP_VERTEX_UKNOTS vprop_uknots;
	VPROP_VERTEX_VKNOTS vprop_vknots;
	VPROP_VERTEX_IMG_FLAG vprop_img_flag;
	VPROP_VERTEX_OFFENDING_FLAG vprop_offending_flag;
	VPROP_VERTEX_WEIGHT vprop_weight;
	FPROP_FACE_OFFENDING_FLAG fprop_offending_flag;
	FPROP_FACE_UK_BOUNDING_BOX fprop_uk_bounding_box;
	VPROP_VERTEX_TARGET_IN_CURRENT_INDEX vprop_target_in_current_index;
	VPROP_VERTEX_TARGET_IN_CURRENT_FLAG vprop_target_in_current_flag;
	VPROP_VERTEX_UV_PLOTCOORDINATE vprop_uv_plotcoordinate;
	VPROP_VERTEX_CORNER_LABEL vprop_corner_label;

	if(pmesh)
		delete pmesh;
	//Change the filename
	int index = filename.Find('.');
	std::string omname = filename.Left(index) + ".om";
	std::string objname = filename.Left(index) + ".obj";
	//std::string name = filename;
	pmesh = new TsplineMesh();

	if (!pmesh->get_property_handle(vprop_uv_coordinate_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		pmesh->add_property(vprop_uv_coordinate_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	if (!pmesh->get_property_handle(vprop_uv_coordinate, PROP_VERTEX_UV_COORDINATE))
		pmesh->add_property(vprop_uv_coordinate, PROP_VERTEX_UV_COORDINATE);

	if (!pmesh->get_property_handle(vprop_uknots, PROP_VERTEX_UKNOTS))
		pmesh->add_property(vprop_uknots, PROP_VERTEX_UKNOTS);

	if (!pmesh->get_property_handle(vprop_vknots, PROP_VERTEX_VKNOTS))
		pmesh->add_property(vprop_vknots, PROP_VERTEX_VKNOTS);

	if (!pmesh->get_property_handle(vprop_img_flag, PROP_VERTEX_IMG_FLAG))
		pmesh->add_property(vprop_img_flag, PROP_VERTEX_IMG_FLAG);

	if (!pmesh->get_property_handle(vprop_offending_flag, PROP_VERTEX_OFFENDING_FLAG))
		pmesh->add_property(vprop_offending_flag, PROP_VERTEX_OFFENDING_FLAG);

	if (!pmesh->get_property_handle(vprop_weight, PROP_VERTEX_WEIGHT))
		pmesh->add_property(vprop_weight, PROP_VERTEX_WEIGHT);

	if (!pmesh->get_property_handle(fprop_offending_flag, PROP_FACE_OFFENDING_FLAG))
		pmesh->add_property(fprop_offending_flag, PROP_FACE_OFFENDING_FLAG);

	if (!pmesh->get_property_handle(fprop_uk_bounding_box, PROP_FACE_UV_BOUNDINGBOX))
		pmesh->add_property(fprop_uk_bounding_box, PROP_FACE_UV_BOUNDINGBOX);

	if (!pmesh->get_property_handle(vprop_target_in_current_index, PROP_VERTEX_TARGET_IN_CURRENT_INDEX))
		pmesh->add_property(vprop_target_in_current_index, PROP_VERTEX_TARGET_IN_CURRENT_INDEX);

	if (!pmesh->get_property_handle(vprop_target_in_current_flag, PROP_VERTEX_TARGET_IN_CURRENT_FLAG))
		pmesh->add_property(vprop_target_in_current_flag, PROP_VERTEX_TARGET_IN_CURRENT_FLAG);

	if (!pmesh->get_property_handle(vprop_uv_plotcoordinate, PROP_VERTEX_UV_PLOTCOORDINATE))
		pmesh->add_property(vprop_uv_plotcoordinate, PROP_VERTEX_UV_PLOTCOORDINATE);

	if (!pmesh->get_property_handle(vprop_corner_label, PROP_VERTEX_CORNER_LABEL))
		pmesh->add_property(vprop_corner_label, PROP_VERTEX_CORNER_LABEL);

	//pmesh->mproperty(vprop_uv_coordinate_index).set_persistent(true);
	//pmesh->mproperty(vprop_uknots).set_persistent(true);
	//pmesh->mproperty(vprop_vknots).set_persistent(true);
	//pmesh->mproperty(fprop_uk_bounding_box).set_persistent(true);
	SET_PERS(vprop_uv_coordinate_index);
	SET_PERS(vprop_uv_coordinate);
	SET_PERS(vprop_uknots);
	SET_PERS(vprop_vknots);
	SET_PERS(vprop_img_flag);
	SET_PERS(vprop_offending_flag);
	SET_PERS(vprop_weight);
	SET_PERS(fprop_offending_flag);
	SET_PERS(fprop_uk_bounding_box);
	SET_PERS(vprop_target_in_current_index);
	SET_PERS(vprop_target_in_current_flag);
	SET_PERS(vprop_uv_plotcoordinate);
	SET_PERS(vprop_corner_label);

	//if (!OpenMesh::IO::read_mesh(*pmesh, objname)) 
	//{
	//	return;
	//}

	if (!OpenMesh::IO::read_mesh(*pmesh, omname)) 
	{
		return;
	}
	file >> m_showSurface ;
	file >> m_showControlPointsNetwork ;
	file >> m_showIsoParameterCurves;
	file >> m_showOptimalIsoParameterCurves;

	int m,n;
	file >> m;
	m_targetNurbsUKnots.clear();
	for(int i = 0; i < m; i++)
	{
		double val;
		file >> val;
		m_targetNurbsUKnots.push_back(val);
	}

	file >> n;
	m_targetNurbsVKnots.clear();
	for(int i = 0; i < n; i++)
	{
		double val;
		file >> val;
		m_targetNurbsVKnots.push_back(val);
	}

	file >> udegree >> vdegree;
	file >> umin >> umax >> vmin >> vmax;
	file >> m_Show;

	file >> m_ListOpenGL >> m_ListDone >> m_Modified >> m_ShowTexture;
	m_Transform.Read(file);
	file.close();
	m_Modified = 1;
}