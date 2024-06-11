// Actions_Tspline.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Actions_Tspline.h"
#include "FormView3.h"
#include "disline.h"
#include "InterFunction.h"
#include "math.h"
#include "Nurbs3D.h"
#include "C_NurbsSurf.h"
#include "SDIViewSwitchDoc.h"
#include "lib3d/Base3d.h"
#include <windows.h>
#include "MyTreeView.h"
#include <iostream>
#include <fstream>
#include "resource.h"
#include <atlimage.h>
#include "Tspline.h"

// Actions_Tspline dialog
string DoubleToString(double d);
IMPLEMENT_DYNAMIC(Actions_Tspline, CPropertyPage)

BEGIN_MESSAGE_MAP(Actions_Tspline, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_TSPLINE_GEN, &Actions_Tspline::OnBnClickedTsplineGen)
	ON_BN_CLICKED(IDC_TSPLINE_SIMPLIFICATION, &Actions_Tspline::OnBnClickedTsplineSimplification)
END_MESSAGE_MAP()

Actions_Tspline::Actions_Tspline()
	: CPropertyPage(Actions_Tspline::IDD)
{

}

Actions_Tspline::~Actions_Tspline()
{
}

void Actions_Tspline::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

void Actions_Tspline::OnBnClickedTsplineGen()
{
	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = 0;

	if(currentindex < 0 || currentindex >= num)
		return;

	if(pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);
		Tspline* ptspline = new Tspline;
		std::vector<int> m_uknotindex,m_vknotindex;
		ptspline->BuildFromNURBS(pSurface,m_uknotindex,m_vknotindex);
		ptspline->filename = "Tspline";
		ptspline->SetTransform(*pSurface->GetTransform());
		pDoc->m_SceneGraph.Add(ptspline);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
}
C_NurbsSurf* Actions_Tspline::Generate_NURBS_Least_Square_Fitting(CMesh3d* pMesh1,CMesh3d* pMesh2,CNurbsSuface* pSur)
{
	Point3D lower,upper;
	pMesh2->ComputeBoundingBox(lower,upper);
	double length = upper.x - lower.x;
	if(length < upper.y - lower.y)
		length = upper.y - lower.y;

	Point3D p00,pm0,p0n,pmn;
	int index00,indexm0,index0n,indexmn;
	pMesh2->GetFourCornerPoints(index00,indexm0,index0n,indexmn);
	p00[0] = pMesh1->GetVertex(index00)->x();
	p00[1] = pMesh1->GetVertex(index00)->y();
	p00[2] = pMesh1->GetVertex(index00)->z();

	pm0[0] = pMesh1->GetVertex(indexm0)->x();
	pm0[1] = pMesh1->GetVertex(indexm0)->y();
	pm0[2] = pMesh1->GetVertex(indexm0)->z();

	p0n[0] = pMesh1->GetVertex(index0n)->x();
	p0n[1] = pMesh1->GetVertex(index0n)->y();
	p0n[2] = pMesh1->GetVertex(index0n)->z();

	pmn[0] = pMesh1->GetVertex(indexmn)->x();
	pmn[1] = pMesh1->GetVertex(indexmn)->y();
	pmn[2] = pMesh1->GetVertex(indexmn)->z();

	int vnum = pMesh2->NbVertex();
	//vnum = 100;
	//degree + 1
	
	int u_order = pSur->m_udegree + 1;
	int v_order = pSur->m_vdegree + 1;

	int uknotnum = pSur->m_uknotcount - u_order * 2;
	int vknotnum = pSur->m_vknotcount - v_order * 2;

	double *uknot = new double[u_order * 2 + uknotnum];
	double *vknot = new double[v_order * 2 + vknotnum];

	for(int i = 0; i < u_order * 2 + uknotnum; i++)
	{
		uknot[i] = pSur->m_uknot[i];
	}

	for(int i = 0; i < v_order * 2 + vknotnum; i++)
	{
		vknot[i] = pSur->m_vknot[i];
	}

	//

	Point3D* pControlPoint = new Point3D[(u_order + uknotnum) * (v_order + vknotnum)];

	double* pWeight = new double[(u_order + uknotnum) * (v_order + vknotnum)];

	C_NurbsSurf* pNurbsSurface = new C_NurbsSurf(u_order + uknotnum, v_order + vknotnum, u_order, v_order, uknot,vknot,pControlPoint,pWeight);

	double c;
	std::vector<std::vector<double>> coeffcients;

	for(int j1 = 0; j1 < v_order + vknotnum; j1++)
		for(int i1 = 0; i1 < u_order + uknotnum; i1++)
		{
			//the four corners
			if(i1 == 0 && j1 == 0)
				continue;
			if(i1 == u_order + uknotnum - 1 && j1 == 0)
				continue;
			if(i1 == 0 && j1 == v_order + vknotnum - 1)
				continue;
			if(i1 == u_order + uknotnum - 1 && j1 == v_order + vknotnum - 1)
				continue;

			std::vector<double> coeff;
			for(int j = 0; j < v_order + vknotnum; j++)
				for(int i = 0; i < u_order + uknotnum; i++)
				{
					//the four corners
					if(i == 0 && j == 0)
						continue;
					if(i == u_order + uknotnum - 1 && j == 0)
						continue;
					if(i == 0 && j == v_order + vknotnum - 1)
						continue;
					if(i == u_order + uknotnum - 1 && j == v_order + vknotnum - 1)
						continue;

					c = 0;
					for(int k = 0; k < vnum; k++)
					{
						CVertex3d* pVertex = pMesh2->GetVertex(k);
						double u = pVertex->x();
						double v = pVertex->y();

						//u = (u - lower.x) / length;
						//v = (v - lower.y) / length;

						//u = (u - lower.x) / (upper.x - lower.x);
						//v = (v - lower.y) / (upper.y - lower.y);
						
						double ubas = pNurbsSurface->ComputeUBasis(i,u);
						double vbas = pNurbsSurface->ComputeVBasis(j,v);

						double ubas1 = pNurbsSurface->ComputeUBasis(i1,u);
						double vbas1 = pNurbsSurface->ComputeVBasis(j1,v);
						c = c + ubas * vbas * ubas1 * vbas1 ;
					}
					coeff.push_back(c);
				}
				c = 0;
				for(int k = 0; k < vnum; k++)
				{
					CVertex3d* pVertex = pMesh2->GetVertex(k);
					CVertex3d* pVertex1 = pMesh1->GetVertex(k);
					double u = pVertex->x();
					double v = pVertex->y();

					//u = (u - lower.x) / length;
					//v = (v - lower.y) / length;

					//u = (u - lower.x) / (upper.x - lower.x);
					//v = (v - lower.y) / (upper.y - lower.y);

					double ubas = pNurbsSurface->ComputeUBasis(i1,u);
					double vbas = pNurbsSurface->ComputeVBasis(j1,v);

					double ubas0 = pNurbsSurface->ComputeUBasis(0,u);
					double vbas0 = pNurbsSurface->ComputeVBasis(0,v);

					double ubasm = pNurbsSurface->ComputeUBasis(u_order + uknotnum - 1,u);
					double vbasn = pNurbsSurface->ComputeVBasis(v_order + vknotnum - 1,v);

					c = c + ubas * vbas * (pVertex1->x() - (ubas0 * vbas0 * p00.x + ubasm * vbas0 * pm0.x + ubas0 * vbasn * p0n.x + ubasm * vbasn * pmn.x));
				}
				coeff.push_back(c);
				c = 0;
				for(int k = 0; k < vnum; k++)
				{
					CVertex3d* pVertex = pMesh2->GetVertex(k);
					CVertex3d* pVertex1 = pMesh1->GetVertex(k);
					double u = pVertex->x();
					double v = pVertex->y();

					double ubas = pNurbsSurface->ComputeUBasis(i1,u);
					double vbas = pNurbsSurface->ComputeVBasis(j1,v);

					double ubas0 = pNurbsSurface->ComputeUBasis(0,u);
					double vbas0 = pNurbsSurface->ComputeVBasis(0,v);

					double ubasm = pNurbsSurface->ComputeUBasis(u_order + uknotnum - 1,u);
					double vbasn = pNurbsSurface->ComputeVBasis(v_order + vknotnum - 1,v);

					c = c + ubas * vbas * (pVertex1->y() - (ubas0 * vbas0 * p00.y + ubasm * vbas0 * pm0.y + ubas0 * vbasn * p0n.y + ubasm * vbasn * pmn.y));
				}
				coeff.push_back(c);
				c = 0;
				for(int k = 0; k < vnum; k++)
				{
					CVertex3d* pVertex = pMesh2->GetVertex(k);
					CVertex3d* pVertex1 = pMesh1->GetVertex(k);
					double u = pVertex->x();
					double v = pVertex->y();

					//u = (u - lower.x) / length;
					//v = (v - lower.y) / length;

					//u = (u - lower.x) / (upper.x - lower.x);
					//v = (v - lower.y) / (upper.y - lower.y);

					double ubas = pNurbsSurface->ComputeUBasis(i1,u);
					double vbas = pNurbsSurface->ComputeVBasis(j1,v);

					double ubas0 = pNurbsSurface->ComputeUBasis(0,u);
					double vbas0 = pNurbsSurface->ComputeVBasis(0,v);

					double ubasm = pNurbsSurface->ComputeUBasis(u_order + uknotnum - 1,u);
					double vbasn = pNurbsSurface->ComputeVBasis(v_order + vknotnum - 1,v);

					c = c + ubas * vbas * (pVertex1->z() - (ubas0 * vbas0 * p00.z + ubasm * vbas0 * pm0.z + ubas0 * vbasn * p0n.z + ubasm * vbasn * pmn.z));
				}
				coeff.push_back(c);
				coeffcients.push_back(coeff);
		}

		using namespace alglib;

		//get the control points
		string str = "[";
		string str1 = "[";
		string str2 = "[";
		string str3="[";
		for(int i=0;i<coeffcients.size();i++)
		{
			str3 += "[";
			for(int j=0;j<coeffcients.size();j++)
			{
				str3+=DoubleToString(coeffcients[i][j]);
				if(j < coeffcients[i].size() - 4)
					str3 += ",";
			}
			str3 += "]";
			if(i < coeffcients.size() - 1)
				str3 += ",";
			str += DoubleToString(coeffcients[i][coeffcients.size()]);
			str1 += DoubleToString(coeffcients[i][coeffcients.size() + 1]);
			str2 += DoubleToString(coeffcients[i][coeffcients.size() + 2]);
			if(i < coeffcients.size() - 1)
			{
				str += ",";
				str1 += ",";
				str2 += ",";
			}else
			{
				str += "]";
				str1 += "]";
				str2 += "]";
			}
		}
			str3 += "]";

		real_1d_array	b(str.c_str());
		real_1d_array	b1(str1.c_str());
		real_1d_array	b2(str2.c_str());
		real_2d_array 	A(str3.c_str());
		real_1d_array   x;
		real_1d_array   y;
		real_1d_array   z;
		int64_t val;
		densesolverreport rep;

		rmatrixsolve(A,coeffcients.size(),b,val,rep,x);
		rmatrixsolve(A,coeffcients.size(),b1,val,rep,y);
		rmatrixsolve(A,coeffcients.size(),b2,val,rep,z);

		int num = 0;

		//four corner points
		pNurbsSurface->SetControlPoint(0,0,p00);
		pNurbsSurface->SetControlPoint(u_order + uknotnum - 1,0,pm0);
		pNurbsSurface->SetControlPoint(0,v_order + vknotnum - 1,p0n);
		pNurbsSurface->SetControlPoint(u_order + uknotnum - 1,v_order + vknotnum - 1,pmn);
		pNurbsSurface->SetWeight(0,0,1);
		pNurbsSurface->SetWeight(u_order + uknotnum - 1,0,1);
		pNurbsSurface->SetWeight(0,v_order + vknotnum - 1,1);
		pNurbsSurface->SetWeight(u_order + uknotnum - 1,v_order + vknotnum - 1,1);

		for(int j1 = 0; j1 < v_order + vknotnum; j1++)
			for(int i1 = 0; i1 < u_order + uknotnum; i1++)
			{
				//the four corners
				if(i1 == 0 && j1 == 0)
					continue;
				if(i1 == u_order + uknotnum - 1 && j1 == 0)
					continue;
				if(i1 == 0 && j1 == v_order + vknotnum - 1)
					continue;
				if(i1 == u_order + uknotnum - 1 && j1 == v_order + vknotnum - 1)
					continue;

				Point3D point;
				point[0] = x[num];
				point[1] = y[num];
				point[2] = z[num];
				num++;
				pNurbsSurface->SetControlPoint(i1,j1,point);
				Point3D point1 = pNurbsSurface->GetControlPoint(i1,j1);
				pNurbsSurface->SetWeight(i1,j1,1);
			}
		return pNurbsSurface;
}

void Actions_Tspline::Generate_Tspline_Least_Square_Fitting(CMesh3d* pMesh1,CMesh3d* pMesh2,Tspline* pSur)
{
	VPROP_VERTEX_CORNER_LABEL vprop_corner_label;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag;
	VPROP_VERTEX_UV_COORDINATE vprop_uv_coordinate;

	//IMG
	if (!pSur->pmesh->get_property_handle(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG))
		pSur->pmesh->add_property(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG);

	//CORNER
	if (!pSur->pmesh->get_property_handle(vprop_corner_label, PROP_VERTEX_CORNER_LABEL))
		pSur->pmesh->add_property(vprop_corner_label, PROP_VERTEX_CORNER_LABEL);

	//UV coordinate
	if (!pSur->pmesh->get_property_handle(vprop_uv_coordinate, PROP_VERTEX_UV_COORDINATE))
		pSur->pmesh->add_property(vprop_uv_coordinate, PROP_VERTEX_UV_COORDINATE);

	Point3D lower,upper;
	pMesh2->ComputeBoundingBox(lower,upper);

	Point3D p1,p2,p3,p4;
	int index00,indexm0,index0n,indexmn;
	int index1,index2,index3,index4;
	pMesh2->GetFourCornerPoints(index00,indexm0,index0n,indexmn);

	TsplineMesh::VertexHandle vh1,vh2,vh3,vh4;	
	Vector2D uv1,uv2,uv3,uv4;
	//
	for( TsplineMesh::VertexIter v_iter = pSur->pmesh->vertices_begin(); v_iter!=pSur->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = pSur->pmesh->property(vprop_IMGFlag,v_iter.handle());
		if(flag)
			continue;

		//Remember the four corner points
		int cornerflag = pSur->pmesh->property(vprop_corner_label,v_iter.handle());
		Vector2D uv = pSur->pmesh->property(vprop_uv_coordinate,v_iter.handle());
		if(cornerflag == 1)
		{
			vh1 = v_iter.handle();
			uv1 = uv;
		}
		if(cornerflag == 2)
		{
			vh2 = v_iter.handle();
			uv2 = uv;
		}
		if(cornerflag == 3)
		{
			vh3 = v_iter.handle();
			uv3 = uv;
		}
		if(cornerflag == 4)
		{
			vh4 = v_iter.handle();
			uv4 = uv;
		}
	}
	
	std::vector<Vector2D> uvarray;
	std::vector<int> indexarray;
	Vector2D uv00(pMesh2->GetVertex(index00)->x(),pMesh2->GetVertex(index00)->y());
	Vector2D uvm0(pMesh2->GetVertex(indexm0)->x(),pMesh2->GetVertex(indexm0)->y());
	Vector2D uv0n(pMesh2->GetVertex(index0n)->x(),pMesh2->GetVertex(index0n)->y());
	Vector2D uvmn(pMesh2->GetVertex(indexmn)->x(),pMesh2->GetVertex(indexmn)->y());
	uvarray.push_back(uv00);
	uvarray.push_back(uvm0);
	uvarray.push_back(uv0n);
	uvarray.push_back(uvmn);
	indexarray.push_back(index00);
	indexarray.push_back(indexm0);
	indexarray.push_back(index0n);
	indexarray.push_back(indexmn);

	for(int ii = 0; ii < uvarray.size(); ii++)
	{
		if(Length(uvarray[ii] - uv1) < 1e-8)
			index1 = indexarray[ii];
		if(Length(uvarray[ii] - uv2) < 1e-8)
			index2 = indexarray[ii];
		if(Length(uvarray[ii] - uv3) < 1e-8)
			index3 = indexarray[ii];
		if(Length(uvarray[ii] - uv4) < 1e-8)
			index4 = indexarray[ii];
	}

	p1[0] = pMesh1->GetVertex(index1)->x();
	p1[1] = pMesh1->GetVertex(index1)->y();
	p1[2] = pMesh1->GetVertex(index1)->z();

	p2[0] = pMesh1->GetVertex(index2)->x();
	p2[1] = pMesh1->GetVertex(index2)->y();
	p2[2] = pMesh1->GetVertex(index2)->z();

	p3[0] = pMesh1->GetVertex(index3)->x();
	p3[1] = pMesh1->GetVertex(index3)->y();
	p3[2] = pMesh1->GetVertex(index3)->z();

	p4[0] = pMesh1->GetVertex(index4)->x();
	p4[1] = pMesh1->GetVertex(index4)->y();
	p4[2] = pMesh1->GetVertex(index4)->z();

	int vnum = pMesh2->NbVertex();

	double c;
	std::vector<std::vector<double>> coeffcients;

	for( TsplineMesh::VertexIter v_iter = pSur->pmesh->vertices_begin(); v_iter!=pSur->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = pSur->pmesh->property(vprop_IMGFlag,v_iter.handle());
		if(flag)
			continue;

		//skip the four corner points
		int cornerflag = pSur->pmesh->property(vprop_corner_label,v_iter.handle());
		if(cornerflag > 0)
			continue;

		std::vector<double> coeff;
		for( TsplineMesh::VertexIter v_iter1 = pSur->pmesh->vertices_begin(); v_iter1!=pSur->pmesh->vertices_end(); ++v_iter1 ) 
		{
			//Skip the Imagination vertex
			int flag = pSur->pmesh->property(vprop_IMGFlag,v_iter1.handle());
			if(flag)
				continue;

			//skip the four corner points
			int cornerflag = pSur->pmesh->property(vprop_corner_label,v_iter1.handle());
			if(cornerflag > 0)
				continue;

			c = 0;
			for(int k = 0; k < vnum; k++)
			{
				CVertex3d* pVertex = pMesh2->GetVertex(k);
				double u = pVertex->x();
				double v = pVertex->y();

				//u = (u - lower.x) / length;
				//v = (v - lower.y) / length;

				//u = (u - lower.x) / (upper.x - lower.x);
				//v = (v - lower.y) / (upper.y - lower.y);

				double basis = pSur->ComputeBasis(v_iter.handle(),u,v);
				double basis1 = pSur->ComputeBasis(v_iter1.handle(),u,v);
				c = c + basis * basis1 ;
			}
			coeff.push_back(c);
		}
		c = 0;
		for(int k = 0; k < vnum; k++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(k);
			CVertex3d* pVertex1 = pMesh1->GetVertex(k);
			double u = pVertex->x();
			double v = pVertex->y();

			//u = (u - lower.x) / length;
			//v = (v - lower.y) / length;

			//u = (u - lower.x) / (upper.x - lower.x);
			//v = (v - lower.y) / (upper.y - lower.y);

			double basis = pSur->ComputeBasis(v_iter.handle(),u,v);
			double basis1 = pSur->ComputeBasis(vh1,u,v);
			double basis2 = pSur->ComputeBasis(vh2,u,v);
			double basis3 = pSur->ComputeBasis(vh3,u,v);
			double basis4 = pSur->ComputeBasis(vh4,u,v);

			c = c + basis * (pVertex1->x() - (basis1 * p1.x + basis2 * p2.x + basis3 * p3.x + basis4 * p4.x));
		}
		coeff.push_back(c);
		c = 0;
		for(int k = 0; k < vnum; k++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(k);
			CVertex3d* pVertex1 = pMesh1->GetVertex(k);
			double u = pVertex->x();
			double v = pVertex->y();

			double basis = pSur->ComputeBasis(v_iter.handle(),u,v);
			double basis1 = pSur->ComputeBasis(vh1,u,v);
			double basis2 = pSur->ComputeBasis(vh2,u,v);
			double basis3 = pSur->ComputeBasis(vh3,u,v);
			double basis4 = pSur->ComputeBasis(vh4,u,v);

			c = c + basis * (pVertex1->y() - (basis1 * p1.y + basis2 * p2.y + basis3 * p3.y + basis4 * p4.y));
		}
		coeff.push_back(c);
		c = 0;
		for(int k = 0; k < vnum; k++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(k);
			CVertex3d* pVertex1 = pMesh1->GetVertex(k);
			double u = pVertex->x();
			double v = pVertex->y();

			//u = (u - lower.x) / length;
			//v = (v - lower.y) / length;

			//u = (u - lower.x) / (upper.x - lower.x);
			//v = (v - lower.y) / (upper.y - lower.y);

			double basis = pSur->ComputeBasis(v_iter.handle(),u,v);
			double basis1 = pSur->ComputeBasis(vh1,u,v);
			double basis2 = pSur->ComputeBasis(vh2,u,v);
			double basis3 = pSur->ComputeBasis(vh3,u,v);
			double basis4 = pSur->ComputeBasis(vh4,u,v);

			c = c + basis * (pVertex1->z() - (basis1 * p1.z + basis2 * p2.z + basis3 * p3.z + basis4 * p4.z));
		}
		coeff.push_back(c);
		coeffcients.push_back(coeff);
	}

		using namespace alglib;

		//get the control points
		string str = "[";
		string str1 = "[";
		string str2 = "[";
		string str3="[";
		for(int i=0;i<coeffcients.size();i++)
		{
			str3 += "[";
			for(int j=0;j<coeffcients.size();j++)
			{
				str3+=DoubleToString(coeffcients[i][j]);
				if(j < coeffcients[i].size() - 4)
					str3 += ",";
			}
			str3 += "]";
			if(i < coeffcients.size() - 1)
				str3 += ",";
			str += DoubleToString(coeffcients[i][coeffcients.size()]);
			str1 += DoubleToString(coeffcients[i][coeffcients.size() + 1]);
			str2 += DoubleToString(coeffcients[i][coeffcients.size() + 2]);
			if(i < coeffcients.size() - 1)
			{
				str += ",";
				str1 += ",";
				str2 += ",";
			}else
			{
				str += "]";
				str1 += "]";
				str2 += "]";
			}
		}
		str3 += "]";

		real_1d_array	b(str.c_str());
		real_1d_array	b1(str1.c_str());
		real_1d_array	b2(str2.c_str());
		real_2d_array 	A(str3.c_str());
		real_1d_array   x;
		real_1d_array   y;
		real_1d_array   z;
		int64_t val;
		densesolverreport rep;

		rmatrixsolve(A,coeffcients.size(),b,val,rep,x);
		rmatrixsolve(A,coeffcients.size(),b1,val,rep,y);
		rmatrixsolve(A,coeffcients.size(),b2,val,rep,z);

		int num = 0;

		VPROP_VERTEX_WEIGHT vprop_vweights;

		//Vertex weight
		if (!pSur->pmesh->get_property_handle(vprop_vweights, PROP_VERTEX_WEIGHT))
			pSur->pmesh->add_property(vprop_vweights, PROP_VERTEX_WEIGHT);

		//four corner points
		OpenMesh::Vec3d pt1(p1.x,p1.y,p1.z);
		OpenMesh::Vec3d pt2(p2.x,p2.y,p2.z);
		OpenMesh::Vec3d pt3(p3.x,p3.y,p3.z);
		OpenMesh::Vec3d pt4(p4.x,p4.y,p4.z);
		pSur->pmesh->set_point(vh1,pt1);
		pSur->pmesh->set_point(vh2,pt2);
		pSur->pmesh->set_point(vh3,pt3);
		pSur->pmesh->set_point(vh4,pt4);
		pSur->pmesh->property(vprop_vweights,vh1) = 1;
		pSur->pmesh->property(vprop_vweights,vh2) = 1;
		pSur->pmesh->property(vprop_vweights,vh3) = 1;
		pSur->pmesh->property(vprop_vweights,vh4) = 1;

		for( TsplineMesh::VertexIter v_iter = pSur->pmesh->vertices_begin(); v_iter!=pSur->pmesh->vertices_end(); ++v_iter ) 
		{
			//Skip the Imagination vertex
			int flag = pSur->pmesh->property(vprop_IMGFlag,v_iter.handle());
			if(flag)
				continue;

			//skip the four corner points
			int cornerflag = pSur->pmesh->property(vprop_corner_label,v_iter.handle());
			if(cornerflag > 0)
				continue;

			OpenMesh::Vec3d pt;
			pt[0] = x[num];
			pt[1] = y[num];
			pt[2] = z[num];
			num++;

			pSur->pmesh->set_point(v_iter.handle(),pt);
			pSur->pmesh->property(vprop_vweights,v_iter.handle()) = 1;
		}
}

void Actions_Tspline::Generate_Tspline_Least_Square_Fitting_Homogeneity(CMesh3d* pMesh1,CMesh3d* pMesh2,Tspline* pSur)
{
	VPROP_VERTEX_CORNER_LABEL vprop_corner_label;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag;
	VPROP_VERTEX_UV_COORDINATE vprop_uv_coordinate;

	//IMG
	if (!pSur->pmesh->get_property_handle(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG))
		pSur->pmesh->add_property(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG);

	//CORNER
	if (!pSur->pmesh->get_property_handle(vprop_corner_label, PROP_VERTEX_CORNER_LABEL))
		pSur->pmesh->add_property(vprop_corner_label, PROP_VERTEX_CORNER_LABEL);

	//UV coordinate
	if (!pSur->pmesh->get_property_handle(vprop_uv_coordinate, PROP_VERTEX_UV_COORDINATE))
		pSur->pmesh->add_property(vprop_uv_coordinate, PROP_VERTEX_UV_COORDINATE);

	VPROP_VERTEX_WEIGHT vprop_vweights;

	//Vertex weight
	if (!pSur->pmesh->get_property_handle(vprop_vweights, PROP_VERTEX_WEIGHT))
		pSur->pmesh->add_property(vprop_vweights, PROP_VERTEX_WEIGHT);

	Point3D lower,upper;
	pMesh2->ComputeBoundingBox(lower,upper);

	Point3D p1,p2,p3,p4;
	int index00,indexm0,index0n,indexmn;
	int index1,index2,index3,index4;
	pMesh2->GetFourCornerPoints(index00,indexm0,index0n,indexmn);

	TsplineMesh::VertexHandle vh1,vh2,vh3,vh4;	
	Vector2D uv1,uv2,uv3,uv4;
	//
	for( TsplineMesh::VertexIter v_iter = pSur->pmesh->vertices_begin(); v_iter!=pSur->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = pSur->pmesh->property(vprop_IMGFlag,v_iter.handle());
		if(flag)
			continue;

		//Remember the four corner points
		int cornerflag = pSur->pmesh->property(vprop_corner_label,v_iter.handle());
		Vector2D uv = pSur->pmesh->property(vprop_uv_coordinate,v_iter.handle());
		if(cornerflag == 1)
		{
			vh1 = v_iter.handle();
			uv1 = uv;
		}
		if(cornerflag == 2)
		{
			vh2 = v_iter.handle();
			uv2 = uv;
		}
		if(cornerflag == 3)
		{
			vh3 = v_iter.handle();
			uv3 = uv;
		}
		if(cornerflag == 4)
		{
			vh4 = v_iter.handle();
			uv4 = uv;
		}
	}

	std::vector<Vector2D> uvarray;
	std::vector<int> indexarray;
	Vector2D uv00(pMesh2->GetVertex(index00)->x(),pMesh2->GetVertex(index00)->y());
	Vector2D uvm0(pMesh2->GetVertex(indexm0)->x(),pMesh2->GetVertex(indexm0)->y());
	Vector2D uv0n(pMesh2->GetVertex(index0n)->x(),pMesh2->GetVertex(index0n)->y());
	Vector2D uvmn(pMesh2->GetVertex(indexmn)->x(),pMesh2->GetVertex(indexmn)->y());
	uvarray.push_back(uv00);
	uvarray.push_back(uvm0);
	uvarray.push_back(uv0n);
	uvarray.push_back(uvmn);
	indexarray.push_back(index00);
	indexarray.push_back(indexm0);
	indexarray.push_back(index0n);
	indexarray.push_back(indexmn);

	for(int ii = 0; ii < uvarray.size(); ii++)
	{
		if(Length(uvarray[ii] - uv1) < 1e-8)
			index1 = indexarray[ii];
		if(Length(uvarray[ii] - uv2) < 1e-8)
			index2 = indexarray[ii];
		if(Length(uvarray[ii] - uv3) < 1e-8)
			index3 = indexarray[ii];
		if(Length(uvarray[ii] - uv4) < 1e-8)
			index4 = indexarray[ii];
	}

	p1[0] = pMesh1->GetVertex(index1)->x();
	p1[1] = pMesh1->GetVertex(index1)->y();
	p1[2] = pMesh1->GetVertex(index1)->z();

	p2[0] = pMesh1->GetVertex(index2)->x();
	p2[1] = pMesh1->GetVertex(index2)->y();
	p2[2] = pMesh1->GetVertex(index2)->z();

	p3[0] = pMesh1->GetVertex(index3)->x();
	p3[1] = pMesh1->GetVertex(index3)->y();
	p3[2] = pMesh1->GetVertex(index3)->z();

	p4[0] = pMesh1->GetVertex(index4)->x();
	p4[1] = pMesh1->GetVertex(index4)->y();
	p4[2] = pMesh1->GetVertex(index4)->z();

	int vnum = pMesh2->NbVertex();

	double c;
	std::vector<std::vector<double>> coeffcients;

	for( TsplineMesh::VertexIter v_iter = pSur->pmesh->vertices_begin(); v_iter!=pSur->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = pSur->pmesh->property(vprop_IMGFlag,v_iter.handle());
		if(flag)
			continue;

		//skip the four corner points
		int cornerflag = pSur->pmesh->property(vprop_corner_label,v_iter.handle());
		if(cornerflag > 0)
			continue;

		double weight = pSur->pmesh->property(vprop_vweights,v_iter.handle());

		std::vector<double> coeff;
		for( TsplineMesh::VertexIter v_iter1 = pSur->pmesh->vertices_begin(); v_iter1!=pSur->pmesh->vertices_end(); ++v_iter1 ) 
		{
			//Skip the Imagination vertex
			int flag = pSur->pmesh->property(vprop_IMGFlag,v_iter1.handle());
			if(flag)
				continue;

			//skip the four corner points
			int cornerflag = pSur->pmesh->property(vprop_corner_label,v_iter1.handle());
			if(cornerflag > 0)
				continue;

			c = 0;
			for(int k = 0; k < vnum; k++)
			{
				CVertex3d* pVertex = pMesh2->GetVertex(k);
				double u = pVertex->x();
				double v = pVertex->y();

				//u = (u - lower.x) / length;
				//v = (v - lower.y) / length;

				//u = (u - lower.x) / (upper.x - lower.x);
				//v = (v - lower.y) / (upper.y - lower.y);

				double basis = pSur->ComputeBasis(v_iter.handle(),u,v);
				double basis1 = pSur->ComputeBasis(v_iter1.handle(),u,v);
				c = c + basis * basis1 ;
			}
			coeff.push_back(c);
		}
		c = 0;
		for(int k = 0; k < vnum; k++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(k);
			CVertex3d* pVertex1 = pMesh1->GetVertex(k);
			double u = pVertex->x();
			double v = pVertex->y();

			//u = (u - lower.x) / length;
			//v = (v - lower.y) / length;

			//u = (u - lower.x) / (upper.x - lower.x);
			//v = (v - lower.y) / (upper.y - lower.y);

			double basis = pSur->ComputeBasis(v_iter.handle(),u,v);
			double basis1 = pSur->ComputeBasis(vh1,u,v);
			double basis2 = pSur->ComputeBasis(vh2,u,v);
			double basis3 = pSur->ComputeBasis(vh3,u,v);
			double basis4 = pSur->ComputeBasis(vh4,u,v);

			c = c + basis * (pVertex1->x() * weight - (basis1 * p1.x + basis2 * p2.x + basis3 * p3.x + basis4 * p4.x));
		}
		coeff.push_back(c);
		c = 0;
		for(int k = 0; k < vnum; k++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(k);
			CVertex3d* pVertex1 = pMesh1->GetVertex(k);
			double u = pVertex->x();
			double v = pVertex->y();

			double basis = pSur->ComputeBasis(v_iter.handle(),u,v);
			double basis1 = pSur->ComputeBasis(vh1,u,v);
			double basis2 = pSur->ComputeBasis(vh2,u,v);
			double basis3 = pSur->ComputeBasis(vh3,u,v);
			double basis4 = pSur->ComputeBasis(vh4,u,v);

			c = c + basis * (pVertex1->y() * weight - (basis1 * p1.y + basis2 * p2.y + basis3 * p3.y + basis4 * p4.y));
		}
		coeff.push_back(c);
		c = 0;
		for(int k = 0; k < vnum; k++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(k);
			CVertex3d* pVertex1 = pMesh1->GetVertex(k);
			double u = pVertex->x();
			double v = pVertex->y();

			//u = (u - lower.x) / length;
			//v = (v - lower.y) / length;

			//u = (u - lower.x) / (upper.x - lower.x);
			//v = (v - lower.y) / (upper.y - lower.y);

			double basis = pSur->ComputeBasis(v_iter.handle(),u,v);
			double basis1 = pSur->ComputeBasis(vh1,u,v);
			double basis2 = pSur->ComputeBasis(vh2,u,v);
			double basis3 = pSur->ComputeBasis(vh3,u,v);
			double basis4 = pSur->ComputeBasis(vh4,u,v);

			c = c + basis * (pVertex1->z() * weight - (basis1 * p1.z + basis2 * p2.z + basis3 * p3.z + basis4 * p4.z));
		}
		coeff.push_back(c);
		c = 0;
		for(int k = 0; k < vnum; k++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(k);
			CVertex3d* pVertex1 = pMesh1->GetVertex(k);
			double u = pVertex->x();
			double v = pVertex->y();

			//u = (u - lower.x) / length;
			//v = (v - lower.y) / length;

			//u = (u - lower.x) / (upper.x - lower.x);
			//v = (v - lower.y) / (upper.y - lower.y);

			double basis = pSur->ComputeBasis(v_iter.handle(),u,v);
			double basis1 = pSur->ComputeBasis(vh1,u,v);
			double basis2 = pSur->ComputeBasis(vh2,u,v);
			double basis3 = pSur->ComputeBasis(vh3,u,v);
			double basis4 = pSur->ComputeBasis(vh4,u,v);

			c = c + basis * (weight - (basis1 + basis2 + basis3 + basis4));
		}
		coeff.push_back(c);
		coeffcients.push_back(coeff);
	}

	using namespace alglib;

	//get the control points
	string str = "[";
	string str1 = "[";
	string str2 = "[";
	string str4 = "[";
	string str3="[";
	for(int i=0;i<coeffcients.size();i++)
	{
		str3 += "[";
		for(int j=0;j<coeffcients.size();j++)
		{
			str3+=DoubleToString(coeffcients[i][j]);
			if(j < coeffcients[i].size() - 5)
				str3 += ",";
		}
		str3 += "]";
		if(i < coeffcients.size() - 1)
			str3 += ",";
		str += DoubleToString(coeffcients[i][coeffcients.size()]);
		str1 += DoubleToString(coeffcients[i][coeffcients.size() + 1]);
		str2 += DoubleToString(coeffcients[i][coeffcients.size() + 2]);
		str4 += DoubleToString(coeffcients[i][coeffcients.size() + 3]);
		if(i < coeffcients.size() - 1)
		{
			str += ",";
			str1 += ",";
			str2 += ",";
			str4 += ",";
		}else
		{
			str += "]";
			str1 += "]";
			str2 += "]";
			str4 += "]";
		}
	}
	str3 += "]";

	real_1d_array	b(str.c_str());
	real_1d_array	b1(str1.c_str());
	real_1d_array	b2(str2.c_str());
	real_1d_array	b3(str4.c_str());
	real_2d_array 	A(str3.c_str());
	real_1d_array   x;
	real_1d_array   y;
	real_1d_array   z;
	real_1d_array   w;
	int64_t val;
	densesolverreport rep;

	rmatrixsolve(A,coeffcients.size(),b,val,rep,x);
	rmatrixsolve(A,coeffcients.size(),b1,val,rep,y);
	rmatrixsolve(A,coeffcients.size(),b2,val,rep,z);
	rmatrixsolve(A,coeffcients.size(),b3,val,rep,w);
	int num = 0;

	//four corner points
	OpenMesh::Vec3d pt1(p1.x,p1.y,p1.z);
	OpenMesh::Vec3d pt2(p2.x,p2.y,p2.z);
	OpenMesh::Vec3d pt3(p3.x,p3.y,p3.z);
	OpenMesh::Vec3d pt4(p4.x,p4.y,p4.z);
	pSur->pmesh->set_point(vh1,pt1);
	pSur->pmesh->set_point(vh2,pt2);
	pSur->pmesh->set_point(vh3,pt3);
	pSur->pmesh->set_point(vh4,pt4);
	pSur->pmesh->property(vprop_vweights,vh1) = 1;
	pSur->pmesh->property(vprop_vweights,vh2) = 1;
	pSur->pmesh->property(vprop_vweights,vh3) = 1;
	pSur->pmesh->property(vprop_vweights,vh4) = 1;

	for( TsplineMesh::VertexIter v_iter = pSur->pmesh->vertices_begin(); v_iter!=pSur->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = pSur->pmesh->property(vprop_IMGFlag,v_iter.handle());
		if(flag)
			continue;

		//skip the four corner points
		int cornerflag = pSur->pmesh->property(vprop_corner_label,v_iter.handle());
		if(cornerflag > 0)
			continue;

		OpenMesh::Vec3d pt;
		pt[0] = x[num] / w[num];
		pt[1] = y[num] / w[num];
		pt[2] = z[num] / w[num];
		
		pSur->pmesh->set_point(v_iter.handle(),pt);
		pSur->pmesh->property(vprop_vweights,v_iter.handle()) = w[num];
		num++;
	}
}
Tspline* Actions_Tspline::OverSimplifyTspline(Tspline* pSurface,CMesh3d* pMesh3D,CMesh3d* pMesh2D,CNurbsSuface* pOriginalSurface)
{
	CNurbsSuface* pSur = new CNurbsSuface;
	pSur->m_udegree = pSurface->udegree;
	pSur->m_vdegree = pSurface->vdegree;
	pSur->m_uknotcount = 8;
	pSur->m_vknotcount = 8;
	pSur->m_uknot = new GLfloat[8];
	pSur->m_vknot = new GLfloat[8];
	for(int i = 0; i < 4; i++)
	{
		pSur->m_uknot[i] = pSurface->umin;
		pSur->m_uknot[i + 4] = pSurface->umax;
		pSur->m_vknot[i] = pSurface->vmin;
		pSur->m_vknot[i + 4] = pSurface->vmax;
	}

	std::vector<int> m_uknotindex;
	std::vector<int> m_vknotindex;
	m_uknotindex.push_back(0);
	m_uknotindex.push_back(1);
	m_uknotindex.push_back(2);
	m_uknotindex.push_back(3);

	m_uknotindex.push_back(pOriginalSurface->m_uknotcount - 4);
	m_uknotindex.push_back(pOriginalSurface->m_uknotcount - 3);
	m_uknotindex.push_back(pOriginalSurface->m_uknotcount - 2);
	m_uknotindex.push_back(pOriginalSurface->m_uknotcount - 1);

	m_vknotindex.push_back(0);
	m_vknotindex.push_back(1);
	m_vknotindex.push_back(2);
	m_vknotindex.push_back(3);

	m_vknotindex.push_back(pOriginalSurface->m_vknotcount - 4);
	m_vknotindex.push_back(pOriginalSurface->m_vknotcount - 3);
	m_vknotindex.push_back(pOriginalSurface->m_vknotcount - 2);
	m_vknotindex.push_back(pOriginalSurface->m_vknotcount - 1);
	Tspline* T_ = new Tspline;
	C_NurbsSurf* pNurbsSurf = Generate_NURBS_Least_Square_Fitting(pMesh3D,pMesh2D,pSur);
	CNurbsSuface* pCNurbs = pNurbsSurf->GetCNurbsSurface();
	T_->copySetting(pSurface);
	T_->BuildFromNURBS(pCNurbs,m_uknotindex,m_vknotindex);

	//For test
	//pCNurbs->m_Transform.Copy(&pMesh3D->m_Transform);
	//pDoc->m_SceneGraph.Add(pCNurbs);
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	return T_;
}
extern string DoubleToString(double d);
void ConvertDouble2DToReal_2d_Array(int m,int n, std::vector<std::vector<double>> &p,real_2d_array& A)
{
	string str3="[";
	for(int i = 0; i < m ;i++)
	{
		str3 += "[";
		for(int j = 0; j < n; j++)
		{
			str3 += DoubleToString(p[i][j]);
			if(j < n - 1)
				str3 += ",";
		}
		str3 += "]";
	}
	str3 += "]";
	real_2d_array 	A1(str3.c_str());
	A = A1;
}
void Actions_Tspline::Compute_RefinementMatrix(Tspline* T_current,Tspline* T_target,alglib::real_2d_array &M_refinement_matrix)
{
	int usamplingdensity = 10;
	int vsamplingdensity = 10;
	
	using namespace alglib;
	
	int m,n_target,n_current;//the dimension of the matrix
	m = (usamplingdensity + 1) * (vsamplingdensity + 1);

	//Four Corner Points
	n_target = T_target->GetNon_ImaginationVertices();
	n_current = T_current->GetNon_ImaginationVertices();

	double threshold = 0;
	int64_t info;
	densesolverlsreport rep;

	M_refinement_matrix.setlength(n_target,n_current);

	alglib::real_2d_array A;
	A.setlength(m,n_target);

	alglib::real_1d_array b,x,row;
	b.setlength(m);

	int c_i = 0;
	int row_index = 0;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_target;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_current;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index_current;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index_target;
	//VPROP_VERTEX_CORNER_LABEL vprop_vertex_corner_label_current;
	//VPROP_VERTEX_CORNER_LABEL vprop_vertex_corner_label_target;

	//vertex imagination flag
	if (!T_target->pmesh->get_property_handle(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG))
		T_target->pmesh->add_property(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG);

	if (!T_current->pmesh->get_property_handle(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG))
		T_current->pmesh->add_property(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG);

	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv_index_current, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_current->pmesh->add_property(vprop_vertex_uv_index_current, PROP_VERTEX_UV_COORDINATE_INDEX);

	if (!T_target->pmesh->get_property_handle(vprop_vertex_uv_index_target, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_target->pmesh->add_property(vprop_vertex_uv_index_target, PROP_VERTEX_UV_COORDINATE_INDEX);

	//if (!T_current->pmesh->get_property_handle(vprop_vertex_corner_label_current, PROP_VERTEX_CORNER_LABEL))
	//	T_current->pmesh->add_property(vprop_vertex_corner_label_current, PROP_VERTEX_CORNER_LABEL);

	//if (!T_target->pmesh->get_property_handle(vprop_vertex_corner_label_target, PROP_VERTEX_CORNER_LABEL))
	//	T_target->pmesh->add_property(vprop_vertex_corner_label_target, PROP_VERTEX_CORNER_LABEL);

	for( TsplineMesh::VertexIter v_iter_simple = T_current->pmesh->vertices_begin(); v_iter_simple!=T_current->pmesh->vertices_end(); ++v_iter_simple) 
	{
		//Skip the Imagination vertex
		int flag = T_current->pmesh->property(vprop_IMGFlag_current,v_iter_simple.handle());
		if(flag)
			continue;

		//skip the four corner points
		//int cornerlabel_current = T_current->pmesh->property(vprop_vertex_corner_label_current,v_iter_simple.handle());
		//if(cornerlabel_current > 0)
		//	continue;
		
		row_index = 0;
		for(int i = 0; i <= usamplingdensity; i++)
		{
			for(int j = 0; j <= vsamplingdensity; j++)
			{

				//u,v
				double u = T_current->umin + (T_current->umax - T_current->umin) * (double)i / (usamplingdensity);
				double v = T_current->vmin + (T_current->vmax - T_current->vmin) * (double)j / (vsamplingdensity);

				double val = T_current->ComputeBasis(v_iter_simple.handle(),u,v);
				b[row_index] = val;

				int c_j = 0;
				for( TsplineMesh::VertexIter v_iter = T_target->pmesh->vertices_begin(); v_iter!=T_target->pmesh->vertices_end(); ++v_iter ) 
				{
					int flag1 = T_target->pmesh->property(vprop_IMGFlag_target,v_iter.handle());
					if(flag1)
						continue;

					//skip the four corner points
					//int cornerlabel_target = T_target->pmesh->property(vprop_vertex_corner_label_target,v_iter.handle());
					//if(cornerlabel_target > 0)
					//	continue;

					double val = T_target->ComputeBasis(v_iter.handle(),u,v);
					A[row_index][c_j++] = val;
				}

				row_index++;
			}
		}
		
		//alglib::real_2d_array Atranspose;
		//Atranspose.setlength(n,m);
		//rmatrixtranspose(m,n, A,0,0,Atranspose,0,0);
		
		//for debug
		/*TRACE("\n");
		for(int i = 0; i < A.rows(); i++)
		{
			for(int j = 0; j < A.cols(); j++)
			{
				TRACE(" %f ",A[i][j]);
			}
			TRACE("\n");
		}
		TRACE("\n");

		for(int j = 0; j < b.length(); j++)
		{
			TRACE(" %f ",b[j]);
		}
		TRACE("\n");*/

		//alglib::real_2d_array A_new;
		//A_new.setlength(n_target,n_target);

		//At*A
		//rmatrixgemm(n_target,n_target,m,1,A,0,0,1,A,0,0,0,0,A_new,0,0);
		/*TRACE("\n");
		for(int i = 0; i < A_new.rows(); i++)
		{
			for(int j = 0; j < A_new.cols(); j++)
			{
				TRACE(" %f ",A_new[i][j]);
			}
			TRACE("\n");
		}*/

		//alglib::real_1d_array b_new;
		//b_new.setlength(n_target);

		//At*b
		//rmatrixmv(n_target,m,A,0,0,1,b,0,b_new,0);
		/*TRACE("\n");
		for(int j = 0; j < b_new.length(); j++)
		{
			TRACE(" %f ",b_new[j]);
		}
		TRACE("\n");*/

		real_1d_array x;

		//get the result
		//A_new * x = b_new
		//alglib::rmatrixsolvels(A_new,n_target,n_target,b_new,threshold,info,rep,x);
		alglib::rmatrixsolvels(A,m,n_target,b,threshold,info,rep,x);
		/*TRACE("\n");
		for(int j = 0; j < x.length(); j++)
		{
			TRACE(" %f ",x[j]);
		}
		TRACE("\n");*/
		//verify x

		//A*x-b
		/*real_1d_array temp;
		temp.setlength(m);
		rmatrixmv(m,n_target,A,0,0,0,x,0,temp,0);
		
		real_1d_array TVec;
		TVec.setlength(temp.length());
		double maxvalue = 0;
		for(int ii = 0; ii < temp.length(); ii++)
		{
			TVec[ii] = fabs(temp[ii] - b[ii]);
			//TRACE("%f %f\n",temp[ii],b[ii]);
			if(TVec[ii] > maxvalue)
			{
				maxvalue = TVec[ii];
			}
		}*/

		//
		for(int ii = 0; ii < x.length(); ii++)
			M_refinement_matrix[ii][c_i] = x[ii];
		++c_i;
	}

	ValidateRefinementMatrix(T_current,T_target,M_refinement_matrix);
	//alglib::real_2d_array M_new;
	//M_new.setlength(n_current,n_current);

	//Mt*M
	//rmatrixgemm(n_current,n_current,n_target,1,M_refinement_matrix,0,0,1,M_refinement_matrix,0,0,0,0,M_new,0,0);

	//Set the  current mesh points and weights
	//Compute the weights
	//ComputeWeights(T_target,T_current,M_refinement_matrix);

	//Compute the x
	//ComputePosition(T_target,T_current,M_refinement_matrix,0);

	//Compute the y
	//ComputePosition(T_target,T_current,M_refinement_matrix,1);

	//Compute the z
	//ComputePosition(T_target,T_current,M_refinement_matrix,2);
}
void Actions_Tspline::ValidateRefinementMatrix(Tspline* T_current,Tspline* T_target,alglib::real_2d_array &M_refinement_matrix)
{
	alglib::real_2d_array w_target,w_target_new,w_current;

	w_target.setlength(T_target->GetNon_ImaginationVertices(),4);
	w_current.setlength(T_current->GetNon_ImaginationVertices(),4);

	VPROP_VERTEX_WEIGHT vprop_vweights_target;
	VPROP_VERTEX_WEIGHT vprop_vweights_current;
	VPROP_VERTEX_WEIGHT vprop_vweights_target_bak;
	VPROP_VERTEX_OFFENDING_FLAG vprop_vertex_offending;

	//Vertex weight
	if (!T_target->pmesh->get_property_handle(vprop_vweights_target, PROP_VERTEX_WEIGHT))
		T_target->pmesh->add_property(vprop_vweights_target, PROP_VERTEX_WEIGHT);

	if (!T_current->pmesh->get_property_handle(vprop_vweights_current, PROP_VERTEX_WEIGHT))
		T_current->pmesh->add_property(vprop_vweights_current, PROP_VERTEX_WEIGHT);

	//Vertex offending
	if (!T_target->pmesh->get_property_handle(vprop_vertex_offending, PROP_VERTEX_OFFENDING_FLAG))
		T_target->pmesh->add_property(vprop_vertex_offending, PROP_VERTEX_OFFENDING_FLAG);

	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_target;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_current;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_target_bak;

	//vertex imagination flag
	if (!T_target->pmesh->get_property_handle(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG))
		T_target->pmesh->add_property(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG);

	if (!T_current->pmesh->get_property_handle(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG))
		T_current->pmesh->add_property(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG);

	VPROP_VERTEX_CORNER_LABEL vprop_vertex_corner_label_target;
	VPROP_VERTEX_CORNER_LABEL vprop_vertex_corner_label_current;
	//vertex corner flag
	if (!T_target->pmesh->get_property_handle(vprop_vertex_corner_label_target, PROP_VERTEX_CORNER_LABEL))
		T_target->pmesh->add_property(vprop_vertex_corner_label_target, PROP_VERTEX_CORNER_LABEL);

	if (!T_current->pmesh->get_property_handle(vprop_vertex_corner_label_current, PROP_VERTEX_CORNER_LABEL))
		T_current->pmesh->add_property(vprop_vertex_corner_label_current, PROP_VERTEX_CORNER_LABEL);


	int i = 0;
	OpenMesh::Vec3d pt,pt1;
	double weight;
	for( TsplineMesh::VertexIter v_iter = T_current->pmesh->vertices_begin(); v_iter!=T_current->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = T_current->pmesh->property(vprop_IMGFlag_current,v_iter.handle());
		if(flag)
			continue;

		//Get the weight
		weight = T_current->pmesh->property(vprop_vweights_current,v_iter.handle());

		w_current[i][3] = weight;

		//vertex position
		pt = T_current->pmesh->point (v_iter.handle());

		//Set the homogenous position
		w_current[i][0] = pt[0] * weight;
		w_current[i][1] = pt[1] * weight;
		w_current[i][2] = pt[2] * weight;

		i++;
	}

	w_target_new.setlength(T_target->GetNon_ImaginationVertices(),4);
	//compute the current position
	double val;
	for(int index = 0; index < 4; index++)
	{
		for(i = 0; i < M_refinement_matrix.rows(); i++)
		{
			val = 0;
			for(int j = 0; j < M_refinement_matrix.cols(); j++)
			{
				val += w_current[j][index] * M_refinement_matrix[i][j];
			}
			w_target_new[i][index] = val;
		}
	}
	Tspline* T_target_bak = T_target->copy();
	if (!T_target_bak->pmesh->get_property_handle(vprop_IMGFlag_target_bak, PROP_VERTEX_IMG_FLAG))
		T_target_bak->pmesh->add_property(vprop_IMGFlag_target_bak, PROP_VERTEX_IMG_FLAG);

	if (!T_target_bak->pmesh->get_property_handle(vprop_vweights_target_bak, PROP_VERTEX_WEIGHT))
		T_target_bak->pmesh->add_property(vprop_vweights_target_bak, PROP_VERTEX_WEIGHT);

	i = 0;
	for( TsplineMesh::VertexIter v_iter = T_target_bak->pmesh->vertices_begin(); v_iter!=T_target_bak->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = T_target_bak->pmesh->property(vprop_IMGFlag_target_bak,v_iter.handle());
		if(flag)
			continue;

		weight = w_target_new[i][3];
		pt[0] = w_target_new[i][0] / weight;
		pt[1] = w_target_new[i][1] / weight;
		pt[2] = w_target_new[i][2] / weight;

		T_target_bak->pmesh->set_point(v_iter.handle(),pt);
		T_target_bak->pmesh->property(vprop_vweights_target_bak,v_iter.handle()) = weight;
		
		i++;
	}

	double u1 = T_target_bak->umin * 0.1 + T_target_bak->umax * 0.9;
	double v1 = T_target_bak->umin * 0.3 + T_target_bak->umax * 0.7;
	Point3D p1_ = T_target_bak->ComputePosition(u1,v1);
	Point3D p1 = T_current->ComputePosition(u1,v1);
	double len1 = Length(p1 - p1_);
	ASSERT(len1 < 1e-8);

	//Point3D p2_ = T_target_bak->ComputePosition(0.5,0.3);
	//Point3D p2 = T_current->ComputePosition(0.5,0.3);
	//double len2 = Length(p2 - p2_);
	//ASSERT(len2 < 1e-8);
}
void Actions_Tspline::LabelFourCornerPoints(Tspline* T_mesh)
{
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index;
	VPROP_VERTEX_CORNER_LABEL vprop_vertex_corner_label;

	//vertex imagination flag
	if (!T_mesh->pmesh->get_property_handle(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG))
		T_mesh->pmesh->add_property(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG);

	if (!T_mesh->pmesh->get_property_handle(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_mesh->pmesh->add_property(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	if (!T_mesh->pmesh->get_property_handle(vprop_vertex_corner_label, PROP_VERTEX_CORNER_LABEL))
		T_mesh->pmesh->add_property(vprop_vertex_corner_label, PROP_VERTEX_CORNER_LABEL);

	//find the four corner points
	int umin_index = 1e10,umax_index = -1e10;
	int vmin_index = 1e10,vmax_index = -1e10;

	for( TsplineMesh::VertexIter v_iter_simple = T_mesh->pmesh->vertices_begin(); v_iter_simple!=T_mesh->pmesh->vertices_end(); ++v_iter_simple) 
	{
		//Skip the Imagination vertex
		int flag = T_mesh->pmesh->property(vprop_IMGFlag,v_iter_simple.handle());
		if(flag)
			continue;

		std::vector<int> uv_index = T_mesh->pmesh->property(vprop_vertex_uv_index,v_iter_simple.handle());
		if(uv_index[0] < umin_index)
			umin_index = uv_index[0];
		if(uv_index[0] > umax_index)
			umax_index = uv_index[0];
		if(uv_index[1] < vmin_index)
			vmin_index = uv_index[1];
		if(uv_index[1] > vmax_index)
			vmax_index = uv_index[1];
	}
	for( TsplineMesh::VertexIter v_iter_simple = T_mesh->pmesh->vertices_begin(); v_iter_simple!=T_mesh->pmesh->vertices_end(); ++v_iter_simple) 
	{
		//Skip the Imagination vertex
		int flag = T_mesh->pmesh->property(vprop_IMGFlag,v_iter_simple.handle());
		if(flag)
			continue;

		std::vector<int> uv_index = T_mesh->pmesh->property(vprop_vertex_uv_index,v_iter_simple.handle());
		if(uv_index[0] == umin_index && uv_index[1] == vmin_index)
			T_mesh->pmesh->property(vprop_vertex_corner_label,v_iter_simple.handle()) = 1;
		else
		if(uv_index[0] == umax_index && uv_index[1] == vmin_index)
			T_mesh->pmesh->property(vprop_vertex_corner_label,v_iter_simple.handle()) = 2;
		else
		if(uv_index[0] == umin_index && uv_index[1] == vmax_index)
			T_mesh->pmesh->property(vprop_vertex_corner_label,v_iter_simple.handle()) = 3;
		else
		if(uv_index[0] == umax_index && uv_index[1] == vmax_index)
			T_mesh->pmesh->property(vprop_vertex_corner_label,v_iter_simple.handle()) = 4;
		else
			T_mesh->pmesh->property(vprop_vertex_corner_label,v_iter_simple.handle()) = 0;
	}

}
void Actions_Tspline::LeastSquareFitting_Tspline(Tspline* T_current,Tspline* T_target)
{
	//Least Square Fitting

	//first sampling the Tspline mesh
	CMesh3d* pMesh3d,*pMesh2d;
	int samplingm = 100;
	int samplingn = 100;
	T_target->GenerateSamplingMesh(pMesh3d,pMesh2d,samplingm,samplingn);

	pMesh3d->m_Transform.Copy(&T_target->m_Transform);
	pMesh2d->m_Transform.Copy(&T_target->m_Transform);
	pDoc->m_SceneGraph.Add(pMesh3d);
	pDoc->m_SceneGraph.Add(pMesh2d);
	//Least Square
	Generate_Tspline_Least_Square_Fitting(pMesh3d,pMesh2d,T_current);
	//Generate_Tspline_Least_Square_Fitting_Homogeneity(pMesh3d,pMesh2d,T_current);
}
double Actions_Tspline::LeastSquareFit(Tspline* T_current,Tspline* T_target,double TOL,BOOL fitting)
{
	//Given the current and target meshes, 
	//first compute the refinement matrix
	//then compute the fitting error 
	//

	alglib::real_2d_array M_refinement_matrix;

	//label four corner points
	LabelFourCornerPoints(T_current);

	LabelFourCornerPoints(T_target);

	//compute the refinement matrix
	Compute_RefinementMatrix(T_current,T_target,M_refinement_matrix);

	if(fitting)
	{
		LeastSquareFitting_Tspline(T_current,T_target);
		//Set the current mesh
		//ComputeWeights(T_target,T_current,M_refinement_matrix);

		//Compute the x
		//ComputePosition(T_target,T_current,M_refinement_matrix,0);

		//Compute the y
		//ComputePosition(T_target,T_current,M_refinement_matrix,1);

		//Compute the z
		//ComputePosition(T_target,T_current,M_refinement_matrix,2);

	}

	//return the fitting error
	double error = GetFittingError(T_target,T_current,M_refinement_matrix,TOL);

	return error;
}
double Actions_Tspline::GetFittingError(Tspline* T_target,Tspline* T_current,alglib::real_2d_array &M_refinement_matrix,double TOL)
{
	//T_target  the original mesh to be approximated, served as a target mesh
	//T_current the current mesh

	alglib::real_2d_array w_target,w_target_new,w_current;

	w_target.setlength(T_target->GetNon_ImaginationVertices(),4);
	w_current.setlength(T_current->GetNon_ImaginationVertices(),4);

	VPROP_VERTEX_WEIGHT vprop_vweights_target;
	VPROP_VERTEX_WEIGHT vprop_vweights_current;
	VPROP_VERTEX_OFFENDING_FLAG vprop_vertex_offending;

	//Vertex weight
	if (!T_target->pmesh->get_property_handle(vprop_vweights_target, PROP_VERTEX_WEIGHT))
		T_target->pmesh->add_property(vprop_vweights_target, PROP_VERTEX_WEIGHT);

	if (!T_current->pmesh->get_property_handle(vprop_vweights_current, PROP_VERTEX_WEIGHT))
		T_current->pmesh->add_property(vprop_vweights_current, PROP_VERTEX_WEIGHT);

	//Vertex offending
	if (!T_target->pmesh->get_property_handle(vprop_vertex_offending, PROP_VERTEX_OFFENDING_FLAG))
		T_target->pmesh->add_property(vprop_vertex_offending, PROP_VERTEX_OFFENDING_FLAG);

	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_target;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_current;

	//vertex imagination flag
	if (!T_target->pmesh->get_property_handle(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG))
		T_target->pmesh->add_property(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG);

	if (!T_current->pmesh->get_property_handle(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG))
		T_current->pmesh->add_property(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG);

	VPROP_VERTEX_CORNER_LABEL vprop_vertex_corner_label_target;
	VPROP_VERTEX_CORNER_LABEL vprop_vertex_corner_label_current;
	//vertex corner flag
	if (!T_target->pmesh->get_property_handle(vprop_vertex_corner_label_target, PROP_VERTEX_CORNER_LABEL))
		T_target->pmesh->add_property(vprop_vertex_corner_label_target, PROP_VERTEX_CORNER_LABEL);

	if (!T_current->pmesh->get_property_handle(vprop_vertex_corner_label_current, PROP_VERTEX_CORNER_LABEL))
		T_current->pmesh->add_property(vprop_vertex_corner_label_current, PROP_VERTEX_CORNER_LABEL);

	//Initialization set the vertex offending flag as zero.
	for( TsplineMesh::VertexIter v_iter = T_target->pmesh->vertices_begin(); v_iter!=T_target->pmesh->vertices_end(); ++v_iter ) 
	{
		T_target->pmesh->property(vprop_vertex_offending,v_iter.handle()) = 0;
	}

	int i = 0;
	OpenMesh::Vec3d pt,pt1;
	double weight;
	for( TsplineMesh::VertexIter v_iter = T_current->pmesh->vertices_begin(); v_iter!=T_current->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = T_current->pmesh->property(vprop_IMGFlag_current,v_iter.handle());
		if(flag)
			continue;

		//int cornerflag = T_current->pmesh->property(vprop_vertex_corner_label_current,v_iter.handle());
		//if(cornerflag > 0)
		//	continue;

		//Get the weight
		weight = T_current->pmesh->property(vprop_vweights_current,v_iter.handle());

		w_current[i][3] = weight;

		//vertex position
		pt = T_current->pmesh->point (v_iter.handle());
	
		//Set the homogenous position
		w_current[i][0] = pt[0] * weight;
		w_current[i][1] = pt[1] * weight;
		w_current[i][2] = pt[2] * weight;

		i++;
	}

	w_target_new.setlength(T_target->GetNon_ImaginationVertices(),4);
	//compute the current position
	double val;
	for(int index = 0; index < 4; index++)
	{
		for(i = 0; i < M_refinement_matrix.rows(); i++)
		{
			val = 0;
			for(int j = 0; j < M_refinement_matrix.cols(); j++)
			{
				val += w_current[j][index] * M_refinement_matrix[i][j];
			}
			w_target_new[i][index] = val;
		}
	}

	//Compute the difference
	i = 0; 
	double maxd = -1e10;
	Vector4D v1,v2;
 	for( TsplineMesh::VertexIter v_iter = T_target->pmesh->vertices_begin(); v_iter!=T_target->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = T_target->pmesh->property(vprop_IMGFlag_target,v_iter.handle());
		if(flag)
			continue;

		//int cornerflag = T_target->pmesh->property(vprop_vertex_corner_label_target,v_iter.handle());
		//if(cornerflag > 0)
		//	continue;

		//vertex position
		pt = T_target->pmesh->point (v_iter.handle());

		//Get the weight
		weight = T_target->pmesh->property(vprop_vweights_target,v_iter.handle());

		v1[0] = pt[0] * weight;
		v1[1] = pt[1] * weight;
		v1[2] = pt[2] * weight;
		v1[3] = weight;

		v2[0] = w_target_new[i][0];
		v2[1] = w_target_new[i][1];
		v2[2] = w_target_new[i][2];
		v2[3] = w_target_new[i][3];

		double dis = Length(v1 - v2);
		if(dis > TOL)
		{
			//set the offending flag
			T_target->pmesh->property(vprop_vertex_offending,v_iter.handle()) = 1;
		}
		if(dis > maxd)
			maxd = dis;
		i++;
	}
	return maxd;
}
void Actions_Tspline::ComputeWeights(Tspline* T_target,Tspline* T_current,alglib::real_2d_array &M_refinement_matrix)
{
	int m = M_refinement_matrix.rows();
	int n = M_refinement_matrix.cols();

	double threshold = 0;
	int64_t info;
	densesolverlsreport rep;

	//solve the weights
	alglib::real_1d_array w_targetweight,w_currentweight;

	w_currentweight.setlength(n);
	w_targetweight.setlength(m);
	VPROP_VERTEX_WEIGHT vprop_vweights_target;
	VPROP_VERTEX_WEIGHT vprop_vweights_current;


	//Vertex weight
	if (!T_target->pmesh->get_property_handle(vprop_vweights_target, PROP_VERTEX_WEIGHT))
		T_target->pmesh->add_property(vprop_vweights_target, PROP_VERTEX_WEIGHT);

	if (!T_current->pmesh->get_property_handle(vprop_vweights_current, PROP_VERTEX_WEIGHT))
		T_current->pmesh->add_property(vprop_vweights_current, PROP_VERTEX_WEIGHT);

	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_target;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_current;

	//vertex imagination flag
	if (!T_target->pmesh->get_property_handle(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG))
		T_target->pmesh->add_property(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG);

	if (!T_current->pmesh->get_property_handle(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG))
		T_current->pmesh->add_property(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG);

	//VPROP_VERTEX_CORNER_LABEL vprop_corner_label_current;
	//VPROP_VERTEX_CORNER_LABEL vprop_corner_label_target;

	//vertex corner label
	//if (!T_target->pmesh->get_property_handle(vprop_corner_label_target, PROP_VERTEX_CORNER_LABEL))
	//	T_target->pmesh->add_property(vprop_corner_label_target, PROP_VERTEX_CORNER_LABEL);

	//if (!T_current->pmesh->get_property_handle(vprop_corner_label_current, PROP_VERTEX_CORNER_LABEL))
	//	T_current->pmesh->add_property(vprop_corner_label_current, PROP_VERTEX_CORNER_LABEL);

	int i = 0;
	double leftbottomweight,rightbottomweight,topleftweight,toprightweight;
	for( TsplineMesh::VertexIter v_iter = T_target->pmesh->vertices_begin(); v_iter!=T_target->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = T_target->pmesh->property(vprop_IMGFlag_target,v_iter.handle());
		if(flag)
			continue;
		//skip corner points
		//int cornerlabel = T_target->pmesh->property(vprop_corner_label_target,v_iter.handle());
		//if(cornerlabel == 1)
		//	leftbottomweight = T_target->pmesh->property(vprop_vweights_target,v_iter.handle());
		//if(cornerlabel == 2)
		//	rightbottomweight = T_target->pmesh->property(vprop_vweights_target,v_iter.handle());
		//if(cornerlabel == 3)
		//	topleftweight = T_target->pmesh->property(vprop_vweights_target,v_iter.handle());
		//if(cornerlabel == 4)
		//	toprightweight = T_target->pmesh->property(vprop_vweights_target,v_iter.handle());
		//if(cornerlabel > 0)
		//	continue;

		w_targetweight[i++] = T_target->pmesh->property(vprop_vweights_target,v_iter.handle());
	}

	alglib::real_2d_array M_new;
	M_new.setlength(n,n);

	//Mt*M
	rmatrixgemm(n,n,m,1,M_refinement_matrix,0,0,1,M_refinement_matrix,0,0,0,0,M_new,0,0);

	alglib::real_1d_array w_targetweight_new;
	w_targetweight_new.setlength(w_targetweight.length());

	//Mt*b
	rmatrixmv(n,m,M_refinement_matrix,0,0,1,w_targetweight,0,w_targetweight_new,0);

	//M*current = target
	//rmatrixmv(m,n,M_refinement_matrix,0,0,0,w_currentweight,0,w_targetweight,0);

	//
	//get the weight
	//Mt*M*w_currentweight = Mt*w_targetweight
	alglib::rmatrixsolvels(M_new,n,n,w_targetweight_new,threshold,info,rep,w_currentweight);

	//
	//Set the Vertex weight for the current mesh T_current
	if (!T_current->pmesh->get_property_handle(vprop_vweights_current, PROP_VERTEX_WEIGHT))
		T_current->pmesh->add_property(vprop_vweights_current, PROP_VERTEX_WEIGHT);

	i = 0;
	for( TsplineMesh::VertexIter v_iter = T_current->pmesh->vertices_begin(); v_iter!=T_current->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = T_current->pmesh->property(vprop_IMGFlag_current,v_iter.handle());
		if(flag)
			continue;

		//skip corner points
		//int cornerlabel = T_current->pmesh->property(vprop_corner_label_current,v_iter.handle());
		//Set the four corner points
		//if(cornerlabel == 1)
		//	T_current->pmesh->property(vprop_vweights_current,v_iter.handle()) = leftbottomweight;
		//if(cornerlabel == 2)
		//	T_current->pmesh->property(vprop_vweights_current,v_iter.handle()) = rightbottomweight;
		//if(cornerlabel == 3)
		//	T_current->pmesh->property(vprop_vweights_current,v_iter.handle()) = topleftweight;
		//if(cornerlabel == 4)
		//	T_current->pmesh->property(vprop_vweights_current,v_iter.handle()) = toprightweight;
		//if(cornerlabel > 0)
		//	continue;

		//TRACE("%f \n",w_currentweight[i]);
		T_current->pmesh->property(vprop_vweights_current,v_iter.handle()) = w_currentweight[i++];
	}
}
void Actions_Tspline::ComputePosition(Tspline* T_target,Tspline* T_current,alglib::real_2d_array &M_refinement_matrix,int index)
{
	int m = M_refinement_matrix.rows();
	int n = M_refinement_matrix.cols();

	//index = 0,1,2
	//        x,y,z

	double threshold = 0;
	int64_t info;
	densesolverlsreport rep;

	//solve the weights
	alglib::real_1d_array w_target,w_current;

	VPROP_VERTEX_WEIGHT vprop_vweights_target;
	VPROP_VERTEX_WEIGHT vprop_vweights_current;

	//Vertex weight
	if (!T_current->pmesh->get_property_handle(vprop_vweights_current, PROP_VERTEX_WEIGHT))
		T_current->pmesh->add_property(vprop_vweights_current, PROP_VERTEX_WEIGHT);

	if (!T_target->pmesh->get_property_handle(vprop_vweights_target, PROP_VERTEX_WEIGHT))
		T_target->pmesh->add_property(vprop_vweights_target, PROP_VERTEX_WEIGHT);

	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_target;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_current;

	//vertex imagination flag
	if (!T_target->pmesh->get_property_handle(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG))
		T_target->pmesh->add_property(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG);

	if (!T_current->pmesh->get_property_handle(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG))
		T_current->pmesh->add_property(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG);

	//VPROP_VERTEX_CORNER_LABEL vprop_corner_label_current;
	//VPROP_VERTEX_CORNER_LABEL vprop_corner_label_target;

	//vertex corner label
	//if (!T_target->pmesh->get_property_handle(vprop_corner_label_target, PROP_VERTEX_CORNER_LABEL))
	//	T_target->pmesh->add_property(vprop_corner_label_target, PROP_VERTEX_CORNER_LABEL);

	//if (!T_current->pmesh->get_property_handle(vprop_corner_label_current, PROP_VERTEX_CORNER_LABEL))
	//	T_current->pmesh->add_property(vprop_corner_label_current, PROP_VERTEX_CORNER_LABEL);

	//reserve the space
	w_current.setlength(n);
	w_target.setlength(m);

	OpenMesh::Vec3d pt;
	int i = 0;
	double weight;
	double leftbottomval,rightbottomval,topleftval,toprightval;
	for( TsplineMesh::VertexIter v_iter = T_target->pmesh->vertices_begin(); v_iter!=T_target->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = T_target->pmesh->property(vprop_IMGFlag_target,v_iter.handle());
		if(flag)
			continue;

		//vertex position
		pt = T_target->pmesh->point (v_iter.handle());

		//int cornerflag = T_target->pmesh->property(vprop_corner_label_target,v_iter.handle());
		//if(cornerflag == 1)
		//	leftbottomval = pt[index];
		//else
		//if(cornerflag == 2)
		//	rightbottomval = pt[index];
		//else
		//if(cornerflag == 3)
		//	topleftval = pt[index];
		//else
		//if(cornerflag == 4)
		//	toprightval = pt[index];

		//if(cornerflag > 0)
		//	continue;
		
		weight = T_target->pmesh->property(vprop_vweights_target,v_iter.handle());
		w_target[i++] = pt[index] * weight;
	}

	alglib::real_2d_array M_new;
	M_new.setlength(n,n);

	//Mt*M
	rmatrixgemm(n,n,m,1,M_refinement_matrix,0,0,1,M_refinement_matrix,0,0,0,0,M_new,0,0);

	alglib::real_1d_array w_targetweight_new;
	w_targetweight_new.setlength(w_target.length());

	//Mt*b
	rmatrixmv(n,m,M_refinement_matrix,0,0,1,w_target,0,w_targetweight_new,0);

	//M * x
	//rmatrixmv(m,n,M_refinement_matrix,0,0,0,w_current,0,w_target,0);

	//
	//get the position
	//Mt*M*w_current = Mt*w_target_new
	alglib::rmatrixsolvels(M_new,n,n,w_targetweight_new,threshold,info,rep,w_current);

	//
	//Set the Vertex Position for the current mesh T_
	if (!T_current->pmesh->get_property_handle(vprop_vweights_current, PROP_VERTEX_WEIGHT))
		T_current->pmesh->add_property(vprop_vweights_current, PROP_VERTEX_WEIGHT);

	i = 0;
	for( TsplineMesh::VertexIter v_iter = T_current->pmesh->vertices_begin(); v_iter!=T_current->pmesh->vertices_end(); ++v_iter ) 
	{
		//Skip the Imagination vertex
		int flag = T_current->pmesh->property(vprop_IMGFlag_current,v_iter.handle());
		if(flag)
			continue;

		weight = T_current->pmesh->property(vprop_vweights_current,v_iter.handle());
		pt = T_current->pmesh->point (v_iter.handle());

		//int cornerflag = T_current->pmesh->property(vprop_corner_label_current,v_iter.handle());
		//if(cornerflag == 1)
		//	pt[index] = leftbottomval;
		//else
		//if(cornerflag == 2)
		//	pt[index] = rightbottomval;
		//else
		//if(cornerflag == 3)
		//	pt[index] = topleftval;
		//else
		//if(cornerflag == 4)
		//	pt[index] = toprightval;
		//else
		{
			pt[index] = w_current[i++] / weight;
		}

		T_current->pmesh->set_point(v_iter.handle(),pt);
	}
}
double Actions_Tspline::GetFaceError(Tspline* T_target,Tspline* T_current,TsplineMesh::FaceHandle handle)
{

	return 0;
}

void Actions_Tspline::SubdivideFace(Tspline* T_current,TsplineMesh::FaceHandle handle)
{
	//TRACE("Subdivide Face %d \n",handle.idx());
	if(handle.idx() == 74)
	{
		int ii = 0;
	}
	FPROP_FACE_UK_BOUNDING_BOX fprop_face_bounding_box;
	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index;

	//Vertex UV Coordinate
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE))
		T_current->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE);

	//Vertex UV Coordinate Index
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_current->pmesh->add_property(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	//Face bounding box
	if (!T_current->pmesh->get_property_handle(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX))
		T_current->pmesh->add_property(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX);

	std::vector<double> boundingbox = T_current->pmesh->property(fprop_face_bounding_box,handle);

	ASSERT(boundingbox.size() == 4);

	double umin = boundingbox[0];
	double umax = boundingbox[1];
	double vmin = boundingbox[2];
	double vmax = boundingbox[3];

	int uknot_num_inface = 0,vknot_num_inface = 0;
	int umin_index,umax_index,vmin_index,vmax_index;

	umin_index = 1e10;
	vmin_index = 1e10;
	umax_index = -1e10;
	vmax_index = -1e10;
	for(TsplineMesh::FaceVertexIter fv_it = T_current->pmesh->fv_iter(handle); fv_it; ++fv_it )
	{
		Vector2D uv = T_current->pmesh->property(vprop_vertex_uv, fv_it.handle());
		std::vector<int> uvindex = T_current->pmesh->property(vprop_vertex_uv_index, fv_it.handle());
			
		if(fabs(uv.x - umin) < 1e-8)
		{
			if(uvindex[0] < umin_index)
			umin_index = uvindex[0];
		}
		if(fabs(uv.x - umax) < 1e-8)
		{
			if(uvindex[0] > umax_index)
				umax_index = uvindex[0];
		}
		if(fabs(uv.y - vmin) < 1e-8)
		{
			if(uvindex[1] < vmin_index)
				vmin_index = uvindex[1];
		}
		if(fabs(uv.y - vmax) < 1e-8)
		{
			if(uvindex[1] > vmax_index)
				vmax_index = uvindex[1];
		}
	}
	/*for(int i = 0; i < T_current->m_targetNurbsUKnots.size(); i++)
	{
		double u = T_current->m_targetNurbsUKnots[i];
		if(fabs(umin - u) < 1e-8)
			umin_index = i;
		//else
		if(fabs(umax - u) < 1e-8)
			umax_index = i;
	}
	for(int i = 0; i < T_current->m_targetNurbsVKnots.size(); i++)
	{
		double v = T_current->m_targetNurbsVKnots[i];
		if(fabs(vmin - v) < 1e-8)
			vmin_index = i;
		//else
		if(fabs(vmax - v) < 1e-8)
				vmax_index = i;
	}*/
	//umin_index = 2;
	//umax_index = T_current->m_targetNurbsUKnots.size() -  3;

	//vmin_index = 2;
	//vmax_index = T_current->m_targetNurbsVKnots.size() -  3;

	/*for(int i = 0; i < T_current->m_targetNurbsUKnots.size(); i++)
	{
		double u = T_current->m_targetNurbsUKnots[i];
		if(u > umin && u < umax)
			uknot_num_inface++;
	}
	for(int i = 0; i < T_current->m_targetNurbsVKnots.size(); i++)
	{
		double v = T_current->m_targetNurbsVKnots[i];
		if(v > vmin && v < vmax)
			vknot_num_inface++;
	}*/
	uknot_num_inface = umax_index - umin_index - 1;
	vknot_num_inface = vmax_index - vmin_index - 1;

	if(uknot_num_inface == 0 && vknot_num_inface == 0)
		return;

	if(uknot_num_inface >= vknot_num_inface)
	{
		SplitVertical(T_current,handle,uknot_num_inface,vknot_num_inface,umin_index,umax_index,umin,umax,vmin,vmax,vmin_index,vmax_index);
	}else
	{
		SplitHorizontal(T_current,handle,uknot_num_inface,vknot_num_inface,vmin_index,vmax_index,umin,umax,vmin,vmax,umin_index,umax_index);
	}

	//modify the mesh
	//T_current->pmesh->garbage_collection();
}
void Actions_Tspline::SplitHorizontal(Tspline* T_current,TsplineMesh::FaceHandle handle,int uknot_num_inface,int vknot_num_inface,int vmin_index,int vmax_index,double umin,double umax,double vmin,double vmax,int umin_index,int umax_index)
{
	FPROP_FACE_UK_BOUNDING_BOX fprop_face_bounding_box;
	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index;


	//Vertex UV Coordinate Index
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_current->pmesh->add_property(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	//Vertex UV Coordinate
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE))
		T_current->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE);

	//Face bounding box
	if (!T_current->pmesh->get_property_handle(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX))
		T_current->pmesh->add_property(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX);


	//Subdivide the face vertically
	int index = (vmin_index + vmax_index) / 2;
	double v0 = T_current->m_targetNurbsVKnots[index];
	double u0 = umin;
	Vector2D p0(u0,v0);
	std::vector<int> p0_index;
	p0_index.push_back(umin_index);
	p0_index.push_back(index);
	//Vector2D p0_index(umin_index,index);

	double v1 = T_current->m_targetNurbsVKnots[index];
	double u1 = umax;
	Vector2D p1(u1,v1);
	std::vector<int> p1_index;
	p1_index.push_back(umax_index);
	p1_index.push_back(index);
	//Vector2D p1_index(umax_index,index);

	TsplineMesh::VertexHandle vh1,vh2;
	BOOL findflag1 = FALSE;
	BOOL findflag2 = FALSE;
	//search for the two end points
	for( TsplineMesh::VertexIter v_iter = T_current->pmesh->vertices_begin(); v_iter!=T_current->pmesh->vertices_end(); ++v_iter ) 
	{
		//Vector2D uv = T_current->pmesh->property(vprop_vertex_uv,v_iter.handle());
		std::vector<int> uv_index = T_current->pmesh->property(vprop_vertex_uv_index,v_iter.handle());
		if(uv_index[0] == p0_index[0] && uv_index[1] == p0_index[1])
		{
			vh1 = v_iter.handle();
			findflag1 = TRUE;
			break;
		}
	}
	if(!findflag1)
	{
		vh1 = T_current->pmesh->add_vertex(TsplineMesh::Point(0, 0, 0));

		T_current->pmesh->property(vprop_vertex_uv,vh1) = p0;
		T_current->pmesh->property(vprop_vertex_uv_index,vh1) = p0_index;
	}

	for( TsplineMesh::VertexIter v_iter = T_current->pmesh->vertices_begin(); v_iter!=T_current->pmesh->vertices_end(); ++v_iter ) 
	{
		//skip vh1
		if(v_iter.handle() == vh1)
			continue;

		//Vector2D uv = T_current->pmesh->property(vprop_vertex_uv,v_iter.handle());
		std::vector<int> uv_index = T_current->pmesh->property(vprop_vertex_uv_index,v_iter.handle());
		if(uv_index[0] == p1_index[0] && uv_index[1] == p1_index[1])
		{
			vh2 = v_iter.handle();
			findflag2 = TRUE;
			break;
		}
	}
	if(!findflag2)
	{
		vh2 = T_current->pmesh->add_vertex(TsplineMesh::Point(0, 0, 0));

		T_current->pmesh->property(vprop_vertex_uv,vh2) = p1;
		T_current->pmesh->property(vprop_vertex_uv_index,vh2) = p1_index;
	}

	SplitHorizontal_Topology(T_current,handle,index,index,umin_index,umax_index,vmin_index,vmax_index,vh1,vh2,findflag1,findflag2);
}
void Actions_Tspline::SplitVertical(Tspline* T_current,TsplineMesh::FaceHandle handle,int uknot_num_inface,int vknot_num_inface,int umin_index,int umax_index,double umin,double umax,double vmin,double vmax,int vmin_index,int vmax_index)
{
	FPROP_FACE_UK_BOUNDING_BOX fprop_face_bounding_box;
	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index;

	//Vertex UV Coordinate
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE))
		T_current->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE);

	//Face bounding box
	if (!T_current->pmesh->get_property_handle(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX))
		T_current->pmesh->add_property(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX);

	//Vertex UV Coordinate Index
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_current->pmesh->add_property(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	//Subdivide the face vertically
	int index = (umin_index + umax_index) / 2;
	double u0 = T_current->m_targetNurbsUKnots[index];
	double v0 = vmin;
	Vector2D p0(u0,v0);
	std::vector<int> p0_index;
	p0_index.push_back(index);
	p0_index.push_back(vmin_index);

	double u1 = T_current->m_targetNurbsUKnots[index];
	double v1 = vmax;
	Vector2D p1(u1,v1);
	std::vector<int> p1_index;
	p1_index.push_back(index);
	p1_index.push_back(vmax_index);

	TsplineMesh::VertexHandle vh1,vh2;
	BOOL findflag1 = FALSE;
	BOOL findflag2 = FALSE;

	//search for the two end points
	for( TsplineMesh::VertexIter v_iter = T_current->pmesh->vertices_begin(); v_iter!=T_current->pmesh->vertices_end(); ++v_iter ) 
	{
		//Vector2D uv = T_current->pmesh->property(vprop_vertex_uv,v_iter.handle());
		std::vector<int> uv_index = T_current->pmesh->property(vprop_vertex_uv_index,v_iter.handle());
		if(uv_index[0] == p0_index[0] && uv_index[1] == p0_index[1])
		{
			vh1 = v_iter.handle();
			findflag1 = TRUE;
			break;
		}
	}
	if(!findflag1)
	{
		vh1 = T_current->pmesh->add_vertex(TsplineMesh::Point(0, 0, 0));
		T_current->pmesh->property(vprop_vertex_uv,vh1) = p0;
		T_current->pmesh->property(vprop_vertex_uv_index,vh1) = p0_index;
	}

	for( TsplineMesh::VertexIter v_iter = T_current->pmesh->vertices_begin(); v_iter!=T_current->pmesh->vertices_end(); ++v_iter ) 
	{
		//skip vh1
		if(v_iter.handle() == vh1)
			continue;

		//Vector2D uv = T_current->pmesh->property(vprop_vertex_uv,v_iter.handle());
		std::vector<int> uv_index = T_current->pmesh->property(vprop_vertex_uv_index,v_iter.handle());
		if(uv_index[0] == p1_index[0] && uv_index[1] == p1_index[1])
		{
			vh2 = v_iter.handle();
			findflag2 = TRUE;
			break;
		}
	}
	if(!findflag2)
	{
		vh2 = T_current->pmesh->add_vertex(TsplineMesh::Point(0, 0, 0));
		T_current->pmesh->property(vprop_vertex_uv,vh2) = p1;
		T_current->pmesh->property(vprop_vertex_uv_index,vh2) = p1_index;
	}

	SplitVertical_Topology(T_current,handle,index,index,umin_index,umax_index,vmin_index,vmax_index,vh1,vh2,findflag1,findflag2);
}
void Actions_Tspline::CollectandSplitOppositeFace_Vertical(Tspline* T_current,TsplineMesh::VertexHandle fromvh,TsplineMesh::VertexHandle tovh,TsplineMesh::VertexHandle insertvh,TsplineMesh::FaceHandle fhandle)
{
	MyMesh::FaceHandle fh_opposite_handle,fh2_handle;
	std::vector<MyMesh::VertexHandle>  face_vhandles;
	//the halfedge should be fromvh -> tovh
	//then insert one vertex handle in the edge
	//find the adjacent face handle with the same edge vhandles[ii] - vhandles[(ii + 1)%vhandles.size()]

	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv;
	FPROP_FACE_OFFENDING_FLAG fprop_face_offending;
	VPROP_VERTEX_UV_PLOTCOORDINATE vprop_vertex_plotuv;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_index;

	//Vertex UV Coordinate
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE))
		T_current->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE);

	//Vertex UV Index
	if (!T_current->pmesh->get_property_handle(vprop_vertex_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_current->pmesh->add_property(vprop_vertex_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	//Face offending flag
	if (!T_current->pmesh->get_property_handle(fprop_face_offending, PROP_FACE_OFFENDING_FLAG))
		T_current->pmesh->add_property(fprop_face_offending, PROP_FACE_OFFENDING_FLAG);

	//Vertex Plot UV
	if (!T_current->pmesh->get_property_handle(vprop_vertex_plotuv, PROP_VERTEX_UV_PLOTCOORDINATE))
		T_current->pmesh->add_property(vprop_vertex_plotuv, PROP_VERTEX_UV_PLOTCOORDINATE);

	//set vertex plot coordinate
	Vector2D fromuv_coordinate = T_current->pmesh->property(vprop_vertex_plotuv,fromvh);
	Vector2D touv_coordinate = T_current->pmesh->property(vprop_vertex_plotuv,tovh);
	Vector2D insertuv_coordinate = (fromuv_coordinate + touv_coordinate) / 2;
	T_current->pmesh->property(vprop_vertex_plotuv,insertvh) = insertuv_coordinate;

	int minv_index = 1e10,maxv_index = -1e10;
	for(MyMesh::VertexOHalfedgeIter voh_it = T_current->pmesh->voh_iter(tovh); voh_it; ++voh_it) 
	{
		// Iterate over all outgoing halfedges...
		MyMesh::FaceHandle fh = T_current->pmesh->face_handle(voh_it.handle());
		if(fh == fhandle)
		{
			fh_opposite_handle = T_current->pmesh->opposite_face_handle(voh_it.handle());

			//boundary face
			if(!T_current->pmesh->is_valid_handle(fh_opposite_handle))
				return;

			//add the vertex to  the opposite face 
			for (MyMesh::FaceVertexIter fv_it = T_current->pmesh->fv_iter(fh_opposite_handle); fv_it; ++fv_it)  
			{  
				MyMesh::VertexHandle vh = fv_it.handle();
				std::vector<int> uv_index = T_current->pmesh->property(vprop_vertex_index,vh);
				if(uv_index[1] < minv_index)
					minv_index = uv_index[1];
				if(uv_index[1] > maxv_index)
					maxv_index = uv_index[1];
				face_vhandles.push_back(fv_it.handle());
				if(fv_it.handle() == fromvh)
				{
					face_vhandles.push_back(insertvh);
				}
			}

			//Rule 2: 
			//Judge whether to split the opposite face
			//first judge the edge is split horizontally or vertically
			std::vector<int> insertuv_index = T_current->pmesh->property(vprop_vertex_index,insertvh);
			std::vector<int> newuv_index = insertuv_index;
			if(newuv_index[1] == minv_index)
				newuv_index[1] = maxv_index;
			else
				newuv_index[1] = minv_index;
			
			//Search whether newuv exists in the mesh.
			for (MyMesh::FaceVertexIter fv_it = T_current->pmesh->fv_iter(fh_opposite_handle); fv_it; ++fv_it)  
			{  
				Vector2D uv_index = T_current->pmesh->property(vprop_vertex_uv,fv_it.handle());
				MyMesh::VertexHandle newvh;	
				if(uv_index[0] == newuv_index[0] && uv_index[1] == newuv_index[1])
				{
					//Split the face between insertuv and newuv
					std::vector<MyMesh::VertexHandle>  face_vhandles1,face_vhandles2;
					newvh = fv_it.handle();

					int newvhindex,insertvhindex;
					for(int ii = 0; ii < face_vhandles.size(); ii++)
					{
						if(face_vhandles[ii] == insertvh)
							insertvhindex = ii;
						else
						if(face_vhandles[ii] == newvh)
							newvhindex = ii;
					}

					for(int ii = insertvhindex; ii != newvhindex; ii = (ii + 1) % face_vhandles.size())
					{
						face_vhandles1.push_back(face_vhandles[ii]);
					}
					face_vhandles1.push_back(face_vhandles[newvhindex]);
					for(int ii = newvhindex; ii != insertvhindex; ii = (ii + 1) % face_vhandles.size())
					{
						face_vhandles2.push_back(face_vhandles[ii]);
					}
					face_vhandles2.push_back(face_vhandles[insertvhindex]);

					T_current->pmesh->delete_face(fh_opposite_handle,FALSE);
					MyMesh::FaceHandle newvf1 = T_current->pmesh->add_face(face_vhandles1);
					MyMesh::FaceHandle newvf2 = T_current->pmesh->add_face(face_vhandles2);

					//T_current->pmesh->garbage_collection();
					return;
				}
			}

			//add the new face
			int flag = T_current->pmesh->property(fprop_face_offending,fh_opposite_handle);
			T_current->pmesh->delete_face(fh_opposite_handle,FALSE);
			MyMesh::FaceHandle newvf1 = T_current->pmesh->add_face(face_vhandles);
			T_current->pmesh->property(fprop_face_offending,newvf1) = 0;
			
			//int faces1 = T_current->pmesh->n_faces();
			//T_current->pmesh->garbage_collection();
			//int faces2 = T_current->pmesh->n_faces();

			break;
		}
	}
}
void Actions_Tspline::CollectandSplitOppositeFace_Horizontal(Tspline* T_current,TsplineMesh::VertexHandle fromvh,TsplineMesh::VertexHandle tovh,TsplineMesh::VertexHandle insertvh,TsplineMesh::FaceHandle fhandle)
{
	MyMesh::FaceHandle fh_opposite_handle,fh2_handle;
	std::vector<MyMesh::VertexHandle>  face_vhandles;
	//the halfedge should be fromvh -> tovh
	//then insert one vertex handle in the edge
	//find the adjacent face handle with the same edge vhandles[ii] - vhandles[(ii + 1)%vhandles.size()]

	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv;
	FPROP_FACE_OFFENDING_FLAG fprop_face_offending;
	VPROP_VERTEX_UV_PLOTCOORDINATE vprop_vertex_plotuv;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index;

	//Vertex UV Coordinate
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE))
		T_current->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE);

	//Vertex UV Coordinate Index
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_current->pmesh->add_property(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	//Face offending flag
	if (!T_current->pmesh->get_property_handle(fprop_face_offending, PROP_FACE_OFFENDING_FLAG))
		T_current->pmesh->add_property(fprop_face_offending, PROP_FACE_OFFENDING_FLAG);

	//Vertex Plot UV
	if (!T_current->pmesh->get_property_handle(vprop_vertex_plotuv, PROP_VERTEX_UV_PLOTCOORDINATE))
		T_current->pmesh->add_property(vprop_vertex_plotuv, PROP_VERTEX_UV_PLOTCOORDINATE);

	//set vertex plot coordinate
	Vector2D fromuv_coordinate = T_current->pmesh->property(vprop_vertex_plotuv,fromvh);
	Vector2D touv_coordinate = T_current->pmesh->property(vprop_vertex_plotuv,tovh);
	Vector2D insertuv_coordinate = (fromuv_coordinate + touv_coordinate) / 2;
	T_current->pmesh->property(vprop_vertex_plotuv,insertvh) = insertuv_coordinate;

	int minu_index = 1e10,maxu_index = -1e10;
	for(MyMesh::VertexOHalfedgeIter voh_it = T_current->pmesh->voh_iter(tovh); voh_it; ++voh_it) 
	{
		// Iterate over all outgoing halfedges...
		MyMesh::FaceHandle fh = T_current->pmesh->face_handle(voh_it.handle());
		if(fh == fhandle)
		{
			fh_opposite_handle = T_current->pmesh->opposite_face_handle(voh_it.handle());

			//boundary face
			if(!T_current->pmesh->is_valid_handle(fh_opposite_handle))
				return;

			//add the vertex to  the opposite face 
			for (MyMesh::FaceVertexIter fv_it = T_current->pmesh->fv_iter(fh_opposite_handle); fv_it; ++fv_it)  
			{  
				MyMesh::VertexHandle vh = fv_it.handle();
				std::vector<int> uv_index = T_current->pmesh->property(vprop_vertex_uv_index,vh);
				if(uv_index[0] < minu_index)
					minu_index = uv_index[0];
				if(uv_index[0] > maxu_index)
					maxu_index = uv_index[0];
				face_vhandles.push_back(fv_it.handle());
				if(fv_it.handle() == fromvh)
				{
					face_vhandles.push_back(insertvh);
				}
			}

			//Rule 2: 
			//Judge whether to split the opposite face
			//first judge the edge is split horizontally or vertically
			std::vector<int> insertuv_index = T_current->pmesh->property(vprop_vertex_uv_index,insertvh);
			std::vector<int> newuv_index = insertuv_index;
			if(newuv_index[0] == minu_index)
				newuv_index[0] = maxu_index;
			else
				newuv_index[0] = minu_index;

			//Search whether newuv exists in the mesh.
			for (MyMesh::FaceVertexIter fv_it = T_current->pmesh->fv_iter(fh_opposite_handle); fv_it; ++fv_it)  
			{  
				//Vector2D uv = T_current->pmesh->property(vprop_vertex_uv,fv_it.handle());
				std::vector<int> uv_index = T_current->pmesh->property(vprop_vertex_uv_index,fv_it.handle());
				MyMesh::VertexHandle newvh;	
				if(uv_index[0] == newuv_index[0] && uv_index[1] == newuv_index[1])
				{
					//Split the face between insertuv and newuv
					std::vector<MyMesh::VertexHandle>  face_vhandles1,face_vhandles2;
					newvh = fv_it.handle();

					int newvhindex,insertvhindex;
					for(int ii = 0; ii < face_vhandles.size(); ii++)
					{
						if(face_vhandles[ii] == insertvh)
							insertvhindex = ii;
						else
							if(face_vhandles[ii] == newvh)
								newvhindex = ii;
					}

					for(int ii = insertvhindex; ii != newvhindex; ii = (ii + 1) % face_vhandles.size())
					{
						face_vhandles1.push_back(face_vhandles[ii]);
					}
					face_vhandles1.push_back(face_vhandles[newvhindex]);
					for(int ii = newvhindex; ii != insertvhindex; ii = (ii + 1) % face_vhandles.size())
					{
						face_vhandles2.push_back(face_vhandles[ii]);
					}
					face_vhandles2.push_back(face_vhandles[insertvhindex]);

					T_current->pmesh->delete_face(fh_opposite_handle,FALSE);
					MyMesh::FaceHandle newvf1 = T_current->pmesh->add_face(face_vhandles1);
					MyMesh::FaceHandle newvf2 = T_current->pmesh->add_face(face_vhandles2);

					//T_current->pmesh->garbage_collection();
					return;
				}
			}

			//add the new face

			int flag = T_current->pmesh->property(fprop_face_offending,fh_opposite_handle);
			T_current->pmesh->delete_face(fh_opposite_handle,FALSE);
			MyMesh::FaceHandle newvf1 = T_current->pmesh->add_face(face_vhandles);
			T_current->pmesh->property(fprop_face_offending,newvf1) = 0;
			//int faces1 = T_current->pmesh->n_faces();
			//T_current->pmesh->garbage_collection();
			//int faces2 = T_current->pmesh->n_faces();
			break;
		}
	}
}
void Actions_Tspline::SplitHorizontal_Topology(Tspline* T_current,TsplineMesh::FaceHandle handle,int v0_index,int v1_index,int umin_index,int umax_index,int vmin_index,int vmax_index,TsplineMesh::VertexHandle vh1,TsplineMesh::VertexHandle vh2,BOOL findflag1,BOOL findflag2)
{
	FPROP_FACE_UK_BOUNDING_BOX fprop_face_bounding_box;
	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index;

	//Vertex UV Coordinate
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE))
		T_current->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE);

	//Vertex UV Coordinate Index
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_current->pmesh->add_property(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	//Face bounding box
	if (!T_current->pmesh->get_property_handle(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX))
		T_current->pmesh->add_property(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX);

	// 
	std::vector<TsplineMesh::VertexHandle> vhandles;
	for(TsplineMesh::FaceVertexIter fv_it = T_current->pmesh->fv_iter(handle); fv_it; ++fv_it )
	{
		vhandles.push_back(fv_it.handle());
	}

	//T_current->pmesh->split();

	//add the vh1 and vh2 into the array
	//vh1 vmin
	std::vector<TsplineMesh::VertexHandle>::iterator it = vhandles.begin(); 
	int vh1_index,vh2_index;
	BOOL vh1flag = FALSE;
	BOOL vh2flag = FALSE;
	MyMesh::FaceHandle fh1_handle,fh2_handle;
	for(int ii = 0; ii < vhandles.size(); ii++)
	{
		if(vh1flag && vh2flag)
			break;

		TsplineMesh::VertexHandle fromvh = vhandles[ii];
		TsplineMesh::VertexHandle tovh = vhandles[(ii + 1)%vhandles.size()];

		//Vector2D uv_before = T_current->pmesh->property(vprop_vertex_uv,fromvh);
		//Vector2D uv_after = T_current->pmesh->property(vprop_vertex_uv,tovh);

		std::vector<int> uv_before_index = T_current->pmesh->property(vprop_vertex_uv_index,fromvh);
		std::vector<int> uv_after_index = T_current->pmesh->property(vprop_vertex_uv_index,tovh);

		if(!vh1flag && uv_before_index[0] == umin_index && uv_after_index[0] == umin_index &&
			( v0_index > uv_before_index[1] && v0_index < uv_after_index[1] || v0_index < uv_before_index[1] && v0_index > uv_after_index[1]))
		{
			vh1flag = TRUE;
			int index = (ii + 1)%vhandles.size();
			vhandles.insert(vhandles.begin() + index,vh1);

			//deal with the opposite face
			if(!findflag1)
				CollectandSplitOppositeFace_Horizontal(T_current,tovh,fromvh,vh1,handle);

			//vh1_index = index;
			ii++;
			continue;
		}
		if(!vh2flag && uv_before_index[0] == umax_index && uv_after_index[0] == umax_index &&
			(v1_index < uv_before_index[1] && v1_index > uv_after_index[1] || v1_index > uv_before_index[1] && v1_index < uv_after_index[1]) )
		{
			vh2flag = TRUE;
			int index = (ii + 1)%vhandles.size();
			vhandles.insert(vhandles.begin() + index,vh2);

			//deal with the opposite face
			//find the adjacent face handle with the same edge vhandles[ii] - vhandles[(ii + 1)%vhandles.size()]
			if(!findflag2)
				CollectandSplitOppositeFace_Horizontal(T_current,tovh,fromvh,vh2,handle);

			//vh2_index = index;
			ii++;
			continue;
		}
	}

	for(int ii = 0; ii < vhandles.size(); ii++)
	{
		if(vhandles[ii] == vh1)
			vh1_index = ii;
		else
			if(vhandles[ii] == vh2)
				vh2_index = ii;
	}
	//first delete the original face
	T_current->pmesh->delete_face(handle,FALSE);

	//then add two faces
	std::vector<TsplineMesh::VertexHandle> f1vhandles;
	for(int ii = vh2_index; (ii) % vhandles.size() != vh1_index; ii++)
	{
		f1vhandles.push_back(vhandles[(ii) % vhandles.size()]);
	}
	f1vhandles.push_back(vhandles[vh1_index]);
	T_current->pmesh->add_face(f1vhandles);
	std::vector<TsplineMesh::VertexHandle> f2vhandles;
	for(int ii = vh1_index; (ii) % vhandles.size() != vh2_index; ii++)
	{
		f2vhandles.push_back(vhandles[(ii) % vhandles.size()]);
	}
	f2vhandles.push_back(vhandles[vh2_index]);
	T_current->pmesh->add_face(f2vhandles);

	//T_current->pmesh->garbage_collection();  
}
void Actions_Tspline::SplitVertical_Topology(Tspline* T_current,TsplineMesh::FaceHandle handle,int u0_index,int u1_index,int umin_index,int umax_index,int vmin_index,int vmax_index,TsplineMesh::VertexHandle vh1,TsplineMesh::VertexHandle vh2,BOOL findflag1,BOOL findflag2)
{
	FPROP_FACE_UK_BOUNDING_BOX fprop_face_bounding_box;
	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index;

	//Vertex UV Coordinate
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE))
		T_current->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE);

	//Vertex UV Coordinate Index
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		T_current->pmesh->add_property(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	//Face bounding box
	if (!T_current->pmesh->get_property_handle(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX))
		T_current->pmesh->add_property(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX);

	// 
	std::vector<TsplineMesh::VertexHandle> vhandles;
	for(TsplineMesh::FaceVertexIter fv_it = T_current->pmesh->fv_iter(handle); fv_it; ++fv_it )
	{
		vhandles.push_back(fv_it.handle());
	}

	//T_current->pmesh->split();

	//add the vh1 and vh2 into the array
	//vh1 vmin
	std::vector<TsplineMesh::VertexHandle>::iterator it = vhandles.begin(); 
	int vh1_index,vh2_index;
	MyMesh::FaceHandle fh1_handle,fh2_handle;
	BOOL vh1flag = FALSE;
	BOOL vh2flag = FALSE;
	for(int ii = 0; ii < vhandles.size(); ii++)
	{
		if(vh1flag && vh2flag)
			break;

		TsplineMesh::VertexHandle fromvh = vhandles[ii];
		TsplineMesh::VertexHandle tovh = vhandles[(ii + 1)%vhandles.size()];

		//Vector2D uv_before = T_current->pmesh->property(vprop_vertex_uv,fromvh);
		//Vector2D uv_after = T_current->pmesh->property(vprop_vertex_uv,tovh);

		std::vector<int> uv_before_index = T_current->pmesh->property(vprop_vertex_uv_index,fromvh);
		std::vector<int> uv_after_index = T_current->pmesh->property(vprop_vertex_uv_index,tovh);

		if(!vh1flag && uv_before_index[1] == vmin_index && uv_after_index[1] == vmin_index &&
			(u0_index > uv_before_index[0] && u0_index < uv_after_index[0] || u0_index < uv_before_index[0] && u0_index > uv_after_index[0]))
		{
			vh1flag = TRUE;
			int index = (ii + 1)%vhandles.size();
			vhandles.insert(vhandles.begin() + index,vh1);
			
			//deal with the opposite face
			if(!findflag1)
				CollectandSplitOppositeFace_Vertical(T_current,tovh,fromvh,vh1,handle);

			//vh1_index = index;
			ii++;
			continue;
		}
		if(!vh2flag && uv_before_index[1] == vmax_index && uv_after_index[1] == vmax_index &&
			(u1_index < uv_before_index[0] && u1_index > uv_after_index[0] || u1_index > uv_before_index[0] && u1_index < uv_after_index[0]) )
		{
			vh2flag = TRUE;
			int index = (ii + 1)%vhandles.size();
			vhandles.insert(vhandles.begin() + index,vh2);
			
			//deal with the opposite face
			//find the adjacent face handle with the same edge vhandles[ii] - vhandles[(ii + 1)%vhandles.size()]
			if(!findflag2)
				CollectandSplitOppositeFace_Vertical(T_current,tovh,fromvh,vh2,handle);

			//vh2_index = index;
			ii++;
			continue;
		}
	}

	for(int ii = 0; ii < vhandles.size(); ii++)
	{
		if(vhandles[ii] == vh1)
			vh1_index = ii;
		else
		if(vhandles[ii] == vh2)
			vh2_index = ii;
	}
	//first delete the original face
	T_current->pmesh->delete_face(handle,FALSE);

	//then add two faces
	std::vector<TsplineMesh::VertexHandle> f1vhandles;
	for(int ii = vh2_index; (ii) % vhandles.size() != vh1_index; ii++)
	{
		f1vhandles.push_back(vhandles[(ii) % vhandles.size()]);
	}
	f1vhandles.push_back(vhandles[vh1_index]);
	T_current->pmesh->add_face(f1vhandles);
	std::vector<TsplineMesh::VertexHandle> f2vhandles;
	for(int ii = vh1_index; (ii) % vhandles.size() != vh2_index; ii++)
	{
		f2vhandles.push_back(vhandles[(ii) % vhandles.size()]);
	}
	f2vhandles.push_back(vhandles[vh2_index]);
	T_current->pmesh->add_face(f2vhandles);

	//T_current->pmesh->garbage_collection();  
}

/*void Actions_Tspline::SplitVertical_BothExistingCase(Tspline* T_current,TsplineMesh::FaceHandle handle,double u0,double u1,double umin,double umax,double vmin,double vmax,TsplineMesh::VertexHandle vh1,TsplineMesh::VertexHandle vh2)
{
	FPROP_FACE_UK_BOUNDING_BOX fprop_face_bounding_box;
	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv;

	//Vertex UV Coordinate
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE))
		T_current->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE);

	//Face bounding box
	if (!T_current->pmesh->get_property_handle(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX))
		T_current->pmesh->add_property(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX);

	//find both the two points
	std::vector<TsplineMesh::VertexHandle> vhandles;
	for(TsplineMesh::FaceVertexIter fv_it = T_current->pmesh->fv_iter(handle); fv_it; ++fv_it )
	{
		vhandles.push_back(fv_it.handle());
	}
	//first delete the original face
	T_current->pmesh->delete_face(handle,FALSE);

	//add the vh1 and vh2 into the array
	//vh1 vmin
	int vh1_index,vh2_index;
	for(int ii = 0; ii < vhandles.size(); ii++)
	{
		Vector2D uv_before = T_current->pmesh->property(vprop_vertex_uv,vhandles[ii]);
		Vector2D uv_after = T_current->pmesh->property(vprop_vertex_uv,vhandles[(ii + 1)%vhandles.size()]);

		std::vector<TsplineMesh::VertexHandle>::iterator it;
		if(fabs(uv_before[1] - vmin) < 1e-8 && fabs(uv_after[1] - vmin) < 1e-8 &&
			u0 > uv_before[1] && u0 < uv_after[1] )
		{
			vhandles.insert(it + (ii + 1)%vhandles.size(),vh1);
			vh1_index = (ii + 1)%(vhandles.size() + 1);
		}
		if(fabs(uv_before[1] - vmax) < 1e-8 && fabs(uv_after[1] - vmax) < 1e-8 &&
			u1 < uv_before[1] && u1 > uv_after[1] )
		{
			vhandles.insert(it + (ii + 1)%vhandles.size(),vh2);
			vh2_index = (ii + 1)%(vhandles.size() + 1);
		}
	}

	//then add two faces
	std::vector<TsplineMesh::VertexHandle> f1vhandles;
	for(int ii = vh2_index; (ii) % vhandles.size() != vh1_index; ii++)
	{
		f1vhandles.push_back(vhandles[ii]);
	}
	T_current->pmesh->add_face(f1vhandles);
	std::vector<TsplineMesh::VertexHandle> f2vhandles;
	for(int ii = vh1_index; (ii) % vhandles.size() != vh2_index; ii++)
	{
		f2vhandles.push_back(vhandles[ii]);
	}
	T_current->pmesh->add_face(f2vhandles);
	T_current->pmesh->garbage_collection();  
}*/
void Actions_Tspline::SetOffendingFaceFlag(Tspline* T_target,Tspline* T_current)
{
	//for each offending vertex, search its located faces.

	VPROP_VERTEX_OFFENDING_FLAG vprop_vertex_offending;
	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv_target;
	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv_current;
	FPROP_FACE_UK_BOUNDING_BOX fprop_face_bounding_box;
	VPROP_VERTEX_TARGET_IN_CURRENT_INDEX vprop_target_in_current_index;
	VPROP_VERTEX_TARGET_IN_CURRENT_FLAG vprop_target_in_current_flag;
	FPROP_FACE_OFFENDING_FLAG fprop_face_offending_flag;
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag_target,vprop_IMGFlag_current;

	//target vertex imagination flag
	if (!T_target->pmesh->get_property_handle(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG))
		T_target->pmesh->add_property(vprop_IMGFlag_target, PROP_VERTEX_IMG_FLAG);

	//current vertex imagination flag
	if (!T_current->pmesh->get_property_handle(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG))
		T_current->pmesh->add_property(vprop_IMGFlag_current, PROP_VERTEX_IMG_FLAG);

	//Vertex offending
	if (!T_target->pmesh->get_property_handle(vprop_vertex_offending, PROP_VERTEX_OFFENDING_FLAG))
		T_target->pmesh->add_property(vprop_vertex_offending, PROP_VERTEX_OFFENDING_FLAG);

	//Target Vertex uv
	if (!T_target->pmesh->get_property_handle(vprop_vertex_uv_target, PROP_VERTEX_UV_COORDINATE))
		T_target->pmesh->add_property(vprop_vertex_uv_target, PROP_VERTEX_UV_COORDINATE);

	//Current Vertex uv
	if (!T_current->pmesh->get_property_handle(vprop_vertex_uv_current, PROP_VERTEX_UV_COORDINATE))
		T_current->pmesh->add_property(vprop_vertex_uv_current, PROP_VERTEX_UV_COORDINATE);

	//index
	if (!T_target->pmesh->get_property_handle(vprop_target_in_current_index, PROP_VERTEX_TARGET_IN_CURRENT_INDEX))
		T_target->pmesh->add_property(vprop_target_in_current_index, PROP_VERTEX_TARGET_IN_CURRENT_INDEX);

	//flag
	if (!T_target->pmesh->get_property_handle(vprop_target_in_current_flag, PROP_VERTEX_TARGET_IN_CURRENT_FLAG))
		T_target->pmesh->add_property(vprop_target_in_current_flag, PROP_VERTEX_TARGET_IN_CURRENT_FLAG);

	//Face bounding box
	if (!T_current->pmesh->get_property_handle(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX))
		T_current->pmesh->add_property(fprop_face_bounding_box, PROP_FACE_UV_BOUNDINGBOX);

	//Face Offending
	if (!T_current->pmesh->get_property_handle(fprop_face_offending_flag, PROP_FACE_OFFENDING_FLAG))
		T_current->pmesh->add_property(fprop_face_offending_flag, PROP_FACE_OFFENDING_FLAG);

	//Set Face bounding box
	double umin, vmin;
	double umax, vmax;
	std::vector<double> boundingbox;
	int v_imagination_flag;
	for( TsplineMesh::FaceIter f_iter = T_current->pmesh->faces_begin(); f_iter!=T_current->pmesh->faces_end(); ++f_iter ) 
	{
		umin = 1e10;
		vmin = 1e10;
		umax = -1e10;
		vmax = -1e10;
		boundingbox.clear();
		for(TsplineMesh::FaceVertexIter fv_it = T_current->pmesh->fv_iter(f_iter); fv_it; ++fv_it )
		{
			v_imagination_flag = T_current->pmesh->property(vprop_IMGFlag_current,fv_it.handle());
			if(v_imagination_flag)
				continue;

			Vector2D uv = T_current->pmesh->property(vprop_vertex_uv_current,fv_it.handle());
			if(uv[0] < umin)
				umin = uv[0];
			if(uv[0] > umax)
				umax = uv[0];
			if(uv[1] < vmin)
				vmin = uv[1];
			if(uv[1] > vmax)
				vmax = uv[1];
		}
		boundingbox.push_back(umin);
		boundingbox.push_back(umax);
		boundingbox.push_back(vmin);
		boundingbox.push_back(vmax);
		T_current->pmesh->property(fprop_face_bounding_box,f_iter.handle()) = boundingbox;
	}

	//Set the target vertex index and flag 
	int flag;
	int index;
	for( TsplineMesh::VertexIter v_iter = T_target->pmesh->vertices_begin(); v_iter!=T_target->pmesh->vertices_end(); ++v_iter ) 
	{
		v_imagination_flag = T_target->pmesh->property(vprop_IMGFlag_target,v_iter.handle());
		if(v_imagination_flag)
			continue;

		int flag1 = T_target->pmesh->property(vprop_vertex_offending,v_iter.handle());
		if(!flag1)
			continue;

		Vector2D uv = T_target->pmesh->property(vprop_vertex_uv_target,v_iter.handle());
		flag = -1;

		//search the vertex
		for( TsplineMesh::VertexIter v_iter1 = T_current->pmesh->vertices_begin(); v_iter1!=T_current->pmesh->vertices_end(); ++v_iter1 ) 
		{
			v_imagination_flag = T_current->pmesh->property(vprop_IMGFlag_current,v_iter1.handle());
			if(v_imagination_flag)
				continue;

			Vector2D uv1 = T_current->pmesh->property(vprop_vertex_uv_current,v_iter1.handle());
			if(Length(uv - uv1) < 1e-8)
			{
				flag = 0;
				index = v_iter1.handle().idx();
				break;
			}
		}

		//search the edge
		if(flag == -1)
		{
			for( TsplineMesh::EdgeIter e_iter = T_current->pmesh->edges_begin(); e_iter!=T_current->pmesh->edges_end(); ++e_iter ) 
			{
				TsplineMesh::EdgeHandle eh = e_iter.handle();
				TsplineMesh::HalfedgeHandle hh = T_current->pmesh->halfedge_handle(eh, 0);

				TsplineMesh::VertexHandle vh1 = T_current->pmesh->to_vertex_handle(hh);
				TsplineMesh::VertexHandle vh2 = T_current->pmesh->from_vertex_handle(hh);

				Vector2D uv1 = T_current->pmesh->property(vprop_vertex_uv_current,vh1);
				Vector2D uv2 = T_current->pmesh->property(vprop_vertex_uv_current,vh2);

				if(fabs(Length(uv - uv1) + Length(uv - uv2) - Length(uv1 - uv2)) < 1e-8)
				{
					//on the edge
					flag = 1;
					index = e_iter.handle().idx();
					break;
				}
			}
		}

		//search the face
		if(flag == -1)
		{
			for( TsplineMesh::FaceIter f_iter = T_current->pmesh->faces_begin(); f_iter!=T_current->pmesh->faces_end(); ++f_iter ) 
			{
				std::vector<double> boundingbox = T_current->pmesh->property(fprop_face_bounding_box,f_iter.handle());
				if(uv[0] > boundingbox[0] && uv[0] < boundingbox[1] && uv[1] > boundingbox[2] && uv[1] < boundingbox[3])
				{
					flag = 2;
					index = f_iter.handle().idx();
					break;
				}
			}
		}
		if(flag == -1)
		{
			ASSERT(0);
		}
		T_target->pmesh->property(vprop_target_in_current_index,v_iter.handle()) = index;
		T_target->pmesh->property(vprop_target_in_current_flag,v_iter.handle()) = flag;
	}

	//Initialization set the face offending flag as zero.
	for( TsplineMesh::FaceIter f_iter = T_current->pmesh->faces_begin(); f_iter!=T_current->pmesh->faces_end(); ++f_iter ) 
	{
		T_current->pmesh->property(fprop_face_offending_flag,f_iter.handle()) = 0;
	}

	//Set the offending face flag
	for( TsplineMesh::VertexIter v_iter = T_target->pmesh->vertices_begin(); v_iter!=T_target->pmesh->vertices_end(); ++v_iter ) 
	{
		v_imagination_flag = T_target->pmesh->property(vprop_IMGFlag_target,v_iter.handle());
		if(v_imagination_flag)
			continue;

		int flag1 = T_target->pmesh->property(vprop_vertex_offending,v_iter.handle());
		if(!flag1)
			continue;

		index = T_target->pmesh->property(vprop_target_in_current_index,v_iter.handle());
		flag = T_target->pmesh->property(vprop_target_in_current_flag,v_iter.handle());

		if(flag == 0)
		{
			TsplineMesh::VertexHandle vh = T_current->pmesh->vertex_handle(index);

			//set the surrounding face
			for(TsplineMesh::VertexFaceIter vf_it = T_current->pmesh->vf_iter(vh); vf_it; ++vf_it )
			{
				SetFaceOffendingFlag(T_current,vf_it.handle());
			}
		}else
		if(flag == 1)
		{	
			
			TsplineMesh::EdgeHandle eh = T_current->pmesh->edge_handle(index);

			//set the two adjacent faces
			TsplineMesh::HalfedgeHandle hh1 = T_current->pmesh->halfedge_handle(eh, 0);
			TsplineMesh::HalfedgeHandle hh2 = T_current->pmesh->halfedge_handle(eh, 1);

			TsplineMesh::FaceHandle fh1 = T_current->pmesh->face_handle(hh1);
			TsplineMesh::FaceHandle fh2 = T_current->pmesh->face_handle(hh2);

			SetFaceOffendingFlag(T_current,fh1);
			SetFaceOffendingFlag(T_current,fh2);
			
		}else
		if(flag == 2)
		{
			TsplineMesh::FaceHandle fh = T_current->pmesh->face_handle(index);
			SetFaceOffendingFlag(T_current,fh);
		}
	}
}
BOOL Actions_Tspline::SetFaceOffendingFlag(Tspline* T_mesh,TsplineMesh::FaceHandle fh)
{
	VPROP_VERTEX_IMG_FLAG vprop_IMGFlag;
	FPROP_FACE_OFFENDING_FLAG fprop_face_offending_flag;

	//target vertex imagination flag
	if (!T_mesh->pmesh->get_property_handle(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG))
		T_mesh->pmesh->add_property(vprop_IMGFlag, PROP_VERTEX_IMG_FLAG);

	//Face Offending
	if (!T_mesh->pmesh->get_property_handle(fprop_face_offending_flag, PROP_FACE_OFFENDING_FLAG))
		T_mesh->pmesh->add_property(fprop_face_offending_flag, PROP_FACE_OFFENDING_FLAG);

	if(!T_mesh->pmesh->is_valid_handle(fh))
		return FALSE;

	int v_imagination_flag;
	for(TsplineMesh::FaceVertexIter fv_it = T_mesh->pmesh->fv_iter(fh); fv_it; ++fv_it )
	{
		v_imagination_flag = T_mesh->pmesh->property(vprop_IMGFlag,fv_it.handle());
		if(v_imagination_flag)
		{
			return FALSE;
		}
	}
	T_mesh->pmesh->property(fprop_face_offending_flag,fh) = 1;
	return TRUE;
}
// Tspline simplification
void Actions_Tspline::OnBnClickedTsplineSimplification()
{
	//Tspine Simplication : Iterative Refinement

	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = 0;

	if(currentindex < 0 || currentindex >= num)
		return;

	CNurbsSuface* pSurface;
	Tspline* T_target;
	CMesh3d* pMesh3D,*pMesh2D;
	if(pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_NURBSSURFACE)
	{
		pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(currentindex);

		//Generate Sampling Mesh
		pSurface->GenerateSamplingMesh(pMesh3D,pMesh2D,100,100);
		pMesh3D->m_Transform.Copy(&pSurface->m_Transform);
		pMesh2D->m_Transform.Copy(&pSurface->m_Transform);
		//pDoc->m_SceneGraph.Add(pMesh3D);
		//pDoc->m_SceneGraph.Add(pMesh2D);
		//pDoc->UpdateTreeControl();
		//return;
		//Get the initial Tspline
		T_target = new Tspline;
		std::vector<int> m_uknotindex,m_vknotindex;
		T_target->BuildFromNURBS(pSurface,m_uknotindex,m_vknotindex);
	}else
		return;

	//Generate a Bezier Surface
	Tspline* T_current = OverSimplifyTspline(T_target,pMesh3D,pMesh2D,pSurface);
	//Test
	T_current->m_Transform.Copy(&pSurface->m_Transform);
	T_target->m_Transform.Copy(&pSurface->m_Transform);
	//pDoc->m_SceneGraph.Add(T_current);
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	//return;
	//face flag
	FPROP_FACE_OFFENDING_FLAG fprop_face_offending_flag;
	VPROP_VERTEX_OFFENDING_FLAG vprop_vertex_offending_flag;
	VPROP_VERTEX_IMG_FLAG vprop_vertex_img_flag;
	if (!T_current->pmesh->get_property_handle(fprop_face_offending_flag, PROP_FACE_OFFENDING_FLAG))
		T_current->pmesh->add_property(fprop_face_offending_flag, PROP_FACE_OFFENDING_FLAG);
	if (!T_target->pmesh->get_property_handle(vprop_vertex_offending_flag, PROP_VERTEX_OFFENDING_FLAG))
		T_target->pmesh->add_property(vprop_vertex_offending_flag, PROP_VERTEX_OFFENDING_FLAG);
	if (!T_target->pmesh->get_property_handle(vprop_vertex_img_flag, PROP_VERTEX_IMG_FLAG))
		T_target->pmesh->add_property(vprop_vertex_img_flag, PROP_VERTEX_IMG_FLAG);
	//
	double length = 1.0 / T_target->m_Transform.GetScale()->x();
	double TOL = length * 1E-3;
	//pDoc->m_SceneGraph.Add(T_current->copy());
	//pDoc->m_SceneGraph.Add(T_target);
	//Least Square Fitting and set the offending vertex flag
	double tol = LeastSquareFit(T_current,T_target,TOL);
	double pretol;

	//T_current->filename = "0";
	pDoc->m_SceneGraph.Add(T_current->copy());
	//pDoc->m_SceneGraph.Add(T_target);

	//return;

	for(int i = 0; i < pSurface->m_uknotcount; i++)
		T_current->m_targetNurbsUKnots.push_back(pSurface->m_uknot[i]);
	for(int i = 0; i < pSurface->m_vknotcount; i++)
		T_current->m_targetNurbsVKnots.push_back(pSurface->m_vknot[i]);

	int iterationnum = 1;

	//ceshi
	/*int test = 0;
	while(test < 10)
	{
		for (MyMesh::VertexIter v_it = T_target->pmesh->vertices_begin(); v_it != T_target->pmesh->vertices_end(); ++v_it)
		{
			if(T_target->pmesh->property(vprop_vertex_img_flag,v_it.handle()) )
				continue;
			T_target->pmesh->property(vprop_vertex_offending_flag,v_it.handle()) = 1;
		}

		SetOffendingFaceFlag(T_target,T_current);

		for (MyMesh::FaceIter f_it = T_current->pmesh->faces_begin(); f_it != T_current->pmesh->faces_end(); ++f_it)
		{
			if(T_current->pmesh->status(f_it.handle()).deleted())
				continue;

			if(T_current->pmesh->property(fprop_face_offending_flag,f_it.handle()))
			{
				SubdivideFace(T_current,f_it.handle());
				T_current->pmesh->property(fprop_face_offending_flag,f_it.handle()) = FALSE;
			}
		}	
		int face1 = T_current->pmesh->n_faces();
		T_current->pmesh->garbage_collection();
		int face2 = T_current->pmesh->n_faces();
		T_current->SetUVKnotsVector();
		test++;
	}
	tol = LeastSquareFit(T_current,T_target,TOL,TRUE);
	pDoc->m_SceneGraph.Add(T_current);*/
	//return;
	while(tol > TOL)
	{
		//set initial face flag
		for (MyMesh::FaceIter f_it = T_current->pmesh->faces_begin(); f_it != T_current->pmesh->faces_end(); ++f_it)
		{
			T_current->pmesh->property(fprop_face_offending_flag,f_it.handle()) = 0;
		}

		//set the modification flag
		SetOffendingFaceFlag(T_target,T_current);
		
		//if(iterationnum == 3)
		//	return;


		for (MyMesh::FaceIter f_it = T_current->pmesh->faces_begin(); f_it != T_current->pmesh->faces_end(); ++f_it)
		{
			if(T_current->pmesh->status(f_it.handle()).deleted())
				continue;

			if(T_current->pmesh->property(fprop_face_offending_flag,f_it.handle()))
			{
				SubdivideFace(T_current,f_it.handle());
				T_current->pmesh->property(fprop_face_offending_flag,f_it.handle()) = FALSE;
			}
		}	

		int face1 = T_current->pmesh->n_faces();
		T_current->pmesh->garbage_collection();
		int face2 = T_current->pmesh->n_faces();

		//return;
		//Set Vertex UV Vectors
		T_current->SetUVKnotsVector();

		//return;
		pretol = tol;
		tol = LeastSquareFit(T_current,T_target,TOL,TRUE);

		char message[80];
		sprintf(message,"%d",iterationnum);
		T_current->filename = message;
		pDoc->m_SceneGraph.Add(T_current->copy());
		//if(fabs(pretol - tol) < 1e-6)
		//	break;

		iterationnum++;

		if(iterationnum >= 5)
			break;
		//return;
	}
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}