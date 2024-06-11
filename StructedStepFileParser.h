#pragma once
#include "StructedStepFile.h"

class SG_DECLSPEC StructedStepFileParser
{

public://对外接口

	//Body* ParseAndCreateManifoldSolidBrep();

	void ParseAndCreateSurface();

protected:
	//输入
	const StructedStepFile& m_File;

	//输出
	//Body* m_pManifoldSolidBreps;

	std::unordered_map <size_t, void*>  m_TranslatedItemInfo; //（数组类对象的索引，类对象的地址）


public:
	StructedStepFileParser(const StructedStepFile& p_File);
	~StructedStepFileParser();

//protected:
	//double CalculateHeight(Loop* aLoop);

//protected: //Topology
	//Lump* ParseManifoldSolidBrep(size_t p_Index);

	//Shell* ParseShell(size_t p_Index);
	//Shell* ParseClosedShell(size_t p_Index);
	//Shell* ParseOpenShell(size_t p_Index);

	//Face* ParseFace(size_t p_Index);
	//Face* ParseAdvancedFace(size_t p_Index);

	//Loop* ParseFaceBound(size_t p_Index);
	//Loop* ParseFaceInnerBound(size_t p_Index);
	//Loop* ParseFaceOuterBound(size_t p_Index);

	//Loop* ParseLoop(size_t p_Index);
	//Loop* ParseEdgeLoop(size_t p_Index);

	//Coedge* ParseOrientedEdge(size_t p_Index);

	//Edge* ParseEdge(size_t p_Index);
	//Edge* ParseEdgeCurve(size_t p_Index);

	//Vertex* ParseVertex(size_t p_Index);
	//Vertex* ParseVertexPoint(size_t p_Index);

//protected: //Geometry
	//Surface
	//ModelSurface* ParseSurface(size_t p_Index);

	//ModelPlane* ParsePlane(size_t p_Index);

	//ModelCylinderSurface* ParseCylindricalSurface(size_t p_Index);

	//ModelSphereSurface* ParseSphereSurface(size_t p_Index);

	//ModelTorusSurface* ParseTorusSurface(size_t p_Index);

	//ModelBsplineSurface* ParseBsplineSurface(size_t p_Index);

	//ModelNurbsSurface* ParseNurbsSurface(size_t p_Index);

	//ModelConeSurface* ParseConeSurface(size_t p_Index);

	////Curve
	//ModelCurve* ParseCurve(size_t p_Index);

	//ModelLine* ParseLine(size_t p_Index);

	//ModelCircle* ParseCircle(size_t p_Index);

	//ModelEllipse* ParseEllipse(size_t p_Index);

	//ModelBsplineCurve* ParseBsplineCurve(size_t p_Index);

	//ModelNurbsCurve* ParseNurbsCurve(size_t p_Index);

	////Other
	//ModelPoint* ParseCartesianPoint(size_t p_Index);

	//EntityUcs* ParseAxis2Placement3d(size_t p_Index);

	//Vector3D* ParseDirection(size_t p_Index);

	//Vector3D* ParseVector(size_t p_Index);

protected:
	template<class ENTITY>
	ENTITY* GetParsed(size_t p_Index);
};


template<class ENTITY>
ENTITY* StructedStepFileParser::GetParsed(size_t p_Index)
{
	auto iter = m_TranslatedItemInfo.find(p_Index);

	if (iter != m_TranslatedItemInfo.end()) {

		void* pItem = (*iter).second;
		assert(pItem != nullptr);
		return reinterpret_cast<ENTITY*>(pItem);
	}

	return nullptr;
}
