#include "stdafx.h"
#include "pch.h"
#include "StructedStepFileDataItem.h"



	StructedStepFileDataItem::StructedStepFileDataItem(const size_t p_StepId, const String& p_StepType, const String& p_Value) :
		m_StepId(p_StepId),
		m_StepType(p_StepType),
		m_Value(p_Value)
	{

	}

// 在命名空间外部，使用类的完全限定名
//seago::StructedStepFileDataItem::StructedStepFileDataItem(const size_t p_StepId, const String& p_StepType, const String& p_Value)
//	: m_StepId(p_StepId),
//	m_StepType(p_StepType),
//	m_Value(p_Value)
//{
//	// 构造函数实现体
//}

