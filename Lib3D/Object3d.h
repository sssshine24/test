//********************************************
// Object3d.h
//********************************************
// class CObject3d, the base 3d object
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 17/12/97
// Modified : 19/12/97
//********************************************

#ifndef _OBJECT3D_
#define _OBJECT3D_
#include "Transform.h"
#include "Point3D.h"
class CObject3d 
{
public :
	int type;
	CString filename;
	CString name;

	HTREEITEM pTreeItem;

	CObject3d* pParentObject;

	int m_ScenegraphIndex;

	unsigned int m_ListOpenGL;
	unsigned int m_ListDone;
	int m_Modified;
	int m_Show;
	int m_IsMove;

	//glTransform parameters
	float m_xTranslation, m_yTranslation, m_zTranslation, m_xRotation, m_yRotation, m_zRotation,m_xScaling, m_yScaling, m_zScaling;
	//
	CTransform m_Transform;
	CString m_Name;

	//
	virtual void ComputeBoundingBox(Point3D &lower, Point3D &upper) {}

	//store the matrix
	GLdouble	modelview[16];    
	GLdouble	projection[16]; 
	GLint		m_viewPort [4];

public :
	CObject3d() 
	{
		filename = "Mesh";
		type=-1;
		m_IsMove = 1;
		pTreeItem = NULL;
		m_ScenegraphIndex = -1;
		pParentObject = NULL;
	}
	virtual ~CObject3d() {}

	// Datas
	virtual int GetType();
	virtual int glBuildList();
	virtual int glDraw();
	virtual void UpdateTreeControl(CTreeCtrl* pTreeControl);
	int SetTransform(float m_xTranslation, float m_yTranslation, float m_zTranslation,
		float m_xRotation, float m_yRotation, float m_zRotation,
		float m_xScaling, float m_yScaling, float m_zScaling);

	// Transform
	void SetTransform(CTransform& transform) { m_Transform.Copy(transform); }
	CTransform* GetTransform(void) { return &m_Transform; }

};

#endif // _OBJECT3D_
