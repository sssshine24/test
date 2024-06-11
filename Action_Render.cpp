// Action_Render.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Action_Render.h"


// Action_Render dialog

IMPLEMENT_DYNAMIC(Action_Render, CPropertyPage)

Action_Render::Action_Render()
	: CPropertyPage(Action_Render::IDD)
{

}

Action_Render::~Action_Render()
{
}

void Action_Render::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Action_Render, CPropertyPage)
END_MESSAGE_MAP()


// Action_Render message handlers
