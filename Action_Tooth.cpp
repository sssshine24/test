#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Actions_DataAnalysis.h"
#include "afxdialogex.h"
#include "Lib3D/Mesh3d.h"
#include "Action_PolyOmino.h"
#include "svm/Predictor.h"
#include "FormView3.h"
#include "lib3d/Base3d.h"
#include "Lib3D/Mesh3d_OpenMesh.h"
#include <GL\glut.h>
#include "CFramebuffer.h"
#include "MyTreeView.h"
#include <shlwapi.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include "Action_Tooth.h"

Action_Tooth::Action_Tooth()
{
}


Action_Tooth::~Action_Tooth()
{
}
BEGIN_MESSAGE_MAP(Action_Tooth, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_BOUNDARYDETECTION, &Action_Tooth::OnBnClickedButtonToothBoundarydetection)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_MESHSMOOTH, &Action_Tooth::OnBnClickedButtonToothMeshsmooth)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_LOADPATH, &Action_Tooth::OnBnClickedButtonToothLoadpath)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_DeleteSharpEdge, &Action_Tooth::OnBnClickedButtonToothDeletesharpedge)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_AddSharpEdge2, &Action_Tooth::OnBnClickedButtonToothAddsharpedge2)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_ClearSharpEdgeSelection, &Action_Tooth::OnBnClickedButtonToothClearsharpedgeselection)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_SAVEPATH, &Action_Tooth::OnBnClickedButtonToothSavepath)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_ClearSharpVertexSelectio, &Action_Tooth::OnBnClickedButtonToothClearsharpvertexselectio)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_DeleteSharpEdge_TwoPoints, &Action_Tooth::OnBnClickedButtonToothDeletesharpedgeTwopoints)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_SwapEdge, &Action_Tooth::OnBnClickedButtonToothSwapedge)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_SwapEdge_SharpEdge, &Action_Tooth::OnBnClickedButtonToothSwapedgeSharpedge)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_PathSmooth, &Action_Tooth::OnBnClickedButtonToothPathsmooth)
	ON_BN_CLICKED(IDC_BUTTON_TOOTH_PathProjection, &Action_Tooth::OnBnClickedButtonToothPathprojection)
	ON_BN_CLICKED(IDC_BUTTON_CHECKTEETH, &Action_Tooth::OnBnClickedButtonCheckteeth)
END_MESSAGE_MAP()

extern CFormView3 *p_FormView3;
void Action_Tooth::OnBnClickedButtonToothBoundarydetection()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		pMesh->GetVertex(i)->SetFlag(0);
		pMesh->GetVertex(i)->m_num = i;
	}
	CVertex3d* pVertex1 = pMesh->GetVertex(47349);
	CVertex3d* pVertex2 = pMesh->GetVertex(162273);
	CVertex3d* pVertex3 = pMesh->GetVertex(162274);
	Vector3D v_1(pVertex1->x(), pVertex1->y(), pVertex1->z());
	Vector3D v_2(pVertex2->x(), pVertex2->y(), pVertex2->z());
	Vector3D v_3(pVertex3->x(), pVertex3->y(), pVertex3->z());
	Vector3D Yaxis = (v_2 - v_1) ^ (v_3 - v_1);
	Yaxis.SetUnit();

	//pVertex->SetFlag(1);
	//double Yval = -17;
	double dis1,dis2,dis3;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		Vector3D v1(pFace->v1()->x(), pFace->v1()->y(), pFace->v1()->z());
		Vector3D v2(pFace->v2()->x(), pFace->v2()->y(), pFace->v2()->z());
		Vector3D v3(pFace->v3()->x(), pFace->v3()->y(), pFace->v3()->z());

		int num1 = pFace->v1()->m_num;
		int num2 = pFace->v2()->m_num;
		int num3 = pFace->v3()->m_num;

		Vector3D v1v2 = v2 - v1;
		Vector3D v1v3 = v3 - v1;

		Vector3D fn = v1v2 ^ v1v3;
		fn.SetUnit();

		double fdeviation = fn * Yaxis;
		if (fdeviation > -0.8)
			continue;
		//CVector3d nvec = pFace->GetNormal();
		//if (pFace->v1()->y() > Yval || pFace->v2()->y() > Yval || pFace->v3()->y() > Yval)
		//	continue;
		dis1 = (v1 - v_1) * Yaxis;
		dis2 = (v2 - v_1) * Yaxis;
		dis3 = (v3 - v_1) * Yaxis;
		if (fabs(dis1) < 0.1 && fabs(dis2) < 0.1 && fabs(dis3) < 0.1)
		{
			pFace->v1()->SetFlag(1);
			pFace->v2()->SetFlag(1);
			pFace->v3()->SetFlag(1);
		}
	}
}
void Action_Tooth::Meshsmooth(CMesh3d* pMesh)
{
	//CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	CMesh3d* pNewMesh = new CMesh3d();
	pNewMesh->Copy(pMesh);
	int flag;
	pMesh->CalculateLaplaceBeltramiOperator_NoArea();
	double lamda = 0.01;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		flag = pMesh->GetVertex(i)->GetFlag();
		if (flag)
			continue;

		CVertex3d* pVertex = pMesh->GetVertex(i);

		if (Length(pVertex->m_LaplaceOperatorVec) > 1)
			continue;

		double x = pVertex->x() + pVertex->m_LaplaceOperatorVec[0] * lamda;
		double y = pVertex->y() + pVertex->m_LaplaceOperatorVec[1] * lamda;
		double z = pVertex->z() + pVertex->m_LaplaceOperatorVec[2] * lamda;

		pNewMesh->GetVertex(i)->Set(x, y, z);
		//pVertex->Set(x, y, z);
	}
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		pMesh->GetVertex(i)->Set(pNewMesh->GetVertex(i)->x(), pNewMesh->GetVertex(i)->y(), pNewMesh->GetVertex(i)->z());
	}
	delete pNewMesh;
}

void Action_Tooth::OnBnClickedButtonToothMeshsmooth()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	for(int i =0; i < 100; i++)
		Meshsmooth(pMesh);

	pMesh->m_Modified = 1;

	pDoc->UpdateAllViews(NULL);
}


void Action_Tooth::OnBnClickedButtonToothLoadpath()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	pMesh->m_ArraySharpEdges.clear();

	CString FileName;
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;

	//std::string filename = "E:\\workinsdu\\牙齿项目\\data\\Gumline_M_Pts\\VS_a_redline.txt";
	//filename.replace(filename.find("."), 2, "_redline2.txt");
	std::ifstream file(FileName);
	std::pair<int, int> add;
	int index,index1;
	int i = 0;
	while (!file.eof())
	{
		file >> index;
		file >> index1;
		add.first = index;
		add.second = index1;
		pMesh->m_ArraySharpEdges.push_back(add);
		//CVertex3d* pVertex = pMesh->GetVertex(index);
		//pVertex->m_Flag = 1;
	}

	file.close();
	//pMesh->m_ArraySharpVertexIndex.clear();
	//pMesh->m_ArraySharpVertexIndex.push_back(128737);
	//pMesh->m_ArraySharpVertexIndex.push_back(51528);
}


void Action_Tooth::OnBnClickedButtonToothDeletesharpedge()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);

	/*if (pMesh->m_ArraySharpVertexIndex.size() != 1)
		return;

	
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		if (pMesh->m_ArraySharpEdges[i].first == pMesh->m_ArraySharpVertexIndex[0] ||
			pMesh->m_ArraySharpEdges[i].second == pMesh->m_ArraySharpVertexIndex[0])
		{
			pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
			i--;
		}
	}*/
	for (int i = 0; i < pMesh->m_ArraySelectedSharpEdgeIndex.size(); i++)
	{
		pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + pMesh->m_ArraySelectedSharpEdgeIndex[i]);
		break;
	}
	pMesh->m_ArraySelectedSharpEdgeIndex.clear();
	pDoc->UpdateAllViews(NULL);
}


void Action_Tooth::OnBnClickedButtonToothAddsharpedge2()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);

	//if (pMesh->m_ArraySharpVertexIndex.size() != 2)
	//	return;
	for(int i = 0; i < pMesh->m_ArraySharpVertexIndex.size() - 1; i++)
		pMesh->m_ArraySharpEdges.push_back(std::make_pair(pMesh->m_ArraySharpVertexIndex[i], pMesh->m_ArraySharpVertexIndex[i + 1]));

	pMesh->m_ArraySharpVertexIndex.clear();
	pDoc->UpdateAllViews(NULL);
}


void Action_Tooth::OnBnClickedButtonToothClearsharpedgeselection()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);

	pMesh->m_ArraySelectedSharpEdgeIndex.clear();
	pDoc->UpdateAllViews(NULL);
}


void Action_Tooth::OnBnClickedButtonToothSavepath()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);

	CString FileName;
	CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;

	//std::string filename = "E:\\workinsdu\\牙齿项目\\data\\Gumline_M_Pts\\VS_a_redline.txt";;
	//filename.replace(filename.find("."), 2, "_redline.txt");
	std::ofstream file(FileName);

	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int, int> temp;
		temp = pMesh->m_ArraySharpEdges[i];
		file << temp.first << " " << temp.second << " " << "\n";
	}
	file.close();

}


void Action_Tooth::OnBnClickedButtonToothClearsharpvertexselectio()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	pMesh->m_ArraySharpVertexIndex.clear();
	pDoc->UpdateAllViews(NULL);
}


void Action_Tooth::OnBnClickedButtonToothDeletesharpedgeTwopoints()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	if (pMesh->m_ArraySharpVertexIndex.size() != 2)
		return;
	int vindex1 = pMesh->m_ArraySharpVertexIndex[0];
	int vindex2 = pMesh->m_ArraySharpVertexIndex[1];
	std::vector<std::pair<int, int>> m_ArraySharpEdges_Sub1, m_ArraySharpEdges_Sub2;

	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		/*if (pMesh->m_ArraySharpEdges[i].first == vindex1)
		{
			m_ArraySharpEdges_Sub1.push_back(pMesh->m_ArraySharpEdges[i]);
			vindex1 = pMesh->m_ArraySharpEdges[i].second;
			pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
			i = -1;
		}else
			if (pMesh->m_ArraySharpEdges[i].second == vindex1)
			{
				m_ArraySharpEdges_Sub1.push_back(pMesh->m_ArraySharpEdges[i]);
				vindex1 = pMesh->m_ArraySharpEdges[i].first;
				pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
				i = -1;
			}
		if (vindex1 == vindex2)
			break;*/
		if (pMesh->m_ArraySharpEdges[i].first == vindex1 &&
			pMesh->m_ArraySharpEdges[i].second == vindex2 ||
			pMesh->m_ArraySharpEdges[i].second == vindex1 &&
			pMesh->m_ArraySharpEdges[i].first == vindex2)
		{
			pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
			pMesh->m_ArraySharpVertexIndex.clear();
			break;
		}

	}

	//if (pMesh->m_ArraySharpEdges.size() < m_ArraySharpEdges_Sub1.size())
	//{
	//	pMesh->m_ArraySharpEdges = m_ArraySharpEdges_Sub1;
	//}
	pDoc->UpdateAllViews(NULL);
}

extern CMyTreeView* pTreeView;
extern char Post_Message[1000];
void Action_Tooth::OnBnClickedButtonToothSwapedge()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	if (pMesh->m_ArraySharpVertexIndex.size() != 2)
		return;
	int vindex1 = pMesh->m_ArraySharpVertexIndex[0];
	int vindex2 = pMesh->m_ArraySharpVertexIndex[1];
	pMesh->SetFlagOnVertices_ArrayIndex();

	for (int i = 0; i < pMesh->GetVertex(vindex1)->NbFaceNeighbor(); i++)
	{
		CFace3d* pFace3d = pMesh->GetVertex(vindex1)->GetFaceNeighbor(i);
		int index = 0;
		for (int index = 0; index < 3; index++)
		{
			if (pFace3d->v(index)->m_Flag == vindex1 &&
				pFace3d->v((index + 1) % 3)->m_Flag == vindex2 || 
				pFace3d->v(index)->m_Flag == vindex2 &&
				pFace3d->v((index + 1) % 3)->m_Flag == vindex1)
			{
				if (pFace3d->f(index))
				{
					CFace3d* pFace3d1 = pFace3d->f(index);
					//switch
					CVertex3d* pVertex4 = pFace3d1->v1();
					int ii = 1;
					while (pVertex4 == pFace3d->v(index) || pVertex4 == pFace3d->v((index + 1) % 3))
					{
						pVertex4 = pFace3d1->v(ii);
						ii++;
					}

					//Insert Two Faces
					CFace3d* pNewFace1 = new CFace3d(pFace3d->v((index + 2) % 3), pFace3d->v(index), pVertex4);
					CFace3d* pNewFace2 = new CFace3d(pVertex4, pFace3d->v((index + 1) % 3), pFace3d->v((index + 2) % 3));
					pMesh->AddFace(pNewFace1);
					pMesh->AddFace(pNewFace2);
					pMesh->DeleteFace(pFace3d);
					pMesh->DeleteFace(pFace3d1);
					pMesh->BuildAdjacency();
					pMesh->SetModified(TRUE);
					//sprintf(Post_Message, "Flip Faces Successfully!");
					//::PostMessage(g_hwndMain,WM_MESSAGE, 0, (LPARAM)Post_Message);
					pMesh->m_ArraySharpVertexIndex.clear();
					pDoc->UpdateAllViews(NULL);
					return;
				}
			}
		}

	}
}


void Action_Tooth::OnBnClickedButtonToothSwapedgeSharpedge()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	if (pMesh->m_ArraySharpVertexIndex.size() != 2)
		return;
	int vindex1 = pMesh->m_ArraySharpVertexIndex[0];
	int vindex2 = pMesh->m_ArraySharpVertexIndex[1];
	pMesh->SetFlagOnVertices_ArrayIndex();

	for (int i = 0; i < pMesh->GetVertex(vindex1)->NbFaceNeighbor(); i++)
	{
		CFace3d* pFace3d = pMesh->GetVertex(vindex1)->GetFaceNeighbor(i);
		int index = 0;
		for (int index = 0; index < 3; index++)
		{
			if (pFace3d->v(index)->m_Flag == vindex1 &&
				pFace3d->v((index + 1) % 3)->m_Flag == vindex2 ||
				pFace3d->v(index)->m_Flag == vindex2 &&
				pFace3d->v((index + 1) % 3)->m_Flag == vindex1)
			{
				if (pFace3d->f(index))
				{
					CFace3d* pFace3d1 = pFace3d->f(index);
					//switch
					CVertex3d* pVertex4 = pFace3d1->v1();
					int ii = 1;
					while (pVertex4 == pFace3d->v(index) || pVertex4 == pFace3d->v((index + 1) % 3))
					{
						pVertex4 = pFace3d1->v(ii);
						ii++;
					}

					//Insert Two Faces
					CFace3d* pNewFace1 = new CFace3d(pFace3d->v((index + 2) % 3), pFace3d->v(index), pVertex4);
					CFace3d* pNewFace2 = new CFace3d(pVertex4, pFace3d->v((index + 1) % 3), pFace3d->v((index + 2) % 3));
					pMesh->AddFace(pNewFace1);
					pMesh->AddFace(pNewFace2);
					pMesh->DeleteFace(pFace3d);
					pMesh->DeleteFace(pFace3d1);
					pMesh->BuildAdjacency();
					pMesh->SetModified(TRUE);
					pMesh->m_ArraySharpEdges.push_back(std::make_pair(vindex1, vindex2));
					//sprintf(Post_Message, "Flip Faces Successfully!");
					//::PostMessage(g_hwndMain,WM_MESSAGE, 0, (LPARAM)Post_Message);
					pMesh->m_ArraySharpVertexIndex.clear();
					pDoc->UpdateAllViews(NULL);
					return;
				}
			}
		}

	}
}


void Action_Tooth::OnBnClickedButtonToothPathsmooth()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);

	if (pMesh->m_ArraySharpEdges.size() == 0)
		return;

	std::vector<std::pair<int, int>> m_ArraySharpEdges1, m_ArraySharpEdgesbak;
	m_ArraySharpEdgesbak = pMesh->m_ArraySharpEdges;
	m_ArraySharpEdges1.push_back(pMesh->m_ArraySharpEdges[0]);
	pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin());
	while (1)
	{
		int vindex = m_ArraySharpEdges1[m_ArraySharpEdges1.size() - 1].second;
		for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
		{
			if (pMesh->m_ArraySharpEdges[i].first == vindex)
			{
				m_ArraySharpEdges1.push_back(pMesh->m_ArraySharpEdges[i]);
				pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
				break;
			}
			else if (pMesh->m_ArraySharpEdges[i].second == vindex)
			{
				m_ArraySharpEdges1.push_back(std::make_pair(pMesh->m_ArraySharpEdges[i].second, pMesh->m_ArraySharpEdges[i].first));
				pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
				break;
			}
		}
		if (vindex == m_ArraySharpEdges1[0].first)
			break;
	}

	pMesh->m_ArraySharpEdges = m_ArraySharpEdgesbak;
	//
	std::vector<std::vector<double>> m_ArraySharpEdges_Seperate, m_ArraySharpEdges_Seperate1;
	std::vector<double> coordinates, coordinates1, coordinates2;
	for (int i = 0; i < m_ArraySharpEdges1.size(); i++)
	{
		coordinates.clear();
		
		coordinates.push_back(pMesh->GetVertex(m_ArraySharpEdges1[i].first)->x());
		coordinates.push_back(pMesh->GetVertex(m_ArraySharpEdges1[i].first)->y());
		coordinates.push_back(pMesh->GetVertex(m_ArraySharpEdges1[i].first)->z());

		m_ArraySharpEdges_Seperate.push_back(coordinates);
	}
	for (int ii = 0; ii < 10; ii++)
	{
		m_ArraySharpEdges_Seperate1.clear();
		for (int i = 0; i < m_ArraySharpEdges_Seperate.size(); i++)
		{
			coordinates1 = m_ArraySharpEdges_Seperate[(i - 1 + m_ArraySharpEdges_Seperate.size()) % m_ArraySharpEdges_Seperate.size()];
			coordinates2 = m_ArraySharpEdges_Seperate[(i + 1 + m_ArraySharpEdges_Seperate.size()) % m_ArraySharpEdges_Seperate.size()];

			coordinates.clear();
			coordinates.push_back((coordinates1[0] + coordinates2[0]) / 2);
			coordinates.push_back((coordinates1[1] + coordinates2[1]) / 2);
			coordinates.push_back((coordinates1[2] + coordinates2[2]) / 2);

			m_ArraySharpEdges_Seperate1.push_back(coordinates);
		}
		m_ArraySharpEdges_Seperate = m_ArraySharpEdges_Seperate1;
	}

	pMesh->m_ArraySharpEdges_Seperate = m_ArraySharpEdges_Seperate;
	pDoc->UpdateAllViews(NULL);
}


void Action_Tooth::OnBnClickedButtonToothPathprojection()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	if (pMesh->m_ArraySharpEdges_Seperate.size() == 0)
		return;

	//dense sampling
	std::vector<double> sampled_coordinate;
	std::vector<std::vector<double>> m_sampled_coord;
	int sepsize = pMesh->m_ArraySharpEdges_Seperate.size();
	for (int i = 0; i < sepsize; i++)
	{
		m_sampled_coord.push_back(pMesh->m_ArraySharpEdges_Seperate[i]);
		sampled_coordinate[0] = (pMesh->m_ArraySharpEdges_Seperate[i][0] * 3 + pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize][0]) / 4;
		sampled_coordinate[1] = (pMesh->m_ArraySharpEdges_Seperate[i][1] * 3 + pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize][1]) / 4;
		sampled_coordinate[2] = (pMesh->m_ArraySharpEdges_Seperate[i][2] * 3 + pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize][2]) / 4;
		m_sampled_coord.push_back(sampled_coordinate);

		sampled_coordinate.clear();
		sampled_coordinate[0] = (pMesh->m_ArraySharpEdges_Seperate[i][0] + pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize][0]) / 2;
		sampled_coordinate[1] = (pMesh->m_ArraySharpEdges_Seperate[i][1] + pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize][1]) / 2;
		sampled_coordinate[2] = (pMesh->m_ArraySharpEdges_Seperate[i][2] + pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize][2]) / 2;
		m_sampled_coord.push_back(sampled_coordinate);
		
		sampled_coordinate.clear();
		sampled_coordinate[0] = (pMesh->m_ArraySharpEdges_Seperate[i][0] + 3 * pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize][0]) / 4;
		sampled_coordinate[1] = (pMesh->m_ArraySharpEdges_Seperate[i][1] + 3 * pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize][1]) / 4;
		sampled_coordinate[2] = (pMesh->m_ArraySharpEdges_Seperate[i][2] + 3 * pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize][2]) / 4;
		m_sampled_coord.push_back(sampled_coordinate);

		//m_sampled_coord.push_back(pMesh->m_ArraySharpEdges_Seperate[(i + 1) % sepsize]);
	}

	std::vector<double> coordinate = pMesh->m_ArraySharpEdges_Seperate[0];
	CVertex3d* pStartVertex = new CVertex3d(coordinate[0], coordinate[1], coordinate[2]);
	CMesh3d_OpenMesh* pOpenMesh = pMesh->Convert_OpenMesh();

	//GetStart Point Projection
	GetPointProjectionOnMesh(coordinate[0], coordinate[1], coordinate[2], pMesh, pOpenMesh);

	double mindis = 1e10;
	int minvindex;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d * pVertex = pMesh->GetVertex(i);
		double dis = Distance(pStartVertex, pVertex);
		if (dis < mindis)
		{
			mindis = dis;
			minvindex = i;
		}
	}

	//
	std::vector<MyMesh::VertexHandle> m_ArrayOneRingVertices;
	m_ArrayOneRingVertices.clear();

	//search the 1-ring vertices
	int sharpedgeindex;
	double t1, t2;
	for (MyMesh::VertexVertexIter vv_it = pOpenMesh->pmesh->vv_iter(pOpenMesh->pmesh->vertex_handle(minvindex)); vv_it; ++vv_it)
	{
		m_ArrayOneRingVertices.push_back(vv_it.handle());
		if (GetProjectionOnEdge(pMesh, minvindex, vv_it.handle().idx(), sharpedgeindex, t1, t2))
		{

		}
	}


}
int Action_Tooth::GetPointProjectionOnMesh(double x, double y, double z, CMesh3d* pMesh, CMesh3d_OpenMesh* pOpenMesh)
{
	CVertex3d* pStartVertex = new CVertex3d(x, y, z);
	double mindis = 1e10;
	int minvindex;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d * pVertex = pMesh->GetVertex(i);
		double dis = Distance(pStartVertex, pVertex);
		if (dis < mindis)
		{
			mindis = dis;
			minvindex = i;
		}
	}

	//

	return 1;
}
int Action_Tooth::GetProjectionOnEdge(CMesh3d* pMesh, int vindex1, int vindex2, int &sharpedgeindex,double &t1,double &t2)
{
	Vector3D v1, v2, v3, v4;

	CVertex3d* pVertex1 = pMesh->GetVertex(vindex1);
	CVertex3d* pVertex2 = pMesh->GetVertex(vindex2);

	v1.SetValue(pVertex1->x(), pVertex1->y(), pVertex1->z());
	v2.SetValue(pVertex2->x(), pVertex2->y(), pVertex2->z());

	sharpedgeindex = -1;

	std::vector<double> v3coordinates, v4coordinates;
	double a, b, c, d, e, f;
	//double t1, t2;
	for (int i = 0; i < pMesh->m_ArraySharpEdges_Seperate.size(); i++)
	{
		v3coordinates = pMesh->m_ArraySharpEdges_Seperate[i];
		v4coordinates = pMesh->m_ArraySharpEdges_Seperate[(i + 1) % pMesh->m_ArraySharpEdges_Seperate.size()];

		v3.SetValue(v3coordinates[0], v3coordinates[1], v3coordinates[2]);
		v4.SetValue(v4coordinates[0], v4coordinates[1], v4coordinates[2]);

		a = (v2 - v1) * (v2 - v1);
		b = (v4 - v3) * (v1 - v2);
		c = (v1 - v3) * (v2 - v1);
		d = (v2 - v1) * (v4 - v3);
		e = (v4 - v3) * (v3 - v4);
		f = (v1 - v3) * (v4 - v3);

		t1 = (b * f - c * e) / (a * e - b * d);
		t2 = (a * f - c * d) / (b * d - a * e);

		if (t1 > -1e-8 && t1 < 1 + 1e-8 && t2 > -1e-8 && t2 < 1 + 1e-8)
		{
			sharpedgeindex = i;
			return 1;
		}
	}
	return 0;
}


void Action_Tooth::OnBnClickedButtonCheckteeth()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	int currentindex = 0;
	if (currentindex < 0 || currentindex >= num)
		return;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(currentindex);
	CMesh3d_OpenMesh* pOpenmesh = pMesh->Convert_OpenMesh();

	std::vector<int> deletesharpedge = pOpenmesh->check_cutpath_valid(pMesh->m_ArraySharpEdges);
	for (int i = deletesharpedge.size() - 1; i >= 0; i--)
	{
		int temp = deletesharpedge[i];
		pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + temp);
	}

	delete(pOpenmesh);

	pMesh->m_Modified = 1;

	pDoc->UpdateAllViews(NULL);
}
