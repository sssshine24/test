// Actions_HarmonicMapping.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "lib3d/Base3d.h"
#include "Lib3D/Mesh3d_OpenMesh.h"
#include "FormView3.h"
#include "SDIViewSwitchDoc.h"
#include "MySparseMatrix.h"
#include "Action_PolyOmino.h"
#include "Complex_number.h"
#include "Actions_TeichmullerMap.h"
#include "Actions_HarmonicMapping.h"
#include "OpenMesh.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include "kdtree.h"
#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

#include "triangle.h"

// Actions_HarmonicMapping dialog
#define  BOUNDARY_UNIFORM 0
#define  BOUNDARY_CHORDLENGTHE 1
#define  WEIGHTS_UNIFORM 0
#define  WEIGHTS_LAPLACE 0
#define  WEIGHTS_MEANVALUE 1
#define MULENGTH(v) (sqrt(SQR(v[0]) + SQR(v[1])))
extern char Post_Message[1000];
std::vector<int> m_ArrayBoundarylandmarkpoints;
extern Action_PolyOmino* pPolyOmino;
extern Actions_TeichmullerMap* pTeichmullerMap;
IMPLEMENT_DYNAMIC(Actions_HarmonicMapping, CPropertyPage)

Actions_HarmonicMapping* pHarmonicMapping;
Actions_HarmonicMapping::Actions_HarmonicMapping()
	: CPropertyPage(Actions_HarmonicMapping::IDD)
{
	iternum = 0;
	pHarmonicMapping = this;
}

Actions_HarmonicMapping::~Actions_HarmonicMapping()
{
}

void Actions_HarmonicMapping::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Actions_HarmonicMapping, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_HARMONIC, &Actions_HarmonicMapping::OnBnClickedHarmonic)
	ON_BN_CLICKED(IDC_BUTTON_LOADTEST, &Actions_HarmonicMapping::OnBnClickedButtonLoadtest)
	ON_BN_CLICKED(IDC_BUTTON_LOADEXAMPLEWITHSINGULARITY, &Actions_HarmonicMapping::OnBnClickedButtonLoadExamplewithSingulairty)
	ON_BN_CLICKED(IDC_BUTTON_DYNAMICHARMONIC, &Actions_HarmonicMapping::OnBnClickedButtonDynamicHarmonic)
	ON_BN_CLICKED(IDC_BUTTON_DYNAMICHARMONIC_Initialize, &Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonicInitialize)
	ON_BN_CLICKED(IDC_BUTTON_DYNAMICHARMONIC_Iteration, &Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonicIteration)
	ON_BN_CLICKED(IDC_BUTTON_DYNAMICHARMONIC_FREEOFFLIP, &Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonicFreeofflip)
	ON_BN_CLICKED(IDC_BUTTON_MESH_SWAPSEQUENCE, &Actions_HarmonicMapping::OnBnClickedButtonMeshSwapsequence)
	ON_BN_CLICKED(IDC_BUTTON_MESH_MODIFYTOPOLOGY, &Actions_HarmonicMapping::OnBnClickedButtonMeshModifytopology)
	ON_BN_CLICKED(IDC_BUTTON_GENERATEGEOGIAGEBOUNDARY, &Actions_HarmonicMapping::OnBnClickedButtonGenerategeogiageboundary)
	ON_BN_CLICKED(IDC_BUTTON_GENERATEGEOGIAGEHYBRID, &Actions_HarmonicMapping::OnBnClickedButtonGenerategeogiagehybrid)
	ON_BN_CLICKED(IDC_BUTTON_STREIGHTENLANDMARK, &Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark)
	ON_BN_CLICKED(IDC_BUTTON_STREIGHTENLANDMARK2, &Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark2)
	ON_BN_CLICKED(IDC_BUTTON_STREIGHTENLANDMARK3, &Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark3)
	ON_BN_CLICKED(IDC_BUTTON_STREIGHTENLANDMARK4, &Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark4)
	ON_BN_CLICKED(IDC_BUTTON_STREIGHTENLANDMARK5, &Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark5)
	ON_BN_CLICKED(IDC_BUTTON_STREIGHTENLANDMARK6, &Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark6)
	ON_BN_CLICKED(IDC_BUTTON_STREIGHTENLANDMARK7, &Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark7)
	ON_BN_CLICKED(IDC_BUTTON_STREIGHTENLANDMARK8, &Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark8)
	ON_BN_CLICKED(IDC_BUTTON_STREIGHTENLANDMARK9, &Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark9)
	ON_BN_CLICKED(IDC_BUTTON_DYNAMICHARMONIC2, &Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonic2)
	ON_BN_CLICKED(IDC_BUTTON_DYNAMICHARMONIC3, &Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonic3)
	ON_BN_CLICKED(IDC_BUTTON1, &Actions_HarmonicMapping::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Actions_HarmonicMapping::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON9, &Actions_HarmonicMapping::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_USHAPE, &Actions_HarmonicMapping::OnBnClickedButtonUshape)
	ON_BN_CLICKED(IDC_BUTTON_USHAPE_TRIAGULATED, &Actions_HarmonicMapping::OnBnClickedButtonUshapeTriagulated)
	ON_BN_CLICKED(IDC_BUTTON_HARMONIC_ORIGINAL, &Actions_HarmonicMapping::OnBnClickedButtonHarmonicOriginal)
	ON_BN_CLICKED(IDC_BUTTON_FACEMODEL, &Actions_HarmonicMapping::OnBnClickedButtonFacemodel)
	ON_BN_CLICKED(IDC_BUTTON_HARMONIC_STRAIGHTENING, &Actions_HarmonicMapping::OnBnClickedButtonHarmonicStraightening)
	ON_BN_CLICKED(IDC_BUTTON_EXAMPLE1, &Actions_HarmonicMapping::OnBnClickedButtonExample1)
	ON_BN_CLICKED(IDC_BUTTON_HARMONIC_HV_STRAIGHTENING, &Actions_HarmonicMapping::OnBnClickedButtonHarmonicHvStraightening)
	ON_BN_CLICKED(IDC_BUTTON_EXAMPLE2, &Actions_HarmonicMapping::OnBnClickedButtonExample2)
END_MESSAGE_MAP()

BOOL Actions_HarmonicMapping::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	((CButton *)GetDlgItem(IDC_HARMONIC_BOUNDARY_UNIFORM))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_HARMONIC_BOUNDARY_CHORD))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_UNIFORM))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_LAPLACE))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_MEANVALUE))->SetCheck(1);
	//UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void Actions_HarmonicMapping::Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(CMesh3d_OpenMesh* pMesh3D)
{
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
	for (int i = 0; i < pMesh3D->m_FixVerticesArray.size(); i++)
	{
		m_ArraySeletedVertices.push_back(MyMesh::VertexHandle(pMesh3D->m_FixVerticesArray[i]));
	}

	std::vector<MyMesh::VertexHandle> m_ArrarBoundaryVertices;
	//store all the vertices
	MyMesh::HalfedgeHandle he1 = pMesh3D->pmesh->halfedge_handle(m_ArraySeletedVertices[0]);
	MyMesh::HalfedgeHandle he2;
	m_ArrarBoundaryVertices.push_back(m_ArraySeletedVertices[0]);
	MyMesh::VertexHandle prevertexhandle = m_ArraySeletedVertices[0];
	MyMesh::VertexHandle nextvertexhandle;
	while (!pMesh3D->pmesh->is_boundary(he1))
	{
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
		he1 = pMesh3D->pmesh->opposite_halfedge_handle(he1);
	}
	he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	while (1)
	{
		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->from_vertex_handle(he1);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->to_vertex_handle(he1);
		if (vh1 == prevertexhandle)
		{
			nextvertexhandle = vh2;
		}
		else
		{
			nextvertexhandle = vh1;
		}
		if (nextvertexhandle == m_ArrarBoundaryVertices[0])
			break;
		m_ArrarBoundaryVertices.push_back(nextvertexhandle);
		prevertexhandle = nextvertexhandle;
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	}

	std::vector<int> m_ArraySeletedVerticesIndex;
	//m_ArraySeletedVerticesIndex.push_back()
	//Find the index of the corner vertices
	for (int j = 0; j < m_ArraySeletedVertices.size(); j++)
	{
		for (int i = 0; i < m_ArrarBoundaryVertices.size(); i++)
		{
			if (m_ArraySeletedVertices[j] == m_ArrarBoundaryVertices[i])
			{
				m_ArraySeletedVerticesIndex.push_back(i);
				break;
			}
		}
	}

	//Sort the boundaries
	for (int i = 0; i < m_ArraySeletedVerticesIndex.size(); i++)
		for (int j = 0; j < m_ArraySeletedVerticesIndex.size() - 1; j++)
		{
			if (m_ArraySeletedVerticesIndex[j] > m_ArraySeletedVerticesIndex[j + 1])
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
	OpenMesh::Vec3d pt0, pt1, pt2, pt3;
	pt0 = pMesh3D->pmesh->point(m_ArraySeletedVertices[0]);
	pt1 = pMesh3D->pmesh->point(m_ArraySeletedVertices[1]);
	pt2 = pMesh3D->pmesh->point(m_ArraySeletedVertices[2]);
	pt3 = pMesh3D->pmesh->point(m_ArraySeletedVertices[3]);


	std::vector<double> vec1;
	vec1.push_back(pt0[0]);
	vec1.push_back(pt0[1]);
	std::vector<double> vec2;
	vec2.push_back(pt1[0]);
	vec2.push_back(pt1[1]);
	std::vector<double> vec3;
	vec3.push_back(pt2[0]);
	vec3.push_back(pt2[1]);
	std::vector<double> vec4;
	vec4.push_back(pt3[0]);
	vec4.push_back(pt3[1]);

	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[0]) = vec1;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[1]) = vec2;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[2]) = vec3;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[3]) = vec4;

	//Set the other boundary vertices
	//uniform 
	if (BOUNDARY_UNIFORM)
	{
		//uniformly set the parameterization on the boundary
		int num = (m_ArraySeletedVerticesIndex[1] - m_ArraySeletedVerticesIndex[0] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		for (int i = m_ArraySeletedVerticesIndex[0] + 1; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
		{
			vec1.clear();
			vec1.push_back(((i - m_ArraySeletedVerticesIndex[0] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
			vec1.push_back(0);
			pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec1;
		}
		num = (m_ArraySeletedVerticesIndex[2] - m_ArraySeletedVerticesIndex[1] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		for (int i = m_ArraySeletedVerticesIndex[1] + 1; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
		{
			vec1.clear();
			vec1.push_back(1);
			vec1.push_back(((i - m_ArraySeletedVerticesIndex[1] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
			pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec1;
		}
		num = (m_ArraySeletedVerticesIndex[3] - m_ArraySeletedVerticesIndex[2] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		for (int i = m_ArraySeletedVerticesIndex[2] + 1; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
		{
			vec1.clear();
			vec1.push_back(1.0 - ((i - m_ArraySeletedVerticesIndex[2] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
			vec1.push_back(1);
			pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec1;
		}
		num = (m_ArraySeletedVerticesIndex[0] - m_ArraySeletedVerticesIndex[3] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		for (int i = m_ArraySeletedVerticesIndex[3] + 1; i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
		{
			vec1.clear();
			vec1.push_back(0);
			vec1.push_back(1.0 - ((i - m_ArraySeletedVerticesIndex[3] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
			pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec1;
		}
	}
	else
		if (BOUNDARY_CHORDLENGTHE)
		{
			//chord length method

			//the bottom edge
			//first compute the edge length
			double length = 0;
			for (int i = m_ArraySeletedVerticesIndex[0]; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				length += edgelength;
			}
			double accummlateedgelength = 0;
			for (int i = m_ArraySeletedVerticesIndex[0] + 1; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				accummlateedgelength += edgelength;

				double w = accummlateedgelength / length;
				std::vector<double> vec;
				vec.resize(2);
				vec[0] = vec1[0] * (1 - w) + vec2[0] * w;
				vec[1] = vec1[1] * (1 - w) + vec2[1] * w;

				pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec;
			}

			//the right edge
			//first compute the edge length
			length = 0;
			for (int i = m_ArraySeletedVerticesIndex[1]; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				length += edgelength;
			}
			accummlateedgelength = 0;
			for (int i = m_ArraySeletedVerticesIndex[1] + 1; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				accummlateedgelength += edgelength;

				double w = accummlateedgelength / length;
				std::vector<double> vec;
				vec.resize(2);
				vec[0] = vec2[0] * (1 - w) + vec3[0] * w;
				vec[1] = vec2[1] * (1 - w) + vec3[1] * w;

				pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec;
			}

			//the top edge
			//first compute the edge length
			length = 0;
			for (int i = m_ArraySeletedVerticesIndex[2]; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				length += edgelength;
			}
			accummlateedgelength = 0;
			for (int i = m_ArraySeletedVerticesIndex[2] + 1; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				accummlateedgelength += edgelength;

				double w = accummlateedgelength / length;
				std::vector<double> vec;
				vec.resize(2);
				vec[0] = vec3[0] * (1 - w) + vec4[0] * w;
				vec[1] = vec3[1] * (1 - w) + vec4[1] * w;

				pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec;
			}

			//the left edge
			//first compute the edge length
			length = 0;
			for (int i = m_ArraySeletedVerticesIndex[3]; i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				length += edgelength;
			}
			accummlateedgelength = 0;
			for (int i = (m_ArraySeletedVerticesIndex[3] + 1) % m_ArrarBoundaryVertices.size(); i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				accummlateedgelength += edgelength;

				double w = accummlateedgelength / length;
				std::vector<double> vec;
				vec.resize(2);
				vec[0] = vec4[0] * (1 - w) + vec1[0] * w;
				vec[1] = vec4[1] * (1 - w) + vec1[1] * w;

				pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec;
			}
		}
}
void Actions_HarmonicMapping::Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints_Test(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//First set the parameterization for the boundary vertices
	OpenMesh::Vec3d pt0, pt1, pt2, pt3;

	std::vector<double> vec1;
	vec1.push_back(0);
	vec1.push_back(0);
	std::vector<double> vec2;
	vec2.push_back(1);
	vec2.push_back(0);
	std::vector<double> vec3;
	vec3.push_back(1);
	vec3.push_back(0.3);
	std::vector<double> vec4;
	vec4.push_back(0.5);
	vec4.push_back(0.3);
	std::vector<double> vec5;
	vec5.push_back(0.5);
	vec5.push_back(0.6);
	std::vector<double> vec6;
	vec6.push_back(1);
	vec6.push_back(0.6);
	std::vector<double> vec7;
	vec7.push_back(1);
	vec7.push_back(1);
	std::vector<double> vec8;
	vec8.push_back(0);
	vec8.push_back(1);
	pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(0)) = vec1;
	pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(1)) = vec2;
	pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(2)) = vec3;
	pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(3)) = vec4;
	pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(4)) = vec5;
	pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(5)) = vec6;
	pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(6)) = vec7;
	pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(7)) = vec8;
}
void Actions_HarmonicMapping::Compute_Boundary_Vertices_Parameter_Positions_AdditionalFixPoints(CMesh3d_OpenMesh* pMesh3D)
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
	for (int i = 0; i < pMesh3D->m_FixVerticesArray.size(); i++)
	{
		m_ArraySeletedVertices.push_back(MyMesh::VertexHandle(pMesh3D->m_FixVerticesArray[i]));
	}
	std::vector<MyMesh::VertexHandle> m_ArrarBoundaryVertices;
	//store all the vertices
	MyMesh::HalfedgeHandle he1 = pMesh3D->pmesh->halfedge_handle(m_ArraySeletedVertices[0]);
	MyMesh::HalfedgeHandle he2;
	m_ArrarBoundaryVertices.push_back(m_ArraySeletedVertices[0]);
	MyMesh::VertexHandle prevertexhandle = m_ArraySeletedVertices[0];
	MyMesh::VertexHandle nextvertexhandle;
	while (!pMesh3D->pmesh->is_boundary(he1))
	{
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
		he1 = pMesh3D->pmesh->opposite_halfedge_handle(he1);
	}
	he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	while (1)
	{
		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->from_vertex_handle(he1);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->to_vertex_handle(he1);
		if (vh1 == prevertexhandle)
		{
			nextvertexhandle = vh2;
		}
		else
		{
			nextvertexhandle = vh1;
		}
		if (nextvertexhandle == m_ArrarBoundaryVertices[0])
			break;
		m_ArrarBoundaryVertices.push_back(nextvertexhandle);
		prevertexhandle = nextvertexhandle;
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	}

	std::vector<int> m_ArraySeletedVerticesIndex;
	//m_ArraySeletedVerticesIndex.push_back()
	//Find the index of the corner vertices
	for (int j = 0; j < m_ArraySeletedVertices.size(); j++)
	{
		for (int i = 0; i < m_ArrarBoundaryVertices.size(); i++)
		{
			if (m_ArraySeletedVertices[j] == m_ArrarBoundaryVertices[i])
			{
				m_ArraySeletedVerticesIndex.push_back(i);
				break;
			}
		}
	}

	//Sort the boundaries
	for (int i = 0; i < m_ArraySeletedVerticesIndex.size(); i++)
		for (int j = 0; j < m_ArraySeletedVerticesIndex.size() - 1; j++)
		{
			if (m_ArraySeletedVerticesIndex[j] > m_ArraySeletedVerticesIndex[j + 1])
			{
				int temp = m_ArraySeletedVerticesIndex[j];
				m_ArraySeletedVerticesIndex[j] = m_ArraySeletedVerticesIndex[j + 1];
				m_ArraySeletedVerticesIndex[j + 1] = temp;

				MyMesh::VertexHandle temph = m_ArraySeletedVertices[j];
				m_ArraySeletedVertices[j] = m_ArraySeletedVertices[j + 1];
				m_ArraySeletedVertices[j + 1] = temph;
			}
		}

	double r;
	std::vector<double> vec1,vec2,vec3,vec4,vec5;
	//Set the other boundary vertices
	//uniform 
	//if (((CButton *)GetDlgItem(IDC_TEICHMULLER_BOUNDARY_UNIFORM))->GetCheck())
		//if (((CButton *)GetDlgItem(IDC_TEICHMULLER_BOUNDARY_CHORD))->GetCheck())
		if (1)
		{
			//chord length method
			//first compute the edge length
			for (int ii = 0; ii < m_ArraySeletedVertices.size(); ii++)
			{
				vec1 = pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[ii]);
				vec2 = pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[(ii + 1) % m_ArraySeletedVertices.size()]);
				double length = 0;
				for (int i = m_ArraySeletedVerticesIndex[ii]; i != m_ArraySeletedVerticesIndex[(ii + 1) % m_ArraySeletedVertices.size()]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt, endpt;
					startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					length += edgelength;
				}
				double accummlateedgelength = 0;
				for (int i = m_ArraySeletedVerticesIndex[ii] + 1; i != m_ArraySeletedVerticesIndex[(ii + 1) % m_ArraySeletedVertices.size()]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
				{
					int startvertex = i;
					int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
					OpenMesh::Vec3d startpt, endpt;
					startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
					endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
					OpenMesh::Vec3d edge = startpt - endpt;
					double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
					accummlateedgelength += edgelength;

					r = accummlateedgelength / length;
					vec5.clear();
					vec5.push_back(vec1[0] * (1 - r) + vec2[0] * (r));
					vec5.push_back(vec1[1] * (1 - r) + vec2[1] * (r));
					pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec5;
				}
			}
		}
}
void Actions_HarmonicMapping::Compute_Boundary_Vertices_Parameter_Positions_Circle(CMesh3d_OpenMesh* pMesh3D)
{
	//Boundary type 0: quad, 1:unit circle
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

	std::vector<double> vec1;

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
	for (int i = 0; i < pMesh3D->m_FixVerticesArray.size(); i++)
	{
		m_ArraySeletedVertices.push_back(MyMesh::VertexHandle(pMesh3D->m_FixVerticesArray[i]));
	}
	std::vector<MyMesh::VertexHandle> m_ArrarBoundaryVertices;
	//store all the vertices
	MyMesh::HalfedgeHandle he1 = pMesh3D->pmesh->halfedge_handle(m_ArraySeletedVertices[0]);
	MyMesh::HalfedgeHandle he2;
	m_ArrarBoundaryVertices.push_back(m_ArraySeletedVertices[0]);
	MyMesh::VertexHandle prevertexhandle = m_ArraySeletedVertices[0];
	MyMesh::VertexHandle nextvertexhandle;
	while (!pMesh3D->pmesh->is_boundary(he1))
	{
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
		he1 = pMesh3D->pmesh->opposite_halfedge_handle(he1);
	}
	he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	while (1)
	{
		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->from_vertex_handle(he1);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->to_vertex_handle(he1);
		if (vh1 == prevertexhandle)
		{
			nextvertexhandle = vh2;
		}
		else
		{
			nextvertexhandle = vh1;
		}
		if (nextvertexhandle == m_ArrarBoundaryVertices[0])
			break;
		m_ArrarBoundaryVertices.push_back(nextvertexhandle);
		prevertexhandle = nextvertexhandle;
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	}

	//
	double toltallength = 0;
	for (int i = 0; i < m_ArrarBoundaryVertices.size(); i++)
	{
		int startvertex = i;
		int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		OpenMesh::Vec3d startpt, endpt;
		startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
		endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
		OpenMesh::Vec3d edge = startpt - endpt;
		double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
		toltallength += edgelength;
	}
	double accummlateedgelength = 0;
	double r;
	std::vector<double> uv;
	for (int i = 0; i < m_ArrarBoundaryVertices.size(); i++)
	{
		if (i > 0)
		{
			int startvertex = i;
			int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
			OpenMesh::Vec3d startpt, endpt;
			startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
			endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
			OpenMesh::Vec3d edge = startpt - endpt;
			double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
			accummlateedgelength += edgelength;
		}

		r = accummlateedgelength * PI * 2 / toltallength;
		uv.clear();
		uv.push_back(cos(r));
		uv.push_back(sin(r));
		pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = uv;
	}	
}
void Actions_HarmonicMapping::Compute_Boundary_Vertices_Parameter_Positions_Circle_VerticalPoint(CMesh3d_OpenMesh* pMesh3D)
{
	//Boundary type 0: quad, 1:unit circle
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

	std::vector<double> vec1;

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
	for (int i = 0; i < pMesh3D->m_FixVerticesArray.size(); i++)
	{
		m_ArraySeletedVertices.push_back(MyMesh::VertexHandle(pMesh3D->m_FixVerticesArray[i]));
	}
	std::vector<MyMesh::VertexHandle> m_ArrarBoundaryVertices;
	//store all the vertices
	MyMesh::HalfedgeHandle he1 = pMesh3D->pmesh->halfedge_handle(m_ArraySeletedVertices[0]);
	MyMesh::HalfedgeHandle he2;
	m_ArrarBoundaryVertices.push_back(m_ArraySeletedVertices[0]);
	MyMesh::VertexHandle prevertexhandle = m_ArraySeletedVertices[0];
	MyMesh::VertexHandle nextvertexhandle;
	while (!pMesh3D->pmesh->is_boundary(he1))
	{
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
		he1 = pMesh3D->pmesh->opposite_halfedge_handle(he1);
	}
	he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	while (1)
	{
		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->from_vertex_handle(he1);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->to_vertex_handle(he1);
		if (vh1 == prevertexhandle)
		{
			nextvertexhandle = vh2;
		}
		else
		{
			nextvertexhandle = vh1;
		}
		if (nextvertexhandle == m_ArrarBoundaryVertices[0])
			break;
		m_ArrarBoundaryVertices.push_back(nextvertexhandle);
		prevertexhandle = nextvertexhandle;
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	}

	//
	double toltallength = 0;
	for (int i = 0; i < m_ArrarBoundaryVertices.size(); i++)
	{
		int startvertex = i;
		int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		OpenMesh::Vec3d startpt, endpt;
		startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
		endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
		OpenMesh::Vec3d edge = startpt - endpt;
		double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
		toltallength += edgelength;
	}
	double accummlateedgelength = 0;
	double r;
	std::vector<double> uv;
	for (int i = 0; i < m_ArrarBoundaryVertices.size(); i++)
	{
		if (i > 0)
		{
			int startvertex = i;
			int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
			OpenMesh::Vec3d startpt, endpt;
			startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
			endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
			OpenMesh::Vec3d edge = startpt - endpt;
			double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
			accummlateedgelength += edgelength;
		}

		r = accummlateedgelength * PI * 2 / toltallength - PI / 2;
		uv.clear();
		uv.push_back(cos(r));
		uv.push_back(sin(r));
		pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = uv;
	}
}
void Actions_HarmonicMapping::FindAdjacentFixPoints(CMesh3d_OpenMesh* pMesh3D, std::vector<std::vector<int>> &m_ArrayLandmarks, int findlandmarkindex,int vindex, std::vector<int> &m_AdjacentPointsArray)
{
	std::vector<int> m_ArrayLandmarks1;
	for (int i = 0; i < m_ArrayLandmarks.size(); i++)
	{
		for (int j = 0; j < m_ArrayLandmarks[i].size(); j++)
		{
			if (findlandmarkindex == m_ArrayLandmarks[i][j])
			{
				m_ArrayLandmarks1 = m_ArrayLandmarks[i];
				for (int k = 0; k < m_ArrayLandmarks1.size(); k++)
				{
					GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[m_ArrayLandmarks1[k]];
					for (int kk = 0; kk < pLandmark->landmark_points.size(); kk++)
					{
						if (pLandmark->landmark_points[kk] != vindex)
						{
							m_AdjacentPointsArray.push_back(pLandmark->landmark_points[kk]);
						}
					}
				}
				return;
			}
		}
	}
}
void Actions_HarmonicMapping::FixInnerPoints(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<std::vector<int>> m_ArrayLandmarks;

	ObtainConnectedLandmarks(pMesh3D, m_ArrayLandmarks);
	//
	int findlandmarkindex = -1;
	std::vector<int> m_AdjacentPointsArray;
	std::vector<int> m_FixVerticesArray_Inner_bak = pMesh3D->m_FixVerticesArray_Inner;
	for (int i = 0; i < m_FixVerticesArray_Inner_bak.size(); i++)
	{
		int vindex = m_FixVerticesArray_Inner_bak[i];

		for (int j = 0; j < pMesh3D->m_LandMarkArray.size(); j++)
			//for (int i = 0; i < pMesh3D->m_SelectedLandmarkIndexArray.size(); i++)
		{
			GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[j];

			int flag = pLandMark->IsPointOn(vindex);
			if (flag)
			{
				findlandmarkindex = j;
				break;
			}
		}
		//
		if (findlandmarkindex > -1)
		{
			m_AdjacentPointsArray.clear();

			FindAdjacentFixPoints(pMesh3D, m_ArrayLandmarks, findlandmarkindex, vindex, m_AdjacentPointsArray);
			
			pMesh3D->m_FixVerticesArray_Inner.insert(pMesh3D->m_FixVerticesArray_Inner.begin(), m_AdjacentPointsArray.begin(), m_AdjacentPointsArray.end());

			for (int kk = 0; kk < m_AdjacentPointsArray.size(); kk++)
			{
				//pMesh3D->m_FixVerticesArray_Inner_uv
				;
			}
		}
	}
	//Fixed Vertices
	std::vector<double> vec;
	for (int i = 0; i < pMesh3D->m_FixVerticesArray_Inner.size(); i++)
	{
		vec.clear();
		vec.push_back(pMesh3D->m_FixVerticesArray_Inner_uv[i * 2]);
		vec.push_back(pMesh3D->m_FixVerticesArray_Inner_uv[i * 2 + 1]);
		pMesh3D->pmesh->property(vprop_vertex_uv, MyMesh::VertexHandle(pMesh3D->m_FixVerticesArray_Inner[i])) = vec;
	}
}
void Actions_HarmonicMapping::Compute_Boundary_Vertices_Parameter_Positions_Quad(CMesh3d_OpenMesh* pMesh3D)
{
	//Boundary type 0: quad, 1:unit circle
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

	std::vector<double> vec1;
	
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
	for (int i = 0; i < pMesh3D->m_FixVerticesArray.size(); i++)
	{
		m_ArraySeletedVertices.push_back(MyMesh::VertexHandle(pMesh3D->m_FixVerticesArray[i]));
	}
	std::vector<MyMesh::VertexHandle> m_ArrarBoundaryVertices;
	//store all the vertices
	MyMesh::HalfedgeHandle he1 = pMesh3D->pmesh->halfedge_handle(m_ArraySeletedVertices[0]);
	MyMesh::HalfedgeHandle he2;
	m_ArrarBoundaryVertices.push_back(m_ArraySeletedVertices[0]);
	MyMesh::VertexHandle prevertexhandle = m_ArraySeletedVertices[0];
	MyMesh::VertexHandle nextvertexhandle;
	while (!pMesh3D->pmesh->is_boundary(he1))
	{
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
		he1 = pMesh3D->pmesh->opposite_halfedge_handle(he1);
	}
	he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	while (1)
	{
		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->from_vertex_handle(he1);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->to_vertex_handle(he1);
		if (vh1 == prevertexhandle)
		{
			nextvertexhandle = vh2;
		}
		else
		{
			nextvertexhandle = vh1;
		}
		if (nextvertexhandle == m_ArrarBoundaryVertices[0])
			break;
		m_ArrarBoundaryVertices.push_back(nextvertexhandle);
		prevertexhandle = nextvertexhandle;
		he1 = pMesh3D->pmesh->prev_halfedge_handle(he1);
	}

	std::vector<int> m_ArraySeletedVerticesIndex;
	//m_ArraySeletedVerticesIndex.push_back()
	//Find the index of the corner vertices
	for (int j = 0; j < m_ArraySeletedVertices.size(); j++)
	{
		for (int i = 0; i < m_ArrarBoundaryVertices.size(); i++)
		{
			if (m_ArraySeletedVertices[j] == m_ArrarBoundaryVertices[i])
			{
				m_ArraySeletedVerticesIndex.push_back(i);
				break;
			}
		}
	}

	//Sort the boundaries
	for (int i = 0; i < m_ArraySeletedVerticesIndex.size(); i++)
		for (int j = 0; j < m_ArraySeletedVerticesIndex.size() - 1; j++)
		{
			if (m_ArraySeletedVerticesIndex[j] > m_ArraySeletedVerticesIndex[j + 1])
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

	vec1.push_back(pMesh3D->m_FixVerticesArray_uv[0]);
	vec1.push_back(pMesh3D->m_FixVerticesArray_uv[1]);
	std::vector<double> vec2;
	vec2.push_back(pMesh3D->m_FixVerticesArray_uv[2]);
	vec2.push_back(pMesh3D->m_FixVerticesArray_uv[3]);
	std::vector<double> vec3;
	vec3.push_back(pMesh3D->m_FixVerticesArray_uv[4]);
	vec3.push_back(pMesh3D->m_FixVerticesArray_uv[5]);
	std::vector<double> vec4;
	vec4.push_back(pMesh3D->m_FixVerticesArray_uv[6]);
	vec4.push_back(pMesh3D->m_FixVerticesArray_uv[7]);
	std::vector<double> vec5;
	//
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[0]) = vec1;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[1]) = vec2;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[2]) = vec3;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[3]) = vec4;

	//Fixed Vertices
	for (int i = 0; i < pMesh3D->m_FixVerticesArray_Inner.size(); i++)
	{
		vec5.clear();
		vec5.push_back(pMesh3D->m_FixVerticesArray_Inner_uv[i * 2]);
		vec5.push_back(pMesh3D->m_FixVerticesArray_Inner_uv[i * 2 + 1]);
		pMesh3D->pmesh->property(vprop_vertex_uv, MyMesh::VertexHandle(pMesh3D->m_FixVerticesArray_Inner[i])) = vec5;
	}

	/*OpenMesh::Vec3d pt0, pt1, pt2, pt3;
	pt0 = pMesh3D->pmesh->point(m_ArraySeletedVertices[0]);
	pt1 = pMesh3D->pmesh->point(m_ArraySeletedVertices[1]);
	pt2 = pMesh3D->pmesh->point(m_ArraySeletedVertices[2]);
	pt3 = pMesh3D->pmesh->point(m_ArraySeletedVertices[3]);


	std::vector<double> vec1;
	vec1.push_back(pt0[0]);
	vec1.push_back(pt0[1]);
	std::vector<double> vec2;
	vec2.push_back(pt1[0]);
	vec2.push_back(pt1[1]);
	std::vector<double> vec3;
	vec3.push_back(pt2[0]);
	vec3.push_back(pt2[1]);
	std::vector<double> vec4;
	vec4.push_back(pt3[0]);
	vec4.push_back(pt3[1]);
	std::vector<double> vec5;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[0]) = vec1;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[1]) = vec2;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[2]) = vec3;
	pMesh3D->pmesh->property(vprop_vertex_uv, m_ArraySeletedVertices[3]) = vec4;*/

	double r;
	//Set the other boundary vertices
	//uniform 
	//if (((CButton *)GetDlgItem(IDC_TEICHMULLER_BOUNDARY_UNIFORM))->GetCheck())
	if (0)
	{
		//uniformly set the parameterization on the boundary
		int num = (m_ArraySeletedVerticesIndex[1] - m_ArraySeletedVerticesIndex[0] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		for (int i = m_ArraySeletedVerticesIndex[0] + 1; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
		{
			vec1.clear();
			vec1.push_back(((i - m_ArraySeletedVerticesIndex[0] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
			vec1.push_back(0);
			pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec1;
		}
		num = (m_ArraySeletedVerticesIndex[2] - m_ArraySeletedVerticesIndex[1] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		for (int i = m_ArraySeletedVerticesIndex[1] + 1; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
		{
			vec1.clear();
			vec1.push_back(1);
			vec1.push_back(((i - m_ArraySeletedVerticesIndex[1] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
			pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec1;
		}
		num = (m_ArraySeletedVerticesIndex[3] - m_ArraySeletedVerticesIndex[2] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		for (int i = m_ArraySeletedVerticesIndex[2] + 1; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
		{
			vec1.clear();
			vec1.push_back(1.0 - ((i - m_ArraySeletedVerticesIndex[2] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
			vec1.push_back(1);
			pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec1;
		}
		num = (m_ArraySeletedVerticesIndex[0] - m_ArraySeletedVerticesIndex[3] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
		for (int i = m_ArraySeletedVerticesIndex[3] + 1; i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
		{
			vec1.clear();
			vec1.push_back(0);
			vec1.push_back(1.0 - ((i - m_ArraySeletedVerticesIndex[3] + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size() + 0.0) / num);
			pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec1;
		}
	}
	else
		//if (((CButton *)GetDlgItem(IDC_TEICHMULLER_BOUNDARY_CHORD))->GetCheck())
		if (1)
		{
			//chord length method

			//the bottom edge
			//first compute the edge length
			double length = 0;
			for (int i = m_ArraySeletedVerticesIndex[0]; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				length += edgelength;
			}
			double accummlateedgelength = 0;
			for (int i = m_ArraySeletedVerticesIndex[0] + 1; i != m_ArraySeletedVerticesIndex[1]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				accummlateedgelength += edgelength;

				r = accummlateedgelength / length;
				vec5.clear();
				vec5.push_back(vec1[0] * (1 - r) + vec2[0] * (r));
				vec5.push_back(vec1[1]);
				pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec5;
			}

			//the right edge
			//first compute the edge length
			length = 0;
			for (int i = m_ArraySeletedVerticesIndex[1]; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				length += edgelength;
			}
			accummlateedgelength = 0;
			for (int i = m_ArraySeletedVerticesIndex[1] + 1; i != m_ArraySeletedVerticesIndex[2]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				accummlateedgelength += edgelength;

				r = accummlateedgelength / length;
				vec5.clear();
				vec5.push_back(vec2[0]);
				vec5.push_back(vec2[1] * (1 - r) + vec3[1] * r);
				pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec5;
			}

			//the top edge
			//first compute the edge length
			length = 0;
			for (int i = m_ArraySeletedVerticesIndex[2]; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				length += edgelength;
			}
			accummlateedgelength = 0;
			for (int i = m_ArraySeletedVerticesIndex[2] + 1; i != m_ArraySeletedVerticesIndex[3]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				accummlateedgelength += edgelength;

				r = accummlateedgelength / length;
				vec5.clear();
				vec5.push_back(vec3[0] * (1 - r) + vec4[0] * r);
				vec5.push_back(vec3[1]);
				pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec5;
			}

			//the left edge
			//first compute the edge length
			length = 0;
			for (int i = m_ArraySeletedVerticesIndex[3]; i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				length += edgelength;
			}
			accummlateedgelength = 0;
			for (int i = (m_ArraySeletedVerticesIndex[3] + 1) % m_ArrarBoundaryVertices.size(); i != m_ArraySeletedVerticesIndex[0]; i = (i + 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size())
			{
				int startvertex = i;
				int endvertex = (i - 1 + m_ArrarBoundaryVertices.size()) % m_ArrarBoundaryVertices.size();
				OpenMesh::Vec3d startpt, endpt;
				startpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[startvertex]);
				endpt = pMesh3D->pmesh->point(m_ArrarBoundaryVertices[endvertex]);
				OpenMesh::Vec3d edge = startpt - endpt;
				double edgelength = sqrt(edge[0] * edge[0] + edge[1] * edge[1] + edge[2] * edge[2]);
				accummlateedgelength += edgelength;

				r = accummlateedgelength / length;
				vec5.clear();
				vec5.push_back(vec1[0]);
				vec5.push_back(vec4[1] * (1 - r) + vec1[1] * r);
				pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrarBoundaryVertices[i]) = vec5;
			}
	}
}
void Actions_HarmonicMapping::Compute_Boundary_Vertices_Parameter_Positions_VerticalPoint(CMesh3d_OpenMesh* pMesh3D, int boundarytype = 0)
{
	//Boundary type 0: quad, 1:unit circle
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

	std::vector<double> vec1;
	if (pMesh3D->pBoundaryMeshReference)
	{
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			OpenMesh::Vec3d pt;
			pt = pMesh3D->pBoundaryMeshReference->pmesh->point(v_it);

			vec1.clear();
			vec1.push_back(pt[0]);
			vec1.push_back(pt[1]);
			pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = vec1;
		}
		return;
	}

	if (boundarytype == 0)
	{
		//Quad
		Compute_Boundary_Vertices_Parameter_Positions_Quad(pMesh3D);
	}
	else
	{
		//Circle
		Compute_Boundary_Vertices_Parameter_Positions_Circle_VerticalPoint(pMesh3D);
	}
}

void Actions_HarmonicMapping::Compute_Boundary_Vertices_Parameter_Positions(CMesh3d_OpenMesh* pMesh3D,int boundarytype = 0)
{
	//Boundary type 0: quad, 1:unit circle
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

	std::vector<double> vec1;
	if (pMesh3D->pBoundaryMeshReference)
	{
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			OpenMesh::Vec3d pt;
			pt = pMesh3D->pBoundaryMeshReference->pmesh->point (v_it);

			vec1.clear();
			vec1.push_back(pt[0]);
			vec1.push_back(pt[1]);
			pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = vec1;
		}
		return;
	}

	if (boundarytype == 0)
	{
		//Quad
		Compute_Boundary_Vertices_Parameter_Positions_Quad(pMesh3D);
	}
	else
	{
		//Circle
		Compute_Boundary_Vertices_Parameter_Positions_Circle(pMesh3D);
	}
}
double Actions_HarmonicMapping::ComputeNewEdgeLength(double x, double y, double a, double b)
{
	double aa = a * x + b * y + x;
	double bb = b * x - a * y + y;
	double val = sqrt(SQR(aa) + SQR(bb));
	return val;
}
double Actions_HarmonicMapping::ComputeNewEdgeLength_FromMu(OpenMesh::Vec3d v1, OpenMesh::Vec3d v2, std::vector<double> v1_mu, std::vector<double> v2_mu)
{
	OpenMesh::Vec3d dz_vec3d = v2 - v1;
	Complex_number dz(dz_vec3d[0], dz_vec3d[1]);
	Complex_number dz_bar(dz_vec3d[0], -dz_vec3d[1]);
	Complex_number mu((v1_mu[0] + v2_mu[0]) / 2, (v1_mu[1] + v2_mu[1]) / 2);
	Complex_number newdz = dz + mu * dz_bar;
	return newdz.length();
}
double Actions_HarmonicMapping::ComputeAngleFromDistortedTriangle(OpenMesh::Vec3d v1, OpenMesh::Vec3d v2, OpenMesh::Vec3d v3, std::vector<double> &v1_mu, std::vector<double> &v2_mu, std::vector<double> &v3_mu)
{
	Complex_number v1_complex(v1[0], v1[1]);
	Complex_number v2_complex(v2[0], v2[1]);
	Complex_number v3_complex(v3[0], v3[1]);
	Complex_number v1_mu_complex(v1_mu[0], v1_mu[1]);
	Complex_number v2_mu_complex(v2_mu[0], v2_mu[1]);
	Complex_number v3_mu_complex(v3_mu[0], v3_mu[1]);

	Complex_number v1_new_complex = v1_complex + (v1_mu_complex + v2_mu_complex + v3_mu_complex) * v1_complex.Bar() / 3;
	Complex_number v2_new_complex = v2_complex + (v1_mu_complex + v2_mu_complex + v3_mu_complex) * v2_complex.Bar() / 3;
	Complex_number v3_new_complex = v3_complex + (v1_mu_complex + v2_mu_complex + v3_mu_complex) * v3_complex.Bar() / 3;
	double v1_v2_newlength = Length(v1_new_complex - v2_new_complex);
	double v1_v3_newlength = Length(v1_new_complex - v3_new_complex);
	double v2_v3_newlength = Length(v2_new_complex - v3_new_complex);

	double angle1 = acos((SQR(v1_v3_newlength) + SQR(v2_v3_newlength) - SQR(v1_v2_newlength)) / (2 * v1_v3_newlength * v2_v3_newlength));
	return angle1;
}
void Actions_HarmonicMapping::Compute_Linear_Equations__LinearConstraints_ComputeEdgeWeight(CMesh3d_OpenMesh* pMesh3D, int muflag)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	VPROP_VERTEX_MU vprop_vertex_mu;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pMesh3D->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	//set the weight on the edge.
	for (MyMesh::EdgeIter e_it = pMesh3D->pmesh->edges_begin(); e_it != pMesh3D->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		int ehindex = eh.idx();
	
		int flag = pMesh3D->pmesh->is_boundary(eh);
		if (flag)
			continue;

		MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->from_vertex_handle(hh0);

		int vh1index = vh1.idx();
		int vh2index = vh2.idx();

		MyMesh::VertexHandle vh3, vh4;
		MyMesh::FaceHandle fh1 = pMesh3D->pmesh->face_handle(hh0);
		MyMesh::FaceHandle fh2 = pMesh3D->pmesh->face_handle(hh1);
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(fh1); fv_it; ++fv_it)
		{
			if (fv_it.handle().idx() != vh1.idx() && fv_it.handle().idx() != vh2.idx())
			{
				vh3 = fv_it.handle();
				break;
			}
		}
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(fh2); fv_it; ++fv_it)
		{
			if (fv_it.handle().idx() != vh1.idx() && fv_it.handle().idx() != vh2.idx())
			{
				vh4 = fv_it.handle();
				break;
			}
		}

		std::vector<double> v1_mu = pMesh3D->pmesh->property(vprop_vertex_mu, vh1);
		std::vector<double> v2_mu = pMesh3D->pmesh->property(vprop_vertex_mu, vh2);
		std::vector<double> v3_mu = pMesh3D->pmesh->property(vprop_vertex_mu, vh3);
		std::vector<double> v4_mu = pMesh3D->pmesh->property(vprop_vertex_mu, vh4);

		OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(vh1);
		OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(vh2);
		OpenMesh::Vec3d v3 = pMesh3D->pmesh->point(vh3);
		OpenMesh::Vec3d v4 = pMesh3D->pmesh->point(vh4);

		//double v1_v2 = ComputeNewEdgeLength_FromMu(v1,v2,v1_mu,v2_mu);
		//double v1_v3 = ComputeNewEdgeLength_FromMu(v1,v3,v1_mu,v3_mu);
		//double v2_v3 = ComputeNewEdgeLength_FromMu(v2,v3,v2_mu,v3_mu);
		//double v1_v4 = ComputeNewEdgeLength_FromMu(v1,v4,v1_mu,v4_mu);
		//double v2_v4 = ComputeNewEdgeLength_FromMu(v2,v4,v2_mu,v4_mu);

		double v1_v2, v1_v3, v2_v3, v1_v4, v2_v4;
		double angle1, angle2;
		if (muflag)
		{
			//v1_v2 = ComputeNewEdgeLength(v1[0] - v2[0],v1[1] - v2[1],(v1_mu[0] + v2_mu[0])/ 2, (v1_mu[1] + v2_mu[1])/ 2);
			//v1_v3 = ComputeNewEdgeLength(v1[0] - v3[0],v1[1] - v3[1],(v1_mu[0] + v3_mu[0])/ 2, (v1_mu[1] + v3_mu[1])/ 2);
			//v2_v3 = ComputeNewEdgeLength(v2[0] - v3[0],v2[1] - v3[1],(v2_mu[0] + v3_mu[0])/ 2, (v2_mu[1] + v3_mu[1])/ 2);
			//v1_v4 = ComputeNewEdgeLength(v1[0] - v4[0],v1[1] - v4[1],(v1_mu[0] + v4_mu[0])/ 2, (v1_mu[1] + v4_mu[1])/ 2);
			//v2_v4 = ComputeNewEdgeLength(v2[0] - v4[0],v2[1] - v4[1],(v2_mu[0] + v4_mu[0])/ 2, (v2_mu[1] + v4_mu[1])/ 2);
			angle1 = ComputeAngleFromDistortedTriangle(v1, v2, v3, v1_mu, v2_mu, v3_mu);
			angle2 = ComputeAngleFromDistortedTriangle(v1, v2, v4, v1_mu, v2_mu, v4_mu);
			//angle1 = acos((SQR(v1_v3) + SQR(v2_v3) - SQR(v1_v2)) / (2 * v1_v3 * v2_v3));
			//angle2 = acos((SQR(v1_v4) + SQR(v2_v4) - SQR(v1_v2)) / (2 * v1_v4 * v2_v4));
		}
		else
		{
			v1_v2 = Length(v1 - v2);
			v1_v3 = Length(v1 - v3);
			v2_v3 = Length(v2 - v3);
			v1_v4 = Length(v1 - v4);
			v2_v4 = Length(v2 - v4);

			angle1 = acos((SQR(v1_v3) + SQR(v2_v3) - SQR(v1_v2)) / (2 * v1_v3 * v2_v3));
			angle2 = acos((SQR(v1_v4) + SQR(v2_v4) - SQR(v1_v2)) / (2 * v1_v4 * v2_v4));
		}

		double w = 1.0 / tan(angle1) + 1.0 / tan(angle2);
		pMesh3D->pmesh->property(eprop_laplaceweight, eh) = w;
	}
}
int Actions_HarmonicMapping::Compute_DynamicHarmonic_Gradient(CMesh3d_OpenMesh* pMesh3D, OpenMesh::VertexHandle vh, std::vector<double> &m_Position, int ii, std::vector<std::pair<int, int>> &m_VerticalConstraints, std::vector<std::pair<int, int>> &m_HorizontalConstraints, int m_interior_vertices_num, double &derivativeval)
{
	//First X val, Then Y val, Vertical constraints, Horizontal Constraints
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	VPROP_VERTEX_MU vprop_vertex_mu;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pMesh3D->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	std::vector<std::pair<int, double>> m_ArrayElements;
	OpenMesh::Vec3d v0;
	std::pair<int, int> pair;

	derivativeval = 0;

	//Get the position
	v0 = pMesh3D->pmesh->point(vh);
	int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh);

	int boundaryflag = pMesh3D->pmesh->is_boundary(vh);

	if (boundaryflag)
		return 0;

	double pos1, pos2;
	for (MyMesh::VertexEdgeIter ve_it = pMesh3D->pmesh->ve_iter(vh); ve_it; ++ve_it)
	{
		MyMesh::EdgeHandle eh = ve_it.handle();
		int edgeflag = pMesh3D->pmesh->is_boundary(eh);
		if (edgeflag)
			continue;
		MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->from_vertex_handle(hh0);

		//
		int derivativeflag = 1;
		if (vh.idx() == vh2.idx())
			derivativeflag = -1;

		double w = pMesh3D->pmesh->property(eprop_laplaceweight, eh);
		w = w * derivativeflag;

		int vh1_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh1);
		int vh2_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh2);

		int vh1_flag = pMesh3D->pmesh->is_boundary(vh1);
		int vh2_flag = pMesh3D->pmesh->is_boundary(vh2);

		//std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_uv,vh1);
		//std::vector<double> uv2 = pMesh3D->pmesh->property(vprop_vertex_uv,vh2);

		if (vh1_flag)
		{
			OpenMesh::Vec3d vec = pMesh3D->pmesh->point(vh1);
			pos1 = vec[ii];
		}
		else
			pos1 = m_Position[m_interior_vertices_num * ii + vh1_index];

		if (vh2_flag)
		{
			OpenMesh::Vec3d vec = pMesh3D->pmesh->point(vh2);
			pos2 = vec[ii];
		}
		else
			pos2 = m_Position[m_interior_vertices_num * ii + vh2_index];

		derivativeval += w * 2 * (pos1 - pos2);
	}

	if (ii == 0)
	{
		for (int ij = 0; ij < m_VerticalConstraints.size(); ij++)
		{
			pair = m_VerticalConstraints[ij];
			if (pair.first == v0_index)
			{

				derivativeval += -1 * m_Position[m_interior_vertices_num * 2 + ij];
				//A.IncreaseItem(rowindex,m_interior_vertices_num + ij,1);
			}
			else if (pair.second == v0_index)
			{
				derivativeval += m_Position[m_interior_vertices_num * 2 + ij];
				//A.IncreaseItem(rowindex,m_interior_vertices_num + ij,-1);
			}
		}
	}
	else
	{
		for (int ij = 0; ij < m_HorizontalConstraints.size(); ij++)
		{
			pair = m_HorizontalConstraints[ij];
			if (pair.first == v0_index)
			{
				derivativeval += -1 * m_Position[m_interior_vertices_num * 2 + m_VerticalConstraints.size() + ij];
				//IncreaseItem(m_ArrayElements,m_interior_vertices_num + ij,1);
				//A.IncreaseItem(rowindex,m_interior_vertices_num + ij,1);
			}
			else if (pair.second == v0_index)
			{
				derivativeval += m_Position[m_interior_vertices_num * 2 + m_VerticalConstraints.size() + ij];
				//IncreaseItem(m_ArrayElements,m_interior_vertices_num + ij,-1);
				//A.IncreaseItem(rowindex,m_interior_vertices_num + ij,-1);
			}
		}
	}

	return 1;
}
void Actions_HarmonicMapping::Compute_LinearConstraints_Dynamic_Harmonic(CMesh3d_OpenMesh* pMesh3D, int muflag)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	VPROP_VERTEX_MU vprop_vertex_mu;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pMesh3D->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	//Set the vertice a non-boundary index 
	int m_interior_vertices_num = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->is_boundary(v_it.handle());
		if (flag)
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = -1;
		}
		else
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = m_interior_vertices_num;
			m_interior_vertices_num++;
		}
	}
	std::vector<std::pair<int, int>> m_HorizontalConstraints, m_VerticalConstraints;
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int index1 = pLandMark->landmark_points[j];
			int index2 = pLandMark->landmark_points[j + 1];

			int nonboundaryindex1 = pMesh3D->pmesh->property(vprop_nonboundaryindex, pMesh3D->pmesh->vertex_handle(index1));
			int nonboundaryindex2 = pMesh3D->pmesh->property(vprop_nonboundaryindex, pMesh3D->pmesh->vertex_handle(index2));

			pair.first = nonboundaryindex1;
			pair.second = nonboundaryindex2;

			if (pLandMark->style == 0)
				m_HorizontalConstraints.push_back(pair);
			else
				m_VerticalConstraints.push_back(pair);
		}
	}
	std::vector<MyMesh::VertexHandle> m_ArrayOneRingVertices;
	std::vector<double> omegaarray;
	std::vector<double> equation;
	//std::vector<std::vector<double>> A;
	//alglib::sparsematrix ma;
	MySparseMatrix A;
	//alglib::sparsecreate(m_interior_vertices_num, m_interior_vertices_num, ma);
	std::vector<double> b;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	OpenMesh::Vec3d v0, v_i, v_iminus1, v_iadd1;

	//
	Compute_Linear_Equations__LinearConstraints_ComputeEdgeWeight(pMesh3D, muflag);

	int rowindex;
	double bval;
	int flag;

	std::vector<double> m_Position, Gradientvec;
	m_Position.resize(m_interior_vertices_num * 2 + m_VerticalConstraints.size() + m_HorizontalConstraints.size(), 0);

	int num = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		OpenMesh::Vec3d vec = pMesh3D->pmesh->point(v_it.handle());
		m_Position[num] = vec[0];
		m_Position[m_interior_vertices_num + num] = vec[1];
		num++;
	}
	double derivativeval;
	int vnum = 0;
	double step = 0.05;
	int itenum = 0;
	TRACE("\n");
	for (int ij = 0; ij < m_Position.size(); ij++)
		TRACE(" %11.5f ", m_Position[ij]);
	TRACE("\n");
	while (1)
	{
		Gradientvec.clear();
		Gradientvec.resize(m_interior_vertices_num * 2 + m_VerticalConstraints.size() + m_HorizontalConstraints.size(), 0);
		for (int ii = 0; ii < 2; ii++)
		{
			vnum = 0;
			for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
			{
				if (pMesh3D->pmesh->is_boundary(v_it.handle()))
					continue;
				flag = Compute_DynamicHarmonic_Gradient(pMesh3D, v_it.handle(), m_Position, ii, m_VerticalConstraints, m_HorizontalConstraints, m_interior_vertices_num, derivativeval);
				Gradientvec[ii * m_interior_vertices_num + vnum] = derivativeval;
				vnum++;
			}
		}

		for (int i = 0; i < m_VerticalConstraints.size(); i++)
		{
			pair = m_VerticalConstraints[i];
			Gradientvec[m_interior_vertices_num * 2 + i] = m_Position[pair.first] - m_Position[pair.second];
		}

		for (int i = 0; i < m_HorizontalConstraints.size(); i++)
		{
			pair = m_HorizontalConstraints[i];
			Gradientvec[m_interior_vertices_num * 2 + m_VerticalConstraints.size() + i] = m_Position[m_interior_vertices_num + pair.first] - m_Position[m_interior_vertices_num + pair.second];
		}
		//Compute_Linear_Equations__LinearConstraints_Constraints(A,b,rowindex,ii,m_VerticalConstraints,m_HorizontalConstraints);

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
		//Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A,b,uvarray[ii]);

		for (int ij = 0; ij < m_Position.size(); ij++)
			m_Position[ij] = m_Position[ij] - Gradientvec[ij] * step;

		double len1 = 0;
		for (int ij = 0; ij < Gradientvec.size(); ij++)
		{
			len1 += SQR(Gradientvec[ij]);
		}
		double len = sqrt(len1);
		TRACE("%5d, %11.5f, ", itenum, len);
		for (int ij = 0; ij < m_Position.size(); ij++)
			TRACE(" %11.5f ", m_Position[ij]);
		TRACE("\n");
		if (len < 1e-5)
			break;
		itenum++;
	}


	//set the interior parameters
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	vnum = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int boundaryflag = pMesh3D->pmesh->is_boundary(v_it.handle());

		if (boundaryflag)
			continue;

		uvvalues[0] = m_Position[vnum];
		uvvalues[1] = m_Position[m_interior_vertices_num + vnum];

		pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
		vnum++;
	}
}
int Actions_HarmonicMapping::MeshUVLandmarkInvalid(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected_flag;
	VPROP_VERTEX_INEQUAL_CONSTRAINT vprop_vertex_inequal_flag;

	//Vertex selected flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED);

	//Vertex Inequal flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_inequal_flag, PROP_VERTEX_INEQUAL_CONSTRAINT))
		pMesh3D->pmesh->add_property(vprop_vertex_inequal_flag, PROP_VERTEX_INEQUAL_CONSTRAINT);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_selected_flag, v_it.handle()) = 0;
	}
	int invalidnumber = 0;
	//whether there is a error
	std::vector<double> uv1, uv2, uv3;
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		for (int j = 0; j < pLandMark->landmark_points.size() - 2; j++)
		{
			int index1 = pLandMark->landmark_points[j];
			int index2 = pLandMark->landmark_points[j + 1];
			int index3 = pLandMark->landmark_points[j + 2];

			OpenMesh::VertexHandle vh1 = pMesh3D->pmesh->vertex_handle(index1);
			OpenMesh::VertexHandle vh2 = pMesh3D->pmesh->vertex_handle(index2);
			OpenMesh::VertexHandle vh3 = pMesh3D->pmesh->vertex_handle(index3);

			uv1.clear();
			uv2.clear();
			uv3.clear();
			uv1 = pMesh3D->pmesh->property(vprop_vertex_uv, vh1);
			uv2 = pMesh3D->pmesh->property(vprop_vertex_uv, vh2);
			uv3 = pMesh3D->pmesh->property(vprop_vertex_uv, vh3);

			if (pLandMark->style == 0)
			{
				if (uv2[0]  < uv3[0] && uv2[0] < uv1[0] || uv2[0]  > uv3[0] && uv2[0] > uv1[0])
				{
					invalidnumber++;
					pMesh3D->pmesh->property(vprop_vertex_selected_flag, vh1) = 1;
					pMesh3D->pmesh->property(vprop_vertex_selected_flag, vh2) = 1;
					pMesh3D->pmesh->property(vprop_vertex_selected_flag, vh3) = 1;
					pMesh3D->pmesh->property(vprop_vertex_inequal_flag, vh1) = 1;
					pMesh3D->pmesh->property(vprop_vertex_inequal_flag, vh2) = 1;
					pMesh3D->pmesh->property(vprop_vertex_inequal_flag, vh3) = 1;
				}
			}
			else
			{
				if (uv2[1]  < uv3[1] && uv2[1] < uv1[1] || uv2[1]  > uv3[1] && uv2[1] > uv1[1])
				{
					invalidnumber++;
					pMesh3D->pmesh->property(vprop_vertex_selected_flag, vh1) = 1;
					pMesh3D->pmesh->property(vprop_vertex_selected_flag, vh2) = 1;
					pMesh3D->pmesh->property(vprop_vertex_selected_flag, vh3) = 1;

					pMesh3D->pmesh->property(vprop_vertex_inequal_flag, vh1) = 1;
					pMesh3D->pmesh->property(vprop_vertex_inequal_flag, vh2) = 1;
					pMesh3D->pmesh->property(vprop_vertex_inequal_flag, vh3) = 1;
				}
			}
		}
	}

	//copy the vertex selected flag
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh->GetVertex(v_it.handle().idx())->m_Flag = pMesh3D->pmesh->property(vprop_vertex_selected_flag, v_it.handle());
	}
	//if(invalidnumber > 0)
	return invalidnumber;
}
void Actions_HarmonicMapping::Compute_Constaints(CMesh3d_OpenMesh* pMesh3D, MySparseMatrix &Acon, std::vector<double>& bcon, int ii)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	VPROP_VERTEX_MU vprop_vertex_mu;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected_flag;
	VPROP_VERTEX_INEQUAL_CONSTRAINT vprop_vertex_inequal_flag;

	//Vertex Inequal flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_inequal_flag, PROP_VERTEX_INEQUAL_CONSTRAINT))
		pMesh3D->pmesh->add_property(vprop_vertex_inequal_flag, PROP_VERTEX_INEQUAL_CONSTRAINT);

	//Vertex selected flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Vertex nonboundary index;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Edge laplace weight;
	if (!pMesh3D->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pMesh3D->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	int rowindex = 0;
	int landmark_startvindex, landmark_endvindex;
	//
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		if (pLandMark->style != ii)
			continue;
		landmark_startvindex = pLandMark->landmark_points[0];
		landmark_endvindex = pLandMark->landmark_points[pLandMark->landmark_points.size() - 1];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			//if (j != 0 && j != pLandMark->landmark_points.size() - 2)
			//	continue;

			int index1 = pLandMark->landmark_points[j];
			int index2 = pLandMark->landmark_points[j + 1];

			OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(landmark_startvindex));
			OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(landmark_endvindex));

			int nonboundaryindex1 = pMesh3D->pmesh->property(vprop_nonboundaryindex, pMesh3D->pmesh->vertex_handle(index1));
			int nonboundaryindex2 = pMesh3D->pmesh->property(vprop_nonboundaryindex, pMesh3D->pmesh->vertex_handle(index2));

			int vflag1 = pMesh3D->pmesh->property(vprop_vertex_inequal_flag, pMesh3D->pmesh->vertex_handle(index1));
			int vflag2 = pMesh3D->pmesh->property(vprop_vertex_inequal_flag, pMesh3D->pmesh->vertex_handle(index2));

			if (!vflag1 || !vflag2)
				continue;

			if (v1[ii] < v2[ii])
			{
				Acon.AddItem(rowindex, nonboundaryindex1, 1);
				Acon.AddItem(rowindex, nonboundaryindex2, -1);
				rowindex++;
			}
			else
			{
				Acon.AddItem(rowindex, nonboundaryindex2, 1);
				Acon.AddItem(rowindex, nonboundaryindex1, -1);
				rowindex++;
			}
		}
	}
	Acon.m = rowindex;
	bcon.resize(rowindex, 0);
}
void Actions_HarmonicMapping::ObtainConnectedLandmarks(CMesh3d_OpenMesh* pMesh3D, std::vector<std::vector<int>> &m_ArrayLandmarks)
{
	VPROP_VERTEX_INTERINFO vprop_vertex_inter_info;

	//Vertex selected flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_inter_info, PROP_VERTEX_INTERMEDIATEINFO))
		pMesh3D->pmesh->add_property(vprop_vertex_inter_info, PROP_VERTEX_INTERMEDIATEINFO);

	std::vector<int> temp;
	std::vector<int> landmarkarray;

	temp.clear();
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_inter_info, v_it.handle()) = temp;
	}
	//initialize the vertex - landmark connection
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
		//for (int i = 0; i < pMesh3D->m_SelectedLandmarkIndexArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		//if (pLandMark->style != 1 - ii)
		//	continue;

		landmarkarray.push_back(i);

		int startindex = pLandMark->landmark_points[0];
		int endindex = pLandMark->landmark_points[pLandMark->landmark_points.size() - 1];

		//
		temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(startindex));
		temp.push_back(i);
		pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(startindex)) = temp;

		temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(endindex));
		temp.push_back(i);
		pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(endindex)) = temp;
	}

	std::vector<int> larray;
	//
	while (landmarkarray.size() > 0)
	{
		larray.clear();
		larray.push_back(landmarkarray[0]);
		landmarkarray.erase(landmarkarray.begin());
		int currentlandmarkindex = larray[0];

		//along this direction
		GeoMapLandMark* pCurrentLandMark = pMesh3D->m_LandMarkArray[currentlandmarkindex];
		int start = pCurrentLandMark->landmark_points[0];
		temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(start));

		while (temp.size() == 2)
		{
			if (temp[0] == currentlandmarkindex)
				currentlandmarkindex = temp[1];
			else
				currentlandmarkindex = temp[0];

			//
			if (std::find(larray.begin(), larray.end(), currentlandmarkindex) != larray.end())
				break;

			GeoMapLandMark* pNewLandMark = pMesh3D->m_LandMarkArray[currentlandmarkindex];

			if (pCurrentLandMark->style != pNewLandMark->style)
				break;

			larray.push_back(currentlandmarkindex);
			landmarkarray.erase(std::find(landmarkarray.begin(), landmarkarray.end(), currentlandmarkindex));

			int newstart = pNewLandMark->landmark_points[0];
			int newend = pNewLandMark->landmark_points[pNewLandMark->landmark_points.size() - 1];

			if (newstart == start)
				start = newend;
			else
				start = newstart;

			temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(start));
		}

		currentlandmarkindex = larray[0];
		//along another direction
		int end = pCurrentLandMark->landmark_points[pCurrentLandMark->landmark_points.size() - 1];
		temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(end));
		while (temp.size() == 2)
		{
			if (temp[0] == currentlandmarkindex)
				currentlandmarkindex = temp[1];
			else
				currentlandmarkindex = temp[0];

			GeoMapLandMark* pNewLandMark = pMesh3D->m_LandMarkArray[currentlandmarkindex];

			//
			if (std::find(larray.begin(), larray.end(), currentlandmarkindex) != larray.end())
				break;

			//
			if (pCurrentLandMark->style != pNewLandMark->style)
				break;

			larray.push_back(currentlandmarkindex);
			landmarkarray.erase(std::find(landmarkarray.begin(), landmarkarray.end(), currentlandmarkindex));

			int newstart = pNewLandMark->landmark_points[0];
			int newend = pNewLandMark->landmark_points[pNewLandMark->landmark_points.size() - 1];

			if (newstart == end)
				end = newend;
			else
				end = newstart;

			temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(end));
		}
		m_ArrayLandmarks.push_back(larray);
	}
}
void Actions_HarmonicMapping::RemoveLandmarksWithFixedVerticesandSetCorrespondingUVValue(CMesh3d_OpenMesh* pMesh3D, std::vector<std::vector<int>> &m_ArrayLandmarks, int ii)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	int vindex;
	std::vector<double> uv,uv1;
	uv.resize(2);
	double u, v;
	for (int k = 0; k < pMesh3D->m_FixVerticesArray_Inner.size(); k++)
	{
		vindex = pMesh3D->m_FixVerticesArray_Inner[k];
		u = pMesh3D->m_FixVerticesArray_Inner_uv[k * 2];
		v = pMesh3D->m_FixVerticesArray_Inner_uv[k * 2 + 1];
		for (int i = 0; i < m_ArrayLandmarks.size(); i++)
			for (int j = 0; j < m_ArrayLandmarks[i].size(); j++)
			{
				GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[m_ArrayLandmarks[i][j]];
				int flag = pLandMark->IsPointOn(vindex);
				if (flag)
				{
					for (int kk = 0; kk < m_ArrayLandmarks[i].size(); kk++)
					{
						GeoMapLandMark* pLandMark1 = pMesh3D->m_LandMarkArray[m_ArrayLandmarks[i][kk]];
						for (int kk1 = 0; kk1 < pLandMark1->landmark_points.size(); kk1++)
						{
							int vvindex = pLandMark1->landmark_points[kk1];
							if (vvindex == vindex)
								continue;

							pMesh3D->m_FixVerticesArray_Inner_Landmark.push_back(vvindex);

							uv1 = pMesh3D->pmesh->property(vprop_vertex_uv, MyMesh::VertexHandle(vvindex));
							if (ii == 1)
								uv[0] = uv1[0];

							if (ii == 0)
								uv[0] = u;
							else
								uv[1] = v;

							pMesh3D->pmesh->property(vprop_vertex_uv, MyMesh::VertexHandle(pLandMark1->landmark_points[kk1])) = uv;
						}
					}
					m_ArrayLandmarks.erase(m_ArrayLandmarks.begin() + i);
					//i--;
					goto AAA;
					//return;
				}
			}
	AAA:;
	}
}
void Actions_HarmonicMapping::ObtainConnectedLandmarks(CMesh3d_OpenMesh* pMesh3D, std::vector<std::vector<int>> &m_ArrayLandmarks, int ii)
{
	VPROP_VERTEX_INTERINFO vprop_vertex_inter_info;

	//Vertex selected flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_inter_info, PROP_VERTEX_INTERMEDIATEINFO))
		pMesh3D->pmesh->add_property(vprop_vertex_inter_info, PROP_VERTEX_INTERMEDIATEINFO);

	std::vector<int> temp;
	std::vector<int> landmarkarray;

	temp.clear();
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_inter_info, v_it.handle()) = temp;
	}
	//initialize the vertex - landmark connection
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	//for (int i = 0; i < pMesh3D->m_SelectedLandmarkIndexArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		if (pLandMark->style != 1 - ii)
			continue;

		landmarkarray.push_back(i);

		int startindex = pLandMark->landmark_points[0];
		int endindex = pLandMark->landmark_points[pLandMark->landmark_points.size() - 1];

		//
		temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(startindex));
		temp.push_back(i);
		pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(startindex)) = temp;

		temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(endindex));
		temp.push_back(i);
		pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(endindex)) = temp;
	}

	std::vector<int> larray;
	//
	while (landmarkarray.size() > 0)
	{
		larray.clear();
		larray.push_back(landmarkarray[0]);
		landmarkarray.erase(landmarkarray.begin());
		int currentlandmarkindex = larray[0];

		//along this direction
		GeoMapLandMark* pCurrentLandMark = pMesh3D->m_LandMarkArray[currentlandmarkindex];
		int start = pCurrentLandMark->landmark_points[0];
		temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(start));

		while (temp.size() == 2)
		{
			if (temp[0] == currentlandmarkindex)
				currentlandmarkindex = temp[1];
			else
				currentlandmarkindex = temp[0];

			//
			if (std::find(larray.begin(), larray.end(), currentlandmarkindex) != larray.end())
				break;

			larray.push_back(currentlandmarkindex);
			landmarkarray.erase(std::find(landmarkarray.begin(), landmarkarray.end(), currentlandmarkindex));

			GeoMapLandMark* pNewLandMark = pMesh3D->m_LandMarkArray[currentlandmarkindex];
			int newstart = pNewLandMark->landmark_points[0];
			int newend = pNewLandMark->landmark_points[pNewLandMark->landmark_points.size() - 1];

			if (newstart == start)
				start = newend;
			else
				start = newstart;

			temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(start));
		}

		currentlandmarkindex = larray[0];
		//along another direction
		int end = pCurrentLandMark->landmark_points[pCurrentLandMark->landmark_points.size() - 1];
		temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(end));
		while (temp.size() == 2)
		{
			if (temp[0] == currentlandmarkindex)
				currentlandmarkindex = temp[1];
			else
				currentlandmarkindex = temp[0];

			//
			if (std::find(larray.begin(), larray.end(), currentlandmarkindex) != larray.end())
				break;

			larray.push_back(currentlandmarkindex);
			landmarkarray.erase(std::find(landmarkarray.begin(), landmarkarray.end(), currentlandmarkindex));
			GeoMapLandMark* pNewLandMark = pMesh3D->m_LandMarkArray[currentlandmarkindex];
			int newstart = pNewLandMark->landmark_points[0];
			int newend = pNewLandMark->landmark_points[pNewLandMark->landmark_points.size() - 1];

			if (newstart == end)
				end = newend;
			else
				end = newstart;

			temp = pMesh3D->pmesh->property(vprop_vertex_inter_info, pMesh3D->pmesh->vertex_handle(end));
		}
		m_ArrayLandmarks.push_back(larray);
	}
}
void Actions_HarmonicMapping::Compute_Linear_Equations_LinearConstraints_SingleVariable_Selected(CMesh3d_OpenMesh* pMesh3D, int muflag)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	VPROP_VERTEX_MU vprop_vertex_mu;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected_flag;
	VPROP_VERTEX_INTERINFO vprop_vertex_inter_info;

	//Vertex selected flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_inter_info, PROP_VERTEX_INTERMEDIATEINFO))
		pMesh3D->pmesh->add_property(vprop_vertex_inter_info, PROP_VERTEX_INTERMEDIATEINFO);

	//Vertex selected flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Vertex nonboundary index;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Edge laplace weight;
	if (!pMesh3D->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pMesh3D->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	//Compute Edge Weight
	Compute_Linear_Equations__LinearConstraints_ComputeEdgeWeight(pMesh3D, muflag);

	int landmarknum;
	int landmarkpointnum;
	int interiorpointnum;
	std::vector<std::vector<double>> uvarray;
	std::vector<std::vector<int>> m_ArrayLandmarks;
	uvarray.resize(2);

	for (int ii = 0; ii < 2; ii++)
	{
		//clear
		m_ArrayLandmarks.clear();

		pMesh3D->m_FixVerticesArray_Inner_Landmark.clear();

		//First Obtain Connected Landmarks
		ObtainConnectedLandmarks(pMesh3D, m_ArrayLandmarks, ii);

		//Remove the landmarks with fixed vertices and set the corresponding value
		RemoveLandmarksWithFixedVerticesandSetCorrespondingUVValue(pMesh3D, m_ArrayLandmarks, ii);

		//initialize the vertex selected flag
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			pMesh3D->pmesh->property(vprop_vertex_selected_flag, v_it.handle()) = -1;
		}

		//first label the vertices on the vertical landmark
		landmarknum = 0;
		landmarkpointnum = 0;
		interiorpointnum = 0;

		for (int i = 0; i < m_ArrayLandmarks.size(); i++)
		{
			for (int j = 0; j < m_ArrayLandmarks[i].size(); j++)
			{
				GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[m_ArrayLandmarks[i][j]];
				landmarkpointnum += pLandMark->landmark_points.size();
				//
				for (int j = 0; j < pLandMark->landmark_points.size(); j++)
				{
					int index = pLandMark->landmark_points[j];
					OpenMesh::VertexHandle vh = pMesh3D->pmesh->vertex_handle(index);
					pMesh3D->pmesh->property(vprop_vertex_selected_flag, vh) = i;
				}
			}
			landmarkpointnum -= m_ArrayLandmarks[i].size() - 1;
		}

		landmarknum = m_ArrayLandmarks.size();

		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			//int flag = pMesh3D->pmesh->is_boundary(v_it.handle());
			int flag = IsFixed(pMesh3D, v_it.handle());
			if (flag)
				continue;
			interiorpointnum++;
		}
		//interiorpointnum - landmarkpointnum + landmarknum
		//Set the vertice a non-boundary index 
		int m_interior_vertices_index = 0;
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			//int flag = pMesh3D->pmesh->is_boundary(v_it.handle());
			int flag = IsFixed(pMesh3D, v_it.handle());
			if (flag)
			{
				pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = -1;
				continue;
			}

			int landmarkindex = pMesh3D->pmesh->property(vprop_vertex_selected_flag, v_it.handle());
			if (landmarkindex > -1)
			{
				pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = interiorpointnum - landmarkpointnum + landmarkindex;
				continue;
			}

			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = m_interior_vertices_index;
			m_interior_vertices_index++;
		}

		//Add the elements to the matrix

		MySparseMatrix A, Acon;
		std::vector<double> b, bcon;
		//For each interior vertex we have a separate row
		//
		A.m = interiorpointnum - landmarkpointnum + landmarknum;
		A.n = interiorpointnum - landmarkpointnum + landmarknum;
		b.clear();
		A.Clear();

		int rowindex = 0;
		b.resize(interiorpointnum - landmarkpointnum + landmarknum, 0);
		//all the linear equations
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			Compute_Linear_Equations__LinearConstraints_PerVertex_SingleVariable(pMesh3D, v_it.handle(), rowindex, A, ii, b);
		}
		Acon.n = A.n;
		Acon.m = 0;
		bcon.clear();
		//Compute_Constaints(pMesh3D, Acon, bcon, ii);

		if (b.size() > 0)
		{
			//std::vector<double> temp;
			if (bcon.size() > 0)
				Action_PolyOmino::SolveSpareseLinearSystem_matlab_quadprog(&A, b, &Acon, bcon, uvarray[ii]);
			else
				Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A, b, uvarray[ii]);
		}

		//set the interior parameters
		std::vector<double> uvvalues, uvvalues1;
		uvvalues.resize(2);
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			//int boundaryflag = pMesh3D->pmesh->is_boundary(v_it.handle());
			int boundaryflag = IsFixed(pMesh3D, v_it.handle());

			if (boundaryflag)
				continue;

			int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

			uvvalues1 = pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle());
			if (ii == 1)
				uvvalues[0] = uvvalues1[0];

			uvvalues[ii] = uvarray[ii][v0_index];
			//uvvalues[1] = uvarray[1][v0_index];

			pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
		}
	}
}
void Actions_HarmonicMapping::Compute_Linear_Equations_LinearConstraints_SingleVariable(CMesh3d_OpenMesh* pMesh3D, int muflag)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	VPROP_VERTEX_MU vprop_vertex_mu;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected_flag;
	VPROP_VERTEX_INTERINFO vprop_vertex_inter_info;

	//Vertex selected flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_inter_info, PROP_VERTEX_INTERMEDIATEINFO))
		pMesh3D->pmesh->add_property(vprop_vertex_inter_info, PROP_VERTEX_INTERMEDIATEINFO);

	//Vertex selected flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Vertex nonboundary index;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Edge laplace weight;
	if (!pMesh3D->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pMesh3D->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	//Compute Edge Weight
	Compute_Linear_Equations__LinearConstraints_ComputeEdgeWeight(pMesh3D, muflag);

	int landmarknum;
	int landmarkpointnum;
	int interiorpointnum;
	std::vector<std::vector<double>> uvarray;
	std::vector<std::vector<int>> m_ArrayLandmarks;
	uvarray.resize(2);

	for (int ii = 0; ii < 2; ii++)
	{
		//clear
		m_ArrayLandmarks.clear();

		//First Obtain Connected Landmarks
		ObtainConnectedLandmarks(pMesh3D, m_ArrayLandmarks, ii);

		//initialize the vertex selected flag
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			pMesh3D->pmesh->property(vprop_vertex_selected_flag, v_it.handle()) = -1;
		}

		//first label the vertices on the vertical landmark
		landmarknum = 0;
		landmarkpointnum = 0;
		interiorpointnum = 0;

		for (int i = 0; i < m_ArrayLandmarks.size(); i++)
		{
			for (int j = 0; j < m_ArrayLandmarks[i].size(); j++)
			{
				GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[m_ArrayLandmarks[i][j]];
				landmarkpointnum += pLandMark->landmark_points.size();
				//
				for (int j = 0; j < pLandMark->landmark_points.size(); j++)
				{
					int index = pLandMark->landmark_points[j];
					OpenMesh::VertexHandle vh = pMesh3D->pmesh->vertex_handle(index);
					pMesh3D->pmesh->property(vprop_vertex_selected_flag, vh) = i;
				}
			}
			landmarkpointnum -= m_ArrayLandmarks[i].size() - 1;
		}

		landmarknum = m_ArrayLandmarks.size();

		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			//int flag = pMesh3D->pmesh->is_boundary(v_it.handle());
			int flag = IsFixed(pMesh3D, v_it.handle());
			if (flag)
				continue;
			interiorpointnum++;
		}
		//interiorpointnum - landmarkpointnum + landmarknum
		//Set the vertice a non-boundary index 
		int m_interior_vertices_index = 0;
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			//int flag = pMesh3D->pmesh->is_boundary(v_it.handle());
			int flag = IsFixed(pMesh3D, v_it.handle());
			if (flag)
			{
				pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = -1;
				continue;
			}

			int landmarkindex = pMesh3D->pmesh->property(vprop_vertex_selected_flag, v_it.handle());
			if (landmarkindex > -1)
			{
				pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = interiorpointnum - landmarkpointnum + landmarkindex;
				continue;
			}

			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = m_interior_vertices_index;
			m_interior_vertices_index++;
		}

		//Add the elements to the matrix

		MySparseMatrix A, Acon;
		std::vector<double> b, bcon;
		//For each interior vertex we have a separate row
		//
		A.m = interiorpointnum - landmarkpointnum + landmarknum;
		A.n = interiorpointnum - landmarkpointnum + landmarknum;
		b.clear();
		A.Clear();

		int rowindex = 0;
		b.resize(interiorpointnum - landmarkpointnum + landmarknum, 0);
		//all the linear equations
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			Compute_Linear_Equations__LinearConstraints_PerVertex_SingleVariable(pMesh3D, v_it.handle(), rowindex, A, ii, b);
		}
		Acon.n = A.n;
		Acon.m = 0;
		bcon.clear();
		Compute_Constaints(pMesh3D, Acon, bcon, ii);

		if (b.size() > 0)
		{
			//std::vector<double> temp;
			if (bcon.size() > 0)
				Action_PolyOmino::SolveSpareseLinearSystem_matlab_quadprog(&A, b, &Acon, bcon, uvarray[ii]);
			else
				Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A, b, uvarray[ii]);
		}

		//set the interior parameters
		std::vector<double> uvvalues, uvvalues1;
		uvvalues.resize(2);
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			//int boundaryflag = pMesh3D->pmesh->is_boundary(v_it.handle());
			int boundaryflag = IsFixed(pMesh3D, v_it.handle());

			if (boundaryflag)
				continue;

			int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

			uvvalues1 = pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle());
			if (ii == 1)
				uvvalues[0] = uvvalues1[0];

			uvvalues[ii] = uvarray[ii][v0_index];
			//uvvalues[1] = uvarray[1][v0_index];

			pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
		}
	}
}
void Actions_HarmonicMapping::Compute_Linear_Equations_LinearConstraints(CMesh3d_OpenMesh* pMesh3D, int muflag)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	VPROP_VERTEX_MU vprop_vertex_mu;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_selected_flag;

	//Vertex selected flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_selected_flag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Vertex nonboundary index;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Edge laplace weight;
	if (!pMesh3D->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pMesh3D->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	//initialize the vertex selected flag
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_selected_flag, v_it.handle()) = 0;
	}

	//Set the vertice a non-boundary index 
	int m_interior_vertices_num = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->is_boundary(v_it.handle());
		if (flag)
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = -1;
		}
		else
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = m_interior_vertices_num;
			m_interior_vertices_num++;
		}
	}
	std::vector<std::pair<int, int>> m_HorizontalConstraints, m_VerticalConstraints;
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int index1 = pLandMark->landmark_points[j];
			int index2 = pLandMark->landmark_points[j + 1];

			int nonboundaryindex1 = pMesh3D->pmesh->property(vprop_nonboundaryindex, pMesh3D->pmesh->vertex_handle(index1));
			int nonboundaryindex2 = pMesh3D->pmesh->property(vprop_nonboundaryindex, pMesh3D->pmesh->vertex_handle(index2));

			pair.first = nonboundaryindex1;
			pair.second = nonboundaryindex2;

			if (pLandMark->style == 0)
				m_HorizontalConstraints.push_back(pair);
			else
				m_VerticalConstraints.push_back(pair);
		}
	}
	std::vector<MyMesh::VertexHandle> m_ArrayOneRingVertices;
	std::vector<double> omegaarray;
	std::vector<double> equation;
	//std::vector<std::vector<double>> A;
	//alglib::sparsematrix ma;
	MySparseMatrix A, Acon;
	//alglib::sparsecreate(m_interior_vertices_num, m_interior_vertices_num, ma);
	std::vector<double> b, bcon;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	OpenMesh::Vec3d v0, v_i, v_iminus1, v_iadd1;

	//sprintf(Post_Message, "Computing Edge Weight...");
	//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

	//
	Compute_Linear_Equations__LinearConstraints_ComputeEdgeWeight(pMesh3D, muflag);

	int rowindex;
	double bval;

	for (int ii = 0; ii < 2; ii++)
	{
		if (ii == 0)
		{
			Acon.m = A.m = m_interior_vertices_num + m_VerticalConstraints.size();
			Acon.n = A.n = m_interior_vertices_num + m_VerticalConstraints.size();
		}
		else
		{
			Acon.m = A.m = m_interior_vertices_num + m_HorizontalConstraints.size();
			Acon.n = A.n = m_interior_vertices_num + m_HorizontalConstraints.size();
		}
		b.clear();
		A.Clear();
		Acon.Clear();

		rowindex = 0;

		//all the linear equations
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			//
			//sprintf(Post_Message, "Constructing Linear Equations%d...", v_it.handle().idx());
			//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

			//
			Compute_Linear_Equations__LinearConstraints_PerVertex(pMesh3D, v_it.handle(), rowindex, A, bval, ii, m_VerticalConstraints, m_HorizontalConstraints, m_interior_vertices_num, b);
		}

		Compute_Linear_Equations__LinearConstraints_Constraints(A, b, rowindex, ii, m_VerticalConstraints, m_HorizontalConstraints);

		//
		//Acon.m = 0;
		//bcon.clear();
		Compute_Constaints(pMesh3D, Acon, bcon, ii);
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
		if (b.size() > 0)
		{
			//std::vector<double> temp;
			//Action_PolyOmino::SolveSpareseLinearSystem_matlab_quadprog(&A, b, &Acon, bcon, uvarray[ii]);
			Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A, b, uvarray[ii]);
		}

	}

	//set the interior parameters
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int boundaryflag = pMesh3D->pmesh->is_boundary(v_it.handle());

		if (boundaryflag)
			continue;

		int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

		uvvalues[0] = uvarray[0][v0_index];
		uvvalues[1] = uvarray[1][v0_index];

		pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
	}
}
void Actions_HarmonicMapping::Compute_Linear_Equations__LinearConstraints_Constraints(MySparseMatrix &A, std::vector<double> &b, int &rowindex, int ii, std::vector<std::pair<int, int>> &m_VerticalConstraints, std::vector<std::pair<int, int>> &m_HorizontalConstraints)
{
	std::pair<int, int> pair;
	//
	if (ii == 0)
	{
		for (int ij = 0; ij < m_VerticalConstraints.size(); ij++)
		{
			pair = m_VerticalConstraints[ij];
			//A.IncreaseItem(rowindex,pair.first,1);
			//A.IncreaseItem(rowindex,pair.second,-1);
			A.AddItem(rowindex, pair.first, 1);
			A.AddItem(rowindex, pair.second, -1);
			rowindex++;
			b.push_back(0);
		}
	}
	else
	{
		for (int ij = 0; ij < m_HorizontalConstraints.size(); ij++)
		{
			pair = m_HorizontalConstraints[ij];
			//A.IncreaseItem(rowindex,pair.first,1);
			//A.IncreaseItem(rowindex,pair.second,-1);
			A.AddItem(rowindex, pair.first, 1);
			A.AddItem(rowindex, pair.second, -1);
			rowindex++;
			b.push_back(0);
		}
	}
}
BOOL Actions_HarmonicMapping::IsFixed(CMesh3d_OpenMesh* pMesh3D, MyMesh::VertexHandle vh)
{
	int vindex = vh.idx();
	if (pMesh3D->pmesh->is_boundary(vh))
		return TRUE;
	//else
	//	return FALSE;

	if (std::find(pMesh3D->m_FixVerticesArray_Inner.begin(), pMesh3D->m_FixVerticesArray_Inner.end(), vh.idx()) != pMesh3D->m_FixVerticesArray_Inner.end())
		return TRUE;
	//else
	//	return FALSE;

	if (std::find(pMesh3D->m_FixVerticesArray_Inner_Landmark.begin(), pMesh3D->m_FixVerticesArray_Inner_Landmark.end(), vh.idx()) != pMesh3D->m_FixVerticesArray_Inner_Landmark.end())
			return TRUE;
	else
		return FALSE;

	//if (std::find(m_ArrayBoundarylandmarkpoints.begin(), m_ArrayBoundarylandmarkpoints.end(), vh.idx()) != m_ArrayBoundarylandmarkpoints.end())
	//	return TRUE;
	//else
	//	return FALSE;
}
BOOL Actions_HarmonicMapping::IsFixed(CMesh3d_OpenMesh* pMesh3D, MyMesh::EdgeHandle eh)
{
	if (pMesh3D->pmesh->is_boundary(eh))
		return TRUE;

	/*MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
	MyMesh::VertexHandle vh1 = pMesh3D->pmesh->to_vertex_handle(hh0);
	MyMesh::VertexHandle vh2 = pMesh3D->pmesh->from_vertex_handle(hh0);
	if (IsFixed(pMesh3D, vh1) && IsFixed(pMesh3D, vh2))
		return TRUE;*/
	return FALSE;
}
void Actions_HarmonicMapping::Compute_Linear_Equations__LinearConstraints_PerVertex_SingleVariable(CMesh3d_OpenMesh* pMesh3D, OpenMesh::VertexHandle vh, int &rowindex, MySparseMatrix &A, int ii, std::vector<double> &b)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	VPROP_VERTEX_MU vprop_vertex_mu;

	if (vh.idx() == 19499)
	{
		int ij = 0;
	}

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pMesh3D->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	std::vector<std::pair<int, double>> m_ArrayElements;
	OpenMesh::Vec3d v0;
	std::pair<int, int> pair;

	double bval = 0;

	//Get the position
	v0 = pMesh3D->pmesh->point(vh);
	int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh);

	//int boundaryflag = pMesh3D->pmesh->is_boundary(vh);
	int boundaryflag = IsFixed(pMesh3D, vh);

	if (boundaryflag || v0_index == -1)
		return;

	rowindex = v0_index;
	for (MyMesh::VertexEdgeIter ve_it = pMesh3D->pmesh->ve_iter(vh); ve_it; ++ve_it)
	{
		MyMesh::EdgeHandle eh = ve_it.handle();
		//int edgeflag = pMesh3D->pmesh->is_boundary(eh);
		int edgeflag = IsFixed(pMesh3D, eh);
		if (edgeflag)
			continue;
		MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->from_vertex_handle(hh0);

		//
		int derivativeflag = 1;
		if (vh.idx() == vh2.idx())
			derivativeflag = -1;

		double w = pMesh3D->pmesh->property(eprop_laplaceweight, eh);
		w = w * derivativeflag;

		int vh1_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh1);
		int vh2_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh2);

		int vidx1 = vh1.idx();
		int vidx2 = vh2.idx();
		if (vidx1 == 3068 && vidx2 == 3101)
		{
			int ij = 0;
		}
		//int vh1_flag = pMesh3D->pmesh->is_boundary(vh1);
		//int vh2_flag = pMesh3D->pmesh->is_boundary(vh2);
		int vh1_flag = IsFixed(pMesh3D, vh1);
		int vh2_flag = IsFixed(pMesh3D, vh2);

		std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_uv, vh1);
		std::vector<double> uv2 = pMesh3D->pmesh->property(vprop_vertex_uv, vh2);

		if (vh1_flag)
		{
			bval += -w * 2 * uv1[ii];
		}
		else
		{
			//IncreaseItem(m_ArrayElements, vh1_index, w * 2);
			A.IncreaseItem(rowindex, vh1_index, w * 2);
		}
		if (vh2_flag)
			bval += w * 2 * uv2[ii];
		else
		{
			//IncreaseItem(m_ArrayElements, vh2_index, -w * 2);
			A.IncreaseItem(rowindex, vh2_index, -w * 2);
		}
	}


	//for (int i = 0; i < m_ArrayElements.size(); i++)
	//{
	//	A.AddItem(rowindex, m_ArrayElements[i].first, m_ArrayElements[i].second);
	//}

	//rowindex++;
	b[rowindex] += bval;
}
void Actions_HarmonicMapping::Compute_Linear_Equations__LinearConstraints_PerVertex(CMesh3d_OpenMesh* pMesh3D, OpenMesh::VertexHandle vh, int &rowindex, MySparseMatrix &A, double &bval, int ii, std::vector<std::pair<int, int>> &m_VerticalConstraints, std::vector<std::pair<int, int>> &m_HorizontalConstraints, int m_interior_vertices_num, std::vector<double> &b)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	EPROP_EDGE_LAPLACE_WEIGHT eprop_laplaceweight;
	VPROP_VERTEX_MU vprop_vertex_mu;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT))
		pMesh3D->pmesh->add_property(eprop_laplaceweight, PROP_EDGE_LAPLACE_WEIGHT);

	std::vector<std::pair<int, double>> m_ArrayElements;
	OpenMesh::Vec3d v0;
	std::pair<int, int> pair;

	bval = 0;

	//Get the position
	v0 = pMesh3D->pmesh->point(vh);
	int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh);

	int boundaryflag = pMesh3D->pmesh->is_boundary(vh);

	if (boundaryflag)
		return;

	for (MyMesh::VertexEdgeIter ve_it = pMesh3D->pmesh->ve_iter(vh); ve_it; ++ve_it)
	{
		MyMesh::EdgeHandle eh = ve_it.handle();
		int edgeflag = pMesh3D->pmesh->is_boundary(eh);
		if (edgeflag)
			continue;
		MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->to_vertex_handle(hh0);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->from_vertex_handle(hh0);

		//
		int derivativeflag = 1;
		if (vh.idx() == vh2.idx())
			derivativeflag = -1;

		double w = pMesh3D->pmesh->property(eprop_laplaceweight, eh);
		w = w * derivativeflag;

		int vh1_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh1);
		int vh2_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh2);

		int vh1_flag = pMesh3D->pmesh->is_boundary(vh1);
		int vh2_flag = pMesh3D->pmesh->is_boundary(vh2);

		std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_uv, vh1);
		std::vector<double> uv2 = pMesh3D->pmesh->property(vprop_vertex_uv, vh2);

		if (vh1_flag)
		{
			bval += -w * 2 * uv1[ii];
		}
		else
		{
			IncreaseItem(m_ArrayElements, vh1_index, w * 2);
			//A.IncreaseItem(rowindex,vh1_index,w * 2);
		}
		if (vh2_flag)
			bval += w * 2 * uv2[ii];
		else
		{
			IncreaseItem(m_ArrayElements, vh2_index, -w * 2);
			//A.IncreaseItem(rowindex,vh2_index,-w * 2);
		}
	}

	if (ii == 0)
	{
		for (int ij = 0; ij < m_VerticalConstraints.size(); ij++)
		{
			pair = m_VerticalConstraints[ij];
			if (pair.first == v0_index)
			{
				IncreaseItem(m_ArrayElements, m_interior_vertices_num + ij, 1);
				//A.IncreaseItem(rowindex,m_interior_vertices_num + ij,1);
			}
			else if (pair.second == v0_index)
			{
				IncreaseItem(m_ArrayElements, m_interior_vertices_num + ij, -1);
				//A.IncreaseItem(rowindex,m_interior_vertices_num + ij,-1);
			}
		}
	}
	else
	{
		for (int ij = 0; ij < m_HorizontalConstraints.size(); ij++)
		{
			pair = m_HorizontalConstraints[ij];
			if (pair.first == v0_index)
			{
				IncreaseItem(m_ArrayElements, m_interior_vertices_num + ij, 1);
				//A.IncreaseItem(rowindex,m_interior_vertices_num + ij,1);
			}
			else if (pair.second == v0_index)
			{
				IncreaseItem(m_ArrayElements, m_interior_vertices_num + ij, -1);
				//A.IncreaseItem(rowindex,m_interior_vertices_num + ij,-1);
			}
		}
	}

	for (int i = 0; i < m_ArrayElements.size(); i++)
	{
		A.AddItem(rowindex, m_ArrayElements[i].first, m_ArrayElements[i].second);
	}

	rowindex++;
	b.push_back(bval);
}
void Actions_HarmonicMapping::IncreaseItem(std::vector<std::pair<int, double>> &m_ArrayElements, int index, double val)
{
	int i;
	for (i = 0; i < m_ArrayElements.size(); i++)
	{
		if (m_ArrayElements[i].first == index)
		{
			m_ArrayElements[i].second += val;
			break;
		}
	}
	if (i == m_ArrayElements.size())
	{
		m_ArrayElements.push_back(std::pair<int, double>(index, val));
	}
}
double Distance(OpenMesh::Vec3d v1, OpenMesh::Vec3d v2)
{
	double dis = sqrt(SQR(v1[0] - v2[0]) + SQR(v1[1] - v2[1]) + SQR(v1[2] - v2[2]));
	return dis;
}

void Actions_HarmonicMapping::Compute_Linear_Equations_WithStraighteningConstraints_FixEndPoints(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_INTERINFO vprop_landmarkinteriorvertex;

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Landmark Interior Vertex 
	if (!pMesh3D->pmesh->get_property_handle(vprop_landmarkinteriorvertex, PROP_VERTEX_INTERMEDIATEINFO))
		pMesh3D->pmesh->add_property(vprop_landmarkinteriorvertex, PROP_VERTEX_INTERMEDIATEINFO);

	//Set the vertice a non-boundary index 
	int m_interior_vertices_num = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->is_boundary(v_it.handle());

		//
		flag += pMesh3D->pmesh->property(vprop_selectedflag, v_it.handle());

		if (flag)
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = -1;
		}
		else
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = m_interior_vertices_num;
			m_interior_vertices_num++;
		}
	}

	//set the landmark interior vertex
	std::vector<int> adjacentvertices;
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[i];
		//
		for (int j = 1; j < pLandmark->landmark_points.size() - 1; j++)
		{
			adjacentvertices.clear();
			adjacentvertices.push_back(pLandmark->landmark_points[j - 1]);
			adjacentvertices.push_back(pLandmark->landmark_points[j + 1]);
			pMesh3D->pmesh->property(vprop_landmarkinteriorvertex, pMesh3D->pmesh->vertex_handle(pLandmark->landmark_points[j])) = adjacentvertices;
		}
	}

	std::vector<MyMesh::VertexHandle> m_ArrayOneRingVertices;
	std::vector<double> omegaarray;
	std::vector<double> equation;
	//std::vector<std::vector<double>> A;
	MySparseMatrix A;
	std::vector<double> b;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	OpenMesh::Vec3d v0, v_i, v_iminus1, v_iadd1;

	for (int ii = 0; ii < 2; ii++)
	{
		//A.clear();
		A.m = m_interior_vertices_num;
		A.n = m_interior_vertices_num;
		A.Clear();
		b.clear();
		int rowindex = 0;
		//all the linear equations
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			equation.clear();
			equation.resize(m_interior_vertices_num, 0);

			//Get the position
			v0 = pMesh3D->pmesh->point(v_it.handle());
			int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

			int boundaryflag = pMesh3D->pmesh->is_boundary(v_it.handle());

			if (v0_index < 0)
				continue;

			adjacentvertices = pMesh3D->pmesh->property(vprop_landmarkinteriorvertex, v_it.handle());
			double b1 = 0;
			if (adjacentvertices.size() == 0)
			{
				//for each interior vertex
				m_ArrayOneRingVertices.clear();

				//search the 1-ring vertices
				for (MyMesh::VertexVertexIter vv_it = pMesh3D->pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
				{
					m_ArrayOneRingVertices.push_back(vv_it.handle());
				}

				omegaarray.clear();
				double sumweight = 0;
				//compute the weight
				for (int i = 0; i < m_ArrayOneRingVertices.size(); i++)
				{
					//
					v_i = pMesh3D->pmesh->point(m_ArrayOneRingVertices[i]);
					v_iminus1 = pMesh3D->pmesh->point(m_ArrayOneRingVertices[(i - 1 + m_ArrayOneRingVertices.size()) % m_ArrayOneRingVertices.size()]);
					v_iadd1 = pMesh3D->pmesh->point(m_ArrayOneRingVertices[(i + 1) % m_ArrayOneRingVertices.size()]);

					double v0_v_iminus1 = Length(v0 - v_iminus1);
					double v0_v_i = Length(v0 - v_i);
					double v0_v_iadd1 = Length(v0 - v_iadd1);
					double vi_v_iadd1 = Length(v_i - v_iadd1);
					double vi_v_iminus1 = Length(v_i - v_iminus1);
					//double vi_v0 = Length(v_i - v0);

					double alpha_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(v0_v_i) - SQR(vi_v_iminus1)) / (2 * v0_v_iminus1 * v0_v_i));
					double alpha_i = acos((SQR(v0_v_iadd1) + SQR(v0_v_i) - SQR(vi_v_iadd1)) / (2 * v0_v_iadd1 * v0_v_i));
					double beta_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(vi_v_iminus1) - SQR(v0_v_i)) / (2 * v0_v_iminus1 * vi_v_iminus1));
					double gamai = acos((SQR(v0_v_iadd1) + SQR(vi_v_iadd1) - SQR(v0_v_i)) / (2 * v0_v_iadd1 * vi_v_iadd1));
					double w;
					if (((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_UNIFORM))->GetCheck())
					{
						w = 1;
					}
					else
						if (((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_LAPLACE))->GetCheck())
						{
							w = 1.0 / tan(beta_i_minus1) + 1.0 / tan(gamai);
						}
						else
							if (((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_MEANVALUE))->GetCheck())
							{
								w = (tan(alpha_i_minus1 / 2) + tan(alpha_i / 2)) / v0_v_i;

							}
					sumweight += w;
					omegaarray.push_back(w);
				}
				for (int i = 0; i < omegaarray.size(); i++)
				{

					omegaarray[i] = omegaarray[i] / sumweight;
				}


				for (int i = 0; i < m_ArrayOneRingVertices.size(); i++)
				{	
					//int boundaryflag = pMesh3D->pmesh->is_boundary(m_ArrayOneRingVertices[i]);
					std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrayOneRingVertices[i]);
					int index = pMesh3D->pmesh->property(vprop_nonboundaryindex, m_ArrayOneRingVertices[i]);
					if (index < 0)
					{
						b1 += omegaarray[i] * uv[ii];
					}
					else
					{
						//equation[index] = -omegaarray[i];
						A.IncreaseItem(rowindex, index, -omegaarray[i]);
					}
				}
			}
			else
			{
				//landmark interior vertices
				int index1 = adjacentvertices[0];
				int index2 = adjacentvertices[1];
				OpenMesh::VertexHandle vh1 = pMesh3D->pmesh->vertex_handle(index1);
				OpenMesh::VertexHandle vh2 = pMesh3D->pmesh->vertex_handle(index2);
				OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(vh1);
				OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(vh2);
				std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_uv, vh1);
				std::vector<double> uv2 = pMesh3D->pmesh->property(vprop_vertex_uv, vh2);
				int vindex1_nonboundary = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh1);
				int vindex2_nonboundary = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh2);
				double d1 = Distance(v0, v1);
				double d2 = Distance(v0, v2);
				double w1 = d2 / (d1 + d2);
				double w2 = 1 - w1;
				if (vindex1_nonboundary < 0)
				{
					b1 += w1 * uv1[ii];
				}
				else
				{
					A.IncreaseItem(rowindex, vindex1_nonboundary, -w1);
				}

				if (vindex2_nonboundary < 0)
				{
					b1 += w2 * uv2[ii];
				}
				else
				{
					A.IncreaseItem(rowindex, vindex2_nonboundary, -w2);
				}
			}

			A.IncreaseItem(rowindex, v0_index, 1);
			//equation[v0_index] = 1;
			//A.push_back(equation);
			b.push_back(b1);
			rowindex++;
		}
		//Debug
		/*TRACE("A   \n");
		for (int i = 0; i < A.size(); i++)
		{
		for (int j = 0; j < A[0].size(); j++)
		{
		TRACE(" %f ", A[i][j]);
		}
		TRACE("\n");
		}

		TRACE("b   \n");
		for (int i = 0; i < b.size(); i++)
		TRACE(" %f ", b[i]);*/
		//std::vector<double> test;
		//Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A, b, test);
		//Action_PolyOmino::SolveSpareseLinearSystem_matlab_FAST(&A, b, uvarray[ii]);
		Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A, b, uvarray[ii]);
		/*double tt;
		double maxtt = -1;
		for (int ii1 = 0; ii1 < test.size(); ii1++)
		{
			tt = fabs(test[ii1] - uvarray[ii][ii1]);
			if (tt > maxtt)
				maxtt = tt;
		}*/
		//p_FormView3->SolveLinearSystem(A, b, uvarray[ii]);
	}

	//set the interior parameters
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

		if (v0_index < 0)
			continue;

		uvvalues[0] = uvarray[0][v0_index];
		uvvalues[1] = uvarray[1][v0_index];

		pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
	}
}
void Actions_HarmonicMapping::Compute_Linear_Equations_WithStraighteningConstraints(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_INTERINFO vprop_landmarkinteriorvertex;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Landmark Interior Vertex 
	if (!pMesh3D->pmesh->get_property_handle(vprop_landmarkinteriorvertex, PROP_VERTEX_INTERMEDIATEINFO))
		pMesh3D->pmesh->add_property(vprop_landmarkinteriorvertex, PROP_VERTEX_INTERMEDIATEINFO);

	//Set the vertice a non-boundary index 
	int m_interior_vertices_num = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->is_boundary(v_it.handle());
		if (flag)
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = -1;
		}
		else
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = m_interior_vertices_num;
			m_interior_vertices_num++;
		}
	}

	//set the landmark interior vertex
	std::vector<int> adjacentvertices;
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[i];
		//
		for (int j = 1; j < pLandmark->landmark_points.size() - 1; j++)
		{
			adjacentvertices.clear();
			adjacentvertices.push_back(pLandmark->landmark_points[j - 1]);
			adjacentvertices.push_back(pLandmark->landmark_points[j + 1]);
			pMesh3D->pmesh->property(vprop_landmarkinteriorvertex, pMesh3D->pmesh->vertex_handle(pLandmark->landmark_points[j])) = adjacentvertices;
		}
	}

	std::vector<MyMesh::VertexHandle> m_ArrayOneRingVertices;
	std::vector<double> omegaarray;
	std::vector<double> equation;
	//std::vector<std::vector<double>> A;
	MySparseMatrix A;
	std::vector<double> b;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	OpenMesh::Vec3d v0, v_i, v_iminus1, v_iadd1;

	for (int ii = 0; ii < 2; ii++)
	{
		//A.clear();
		A.m = m_interior_vertices_num;
		A.n = m_interior_vertices_num;
		A.Clear();
		b.clear();
		int rowindex = 0;
		//all the linear equations
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			equation.clear();
			equation.resize(m_interior_vertices_num, 0);

			//Get the position
			v0 = pMesh3D->pmesh->point(v_it.handle());
			int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

			int boundaryflag = pMesh3D->pmesh->is_boundary(v_it.handle());

			if (boundaryflag)
				continue;

			adjacentvertices = pMesh3D->pmesh->property(vprop_landmarkinteriorvertex, v_it.handle());
			double b1 = 0;
			if (adjacentvertices.size() == 0)
			{
				//for each interior vertex
				m_ArrayOneRingVertices.clear();

				//search the 1-ring vertices
				for (MyMesh::VertexVertexIter vv_it = pMesh3D->pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
				{
					m_ArrayOneRingVertices.push_back(vv_it.handle());
				}

				omegaarray.clear();
				double sumweight = 0;
				//compute the weight
				for (int i = 0; i < m_ArrayOneRingVertices.size(); i++)
				{
					//
					v_i = pMesh3D->pmesh->point(m_ArrayOneRingVertices[i]);
					v_iminus1 = pMesh3D->pmesh->point(m_ArrayOneRingVertices[(i - 1 + m_ArrayOneRingVertices.size()) % m_ArrayOneRingVertices.size()]);
					v_iadd1 = pMesh3D->pmesh->point(m_ArrayOneRingVertices[(i + 1) % m_ArrayOneRingVertices.size()]);

					double v0_v_iminus1 = Length(v0 - v_iminus1);
					double v0_v_i = Length(v0 - v_i);
					double v0_v_iadd1 = Length(v0 - v_iadd1);
					double vi_v_iadd1 = Length(v_i - v_iadd1);
					double vi_v_iminus1 = Length(v_i - v_iminus1);
					//double vi_v0 = Length(v_i - v0);

					double alpha_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(v0_v_i) - SQR(vi_v_iminus1)) / (2 * v0_v_iminus1 * v0_v_i));
					double alpha_i = acos((SQR(v0_v_iadd1) + SQR(v0_v_i) - SQR(vi_v_iadd1)) / (2 * v0_v_iadd1 * v0_v_i));
					double beta_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(vi_v_iminus1) - SQR(v0_v_i)) / (2 * v0_v_iminus1 * vi_v_iminus1));
					double gamai = acos((SQR(v0_v_iadd1) + SQR(vi_v_iadd1) - SQR(v0_v_i)) / (2 * v0_v_iadd1 * vi_v_iadd1));
					double w;
					if (((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_UNIFORM))->GetCheck())
					{
						w = 1;
					}
					else
						if (((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_LAPLACE))->GetCheck())
						{
							w = 1.0 / tan(beta_i_minus1) + 1.0 / tan(gamai);
						}
						else
							if (((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_MEANVALUE))->GetCheck())
							{
								w = (tan(alpha_i_minus1 / 2) + tan(alpha_i / 2)) / v0_v_i;

							}
					sumweight += w;
					omegaarray.push_back(w);
				}
				for (int i = 0; i < omegaarray.size(); i++)
				{

					omegaarray[i] = omegaarray[i] / sumweight;
				}

				
				for (int i = 0; i < m_ArrayOneRingVertices.size(); i++)
				{
					int boundaryflag = pMesh3D->pmesh->is_boundary(m_ArrayOneRingVertices[i]);
					std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrayOneRingVertices[i]);
					int index = pMesh3D->pmesh->property(vprop_nonboundaryindex, m_ArrayOneRingVertices[i]);
					if (boundaryflag)
					{
						if(uv.size() > 0)
							b1 += omegaarray[i] * uv[ii];
					}
					else
					{
						//equation[index] = -omegaarray[i];
						A.IncreaseItem(rowindex, index, -omegaarray[i]);
					}
				}
			}
			else
			{
				//landmark interior vertices
				int index1 = adjacentvertices[0];
				int index2 = adjacentvertices[1];
				OpenMesh::VertexHandle vh1 = pMesh3D->pmesh->vertex_handle(index1);
				OpenMesh::VertexHandle vh2 = pMesh3D->pmesh->vertex_handle(index2);
				OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(vh1);
				OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(vh2);
				std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_uv, vh1);
				std::vector<double> uv2 = pMesh3D->pmesh->property(vprop_vertex_uv, vh2);
				int vindex1_nonboundary = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh1);
				int vindex2_nonboundary = pMesh3D->pmesh->property(vprop_nonboundaryindex, vh2);
				double d1 = Distance(v0, v1);
				double d2 = Distance(v0, v2);
				double w1 = d2 / (d1 + d2);
				double w2 = 1 - w1;
				if (vindex1_nonboundary < 0)
				{
					b1 += w1 * uv1[ii];
				}else
					A.IncreaseItem(rowindex, vindex1_nonboundary, -w1);
				if (vindex2_nonboundary < 0)
				{
					b1 += w2 * uv2[ii];
				}
				else
					A.IncreaseItem(rowindex, vindex2_nonboundary, -w2);
			}

			A.IncreaseItem(rowindex, v0_index, 1);
			//equation[v0_index] = 1;
			//A.push_back(equation);
			b.push_back(b1);
			rowindex++;
		}
		//Debug
		/*TRACE("A   \n");
		for (int i = 0; i < A.size(); i++)
		{
		for (int j = 0; j < A[0].size(); j++)
		{
		TRACE(" %f ", A[i][j]);
		}
		TRACE("\n");
		}

		TRACE("b   \n");
		for (int i = 0; i < b.size(); i++)
		TRACE(" %f ", b[i]);*/
		Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A, b, uvarray[ii]);
		//p_FormView3->SolveLinearSystem(A, b, uvarray[ii]);
 	}

	//set the interior parameters
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int boundaryflag = pMesh3D->pmesh->is_boundary(v_it.handle());

		if (boundaryflag)
			continue;

		int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

		uvvalues[0] = uvarray[0][v0_index];
		uvvalues[1] = uvarray[1][v0_index];

		pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
	}
}

void Actions_HarmonicMapping::Compute_Linear_Equations(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Set the vertice a non-boundary index 
	int m_interior_vertices_num = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int flag = pMesh3D->pmesh->is_boundary(v_it.handle());
		if (flag)
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = -1;
		}
		else
		{
			pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle()) = m_interior_vertices_num;
			m_interior_vertices_num++;
		}
	}

	std::vector<MyMesh::VertexHandle> m_ArrayOneRingVertices;
	std::vector<double> omegaarray;
	std::vector<double> equation;
	//std::vector<std::vector<double>> A;
	MySparseMatrix A;
	std::vector<double> b;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	OpenMesh::Vec3d v0, v_i, v_iminus1, v_iadd1;

	for (int ii = 0; ii < 2; ii++)
	{
		//A.clear();
		A.m = m_interior_vertices_num;
		A.n = m_interior_vertices_num;
		A.Clear();
		b.clear();
		int rowindex = 0;
		//all the linear equations
		for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
		{
			equation.clear();
			equation.resize(m_interior_vertices_num, 0);

			//Get the position
			v0 = pMesh3D->pmesh->point(v_it.handle());
			int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

			int boundaryflag = pMesh3D->pmesh->is_boundary(v_it.handle());

			if (boundaryflag)
				continue;

			//for each interior vertex
			m_ArrayOneRingVertices.clear();

			//search the 1-ring vertices
			for (MyMesh::VertexVertexIter vv_it = pMesh3D->pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
			{
				m_ArrayOneRingVertices.push_back(vv_it.handle());
			}

			omegaarray.clear();
			double sumweight = 0;
			//compute the weight
			for (int i = 0; i < m_ArrayOneRingVertices.size(); i++)
			{
				//
				v_i = pMesh3D->pmesh->point(m_ArrayOneRingVertices[i]);
				v_iminus1 = pMesh3D->pmesh->point(m_ArrayOneRingVertices[(i - 1 + m_ArrayOneRingVertices.size()) % m_ArrayOneRingVertices.size()]);
				v_iadd1 = pMesh3D->pmesh->point(m_ArrayOneRingVertices[(i + 1) % m_ArrayOneRingVertices.size()]);

				double v0_v_iminus1 = Length(v0 - v_iminus1);
				double v0_v_i = Length(v0 - v_i);
				double v0_v_iadd1 = Length(v0 - v_iadd1);
				double vi_v_iadd1 = Length(v_i - v_iadd1);
				double vi_v_iminus1 = Length(v_i - v_iminus1);
				//double vi_v0 = Length(v_i - v0);

				double alpha_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(v0_v_i) - SQR(vi_v_iminus1)) / (2 * v0_v_iminus1 * v0_v_i));
				double alpha_i = acos((SQR(v0_v_iadd1) + SQR(v0_v_i) - SQR(vi_v_iadd1)) / (2 * v0_v_iadd1 * v0_v_i));
				double beta_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(vi_v_iminus1) - SQR(v0_v_i)) / (2 * v0_v_iminus1 * vi_v_iminus1));
				double gamai = acos((SQR(v0_v_iadd1) + SQR(vi_v_iadd1) - SQR(v0_v_i)) / (2 * v0_v_iadd1 * vi_v_iadd1));
				double w;
				if (((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_UNIFORM))->GetCheck())
				{
					w = 1;
				}
				else
					if (((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_LAPLACE))->GetCheck())
					{
						w = 1.0 / tan(beta_i_minus1) + 1.0 / tan(gamai);
					}
					else
						if (((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_MEANVALUE))->GetCheck())
						{
							w = (tan(alpha_i_minus1 / 2) + tan(alpha_i / 2)) / v0_v_i;

						}
				sumweight += w;
				omegaarray.push_back(w);
			}
			for (int i = 0; i < omegaarray.size(); i++)
			{

				omegaarray[i] = omegaarray[i] / sumweight;
			}

			double b1 = 0;
			for (int i = 0; i < m_ArrayOneRingVertices.size(); i++)
			{
				int boundaryflag = pMesh3D->pmesh->is_boundary(m_ArrayOneRingVertices[i]);
				std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, m_ArrayOneRingVertices[i]);
				int index = pMesh3D->pmesh->property(vprop_nonboundaryindex, m_ArrayOneRingVertices[i]);
				if (boundaryflag)
				{
					b1 += omegaarray[i] * uv[ii];
				}
				else
				{
					//equation[index] = -omegaarray[i];
					A.IncreaseItem(rowindex, index, -omegaarray[i]);
				}
			}
			A.IncreaseItem(rowindex, v0_index, 1);
			//equation[v0_index] = 1;
			//A.push_back(equation);
			b.push_back(b1);
			rowindex++;
		}
		//Debug
		/*TRACE("A   \n");
		for (int i = 0; i < A.size(); i++)
		{
			for (int j = 0; j < A[0].size(); j++)
			{
				TRACE(" %f ", A[i][j]);
			}
			TRACE("\n");
		}

		TRACE("b   \n");
		for (int i = 0; i < b.size(); i++)
			TRACE(" %f ", b[i]);*/
		Action_PolyOmino::SolveSpareseLinearSystem_matlab(&A, b, uvarray[ii]);
		//p_FormView3->SolveLinearSystem(A, b, uvarray[ii]);
	}

	//set the interior parameters
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int boundaryflag = pMesh3D->pmesh->is_boundary(v_it.handle());

		if (boundaryflag)
			continue;

		int v0_index = pMesh3D->pmesh->property(vprop_nonboundaryindex, v_it.handle());

		uvvalues[0] = uvarray[0][v0_index];
		uvvalues[1] = uvarray[1][v0_index];

		pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
	}
}

// Actions_HarmonicMapping message handlers
void Actions_HarmonicMapping::OnBnClickedHarmonic()
{
	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = 0;

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();
		pOpenMesh->m_FixVerticesArray.push_back(0);
		pOpenMesh->m_FixVerticesArray.push_back(1);
		pOpenMesh->m_FixVerticesArray.push_back(2);
		pOpenMesh->m_FixVerticesArray.push_back(3);
		pOpenMesh->m_Transform.Copy(*pMesh3D->GetTransform());
		pDoc->m_SceneGraph.Add(pOpenMesh);
		Actions_HarmonicMapping::ComputeHarmonic_LinearConstraints(pOpenMesh);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
		{
			CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
			pMesh3D->m_FixVerticesArray.push_back(0);
			pMesh3D->m_FixVerticesArray.push_back(1);
			pMesh3D->m_FixVerticesArray.push_back(2);
			pMesh3D->m_FixVerticesArray.push_back(3);
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
void Actions_HarmonicMapping::SetZeroMu(CMesh3d_OpenMesh* pMesh3D)
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
	vec.push_back(0);
	vec.push_back(0);
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		pMesh3D->pmesh->property(fprop_face_mu, f_it.handle()) = vec;
	}
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle()) = vec;
	}
}
void Actions_HarmonicMapping::InitilizeVertexUV(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uv;
	OpenMesh::Vec3d pos;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pos = pMesh3D->pmesh->point(v_it.handle());
		uv.clear();
		uv.push_back(pos[0]);
		uv.push_back(pos[1]);
		pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uv;
	}
}
void Actions_HarmonicMapping::InitializeMeshOriginalLocation(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_ORIGINALLOCATION vprop_vertex_originallocation;

	//Vertex original location
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_originallocation, PROP_VERTEX_ORIGINALLOCATION))
		pMesh3D->pmesh->add_property(vprop_vertex_originallocation, PROP_VERTEX_ORIGINALLOCATION);

	std::vector<double> interinfo;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		int idx = v_it.handle().idx();
		OpenMesh::Vec3d vec = pMesh3D->pmesh->point(v_it.handle());
		interinfo.clear();
		interinfo.push_back(vec[0]);
		interinfo.push_back(vec[1]);
		pMesh3D->pmesh->property(vprop_vertex_originallocation, v_it.handle()) = interinfo;
	}
}
void Actions_HarmonicMapping::InitializeMeshInequalConstraintFlag(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_INEQUAL_CONSTRAINT vprop_vertex_inequal_flag;

	//Vertex Inequal flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_inequal_flag, PROP_VERTEX_INEQUAL_CONSTRAINT))
		pMesh3D->pmesh->add_property(vprop_vertex_inequal_flag, PROP_VERTEX_INEQUAL_CONSTRAINT);

	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		pMesh3D->pmesh->property(vprop_vertex_inequal_flag, v_it.handle()) = 0;
	}
}
int  Actions_HarmonicMapping::GetMeshInequalConstraintCount(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_INEQUAL_CONSTRAINT vprop_vertex_inequal_flag;

	//Vertex Inequal flag
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_inequal_flag, PROP_VERTEX_INEQUAL_CONSTRAINT))
		pMesh3D->pmesh->add_property(vprop_vertex_inequal_flag, PROP_VERTEX_INEQUAL_CONSTRAINT);

	int num = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->property(vprop_vertex_inequal_flag, v_it.handle()))
			num++;
	}
	return num;
}
void Actions_HarmonicMapping::InitializeMeshFaceSelectedFlag(CMesh3d_OpenMesh* pMesh3D)
{
	FPROP_FACE_SELECTED_FLAG fprop_faceselectedflag;

	//Face selected flag;
	if (!pMesh3D->pmesh->get_property_handle(fprop_faceselectedflag, PROP_FACE_SELECTED))
		pMesh3D->pmesh->add_property(fprop_faceselectedflag, PROP_FACE_SELECTED);

	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		pMesh3D->pmesh->property(fprop_faceselectedflag, f_it.handle()) = 0;
	}
}
void Actions_HarmonicMapping::InitializeMeshMu(CMesh3d_OpenMesh* pMesh3D)
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
}
void Actions_HarmonicMapping::GetMaxMinmuLength(CMesh3d_OpenMesh* pMesh3D, double &maxmulength, double &minmulength, double &averagemulength, double &standarddeviation)
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
	std::vector<double> x, y, u, v;
	std::vector<int> m_vindexarray;
	std::vector<double> m_mu;
	double currentmulength;
	averagemulength = GetAverageMeshMuModule(pMesh3D);
	standarddeviation = 0;
	int facenum = 0;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		if (IsFaceFixed(pMesh3D, f_it.handle()))
			continue;

		m_mu = pMesh3D->pmesh->property(fprop_face_mu, f_it.handle());

		double currentmulength = MULENGTH(m_mu);

		standarddeviation += SQR(currentmulength - averagemulength);
		facenum++;

		if (currentmulength > maxmulength)
			maxmulength = currentmulength;
		if (currentmulength < minmulength)
			minmulength = currentmulength;
	}
	standarddeviation = sqrt(standarddeviation / facenum);
}
void Actions_HarmonicMapping::GetMaxMinmuLength_Vertex(CMesh3d_OpenMesh* pMesh3D, double &maxmulength, double &minmulength, double &averagemulength, double &standarddeviation)
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
	std::vector<double> x, y, u, v;
	std::vector<int> m_vindexarray;
	std::vector<double> m_mu;
	double currentmulength;
	averagemulength = GetAverageMeshMuModule(pMesh3D);
	standarddeviation = 0;
	int facenum = 0;
	int vertexnum = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		m_mu = pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle());

		double currentmulength = MULENGTH(m_mu);

		standarddeviation += SQR(currentmulength - averagemulength);
		vertexnum++;

		if (currentmulength > maxmulength)
			maxmulength = currentmulength;
		if (currentmulength < minmulength)
			minmulength = currentmulength;
	}
	standarddeviation = sqrt(standarddeviation / vertexnum);
}
void Actions_HarmonicMapping::AddConstraints(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::pair<int, std::vector<double>> pair;
	std::vector<double> doublepair;

	//add constraints
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[i];
		pair.first = pLandmark->landmark_points[0];
		doublepair = pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(pair.first));
		pair.second = doublepair;
		pMesh3D->m_ArrayConstraints.push_back(pair);
		pMesh3D->m_FixVerticesArray.push_back(pair.first);
	}
}
CMesh3d_OpenMesh* Actions_HarmonicMapping::GetMeshfreeofFlip_quasiconformal(CMesh3d_OpenMesh* pMesh3D, int iternum)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	int step = 0;
	double scale = 0.5;
	//Set the Constraints
	//pTeichmullerMap->AddConstraints(pMesh3D);

	while (1)
	{
		if (IsMeshUVFlip(pMesh3D, iternum))
		{
			//
			pTeichmullerMap->SetZeroMu(pMesh3D);

			//Compute the mu
			pTeichmullerMap->ComputeMu(pMesh3D);

			//
			pTeichmullerMap->ScaleMu(pMesh3D,scale);

			//
			pTeichmullerMap->Compute_Linear_Equations_Registration(pMesh3D);

			//Debug
			//GenerateParameterDomain_LinearConstaints(pMesh3D, "nonflipmesh_smallstep");
			scale = scale * 0.5;
		}
		else
			break;
		step++;
	}

	char name[80];
	sprintf(name, "D:\\%d_nonflipmesh_smallstep",iternum);
	pMesh3D->SaveOBJParameterDomainFile(name);

	GenerateParameterDomain_LinearConstaints(pMesh3D, "nonflipmesh_smallstep");

	//
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle());
		OpenMesh::Vec3d pos = pMesh3D->pmesh->point(v_it.handle());

		//swap the face position and mu vector
		double tempx = pos[0];
		double tempy = pos[1];
		pos[0] = uv[0];
		pos[1] = uv[1];
		pMesh3D->pmesh->set_point(v_it.handle(), pos);

		uv[0] = tempx;
		uv[1] = tempy;
		pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uv;
	}

	//CDT_EdgeFlip(pMesh3D,iternum);
	//Constrained Delaunay Triangulation
	CDT_New(pMesh3D);
	//CDT_Direct(pMesh3D);
	//
	sprintf(name, "D:\\%d_nonflipmesh_smallstep_CDT", iternum);
	pMesh3D->SaveOBJFile(name);

	GenerateDuplicateMesh_LinearConstaints(pMesh3D, "CDT");

	//Save the original mesh after edge flip
	//char filename[80];
	//sprintf(filename, "D:\\originalmesh%d.obj", iternum);
	//SaveParameterDomainMesh(pMesh3D, filename);
	//pDoc->OnOpenDocument(filename);
	return pMesh3D;
	//
	//CMesh3d_OpenMesh* newmesh = CDT(pMesh3D,iternum);
	//newmesh->m_LandMarkArray = pMesh3D->m_LandMarkArray;
	//newmesh->m_FixVerticesArray = pMesh3D->m_FixVerticesArray;
	//return newmesh;
}
CMesh3d_OpenMesh* Actions_HarmonicMapping::GetMeshfreeofFlip(CMesh3d_OpenMesh* pMesh3D, int iternum)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	int step = 0;
	while (1)
	{
		if (IsMeshUVFlip(pMesh3D, iternum))
		{
			for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
			{
				//if (pMesh3D->pmesh->is_boundary(v_it.handle()))
				//	continue;
				std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle());
				OpenMesh::Vec3d pos = pMesh3D->pmesh->point(v_it.handle());

				uv[0] = (uv[0] + pos[0]) / 2;
				uv[1] = (uv[1] + pos[1]) / 2;
				pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uv;
			}
		}
		else
			break;
		step++;

	}
	GenerateParameterDomain_LinearConstaints(pMesh3D, "nonflipmesh_smallstep");
	//
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle());
		OpenMesh::Vec3d pos = pMesh3D->pmesh->point(v_it.handle());

		//swap the face position and mu vector
		double tempx = pos[0];
		double tempy = pos[1];
		pos[0] = uv[0];
		pos[1] = uv[1];
		pMesh3D->pmesh->set_point(v_it.handle(), pos);

		uv[0] = tempx;
		uv[1] = tempy;
		pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle()) = uv;
	}

	//CDT_EdgeFlip(pMesh3D,iternum);
	CDT_New(pMesh3D);
	//CDT_Direct(pMesh3D);
	GenerateDuplicateMesh_LinearConstaints(pMesh3D, "CDT");

	//Save the original mesh after edge flip
	char filename[80];
	sprintf(filename, "D:\\originalmesh%d.obj", iternum);
	SaveParameterDomainMesh(pMesh3D, filename);
	//pDoc->OnOpenDocument(filename);
	return pMesh3D;
	//
	//CMesh3d_OpenMesh* newmesh = CDT(pMesh3D,iternum);
	//newmesh->m_LandMarkArray = pMesh3D->m_LandMarkArray;
	//newmesh->m_FixVerticesArray = pMesh3D->m_FixVerticesArray;
	//return newmesh;
}

int Actions_HarmonicMapping::IsMeshFlip(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_ORIGINALLOCATION vprop_vertex_originallocation;
	//Vertex Original Location
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_originallocation, PROP_VERTEX_ORIGINALLOCATION))
		pMesh3D->pmesh->add_property(vprop_vertex_originallocation, PROP_VERTEX_ORIGINALLOCATION);

	FPROP_FACE_SELECTED_FLAG fprop_faceselectedflag;
	VPROP_VERTEX_SELECTED_FLAG vprop_vertexselectedflag;

	//Face selected flag;
	if (!pMesh3D->pmesh->get_property_handle(fprop_faceselectedflag, PROP_FACE_SELECTED))
		pMesh3D->pmesh->add_property(fprop_faceselectedflag, PROP_FACE_SELECTED);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertexselectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertexselectedflag, PROP_VERTEX_SELECTED);

	double minlen = 1e10;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle());

		//std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_originallocation, fv_it.handle());
		OpenMesh::VertexHandle vh1 = fv_it.handle();
		OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(vh1);
		fv_it++;
		//std::vector<double> uv2 = pMesh3D->pmesh->property(vprop_vertex_originallocation, fv_it.handle());
		OpenMesh::VertexHandle vh2 = fv_it.handle();
		OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(vh2);
		fv_it++;
		OpenMesh::VertexHandle vh3 = fv_it.handle();
		OpenMesh::Vec3d v3 = pMesh3D->pmesh->point(vh3);
		//std::vector<double> uv3 = pMesh3D->pmesh->property(vprop_vertex_originallocation, fv_it.handle());

		Vector3D vv1, vv2, vv3;
		vv1.SetValue(v1[0], v1[1], 0);
		vv2.SetValue(v2[0], v2[1], 0);
		vv3.SetValue(v3[0], v3[1], 0);

		Vector3D vv4 = vv2 - vv1;
		Vector3D vv5 = vv3 - vv1;
		vv4.SetUnit();
		vv5.SetUnit();
		Vector3D N = vv4 ^ vv5;
		if (N[2] < minlen)
			minlen = N[2];
		if (N[2] <= 1e-10)
		{
			pMesh3D->pmesh->property(fprop_faceselectedflag, f_it.handle()) = 1;
			pMesh3D->pmesh->property(vprop_vertexselectedflag, vh1) = 1;
			pMesh3D->pmesh->property(vprop_vertexselectedflag, vh2) = 1;
			pMesh3D->pmesh->property(vprop_vertexselectedflag, vh3) = 1;
		}
		//double len = Length(N);
	}
	if (minlen <= 0)
		return 1;
	else
		return 0;
}
int Actions_HarmonicMapping::IsMeshOriginalFlip(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_ORIGINALLOCATION vprop_vertex_originallocation;
	//Vertex Original Location
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_originallocation, PROP_VERTEX_ORIGINALLOCATION))
		pMesh3D->pmesh->add_property(vprop_vertex_originallocation, PROP_VERTEX_ORIGINALLOCATION);

	FPROP_FACE_SELECTED_FLAG fprop_faceselectedflag;

	//Face selected flag;
	if (!pMesh3D->pmesh->get_property_handle(fprop_faceselectedflag, PROP_FACE_SELECTED))
		pMesh3D->pmesh->add_property(fprop_faceselectedflag, PROP_FACE_SELECTED);

	double minlen = 1e10;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle());

		std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_originallocation, fv_it.handle());
		//OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(fv_it.handle());
		fv_it++;
		std::vector<double> uv2 = pMesh3D->pmesh->property(vprop_vertex_originallocation, fv_it.handle());
		//OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(fv_it.handle());
		fv_it++;
		//OpenMesh::Vec3d v3 = pMesh3D->pmesh->point(fv_it.handle());
		std::vector<double> uv3 = pMesh3D->pmesh->property(vprop_vertex_originallocation, fv_it.handle());

		Vector3D vv1, vv2, vv3;
		vv1.SetValue(uv1[0], uv1[1], 0);
		vv2.SetValue(uv2[0], uv2[1], 0);
		vv3.SetValue(uv3[0], uv3[1], 0);

		Vector3D vv4 = vv2 - vv1;
		Vector3D vv5 = vv3 - vv1;
		//vv4.SetUnit();
		//vv5.SetUnit();
		Vector3D N = vv4 ^ vv5;
		if (N[2] < minlen)
			minlen = N[2];
		if (N[2] < 0)
		{
			pMesh3D->pmesh->property(fprop_faceselectedflag, f_it.handle()) = 1;
		}
		//double len = Length(N);
	}
	if (minlen <= 0)
		return 1;
	else
		return 0;
}

int Actions_HarmonicMapping::IsValidTriangle(CMesh3d_OpenMesh* pMesh3D, int facehandleindex)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(pMesh3D->pmesh->face_handle(facehandleindex));

	std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_uv, fv_it.handle());
	//OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(fv_it.handle());
	fv_it++;
	std::vector<double> uv2 = pMesh3D->pmesh->property(vprop_vertex_uv, fv_it.handle());
	//OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(fv_it.handle());
	fv_it++;
	//OpenMesh::Vec3d v3 = pMesh3D->pmesh->point(fv_it.handle());
	std::vector<double> uv3 = pMesh3D->pmesh->property(vprop_vertex_uv, fv_it.handle());

	Vector3D vv1, vv2, vv3;
	vv1.SetValue(uv1[0], uv1[1], 0);
	vv2.SetValue(uv2[0], uv2[1], 0);
	vv3.SetValue(uv3[0], uv3[1], 0);

	double angle1, angle2, angle3;
	double minangle = pMesh3D->GetTriangleAngle(vv1, vv2, vv3, angle1, angle2, angle3);
	if (minangle < 0.01)
		return 0;
}
int Actions_HarmonicMapping::IsMeshUVFlip(CMesh3d_OpenMesh* pMesh3D, int iternum, int showmessage)
{
	//FPROP_FACE_MU fprop_face_mu;

	//Face MU
	//if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
	//	pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);
	/*Compute_Mu(pMesh3D);

	for(MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		std::vector<double> mu = pMesh3D->pmesh->property(fprop_face_mu,f_it.handle());
		double len = MULENGTH(mu);
		if(len > 1)
			return 1;
	}
	return 0;*/
	FPROP_FACE_SELECTED_FLAG fprop_face_selectedflag;

	//Selected Face
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_selectedflag, PROP_FACE_SELECTED))
		pMesh3D->pmesh->add_property(fprop_face_selectedflag, PROP_FACE_SELECTED);

	double minarea = 1e10;
	int invalidfacenum = 0;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		pMesh3D->pmesh->property(fprop_face_selectedflag, f_it.handle()) = 0;

		double area = pMesh3D->GetTriangleSignArea_UV(f_it.handle().idx());

		if (minarea > area)
			minarea = area;

		int flag = pMesh3D->IsTriangleDegenerate_Colinear_UV(f_it.handle().idx());
		//if (flag)
		//	return 1;

		if (area < 0 || flag)
		{
			invalidfacenum++;
			pMesh3D->pmesh->property(fprop_face_selectedflag, f_it.handle()) = 1;
		}
		//double len = Length(N);
	}

	if (showmessage)
	{
		sprintf(Post_Message, "Invalid Face number: %5d;minimu length%10.7f;Iteration Number:%5d", invalidfacenum, minarea, iternum);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	}

	if (invalidfacenum > 0)
		return 1;
	else
		return 0;
}
double Actions_HarmonicMapping::GetMaxLandmarkDeviation(CMesh3d_OpenMesh* pMesh3D)
{
	double maxdeviation = -1e10;
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int index1 = pLandMark->landmark_points[j];
			int index2 = pLandMark->landmark_points[j + 1];

			OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(index1));
			OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(index2));

			if (pLandMark->style == 0)
			{
				double len = fabs(v1[1] - v2[1]);
				if (len > maxdeviation)
					maxdeviation = len;
			}
			else
			{
				double len = fabs(v1[0] - v2[0]);
				if (len > maxdeviation)
					maxdeviation = len;
			}
		}
	}
	return maxdeviation;
}
void Actions_HarmonicMapping::ComputeDynamicHarmonic_LinearConstraints(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face MU
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//pMesh3D->pmesh->property(vprop_vertex_uv).set_persistent(true);
	//pMesh3D->pmesh->property(vprop_selectedflag).set_persistent(true);
	//pMesh3D->pmesh->property(fprop_face_mu).set_persistent(true);
	//pMesh3D->pmesh->property(vprop_vertex_mu).set_persistent(true);

	//first fix the boundary
	Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pMesh3D);

	//
	InitializeMeshFaceSelectedFlag(pMesh3D);

	//
	InitializeMeshOriginalLocation(pMesh3D);

	//
	//InitializeMeshMu(pMesh3D);
	double maxmulength, minmulength, averagemulength, standarddeviation;
	double premaxmulength;
	int iternum = 0;
	char filename[80];
	char Muinfo[80];
	sprintf(Muinfo, "D:\\muinfo.txt");
	std::ofstream Mufile(Muinfo);
	int flag;

	//
	//Compute_LinearConstraints_Dynamic_Harmonic(pMesh3D,1);
	//IsMeshUVFlip(pMesh3D);
	int invliadnumber;

	InitializeMeshInequalConstraintFlag(pMesh3D);
	int inequalconstraints;
	while (1)
	{
		InitializeMeshFaceSelectedFlag(pMesh3D);

		inequalconstraints = GetMeshInequalConstraintCount(pMesh3D);
		while (1)
		{
			//std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_uv, pMesh3D->pmesh->vertex_handle(161));
			//Obtain the linear equations
			//Compute_Linear_Equations_LinearConstraints(pMesh3D,0);
			//
			SetBoundaryUV(pMesh3D);

			//
			Compute_Linear_Equations_LinearConstraints_SingleVariable(pMesh3D, 0);
			break;
			invliadnumber = MeshUVLandmarkInvalid(pMesh3D);
			if (invliadnumber == 0)
				break;
		}
		inequalconstraints = GetMeshInequalConstraintCount(pMesh3D);
		break;
		if (invliadnumber)
		{
			AfxMessageBox("Need to add auxilary landmark!");
			break;
		}

		if (!IsMeshUVFlip(pMesh3D, iternum))
		{
			AfxMessageBox("Succeed!");
			break;
		}

		//break;
		//if(iternum == 1)
		//	break;
		//Get the mesh without flip
		GetMeshfreeofFlip(pMesh3D, iternum);

		//break;
		//pMesh3D = newmesh;
		//TRACE("%d \n",iternum);
		//break;

		//Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pMesh3D);

		//
		//InitializeMeshMu(pMesh3D);
		//break;

		//Compute the mu, quasi conformal 
		//Compute_Mu(pMesh3D);

		//GetMaxMinmuLength_Vertex(pMesh3D,maxmulength,minmulength,averagemulength,standarddeviation);
		//TRACE("%5d, Mu: %11.6f %11.6f %11.6f %11.6f\n",iternum, maxmulength,minmulength,averagemulength,standarddeviation);
		//Mufile << iternum << " " << maxmulength << " " << minmulength << " " << averagemulength << " " << standarddeviation << "\n";
		//Mufile.flush();
		//if(iternum > 0 && maxmulength > premaxmulength)
		//	break;

		//sprintf(filename,"D:\\%d.obj",iternum);

		//
		//SaveParameterDomainMesh(pMesh3D,filename);
		//break;

		//premaxmulength = maxmulength;

		//if(fabs(maxmulength - minmulength) < 0.01)
		//	break;

		sprintf(filename, "D:\\%d.obj", iternum);
		OpenMesh::IO::write_mesh(*pMesh3D->pmesh, filename);
		//double len = GetMaxLandmarkDeviation(pMesh3D);
		//Mufile << iternum << " " << len << "\n";
		//Mufile.flush();
		//if(len < 1e-5)
		//	break;

		iternum++;
		if (iternum > 500)
			break;

		//flag = Subdivide_Triangle_Mulargerthan1(pMesh3D);

		//sprintf(filename,"D:\\%d_1.obj",iternum);
		//OpenMesh::IO::write_mesh(*pMesh3D->pmesh, filename);

		//if(flag)
		//	continue;

		//Compute_Mu(pMesh3D);

		//GenerateParameterDomain_LinearConstaints(pMesh3D);

		//break;
		//
		//AverageMeshMu(pMesh3D);
	}
	Mufile.close();
	//
	//Compute_Linear_Equations_LinearConstraints(pMesh3D,1);
	//
	//Compute_Mu(pMesh3D);

	//
	InitializeMeshFaceSelectedFlag(pMesh3D);

	//
	IsMeshOriginalFlip(pMesh3D);

	//Generate a new parameter domain mesh
	GenerateParameterDomain_LinearConstaints(pMesh3D);

	//if(IsMeshOriginalFlip(pMesh3D))
	//	ASSERT(0);

	//
	GenerateOriginalMesh_LinearConstaints(pMesh3D);
}
void Actions_HarmonicMapping::ComputeDynamicHarmonic_LinearConstraints_Test(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face MU
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//pMesh3D->pmesh->property(vprop_vertex_uv).set_persistent(true);
	//pMesh3D->pmesh->property(vprop_selectedflag).set_persistent(true);
	//pMesh3D->pmesh->property(fprop_face_mu).set_persistent(true);
	//pMesh3D->pmesh->property(vprop_vertex_mu).set_persistent(true);

	//first fix the boundary
	Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints_Test(pMesh3D);

	//
	//InitializeMeshMu(pMesh3D);
	double maxmulength, minmulength, averagemulength, standarddeviation;
	double premaxmulength;
	int iternum = 0;
	char filename[80];
	char Muinfo[80];
	sprintf(Muinfo, "D:\\muinfo.txt");
	std::ofstream Mufile(Muinfo);
	int flag;

	//
	//Compute_LinearConstraints_Dynamic_Harmonic(pMesh3D,1);
	//IsMeshUVFlip(pMesh3D);

	while (1)
	{
		//Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints_Test(pMesh3D);

		//Obtain the linear equations
		Compute_Linear_Equations_LinearConstraints(pMesh3D, 0);

		if (!IsMeshUVFlip(pMesh3D, iternum))
			break;
		//Get the mesh without flip
		GetMeshfreeofFlip(pMesh3D, iternum);

		pDoc->m_SceneGraph.Add(pMesh3D->copy());

		//break;
		//pMesh3D = newmesh;
		//TRACE("%d \n",iternum);
		//break;

		//Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pMesh3D);

		//
		//InitializeMeshMu(pMesh3D);
		//break;

		//Compute the mu, quasi conformal 
		//Compute_Mu(pMesh3D);

		//GetMaxMinmuLength_Vertex(pMesh3D,maxmulength,minmulength,averagemulength,standarddeviation);
		//TRACE("%5d, Mu: %11.6f %11.6f %11.6f %11.6f\n",iternum, maxmulength,minmulength,averagemulength,standarddeviation);
		//Mufile << iternum << " " << maxmulength << " " << minmulength << " " << averagemulength << " " << standarddeviation << "\n";
		//Mufile.flush();
		//if(iternum > 0 && maxmulength > premaxmulength)
		//	break;

		//sprintf(filename,"D:\\%d.obj",iternum);

		//
		//SaveParameterDomainMesh(pMesh3D,filename);
		//break;

		//premaxmulength = maxmulength;

		//if(fabs(maxmulength - minmulength) < 0.01)
		//	break;

		sprintf(filename, "D:\\%d.obj", iternum);
		OpenMesh::IO::write_mesh(*pMesh3D->pmesh, filename);
		//double len = GetMaxLandmarkDeviation(pMesh3D);
		//Mufile << iternum << " " << len << "\n";
		//Mufile.flush();
		//if(len < 1e-5)
		//	break;

		iternum++;
		if (iternum > 500)
			break;

		//flag = Subdivide_Triangle_Mulargerthan1(pMesh3D);

		//sprintf(filename,"D:\\%d_1.obj",iternum);
		//OpenMesh::IO::write_mesh(*pMesh3D->pmesh, filename);

		//if(flag)
		//	continue;

		//Compute_Mu(pMesh3D);

		//GenerateParameterDomain_LinearConstaints(pMesh3D);

		//break;
		//
		//AverageMeshMu(pMesh3D);
	}
	Mufile.close();
	//
	//Compute_Linear_Equations_LinearConstraints(pMesh3D,1);
	//
	//Compute_Mu(pMesh3D);

	//Generate a new parameter domain mesh
	GenerateParameterDomain_LinearConstaints(pMesh3D);
}
void Actions_HarmonicMapping::ComputeHarmonic_LinearConstraints(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Vertex MU
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face MU
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	pMesh3D->pmesh->property(vprop_vertex_uv).set_persistent(true);
	pMesh3D->pmesh->property(vprop_selectedflag).set_persistent(true);
	pMesh3D->pmesh->property(fprop_face_mu).set_persistent(true);
	pMesh3D->pmesh->property(vprop_vertex_mu).set_persistent(true);

	//first fix the boundary
	Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pMesh3D);

	//
	InitializeMeshMu(pMesh3D);
	double maxmulength, minmulength, averagemulength, standarddeviation;
	double premaxmulength;
	int iternum = 0;
	char filename[80];
	char Muinfo[80];
	sprintf(Muinfo, "D:\\muinfo.txt");
	std::ofstream Mufile(Muinfo);
	int flag;
	while (1)
	{
		//Obtain the linear equations
		Compute_Linear_Equations_LinearConstraints(pMesh3D, 1);

		//Compute the mu, quasi conformal 
		Compute_Mu(pMesh3D);

		GetMaxMinmuLength_Vertex(pMesh3D, maxmulength, minmulength, averagemulength, standarddeviation);
		//TRACE("%5d, Mu: %11.6f %11.6f %11.6f %11.6f\n",iternum, maxmulength,minmulength,averagemulength,standarddeviation);
		Mufile << iternum << " " << maxmulength << " " << minmulength << " " << averagemulength << " " << standarddeviation << "\n";
		Mufile.flush();
		//if(iternum > 0 && maxmulength > premaxmulength)
		//	break;

		sprintf(filename, "D:\\%d.obj", iternum);

		//
		SaveParameterDomainMesh(pMesh3D, filename);
		//break;

		premaxmulength = maxmulength;

		if (fabs(maxmulength - minmulength) < 0.01)
			break;

		iternum++;
		if (iternum > 500)
			break;

		sprintf(filename, "D:\\%d_0.obj", iternum);
		OpenMesh::IO::write_mesh(*pMesh3D->pmesh, filename);


		flag = Subdivide_Triangle_Mulargerthan1(pMesh3D);

		sprintf(filename, "D:\\%d_1.obj", iternum);
		OpenMesh::IO::write_mesh(*pMesh3D->pmesh, filename);

		if (flag)
			continue;

		Compute_Mu(pMesh3D);

		//GenerateParameterDomain_LinearConstaints(pMesh3D);

		//break;
		//
		AverageMeshMu(pMesh3D);
	}
	Mufile.close();
	//
	//Compute_Linear_Equations_LinearConstraints(pMesh3D,1);
	//
	Compute_Mu(pMesh3D);

	//Generate a new parameter domain mesh
	GenerateParameterDomain_LinearConstaints(pMesh3D);
}
void Actions_HarmonicMapping::SaveParameterDomainMesh(CMesh3d_OpenMesh* pMesh3D, char *filename)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::ofstream outfile1(filename);
	std::vector<double> uv;
	OpenMesh::Vec3d vec;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		uv = pMesh3D->pmesh->property(vprop_vertex_uv, v_it.handle());
		vec = pMesh3D->pmesh->point(v_it.handle());
		if (uv.size() > 0)
		{
			outfile1 << "v " << uv[0] << " " << uv[1] << " 0 \n";
		}
		else
		{
			outfile1 << "v " << vec[0] << " " << vec[1] << " " << vec[2] << "\n";
		}
	}
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		outfile1 << "f ";
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
		{
			outfile1 << fv_it.handle().idx() + 1 << " ";
		}
		outfile1 << "\n";
	}
	outfile1.close();
}
BOOL Actions_HarmonicMapping::IsFaceFixed(CMesh3d_OpenMesh* pMesh3D, OpenMesh::FaceHandle fh)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_vertex_flag;

	//Vertex Selected
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_flag, PROP_VERTEX_SELECTED))
		pMesh3D->pmesh->add_property(vprop_vertex_flag, PROP_VERTEX_SELECTED);

	MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(fh);
	int vflag1 = pMesh3D->pmesh->property(vprop_vertex_flag, fv_it.handle());
	fv_it++;
	int vflag2 = pMesh3D->pmesh->property(vprop_vertex_flag, fv_it.handle());
	fv_it++;
	int vflag3 = pMesh3D->pmesh->property(vprop_vertex_flag, fv_it.handle());
	if (vflag1 && vflag2 && vflag3)
		return 1;
	else
		return 0;
}
double Actions_HarmonicMapping::GetAverageMeshMuModule(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_MU vprop_vertex_mu;
	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	double x = 0;
	double y = 0;
	std::vector<double> vec;
	int facenum = 0;
	int vertexnum = 0;
	double totalmulength = 0;
	double currentlength;
	std::vector<std::vector<double>> newmuarray;
	/*for(MyMesh::FaceIter f_it=pMesh3D->pmesh->faces_begin(); f_it!=pMesh3D->pmesh->faces_end(); ++f_it)
	{
		if(IsFaceFixed(pMesh3D,f_it.handle()))
			continue;

		vec = pMesh3D->pmesh->property(fprop_face_mu,f_it.handle());
		currentlength = sqrt(SQR(vec[0]) + SQR(vec[1]));
		if(currentlength < 1)
		{
			totalmulength += currentlength;
			facenum++;
		}
	}*/
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		vec = pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle());
		currentlength = sqrt(SQR(vec[0]) + SQR(vec[1]));
		if (currentlength < 1)
		{
			totalmulength += currentlength;
			vertexnum++;
		}
	}
	double averagemulength = totalmulength / vertexnum;
	return averagemulength;
}
double Actions_HarmonicMapping::SearchForOptimalStep(CMesh3d_OpenMesh* pMesh3D)
{
	double maxmulength, minmulength, averagemulength, standarddeviation;
	std::vector<double> maxmuarray, minmuarray;
	double minimalmulength = 1e10;
	int minimalmuindex;
	for (int i = 0; i <= 10; i++)
	{
		CMesh3d_OpenMesh* pNewMesh = pMesh3D->copy();
		ModifyFaceMubyStep(pNewMesh, i * 0.1);

		//
		Compute_Linear_Equations_LinearConstraints(pNewMesh, 1);

		//
		Compute_Mu(pNewMesh);

		//
		GetMaxMinmuLength(pNewMesh, maxmulength, minmulength, averagemulength, standarddeviation);

		maxmuarray.push_back(maxmulength);

		minmuarray.push_back(minmulength);

		if (maxmulength < minimalmulength)
		{
			minimalmulength = maxmulength;
			minimalmuindex = i;
		}

		delete pNewMesh;
	}
	double aa = (minimalmuindex - 1) * 0.1;
	double bb = (minimalmuindex + 1) * 0.1;
	minimalmulength = 1e10;
	maxmuarray.clear();
	minmuarray.clear();
	for (int ii = 0; ii <= 20; ii++)
	{
		CMesh3d_OpenMesh* pNewMesh = pMesh3D->copy();
		ModifyFaceMubyStep(pNewMesh, aa + 0.01 * ii);

		//
		Compute_Linear_Equations_LinearConstraints(pNewMesh, 1);

		//
		Compute_Mu(pNewMesh);

		//
		GetMaxMinmuLength(pNewMesh, maxmulength, minmulength, averagemulength, standarddeviation);

		maxmuarray.push_back(maxmulength);

		minmuarray.push_back(minmulength);

		if (maxmulength < minimalmulength)
		{
			minimalmulength = maxmulength;
			minimalmuindex = ii;
		}

		delete pNewMesh;
	}
	return minimalmuindex * 0.01 + aa;
}
void Actions_HarmonicMapping::LaplaceSmoothMeshMu(CMesh3d_OpenMesh* pMesh3D)
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
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		num = 0;
		x = y = 0;

		faceidxarray.clear();
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
		{
			for (MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(fv_it.handle()); vf_it; vf_it++)
			{
				int idx = vf_it.handle().idx();
				if (std::find(faceidxarray.begin(), faceidxarray.end(), idx) == faceidxarray.end())
					faceidxarray.push_back(idx);
			}
		}
		for (int i = 0; i < faceidxarray.size(); i++)
		{
			vec = pMesh3D->pmesh->property(fprop_face_mu, pMesh3D->pmesh->face_handle(faceidxarray[i]));
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
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		pMesh3D->pmesh->property(fprop_face_mu, f_it.handle()) = newmuarray[num++];
	}
}
void Actions_HarmonicMapping::GeneratePSLGFile(CMesh3d_OpenMesh* pMesh3D, std::vector<int>& m_ArrayOutputVerticesIndex, char* outputfile)
{
	//Output the vertices
	int nvertices = pMesh3D->pmesh->n_vertices();
	std::ofstream outfile(outputfile);

	outfile << nvertices << " 2 0 1" << "\n";
	int flag;
	int num = 0;
	int boundaryvertices = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
		{
			flag = 1;
			boundaryvertices++;
		}
		else
			flag = 0;
		OpenMesh::Vec3d pos = pMesh3D->pmesh->point(v_it.handle());

		outfile << num << " " << pos[0] << " " << pos[1] << " " << flag << "\n";
		num++;
	}

	int numofsegments = boundaryvertices;
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		numofsegments += pLandMark->landmark_points.size() - 1;
	}

	outfile << numofsegments << " 1 " << "\n";

	//first output boundary edge
	int edgeindex = 0;
	for (MyMesh::EdgeIter e_it = pMesh3D->pmesh->edges_begin(); e_it != pMesh3D->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		MyMesh::HalfedgeHandle hh = pMesh3D->pmesh->halfedge_handle(eh, 0);

		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->to_vertex_handle(hh);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->from_vertex_handle(hh);

		if (pMesh3D->pmesh->is_boundary(eh))
		{
			outfile << edgeindex << " " << vh1.idx() << " " << vh2.idx() << " 1" << "\n";
			edgeindex++;
		}
	}

	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int start = pLandMark->landmark_points[j];
			int end = pLandMark->landmark_points[j + 1];

			outfile << edgeindex << " " << start << " " << end << " 0 " << "\n";
			edgeindex++;
		}
	}
	outfile << " 0 " << "\n";
	outfile.close();
}
void Actions_HarmonicMapping::CDT_EdgeFlip(CMesh3d_OpenMesh* pMesh3D, int iternum)
{
	EPROP_EDGE_SELECTED_FLAG eprop_selectedflag;

	//Edge selected flag;
	if (!pMesh3D->pmesh->get_property_handle(eprop_selectedflag, PROP_EDGE_SELECTED))
		pMesh3D->pmesh->add_property(eprop_selectedflag, PROP_EDGE_SELECTED);

	//Initialize the edge select flag
	for (MyMesh::EdgeIter e_it = pMesh3D->pmesh->edges_begin(); e_it != pMesh3D->pmesh->edges_end(); ++e_it)
	{
		pMesh3D->pmesh->property(eprop_selectedflag, e_it.handle()) = 0;
	}

	//label the edge flag for all the landamrk edges
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size() - 1; j++)
		{
			int vindex1 = pLandmark->landmark_points[j];
			int vindex2 = pLandmark->landmark_points[j + 1];

			for (MyMesh::VertexEdgeIter ve_it = pMesh3D->pmesh->ve_iter(pMesh3D->pmesh->vertex_handle(vindex1)); ve_it; ++ve_it)
			{
				MyMesh::EdgeHandle eh = ve_it.handle();

				MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
				MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

				int vh1 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
				int vh2 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();

				if (vh1 == vindex1 && vh2 == vindex2 || vh1 == vindex2 && vh2 == vindex1)
				{
					pMesh3D->pmesh->property(eprop_selectedflag, eh) = 1;
					break;
				}
			}
		}
	}

	std::vector<int> m_ArrayEdgeId;
	//
	for (MyMesh::EdgeIter e_it = pMesh3D->pmesh->edges_begin(); e_it != pMesh3D->pmesh->edges_end(); ++e_it)
	{
		int flag = pMesh3D->pmesh->property(eprop_selectedflag, e_it.handle());
		if (flag || pMesh3D->pmesh->is_boundary(e_it.handle()))
			continue;

		MyMesh::EdgeHandle eh = e_it.handle();

		MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

		int vh1 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
		int vh2 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();

		flag = IsDelaunay(pMesh3D, eh.idx());
		if (!flag)
		{
			m_ArrayEdgeId.push_back(eh.idx());
			//SwapAdjacentTriangles(pMesh3D,eh.idx());
		}
	}
	int iterstep = 0;
	while (m_ArrayEdgeId.size() > 0)
	{
		SwapAdjacentTriangles(pMesh3D, m_ArrayEdgeId[0], m_ArrayEdgeId);
		//pMesh3D->m_Modified = TRUE;
		//pMesh3D->glBuildList();
		//break;
		m_ArrayEdgeId.erase(m_ArrayEdgeId.begin());
		iterstep++;
		//if(iterstep == 500)
		//	break;
	}
	pMesh3D->pmesh->garbage_collection();
	pMesh3D->m_Modified = 1;
}
Vector3D Actions_HarmonicMapping::GetFaceNormal(CMesh3d_OpenMesh* pMesh3D, MyMesh::FaceHandle fh)
{
	MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(fh);

	int vh1index, vh2index, vh3index;
	OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(fv_it.handle());
	vh1index = fv_it.handle().idx();
	fv_it++;
	OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(fv_it.handle());
	vh2index = fv_it.handle().idx();
	fv_it++;
	OpenMesh::Vec3d v3 = pMesh3D->pmesh->point(fv_it.handle());
	vh3index = fv_it.handle().idx();

	Vector3D vv1, vv2, vv3;
	vv1.SetValue(v1[0], v1[1], v1[2]);
	vv2.SetValue(v2[0], v2[1], v2[2]);
	vv3.SetValue(v3[0], v3[1], v3[2]);

	Vector3D vv4 = vv2 - vv1;
	Vector3D vv5 = vv3 - vv1;
	Vector3D nor1 = vv4 ^ vv5;
	return nor1;
}
Vector3D Actions_HarmonicMapping::GetFaceNormal_THREEPOINTS(CMesh3d_OpenMesh* pMesh3D, int vh1,int vh2,int vh3)
{
	int vh1index, vh2index, vh3index;
	OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh1));
	OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh2));
	OpenMesh::Vec3d v3 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh3));

	Vector3D vv1, vv2, vv3;
	vv1.SetValue(v1[0], v1[1], v1[2]);
	vv2.SetValue(v2[0], v2[1], v2[2]);
	vv3.SetValue(v3[0], v3[1], v3[2]);

	Vector3D vv4 = vv2 - vv1;
	Vector3D vv5 = vv3 - vv1;
	Vector3D nor1 = vv4 ^ vv5;
	return nor1;
}
int Actions_HarmonicMapping::SwapAdjacentTriangles(CMesh3d_OpenMesh* pMesh3D, int ehindex)
{
	MyMesh::EdgeHandle eh = pMesh3D->pmesh->edge_handle(ehindex);

	MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

	int vh2 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
	int vh1 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();

	std::vector<int> oppsitevertices;
	GetTwoOppositePoints(pMesh3D, ehindex, oppsitevertices);

	int vh3 = oppsitevertices[0];
	int vh4 = oppsitevertices[1];
	MyMesh::FaceHandle fh0 = pMesh3D->pmesh->face_handle(hh0);
	MyMesh::FaceHandle fh1 = pMesh3D->pmesh->face_handle(hh1);

	Vector3D  originalnormal = pHarmonicMapping->GetFaceNormal(pMesh3D, fh0);
	pMesh3D->pmesh->delete_face(fh0, FALSE);
	pMesh3D->pmesh->delete_face(fh1, FALSE);

	Vector3D  newnormal = pHarmonicMapping->GetFaceNormal_THREEPOINTS(pMesh3D, vh3, vh4, vh2);
	double val = originalnormal * newnormal;
	std::vector<MyMesh::VertexHandle>  face_vhandles;

	MyMesh::FaceHandle fh2, fh3;
	if (val > 0)
	{
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh2));
		fh2 = pMesh3D->pmesh->add_face(face_vhandles);
	}
	else
	{
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh2));
		fh2 = pMesh3D->pmesh->add_face(face_vhandles);
	}

	newnormal = pHarmonicMapping->GetFaceNormal_THREEPOINTS(pMesh3D, vh3, vh1, vh4);
	val = originalnormal * newnormal;
	if (val > 0)
	{
		face_vhandles.clear();
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh1));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		fh3 = pMesh3D->pmesh->add_face(face_vhandles);
	}
	else
	{
		face_vhandles.clear();
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh1));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		fh3 = pMesh3D->pmesh->add_face(face_vhandles);
	}

	//pMesh3D->pmesh->garbage_collection();
	return 1;
}

int Actions_HarmonicMapping::SwapAdjacentTriangles_Propogate(CMesh3d_OpenMesh* pMesh3D, int ehindex)
{
	MyMesh::EdgeHandle eh = pMesh3D->pmesh->edge_handle(ehindex);

	MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

	if (!hh0.is_valid() || !hh1.is_valid())
		return 0;

	int vh2 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
	int vh1 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();

	std::vector<int> oppsitevertices;
	GetTwoOppositePoints(pMesh3D, ehindex, oppsitevertices);

	double angle1, angle2;
	int ehindex1, ehindex2;
	int flag;
	double anglesum1 = GetAdjacentTwoAnglesSum(pMesh3D, ehindex, vh1, angle1,angle2, ehindex1, ehindex2);
	if (anglesum1 > PI)
	{
		if (angle1 > angle2)
		{
			flag = SwapAdjacentTriangles_Propogate(pMesh3D, ehindex1);
		}
		else
		{
			flag = SwapAdjacentTriangles_Propogate(pMesh3D, ehindex2);
		}
		if (!flag)
			return 0;

		ehindex = pMesh3D->GetEdgeHandleFromTwoVertices(vh1, vh2);

		eh = pMesh3D->pmesh->edge_handle(ehindex);

		hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
		hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

		oppsitevertices.clear();
		GetTwoOppositePoints(pMesh3D, ehindex, oppsitevertices);
	}
	double anglesum2 = GetAdjacentTwoAnglesSum(pMesh3D, ehindex, vh2, angle1, angle2, ehindex1, ehindex2);
	if (anglesum2 > PI)
	{
		if (angle1 > angle2)
		{
			flag = SwapAdjacentTriangles_Propogate(pMesh3D, ehindex1);
		}
		else
		{
			flag = SwapAdjacentTriangles_Propogate(pMesh3D, ehindex2);
		}
		if (!flag)
			return 0;

		ehindex = pMesh3D->GetEdgeHandleFromTwoVertices(vh1, vh2);

		eh = pMesh3D->pmesh->edge_handle(ehindex);

		hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
		hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

		oppsitevertices.clear();
		GetTwoOppositePoints(pMesh3D, ehindex, oppsitevertices);
	}

	if (oppsitevertices.size() != 2)
		return 0;

	int vh3 = oppsitevertices[0];
	int vh4 = oppsitevertices[1];
	MyMesh::FaceHandle fh0 = pMesh3D->pmesh->face_handle(hh0);
	MyMesh::FaceHandle fh1 = pMesh3D->pmesh->face_handle(hh1);

	Vector3D  originalnormal = pHarmonicMapping->GetFaceNormal(pMesh3D, fh0);
	pMesh3D->pmesh->delete_face(fh0, FALSE);
	pMesh3D->pmesh->delete_face(fh1, FALSE);

	Vector3D  newnormal = pHarmonicMapping->GetFaceNormal_THREEPOINTS(pMesh3D, vh3, vh4, vh2);
	double val = originalnormal * newnormal;
	std::vector<MyMesh::VertexHandle>  face_vhandles;

	MyMesh::FaceHandle fh2, fh3;
	if (val > 0)
	{
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh2));
		fh2 = pMesh3D->pmesh->add_face(face_vhandles);
	}
	else
	{
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh2));
		fh2 = pMesh3D->pmesh->add_face(face_vhandles);
	}

	newnormal = pHarmonicMapping->GetFaceNormal_THREEPOINTS(pMesh3D, vh3, vh1, vh4);
	val = originalnormal * newnormal;
	if (val > 0)
	{
		face_vhandles.clear();
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh1));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		fh3 = pMesh3D->pmesh->add_face(face_vhandles);
	}
	else
	{
		face_vhandles.clear();
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh1));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		fh3 = pMesh3D->pmesh->add_face(face_vhandles);
	}

	pMesh3D->pmesh->garbage_collection();
	return 1;
}

int Actions_HarmonicMapping::IsValidSwapAdjacentTriangles(CMesh3d_OpenMesh* pMesh3D, int ehindex)
{
	MyMesh::EdgeHandle eh = pMesh3D->pmesh->edge_handle(ehindex);

	MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

	int vh2 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
	int vh1 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();

	std::vector<int> oppsitevertices;
	GetTwoOppositePoints(pMesh3D, ehindex, oppsitevertices);

	int vh3 = oppsitevertices[0];
	int vh4 = oppsitevertices[1];
	MyMesh::FaceHandle fh0 = pMesh3D->pmesh->face_handle(hh0);
	MyMesh::FaceHandle fh1 = pMesh3D->pmesh->face_handle(hh1);

	Vector3D  newnormal1 = pHarmonicMapping->GetFaceNormal_THREEPOINTS(pMesh3D, vh3, vh4, vh2);
	Vector3D  newnormal2 = pHarmonicMapping->GetFaceNormal_THREEPOINTS(pMesh3D, vh4, vh3, vh1);
	double val = newnormal1 * newnormal2;
	if (val <= 0)
		return 0;
	else
		return 1;
}

int Actions_HarmonicMapping::SwapAdjacentTriangles(CMesh3d_OpenMesh* pMesh3D, int ehindex, std::vector<int> &m_ArrayEdgeId)
{
	MyMesh::EdgeHandle eh = pMesh3D->pmesh->edge_handle(ehindex);

	MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

	int vh2 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
	int vh1 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();

	std::vector<int> oppsitevertices;
	GetTwoOppositePoints(pMesh3D, ehindex, oppsitevertices);

	int vh3 = oppsitevertices[0];
	int vh4 = oppsitevertices[1];
	MyMesh::FaceHandle fh0 = pMesh3D->pmesh->face_handle(hh0);
	MyMesh::FaceHandle fh1 = pMesh3D->pmesh->face_handle(hh1);

	Vector3D  originalnormal = pHarmonicMapping->GetFaceNormal(pMesh3D, fh0);
	pMesh3D->pmesh->delete_face(fh0, FALSE);
	pMesh3D->pmesh->delete_face(fh1, FALSE);

	Vector3D  newnormal = pHarmonicMapping->GetFaceNormal_THREEPOINTS(pMesh3D,vh3,vh4,vh2);
	double val = originalnormal * newnormal;
	std::vector<MyMesh::VertexHandle>  face_vhandles;

	MyMesh::FaceHandle fh2, fh3;
	if (val > 0)
	{
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh2));
		fh2 = pMesh3D->pmesh->add_face(face_vhandles);
	}
	else
	{
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh2));
		fh2 = pMesh3D->pmesh->add_face(face_vhandles);
	}

	newnormal = pHarmonicMapping->GetFaceNormal_THREEPOINTS(pMesh3D, vh3, vh1, vh4);
	val = originalnormal * newnormal;
	if (val > 0)
	{
		face_vhandles.clear();
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh1));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		fh3 = pMesh3D->pmesh->add_face(face_vhandles);
	}
	else
	{
		face_vhandles.clear();
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh1));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh3));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(vh4));
		fh3 = pMesh3D->pmesh->add_face(face_vhandles);
	}


	//update ids
	for (MyMesh::FaceEdgeIter fe_it = pMesh3D->pmesh->fe_iter(fh2); fe_it; ++fe_it)
	{
		if (!IsDelaunay(pMesh3D, fe_it.handle().idx()))
			m_ArrayEdgeId.push_back(fe_it.handle().idx());
	}
	for (MyMesh::FaceEdgeIter fe_it = pMesh3D->pmesh->fe_iter(fh3); fe_it; ++fe_it)
	{
		if (!IsDelaunay(pMesh3D, fe_it.handle().idx()))
			m_ArrayEdgeId.push_back(fe_it.handle().idx());
	}
	//pMesh3D->pmesh->garbage_collection();
	return 1;
}
int Actions_HarmonicMapping::IsDelaunay(CMesh3d_OpenMesh* pMesh3D, int ehindex)
{
	double TOL = 0.0001;
	MyMesh::EdgeHandle eh = pMesh3D->pmesh->edge_handle(ehindex);

	MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

	int vh1 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
	int vh2 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();

	std::vector<int> oppsitevertices;
	GetTwoOppositePoints(pMesh3D, ehindex, oppsitevertices);

	if (oppsitevertices.size() == 2)
	{
		int vh3 = oppsitevertices[0];
		int vh4 = oppsitevertices[1];

		OpenMesh::Vec3d vv1 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh1));
		OpenMesh::Vec3d vv2 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh2));
		OpenMesh::Vec3d vv3 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh3));
		OpenMesh::Vec3d vv4 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vh4));

		double e23 = Length(vv2 - vv3);
		double e13 = Length(vv1 - vv3);
		double e12 = Length(vv1 - vv2);
		double e24 = Length(vv2 - vv4);
		double e14 = Length(vv1 - vv4);

		double angle231 = acos((SQR(e23) + SQR(e13) - SQR(e12)) / (2 * e23 * e13));
		double angle241 = acos((SQR(e24) + SQR(e14) - SQR(e12)) / (2 * e24 * e14));

		if (angle231 + angle241 < 3.1415926 + TOL)
			return 1;
	}
	return 0;
}
double Actions_HarmonicMapping::GetAdjacentTwoAnglesSum(CMesh3d_OpenMesh* pMesh3D, int ehindex, int vindex,double &angle1,double &angle2,int &ehindex1,int &ehindex2)
{
	MyMesh::EdgeHandle eh = pMesh3D->pmesh->edge_handle(ehindex);

	MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

	double anglesum = 0;

	int vh1 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
	int vh2 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();
	int vh3;
	MyMesh::HalfedgeHandle hh2,hh3;
	if (vh1 == vindex)
	{
		hh2 = pMesh3D->pmesh->next_halfedge_handle(hh0);
		vh3 = pMesh3D->pmesh->to_vertex_handle(hh2).idx();
		angle1 = GetAngle_ThreeVertices(pMesh3D, vh2, vh1, vh3);
		ehindex1 = pMesh3D->GetEdgeHandleFromTwoVertices(vh2, vh3);
		anglesum += angle1;
	}
	else if(vh2 == vindex)
	{
		hh2 = pMesh3D->pmesh->prev_halfedge_handle(hh0);
		vh3 = pMesh3D->pmesh->from_vertex_handle(hh2).idx();
		angle1 = GetAngle_ThreeVertices(pMesh3D, vh1, vh2, vh3);
		ehindex1 = pMesh3D->GetEdgeHandleFromTwoVertices(vh1, vh3);
		anglesum += angle1;
	}

	//
	vh1 = pMesh3D->pmesh->to_vertex_handle(hh1).idx();
	vh2 = pMesh3D->pmesh->from_vertex_handle(hh1).idx();

	if (vh1 == vindex)
	{
		hh2 = pMesh3D->pmesh->next_halfedge_handle(hh1);
		vh3 = pMesh3D->pmesh->to_vertex_handle(hh2).idx();
		angle2 = GetAngle_ThreeVertices(pMesh3D, vh2, vh1, vh3);
		ehindex2 = pMesh3D->GetEdgeHandleFromTwoVertices(vh2, vh3);
		anglesum += angle2;
	}
	else if (vh2 == vindex)
	{
		hh2 = pMesh3D->pmesh->prev_halfedge_handle(hh1);
		vh3 = pMesh3D->pmesh->from_vertex_handle(hh2).idx();
		angle2 = GetAngle_ThreeVertices(pMesh3D, vh1, vh2, vh3);
		ehindex2 = pMesh3D->GetEdgeHandleFromTwoVertices(vh1, vh3);
		anglesum += angle2;
	}

	return anglesum;
}
double Actions_HarmonicMapping::GetAngle_ThreeVertices(CMesh3d_OpenMesh* pMesh3D, int vindex1, int vindex2, int vindex3)
{
	OpenMesh::Vec3d v1 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vindex1));
	OpenMesh::Vec3d v2 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vindex2));
	OpenMesh::Vec3d v3 = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(vindex3));

	Vector3D vv1(v1[0], v1[1], v1[2]);
	Vector3D vv2(v2[0], v2[1], v2[2]);
	Vector3D vv3(v3[0], v3[1], v3[2]);

	Vector3D vv4 = vv1 - vv2;
	Vector3D vv5 = vv3 - vv2;
	vv4.SetUnit();
	vv5.SetUnit();

	double cosval = vv4 * vv5;
	double angle = acos(cosval);
	return angle;
}
void Actions_HarmonicMapping::GetTwoOppositePoints(CMesh3d_OpenMesh* pMesh3D, int ehindex, std::vector<int> &oppsitevertices)
{
	MyMesh::EdgeHandle eh = pMesh3D->pmesh->edge_handle(ehindex);

	MyMesh::HalfedgeHandle hh0 = pMesh3D->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pMesh3D->pmesh->halfedge_handle(eh, 1);

	int vh1 = pMesh3D->pmesh->to_vertex_handle(hh0).idx();
	int vh2 = pMesh3D->pmesh->from_vertex_handle(hh0).idx();
	if (pMesh3D->pmesh->is_valid_handle(hh0))
	{
		MyMesh::FaceHandle fh0 = pMesh3D->pmesh->face_handle(hh0);
		if (pMesh3D->pmesh->is_valid_handle(fh0))
		{
			for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(fh0); fv_it; ++fv_it)
			{
				if (fv_it.handle().idx() != vh1 && fv_it.handle().idx() != vh2)
					oppsitevertices.push_back(fv_it.handle().idx());
			}
		}
	}
	if (pMesh3D->pmesh->is_valid_handle(hh1))
	{
		MyMesh::FaceHandle fh1 = pMesh3D->pmesh->face_handle(hh1);
		if (pMesh3D->pmesh->is_valid_handle(fh1))
		{
			for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(fh1); fv_it; ++fv_it)
			{
				if (fv_it.handle().idx() != vh1 && fv_it.handle().idx() != vh2)
					oppsitevertices.push_back(fv_it.handle().idx());
			}
		}
	}
}
void Actions_HarmonicMapping::CDT_Direct(CMesh3d_OpenMesh* pMesh3D)
{
	struct triangulateio in, mid, vorout;
	//Set the input
	in.numberofpoints = pMesh3D->pmesh->n_vertices();
	in.numberofpointattributes = 0;
	in.pointlist = (REAL *)malloc(in.numberofpoints * 2 * sizeof(REAL));
	int vindex = 0;
	int boundaryvertices = 0;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d pos = pMesh3D->pmesh->point(v_it.handle());
		in.pointlist[vindex * 2] = pos[0];
		in.pointlist[vindex * 2 + 1] = pos[1];
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
		{
			boundaryvertices++;
		}
		vindex++;
	}

	in.pointattributelist = NULL;
	mid.pointattributelist = NULL;
	in.pointmarkerlist = NULL;

	int numofsegments = boundaryvertices;
	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		numofsegments += pLandMark->landmark_points.size() - 1;
	}

	in.numberofsegments = numofsegments;
	in.segmentlist = (int *)malloc(in.numberofsegments * 2 * sizeof(int));
	in.segmentmarkerlist = (int *)malloc(in.numberofsegments * sizeof(int));;

	//first output boundary edge
	int edgeindex = 0;
	for (MyMesh::EdgeIter e_it = pMesh3D->pmesh->edges_begin(); e_it != pMesh3D->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();
		MyMesh::HalfedgeHandle hh = pMesh3D->pmesh->halfedge_handle(eh, 0);

		MyMesh::VertexHandle vh1 = pMesh3D->pmesh->to_vertex_handle(hh);
		MyMesh::VertexHandle vh2 = pMesh3D->pmesh->from_vertex_handle(hh);

		if (pMesh3D->pmesh->is_boundary(eh))
		{
			in.segmentlist[edgeindex * 2] = vh1.idx();
			in.segmentlist[edgeindex * 2 + 1] = vh2.idx();
			in.segmentmarkerlist[edgeindex] = 1;
			edgeindex++;
		}
	}

	for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh3D->m_LandMarkArray[i];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int start = pLandMark->landmark_points[j];
			int end = pLandMark->landmark_points[j + 1];

			in.segmentlist[edgeindex * 2] = start;
			in.segmentlist[edgeindex * 2 + 1] = end;
			in.segmentmarkerlist[edgeindex] = 0;
			edgeindex++;
		}
	}

	in.numberofholes = 0;
	in.holelist = NULL;
	in.numberofregions = 0;
	in.regionlist = NULL;

	/* Triangulate the points.  Switches are chosen to read and write a  */
	/*   PSLG (p), preserve the convex hull (c), number everything from  */
	/*   zero (z), assign a regional attribute to each element (A), and  */
	/*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
	/*   neighbor list (n).                                              */

	triangulate("pcz", &in, &mid, &vorout);

	//delete the face
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		pMesh3D->pmesh->delete_face(f_it.handle(), FALSE);
	}
	int nindex1, nindex2, nindex3;
	std::vector<MyMesh::VertexHandle>  face_vhandles;
	for (int i = 0; i < mid.numberoftriangles; i++)
	{
		face_vhandles.clear();
		nindex1 = mid.trianglelist[i * 3];
		nindex2 = mid.trianglelist[i * 3 + 1];
		nindex3 = mid.trianglelist[i * 3 + 2];

		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(nindex1));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(nindex2));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(nindex3));
		pMesh3D->pmesh->add_face(face_vhandles);
	}

	pMesh3D->pmesh->garbage_collection();
	pMesh3D->m_Modified = TRUE;
}
void Actions_HarmonicMapping::CDT_New(CMesh3d_OpenMesh* pMesh3D)
{
	std::vector<int> m_ArrayOutputVerticesIndex;
	char resultmeshname[200];

	//first write the smooth results to A pslg file
	sprintf(resultmeshname, "D:\\papers\\code\\Triangle\\CDT.poly");
	GeneratePSLGFile(pMesh3D, m_ArrayOutputVerticesIndex, resultmeshname);

	char step1[200];
	SetCurrentDirectory("D:\\papers\\code\\triangle");
	//sprintf(step1,"triangle.exe -pq33.8 Geo");
	sprintf(step1, "triangle.exe -p CDT");
	system(step1);

	sprintf(resultmeshname, "D:\\papers\\code\\triangle\\CDT.1.ele");
	//CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	//ASSERT_VALID(pDoc);
	//pDoc->OnOpenDocument(resultmeshname);
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		pMesh3D->pmesh->delete_face(f_it.handle(), FALSE);
	}

	std::ifstream elefile(resultmeshname);

	int numtriangles, numofverticespertriangle, triangleattributes, index;
	elefile >> numtriangles >> numofverticespertriangle >> triangleattributes;
	if (numofverticespertriangle != 3)
		return;
	int nindex1, nindex2, nindex3;
	std::vector<MyMesh::VertexHandle>  face_vhandles;
	for (int i = 0; i < numtriangles; i++)
	{
		face_vhandles.clear();
		elefile >> index >> nindex1 >> nindex2 >> nindex3;
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(nindex1));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(nindex2));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(nindex3));
		pMesh3D->pmesh->add_face(face_vhandles);
	}
	elefile.close();
	pMesh3D->pmesh->garbage_collection();
	pMesh3D->m_Modified = TRUE;
}
CMesh3d_OpenMesh* Actions_HarmonicMapping::CDT(CMesh3d_OpenMesh* pMesh3D, int iternum)
{
	std::vector<int> m_ArrayOutputVerticesIndex;
	char resultmeshname[200];

	//first write the smooth results to A pslg file
	sprintf(resultmeshname, "D:\\papers\\code\\Triangle\\CDT%d.poly", iternum);
	GeneratePSLGFile(pMesh3D, m_ArrayOutputVerticesIndex, resultmeshname);

	char step1[200];
	SetCurrentDirectory("D:\\papers\\code\\triangle");
	//sprintf(step1,"triangle.exe -pq33.8 Geo");
	sprintf(step1, "triangle.exe -p CDT%d", iternum);
	system(step1);

	sprintf(resultmeshname, "D:\\papers\\code\\triangle\\CDT%d.1.ele", iternum);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument(resultmeshname);

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	CMesh3d_OpenMesh* pMesh1_Openmesh = pMesh1->Convert_OpenMesh();
	/*for(int i = 0; i < pMesh1->m_ArrayVertex.GetSize(); i++)
	{
		CVertex3d* pVertex = pMesh1->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		pVertex->Set(y,x,0);
	}*/
	pMesh1_Openmesh->m_LandMarkArray = pMesh3D->m_LandMarkArray;
	pMesh1_Openmesh->m_ArraySharpEdges = pMesh3D->m_ArraySharpEdges;
	pMesh1_Openmesh->m_Transform.Copy(&pMesh3D->m_Transform);
	//
	//DetectandSplitSharpEdges(pMesh1,m_ArrayOutputVerticesIndex);

	//this->pMesh = pMesh1;
	//GenerateLandMarks();
	//pMesh1->m_showFace = FALSE;
	//pMesh1->m_showSharpEdge = FALSE;
	//pMesh1->m_showLandmarkEdge = TRUE;
	//pMesh1->m_showLandmarkVertex = FALSE;
	//pDoc->UpdateAllViews(NULL);
	return pMesh1_Openmesh;
}
int Actions_HarmonicMapping::Subdivide_Triangle_Mulargerthan1(CMesh3d_OpenMesh* pMesh3D)
{
	FPROP_FACE_MU fprop_face_mu;
	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> vec, uvvec;
	std::vector<MyMesh::VertexHandle> m_ArrayVertexHandles;
	OpenMesh::Vec3d pos, currentpos;
	std::vector<MyMesh::VertexHandle>  face_vhandles;
	MyMesh::VertexHandle vh;
	double uvx, uvy;
	int flag = 0;
	std::vector<MyMesh::FaceHandle>  face_array;

	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		vec = pMesh3D->pmesh->property(fprop_face_mu, f_it.handle());
		if (vec.size() < 2)
			continue;

		double currentmulength = MULENGTH(vec);
		if (currentmulength > 1)
		{
			face_array.push_back(f_it.handle());
			for (MyMesh::FaceFaceIter ff_it = pMesh3D->pmesh->ff_iter(f_it.handle()); ff_it; ++ff_it)
				face_array.push_back(ff_it.handle());
		}
	}
	for (int i = 0; i < face_array.size(); i++)
	{
		flag = 1;
		pos[0] = pos[1] = pos[2] = 0;
		uvx = uvy = 0;
		m_ArrayVertexHandles.clear();
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(face_array[i]); fv_it; ++fv_it)
		{
			uvvec = pMesh3D->pmesh->property(vprop_vertex_uv, fv_it.handle());
			uvx += uvvec[0];
			uvy += uvvec[1];
			m_ArrayVertexHandles.push_back(fv_it.handle());
			currentpos = pMesh3D->pmesh->point(fv_it.handle());
			pos = pos + currentpos;
		}
		pos = pos / 3;
		uvx = uvx / 3;
		uvy = uvy / 3;
		uvvec.clear();
		uvvec.push_back(uvx);
		uvvec.push_back(uvy);
		pMesh3D->pmesh->delete_face(face_array[i]);
		vh = pMesh3D->pmesh->add_vertex(MyMesh::Point(pos[0], pos[1], pos[2]));
		m_ArrayVertexHandles.push_back(vh);
		pMesh3D->pmesh->property(vprop_vertex_uv, vh) = uvvec;

		face_vhandles.clear();
		face_vhandles.push_back(m_ArrayVertexHandles[0]);
		face_vhandles.push_back(m_ArrayVertexHandles[1]);
		face_vhandles.push_back(m_ArrayVertexHandles[3]);
		pMesh3D->pmesh->add_face(face_vhandles);
		face_vhandles.clear();
		face_vhandles.push_back(m_ArrayVertexHandles[1]);
		face_vhandles.push_back(m_ArrayVertexHandles[2]);
		face_vhandles.push_back(m_ArrayVertexHandles[3]);
		pMesh3D->pmesh->add_face(face_vhandles);
		face_vhandles.clear();
		face_vhandles.push_back(m_ArrayVertexHandles[2]);
		face_vhandles.push_back(m_ArrayVertexHandles[0]);
		face_vhandles.push_back(m_ArrayVertexHandles[3]);
		pMesh3D->pmesh->add_face(face_vhandles);
	}
	if (flag)
	{
		pMesh3D->pmesh->garbage_collection();
		InitializeMeshMu(pMesh3D);
	}
	return flag;
}
void Actions_HarmonicMapping::AverageMeshMu(CMesh3d_OpenMesh* pMesh3D)
{
	//double step = 0.1;
	//LaplaceSmoothMeshMu(pMesh3D);

	//double step = SearchForOptimalStep(pMesh3D);
	double step = 1.0;
	//
	//ModifyFaceMubyStep(pMesh3D,step);
	ModifyVertexMubyStep(pMesh3D, step);
}
void Actions_HarmonicMapping::ModifyFaceMubyStep(CMesh3d_OpenMesh* pMesh3D, double step)
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

	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		vec = pMesh3D->pmesh->property(fprop_face_mu, f_it.handle());
		double currentmulength = MULENGTH(vec);
		if (currentmulength > 1e-5)
		{
			double newmulength = currentmulength + (averagemulength - currentmulength) * step;
			double ratio = newmulength / currentmulength;
			vec[0] = vec[0] * ratio;
			vec[1] = vec[1] * ratio;
			pMesh3D->pmesh->property(fprop_face_mu, f_it.handle()) = vec;
		}
	}

	double mux, muy;
	int num;
	//double totalmulength = 0;
	//int vnum = 0;
	std::vector<double> muvec;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;
		mux = 0;
		muy = 0;
		num = 0;
		for (MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(v_it.handle()); vf_it; vf_it++)
		{
			std::vector<double> mu = pMesh3D->pmesh->property(fprop_face_mu, vf_it.handle());
			if (mu.size() == 2)
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
		pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle()) = muvec;
		//vnum++;
	}
}
void Actions_HarmonicMapping::ModifyVertexMubyStep(CMesh3d_OpenMesh* pMesh3D, double step)
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

	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		vec = pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle());
		double currentmulength = MULENGTH(vec);

		if (currentmulength > 1)
		{
			//AverageNeibouringVertexMuVector_TwoRing(pMesh3D,vec,v_it.handle());
			AverageNeibouringVertexMuVector_OneRing(pMesh3D, vec, v_it.handle());
			//pMesh3D->pmesh->property(vprop_vertex_mu,v_it.handle()) = vec;
		}
	}

	/*for(MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if(pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		vec = pMesh3D->pmesh->property(vprop_vertex_mu,v_it.handle());
		double currentmulength = MULENGTH(vec);
		if(currentmulength < 1e-5)
			continue;

		double newmulength = currentmulength + (averagemulength - currentmulength) * step;
		double ratio = newmulength / currentmulength;
		vec[0] = vec[0] * ratio;
		vec[1] = vec[1] * ratio;
		pMesh3D->pmesh->property(vprop_vertex_mu,v_it.handle()) = vec;
	}*/
}
void Actions_HarmonicMapping::AverageNeibouringVertexMuVector_OneRing(CMesh3d_OpenMesh* pMesh3D, std::vector<double> &vec, OpenMesh::VertexHandle vh)
{
	VPROP_VERTEX_MU vprop_vertex_mu;

	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	double x, y;
	x = y = 0;
	int vertexnum = 0;
	std::vector<double> muvec;
	for (MyMesh::VertexVertexIter vv_it = pMesh3D->pmesh->vv_iter(vh); vv_it; ++vv_it)
	{
		if (pMesh3D->pmesh->is_boundary(vv_it.handle()))
			continue;

		muvec = pMesh3D->pmesh->property(vprop_vertex_mu, vv_it.handle());
		double currentmulength = MULENGTH(muvec);
		if (currentmulength > 1)
		{
			continue;
		}
		x += muvec[0];
		y += muvec[1];
		vertexnum++;
	}
	if (vertexnum > 0)
	{
		x = x / vertexnum;
		y = y / vertexnum;
	}

	vec.clear();
	vec.push_back(x);
	vec.push_back(y);
	pMesh3D->pmesh->property(vprop_vertex_mu, vh) = vec;
}
void Actions_HarmonicMapping::AverageNeibouringVertexMuVector_TwoRing(CMesh3d_OpenMesh* pMesh3D, std::vector<double> &vec, OpenMesh::VertexHandle vh)
{
	VPROP_VERTEX_MU vprop_vertex_mu;

	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	double x, y;
	x = y = 0;
	int vertexnum = 0;
	std::vector<double> muvec;
	std::vector<int> m_VertexArray;
	for (MyMesh::VertexVertexIter vv_it = pMesh3D->pmesh->vv_iter(vh); vv_it; ++vv_it)
	{
		m_VertexArray.push_back(vv_it.handle().idx());
	}
	for (int i = 0; i < m_VertexArray.size(); i++)
	{
		for (MyMesh::VertexVertexIter vv_it = pMesh3D->pmesh->vv_iter(pMesh3D->pmesh->vertex_handle(m_VertexArray[i])); vv_it; ++vv_it)
		{
			if (std::find(m_VertexArray.begin(), m_VertexArray.end(), vv_it.handle().idx()) == m_VertexArray.end())
				m_VertexArray.push_back(vv_it.handle().idx());
		}
	}

	for (int i = 0; i < m_VertexArray.size(); i++)
	{
		if (pMesh3D->pmesh->is_boundary(pMesh3D->pmesh->vertex_handle(m_VertexArray[i])))
			continue;

		muvec = pMesh3D->pmesh->property(vprop_vertex_mu, pMesh3D->pmesh->vertex_handle(m_VertexArray[i]));
		double currentmulength = MULENGTH(muvec);
		if (currentmulength > 1)
		{
			continue;
		}
		x += muvec[0];
		y += muvec[1];
		vertexnum++;
	}
	if (vertexnum > 0)
	{
		x = x / vertexnum;
		y = y / vertexnum;
	}

	vec.clear();
	vec.push_back(x);
	vec.push_back(y);
	pMesh3D->pmesh->property(vprop_vertex_mu, vh) = vec;
}
void Actions_HarmonicMapping::Compute_Mu(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex mu
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pMesh3D->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face mu
	if (!pMesh3D->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pMesh3D->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	std::vector<double> x, y, u, v;
	double minfaimou = 1e10;
	double maxfaimou = -1e10;
	std::vector<int> m_tongji;
	m_tongji.resize(11, 0);
	std::vector<double> m_mu;
	std::vector<int> m_vindexarray;
	for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	{
		x.clear();
		y.clear();
		u.clear();
		v.clear();
		m_vindexarray.clear();
		for (MyMesh::FaceVertexIter fv_it = pMesh3D->pmesh->fv_iter(f_it.handle()); fv_it; ++fv_it)
		{
			std::vector<double> uv = pMesh3D->pmesh->property(vprop_vertex_uv, fv_it.handle());
			OpenMesh::Vec3d pt;
			pt = pMesh3D->pmesh->point(fv_it.handle());
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
		double dT = (x[0] * y[1] - y[0] * x[1]) + (x[1] * y[2] - y[1] * x[2]) + (x[2] * y[0] - y[2] * x[0]);
		double partialu_partialx = (u[0] * (y[1] - y[2]) + u[1] * (y[2] - y[0]) + u[2] * (y[0] - y[1])) / dT;
		double partialu_partialy = (u[0] * (x[2] - x[1]) + u[1] * (x[0] - x[2]) + u[2] * (x[1] - x[0])) / dT;
		double partialv_partialx = (v[0] * (y[1] - y[2]) + v[1] * (y[2] - y[0]) + v[2] * (y[0] - y[1])) / dT;
		double partialv_partialy = (v[0] * (x[2] - x[1]) + v[1] * (x[0] - x[2]) + v[2] * (x[1] - x[0])) / dT;

		double partialdzbar_real = (partialu_partialx - partialv_partialy) / 2;
		double partialdzbar_imaginary = (partialv_partialx + partialu_partialy) / 2;

		double partialdz_real = (partialu_partialx + partialv_partialy) / 2;
		double partialdz_imaginary = (partialv_partialx - partialu_partialy) / 2;

		double denorminator = SQR(partialdz_real) + SQR(partialdz_imaginary);
		double fai_real = (partialdzbar_real * partialdz_real + partialdzbar_imaginary * partialdz_imaginary) / denorminator;
		double fai_imaginary = (partialdzbar_imaginary * partialdz_real - partialdzbar_real * partialdz_imaginary) / denorminator;
		double fai_mou = sqrt(SQR(fai_real) + SQR(fai_imaginary));
		m_mu.clear();
		m_mu.push_back(fai_real);
		m_mu.push_back(fai_imaginary);
		pMesh3D->pmesh->property(fprop_face_mu, f_it.handle()) = m_mu;

		if (fai_mou > maxfaimou)
			maxfaimou = fai_mou;
		if (fai_mou < minfaimou)
			minfaimou = fai_mou;
		/*if(fai_mou > 1)
			m_tongji[10]++;
		else if(fai_mou > 0.9)
			m_tongji[9]++;
		else if(fai_mou > 0.8)
			m_tongji[8]++;
		else if(fai_mou > 0.7)
			m_tongji[7]++;
		else if(fai_mou > 0.6)
			m_tongji[6]++;
		else if(fai_mou > 0.5)
			m_tongji[5]++;
		else if(fai_mou > 0.4)
			m_tongji[4]++;
		else if(fai_mou > 0.3)
			m_tongji[3]++;
		else if(fai_mou > 0.2)
			m_tongji[2]++;
		else if(fai_mou > 0.1)
			m_tongji[1]++;
		else
			m_tongji[0]++;*/
	}
	double mux, muy;
	int num;
	double totalmulength = 0;
	int vnum = 0;
	std::vector<double> muvec;
	for (MyMesh::VertexIter v_it = pMesh3D->pmesh->vertices_begin(); v_it != pMesh3D->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh3D->pmesh->is_boundary(v_it.handle()))
			continue;

		mux = 0;
		muy = 0;
		num = 0;

		for (MyMesh::VertexFaceIter vf_it = pMesh3D->pmesh->vf_iter(v_it.handle()); vf_it; vf_it++)
		{
			std::vector<double> mu = pMesh3D->pmesh->property(fprop_face_mu, vf_it.handle());
			if (mu.size() == 2)
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
		totalmulength += sqrt(SQR(mux) + SQR(muy));
		pMesh3D->pmesh->property(vprop_vertex_mu, v_it.handle()) = muvec;
		vnum++;
	}
}
void Actions_HarmonicMapping::GenerateOriginalMesh_LinearConstaints(CMesh3d_OpenMesh* pMesh3D)
{
	VPROP_VERTEX_ORIGINALLOCATION vprop_vertex_originallocation;

	//Vertex original location
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_originallocation, PROP_VERTEX_ORIGINALLOCATION))
		pMesh3D->pmesh->add_property(vprop_vertex_originallocation, PROP_VERTEX_ORIGINALLOCATION);

	MyMesh* newmesh = new MyMesh(*pMesh3D->pmesh);

	for (MyMesh::VertexIter v_it = newmesh->vertices_begin(); v_it != newmesh->vertices_end(); ++v_it)
	{
		int idx = v_it.handle().idx();
		std::vector<double> interinfo = pMesh3D->pmesh->property(vprop_vertex_originallocation, v_it.handle());

		OpenMesh::Vec3d p;
		p[0] = interinfo[0];
		p[1] = interinfo[1];
		p[2] = 0;
		newmesh->set_point(v_it.handle(), p);
	}

	CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pNewMesh->pmesh = newmesh;
	pNewMesh->m_LandMarkArray = pMesh3D->m_LandMarkArray;
	pNewMesh->m_ArraySharpEdges = pMesh3D->m_ArraySharpEdges;
	pNewMesh->m_Transform.Copy(&pMesh3D->m_Transform);

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	pDoc->m_SceneGraph.Add(pNewMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_HarmonicMapping::GenerateDuplicateMesh_LinearConstaints(CMesh3d_OpenMesh* pMesh3D, char* filename)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_DISPLACEMENT vprop_vertex_displacement_original;
	VPROP_VERTEX_DISPLACEMENT vprop_vertex_displacement_new;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Vertex Displacement
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_displacement_original, PROP_VERTEX_DISPLACEMENT))
		pMesh3D->pmesh->add_property(vprop_vertex_displacement_original, PROP_VERTEX_DISPLACEMENT);

	MyMesh* newmesh = new MyMesh(*pMesh3D->pmesh);

	CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pNewMesh->pmesh = newmesh;
	pNewMesh->m_LandMarkArray = pMesh3D->m_LandMarkArray;
	pNewMesh->m_ArraySharpEdges = pMesh3D->m_ArraySharpEdges;
	pNewMesh->m_Transform.Copy(&pMesh3D->m_Transform);
	if (filename)
		pNewMesh->filename = filename;
	else
		pNewMesh->filename = "DuplicateMesh";

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	pDoc->m_SceneGraph.Add(pNewMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_HarmonicMapping::GenerateParameterDomain_LinearConstaints(CMesh3d_OpenMesh* pMesh3D, char* filename)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;
	VPROP_VERTEX_DISPLACEMENT vprop_vertex_displacement_original;
	VPROP_VERTEX_DISPLACEMENT vprop_vertex_displacement_new;

	//Vertex UV
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh3D->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!pMesh3D->pmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		pMesh3D->pmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	//Vertex Displacement
	if (!pMesh3D->pmesh->get_property_handle(vprop_vertex_displacement_original, PROP_VERTEX_DISPLACEMENT))
		pMesh3D->pmesh->add_property(vprop_vertex_displacement_original, PROP_VERTEX_DISPLACEMENT);

	//OpenMesh::Vec3d p = pMesh3D->pmesh->point(pMesh3D->pmesh->vertex_handle(0));

	MyMesh* newmesh = new MyMesh(*pMesh3D->pmesh);

	//p[0] = p[0] + 1;
	//pMesh3D->pmesh->set_point(pMesh3D->pmesh->vertex_handle(0), p);
	//OpenMesh::Vec3d p1 = newmesh->point(newmesh->vertex_handle(0));

	//Vertex Displacement
	if (!newmesh->get_property_handle(vprop_vertex_displacement_new, PROP_VERTEX_DISPLACEMENT))
		newmesh->add_property(vprop_vertex_displacement_new, PROP_VERTEX_DISPLACEMENT);

	for (MyMesh::VertexIter v_it = newmesh->vertices_begin(); v_it != newmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d p;
		std::vector<double> uv = newmesh->property(vprop_vertex_uv, v_it.handle());
		int idx = v_it.handle().idx();
		if (idx == 0 || idx == 1 || idx == 2 || idx == 3)
		{
			int ij = 0;
		}
		p[0] = uv[0];
		p[1] = uv[1];
		p[2] = 0;
		OpenMesh::Vec3d pos = pMesh3D->pmesh->point(v_it.handle());
		std::vector<double> dis;
		dis.clear();
		dis.push_back(uv[0] - pos[0]);
		dis.push_back(uv[1] - pos[1]);
		dis.push_back(0);
		newmesh->property(vprop_vertex_displacement_new, v_it.handle()) = dis;
		newmesh->property(vprop_vertex_displacement_original, v_it.handle()) = dis;
		newmesh->set_point(v_it.handle(), p);
	}

	CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pNewMesh->pmesh = newmesh;
	pNewMesh->m_LandMarkArray = pMesh3D->m_LandMarkArray;
	pNewMesh->m_ArraySharpEdges = pMesh3D->m_ArraySharpEdges;
	pNewMesh->m_Transform.Copy(&pMesh3D->m_Transform);
	if (filename)
		pNewMesh->filename = filename;
	else
		pNewMesh->filename = "parameterdomain";

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	pDoc->m_SceneGraph.Add(pNewMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_HarmonicMapping::GenerateParameterDomain(CMesh3d_OpenMesh* pMesh3D, char* filename)
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
	for (MyMesh::VertexIter v_it = newmesh->vertices_begin(); v_it != newmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d p;
		std::vector<double> uv = newmesh->property(vprop_vertex_uv, v_it.handle());
		if (uv.size() > 0)
		{
			p[0] = uv[0];
			p[1] = uv[1];
			p[2] = 0;
			newmesh->set_point(v_it.handle(), p);
		}
	}

	CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pNewMesh->pmesh = newmesh;
	
	if (filename)
		pNewMesh->filename = filename;
	else
		pNewMesh->filename = "Parameterization";

	pNewMesh->m_Transform.Copy(&pMesh3D->m_Transform);
	pNewMesh->m_LandMarkArray = pMesh3D->m_LandMarkArray;
	pNewMesh->m_FixVerticesArray = pMesh3D->m_FixVerticesArray;
	pNewMesh->m_FixVerticesArray_uv = pMesh3D->m_FixVerticesArray_uv;
	pNewMesh->m_showFace = 0;
	pNewMesh->m_showLandmarkEdge = 1;

	CMesh3d* pNewMesh3d = pNewMesh->ConvertMesh3d();
	pNewMesh3d->m_Transform.Copy(&pMesh3D->m_Transform);
	pNewMesh3d->m_showFace = 0;
	pNewMesh3d->m_showLandmarkEdge = TRUE;
	pNewMesh3d->m_showLandmarkVertex = FALSE;
	pNewMesh3d->m_showLandmarkVertexIndex = FALSE;
	pNewMesh3d->m_showLandmarkIndex = FALSE;

	pNewMesh->m_Transform.Copy(&pMesh3D->m_Transform);
	pNewMesh->m_showFace = 0;
	pNewMesh->m_showLandmarkEdge = 1;

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	pDoc->m_SceneGraph.Add(pNewMesh3d);
	//pDoc->m_SceneGraph.Add(pNewMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void Actions_HarmonicMapping::GenerateIntermediateMesh()
{
	pPolyOmino->OnBnClickedOminoLoadusamap();
	pPolyOmino->OnBnClickedOminoLoadusastreighten();

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);


	CMesh3d_OpenMesh* pMesh1_Openmesh = pMesh1->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh2_Openmesh = pMesh2->Convert_OpenMesh();
	std::vector<CMesh3d_OpenMesh*> m_ArrayMesh;
	m_ArrayMesh.resize(11);
	m_ArrayMesh[0] = pMesh1_Openmesh;
	m_ArrayMesh[10] = pMesh2_Openmesh;

	for (int i = 1; i <= 9; i++)
	{
		CMesh3d_OpenMesh* pNewMesh = pMesh1_Openmesh->copy();
		//pNewMesh->Copy(pMesh1);
		for (MyMesh::VertexIter v_it = pNewMesh->pmesh->vertices_begin(); v_it != pNewMesh->pmesh->vertices_end(); ++v_it)
		{
			OpenMesh::Vec3d point1 = pMesh1_Openmesh->pmesh->point(v_it.handle());
			OpenMesh::Vec3d point2 = pMesh2_Openmesh->pmesh->point(v_it.handle());

			double r = 1 - i * 0.1;
			OpenMesh::Vec3d newpoint;
			newpoint[0] = point1[0] * r + point2[0] * (1 - r);
			newpoint[1] = point1[1] * r + point2[1] * (1 - r);
			newpoint[2] = point1[2] * r + point2[2] * (1 - r);
			pNewMesh->pmesh->set_point(v_it.handle(), newpoint);
		}
		/*for(int j = 0; j < pNewMesh->m_ArrayVertex.GetSize(); j++)
		{
			CVertex3d* pVertex1 = pMesh1->GetVertex(j);
			CVertex3d* pVertex2 = pMesh2->GetVertex(j);
			CVertex3d* pVertex3 = pNewMesh->GetVertex(j);

			double r = 1 - i * 0.1;
			pVertex3->Set(pVertex1->x() * r + pVertex2->x() * (1 - r),pVertex1->y() * r + pVertex2->y() * (1 - r),pVertex1->z() * r + pVertex2->z() * (1 - r));
		}*/
		m_ArrayMesh[i] = pNewMesh;
		pNewMesh->m_Transform.Copy(&pMesh1->m_Transform);
		pDoc->m_SceneGraph.Add(pNewMesh);
		//break;

		//Save constraints
		std::ofstream outfile("D:\\constraints.txt");
		for (MyMesh::VertexIter v_it = pNewMesh->pmesh->vertices_begin(); v_it != pNewMesh->pmesh->vertices_end(); ++v_it)
		{
			if (pNewMesh->pmesh->is_boundary(v_it.handle()))
			{
				outfile << v_it.handle().idx();
				OpenMesh::Vec3d point1 = pNewMesh->pmesh->point(v_it.handle());
				outfile << " " << point1[0] << " " << point1[1] << "\n";
			}

		}
		//
		for (int j = 0; j < pNewMesh->m_LandMarkArray.size(); j++)
		{
			GeoMapLandMark* pLandmark = pMesh1->m_LandMarkArray[j];
			for (int jj = 0; jj < pLandmark->landmark_points.size(); jj++)
			{
				int index = pLandmark->landmark_points[jj];
				OpenMesh::Vec3d point1 = pNewMesh->pmesh->point(pNewMesh->pmesh->vertex_handle(index));
				outfile << index << " " << point1[0] << " " << point1[1] << "\n";
			}
		}
		outfile << -1;
		outfile.close();

		CMesh3d_OpenMesh* pNewMesh1 = pTeichmullerMap->ComputeOptimalTeichurmullerMap(m_ArrayMesh[i - 1], "D:\\constraints.txt");
		pNewMesh1->m_Transform.Copy(&pMesh1->m_Transform);
		m_ArrayMesh[i] = pNewMesh1;
		pDoc->m_SceneGraph.Add(pNewMesh1);
		break;
		//Save the constraints
	}
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_HarmonicMapping::GenerateIntermediateMesh_()
{
	pPolyOmino->OnBnClickedOminoLoadusamap();
	pPolyOmino->OnBnClickedOminoLoadusastreighten();

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);


	CMesh3d_OpenMesh* pMesh1_Openmesh = pMesh1->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh2_Openmesh = pMesh2->Convert_OpenMesh();

	//Save constraints
	std::ofstream outfile("D:\\constraints.txt");
	for (MyMesh::VertexIter v_it = pMesh2_Openmesh->pmesh->vertices_begin(); v_it != pMesh2_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		if (pMesh2_Openmesh->pmesh->is_boundary(v_it.handle()))
		{
			outfile << v_it.handle().idx();
			OpenMesh::Vec3d point1 = pMesh2_Openmesh->pmesh->point(v_it.handle());
			outfile << " " << point1[0] << " " << point1[1] << "\n";
		}

	}
	//
	for (int j = 0; j < pMesh1_Openmesh->m_LandMarkArray.size(); j++)
	{
		GeoMapLandMark* pLandmark = pMesh1->m_LandMarkArray[j];
		for (int jj = 0; jj < pLandmark->landmark_points.size(); jj++)
		{
			int index = pLandmark->landmark_points[jj];
			OpenMesh::Vec3d point1 = pMesh2_Openmesh->pmesh->point(pMesh2_Openmesh->pmesh->vertex_handle(index));
			outfile << index << " " << point1[0] << " " << point1[1] << "\n";
		}
	}
	outfile << -1;
	outfile.close();

	CMesh3d_OpenMesh* pNewMesh1 = pTeichmullerMap->ComputeOptimalTeichurmullerMap(pMesh1_Openmesh, "D:\\constraints.txt");
	pNewMesh1->m_Transform.Copy(&pMesh1->m_Transform);
	pDoc->m_SceneGraph.Add(pNewMesh1);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_HarmonicMapping::SetMeshParameterUV(CMesh3d_OpenMesh* pMesh, CMesh3d_OpenMesh* pParameter)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uv;
	for (MyMesh::VertexIter v_it = pMesh->pmesh->vertices_begin(); v_it != pMesh->pmesh->vertices_end(); ++v_it)
	{
		uv.clear();
		OpenMesh::Vec3d pt;
		pt = pParameter->pmesh->point(v_it);
		uv.push_back(pt[0]);
		uv.push_back(pt[1]);
		pMesh->pmesh->property(vprop_vertex_uv, v_it.handle()) = uv;
	}
}
void Actions_HarmonicMapping::OptimizeUsingConstraints()
{
	pPolyOmino->OnBnClickedOminoLoadusamap();
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonictest\\result\\1.obj");

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d_OpenMesh* pMesh1_Openmesh1 = pMesh1->Convert_OpenMesh();
	pMesh1_Openmesh1->m_Transform.Copy(&pMesh1->m_Transform);
	//CMesh3d_OpenMesh* pMesh1_Openmesh2 = pMesh1_Openmesh1->copy();
	pDoc->m_SceneGraph.Add(pMesh1_Openmesh1);
	//pDoc->m_SceneGraph.Add(pMesh1_Openmesh2);

	//CMesh3d_OpenMesh* pMesh2 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(1);
	//SetMeshParameterUV(pMesh1_Openmesh1,pMesh2);
	//pTeichmullerMap->ComputeMu(pMesh1_Openmesh1);

	//pTeichmullerMap->LoadConstraints(pMesh1_Openmesh2,"D:\\papers\\code\\data\\Geo\\harmonictest\\result\\constraints.txt");
	//pTeichmullerMap->ComputeMu(pMesh1_Openmesh2);

	CMesh3d_OpenMesh* pNewMesh1 = pTeichmullerMap->ComputeOptimalTeichurmullerMap(pMesh1_Openmesh1, "D:\\papers\\code\\data\\Geo\\harmonictest\\result\\constraints.txt");
	pNewMesh1->m_Transform.Copy(&pMesh1->m_Transform);
	pDoc->m_SceneGraph.Add(pNewMesh1);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
extern int main_();
void Actions_HarmonicMapping::OnBnClickedButtonLoadExamplewithSingulairty()
{
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonictest\\horizontalwithsingulairty\\HorizontalConstraints_Withsingularitiesbak.m");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	pPolyOmino->OnBnClickedGeneratelandmarks();
	//CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	//CMesh3d_OpenMesh* pMesh1_Openmesh1 = pMesh1->Convert_OpenMesh();
	//pMesh1_Openmesh1->m_Transform.Copy(pMesh1->GetTransform());
	//pDoc->m_SceneGraph.Add(pMesh1_Openmesh1);
	//ComputeDynamicHarmonic_LinearConstraints_Test(pMesh1_Openmesh1);
	//int flag = IsMeshFlip(pMesh1_Openmesh1);
	//pMesh1_Openmesh1->m_Modified = 1;

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\harmonictest\\horizontalwithsingulairty\\landmark1.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_HarmonicMapping::OnBnClickedButtonLoadtest()
{
	//main_();
	//return;
	// TODO: Add your control notification handler code here
	//GenerateIntermediateMesh_();
	//return;
	//OptimizeUsingConstraints();
	//return;
	//GenerateIntermediateMesh();
	//return;
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonictest\\2.m");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	pPolyOmino->OnBnClickedGeneratelandmarks();
	//CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	//CMesh3d_OpenMesh* pMesh1_Openmesh1 = pMesh1->Convert_OpenMesh();
	//pMesh1_Openmesh1->m_Transform.Copy(pMesh1->GetTransform());
	//pDoc->m_SceneGraph.Add(pMesh1_Openmesh1);
	//ComputeDynamicHarmonic_LinearConstraints_Test(pMesh1_Openmesh1);
	//int flag = IsMeshFlip(pMesh1_Openmesh1);
	//pMesh1_Openmesh1->m_Modified = 1;

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\harmonictest\\landmark2.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void Actions_HarmonicMapping::OnBnClickedButtonDynamicHarmonic()
{
	// TODO: Add your control notification handler code here
	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = 0;

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();
		pOpenMesh->m_FixVerticesArray.push_back(0);
		pOpenMesh->m_FixVerticesArray.push_back(1);
		pOpenMesh->m_FixVerticesArray.push_back(2);
		pOpenMesh->m_FixVerticesArray.push_back(3);
		pOpenMesh->m_Transform.Copy(*pMesh3D->GetTransform());
		pDoc->m_SceneGraph.Add(pOpenMesh);

		ComputeDynamicHarmonic_LinearConstraints(pOpenMesh);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
		{
			CMesh3d_OpenMesh* pOpenMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
			pOpenMesh->m_FixVerticesArray.clear();
			pOpenMesh->m_FixVerticesArray.push_back(0);
			pOpenMesh->m_FixVerticesArray.push_back(1);
			pOpenMesh->m_FixVerticesArray.push_back(2);
			pOpenMesh->m_FixVerticesArray.push_back(3);

			ComputeDynamicHarmonic_LinearConstraints(pOpenMesh);
			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);
			currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		}
}


void Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonicInitialize()
{
	// TODO: Add your control notification handler code here
	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = 0;
	iternum = 0;
	if (currentindex < 0 || currentindex >= num)
		return;
	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();
		pOpenMesh->m_FixVerticesArray.push_back(0);
		pOpenMesh->m_FixVerticesArray.push_back(1);
		pOpenMesh->m_FixVerticesArray.push_back(2);
		pOpenMesh->m_FixVerticesArray.push_back(3);
		pOpenMesh->m_Transform.Copy(*pMesh3D->GetTransform());
		pDoc->m_SceneGraph.Add(pOpenMesh);

		//ComputeDynamicHarmonic_LinearConstraints(pOpenMesh);

		VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
		VPROP_VERTEX_UV vprop_vertex_uv;

		//Vertex selected flag;
		if (!pOpenMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
			pOpenMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

		//Vertex UV
		if (!pOpenMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
			pOpenMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

		VPROP_VERTEX_MU vprop_vertex_mu;
		FPROP_FACE_MU fprop_face_mu;

		//Vertex MU
		if (!pOpenMesh->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
			pOpenMesh->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

		//Face MU
		if (!pOpenMesh->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
			pOpenMesh->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

		//pMesh3D->pmesh->property(vprop_vertex_uv).set_persistent(true);
		//pMesh3D->pmesh->property(vprop_selectedflag).set_persistent(true);
		//pMesh3D->pmesh->property(fprop_face_mu).set_persistent(true);
		//pMesh3D->pmesh->property(vprop_vertex_mu).set_persistent(true);

		//first fix the boundary
		Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pOpenMesh);

		//
		InitializeMeshFaceSelectedFlag(pOpenMesh);

		//
		InitializeMeshOriginalLocation(pOpenMesh);

		//
		InitializeMeshMu(pOpenMesh);

		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
}


void Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonicIteration()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(1);

	InitializeMeshFaceSelectedFlag(pMesh3D);

	//Obtain the linear equations
	Compute_Linear_Equations_LinearConstraints(pMesh3D, 0);

	if (!IsMeshUVFlip(pMesh3D, iternum, 1))
		AfxMessageBox("Done");

	//Generate a new parameter domain mesh
	GenerateParameterDomain_LinearConstaints(pMesh3D);

	//break;
	//if(iternum == 1)
	//	break;
	//Get the mesh without flip
	//GetMeshfreeofFlip(pMesh3D, iternum);
	GetMeshfreeofFlip(pMesh3D, iternum);
	//break;
	//pMesh3D = newmesh;
	//TRACE("%d \n",iternum);
	//break;

	//Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pMesh3D);

	//
	//InitializeMeshMu(pMesh3D);
	//break;

	//Compute the mu, quasi conformal 
	//Compute_Mu(pMesh3D);

	//GetMaxMinmuLength_Vertex(pMesh3D,maxmulength,minmulength,averagemulength,standarddeviation);
	//TRACE("%5d, Mu: %11.6f %11.6f %11.6f %11.6f\n",iternum, maxmulength,minmulength,averagemulength,standarddeviation);
	//Mufile << iternum << " " << maxmulength << " " << minmulength << " " << averagemulength << " " << standarddeviation << "\n";
	//Mufile.flush();
	//if(iternum > 0 && maxmulength > premaxmulength)
	//	break;

	//sprintf(filename,"D:\\%d.obj",iternum);

	//
	//SaveParameterDomainMesh(pMesh3D,filename);
	//break;

	//premaxmulength = maxmulength;

	//if(fabs(maxmulength - minmulength) < 0.01)
	//	break;
	//char filename[100];
	//sprintf(filename, "D:\\%d.obj", iternum);
	//OpenMesh::IO::write_mesh(*pMesh3D->pmesh, filename);
	//double len = GetMaxLandmarkDeviation(pMesh3D);
	//Mufile << iternum << " " << len << "\n";
	//Mufile.flush();
	//if (len < 1e-5)
	//	break;

	iternum++;
	//pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonicFreeofflip()
{
	// TODO: Add your control notification handler code here
	CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(1);
	GetMeshfreeofFlip(pMesh3D, iternum);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonMeshSwapsequence()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonicgenerateddata\\nonflipresult\\original_withresulttopology.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonicgenerateddata\\nonflipresult\\result_nonflip.obj");

	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(2);

	CMesh3d_OpenMesh* pMesh0_Openmesh = pMesh0->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh1_Openmesh = pMesh1->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh2_Openmesh = pMesh2->Convert_OpenMesh();
	pMesh0_Openmesh->m_Transform.Copy(*pMesh0->GetTransform());
	pDoc->m_SceneGraph.Add(pMesh0_Openmesh);
	pMesh1_Openmesh->m_Transform.Copy(*pMesh0->GetTransform());
	pDoc->m_SceneGraph.Add(pMesh1_Openmesh);
	pMesh2_Openmesh->m_Transform.Copy(*pMesh0->GetTransform());
	pDoc->m_SceneGraph.Add(pMesh2_Openmesh);
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> m_ArrayEdgeSwap;

	EPROP_EDGE_SELECTED_FLAG eprop_selectedflag;
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	//Edge Selected Flag;
	if (!pMesh0_Openmesh->pmesh->get_property_handle(eprop_selectedflag, PROP_EDGE_SELECTED))
		pMesh0_Openmesh->pmesh->add_property(eprop_selectedflag, PROP_EDGE_SELECTED);

	//Vertex Selected Flag
	if (!pMesh0_Openmesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pMesh0_Openmesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Initialize the edge select flag
	for (MyMesh::EdgeIter e_it = pMesh0_Openmesh->pmesh->edges_begin(); e_it != pMesh0_Openmesh->pmesh->edges_end(); ++e_it)
	{
		pMesh0_Openmesh->pmesh->property(eprop_selectedflag, e_it.handle()) = 0;
	}

	for (MyMesh::VertexIter v_it = pMesh0_Openmesh->pmesh->vertices_begin(); v_it != pMesh0_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		pMesh0_Openmesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}

	//label the edge flag for all the landamrk edges
	for (int i = 0; i < pMesh0->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh0->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size() - 1; j++)
		{
			int vindex1 = pLandmark->landmark_points[j];
			int vindex2 = pLandmark->landmark_points[j + 1];

			for (MyMesh::VertexEdgeIter ve_it = pMesh0_Openmesh->pmesh->ve_iter(pMesh0_Openmesh->pmesh->vertex_handle(vindex1)); ve_it; ++ve_it)
			{
				MyMesh::EdgeHandle eh = ve_it.handle();

				MyMesh::HalfedgeHandle hh0 = pMesh0_Openmesh->pmesh->halfedge_handle(eh, 0);
				MyMesh::HalfedgeHandle hh1 = pMesh0_Openmesh->pmesh->halfedge_handle(eh, 1);

				int vh1 = pMesh0_Openmesh->pmesh->to_vertex_handle(hh0).idx();
				int vh2 = pMesh0_Openmesh->pmesh->from_vertex_handle(hh0).idx();

				if (vh1 == vindex1 && vh2 == vindex2 || vh1 == vindex2 && vh2 == vindex1)
				{
					pMesh0_Openmesh->pmesh->property(eprop_selectedflag, eh) = 1;
					break;
				}
			}
		}
	}

	//search for all the swap edges
	//1:The edge doesnot exist in pMesh1_Openmesh
	//2:the swap edge exist in pMesh1_Openmesh
	int iterationflag = 1;
	while (1)
	{
		iterationflag = 1;
		for (MyMesh::EdgeIter e_it = pMesh0_Openmesh->pmesh->edges_begin(); e_it != pMesh0_Openmesh->pmesh->edges_end(); ++e_it)
		{
			//boundary edges
			MyMesh::EdgeHandle eh = e_it.handle();
			int flag = pMesh0_Openmesh->pmesh->is_boundary(eh);
			if (flag)
				continue;

			//constrained edges
			int flag1 = pMesh0_Openmesh->pmesh->property(eprop_selectedflag, eh);
			if (flag1)
				continue;

			MyMesh::HalfedgeHandle hh0 = pMesh0_Openmesh->pmesh->halfedge_handle(eh, 0);
			MyMesh::HalfedgeHandle hh1 = pMesh0_Openmesh->pmesh->halfedge_handle(eh, 1);
			MyMesh::FaceHandle fh0 = pMesh0_Openmesh->pmesh->face_handle(hh0);
			MyMesh::FaceHandle fh1 = pMesh0_Openmesh->pmesh->face_handle(hh1);
			MyMesh::VertexHandle vh1 = pMesh0_Openmesh->pmesh->to_vertex_handle(hh0);
			MyMesh::VertexHandle vh2 = pMesh0_Openmesh->pmesh->from_vertex_handle(hh0);

			int edgeflag = pMesh1_Openmesh->IsExistEdge(vh1.idx(), vh2.idx());
			if (!edgeflag)
			{
				int vindex3, vindex4;
				pMesh0_Openmesh->GetEdgeOppositeVertices(eh, vindex3, vindex4);
				int swapedgeflag = pMesh1_Openmesh->IsExistEdge(vindex3, vindex4);
				if (swapedgeflag)
				{
					pMesh0_Openmesh->pmesh->property(vprop_selectedflag, vh1) = 1;
					pMesh0_Openmesh->pmesh->property(vprop_selectedflag, vh2) = 1;
					pMesh0_Openmesh->pmesh->property(vprop_selectedflag, pMesh0_Openmesh->pmesh->vertex_handle(vindex3)) = 1;
					pMesh0_Openmesh->pmesh->property(vprop_selectedflag, pMesh0_Openmesh->pmesh->vertex_handle(vindex4)) = 1;

					iterationflag = 0;
					m_ArrayEdgeSwap.push_back(std::make_pair(std::make_pair(vh1.idx(), vh2.idx()), std::make_pair(vindex3, vindex4)));
					//delete two faces
					pMesh0_Openmesh->pmesh->delete_face(fh0, FALSE);
					pMesh0_Openmesh->pmesh->delete_face(fh1, FALSE);

					std::vector<MyMesh::VertexHandle>  face_vhandles;
					face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(vh1.idx()));
					face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(vindex3));
					face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(vindex4));
					MyMesh::FaceHandle fh2 = pMesh0_Openmesh->pmesh->add_face(face_vhandles);
					face_vhandles.clear();
					face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(vindex3));
					face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(vh2.idx()));
					face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(vindex4));
					MyMesh::FaceHandle fh3 = pMesh0_Openmesh->pmesh->add_face(face_vhandles);
					pMesh0_Openmesh->pmesh->garbage_collection();
					pMesh0_Openmesh->m_Modified = 1;
					break;
				}
			}
			else
				pMesh0_Openmesh->pmesh->property(eprop_selectedflag, eh) = 1;
		}
		if (iterationflag)
			break;
	}

	//save to file
	char swapinfo[80];
	sprintf(swapinfo, "D:\\papers\\code\\data\\Geo\\harmonicgenerateddata\\nonflipresult\\swapinfo.txt");
	std::ofstream swapfile(swapinfo);
	int flag;
	for (int i = 0; i < m_ArrayEdgeSwap.size(); i++)
	{
		swapfile << m_ArrayEdgeSwap[i].first.first << " ";
		swapfile << m_ArrayEdgeSwap[i].first.second << " ";
		swapfile << m_ArrayEdgeSwap[i].second.first << " ";
		swapfile << m_ArrayEdgeSwap[i].second.second << " ";
		swapfile << "\n";
	}
	swapfile.close();
	//Get the swap sequence in the m_ArrayEdgeSwap
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonMeshModifytopology()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonicgenerateddata\\nonflipresult\\original_withresulttopology.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonicgenerateddata\\nonflipresult\\result_nonflip.obj");

	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(2);
	pMesh2->m_LandMarkArray = pMesh0->m_LandMarkArray;
	CMesh3d_OpenMesh* pMesh0_Openmesh = pMesh0->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh1_Openmesh = pMesh1->Convert_OpenMesh();
	CMesh3d_OpenMesh* pMesh2_Openmesh = pMesh2->Convert_OpenMesh();
	pMesh0_Openmesh->m_Transform.Copy(*pMesh0->GetTransform());
	pDoc->m_SceneGraph.Add(pMesh0_Openmesh);
	pMesh1_Openmesh->m_Transform.Copy(*pMesh0->GetTransform());
	pDoc->m_SceneGraph.Add(pMesh1_Openmesh);
	pMesh2_Openmesh->m_Transform.Copy(*pMesh0->GetTransform());
	pDoc->m_SceneGraph.Add(pMesh2_Openmesh);
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> m_ArrayEdgeSwap;

	EPROP_EDGE_SELECTED_FLAG eprop_selectedflag;
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag0, vprop_selectedflag2;
	FPROP_FACE_SELECTED_FLAG fprop_selectedflag0, fprop_selectedflag2;
	//Edge Selected Flag;
	if (!pMesh0_Openmesh->pmesh->get_property_handle(eprop_selectedflag, PROP_EDGE_SELECTED))
		pMesh0_Openmesh->pmesh->add_property(eprop_selectedflag, PROP_EDGE_SELECTED);

	//Vertex Selected Flag
	if (!pMesh0_Openmesh->pmesh->get_property_handle(vprop_selectedflag0, PROP_VERTEX_SELECTED))
		pMesh0_Openmesh->pmesh->add_property(vprop_selectedflag0, PROP_VERTEX_SELECTED);

	//Vertex Selected Flag
	if (!pMesh2_Openmesh->pmesh->get_property_handle(vprop_selectedflag2, PROP_VERTEX_SELECTED))
		pMesh2_Openmesh->pmesh->add_property(vprop_selectedflag2, PROP_VERTEX_SELECTED);

	//face selected flag
	if (!pMesh2_Openmesh->pmesh->get_property_handle(fprop_selectedflag2, PROP_FACE_SELECTED))
		pMesh2_Openmesh->pmesh->add_property(fprop_selectedflag2, PROP_FACE_SELECTED);

	//face selected flag
	if (!pMesh0_Openmesh->pmesh->get_property_handle(fprop_selectedflag0, PROP_FACE_SELECTED))
		pMesh0_Openmesh->pmesh->add_property(fprop_selectedflag0, PROP_FACE_SELECTED);

	//Initialize the edge select flag
	for (MyMesh::EdgeIter e_it = pMesh0_Openmesh->pmesh->edges_begin(); e_it != pMesh0_Openmesh->pmesh->edges_end(); ++e_it)
	{
		pMesh0_Openmesh->pmesh->property(eprop_selectedflag, e_it.handle()) = 0;
	}

	for (MyMesh::VertexIter v_it = pMesh0_Openmesh->pmesh->vertices_begin(); v_it != pMesh0_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, v_it.handle()) = 0;
	}

	for (MyMesh::FaceIter f_it = pMesh2_Openmesh->pmesh->faces_begin(); f_it != pMesh2_Openmesh->pmesh->faces_end(); ++f_it)
	{
		pMesh2_Openmesh->pmesh->property(fprop_selectedflag2, f_it.handle()) = 0;
	}
	for (MyMesh::FaceIter f_it = pMesh0_Openmesh->pmesh->faces_begin(); f_it != pMesh0_Openmesh->pmesh->faces_end(); ++f_it)
	{
		pMesh0_Openmesh->pmesh->property(fprop_selectedflag0, f_it.handle()) = 0;
	}
	char swapinfo[80];

	sprintf(swapinfo, "D:\\papers\\code\\data\\Geo\\harmonicgenerateddata\\nonflipresult\\swapinfo.txt");
	std::ifstream swapfile(swapinfo);
	int index1, index2, index3, index4;
	while (!swapfile.eof())
	{
		swapfile >> index1 >> index2 >> index3 >> index4;
		m_ArrayEdgeSwap.push_back(std::make_pair(std::make_pair(index1, index2), std::make_pair(index3, index4)));
	}
	swapfile.close();

	//Modify topology
	for (int i = 0; i < m_ArrayEdgeSwap.size(); i++)
	{
		index1 = m_ArrayEdgeSwap[i].first.first;
		index2 = m_ArrayEdgeSwap[i].first.second;
		index3 = m_ArrayEdgeSwap[i].second.first;
		index4 = m_ArrayEdgeSwap[i].second.second;

		//pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index1)) = 1;
		//pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index2)) = 1;
		//pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index3)) = 1;
		//pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index4)) = 1;

		//pMesh2_Openmesh->pmesh->property(vprop_selectedflag2, pMesh2_Openmesh->pmesh->vertex_handle(index1)) = 1;
		//pMesh2_Openmesh->pmesh->property(vprop_selectedflag2, pMesh2_Openmesh->pmesh->vertex_handle(index2)) = 1;
		//pMesh2_Openmesh->pmesh->property(vprop_selectedflag2, pMesh2_Openmesh->pmesh->vertex_handle(index3)) = 1;
		//pMesh2_Openmesh->pmesh->property(vprop_selectedflag2, pMesh2_Openmesh->pmesh->vertex_handle(index4)) = 1;

		//continue;
		//1 3 4 , 3 2 4
		double area0 = pMesh0_Openmesh->GetTriangleSignArea_2D(index1, index3, index4);
		double area1 = pMesh0_Openmesh->GetTriangleSignArea_2D(index3, index2, index4);

		if (area0 <= 0 || area1 <= 0)
		{
			pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index1)) = 1;
			pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index2)) = 1;
			pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index3)) = 1;
			pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index4)) = 1;

			pMesh2_Openmesh->pmesh->property(vprop_selectedflag2, pMesh2_Openmesh->pmesh->vertex_handle(index1)) = 1;
			pMesh2_Openmesh->pmesh->property(vprop_selectedflag2, pMesh2_Openmesh->pmesh->vertex_handle(index2)) = 1;
			pMesh2_Openmesh->pmesh->property(vprop_selectedflag2, pMesh2_Openmesh->pmesh->vertex_handle(index3)) = 1;
			pMesh2_Openmesh->pmesh->property(vprop_selectedflag2, pMesh2_Openmesh->pmesh->vertex_handle(index4)) = 1;

			int edgeindex = pMesh0_Openmesh->GetEdgeHandleIdx(index1, index2);
			MyMesh::EdgeHandle eh = pMesh0_Openmesh->pmesh->edge_handle(edgeindex);
			//modify index3 index4
			pMesh0_Openmesh->GetEdgeOppositeVertices(eh, index3, index4);

			//Delete the original two faces and ad four new faces.
			pMesh0_Openmesh->splitEdge(index1, index2);
			pMesh2_Openmesh->splitEdge(index3, index4);
			//break;
		}
		else
		{
			//pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index1)) = 1;
			//pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index2)) = 1;
			//pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index3)) = 1;
			//pMesh0_Openmesh->pmesh->property(vprop_selectedflag0, pMesh0_Openmesh->pmesh->vertex_handle(index4)) = 1;

			int edgeindex = pMesh0_Openmesh->GetEdgeHandleIdx(index1, index2);
			if (edgeindex < 0)
			{
				char message[80];
				sprintf(message, "Edge(%d,%d) not found!", index1, index2);
				AfxMessageBox(message);
				break;
			}

			ASSERT(edgeindex >= 0);
			MyMesh::EdgeHandle eh = pMesh0_Openmesh->pmesh->edge_handle(edgeindex);
			//modify index3 index4
			pMesh0_Openmesh->GetEdgeOppositeVertices(eh, index3, index4);

			MyMesh::HalfedgeHandle hh0 = pMesh0_Openmesh->pmesh->halfedge_handle(eh, 0);
			MyMesh::HalfedgeHandle hh1 = pMesh0_Openmesh->pmesh->halfedge_handle(eh, 1);
			MyMesh::FaceHandle fh0 = pMesh0_Openmesh->pmesh->face_handle(hh0);
			MyMesh::FaceHandle fh1 = pMesh0_Openmesh->pmesh->face_handle(hh1);

			int edgeindex_ = pMesh2_Openmesh->GetEdgeHandleIdx(index3, index4);
			MyMesh::EdgeHandle eh_ = pMesh2_Openmesh->pmesh->edge_handle(edgeindex_);
			MyMesh::HalfedgeHandle hh0_ = pMesh2_Openmesh->pmesh->halfedge_handle(eh_, 0);
			MyMesh::HalfedgeHandle hh1_ = pMesh2_Openmesh->pmesh->halfedge_handle(eh_, 1);
			MyMesh::FaceHandle fh0_ = pMesh2_Openmesh->pmesh->face_handle(hh0_);
			MyMesh::FaceHandle fh1_ = pMesh2_Openmesh->pmesh->face_handle(hh1_);
			pMesh2_Openmesh->pmesh->property(fprop_selectedflag2, fh0_) = 1;
			pMesh2_Openmesh->pmesh->property(fprop_selectedflag2, fh1_) = 1;

			//label the another faces.

			//m_ArrayEdgeSwap.push_back(std::make_pair(std::make_pair(vh1.idx(), vh2.idx()), std::make_pair(vindex3, vindex4)));
			//delete two faces
			pMesh0_Openmesh->pmesh->delete_face(fh0, FALSE);
			pMesh0_Openmesh->pmesh->delete_face(fh1, FALSE);

			std::vector<MyMesh::VertexHandle>  face_vhandles;
			face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(index1));
			face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(index3));
			face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(index4));
			MyMesh::FaceHandle fh2 = pMesh0_Openmesh->pmesh->add_face(face_vhandles);
			face_vhandles.clear();
			face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(index3));
			face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(index2));
			face_vhandles.push_back(pMesh0_Openmesh->pmesh->vertex_handle(index4));
			MyMesh::FaceHandle fh3 = pMesh0_Openmesh->pmesh->add_face(face_vhandles);

			pMesh0_Openmesh->pmesh->property(fprop_selectedflag0, fh2) = 1;
			pMesh0_Openmesh->pmesh->property(fprop_selectedflag0, fh3) = 1;
			pMesh0_Openmesh->pmesh->garbage_collection();
			pMesh0_Openmesh->m_Modified = 1;
		}
	}
	char filename[100];
	sprintf(filename, "D:\\original.obj");
	OpenMesh::IO::write_mesh(*pMesh0_Openmesh->pmesh, filename);

	sprintf(filename, "D:\\result.obj");
	OpenMesh::IO::write_mesh(*pMesh2_Openmesh->pmesh, filename);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonGenerategeogiageboundary()
{
	// TODO: Add your control notification handler code here
	char boundaryinfo[80];

	sprintf(boundaryinfo, "D:\\papers\\code\\data\\Geo\\Geogiaboundary.txt");
	std::ifstream boundaryfile(boundaryinfo);
	std::vector<int> m_ArrayBoundaryedges;
	int index1, index2, index3, index4;
	while (!boundaryfile.eof())
	{
		boundaryfile >> index1 >> index2;
		m_ArrayBoundaryedges.push_back(index1);
	}
	boundaryfile.close();

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d_OpenMesh* pMesh_Openmesh = pMesh->Convert_OpenMesh();
	MyMesh* newmesh = new MyMesh(*pMesh_Openmesh->pmesh);

	CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pNewMesh->pmesh = newmesh;
	pNewMesh->m_Transform.Copy(&pMesh->m_Transform);

	for (int i = 0; i < m_ArrayBoundaryedges.size() - 1; i++)
	{
		pNewMesh->m_ArraySharpEdges.push_back(std::make_pair(m_ArrayBoundaryedges[i], m_ArrayBoundaryedges[i + 1]));
	}

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	pDoc->m_SceneGraph.Add(pNewMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

double Actions_HarmonicMapping::GetEdgeLength(CMesh3d_OpenMesh* pOpenMesh1, int bindex1, int bindex3)
{
	double totallen = 0;
	for (int i = bindex1; i < bindex3; i++)
	{
		int index1 = m_ArrayBoundarylandmarkpoints[i];
		int index2 = m_ArrayBoundarylandmarkpoints[(i + 1) % m_ArrayBoundarylandmarkpoints.size()];

		OpenMesh::Vec3d v1 = pOpenMesh1->pmesh->point(pOpenMesh1->pmesh->vertex_handle(index1));
		OpenMesh::Vec3d v2 = pOpenMesh1->pmesh->point(pOpenMesh1->pmesh->vertex_handle(index2));

		totallen += Length(v1 - v2);
		//i = (i + 1) % m_ArrayBoundarylandmarkpoints.size();
	}
	return totallen;
}
void Actions_HarmonicMapping::ChangeVertexIndex()
{
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\georgia\\georgia_county_normalized_33_simplified_cornerflip.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\georgia\\georgia_county_normalized_33_simplified_cornerflip_removeoutertrianggle.obj");
	pDoc->OnOpenDocument("D:\\georgia1.obj");
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);

	CMesh3d_OpenMesh* pOpenMesh0 = pMesh0->Convert_OpenMesh();
	CMesh3d_OpenMesh* pOpenMesh1 = pMesh1->Convert_OpenMesh();
	int n;
	for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
	{
		n = 0;
		for (MyMesh::VertexVertexIter vv_it = pOpenMesh1->pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
		{
			n++;
		}
		if (n == 0)
		{
			pOpenMesh1->pmesh->delete_vertex(v_it.handle());
		}
	}
	pOpenMesh1->pmesh->garbage_collection();
	int n1 = pOpenMesh0->pmesh->n_vertices();
	int n2 = pOpenMesh1->pmesh->n_vertices();

	pMesh1 = pOpenMesh1->ConvertMesh3d();

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\landmark42.txt");
	pPolyOmino->pMesh = pMesh0;
	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);

	for (int i = 0; i < pMesh0->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh0->GetVertex(i);
		pVertex->m_Flag = -1;
	}
	for (int i = 0; i < pMesh1->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh1->GetVertex(i);
		double mindis = 1e10;
		CVertex3d* pMinVertex;
		for (int j = 0; j < pMesh0->NbVertex(); j++)
		{
			CVertex3d* pVertex1 = pMesh0->GetVertex(j);
			double dis = Distance(pVertex, pVertex1);
			if (dis < mindis)
			{
				mindis = dis;
				pMinVertex = pVertex1;
			}
		}
		pMinVertex->m_Flag = i;
	}

	for (int i = 0; i < pMesh0->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh0->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size(); j++)
		{
			int index = pLandmark->landmark_points[j];
			pLandmark->landmark_points[j] = pMesh0->GetVertex(index)->m_Flag;
		}
	}

	pPolyOmino->SaveLandmark("D:\\landmark.txt");

	sprintf(message, "D:\\papers\\code\\data\\Geo\\Geogiaboundary.txt");
	std::ifstream boundaryfile1(message);
	std::vector<int> m_ArrayBoundarypointspair;
	int index1, index2, index3, index4;
	while (!boundaryfile1.eof())
	{
		boundaryfile1 >> index1 >> index2;
		index2 = pMesh0->GetVertex(index2)->m_Flag;
		m_ArrayBoundarypointspair.push_back(index1);
		m_ArrayBoundarypointspair.push_back(index2);
	}
	boundaryfile1.close();

	sprintf(message, "D:\\Geogiaboundary1.txt");
	std::ofstream boundaryfile2(message);

	for (int i = 0; i < m_ArrayBoundarypointspair.size() / 2; i++)
	{
		boundaryfile2 << m_ArrayBoundarypointspair[i * 2] << " " << m_ArrayBoundarypointspair[i * 2 + 1] << "\n";
	}
	boundaryfile2.close();
}
void Actions_HarmonicMapping::OnBnClickedButtonGenerategeogiagehybrid()
{
	// TODO: Add your control notification handler code here
	//First input the files
	//ChangeVertexIndex();
	//return;
	{
		pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Georgia\\newboundary\\814.obj");
		CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
		pPolyOmino->pMesh = pMesh0;
		pPolyOmino->OnBnClickedOminoSetcurrentmesh();
		pPolyOmino->OnBnClickedGeneratelandmarks();

		//Load Landmarks
		char message[200];
		//sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\landmark42.txt");
		sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\newboundary\\landmark69.txt");
		CString landmarkname = message;
		pPolyOmino->Loadlandmark(landmarkname);

		/*sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\landmarindexonoriginalmesh.txt");
		std::ifstream boundaryfile(message);
		std::vector<int> m_ArrayBoundarylandmarks;
		int index1;
		while (!boundaryfile.eof())
		{
			boundaryfile >> index1;
			m_ArrayBoundarylandmarks.push_back(index1);
		}
		boundaryfile.close();
		for (int i = 0; i < m_ArrayBoundarylandmarks.size(); i++)
		{
			pMesh0->m_LandMarkArray[m_ArrayBoundarylandmarks[i]]->deleteflag = 1;
		}
		for (int i = 0; i < pMesh0->m_LandMarkArray.size(); i++)
		{
			if (pMesh0->m_LandMarkArray[i]->deleteflag)
			{
				pMesh0->m_LandMarkArray.erase(pMesh0->m_LandMarkArray.begin() + i);
				i--;
			}
		}*/

		return;
	}

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonicgenerateddata\\nonflipresult\\result_nonflip_removeflip.obj");

	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonicgenerateddata\\nonflipresult\\result_nonflip.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\georgia\\georgia_county_normalized_33_simplified_cornerflip.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\georgia\\georgia_county_normalized_33_simplified_cornerflip_removeoutertrianggle.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Georgia\\newboundary\\georgia2.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Georgia\\newboundary\\georgianewboundary.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Georgia\\newboundary\\georgianewboundary.obj");
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	//CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(2);

	CMesh3d_OpenMesh* pOpenMesh0 = pMesh0->Convert_OpenMesh();
	CMesh3d_OpenMesh* pOpenMesh1 = pMesh1->Convert_OpenMesh();
	int n;
	/*for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
	{
		n = 0;
		for (MyMesh::VertexVertexIter vv_it = pOpenMesh1->pmesh->vv_iter(v_it.handle()); vv_it; ++vv_it)
		{
			n++;
		}
		if (n == 0)
		{
			pOpenMesh1->pmesh->delete_vertex(v_it.handle());
		}
	}
	pOpenMesh1->pmesh->garbage_collection();*/
	//int n1 = pOpenMesh0->pmesh->n_vertices();
	//int n2 = pOpenMesh1->pmesh->n_vertices();
	//pOpenMesh1->m_Transform.Copy(*pMesh1->GetTransform());
	//pDoc->m_SceneGraph.Add(pOpenMesh1);
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	//return;
	//CMesh3d_OpenMesh* pOpenMesh2 = pMesh1->Convert_OpenMesh();

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->pMesh = pMesh1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\landmark42.txt");
	sprintf(message, "D:\\landmark.txt");
	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);

	sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\outerlandmarkindex.txt");
	std::ifstream boundaryfile(message);
	std::vector<int> m_ArrayBoundarylandmarks;
	int index1;
	while (!boundaryfile.eof())
	{
		boundaryfile >> index1;
		m_ArrayBoundarylandmarks.push_back(index1);
	}
	boundaryfile.close();

	//sprintf(message, "D:\\papers\\code\\data\\Geo\\Geogiaboundary.txt");
	//Set boundary vertices uv

	m_ArrayBoundarylandmarkpoints.clear();
	std::vector<int> m_ArrayBoundarylandmarksreverse;
	m_ArrayBoundarylandmarksreverse.resize(m_ArrayBoundarylandmarks.size());
	for (int i = 0; i < m_ArrayBoundarylandmarksreverse.size(); i++)
	{
		m_ArrayBoundarylandmarksreverse[i] = m_ArrayBoundarylandmarks[m_ArrayBoundarylandmarks.size() - 1 - i];
	}
	m_ArrayBoundarylandmarks = m_ArrayBoundarylandmarksreverse;
	int flag;
	for (int i = 0; i < m_ArrayBoundarylandmarks.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh1->m_LandMarkArray[m_ArrayBoundarylandmarks[i]];

		if (m_ArrayBoundarylandmarkpoints.size() == 0)
			m_ArrayBoundarylandmarkpoints.push_back(pLandmark->landmark_points[0]);

		if (pLandmark->landmark_points[0] == m_ArrayBoundarylandmarkpoints[m_ArrayBoundarylandmarkpoints.size() - 1])
		{
			for (int j = 1; j < pLandmark->landmark_points.size(); j++)
			{
				m_ArrayBoundarylandmarkpoints.push_back(pLandmark->landmark_points[j]);
			}
		}
		else if (pLandmark->landmark_points[pLandmark->landmark_points.size() - 1] == m_ArrayBoundarylandmarkpoints[m_ArrayBoundarylandmarkpoints.size() - 1])
		{
			for (int j = pLandmark->landmark_points.size() - 2; j >= 0; j--)
			{
				m_ArrayBoundarylandmarkpoints.push_back(pLandmark->landmark_points[j]);
			}
		}
		else if (pLandmark->landmark_points[0] == m_ArrayBoundarylandmarkpoints[0])
		{
			for (int j = 1; j < pLandmark->landmark_points.size(); j++)
			{
				m_ArrayBoundarylandmarkpoints.insert(m_ArrayBoundarylandmarkpoints.begin(), pLandmark->landmark_points[j]);
			}
		}
		else if (pLandmark->landmark_points[pLandmark->landmark_points.size() - 1] == m_ArrayBoundarylandmarkpoints[0])
		{
			for (int j = pLandmark->landmark_points.size() - 2; j >= 0; j--)
			{
				m_ArrayBoundarylandmarkpoints.insert(m_ArrayBoundarylandmarkpoints.begin(), pLandmark->landmark_points[j]);
			}
		}
		else
			ASSERT(0);
	}
	m_ArrayBoundarylandmarkpoints.erase(m_ArrayBoundarylandmarkpoints.begin() + m_ArrayBoundarylandmarkpoints.size() - 1);

	//
	SetBoundaryUV(pOpenMesh1);

	/*for (int i = 0; i < pPolyOmino->pMesh->m_LandMarkArray.size(); i++)
	{
		pPolyOmino->pMesh->m_LandMarkArray[i]->isVisible = FALSE;
	}
	for (int i = 0; i < m_ArrayBoundarylandmarks.size(); i++)
	{
		pPolyOmino->pMesh->m_LandMarkArray[m_ArrayBoundarylandmarks[i]]->isVisible = TRUE;
	}*/

	int currentindex = 1;
	//return;
	//if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		//CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		//CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();
		//pOpenMesh1->m_FixVerticesArray.push_back(0);
		//pOpenMesh1->m_FixVerticesArray.push_back(1);
		//pOpenMesh1->m_FixVerticesArray.push_back(2);
		//pOpenMesh1->m_FixVerticesArray.push_back(3);
		pOpenMesh1->m_Transform.Copy(*pMesh1->GetTransform());

		/*for (int i = 0; i < m_ArrayBoundarylandmarks.size(); i++)
		{
			GeoMapLandMark* pLandmark = pMesh1->m_LandMarkArray[m_ArrayBoundarylandmarks[i]];
			pLandmark->deleteflag = 1;
		}
		for (int i = 0; i < pMesh1->m_LandMarkArray.size(); i++)
		{
			GeoMapLandMark* pLandmark = pMesh1->m_LandMarkArray[i];
			if (pLandmark->deleteflag)
			{
				pMesh1->m_LandMarkArray.erase(pMesh1->m_LandMarkArray.begin() + i);
				i--;
			}
		}*/
		/*for (int i = 1; i < pMesh1->m_LandMarkArray.size(); i++)
		{
			GeoMapLandMark* pLandmark = pMesh1->m_LandMarkArray[i];
			pLandmark->isVisible = FALSE;
		}*/
		pOpenMesh1->m_LandMarkArray = pMesh1->m_LandMarkArray;
		//pOpenMesh1->m_LandMarkArray.push_back(pMesh1->m_LandMarkArray[0]);
		//pOpenMesh1->m_LandMarkArray.clear();
		pDoc->m_SceneGraph.Add(pOpenMesh1);

		//std::vector<double> uv = pOpenMesh1->pmesh->property(vprop_vertex_uv, pOpenMesh1->pmesh->vertex_handle(161));
		//ComputeDynamicHarmonic_LinearConstraints(pOpenMesh1);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	}
}
void Actions_HarmonicMapping::SetBoundaryUV(CMesh3d_OpenMesh* pOpenMesh1)
{
	std::vector<double> uv;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pOpenMesh1->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOpenMesh1->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	for (MyMesh::VertexIter v_it = pOpenMesh1->pmesh->vertices_begin(); v_it != pOpenMesh1->pmesh->vertices_end(); ++v_it)
	{
		if (pOpenMesh1->pmesh->is_boundary(v_it.handle()))
		{
			OpenMesh::Vec3d pos = pOpenMesh1->pmesh->point(v_it.handle());
			uv.clear();
			uv.push_back(pos[0]);
			uv.push_back(pos[1]);
			pOpenMesh1->pmesh->property(vprop_vertex_uv, v_it.handle()) = uv;
		}
	}
	return;
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	//CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(2);

	CMesh3d_OpenMesh* pOpenMesh0 = pMesh0->Convert_OpenMesh();

	char message[100];
	sprintf(message, "D:\\Geogiaboundary1.txt");
	std::ifstream boundaryfile1(message);
	std::vector<int> m_ArrayBoundarypointspair;
	int index1, index2, index3, index4;
	while (!boundaryfile1.eof())
	{
		boundaryfile1 >> index1 >> index2;
		m_ArrayBoundarypointspair.push_back(index1);
		m_ArrayBoundarypointspair.push_back(index2);
	}
	boundaryfile1.close();


	for (int i = 0; i < m_ArrayBoundarypointspair.size() - 2; i = i + 2)
	{
		index1 = m_ArrayBoundarypointspair[i];
		index2 = m_ArrayBoundarypointspair[i + 1];
		index3 = m_ArrayBoundarypointspair[i + 2];
		index4 = m_ArrayBoundarypointspair[i + 3];

		int bindex1 = std::find(m_ArrayBoundarylandmarkpoints.begin(), m_ArrayBoundarylandmarkpoints.end(), index2) - m_ArrayBoundarylandmarkpoints.begin();
		int bindex3 = std::find(m_ArrayBoundarylandmarkpoints.begin(), m_ArrayBoundarylandmarkpoints.end(), index4) - m_ArrayBoundarylandmarkpoints.begin();
		if (bindex3 == 0)
			bindex3 = m_ArrayBoundarylandmarkpoints.size();


		double edgelength = GetEdgeLength(pOpenMesh1, bindex1, bindex3);
		MyMesh::VertexHandle vhindex2 = pOpenMesh0->pmesh->vertex_handle(index1);
		MyMesh::VertexHandle vhindex4 = pOpenMesh0->pmesh->vertex_handle(index3);
		OpenMesh::Vec3d v2 = pOpenMesh0->pmesh->point(vhindex2);
		OpenMesh::Vec3d v4 = pOpenMesh0->pmesh->point(vhindex4);

		for (int j = bindex1; j <= bindex3; j++)
		{
			double currentlen = GetEdgeLength(pOpenMesh1, bindex1, j);
			double ratio = currentlen / edgelength;
			OpenMesh::Vec3d vv = v2 * (1 - ratio) + v4 * ratio;
			uv.clear();
			uv.push_back(vv[0]);
			uv.push_back(vv[1]);
			//if (m_ArrayBoundarylandmarkpoints[j] >= pOpenMesh1->pmesh->n_vertices())
			//	ASSERT(0);
			MyMesh::VertexHandle vhindex = pOpenMesh1->pmesh->vertex_handle(m_ArrayBoundarylandmarkpoints[j %  m_ArrayBoundarylandmarkpoints.size()]);
			pOpenMesh1->pmesh->property(vprop_vertex_uv, vhindex) = uv;
		}
	}
}

void Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark()
{
	// TODO: Add your control notification handler code here
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	if (pMesh0->m_SelectedLandmarkIndexArray.size() != 1)
		return;

	if (pMesh0->m_ArraySharpVertexIndex.size() != 1)
		return;

	pMesh0->SetFlagOnVertices(0);
	iternum++;
	pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0])->m_Flag = 1;

	int landmarkindex = pMesh0->m_SelectedLandmarkIndexArray[0];
	GeoMapLandMark* pLandmark = pMesh0->m_LandMarkArray[landmarkindex];
	int startvindex = pLandmark->landmark_points[0];
	int endvindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
	CVertex3d* pStartVertex = pMesh0->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh0->GetVertex(endvindex);
	CVertex3d* pCommonVertex;
	int style = pLandmark->style;
	int selectnum = 0;
	double x, y;
	if (pStartVertex->m_Flag && !pEndVertex->m_Flag)
	{
		selectnum++;
		x = pStartVertex->x();
		y = pStartVertex->y();
		pStartVertex->m_Flag = 0;
	}
	else if (!pStartVertex->m_Flag && pEndVertex->m_Flag)
	{
		selectnum++;
		x = pEndVertex->x();
		y = pEndVertex->y();
		pEndVertex->m_Flag = 0;
	}
	else
	{
		x = (pStartVertex->x() + pEndVertex->x()) / 2;
		y = (pStartVertex->y() + pEndVertex->y()) / 2;
		pEndVertex->m_Flag = 0;
		pStartVertex->m_Flag = 0;
	}


	if (style == 0)
	{
		for (int i = 0; i < pLandmark->landmark_points.size(); i++)
		{
			CVertex3d* pVertex = pMesh0->GetVertex(pLandmark->landmark_points[i]);
			pVertex->Set(pVertex->x(), y, pVertex->z());
		}
	}
	else
	{
		for (int i = 0; i < pLandmark->landmark_points.size(); i++)
		{
			CVertex3d* pVertex = pMesh0->GetVertex(pLandmark->landmark_points[i]);
			pVertex->Set(x, pVertex->y(), pVertex->z());
		}
	}

	char message[80];
	sprintf(message, "D:\\%d.obj", iternum);
	pMesh0->SaveOBJFile(message);

	pMesh0->m_SelectedLandmarkIndexArray.clear();
	pMesh0->m_ArraySharpVertexIndex.clear();
	pMesh0->SetModified(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark2()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	if (pMesh0->m_SelectedLandmarkIndexArray.size() != 1)
		return;
	if (pMesh0->m_ArraySharpVertexIndex.size() != 1)
		return;
	iternum++;
	pMesh0->SetFlagOnVertices(0);
	pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0])->m_Flag = 1;
	int landmarkindex = pMesh0->m_SelectedLandmarkIndexArray[0];
	GeoMapLandMark* pLandmark = pMesh0->m_LandMarkArray[landmarkindex];
	int startvindex = pLandmark->landmark_points[0];
	int endvindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
	CVertex3d* pStartVertex = pMesh0->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh0->GetVertex(endvindex);
	CVertex3d* pCommonVertex;
	int style = pLandmark->style;
	int selectnum = 0;
	double x, y;
	if (pStartVertex->m_Flag && !pEndVertex->m_Flag)
	{
		pStartVertex->m_Flag = 0;

		selectnum++;
		x = pStartVertex->x();
		y = pStartVertex->y();
		if (style == 0)
		{
			int sign = 1;
			if (pEndVertex->x() > pStartVertex->x())
				sign = 1;
			else
				sign = -1;

			double totallength = 0;
			for (int i = 1; i < pLandmark->landmark_points.size(); i++)
			{
				CVertex3d* pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				CVertex3d* pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				totallength += Distance(pVertex1, pVertex2);
				pVertex1->Set(pStartVertex->x() + totallength * sign, y, pVertex1->z());
			}
		}
		else
		{
			int sign = 1;
			if (pEndVertex->y() > pStartVertex->y())
				sign = 1;
			else
				sign = -1;

			double totallength = 0;
			for (int i = 1; i < pLandmark->landmark_points.size(); i++)
			{
				CVertex3d* pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				CVertex3d* pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				totallength += Distance(pVertex1, pVertex2);
				pVertex1->Set(x, pStartVertex->y() + totallength * sign, pVertex1->z());
			}
		}
	}
	else if (!pStartVertex->m_Flag && pEndVertex->m_Flag)
	{
		pEndVertex->m_Flag = 0;
		selectnum++;
		x = pEndVertex->x();
		y = pEndVertex->y();
		if (style == 0)
		{
			int sign = 1;
			if (pStartVertex->x() > pEndVertex->x())
				sign = 1;
			else
				sign = -1;

			double totallength = 0;
			for (int i = pLandmark->landmark_points.size() - 1; i > 0; i--)
			{
				CVertex3d* pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				CVertex3d* pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				totallength += Distance(pVertex1, pVertex2);
				pVertex2->Set(pEndVertex->x() + totallength * sign, y, pVertex2->z());
			}
		}
		else
		{
			int sign = 1;
			if (pStartVertex->y() > pEndVertex->y())
				sign = 1;
			else
				sign = -1;

			double totallength = 0;
			for (int i = pLandmark->landmark_points.size() - 1; i > 0; i--)
			{
				CVertex3d* pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				CVertex3d* pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				totallength += Distance(pVertex1, pVertex2);
				pVertex2->Set(x, pEndVertex->y() + totallength * sign, pVertex2->z());
			}
		}
	}
	char message[80];
	sprintf(message, "D:\\%d.obj", iternum);
	pMesh0->SaveOBJFile(message);
	pMesh0->m_SelectedLandmarkIndexArray.clear();
	pMesh0->m_ArraySharpVertexIndex.clear();
	pMesh0->SetModified(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark3()
{
	// TODO: Add your control notification handler code here
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	if (pMesh0->m_SelectedLandmarkIndexArray.size() != 1)
		return;
	if (pMesh0->m_ArraySharpVertexIndex.size() != 1)
		return;
	iternum++;
	pMesh0->SetFlagOnVertices(0);
	pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0])->m_Flag = 1;
	int landmarkindex = pMesh0->m_SelectedLandmarkIndexArray[0];
	GeoMapLandMark* pLandmark = pMesh0->m_LandMarkArray[landmarkindex];
	int startvindex = pLandmark->landmark_points[0];
	int endvindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
	CVertex3d* pStartVertex = pMesh0->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh0->GetVertex(endvindex);
	CVertex3d* pCommonVertex;
	int style = pLandmark->style;
	int selectnum = 0;
	double landmarklength = 0;
	for (int i = 0; i < pLandmark->landmark_points.size() - 1; i++)
	{
		CVertex3d* pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
		CVertex3d* pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i + 1]);
		landmarklength += Distance(pVertex1, pVertex2);
	}

	double x, y;
	double val;
	CVertex3d* pVertex1, *pVertex2;
	std::vector<double> m_ArrayVals;
	if (pStartVertex->m_Flag && !pEndVertex->m_Flag)
	{
		pStartVertex->m_Flag = 0;

		selectnum++;
		x = pStartVertex->x();
		y = pStartVertex->y();
		if (style == 0)
		{
			int sign = 1;
			//if (pEndVertex->x() > pStartVertex->x())
			//	sign = 1;
			//else
			//	sign = -1;

			double totallength = 0;
			for (int i = 1; i < pLandmark->landmark_points.size(); i++)
			{
				CVertex3d* pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				CVertex3d* pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				totallength += Distance(pVertex1, pVertex2);
				val = pStartVertex->x() + (pEndVertex->x() - pStartVertex->x()) * totallength * sign / landmarklength;
				m_ArrayVals.push_back(val);
				//pVertex1->Set(val, y, pVertex1->z());
			}
			for (int i = 1; i < pLandmark->landmark_points.size(); i++)
			{
				CVertex3d* pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				pVertex1->Set(m_ArrayVals[i - 1], y, pVertex1->z());
			}
		}
		else
		{
			int sign = 1;
			//if (pEndVertex->y() > pStartVertex->y())
			//	sign = 1;
			//else
			//	sign = -1;

			double totallength = 0;

			for (int i = 1; i < pLandmark->landmark_points.size(); i++)
			{
				pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				totallength += Distance(pVertex1, pVertex2);
				val = pStartVertex->y() + (pEndVertex->y() - pStartVertex->y()) * totallength * sign / landmarklength;
				m_ArrayVals.push_back(val);
				//pVertex1->Set(x, val, pVertex1->z());
			}

			for (int i = 1; i < pLandmark->landmark_points.size(); i++)
			{
				pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				pVertex1->Set(x, m_ArrayVals[i - 1], pVertex1->z());
			}
		}
	}
	else if (!pStartVertex->m_Flag && pEndVertex->m_Flag)
	{
		pEndVertex->m_Flag = 0;
		selectnum++;
		x = pEndVertex->x();
		y = pEndVertex->y();
		if (style == 0)
		{
			int sign = 1;
			//if (pStartVertex->x() > pEndVertex->x())
			//	sign = 1;
			//else
			//	sign = -1;

			double totallength = 0;

			for (int i = pLandmark->landmark_points.size() - 1; i > 0; i--)
			{
				pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				totallength += Distance(pVertex1, pVertex2);
				val = pEndVertex->x() + (pStartVertex->x() - pEndVertex->x()) * totallength * sign / landmarklength;
				m_ArrayVals.push_back(val);
				//pVertex2->Set(val, y, pVertex2->z());
			}
			for (int i = pLandmark->landmark_points.size() - 1; i > 0; i--)
			{
				pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				//m_ArrayVals.push_back(val);
				pVertex2->Set(m_ArrayVals[pLandmark->landmark_points.size() - 1 - i], y, pVertex2->z());
			}

		}
		else
		{
			int sign = 1;
			//if (pStartVertex->y() > pEndVertex->y())
			//	sign = 1;
			//else
			//	sign = -1;

			double totallength = 0;
			for (int i = pLandmark->landmark_points.size() - 1; i > 0; i--)
			{
				CVertex3d* pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				CVertex3d* pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				totallength += Distance(pVertex1, pVertex2);
				val = pEndVertex->y() + (pStartVertex->y() - pEndVertex->y()) * totallength * sign / landmarklength;
				m_ArrayVals.push_back(val);
				//pVertex2->Set(x, val, pVertex2->z());
			}

			for (int i = pLandmark->landmark_points.size() - 1; i > 0; i--)
			{
				CVertex3d* pVertex1 = pMesh0->GetVertex(pLandmark->landmark_points[i]);
				CVertex3d* pVertex2 = pMesh0->GetVertex(pLandmark->landmark_points[i - 1]);
				m_ArrayVals.push_back(val);
				pVertex2->Set(x, m_ArrayVals[pLandmark->landmark_points.size() - 1 - i], pVertex2->z());
			}
		}
	}
	char message[80];
	sprintf(message, "D:\\%d.obj", iternum);
	pMesh0->SaveOBJFile(message);
	pMesh0->m_SelectedLandmarkIndexArray.clear();
	pMesh0->m_ArraySharpVertexIndex.clear();
	pMesh0->SetModified(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark4()
{
	// TODO: Add your control notification handler code here
	//up
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	if (pMesh0->m_SelectedLandmarkIndexArray.size() != 1)
		return;
	if (pMesh0->m_ArraySharpVertexIndex.size() != 2)
		return;
	iternum++;
	//pMesh0->SetFlagOnVertices(0);
	//pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0])->m_Flag = 1;
	int landmarkindex = pMesh0->m_SelectedLandmarkIndexArray[0];
	GeoMapLandMark* pLandmark = pMesh0->m_LandMarkArray[landmarkindex];
	int startvindex = pLandmark->landmark_points[0];
	int endvindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
	CVertex3d* pStartVertex = pMesh0->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh0->GetVertex(endvindex);

	double offset = Distance(pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0]), pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[1]));

	for (int i = 0; i < pLandmark->landmark_points.size(); i++)
	{
		CVertex3d* pVertex = pMesh0->GetVertex(pLandmark->landmark_points[i]);
		pVertex->Set(pVertex->x(), pVertex->y() + offset, pVertex->z());
	}

	char message[80];
	sprintf(message, "D:\\%d.obj", iternum);
	pMesh0->SaveOBJFile(message);
	pMesh0->m_SelectedLandmarkIndexArray.clear();
	pMesh0->m_ArraySharpVertexIndex.clear();
	pMesh0->SetModified(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark5()
{
	// TODO: Add your control notification handler code here
	//down
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	if (pMesh0->m_SelectedLandmarkIndexArray.size() != 1)
		return;
	if (pMesh0->m_ArraySharpVertexIndex.size() != 2)
		return;
	iternum++;
	//pMesh0->SetFlagOnVertices(0);
	//pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0])->m_Flag = 1;
	int landmarkindex = pMesh0->m_SelectedLandmarkIndexArray[0];
	GeoMapLandMark* pLandmark = pMesh0->m_LandMarkArray[landmarkindex];
	int startvindex = pLandmark->landmark_points[0];
	int endvindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
	CVertex3d* pStartVertex = pMesh0->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh0->GetVertex(endvindex);

	double offset = Distance(pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0]), pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[1]));

	for (int i = 0; i < pLandmark->landmark_points.size(); i++)
	{
		CVertex3d* pVertex = pMesh0->GetVertex(pLandmark->landmark_points[i]);
		pVertex->Set(pVertex->x(), pVertex->y() - offset, pVertex->z());
	}

	char message[80];
	sprintf(message, "D:\\%d.obj", iternum);
	pMesh0->SaveOBJFile(message);
	pMesh0->m_SelectedLandmarkIndexArray.clear();
	pMesh0->m_ArraySharpVertexIndex.clear();
	pMesh0->SetModified(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark6()
{
	// TODO: Add your control notification handler code here
	//left
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	if (pMesh0->m_SelectedLandmarkIndexArray.size() != 1)
		return;
	if (pMesh0->m_ArraySharpVertexIndex.size() != 2)
		return;
	iternum++;
	//pMesh0->SetFlagOnVertices(0);
	//pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0])->m_Flag = 1;
	int landmarkindex = pMesh0->m_SelectedLandmarkIndexArray[0];
	GeoMapLandMark* pLandmark = pMesh0->m_LandMarkArray[landmarkindex];
	int startvindex = pLandmark->landmark_points[0];
	int endvindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
	CVertex3d* pStartVertex = pMesh0->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh0->GetVertex(endvindex);

	double offset = Distance(pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0]), pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[1]));

	for (int i = 0; i < pLandmark->landmark_points.size(); i++)
	{
		CVertex3d* pVertex = pMesh0->GetVertex(pLandmark->landmark_points[i]);
		pVertex->Set(pVertex->x() - offset, pVertex->y(), pVertex->z());
	}

	char message[80];
	sprintf(message, "D:\\%d.obj", iternum);
	pMesh0->SaveOBJFile(message);
	pMesh0->m_SelectedLandmarkIndexArray.clear();
	pMesh0->m_ArraySharpVertexIndex.clear();
	pMesh0->SetModified(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark7()
{
	//right
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	if (pMesh0->m_SelectedLandmarkIndexArray.size() != 1)
		return;
	if (pMesh0->m_ArraySharpVertexIndex.size() != 2)
		return;
	iternum++;
	//pMesh0->SetFlagOnVertices(0);
	//pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0])->m_Flag = 1;
	int landmarkindex = pMesh0->m_SelectedLandmarkIndexArray[0];
	GeoMapLandMark* pLandmark = pMesh0->m_LandMarkArray[landmarkindex];
	int startvindex = pLandmark->landmark_points[0];
	int endvindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
	CVertex3d* pStartVertex = pMesh0->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh0->GetVertex(endvindex);

	double offset = Distance(pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[0]), pMesh0->GetVertex(pMesh0->m_ArraySharpVertexIndex[1]));

	for (int i = 0; i < pLandmark->landmark_points.size(); i++)
	{
		CVertex3d* pVertex = pMesh0->GetVertex(pLandmark->landmark_points[i]);
		pVertex->Set(pVertex->x() + offset, pVertex->y(), pVertex->z());
	}

	char message[80];
	sprintf(message, "D:\\%d.obj", iternum);
	pMesh0->SaveOBJFile(message);
	pMesh0->m_SelectedLandmarkIndexArray.clear();
	pMesh0->m_ArraySharpVertexIndex.clear();
	pMesh0->SetModified(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark8()
{
	// TODO: Add your control notification handler code here
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	pMesh0->m_SelectedLandmarkIndexArray.clear();
	pMesh0->m_ArraySharpVertexIndex.clear();
	pMesh0->SetModified(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonStreightenlandmark9()
{
	// TODO: Add your control notification handler code here

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Georgia\\newboundary\\georgia2.obj");
	CMesh3d* pMesh0 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	pPolyOmino->pMesh = pMesh0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\landmark42.txt");
	sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\newboundary\\landmark69.txt");
	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);
	int n;

	//Load Landmarks
	/*sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\landmarindexonoriginalmesh.txt");
	std::ifstream boundaryfile(message);
	std::vector<int> m_ArrayBoundarylandmarks;
	int index1;
	while (!boundaryfile.eof())
	{
		boundaryfile >> index1;
		m_ArrayBoundarylandmarks.push_back(index1);
	}
	boundaryfile.close();
	for (int i = 0; i < m_ArrayBoundarylandmarks.size(); i++)
	{
		pMesh0->m_LandMarkArray[m_ArrayBoundarylandmarks[i]]->deleteflag = 1;
	}
	for (int i = 0; i < pMesh0->m_LandMarkArray.size(); i++)
	{
		if (pMesh0->m_LandMarkArray[i]->deleteflag)
		{
			pMesh0->m_LandMarkArray.erase(pMesh0->m_LandMarkArray.begin() + i);
			i--;
		}
	}*/
	pMesh0->SetModified(1);
	pDoc->UpdateAllViews(NULL);
}


void Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonic2()
{
	// TODO: Add your control notification handler code here
	//
	// TODO: Add your control notification handler code here
	//pDoc->OnOpenDocument("D:\\Razib\\lamcc_smooth_org_normalize.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = 0;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	sprintf(message, "D:\\landmark14.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);

	//
	OnBnClickedButtonDynamicHarmonic();
	p_FormView3->currentindex = 0;
	pPolyOmino->pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
}


void Actions_HarmonicMapping::OnBnClickedButtonDynamicharmonic3()
{
	pPolyOmino->pKdTree = new KDTree();

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\landmark14.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);

	//
	pDoc->OnOpenDocument("D:\\usamapdetailedlandmark_streightened.obj");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	//char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	sprintf(message, "D:\\landmark14.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//pPolyOmino->Initialization_KDTree();

	//Meare the largest deviations
	//pKdTree->maxDeviation = GetLargestPointDeviations();

	//return;
	//ConstructKdTreeFromPoints();
	//return;

	//pPolyOmino->GetPointandEdgeCorrespondence();

	//m_InitialIndex = 1;

	pPolyOmino->m_InitialIndex = 0;

	//pPolyOmino->pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);
	pPolyOmino->Initialization_KDTree1();

	/*pPolyOmino->InitializeHorizontalandVerticalSegmentArray();
	std::pair<int, int> pair1;
	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_LandMarkArray.size(); i++)
	{
		int startindex = pPolyOmino->pUniformGridMesh->m_LandMarkArray[i]->landmark_points[0];
		int endindex = pPolyOmino->pUniformGridMesh->m_LandMarkArray[i]->landmark_points[pPolyOmino->pUniformGridMesh->m_LandMarkArray[i]->landmark_points.size() - 1];

		pair1.first = startindex;
		pair1.second = endindex;
		pPolyOmino->pUniformGridMesh->m_ArraySharpEdges.push_back(pair1);
	}

	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	pPolyOmino->InitializeHorizontalandVerticalSegmentArray();

	pPolyOmino->GetPointandEdgeCorrespondence();*/

	pPolyOmino->ConstructKdTreeFromEdges();

	//PointSearchTest();

	//
	//AdaptKdTreeEdges();

	//
	//ComputeKdTreeNodeDenotedAreas();

	//
	//ComputeKdTreeLeafLocatedStates();
	//Search which state the point (0.5,0.5) lies in.
}


void Actions_HarmonicMapping::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//pPolyOmino->OnBnClickedPolyominoTuteembedding();
	std::vector<std::pair<int, int>> m_BoundaryVertices;
	std::vector<std::vector<int>> m_ArrayPointConnectedSelectEdgeIndex;
	BOOL isUniform = TRUE;
	std::vector<std::pair<int, std::pair<double, double>>> m_InteriorPointsArray;
	std::vector<double> boundaryedgelength;
	std::vector<int> m_boundaryRingVertices;
	//Fix the boundary 
	pPolyOmino->Compute_Boundary_Vertices_Parameter_Positions_TutteEmbedding_FixOuterBoundary(pPolyOmino->pMesh, m_boundaryRingVertices, m_BoundaryVertices, m_ArrayPointConnectedSelectEdgeIndex, boundaryedgelength, isUniform);

	//Compute the Linear System
	pPolyOmino->Compute_Linear_Equations_TutteEmbedding(pPolyOmino->pMesh, m_BoundaryVertices, m_ArrayPointConnectedSelectEdgeIndex, m_InteriorPointsArray, isUniform);

	isUniform = FALSE;

	pPolyOmino->Compute_Boundary_Vertices_Parameter_Positions_TutteEmbedding_FixOuterBoundary(pPolyOmino->pMesh, m_boundaryRingVertices, m_BoundaryVertices, m_ArrayPointConnectedSelectEdgeIndex, boundaryedgelength, isUniform);

	//Compute the Linear System
	pPolyOmino->Compute_Linear_Equations_TutteEmbedding(pPolyOmino->pMesh, m_BoundaryVertices, m_ArrayPointConnectedSelectEdgeIndex, m_InteriorPointsArray, isUniform);

	//
}


void Actions_HarmonicMapping::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
  /*pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\tuttleembedding\\test.obj");
  //pDoc->OnOpenDocument("D:\\41.obj");
  p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
  pPolyOmino->OnBnClickedOminoSetcurrentmesh();
  pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
  pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
  pPolyOmino->pMesh->m_showFace = FALSE;

  pPolyOmino->OnBnClickedGeneratelandmarks();

  //Load Landmarks
  char message[200];
  sprintf(message, "D:\\papers\\code\\data\\Geo\\tuttleembedding\\landmark.txt");

  CString landmarkname = message;
  pPolyOmino->Loadlandmark(landmarkname);*/
  // TODO: Add your control notification handler code here
  //pDoc->OnOpenDocument("D:\\Razib\\lamcc_smooth_org_normalize.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\tuttleembedding\\usalandmark3.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);
}


void Actions_HarmonicMapping::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	for (int k = 0; k < pPolyOmino->pMesh->m_LandMarkArray.size(); k++)
	{
		GeoMapLandMark* pLandmark = pPolyOmino->pMesh->m_LandMarkArray[k];
		pLandmark->style = 0;
	}
}


void Actions_HarmonicMapping::OnBnClickedButtonUshape()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonictest\\ushape.m");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	std::vector<int> m_ArrayOutputVerticesIndex;
	char resultmeshname[200];

	//first write the smooth results to A pslg file
	//sprintf(resultmeshname, "D:\\papers\\code\\Triangle\\CDT.poly");
	//GeneratePSLGFile(pMesh3D, m_ArrayOutputVerticesIndex, resultmeshname);

	char step1[200];
	SetCurrentDirectory("D:\\papers\\code\\Triangle");
	//sprintf(step1,"triangle.exe -pq33.8 ushape");
	sprintf(step1, "triangle.exe -pq38.1 ushape");
	//sprintf(step1, "triangle.exe -p CDT");
	system(step1);

	sprintf(resultmeshname, "D:\\papers\\code\\Triangle\\ushape.1.ele");
	//CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	//ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument(resultmeshname);
	//for (MyMesh::FaceIter f_it = pMesh3D->pmesh->faces_begin(); f_it != pMesh3D->pmesh->faces_end(); ++f_it)
	//{
	//	pMesh3D->pmesh->delete_face(f_it.handle(), FALSE);
	//}

	/*std::ifstream elefile(resultmeshname);

	int numtriangles, numofverticespertriangle, triangleattributes, index;
	elefile >> numtriangles >> numofverticespertriangle >> triangleattributes;
	if (numofverticespertriangle != 3)
		return;
	int nindex1, nindex2, nindex3;
	std::vector<MyMesh::VertexHandle>  face_vhandles;
	for (int i = 0; i < numtriangles; i++)
	{
		face_vhandles.clear();
		elefile >> index >> nindex1 >> nindex2 >> nindex3;
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(nindex1));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(nindex2));
		face_vhandles.push_back(pMesh3D->pmesh->vertex_handle(nindex3));
		pMesh3D->pmesh->add_face(face_vhandles);
	}
	elefile.close();
	pMesh3D->pmesh->garbage_collection();
	pMesh3D->m_Modified = TRUE;*/
}


void Actions_HarmonicMapping::OnBnClickedButtonUshapeTriagulated()
{
	// TODO: Add your control notification handler code here
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonictest\\ushapetriangle.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonictest\\ushapetriangle.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonictest\\ushapetrianglenormalized.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;
}

void Actions_HarmonicMapping::HarmonicMappingLaplace(CMesh3d_OpenMesh* pOriginalMesh, CMesh3d_OpenMesh* pTargetMesh)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	((CButton *)GetDlgItem(IDC_HARMONIC_WEIGHTS_LAPLACE))->SetCheck(TRUE);

	//Vertex selected flag;
	if (!pOriginalMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pOriginalMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pOriginalMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pOriginalMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//first fix the boundary
	pOriginalMesh->pBoundaryMeshReference = pTargetMesh;

	Compute_Boundary_Vertices_Parameter_Positions(pOriginalMesh);

	//Obtain the linear equations
	Compute_Linear_Equations(pOriginalMesh);

	//Generate a new parameter domain mesh
	//GenerateParameterDomain(pOriginalMesh);

	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	//currentindex = pDoc->m_SceneGraph.NbObject() - 1;
}
int Actions_HarmonicMapping::GetOneBoundaryVertex(CMesh3d_OpenMesh* pOpenMesh)
{
	for (MyMesh::VertexIter v_it = pOpenMesh->pmesh->vertices_begin(); v_it != pOpenMesh->pmesh->vertices_end(); ++v_it)
	{
		if (pOpenMesh->pmesh->is_boundary(v_it.handle()))
		{
			return v_it.handle().idx();
		}
	}
}
void Actions_HarmonicMapping::OnBnClickedButtonHarmonicOriginal()
{
	// TODO: Add your control notification handler code here
	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = num - 1;

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();
		/*pOpenMesh->m_FixVerticesArray.push_back(0);
		pOpenMesh->m_FixVerticesArray.push_back(3);
		pOpenMesh->m_FixVerticesArray.push_back(2);
		pOpenMesh->m_FixVerticesArray.push_back(1);*/
		//pOpenMesh->m_FixVerticesArray.push_back(2557);
		//pOpenMesh->m_FixVerticesArray.push_back(2);
		//pOpenMesh->m_FixVerticesArray.push_back(356);
		//pOpenMesh->m_FixVerticesArray.push_back(2597);
		//pOpenMesh->m_FixVerticesArray.push_back(2556);
		//pOpenMesh->m_FixVerticesArray.push_back(561);
		//pOpenMesh->m_FixVerticesArray.push_back(713);
		//pOpenMesh->m_FixVerticesArray.push_back(2039);
		/*pOpenMesh->m_FixVerticesArray.push_back(0);
		pOpenMesh->m_FixVerticesArray.push_back(14);
		pOpenMesh->m_FixVerticesArray.push_back(224);
		pOpenMesh->m_FixVerticesArray.push_back(210);*/

		/*pOpenMesh->m_FixVerticesArray_uv.push_back(0);
		pOpenMesh->m_FixVerticesArray_uv.push_back(0);
		pOpenMesh->m_FixVerticesArray_uv.push_back(1);
		pOpenMesh->m_FixVerticesArray_uv.push_back(0);
		pOpenMesh->m_FixVerticesArray_uv.push_back(1);
		pOpenMesh->m_FixVerticesArray_uv.push_back(1);
		pOpenMesh->m_FixVerticesArray_uv.push_back(0);
		pOpenMesh->m_FixVerticesArray_uv.push_back(1);*/
		pOpenMesh->m_FixVerticesArray.push_back(0);
		pOpenMesh->m_FixVerticesArray.push_back(100);
		pOpenMesh->m_FixVerticesArray.push_back(10200);
		pOpenMesh->m_FixVerticesArray.push_back(10100);
		pOpenMesh->m_FixVerticesArray_uv.push_back(0);
		pOpenMesh->m_FixVerticesArray_uv.push_back(0);
		pOpenMesh->m_FixVerticesArray_uv.push_back(1);
		pOpenMesh->m_FixVerticesArray_uv.push_back(0);
		pOpenMesh->m_FixVerticesArray_uv.push_back(1);
		pOpenMesh->m_FixVerticesArray_uv.push_back(1);
		pOpenMesh->m_FixVerticesArray_uv.push_back(0);
		pOpenMesh->m_FixVerticesArray_uv.push_back(1);
//		int vindex = GetOneBoundaryVertex(pOpenMesh);
//		pOpenMesh->m_FixVerticesArray.push_back(vindex);

		pOpenMesh->m_Transform.Copy(*pMesh3D->GetTransform());
		pDoc->m_SceneGraph.Add(pOpenMesh);
		
		VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
		VPROP_VERTEX_UV vprop_vertex_uv;

		//Vertex selected flag;
		if (!pOpenMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
			pOpenMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

		//Vertex UV
		if (!pOpenMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
			pOpenMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

		//first fix the boundary
		Compute_Boundary_Vertices_Parameter_Positions(pOpenMesh, 0);

		//Obtain the linear equations
		Compute_Linear_Equations(pOpenMesh);

		//Generate a new parameter domain mesh
		GenerateParameterDomain(pOpenMesh);

		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
		{
			CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
			//pMesh3D->m_FixVerticesArray.push_back(0);
			//pMesh3D->m_FixVerticesArray.push_back(1);
			//pMesh3D->m_FixVerticesArray.push_back(2);
			//pMesh3D->m_FixVerticesArray.push_back(3);
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

			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);
			currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		}
}


void Actions_HarmonicMapping::OnBnClickedButtonFacemodel()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\facestraightening\\face50_50.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;
	pPolyOmino->Loadlandmark("D:\\papers\\code\\data\\facestraightening\\mouth1.txt");
	CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();
	pOpenMesh->m_LandMarkArray = pMesh3D->m_LandMarkArray;
	//		pOpenMesh->m_FixVerticesArray.push_back(0);
	//		pOpenMesh->m_FixVerticesArray.push_back(3);
	//		pOpenMesh->m_FixVerticesArray.push_back(2);
	//		pOpenMesh->m_FixVerticesArray.push_back(1);
	pOpenMesh->m_FixVerticesArray.push_back(2557);
	pOpenMesh->m_FixVerticesArray.push_back(2);
	pOpenMesh->m_FixVerticesArray.push_back(356);
	pOpenMesh->m_FixVerticesArray.push_back(2597);
	pDoc->m_SceneGraph.Add(pOpenMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_HarmonicMapping::TwoDHarmonicStraightening_FixLandmarkEndPoints_Sophie(CMesh3d_OpenMesh* pOpenMesh)
{
	//first fix the boundary
	//Compute_Boundary_Vertices_Parameter_Positions(pOpenMesh);
	//Compute_Boundary_Vertices_Parameter_Positions_AdditionalFixPoints(pOpenMesh);

	//Obtain the linear equations
	Compute_Linear_Equations_WithStraighteningConstraints_FixEndPoints(pOpenMesh);

	//Generate a new parameter domain mesh
	//GenerateParameterDomain(pOpenMesh);

	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	//currentindex = pDoc->m_SceneGraph.NbObject() - 1;
}
void Actions_HarmonicMapping::TwoDHarmonicStraightening_FixLandmarkEndPoints(CMesh3d_OpenMesh* pOpenMesh)
{
	//first fix the boundary
	//Compute_Boundary_Vertices_Parameter_Positions(pOpenMesh);
	Compute_Boundary_Vertices_Parameter_Positions_AdditionalFixPoints(pOpenMesh);

	//Obtain the linear equations
	Compute_Linear_Equations_WithStraighteningConstraints_FixEndPoints(pOpenMesh);

	//Generate a new parameter domain mesh
	//GenerateParameterDomain(pOpenMesh);

	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	//currentindex = pDoc->m_SceneGraph.NbObject() - 1;
}
void Actions_HarmonicMapping::HarmonicStraightening_VerticalPoint(CMesh3d_OpenMesh* pOpenMesh, int boundarytype = 0)
{
	//first fix the boundary
	Compute_Boundary_Vertices_Parameter_Positions_VerticalPoint(pOpenMesh, boundarytype);

	//Obtain the linear equations
	Compute_Linear_Equations_WithStraighteningConstraints(pOpenMesh);

	//Generate a new parameter domain mesh
	GenerateParameterDomain(pOpenMesh);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	//currentindex = pDoc->m_SceneGraph.NbObject() - 1;
}

void Actions_HarmonicMapping::HarmonicStraightening(CMesh3d_OpenMesh* pOpenMesh,int boundarytype = 0)
{
	//first fix the boundary
	Compute_Boundary_Vertices_Parameter_Positions(pOpenMesh,boundarytype);

	//Obtain the linear equations
	Compute_Linear_Equations_WithStraighteningConstraints(pOpenMesh);

	//Generate a new parameter domain mesh
	GenerateParameterDomain(pOpenMesh);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	//currentindex = pDoc->m_SceneGraph.NbObject() - 1;
}

void Actions_HarmonicMapping::OnBnClickedButtonHarmonicStraightening()
{
	// TODO: Add your control notification handler code here
	//IDC_BUTTON_HARMONIC_STRAIGHTENING
	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = num - 1;

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();
		pOpenMesh->m_FixVerticesArray = pMesh3D->m_FixVerticesArray;
		//pOpenMesh->m_FixVerticesArray.push_back(2557);
		//pOpenMesh->m_FixVerticesArray.push_back(2);
		//pOpenMesh->m_FixVerticesArray.push_back(356);
		//pOpenMesh->m_FixVerticesArray.push_back(2597);
		pOpenMesh->m_Transform.Copy(*pMesh3D->GetTransform());
		//pDoc->m_SceneGraph.Add(pOpenMesh);

		VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
		VPROP_VERTEX_UV vprop_vertex_uv;

		//Vertex selected flag;
		if (!pOpenMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
			pOpenMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

		//Vertex UV
		if (!pOpenMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
			pOpenMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

		//first fix the boundary
		Compute_Boundary_Vertices_Parameter_Positions(pOpenMesh);

		//Obtain the linear equations
		Compute_Linear_Equations_WithStraighteningConstraints(pOpenMesh);

		//Generate a new parameter domain mesh
		GenerateParameterDomain(pOpenMesh);

		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
		{
			CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);
			//pMesh3D->m_FixVerticesArray.push_back(0);
			//pMesh3D->m_FixVerticesArray.push_back(1);
			//pMesh3D->m_FixVerticesArray.push_back(2);
			//pMesh3D->m_FixVerticesArray.push_back(3);
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
			Compute_Linear_Equations_WithStraighteningConstraints(pMesh3D);

			//Generate a new parameter domain mesh
			GenerateParameterDomain(pMesh3D);

			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);
			currentindex = pDoc->m_SceneGraph.NbObject() - 1;
		}
}


void Actions_HarmonicMapping::OnBnClickedButtonExample1()
{
// TODO: Add your control notification handler code here
  pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonictest\\horizontalwithsingulairty\\HorizontalConstraints_Withsingularities.m");
  //pDoc->OnOpenDocument("D:\\41.obj");
  p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
  pPolyOmino->OnBnClickedOminoSetcurrentmesh();
  pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
  pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
  pPolyOmino->pMesh->m_showFace = FALSE;
  pPolyOmino->OnBnClickedGeneratelandmarks();

  //Load Landmarks
  char message[200];
  sprintf(message, "D:\\papers\\code\\data\\Geo\\harmonictest\\horizontalwithsingulairty\\landmark1.txt");
  //sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
  //sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

  CString landmarkname = message;
  pPolyOmino->Loadlandmark(landmarkname);
}


void Actions_HarmonicMapping::OnBnClickedButtonHarmonicHvStraightening()
{
	// TODO: Add your control notification handler code here
	int num = pDoc->m_SceneGraph.NbObject();
	int currentindex = 0;

	if (currentindex < 0 || currentindex >= num)
		return;

	if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3D = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
		CMesh3d_OpenMesh* pOpenMesh = pMesh3D->Convert_OpenMesh();

		pOpenMesh->m_Transform.Copy(*pMesh3D->GetTransform());
		//pDoc->m_SceneGraph.Add(pOpenMesh);
		//Actions_HarmonicMapping::ComputeHarmonic_LinearConstraints(pOpenMesh);
		//first fix the boundary
		Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pOpenMesh);

		//Obtain the linear equations
		Compute_Linear_Equations_LinearConstraints(pOpenMesh);

		//Generate a new parameter domain mesh
		GenerateParameterDomain(pOpenMesh);

		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	}
	else
		if (pDoc->m_SceneGraph.GetAt(currentindex)->GetType() == TYPE_MESH3D_OPENMESH)
		{
			CMesh3d_OpenMesh* pMesh3D = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(currentindex);

			//first fix the boundary
			Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pMesh3D);

			//Obtain the linear equations
			Compute_Linear_Equations_LinearConstraints(pMesh3D);

			//Generate a new parameter domain mesh
			GenerateParameterDomain(pMesh3D);

			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);
		}

}


void Actions_HarmonicMapping::OnBnClickedButtonExample2()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\harmonictest\\horizontalwithsingulairty\\HorizontalConstraints_Withsingularities_1.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;
	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\harmonictest\\horizontalwithsingulairty\\landmark1_1.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);
}
