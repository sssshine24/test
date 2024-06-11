#include "stdafx.h"
#include "pch.h"
//#include "include.h"
#include "StructedStepFileParser.h"  
#include "StructedStepFile.h"
//#include "WriteToStepFile.h"
#include "StepInputAPI.h"

using namespace std;

void StepInputAPI::StepFileLoad(const String& p_Path)
{
	assert(false == p_Path.empty());

	StructedStepFile stepFile;

	if (true == stepFile.Load(p_Path))
	{
		cout << stepFile.GetDataCount() << endl;
	}
}

//Body* StepInputAPI::StepFileLoadAndParse(const String& p_Path)
//{
//	assert(false == p_Path.empty());
//
//	StructedStepFile stepFile;
//	stepFile.Load(p_Path);
//
//	StructedStepFileParser parser(stepFile);
//
//	Body* body = parser.ParseAndCreateManifoldSolidBrep();
//
//	return body;
//
//}
int StepInputAPI::StepFileLoadAndParse(const String& p_Path)
{
	assert(false == p_Path.empty());

	StructedStepFile stepFile;
	stepFile.Load(p_Path);

	StructedStepFileParser parser(stepFile);

	//Body* body = parser.ParseAndCreateManifoldSolidBrep();

	parser.ParseAndCreateSurface();

	return 1;

}
