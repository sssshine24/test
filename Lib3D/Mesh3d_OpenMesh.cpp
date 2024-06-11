#include "stdafx.h"
#include "Mesh3d_OpenMesh.h"
#include <math.h>
#include "Base3d.h"
#include "../SDIViewSwitch.h"
#include "Mesh3d.h"
#include "ColorRamp.h"
#include "../FormView3.h"
#include "../MyTreeView.h"
#include <fstream>
#include <atlimage.h>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/CirculatorsT.hh>
#include "Vector3d.h"

CMesh3d_OpenMesh::CMesh3d_OpenMesh(void)
{
	type = TYPE_MESH3D_OPENMESH;
	m_NormalBinding = NORMAL_PER_VERTEX;
	m_ColorBinding = COLOR_PER_VERTEX;
	m_showFace = 1;
	m_showEdge = 0;
	m_showVertex = 0;
	m_IndexTexture = -1;
	m_showVertexMUMou = 0;
	m_showVertexMUAngle = 0;
	m_showTriangleMU = 0;
	m_showSharpEdge = 0;
	m_showLandmarkEdge = 0;
	m_showVertexMUVectorField = 0;
	m_showVertexDisplacement = 0;
	m_showVertexIndex = 0;
	m_showEdgeIndex = 0;
	m_showFaceIndex = 0;
	m_showPointsOnSurface = 0;
	m_Show = 1;
	m_showSelectedVertex = 0;
	m_showSelectedFace = 0;
	m_showSharpEdgeIndex = 0;
	m_showSelectedEdge = 0;
	m_Initialization_Linear_Equaiton = 0;
	m_showSelectedVertexIndex = 0;
	pmesh = new MyMesh();
	pBoundaryMeshReference = NULL;
	m_showFeatureVertexIndex = FALSE;
	m_showFeatureVertex = FALSE;
	m_EdgeColor.Set(0, 0, 0);

	m_SharpeEdgeColor.Set(0.8 * 256, 0, 0.8 * 256);
}
CMesh3d_OpenMesh* CMesh3d_OpenMesh::copy()
{
	CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pNewMesh->type = this->type;
	pNewMesh->m_NormalBinding = this->m_NormalBinding;
	pNewMesh->m_ColorBinding = this->m_ColorBinding;
	pNewMesh->m_showFace = this->m_showFace;
	pNewMesh->m_showEdge = this->m_showEdge;
	pNewMesh->m_showVertex = this->m_showVertex;
	pNewMesh->m_IndexTexture = this->m_IndexTexture;
	pNewMesh->m_showVertexMUMou = this->m_showVertexMUMou;
	pNewMesh->m_showVertexMUAngle = this->m_showVertexMUAngle;
	pNewMesh->m_showTriangleMU = this->m_showTriangleMU;
	pNewMesh->m_LandMarkArray = this->m_LandMarkArray;
	pNewMesh->m_ArrayConstraints = this->m_ArrayConstraints;
	pNewMesh->m_Initialization_Linear_Equaiton = this->m_Initialization_Linear_Equaiton;
	pNewMesh->m_LandMarkArray = this->m_LandMarkArray;
	pNewMesh->m_FixVerticesArray = this->m_FixVerticesArray;
	pNewMesh->m_FixVerticesArray_uv = this->m_FixVerticesArray_uv;
	m_Show = 1;
	delete pNewMesh->pmesh;
	pNewMesh->pmesh = new MyMesh(*pmesh);
	pNewMesh->m_Transform.Copy(&m_Transform);
	return pNewMesh;
}
CMesh3d_OpenMesh::~CMesh3d_OpenMesh(void)
{
	if(pmesh)
		delete pmesh;
	pmesh = NULL;
}
extern CString m_FileTextureName;
extern int m_ShowTexture;
extern char Post_Message[1000];
void CMesh3d_OpenMesh::ComputeBoundingBox(Point3D &lower, Point3D &upper)
{
	OpenMesh::Vec3d pt;
	pt = pmesh->point(pmesh->vertices_begin());
	CVector3d m_lower, m_upper;
	m_lower.Set(pt[0], pt[1], pt[2]);
	m_upper.Set(pt[0], pt[1], pt[2]);
	double m_x, m_y, m_z;

	for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); ++v_it)
	{
		pt = pmesh->point(v_it);

		m_x = pt[0];
		m_y = pt[1];
		m_z = pt[2];

		if (m_x < m_lower.x())
			m_lower.x(m_x);
		if (m_y < m_lower.y())
			m_lower.y(m_y);
		if (m_z < m_lower.z())
			m_lower.z(m_z);

		if (m_x > m_upper.x())
			m_upper.x(m_x);
		if (m_y > m_upper.y())
			m_upper.y(m_y);
		if (m_z > m_upper.z())
			m_upper.z(m_z);
	}

	lower.setValue(m_lower[0], m_lower[1], m_lower[2]);
	upper.setValue(m_upper[0], m_upper[1], m_upper[2]);
}
void CMesh3d_OpenMesh::SaveMFile(char *filename)
{
	std::ofstream file(filename);

	//file << NbVertex() << "  " << NbFace() << "\n";

	//Vertices
	for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); ++v_it)
	{
		int vindex = v_it.handle().idx();
		OpenMesh::Vec3d pt;
		pt = pmesh->point(v_it.handle());
		file << "Vertex " << vindex + 1 <<" " << pt[0] << " " << pt[1] << " " << pt[2] << " \n";
	}

	//Faces
	std::vector<int> vhandles;
	for (MyMesh::FaceIter f_it = pmesh->faces_begin(); f_it != pmesh->faces_end(); ++f_it)
	{
		vhandles.clear();
		for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
		{
			vhandles.push_back(fv_it.handle().idx());
		}
		file << "Face " << f_it.handle().idx() + 1 << " " << vhandles[0] + 1 << " " << vhandles[1] + 1 << " " << vhandles[2] + 1 << " \n";
	}

	file.close();
}
void CMesh3d_OpenMesh::LoadOBJFile(char *filename)
{
	std::ifstream file(filename);

	//
	file.close();
}
void CMesh3d_OpenMesh::SaveOBJFile(char *filename)
{
	std::ofstream file(filename);

	//file << NbVertex() << "  " << NbFace() << "\n";

	//Vertices
	for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); ++v_it)
	{
		int vindex = v_it.handle().idx();
		OpenMesh::Vec3d pt;
		pt = pmesh->point(v_it.handle());
		file << "v " << pt[0] << " " << pt[1] << " " << pt[2] << " \n";
	}

	//Faces
	std::vector<int> vhandles;
	for (MyMesh::FaceIter f_it = pmesh->faces_begin(); f_it != pmesh->faces_end(); ++f_it)
	{
		vhandles.clear();
		for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
		{
			vhandles.push_back(fv_it.handle().idx());
		}
		file << "f " << vhandles[0] + 1 << " " << vhandles[1] + 1 << " " << vhandles[2] + 1 << " \n";
	}

	file.close();
}
void CMesh3d_OpenMesh::SaveOBJParameterDomainFile(char *filename)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::ofstream file(filename);

	//file << NbVertex() << "  " << NbFace() << "\n";

	//Vertices
	for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); ++v_it)
	{
		//int vindex = v_it.handle().idx();
		OpenMesh::Vec3d pt;
		std::vector<double> uv = pmesh->property(vprop_vertex_uv, v_it.handle());
		//int isonboundary = pmesh->is_boundary(v_it.handle());
		pt[0] = uv[0];
		pt[1] = uv[1];
		pt[2] = 0;

		file << "v " << pt[0] << " " << pt[1] << " " << pt[2] << " \n";
	}

	//Faces
	std::vector<int> vhandles;
	for (MyMesh::FaceIter f_it = pmesh->faces_begin(); f_it != pmesh->faces_end(); ++f_it)
	{
		vhandles.clear();
		for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
		{
			vhandles.push_back(fv_it.handle().idx());
		}
		file << "f " << vhandles[0] + 1 << " " << vhandles[1] + 1 << " " << vhandles[2] + 1 << " \n";
	}

	file.close();
}
int CMesh3d_OpenMesh::ShowTriangleMU()
{
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);

	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	::glBegin(GL_TRIANGLES);
	FPROP_FACE_MU fprop_facemu;

	CImage m_HotTextureImage; 
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	//Vertex selected flag;
	if (!pmesh->get_property_handle(fprop_facemu, PROP_FACE_MU))
		pmesh->add_property(fprop_facemu, PROP_FACE_MU);

	double r,g,b;
	//plot selected vertices
	double maxmumou = -1e10;
	double minmumou = 1e10;
	int m_ArrayNumber[11];
	for(int i = 0; i <= 10; i++)
		m_ArrayNumber[i] = 0;

	std::vector<double> muvec;
	for(MyMesh::FaceIter f_it=pmesh->faces_begin(); f_it!=pmesh->faces_end(); ++f_it)
	{
		muvec = pmesh->property(fprop_facemu,f_it.handle());

		if(muvec.size() > 0)
		{
			double mumou = sqrt(SQR(muvec[0]) + SQR(muvec[1]));

			if(mumou <= 0.1)
				m_ArrayNumber[0]++;
			else if(mumou <= 0.2)
				m_ArrayNumber[1]++;
			else if(mumou <= 0.3)
				m_ArrayNumber[2]++;
			else if(mumou <= 0.4)
				m_ArrayNumber[3]++;
			else if(mumou <= 0.5)
				m_ArrayNumber[4]++;
			else if(mumou <= 0.6)
				m_ArrayNumber[5]++;
			else if(mumou <= 0.7)
				m_ArrayNumber[6]++;
			else if(mumou <= 0.8)
				m_ArrayNumber[7]++;
			else if(mumou <= 0.9)
				m_ArrayNumber[8]++;
			else if(mumou <= 1)
				m_ArrayNumber[9]++;
			else
				m_ArrayNumber[10]++;

			if(mumou > maxmumou)
				maxmumou = mumou;
			if(mumou < minmumou)
				minmumou = mumou;

			int x = mumou * m_HotTextureImage.GetWidth();
			if ( x >= m_HotTextureImage.GetWidth())
				x = m_HotTextureImage.GetWidth() - 1;
			COLORREF color = m_HotTextureImage.GetPixel(x,0);
			r = GetRValue(color) / 256.0;
			g = GetGValue(color) / 256.0;
			b = GetBValue(color) / 256.0;
		}else
			r = g = b = 0;
			::glColor3f(r,g,b);

		for(MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle());fv_it;fv_it++)
		{
			OpenMesh::Vec3d pt;
			pt = pmesh->point (fv_it.handle());
			::glVertex3f(pt[0],pt[1],pt[2]);
		}	
	}
	::glEnd();

	::glDepthFunc(GL_LESS);
	sprintf(Post_Message,"Maximal mu %f, minimal mu %f, [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]",maxmumou,minmumou,m_ArrayNumber[0],m_ArrayNumber[1],m_ArrayNumber[2],m_ArrayNumber[3],m_ArrayNumber[4],m_ArrayNumber[5],m_ArrayNumber[6],m_ArrayNumber[7],m_ArrayNumber[8],m_ArrayNumber[9],m_ArrayNumber[10]);
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	return 1;
}
int CMesh3d_OpenMesh::ShowVertexDisplacement()
{
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);

	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	//::glBegin(GL_POINTS);
	::glBegin(GL_LINES);
	VPROP_VERTEX_DISPLACEMENT vprop_vertexdisplacement;

	CImage m_HotTextureImage; 
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	//Vertex selected flag;
	if (!pmesh->get_property_handle(vprop_vertexdisplacement, PROP_VERTEX_DISPLACEMENT))
		pmesh->add_property(vprop_vertexdisplacement, PROP_VERTEX_DISPLACEMENT);

	double r,g,b;
	//plot selected vertices

	std::vector<double> disvec;

	for(MyMesh::VertexIter v_it = pmesh->vertices_begin();v_it != pmesh->vertices_end(); v_it++)
	{
		disvec = pmesh->property(vprop_vertexdisplacement,v_it.handle());
		if(disvec.size() < 2)
			continue;
		double dismou = sqrt(SQR(disvec[0]) + SQR(disvec[1]));
		int x = dismou * m_HotTextureImage.GetWidth();
		if ( x >= m_HotTextureImage.GetWidth())
			x = m_HotTextureImage.GetWidth() - 1;
		COLORREF color = m_HotTextureImage.GetPixel(x,0);
		r = GetRValue(color) / 256.0;
		g = GetGValue(color) / 256.0;
		b = GetBValue(color) / 256.0;
		::glColor3f(r,g,b);
		OpenMesh::Vec3d pt;
		pt = pmesh->point (v_it.handle());
		::glVertex3f(pt[0],pt[1],pt[2]);
		::glVertex3f(pt[0] + disvec[0],pt[1] + disvec[1],pt[2]);
	}
	::glEnd();

	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d_OpenMesh::ShowVertexMUVectorField()
{
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);

	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	//::glBegin(GL_POINTS);
	::glBegin(GL_LINES);
	VPROP_VERTEX_MU vprop_vertexmu;

	CImage m_HotTextureImage; 
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	//Vertex selected flag;
	if (!pmesh->get_property_handle(vprop_vertexmu, PROP_VERTEX_MU))
		pmesh->add_property(vprop_vertexmu, PROP_VERTEX_MU);

	double r,g,b;
	//plot selected vertices

	std::vector<double> muvec;

	for(MyMesh::VertexIter v_it = pmesh->vertices_begin();v_it != pmesh->vertices_end(); v_it++)
	{
		muvec = pmesh->property(vprop_vertexmu,v_it.handle());
		double mumou = sqrt(SQR(muvec[0]) + SQR(muvec[1]));
		int x = mumou * m_HotTextureImage.GetWidth();
		if ( x >= m_HotTextureImage.GetWidth())
			x = m_HotTextureImage.GetWidth() - 1;
		COLORREF color = m_HotTextureImage.GetPixel(x,0);
		r = GetRValue(color) / 256.0;
		g = GetGValue(color) / 256.0;
		b = GetBValue(color) / 256.0;
		::glColor3f(r,g,b);
		OpenMesh::Vec3d pt;
		pt = pmesh->point (v_it.handle());
		::glVertex3f(pt[0],pt[1],pt[2]);
		::glVertex3f(pt[0] + muvec[0] * 0.01,pt[1] + muvec[1] * 0.01,pt[2]);
	}
	::glEnd();

	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d_OpenMesh::ShowVertexMUMou()
{
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);
	
	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	//::glBegin(GL_POINTS);
	::glBegin(GL_TRIANGLES);
	VPROP_VERTEX_MU vprop_vertexmu;

	CImage m_HotTextureImage; 
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	//Vertex selected flag;
	if (!pmesh->get_property_handle(vprop_vertexmu, PROP_VERTEX_MU))
		pmesh->add_property(vprop_vertexmu, PROP_VERTEX_MU);

	double r,g,b;
	//plot selected vertices
	double maxmumou = -1e10;
	double minmumou = 1e10;
	int m_ArrayNumber[11];
	for(int i = 0; i <= 10; i++)
		m_ArrayNumber[i] = 0;

	std::vector<double> muvec;
	for(MyMesh::FaceIter f_it=pmesh->faces_begin(); f_it!=pmesh->faces_end(); ++f_it)
	{
		for(MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle());fv_it;fv_it++)
		{
			muvec = pmesh->property(vprop_vertexmu,fv_it.handle());
			double mumou = sqrt(SQR(muvec[0]) + SQR(muvec[1]));
			//int x = mumou * m_HotTextureImage.GetWidth();
			//if ( x >= m_HotTextureImage.GetWidth())
			//	x = m_HotTextureImage.GetWidth() - 1;
			//COLORREF color = m_HotTextureImage.GetPixel(x,0);
			//int x = 256 * mumou;
			//r = GetRValue(color) / 256.0;
			//g = GetGValue(color) / 256.0;
			//b = GetBValue(color) / 256.0;
			r = mumou;
			g = b = 0;
			::glColor3f(r,g,b);
			OpenMesh::Vec3d pt;
			pt = pmesh->point (fv_it.handle());
			::glVertex3f(pt[0],pt[1],pt[2]);
		}	
	}
	::glEnd();

	::glDepthFunc(GL_LESS);
	return 1;
}
typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
	hsv         out;
	double      min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	out.v = max;                                // v
	delta = max - min;
	if (delta < 0.00001)
	{
		out.s = 0;
		out.h = 0; // undefined, maybe nan?
		return out;
	}
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0              
		// s = 0, h is undefined
		out.s = 0.0;
		out.h = NAN;                            // its now undefined
		return out;
	}
	if (in.r >= max)                           // > is bogus, just keeps compilor happy
		out.h = (in.g - in.b) / delta;        // between yellow & magenta
	else
		if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
		else
			out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

	out.h *= 60.0;                              // degrees

	if (out.h < 0.0)
		out.h += 360.0;

	return out;
}


rgb hsv2rgb(hsv in)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}

int CMesh3d_OpenMesh::ShowVertexMU()
{
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);

	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	//::glBegin(GL_POINTS);
	::glBegin(GL_TRIANGLES);
	VPROP_VERTEX_MU vprop_vertexmu;

	CImage m_HotTextureImage;
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	//Vertex selected flag;
	if (!pmesh->get_property_handle(vprop_vertexmu, PROP_VERTEX_MU))
		pmesh->add_property(vprop_vertexmu, PROP_VERTEX_MU);

	double r, g, b;
	//plot selected vertices
	double maxmumou = -1e10;
	double minmumou = 1e10;
	int m_ArrayNumber[11];
	for (int i = 0; i <= 10; i++)
		m_ArrayNumber[i] = 0;

	std::vector<double> muvec;
	for (MyMesh::FaceIter f_it = pmesh->faces_begin(); f_it != pmesh->faces_end(); ++f_it)
	{
		for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
		{
			muvec = pmesh->property(vprop_vertexmu, fv_it.handle());
			double angle = atan2(muvec[1], muvec[0]);
			double mumou = sqrt(SQR(muvec[0]) + SQR(muvec[1]));

			hsv hsv1;
			hsv1.h = angle * 360 / (PI * 2);
			hsv1.s = 1;
			hsv1.v = 1 - mumou;

			rgb rgb1 = hsv2rgb(hsv1);
			//int x = mumou * m_HotTextureImage.GetWidth();
			//if (x >= m_HotTextureImage.GetWidth())
			//	x = m_HotTextureImage.GetWidth() - 1;
			//COLORREF color = m_HotTextureImage.GetPixel(x, 0);
			//r = mumou;
			//g = (angle + PI) / (PI * 2);
			//b = 0;
			r = rgb1.r;
			g = rgb1.g;
			b = rgb1.b;

			::glColor3f(r, g, b);
			OpenMesh::Vec3d pt;
			pt = pmesh->point(fv_it.handle());
			::glVertex3f(pt[0], pt[1], pt[2]);
		}
	}
	::glEnd();

	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d_OpenMesh::ShowVertexMUAngle()
{
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);

	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	//::glBegin(GL_POINTS);
	::glBegin(GL_TRIANGLES);
	VPROP_VERTEX_MU vprop_vertexmu;

	CImage m_HotTextureImage;
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	//Vertex selected flag;
	if (!pmesh->get_property_handle(vprop_vertexmu, PROP_VERTEX_MU))
		pmesh->add_property(vprop_vertexmu, PROP_VERTEX_MU);

	double r, g, b;
	//plot selected vertices
	double maxmumou = -1e10;
	double minmumou = 1e10;
	int m_ArrayNumber[11];
	for (int i = 0; i <= 10; i++)
		m_ArrayNumber[i] = 0;

	std::vector<double> muvec;
	for (MyMesh::FaceIter f_it = pmesh->faces_begin(); f_it != pmesh->faces_end(); ++f_it)
	{
		for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
		{
			muvec = pmesh->property(vprop_vertexmu, fv_it.handle());
			double angle = atan2(muvec[1], muvec[0]);
			//double mumou = sqrt(SQR(muvec[0]) + SQR(muvec[1]));
			//int x = mumou * m_HotTextureImage.GetWidth();
			//if (x >= m_HotTextureImage.GetWidth())
			//	x = m_HotTextureImage.GetWidth() - 1;
			//COLORREF color = m_HotTextureImage.GetPixel(x, 0);
			r = 0;
			g = (angle + PI) / (PI * 2);
			b = 0;
			::glColor3f(r, g, b);
			OpenMesh::Vec3d pt;
			pt = pmesh->point(fv_it.handle());
			::glVertex3f(pt[0], pt[1], pt[2]);
		}
	}
	::glEnd();

	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d_OpenMesh::ShowSelectedFace()
{
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);

	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	//::glBegin(GL_POINTS);
	::glBegin(GL_TRIANGLES);
	FPROP_FACE_SELECTED_FLAG fprop_faceselectedflag;

	//Face selected flag;
	if (!pmesh->get_property_handle(fprop_faceselectedflag, PROP_FACE_SELECTED))
		pmesh->add_property(fprop_faceselectedflag, PROP_FACE_SELECTED);

	double r,g,b;
	//plot selected vertices
	double maxmumou = -1e10;
	double minmumou = 1e10;

	std::vector<double> muvec;
	::glColor3f(1,0,0);
	for(MyMesh::FaceIter f_it=pmesh->faces_begin(); f_it!=pmesh->faces_end(); ++f_it)
	{
		int flag = pmesh->property(fprop_faceselectedflag,f_it.handle());
		if(!flag)
			continue;
		for(MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle());fv_it;fv_it++)
		{
			OpenMesh::Vec3d pt;
			pt = pmesh->point (fv_it.handle());
			::glVertex3f(pt[0],pt[1],pt[2]);
		}	
	}
	::glEnd();

	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d_OpenMesh::ShowSelectedVertex()
{
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(1.0,0.0,0.0);
	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	::glBegin(GL_POINTS);
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;

	//Vertex selected flag;
	if (!pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//plot selected vertices
	for(MyMesh::VertexIter v_it=pmesh->vertices_begin(); v_it!=pmesh->vertices_end(); ++v_it)
	{
		if(pmesh->property(vprop_selectedflag,v_it.handle()))
		{
			OpenMesh::Vec3d pt;
			pt = pmesh->point (v_it);
			::glVertex3f(pt[0],pt[1],pt[2]);
		}	
	}
	
	::glEnd();

	::glDepthFunc(GL_LESS);
	return 1;
}
CMesh3d* CMesh3d_OpenMesh::ConvertMesh3d()
{
	CMesh3d* pMesh3d = new CMesh3d();
	CVertex3d* pVertex;
	MyMesh::Color color;
	for(MyMesh::VertexIter v_it=pmesh->vertices_begin(); v_it!=pmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d pt = pmesh->point(v_it.handle());
		color = pmesh->color(v_it.handle());
		pVertex = new CVertex3d(pt[0], pt[1], pt[2]);
		pVertex->SetColor(color[0], color[1], color[2]);
		pMesh3d->m_ArrayVertex.Add(pVertex);
	}
	std::vector<int> vindexarray;
	for(MyMesh::FaceIter f_it=pmesh->faces_begin(); f_it!=pmesh->faces_end(); ++f_it)
	{
		vindexarray.clear();	
		for(MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle());fv_it;fv_it++)
		{
			vindexarray.push_back(fv_it.handle().idx());
		}
		pMesh3d->m_ArrayFace.Add(new CFace3d(pMesh3d->GetVertex(vindexarray[0]),pMesh3d->GetVertex(vindexarray[1]),pMesh3d->GetVertex(vindexarray[2])));
	}
	pMesh3d->m_FixVerticesArray_uv = m_FixVerticesArray_uv;
	pMesh3d->m_LandMarkArray = m_LandMarkArray;
	pMesh3d->m_FixVerticesArray = m_FixVerticesArray;
	pMesh3d->m_FixVerticesArray_Inner = m_FixVerticesArray_Inner;
	pMesh3d->m_FixVerticesArray_Inner_uv = m_FixVerticesArray_Inner_uv;
	pMesh3d->m_Transform.Copy(&m_Transform);
	return pMesh3d;
}
int CMesh3d_OpenMesh::glDraw()
{
	if(!m_Show)
		return 0;

	//construct texture
	GLubyte* pTextureBits;
	GLint width,height;
	CDib*     m_pDib;
	if(m_ShowTexture)
	{
		m_pDib=new CDib(m_FileTextureName);
		pTextureBits=(GLubyte*)m_pDib->GetDibBitsPtr();
		width=m_pDib->GetDibWidth();
		height=m_pDib->GetDibHeight();
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		glTexImage2D(GL_TEXTURE_2D,0,3,width,height,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pTextureBits);
		//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	}

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	::glPushMatrix();

	// Position / translation / scaling
	if(!m_SetCameraMode)
	{
		glScalef(m_Transform.GetScale()->x(),
			m_Transform.GetScale()->y(),
			m_Transform.GetScale()->z());

		glTranslatef(m_Transform.GetTranslation()->x(),
			m_Transform.GetTranslation()->y(),
			m_Transform.GetTranslation()->z());

		glRotatef(m_Transform.GetValueRotation(),
			m_Transform.GetRotation()->x(),
			m_Transform.GetRotation()->y(),
			m_Transform.GetRotation()->z());
	}

	//store the model view & projection view.
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);    
	glGetDoublev(GL_PROJECTION_MATRIX, projection);  
	glGetIntegerv(GL_VIEWPORT,m_viewPort);

	// Build list at first
	if(!m_ListDone || m_Modified)
		glBuildList();

	// Search for a new list
	if(::glIsList(m_ListOpenGL)==GL_TRUE)
	{
		if(m_showFace)
		{
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			if(pDoc->m_Light)
				::glEnable(GL_LIGHTING);
			else
				::glDisable(GL_LIGHTING);

			::glCallList(m_ListOpenGL);
		}

		if(m_showEdge)
		{
			::glDisable(GL_LIGHTING);
			::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			::glEnable(GL_POLYGON_OFFSET_LINE);
			::glPolygonOffset(pDoc->m_PolygonOffset,-0.01f);

			//::glCallList(m_ListOpenGL);
			ShowEdge();
		}

		if(m_showVertex)
		{
			::glDisable(GL_LIGHTING);
			::glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			::glPointSize(5.0);
			::glCallList(m_ListOpenGL);
		}

		if(m_showSelectedVertex)
			ShowSelectedVertex();

		if(m_showSelectedFace)
			ShowSelectedFace();

		//
		if(m_showTriangleMU)
			ShowTriangleMU();
		
		//
		if(m_showVertexMUMou)
			ShowVertexMUMou();

		if (m_showVertexMUAngle)
			ShowVertexMUAngle();

		if(m_showSharpEdge)
			ShowSharpEdges();

		if(m_showLandmarkEdge)
			ShowLandmarkEdge();

		if (m_showSelectedEdge)
			ShowSelectedEdge();

		if(m_showVertexMUVectorField)
			ShowVertexMUVectorField();

		if(m_showVertexDisplacement)
			ShowVertexDisplacement();

		if (m_showEdgeIndex)
			ShowEdgeIndex();

		if(m_showVertexIndex)
			ShowVertexIndex();

		if (m_showFaceIndex)
			ShowFaceIndex();
		//return 1;
	}
	else
	{
		TRACE(" CMesh3d::Draw : unable to draw list %d\n",m_ListOpenGL);
		//return 0;
	}

	::glPopMatrix();
}
int CMesh3d_OpenMesh::ShowFaceIndex()
{
	int num = 0;
	for (MyMesh::FaceIter f_it = pmesh->faces_begin(); f_it != pmesh->faces_end(); f_it++)
	{
		double x, y, z;
		x = y = z = 0;
		for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			num++;
			OpenMesh::Vec3d vec = pmesh->point(fv_it.handle());
			x += vec[0];
			y += vec[1];
			z += vec[2];
		}
		
		x = x / num;
		y = y / num;
		z = z / num;
		char message[100];
		sprintf(message, "%d", f_it.handle().idx());
		p_FormView3->drawOpenglString(x, y, z, message);
	}
	return 1;
}
int CMesh3d_OpenMesh::ShowEdgeIndex()
{
	for (MyMesh::EdgeIter e_it = pmesh->edges_begin(); e_it != pmesh->edges_end(); e_it++)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		//

		MyMesh::HalfedgeHandle hh0 = pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pmesh->from_vertex_handle(hh0);

		double x, y, z;
		OpenMesh::Vec3d vec1 = pmesh->point(vh1);
		OpenMesh::Vec3d vec2 = pmesh->point(vh2);

		x = (vec1[0] + vec2[0]) / 2;
		y = (vec1[1] + vec2[1]) / 2;
		z = (vec1[2] + vec2[2]) / 2;

		char message[100];
		sprintf(message, "%d", eh.idx());
		p_FormView3->drawOpenglString( x, y, z, message);
	}
	return 1;
}
int CMesh3d_OpenMesh::ShowVertexIndex()
{
	for(MyMesh::VertexIter v_it = pmesh->vertices_begin();v_it != pmesh->vertices_end(); v_it++)
	{
		double x,y,z;
		OpenMesh::Vec3d vec = pmesh->point(v_it.handle());
		char message[100];
		sprintf(message,"%d",v_it.handle().idx());
		p_FormView3->drawOpenglString(vec[0],vec[1],vec[2],message);
	}
	return 1;
}
int CMesh3d_OpenMesh::ShowLandmarkEdge()
{
	::glEnable(GL_LINE_SMOOTH);
	::glDisable(GL_LIGHTING);
	//::glDepthFunc(GL_ALWAYS);
	::glDepthFunc(GL_LESS);
	::glPointSize(5.0);

	std::vector<int>::iterator literator;
	for(int i = 0; i < m_LandMarkArray.size(); i++)
	{
		if(!m_LandMarkArray[i]->isVisible)
			continue; 

		//Make the seleted Landmarks thicker
		if(m_LandMarkArray[i]->isAuxillary)
		{
			::glLineWidth(4.0f);
		}else
		{
			::glLineWidth(2.0f);					
		}
		int style = m_LandMarkArray[i]->style;
		if(style)
			//if(m_LandMarkArray[i]->isAuxillary)
			//	::glColor3f(0.9,1.0,0.9);
			//else
			::glColor3f(0.0,1.0,0.0);
		else
			//if(m_LandMarkArray[i]->isAuxillary)
			//	::glColor3f(1.0,0.9,0.9);
			//else
			::glColor3f(1.0,0.0,0.0);

		::glLineWidth(2.0f);

		::glBegin(GL_LINES);
		for(int j = 0; j < m_LandMarkArray[i]->landmark_points.size() - 1; j++)
		{
			int index1 = m_LandMarkArray[i]->landmark_points[j];
			int index2 = m_LandMarkArray[i]->landmark_points[j + 1];

			OpenMesh::Vec3d v1 = pmesh->point(pmesh->vertex_handle(index1));
			OpenMesh::Vec3d v2 = pmesh->point(pmesh->vertex_handle(index2));
			::glVertex3f(v1[0],v1[1],v1[2]);
			::glVertex3f(v2[0],v2[1],v2[2]);
		}
		::glEnd();
	}

	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d_OpenMesh::ShowEdge()
{
	//::glDepthFunc(GL_ALWAYS);
	::glDepthFunc(GL_LESS);
	::glColor3f(m_EdgeColor.r() / 256.0,m_EdgeColor.g() / 256.0,m_EdgeColor.b() / 256.0);
	//::glColor3f(0.0,0.0,128.0 / 256);
	::glDisable(GL_LIGHTING);
	::glLineWidth(1.0);
	::glBegin(GL_LINES);
	for(MyMesh::EdgeIter e_it = pmesh->edges_begin();e_it != pmesh->edges_end(); e_it++)
	{
		MyMesh::HalfedgeHandle hh = pmesh->halfedge_handle(e_it.handle(), 0);
		MyMesh::VertexHandle th = pmesh->to_vertex_handle(hh);
		MyMesh::VertexHandle fh = pmesh->from_vertex_handle(hh);
		int thindex = th.idx();
		int fhindex = fh.idx();
		OpenMesh::Vec3d v1 = pmesh->point(th);
		OpenMesh::Vec3d v2 = pmesh->point(fh);
		::glVertex3f(v1[0],v1[1],v1[2]);
		::glVertex3f(v2[0],v2[1],v2[2]);
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d_OpenMesh::ShowSelectedEdge()
{
	EPROP_EDGE_SELECTED_FLAG eprop_selectedflag;

	//Edge selected flag;
	if (!pmesh->get_property_handle(eprop_selectedflag, PROP_EDGE_SELECTED))
		pmesh->add_property(eprop_selectedflag, PROP_EDGE_SELECTED);

	//::glDepthFunc(GL_ALWAYS);
	::glDepthFunc(GL_LESS);
	::glColor3f(m_SharpeEdgeColor.r() / 256.0, m_SharpeEdgeColor.g() / 256.0, m_SharpeEdgeColor.b() / 256.0);
	//::glColor3f(0.0,0.0,128.0 / 256);
	::glDisable(GL_LIGHTING);
	::glLineWidth(4.0);
	::glBegin(GL_LINES);
	for (MyMesh::EdgeIter e_it = pmesh->edges_begin(); e_it != pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		if (pmesh->property(eprop_selectedflag, e_it.handle()) == 1)
		{
			MyMesh::HalfedgeHandle hh0 = pmesh->halfedge_handle(eh, 0);

			MyMesh::VertexHandle vh1 = pmesh->to_vertex_handle(hh0);
			MyMesh::VertexHandle vh2 = pmesh->from_vertex_handle(hh0);

			OpenMesh::Vec3d v1 = pmesh->point(vh1);
			OpenMesh::Vec3d v2 = pmesh->point(vh2);

			::glVertex3f(v1[0], v1[1], v1[2]);
			::glVertex3f(v2[0], v2[1], v2[2]);
		}
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
void CMesh3d_OpenMesh::CopyLaplaceOperator(CMesh3d* pMesh)
{
	VPROP_VERTEX_DISPLACEMENT vprop_vertex_inter_info;

	//Vertex selected flag
	if (!pmesh->get_property_handle(vprop_vertex_inter_info, PROP_VERTEX_DISPLACEMENT))
		pmesh->add_property(vprop_vertex_inter_info, PROP_VERTEX_DISPLACEMENT);

	std::vector<double> temp;
	for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); v_it++)
	{
		temp = pmesh->property(vprop_vertex_inter_info, v_it.handle());

		pMesh->GetVertex(v_it.handle().idx())->m_LaplaceOperatorVec[0] = temp[0];
		pMesh->GetVertex(v_it.handle().idx())->m_LaplaceOperatorVec[1] = temp[1];
		pMesh->GetVertex(v_it.handle().idx())->m_LaplaceOperatorVec[2] = temp[2];
	}

}
void CMesh3d_OpenMesh::CalculateLaplaceBeltramiOperator()
{
	//COpenMesh
	VPROP_VERTEX_DISPLACEMENT vprop_vertex_inter_info;

	//Vertex selected flag
	if (!pmesh->get_property_handle(vprop_vertex_inter_info, PROP_VERTEX_DISPLACEMENT))
		pmesh->add_property(vprop_vertex_inter_info, PROP_VERTEX_DISPLACEMENT);

	std::vector<MyMesh::VertexHandle> m_ArrayOneRingVertices;
	OpenMesh::Vec3d v0, v_i, v_iminus1, v_iadd1;
	std::vector<double> temp;
	for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); v_it++)
	{
		temp.clear();
		temp.push_back(0);
		temp.push_back(0);
		temp.push_back(0);
		m_ArrayOneRingVertices.clear();
		v0 = pmesh->point(v_it.handle());
		//search the 1-ring vertices
		for (MyMesh::VertexVertexIter vv_it = pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
		{
			m_ArrayOneRingVertices.push_back(vv_it.handle());
		}
		double v0_v_iminus1;
		double v0_v_i;
		double v0_v_iadd1;
		double vi_v_iadd1;
		double vi_v_iminus1;
		for (int i = 0; i < m_ArrayOneRingVertices.size(); i++)
		{
			//
			v_i = pmesh->point(m_ArrayOneRingVertices[i]);
			v_iminus1 = pmesh->point(m_ArrayOneRingVertices[(i - 1 + m_ArrayOneRingVertices.size()) % m_ArrayOneRingVertices.size()]);
			v_iadd1 = pmesh->point(m_ArrayOneRingVertices[(i + 1) % m_ArrayOneRingVertices.size()]);

			v0_v_iminus1 = Length(v0 - v_iminus1);
			v0_v_i = Length(v0 - v_i);
			v0_v_iadd1 = Length(v0 - v_iadd1);
			vi_v_iadd1 = Length(v_i - v_iadd1);
			vi_v_iminus1 = Length(v_i - v_iminus1);
			//double vi_v0 = Length(v_i - v0);

			double beta = acos((SQR(v0_v_iminus1) + SQR(vi_v_iminus1) - SQR(v0_v_i)) / (2 * v0_v_iminus1 * vi_v_iminus1));
			double alpha = acos((SQR(v0_v_iadd1) + SQR(vi_v_iadd1) - SQR(v0_v_i)) / (2 * v0_v_iadd1 * vi_v_iadd1));
			double weight = 0;
			if (fabs(beta) > 1e-2 && fabs(alpha) > 1e-2)
				weight = (cos(alpha) / sin(alpha) + cos(beta) / sin(beta));

			temp[0] += weight * (v_i[0] - v0[0]);
			temp[1] += weight * (v_i[1] - v0[1]);
			temp[2] += weight * (v_i[2] - v0[2]);
		}

		Vector3D v1, v2, v3, v1v2_middlepoint, v1v3_middlepoint;
		Vector3D barycentricpoint;
		double Ai = 0;
		for (int i = 0; i < m_ArrayOneRingVertices.size(); i++)
		{
			v_i = pmesh->point(m_ArrayOneRingVertices[i]);
			v_iminus1 = pmesh->point(m_ArrayOneRingVertices[(i - 1 + m_ArrayOneRingVertices.size()) % m_ArrayOneRingVertices.size()]);

			v1.SetValue(v0[0], v0[1], v0[2]);
			v2.SetValue(v_i[0], v_i[1], v_i[2]);
			v3.SetValue(v_iminus1[0], v_iminus1[1], v_iminus1[2]);

			v1v2_middlepoint = (v1 + v2) / 2;
			v1v3_middlepoint = (v1 + v3) / 2;
			barycentricpoint = (v1 + v2 + v3) / 3;

			Ai += CMesh3d::TriangleArea(v1, v1v2_middlepoint, barycentricpoint);
			Ai += CMesh3d::TriangleArea(v1, v1v3_middlepoint, barycentricpoint);
		}
		if (fabs(Ai) > 0.01)
		{
			temp[0] = temp[0] / (Ai * 2);
			temp[1] = temp[1] / (Ai * 2);
			temp[2] = temp[2] / (Ai * 2);
		}

		pmesh->property(vprop_vertex_inter_info, v_it.handle()) = temp;
	}
}
void CMesh3d_OpenMesh::CopyGaussCurvature(CMesh3d* pMesh)
{
	VPROP_VERTEX_DISPLACEMENT vprop_vertex_inter_info;

	//Vertex selected flag
	if (!pmesh->get_property_handle(vprop_vertex_inter_info, PROP_VERTEX_DISPLACEMENT))
		pmesh->add_property(vprop_vertex_inter_info, PROP_VERTEX_DISPLACEMENT);
	std::vector<double> temp;
	for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); v_it++)
	{
		temp = pmesh->property(vprop_vertex_inter_info, v_it.handle());

		pMesh->GetVertex(v_it.handle().idx())->m_GaussCurvature = temp[0];
	}
}
void CMesh3d_OpenMesh::CalculateGaussCurvature()
{
	//COpenMesh
	VPROP_VERTEX_DISPLACEMENT vprop_vertex_inter_info;


	if (!pmesh->get_property_handle(vprop_vertex_inter_info, PROP_VERTEX_DISPLACEMENT))
		pmesh->add_property(vprop_vertex_inter_info, PROP_VERTEX_DISPLACEMENT);

	std::vector<MyMesh::VertexHandle> m_ArrayOneRingVertices;
	double alpha = 0.0;
	OpenMesh::Vec3d v0, v_i, v_next;
	double l1, l2, l3;
	std::vector<double> temp;
	for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); v_it++)
	{
		int vid = v_it.handle().idx();
		temp.clear();
		m_ArrayOneRingVertices.clear();
		alpha = 0.0;
		v0 = pmesh->point(v_it.handle());   //current point
		for (MyMesh::VertexVertexIter vv_it = pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
		{
			m_ArrayOneRingVertices.push_back(vv_it.handle());
		}
		for (int i = 0; i < m_ArrayOneRingVertices.size(); i++)
		{
			//
			v_i = pmesh->point(m_ArrayOneRingVertices[i]);   //test point
			v_next = pmesh->point(m_ArrayOneRingVertices[(i + 1 + m_ArrayOneRingVertices.size()) % m_ArrayOneRingVertices.size()]);
			l1 = Length(v_i - v0);
			l2 = Length(v_next - v0);
			l3 = Length(v_next - v_i);
			double tn = acos((SQR(l1) + SQR(l2) - SQR(l3)) / (2 * l1 * l2));
			alpha += tn;
			//int id = m_ArrayOneRingVertices[i].idx();
		}

		temp.push_back((PI * 2 - alpha));
		pmesh->property(vprop_vertex_inter_info, v_it.handle()) = temp;
	}
}

int CMesh3d_OpenMesh::AssignUVtoMeshPointLocation()
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uv;
	OpenMesh::Vec3d pt;
	for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); ++v_it)
	{
		uv = pmesh->property(vprop_vertex_uv, v_it.handle());

		pt[0] = uv[0];
		pt[1] = uv[1];
		pt[2] = 0;

		pmesh->set_point(v_it, pt);
	}
	return 1;
}
int CMesh3d_OpenMesh::ShowSharpEdges()
{
	//::glDepthFunc(GL_ALWAYS);
	::glDepthFunc(GL_LESS);
	::glColor3f(m_SharpeEdgeColor.r() / 256.0,m_SharpeEdgeColor.g() / 256.0,m_SharpeEdgeColor.b() / 256.0);
	//::glColor3f(0.0,0.0,128.0 / 256);
	::glDisable(GL_LIGHTING);
	::glLineWidth(10.0);
	::glBegin(GL_LINES);
	unsigned int NbEdge = (unsigned int)m_ArraySharpEdges.size();
	for(int i = 0; i < NbEdge; i++)
	{
		std::pair<int,int> data;
		data = m_ArraySharpEdges[i];
		OpenMesh::Vec3d v1 = pmesh->point(pmesh->vertex_handle(data.first));
		OpenMesh::Vec3d v2 = pmesh->point(pmesh->vertex_handle(data.second));
		::glVertex3f(v1[0],v1[1],v1[2]);
		::glVertex3f(v2[0],v2[1],v2[2]);
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d_OpenMesh::glBuildList()
{
	//TRACE(" Start building list ...\n");
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	//Vertex selected flag;
	if (!pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

  if(!m_Modified && m_ListDone)
		return 0;

	// Erase last list
	::glDeleteLists(m_ListOpenGL,1);

	// Search for a new list
	m_ListOpenGL = ::glGenLists(1);
	if(m_ListOpenGL == 0)
	{
		TRACE("CMesh3d::BuildList : unable to build DrawList\n");
		return 0;
	}
	glFrontFace(GL_CW);
	//
	glEnable(GL_POINT_SMOOTH); // 启用反走样
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND); // 启动融合
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 产生融合因子
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);// 权衡图像质量与绘制速度 
	// Start list
	unsigned int NbVertex = pmesh->n_vertices();
	unsigned int NbFace = pmesh->n_faces();

	if(!NbVertex)
		return 0;
	if(!NbFace)
		return 0;

	CFace3d *pFace;
	CVertex3d* pVertex;
	CVector3d *pVector;
	CColor *pColorPrevious;

	::glNewList(m_ListOpenGL,GL_COMPILE);

	// Init color
	double r,g,b;

	int i = 0;
	// Triangles
	
	for(MyMesh::FaceIter f_it=pmesh->faces_begin(); f_it!=pmesh->faces_end(); ++f_it)
	{
		::glBegin(GL_POLYGON);
		glLoadName(i);
		MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it);  
		if(m_NormalBinding == NORMAL_PER_FACE)
		{
			OpenMesh::Vec3d facenormalvec;
			facenormalvec = pmesh->normal(f_it);
			::glNormal3f(facenormalvec[0],facenormalvec[1],facenormalvec[2]);
		}
		if(m_ColorBinding == COLOR_PER_FACE)
		{
			OpenMesh::Vec3d color;
			color = pmesh->color(f_it);
			::glColor3ub(color[0],color[1],color[2]);
		}
		for(; fv_it; ++fv_it) 
		{  
			OpenMesh::Vec3d pt;
			pt = pmesh->point (fv_it);

			// Normal
			if(m_NormalBinding == NORMAL_PER_VERTEX)
			{
				OpenMesh::Vec3d pn;
				pn = pmesh->normal(fv_it);
				::glNormal3f(pn[0],pn[1],pn[2]);
			}

			// Color (per vertex)
			if(m_ColorBinding == COLOR_PER_VERTEX)
			{
				OpenMesh::Vec3d color;
				color = pmesh->color(fv_it);
				::glColor3ub(color[0],color[1],color[2]);
			}

			if(m_ShowTexture)
			{
				if(pmesh->has_vertex_texcoords2D())
				{
					MyMesh::TexCoord2D tex;
					tex = pmesh->texcoord2D(fv_it);
					glTexCoord2f(tex[0],tex[1]);
				}
			}

			// Vertex
			::glVertex3f(pt[0],pt[1],pt[2]);
		} 
		i++;
		::glEnd();
	}

	//seleted vertices
	::glColor3ub(255,0,0);
	::glPointSize(5.0);	
	::glDisable(GL_LIGHTING);
	::glBegin(GL_POINTS);
	for(MyMesh::VertexIter v_it=pmesh->vertices_begin(); v_it!=pmesh->vertices_end(); ++v_it)
	{
		int flag = pmesh->property(vprop_selectedflag,v_it.handle());
		if(flag)
		{
			OpenMesh::Vec3d pt;
			pt = pmesh->point (v_it);
			::glVertex3f(pt[0],pt[1],pt[2]);
		}
	}
	::glEnd();
	::glEnable(GL_LIGHTING);
	::glEndList();

	// List is done now
	m_ListDone = 1;
	m_Modified = 0;

	return 1;
}

double CMesh3d_OpenMesh::GetTriangleAngle_UV(int facehandleindex, double &angle1, double &angle2, double &angle3)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(pmesh->face_handle(facehandleindex));

	std::vector<double> uv1 = pmesh->property(vprop_vertex_uv, fv_it.handle());
	fv_it++;
	std::vector<double> uv2 = pmesh->property(vprop_vertex_uv, fv_it.handle());
	fv_it++;	
	std::vector<double> uv3 = pmesh->property(vprop_vertex_uv, fv_it.handle());

	Vector3D vv1, vv2, vv3;
	vv1.SetValue(uv1[0], uv1[1], 0);
	vv2.SetValue(uv2[0], uv2[1], 0);
	vv3.SetValue(uv3[0], uv3[1], 0);

	double minangle = GetTriangleAngle(vv1, vv2, vv3, angle1, angle2, angle3);
	return minangle;
}

double CMesh3d_OpenMesh::GetTriangleAngle(int facehandleindex, double &angle1, double &angle2, double &angle3)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(pmesh->face_handle(facehandleindex));

	OpenMesh::Vec3d v1 = pmesh->point(fv_it.handle());
	fv_it++;
	OpenMesh::Vec3d v2 = pmesh->point(fv_it.handle());
	fv_it++;
	OpenMesh::Vec3d v3 = pmesh->point(fv_it.handle());

	Vector3D vv1, vv2, vv3;
	vv1.SetValue(v1[0], v1[1], 0);
	vv2.SetValue(v2[0], v2[1], 0);
	vv3.SetValue(v3[0], v3[1], 0);

	double minangle = GetTriangleAngle(vv1, vv2, vv3, angle1, angle2, angle3);
	return minangle;
}
void CMesh3d_OpenMesh::FindSwitchEdge(OpenMesh::FaceHandle fh, double t, int &vindex1,int &vindex2,int &edgeindex)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(fh);

	int vh1index, vh2index, vh3index;
	std::vector<double> v1 = pmesh->property(vprop_vertex_uv, fv_it.handle());
	OpenMesh::Vec3d v1_ = pmesh->point(fv_it.handle());
	vh1index = fv_it.handle().idx();
	fv_it++;
	std::vector<double> v2 = pmesh->property(vprop_vertex_uv, fv_it.handle());
	OpenMesh::Vec3d v2_ = pmesh->point(fv_it.handle());
	vh2index = fv_it.handle().idx();
	fv_it++;
	std::vector<double> v3 = pmesh->property(vprop_vertex_uv, fv_it.handle());
	OpenMesh::Vec3d v3_ = pmesh->point(fv_it.handle());
	vh3index = fv_it.handle().idx();

	Vector3D vv1, vv2, vv3;
	//vv1.SetValue((v1[0] + v1_[0]) / 2, (v1[1] + v1_[1]) / 2, 0);
	//vv2.SetValue((v2[0] + v2_[0]) / 2, (v2[1] + v2_[1]) / 2, 0);
	//vv3.SetValue((v3[0] + v3_[0]) / 2, (v3[1] + v3_[1]) / 2, 0);
	vv1.SetValue((v1[0] * t + v1_[0] * (1 - t)) , (v1[1] * t + v1_[1] * (1 - t)) , 0);
	vv2.SetValue((v2[0] * t + v2_[0] * (1 - t)) , (v2[1] * t + v2_[1] * (1 - t)) , 0);
	vv3.SetValue((v3[0] * t + v3_[0] * (1 - t)) , (v3[1] * t + v3_[1] * (1 - t)) , 0);

	//double l1 = Length(v1_ - v2_);
	//double l2 = Length(v2_ - v3_);
	//double l3 = Length(v3_ - v1_);
	double l1 = Length(vv1 - vv2);
	double l2 = Length(vv2 - vv3);
	double l3 = Length(vv3 - vv1);

	if (l1 > l2 && l1 > l3)
	{
		vindex1 = vh1index;
		vindex2 = vh2index;
	}
	else if (l2 > l1 && l2 > l3)
	{
		vindex1 = vh2index;
		vindex2 = vh3index;
	}
	else if (l3 > l1 && l3 > l2)
	{
		vindex1 = vh1index;
		vindex2 = vh3index;
	}

	edgeindex = GetEdgeHandleFromTwoVertices(vindex1, vindex2);
}
int CMesh3d_OpenMesh::GetEdgeHandleFromTwoVertices(int vindex1,int vindex2)
{
	//Find the edge index
	int edgeindex;
	for (MyMesh::VertexEdgeIter ve_it = pmesh->ve_iter(pmesh->vertex_handle(vindex1)); ve_it; ++ve_it)
	{
		MyMesh::EdgeHandle eh = ve_it.handle();
		MyMesh::HalfedgeHandle hh0 = pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pmesh->from_vertex_handle(hh0);

		int vh1index = vh1.idx();
		int vh2index = vh2.idx();

		if (vh1index == vindex1 && vh2index == vindex2 || vh1index == vindex2 && vh2index == vindex1)
		{
			edgeindex = eh.idx();
			break;
		}
	}
	return edgeindex;
}
double CMesh3d_OpenMesh::GetTriangleSignArea_Mesh_UV_Interpolate(int facehandleindex, double t)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(pmesh->face_handle(facehandleindex));

	int vh1index, vh2index, vh3index;
	std::vector<double> v1_para = pmesh->property(vprop_vertex_uv, fv_it.handle());
	OpenMesh::Vec3d v1 = pmesh->point(fv_it.handle());
	vh1index = fv_it.handle().idx();
	fv_it++;
	std::vector<double> v2_para = pmesh->property(vprop_vertex_uv, fv_it.handle());
	OpenMesh::Vec3d v2 = pmesh->point(fv_it.handle());
	vh2index = fv_it.handle().idx();
	fv_it++;
	std::vector<double> v3_para = pmesh->property(vprop_vertex_uv, fv_it.handle());
	OpenMesh::Vec3d v3 = pmesh->point(fv_it.handle());
	vh3index = fv_it.handle().idx();

	Vector3D vv1, vv2, vv3;
	//vv1.SetValue((v1[0] + v1_[0]) / 2, (v1[1] + v1_[1]) / 2, 0);
	//vv2.SetValue((v2[0] + v2_[0]) / 2, (v2[1] + v2_[1]) / 2, 0);
	//vv3.SetValue((v3[0] + v3_[0]) / 2, (v3[1] + v3_[1]) / 2, 0);

	vv1.SetValue(v1[0] * (1 - t) + v1_para[0] * t, v1[1] * (1 - t) + v1_para[1] * t, 0);
	vv2.SetValue(v2[0] * (1 - t) + v2_para[0] * t, v2[1] * (1 - t) + v2_para[1] * t, 0);
	vv3.SetValue(v3[0] * (1 - t) + v3_para[0] * t, v3[1] * (1 - t) + v3_para[1] * t, 0);

	double signedarea = GetTriangleSignArea(vv1, vv2, vv3);
	return signedarea;
}
double CMesh3d_OpenMesh::GetTriangleSignArea_UV(int facehandleindex)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(pmesh->face_handle(facehandleindex));

	int vh1index, vh2index, vh3index;
	std::vector<double> v1 = pmesh->property(vprop_vertex_uv, fv_it.handle());
	vh1index = fv_it.handle().idx();
	fv_it++;
	std::vector<double> v2 = pmesh->property(vprop_vertex_uv, fv_it.handle());
	vh2index = fv_it.handle().idx();
	fv_it++;
	std::vector<double> v3 = pmesh->property(vprop_vertex_uv, fv_it.handle());
	vh3index = fv_it.handle().idx();

	Vector3D vv1, vv2, vv3;
	vv1.SetValue(v1[0], v1[1], 0);
	vv2.SetValue(v2[0], v2[1], 0);
	vv3.SetValue(v3[0], v3[1], 0);

	double signedarea = GetTriangleSignArea(vv1, vv2, vv3);
	return signedarea;
}
double CMesh3d_OpenMesh::GetTriangleSignArea(int facehandleindex)
{
	MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(pmesh->face_handle(facehandleindex));

	OpenMesh::Vec3d v1 = pmesh->point(fv_it.handle());
	int facevertexindex1 = fv_it.handle().idx();
	fv_it++;
	OpenMesh::Vec3d v2 = pmesh->point(fv_it.handle());
	int facevertexindex2 = fv_it.handle().idx();
	fv_it++;
	OpenMesh::Vec3d v3 = pmesh->point(fv_it.handle());
	int facevertexindex3 = fv_it.handle().idx();

	Vector3D vv1, vv2, vv3;
	vv1.SetValue(v1[0], v1[1], 0);
	vv2.SetValue(v2[0], v2[1], 0);
	vv3.SetValue(v3[0], v3[1], 0);

	double signedarea = GetTriangleSignArea(vv1, vv2, vv3);
	return signedarea;
}
BOOL CMesh3d_OpenMesh::IsTriangleDegenerate_Colinear(int facehandleindex)
{
	double angle1, angle2, angle3, minangle;
	minangle = GetTriangleAngle(facehandleindex, angle1, angle2, angle3);
	if (minangle < 0.01)
		return 1;
	else
		return 0;
}
BOOL CMesh3d_OpenMesh::IsTriangleDegenerate_Colinear_UV(int facehandleindex)
{
	double angle1, angle2, angle3, minangle;
	minangle = GetTriangleAngle_UV(facehandleindex, angle1, angle2, angle3);
	if (minangle < 0.001)
		return 1;
	else
		return 0;
}
void CMesh3d_OpenMesh::GetEdgeOppositeVertices(MyMesh::EdgeHandle &eh, int &vindex3, int &vindex4)
{
	MyMesh::HalfedgeHandle hh0 = pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pmesh->halfedge_handle(eh, 1);
	MyMesh::VertexHandle vh1 = pmesh->to_vertex_handle(hh0);
	MyMesh::VertexHandle vh2 = pmesh->from_vertex_handle(hh0);
	int vh1index = vh1.idx();
	int vh2index = vh2.idx();
	MyMesh::FaceHandle fh0 = pmesh->face_handle(hh0);
	MyMesh::FaceHandle fh1 = pmesh->face_handle(hh1);
	for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(fh0); fv_it; fv_it++)
	{
		vindex3 = fv_it.handle().idx();
		if (vindex3 != vh1index && vindex3 != vh2index)
			break;
	}
	for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(fh1); fv_it; fv_it++)
	{
		vindex4 = fv_it.handle().idx();
		if (vindex4 != vh1index && vindex4 != vh2index)
			break;
	}
}
BOOL CMesh3d_OpenMesh::IsExistEdge(int vindex1, int vindex2)
{
	for (MyMesh::VertexEdgeIter ve_it = pmesh->ve_iter(pmesh->vertex_handle(vindex1)); ve_it; ++ve_it)
	{
		MyMesh::EdgeHandle eh = ve_it.handle();

		MyMesh::HalfedgeHandle hh0 = pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pmesh->halfedge_handle(eh, 1);

		int vh1 = pmesh->to_vertex_handle(hh0).idx();
		int vh2 = pmesh->from_vertex_handle(hh0).idx();

		if (vh1 == vindex1 && vh2 == vindex2 || vh1 == vindex2 && vh2 == vindex1)
		{
			return 1;
		}
	}
	return 0;
}
int CMesh3d_OpenMesh::splitEdge(int index1, int index2)
{
	FPROP_FACE_SELECTED_FLAG fprop_selectedflag;

	//face selected flag
	if (!pmesh->get_property_handle(fprop_selectedflag, PROP_FACE_SELECTED))
		pmesh->add_property(fprop_selectedflag, PROP_FACE_SELECTED);

	int index3, index4;
	int edgeindex = GetEdgeHandleIdx(index1, index2);
	MyMesh::EdgeHandle eh = pmesh->edge_handle(edgeindex);
	//modify index3 index4
	GetEdgeOppositeVertices(eh, index3, index4);

	MyMesh::HalfedgeHandle hh0 = pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pmesh->halfedge_handle(eh, 1);
	MyMesh::FaceHandle fh0 = pmesh->face_handle(hh0);
	MyMesh::FaceHandle fh1 = pmesh->face_handle(hh1);

	//m_ArrayEdgeSwap.push_back(std::make_pair(std::make_pair(vh1.idx(), vh2.idx()), std::make_pair(vindex3, vindex4)));
	//delete two faces
	pmesh->delete_face(fh0, FALSE);
	pmesh->delete_face(fh1, FALSE);

	OpenMesh::Vec3d v1 = pmesh->point(pmesh->vertex_handle(index1));
	OpenMesh::Vec3d v2 = pmesh->point(pmesh->vertex_handle(index2));
	OpenMesh::Vec3d v12center = (v1 + v2) / 2;
	OpenMesh::VertexHandle vhn = pmesh->add_vertex(v12center);
	int indexn = vhn.idx();

	double area;
	std::vector<MyMesh::VertexHandle>  face_vhandles;
	area = GetTriangleSignArea_2D(indexn, index3, index1);
	if (area >= 0)
	{
		face_vhandles.push_back(pmesh->vertex_handle(indexn));
		face_vhandles.push_back(pmesh->vertex_handle(index3));
		face_vhandles.push_back(pmesh->vertex_handle(index1));
		MyMesh::FaceHandle fh2 = pmesh->add_face(face_vhandles);
		pmesh->property(fprop_selectedflag, fh2) = 2;
	}
	else
	{
		face_vhandles.push_back(pmesh->vertex_handle(index1));
		face_vhandles.push_back(pmesh->vertex_handle(index3));
		face_vhandles.push_back(pmesh->vertex_handle(indexn));
		MyMesh::FaceHandle fh2 = pmesh->add_face(face_vhandles);
		pmesh->property(fprop_selectedflag, fh2) = 2;
	}

	area = GetTriangleSignArea_2D(indexn, index1, index4);
	face_vhandles.clear();
	if (area >= 0)
	{
		face_vhandles.push_back(pmesh->vertex_handle(indexn));
		face_vhandles.push_back(pmesh->vertex_handle(index1));
		face_vhandles.push_back(pmesh->vertex_handle(index4));
		MyMesh::FaceHandle fh3 = pmesh->add_face(face_vhandles);
		pmesh->property(fprop_selectedflag, fh3) = 2;
	}
	else
	{
		face_vhandles.push_back(pmesh->vertex_handle(index4));
		face_vhandles.push_back(pmesh->vertex_handle(index1));
		face_vhandles.push_back(pmesh->vertex_handle(indexn));
		MyMesh::FaceHandle fh3 = pmesh->add_face(face_vhandles);
		pmesh->property(fprop_selectedflag, fh3) = 2;
	}

	face_vhandles.clear();
	area = GetTriangleSignArea_2D(index4, index2, indexn);
	if (area >= 0)
	{
		face_vhandles.push_back(pmesh->vertex_handle(index4));
		face_vhandles.push_back(pmesh->vertex_handle(index2));
		face_vhandles.push_back(pmesh->vertex_handle(indexn));
		MyMesh::FaceHandle fh4 = pmesh->add_face(face_vhandles);
		pmesh->property(fprop_selectedflag, fh4) = 2;
	}
	else
	{
		face_vhandles.push_back(pmesh->vertex_handle(indexn));
		face_vhandles.push_back(pmesh->vertex_handle(index2));
		face_vhandles.push_back(pmesh->vertex_handle(index4));
		MyMesh::FaceHandle fh4 = pmesh->add_face(face_vhandles);
		pmesh->property(fprop_selectedflag, fh4) = 2;
	}

	face_vhandles.clear();
	area = GetTriangleSignArea_2D(indexn, index2, index3);
	if (area >= 0)
	{
		face_vhandles.push_back(pmesh->vertex_handle(indexn));
		face_vhandles.push_back(pmesh->vertex_handle(index2));
		face_vhandles.push_back(pmesh->vertex_handle(index3));
		MyMesh::FaceHandle fh5 = pmesh->add_face(face_vhandles);
		pmesh->property(fprop_selectedflag, fh5) = 2;
	}
	else
	{
		face_vhandles.push_back(pmesh->vertex_handle(index3));
		face_vhandles.push_back(pmesh->vertex_handle(index2));
		face_vhandles.push_back(pmesh->vertex_handle(indexn));
		MyMesh::FaceHandle fh5 = pmesh->add_face(face_vhandles);
		pmesh->property(fprop_selectedflag, fh5) = 2;
	}


	pmesh->garbage_collection();
	m_Modified = 1;
	return 1;
}
int CMesh3d_OpenMesh::GetEdgeHandleIdx(int vindex1, int vindex2)
{
	MyMesh::VertexHandle vh1 = pmesh->vertex_handle(vindex1);
	MyMesh::VertexHandle vh2 = pmesh->vertex_handle(vindex2);
	MyMesh::HalfedgeHandle hh0 = pmesh->find_halfedge(vh1, vh2);
	//if(hh0.idx() == -1)
	//	hh0 = pmesh->find_halfedge(vh2, vh1);
	MyMesh::EdgeHandle eh = pmesh->edge_handle(hh0);
	return eh.idx();
}
double CMesh3d_OpenMesh::GetTriangleSignArea_2D(int vindex1, int vindex2, int vindex3)
{
	OpenMesh::VertexHandle vh1 = pmesh->vertex_handle(vindex1);
	OpenMesh::VertexHandle vh2 = pmesh->vertex_handle(vindex2);
	OpenMesh::VertexHandle vh3 = pmesh->vertex_handle(vindex3);
	OpenMesh::Vec3d v1 = pmesh->point(vh1);
	OpenMesh::Vec3d v2 = pmesh->point(vh2);
	OpenMesh::Vec3d v3 = pmesh->point(vh3);

	double x0 = v1[0];
	double y0 = v1[1];
	double x1 = v2[0];
	double y1 = v2[1];
	double x2 = v3[0];
	double y2 = v3[1];

	double signarea = (x0 * y1 + y0 * x2 + x1 * y2 - x2 * y1 - x0 * y2 - x1 * y0) / 2;
	return signarea;
}
double CMesh3d_OpenMesh::GetTriangleSignArea(Vector3D v1, Vector3D v2, Vector3D v3)
{
	double x0 = v1[0];
	double y0 = v1[1];
	double x1 = v2[0];
	double y1 = v2[1];
	double x2 = v3[0];
	double y2 = v3[1];

	double signarea = (x0 * y1 + y0 * x2 + x1 * y2 - x2 * y1 - x0 * y2 - x1 * y0) / 2;
	return signarea;
}
double CMesh3d_OpenMesh::GetTriangleAngle(Vector3D v1, Vector3D v2, Vector3D v3, double &angle1, double &angle2, double &angle3)
{
	double a = Length(v1);
	double b = Length(v2);
	double c = Length(v3);
	angle1 = acos((b * b + c * c - a * a) / (2 * b * c)) * 180 / PI;
	angle2 = acos((a * a + c * c - b * b) / (2 * a * c)) * 180 / PI;
	angle3 = acos((a * a + b * b - c * c) / (2 * a * b)) * 180 / PI;

	return min(min(angle1, angle2), angle3);
}

std::vector<int> CMesh3d_OpenMesh::check_cutpath_valid(std::vector<std::pair<int, int>> m_ArraySharpEdges)
{
	std::vector<int> sbp;
	for (int i = 0; i < m_ArraySharpEdges.size(); i++)
	{
		int flag = 0;
		std::pair<int, int> temp = m_ArraySharpEdges[i];
		int vertexindex = temp.first;
		MyMesh::VertexHandle fisrtvertex = pmesh->vertex_handle(vertexindex);
		for (MyMesh::VertexVertexIter vv_it = pmesh->vv_iter(fisrtvertex); vv_it; ++vv_it)
		{
			if (vv_it.handle().idx() == temp.second)
				flag = 1;
		}
		if (flag == 0)
		{
			sbp.push_back(i);
		}
	}
	return sbp;
}