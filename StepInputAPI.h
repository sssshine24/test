#pragma once
#include "pch.h"

class SG_DECLSPEC StepInputAPI
{
public:
	// Input
	void StepFileLoad(const String& p_Path);
	// 给定输出文件名（.stp/.step/.STEP后缀），返回体指针
	//Body* StepFileLoadAndParse(const String& p_Path);

	int StepFileLoadAndParse(const String& p_Path);
};