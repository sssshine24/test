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

<<<<<<< HEAD
	void SaveObjFile(char* filename, EntityList_ACIS* pEntityList);
=======
	void SaveObjFile(char* filename, EntityList* pEntityList);
>>>>>>> 4d531c4dcf38db10b06bd94d29ba52f86eb4a186
};

