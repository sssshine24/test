#include "stdafx.h"
#include "stdafx.h"
#include <math.h>
#include "lib3d/Base3d.h"
#include "SDIViewSwitch.h"
#include "lib3d/Mesh3d.h"
#include "lib3d/ColorRamp.h"
#include "FormView3.h"
#include "MyTreeView.h"
#include <fstream>
#include <atlimage.h>
#include <queue> 
#include "Action_PolyOmino.h"
#include <gl/glut.h>
#include "lib3d/Mesh3d_OpenMesh.h"
#include "GeoMapLandMark.h"
#include "Shape_Polygon.h"

Shape_Polygon::Shape_Polygon()
{
	m_showPolygonRingEdge = 1;
	m_showPolygonRingIndex = 0;
	m_showPolygonInnerRing = 0;
	filename = "Polygon";
}


Shape_Polygon::~Shape_Polygon()
{
}
int Shape_Polygon::GetType()
{
	return TYPE_POLYGON;
}
int Shape_Polygon::glBuildList()
{
	return 1;
}

double Shape_Polygon::polygonArea(double* X, double* Y, int numPoints)
{
	double area = 0;         // Accumulates area in the loop
	int j = numPoints - 1;  // The last vertex is the 'previous' one to the first

	for (int i = 0; i < numPoints; i++)
	{
		area = area + (X[j] + X[i]) * (Y[j] - Y[i]);
		j = i;  //j is previous vertex to i
	}
	return area / 2;
}
double Shape_Polygon::polygonArea(Record* pRecord, int ringindex)
{
	//
	int firstvindex, endvindex;
	firstvindex = pRecord->m_recordcontent.m_ArrayParts[ringindex];
	if (ringindex < pRecord->m_recordcontent.m_NumParts - 1)
		endvindex = pRecord->m_recordcontent.m_ArrayParts[ringindex + 1];
	else
		endvindex = pRecord->m_recordcontent.m_ArrayPoints.size();
	endvindex = endvindex - 2;
	int vnum = endvindex - firstvindex + 1;
	double* X = new double[vnum];
	double* Y = new double[vnum];
	for (int i = firstvindex; i <= endvindex; i++)
	{
		X[i - firstvindex] = pRecord->m_recordcontent.m_ArrayPoints[i].x;
		Y[i - firstvindex] = pRecord->m_recordcontent.m_ArrayPoints[i].y;
	}
	double area = polygonArea(X, Y, vnum);
	delete X;
	delete Y;
	return area;
}
int Shape_Polygon::ShowPolygonRingIndex()
{
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(1.0, 0.0, 0.0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(2.0);
	double x, y;
	//::glBegin(GL_LINES);
	for (int i = 0; i < m_ArrayRecords.size(); i++)
	{
		Record* pRecord = m_ArrayRecords[i];
		int firstvindex, endvindex;
		//if (i == 1)
		for (int j = 0; j < pRecord->m_recordcontent.m_NumParts; j++)
		{
			firstvindex = pRecord->m_recordcontent.m_ArrayParts[j];
			if (j < pRecord->m_recordcontent.m_NumParts - 1)
				endvindex = pRecord->m_recordcontent.m_ArrayParts[j + 1];
			else
				endvindex = pRecord->m_recordcontent.m_ArrayPoints.size();

			endvindex--;
			//Compute the central x,y
			x = y = 0;
			for (int k = firstvindex; k < endvindex; k++)
			{
				x += pRecord->m_recordcontent.m_ArrayPoints[k].x;
				y += pRecord->m_recordcontent.m_ArrayPoints[k].y;
			}
			x = x / (endvindex - firstvindex);
			y = y / (endvindex - firstvindex);
			char message[100];
			sprintf(message, "%d", pRecord->m_recordcontent.m_ArrayRingIndex[j]);
			p_FormView3->drawOpenglString(x, y, 0, message);
		}
	}
	::glDepthFunc(GL_LESS);
	return 1;
}
int Shape_Polygon::ShowPolygonInnerRing()
{
	::glColor3f(0.0, 0.0, 128.0 / 256);
	::glDepthFunc(GL_LESS);
	::glDisable(GL_LIGHTING);
	::glLineWidth(4.0);
	::glBegin(GL_LINES);
	for (int i = 0; i < m_ArrayRecords.size(); i++)
	{
		Record* pRecord = m_ArrayRecords[i];
		int firstvindex, endvindex;
		//if (i == 1)
		for (int j = 0; j < pRecord->m_recordcontent.m_NumParts; j++)
		{
			//if (pRecord->m_recordcontent.m_ArrayArea[j] < 0)
			//{
			//	::glColor3f(1.0, 0.0, 0); 
			//}
			//else
			//{
			//	::glColor3f(0.0, 0.0, 128.0 / 256);
			//continue;
			//}
			if (pRecord->m_recordcontent.m_ArrayArea[j] < 0)
			{
				::glColor3f(1.0, 0.0, 0);
			}
			else
			{
				continue;
			}

			firstvindex = pRecord->m_recordcontent.m_ArrayParts[j];
			if (j < pRecord->m_recordcontent.m_NumParts - 1)
				endvindex = pRecord->m_recordcontent.m_ArrayParts[j + 1];
			else
				endvindex = pRecord->m_recordcontent.m_ArrayPoints.size();
			for (int k = firstvindex; k < endvindex - 1; k++)
			{
				::glVertex3f(pRecord->m_recordcontent.m_ArrayPoints[k].x, pRecord->m_recordcontent.m_ArrayPoints[k].y, 0);
				::glVertex3f(pRecord->m_recordcontent.m_ArrayPoints[k + 1].x, pRecord->m_recordcontent.m_ArrayPoints[k + 1].y, 0);
			}
		}
	}

	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int Shape_Polygon::ShowPolygonRings()
{
	::glColor3f(0.0, 0.0, 128.0 / 256);
	::glDepthFunc(GL_LESS);
	::glDisable(GL_LIGHTING);
	::glLineWidth(4.0);
	::glBegin(GL_LINES);
	for (int i = 0; i < m_ArrayRecords.size(); i++)
	{
		Record* pRecord = m_ArrayRecords[i];
		int firstvindex, endvindex;
		//if (i == 1)
		for (int j = 0; j < pRecord->m_recordcontent.m_NumParts; j++)
		{
			//if (pRecord->m_recordcontent.m_ArrayArea[j] < 0)
			//{
			//	::glColor3f(1.0, 0.0, 0); 
			//}
			//else
			//{
			//	::glColor3f(0.0, 0.0, 128.0 / 256);
			//continue;
			//}
			if (pRecord->m_recordcontent.m_ArrayFlag[j])
			{
				::glColor3f(1.0, 0.0, 0);
				continue;
			}
			else
			{
				::glColor3f(0.0, 0.0, 128.0 / 256);
			}

			firstvindex = pRecord->m_recordcontent.m_ArrayParts[j];
			if (j < pRecord->m_recordcontent.m_NumParts - 1)
				endvindex = pRecord->m_recordcontent.m_ArrayParts[j + 1];
			else
				endvindex = pRecord->m_recordcontent.m_ArrayPoints.size();
			for (int k = firstvindex; k < endvindex - 1; k++)
			{
				::glVertex3f(pRecord->m_recordcontent.m_ArrayPoints[k].x, pRecord->m_recordcontent.m_ArrayPoints[k].y, 0);
				::glVertex3f(pRecord->m_recordcontent.m_ArrayPoints[k + 1].x, pRecord->m_recordcontent.m_ArrayPoints[k + 1].y, 0);
			}
		}
	}

	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int Shape_Polygon::glDraw()
{
	if (!m_Show)
		return 0;
	::glPushMatrix();

	// Position / translation / scaling
	if (!m_SetCameraMode)
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
	glGetIntegerv(GL_VIEWPORT, m_viewPort);

	//ShowPolygonRings
	if(m_showPolygonRingEdge)
		ShowPolygonRings();

	//ShowPolygonRingIndex
	if(m_showPolygonRingIndex)
		ShowPolygonRingIndex();

	//ShowPolygonInnerRing
	if (m_showPolygonInnerRing)
		ShowPolygonInnerRing();

	::glPopMatrix();
	return 1;
}
