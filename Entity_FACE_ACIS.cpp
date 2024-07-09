#include "stdafx.h"
#include "Entity_FACE_ACIS.h"
#include "EntityList.h"
#include <Mesh3d.h>
#include <Array3d.h>
#include <vector>
#include <GL/gl.h>
#include <Base3d.h>

Entity_FACE_ACIS::Entity_FACE_ACIS()
{
	type = TYPE_BREP_ACIS_FACE;
}
Entity_FACE_ACIS::~Entity_FACE_ACIS()
{

}
//********************************************
// glDraw
//********************************************
void Entity_FACE_ACIS::glRenderTriangulation()
{
	if (!m_ListDone || m_Modified)
		BuildTriangulationList();

	// Search for a new list
	if (::glIsList(m_ListOpenGL) == GL_TRUE)
	{
		::glCallList(m_ListOpenGL);
		return;
	}
	else
	{
		TRACE(" CMesh3d::Draw : unable to draw list %d\n", m_ListOpenGL);
		return;
	}
}
void Entity_FACE_ACIS::BuildTriangulationList()
{
	if (!m_Modified && m_ListDone)
		return;


	// Erase last list
	::glDeleteLists(m_ListOpenGL, 1);

	m_ListOpenGL = ::glGenLists(1);
	if (m_ListOpenGL == 0)
	{
		TRACE("CMesh3d::BuildList : unable to build DrawList\n");
		return;
	}
	::glNewList(m_ListOpenGL, GL_COMPILE_AND_EXECUTE);
	// Position / translation / scaling
	/*glScalef(m_Transform.GetScale()->x(),
		m_Transform.GetScale()->y(),
		m_Transform.GetScale()->z());

	glRotatef(m_Transform.GetValueRotation(),
		m_Transform.GetRotation()->x(),
		m_Transform.GetRotation()->y(),
		m_Transform.GetRotation()->z());

	glTranslatef(m_Transform.GetTranslation()->x(),
		m_Transform.GetTranslation()->y(),
		m_Transform.GetTranslation()->z());
		*/
		//std::vector<SVNMesh::FaceMesh> faceData;
		//for (auto face : faceData)
		//{
		//	face.di
		//}
	int nTriangles = triangles.size() / 3;
	int n1, n2, n3; //vertex indices
	::glBegin(GL_TRIANGLES);
	for (int i = 0; i < nTriangles; i++)
	{
		n1 = triangles[i * 3];
		n2 = triangles[i * 3 + 1];
		n3 = triangles[i * 3 + 2];
		::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
		::glVertex3f(coords[n1 * 3], coords[n1 * 3 + 1], coords[n1 * 3 + 2]);

		::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
		::glVertex3f(coords[n2 * 3], coords[n2 * 3 + 1], coords[n2 * 3 + 2]);

		::glNormal3f(normalCoords[n3 * 3], normalCoords[n3 * 3 + 1], normalCoords[n3 * 3 + 2]);
		::glVertex3f(coords[n3 * 3], coords[n3 * 3 + 1], coords[n3 * 3 + 2]);
	}
	::glEnd();
	::glEndList();

	m_ListDone = 1;
	m_Modified = 0;
}
void Entity_FACE_ACIS::ComputeBoundingBox(Vector3D& lower, Vector3D& upper)
{
	int n = coords.size() / 3;
	if (n == 0)
		return;
	lower[0] = upper[0] = coords[0];
	lower[1] = upper[1] = coords[1];
	lower[2] = upper[2] = coords[2];

	for (int i = 1; i < n; i++)
	{
		if (lower[0] > coords[i * 3])
			lower[0] = coords[i * 3];
		if (lower[1] > coords[i * 3 + 1])
			lower[1] = coords[i * 3 + 1];
		if (lower[2] > coords[i * 3 + 2])
			lower[2] = coords[i * 3 + 2];

		if (upper[0] < coords[i * 3])
			upper[0] = coords[i * 3];
		if (upper[1] < coords[i * 3 + 1])
			upper[1] = coords[i * 3 + 1];
		if (upper[2] < coords[i * 3 + 2])
			upper[2] = coords[i * 3 + 2];
	}
}
int Entity_FACE_ACIS::glDraw()
{
	if (!m_Show)
		return 0;

	::glPushMatrix();
	glTranslated(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);
	glRotatef(m_zRotation, 0.0, 0.0, 1.0);
	glScalef(m_xScaling, m_yScaling, m_zScaling);

	glFrontFace(GL_CW);
	//------------
	// Material

	//
	glEnable(GL_POINT_SMOOTH); // 启用反走样
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND); // 启动融合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 产生融合因子
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);// 权衡图像质量与绘制速度 


	//construct texture
	GLubyte* pTextureBits;
	GLint width, height;
	/*if (m_ShowTexture)
	{
		m_pDib = new CDib(m_FileTextureName);
		pTextureBits = (GLubyte*)m_pDib->GetDibBitsPtr();
		width = m_pDib->GetDibWidth();
		height = m_pDib->GetDibHeight();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pTextureBits);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}*/
	//---------------
	//This is a for Bezier surface;
	//glMap2f(GL_MAP2_VERTEX_4,
	//	0.0, 1.0, 4, m_udegree + 1, 0.0, 1.0, 800, m_vdegree + 1,
	//	&m_ControlPoint[0][0][0]);

	//
	GLboolean re1 = glIsEnabled(GL_MAP2_VERTEX_4);
	if (re1 == GL_INVALID_ENUM || re1 == GL_INVALID_VALUE)
	{
		int i = 0;
	}

	//----------- Show Texture----------------
	/*if (m_ShowTexture)
	{
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, (const float*)textureCntrlPnt);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_MAP2_TEXTURE_COORD_2);
	}*/
	//-----------End Texture--------------

	//::glDisable(GL_LIGHTING);
	glEnable(GL_MAP2_VERTEX_4);
	//glColor3f(0.0,0.0,1.0);

	glMapGrid2f(30, 0.0, 1.0, 30, 0.0, 1.0);
	::glPushMatrix();

	//Show the Coordinate Axis
	//if (m_ShowCoordinateAxis)
	{
		//	DrawCoordinateAxis();
	}

	if (!m_SetCameraMode)
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
	glGetIntegerv(GL_VIEWPORT, m_viewPort);

	//Draw the iso parameter curves.

	//if (m_showSurface)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//if (pDoc->m_Light)
		::glEnable(GL_LIGHTING);
		//else
		//	::glDisable(GL_LIGHTING);

		//glColor3f(s_r, s_g, s_b);

		{
			//::glDisable(GL_LIGHTING);
			glRenderTriangulation();
			//::glEnable(GL_LIGHTING);
		}

		glLineWidth(2.0f);
	}



	//Draw the Area Preserving Iso-Parameter Curves.
	//Stored in the uarray & varray

	::glPopMatrix();

	::glPopMatrix();
	glFrontFace(GL_CCW);

	glDisable(GL_MAP2_VERTEX_4);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
	glDisable(GL_TEXTURE_2D);

	return 1;
}