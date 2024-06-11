// Actions_Bilinear.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Actions_Bilinear.h"
#include "C_NurbsSurf.h"
#include "Nurbs3D.h"
#include "BilinearTransformation.h"
#include "FormView3.h"
#include <fstream>
// Actions_Bilinear dialog

IMPLEMENT_DYNAMIC(Actions_Bilinear, CPropertyPage)

Actions_Bilinear::Actions_Bilinear()
	: CPropertyPage(Actions_Bilinear::IDD)
{

}

Actions_Bilinear::~Actions_Bilinear()
{
}

void Actions_Bilinear::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Actions_Bilinear, CPropertyPage)
	ON_BN_CLICKED(IDC_BILINEAR_OPTIMIZE, &Actions_Bilinear::OnBnClickedBilinearOptimize)
END_MESSAGE_MAP()


// Actions_Bilinear message handlers
double Actions_Bilinear::ComputeConformalEnergy(CNurbsSuface* pSurf)
{	
	//Compute the current conformal energy
	if(!pSurf->ptransformation)
		return 0;
	double totalenergy;
	if(pSurf->ptransformation->m_ReparameterizationType == bilinear)
	{
		BilinearTransformation* pbilineartrans = (BilinearTransformation*)pSurf->ptransformation;
		int m = 10;
		int n = 10;
		C_NurbsSurf* pSurface = pSurf->Converte_C_NurbsSurface();
		//sample the points in the u,v space
		double s,t,u,v;
		totalenergy = 0;
		for(int i = 0; i <= m; i++)
			for(int j = 0; j <= n; j++)
			{
				u = i * 1.0 / m;
				v = j * 1.0 / n;

				pbilineartrans->InverseEvaluation(u,v,s,t);
				Vector3D Xu = pSurface->ComputUDerivative1(u,v);
				Vector3D Xv = pSurface->ComputVDerivative1(u,v);
				Vector3D N = Xu ^ Xv;
				N.SetUnit();

				double us = pbilineartrans->Comput_u_s(s,t);
				double vs = pbilineartrans->Comput_v_s(s,t);
				double ut = pbilineartrans->Comput_u_t(s,t);
				double vt = pbilineartrans->Comput_v_t(s,t);

				Vector3D Xs = Xu * us + Xv * vs;
				Vector3D Xt = Xu * ut + Xv * vt;

				Vector3D newvec = N ^ Xs;
				double val = (newvec - Xt) * (newvec - Xt);

				//double energyorthogonality = SQR(Xs * Xt);
				//double energyuniformity = SQR(Xs * Xs - Xt * Xt);
				//double energy = energyorthogonality * orthogoanlityweight + energyuniformity * (1 - orthogoanlityweight);
				totalenergy += val;
			}
			//delete pSurface;
			//#ifdef _DEBUG
			//TRACE("Energy: %f", totalenergy);
			//#endif
	}

	return totalenergy;
}
void Actions_Bilinear::OnBnClickedBilinearOptimize()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\2013\\GeneralBilinearTransformations\\SVNNurbs\\data\\example1.sur");
	CNurbsSuface* pNurbs = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	C_NurbsSurf* pNurbsSurf = pNurbs->Converte_C_NurbsSurface();

	double minval = 1e10;
	double mina1,mina2,minb1,minb2;
	BilinearTransformation* pTrans = new BilinearTransformation();
	pTrans->m_ReparameterizationType = bilinear;
	pNurbs->ptransformation = pTrans;
	/*for(int i1 = 40; i1 < 60; i1++)
		for(int i2 = 50; i2 < 70; i2++)
			for(int j1 = 30; j1 < 50; j1++)
				for(int j2 = 60; j2 < 80; j2++)
				{
					double a1 = i1 * 0.01;
					double a2 = i2 * 0.01;
					double b1 = j1 * 0.01;
					double b2 = j2 * 0.01;

					pTrans->m_Bilinear_Alpha1 = a1;
					pTrans->m_Bilinear_Alpha2 = a2;
					pTrans->m_Bilinear_Beta1 = b1;
					pTrans->m_Bilinear_Beta2 = b2;
					
					double val = ComputeConformalEnergy(pNurbs);
					if(val < minval)
					{
						minval = val;
						mina1 = a1;
						mina2 = a2;
						minb1 = b1;
						minb2 = b2;
					}
					
				}*/
		mina1 = 0.46;
		mina2 = 0.62;
		minb1 = 0.37;
		minb2 = 0.68;
		pTrans->m_Bilinear_Alpha1 = mina1;
		pTrans->m_Bilinear_Alpha2 = mina2;
		pTrans->m_Bilinear_Beta1 = minb1;
		pTrans->m_Bilinear_Beta2 = minb2;
		p_FormView3->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
}