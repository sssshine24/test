// Action_LandMark.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Action_LandMark.h"
#include <iostream>
#include <fstream>
#include "MyTreeView.h"
#include "FormView3.h"
#include "Lib3D/Mesh3d.h"
#include "Lib3D/Vertex3d.h"
#include "math.h"
#include "Lib3D/Utils3d.h"
// Action_LandMark dialog

IMPLEMENT_DYNAMIC(Actions_LandMark, CPropertyPage)

Actions_LandMark::Actions_LandMark()
	: CPropertyPage(Actions_LandMark::IDD)
{

}

Actions_LandMark::~Actions_LandMark()
{
}

void Actions_LandMark::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Actions_LandMark, CPropertyPage)
	ON_BN_CLICKED(IDC_READLANDMARK, &Actions_LandMark::OnBnClickedReadlandmark)
	ON_BN_CLICKED(IDC_READSURFACEWITH, &Actions_LandMark::OnBnClickedReadsurfacewith)
	ON_BN_CLICKED(IDC_READSURFACEPLANE, &Actions_LandMark::OnBnClickedReadsurfaceplane)
	ON_BN_CLICKED(IDC_READLIONSURFACEWITHGRAPH, &Actions_LandMark::OnBnClickedReadlionsurfacewithgraph)
	ON_BN_CLICKED(IDC_COPYSELECTEDPOINTSTOANCHORPOINTS, &Actions_LandMark::OnBnClickedCopyselectedpointstoanchorpoints)
	ON_BN_CLICKED(IDC_SAVEANCHORPOINTS, &Actions_LandMark::OnBnClickedSaveanchorpoints)
	ON_BN_CLICKED(IDC_LANDMARK_COMPUTEMU, &Actions_LandMark::OnBnClickedLandmarkComputemu)
	ON_BN_CLICKED(IDC_LANDMARK_COMPUTEAREASTRETCH, &Actions_LandMark::OnBnClickedLandmarkComputeareastretch)
	ON_BN_CLICKED(IDC_LANDMARK_COMPUTEMU_Registration, &Actions_LandMark::OnBnClickedLandmarkComputemuRegistration)
	ON_BN_CLICKED(IDC_LANDMARK_COMPUTEAREASTRETCH_Regitration, &Actions_LandMark::OnBnClickedLandmarkComputeareastretchRegitration)
END_MESSAGE_MAP()


// Action_LandMark message handlers
extern CFormView3 *p_FormView3;
void Actions_LandMark::OnRotateLandMark()
{
	
}
void  Actions_LandMark::circle_center(Vector3D  *center,Vector3D  *pt,double  *radius)  
{  
	Vector3D p1 = pt[0];
	Vector3D p2 = pt[1];
	Vector3D p3 = pt[2];

	Vector3D v1 = (p1 + p2) / 2;
	Vector3D v2 = (p2 + p3) / 2;

	Vector3D d1,d2;
	d1.x = p1.y - p2.y;
	d1.y = -(p1.x - p2.x);
	d1.z = 0;

	d2.x = p2.y - p3.y;
	d2.y =  -(p2.x - p3.x);
	d2.z = 0;

	d1.SetUnit();
	d2.SetUnit();

	double a1 = d1 * d1;
	double b1 = -d1 * d2;
	double c1 = v2 * d1 - v1 * d1;

	double a2 = d1 * d2;
	double b2 = -d2 * d2;
	double c2 = v2 * d2 - v1 * d2;

	double t1 = -(b1*c2-c1*b2)/(a1*b2-a2*b1);
	double t2 = (a1*c2-a2*c1)/(a1*b2-a2*b1);

	*center = v1 + d1 * t1;

	*radius = Length(p1 - *center);
}
void Actions_LandMark::OnRemoveBoundaries_bone()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);

	int m_array[63]={
		29,33,37,41,45,49,53,57,61,
		65,69,73,77,81,85,89,93,97,101,
		105,109,113,117,121,125,129,133,137,141,
		145,149,153,157,161,164,168,172,176,180,
		184,188,192,196,200,204,208,212,216,219,
		222,225,228,231,234,237,240,243,245,247,
		249,251,253,255};

	for(int i = 0; i < pMesh3D->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3D->GetVertex(i);
		pVertex->m_Flag = i;
	}


	//Remove boundary edges
	for(int i = 0; i < pMesh3D->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int,int> pair = pMesh3D->m_ArraySharpEdges[i];
		int index1 = pair.first;
		int index2 = pair.second;

		CVertex3d* pVertex1 = pMesh3D->GetVertex(index1);
		CVertex3d* pVertex2 = pMesh3D->GetVertex(index2);

		if(pVertex1->IsOnBoundary() && pVertex2->IsOnBoundary() || pVertex1->m_Flag == 242 || pVertex2->m_Flag == 242)
		{
			int flag = 0;
			for(int ii = 0; ii < 65; ii++)
				if(pVertex1->m_Flag == m_array[ii] || pVertex2->m_Flag == m_array[ii])
					flag = 1;

			/*if(pVertex1->m_Flag == 242 || pVertex2->m_Flag == 242)
				flag = 1;
			if(pVertex1->m_Flag == 452 || pVertex2->m_Flag == 452)
				flag = 1;
			if(pVertex1->m_Flag == 6625 || pVertex2->m_Flag == 6625)
				flag = 1;
			if(pVertex1->m_Flag == 6759 || pVertex2->m_Flag == 6759)
				flag = 1;
			if(pVertex1->m_Flag == 6491 || pVertex2->m_Flag == 6491)
				flag = 1;
			if(pVertex1->m_Flag == 6357 || pVertex2->m_Flag == 6357)
				flag = 1;*/
			if(flag == 0)
			{
				pMesh3D->m_ArraySharpEdges.erase(pMesh3D->m_ArraySharpEdges.begin() + i);
				i = i - 1;
			}
		}
	}
	for(int i = 0; i < 62; i++)
	{
		std::pair<int,int>  pair1;
		pair1.first = m_array[i];
		pair1.second = m_array[i + 1];
		pMesh3D->m_ArraySharpEdges.push_back(pair1);
	}
	std::pair<int,int>  pair2;
	pair2.first = 25;
	pair2.second = 29;
	pMesh3D->m_ArraySharpEdges.push_back(pair2);
	pair2.first = 255;
	pair2.second = 257;
	pMesh3D->m_ArraySharpEdges.push_back(pair2);

}

void Actions_LandMark::OnRemoveBoundaries()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);

	//Remove boundary edges
	for(int i = 0; i < pMesh3D->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int,int> pair = pMesh3D->m_ArraySharpEdges[i];
		int index1 = pair.first;
		int index2 = pair.second;

		CVertex3d* pVertex1 = pMesh3D->GetVertex(index1);
		CVertex3d* pVertex2 = pMesh3D->GetVertex(index2);

		if(pVertex1->IsOnBoundary() || pVertex2->IsOnBoundary())
		{

			pMesh3D->m_ArraySharpEdges.erase(pMesh3D->m_ArraySharpEdges.begin() + i);
			i = i - 1;

		}
	}
}

void Actions_LandMark::OnBnClickedReadlandmark()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument("D:\\papers\\conformalbin\\bin\\3d001_0.pos_circ.m");
	//pDoc->OnOpenDocument("D:\\papers\\conformalbin\\bin\\3d001_0.landmark.txt");

	Vector3D pointarray[3];
	Vector3D center;
	double radius;


	/*double vv1[5]={0.398,0.234,0.334,0.149,0.356};
	double vv2[5]={0.384,0.213,0.355,0.143,0.365};
	double vv3[5]={0.415,0.223,0.344,0.145,0.36};
	double vv4[5]={0.463,0.236,0.366,0.156,0.343};
	double vv5[5]={0.426,0.183,0.307,0.147,0.376};
	double vv6[5]={0.427,0.197,0.384,0.14,0.359};

	double len1 = 0;
	double len2 = 0;
	double len3 = 0;
	double len4 = 0;
	double len5 = 0;
	for(int i = 0; i < 5; i++)
	{
		len1 += SQR(vv2[i] - vv1[i]);
		len2 += SQR(vv3[i] - vv1[i]);
		len3 += SQR(vv4[i] - vv1[i]);
		len4 += SQR(vv5[i] - vv1[i]);
		len5 += SQR(vv6[i] - vv1[i]);
	}
	len1 = sqrt(len1);
	len2 = sqrt(len2);
	len3 = sqrt(len3);
	len4 = sqrt(len4);
	len5 = sqrt(len5);*/

	CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);

	

	/*double minu = 1e10;
	double maxu = -1e10;
	double minv = 1e10;
	double maxv = -1e10;

	for(int i = 0; i < pMesh3D->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3D->GetVertex(i);
		double u = pVertex->x();;
		double v = pVertex->y();
		if(u < minu)
			minu = u;
		if(u > maxu)
			maxu = u;
		if(v < minv)
			minv = v;
		if(v > maxv)
			maxv = v;
	}*/

	//int array1[6]={1988,5191,515,5030,6022,1462};
	//int array1[6] = {3672,3065,4405,4644,4596,3223};
	//int array1[6] = {3867,2605,406,4143,3765,3751};
	//int array1[6] = {6556,4906,3660,6423,4815,3378};
	//int array1[6] = {2382,2221,102,3554,3474,6527};
	/*int array1[6] = {5548,2273,3444,1042,2942,1419};
	CVertex3d* pVertex1 = pMesh3D->m_ArrayVertex[array1[0]];
	Vector3D v1(pVertex1->x(),pVertex1->y(),pVertex1->z());
	CVertex3d* pVertex2 = pMesh3D->m_ArrayVertex[array1[1]];
	Vector3D v2(pVertex2->x(),pVertex2->y(),pVertex2->z());
	CVertex3d* pVertex3 = pMesh3D->m_ArrayVertex[array1[2]];
	Vector3D v3(pVertex3->x(),pVertex3->y(),pVertex3->z());
	CVertex3d* pVertex4 = pMesh3D->m_ArrayVertex[array1[3]];
	Vector3D v4(pVertex4->x(),pVertex4->y(),pVertex4->z());
	CVertex3d* pVertex5 = pMesh3D->m_ArrayVertex[array1[4]];
	Vector3D v5(pVertex5->x(),pVertex5->y(),pVertex5->z());
	CVertex3d* pVertex7 = pMesh3D->m_ArrayVertex[array1[5]];
	Vector3D v7(pVertex7->x(),pVertex7->y(),pVertex7->z());

	v2 = v2 - v1;
	//v2.SetUnit();
	v3 = v3 - v1;
	//v3.SetUnit();
	v4 = v4 - v1;
	//v4.SetUnit();
	v5 = v5 - v1;
	//v5.SetUnit();

	Vector3D v6 = (v2 + v3) / 2 - v1;
	v6.SetUnit();

	v7 = v7 - v1;
	v7.SetUnit();
	//angle from v7 to v6
	double asy = (acos(v6 * v7 / (Length(v6) * Length(v7)))) / (3.1415926);

	double r1 = Length(v2) * 2 / (maxu - minu);
	double theta1 = acos(v2 * v3 / (Length(v2) * Length(v3))) / (3.1415926 * 2);
	double r2 = Length(v4) * 2 / (maxu - minu);
	double theta12 = (acos(v6 * v4 / (Length(v6) * Length(v4))) - 3.1415926 / 2) / (3.1415926 * 2);
	double theta22 = (3.1415926 * 2 - acos(v6 * v5 / (Length(v6) * Length(v5))) - 3.1415926 / 2) / (3.1415926 * 2);
	return;*/

	//OnRemoveBoundaries();
	OnRemoveBoundaries_bone();
	pMesh3D->m_Modified = TRUE;
	pDoc->UpdateAllViews(NULL);
	return;

	int start, end;
	
	//end = pMesh3D->m_ArraySeletedEdges[0].second;
	std::vector<std::pair<int,int>> m_ArraySeletedEdges_ = pMesh3D->m_ArraySharpEdges;
	while(m_ArraySeletedEdges_.size() > 0)
	{
		std::vector<std::pair<int,int>> m_ArraySeletedEdges_1 = m_ArraySeletedEdges_;
		start = m_ArraySeletedEdges_[0].first;
		for(int i = 0; i < m_ArraySeletedEdges_1.size(); i++)
		{
			if(m_ArraySeletedEdges_1[i].first == start)
			{
				start = m_ArraySeletedEdges_1[i].second;
				m_ArraySeletedEdges_1.erase(m_ArraySeletedEdges_1.begin() + i);
				i = -1;
			}
			else
				if(m_ArraySeletedEdges_1[i].second == start)
				{
					start = m_ArraySeletedEdges_1[i].first;
					m_ArraySeletedEdges_1.erase(m_ArraySeletedEdges_1.begin() + i);
					i = -1;
				}	
		}

		std::vector<int> m_ArrayIndex;
		int start1 = start;
		pMesh3D->m_LandMarkEndPoints.push_back(start);
		//m_ArrayIndex.push_back(start1);
		//m_ArraySeletedEdges_1 = m_ArraySeletedEdges_;
		for(int i = 0; i < m_ArraySeletedEdges_.size(); i++)
		{
			if(m_ArraySeletedEdges_[i].first == start1)
			{
				start1 = m_ArraySeletedEdges_[i].second;
				m_ArrayIndex.push_back(start1);
				m_ArraySeletedEdges_.erase(m_ArraySeletedEdges_.begin() + i);
				i = -1;
			}
			else
				if(m_ArraySeletedEdges_[i].second == start1)
				{
					start1 = m_ArraySeletedEdges_[i].first;
					m_ArrayIndex.push_back(start1);
					m_ArraySeletedEdges_.erase(m_ArraySeletedEdges_.begin() + i);
					i = -1;
				}
				//if(start1 == end)
				//	break;
		}
		pMesh3D->m_LandMarkEndPoints.push_back(start1);
	}

	return;
	//pMesh3D->m_ArraySeletedVertexIndex = m_ArrayIndex;

	double minu = 1e10;
	double maxu = -1e10;
	double minv = 1e10;
	double maxv = -1e10;

	for(int i = 0; i < pMesh3D->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3D->GetVertex(i);
		double u = pVertex->x();;
		double v = pVertex->y();
		if(u < minu)
			minu = u;
		if(u > maxu)
			maxu = u;
		if(v < minv)
			minv = v;
		if(v > maxv)
			maxv = v;
	}

	double centeru,centerv;
	centeru = (minu + maxu) / 2;
	centerv = (minv + maxv) / 2;

	//int index = pMesh3D->m_LandMarkEndPoints[9];
	int index1 = pMesh3D->m_LandMarkEndPoints[0];
	int index2 = pMesh3D->m_LandMarkEndPoints[1];
	double u1 = (pMesh3D->GetVertex(index1)->x() + pMesh3D->GetVertex(index2)->x())/2;
	double v1 = (pMesh3D->GetVertex(index1)->y() + pMesh3D->GetVertex(index2)->y())/2;
	Vector3D v11(u1 - centeru,v1 - centerv,0);
	Vector3D v2(0,1,0);
	v11.SetUnit();
	v2.SetUnit();
	double angle =  3.1415926 + acos(v11 * v2);

	double an = 180 - acos(v11 * v2) * 180 / 3.1415926;
	double cosangle = cos(angle);
	double sinangle = sin(angle);

	for(int i = 0; i < pMesh3D->NbVertex(); i++)
	{
		double x = pMesh3D->GetVertex(i)->x() - centeru;
		double y = pMesh3D->GetVertex(i)->y() - centerv;
		double z = pMesh3D->GetVertex(i)->z() - center.z;

		double nx = x * cosangle - y * sinangle + centeru;
		double ny = x * sinangle + y * cosangle + centerv;
		double nz = 0;

		pMesh3D->GetVertex(i)->x(nx);
		pMesh3D->GetVertex(i)->y(ny);
		pMesh3D->GetVertex(i)->z(nz);
	}
	//

	//return;

	/*Vector3D pointarray[3];
	Vector3D center;
	double radius;
	CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	
	int index1 = pMesh3D->m_ArraySeletedEdges[0].first;
	int index2 = pMesh3D->m_ArraySeletedEdges[0].second;
	int index3 = pMesh3D->m_ArraySeletedEdges[1].first;
	if(index3 == index1 || index3 == index2)
		index3 = pMesh3D->m_ArraySeletedEdges[1].second;

	pointarray[0].x = pMesh3D->m_ArrayVertex[index1]->x();
	pointarray[0].y = pMesh3D->m_ArrayVertex[index1]->y();
	pointarray[0].z = pMesh3D->m_ArrayVertex[index1]->z();

	pointarray[1].x = pMesh3D->m_ArrayVertex[index2]->x();
	pointarray[1].y = pMesh3D->m_ArrayVertex[index2]->y();
	pointarray[1].z = pMesh3D->m_ArrayVertex[index2]->z();

	pointarray[2].x = pMesh3D->m_ArrayVertex[index3]->x();
	pointarray[2].y = pMesh3D->m_ArrayVertex[index3]->y();
	pointarray[2].z = pMesh3D->m_ArrayVertex[index3]->z();

	int start, end;
	start = pMesh3D->m_ArraySeletedEdges[0].first;
	end = pMesh3D->m_ArraySeletedEdges[0].second;
	std::vector<std::pair<int,int>> m_ArraySeletedEdges_ = pMesh3D->m_ArraySeletedEdges;

	for(int i = 0; i < m_ArraySeletedEdges_.size(); i++)
	{
		if(m_ArraySeletedEdges_[i].first == start)
		{
			start = m_ArraySeletedEdges_[i].second;
			m_ArraySeletedEdges_.erase(m_ArraySeletedEdges_.begin() + i);
			i = -1;
		}
		else
			if(m_ArraySeletedEdges_[i].second == start)
			{
				start = m_ArraySeletedEdges_[i].first;
				m_ArraySeletedEdges_.erase(m_ArraySeletedEdges_.begin() + i);
				i = -1;
			}	
	}
	std::vector<int> m_ArrayIndex;
	int start1 = start;
	pMesh3D->m_LandMarkEndPoints.push_back(start1);
	m_ArrayIndex.push_back(start1);
	m_ArraySeletedEdges_ = pMesh3D->m_ArraySeletedEdges;
	for(int i = 0; i < m_ArraySeletedEdges_.size(); i++)
	{
		if(m_ArraySeletedEdges_[i].first == start1)
		{
			start1 = m_ArraySeletedEdges_[i].second;
			m_ArrayIndex.push_back(start1);
			m_ArraySeletedEdges_.erase(m_ArraySeletedEdges_.begin() + i);
			i = -1;
		}
		else
			if(m_ArraySeletedEdges_[i].second == start1)
			{
				start1 = m_ArraySeletedEdges_[i].first;
				m_ArrayIndex.push_back(start1);
				m_ArraySeletedEdges_.erase(m_ArraySeletedEdges_.begin() + i);
				i = -1;
			}
		//if(start1 == end)
		//	break;
	}
	pMesh3D->m_LandMarkEndPoints.push_back(start1);
	pMesh3D->m_ArraySeletedVertexIndex = m_ArrayIndex;
	//return;
	//pointarray[0].x = cos(2.0);
	//pointarray[0].y = sin(2.0);
	//pointarray[0].z = 0;

	//pointarray[1].x = 1;
	//pointarray[1].y = 0;
	//pointarray[1].z = 0;

	//pointarray[2].x = 0;
	//pointarray[2].y = 1;
	//pointarray[2].z = 0;
	//the center and radius
	pointarray[0].x = pMesh3D->m_ArrayVertex[m_ArrayIndex[0]]->x();
	pointarray[1].y = pMesh3D->m_ArrayVertex[m_ArrayIndex[0]]->y();
	pointarray[2].z = pMesh3D->m_ArrayVertex[m_ArrayIndex[0]]->z();
	pointarray[1].x = pMesh3D->m_ArrayVertex[m_ArrayIndex[(m_ArrayIndex.size()) / 2] ]->x();
	pointarray[1].y = pMesh3D->m_ArrayVertex[m_ArrayIndex[(m_ArrayIndex.size()) / 2] ]->y();	
	pointarray[1].z = pMesh3D->m_ArrayVertex[m_ArrayIndex[(m_ArrayIndex.size()) / 2] ]->z();	
	pointarray[2].x = pMesh3D->m_ArrayVertex[m_ArrayIndex[m_ArrayIndex.size() - 1]]->x();
	pointarray[2].y = pMesh3D->m_ArrayVertex[m_ArrayIndex[m_ArrayIndex.size() - 1]]->y();	
	pointarray[2].z = pMesh3D->m_ArrayVertex[m_ArrayIndex[m_ArrayIndex.size() - 1]]->z();	
	circle_center(&center,pointarray,&radius);

	//
	start = m_ArrayIndex[0];
	end = m_ArrayIndex[m_ArrayIndex.size() - 1];
	Vector3D startpoint,endpoint;
	startpoint.x = pMesh3D->m_ArrayVertex[start]->x();
	startpoint.y = pMesh3D->m_ArrayVertex[start]->y();
	startpoint.z = pMesh3D->m_ArrayVertex[start]->z();

	endpoint.x = pMesh3D->m_ArrayVertex[end ]->x();
	endpoint.y = pMesh3D->m_ArrayVertex[end ]->y();
	endpoint.z = pMesh3D->m_ArrayVertex[end ]->z();

	Vector3D middlepoint = (startpoint + endpoint) / 2 - center;
	middlepoint.SetUnit();
	Vector3D v3(pMesh3D->m_ArrayVertex[21906]->x(),pMesh3D->m_ArrayVertex[21906]->y(),pMesh3D->m_ArrayVertex[21906]->z());
	v3 = v3 - center;
	v3.SetUnit();
	Vector3D v1 = startpoint - center;
	Vector3D v2 = endpoint - center;
	Vector3D v4(0,-1,0);
	v1.SetUnit();
	v2.SetUnit();
	double angle;

	double an = acos(middlepoint * v3) * 180 / 3.1415926;
	//if(middlepoint.y < center.y)
	angle = -acos(middlepoint * v4);
	//else
	//	angle = acos(v1 * v2);

	double cosangle = cos(angle);
	double sinangle = sin(angle);

	for(int i = 0; i < pMesh3D->NbVertex(); i++)
	{
		double x = pMesh3D->GetVertex(i)->x() - center.x;
		double y = pMesh3D->GetVertex(i)->y() - center.y;
		double z = pMesh3D->GetVertex(i)->z() - center.z;

		double nx = x * cosangle - y * sinangle + center.x;
		double ny = x * sinangle + y * cosangle + center.y;
		double nz = 0;

		pMesh3D->GetVertex(i)->x(nx);
		pMesh3D->GetVertex(i)->y(ny);
		pMesh3D->GetVertex(i)->z(nz);
	}*/

	pMesh3D->m_Modified = TRUE;
	pDoc->UpdateAllViews(NULL);
}


void Actions_LandMark::OnBnClickedReadsurfacewith()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\1\\a\\face.m");
	CMesh3d* pMesh3d1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	//int anchorpointsarray1[] = {4289,4653,563,12024,7219,4437,12157,5115,5120,16514,723,10254,
	//						   8038,4387,2806,163,521,16879,3982,3860,3588,578,4207,14168,4134,
	//						   12190,15719,16263,13726,16144,3286,13085,3236,2705,4407,4651 };
	std::vector<int> anchorpointsarray1;
	{
		int vindex;
		std::ifstream file("D:\\anchorpoint1_vindex.txt");
		while (!file.eof())
		{
			file >> vindex;
			anchorpointsarray1.push_back(vindex);
		}
		file.close();
	}
	{
		std::ofstream file("D:\\anchorpoint1.txt");
		pMesh3d1->SetFaceFlagAsItsIndex();

		for (int i = 0; i < anchorpointsarray1.size(); i++)
		{
			CVertex3d* pVertex = pMesh3d1->GetVertex(anchorpointsarray1[i]);
			CFace3d* pFace = pVertex->GetFaceNeighbor(0);
			if (pVertex == pFace->v1())
				file << pFace->m_Flag << " 1 0 \n";
			else if (pVertex == pFace->v2())
			{
				file << pFace->m_Flag << " 0 1 \n";
			}
			else
				file << pFace->m_Flag << " 0 0 \n";

			pMesh3d1->m_ArrayAnchorPoints.push_back(anchorpointsarray1[i]);
		}
		file.close();
		
	}

	{
		std::ofstream file("D:\\points.txt");
		pMesh3d1->SetFaceFlagAsItsIndex();

		for (int i = 0; i < pMesh3d1->m_ArrayVertex.GetSize(); i++)
		{
			CVertex3d* pVertex = pMesh3d1->GetVertex(i);
			CFace3d* pFace = pVertex->GetFaceNeighbor(0);
			if (pVertex == pFace->v1())
				file << pFace->m_Flag << " 1 0 \n";
			else if (pVertex == pFace->v2())
			{
				file << pFace->m_Flag << " 0 1 \n";
			}
			else
				file << pFace->m_Flag << " 0 0 \n";
		}
		file.close();
		pMesh3d1->SetVertexFlagAsZero();
	}

	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\e\\face.m");
	CMesh3d* pMesh3d2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	//int anchorpointsarray2[] = {7841,3394,4974,5977,7549,8097,5358,7070,1158,8021,6677,6076,
	//                              7414,8008,7526,886,1146,7795,7351,7161,7417,9352,7579,2208,10702,
	//                              1154,7240,1123,755,5685,8216,5173,10916,1870,7963,10966};
	std::vector<int> anchorpointsarray2;
	{
		int vindex;
		std::ifstream file("D:\\anchorpoint2_vindex.txt");
		while (!file.eof())
		{
			file >> vindex;
			anchorpointsarray2.push_back(vindex);
		}
		file.close();
	}
	{
		std::ofstream file("D:\\anchorpoint2.txt");
		pMesh3d2->SetFaceFlagAsItsIndex();

		for (int i = 0; i < anchorpointsarray2.size(); i++)
		{
			pMesh3d2->m_ArrayAnchorPoints.push_back(anchorpointsarray2[i]);
			CVertex3d* pVertex = pMesh3d2->GetVertex(anchorpointsarray2[i]);
			CFace3d* pFace = pVertex->GetFaceNeighbor(0);
			if (pVertex == pFace->v1())
				file << pFace->m_Flag << " 1 0 \n";
			else if (pVertex == pFace->v2())
			{
				file << pFace->m_Flag << " 0 1 \n";
			}
			else
				file << pFace->m_Flag << " 0 0 \n";

			//pMesh3d2->m_ArrayAnchorPoints.push_back(anchorpointsarray2[i]);
		}
		file.close();
		pMesh3d2->SetVertexFlagAsZero();
	}

	{
		CMesh3d* pMorphMesh = new CMesh3d();
		pMorphMesh->Copy(pMesh3d1);
		pMorphMesh->SetVertexFlagAsZero();
		std::ifstream file("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\data\\t_O.txt");
		for (int i = 0; i < pMesh3d1->NbVertex(); i++)
		{
			int faceindex;
			double w1, w2;
			file >> faceindex >> w1 >> w2;
			if (faceindex < pMesh3d2->NbFace() && faceindex >= 0)
			{
				double x1 = pMesh3d2->GetFace(faceindex)->v1()->x();
				double y1 = pMesh3d2->GetFace(faceindex)->v1()->y();
				double z1 = pMesh3d2->GetFace(faceindex)->v1()->z();

				double x2 = pMesh3d2->GetFace(faceindex)->v2()->x();
				double y2 = pMesh3d2->GetFace(faceindex)->v2()->y();
				double z2 = pMesh3d2->GetFace(faceindex)->v2()->z();

				double x3 = pMesh3d2->GetFace(faceindex)->v3()->x();
				double y3 = pMesh3d2->GetFace(faceindex)->v3()->y();
				double z3 = pMesh3d2->GetFace(faceindex)->v3()->z();

				double x = x1 * w1 + x2 * w2 + x3 * (1 - w1 - w2);
				double y = y1 * w1 + y2 * w2 + y3 * (1 - w1 - w2);
				double z = z1 * w1 + z2 * w2 + z3 * (1 - w1 - w2);

				pMorphMesh->GetVertex(i)->Set(x, y, z);
			}else
				pMorphMesh->GetVertex(i)->m_Flag = 1;

		}
		pMorphMesh->m_ArrayAnchorPoints = pMesh3d1->m_ArrayAnchorPoints;
		p_FormView3->OutPutMeshFaces_PovRay_Smooth_flags(pMorphMesh, "D:\\morphmesh.inc");
		//pMorphMesh->SetVertexFlagAsZero();
		pDoc->m_SceneGraph.Add(pMorphMesh);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\f-k\\face.m1.00.m");
	CMesh3d* pMesh3d3 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	pMesh3d3->m_ArrayAnchorPoints = pMesh3d1->m_ArrayAnchorPoints;
	for (int i = 0; i < pMesh3d3->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3d1->GetVertex(i);
		CVertex3d* pVertex_uv = pMesh3d3->GetVertex(i);
		pVertex_uv->SetColor(*pVertex->GetColor());
	}
	pMesh3d3->m_ColorBinding = COLOR_PER_VERTEX;
	pMesh3d3->m_ArraySharpEdges = pMesh3d1->m_ArraySharpEdges;
	pMesh3d3->SetVertexFlagAsZero();
	p_FormView3->OutPutMeshFaces_PovRay_Smooth_flags(pMesh3d3, "D:\\morphmesh.inc");
}

extern CMyTreeView* pTreeView;
void Actions_LandMark::OnBnClickedReadsurfaceplane()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc); 
	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\weightedaveragesonsurfaces\\WA_code\\data\\plane.obj");

	CMesh3d* pMesh3d1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	std::pair<int, std::pair<double, double>> anchorpoint;
	anchorpoint.first = 0;
	anchorpoint.second.first = 0.3;
	anchorpoint.second.second = 0.3;
	/*double x1, y1, z1, x2, y2, z2, x3, y3, z3,x4,y4,z4;
	{
		CFace3d* pFace = pMesh3d1->GetFace(0);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		 x1 = pVertex1->x() * 0.3 + pVertex2->x() * 0.3 + pVertex3->x() * 0.4;
		 y1 = pVertex1->y() * 0.3 + pVertex2->y() * 0.3 + pVertex3->y() * 0.4;
		 z1 = pVertex1->z() * 0.3 + pVertex2->z() * 0.3 + pVertex3->z() * 0.4;

		std::vector<double> pointcoordinates;
		pointcoordinates.push_back(x1);
		pointcoordinates.push_back(y1);
		pointcoordinates.push_back(z1);

		pMesh3d1->m_ArrayAnchorPoints.push_back(pointcoordinates);
	}
	{
		CFace3d* pFace = pMesh3d1->GetFace(1);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		 x2 = pVertex1->x() * 0.3 + pVertex2->x() * 0.3 + pVertex3->x() * 0.4;
		 y2 = pVertex1->y() * 0.3 + pVertex2->y() * 0.3 + pVertex3->y() * 0.4;
		 z2 = pVertex1->z() * 0.3 + pVertex2->z() * 0.3 + pVertex3->z() * 0.4;

		std::vector<double> pointcoordinates;
		pointcoordinates.push_back(x2);
		pointcoordinates.push_back(y2);
		pointcoordinates.push_back(z2);
		pMesh3d1->m_ArrayAnchorPoints.push_back(pointcoordinates);
	}
	{
		CFace3d* pFace = pMesh3d1->GetFace(2);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		 x3 = pVertex1->x() * 0.3 + pVertex2->x() * 0.3 + pVertex3->x() * 0.4;
		 y3 = pVertex1->y() * 0.3 + pVertex2->y() * 0.3 + pVertex3->y() * 0.4;
		 z3 = pVertex1->z() * 0.3 + pVertex2->z() * 0.3 + pVertex3->z() * 0.4;

		std::vector<double> pointcoordinates;
		pointcoordinates.push_back(x3);
		pointcoordinates.push_back(y3);
		pointcoordinates.push_back(z3);
		pMesh3d1->m_ArrayAnchorPoints.push_back(pointcoordinates);
	}
	{
		CFace3d* pFace = pMesh3d1->GetFace(3);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		x4 = pVertex1->x() * 0.3 + pVertex2->x() * 0.3 + pVertex3->x() * 0.4;
		y4 = pVertex1->y() * 0.3 + pVertex2->y() * 0.3 + pVertex3->y() * 0.4;
		z4 = pVertex1->z() * 0.3 + pVertex2->z() * 0.3 + pVertex3->z() * 0.4;

		std::vector<double> pointcoordinates;
		pointcoordinates.push_back(x4);
		pointcoordinates.push_back(y4);
		pointcoordinates.push_back(z4);
		pMesh3d1->m_ArrayAnchorPoints.push_back(pointcoordinates);
	}
	double A = pTreeView->TriangleArea(x1, y1, x2, y2, x3, y3);
	double A1 = pTreeView->TriangleArea(x1, y1, x2, y2, x4, y4);
	double A2 = pTreeView->TriangleArea(x2, y2, x3, y3, x4, y4);
	double A3 = pTreeView->TriangleArea(x3, y3, x1, y1, x4, y4);

	double Asum = A1 + A2 + A3;

	double w1 = A1 / Asum;
	double w2 = A2 / Asum;
	double w3 = A3 / Asum;

	double x5 = w1 * x3 + w2 * x1 + w3 * x2;
	double y5 = w1 * y3 + w2 * y1 + w3 * y2;*/
}


void Actions_LandMark::OnBnClickedReadlionsurfacewithgraph()
{
	// TODO: Add your control notification handler code here
	//CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	//ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\4\\a\\face.m");
	

	//pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\4\\d\\face.m");
	
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\4\\a\\face.m");
	CMesh3d* pMesh3d1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	//int anchorpointsarray1[] = {4289,4653,563,12024,7219,4437,12157,5115,5120,16514,723,10254,
	//						   8038,4387,2806,163,521,16879,3982,3860,3588,578,4207,14168,4134,
	//						   12190,15719,16263,13726,16144,3286,13085,3236,2705,4407,4651 };
	std::vector<int> anchorpointsarray1;
	{
		int vindex;
		std::ifstream file("D:\\anchorpoint1_vindex.txt");
		while (!file.eof())
		{
			file >> vindex;
			anchorpointsarray1.push_back(vindex);
		}
		file.close();
	}
	{
		std::ofstream file("D:\\anchorpoint1.txt");
		pMesh3d1->SetFaceFlagAsItsIndex();

		for (int i = 0; i < anchorpointsarray1.size(); i++)
		{
			CVertex3d* pVertex = pMesh3d1->GetVertex(anchorpointsarray1[i]);
			CFace3d* pFace = pVertex->GetFaceNeighbor(0);
			if (pVertex == pFace->v1())
				file << pFace->m_Flag << " 1 0 \n";
			else if (pVertex == pFace->v2())
			{
				file << pFace->m_Flag << " 0 1 \n";
			}
			else
				file << pFace->m_Flag << " 0 0 \n";

			pMesh3d1->m_ArrayAnchorPoints.push_back(anchorpointsarray1[i]);
		}
		file.close();

	}

	{
		std::ofstream file("D:\\points.txt");
		pMesh3d1->SetFaceFlagAsItsIndex();

		for (int i = 0; i < pMesh3d1->m_ArrayVertex.GetSize(); i++)
		{
			CVertex3d* pVertex = pMesh3d1->GetVertex(i);
			CFace3d* pFace = pVertex->GetFaceNeighbor(0);
			if (pVertex == pFace->v1())
				file << pFace->m_Flag << " 1 0 \n";
			else if (pVertex == pFace->v2())
			{
				file << pFace->m_Flag << " 0 1 \n";
			}
			else
				file << pFace->m_Flag << " 0 0 \n";
		}
		file.close();
		pMesh3d1->SetVertexFlagAsZero();
	}

	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\4\\d\\face.m");
	CMesh3d* pMesh3d2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	//int anchorpointsarray2[] = {7841,3394,4974,5977,7549,8097,5358,7070,1158,8021,6677,6076,
	//                              7414,8008,7526,886,1146,7795,7351,7161,7417,9352,7579,2208,10702,
	//                              1154,7240,1123,755,5685,8216,5173,10916,1870,7963,10966};
	std::vector<int> anchorpointsarray2;
	{
		int vindex;
		std::ifstream file("D:\\anchorpoint2_vindex.txt");
		while (!file.eof())
		{
			file >> vindex;
			anchorpointsarray2.push_back(vindex);
		}
		file.close();
	}
	{
		std::ofstream file("D:\\anchorpoint2.txt");
		pMesh3d2->SetFaceFlagAsItsIndex();

		for (int i = 0; i < anchorpointsarray2.size(); i++)
		{
			pMesh3d2->m_ArrayAnchorPoints.push_back(anchorpointsarray2[i]);
			CVertex3d* pVertex = pMesh3d2->GetVertex(anchorpointsarray2[i]);
			CFace3d* pFace = pVertex->GetFaceNeighbor(0);
			if (pVertex == pFace->v1())
				file << pFace->m_Flag << " 1 0 \n";
			else if (pVertex == pFace->v2())
			{
				file << pFace->m_Flag << " 0 1 \n";
			}
			else
				file << pFace->m_Flag << " 0 0 \n";

			//pMesh3d2->m_ArrayAnchorPoints.push_back(anchorpointsarray2[i]);
		}
		file.close();
		pMesh3d2->SetVertexFlagAsZero();
	}

	{
		CMesh3d* pMorphMesh = new CMesh3d();
		pMorphMesh->SetVertexFlagAsZero();
		pMorphMesh->Copy(pMesh3d1);
		std::ifstream file("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\data\\t_O.txt");
		for (int i = 0; i < pMesh3d1->NbVertex(); i++)
		{
			int faceindex;
			double w1, w2;
			file >> faceindex >> w1 >> w2;
			if (faceindex < pMesh3d2->NbFace() && faceindex >= 0)
			{
				double x1 = pMesh3d2->GetFace(faceindex)->v1()->x();
				double y1 = pMesh3d2->GetFace(faceindex)->v1()->y();
				double z1 = pMesh3d2->GetFace(faceindex)->v1()->z();

				double x2 = pMesh3d2->GetFace(faceindex)->v2()->x();
				double y2 = pMesh3d2->GetFace(faceindex)->v2()->y();
				double z2 = pMesh3d2->GetFace(faceindex)->v2()->z();

				double x3 = pMesh3d2->GetFace(faceindex)->v3()->x();
				double y3 = pMesh3d2->GetFace(faceindex)->v3()->y();
				double z3 = pMesh3d2->GetFace(faceindex)->v3()->z();

				double x = x1 * w1 + x2 * w2 + x3 * (1 - w1 - w2);
				double y = y1 * w1 + y2 * w2 + y3 * (1 - w1 - w2);
				double z = z1 * w1 + z2 * w2 + z3 * (1 - w1 - w2);

				pMorphMesh->GetVertex(i)->Set(x, y, z);
			}else
				pMorphMesh->GetVertex(i)->m_Flag = 1;

		}
		pMorphMesh->m_ArrayAnchorPoints = pMesh3d1->m_ArrayAnchorPoints;
		p_FormView3->OutPutMeshFaces_PovRay_Smooth_flags(pMorphMesh, "D:\\morphmesh.inc");
		pMorphMesh->SetVertexFlagAsZero();
		pDoc->m_SceneGraph.Add(pMorphMesh);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\4\\f-k\\m95.m1.00.m");
}


void Actions_LandMark::OnBnClickedCopyselectedpointstoanchorpoints()
{
	// TODO: Add your control notification handler code here
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
	pMesh->m_ArrayAnchorPoints.insert(pMesh->m_ArrayAnchorPoints.end(), pMesh->m_ArraySharpVertexIndex.begin(), pMesh->m_ArraySharpVertexIndex.end());

	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(2);
	if (p_FormView3->currentindex == 0)
	{
		pMesh2->m_ArrayAnchorPoints = pMesh->m_ArrayAnchorPoints;
	}
	else
		pMesh0->m_ArrayAnchorPoints = pMesh->m_ArrayAnchorPoints;

	pMesh->m_ArraySharpVertexIndex.clear();
	pDoc->UpdateAllViews(NULL);
}


void Actions_LandMark::OnBnClickedSaveanchorpoints()
{
	// TODO: Add your control notification handler code here
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	CMesh3d* pMesh3 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(3);
	{
		std::ofstream file("D:\\anchorpoint1.txt");
		pMesh1->SetFaceFlagAsItsIndex();

		for (int i = 0; i < pMesh1->m_ArrayAnchorPoints.size(); i++)
		{
			CVertex3d* pVertex = pMesh1->GetVertex(pMesh1->m_ArrayAnchorPoints[i]);
			CFace3d* pFace = pVertex->GetFaceNeighbor(0);
			if (pVertex == pFace->v1())
				file << pFace->m_Flag << " 1 0 \n";
			else if (pVertex == pFace->v2())
			{
				file << pFace->m_Flag << " 0 1 \n";
			}
			else
				file << pFace->m_Flag << " 0 0 \n";
		}
		file.close();
	}

	for (int i = pMesh2->m_ArrayAnchorPoints.size(); i < pMesh1->m_ArrayAnchorPoints.size(); i++)
	{
		CVertex3d* pVertex = pMesh3->GetVertex(pMesh1->m_ArrayAnchorPoints[i]);
		double mindis = 1e10;
		int nearestvindex;
		for (int j = 0; j < pMesh2->NbVertex(); j++)
		{
			CVertex3d* pVertex1 = pMesh2->GetVertex(j);
			double dis = DistanceSquare(pVertex, pVertex1);
			if (dis < mindis)
			{
				mindis = dis;
				nearestvindex = j;
			}
		}
		pMesh2->m_ArrayAnchorPoints.push_back(nearestvindex);
	}

	{
		std::ofstream file("D:\\anchorpoint2.txt");
		pMesh2->SetFaceFlagAsItsIndex();

		for (int i = 0; i < pMesh2->m_ArrayAnchorPoints.size(); i++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(pMesh2->m_ArrayAnchorPoints[i]);
			CFace3d* pFace = pVertex->GetFaceNeighbor(0);
			if (pVertex == pFace->v1())
				file << pFace->m_Flag << " 1 0 \n";
			else if (pVertex == pFace->v2())
			{
				file << pFace->m_Flag << " 0 1 \n";
			}
			else
				file << pFace->m_Flag << " 0 0 \n";
		}
		file.close();
	}

	{
		std::ofstream file("D:\\anchorpoint1_vindex.txt");
		pMesh1->SetFaceFlagAsItsIndex();

		for (int i = 0; i < pMesh1->m_ArrayAnchorPoints.size(); i++)
		{
			file << pMesh1->m_ArrayAnchorPoints[i] << " ";
		}
		file.close();
	}
	{
		std::ofstream file("D:\\anchorpoint2_vindex.txt");
		pMesh2->SetFaceFlagAsItsIndex();

		for (int i = 0; i < pMesh2->m_ArrayAnchorPoints.size(); i++)
		{
			file << pMesh2->m_ArrayAnchorPoints[i] << " ";
		}
		file.close();
	}
}

extern Actions_ConstrainedRegistration* pConstrainedRegistration;
void Actions_LandMark::OnBnClickedLandmarkComputemu()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\a\\face.m");

	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\b\\face.m.wms.pos.n.m");

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	pMesh2->m_ArraySharpEdges = pMesh1->m_ArraySharpEdges;
	pConstrainedRegistration->pMesh = pMesh1;
	pConstrainedRegistration->pMesh_UV = pMesh2;
	//
	pConstrainedRegistration->OnBnClickedConstrainedregistrationTargetvertexcolorMu();
	pDoc->UpdateAllViews(NULL);
}


void Actions_LandMark::OnBnClickedLandmarkComputeareastretch()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\a\\face.m");

	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\b\\face.m.wms.pos.n.m");

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	pMesh2->m_ArraySharpEdges = pMesh1->m_ArraySharpEdges;

	pConstrainedRegistration->pMesh = pMesh1;
	pConstrainedRegistration->pMesh_UV = pMesh2;
	//
	pConstrainedRegistration->OnBnClickedConstrainedregistrationBothMeshAreaStretch();
	pDoc->UpdateAllViews(NULL);
}


void Actions_LandMark::OnBnClickedLandmarkComputemuRegistration()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\a\\face.m");

	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\f-k\\face.m1.00.m");

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	pMesh2->m_ArraySharpEdges = pMesh1->m_ArraySharpEdges;
	pConstrainedRegistration->pMesh = pMesh1;
	pConstrainedRegistration->pMesh_UV = pMesh2;
	//
	pConstrainedRegistration->OnBnClickedConstrainedregistrationTargetvertexcolorMu_Registration();
	pDoc->UpdateAllViews(NULL);
}


void Actions_LandMark::OnBnClickedLandmarkComputeareastretchRegitration()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\a\\face.m");

	pDoc->OnOpenDocument("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\code\\mesh_pov_ray_files-20171104T091225Z-001\\mesh_pov_ray_files\\3\\f-k\\face.m1.00.m");

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	pMesh2->m_ArraySharpEdges = pMesh1->m_ArraySharpEdges;

	pConstrainedRegistration->pMesh = pMesh1;
	pConstrainedRegistration->pMesh_UV = pMesh2;
	//
	pConstrainedRegistration->OnBnClickedConstrainedregistrationBothMeshAreaStretch_Registration();
	pDoc->UpdateAllViews(NULL);
}
