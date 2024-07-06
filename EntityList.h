#pragma once
#include "lib3d/Object3d.h"
#include <Object3d.h>
#include <SVNMesh.h>
class ENTITY_LIST;
class EntityList : public CObject3d
{
public:
	EntityList();
	~EntityList();

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

