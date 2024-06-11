// Actions_TeichmullerMap.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Actions_HarmonicMapping.h"
#include "lib3d/Base3d.h"
#include "Lib3D/Mesh3d_OpenMesh.h"
#include "FormView3.h"
#include "SDIViewSwitchDoc.h"
#include "MySparseMatrix.h"
#include "Actions_TeichmullerMap.h"
#include "afxmt.h"

#define VERTEX_CONSTAINED 1
#define VERTEX_FREE 0
#define MULENGTH(v) (sqrt(SQR(v[0]) + SQR(v[1])))
using namespace std;
// Actions_TeichmullerMap dialog

IMPLEMENT_DYNAMIC(Actions_TeichmullerMap, CPropertyPage)
int Iterationstep = 1;
Actions_TeichmullerMap* pTeichmullerMap;
Actions_TeichmullerMap::Actions_TeichmullerMap()
	: CPropertyPage(Actions_TeichmullerMap::IDD)
{
	pTeichmullerMap = this;
}

Actions_TeichmullerMap::~Actions_TeichmullerMap()
{
}

void Actions_TeichmullerMap::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Actions_TeichmullerMap, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_TEICHMULLER, &Actions_TeichmullerMap::OnBnClickedButtonTeichmuller)
END_MESSAGE_MAP()


void Actions_TeichmullerMap::CheckBoundaryConditions(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected;
	FPROP_FACE_MU fprop_face_mu;

	//
	ComputeMu(pMesh3D);

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex Selected
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	std::pair<int,std::vector<double>> pair;

	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle()) = VERTEX_FREE;
	}
	//Set the constainted vertex as selected.
	for(int i = 0; i < pMesh3D->m_ArrayConstraints.size(); i++)
	{
		pair = pMesh3D->m_ArrayConstraints[i];
		pMesh3D->pmesh->property(vprop_vertex_selected,pMesh3D->pmesh->vertex_handle(pair.first)) = VERTEX_CONSTAINED;
	}
	
	//Set the vertice a non-boundary index 
	int m_interior_vertices_num = 0;
	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle());
		if(flag)
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle()) = -1;
		}else
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle()) = m_interior_vertices_num;
			m_interior_vertices_num++;
		}
	}

	MySparseMatrix A;
	std::vector<double> b;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	int rowindex = 0;
	double bval;

	for(int ii = 0; ii < 2; ii++)
	{
		A.m = m_interior_vertices_num ;
		A.n = m_interior_vertices_num ;
		
		A.Clear();
		b.clear();
		rowindex = 0;
		//all the linear equations
		for(MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			bval = 0;

			int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle());

			int constainedflag = pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle());

			//if(constainedflag)
			//	continue;

			for(MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(v_it.handle()); vf_it; vf_it++)
			{
				std::vector<double> muvec = pMesh3D->pmesh->property(fprop_face_mu,vf_it.handle());
				MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(vf_it.handle());

				int vhindex1,vhindex2,vhindex3;
				OpenMesh::Vec3d v_i = pMesh3D->pmesh->point(fv_it.handle());
				vhindex1 = fv_it.handle().idx();
				fv_it++;
				OpenMesh::Vec3d v_j = pMesh3D->pmesh->point(fv_it.handle());
				vhindex2 = fv_it.handle().idx();
				fv_it++;			
				OpenMesh::Vec3d v_k = pMesh3D->pmesh->point(fv_it.handle());
				vhindex3 = fv_it.handle().idx();

				double gi,hi,gj,hj,gk,hk;
				//double si,sj,sk;
				int nonboundaryindexi,nonboundaryindexj,nonboundaryindexk;
				std::vector<double> uvi,uvj,uvk;
				if(v_it.handle().idx() == vhindex1)
				{
					gi = v_i[0];
					hi = v_i[1];
					gj = v_j[0];
					hj = v_j[1];
					gk = v_k[0];
					hk = v_k[1];
					nonboundaryindexi = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex1));
					nonboundaryindexj = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex2));
					nonboundaryindexk = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex3));

					//get the uv of the constrained
					/*
					uvi = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex1));
					uvj = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex2));
					uvk = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex3));
					// */ 
					//*
					uvi = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhindex1));
					uvj = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhindex2));
					uvk = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhindex3));
					//*/
				}else
				if(v_it.handle().idx() == vhindex2)
				{
					gi = v_j[0];
					hi = v_j[1];
					gj = v_k[0];
					hj = v_k[1];
					gk = v_i[0];
					hk = v_i[1];
					nonboundaryindexi = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex2));
					nonboundaryindexj = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex3));
					nonboundaryindexk = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex1));
					/*
					uvi = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex2));
					uvj = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex3));
					uvk = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex1));
					//*/
					//*
					uvi = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhindex2));
					uvj = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhindex3));
					uvk = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhindex1));
					//*/
				}else
				if(v_it.handle().idx() == vhindex3)
				{
					gi = v_k[0];
					hi = v_k[1];
					gj = v_i[0];
					hj = v_i[1];
					gk = v_j[0];
					hk = v_j[1];
					nonboundaryindexi = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex3));
					nonboundaryindexj = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex1));
					nonboundaryindexk = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex2));
					/*
					uvi = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex3));
					uvj = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex1));
					uvk = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex2));
					//*/
					//*
					uvi = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhindex3));
					uvj = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhindex1));
					uvk = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhindex2));
					//*/
				}

				double AreaT = (gi * hj - hi * gj) + (gj * hk - hj * gk) + (gk * hi - hk * gi); 
				//AreaT = AreaT / 2;
				//AreaT = 1;

				double Ai = (hj - hk) / AreaT;
				double Aj = (hk - hi) / AreaT;
				double Ak = (hi - hj) / AreaT;

				double Bi = -(gj - gk) / AreaT;
				double Bj = -(gk - gi) / AreaT;
				double Bk = -(gi - gj) / AreaT;

				double rou = muvec[0];
				double tao = muvec[1];

				double denorm = (1 - SQR(rou) - SQR(tao));
				double a1 = (SQR(rou - 1) + SQR(tao)) / denorm;
				double a2 = -2 * tao / denorm;
				double a3 = (1 + 2 * rou + SQR(rou) + SQR(tao)) / denorm;

				double aa = Ai * a1 + Bi * a2;
				double bb = Ai * a2 + Bi * a3;
				double ci = aa * Ai + bb * Bi;
				double cj = aa * Aj + bb * Bj;
				double ck = aa * Ak + bb * Bk;

				double si = uvi[0];
				double sj = uvj[0];
				double sk = uvk[0];
				double ti = uvi[1];
				double tj = uvj[1];
				double tk = uvk[1];
				double a = Ai * si + Aj * sj + Ak * sk;
				double b = Bi * si + Bj * sj + Bk * sk;
				double c = Ai * ti + Aj * tj + Ak * tk;
				double d = Bi * ti + Bj * tj + Bk * tk;
				//a = d = 1;
				//b = c = 0;
				double temp = AreaT * (Ai * (a1 * a + a2 * b) + Bi * (a2 * a + a3 * b));
				TRACE(" %5.2f ",temp);
				bval += temp;
			}
			//ASSERT(fabs(bval) <1e-5);
			rowindex++;
			b.push_back(bval);
		}
				//*
		//Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A,b,uvarray[ii]);
	}
}

void Actions_TeichmullerMap::LoadConstraints()
{
	CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	
	//
	LoadConstraints(pMesh3D,"D:\\constraints.txt");
}
void Actions_TeichmullerMap::LoadConstraints(CMesh3d_OpenMesh* pMesh3D,char* filename)
{
	std::ifstream infile(filename);
	std::pair<int,std::vector<double>> pair;
	//CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	double x,y;
	while(1)
	{
		infile >> pair.first;
		if(pair.first == -1)
			break;
		infile >> x >> y;
		pair.second.clear();
		pair.second.push_back(x);
		pair.second.push_back(y);
		pMesh3D->m_ArrayConstraints.push_back(pair);
		pMesh3D->m_FixVerticesArray.push_back(pair.first);

	}
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> vec;
	for(int i = 0; i < pMesh3D->m_ArrayConstraints.size(); i++)
	{
		vec = pMesh3D->m_ArrayConstraints[i].second;
		pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(pMesh3D->m_ArrayConstraints[i].first)) = vec;
	}
}
// Actions_TeichmullerMap message handlers

CMesh3d_OpenMesh* Actions_TeichmullerMap::ComputeOptimalTeichurmullerMap(CMesh3d_OpenMesh* pOpenMesh,char* filename)
{
	//CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();
	//pOpenMesh->m_Transform.Copy(*pMesh3D->GetTransform());
	//pDoc->m_SceneGraph.Add(pOpenMesh);
	//Actions_HarmonicMapping::ComputeHarmonic_LinearConstraints(pOpenMesh);
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);

	//first fix the boundary
	//AddConstraints(pMesh3D);

	//
	pOpenMesh->m_Initialization_Linear_Equaiton = 0;

	LoadConstraints(pOpenMesh,filename);

	//
	InitilizeFaceMU(pOpenMesh);
	double maxmulength,minmulength,standarddeviation,averagemulength;
	int itenum = 0;
	double premaxmulength;
	int flag = 0;
	while(1)
	{
		//GetMaxMinmuLength(pOpenMesh,maxmulength,minmulength);
		//TRACE("Iteration: %f, %f , %5d\n",maxmulength,minmulength,itenum);

		//
		Compute_Linear_Equations(pOpenMesh);

		//return;

		//
		ComputeMu(pOpenMesh);

		//Generate a new parameter domain mesh
		//GenerateParameterDomain(pOpenMesh);
		//

		GetMaxMinmuLength(pOpenMesh,maxmulength,minmulength,averagemulength,standarddeviation);
		TRACE("Iter: %5d  Mu: %11.6f   %11.6f  %11.6f %11.6f \n",itenum,maxmulength,minmulength,averagemulength,standarddeviation);
		if(fabs(maxmulength - minmulength) < 0.01)
			break;

		//if(itenum > 0 && maxmulength > premaxmulength)
		//	break;

		premaxmulength = maxmulength;

		//
		UpdateMeshMu(pOpenMesh);

		itenum++;
		if(itenum > 1000)
			break;
	}

	//
	ComputeMu(pOpenMesh);	

	CMesh3d_OpenMesh* pNewMesh = GenerateParameterDomainMesh(pOpenMesh);

	return pNewMesh;
	//CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	//pDoc->m_SceneGraph.Add(pNewMesh);
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
}
void Actions_TeichmullerMap::OnBnClickedButtonTeichmuller()
{
	// TODO: Add your control notification handler code here
	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = 0;

	if(currentindex < 0 || currentindex >= num)
		return;

	if(pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();
		//pOpenMesh->m_FixVerticesArray.push_back(0);
		//pOpenMesh->m_FixVerticesArray.push_back(1);
		//pOpenMesh->m_FixVerticesArray.push_back(2);
		//pOpenMesh->m_FixVerticesArray.push_back(3);
		//pOpenMesh->m_FixVerticesArray.push_back(4);
		pOpenMesh->m_Transform.Copy(*pMesh3D->GetTransform());
		pDoc->m_SceneGraph.Add(pOpenMesh);
		//Actions_HarmonicMapping::ComputeHarmonic_LinearConstraints(pOpenMesh);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	}//else
	//if(pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
		
		VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
		VPROP_VERTEX_UV vprop_vertex_uv;

		//Vertex selected flag;
		if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
			pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

		//Vertex UV
		if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
			pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

		//first fix the boundary
		//AddConstraints(pMesh3D);
		LoadConstraints();

		//
		InitilizeFaceMU(pMesh3D);
		double maxmulength,minmulength;
		int itenum = 0;
		//GetMaxMinmuLength(pMesh3D,maxmulength,minmulength);
		//double a,b;
		/*for(int i = 0; i <= 100; i++)
		{
			AddConstraints(pMesh3D);

			//
			InitilizeFaceMU(pMesh3D);
			double t = i * 0.01;
			double len = minmulength * (1 - t) + maxmulength * t;
			SetMeshMu(pMesh3D,len);
			Compute_Linear_Equations(pMesh3D);
			SetMeshMu(pMesh3D,len);
			Compute_Linear_Equations(pMesh3D);

			GetMaxMinmuLength(pMesh3D,a,b);
			TRACE("%f, %f, %f, %d\n",len, a,b,itenum);
			itenum++;
		}*/
		//
		//CheckBoundaryConditions(pMesh3D);

		//return;
		//Check
		//CheckEquations(pMesh3D);

		//return;
		//Obtain the linear equations

		int flag = 0;
		double averagemulength,standarddeviation;
		while(1)
		{
			GetMaxMinmuLength(pMesh3D,maxmulength,minmulength,averagemulength,standarddeviation);
			TRACE("Iteration: %f, %f , %5d\n",maxmulength,minmulength,itenum);

			//
			Compute_Linear_Equations(pMesh3D);

			//return;
			
			//
			//ComputeMu(pMesh3D);

			//
			GetMaxMinmuLength(pMesh3D,maxmulength,minmulength,averagemulength,standarddeviation);
			//TRACE("Mu: %f, %f\n",maxmulength,minmulength);
			if(fabs(maxmulength - minmulength) < 0.01)
				break;

			//
			UpdateMeshMu(pMesh3D);

			itenum++;
			if(itenum > 1000)
				break;
		}

		//
		ComputeMu(pMesh3D);

		//Generate a new parameter domain mesh
		CMesh3d_OpenMesh* pNewMesh = GenerateParameterDomainMesh(pMesh3D);
		CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
		pDoc->m_SceneGraph.Add(pNewMesh);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}

}
void Actions_TeichmullerMap::GetMaxMinmuLength(CMesh3d_OpenMesh* pMesh3D,double &maxmulength,double &minmulength,double &averagemulength,double &standarddeviation)
{
	FPROP_FACE_MU fprop_face_mu;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_flag;
	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex Selected
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_flag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_flag, PROP_VERTEX_SELECTED);

	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> vec;
	maxmulength = -1;
	minmulength = 1000;
	std::vector<double> x,y,u,v;
	std::vector<int> m_vindexarray;
	std::vector<double> m_mu;
	double currentmulength;
	averagemulength = GetAverageMeshMuModule(pMesh3D);
	standarddeviation = 0;
	int facenum = 0;
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		if(IsFaceFixed(pMesh3D,f_it.handle()))
			continue;

		m_mu = pMesh3D->pmesh->property(fprop_face_mu,f_it.handle());

		double currentmulength = MULENGTH(m_mu);

		standarddeviation += SQR(currentmulength - averagemulength);
		facenum++;

		if(currentmulength > maxmulength)
			maxmulength = currentmulength;
		if(currentmulength < minmulength)
			minmulength = currentmulength;
	}
	standarddeviation = sqrt(standarddeviation / facenum);
}
void Actions_TeichmullerMap::SetMeshMu(CMesh3d_OpenMesh* pMesh3D,double mumou)
{
	FPROP_FACE_MU fprop_face_mu;
	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	double x = 0;
	double y = 0;
	std::vector<double> vec;
	int facenum = pMesh3D->pmesh->n_faces();
	double totalmulength = 0;
	std::vector<std::vector<double>> newmuarray;

	//double averagemulength = GetAverageMeshMuModule(pMesh3D);

	double step = 0.1;
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		vec = pMesh3D->pmesh->property(fprop_face_mu,f_it.handle());
		double currentmulength = MULENGTH(vec);
		if(currentmulength > 1e-5)
		{
			double newmulength = mumou;
			double ratio = newmulength / currentmulength;
			vec[0] = vec[0] * ratio;
			vec[1] = vec[1] * ratio;
			pMesh3D->pmesh->property(fprop_face_mu,f_it.handle()) = vec;
		}
	}
}
void Actions_TeichmullerMap::ScaleMu(CMesh3d_OpenMesh* pMesh3D, double scale)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	std::vector<double> m_mu;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		if (IsFaceFixed(pMesh3D, f_it.handle()))
			continue;

		m_mu = pMesh3D->pmesh->property(fprop_face_mu, f_it.handle());
		m_mu[0] = m_mu[0] * scale;
		m_mu[1] = m_mu[1] * scale;
		pMesh3D->pmesh->property(fprop_face_mu, f_it.handle()) = m_mu;
	}
}
void Actions_TeichmullerMap::ComputeMu(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> x,y,u,v;
	double minfaimou = 1e10;
	double maxfaimou = -1e10;
	std::vector<double> m_mu;
	std::vector<int> m_vindexarray;
	double maximalmulength = -1;
	int maxmufaceindex;
	double totalmulength = 0;
	int facenum = 0;
	for(MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it!= pMesh3D->pmesh->faces_end(); ++f_it)
	{
		if(IsFaceFixed(pMesh3D,f_it.handle()))
			continue;

		x.clear();
		y.clear();
		u.clear();
		v.clear();
		m_vindexarray.clear();
		for(MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle());fv_it;++fv_it)
		{
			std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv,fv_it.handle());
			OpenMesh::Vec3d pt;
			pt = pMesh3D->pmesh->point (fv_it.handle());
			//x.push_back(Action_Levy_Leastsquare::ComputeCoefficient(pMesh3D,f_it.handle(),fv_it.handle(),0));
			//y.push_back(Action_Levy_Leastsquare::ComputeCoefficient(pMesh3D,f_it.handle(),fv_it.handle(),1));
			x.push_back(pt[0]);
			y.push_back(pt[1]);
			u.push_back(uv[0]);
			v.push_back(uv[1]);
			m_vindexarray.push_back(fv_it.handle().idx());
		}/*
		if(std::find(m_vindexarray.begin(),m_vindexarray.end(),52434) != m_vindexarray.end() && 
			std::find(m_vindexarray.begin(),m_vindexarray.end(),52415) != m_vindexarray.end() &&
			std::find(m_vindexarray.begin(),m_vindexarray.end(),1) != m_vindexarray.end())
		{
			int ij = 0;
		}//*/
		double dT = (x[0]*y[1] - y[0] * x[1]) + (x[1] * y[2] - y[1] * x[2]) + (x[2] * y[0] - y[2] * x[0]); 
		double partialu_partialx = (u[0] * (y[1] - y[2]) + u[1] * (y[2] - y[0]) + u[2] * (y[0] - y[1]))/dT;
		double partialu_partialy = (u[0] * (x[2] - x[1]) + u[1] * (x[0] - x[2]) + u[2] * (x[1] - x[0]))/dT;
		double partialv_partialx = (v[0] * (y[1] - y[2]) + v[1] * (y[2] - y[0]) + v[2] * (y[0] - y[1]))/dT;
		double partialv_partialy = (v[0] * (x[2] - x[1]) + v[1] * (x[0] - x[2]) + v[2] * (x[1] - x[0]))/dT;

		double partialdzbar_real = (partialu_partialx - partialv_partialy) / 2;
		double partialdzbar_imaginary = (partialv_partialx + partialu_partialy) / 2;

		double partialdz_real = (partialu_partialx + partialv_partialy) / 2;
		double partialdz_imaginary = (partialv_partialx - partialu_partialy) / 2;

		double denorminator = SQR(partialdz_real) + SQR(partialdz_imaginary);
		double fai_real  = (partialdzbar_real * partialdz_real + partialdzbar_imaginary * partialdz_imaginary)/denorminator;
		double fai_imaginary = (partialdzbar_imaginary * partialdz_real - partialdzbar_real * partialdz_imaginary)/denorminator;
		double fai_mou = sqrt(SQR(fai_real) + SQR(fai_imaginary));
		//TRACE(" %f ",fai_mou);
		totalmulength += fai_mou;
		if(fai_mou > maximalmulength)
		{
			maximalmulength = fai_mou;
			maxmufaceindex = f_it.handle().idx();
		}
		m_mu.clear();
		m_mu.push_back(fai_real);
		m_mu.push_back(fai_imaginary);
		pMesh3D->pmesh->property(fprop_face_mu,f_it.handle()) = m_mu;
		facenum++;
	}
	//TRACE(" \n ");


	//m_mu = pMesh3D->pmesh->property(fprop_face_mu,pMesh3D->pmesh->face_handle(6));
	//m_mu[0] = m_mu[0] * 0.8;
	//m_mu[1] = m_mu[1] * 0.8;
	//pMesh3D->pmesh->property(fprop_face_mu,pMesh3D->pmesh->face_handle(6)) = m_mu;
	//m_mu = pMesh3D->pmesh->property(fprop_face_mu,pMesh3D->pmesh->face_handle(7));
	//m_mu[0] = m_mu[0] * 0.8;
	//m_mu[1] = m_mu[1] * 0.8;
	//pMesh3D->pmesh->property(fprop_face_mu,pMesh3D->pmesh->face_handle(7)) = m_mu;
}
void Actions_TeichmullerMap::UpdateMeshMu(CMesh3d_OpenMesh* pMesh3D)
{
	//
	//ComputeMu(pMesh3D);

	//
	//for(int i = 0; i < 5; i++)
	//LaplaceSmoothMeshMu(pMesh3D);
	//double step = SearchForOptimalStep(pMesh3D);
	//SearchForOptimalStep_MultiThread(pMesh3D);
	//
	//GetMeshCurrentUV();
	ModifyFaceMubyStep(pMesh3D,1.0);
	//ComputePara
	//GetMAXIAMAL
	//SetMeshCurrentUV();
	//AverageMeshMu(pMesh3D,1);
	//GetMAXIAMAL
}
void Actions_TeichmullerMap::ModifyFaceMubyStep(CMesh3d_OpenMesh* pMesh3D,double step)
{
	FPROP_FACE_MU fprop_face_mu;
	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	VPROP_VERTEX_MU vprop_vertex_mu;

	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	std::vector<double> vec;
	double averagemulength = GetAverageMeshMuModule(pMesh3D);

	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		vec = pMesh3D->pmesh->property(fprop_face_mu,f_it.handle());
		double currentmulength = MULENGTH(vec);
		if(currentmulength > 1e-5)
		{
			double newmulength = currentmulength + (averagemulength - currentmulength) * step;
			double ratio = newmulength / currentmulength;
			vec[0] = vec[0] * ratio;
			vec[1] = vec[1] * ratio;
			pMesh3D->pmesh->property(fprop_face_mu,f_it.handle()) = vec;
		}
	}

	double mux,muy;
	int num;
	//double totalmulength = 0;
	//int vnum = 0;
	std::vector<double> muvec;
	for(MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it!= pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if(pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;
		mux = 0;
		muy = 0;
		num = 0;
		for(MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(v_it.handle());vf_it;vf_it++)
		{
			std::vector<double> mu = pMesh3D->pmesh->property(fprop_face_mu,vf_it.handle());
			if(mu.size() == 2)
			{
				mux += mu[0];
				muy += mu[1];
				num++;
			}
		}
		mux /= num;
		muy /= num;
		muvec.clear();
		muvec.push_back(mux);
		muvec.push_back(muy);
		//totalmulength += sqrt(SQR(mux) + SQR(muy));
		pMesh3D->pmesh->property(vprop_vertex_mu,v_it.handle()) = muvec;
		//vnum++;
	}	
}
double Actions_TeichmullerMap::SearchForOptimalStep_MultiThread(CMesh3d_OpenMesh* pMesh3D)
{
	Thread_Structure* pThreadStruct[11];
	CWinThread* g_pThread[11];
	double m_values[11];
	//m_values.resize(11);
	for(int i = 0; i <= 10; i++)
		m_values[i] = -1;
	for(int i = 0; i <= 10; i++)
	{
		pThreadStruct[i] = new Thread_Structure();
		pThreadStruct[i]->pMesh = pMesh3D;
		pThreadStruct[i]->m_CurrentStep = i * 0.1;
		pThreadStruct[i]->m_ReturnResult = &m_values[i];
		g_pThread[i] = AfxBeginThread(SearchForOptimalStep_Iteration, 
			(LPVOID)pThreadStruct[i],
			THREAD_PRIORITY_HIGHEST,
			0,
			CREATE_SUSPENDED);
		g_pThread[i]->m_bAutoDelete = TRUE;
		g_pThread[i]->ResumeThread();
	}

	for(int i = 0; i <= 10; i++)
	{
		//DWORD code;
		//GetExitCodeThread(g_pThread[i], &code);
		if (m_values[i] < 0)
		{
			Sleep(5000);
			i--;
		}
	}
	for(int i = 0; i <= 10; i++)
	{
		delete pThreadStruct[i];
	}
	double minvalue = 1e10;
	int minindex;
	for(int i = 0; i <= 10; i++)
	{
		if(m_values[i] < minvalue)
		{
			minvalue = m_values[i];
			minindex = i;
		}
	}
	return minindex * 0.1;
}
UINT Actions_TeichmullerMap::SearchForOptimalStep_Iteration(LPVOID lpParam)
{
	double minmulength,averagemulength,standarddeviation;
	Thread_Structure* pThreadStruct = (Thread_Structure*)lpParam;

	CMesh3d_OpenMesh* pNewMesh = pThreadStruct->pMesh->copy();
	ModifyFaceMubyStep(pNewMesh,pThreadStruct->m_CurrentStep);

	//
	Compute_Linear_Equations(pNewMesh);

	//
	ComputeMu(pNewMesh);

	//
	GetMaxMinmuLength(pNewMesh,*pThreadStruct->m_ReturnResult,minmulength,averagemulength,standarddeviation);

	delete pNewMesh;

	/*double aa = (minimalmuindex - 1) * 0.1;
	double bb = (minimalmuindex + 1) * 0.1;
	minimalmulength = 1e10;
	maxmuarray.clear();
	minmuarray.clear();
	for(int ii = 0; ii <= 20; ii++)
	{
		CMesh3d_OpenMesh* pNewMesh = pMesh3D->copy();
		ModifyFaceMubyStep(pNewMesh,aa + 0.01 * ii);

		//
		Compute_Linear_Equations(pNewMesh,1);

		//
		ComputeMu(pNewMesh);

		//
		GetMaxMinmuLength(pNewMesh,maxmulength,minmulength);

		maxmuarray.push_back(maxmulength);

		minmuarray.push_back(minmulength);

		if(maxmulength < minimalmulength )
		{
			minimalmulength = maxmulength;
			minimalmuindex = ii;
		}

		delete pNewMesh;
	}
	return minimalmuindex * 0.01 + aa;	*/
	//return minimalmuindex * 0.1;
	return 1;
}
double Actions_TeichmullerMap::SearchForOptimalStep(CMesh3d_OpenMesh* pMesh3D)
{
	double maxmulength,minmulength,averagemulength,standarddeviation;
	std::vector<double> maxmuarray,minmuarray;
	double minimalmulength = 1e10;
	int minimalmuindex;
	for(int i = 0; i <= 10; i++)
	{
		CMesh3d_OpenMesh* pNewMesh = pMesh3D->copy();
		ModifyFaceMubyStep(pNewMesh,i * 0.1);

		//
		Compute_Linear_Equations(pNewMesh);

		//
		ComputeMu(pNewMesh);

		//
		GetMaxMinmuLength(pNewMesh,maxmulength,minmulength,averagemulength,standarddeviation);

		maxmuarray.push_back(maxmulength);

		minmuarray.push_back(minmulength);

		if(maxmulength < minimalmulength )
		{
			minimalmulength = maxmulength;
			minimalmuindex = i;
		}

		delete pNewMesh;
	}
	/*double aa = (minimalmuindex - 1) * 0.1;
	double bb = (minimalmuindex + 1) * 0.1;
	minimalmulength = 1e10;
	maxmuarray.clear();
	minmuarray.clear();
	for(int ii = 0; ii <= 20; ii++)
	{
		CMesh3d_OpenMesh* pNewMesh = pMesh3D->copy();
		ModifyFaceMubyStep(pNewMesh,aa + 0.01 * ii);

		//
		Compute_Linear_Equations(pNewMesh,1);

		//
		ComputeMu(pNewMesh);

		//
		GetMaxMinmuLength(pNewMesh,maxmulength,minmulength);

		maxmuarray.push_back(maxmulength);

		minmuarray.push_back(minmulength);

		if(maxmulength < minimalmulength )
		{
			minimalmulength = maxmulength;
			minimalmuindex = ii;
		}

		delete pNewMesh;
	}
	return minimalmuindex * 0.01 + aa;	*/
	return minimalmuindex * 0.1;
}
double Actions_TeichmullerMap::GetAverageMeshMuModule(CMesh3d_OpenMesh* pMesh3D)
{
	FPROP_FACE_MU fprop_face_mu;
	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	double x = 0;
	double y = 0;
	std::vector<double> vec;
	int facenum = 0;
	double totalmulength = 0;
	std::vector<std::vector<double>> newmuarray;
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		if(IsFaceFixed(pMesh3D,f_it.handle()))
			continue;

		vec = pMesh3D->pmesh->property(fprop_face_mu,f_it.handle());
		totalmulength += sqrt(SQR(vec[0]) + SQR(vec[1]));
		facenum++;
	}
	double averagemulength = totalmulength / facenum;
	return averagemulength;
}
BOOL Actions_TeichmullerMap::IsFaceFixed(CMesh3d_OpenMesh* pMesh3D,OpenMesh::FaceHandle fh)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_flag;

	//Vertex Selected
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_flag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_flag, PROP_VERTEX_SELECTED);

	MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(fh);
	int vflag1 = pMesh3D->pmesh->property(vprop_vertex_flag,fv_it.handle());
	fv_it++;
	int vflag2 = pMesh3D->pmesh->property(vprop_vertex_flag,fv_it.handle());
	fv_it++;
	int vflag3 = pMesh3D->pmesh->property(vprop_vertex_flag,fv_it.handle());
	if(vflag1 && vflag2 && vflag3)
		return 1;
	else
		return 0;
}
void Actions_TeichmullerMap::AverageMeshMu(CMesh3d_OpenMesh* pMesh3D,double step)
{
	FPROP_FACE_MU fprop_face_mu;
	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	double x = 0;
	double y = 0;
	std::vector<double> vec;
	int facenum = pMesh3D->pmesh->n_faces();
	double totalmulength = 0;
	std::vector<std::vector<double>> newmuarray;
	
	double averagemulength = GetAverageMeshMuModule(pMesh3D);
	
	//double step = 0.1;
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		vec = pMesh3D->pmesh->property(fprop_face_mu,f_it.handle());
		double currentmulength = MULENGTH(vec);
		if(currentmulength > 1e-5)
		{
			double newmulength = currentmulength + (averagemulength - currentmulength) * step;
			double ratio = newmulength / currentmulength;
			vec[0] = vec[0] * ratio;
			vec[1] = vec[1] * ratio;
			pMesh3D->pmesh->property(fprop_face_mu,f_it.handle()) = vec;
		}
	}
}
void Actions_TeichmullerMap::SetZeroMu(CMesh3d_OpenMesh* pMesh3D)
{
	FPROP_FACE_MU fprop_face_mu;
	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	std::vector<double> vec;
	vec.push_back(0);
	vec.push_back(0);
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		pMesh3D->pmesh->property(fprop_face_mu,f_it.handle()) = vec;
	}
}
void Actions_TeichmullerMap::LaplaceSmoothMeshMu(CMesh3d_OpenMesh* pMesh3D)
{
	FPROP_FACE_MU fprop_face_mu;
	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	double x = 0;
	double y = 0;
	std::vector<double> vec;
	int num = 0;
	std::vector<std::vector<double>> newmuarray;
	std::vector<int> faceidxarray;
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		num = 0;
		x = y = 0;

		faceidxarray.clear();
		for(MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
		{
			for(MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(fv_it.handle()); vf_it; vf_it++)
			{
				int idx = vf_it.handle().idx();
				if(std::find(faceidxarray.begin(),faceidxarray.end(),idx) == faceidxarray.end())
					faceidxarray.push_back(idx);
			}
		}
		for(int i = 0; i < faceidxarray.size(); i++)
		{
			vec = pMesh3D->pmesh->property(fprop_face_mu,pMesh3D->pmesh->face_handle(faceidxarray[i]));
			x += vec[0];
			y += vec[1];
			num++;
		}
		x /= num;
		y /= num;
		vec.clear();
		vec.push_back(x);
		vec.push_back(y);
		newmuarray.push_back(vec);
	}
	num = 0;
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		pMesh3D->pmesh->property(fprop_face_mu,f_it.handle()) = newmuarray[num++];
	}
}
void Actions_TeichmullerMap::CheckEquations(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected;
	FPROP_FACE_MU fprop_face_mu;

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex Selected
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	std::pair<int,std::vector<double>> pair;

	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle()) = VERTEX_FREE;
	}
	//Set the constainted vertex as selected.
	for(int i = 0; i < pMesh3D->m_ArrayConstraints.size(); i++)
	{
		pair = pMesh3D->m_ArrayConstraints[i];
		pMesh3D->pmesh->property(vprop_vertex_selected,pMesh3D->pmesh->vertex_handle(pair.first)) = VERTEX_CONSTAINED;
	}
	
	//Set the vertice a non-boundary index 
	int m_interior_vertices_num = 0;
	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle());
		if(flag)
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle()) = -1;
		}else
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle()) = m_interior_vertices_num;
			m_interior_vertices_num++;
		}
	}

	MySparseMatrix A;
	std::vector<double> b;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	int rowindex = 0;
	double bval;

	for(int ii = 0; ii < 2; ii++)
	{
		A.m = m_interior_vertices_num ;
		A.n = m_interior_vertices_num ;
		
		A.Clear();
		b.clear();
		rowindex = 0;
		//all the linear equations
		for(MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			bval = 0;

			int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle());

			int constainedflag = pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle());

			if(constainedflag)
				continue;

			for(MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(v_it.handle()); vf_it; vf_it++)
			{
				std::vector<double> muvec = pMesh3D->pmesh->property(fprop_face_mu,vf_it.handle());
				MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(vf_it.handle());

				int vhindex1,vhindex2,vhindex3;
				OpenMesh::Vec3d v_i = pMesh3D->pmesh->point(fv_it.handle());
				vhindex1 = fv_it.handle().idx();
				fv_it++;
				OpenMesh::Vec3d v_j = pMesh3D->pmesh->point(fv_it.handle());
				vhindex2 = fv_it.handle().idx();
				fv_it++;			
				OpenMesh::Vec3d v_k = pMesh3D->pmesh->point(fv_it.handle());
				vhindex3 = fv_it.handle().idx();

				double gi,hi,gj,hj,gk,hk;
				//double si,sj,sk;
				int nonboundaryindexi,nonboundaryindexj,nonboundaryindexk;
				OpenMesh::Vec3d uvi,uvj,uvk;
				if(v_it.handle().idx() == vhindex1)
				{
					gi = v_i[0];
					hi = v_i[1];
					gj = v_j[0];
					hj = v_j[1];
					gk = v_k[0];
					hk = v_k[1];
					nonboundaryindexi = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex1));
					nonboundaryindexj = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex2));
					nonboundaryindexk = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex3));

					//get the uv of the constrained
					
					uvi = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex1));
					uvj = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex2));
					uvk = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex3));
					
				}else
				if(v_it.handle().idx() == vhindex2)
				{
					gi = v_j[0];
					hi = v_j[1];
					gj = v_k[0];
					hj = v_k[1];
					gk = v_i[0];
					hk = v_i[1];
					nonboundaryindexi = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex2));
					nonboundaryindexj = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex3));
					nonboundaryindexk = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex1));
					
					uvi = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex2));
					uvj = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex3));
					uvk = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex1));
				}else
				if(v_it.handle().idx() == vhindex3)
				{
					gi = v_k[0];
					hi = v_k[1];
					gj = v_i[0];
					hj = v_i[1];
					gk = v_j[0];
					hk = v_j[1];
					nonboundaryindexi = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex3));
					nonboundaryindexj = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex1));
					nonboundaryindexk = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(vhindex2));
					uvi = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex3));
					uvj = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex1));
					uvk = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vhindex2));
				}

				double AreaT = (gi * hj - hi * gj) + (gj * hk - hj * gk) + (gk * hi - hk * gi); 
				//AreaT = AreaT / 2;
				//AreaT = 1;

				double Ai = (hj - hk) / AreaT;
				double Aj = (hk - hi) / AreaT;
				double Ak = (hi - hj) / AreaT;

				double Bi = -(gj - gk) / AreaT;
				double Bj = -(gk - gi) / AreaT;
				double Bk = -(gi - gj) / AreaT;

				double rou = muvec[0];
				double tao = muvec[1];

				double denorm = (1 - SQR(rou) - SQR(tao));
				double a1 = (SQR(rou - 1) + SQR(tao)) / denorm;
				double a2 = -2 * tao / denorm;
				double a3 = (1 + 2 * rou + SQR(rou) + SQR(tao)) / denorm;

				double aa = Ai * a1 + Bi * a2;
				double bb = Ai * a2 + Bi * a3;
				double ci = aa * Ai + bb * Bi;
				double cj = aa * Aj + bb * Bj;
				double ck = aa * Ak + bb * Bk;

				double si = uvi[0];
				double sj = uvj[0];
				double sk = uvk[0];
				double ti = uvi[1];
				double tj = uvj[1];
				double tk = uvk[1];
				double a = Ai * si + Aj * sj + Ak * sk;
				double b = Bi * si + Bj * sj + Bk * sk;
				double c = Ai * ti + Aj * tj + Ak * tk;
				double d = Bi * ti + Bj * tj + Bk * tk;
				//a = d = 1;
				//b = c = 0;
				double temp = AreaT * (Ai * (a1 * a + a2 * b) + Bi * (a2 * a + a3 * b));
				TRACE(" %5.2f ",temp);
				bval += temp;
			}
			ASSERT(fabs(bval) <1e-5);
			rowindex++;
			b.push_back(bval);
		}
				//*
		//Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A,b,uvarray[ii]);
	}
}
void Actions_TeichmullerMap::InitilizeVertexUV(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uv;
	OpenMesh::Vec3d pos;
	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pos = pMesh3D->pmesh->point(v_it.handle());
		uv.clear();
		uv.push_back(pos[0]);
		uv.push_back(pos[1]);
		pMesh3D->pmesh->property(vprop_vertex_uv,v_it.handle()) = uv;
	}
	std::pair<int,std::vector<double>> pair;
	for(int i = 0; i < pMesh3D->m_ArrayConstraints.size(); i++)
	{
		pair = pMesh3D->m_ArrayConstraints[i];
		int vhandleidx = pair.first;
		pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(vhandleidx)) = pair.second;
	}
}
void Actions_TeichmullerMap::InitilizeFaceMU(CMesh3d_OpenMesh* pMesh3D)
{	
	FPROP_FACE_MU fprop_face_mu;

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//
	InitilizeVertexUV(pMesh3D);

	//
	SetZeroMu(pMesh3D);
	//InitilizeFaceMU(pMesh3D);
	//ComputeMu(pMesh3D);
	return;

	//
	double averagemu = GetAverageMeshMuModule(pMesh3D);

	LaplaceSmoothMeshMu(pMesh3D);

	LaplaceSmoothMeshMu(pMesh3D);
	/*std::vector<double> vec;
	vec.push_back(0);
	vec.push_back(0);
	double step = 0.1;
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		vec = pMesh3D->pmesh->property(fprop_face_mu,f_it.handle());
		double currentlength = MULENGTH(vec);
		double newlength = currentlength + (averagemu - currentlength) * step;
		double ratio = newlength / currentlength;
		vec[0] = vec[0] * ratio;
		vec[1] = vec[1] * ratio;
		pMesh3D->pmesh->property(fprop_face_mu,f_it.handle()) = vec;
	}*/
}
void Actions_TeichmullerMap::Compute_Boundary_Vertices_Parameter_Positions(CMesh3d_OpenMesh* pMesh3D)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<MyMesh::VertexHandle> m_ArraySeletedVertices;
	/*for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_selectedflag,v_it.handle());
		if(flag)
		{
			m_ArraySeletedVertices.push_back(v_it.handle());
			//OpenMesh::Vec3d pt;
			//pt = pmesh->point (v_it);
			//::glVertex3f(pt[0],pt[1],pt[2]);
		}
	}*/
	pMesh3D->m_FixVerticesArray.clear();
	pMesh3D->m_FixVerticesArray.push_back(0);
	pMesh3D->m_FixVerticesArray.push_back(1);
	pMesh3D->m_FixVerticesArray.push_back(2);
	pMesh3D->m_FixVerticesArray.push_back(3);
	//pMesh3D->m_FixVerticesArray.push_back(4);
	//pMesh3D->m_FixVerticesArray.push_back(5);
	//pMesh3D->m_FixVerticesArray.push_back(6);
	//pMesh3D->m_FixVerticesArray.push_back(7);
	for(int i = 0; i < pMesh3D->m_FixVerticesArray.size(); i++)
	{
		m_ArraySeletedVertices.push_back(MyMesh::VertexHandle(pMesh3D->m_FixVerticesArray[i]));
	}
	std::vector<MyMesh::VertexHandle> m_ArrarBoundaryVertices;
	//store all the vertices
	MyMesh::HalfedgeHandle he1 = pMesh3D->pmesh->halfedge_handle(m_ArraySeletedVertices[0]);
	MyMesh::HalfedgeHandle he2;
	m_ArrarBoundaryVertices.push_back(m_ArraySeletedVertices[0]);
	MyMesh::VertexHandle prevertexhandle =  m_ArraySeletedVertices[0];
	MyMesh::VertexHandle nextvertexhandle;
	while(!pMesh3D->pmesh->is_boundary(he1)) 
	{
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
		he1 = pMesh3D->pmesh->opposite_halfedge_handle(he1);
	}
	he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	while(1)
	{
		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->from_vertex_handle(he1);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->to_vertex_handle(he1);
		if(vh1 == prevertexhandle)
		{
			nextvertexhandle = vh2;
		}
		else
		{
			nextvertexhandle = vh1;
		}
		if(nextvertexhandle == m_ArrarBoundaryVertices[0])
			break;
		m_ArrarBoundaryVertices.push_back(nextvertexhandle);
		prevertexhandle = nextvertexhandle;
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	}

	std::vector<int> m_ArraySeletedVerticesIndex;
	//m_ArraySeletedVerticesIndex.push_back()
		//Find the index of the corner vertices
		for(int j = 0; j < m_ArraySeletedVertices.size(); j++)
		{
			for(int i = 0; i < m_ArrarBoundaryVertices.size(); i++)
			{
				if(m_ArraySeletedVertices[j] == m_ArrarBoundaryVertices[i])
				{
					m_ArraySeletedVerticesIndex.push_back(i);
					break;
				}
			}
		}

		//Sort the boundaries
		for(int i = 0; i < m_ArraySeletedVerticesIndex.size(); i++)
			for(int j = 0; j < m_ArraySeletedVerticesIndex.size() - 1; j++)
			{
				if(m_ArraySeletedVerticesIndex[j] > m_ArraySeletedVerticesIndex[j + 1])
				{
					int temp = m_ArraySeletedVerticesIndex[j];
					m_ArraySeletedVerticesIndex[j] = m_ArraySeletedVerticesIndex[j + 1];
					m_ArraySeletedVerticesIndex[j + 1] = temp;

					MyMesh::VertexHandle temph = m_ArraySeletedVertices[j];
					m_ArraySeletedVertices[j] = m_ArraySeletedVertices[j + 1];
					m_ArraySeletedVertices[j + 1] = temph;
				}
			}

			//First set the parameterization for the boundary vertices

			std::vector<double> vec1;
			vec1.push_back(0.8);
			vec1.push_back(-0.2);
			std::vector<double> vec2;
			vec2.push_back(1);
			vec2.push_back(0);
			std::vector<double> vec3;
			vec3.push_back(1);
			vec3.push_back(1);
			std::vector<double> vec4;
			vec4.push_back(0.8);
			vec4.push_back(2);
			/*std::vector<double> vec5;
			vec5.push_back(0.3);
			vec5.push_back(0.3);
			std::vector<double> vec6;
			vec6.push_back(0.7);
			vec6.push_back(0.3);
			std::vector<double> vec7;
			vec7.push_back(0.7);
			vec7.push_back(0.7);
			std::vector<double> vec8;
			vec8.push_back(0.3);
			vec8.push_back(0.7);*/
			//
			pMesh3D->m_ArrayConstraints.clear();
			std::pair<int,std::vector<double>> constraint;
			constraint.first = m_ArraySeletedVertices[0].idx();
			constraint.second = vec1;
			pMesh3D->m_ArrayConstraints.push_back(constraint);

			constraint.first = m_ArraySeletedVertices[1].idx();
			constraint.second = vec2;
			pMesh3D->m_ArrayConstraints.push_back(constraint);

			constraint.first = m_ArraySeletedVertices[2].idx();
			constraint.second = vec3;
			pMesh3D->m_ArrayConstraints.push_back(constraint);

			constraint.first = m_ArraySeletedVertices[3].idx();
			constraint.second = vec4;
			pMesh3D->m_ArrayConstraints.push_back(constraint);

			/*constraint.first = m_ArraySeletedVertices[4].idx();
			constraint.second = vec5;
			m_ArrayConstraints.push_back(constraint);

			constraint.first = m_ArraySeletedVertices[5].idx();
			constraint.second = vec6;
			m_ArrayConstraints.push_back(constraint);

			constraint.first = m_ArraySeletedVertices[6].idx();
			constraint.second = vec7;
			m_ArrayConstraints.push_back(constraint);

			constraint.first = m_ArraySeletedVertices[7].idx();
			constraint.second = vec8;
			m_ArrayConstraints.push_back(constraint);*/
			//set uv
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[0]) = vec1;
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[1]) = vec2;
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[2]) = vec3;
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[3]) = vec4;	
			//pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[4]) = vec5;	
			//pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[5]) = vec6;
			//pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[6]) = vec7;	
			//pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[7]) = vec8;	
			return;


			//Set the other boundary vertices
			//uniform 
			if(((CButton *)GetDlgItem(IDC_TEICHMULLER_BOUNDARY_UNIFORM))->GetCheck())
			{
				//uniformly set the parameterization on the boundary
				int num = (m_ArraySeletedVerticesIndex[1] - m_ArraySeletedVerticesIndex[0] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				for(int i = m_ArraySeletedVerticesIndex[0] + 1; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					vec1.clear();
					vec1.push_back(((i - m_ArraySeletedVerticesIndex[0] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
					vec1.push_back(0);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
					constraint.first = m_ArrarBoundaryVertices[i].idx();
					constraint.second = vec1;
					pMesh3D->m_ArrayConstraints.push_back(constraint);
				}
				num = (m_ArraySeletedVerticesIndex[2] - m_ArraySeletedVerticesIndex[1] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				for(int i = m_ArraySeletedVerticesIndex[1] + 1; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					vec1.clear();
					vec1.push_back(1);
					vec1.push_back(((i - m_ArraySeletedVerticesIndex[1] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
					constraint.first = m_ArrarBoundaryVertices[i].idx();
					constraint.second = vec1;
					pMesh3D->m_ArrayConstraints.push_back(constraint);
				}
				num = (m_ArraySeletedVerticesIndex[3] - m_ArraySeletedVerticesIndex[2] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				for(int i = m_ArraySeletedVerticesIndex[2] + 1; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					vec1.clear();
					vec1.push_back(1.0 - ((i - m_ArraySeletedVerticesIndex[2] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
					vec1.push_back(1);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
					constraint.first = m_ArrarBoundaryVertices[i].idx();
					constraint.second = vec1;
					pMesh3D->m_ArrayConstraints.push_back(constraint);
				}
				num = (m_ArraySeletedVerticesIndex[0] - m_ArraySeletedVerticesIndex[3] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				for(int i = m_ArraySeletedVerticesIndex[3] + 1; i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					vec1.clear();
					vec1.push_back(0);
					vec1.push_back(1.0 - ((i - m_ArraySeletedVerticesIndex[3] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
					constraint.first = m_ArrarBoundaryVertices[i].idx();
					constraint.second = vec1;
					pMesh3D->m_ArrayConstraints.push_back(constraint);
				}
			}else
			if(((CButton *)GetDlgItem(IDC_TEICHMULLER_BOUNDARY_CHORD))->GetCheck())
			{
				//chord length method

				//the bottom edge
				//first compute the edge length
				double length = 0;
				for(int i = m_ArraySeletedVerticesIndex[0]; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt,endpt;
					startpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					length += edgelength;
				}
				double accummlateedgelength = 0;
				for(int i = m_ArraySeletedVerticesIndex[0] + 1; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt,endpt;
					startpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					accummlateedgelength += edgelength;

					vec1.clear();
					vec1.push_back(accummlateedgelength / length);
					vec1.push_back(0);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
					constraint.first = m_ArrarBoundaryVertices[i].idx();
					constraint.second = vec1;
					pMesh3D->m_ArrayConstraints.push_back(constraint);
				}

				//the right edge
				//first compute the edge length
				length = 0;
				for(int i = m_ArraySeletedVerticesIndex[1]; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt,endpt;
					startpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					length += edgelength;
				}
				accummlateedgelength = 0;
				for(int i = m_ArraySeletedVerticesIndex[1] + 1; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt,endpt;
					startpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					accummlateedgelength += edgelength;

					vec1.clear();
					vec1.push_back(1);
					vec1.push_back((accummlateedgelength / length));
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
					constraint.first = m_ArrarBoundaryVertices[i].idx();
					constraint.second = vec1;
					pMesh3D->m_ArrayConstraints.push_back(constraint);
				}

				//the top edge
				//first compute the edge length
				length = 0;
				for(int i = m_ArraySeletedVerticesIndex[2]; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt,endpt;
					startpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					length += edgelength;
				}
				accummlateedgelength = 0;
				for(int i = m_ArraySeletedVerticesIndex[2] + 1; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt,endpt;
					startpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					accummlateedgelength += edgelength;

					vec1.clear();
					vec1.push_back(1 - accummlateedgelength / length);
					vec1.push_back(1);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
					constraint.first = m_ArrarBoundaryVertices[i].idx();
					constraint.second = vec1;
					pMesh3D->m_ArrayConstraints.push_back(constraint);
				}

				//the left edge
				//first compute the edge length
				length = 0;
				for(int i = m_ArraySeletedVerticesIndex[3]; i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt,endpt;
					startpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					length += edgelength;
				}
				accummlateedgelength = 0;
				for(int i = (m_ArraySeletedVerticesIndex[3] + 1)% m_ArrarBoundaryVertices.size(); i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt,endpt;
					startpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point (m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					accummlateedgelength += edgelength;

					vec1.clear();
					vec1.push_back(0);
					vec1.push_back((1 - accummlateedgelength / length));
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
					constraint.first = m_ArrarBoundaryVertices[i].idx();
					constraint.second = vec1;
					pMesh3D->m_ArrayConstraints.push_back(constraint);
				}
			}
}
void Actions_TeichmullerMap::Intilization_Linear_Equation(CMesh3d_OpenMesh* pMesh3D,int &m_interior_vertices_num)
{
	//if(pMesh3D->m_Initialization_Linear_Equaiton)
	//{
	//	m_interior_vertices_num = pMesh3D->m_Initialization_Linear_Equaiton;
	//	return;
	//}

	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected;

	//Vertex Selected
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected, PROP_VERTEX_SELECTED);

	//
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);


	std::pair<int,std::vector<double>> pair;

	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle()) = VERTEX_FREE;
	}
	//Set the constainted vertex as selected.
	for(int i = 0; i < pMesh3D->m_ArrayConstraints.size(); i++)
	{
		pair = pMesh3D->m_ArrayConstraints[i];
		pMesh3D->pmesh->property(vprop_vertex_selected,pMesh3D->pmesh->vertex_handle(pair.first)) = VERTEX_CONSTAINED;
	}

	//Set the vertice a non-boundary index 
	int m_interior_vertices_num1 = 0;
	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle());
		int isonboundary = pMesh3D->pmesh->is_boundary(v_it.handle());
		if(flag || isonboundary)
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle()) = -1;
		}else
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle()) = m_interior_vertices_num1;
			m_interior_vertices_num1++;
		}
	}
	pMesh3D->m_Initialization_Linear_Equaiton = m_interior_vertices_num1;
	m_interior_vertices_num = m_interior_vertices_num1;
}
void Actions_TeichmullerMap::Compute_Linear_Equations_PerVertex(CMesh3d_OpenMesh* pMesh3D,OpenMesh::VertexHandle vh,int rowindex,MySparseMatrix &A,double &bval, int ii)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected;
	FPROP_FACE_MU fprop_face_mu;

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex Selected
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	double AreaT,Ai,Aj,Ak,Bi,Bj,Bk;
	std::vector<double> uvi,uvj,uvk;
	int vhindex1,vhindex2,vhindex3;
	std::vector<int> m_VertexHandleArray;
	std::vector<OpenMesh::Vec3d> m_VertexVecArray;
	int nonboundaryindexi,nonboundaryindexj,nonboundaryindexk;
	std::vector<std::pair<int,double>> m_ArrayElements;

	for(MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(vh); vf_it; vf_it++)
	{
		std::vector<double> muvec = pMesh3D->pmesh->property(fprop_face_mu,vf_it.handle());
		m_VertexVecArray.clear();
		m_VertexHandleArray.clear();
		for(MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(vf_it.handle()); fv_it;fv_it++)
		{
			OpenMesh::Vec3d vv = pMesh3D->pmesh->point(fv_it.handle());
			m_VertexVecArray.push_back(vv);
			m_VertexHandleArray.push_back(fv_it.handle().idx());
		}

		double gi,hi,gj,hj,gk,hk;
		int offset,ij;
		for(ij = 0; ij < 3; ij++)
			if(vh.idx() == m_VertexHandleArray[ij])
			{
				offset = ij;
				break;
			}

		gi = m_VertexVecArray[offset][0];
		hi = m_VertexVecArray[offset][1];
		gj = m_VertexVecArray[(offset + 1) % 3][0];
		hj = m_VertexVecArray[(offset + 1) % 3][1];
		gk = m_VertexVecArray[(offset + 2) % 3][0];
		hk = m_VertexVecArray[(offset + 2) % 3][1];

		nonboundaryindexi = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(m_VertexHandleArray[offset]));
		nonboundaryindexj = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(m_VertexHandleArray[(offset + 1) % 3]));
		nonboundaryindexk = pMesh3D->pmesh->property(vprop_nonboundaryindex,pMesh3D->pmesh->vertex_handle(m_VertexHandleArray[(offset + 2) % 3]));

		//get the uv
		uvi = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(m_VertexHandleArray[offset]));
		uvj = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(m_VertexHandleArray[(offset + 1) % 3]));
		uvk = pMesh3D->pmesh->property(vprop_vertex_uv,pMesh3D->pmesh->vertex_handle(m_VertexHandleArray[(offset + 2) % 3]));

		AreaT = (gi * hj - hi * gj) + (gj * hk - hj * gk) + (gk * hi - hk * gi); 

		Ai = (hj - hk) / AreaT;
		Aj = (hk - hi) / AreaT;
		Ak = (hi - hj) / AreaT;

		Bi = -(gj - gk) / AreaT;
		Bj = -(gk - gi) / AreaT;
		Bk = -(gi - gj) / AreaT;

		double rou = muvec[0];
		double tao = muvec[1];

		double denorm = (1 - SQR(rou) - SQR(tao));
		double a1 = (SQR(rou - 1) + SQR(tao)) / denorm;
		double a2 = -2 * tao / denorm;
		double a3 = (1 + 2 * rou + SQR(rou) + SQR(tao)) / denorm;

		double aa = Ai * a1 + Bi * a2;
		double bb = Ai * a2 + Bi * a3;
		double ci = aa * Ai + bb * Bi;
		double cj = aa * Aj + bb * Bj;
		double ck = aa * Ak + bb * Bk;

		ci = ci * AreaT;
		cj = cj * AreaT;
		ck = ck * AreaT;
		if(nonboundaryindexi >= 0)
		{
			IncreaseItem(m_ArrayElements,nonboundaryindexi,ci);
			//A.IncreaseItem(rowindex,nonboundaryindexi,ci);
		}
		else
			bval += -ci * uvi[ii];

		if(nonboundaryindexj >= 0)
		{
			IncreaseItem(m_ArrayElements,nonboundaryindexj,cj);
			//A.IncreaseItem(rowindex,nonboundaryindexj,cj);
		}
		else
			bval += -cj * uvj[ii];

		if(nonboundaryindexk >= 0)
		{
			IncreaseItem(m_ArrayElements,nonboundaryindexk,ck);
			//A.IncreaseItem(rowindex,nonboundaryindexk,ck);
		}
		else
			bval += -ck * uvk[ii];
	}
	for(int i = 0; i < m_ArrayElements.size(); i++)
	{
		A.AddItem(rowindex,m_ArrayElements[i].first,m_ArrayElements[i].second);
	}
}
void Actions_TeichmullerMap::IncreaseItem(std::vector<std::pair<int,double>> &m_ArrayElements,int index,double val)
{
	int i;
	for(i = 0; i < m_ArrayElements.size(); i++)
	{
		if(m_ArrayElements[i].first == index)
		{
			m_ArrayElements[i].second += val;
			break;
		}
	}
	if(i == m_ArrayElements.size())
	{
		m_ArrayElements.push_back(std::pair<int,double>(index,val));
	}
}
CCriticalSection global_CriticalSection; 
void Actions_TeichmullerMap::Compute_Linear_Equations(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected;
	FPROP_FACE_MU fprop_face_mu;
	FPROP_FACE_INTERINFO fprop_face_interinfo;

	//Face Intermediate Information
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_interinfo, PROP_FACE_INTERMEDIATEINFO))
		pMesh3D->pmesh->add_property(fprop_face_interinfo, PROP_FACE_INTERMEDIATEINFO);

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex Selected
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	int m_interior_vertices_num;

	//Initialization
	Intilization_Linear_Equation(pMesh3D,m_interior_vertices_num);

	MySparseMatrix A;
	std::vector<double> b;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	int rowindex = 0;
	double bval;

	for(int ii = 0; ii < 2; ii++)
	{
		A.m = m_interior_vertices_num ;
		A.n = m_interior_vertices_num ;
		
		A.Clear();
		b.clear();
		rowindex = 0;
		//all the linear equations
		for(MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			bval = 0;

			int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle());

			int constainedflag = pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle());

			if(constainedflag || v0_index < 0)
				continue;

			Compute_Linear_Equations_PerVertex(pMesh3D,v_it.handle(),rowindex,A,bval,ii);
			rowindex++;
			b.push_back(bval);
		}
				/*
		TRACE("A   \n");
		for(int i = 0; i < A.m; i++)
		{
			for(int j = 0; j < A.n; j++)
			{
				TRACE(" %f ",A.m_Array[std::pair<int,int>(i,j)]);
			}
			TRACE("\n");
		}

		TRACE("b   \n");
		for(int i = 0; i < b.size(); i++)
			TRACE(" %f ",b[i]);//*/
		global_CriticalSection.Lock();
		Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A,b,uvarray[ii]);
		global_CriticalSection.Unlock();
	}

	//set the interior parameters
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	for(MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_vertex_selected,v_it.handle());

		if(flag)
			continue;

		int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle());

		if (v0_index < 0)
			continue;

		uvvalues[0] = uvarray[0][v0_index];
		uvvalues[1] = uvarray[1][v0_index];

		pMesh3D->pmesh->property(vprop_vertex_uv,v_it.handle()) = uvvalues;
	}
}

void Actions_TeichmullerMap::Compute_Linear_Equations_Registration(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected;
	FPROP_FACE_MU fprop_face_mu;
	FPROP_FACE_INTERINFO fprop_face_interinfo;

	//Face Intermediate Information
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_interinfo, PROP_FACE_INTERMEDIATEINFO))
		pMesh3D->pmesh->add_property(fprop_face_interinfo, PROP_FACE_INTERMEDIATEINFO);

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//Vertex Selected
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	int m_interior_vertices_num;

	//Initialization
	Intilization_Linear_Equation(pMesh3D, m_interior_vertices_num);

	MySparseMatrix A;
	std::vector<double> b;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	int rowindex = 0;
	double bval;

	for (int ii = 0; ii < 2; ii++)
	{
		A.m = m_interior_vertices_num;
		A.n = m_interior_vertices_num;

		A.Clear();
		b.clear();
		rowindex = 0;
		//all the linear equations
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			bval = 0;
			int vidindex = v_it.handle().idx();

			int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

			int constainedflag = pMesh3D->pmesh->property(vprop_vertex_selected, v_it.handle());

			if ( v0_index < 0)
				continue;

			Compute_Linear_Equations_PerVertex(pMesh3D, v_it.handle(), rowindex, A, bval, ii);
			rowindex++;
			b.push_back(bval);
		}
		/*
		TRACE("A   \n");
		for(int i = 0; i < A.m; i++)
		{
		for(int j = 0; j < A.n; j++)
		{
		TRACE(" %f ",A.m_Array[std::pair<int,int>(i,j)]);
		}
		TRACE("\n");
		}

		TRACE("b   \n");
		for(int i = 0; i < b.size(); i++)
		TRACE(" %f ",b[i]);//*/
		global_CriticalSection.Lock();
		Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A, b, uvarray[ii]);
		global_CriticalSection.Unlock();
	}

	//set the interior parameters
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_vertex_selected, v_it.handle());

		if (flag)
			continue;

		int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

		if (v0_index < 0)
			continue;

		uvvalues[0] = uvarray[0][v0_index];
		uvvalues[1] = uvarray[1][v0_index];

		pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
	}
}

void Actions_TeichmullerMap::AddConstraints(CMesh3d_OpenMesh* pMesh3D)
{
	//Compute_Boundary_Vertices_Parameter_Positions(pMesh3D);
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	pMesh3D->m_ArrayConstraints.clear();
	std::pair<int, std::vector<double>> constraint;
	std::vector<double> uv;
	int index;
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[i];
		int start = pLandmark->landmark_points[0];
		int end = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
		int flag = pMesh3D->pmesh->is_boundary(pMesh3D->pmesh->vertex_handle(start));
		uv = pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(start));
		for (index = 0; index < pMesh3D->m_ArrayConstraints.size(); index++)
		{
			if (start == pMesh3D->m_ArrayConstraints[index].first)
				break;
		}
		if (!flag && index == pMesh3D->m_ArrayConstraints.size())
		{
			constraint.first = start;
			constraint.second = uv;
			pMesh3D->m_ArrayConstraints.push_back(constraint);
		}
		flag = pMesh3D->pmesh->is_boundary(pMesh3D->pmesh->vertex_handle(end));
		uv = pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(end));
		for (index = 0; index < pMesh3D->m_ArrayConstraints.size(); index++)
		{
			if (end == pMesh3D->m_ArrayConstraints[index].first)
				break;
		}
		if (!flag && index == pMesh3D->m_ArrayConstraints.size())
		{
			constraint.first = end;
			constraint.second = uv;
			pMesh3D->m_ArrayConstraints.push_back(constraint);
		}
	}
}
CMesh3d_OpenMesh* Actions_TeichmullerMap::GenerateParameterDomainMesh(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu_original;

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu_original, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu_original, PROP_FACE_MU);

	//
	ComputeMu(pMesh3D);

	MyMesh* newmesh = new MyMesh(*pMesh3D->pmesh);
	for(MyMesh::VertexIter v_it = newmesh->vertices_begin(); v_it!= newmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d p;
		std::vector<double> uv = newmesh->property(vprop_vertex_uv,v_it.handle());
		p[0] = uv[0];
		p[1] = uv[1];
		p[2] = 0;
		newmesh->set_point(v_it.handle(),p);
	}

	CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pNewMesh->pmesh = newmesh;
	pNewMesh->m_Transform.Copy(&pMesh3D->m_Transform);

	FPROP_FACE_MU fprop_face_mu_final;

	//Face mu
	if (!pNewMesh->pmesh->get_property_handle(fprop_face_mu_final, PROP_FACE_MU))
		pNewMesh->pmesh->add_property(fprop_face_mu_final, PROP_FACE_MU);

	for(MyMesh::FaceIter f_it = pNewMesh->pmesh->faces_begin(); f_it!= pNewMesh->pmesh->faces_end(); ++f_it)
	{
		pNewMesh->pmesh->property(fprop_face_mu_final,f_it.handle()) = pMesh3D->pmesh->property(fprop_face_mu_original,f_it.handle());
	}
	return pNewMesh;
}