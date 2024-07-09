#pragma once
#include "lib3d/Object3d.h"
#include <Object3d.h>
#include <SVNMesh.h>
class ENTITY_LIST;
<<<<<<< HEAD
class EntityList_ACIS : public CObject3d
{
public:
	EntityList_ACIS();
	~EntityList_ACIS();
=======
class EntityList : public CObject3d
{
public:
	EntityList();
	~EntityList();
>>>>>>> 4d531c4dcf38db10b06bd94d29ba52f86eb4a186

	ENTITY_LIST* pAcisEntities;

	std::vector<float> coords;
	std::vector<int> triangles;
	std::vector<float> normalCoords;

	std::vector<SVNMesh::FaceMesh> faceData;

	//
	void SetTransform(CTransform& transform) { m_Transform.Copy(transform); }

	//
	CTransform* GetTransform(void) { return &m_Transform; }

	void BuildTriangulationList();

	void glRenderTriangulation();

	virtual int glDraw();

	virtual void ComputeBoundingBox(Vector3D& lower, Vector3D& upper);
	
};

