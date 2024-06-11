// Action_Levy_Leastsquare.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "lib3d/Base3d.h"
#include "Lib3D/Mesh3d_OpenMesh.h"
#include "FormView3.h"
#include "Action_Levy_Leastsquare.h"
// Action_Levy_Leastsquare dialog

IMPLEMENT_DYNAMIC(Action_Levy_Leastsquare, CPropertyPage)

Action_Levy_Leastsquare::Action_Levy_Leastsquare()
	: CPropertyPage(Action_Levy_Leastsquare::IDD)
{

}

Action_Levy_Leastsquare::~Action_Levy_Leastsquare()
{
}

void Action_Levy_Leastsquare::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Action_Levy_Leastsquare, CPropertyPage)
	ON_BN_CLICKED(IDC_LEVY_LEAST_SQUARE, &Action_Levy_Leastsquare::OnBnClickedLevyLeastSquare)
END_MESSAGE_MAP()


BOOL Action_Levy_Leastsquare::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	((CButton *)GetDlgItem(IDC_LEVY_TWOPOINT))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_LEVY_UNIFORM_BOUNDARY))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_LEVY_CHORD_BOUNDARY))->SetCheck(0);
	//UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// Action_Levy_Leastsquare message handlers

void Action_Levy_Leastsquare::OnBnClickedLevyLeastSquare()
{
	// TODO: Add your control notification handler code here
	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = 0;

	if(currentindex < 0 || currentindex >= num)
		return;

	if(pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
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
		Compute_Boundary_Vertices_Parameter_Positions(pMesh3D);

		//Obtain the linear equations
		Compute_Linear_Equations(pMesh3D);

		//Generate a new parameter domain mesh
		GenerateParameterDomain(pMesh3D);
	}
}
void Action_Levy_Leastsquare::Compute_Boundary_Vertices_Parameter_Positions(CMesh3d_OpenMesh* pMesh3D)
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
	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_selectedflag,v_it.handle());
		if(flag)
		{
			m_ArraySeletedVertices.push_back(v_it.handle());
			//OpenMesh::Vec3d pt;
			//pt = pmesh->point (v_it);
			//::glVertex3f(pt[0],pt[1],pt[2]);
		}
	}
	//for(int i = 0; i < pMesh3D->m_FixVerticesArray.size(); i++)
	//{
	//	m_ArraySeletedVertices.push_back(MyMesh::VertexHandle(pMesh3D->m_FixVerticesArray[i]));
	//}
	if(((CButton *)GetDlgItem(IDC_LEVY_TWOPOINT))->GetCheck())
	{
		if(m_ArraySeletedVertices.size() == 2)
		{
			std::vector<double> vec1;
			vec1.push_back(0);
			vec1.push_back(0);
			std::vector<double> vec2;
			vec2.push_back(1);
			vec2.push_back(0);
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[0]) = vec1;
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[1]) = vec2;

			//std::vector<double> vec1,vec2;
			//OpenMesh::Vec3d pt1 = pMesh3D->pmesh->point (m_ArraySeletedVertices[0]);
			//OpenMesh::Vec3d pt2 = pMesh3D->pmesh->point (m_ArraySeletedVertices[1]);
			//vec1.push_back(pt1[0]);
			//vec1.push_back(pt1[1]);
			//vec2.push_back(pt2[0]);
			//vec2.push_back(pt2[1]);
			//pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[0]) = vec1;
			//pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[1]) = vec2;
		}else
			ASSERT(0);
	}else
	{
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
			//select
			pMesh3D->pmesh->property(vprop_selectedflag,nextvertexhandle) = 1;
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
			vec1.push_back(0);
			vec1.push_back(0);
			std::vector<double> vec2;
			vec2.push_back(1);
			vec2.push_back(0);
			std::vector<double> vec3;
			vec3.push_back(1);
			vec3.push_back(1);
			std::vector<double> vec4;
			vec4.push_back(0);
			vec4.push_back(1);
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[0]) = vec1;
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[1]) = vec2;
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[2]) = vec3;
			pMesh3D->pmesh->property(vprop_vertex_uv,m_ArraySeletedVertices[3]) = vec4;
			if(((CButton *)GetDlgItem(IDC_LEVY_UNIFORM_BOUNDARY))->GetCheck())
			{
				//uniformly set the parameterization on the boundary
				int num = (m_ArraySeletedVerticesIndex[1] - m_ArraySeletedVerticesIndex[0] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				for(int i = m_ArraySeletedVerticesIndex[0] + 1; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					vec1.clear();
					vec1.push_back(((i - m_ArraySeletedVerticesIndex[0] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
					vec1.push_back(0);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
				}
				num = (m_ArraySeletedVerticesIndex[2] - m_ArraySeletedVerticesIndex[1] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				for(int i = m_ArraySeletedVerticesIndex[1] + 1; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					vec1.clear();
					vec1.push_back(1);
					vec1.push_back(((i - m_ArraySeletedVerticesIndex[1] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
				}
				num = (m_ArraySeletedVerticesIndex[3] - m_ArraySeletedVerticesIndex[2] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				for(int i = m_ArraySeletedVerticesIndex[2] + 1; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					vec1.clear();
					vec1.push_back(1.0 - ((i - m_ArraySeletedVerticesIndex[2] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
					vec1.push_back(1);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
				}
				num = (m_ArraySeletedVerticesIndex[0] - m_ArraySeletedVerticesIndex[3] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				for(int i = m_ArraySeletedVerticesIndex[3] + 1; i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
				{
					vec1.clear();
					vec1.push_back(0);
					vec1.push_back(1.0 - ((i - m_ArraySeletedVerticesIndex[3] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
				}
			}else
			if(((CButton *)GetDlgItem(IDC_LEVY_CHORD_BOUNDARY))->GetCheck())
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
					vec1.push_back(accummlateedgelength / length);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
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
				for(int i = m_ArraySeletedVerticesIndex[3] + 1; i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size())%m_ArrarBoundaryVertices.size())
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
					vec1.push_back(1 - accummlateedgelength / length);
					pMesh3D->pmesh->property(vprop_vertex_uv,m_ArrarBoundaryVertices[i]) = vec1;
				}
			}
	}
}
double Action_Levy_Leastsquare::ComputeProjection(CMesh3d_OpenMesh* pMesh3D, OpenMesh::FaceHandle fh, OpenMesh::VertexHandle vh, int flag)
{
	OpenMesh::Vec3d pt[3];
	int vindex, i, j;
	j = 0;
	for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(fh); fv_it; ++fv_it)
	{
		if (fv_it.handle() == vh)
		{
			vindex = j;
		}
		pt[j] = pMesh3D->pmesh->point(fv_it);
		j++;
	}
	Vector3D p1, p2, p3;
	p1.SetValue(pt[0][0], pt[0][1], pt[0][2]);
	p2.SetValue(pt[1][0], pt[1][1], pt[1][2]);
	p3.SetValue(pt[2][0], pt[2][1], pt[2][2]);

	Vector3D origin, X, Y, Z;
	X = p2 - p1;
	X.SetUnit();
	Y = p3 - p2;
	Z = X ^ Y;
	Z.SetUnit();
	Y = Z ^ X;
	origin = p1;
	double x1, y1, x2, y2, x3, y3;
	x1 = 0;
	y1 = 0;
	x2 = (p2 - p1) * X;
	y2 = (p2 - p1) * Y;
	x3 = (p3 - p1) * X;
	y3 = (p3 - p1) * Y;
	//
	//x1 = p1.x;
	//y1 = p1.y;
	//x2 = p2.x;
	//y2 = p2.y;
	//x3 = p3.x;
	//y3 = p3.y;

	double val;
	double dT = (x1 * y2 - y1 * x2) + (x2 * y3 - y2 * x3) + (x3 * y1 - y3 * x1);

	if (flag == 0)
	{
		//real part
		if (vindex == 0)
		{
			val = x1;
		}
		else
			if (vindex == 1)
			{
				val = x2;
			}
			else
			{
				val = x3;
			}
	}
	else
	{
		//imaginary part
		if (vindex == 0)
		{
			val = y1;
		}
		else
			if (vindex == 1)
			{
				val = y2;
			}
			else
			{
				val = y3;
			}
	}
	return val;
}
double Action_Levy_Leastsquare::ComputeCoefficient(CMesh3d_OpenMesh* pMesh3D,OpenMesh::FaceHandle fh,OpenMesh::VertexHandle vh,int flag)
{
	OpenMesh::Vec3d pt[3];
	int vindex,i,j;
	j = 0;
	for(MyMesh::FaceVertexIter fv_it=pMesh3D->pmesh->fv_iter(fh); fv_it; ++fv_it)
	{
		if(fv_it.handle() == vh)
		{
			vindex = j;
		}
		pt[j] = pMesh3D->pmesh->point(fv_it);
		j++;
	}
	Vector3D p1,p2,p3;
	p1.SetValue(pt[0][0],pt[0][1],pt[0][2]);
	p2.SetValue(pt[1][0],pt[1][1],pt[1][2]);
	p3.SetValue(pt[2][0],pt[2][1],pt[2][2]);

	Vector3D origin,X,Y,Z;
	X = p2 - p1;
	X.SetUnit();
	Y = p3 - p2;
	Z = X ^ Y;
	Z.SetUnit();
	Y = Z ^ X;
	origin = p1;
	double x1,y1,x2,y2,x3,y3;
	x1 = 0; 
	y1 = 0;
	x2 = (p2 - p1) * X;
	y2 = (p2 - p1) * Y;
	x3 = (p3 - p1) * X;
	y3 = (p3 - p1) * Y;
	//
	//x1 = p1.x;
	//y1 = p1.y;
	//x2 = p2.x;
	//y2 = p2.y;
	//x3 = p3.x;
	//y3 = p3.y;

	double val;
	double dT = (x1 * y2 - y1 * x2) + (x2 * y3 - y2 * x3) + (x3 * y1 - y3 * x1);
	
	if(flag == 0)
	{
		//real part
		if(vindex == 0)
		{
			val =  (x3 - x2) / sqrt(dT);
		}else
		if(vindex == 1)
		{
			val =  (x1 - x3) / sqrt(dT);			
		}else
		{
			val =  (x2 - x1) / sqrt(dT);	
		}
	}else
	{
		//imaginary part
		if(vindex == 0)
		{
			val =  (y3 - y2) / sqrt(dT);
		}else
			if(vindex == 1)
			{
				val =  (y1 - y3) / sqrt(dT);			
			}else
			{
				val =  (y2 - y1) / sqrt(dT);	
			}
	}
	return val;
}
void Action_Levy_Leastsquare::Compute_Linear_Equations(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//use the nonboundaryindex to record the non-fix vertex index.
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Set the vertice a non-fixed index 
	int m_free_vertices_num = 0;
	int m_pin_vertices_num = 0;
	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_selectedflag,v_it.handle());
		if(flag)
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle()) = m_pin_vertices_num;
			m_pin_vertices_num++;
		}else
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle()) = m_free_vertices_num;
			m_free_vertices_num++;
		}
	}

	//the number of the triangle 
	int m_triangle_num = pMesh3D->pmesh->n_faces();
	std::vector<std::vector<double>> M1f,M2f,M1p,M2p;
	std::vector<double> Up;
	std::vector<double> rowvector;
	rowvector.resize(m_free_vertices_num);
	Up.resize(m_pin_vertices_num * 2);
	int i,j;
	//M1f

	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		for(int i = 0; i < m_free_vertices_num; i++)
			rowvector[i] = 0;

		for(MyMesh::FaceVertexIter fv_it=pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			int flag = pMesh3D->pmesh->property(vprop_selectedflag,fv_it.handle());
			if(flag)
				continue;
			int index = pMesh3D->pmesh->property(vprop_nonboundaryindex,fv_it.handle());

			rowvector[index] = ComputeCoefficient(pMesh3D,f_it.handle(),fv_it.handle(),0);
		}
		M1f.push_back(rowvector);
	}
	//M2f
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		for(int i = 0; i < m_free_vertices_num; i++)
			rowvector[i] = 0;

		for(MyMesh::FaceVertexIter fv_it=pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			int flag = pMesh3D->pmesh->property(vprop_selectedflag,fv_it.handle());
			if(flag)
				continue;
			int index = pMesh3D->pmesh->property(vprop_nonboundaryindex,fv_it.handle());

			rowvector[index] = ComputeCoefficient(pMesh3D,f_it.handle(),fv_it.handle(),1);
		}
		M2f.push_back(rowvector);
	}
	rowvector.resize(m_pin_vertices_num);
	//M1p
	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		for(int i = 0; i < m_pin_vertices_num; i++)
			rowvector[i] = 0;

		for(MyMesh::FaceVertexIter fv_it=pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			int flag = pMesh3D->pmesh->property(vprop_selectedflag,fv_it.handle());
			if(!flag)
				continue;
			int index = pMesh3D->pmesh->property(vprop_nonboundaryindex,fv_it.handle());

			rowvector[index] = ComputeCoefficient(pMesh3D,f_it.handle(),fv_it.handle(),0);
		}
		M1p.push_back(rowvector);
	}

	for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		for(int i = 0; i < m_pin_vertices_num; i++)
			rowvector[i] = 0;

		for(MyMesh::FaceVertexIter fv_it=pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			int flag = pMesh3D->pmesh->property(vprop_selectedflag,fv_it.handle());
			if(!flag)
				continue;
			int index = pMesh3D->pmesh->property(vprop_nonboundaryindex,fv_it.handle());

			rowvector[index] = ComputeCoefficient(pMesh3D,f_it.handle(),fv_it.handle(),1);
		}
		M2p.push_back(rowvector);
	}

	for(MyMesh::VertexIter v_it=pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_selectedflag,v_it.handle());
		if(flag)
		{
			int index = pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle());
			std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv,v_it.handle());
			
			Up[index] = uv[0];
			Up[m_pin_vertices_num + index] = uv[1];
		}
	}
	std::vector<std::vector<double>> A;
	A.resize(m_triangle_num * 2);
	for(int i = 0; i < m_triangle_num * 2; i++)
		A[i].resize(2 * m_free_vertices_num);
	for(int i = 0; i < m_triangle_num; i++)
		for(int j = 0; j < m_free_vertices_num; j++)
		{
			A[i][j] = M1f[i][j];
			A[i][j + m_free_vertices_num] = -M2f[i][j];
			A[i + m_triangle_num][j] = M2f[i][j];
			A[i + m_triangle_num][j + m_free_vertices_num] = M1f[i][j];
		}

	std::vector<std::vector<double>> A_;
	A_.resize(m_triangle_num * 2);
	for(int i = 0; i < m_triangle_num * 2; i++)
		A_[i].resize(2 * m_pin_vertices_num);
	for(int i = 0; i < m_triangle_num; i++)
		for(int j = 0; j < m_pin_vertices_num; j++)
		{
			A_[i][j] = M1p[i][j];
			A_[i][j + m_pin_vertices_num] = -M2p[i][j];
			A_[i + m_triangle_num][j] = M2p[i][j];
			A_[i + m_triangle_num][j + m_pin_vertices_num] = M1p[i][j];
		}

	std::vector<double> b = p_FormView3->MatrixVectorMultiplication(A_,Up);
	for(int i = 0; i < b.size(); i++)
		b[i] = -b[i];
	std::vector<double> uvarray;
	TRACE("\n");
	for(int i = 0; i < A.size(); i++)
	{
		for(int j = 0; j < A[i].size(); j++)
		{
			TRACE(" %f ",A[i][j]);
		}
		TRACE(" ; ");
	}
	TRACE("\n");
	for(int i = 0; i < b.size(); i++)
	{
		TRACE(" %f ;",b[i]);
	}

	Action_PolyOmino::SolveLinearSystem_matlab(A,b,uvarray);
	//p_FormView3->SolveLinearSystem(A,b,uvarray);

	//set the interior parameters
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	for(MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it!=pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->property(vprop_selectedflag,v_it.handle());

		if(flag)
			continue;

		int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex,v_it.handle());

		uvvalues[0] = uvarray[v0_index];
		uvvalues[1] = uvarray[v0_index + m_free_vertices_num];

		pMesh3D->pmesh->property(vprop_vertex_uv,v_it.handle()) = uvvalues;
	}
}
void Action_Levy_Leastsquare::GenerateParameterDomain(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

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

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	pDoc->m_SceneGraph.Add(pNewMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}