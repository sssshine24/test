#include "stdafx.h"
#include "pch.h"
#include "StructedStepFileDataItem.h"



	StructedStepFileDataItem::StructedStepFileDataItem(const size_t p_StepId, const String& p_StepType, const String& p_Value) :
		m_StepId(p_StepId),
		m_StepType(p_StepType),
		m_Value(p_Value)
	{

	}

// �������ռ��ⲿ��ʹ�������ȫ�޶���
//seago::StructedStepFileDataItem::StructedStepFileDataItem(const size_t p_StepId, const String& p_StepType, const String& p_Value)
//	: m_StepId(p_StepId),
//	m_StepType(p_StepType),
//	m_Value(p_Value)
//{
//	// ���캯��ʵ����
//}

