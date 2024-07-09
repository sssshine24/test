#pragma once
#include "Object3d.h"
#include <SVNMesh.h>
class FACE;
class EntityList_ACIS;
class Entity_FACE_ACIS :
    public CObject3d
{

public:
	Entity_FACE_ACIS();
	~Entity_FACE_ACIS();

	FACE* pAcisEntities;

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

