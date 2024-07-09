#pragma once
#include "EntityList.h"
class CSceneGraph3d;
class ENTITY_LIST;
class CParseSTEP
{
	//Parse PSLG
public:
	//
	int Run_STEP(char* filename, CSceneGraph3d* pSceneGraph);
	//
	int get_FaceList_from_EntityList(ENTITY_LIST* pAcisEntities, ENTITY_LIST* pFaceEntities);

	void SaveObjFile(char* filename, EntityList_ACIS* pEntityList);
};

