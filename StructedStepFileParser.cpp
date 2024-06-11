#include "stdafx.h"
#include "pch.h"
#include "include.h"
#include "StringUtility.h"
#include "StructedStepFileDataItem.h"
#include "StructedStepFile.h"
#include "StructedStepFileParser.h"


using namespace std;

StructedStepFileParser::StructedStepFileParser(const StructedStepFile& p_File) :
	m_File(p_File) {


}

StructedStepFileParser::~StructedStepFileParser()
{

}
void StructedStepFileParser::ParseAndCreateSurface()
{




}
//
//Lump* StructedStepFileParser::ParseManifoldSolidBrep(size_t p_Index)
//{
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	Lump* r_pLump = GetParsed<Lump>(p_Index);
//
//	//如果已经解析过
//	if (r_pLump != nullptr)
//	{
//		return r_pLump;
//	}
//
//	//r_pLump = new Lump(nullptr, nullptr, nullptr);
//	r_pLump = new Lump();
//
//	//ManifoldSolidBrep STEP文件文本实例
//	//#229 = MANIFOLD_SOLID_BREP ( '凸台-拉伸2', #436 ) ;
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//对其进行解析，创建具体的ManifoldSolidBrep类对象
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(2 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 shells StepClosedShell*-->ClosedShell*
//	Shell* pShell;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//		// 创建ClosedShell对象，将其地址赋给当前对象属性pShell
//		pShell = ParseClosedShell(id - 1);
//		assert(nullptr != pShell);
//		pShell->SetOwner(r_pLump);
//	}
//	pShell->SetNext(pShell);
//
//	r_pLump->SetShell(pShell);
//	assert(r_pLump != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pLump));
//
//	return r_pLump;
//}
//
//Shell* StructedStepFileParser::ParseShell(size_t p_Index)
//{
//	Shell* r_pShell = nullptr;
//
//	const String& itemType = m_File[p_Index].m_StepType;
//	if (itemType == _T("OPEN_SHELL"))
//	{
//		r_pShell = ParseOpenShell(p_Index);
//	}
//	if (itemType == _T("CLOSED_SHELL"))
//	{
//		r_pShell = ParseClosedShell(p_Index);
//	}
//
//	assert(r_pShell != nullptr);
//	return r_pShell;
//}
//
//Shell* StructedStepFileParser::ParseClosedShell(size_t p_Index)
//{
//	Shell* r_pClosedShell = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pClosedShell = GetParsed<Shell>(p_Index);
//
//	//如果已经转换
//	if (r_pClosedShell != nullptr) {
//		return r_pClosedShell;
//	}
//
//	//r_pClosedShell = new Shell(nullptr, nullptr, nullptr, nullptr, nullptr);
//	r_pClosedShell = new Shell();
//
//	//Closed_Shell  STEP文件文本实例
//	//#77 = CLOSED_SHELL ( 'NONE', ( #120, #41 ) );
//	//StepClosedShell（String name, vector<StepFace*>）-- > ClosedShell(vector<Face*>)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(2 == splits.size());
//	//开始根据 value 分割结果构建参数
//
//
//	//参数1：name
//	const String& name = splits[0];
//
//	//参数2：pFaces  vector<StepFace*>-->vector<Face*>
//	bool flag = false;
//	Face* p_Faces = nullptr;
//	Face* ans = nullptr;
//	{
//		String& FaceArrayObject = splits[1];
//		assert(FaceArrayObject.size() > 2);
//		String FaceArray = FaceArrayObject.substr(1, FaceArrayObject.size() - 2);
//		vector<String> faceIds = SplitBy(FaceArray, _T("$"));
//		assert(0 < faceIds.size());
//
//		for (const auto& bid : faceIds)
//		{
//			size_t id = stoul(bid);
//			assert(id > 0);
//
//			//将对应id-1位置的StepFace对象转换成Face对象
//			if (!flag) {
//				p_Faces = ParseFace(id - 1);
//				p_Faces->SetOwner(r_pClosedShell);
//				ans = p_Faces;
//				flag = true;
//			}
//			else {
//				Face* temp = ParseFace(id - 1);
//				assert(nullptr != temp);
//				ans->SetNext(temp);
//				temp->SetOwner(r_pClosedShell);
//				ans = temp;
//			}
//
//		}
//	}
//	ans->SetNext(p_Faces);
//
//	//r_pClosedShell->SetContainFace(p_Faces);
//	SubShell* subshell = new SubShell(r_pClosedShell, nullptr, p_Faces, nullptr);
//	r_pClosedShell->SetContainSubShell(subshell);
//	assert(r_pClosedShell != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pClosedShell));
//
//	return r_pClosedShell;
//}
//
//Shell* StructedStepFileParser::ParseOpenShell(size_t p_Index)
//{
//	Shell* r_pOpenShell = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pOpenShell = GetParsed<Shell>(p_Index);
//
//	//如果已经转换
//	if (r_pOpenShell != nullptr) {
//		return r_pOpenShell;
//	}
//
//	//r_pOpenShell = new Shell(nullptr, nullptr, nullptr, nullptr, nullptr);
//	r_pOpenShell = new Shell();
//
//	//Open_Shell  STEP文件文本实例
//	//#76 = Open_SHELL ( 'NONE', ( #15, #41 ) );
//	//StepOpenShell（String name, vector<StepFace*>）-- > OpenShell(vector<Face*>)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(2 == splits.size());
//	//开始根据 value 分割结果构建参数
//
//
//	//参数1：name
//	const String& name = splits[0];
//	//参数2：pFaces  vector<StepFace*>-->vector<Face*>
//	bool flag = false;
//	Face* p_Faces = nullptr;
//	Face* ans = nullptr;
//	{
//		String& FaceArrayObject = splits[1];
//		assert(FaceArrayObject.size() > 2);
//		String FaceArray = FaceArrayObject.substr(1, FaceArrayObject.size() - 2);
//		vector<String> faceIds = SplitBy(FaceArray, _T("$"));
//		assert(0 < faceIds.size());
//
//		for (const auto& bid : faceIds)
//		{
//			size_t id = stoul(bid);
//			assert(id > 0);
//
//			//将对应id-1位置的StepFace对象转换成Face对象
//			if (!flag) {
//				p_Faces = ParseFace(id - 1);
//				p_Faces->SetOwner(r_pOpenShell);
//				ans = p_Faces;
//				flag = true;
//			}
//			else {
//				Face* temp = ParseFace(id - 1);
//				assert(nullptr != temp);
//				ans->SetNext(temp);
//				temp->SetOwner(r_pOpenShell);
//				ans = temp;
//			}
//		}
//	}
//	ans->SetNext(p_Faces);
//
//	r_pOpenShell->SetContainFace(p_Faces);
//	SubShell* subshell = new SubShell(r_pOpenShell, nullptr, p_Faces, nullptr);
//	r_pOpenShell->SetContainSubShell(subshell);
//	assert(r_pOpenShell != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pOpenShell));
//
//	return r_pOpenShell;
//}
//
///*
//	StepFace类型对象转换成Face类型对象的入口
//*/
//Face* StructedStepFileParser::ParseFace(size_t p_Index)
//{
//	Face* r_pFace = nullptr;
//
//	const String& itemType = m_File[p_Index].m_StepType;
//	if (itemType == _T("ADVANCED_FACE"))
//	{
//		r_pFace = ParseAdvancedFace(p_Index);
//	}
//
//
//	assert(r_pFace != nullptr);
//	return r_pFace;
//}
//
//Face* StructedStepFileParser::ParseAdvancedFace(size_t p_Index)
//{
//	Face* r_pFace = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pFace = GetParsed<Face>(p_Index);
//
//	//若已经转换
//	if (r_pFace != nullptr) {
//		return r_pFace;
//	}
//
//	//r_pFace = new Face(nullptr, nullptr, nullptr, nullptr);
//	r_pFace = new Face();
//
//	//AdvancedFace  STEP文件文本实例
//	//#41 = ADVANCED_FACE('NONE', (#65), #117, .T.);
//	//StepAdvancedFace(String name, vector<StepFaceBound*>, StepSurface*, bool sameSense)
//	//	--->AdvancedFace(vector<FaceBound*>, Surface3d*, bool sameSense)
//
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(4 == splits.size());
//	//开始根据 value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 p_faceBound  vector<StepFaceBound*>-->vector<FaceBound*>
//	bool flag = false;
//	Loop* p_faceBound = nullptr;
//	Loop* ans = nullptr;
//	{
//		String& faceBoundArrayObject = splits[1];
//		assert(faceBoundArrayObject.size() > 2);
//		String faceBoundArray = faceBoundArrayObject.substr(1, faceBoundArrayObject.size() - 2);
//		vector<String> boundIds = SplitBy(faceBoundArray, _T("$"));
//		assert(0 < boundIds.size());
//		for (const auto& bid : boundIds)
//		{
//			size_t id = stoul(bid);
//			assert(id > 0);
//			//将对应id-1位置的StepFaceBound*转换成FaceBound*
//			if (!flag) {
//				p_faceBound = ParseFaceBound(id - 1);
//				assert(nullptr != p_faceBound);
//				p_faceBound->SetFace(r_pFace);
//				ans = p_faceBound;
//				flag = true;
//			}
//			else {
//				Loop* temp = ParseFaceBound(id - 1);
//				assert(nullptr != temp);
//				const String& itemType = m_File[id - 1].m_StepType;
//				if (itemType == _T("FACE_OUTER_BOUND"))
//				{
//					temp->SetFace(r_pFace);
//					temp->SetNext(p_faceBound);
//					p_faceBound = temp;
//					continue;
//				}
//				ans->SetNext(temp);
//				temp->SetFace(r_pFace);
//				ans = temp;
//			}
//
//		}
//	}
//	ans->SetNext(p_faceBound);
//
//	//参数 3 p_faceGeometry  StepSurface*-->Surface3d*
//	ModelSurface* p_faceGeometry = nullptr;
//	{
//		size_t id = stoul(splits[2]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepSurFace*转换成Surface3d*
//		p_faceGeometry = ParseSurface(id - 1);
//		assert(nullptr != p_faceGeometry);
//	}
//	//参数 4 sameSense
//	bool sameSense = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[3][1])
//		{
//		case 'T':
//			sameSense = true;
//			break;
//		case 'F':
//			sameSense = false;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	r_pFace->SetLoop(p_faceBound);
//	r_pFace->SetGeometry(p_faceGeometry);
//	r_pFace->SetSense(sameSense);
//	assert(r_pFace != nullptr);
//
//	uint16_t element = r_pFace->GetGeometry()->GetType();
//	ModelSurface* surface = r_pFace->GetGeometry();
//
//	if (element == PLANE_MODEL)
//	{
//		//ModelPlane* plane = static_cast<ModelPlane*>(surface);
//
//	}
//	else if (element == CYLINDERSURFACE_MODEL)
//	{
//		ModelCylinderSurface* cylinder = static_cast<ModelCylinderSurface*>(surface);
//		ConsCylinderSurface* conscylinder = cylinder->GetGeometry();
//
//
//		Loop* head_loop = r_pFace->GetLoop();
//		Loop* ptr_loop = head_loop;
//		bool flag_loop = false;
//		double max_u = DBL_MIN, min_u = DBL_MAX;
//		double max_v = DBL_MIN, min_v = DBL_MAX;
//
//		double aMaxLineHeight = CalculateHeight(head_loop);
//
//		r_pFace->GetGeometry()->GetGeometry()->SetIntervalU(0.0, 1.0, 0);
//		r_pFace->GetGeometry()->GetGeometry()->SetIntervalV(0.0, aMaxLineHeight, 0);
//
//		while (ptr_loop)
//		{
//			if (head_loop == ptr_loop && flag_loop == true)
//			{
//				break;
//			}
//
//			Coedge* coedge = ptr_loop->GetStart();
//			Coedge* head_coedge = dynamic_cast<Coedge*>(coedge);
//			Coedge* ptr_coedge = head_coedge;
//			bool flag_coedge = false;
//
//			while (ptr_coedge)
//			{
//				if (head_coedge == ptr_coedge && flag_coedge == true)
//				{
//					break;
//				}
//				ConsCurve* curve = ptr_coedge->GetEdge()->GetGeometry()->GetGeometry();
//
//				bool sense = ptr_coedge->GetSense();
//				Curve2D* curve2d = PCurveConstruct::MapCurveToPara(curve, conscylinder, sense, sameSense);
//				PCurve* pcurve = new PCurve(curve2d);
//				ModelPCurve* modelpcurve = new ModelPCurve(pcurve);
//				ptr_coedge->SetGeometry(modelpcurve);
//				/*
//				double start_x = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetStart().GetX();
//				double start_y = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetStart().GetY();
//				double end_x = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetEnd().GetX();
//				double end_y = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetEnd().GetY();
//				*/
//
//				flag_coedge = true;
//				ptr_coedge = ptr_coedge->GetNext();
//			}
//			flag_loop = true;
//			ptr_loop = ptr_loop->GetNext();
//		}
//
//	}
//	else if (element == SPHERESURFACE_MODEL)
//	{
//		ModelSphereSurface* sphere = static_cast<ModelSphereSurface*>(surface);
//		ConsSphereSurface* conssphere = sphere->GetGeometry();
//
//
//		Loop* head_loop = r_pFace->GetLoop();
//		Loop* ptr_loop = head_loop;
//		bool flag_loop = false;
//		double max_u = DBL_MIN, min_u = DBL_MAX;
//		double max_v = DBL_MIN, min_v = DBL_MAX;
//
//		double r = conssphere->GetRadius();
//
//		r_pFace->GetGeometry()->GetGeometry()->SetIntervalU(0.0, 1.0, 0);
//		r_pFace->GetGeometry()->GetGeometry()->SetIntervalV(0, 2 * r, 0);
//
//		while (ptr_loop)
//		{
//			if (head_loop == ptr_loop && flag_loop == true)
//			{
//				break;
//			}
//
//			Coedge* coedge = ptr_loop->GetStart();
//			Coedge* head_coedge = dynamic_cast<Coedge*>(coedge);
//			Coedge* ptr_coedge = head_coedge;
//			bool flag_coedge = false;
//
//			while (ptr_coedge)
//			{
//				if (head_coedge == ptr_coedge && flag_coedge == true)
//				{
//					break;
//				}
//				ConsCurve* curve = ptr_coedge->GetEdge()->GetGeometry()->GetGeometry();
//
//				bool sense = ptr_coedge->GetSense();
//				Curve2D* curve2d = PCurveConstruct::MapCurveToPara(curve, conssphere, sense, sameSense);
//				PCurve* pcurve = new PCurve(curve2d);
//				ModelPCurve* modelpcurve = new ModelPCurve(pcurve);
//				ptr_coedge->SetGeometry(modelpcurve);
//				/*
//				double start_x = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetStart().GetX();
//				double start_y = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetStart().GetY();
//				double end_x = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetEnd().GetX();
//				double end_y = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetEnd().GetY();
//				*/
//
//				flag_coedge = true;
//				ptr_coedge = ptr_coedge->GetNext();
//			}
//			flag_loop = true;
//			ptr_loop = ptr_loop->GetNext();
//		}
//
//	}
//	else if (element == TOURSSURFACE_MODEL)
//	{
//		//ModelTorusSurface* torus = static_cast<ModelTorusSurface*>(surface);
//
//	}
//	else if (element == CONESURFACE_MODEL)
//	{
//		ModelConeSurface* cone = static_cast<ModelConeSurface*>(surface);
//		ConsConeSurface* conscone = cone->GetGeometry();
//
//
//		Loop* head_loop = r_pFace->GetLoop();
//		Loop* ptr_loop = head_loop;
//		bool flag_loop = false;
//		double max_u = DBL_MIN, min_u = DBL_MAX;
//		double max_v = DBL_MIN, min_v = DBL_MAX;
//
//
//		while (ptr_loop)
//		{
//			if (head_loop == ptr_loop && flag_loop == true)
//			{
//				break;
//			}
//
//			Coedge* coedge = ptr_loop->GetStart();
//			Coedge* head_coedge = dynamic_cast<Coedge*>(coedge);
//			Coedge* ptr_coedge = head_coedge;
//			bool flag_coedge = false;
//
//			while (ptr_coedge)
//			{
//				if (head_coedge == ptr_coedge && flag_coedge == true)
//				{
//					break;
//				}
//				ConsCurve* curve = ptr_coedge->GetEdge()->GetGeometry()->GetGeometry();
//
//				bool sense = ptr_coedge->GetSense();
//				Curve2D* curve2d = PCurveConstruct::MapCurveToPara(curve, conscone, sense, sameSense);
//				PCurve* pcurve = new PCurve(curve2d);
//				ModelPCurve* modelpcurve = new ModelPCurve(pcurve);
//				ptr_coedge->SetGeometry(modelpcurve);
//				/*
//				double start_x = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetStart().GetX();
//				double start_y = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetStart().GetY();
//				double end_x = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetEnd().GetX();
//				double end_y = ptr_coedge->GetGeometry()->GetGeometry()->GetCurve2D()->GetEnd().GetY();
//				*/
//
//				flag_coedge = true;
//				ptr_coedge = ptr_coedge->GetNext();
//			}
//			flag_loop = true;
//			ptr_loop = ptr_loop->GetNext();
//		}
//
//
//	}
//	else if (element == BSPLINESURFACE_MODEL)
//	{
//		//ModelBsplineSurface* bspline = static_cast<ModelBsplineSurface*>(surface);
//
//	}
//	else if (element == NURBSSURFACE_MODEL)
//	{
//		//ModelNurbsSurface* nurbs = static_cast<ModelNurbsSurface*>(surface);
//
//	}
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pFace));
//
//	return r_pFace;
//}
//
//double StructedStepFileParser::CalculateHeight(Loop* aLoop)
//{
//	double aHeight = 0.0;
//	Loop* ptr_loop = aLoop;
//	bool flag_loop = false;
//	while (ptr_loop)
//	{
//		if (aLoop == ptr_loop && flag_loop == true)
//		{
//			break;
//		}
//
//		Coedge* head_coedge = ptr_loop->GetStart();
//		Coedge* ptr_coedge = head_coedge;
//
//		bool flag_coedge = false;
//		while (ptr_coedge)
//		{
//			if (head_coedge == ptr_coedge && flag_coedge == true)
//			{
//				break;
//			}
//			ConsCurve* curve = ptr_coedge->GetEdge()->GetGeometry()->GetGeometry();
//
//			if (curve->GetType() == LINE_CONS)
//			{
//				double aLineHeight = curve->GetInterval().GetMaxIntervalValue();
//				aHeight = aHeight > aLineHeight ? aHeight : aLineHeight;
//			}
//			flag_coedge = true;
//			ptr_coedge = ptr_coedge->GetNext();
//		}
//		flag_loop = true;
//		ptr_loop = ptr_loop->GetNext();
//	}
//
//	return aHeight;
//}
//
///*
//	StepFaceBound类型对象转换成FaceBound类型对象的总入口
//*/
//Loop* StructedStepFileParser::ParseFaceBound(size_t p_Index)
//{
//	Loop* r_pFaceBound = nullptr;
//
//	const String& itemType = m_File[p_Index].m_StepType;
//	if (itemType == _T("FACE_OUTER_BOUND"))
//	{
//		r_pFaceBound = ParseFaceOuterBound(p_Index);
//	}
//
//	if (itemType == _T("FACE_BOUND"))
//	{
//		r_pFaceBound = ParseFaceInnerBound(p_Index);
//	}
//
//	assert(r_pFaceBound != nullptr);
//	return r_pFaceBound;
//}
//
//Loop* StructedStepFileParser::ParseFaceInnerBound(size_t p_Index)
//{
//	//FaceBound  STEP文件文本实例
//	//#15=FACE_BOUND('',#26,.T.)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析 p_Value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(3 == splits.size());
//
//	//开始根据 value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 p_bound  StepLoop*-->Loop*
//	Loop* p_bound = nullptr;
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	p_bound = GetParsed<Loop>(p_Index);
//	//若已经转换
//	if (p_bound != nullptr) {
//		return p_bound;
//	}
//
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepLoop*转换成Loop*
//		p_bound = ParseLoop(id - 1);
//		assert(nullptr != p_bound);
//	}
//
//	//参数 3 orientation
//	bool orientation = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[2][1])
//		{
//		case 'T':
//			orientation = true;
//			break;
//		case 'F':
//			orientation = false;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, p_bound));
//	return p_bound;
//}
//
//Loop* StructedStepFileParser::ParseFaceOuterBound(size_t p_Index)
//{
//	//FaceOuterBound  STEP文件文本实例
//	//#58 = FACE_OUTER_BOUND ( 'NONE', #49, .T. )
//	//StepFaceOuterBound(String name, StepLoop*, bool orientation)-- > FaceOuterBound(Loop*, bool orientation)
//
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析 p_Value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(3 == splits.size());
//
//	//开始根据 value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 p_bound  StepLoop*-->Loop*
//	Loop* p_bound = nullptr;
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	p_bound = GetParsed<Loop>(p_Index);
//	//若已经转换
//	if (p_bound != nullptr) {
//		return p_bound;
//	}
//
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepLoop*转换成Loop*
//		p_bound = ParseLoop(id - 1);
//		assert(nullptr != p_bound);
//	}
//	//参数 3 orientation
//	bool orientation = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[2][1])
//		{
//		case 'T':
//			orientation = true;
//			break;
//		case 'F':
//			orientation = false;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, p_bound));
//	return p_bound;
//}
//
///*
//	StepLoop类型对象转换成Loop类型对象的入口
//*/
//Loop* StructedStepFileParser::ParseLoop(size_t p_Index)
//{
//	Loop* r_pLoop = nullptr;
//
//	const String& itemType = m_File[p_Index].m_StepType;
//	if (itemType == _T("EDGE_LOOP"))
//	{
//		r_pLoop = ParseEdgeLoop(p_Index);
//	}
//
//	assert(r_pLoop != nullptr);
//	return r_pLoop;
//}
//
//Loop* StructedStepFileParser::ParseEdgeLoop(size_t p_Index)
//{
//	Loop* r_pEdgeLoop = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pEdgeLoop = GetParsed<Loop>(p_Index);
//	//若已经转换
//	if (r_pEdgeLoop != nullptr) {
//		return r_pEdgeLoop;
//	}
//
//	r_pEdgeLoop = new Loop();
//
//	//EdgeLoop  STEP文件文本实例
//	//#106 = EDGE_LOOP ( 'NONE', ( #29, #20 ) ) ;
//	//StepEdgeLoop(name,vector<StepOrientedEdge*>)-->EdgeLoop(vector<OrientedEdge*>)
//
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析 value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(2 == splits.size());
//
//	//开始根据 value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2  p_edgeList vector<StepOrientedEdge*> -->vector<OrientedEdge*> 
//	bool flag = false;
//	Coedge* p_EdgeList = nullptr;
//	Coedge* ans = nullptr;
//	{
//		String& edgeListArrayObject = splits[1];
//		assert(edgeListArrayObject.size() > 2);
//
//		String edgeListArray = edgeListArrayObject.substr(1, edgeListArrayObject.size() - 2);
//		vector<String> edgeIds = SplitBy(edgeListArray, _T("$"));
//		assert(0 < edgeIds.size());
//
//		for (const auto& bid : edgeIds)
//		{
//			size_t id = stoul(bid);
//			assert(id > 0);
//
//			if (!flag) {
//				//将对应id-1位置的StepOrientedEdge*转换成OrientedEdge*
//				p_EdgeList = ParseOrientedEdge(id - 1);
//				assert(nullptr != p_EdgeList);
//				p_EdgeList->SetOwner(r_pEdgeLoop);
//				ans = p_EdgeList;
//				flag = true;
//			}
//			else {
//				Coedge* temp = ParseOrientedEdge(id - 1);
//				assert(nullptr != temp);
//				ans->SetNext(temp);
//				temp->SetPrevious(ans);
//				temp->SetOwner(r_pEdgeLoop);
//				ans = temp;
//			}
//		}
//	}
//	ans->SetNext(p_EdgeList);
//	p_EdgeList->SetPrevious(ans);
//
//	r_pEdgeLoop->SetStart(p_EdgeList);
//	//r_pEdgeLoop->SetRevFlag(p_EdgeList->GetSense());
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pEdgeLoop));
//
//	return r_pEdgeLoop;
//}
//
//Coedge* StructedStepFileParser::ParseOrientedEdge(size_t p_Index)
//{
//	Coedge* r_pOrientedEdge = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pOrientedEdge = GetParsed<Coedge>(p_Index);
//	//若已经转换
//	if (r_pOrientedEdge != nullptr) {
//		return r_pOrientedEdge;
//	}
//
//	//r_pOrientedEdge = new Coedge(nullptr, true, nullptr, nullptr);
//	r_pOrientedEdge = new Coedge();
//
//	//OrientedEdge  STEP文件文本实例
//	//#20 = ORIENTED_EDGE ( 'NONE', *, *, #92, .T. ) ;
//	//StepOrientedEdge(name,StepVertex*,StepVertex*,StepEdge*,bool)-->OriectedEdge(Edge*,bool)
//
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	// 解析value 
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(5 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	////参数2 p_edgeStart
//	//Vertex* p_edgeStart = nullptr;
//	//{
//	//	size_t id = stoul(splits[1]);
//	//	assert(id > 0);
//	//	assert(m_StepItems.size() >= id);
//	//	p_edgeStart = reinterpret_cast<Vertex*>(m_StepItems[id - 1]);
//	//	assert(nullptr != p_edgeStart);
//	//}
//
//	////参数3 p_edgeEnd
//	//Vertex* p_edgeEnd = nullptr;
//	//{
//	//	size_t id = stoul(splits[2]);
//	//	assert(id > 0);
//	//	assert(m_StepItems.size() >= id);
//	//	p_edgeEnd = reinterpret_cast<Vertex*>(m_StepItems[id - 1]);
//	//	assert(nullptr != p_edgeEnd);
//	//}
//
//	//参数 4 p_edgeElement  StepEdge*-->Edge*
//	bool isCreated = false;
//	Edge* p_EdgeElement = nullptr;
//	{
//		size_t id = stoul(splits[3]);
//		assert(id > 0);
//		//将对应id-1位置的Edge对象进行转换并将地址赋给当前对象属性
//		//////判断边是否建立，如果建立了
//		p_EdgeElement = GetParsed<Edge>(id - 1);
//		if (p_EdgeElement) {//已存在
//			isCreated = true;
//		}
//		else {
//			p_EdgeElement = ParseEdge(id - 1);
//			p_EdgeElement->SetCoedge(r_pOrientedEdge);
//		}
//
//		assert(nullptr != p_EdgeElement);
//	}
//
//	//参数 5 orientation
//	bool orientation = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[4][1])
//		{
//		case 'T':
//			orientation = true;
//			break;
//		case 'F':
//			orientation = false;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	r_pOrientedEdge->SetEdge(p_EdgeElement);
//	r_pOrientedEdge->SetSense(orientation);
//
//	if (isCreated) {
//		//说明edge存在，另一条coedge已存在
//		//先获取零一条coedge，建立双向关系
//		Coedge* b_coedge = dynamic_cast<Coedge*>(p_EdgeElement->GetCoedge());
//		r_pOrientedEdge->SetPartner(b_coedge);
//		b_coedge->SetPartner(r_pOrientedEdge);
//	}
//	else {
//		p_EdgeElement->SetCoedge(r_pOrientedEdge);
//	}
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pOrientedEdge));
//
//	return r_pOrientedEdge;
//
//}
//
///*
//	StepEdge类型对象转换成Edge类型对象的入口
//*/
//Edge* StructedStepFileParser::ParseEdge(size_t p_Index)
//{
//	Edge* r_pEdge = nullptr;
//
//	const String& itemType = m_File[p_Index].m_StepType;
//	if (itemType == _T("EDGE_CURVE"))
//	{
//		r_pEdge = ParseEdgeCurve(p_Index);
//	}
//
//	assert(r_pEdge != nullptr);
//	return r_pEdge;
//}
//
//Edge* StructedStepFileParser::ParseEdgeCurve(size_t p_Index)
//{
//	Edge* r_pEdgeCurve = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pEdgeCurve = GetParsed<Edge>(p_Index);
//	//若已经转换
//	if (r_pEdgeCurve != nullptr) {
//		return r_pEdgeCurve;
//	}
//
//	r_pEdgeCurve = new Edge();
//
//	//EdgeCurve  STEP文件文本实例
//	//#92 = EDGE_CURVE ( 'NONE', #119, #115, #21, .T. ) ;
//	//StepEdgeCurve(name,StepVertex*,StepVertex*,StepCurve*, bool sameSense)
//	//-- > EdgeCurve(Vertex*, Vertex*, Curve3d*, bool sameSense);
//
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	// 解析value 
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(5 == splits.size());
//	//开始根据 value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 p_edgeStart StepVertex*-->Vertex*
//	Vertex* p_edgeStart = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepVertex*转换成Vertex*
//		p_edgeStart = ParseVertex(id - 1);
//		p_edgeStart->SetEdge(r_pEdgeCurve);
//		assert(nullptr != p_edgeStart);
//	}
//
//	//参数 3 p_edgeEnd  StepVertex*-->Vertex*
//	Vertex* p_edgeEnd = nullptr;
//	{
//		size_t id = stoul(splits[2]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepVertex*转换成Vertex*
//		p_edgeEnd = ParseVertex(id - 1);
//		p_edgeEnd->SetEdge(r_pEdgeCurve);
//		assert(nullptr != p_edgeEnd);
//	}
//
//	//参数 4  p_edgeGeometry  StepCurve*-->Curve3d*
//	ModelCurve* p_edgeGeometry = nullptr;
//	{
//		size_t id = stoul(splits[3]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepCurve对象转换成Curve3d*
//		p_edgeGeometry = ParseCurve(id - 1);
//		assert(nullptr != p_edgeGeometry);
//	}
//
//	//参数 5 sameSense
//	bool sameSense = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[4][1])
//		{
//		case 'T':
//			sameSense = true;
//			break;
//		case 'F':
//			sameSense = false;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//
//	if (sameSense == false)
//	{
//		size_t element = p_edgeGeometry->GetType();
//		if (element == LINE_MODEL)
//		{
//			ModelLine* modelcurve = static_cast<ModelLine*>(p_edgeGeometry);
//			ConsLine consline = *modelcurve->GetGeometry();
//			consline.SetDirection(Vector3D(-consline.GetDirection().GetX(), -consline.GetDirection().GetY(), -consline.GetDirection().GetZ()));
//		}
//		else if (element == CIRCLECURVE_MODEL)
//		{
//			ModelCircle* modelcurve = static_cast<ModelCircle*>(p_edgeGeometry);
//			ConsCircle conscircle = *modelcurve->GetGeometry();
//			conscircle.SetEntityUcs(EntityUcs(conscircle.GetEntityUcs().GetOrigin(), conscircle.GetEntityUcs().GetUcsY(), -conscircle.GetEntityUcs().GetUcsZ()));
//		}
//		else if (element == ELLIPSECURVE_MODEL)
//		{
//			ModelEllipse* modelcurve = static_cast<ModelEllipse*>(p_edgeGeometry);
//			ConsEllipse consellipse = *modelcurve->GetGeometry();
//			consellipse.SetEntityUcs(EntityUcs(consellipse.GetEntityUcs().GetOrigin(), consellipse.GetEntityUcs().GetUcsY(), -consellipse.GetEntityUcs().GetUcsZ()));
//		}
//		else if (element == BSPLINECURVE_MODEL)
//		{
//			/*
//			ModelBsplineCurve* modelcurve = static_cast<ModelBsplineCurve*>(p_edgeGeometry);
//			ConsBsplineCurve consbspline = modelcurve->GetGeometry();
//
//			//反转controlpoint
//			int nums = consbspline.GetControlPoint().size();
//			for (int i = 0; i < nums / 2; ++i)
//			{
//				Point3D temp = consbspline.GetControlPoint()[i];
//				consbspline.GetControlPoint()[i] = consbspline.GetControlPoint()[nums - i - 1];
//				consbspline.GetControlPoint()[nums - i - 1] = temp;
//			}
//
//			//反转knots
//			nums = consbspline.GetKnots().size();
//			for (int i = 0; i < nums / 2; ++i)
//			{
//				double temp = consbspline.GetKnots()[i];
//				consbspline.GetKnots()[i] = consbspline.GetKnots()[nums - i - 1];
//				consbspline.GetKnots()[nums - i - 1] = temp;
//			}
//			*/
//		}
//		else if (element == NURBSCURVE_MODEL)
//		{
//			/*
//			ModelNurbsCurve* modelcurve = static_cast<ModelNurbsCurve*>(p_edgeGeometry);
//			ConsNurbsCurve consnurbs = modelcurve->GetGeometry();
//
//			//反转controlpoint
//			int nums = consnurbs.GetControlPoint().size();
//			for (int i = 0; i < nums / 2; ++i)
//			{
//				Point3D temp = consnurbs.GetControlPoint()[i];
//				consnurbs.GetControlPoint()[i] = consnurbs.GetControlPoint()[nums - i - 1];
//				consnurbs.GetControlPoint()[nums - i - 1] = temp;
//			}
//
//			//反转knots
//			nums = consnurbs.GetKnots().size();
//			for (int i = 0; i < nums / 2; ++i)
//			{
//				real temp = consnurbs.GetKnots()[i];
//				consnurbs.GetKnots()[i] = consnurbs.GetKnots()[nums - i - 1];
//				consnurbs.GetKnots()[nums - i - 1] = temp;
//			}
//
//			//反转weights
//			nums = consnurbs.GetWeight().size();
//			for (int i = 0; i < nums / 2; ++i)
//			{
//				double temp = consnurbs.GetWeight()[i];
//				consnurbs.GetWeight()[i] = consnurbs.GetWeight()[nums - i - 1];
//				consnurbs.GetWeight()[nums - i - 1] = temp;
//			}
//			*/
//		}
//		r_pEdgeCurve->SetStart(p_edgeEnd);
//		r_pEdgeCurve->SetEnd(p_edgeStart);
//		r_pEdgeCurve->SetGeometry(p_edgeGeometry);
//	}
//	else
//	{
//		r_pEdgeCurve->SetStart(p_edgeStart);
//		r_pEdgeCurve->SetEnd(p_edgeEnd);
//		r_pEdgeCurve->SetGeometry(p_edgeGeometry);
//	}
//
//	//r_pEdgeCurve->SetStart(p_edgeStart);
//	//r_pEdgeCurve->SetEnd(p_edgeEnd);
//	//r_pEdgeCurve->SetGeometry(p_edgeGeometry);
//
//	ModelCurve* curve = r_pEdgeCurve->GetGeometry();
//	uint16_t element = curve->GetType();
//
//	if (element == LINE_MODEL)
//	{
//		ModelLine* line = static_cast<ModelLine*>(curve);
//		Vertex* b_vertex_start = dynamic_cast<Vertex*>(r_pEdgeCurve->GetStart());
//		Vertex* b_vertex_end = dynamic_cast<Vertex*>(r_pEdgeCurve->GetEnd());
//		line->GetGeometry()->SetPos(b_vertex_start->GetGeometry()->GetPoint());
//		// 设置Interval
//		double minInterval = 0;
//		double maxInterval = line->GetGeometry()->GetPointPara(b_vertex_end->GetGeometry()->GetPoint());
//		line->GetGeometry()->SetMinIntervalValue(minInterval);
//		line->GetGeometry()->SetMaxIntervalValue(maxInterval);
//	}
//	else if (element == CIRCLECURVE_MODEL)
//	{
//		ModelCircle* circle = static_cast<ModelCircle*>(curve);
//		// 设置Interval
//		Vertex* b_vertex_start = dynamic_cast<Vertex*>(r_pEdgeCurve->GetStart());
//		Vertex* b_vertex_end = dynamic_cast<Vertex*>(r_pEdgeCurve->GetEnd());
//		double minInterval = circle->GetGeometry()->GetPointPara(b_vertex_start->GetGeometry()->GetPoint());
//		double maxInterval = circle->GetGeometry()->GetPointPara(b_vertex_end->GetGeometry()->GetPoint());
//		if (maxInterval < minInterval)
//		{
//			maxInterval += 1.0;
//		}
//		circle->GetGeometry()->SetMinIntervalValue(minInterval);
//		circle->GetGeometry()->SetMaxIntervalValue(maxInterval);
//	}
//	else if (element == ELLIPSECURVE_MODEL)
//	{
//		ModelEllipse* ellipse = static_cast<ModelEllipse*>(curve);
//		// 设置Interval
//		Vertex* b_vertex_start = dynamic_cast<Vertex*>(r_pEdgeCurve->GetStart());
//		Vertex* c = dynamic_cast<Vertex*>(r_pEdgeCurve->GetEnd());
//		double minInterval = ellipse->GetGeometry()->GetPointPara(b_vertex_start->GetGeometry()->GetPoint());
//		double maxInterval = ellipse->GetGeometry()->GetPointPara(b_vertex_start->GetGeometry()->GetPoint());
//		if (maxInterval < minInterval)
//		{
//			maxInterval += 1.0;
//		}
//		ellipse->GetGeometry()->SetMinIntervalValue(minInterval);
//		ellipse->GetGeometry()->SetMaxIntervalValue(maxInterval);
//	}
//	else if (element == BSPLINECURVE_MODEL)
//	{
//		ModelBsplineCurve* bspline = static_cast<ModelBsplineCurve*>(curve);
//		// 设置Interval
//	}
//	else if (element == NURBSCURVE_MODEL)
//	{
//		ModelNurbsCurve* nurbs = static_cast<ModelNurbsCurve*>(curve);
//		// 设置Interval
//	}
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pEdgeCurve));
//
//	return r_pEdgeCurve;
//}
//
///*
//	StepVertex对象转换成Vertex对象的入口
//*/
//Vertex* StructedStepFileParser::ParseVertex(size_t p_Index)
//{
//	Vertex* r_pVertex = nullptr;
//
//	const String& itemType = m_File[p_Index].m_StepType;
//	if (itemType == _T("VERTEX_POINT"))
//	{
//		r_pVertex = ParseVertexPoint(p_Index);
//	}
//
//	assert(r_pVertex != nullptr);
//	return r_pVertex;
//}
//
///*
//
//
//*/
//Vertex* StructedStepFileParser::ParseVertexPoint(size_t p_Index)
//{
//	Vertex* r_pVertexPoint = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pVertexPoint = GetParsed<Vertex>(p_Index);
//	//若已经转换
//	if (r_pVertexPoint != nullptr) {
//		return r_pVertexPoint;
//	}
//
//	//VertexPoint  STEP文件文本实例
//	//#82 = VERTEX_POINT ( 'NONE', #78 ) -->（name，StepPoint*）
//	//StepVertexPoint(StepPoinr*)-->VertexPoint( Point3d* )
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	// 解析value 
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(2 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 StepPoint(StepCartesianPoint)*--->Point3d*
//	ModelPoint* pVertexGeometry = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		pVertexGeometry = ParseCartesianPoint(id - 1);
//		assert(nullptr != pVertexGeometry);
//	}
//
//	r_pVertexPoint = new Vertex(nullptr, pVertexGeometry);
//	assert(r_pVertexPoint != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pVertexPoint));
//
//	return r_pVertexPoint;
//}
//
//ModelPoint* StructedStepFileParser::ParseCartesianPoint(size_t p_Index)
//{
//	ModelPoint* r_pPoint3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pPoint3d = GetParsed<ModelPoint>(p_Index);
//	//若已经转换
//	if (r_pPoint3d != nullptr) {
//		return r_pPoint3d;
//	}
//
//	//CartesianPoint  STEP文件文本实例
//	//#30 = CARTESIAN_POINT ( 'NONE',  ( 0.0, 10.0, 0.0 ) )
//	//StepCartesianPoint(name,vector<double> m_Coordinates)--->Point3d(real x,real y,real z)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(2 == splits.size());
//
//	//参数1 name
//	const String& name = splits[0];
//
//	//参数2  vector<double> m_Coordinates--->(real x,real y,real z)
//	String& CoordsArrayObject = splits[1];
//	String CoordsArray = CoordsArrayObject.substr(1, CoordsArrayObject.size() - 2);
//	vector<String> coordsId = SplitBy(CoordsArray, _T("$"));
//	assert(0 < coordsId.size());
//	real px, py, pz;
//	{
//		px = stod(coordsId[0]);
//		py = stod(coordsId[1]);
//		pz = stod(coordsId[2]);
//	}
//
//	Point3D* point = new Point3D(px, py, pz);
//	ConsPoint* conspoint = new ConsPoint(*point);
//	r_pPoint3d = new ModelPoint(conspoint);
//	assert(r_pPoint3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pPoint3d));
//
//	return r_pPoint3d;
//
//}
//
///*
//	StepCurve类型对象转换为Curve3d类型对象的入口
//*/
//ModelCurve* StructedStepFileParser::ParseCurve(size_t p_Index)
//{
//	ModelCurve* r_pCurve3d = nullptr;
//
//	const String& itemType = m_File[p_Index].m_StepType;
//
//	if (itemType == _T("LINE"))
//	{
//		r_pCurve3d = ParseLine(p_Index);
//	}
//	if (itemType == _T("CIRCLE"))
//	{
//		r_pCurve3d = ParseCircle(p_Index);
//	}
//	if (itemType == _T("ELLIPSE"))
//	{
//		r_pCurve3d = ParseEllipse(p_Index);
//	}
//	if (itemType == _T("B_SPLINE_CURVE_WITH_KNOTS"))
//	{
//		r_pCurve3d = ParseBsplineCurve(p_Index);
//	}
//	if (itemType == _T("RATIONAL_B_SPLINE_CURVE"))
//	{
//		r_pCurve3d = ParseNurbsCurve(p_Index);
//	}
//
//	assert(r_pCurve3d != nullptr);
//	return r_pCurve3d;
//}
//
//ModelLine* StructedStepFileParser::ParseLine(size_t p_Index)
//{
//	ModelLine* r_pLine3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pLine3d = GetParsed<ModelLine>(p_Index);
//	//若已经转换
//	if (r_pLine3d != nullptr) {
//		return r_pLine3d;
//	}
//
//	//Line  STEP文件文本实例
//	//#211 = LINE ( 'NONE', #286, #374 ) ;
//	//StepLine(name,StepCartesianPoint*,StepVector*) --> Line(Point3d& ,Direction3d&)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(3 == splits.size());
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 StepCartesianPoint*-->Point3d
//	ModelPoint* p_Pnt;
//	Point3D origin;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//		p_Pnt = ParseCartesianPoint(id - 1);
//		assert(nullptr != p_Pnt);
//		origin = p_Pnt->GetPoint();
//	}
//
//	//参数 3 StepVector*-->Vector3d*-->Direction3d
//	Vector3D* p_Dir = nullptr;
//	{
//		size_t id = stoul(splits[2]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepVector对象转换成Vector3d*
//		p_Dir = ParseVector(id - 1);
//		assert(nullptr != p_Dir);
//	}
//
//	Vector3D direction = Vector3D(*p_Dir);
//
//	ConsLine* consline = new ConsLine(origin, direction);
//	r_pLine3d = new ModelLine(consline);
//	assert(r_pLine3d != nullptr);
//
//	//将new的Point3d对象,Vector3d对象删除
//
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pLine3d));
//
//	return r_pLine3d;
//}
//
//ModelCircle* StructedStepFileParser::ParseCircle(size_t p_Index)
//{
//	ModelCircle* r_pCircle3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pCircle3d = GetParsed<ModelCircle>(p_Index);
//	//若已经转换
//	if (r_pCircle3d != nullptr) {
//		return r_pCircle3d;
//	}
//
//	//Circle  STEP文件文本实例
//	//#9 = CIRCLE('NONE', #25, 10.0)
//	//StepCircle(name,StepAxis2Placement3d*,double)--->Circle3d( CartesianCoordinate3d , real )
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(3 == splits.size());
//
//	//参数1 name
//	const String& name = splits[0];
//
//	//参数2 StepAxis2placement3d*-->Axis2placement3d*-->CartesianCoordinate3d
//	EntityUcs* pPoistion = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepAxis2Placement3d*进行转换成Axis2Placement3d*
//		pPoistion = ParseAxis2Placement3d(id - 1);
//		assert(nullptr != pPoistion);
//	}
//
//	//将Axis2Placement3d* 手动删除
//	//delete pPoistion;
//
//	//参数3 radius
//	real radius = 0.0;
//	{
//		String& radiusObject = splits[2];
//		radius = stod(radiusObject);
//		assert(radius > 0);
//	}
//
//	ConsCircle* conscircle = new ConsCircle(*pPoistion, radius);
//	r_pCircle3d = new ModelCircle(conscircle);
//	assert(r_pCircle3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pCircle3d));
//
//	return r_pCircle3d;
//}
//
//ModelEllipse* StructedStepFileParser::ParseEllipse(size_t p_Index)
//{
//	ModelEllipse* r_pEllipse3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pEllipse3d = GetParsed<ModelEllipse>(p_Index);
//	//若已经转换
//	if (r_pEllipse3d != nullptr) {
//		return r_pEllipse3d;
//	}
//
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(4 == splits.size());
//
//	//参数1 name
//	const String& name = splits[0];
//
//	//参数2 StepAxis2placement3d*-->Axis2placement3d*-->CartesianCoordinate3d
//	EntityUcs* pPoistion = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepAxis2Placement3d*进行转换成Axis2Placement3d*
//		pPoistion = ParseAxis2Placement3d(id - 1);
//		assert(nullptr != pPoistion);
//	}
//
//	//将Axis2Placement3d* 手动删除
//	//delete pPoistion;
//
//	//参数3 m_semiAxis1
//	real semiAxis1 = 0.0;
//	{
//		String& semiAxis1Object = splits[2];
//		semiAxis1 = stod(semiAxis1Object);
//		assert(semiAxis1 > 0);
//	}
//	//参数4 m_semiAxis2
//	real semiAxis2 = 0.0;
//	{
//		String& semiAxis2Object = splits[3];
//		semiAxis2 = stod(semiAxis2Object);
//		assert(semiAxis2 > 0);
//	}
//
//	ConsEllipse* consellipse = new ConsEllipse(*pPoistion, semiAxis1, semiAxis2);
//	r_pEllipse3d = new ModelEllipse(consellipse);
//	assert(r_pEllipse3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pEllipse3d));
//
//	return r_pEllipse3d;
//}
//
//ModelBsplineCurve* StructedStepFileParser::ParseBsplineCurve(size_t p_Index)
//{
//
//	//#165=B_SPLINE_CURVE_WITH_KNOTS('',5,(#166,#167,#168,#169,#170,#171,#172,#173,#174),.UNSPECIFIED.,.F.,.U.,(6,3,6),(0.,39.2699081699,78.5398163397),.UNSPECIFIED.) ;
//	ModelBsplineCurve* r_pB_SplineCurve3d = nullptr;
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pB_SplineCurve3d = GetParsed<ModelBsplineCurve>(p_Index);
//	//若已经转换
//	if (r_pB_SplineCurve3d != nullptr) {
//		return r_pB_SplineCurve3d;
//	}
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(9 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 degree
//	ushort p_Degree;
//	{
//		ushort id = stoul(splits[1]);
//		assert(id > 0);
//		p_Degree = id;
//	}
//
//	//参数 3 ControlPointsList 控制点
//	vector<Point3D> p_ControlPointsList;
//	{
//		String& ControlPointListArrayObject = splits[2];
//		assert(ControlPointListArrayObject.size() > 2);
//
//		String ControlPointListArray = ControlPointListArrayObject.substr(1, ControlPointListArrayObject.size() - 2);
//		vector<String> PointIds = SplitBy(ControlPointListArray, _T("$"));
//		assert(0 < PointIds.size());
//
//		for (const auto& bid : PointIds)
//		{
//			size_t id = stoul(bid);
//			assert(id > 0);
//
//			//将对应id-1位置的StepCartesianPoint*转换成Point3d*
//			ModelPoint* pointPtr = ParseCartesianPoint(id - 1);
//			assert(nullptr != pointPtr);
//			Point3D point = pointPtr->GetPoint();
//			p_ControlPointsList.push_back(point);
//		}
//	}
//
//	//参数4 Curveform
//	const String& Curveform = splits[3];
//
//	//参数5 ClosedCurve
//	bool ClosedCurve = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[4][1])
//		{
//		case 'T':
//			ClosedCurve = true;
//			break;
//		case 'F':
//			ClosedCurve = false;
//			break;
//		case 'U':
//			ClosedCurve = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	//参数6 SelfIntersect
//	bool SelfIntersect = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[5][1])
//		{
//		case 'T':
//			SelfIntersect = true;
//			break;
//		case 'F':
//			SelfIntersect = false;
//			break;
//		case 'U':
//			SelfIntersect = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	//参数7 Multiplicities
//	vector<ushort> p_Multiplicities;
//	String& MultiplicitiesArrayObject = splits[6];
//	assert(MultiplicitiesArrayObject.size() > 2);
//	String MultiplicitiesArray = MultiplicitiesArrayObject.substr(1, MultiplicitiesArrayObject.size() - 2);
//	vector<String> MultiplicitiesIds = SplitBy(MultiplicitiesArray, _T("$"));
//	assert(0 < MultiplicitiesIds.size());
//	for (const auto& bid : MultiplicitiesIds)
//	{
//		size_t id = stoul(bid);
//		assert(id > 0);
//		p_Multiplicities.push_back(id);
//	}
//
//	//参数8 Knots
//	vector<real> p_Knots;
//	String& KnotsArrayObject = splits[7];
//	assert(KnotsArrayObject.size() > 2);
//	String KnotsArray = KnotsArrayObject.substr(1, KnotsArrayObject.size() - 2);
//	vector<String> KnotsIds = SplitBy(KnotsArray, _T("$"));
//	assert(0 < KnotsIds.size());
//	int p_Multindex = 0;
//	for (const auto& bid : KnotsIds)
//	{
//		double id = stod(bid);
//		//assert(id >= 0);
//		for (auto i = 0; i < p_Multiplicities[p_Multindex]; ++i)
//		{
//			p_Knots.push_back(id);
//		}
//		++p_Multindex;
//	}
//
//	//参数9 KnotType
//	const String& KnotType = splits[8];
//
//	ConsBsplineCurve* consbspline = new ConsBsplineCurve(p_ControlPointsList, p_Knots, p_Degree);
//	r_pB_SplineCurve3d = new ModelBsplineCurve(consbspline);
//	assert(r_pB_SplineCurve3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pB_SplineCurve3d));
//
//	return r_pB_SplineCurve3d;
//}
//
//ModelNurbsCurve* StructedStepFileParser::ParseNurbsCurve(size_t p_Index)
//{
//	ModelNurbsCurve* r_pRational_B_SplineCurve3d = nullptr;
//	//判断是否已经解析过数组中对应p_Index位置的对象
//
//	r_pRational_B_SplineCurve3d = GetParsed<ModelNurbsCurve>(p_Index);
//	//若已经转换
//	if (r_pRational_B_SplineCurve3d != nullptr) {
//		return r_pRational_B_SplineCurve3d;
//	}
//
//
//
//	//获取属性字符串,与其他类型的属性字符串不太一样
//	//3$(58743%123242%93817%29258)$.UNSPECIFIED.$.F.$.T.,(4%4)$(4.814598377544293800%4.930555136883981500)$.UNSPECIFIED.,(1.000000000000000000%0.9988798163404487600%0.9988798163404487600%1.000000000000000000)
//	//value = L"2$(5%6%7)$.UNSPECIFIED.$.F.$.U.,(3%3)$(0.%1.5707963267949)$.UNSPECIFIED.,(1.%0.707106781186548%1.)"
//	const String& value = m_File[p_Index].m_Value;
//
//	//参数 1 name
//	const String name = _T("null");
//
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(3 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//第一部分参数：B_SPLINE_CURVE的参数
//	const String& para_B_SplineCurve = splits[0];
//
//	vector<String> splits_part1 = SplitBy(para_B_SplineCurve, _T("$"));
//	assert(5 == splits_part1.size());
//
//
//	//参数 1 degree
//	ushort p_Degree;
//	{
//		ushort id = stoul(splits_part1[0]);
//		assert(id > 0);
//		p_Degree = id;
//	}
//
//	//参数 2 ControlPointsList 控制点
//	vector<Point3D> p_ControlPointsList;
//	{
//		String& ControlPointListArrayObject = splits_part1[1];
//		assert(ControlPointListArrayObject.size() > 2);
//
//		String ControlPointListArray = ControlPointListArrayObject.substr(1, ControlPointListArrayObject.size() - 2);
//		vector<String> PointIds = SplitBy(ControlPointListArray, _T("%"));
//		assert(0 < PointIds.size());
//
//		for (const auto& bid : PointIds)
//		{
//			size_t id = stoul(bid);
//			assert(id > 0);
//
//			//将对应id-1位置的StepCartesianPoint*转换成Point3d*
//			ModelPoint* pointPtr = ParseCartesianPoint(id - 1);
//			assert(nullptr != pointPtr);
//			Point3D point = pointPtr->GetPoint();
//			p_ControlPointsList.push_back(point);
//		}
//	}
//
//	//参数3 Curveform
//	const String& Curveform = splits_part1[2];
//
//	//参数4 ClosedCurve
//	bool ClosedCurve = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits_part1[3][1])
//		{
//		case 'T':
//			ClosedCurve = true;
//			break;
//		case 'F':
//			ClosedCurve = false;
//			break;
//		case 'U':
//			ClosedCurve = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	//参数5 SelfIntersect
//	bool SelfIntersect = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits_part1[4][1])
//		{
//		case 'T':
//			SelfIntersect = true;
//			break;
//		case 'F':
//			SelfIntersect = false;
//			break;
//		case 'U':
//			SelfIntersect = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	//第二部分：B_SPLINE_CURVE_WITH_KNOTS的属性值
//	const String& para_B_SPLINE_CURVE_WITH_KNOTS = splits[1];
//	vector<String> splits_part2 = SplitBy(para_B_SPLINE_CURVE_WITH_KNOTS, _T("$"));
//	assert(3 == splits_part2.size());
//
//	//参数1 Multiplicities
//	vector<ushort> p_Multiplicities;
//	String& MultiplicitiesArrayObject = splits_part2[0];
//	assert(MultiplicitiesArrayObject.size() > 2);
//	String MultiplicitiesArray = MultiplicitiesArrayObject.substr(1, MultiplicitiesArrayObject.size() - 2);
//	vector<String> MultiplicitiesIds = SplitBy(MultiplicitiesArray, _T("%"));
//	assert(0 < MultiplicitiesIds.size());
//	for (const auto& bid : MultiplicitiesIds)
//	{
//		size_t id = stoul(bid);
//		assert(id > 0);
//		p_Multiplicities.push_back(id);
//	}
//
//	//参数2 Knots
//	vector<real> p_Knots;
//	String& KnotsArrayObject = splits_part2[1];
//	assert(KnotsArrayObject.size() > 2);
//	String KnotsArray = KnotsArrayObject.substr(1, KnotsArrayObject.size() - 2);
//	vector<String> KnotsIds = SplitBy(KnotsArray, _T("%"));
//	assert(0 < KnotsIds.size());
//	int p_Multindex = 0;
//	for (const auto& bid : KnotsIds)
//	{
//		double id = stod(bid);
//		assert(id >= 0);
//		for (auto i = 0; i < p_Multiplicities[p_Multindex]; ++i)
//		{
//			p_Knots.push_back(id);
//		}
//		++p_Multindex;
//	}
//
//	//参数3 KnotType
//	const String& KnotType = splits_part2[2];
//
//
//	//第三部分：RATIONAL_B_SPLINE_CURVE的属性值
//
//	//参数1 Weights
//	vector<real> p_Weights;
//	String& WeightsArrayObject = splits[2];
//	assert(WeightsArrayObject.size() > 2);
//	String WeightsArray = WeightsArrayObject.substr(1, WeightsArrayObject.size() - 2);
//	vector<String> WeightsIds = SplitBy(WeightsArray, _T("%"));
//	assert(0 < WeightsIds.size());
//	for (const auto& bid : WeightsIds)
//	{
//		size_t id = stod(bid);
//		assert(id >= 0);
//		p_Weights.push_back(id);
//	}
//
//	ConsNurbsCurve* consnurbs = new ConsNurbsCurve(p_ControlPointsList, p_Knots, p_Degree, p_Weights);
//	r_pRational_B_SplineCurve3d = new ModelNurbsCurve(consnurbs);
//	assert(r_pRational_B_SplineCurve3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pRational_B_SplineCurve3d));
//
//	return r_pRational_B_SplineCurve3d;
//
//}
//
///*
//	StepSurface类型对象转换为Surface3d*类型的对象
//*/
//ModelSurface* StructedStepFileParser::ParseSurface(size_t p_Index)
//{
//	ModelSurface* r_pSurface3d = nullptr;
//
//	const String& itemType = m_File[p_Index].m_StepType;
//	if (itemType == _T("PLANE"))
//	{
//		r_pSurface3d = ParsePlane(p_Index);
//	}
//	if (itemType == _T("CYLINDRICAL_SURFACE"))
//	{
//		r_pSurface3d = ParseCylindricalSurface(p_Index);
//	}
//	if (itemType == _T("SPHERICAL_SURFACE"))
//	{
//		r_pSurface3d = ParseSphereSurface(p_Index);
//	}
//	if (itemType == _T("TOROIDAL_SURFACE"))
//	{
//		r_pSurface3d = ParseTorusSurface(p_Index);
//	}
//	if (itemType == _T("B_SPLINE_SURFACE_WITH_KNOTS"))
//	{
//		r_pSurface3d = ParseBsplineSurface(p_Index);
//	}
//	if (itemType == _T("RATIONAL_B_SPLINE_SURFACE"))
//	{
//		r_pSurface3d = ParseNurbsSurface(p_Index);
//	}
//	if (itemType == _T("CONICAL_SURFACE"))
//	{
//		r_pSurface3d = ParseConeSurface(p_Index);
//	}
//
//	assert(r_pSurface3d != nullptr);
//	return r_pSurface3d;
//}
//
//ModelPlane* StructedStepFileParser::ParsePlane(size_t p_Index)
//{
//	ModelPlane* r_pPlane3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pPlane3d = GetParsed<ModelPlane>(p_Index);
//	//若已经转换
//	if (r_pPlane3d != nullptr) {
//		return r_pPlane3d;
//	}
//
//	//Plane  STEP文件文本实例
//	//#233 = PLANE ( 'NONE',  #306 ) ;
//	//StepPlane(name,Axis2Placement3d*)-->Plane3d(CartesianCoordinate3d)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(2 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 StepAxis2Placement3d*->Axis2Placement3d*->CartesianCoordinate3d
//	EntityUcs* p_Poistion = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//		////将对应id-1位置的StepAxis2Placement3d*进行转换成Axis2Placement3d*
//		p_Poistion = ParseAxis2Placement3d(id - 1);
//		assert(nullptr != p_Poistion);
//	}
//
//	//将Axis2Placement3d对象 转换为CartesianCoordinate3d 对象
//	Point3D positionGeometry = p_Poistion->GetOrigin();
//
//	Vector3D directionGeometry = p_Poistion->GetUcsZ();
//
//	ConsPlane* consplane = new ConsPlane(positionGeometry, directionGeometry);
//	r_pPlane3d = new ModelPlane(consplane);
//	r_pPlane3d->GetGeometry()->SetXaxis(p_Poistion->GetUcsX());
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pPlane3d));
//
//	return r_pPlane3d;
//
//}
//
//ModelCylinderSurface* StructedStepFileParser::ParseCylindricalSurface(size_t p_Index)
//{
//	ModelCylinderSurface* r_pCylindricalSurface3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pCylindricalSurface3d = GetParsed<ModelCylinderSurface>(p_Index);
//	//若已经转换
//	if (r_pCylindricalSurface3d != nullptr) {
//		return r_pCylindricalSurface3d;
//	}
//
//	//CylindricalSurface  STEP文件文本实例
//	//#64 = CYLINDRICAL_SURFACE ( 'NONE', #66, 20.0);
//	//StepCylindricalSurface(name, StepAxis2Placement3d* ,double)-->CylindricalSurface3d(CartesianCoordinate3d,real)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(3 == splits.size());
//
//	//参数1 name
//	const String& name = splits[0];
//
//	//参数2 axis2placement3d  StepAxis2Placement3d*-->Axis2Placement3d*-->CartesianCoordinate3d
//	EntityUcs* p_Poistion = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepAxis2Placement3d*进行转换成Axis2Placement3d*
//		p_Poistion = ParseAxis2Placement3d(id - 1);
//		assert(nullptr != p_Poistion);
//	}
//
//	//将Axis2Placement3d* 手动删除
//	//delete p_Poistion;
//
//	//参数3 radius
//	double radius = 0.0;
//	{
//		String& radiusObject = splits[2];
//		radius = stod(radiusObject);
//		assert(radius > 0);
//	}
//
//	ConsCylinderSurface* conscylindersurface = new ConsCylinderSurface(*p_Poistion, radius);
//	r_pCylindricalSurface3d = new ModelCylinderSurface(conscylindersurface);
//	assert(r_pCylindricalSurface3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pCylindricalSurface3d));
//
//	return r_pCylindricalSurface3d;
//}
//
//ModelSphereSurface* StructedStepFileParser::ParseSphereSurface(size_t p_Index)
//{
//	ModelSphereSurface* r_pSphericalSurface3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pSphericalSurface3d = GetParsed<ModelSphereSurface>(p_Index);
//	//若已经转换
//	if (r_pSphericalSurface3d != nullptr) {
//		return r_pSphericalSurface3d;
//	}
//
//	//SphericalSurface  STEP文件文本实例
//	//#109 = SPHERICAL_SURFACE ( 'NONE', #21, 10.0 ) ;
//	//StepSphericalSurface(name,StepAxis2Placement3d* ,double radius)
//	//-- > SphericalSurface(CartesianCoordinate3d, double radius)
//
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(3 == splits.size());
//
//	//参数1 name
//	const String& name = splits[0];
//
//	//参数2 StepAxis2Placement3d*->Axis2Placement3d*->CartesianCoordinate3d
//	EntityUcs* p_Poistion = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepAxis2Placement3d*进行转换成Axis2Placement3d*
//		p_Poistion = ParseAxis2Placement3d(id - 1);
//		assert(nullptr != p_Poistion);
//	}
//
//	//将Axis2Placement3d* 手动删除
//	//delete p_Poistion;
//
//	//参数3 radius
//	double radius = 0.0;
//	{
//		String& radiusObject = splits[2];
//		radius = stod(radiusObject);
//		assert(radius > 0);
//	}
//
//	ConsSphereSurface* conssphere = new ConsSphereSurface(*p_Poistion, radius);
//	r_pSphericalSurface3d = new ModelSphereSurface(conssphere);
//	assert(r_pSphericalSurface3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pSphericalSurface3d));
//
//	return r_pSphericalSurface3d;
//}
//
//ModelTorusSurface* StructedStepFileParser::ParseTorusSurface(size_t p_Index)
//{
//	ModelTorusSurface* r_pToroidalSurface = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pToroidalSurface = GetParsed<ModelTorusSurface>(p_Index);
//	//若已经转换
//	if (r_pToroidalSurface != nullptr) {
//		return r_pToroidalSurface;
//	}
//
//	//ToroidalSurface  STEP文件文本实例
//	//#58721 = TOROIDAL_SURFACE('NONE', #29, 1.74999999998400, 0.5000000000000);
//	//StepToroidalSurface(name,StepAxis2Placement3d* ,double majorradius,double minorradius)
//	//-- > ToroidalSurface(CartesianCoordinate3d, double majorradius,double minorradius)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(4 == splits.size());
//
//	//参数1 name
//	const String& name = splits[0];
//
//	//参数2 StepAxis2Placement3d*->Axis2Placement3d*->CartesianCoordinate3d
//	EntityUcs* p_Poistion = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepAxis2Placement3d*进行转换成Axis2Placement3d*
//		p_Poistion = ParseAxis2Placement3d(id - 1);
//		assert(nullptr != p_Poistion);
//	}
//
//	//参数3 majorradius
//	double majorradius = 0.0;
//	{
//		String& majorradiusObject = splits[2];
//		majorradius = stod(majorradiusObject);
//		//assert(majorradius > 0);
//	}
//
//	//参数4 minorradius
//	double minorradius = 0.0;
//	{
//		String& minorradiusObject = splits[3];
//		minorradius = stod(minorradiusObject);
//		//assert(minorradius > 0);
//	}
//
//	ConsTorusSurface* constoursurface = new ConsTorusSurface(*p_Poistion, majorradius, minorradius);
//	r_pToroidalSurface = new ModelTorusSurface(constoursurface);
//	assert(r_pToroidalSurface != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pToroidalSurface));
//
//	return r_pToroidalSurface;
//}
//
//ModelBsplineSurface* StructedStepFileParser::ParseBsplineSurface(size_t p_Index)
//{
//	ModelBsplineSurface* r_pB_SplineSurfaceWithKnots = nullptr;
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pB_SplineSurfaceWithKnots = GetParsed<ModelBsplineSurface>(p_Index);
//	//若已经转换
//	if (r_pB_SplineSurfaceWithKnots != nullptr) {
//		return r_pB_SplineSurfaceWithKnots;
//	}
//
//	//文本实例
//	//#148=B_SPLINE_SURFACE_WITH_KNOTS('',5,1,((#149,#150),(#151,#152),(#153,#154),(#155,#156),(#157,#158),(#159,#160),(#161,#162),(#163,#164),(#165,#166),(#167,#168),(#169,#170),(#171,#172)),.UNSPECIFIED.,.F.,.F.,.U.,(6,3,3,6),(2,2),(0.,1709.3906317,2822.66859157,5696.69082421),(-19.,-9.),.UNSPECIFIED.) ;
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(13 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 Udegree
//	int p_UDegree;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//		p_UDegree = id;
//	}
//
//	//参数 3 Vdegree
//	int p_VDegree;
//	{
//		size_t id = stoul(splits[2]);
//		assert(id > 0);
//		p_VDegree = id;
//	}
//
//	//参数 4 ControlPointsList 控制点
//	vector<vector<Point3D>> p_ControlPointsList;
//	{
//		String& ControlPointListArrayObject = splits[3];
//		assert(ControlPointListArrayObject.size() > 2);
//
//		String ControlPointListArray = ControlPointListArrayObject.substr(1, ControlPointListArrayObject.size() - 2);
//		vector<String> PointVectorIds = SplitBy(ControlPointListArray, _T("$"));
//		assert(0 < PointVectorIds.size());
//
//		vector<Point3D> p_PointsList;
//
//		for (String bid : PointVectorIds)
//		{
//			bid = bid.substr(1, bid.size() - 2);
//			vector<String>  pointsId = SplitBy(bid, _T("%"));
//
//			for (String pid : pointsId)
//			{
//				size_t id = stoul(pid);
//				assert(id > 0);
//
//				//将对应id-1位置的StepCartesianPoint*转换成Point3d*
//				ModelPoint* pointPtr = ParseCartesianPoint(id - 1);
//				Point3D point = pointPtr->GetPoint();
//				p_PointsList.push_back(point);
//			}
//
//			p_ControlPointsList.push_back(p_PointsList);
//			p_PointsList.clear();
//		}
//	}
//
//	//参数5 Surfaceform
//	const String& Surfaceform = splits[4];
//
//	//参数6 UClosedCurve
//	bool UClosedCurve = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[5][1])
//		{
//		case 'T':
//			UClosedCurve = true;
//			break;
//		case 'F':
//			UClosedCurve = false;
//			break;
//		case 'U':
//			UClosedCurve = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	//参数7 VClosedCurve
//	bool VClosedCurve = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[6][1])
//		{
//		case 'T':
//			VClosedCurve = true;
//			break;
//		case 'F':
//			VClosedCurve = false;
//			break;
//		case 'U':
//			VClosedCurve = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	//参数8 SelfIntersect
//	bool SelfIntersect = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits[7][1])
//		{
//		case 'T':
//			SelfIntersect = true;
//			break;
//		case 'F':
//			SelfIntersect = false;
//			break;
//		case 'U':
//			SelfIntersect = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	//参数9 UMultiplicities
//	vector<ushort> p_UMultiplicities;
//	String& UMultiplicitiesArrayObject = splits[8];
//	assert(UMultiplicitiesArrayObject.size() > 2);
//	String UMultiplicitiesArray = UMultiplicitiesArrayObject.substr(1, UMultiplicitiesArrayObject.size() - 2);
//	vector<String> UMultiplicitiesIds = SplitBy(UMultiplicitiesArray, _T("$"));
//	assert(0 < UMultiplicitiesIds.size());
//	for (const auto& bid : UMultiplicitiesIds)
//	{
//		size_t id = stoul(bid);
//		assert(id > 0);
//		p_UMultiplicities.push_back(id);
//	}
//
//	//参数10 VMultiplicities
//	vector<ushort> p_VMultiplicities;
//	String& VMultiplicitiesArrayObject = splits[9];
//	assert(VMultiplicitiesArrayObject.size() > 2);
//	String VMultiplicitiesArray = VMultiplicitiesArrayObject.substr(1, VMultiplicitiesArrayObject.size() - 2);
//	vector<String> VMultiplicitiesIds = SplitBy(VMultiplicitiesArray, _T("$"));
//	assert(0 < VMultiplicitiesIds.size());
//	for (const auto& bid : VMultiplicitiesIds)
//	{
//		size_t id = stoul(bid);
//		assert(id > 0);
//		p_VMultiplicities.push_back(id);
//	}
//
//	/*
//	vector<real> p_Knots;
//	String& KnotsArrayObject = splits_part2[1];
//	assert(KnotsArrayObject.size() > 2);
//	String KnotsArray = KnotsArrayObject.substr(1, KnotsArrayObject.size() - 2);
//	vector<String> KnotsIds = SplitBy(KnotsArray, _T("%"));
//	assert(0 < KnotsIds.size());
//	int p_Multindex = 0;
//	for (const auto& bid : KnotsIds)
//	{
//		double id = stod(bid);
//		assert(id >= 0);
//		for (auto i = 0; i < p_Multiplicities[p_Multindex]; ++i)
//		{
//			p_Knots.push_back(id);
//		}
//		++p_Multindex;
//	}
//	*/
//	//参数11 UKnots
//	vector<real> p_UKnots;
//	String& UKnotsArrayObject = splits[10];
//	assert(UKnotsArrayObject.size() > 2);
//	String UKnotsArray = UKnotsArrayObject.substr(1, UKnotsArrayObject.size() - 2);
//	vector<String> UKnotsIds = SplitBy(UKnotsArray, _T("$"));
//	assert(0 < UKnotsIds.size());
//	int p_UMultindex = 0;
//	for (const auto& bid : UKnotsIds)
//	{
//		double id = stod(bid);
//		assert(id >= 0);
//		for (auto i = 0; i < p_UMultiplicities[p_UMultindex]; ++i)
//		{
//			p_UKnots.push_back(id);
//		}
//		++p_UMultindex;
//	}
//
//	//参数12 VKnots
//	vector<real> p_VKnots;
//	String& VKnotsArrayObject = splits[11];
//	assert(VKnotsArrayObject.size() > 2);
//	String VKnotsArray = VKnotsArrayObject.substr(1, VKnotsArrayObject.size() - 2);
//	vector<String> VKnotsIds = SplitBy(VKnotsArray, _T("$"));
//	assert(0 < VKnotsIds.size());
//	int p_VMultindex = 0;
//	for (const auto& bid : VKnotsIds)
//	{
//		double id = stod(bid);
//		//assert(id >= 0);
//		for (auto i = 0; i < p_VMultiplicities[p_VMultindex]; ++i)
//		{
//			p_VKnots.push_back(id);
//		}
//		++p_VMultindex;
//	}
//
//	//参数13 KnotType
//	const String& KnotType = splits[12];
//
//	ConsBsplineSurface* consbspline = new ConsBsplineSurface(p_ControlPointsList, p_UKnots, p_UDegree, p_VKnots, p_VDegree);
//	r_pB_SplineSurfaceWithKnots = new ModelBsplineSurface(consbspline);
//	assert(r_pB_SplineSurfaceWithKnots != nullptr);
//
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pB_SplineSurfaceWithKnots));
//
//	return r_pB_SplineSurfaceWithKnots;
//}
//
//ModelNurbsSurface* StructedStepFileParser::ParseNurbsSurface(size_t p_Index)
//{
//	ModelNurbsSurface* r_pRational_B_SplineSurface3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pRational_B_SplineSurface3d = GetParsed<ModelNurbsSurface>(p_Index);
//	//若已经转换
//	if (r_pRational_B_SplineSurface3d != nullptr) {
//		return r_pRational_B_SplineSurface3d;
//	}
//
//
//	//获取属性字符串,与其他类型的属性字符串不太一样
//	/*
//	#113 = (
//		BOUNDED_SURFACE ()
//		B_SPLINE_SURFACE (2, 2, ((#86, #87, #88), (#89, #90, #91), (#92, #93, #94), (#95, #96, #97), (#98, #99, #100), (#101, #102, #103), (#104, #105, #106), (#107, #108, #109), (#110, #111, #112)), .UNSPECIFIED., .T., .F., .U.)
//		B_SPLINE_SURFACE_WITH_KNOTS ((3, 2, 2, 2, 3), (3, 3), (0., 0.25, 0.5, 0.75, 1.), (0., 1.), .UNSPECIFIED.)
//		SURFACE ()
//		GEOMETRIC_REPRESENTATION_ITEM ()
//		RATIONAL_B_SPLINE_SURFACE (((1., 1., 1.), (0.707106781186548, 0.707106781186548, 0.707106781186548), (1., 1., 1.), (0.707106781186548, 0.707106781186548, 0.707106781186548), (1., 1., 1.), (0.707106781186548, 0.707106781186548, 0.707106781186548), (1., 1., 1.), (0.707106781186548, 0.707106781186548, 0.707106781186548), (1., 1., 1.)))
//		REPRESENTATION_ITEM ('')
//	);
//	*/
//
//	const String& value = m_File[p_Index].m_Value;
//
//	//参数 1 name
//	const String name = _T("null");
//
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(3 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//第一部分参数：B_SPLINE_SURFACE的参数
//	const String& para_B_SplineCurve = splits[0];
//
//	vector<String> splits_part1 = SplitBy(para_B_SplineCurve, _T("$"));
//	assert(7 == splits_part1.size());
//
//
//	//参数 1 degreeU
//	ushort p_DegreeU;
//	{
//		ushort id = stoul(splits_part1[0]);
//		assert(id > 0);
//		p_DegreeU = id;
//	}
//	//参数 2 degreeV
//	ushort p_DegreeV;
//	{
//		ushort id = stoul(splits_part1[1]);
//		assert(id > 0);
//		p_DegreeV = id;
//	}
//
//	//参数 3 ControlPointsList 控制点
//	vector<vector<Point3D>> p_ControlPointsLists;
//	{
//		String& ControlPointListArrayObject = splits_part1[2];
//		assert(ControlPointListArrayObject.size() > 2);
//
//		String ControlPointListArray = ControlPointListArrayObject.substr(1, ControlPointListArrayObject.size() - 2);
//
//		vector<String> PointListIds = SplitBy(ControlPointListArray, _T("%"));
//		assert(0 < PointListIds.size());
//
//		for (auto& poistList : PointListIds)
//		{
//			poistList = poistList.substr(1, poistList.size() - 2);
//			vector<String> pointIds = SplitBy(poistList, _T("@"));
//
//			vector<Point3D> p_ControlPointsList;
//			for (const auto& bid : pointIds)
//			{
//				size_t id = stoul(bid);
//				assert(id > 0);
//
//				//将对应id-1位置的StepCartesianPoint*转换成Point3d*
//				ModelPoint* pointPtr = ParseCartesianPoint(id - 1);
//				assert(nullptr != pointPtr);
//				Point3D point = pointPtr->GetPoint();
//
//				//delete pointPtr;
//
//				p_ControlPointsList.push_back(point);
//			}
//			p_ControlPointsLists.push_back(p_ControlPointsList);
//		}
//	}
//
//	//参数4 Surfaceform
//	const String& Surfaceform = splits_part1[3];
//
//	//参数5 ClosedCurveU
//	bool ClosedCurve_U = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits_part1[4][1])
//		{
//		case 'T':
//			ClosedCurve_U = true;
//			break;
//		case 'F':
//			ClosedCurve_U = false;
//			break;
//		case 'U':
//			ClosedCurve_U = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//	//参数6 ClosedCurveV
//	bool ClosedCurve_V = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits_part1[5][1])
//		{
//		case 'T':
//			ClosedCurve_V = true;
//			break;
//		case 'F':
//			ClosedCurve_V = false;
//			break;
//		case 'U':
//			ClosedCurve_V = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	//参数7 SelfIntersect
//	bool SelfIntersect = false;
//	{
//		//形式为 .T. 或 .F.
//		switch (splits_part1[6][1])
//		{
//		case 'T':
//			SelfIntersect = true;
//			break;
//		case 'F':
//			SelfIntersect = false;
//			break;
//		case 'U':
//			SelfIntersect = true;
//			break;
//		default:
//			assert(false);
//			break;
//		}
//	}
//
//	//第二部分：B_SPLINE_Surface_WITH_KNOTS的属性值
//	const String& para_B_SPLINE_SURFACE_WITH_KNOTS = splits[1];
//	vector<String> splits_part2 = SplitBy(para_B_SPLINE_SURFACE_WITH_KNOTS, _T("$"));
//	assert(5 == splits_part2.size());
//
//	//参数1 MultiplicitiesU
//	vector<ushort> p_Multiplicities_U;
//	String& MultiplicitiesArrayObject = splits_part2[0];
//	assert(MultiplicitiesArrayObject.size() > 2);
//	String MultiplicitiesArray = MultiplicitiesArrayObject.substr(1, MultiplicitiesArrayObject.size() - 2);
//	vector<String> MultiplicitiesIds = SplitBy(MultiplicitiesArray, _T("%"));
//	assert(0 < MultiplicitiesIds.size());
//	for (const auto& bid : MultiplicitiesIds)
//	{
//		size_t id = stoul(bid);
//		assert(id > 0);
//		p_Multiplicities_U.push_back(id);
//	}
//
//	//参数2  MultiplicitiesV
//	vector<ushort> p_Multiplicities_V;
//	MultiplicitiesArrayObject = splits_part2[1];
//	assert(MultiplicitiesArrayObject.size() > 2);
//	MultiplicitiesArray = MultiplicitiesArrayObject.substr(1, MultiplicitiesArrayObject.size() - 2);
//	MultiplicitiesIds = SplitBy(MultiplicitiesArray, _T("%"));
//	assert(0 < MultiplicitiesIds.size());
//	for (const auto& bid : MultiplicitiesIds)
//	{
//		size_t id = stoul(bid);
//		assert(id > 0);
//		p_Multiplicities_V.push_back(id);
//	}
//
//	//参数3 KnotsU
//	vector<real> p_Knots_U;
//	String& KnotsArrayObject = splits_part2[2];
//	assert(KnotsArrayObject.size() > 2);
//	String KnotsArray = KnotsArrayObject.substr(1, KnotsArrayObject.size() - 2);
//	vector<String> KnotsIds = SplitBy(KnotsArray, _T("%"));
//	assert(0 < KnotsIds.size());
//	int p_Multindex_U = 0;
//	for (const auto& bid : KnotsIds)
//	{
//		double id = stod(bid);
//		//assert(id >= 0);
//		for (auto i = 0; i < p_Multiplicities_U[p_Multindex_U]; ++i)
//		{
//			p_Knots_U.push_back(id);
//		}
//		++p_Multindex_U;
//	}
//
//	//参数4 KnotsV
//	vector<real> p_Knots_V;
//	KnotsArrayObject = splits_part2[3];
//	assert(KnotsArrayObject.size() > 2);
//	KnotsArray = KnotsArrayObject.substr(1, KnotsArrayObject.size() - 2);
//	KnotsIds = SplitBy(KnotsArray, _T("%"));
//	assert(0 < KnotsIds.size());
//	int p_Multindex_V = 0;
//	for (const auto& bid : KnotsIds)
//	{
//		double id = stod(bid);
//		//assert(id >= 0);
//		for (auto i = 0; i < p_Multiplicities_V[p_Multindex_V]; ++i)
//		{
//			p_Knots_V.push_back(id);
//		}
//		++p_Multindex_V;
//	}
//
//	//参数5 KnotType
//	const String& KnotType = splits_part2[4];
//
//
//	//第三部分：RATIONAL_B_SPLINE_SURFACE的属性值
//
//	//参数1 Weights
//	vector<vector<real>> p_WeightLists;
//	String& WeightsArrayObject = splits[2];
//	assert(WeightsArrayObject.size() > 2);
//	String WeightsArray = WeightsArrayObject.substr(1, WeightsArrayObject.size() - 2);
//	vector<String> p_weights = SplitBy(WeightsArray, _T("%"));
//
//	for (String& weights : p_weights)
//	{
//		vector<real> p_WeightList;
//		weights = weights.substr(1, weights.size() - 2);
//		vector<String> WeightsIds = SplitBy(weights, _T("@"));
//		assert(0 < WeightsIds.size());
//		for (const auto& bid : WeightsIds)
//		{
//			double id = stod(bid);
//			assert(id >= 0);
//			p_WeightList.push_back(id);
//		}
//		p_WeightLists.push_back(p_WeightList);
//	}
//
//	ConsNurbsSurface* consnurbs = new ConsNurbsSurface(p_ControlPointsLists, p_Knots_U, p_DegreeU, p_Knots_V, p_DegreeV, p_WeightLists);
//	r_pRational_B_SplineSurface3d = new ModelNurbsSurface(consnurbs);
//	assert(r_pRational_B_SplineSurface3d != nullptr);
//
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pRational_B_SplineSurface3d));
//
//	return r_pRational_B_SplineSurface3d;
//}
//
//ModelConeSurface* StructedStepFileParser::ParseConeSurface(size_t p_Index)
//{
//	ModelConeSurface* r_pConicalSurface3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pConicalSurface3d = GetParsed<ModelConeSurface>(p_Index);
//	//若已经转换
//	if (r_pConicalSurface3d != nullptr) {
//		return r_pConicalSurface3d;
//	}
//
//	//ConicalSurface  STEP文件文本实例
//	//#117 = CONICAL_SURFACE ( 'NONE', #1, 10.00000000000000000, 0.5235987755982989267 ) ;
//	//StepConicalSurface(name, StepAxis2Placement3d*, double，double)-->ConicalSurface3d(CartesianCoordinate3d,real,real)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(4 == splits.size());
//
//	//参数1 name
//	const String& name = splits[0];
//
//	//参数3 radius
//	double radius = 0.0;
//	{
//		String& radiusObject = splits[2];
//		radius = stod(radiusObject);
//		assert(radius > 0);
//	}
//
//	//参数4 semiangle
//	double semiangle = 0.0;
//	{
//		String& SemiAngleObject = splits[3];
//		semiangle = stod(SemiAngleObject);
//		//	assert(Abs(semiangle) >= 0 && Abs(semiangle) <= acos(-1));
//	}
//
//	double c = 1 / tan(semiangle);
//
//	//参数2 StepAxis2Placement3d*->Axis2Placement3d*->CartesianCoordinate3d
//	EntityUcs* p_Position = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//
//		//将对应id-1位置的StepAxis2Placement3d*进行转换成Axis2Placement3d*
//		EntityUcs* temp_position = ParseAxis2Placement3d(id - 1);
//
//		Vector3D zz_axis(-temp_position->GetUcsZ());
//
//		double n = sqrt(zz_axis.GetX() * zz_axis.GetX() +
//			zz_axis.GetY() * zz_axis.GetY() +
//			zz_axis.GetZ() * zz_axis.GetZ());
//
//		double xx = temp_position->GetOrigin().GetX() + c * zz_axis.GetX() / n;
//		double yy = temp_position->GetOrigin().GetY() + c * zz_axis.GetY() / n;
//		double zz = temp_position->GetOrigin().GetZ() + c * zz_axis.GetZ() / n;
//		Point3D origin_point(xx, yy, zz);
//
//		p_Position = new EntityUcs(origin_point, temp_position->GetUcsX(), temp_position->GetUcsZ(), 1.0);
//
//		assert(nullptr != p_Position);
//
//	}
//
//	ConsConeSurface* conscone = new ConsConeSurface(*p_Position, c);
//	conscone->SetIntervalV(1e-6, radius / tan(semiangle), 0);
//	r_pConicalSurface3d = new ModelConeSurface(conscone);
//	assert(r_pConicalSurface3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pConicalSurface3d));
//
//	return r_pConicalSurface3d;
//}
//
///*
//
//
//*/
//EntityUcs* StructedStepFileParser::ParseAxis2Placement3d(size_t p_Index)
//{
//	EntityUcs* r_pAxis2Placement3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pAxis2Placement3d = GetParsed<EntityUcs>(p_Index);
//	//若已经转换
//	if (r_pAxis2Placement3d != nullptr) {
//		return r_pAxis2Placement3d;
//	}
//
//	//Axis2Placement3d  STEP文件文本实例
//	//#231 = AXIS2_PLACEMENT_3D ( 'NONE', #397, #21, #365 ) ;
//	//StepAxis2Placement3d(name,StepCartesianPoint*,StepDirection*,StepDirection*)；
//	//-- > Axis2Placement3d(Point3d, Direction3d, Direction3d)
//
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(4 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 location  StepCartesianPoint*-->Point3d
//	ModelPoint* location = nullptr;
//	Point3D origin;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//		//将对应id-1位置的StepCartesianPoint*进行转换成CartesianPoint*
//		location = ParseCartesianPoint(id - 1);
//		assert(nullptr != location);
//		origin = location->GetPoint();
//	}
//
//	//参数 3 pAxisdirection StepDirection*-->Direction3d
//	Vector3D* pAxisdirection = nullptr;
//	{
//		size_t id = stoul(splits[2]);
//		assert(id > 0);
//		//将对应id-1位置的StepDirection*进行转换成Direction3d*
//		pAxisdirection = ParseDirection(id - 1);
//		assert(nullptr != pAxisdirection);
//	}
//
//	//参数 4 pRefdirection  StepDirection*-->Direction3d
//	Vector3D* pRefdirection = nullptr;
//	{
//		if (splits[3] == _T("$"))
//		{
//			double zz = 0;
//			double yy = 0;
//			double xx = 0;
//			if (abs(pAxisdirection->GetX()) >= abs(pAxisdirection->GetY()) && abs(pAxisdirection->GetX()) >= abs(pAxisdirection->GetZ()))
//			{
//				yy = 1.0;
//				zz = 1.0;
//				xx = (-pAxisdirection->GetY() * yy - pAxisdirection->GetZ() * zz) / (pAxisdirection->GetX());
//			}
//			else if (abs(pAxisdirection->GetY()) >= abs(pAxisdirection->GetX()) && abs(pAxisdirection->GetY()) >= abs(pAxisdirection->GetZ()))
//			{
//				xx = 1.0;
//				zz = 1.0;
//				yy = (-pAxisdirection->GetX() * xx - pAxisdirection->GetZ() * zz) / (pAxisdirection->GetY());
//			}
//			else if (abs(pAxisdirection->GetZ()) >= abs(pAxisdirection->GetX()) && abs(pAxisdirection->GetZ()) >= abs(pAxisdirection->GetY()))
//			{
//				xx = 1.0;
//				yy = 1.0;
//				zz = (-pAxisdirection->GetX() * xx - pAxisdirection->GetY() * yy) / (pAxisdirection->GetZ());
//			}
//			double n = sqrt(xx * xx + yy * yy + zz * zz);
//			xx = xx / n;
//			yy = yy / n;
//			zz = zz / n;
//			pRefdirection = new Vector3D(xx, yy, zz);
//		}
//		else
//		{
//			size_t id = stoul(splits[3]);
//			assert(id > 0);
//			//将对应id-1位置的StepDirection*进行转换成Direction3d*
//			pRefdirection = ParseDirection(id - 1);
//			assert(nullptr != pRefdirection);
//		}
//	}
//
//	r_pAxis2Placement3d = new EntityUcs(origin, *pRefdirection, *pAxisdirection, 1.0);
//	assert(r_pAxis2Placement3d != nullptr);
//
//	//将new的对象删除；
//	//delete location;
//	//delete pAxisdirection;
//	//delete pRefdirection;
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pAxis2Placement3d));
//
//	return r_pAxis2Placement3d;
//}
//
///*
//
//
//*/
//Vector3D* StructedStepFileParser::ParseDirection(size_t p_Index)
//{
//	Vector3D* r_pDirection3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pDirection3d = GetParsed<Vector3D>(p_Index);
//	//若已经转换
//	if (r_pDirection3d != nullptr) {
//		return r_pDirection3d;
//	}
//
//	//Direction  STEP文件文本实例
//	//#63 = DIRECTION ( 'NONE',  ( -0.0, -0.0, -1.0 ) ) ;
//	//StepDirection(name,vector<double> )-->Direction3d(real x,real y,real z)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(2 == splits.size());
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2  vector<double> --> (real x,real y,real z)
//	String& directionRationsArrayObject = splits[1];
//	assert(directionRationsArrayObject.size() > 2);
//	String directionRationsArray = directionRationsArrayObject.substr(1, directionRationsArrayObject.size() - 2);
//	vector<String> RationsIds = SplitBy(directionRationsArray, _T("$"));
//	assert(0 < RationsIds.size());
//
//	real px, py, pz;
//	{
//		px = stod(RationsIds[0]);
//		py = stod(RationsIds[1]);
//		pz = stod(RationsIds[2]);
//	}
//
//	//	Vector3d  vector = new Vector3d(px, py, pz);
//	//	r_pDirection3d = new Direction3d(vector);
//
//	r_pDirection3d = new Vector3D(px, py, pz);
//	assert(r_pDirection3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pDirection3d));
//
//	return r_pDirection3d;
//}
//
//Vector3D* StructedStepFileParser::ParseVector(size_t p_Index)
//{
//	Vector3D* r_pVector3d = nullptr;
//
//	//判断是否已经解析过数组中对应p_Index位置的对象
//	r_pVector3d = GetParsed<Vector3D>(p_Index);
//	//若已经转换
//	if (r_pVector3d != nullptr) {
//		return r_pVector3d;
//	}
//
//	//Vector  STEP文件文本实例
//	//#232 = VECTOR ( 'NONE', #183, 1000.0 ) ;
//	//StepVector(String name,StepDirection*,double radius)-->Vector3d(real x, real y,real z)
//
//	//获取属性字符串
//	const String& value = m_File[p_Index].m_Value;
//
//	//解析value
//	vector<String> splits = SplitBy(value, _T(","));
//	assert(3 == splits.size());
//	//开始根据 p_Value 分割结果构建参数
//
//	//参数 1 name
//	const String& name = splits[0];
//
//	//参数 2 StepDirection*-->Direction3d*-->(x,y,z)
//	Vector3D* directionGeometry = nullptr;
//	{
//		size_t id = stoul(splits[1]);
//		assert(id > 0);
//		//将对应id-1位置的StepDirection*进行转换成Direction3d*
//		directionGeometry = ParseDirection(id - 1);
//		assert(nullptr != directionGeometry);
//	}
//
//	r_pVector3d = directionGeometry;
//
//	//参数 3 Magnitude
//	double Magnitude = stod(splits[2]);
//	//assert(Magnitude >0);
//
//
//	assert(r_pVector3d != nullptr);
//
//	//把转换后的对象映射关系存入map中
//	m_TranslatedItemInfo.insert(make_pair(p_Index, r_pVector3d));
//
//	return r_pVector3d;
//
//}
//
//Body* StructedStepFileParser::ParseAndCreateManifoldSolidBrep()
//{
//
//	//	cout << m_File.GetDataCount()<< endl;
//		//对ManifoldSolidBrep进行转换		
//
//	vector<size_t> manifoldSolidBrepIds = m_File.GetManifoldSolidBrepIndexs();
//	Body* body = new Body();
//	bool flag = false;
//	Lump* model = nullptr;
//	Lump* ans = nullptr;
//	for (size_t id : manifoldSolidBrepIds)
//	{
//		if (!flag) {
//			model = ParseManifoldSolidBrep(id);
//			assert(nullptr != model);
//			model->SetOwner(body);
//			ans = model;
//			flag = true;
//		}
//		else {
//			Lump* temp = ParseManifoldSolidBrep(id);
//			assert(nullptr != temp);
//			ans->SetNext(temp);
//			temp->SetOwner(body);
//			ans = temp;
//		}
//	}
//	ans->SetNext(model);
//
//	body->SetContainLump(model);
//
//	return body;
//
//}
