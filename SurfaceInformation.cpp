// SurfaceInformation.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "SurfaceInformation.h"
#include "Nurbs3D.h"
#include "generalbilineartransformation.h"
// SurfaceInformation dialog

IMPLEMENT_DYNAMIC(SurfaceInformation, CDialog)

SurfaceInformation::SurfaceInformation(CWnd* pParent /*=NULL*/)
	: CDialog(SurfaceInformation::IDD, pParent)
	, m_surfaceinformation(_T(""))
{
	pSurface = NULL;
}

SurfaceInformation::~SurfaceInformation()
{
}

void SurfaceInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_surfaceinformation);
}


BEGIN_MESSAGE_MAP(SurfaceInformation, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// SurfaceInformation message handlers

void SurfaceInformation::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	if(!pSurface)
		return;
	CString s1;

	char message[500];
	sprintf(message,"Control number : %d %d  \r\n",pSurface->GetCtrlNumberU(),pSurface->GetCtrlNumberV());
	m_surfaceinformation = message;

	sprintf(message,"Degree: %d %d  \r\n",pSurface->m_udegree,pSurface->m_vdegree);
	s1 = message;
	m_surfaceinformation = m_surfaceinformation + s1;

	for(int i = 0; i < pSurface->m_uknotcount; i++)
	{
		sprintf(message," %1.2f ",pSurface->m_uknot[i]);
		s1 = message;
		m_surfaceinformation = m_surfaceinformation + s1;
	}
	sprintf(message, "\r\n");
	s1 = message;
	m_surfaceinformation = m_surfaceinformation + s1;
	for(int i = 0; i < pSurface->m_vknotcount; i++)
	{
		sprintf(message," %1.2f ",pSurface->m_vknot[i]);
		s1 = message;
		m_surfaceinformation = m_surfaceinformation + s1;
	}
	sprintf(message, "\r\n");
	s1 = message;
	m_surfaceinformation = m_surfaceinformation + s1;

	for(int i = 0; i < pSurface->GetCtrlNumberU(); i++)
		for(int j = 0; j < pSurface->GetCtrlNumberV(); j++)
		{
			Point3D p1 = pSurface->GetControlPoint(i,j);
			double w = pSurface->GetWeight(i,j);
			sprintf(message,"(%d  %d):  %1.6f   %1.6f   %1.6f  %1.6f \r\n",i,j,p1.x,p1.y,p1.z,w);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;
		}

	if(pSurface->ptransformation)
	{
		if(pSurface->ptransformation->m_ReparameterizationType == generalbilinear)
		{
			sprintf(message,"General bilinear transformation \r\n");
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			GeneralBilinearTransformation* pTrans = (GeneralBilinearTransformation*)pSurface->ptransformation;
			sprintf(message,"e00 =  %1.6f \r\n",pTrans->e00);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"e01 =  %1.6f \r\n",pTrans->e01);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"e10 =  %1.6f \r\n",pTrans->e10);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"e11 =  %1.6f \r\n",pTrans->e11);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"f00 =  %1.6f \r\n",pTrans->f00);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"f01 =  %1.6f \r\n",pTrans->f01);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"f10 =  %1.6f \r\n",pTrans->f10);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"f11 =  %1.6f \r\n",pTrans->f11);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"g00 =  %1.6f \r\n",pTrans->g00);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"g01 =  %1.6f \r\n",pTrans->g01);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"g10 =  %1.6f \r\n",pTrans->g10);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"g11 =  %1.6f \r\n",pTrans->g11);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"h00 =  %1.6f \r\n",pTrans->h00);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"h01 =  %1.6f \r\n",pTrans->h01);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"h10 =  %1.6f \r\n",pTrans->h10);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;

			sprintf(message,"h11 =  %1.6f \r\n",pTrans->h11);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;
		}
	}
	UpdateData(FALSE);
}
