#pragma once
#include "pch.h"

class SG_DECLSPEC StepInputAPI
{
public:
	// Input
	void StepFileLoad(const String& p_Path);
	// ��������ļ�����.stp/.step/.STEP��׺����������ָ��
	//Body* StepFileLoadAndParse(const String& p_Path);

	int StepFileLoadAndParse(const String& p_Path);
};