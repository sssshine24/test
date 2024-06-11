// ProjectToParameterDomain.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "ProjectToParameterDomain.h"
#include "Nurbs3D.h"
#include "math.h"
// ProjectToParameterDomain dialog

IMPLEMENT_DYNAMIC(ProjectToParameterDomain, CDialog)

ProjectToParameterDomain::ProjectToParameterDomain(CWnd* pParent /*=NULL*/)
	: CDialog(ProjectToParameterDomain::IDD, pParent)
{

}

ProjectToParameterDomain::~ProjectToParameterDomain()
{
}

void ProjectToParameterDomain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ProjectToParameterDomain, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ProjectToParameterDomain message handlers

void ProjectToParameterDomain::OnPaint()
{ // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CClientDC dc(this);
	double c00,c01,c02,c10,c11,c12,c20,c21,c22;
	//c00 = 0.24698302921186499;
	//c01 = 0.62297584374172787;
	//c02 = 1.4760161117762687;
	//c10 = 0.28587394469314059;
	//c11 = 0.78966837867148310;
	//c12 = 1.6847988763746118;
	//c20 = 0.30051277767074303;
	//c21 = 0.83061174771034052;
	//c22 = 1.6724291188053642;
	//c00 = 1.2746987047100420;
	//c01 = 2.4028286869769886;
	//c02 = 4.0023654276792318;
	//c10 = 0.89795851813650795;
	//c11 = 1.7132705413474636;
	//c12 = 2.9809116576249730;
	//c20 = 0.62284479059770148;
	//c21 = 1.2113805542290061;
	//c22 = 2.0663024666358623;
	//c00 = 1.378985;
	//c01 = 2.389813;
	//c02 = 4.360719;
	//c10 = 3.157263;
	//c11 = 5.169051;
	//c12 = 9.254374;
	//c20 = 6.688000;
	//c21 = 9.721905;
	//c22 = 19.213077;
	c00= 8.646948;
    c01= 2.530182;
    c02= 0.789364;
    c10= 27.925183;
    c11= 7.413554;
    c12= 2.460271;
    c20= 90.532127;
    c21= 23.007546;
    c22= 8.159436;
	char message[80];
	dc.MoveTo(10,10);
	dc.LineTo(510,10);
	dc.LineTo(510,510);
	dc.LineTo(10,510);
	dc.LineTo(10,10);

	//plot one curve.
	for(int i = 0; i <= 1000; i++)
	{
		double b = i * 0.001;
		double val = (c00*(1-b)*(1-b)+c01*(1-b)*b+c02*b*b)/(c20*(1-b)*(1-b)+c21*(1-b)*b+c22*b*b);
		double a = sqrt(val)/(1 + sqrt(val));
		if(i == 0)
			dc.MoveTo(a * 500 + 10,510 - b * 500);
		else
			dc.LineTo(a * 500 + 10,510 -  b * 500);
	}
	for(int i = 0; i <= 1000; i++)
	{
		double a = i * 0.001;
		double val = (c00*(1-a)*(1-a)+c10*(1-a)*a+c20*a*a)/(c02*(1-a)*(1-a)+c12*(1-a)*a+c22*a*a);
		double b = sqrt(val)/(1 + sqrt(val));
		if(i == 0)
			dc.MoveTo(a * 500 + 10,510 - b * 500);
		else
			dc.LineTo(a * 500 + 10,510 - b * 500);
	}


}
