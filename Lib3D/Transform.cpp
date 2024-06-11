//********************************************
// Transform.cpp
//********************************************
// class CTransform
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 23/02/98
// Modified : 23/02/98
//********************************************

#include "stdafx.h"
#include "Transform.h"


//********************************************
// Constructor
//********************************************
CTransform::CTransform()
{
	SetScale(CVector3d(1.0f,1.0f,1.0f));
	SetTranslation(CVector3d(0.0f,0.0f,0.0f));
	SetValueRotation(0.0f);
}

//********************************************
// Constructor
//********************************************
void CTransform::Clear(void)
{
	SetScale(CVector3d(1.0f,1.0f,1.0f));
	SetTranslation(CVector3d(0.0f,0.0f,0.0f));
	SetValueRotation(0.0f);
}

//********************************************
// Copy
//********************************************
void CTransform::Copy(CTransform &transform)
{
	SetScale(transform.GetScale());
	SetTranslation(transform.GetTranslation());
	SetRotation(transform.GetRotation());
	SetValueRotation(transform.GetValueRotation());
}

//********************************************
// Copy
//********************************************
void CTransform::Copy(CTransform *pTransform)
{
	SetScale(pTransform->GetScale());
	SetTranslation(pTransform->GetTranslation());
	SetRotation(pTransform->GetRotation());
	SetValueRotation(pTransform->GetValueRotation());
}




//////////////////////////////////////////////
// DATAS
//////////////////////////////////////////////
void CTransform::Read(std::ifstream &file)
{
	file >> m_Scale.m_x >> m_Scale.m_y >> m_Scale.m_z;
	file >> m_Rotation.m_x >> m_Rotation.m_y >> m_Rotation.m_z;
	file >> m_Translation.m_x >> m_Translation.m_y >> m_Translation.m_z;
	file >> m_ValueRotation;
}
void CTransform::Save(std::ofstream &file)
{
	file << m_Scale.m_x << " " << m_Scale.m_y << " "<< m_Scale.m_z << "\n";
	file << m_Rotation.m_x << " " << m_Rotation.m_y << " "<< m_Rotation.m_z << "\n";
	file << m_Translation.m_x << " " << m_Translation.m_y << " "<< m_Translation.m_z << "\n";
	file << m_ValueRotation << "\n";
}


// ** EOF **
