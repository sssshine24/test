// Action_PolyOmino2.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Action_PolyOmino2.h"
#include "afxdialogex.h"
#include "FormView3.h"
#include "lib3d/Base3d.h"
#include "Lib3D/Mesh3d_OpenMesh.h"
#include "Action_PolyOmino.h"
#include "MyTreeView.h"
#include "Actions_HarmonicMapping.h"
#include "Action_PolyOmino.h"
#include <stack> 
// Action_PolyOmino2 dialog

IMPLEMENT_DYNAMIC(Action_PolyOmino2, CPropertyPage)
extern Action_PolyOmino* pPolyOmino;
Action_PolyOmino2::Action_PolyOmino2()
	: CPropertyPage(IDD_POLYOMINO2)
	, m_Step(0.1)
	, m_Subdivision_Index(0)
	, m_MinIndex(0)
	, m_MaxIndex(0)
	, m_PuzzleSize(3)
	, m_PuzzleXNum(1)
	, m_PuzzleYNum(1)
{

}

Action_PolyOmino2::~Action_PolyOmino2()
{
}

void Action_PolyOmino2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STEP_POLYOMINO, m_Step);
	DDX_Text(pDX, IDC_EDIT_NUM_POLYOMINO2, m_Subdivision_Index);
	DDX_Text(pDX, IDC_EDIT_NUM_POLYOMINO_MININDEX, m_MinIndex);
	DDX_Text(pDX, IDC_EDIT_NUM_POLYOMINO_MAXINDEX, m_MaxIndex);
	DDX_Control(pDX, IDC_SPIN1, m_SpinRows);
	DDX_Control(pDX, IDC_SPIN2, m_spinColumns);
	DDX_Control(pDX, IDC_SPIN3, m_SpinPuzzleSize);
	DDX_Text(pDX, IDC_EDIT_NUM_POLYOMINO_PUZZLESIZE, m_PuzzleSize);
	DDX_Text(pDX, IDC_EDIT_NUM_POLYOMINO_XNUMBER, m_PuzzleXNum);
	DDX_Text(pDX, IDC_EDIT_NUM_POLYOMINO_YNUMBER, m_PuzzleYNum);
	DDX_Control(pDX, IDC_SPIN4, m_SpinXNum);
	DDX_Control(pDX, IDC_SPIN5, m_SpinYNum);
	DDV_MinMaxInt(pDX, m_PuzzleXNum, 0, 10000);
	DDV_MinMaxInt(pDX, m_PuzzleYNum, 0, 10000);
	DDV_MinMaxInt(pDX, m_MinIndex, 0, 10000);
	DDV_MinMaxInt(pDX, m_MaxIndex, 0, 10000);
	DDV_MinMaxInt(pDX, m_PuzzleSize, 0, 10000);
}


BEGIN_MESSAGE_MAP(Action_PolyOmino2, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SetMeshUV_Mesh_Polyomino, &Action_PolyOmino2::OnBnClickedButtonSetmeshuvMeshPolyomino)
	ON_BN_CLICKED(IDC_BUTTON_SetMeshUV_UV_Polyomino, &Action_PolyOmino2::OnBnClickedButtonSetmeshuvUvPolyomino)
	ON_BN_CLICKED(IDC_BUTTON_COPYSELECTEDLANDMARKVERTEXPOSITION, &Action_PolyOmino2::OnBnClickedButtonCopyselectedlandmarkvertexposition)
	ON_BN_CLICKED(IDC_BUTTON_LOADFINALRECTILINEARMAP, &Action_PolyOmino2::OnBnClickedButtonLoadfinalrectilinearmap)
	ON_BN_CLICKED(IDC_BUTTON_UP_POLYOMINO, &Action_PolyOmino2::OnBnClickedButtonUpPolyomino)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_POLYOMINO, &Action_PolyOmino2::OnBnClickedButtonRightPolyomino)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_POLYOMINO, &Action_PolyOmino2::OnBnClickedButtonDownPolyomino)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_POLYOMINO, &Action_PolyOmino2::OnBnClickedButtonLeftPolyomino)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_STEP_POLYOMINO, &Action_PolyOmino2::OnBnClickedButtonUpdateStepPolyomino)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FIRSTUNIFORMGRID, &Action_PolyOmino2::OnBnClickedButtonLoadFirstuniformgrid)
	ON_BN_CLICKED(IDC_BUTTON_INITIALIZEUNIFORMGRID, &Action_PolyOmino2::OnBnClickedButtonInitializeuniformgrid)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSELECTEDSHARPEDGES, &Action_PolyOmino2::OnBnClickedButtonClearselectedsharpedges)
	ON_BN_CLICKED(IDC_BUTTON_SetSourceSharpEdge, &Action_PolyOmino2::OnBnClickedButtonSetsourcesharpedge)
	ON_BN_CLICKED(IDC_BUTTON_SetTargetSharpEdge2, &Action_PolyOmino2::OnBnClickedButtonSettargetsharpedge2)
	ON_BN_CLICKED(IDC_BUTTON_MoveSourceToTarget, &Action_PolyOmino2::OnBnClickedButtonMovesourcetotarget)
	ON_BN_CLICKED(IDC_BUTTON_UpdateSharpStructure, &Action_PolyOmino2::OnBnClickedButtonUpdatesharpstructure)
	ON_BN_CLICKED(IDC_BUTTON_SUBDIVIDESHARPEDGE, &Action_PolyOmino2::OnBnClickedButtonSubdividesharpedge)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSelectedSharpEdges, &Action_PolyOmino2::OnBnClickedButtonDeleteselectedsharpedges)
	ON_BN_CLICKED(IDC_BUTTON_UP_POLYOMINO2, &Action_PolyOmino2::OnBnClickedButtonUpPolyomino2)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_POLYOMINO2, &Action_PolyOmino2::OnBnClickedButtonRightPolyomino2)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_POLYOMINO2, &Action_PolyOmino2::OnBnClickedButtonLeftPolyomino2)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_POLYOMINO2, &Action_PolyOmino2::OnBnClickedButtonDownPolyomino2)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSelectedSharpEdges2, &Action_PolyOmino2::OnBnClickedButtonDeleteselectedsharpedges2)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSelectedSharpEdges3, &Action_PolyOmino2::OnBnClickedButtonDeleteselectedsharpedges3)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSelectedSharpEdges4, &Action_PolyOmino2::OnBnClickedButtonDeleteselectedsharpedges4)
	ON_BN_CLICKED(IDC_BUTTON_Polyomino_GenerateNewPoint, &Action_PolyOmino2::OnBnClickedButtonPolyominoGeneratenewpoint)
	ON_BN_CLICKED(IDC_BUTTON_Polyomino_GenerateTwoPointLine, &Action_PolyOmino2::OnBnClickedButtonPolyominoGeneratetwopointline)
	ON_BN_CLICKED(IDC_BUTTON_Polyomino_GenerateNewPoint2Line, &Action_PolyOmino2::OnBnClickedButtonPolyominoGeneratenewpoint2line)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FinalUNIFORMGRID, &Action_PolyOmino2::OnBnClickedButtonLoadFinaluniformgrid)
	ON_BN_CLICKED(IDC_BUTTON_VisualizationPopulation, &Action_PolyOmino2::OnBnClickedButtonVisualizationpopulation)
	ON_BN_CLICKED(IDC_BUTTON_MIDDLECENTER, &Action_PolyOmino2::OnBnClickedButtonMiddlecenter)
	ON_BN_CLICKED(IDC_BUTTON_AVERAGE_HORIZONTAL, &Action_PolyOmino2::OnBnClickedButtonAverageHorizontal)
	ON_BN_CLICKED(IDC_BUTTON_AVERAGE_VERTICAL, &Action_PolyOmino2::OnBnClickedButtonAverageVertical)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SPIN3, &Action_PolyOmino2::OnThemechangedSpin3)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SPIN2, &Action_PolyOmino2::OnThemechangedSpin2)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SPIN1, &Action_PolyOmino2::OnThemechangedSpin1)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_ADDPUZZLE, &Action_PolyOmino2::OnBnClickedButtonAddpuzzle)
	ON_BN_CLICKED(IDC_BUTTON_LOADPUZZLE, &Action_PolyOmino2::OnBnClickedButtonLoadpuzzle)
	ON_BN_CLICKED(IDC_BUTTON_SAVEPUZZLE, &Action_PolyOmino2::OnBnClickedButtonSavepuzzle)
	ON_BN_CLICKED(IDC_BUTTON_UP_POLYOMINO_BASE, &Action_PolyOmino2::OnBnClickedButtonUpPolyominoBase)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_POLYOMINO_BASE, &Action_PolyOmino2::OnBnClickedButtonLeftPolyominoBase)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_POLYOMINO_BASE, &Action_PolyOmino2::OnBnClickedButtonDownPolyominoBase)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_POLYOMINO_BASE, &Action_PolyOmino2::OnBnClickedButtonRightPolyominoBase)
	ON_BN_CLICKED(IDC_BUTTON_SETFACEPOINTS_SELECTED, &Action_PolyOmino2::OnBnClickedButtonSetfacepointsSelected)
	ON_BN_CLICKED(IDC_BUTTON_SETFACEPOINTS_DELETE, &Action_PolyOmino2::OnBnClickedButtonSetfacepointsDelete)
	ON_BN_CLICKED(IDC_BUTTON_SaveDeletedFaces, &Action_PolyOmino2::OnBnClickedButtonSavedeletedfaces)
	ON_BN_CLICKED(IDC_BUTTON_SavePUZZLEPIECE, &Action_PolyOmino2::OnBnClickedButtonSavepuzzlepiece)
	ON_BN_CLICKED(IDC_BUTTON_SavePUZZLEPIECE2, &Action_PolyOmino2::OnBnClickedButtonSavepuzzlepiece2)
	ON_BN_CLICKED(IDC_BUTTON_LoadPUZZLEPIECE, &Action_PolyOmino2::OnBnClickedButtonLoadpuzzlepiece)
	ON_BN_CLICKED(IDC_BUTTON_CHECKPOINTWITHFIVEREGIONS, &Action_PolyOmino2::OnBnClickedButtonCheckpointwithfiveregions)
	ON_BN_CLICKED(IDC_BUTTON_USRASTRERIZATION, &Action_PolyOmino2::OnBnClickedButtonUsrastrerization)
	ON_BN_CLICKED(IDC_BUTTON_LOADCHINA, &Action_PolyOmino2::OnBnClickedButtonLoadchina)
	ON_BN_CLICKED(IDC_BUTTON_LOADCHINA_TRIANGULATE, &Action_PolyOmino2::OnBnClickedButtonLoadchinaTriangulate)
	ON_BN_CLICKED(IDC_BUTTON_SETALLLANDMARK_REGULAR, &Action_PolyOmino2::OnBnClickedButtonSetalllandmarkRegular)
	ON_BN_CLICKED(IDC_BUTTON_DELETEDUPLICATEPOINTSINLANDMARK, &Action_PolyOmino2::OnBnClickedButtonDeleteduplicatepointsinlandmark)
	ON_BN_CLICKED(IDC_BUTTON_LOADCHINASTRAIGHTENED, &Action_PolyOmino2::OnBnClickedButtonLoadchinastraightened)
	ON_BN_CLICKED(IDC_BUTTON_SAVEDATAFROMMESH, &Action_PolyOmino2::OnBnClickedButtonSavedatafrommesh)
	ON_BN_CLICKED(IDC_BUTTON_LOADRECTILINEARMESH, &Action_PolyOmino2::OnBnClickedButtonLoadrectilinearmesh)
	ON_BN_CLICKED(IDC_BUTTON_MODIFYRECTILINEAR3414, &Action_PolyOmino2::OnBnClickedButtonModifyrectilinear3414)
	ON_BN_CLICKED(IDC_BUTTON_MODIFYRECTILINEARMESH3350, &Action_PolyOmino2::OnBnClickedButtonModifyrectilinearmesh3350)
	ON_BN_CLICKED(IDC_BUTTON_LoadFinalChinaPolyomino, &Action_PolyOmino2::OnBnClickedButtonLoadfinalchinapolyomino)
END_MESSAGE_MAP()


// Action_PolyOmino2 message handlers

extern CFormView3 *p_FormView3;
void Action_PolyOmino2::OnBnClickedButtonSetmeshuvMeshPolyomino()
{
	// TODO: Add your control notification handler code here
	pMesh = pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
	char message[80];
	sprintf(message, "%d", p_FormView3->currentindex);
	GetDlgItem(IDC_BUTTON_SetMeshUV_Mesh_Text_Polyomino)->SetWindowText(message);
}


void Action_PolyOmino2::OnBnClickedButtonSetmeshuvUvPolyomino()
{
	// TODO: Add your control notification handler code here
	pMesh_UV = pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
	char message[80];
	sprintf(message, "%d", p_FormView3->currentindex);
	GetDlgItem(IDC_BUTTON_SetMeshUV_UV_Text_Polyomino)->SetWindowText(message);
}

extern char Post_Message[1000];
void Action_PolyOmino2::OnBnClickedButtonCopyselectedlandmarkvertexposition()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if (!pMesh)
	{
		sprintf(Post_Message, "Please select the mesh!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	if (!pMesh_UV)
	{
		sprintf(Post_Message, "Please select the parameterization!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		return;
	}
	CMesh3d* pMeshCopy = NULL;
	CMesh3d* pMeshUVCopy = NULL;
	if (pMesh->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh = pMeshCopy = ((CMesh3d_OpenMesh*)pMesh)->ConvertMesh3d();
	if (pMesh_UV->GetType() == TYPE_MESH3D_OPENMESH)
		pMesh_UV = pMeshUVCopy = ((CMesh3d_OpenMesh*)pMesh_UV)->ConvertMesh3d();
	//if (pMesh->GetType() == TYPE_MESH3D && pMesh_UV->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pMesh;
		CMesh3d* pMesh3dUV = (CMesh3d*)pMesh_UV;

		for (int i = 0; i < pMesh3dUV->m_SelectedLandmarkIndexArray.size(); i++)
		{
			int lindex = pMesh3dUV->m_SelectedLandmarkIndexArray[i];
			GeoMapLandMark* pLandmark = pMesh3dUV->m_LandMarkArray[lindex];

			for (int j = 0; j < pLandmark->landmark_points.size(); j++)
			{
				int vindex = pLandmark->landmark_points[j];
				CVertex3d* pVertex = pMesh3d->GetVertex(vindex);
				CVertex3d* pVertex1 = pMesh3dUV->GetVertex(vindex);
				pVertex1->Set(pVertex->x(), pVertex->y(), pVertex->z());
			}
		}
		
		sprintf(Post_Message, "Type: both mesh3d, Copy Vertex Position Successfully!");
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		if (!pMeshCopy)
			delete pMeshCopy;
		if (!pMeshUVCopy)
			delete pMeshUVCopy;
		return;
	}
}


void Action_PolyOmino2::OnBnClickedButtonLoadfinalrectilinearmap()
{
	// TODO: Add your control notification handler code here
	//find the latest saved map
	pPolyOmino->GetLatestWorldMapIndex();

	//find the latest saved landmark
	pPolyOmino->GetLatestLandmarkIndex();

	//find the latest selected landmark index
	//pPolyOmino->GetLatestSelectedLandmarkIndex();

	//GetLatestWorldMap
	//pPolyOmino->GetLatestWorldMap();
	GetFinalRectilinearGeomap();

	//GetLatestLandmark
	pPolyOmino->GetLatestLandmark();
}
void Action_PolyOmino2::GetFinalRectilinearGeomap()
{
	char filename[180];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\finalrectilinearmap\\worldmaptriangulation_coarse_more253.m");
	pDoc->OnOpenDocument(filename);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	pPolyOmino->OnBnClickedGeneratelandmarks();
	pPolyOmino->pFinalRectilinearMesh = pPolyOmino->pMesh;
}
void Action_PolyOmino2::UpdateSelectedLandmarkPosition(CMesh3d* pMesh, double deltax, double deltay)
{
	std::vector<int> m_ArrayPointArray;
	for (int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
	{
		int landmarkindex = pMesh->m_SelectedLandmarkIndexArray[i];
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[landmarkindex];
		for (int j = 0; j < pLandmark->landmark_points.size(); j++)
		{
			int vindex = pLandmark->landmark_points[j];
			if(std::find(m_ArrayPointArray.begin(), m_ArrayPointArray.end(),vindex) == m_ArrayPointArray.end())
				m_ArrayPointArray.push_back(vindex);
		}
	}

	for (int i = 0; i < m_ArrayPointArray.size(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(m_ArrayPointArray[i]);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		pVertex->Set(x + deltax, y + deltay, z);
	}
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino2::UpdateSelectedSharpEdgePosition_(CMesh3d* pMesh, double deltax, double deltay)
{
	if (pMesh->m_ArraySelectedSharpEdgeIndex.size() == 0)
		return;

	//first find the 
	std::pair<int, int> pair = pMesh->m_ArraySharpEdges[pMesh->m_ArraySelectedSharpEdgeIndex[0]];
	int startvindex = pair.first;
	int endvindex = pair.second;
	CVertex3d* pStartVertex = pMesh->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh->GetVertex(endvindex);

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	int index;
	double x, y;
	if (fabs(x1 - x2) < 1e-8)
	{
		//Vertical
		index = -1;
		for (int j = 0; j < pMesh->xinterval.size(); j++)
		{
			if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
			{
				index = j;
				break;
			}
		}
		if (index < 0)
			ASSERT(0);

		x = pMesh->xinterval[index] + deltax;

		for (int i = 0; i < pPolyOmino->m_VerticalSegments[index].size(); i++)
		{
			int edgeindex = pPolyOmino->m_VerticalSegments[index][i];
			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[edgeindex];
			int startvindex1 = pair1.first;
			int endvindex1 = pair1.second;
			CVertex3d* pStartVertex1 = pMesh->GetVertex(startvindex1);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(endvindex1);

			pStartVertex1->x(x);
			pEndVertex1->x(x);
		}
		if (x > pMesh->xinterval[index + 1])
		{
			//swap
			std::vector<int> temp = pPolyOmino->m_VerticalSegments[index + 1];
			pPolyOmino->m_VerticalSegments[index + 1] = pPolyOmino->m_VerticalSegments[index];
			pPolyOmino->m_VerticalSegments[index] = temp;
		}else
		if (x < pMesh->xinterval[index - 1])
		{
			//swap
			std::vector<int> temp = pPolyOmino->m_VerticalSegments[index - 1];
			pPolyOmino->m_VerticalSegments[index - 1] = pPolyOmino->m_VerticalSegments[index];
			pPolyOmino->m_VerticalSegments[index] = temp;
		}
	}
	else
	{
		index = -1;
		for (int j = 0; j < pMesh->yinterval.size(); j++)
		{
			if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
			{
				index = j;
				break;
			}
		}
		if (index < 0)
			ASSERT(0);

		y = pMesh->yinterval[index] + deltay;

		for (int i = 0; i < pPolyOmino->m_HorizontalSegments[index].size(); i++)
		{
			int edgeindex = pPolyOmino->m_HorizontalSegments[index][i];
			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[edgeindex];
			int startvindex1 = pair1.first;
			int endvindex1 = pair1.second;
			CVertex3d* pStartVertex1 = pMesh->GetVertex(startvindex1);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(endvindex1);

			pStartVertex1->y(y);
			pEndVertex1->y(y);
		}
		if (y > pMesh->yinterval[index + 1])
		{
			//swap
			std::vector<int> temp = pPolyOmino->m_HorizontalSegments[index + 1];
			pPolyOmino->m_HorizontalSegments[index + 1] = pPolyOmino->m_HorizontalSegments[index];
			pPolyOmino->m_HorizontalSegments[index] = temp;
		}
		else
			if (y < pMesh->yinterval[index - 1])
			{
				//swap
				std::vector<int> temp = pPolyOmino->m_HorizontalSegments[index - 1];
				pPolyOmino->m_HorizontalSegments[index - 1] = pPolyOmino->m_HorizontalSegments[index];
				pPolyOmino->m_HorizontalSegments[index] = temp;
			}
	}
	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);

	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino2::UpdateSelectedSharpEdgePosition(CMesh3d* pMesh, int deltax, int deltay)
{
	std::vector<int> m_SelectedSharpEdge;
	for (int i = 0; i < pMesh->m_ArraySelectedSharpEdgeIndex.size(); i++)
	{
		if (std::find(m_SelectedSharpEdge.begin(), m_SelectedSharpEdge.end(), pMesh->m_ArraySelectedSharpEdgeIndex[i]) == m_SelectedSharpEdge.end())
		{
			m_SelectedSharpEdge.push_back(pMesh->m_ArraySelectedSharpEdgeIndex[i]);
		}
	}
	int verticalindex, horizontalindex;
	for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
	{
		std::pair<int, int> pair = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[i]];
		int startvindex = pair.first;
		int endvindex = pair.second;
		CVertex3d* pStartVertex = pMesh->GetVertex(startvindex);
		CVertex3d* pEndVertex = pMesh->GetVertex(endvindex);

		double x1 = pStartVertex->x();
		double y1 = pStartVertex->y();
		double x2 = pEndVertex->x();
		double y2 = pEndVertex->y();

		verticalindex = -1;
		horizontalindex = -1;
		for (int j = 0; j < pPolyOmino->m_VerticalSegments.size(); j++)
		{
			if (std::find(pPolyOmino->m_VerticalSegments[j].begin(), pPolyOmino->m_VerticalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_VerticalSegments[j].end())
			{
				verticalindex = j;
				break;
			}
		}

		for (int j = 0; j < pPolyOmino->m_HorizontalSegments.size(); j++)
		{
			if (std::find(pPolyOmino->m_HorizontalSegments[j].begin(), pPolyOmino->m_HorizontalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_HorizontalSegments[j].end())
			{
				horizontalindex = j;
				break;
			}
		}

		int index;
		if (verticalindex > -1)
		{
			//Vertical
			index = verticalindex;
			/*for (int j = 0; j < pPolyOmino->m_VerticalSegments.size(); j++)
			{
				if (std::find(pPolyOmino->m_VerticalSegments[j].begin(), pPolyOmino->m_VerticalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_VerticalSegments[j].end())
				{
					index = j;
					break;
				}
			}*/
			/*for (int j = 0; j < pMesh->xinterval.size(); j++)
			{
				if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
				{
					index = j;
					break;
				}
			}*/
			if (index < 0)
				ASSERT(0);

			pStartVertex->x(pMesh->xinterval[index + deltax]);
			pEndVertex->x(pMesh->xinterval[index + deltax]);

			//Update the vertical array	
			if(std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_VerticalSegments[index].end())
				pPolyOmino->m_VerticalSegments[index].erase(std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]));
			pPolyOmino->m_VerticalSegments[index + deltax].push_back(m_SelectedSharpEdge[i]);
			if (pPolyOmino->m_VerticalSegments[index].size() == 0)
			{
				pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + index);
			}
		}
		else
		{
			index = horizontalindex;
			/*for (int j = 0; j < pPolyOmino->m_HorizontalSegments.size(); j++)
			{
				if (std::find(pPolyOmino->m_HorizontalSegments[j].begin(), pPolyOmino->m_HorizontalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_HorizontalSegments[j].end())
				{
					index = j;
					break;
				}
			}*/
			/*for (int j = 0; j < pMesh->yinterval.size(); j++)
			{
				if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
				{
					index = j;
					break;
				}
			}*/
			if (index < 0)
				ASSERT(0);

			pStartVertex->y(pMesh->yinterval[index + deltay]);
			pEndVertex->y(pMesh->yinterval[index + deltay]);

			//Update the horizontal array
			if(std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_HorizontalSegments[index].end())
				pPolyOmino->m_HorizontalSegments[index].erase(std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]));
			pPolyOmino->m_HorizontalSegments[index + deltay].push_back(m_SelectedSharpEdge[i]);
			if (pPolyOmino->m_HorizontalSegments[index].size() == 0)
			{
				pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + index);
			}	
		}
	}
	//Update Uniform Grid
	/*OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);*/

	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino2::OnBnClickedButtonUpPolyomino()
{
	// TODO: Add your control notification handler code here
	//UpdateSelectedLandmarkPosition(pPolyOmino->pMesh, 0, m_Step);
	UpdateSelectedSharpEdgePosition(pPolyOmino->pUniformGridMesh, 0, 1);
}


void Action_PolyOmino2::OnBnClickedButtonRightPolyomino()
{
	// TODO: Add your control notification handler code here
	//UpdateSelectedLandmarkPosition(pPolyOmino->pMesh, m_Step, 0);
	UpdateSelectedSharpEdgePosition(pPolyOmino->pUniformGridMesh, 1, 0);
}


void Action_PolyOmino2::OnBnClickedButtonDownPolyomino()
{
	// TODO: Add your control notification handler code here
	//UpdateSelectedLandmarkPosition(pPolyOmino->pMesh, 0, -m_Step);
	UpdateSelectedSharpEdgePosition(pPolyOmino->pUniformGridMesh, 0, -1);
}


void Action_PolyOmino2::OnBnClickedButtonLeftPolyomino()
{
	// TODO: Add your control notification handler code here
	//UpdateSelectedLandmarkPosition(pPolyOmino->pMesh, -m_Step, 0);
	UpdateSelectedSharpEdgePosition(pPolyOmino->pUniformGridMesh, -1, 0);
}


void Action_PolyOmino2::OnBnClickedButtonUpdateStepPolyomino()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (pPolyOmino->pUniformGridMesh)
	{
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleXPos = m_MinIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleYPos = m_MaxIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleSize = m_PuzzleSize;
	}
}


void Action_PolyOmino2::OnBnClickedButtonLoadFirstuniformgrid()
{
	// TODO: Add your control notification handler code here
	/*char filename[180];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\finalrectilinearmap\\worldmapUniformGrid.m");
	pDoc->OnOpenDocument(filename);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showSharpEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = FALSE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	//pPolyOmino->OnBnClickedGeneratelandmarks();
	pPolyOmino->pUniformGridMesh = pPolyOmino->pMesh;*/
	CString FileName("D:\\papers\\code\\data\\Geo\\ditu\\dense\\5625.m");
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;

	//int iterationnum;
	pPolyOmino->pMesh = NULL;
	pPolyOmino->LoadData(FileName, iterationnum);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showSharpEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = FALSE;
	pPolyOmino->pMesh->m_showFace = FALSE;
}


void Action_PolyOmino2::OnBnClickedButtonInitializeuniformgrid()
{
	// TODO: Add your control notification handler code here
	pPolyOmino->pUniformGridMesh->xinterval.clear();
	pPolyOmino->pUniformGridMesh->yinterval.clear();
	std::pair<int, int> edgepair;
	double x, y;
	for (int i = 0; i < pPolyOmino->m_HorizontalSegments.size(); i++)
	{
		int edgeindex = pPolyOmino->m_HorizontalSegments[i][0];
		edgepair = pPolyOmino->pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex = pPolyOmino->pUniformGridMesh->GetVertex(edgepair.first);
		y = pVertex->y();
		pPolyOmino->pUniformGridMesh->yinterval.push_back(y);
	}
	std::vector<int> edgeset;
	for (int i = 0; i < pPolyOmino->m_VerticalSegments.size(); i++)
	{
		edgeset = pPolyOmino->m_VerticalSegments[i];
		int edgeindex = pPolyOmino->m_VerticalSegments[i][0];
		edgepair = pPolyOmino->pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex = pPolyOmino->pUniformGridMesh->GetVertex(edgepair.first);
		x = pVertex->x();
		pPolyOmino->pUniformGridMesh->xinterval.push_back(x);
	}
}


void Action_PolyOmino2::OnBnClickedButtonClearselectedsharpedges()
{
	// TODO: Add your control notification handler code here
	pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex.clear();
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonSetsourcesharpedge()
{
	// TODO: Add your control notification handler code here
	m_SourceSharpEdges = pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex;
}


void Action_PolyOmino2::OnBnClickedButtonSettargetsharpedge2()
{
	// TODO: Add your control notification handler code here
	m_TargetSharpEdges = pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex;
}


void Action_PolyOmino2::OnBnClickedButtonMovesourcetotarget()
{
	// TODO: Add your control notification handler code here
}

void Action_PolyOmino2::MergeHorizontalAndVerticalSegments()
{
	int flag = 0;
	while (1)
	{
		flag = 0;
		for (int i = 0; i < pPolyOmino->pUniformGridMesh->xinterval.size() - 1; i++)
		{
			if (fabs(pPolyOmino->pUniformGridMesh->xinterval[i] - pPolyOmino->pUniformGridMesh->xinterval[i + 1]) < 1e-2)
			{
				pPolyOmino->m_VerticalSegments[i].insert(pPolyOmino->m_VerticalSegments[i].begin(), pPolyOmino->m_VerticalSegments[i + 1].begin(), pPolyOmino->m_VerticalSegments[i + 1].end());
				pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + i + 1);
				pPolyOmino->pUniformGridMesh->xinterval.erase(pPolyOmino->pUniformGridMesh->xinterval.begin() + i + 1);
				flag = 1;
				break;
			}
		}
		for (int i = 0; i < pPolyOmino->pUniformGridMesh->yinterval.size() - 1; i++)
		{
			if (fabs(pPolyOmino->pUniformGridMesh->yinterval[i] - pPolyOmino->pUniformGridMesh->yinterval[i + 1]) < 1e-2)
			{
				pPolyOmino->m_HorizontalSegments[i].insert(pPolyOmino->m_HorizontalSegments[i].begin(), pPolyOmino->m_HorizontalSegments[i + 1].begin(), pPolyOmino->m_HorizontalSegments[i + 1].end());
				pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + i + 1);
				pPolyOmino->pUniformGridMesh->yinterval.erase(pPolyOmino->pUniformGridMesh->yinterval.begin() + i + 1);
				flag = 1;
				break;
			}
		}


		for (int i = 0; i < pPolyOmino->pUniformGridMesh->xinterval.size() - 1; i++)
		{
			if (pPolyOmino->pUniformGridMesh->xinterval[i] > pPolyOmino->pUniformGridMesh->xinterval[i + 1])
			{
				std::vector<int> edgeset = pPolyOmino->m_VerticalSegments[i + 1];
				pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + i + 1);
				pPolyOmino->m_VerticalSegments.insert(pPolyOmino->m_VerticalSegments.begin() + i, edgeset);
				//pPolyOmino->m_VerticalSegments[i].insert(pPolyOmino->m_VerticalSegments[i].begin(), pPolyOmino->m_VerticalSegments[i + 1].begin(), pPolyOmino->m_VerticalSegments[i + 1].end());
				//pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + i + 1);
				double val = pPolyOmino->pUniformGridMesh->xinterval[i + 1];
				pPolyOmino->pUniformGridMesh->xinterval.erase(pPolyOmino->pUniformGridMesh->xinterval.begin() + i + 1);
				pPolyOmino->pUniformGridMesh->xinterval.insert(pPolyOmino->pUniformGridMesh->xinterval.begin() + i, val);
				flag = 1;
				break;
			}
		}
		for (int i = 0; i < pPolyOmino->pUniformGridMesh->yinterval.size() - 1; i++)
		{
			if (pPolyOmino->pUniformGridMesh->yinterval[i] > pPolyOmino->pUniformGridMesh->yinterval[i + 1])
			{
				std::vector<int> edgeset = pPolyOmino->m_HorizontalSegments[i + 1];
				pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + i + 1);
				pPolyOmino->m_HorizontalSegments.insert(pPolyOmino->m_HorizontalSegments.begin() + i, edgeset);
				//pPolyOmino->m_VerticalSegments[i].insert(pPolyOmino->m_VerticalSegments[i].begin(), pPolyOmino->m_VerticalSegments[i + 1].begin(), pPolyOmino->m_VerticalSegments[i + 1].end());
				//pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + i + 1);
				double val = pPolyOmino->pUniformGridMesh->yinterval[i + 1];
				pPolyOmino->pUniformGridMesh->yinterval.erase(pPolyOmino->pUniformGridMesh->yinterval.begin() + i + 1);
				pPolyOmino->pUniformGridMesh->yinterval.insert(pPolyOmino->pUniformGridMesh->yinterval.begin() + i, val);
				flag = 1;
				break;
			}
		}
		if (!flag)
			break;
	}
}
void Action_PolyOmino2::OnBnClickedButtonUpdatesharpstructure()
{
	// TODO: Add your control notification handler code here
	//Delete sharp edges with zero length
	pPolyOmino->pUniformGridMesh->m_ArraySharpEdgesFlag.resize(pPolyOmino->pUniformGridMesh->m_ArraySharpEdges.size(), 0);
	OnBnClickedButtonInitializeuniformgrid();
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	MergeHorizontalAndVerticalSegments();

	OnBnClickedButtonInitializeuniformgrid();
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	pPolyOmino->DeleteDuplicateVertices();
	pPolyOmino->InsertSavedEdgeSet();
	if (pPolyOmino->m_DeletedEdgeArray.size() > 0)
		pPolyOmino->RemoveEdgeSet(pPolyOmino->m_DeletedEdgeArray);
	//pPolyOmino->DeleteUnusedVertices();
	//if (pPolyOmino->m_DeletedEdgeArray.size() > 0)
	//	pPolyOmino->RemoveEdgeSet(pPolyOmino->m_DeletedEdgeArray);
	//pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();
	//pPolyOmino->InitializeHorizontalandVerticalSegmentArray();
	//OnBnClickedButtonInitializeuniformgrid();
	pPolyOmino->MergeConnectedSegments();
	pPolyOmino->InsertSavedEdgeSet();
	if (pPolyOmino->m_DeletedEdgeArray.size() > 0)
		pPolyOmino->RemoveEdgeSet(pPolyOmino->m_DeletedEdgeArray);
	//pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();
	//pPolyOmino->InitializeHorizontalandVerticalSegmentArray();
	//OnBnClickedButtonInitializeuniformgrid();
	pPolyOmino->DeleteUnusedVertices();
	if (pPolyOmino->m_DeletedEdgeArray.size() > 0)
		pPolyOmino->RemoveEdgeSet(pPolyOmino->m_DeletedEdgeArray);
	OnBnClickedButtonInitializeuniformgrid();
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();
	//pPolyOmino->InitializeHorizontalandVerticalSegmentArray();

	//
	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\final\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\final\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonSubdividesharpedge()
{
	// TODO: Add your control notification handler code here
	if (pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex.size() != 2)
		return;
	if (pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex.size() < 1)
		return;
	int vindex1 = pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex[0];
	int vindex2 = pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex[1];
	int eindex = pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex[0];

	UpdateData(TRUE);

	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;

	CVertex3d* pStartVertex = pMesh->GetVertex(vindex1);
	CVertex3d* pEndVertex = pMesh->GetVertex(vindex2);

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	int xindex1,yindex1,xindex2,yindex2;

	xindex1 = -1;
	yindex1 = -1;
	xindex2 = -1;
	yindex2 = -1;
	for (int j = 0; j < pMesh->xinterval.size(); j++)
	{
		if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
		{
			xindex1 = j;
			break;
		}
	}
	for (int j = 0; j < pMesh->yinterval.size(); j++)
	{
		if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
		{
			yindex1 = j;
			break;
		}
	}

	for (int j = 0; j < pMesh->xinterval.size(); j++)
	{
		if (fabs(x2 - pMesh->xinterval[j]) < 1e-8)
		{
			xindex2 = j;
			break;
		}
	}
	for (int j = 0; j < pMesh->yinterval.size(); j++)
	{
		if (fabs(y2 - pMesh->yinterval[j]) < 1e-8)
		{
			yindex2 = j;
			break;
		}
	}
	double x, y;
	CVertex3d* pNewVertex1, *pNewVertex2;
	pNewVertex1 = new CVertex3d();
	pNewVertex2 = new CVertex3d();
	int startvindex, endvindex;
	if (fabs(x1 - x2) < 1e-8)
	{
		//Vertical
		int sign = 1;
		if (yindex2 < yindex1)
			sign = -1;
		y = pMesh->yinterval[yindex1 + m_Subdivision_Index * sign];
		x = x1;

		pNewVertex1->Set(x, y, 0);
		pNewVertex2->Set(x, y, 0);
		pMesh->AddVertex(pNewVertex1);
		pMesh->AddVertex(pNewVertex2);
		pMesh->m_SetVertexFlagAsItsIndex = 0;
		pMesh->SetVertexFlagAsItsIndex();

		startvindex = pStartVertex->m_Flag;
		endvindex = pEndVertex->m_Flag;
		std::pair<int, int> newedgepair1, newedgepair2, newedgepair3;
		newedgepair1.first = startvindex;
		newedgepair1.second = pNewVertex1->m_Flag;
		newedgepair2.first = pNewVertex1->m_Flag;
		newedgepair2.second = pNewVertex2->m_Flag;
		newedgepair3.first = pNewVertex2->m_Flag;
		newedgepair3.second = endvindex;

		pMesh->m_ArraySharpEdges[eindex] = newedgepair1;

		//pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + eindex);
		int newedgeindex1 = eindex;
		int newedgeindex2 = pMesh->m_ArraySharpEdges.size();
		int newedgeindex3 = pMesh->m_ArraySharpEdges.size() + 1;
		//pMesh->m_ArraySharpEdges.push_back(newedgepair1);
		pMesh->m_ArraySharpEdges.push_back(newedgepair2);
		pMesh->m_ArraySharpEdges.push_back(newedgepair3);

		//Update the vertical array	
		//pPolyOmino->m_VerticalSegments[xindex1].erase(std::find(pPolyOmino->m_VerticalSegments[xindex2].begin(), pPolyOmino->m_VerticalSegments[xindex2].end(), eindex));
		//pPolyOmino->m_VerticalSegments[xindex1].push_back(newedgeindex1);
		pPolyOmino->m_VerticalSegments[xindex1].push_back(newedgeindex3);
		pPolyOmino->m_HorizontalSegments[yindex1 + m_Subdivision_Index * sign].push_back(newedgeindex2);
	}
	else
	{
		//Horizontal
		int sign = 1;
		if (xindex2 < xindex1)
			sign = -1;
		x = pMesh->xinterval[xindex1 + m_Subdivision_Index * sign];
		y = y1;

		pNewVertex1->Set(x, y, 0);
		pNewVertex2->Set(x, y, 0);
		pMesh->AddVertex(pNewVertex1);
		pMesh->AddVertex(pNewVertex2);
		pMesh->m_SetVertexFlagAsItsIndex = 0;
		pMesh->SetVertexFlagAsItsIndex();

		startvindex = pStartVertex->m_Flag;
		endvindex = pEndVertex->m_Flag;
		std::pair<int, int> newedgepair1, newedgepair2, newedgepair3;
		newedgepair1.first = startvindex;
		newedgepair1.second = pNewVertex1->m_Flag;
		newedgepair2.first = pNewVertex1->m_Flag;
		newedgepair2.second = pNewVertex2->m_Flag;
		newedgepair3.first = pNewVertex2->m_Flag;
		newedgepair3.second = endvindex;

		pMesh->m_ArraySharpEdges[eindex] = newedgepair1;

		int newedgeindex1 = eindex;
		int newedgeindex2 = pMesh->m_ArraySharpEdges.size();
		int newedgeindex3 = pMesh->m_ArraySharpEdges.size() + 1;
		//pMesh->m_ArraySharpEdges.push_back(newedgepair1);
		pMesh->m_ArraySharpEdges.push_back(newedgepair2);
		pMesh->m_ArraySharpEdges.push_back(newedgepair3);

		//Update the Horizontal array	
		//pPolyOmino->m_HorizontalSegments[yindex1].erase(std::find(pPolyOmino->m_HorizontalSegments[yindex1].begin(), pPolyOmino->m_HorizontalSegments[yindex1].end(), eindex));
		//pPolyOmino->m_HorizontalSegments[yindex1].push_back(newedgeindex1);
		pPolyOmino->m_HorizontalSegments[yindex1].push_back(newedgeindex3);
		pPolyOmino->m_VerticalSegments[xindex1 + m_Subdivision_Index * sign].push_back(newedgeindex2);
	}
	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);

	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonDeleteselectedsharpedges()
{
	// TODO: Add your control notification handler code here
	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;
	
	std::vector<int> m_SelectedSharpEdge;
	for (int i = 0; i < pMesh->m_ArraySelectedSharpEdgeIndex.size(); i++)
	{
		if (std::find(m_SelectedSharpEdge.begin(), m_SelectedSharpEdge.end(), pMesh->m_ArraySelectedSharpEdgeIndex[i]) == m_SelectedSharpEdge.end())
		{
			m_SelectedSharpEdge.push_back(pMesh->m_ArraySelectedSharpEdgeIndex[i]);
		}
	}
	struct sort_pred {
		bool operator()(const int &left, const int &right)
		{
			return left > right;
		}
	};
	std::sort(m_SelectedSharpEdge.begin(), m_SelectedSharpEdge.end(), sort_pred());

	std::vector<int> m_ArraySharpEdgeIndex;
	m_ArraySharpEdgeIndex.resize(pMesh->m_ArraySharpEdges.size());
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
		m_ArraySharpEdgeIndex[i] = i;

	for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
	{
		for (int j = m_SelectedSharpEdge[i] + 1; j < m_ArraySharpEdgeIndex.size(); j++)
		{
			m_ArraySharpEdgeIndex[j]--;
		}
	}
	int flag = 0;
	for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
	{
		flag = 0;
		for (int j = 0; j < pPolyOmino->m_VerticalSegments.size(); j++)
		{
			if (std::find(pPolyOmino->m_VerticalSegments[j].begin(), pPolyOmino->m_VerticalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_VerticalSegments[j].end())
			{
				pPolyOmino->m_VerticalSegments[j].erase(std::find(pPolyOmino->m_VerticalSegments[j].begin(), pPolyOmino->m_VerticalSegments[j].end(), m_SelectedSharpEdge[i]));
				flag = 1;
				break;
			}
		}
		if (flag)
			continue;
		for (int j = 0; j < pPolyOmino->m_HorizontalSegments.size(); j++)
		{
			if (std::find(pPolyOmino->m_HorizontalSegments[j].begin(), pPolyOmino->m_HorizontalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_HorizontalSegments[j].end())
			{
				pPolyOmino->m_HorizontalSegments[j].erase(std::find(pPolyOmino->m_HorizontalSegments[j].begin(), pPolyOmino->m_HorizontalSegments[j].end(), m_SelectedSharpEdge[i]));
				flag = 1;
				break;
			}
		}

		/*std::pair<int, int> pair = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[i]];
		int startvindex = pair.first;
		int endvindex = pair.second;
		CVertex3d* pStartVertex = pMesh->GetVertex(startvindex);
		CVertex3d* pEndVertex = pMesh->GetVertex(endvindex);

		double x1 = pStartVertex->x();
		double y1 = pStartVertex->y();
		double x2 = pEndVertex->x();
		double y2 = pEndVertex->y();
		int index;
		if (fabs(x1 - x2) < 1e-8)
		{
			//Vertical
			index = -1;
			for (int j = 0; j < pMesh->xinterval.size(); j++)
			{
				if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
				{
					index = j;
					break;
				}
			}
			if (index < 0)
				ASSERT(0);

			//pStartVertex->x(pMesh->xinterval[index + deltax]);
			//pEndVertex->x(pMesh->xinterval[index + deltax]);

			//Update the vertical array	
			pPolyOmino->m_VerticalSegments[index].erase(std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]));
			//pPolyOmino->m_VerticalSegments[index + deltax].push_back(m_SelectedSharpEdge[i]);
			//if (pPolyOmino->m_VerticalSegments[index].size() == 0)
			//{
			//	pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + index);
			//}
		}
		else
		{
			index = -1;
			for (int j = 0; j < pMesh->yinterval.size(); j++)
			{
				if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
				{
					index = j;
					break;
				}
			}
			if (index < 0)
				ASSERT(0);

			//pStartVertex->y(pMesh->yinterval[index + deltay]);
			//pEndVertex->y(pMesh->yinterval[index + deltay]);

			//Update the horizontal array
			pPolyOmino->m_HorizontalSegments[index].erase(std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]));
			//pPolyOmino->m_HorizontalSegments[index + deltay].push_back(m_SelectedSharpEdge[i]);
			//if (pPolyOmino->m_HorizontalSegments[index].size() == 0)
			//{
			//	pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + index);
			//}
		}*/
	}

	for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
	{
		pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + m_SelectedSharpEdge[i]);
	}
	for (int i = 0; i < pPolyOmino->m_HorizontalSegments.size(); i++)
	{
		for (int j = 0; j < pPolyOmino->m_HorizontalSegments[i].size(); j++)
		{
			pPolyOmino->m_HorizontalSegments[i][j] = m_ArraySharpEdgeIndex[pPolyOmino->m_HorizontalSegments[i][j]];
		}
	}

	for (int i = 0; i < pPolyOmino->m_VerticalSegments.size(); i++)
	{
		for (int j = 0; j < pPolyOmino->m_VerticalSegments[i].size(); j++)
		{
			pPolyOmino->m_VerticalSegments[i][j] = m_ArraySharpEdgeIndex[pPolyOmino->m_VerticalSegments[i][j]];
		}
	}

	for (int i = 0; i < pPolyOmino->m_HorizontalSegments.size(); i++)
	{
		if (pPolyOmino->m_HorizontalSegments[i].size() == 0)
		{
			pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < pPolyOmino->m_VerticalSegments.size(); i++)
	{
		if (pPolyOmino->m_VerticalSegments[i].size() == 0)
		{
			pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + i);
			i--;
		}
	}

	//Update Uniform Grid
	//OnBnClickedButtonInitializeuniformgrid();
	//
	//pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	//iterationnum++;
	//char filename[200];
	//sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	//pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	//sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	//pPolyOmino->SaveDebugInformation(filename);
	pMesh->m_ArraySelectedSharpEdgeIndex.clear();
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonUpPolyomino2()
{
	// TODO: Add your control notification handler code here
	UpdateSelectedSharpEdgePosition_(pPolyOmino->pUniformGridMesh, 0, m_Step);
}


void Action_PolyOmino2::OnBnClickedButtonRightPolyomino2()
{
	// TODO: Add your control notification handler code here
	UpdateSelectedSharpEdgePosition_(pPolyOmino->pUniformGridMesh, m_Step, 0);
}


void Action_PolyOmino2::OnBnClickedButtonLeftPolyomino2()
{
	// TODO: Add your control notification handler code here
	UpdateSelectedSharpEdgePosition_(pPolyOmino->pUniformGridMesh, -m_Step, 0);
}


void Action_PolyOmino2::OnBnClickedButtonDownPolyomino2()
{
	// TODO: Add your control notification handler code here
	UpdateSelectedSharpEdgePosition_(pPolyOmino->pUniformGridMesh, 0, -m_Step);
}


void Action_PolyOmino2::OnBnClickedButtonDeleteselectedsharpedges2()
{
	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;
	//Up/Left
	std::vector<int> m_SelectedSharpEdge;
	for (int i = 0; i < pMesh->m_ArraySelectedSharpEdgeIndex.size(); i++)
	{
		if (std::find(m_SelectedSharpEdge.begin(), m_SelectedSharpEdge.end(), pMesh->m_ArraySelectedSharpEdgeIndex[i]) == m_SelectedSharpEdge.end())
		{
			m_SelectedSharpEdge.push_back(pMesh->m_ArraySelectedSharpEdgeIndex[i]);
		}
	}

	std::pair<int, int> pair = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[0]];
	int startvindex = pair.first;
	int endvindex = pair.second;
	CVertex3d* pStartVertex = pMesh->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh->GetVertex(endvindex);

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	int index;
	double x, y;
	if (fabs(x1 - x2) < 1e-8)
	{
		//Vertical
		index = -1;
		for (int j = 0; j < pMesh->xinterval.size(); j++)
		{
			if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
			{
				index = j;
				break;
			}
		}
		if (index < 0)
			ASSERT(0);

		x = (pMesh->xinterval[index] + pMesh->xinterval[index - 1]) / 2;

		for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
		{
			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[i]];
			int startvindex1 = pair1.first;
			int endvindex1 = pair1.second;
			CVertex3d* pStartVertex1 = pMesh->GetVertex(startvindex1);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(endvindex1);
			pStartVertex1->x(x);
			pEndVertex1->x(x);
			pPolyOmino->m_VerticalSegments[index].erase(std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]));
		}
		if (pPolyOmino->m_VerticalSegments[index].size() == 0)
		{
			pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + index);
		}
		pPolyOmino->m_VerticalSegments.insert(pPolyOmino->m_VerticalSegments.begin() + index - 1, m_SelectedSharpEdge);
	}
	else
	{
		index = -1;
		for (int j = 0; j < pMesh->yinterval.size(); j++)
		{
			if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
			{
				index = j;
				break;
			}
		}
		if (index < 0)
			ASSERT(0);

		y = (pMesh->yinterval[index] + pMesh->yinterval[index + 1]) / 2;

		for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
		{
			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[i]];
			int startvindex1 = pair1.first;
			int endvindex1 = pair1.second;
			CVertex3d* pStartVertex1 = pMesh->GetVertex(startvindex1);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(endvindex1);
			pStartVertex1->y(y);
			pEndVertex1->y(y);
			pPolyOmino->m_HorizontalSegments[index].erase(std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]));
		}
		if (pPolyOmino->m_HorizontalSegments[index].size() == 0)
		{
			pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + index);
		}
		pPolyOmino->m_HorizontalSegments.insert(pPolyOmino->m_HorizontalSegments.begin() + index, m_SelectedSharpEdge);
	}

	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);

	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonDeleteselectedsharpedges3()
{
	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;
	//Down/Right
	std::vector<int> m_SelectedSharpEdge;
	for (int i = 0; i < pMesh->m_ArraySelectedSharpEdgeIndex.size(); i++)
	{
		if (std::find(m_SelectedSharpEdge.begin(), m_SelectedSharpEdge.end(), pMesh->m_ArraySelectedSharpEdgeIndex[i]) == m_SelectedSharpEdge.end())
		{
			m_SelectedSharpEdge.push_back(pMesh->m_ArraySelectedSharpEdgeIndex[i]);
		}
	}

	std::pair<int, int> pair = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[0]];
	int startvindex = pair.first;
	int endvindex = pair.second;
	CVertex3d* pStartVertex = pMesh->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh->GetVertex(endvindex);

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	int index;
	double x, y;
	if (fabs(x1 - x2) < 1e-8)
	{
		//Vertical
		index = -1;
		for (int j = 0; j < pMesh->xinterval.size(); j++)
		{
			if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
			{
				index = j;
				break;
			}
		}
		if (index < 0)
			ASSERT(0);

		x = (pMesh->xinterval[index] + pMesh->xinterval[index + 1]) / 2;

		for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
		{
			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[i]];
			int startvindex1 = pair1.first;
			int endvindex1 = pair1.second;
			CVertex3d* pStartVertex1 = pMesh->GetVertex(startvindex1);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(endvindex1);
			pStartVertex1->x(x);
			pEndVertex1->x(x);
			pPolyOmino->m_VerticalSegments[index].erase(std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]));
		}
		if (pPolyOmino->m_VerticalSegments[index].size() == 0)
		{
			pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + index);
		}
		pPolyOmino->m_VerticalSegments.insert(pPolyOmino->m_VerticalSegments.begin() + index + 1, m_SelectedSharpEdge);
	}
	else
	{
		index = -1;
		for (int j = 0; j < pMesh->yinterval.size(); j++)
		{
			if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
			{
				index = j;
				break;
			}
		}
		if (index < 0)
			ASSERT(0);

		y = (pMesh->yinterval[index] + pMesh->yinterval[index - 1]) / 2;

		for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
		{
			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[i]];
			int startvindex1 = pair1.first;
			int endvindex1 = pair1.second;
			CVertex3d* pStartVertex1 = pMesh->GetVertex(startvindex1);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(endvindex1);
			pStartVertex1->y(y);
			pEndVertex1->y(y);
			pPolyOmino->m_HorizontalSegments[index].erase(std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]));
		}
		if (pPolyOmino->m_HorizontalSegments[index].size() == 0)
		{
			pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + index);
		}
		pPolyOmino->m_HorizontalSegments.insert(pPolyOmino->m_HorizontalSegments.begin() + index, m_SelectedSharpEdge);
	}

	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);

	pDoc->UpdateAllViews(NULL);

}


void Action_PolyOmino2::OnBnClickedButtonDeleteselectedsharpedges4()
{
	// TODO: Add your control notification handler code here
	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;
	//
	std::vector<int> m_SelectedSharpEdge;
	for (int i = 0; i < pMesh->m_ArraySelectedSharpEdgeIndex.size(); i++)
	{
		if (std::find(m_SelectedSharpEdge.begin(), m_SelectedSharpEdge.end(), pMesh->m_ArraySelectedSharpEdgeIndex[i]) == m_SelectedSharpEdge.end())
		{
			m_SelectedSharpEdge.push_back(pMesh->m_ArraySelectedSharpEdgeIndex[i]);
		}
	}

	std::pair<int, int> pair = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[0]];
	int startvindex = pair.first;
	int endvindex = pair.second;
	CVertex3d* pStartVertex = pMesh->GetVertex(startvindex);
	CVertex3d* pEndVertex = pMesh->GetVertex(endvindex);

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	int index;
	double x, y;
	if (fabs(x1 - x2) < 1e-8)
	{
		//Vertical
		index = -1;
		for (int j = 0; j < pMesh->xinterval.size(); j++)
		{
			if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
			{
				index = j;
				break;
			}
		}
		if (index < 0)
			ASSERT(0);

		//x = (pMesh->xinterval[index] + pMesh->xinterval[index + 1]) / 2;

		for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
		{
			//std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[i]];
			//int startvindex1 = pair1.first;
			//int endvindex1 = pair1.second;
			//CVertex3d* pStartVertex1 = pMesh->GetVertex(startvindex1);
			//CVertex3d* pEndVertex1 = pMesh->GetVertex(endvindex1);
			//pStartVertex1->x(x);
			//pEndVertex1->x(x);
			pPolyOmino->m_VerticalSegments[index].erase(std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]));
		}
		if (pPolyOmino->m_VerticalSegments[index].size() == 0)
		{
			pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + index);
		}
		pPolyOmino->m_VerticalSegments.insert(pPolyOmino->m_VerticalSegments.begin() + index + 1, m_SelectedSharpEdge);
	}
	else
	{
		index = -1;
		for (int j = 0; j < pMesh->yinterval.size(); j++)
		{
			if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
			{
				index = j;
				break;
			}
		}
		if (index < 0)
			ASSERT(0);

		//y = (pMesh->yinterval[index] + pMesh->yinterval[index - 1]) / 2;

		for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
		{
			//std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[i]];
			//int startvindex1 = pair1.first;
			//int endvindex1 = pair1.second;
			//CVertex3d* pStartVertex1 = pMesh->GetVertex(startvindex1);
			//CVertex3d* pEndVertex1 = pMesh->GetVertex(endvindex1);
			//pStartVertex1->y(y);
			//pEndVertex1->y(y);
			pPolyOmino->m_HorizontalSegments[index].erase(std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]));
		}
		if (pPolyOmino->m_HorizontalSegments[index].size() == 0)
		{
			pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + index);
		}
		pPolyOmino->m_HorizontalSegments.insert(pPolyOmino->m_HorizontalSegments.begin() + index, m_SelectedSharpEdge);
	}

	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);

	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonPolyominoGeneratenewpoint()
{
	// TODO: Add your control notification handler code here
	if (pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex.size() != 2)
		return;
	if (pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex.size() < 1)
		return;
	int vindex1 = pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex[0];
	int vindex2 = pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex[1];
	int eindex = pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex[0];

	UpdateData(TRUE);

	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;

	CVertex3d* pStartVertex = pMesh->GetVertex(vindex1);
	CVertex3d* pEndVertex = pMesh->GetVertex(vindex2);

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	int xindex1, yindex1, xindex2, yindex2;

	xindex1 = -1;
	yindex1 = -1;
	xindex2 = -1;
	yindex2 = -1;
	for (int j = 0; j < pMesh->xinterval.size(); j++)
	{
		if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
		{
			xindex1 = j;
			break;
		}
	}
	for (int j = 0; j < pMesh->yinterval.size(); j++)
	{
		if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
		{
			yindex1 = j;
			break;
		}
	}

	for (int j = 0; j < pMesh->xinterval.size(); j++)
	{
		if (fabs(x2 - pMesh->xinterval[j]) < 1e-8)
		{
			xindex2 = j;
			break;
		}
	}
	for (int j = 0; j < pMesh->yinterval.size(); j++)
	{
		if (fabs(y2 - pMesh->yinterval[j]) < 1e-8)
		{
			yindex2 = j;
			break;
		}
	}
	double x, y;
	CVertex3d* pNewVertex1, *pNewVertex2;
	pNewVertex1 = new CVertex3d();
	//pNewVertex2 = new CVertex3d();
	int startvindex, endvindex;
	if (fabs(x1 - x2) < 1e-8)
	{
		//Vertical
		int sign = 1;
		if (yindex2 < yindex1)
			sign = -1;
		y = pMesh->yinterval[yindex1 + m_Subdivision_Index * sign];
		x = x1;

		pNewVertex1->Set(x, y, 0);
		//pNewVertex2->Set(x, y, 0);
		pMesh->AddVertex(pNewVertex1);
		//pMesh->AddVertex(pNewVertex2);
		pMesh->m_SetVertexFlagAsItsIndex = 0;
		pMesh->SetVertexFlagAsItsIndex();

		startvindex = pStartVertex->m_Flag;
		endvindex = pEndVertex->m_Flag;
		std::pair<int, int> newedgepair1, newedgepair2;
		newedgepair1.first = startvindex;
		newedgepair1.second = pNewVertex1->m_Flag;
		//newedgepair2.first = pNewVertex1->m_Flag;
		//newedgepair2.second = pNewVertex2->m_Flag;
		newedgepair2.first = pNewVertex1->m_Flag;
		newedgepair2.second = endvindex;

		pMesh->m_ArraySharpEdges[eindex] = newedgepair1;

		//pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + eindex);
		int newedgeindex1 = eindex;
		int newedgeindex2 = pMesh->m_ArraySharpEdges.size();
		//int newedgeindex3 = pMesh->m_ArraySharpEdges.size() + 1;
		//pMesh->m_ArraySharpEdges.push_back(newedgepair1);
		pMesh->m_ArraySharpEdges.push_back(newedgepair2);
		//pMesh->m_ArraySharpEdges.push_back(newedgepair3);

		//Update the vertical array	
		//pPolyOmino->m_VerticalSegments[xindex1].erase(std::find(pPolyOmino->m_VerticalSegments[xindex2].begin(), pPolyOmino->m_VerticalSegments[xindex2].end(), eindex));
		//pPolyOmino->m_VerticalSegments[xindex1].push_back(newedgeindex1);
		pPolyOmino->m_VerticalSegments[xindex1].push_back(newedgeindex2);
		//pPolyOmino->m_HorizontalSegments[yindex1 + m_Subdivision_Index * sign].push_back(newedgeindex2);
	}
	else
	{
		//Horizontal
		int sign = 1;
		if (xindex2 < xindex1)
			sign = -1;
		x = pMesh->xinterval[xindex1 + m_Subdivision_Index * sign];
		y = y1;

		pNewVertex1->Set(x, y, 0);
		//pNewVertex2->Set(x, y, 0);
		pMesh->AddVertex(pNewVertex1);
		//pMesh->AddVertex(pNewVertex2);
		pMesh->m_SetVertexFlagAsItsIndex = 0;
		pMesh->SetVertexFlagAsItsIndex();

		startvindex = pStartVertex->m_Flag;
		endvindex = pEndVertex->m_Flag;
		std::pair<int, int> newedgepair1, newedgepair2;
		newedgepair1.first = startvindex;
		newedgepair1.second = pNewVertex1->m_Flag;
		//newedgepair2.first = pNewVertex1->m_Flag;
		//newedgepair2.second = pNewVertex2->m_Flag;
		newedgepair2.first = pNewVertex1->m_Flag;
		newedgepair2.second = endvindex;

		pMesh->m_ArraySharpEdges[eindex] = newedgepair1;

		int newedgeindex1 = eindex;
		int newedgeindex2 = pMesh->m_ArraySharpEdges.size();
		//int newedgeindex3 = pMesh->m_ArraySharpEdges.size() + 1;
		//pMesh->m_ArraySharpEdges.push_back(newedgepair1);
		pMesh->m_ArraySharpEdges.push_back(newedgepair2);
		//pMesh->m_ArraySharpEdges.push_back(newedgepair3);

		//Update the Horizontal array	
		//pPolyOmino->m_HorizontalSegments[yindex1].erase(std::find(pPolyOmino->m_HorizontalSegments[yindex1].begin(), pPolyOmino->m_HorizontalSegments[yindex1].end(), eindex));
		//pPolyOmino->m_HorizontalSegments[yindex1].push_back(newedgeindex1);
		pPolyOmino->m_HorizontalSegments[yindex1].push_back(newedgeindex2);
		//pPolyOmino->m_VerticalSegments[xindex1 + m_Subdivision_Index * sign].push_back(newedgeindex2);
	}
	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);

	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonPolyominoGeneratetwopointline()
{
	// TODO: Add your control notification handler code here
	if (pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex.size() != 2)
		return;
	int vindex1 = pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex[0];
	int vindex2 = pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex[1];
	
	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;

	CVertex3d* pStartVertex = pMesh->GetVertex(vindex1);
	CVertex3d* pEndVertex = pMesh->GetVertex(vindex2);

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	int xindex1, yindex1, xindex2, yindex2;

	xindex1 = -1;
	yindex1 = -1;
	xindex2 = -1;
	yindex2 = -1;
	for (int j = 0; j < pMesh->xinterval.size(); j++)
	{
		if (fabs(x1 - pMesh->xinterval[j]) < 5e-2)
		{
			xindex1 = j;
			break;
		}
	}
	for (int j = 0; j < pMesh->yinterval.size(); j++)
	{
		if (fabs(y1 - pMesh->yinterval[j]) < 5e-2)
		{
			yindex1 = j;
			break;
		}
	}

	for (int j = 0; j < pMesh->xinterval.size(); j++)
	{
		if (fabs(x2 - pMesh->xinterval[j]) < 5e-2)
		{
			xindex2 = j;
			break;
		}
	}
	for (int j = 0; j < pMesh->yinterval.size(); j++)
	{
		if (fabs(y2 - pMesh->yinterval[j]) < 5e-2)
		{
			yindex2 = j;
			break;
		}
	}
	double x, y;
	CVertex3d* pNewVertex1, *pNewVertex2;
	pNewVertex1 = new CVertex3d();
	//pNewVertex2 = new CVertex3d();
	if (fabs(x1 - x2) < 5e-2)
	{
		//Vertical
		int sign = 1;
		if (yindex2 < yindex1)
			sign = -1;
		
		std::pair<int, int> newedgepair1;
		newedgepair1.first = vindex1;
		newedgepair1.second = vindex2;

		//pMesh->m_ArraySharpEdges[eindex] = newedgepair1;

		//pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + eindex);
		//int newedgeindex1 = eindex;
		int newedgeindex1 = pMesh->m_ArraySharpEdges.size();
		//int newedgeindex3 = pMesh->m_ArraySharpEdges.size() + 1;
		//pMesh->m_ArraySharpEdges.push_back(newedgepair1);
		pMesh->m_ArraySharpEdges.push_back(newedgepair1);
		//pMesh->m_ArraySharpEdges.push_back(newedgepair3);

		//Update the vertical array	
		//pPolyOmino->m_VerticalSegments[xindex1].erase(std::find(pPolyOmino->m_VerticalSegments[xindex2].begin(), pPolyOmino->m_VerticalSegments[xindex2].end(), eindex));
		//pPolyOmino->m_VerticalSegments[xindex1].push_back(newedgeindex1);
		pPolyOmino->m_VerticalSegments[xindex1].push_back(newedgeindex1);
		//pPolyOmino->m_HorizontalSegments[yindex1 + m_Subdivision_Index * sign].push_back(newedgeindex2);
	}
	else
	{
		//Horizontal
		int sign = 1;
		if (xindex2 < xindex1)
			sign = -1;

		std::pair<int, int> newedgepair1;
		newedgepair1.first = vindex1;
		newedgepair1.second = vindex2;
		//newedgepair2.first = pNewVertex1->m_Flag;
		//newedgepair2.second = pNewVertex2->m_Flag;
		//newedgepair2.first = pNewVertex1->m_Flag;
		//newedgepair2.second = endvindex;

		//pMesh->m_ArraySharpEdges[eindex] = newedgepair1;

		//int newedgeindex1 = eindex;
		int newedgeindex1 = pMesh->m_ArraySharpEdges.size();
		//int newedgeindex3 = pMesh->m_ArraySharpEdges.size() + 1;
		//pMesh->m_ArraySharpEdges.push_back(newedgepair1);
		pMesh->m_ArraySharpEdges.push_back(newedgepair1);
		//pMesh->m_ArraySharpEdges.push_back(newedgepair3);

		//Update the Horizontal array	
		//pPolyOmino->m_HorizontalSegments[yindex1].erase(std::find(pPolyOmino->m_HorizontalSegments[yindex1].begin(), pPolyOmino->m_HorizontalSegments[yindex1].end(), eindex));
		//pPolyOmino->m_HorizontalSegments[yindex1].push_back(newedgeindex1);
		pPolyOmino->m_HorizontalSegments[yindex1].push_back(newedgeindex1);
		//pPolyOmino->m_VerticalSegments[xindex1 + m_Subdivision_Index * sign].push_back(newedgeindex2);
	}
	//Update Uniform Grid
	/*OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);*/

	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonPolyominoGeneratenewpoint2line()
{
	std::vector<int> m_SelectedSharpEdge;
	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex.size(); i++)
	{
		if (std::find(m_SelectedSharpEdge.begin(), m_SelectedSharpEdge.end(), pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex[i]) == m_SelectedSharpEdge.end())
		{
			m_SelectedSharpEdge.push_back(pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex[i]);
		}
	}

	if (m_SelectedSharpEdge.size() != 2)
		return;
	int eindex1 = m_SelectedSharpEdge[0];
	int eindex2 = m_SelectedSharpEdge[1];

	int verticalindex = -1;
	int horizontalindex = -1;
	for (int i = 0; i < pPolyOmino->m_VerticalSegments.size(); i++)
	{
		if (std::find(pPolyOmino->m_VerticalSegments[i].begin(), pPolyOmino->m_VerticalSegments[i].end(), eindex1) != pPolyOmino->m_VerticalSegments[i].end())
		{
			verticalindex = i;
			break;
		}
		if (std::find(pPolyOmino->m_VerticalSegments[i].begin(), pPolyOmino->m_VerticalSegments[i].end(), eindex2) != pPolyOmino->m_VerticalSegments[i].end())
		{
			verticalindex = i;
			break;
		}
	}
	for (int i = 0; i < pPolyOmino->m_HorizontalSegments.size(); i++)
	{
		if (std::find(pPolyOmino->m_HorizontalSegments[i].begin(), pPolyOmino->m_HorizontalSegments[i].end(), eindex1) != pPolyOmino->m_HorizontalSegments[i].end())
		{
			horizontalindex = i;
			break;
		}
		if (std::find(pPolyOmino->m_HorizontalSegments[i].begin(), pPolyOmino->m_HorizontalSegments[i].end(), eindex2) != pPolyOmino->m_HorizontalSegments[i].end())
		{
			horizontalindex = i;
			break;
		}
	}
	double x = pPolyOmino->pUniformGridMesh->xinterval[verticalindex];
	double y = pPolyOmino->pUniformGridMesh->yinterval[horizontalindex];

	CVertex3d* pNewVertex1 = new CVertex3d();
	pNewVertex1->Set(x, y, 0);
	//pNewVertex2->Set(x, y, 0);
	pPolyOmino->pUniformGridMesh->AddVertex(pNewVertex1);
	//pMesh->AddVertex(pNewVertex2);
	pPolyOmino->pUniformGridMesh->m_SetVertexFlagAsItsIndex = 0;
	pPolyOmino->pUniformGridMesh->SetVertexFlagAsItsIndex();

	//Update Uniform Grid
	/*OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);*/

	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonLoadFinaluniformgrid()
{
	CString FileName("D:\\papers\\code\\data\\Geo\\ditu\\dense\\6899.m");

	//int iterationnum;
	pPolyOmino->pMesh = NULL;
	pPolyOmino->LoadData(FileName, iterationnum);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showSharpEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = FALSE;
	pPolyOmino->pMesh->m_showFace = FALSE;
}

void Action_PolyOmino2::SetStatesArray_worldmap()
{
	pPolyOmino->pUniformGridMesh->m_ArrayStates.resize(pPolyOmino->pUniformGridMesh->m_ArrayFaceAreaArray.size());

	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArrayStates.size(); i++)
	{
		pPolyOmino->pUniformGridMesh->m_ArrayStates[i].push_back(i);
	}
	//for (int i = 0; i < pPolyOmino->pUniformGridMesh->)
	//pPolyOmino->pUniformGridMesh->m_ArrayStates[29].push_back(31);
	//pPolyOmino->pUniformGridMesh->m_ArrayStates[30].push_back(30);
	//for (int i = 32; i < pPolyOmino->pUniformGridMesh->m_ArrayFaceAreaArray.size(); i++)
	//	pPolyOmino->pUniformGridMesh->m_ArrayStates[i - 1].push_back(i);
	//29 and 31 For michigan
	//pUniformGridMesh->m_ArrayUniformCells[29].insert(pUniformGridMesh->m_ArrayUniformCells[29].begin(),
	//	pUniformGridMesh->m_ArrayUniformCells[31].begin(),pUniformGridMesh->m_ArrayUniformCells[31].end());
	//pUniformGridMesh->m_ArrayUniformCells.erase(pUniformGridMesh->m_ArrayUniformCells.begin() + 31);
	//pUniformGridMesh->m_ArrayDistrictDensity.erase(pUniformGridMesh->m_ArrayDistrictDensity.begin() + 31);
	//Draw 
	//PlotFaceCellsbySingleColorFromCenter(maximaldensityeachcell);
}
int Action_PolyOmino2::IsFaceEqualPoints(int i, int j)
{
	std::vector<int> facepoints1 = pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray[i];
	std::vector<int> facepoints2 = pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray[j];

	if (facepoints1.size() != facepoints2.size())
		return 0;

	struct sort_pred {
		bool operator()(const int &left, const int &right)
		{
			return left > right;
		}
	};

	std::sort(facepoints1.begin(), facepoints1.end(), sort_pred());
	std::sort(facepoints2.begin(), facepoints2.end(), sort_pred());

	for (int i = 0; i < facepoints1.size(); i++)
	{
		if (facepoints1[i] != facepoints2[i])
			return 0;
	}
	return 1;
}
void Action_PolyOmino2::RemoveDulicateFaces()
{
	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.size(); i++)
	{
		for (int j = i + 1; j < pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.size(); j++)
		{
			if (IsFaceEqualPoints(i, j))
			{
				pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.begin() + j);
				pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + j);
				break;
			}
		}
	}
}

void Action_PolyOmino2::OnBnClickedButtonVisualizationpopulation()
{
	// TODO: Add your control notification handler code here
	//int iterationnum;
	//pPolyOmino->LoadData1("D:\\papers\\code\\data\\Geo\\ditu\\dense\\6899.m", iterationnum);
	//pPolyOmino->LoadData1("D:\\papers\\code\\data\\Geo\\ditu\\dense\\7838.m", iterationnum);
	//pPolyOmino->LoadData1("D:\\papers\\code\\data\\Geo\\ditu\\dense\\7804.m", iterationnum);
	//pPolyOmino->LoadData1("D:\\papers\\code\\data\\Geo\\ditu\\dense\\final\\8059.m", iterationnum);
	//pPolyOmino->LoadData1("D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\3414.m", iterationnum);
	//pPolyOmino->LoadData1("D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\3350.m", iterationnum);
	//pPolyOmino->LoadData1("D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\3200.m", iterationnum);
	//pPolyOmino->LoadData1("D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\3414.m", iterationnum);
	//move the horizontal and vertical lines such that the intervals are the 
	//integer times of the minimal interval distance.
	//return;
	pPolyOmino->AdjustHorizontalandVerticalGroupofLinePositions();

	//pPolyOmino->DetectInvalidSharpEdges();
	//pPolyOmino->AdjustHorizontalandVerticalGroupofLinePositions_3base();
	//Each four elements form one group

	//return;
	std::vector<int> m_ArrayFacePoints, m_ArrayFaceEdges;

	//Trace Faces
	pPolyOmino->TraceFaces(m_ArrayFacePoints, m_ArrayFaceEdges);

	//Remove the outer face
	//pPolyOmino->RemoveOuterFace(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray, pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray);

	//pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.begin() + 2);
	//pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + 2);

	pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.begin() + 2);
	pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + 2);

	//Remove Duplicate Faces
	RemoveDulicateFaces();

	//
	//RemoveDeletedFaces();

	//return;
	//
	pPolyOmino->ComputeFacesFourElementArray(m_ArrayFacePoints, m_ArrayFaceEdges);

	//
	pPolyOmino->AssignUniformCellstoFaces();

	//pUniformGridMesh->m_ArrayTargetFaceArea.resize(pUniformGridMesh->m_ArrayFaceAreaArray.size());
	p_FormView3->OutPutMeshPolyOminoEdges_PovRay(pPolyOmino->pUniformGridMesh, "D:\\polyominomesh.inc");

	pPolyOmino->Loadworldpopulation();
	//return;
	//pPolyOmino->LoadTargetFaceArea("D:\\papers\\code\\data\\Geo\\dense\\1788_usapopulation.txt");

	//Get the maximal density for each grid cell
	double maximaldensityeachcell = -1;
	double minimaldensityeachcell = 1e10;
	double maximalstatepopulation = -1;
	double minimalstatepopulation = 1e10;

	//SetStatesArray_worldmap();
	//
	//pPolyOmino->SetStatesArray();

	//
	//pPolyOmino->LoadStateName("D:\\papers\\code\\data\\Geo\\dense\\1788_districtname.txt");
	//LoadStateName("D:\\papers\\code\\data\\Geo\\dense\\1788_districtname_abbre.txt");
	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usapopulation_2010_2014.txt");
	//pPolyOmino->LoadTargetFaceAreaArray(1);
	//LoadTargetFaceAreaArray_facebookpenetration("D:\\papers\\code\\data\\Geo\\dense\\1788_facebookpenetration.txt");

	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usa_completed_highschool.txt");

	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usa_familyincome.txt");

	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usa_median_household_income.txt");

	//
	//pPolyOmino->ComputetheDifference(0, 24);

	//
	//GetFacesDifferenceMaximalandMinimalDensityPerCell_Array(maximaldensityeachcell,minimaldensityeachcell,maximalstatepopulation,minimalstatepopulation);

	//
	pPolyOmino->GetStateMaximalandMinimalDensityPerCell_Array(maximaldensityeachcell, minimaldensityeachcell, maximalstatepopulation, minimalstatepopulation);

	//PlotFaceCellDifference
	//PlotFaceCellByDifference_Array(3,maximaldensityeachcell,minimaldensityeachcell,maximalstatepopulation,minimalstatepopulation);

	//
	//pPolyOmino->GetFacesMaximalandMinimalDensityPerCell_Array(maximaldensityeachcell, minimaldensityeachcell, maximalstatepopulation, minimalstatepopulation);

	//PlotFaceCellsBySeveralColorsFromTopLeft(3,maximaldensityeachcell,minimaldensityeachcell);
	//2014 24 
	//2000 10
	pPolyOmino->PlotFaceCellsBySeveralColorsFromTopLeft_Array(0, 3, maximaldensityeachcell, minimaldensityeachcell, maximalstatepopulation, minimalstatepopulation);

	//colorbar
	//PlotFaceCellsbyContinuousColor(maximaldensityeachcell);
	//pPolyOmino->LoadStateName("D:\\papers\\code\\data\\Geo\\dense\\1788_districtname_abbre.txt");*/
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonMiddlecenter()
{
	if (pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex.size() < 1)
		return;

	int eindex = pPolyOmino->pUniformGridMesh->m_ArraySelectedSharpEdgeIndex[0];

	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;

	std::pair<int, int> pair = pMesh->m_ArraySharpEdges[eindex];
	int vindex1 = pair.first;
	int vindex2 = pair.second;

	CVertex3d* pStartVertex = pMesh->GetVertex(vindex1);
	CVertex3d* pEndVertex = pMesh->GetVertex(vindex2);

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	int xindex1, yindex1, xindex2, yindex2;

	xindex1 = -1;
	yindex1 = -1;
	xindex2 = -1;
	yindex2 = -1;
	for (int j = 0; j < pMesh->xinterval.size(); j++)
	{
		if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
		{
			xindex1 = j;
			break;
		}
	}
	for (int j = 0; j < pMesh->yinterval.size(); j++)
	{
		if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
		{
			yindex1 = j;
			break;
		}
	}

	for (int j = 0; j < pMesh->xinterval.size(); j++)
	{
		if (fabs(x2 - pMesh->xinterval[j]) < 1e-8)
		{
			xindex2 = j;
			break;
		}
	}
	for (int j = 0; j < pMesh->yinterval.size(); j++)
	{
		if (fabs(y2 - pMesh->yinterval[j]) < 1e-8)
		{
			yindex2 = j;
			break;
		}
	}
	double x, y;
	CVertex3d* pNewVertex1, *pNewVertex2;
	pNewVertex1 = new CVertex3d();
	//pNewVertex2 = new CVertex3d();
	int startvindex, endvindex;
	if (fabs(x1 - x2) < 1e-8)
	{
		//Vertical
		x = (pMesh->xinterval[xindex2 - 1] + pMesh->xinterval[xindex2 + 1]) / 2;
		
		for (int i = 0; i < pPolyOmino->m_VerticalSegments[xindex2].size(); i++)
		{
			int eindex1 = pPolyOmino->m_VerticalSegments[xindex2][i];

			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[eindex1];
			int vindex1_ = pair1.first;
			int vindex2_ = pair1.second;

			CVertex3d* pStartVertex1 = pMesh->GetVertex(vindex1_);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(vindex2_);

			pStartVertex1->x(x);
			pEndVertex1->x(x);
		}
	}
	else
	{
		//Horizontal
		y = (pMesh->yinterval[yindex2 - 1] + pMesh->yinterval[yindex2 + 1]) / 2;

		for (int i = 0; i < pPolyOmino->m_HorizontalSegments[yindex2].size(); i++)
		{
			int eindex1 = pPolyOmino->m_HorizontalSegments[yindex2][i];

			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[eindex1];
			int vindex1_ = pair1.first;
			int vindex2_ = pair1.second;

			CVertex3d* pStartVertex1 = pMesh->GetVertex(vindex1_);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(vindex2_);

			pStartVertex1->y(y);
			pEndVertex1->y(y);
		}
	}

	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);

	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonAverageHorizontal()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;
	double minvalue = pMesh->yinterval[m_MinIndex];
	double maxvalue = pMesh->yinterval[m_MaxIndex];
	double val;
	for (int i = m_MinIndex + 1; i < m_MaxIndex; i++)
	{
		val = ((i - m_MinIndex) * maxvalue + (m_MaxIndex - i) * minvalue) / (m_MaxIndex - m_MinIndex);
		for (int j = 0; j < pPolyOmino->m_HorizontalSegments[i].size(); j++)
		{
			int edgeindex = pPolyOmino->m_HorizontalSegments[i][j];

			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[edgeindex];
			int vindex1_ = pair1.first;
			int vindex2_ = pair1.second;

			CVertex3d* pStartVertex1 = pMesh->GetVertex(vindex1_);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(vindex2_);

			pStartVertex1->y(val);
			pEndVertex1->y(val);
		}
	}

	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);

	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonAverageVertical()
{
	UpdateData(TRUE);
	CMesh3d* pMesh = pPolyOmino->pUniformGridMesh;
	double minvalue = pMesh->xinterval[m_MinIndex];
	double maxvalue = pMesh->xinterval[m_MaxIndex];
	double val;
	for (int i = m_MinIndex + 1; i < m_MaxIndex; i++)
	{
		val = ((i - m_MinIndex) * maxvalue + (m_MaxIndex - i) * minvalue) / (m_MaxIndex - m_MinIndex);
		for (int j = 0; j < pPolyOmino->m_VerticalSegments[i].size(); j++)
		{
			int edgeindex = pPolyOmino->m_VerticalSegments[i][j];

			std::pair<int, int> pair1 = pMesh->m_ArraySharpEdges[edgeindex];
			int vindex1_ = pair1.first;
			int vindex2_ = pair1.second;

			CVertex3d* pStartVertex1 = pMesh->GetVertex(vindex1_);
			CVertex3d* pEndVertex1 = pMesh->GetVertex(vindex2_);

			pStartVertex1->x(val);
			pEndVertex1->x(val);
		}
	}

	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);

	pDoc->UpdateAllViews(NULL);
}


BOOL Action_PolyOmino2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_SpinRows.SetBuddy(GetDlgItem(IDC_EDIT_NUM_POLYOMINO_MININDEX));
	m_spinColumns.SetBuddy(GetDlgItem(IDC_EDIT_NUM_POLYOMINO_MAXINDEX));
	m_SpinPuzzleSize.SetBuddy(GetDlgItem(IDC_EDIT_NUM_POLYOMINO_PUZZLESIZE));
	m_SpinXNum.SetBuddy(GetDlgItem(IDC_EDIT_NUM_POLYOMINO_XNUMBER));
	m_SpinYNum.SetBuddy(GetDlgItem(IDC_EDIT_NUM_POLYOMINO_YNUMBER));
	m_SpinYNum.SetRange(0, 10000);
	m_SpinXNum.SetRange(0, 10000);
	m_SpinRows.SetRange(0, 10000);
	m_spinColumns.SetRange(0, 10000);
	m_SpinPuzzleSize.SetRange(0, 10000);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Action_PolyOmino2::OnThemechangedSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	if (pPolyOmino->pUniformGridMesh)
	{
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleXPos = m_MinIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleYPos = m_MaxIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleSize = m_PuzzleSize;
	}

	*pResult = 0;
}


void Action_PolyOmino2::OnThemechangedSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	if (pPolyOmino->pUniformGridMesh)
	{
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleXPos = m_MinIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleYPos = m_MaxIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleSize = m_PuzzleSize;
	}
	*pResult = 0;
}


void Action_PolyOmino2::OnThemechangedSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	if (pPolyOmino->pUniformGridMesh)
	{
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleXPos = m_MinIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleYPos = m_MaxIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleSize = m_PuzzleSize;
	}
	*pResult = 0;
}


void Action_PolyOmino2::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	if (pPolyOmino->pUniformGridMesh)
	{
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleXPos = m_MinIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleYPos = m_MaxIndex;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleSize = m_PuzzleSize;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleXNum = m_PuzzleXNum;
		pPolyOmino->pUniformGridMesh->m_CurrentPuzzleYNum = m_PuzzleYNum;
		//OnBnClickedButtonSetfacepointsSelected();
		pDoc->UpdateAllViews(NULL);
	}
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


void Action_PolyOmino2::OnBnClickedButtonAddpuzzle()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_PuzzleXNum; i++)
		for (int j = 0; j < m_PuzzleYNum; j++)
		{
			std::vector<int> puzzledata;
			puzzledata.push_back(m_MinIndex + i * m_PuzzleSize);
			puzzledata.push_back(m_MaxIndex + j * m_PuzzleSize);
			puzzledata.push_back(m_PuzzleSize);
			puzzledata.push_back(m_Subdivision_Index);
			pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.push_back(puzzledata);
		}

}


void Action_PolyOmino2::OnBnClickedButtonLoadpuzzle()
{
	std::ifstream file("d:\\puzzle.txt");
	int size;
	file >> size;
	pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.clear();
	//pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.resize(size);
	std::vector<int> puzzledata;
	int val;
	for (int i = 0; i < size; i++)
	{
		puzzledata.clear();
		file >> val;
		puzzledata.push_back(val);
		file >> val;
		puzzledata.push_back(val);
		file >> val;
		puzzledata.push_back(val);
		file >> val;
		puzzledata.push_back(val);
		pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.push_back(puzzledata);
	}
	file.close();
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonSavepuzzle()
{
	std::ofstream file("d:\\puzzle.txt");

	file << pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.size() << "\n";
	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.size(); i++)
	{
		file << pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[i][0] << " ";
		file << pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[i][1] << " ";
		file << pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[i][2] << " ";
		file << pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[i][3] << "\n";
	}
	file.close();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino2::UpdateSelectedSharpEdgePosition_Base(CMesh3d* pMesh, int deltax, int deltay)
{
	std::vector<int> m_SelectedSharpEdge;
	for (int i = 0; i < pMesh->m_ArraySelectedSharpEdgeIndex.size(); i++)
	{
		if (std::find(m_SelectedSharpEdge.begin(), m_SelectedSharpEdge.end(), pMesh->m_ArraySelectedSharpEdgeIndex[i]) == m_SelectedSharpEdge.end())
		{
			m_SelectedSharpEdge.push_back(pMesh->m_ArraySelectedSharpEdgeIndex[i]);
		}
	}
	int verticalindex, horizontalindex;
	for (int i = 0; i < m_SelectedSharpEdge.size(); i++)
	{
		std::pair<int, int> pair = pMesh->m_ArraySharpEdges[m_SelectedSharpEdge[i]];
		int startvindex = pair.first;
		int endvindex = pair.second;
		CVertex3d* pStartVertex = pMesh->GetVertex(startvindex);
		CVertex3d* pEndVertex = pMesh->GetVertex(endvindex);

		double x1 = pStartVertex->x();
		double y1 = pStartVertex->y();
		double x2 = pEndVertex->x();
		double y2 = pEndVertex->y();

		verticalindex = -1;
		horizontalindex = -1;
		for (int j = 0; j < pPolyOmino->m_VerticalSegments.size(); j++)
		{
			if (std::find(pPolyOmino->m_VerticalSegments[j].begin(), pPolyOmino->m_VerticalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_VerticalSegments[j].end())
			{
				verticalindex = j;
				break;
			}
		}

		for (int j = 0; j < pPolyOmino->m_HorizontalSegments.size(); j++)
		{
			if (std::find(pPolyOmino->m_HorizontalSegments[j].begin(), pPolyOmino->m_HorizontalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_HorizontalSegments[j].end())
			{
				horizontalindex = j;
				break;
			}
		}

		int index;
		if (verticalindex > -1)
		{
			//Vertical
			index = verticalindex;
			/*for (int j = 0; j < pPolyOmino->m_VerticalSegments.size(); j++)
			{
			if (std::find(pPolyOmino->m_VerticalSegments[j].begin(), pPolyOmino->m_VerticalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_VerticalSegments[j].end())
			{
			index = j;
			break;
			}
			}*/
			/*for (int j = 0; j < pMesh->xinterval.size(); j++)
			{
			if (fabs(x1 - pMesh->xinterval[j]) < 1e-8)
			{
			index = j;
			break;
			}
			}*/
			if (index < 0)
				ASSERT(0);

			if ((index + deltax > -1) && (index + deltax < pMesh->xinterval.size()) && (fabs(pMesh->xinterval[index + deltax] - pMesh->xinterval[index]) - pMesh->baseinterval) < 5e-2)
			{
				pStartVertex->x(pMesh->xinterval[index + deltax]);
				pEndVertex->x(pMesh->xinterval[index + deltax]);

				//Update the vertical array	
				if (std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_VerticalSegments[index].end())
					pPolyOmino->m_VerticalSegments[index].erase(std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]));
				pPolyOmino->m_VerticalSegments[index + deltax].push_back(m_SelectedSharpEdge[i]);
				if (pPolyOmino->m_VerticalSegments[index].size() == 0)
				{
					pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + index);
				}
			}
			else
			{
				std::vector<int> edgearray;
				edgearray.clear();
				edgearray.push_back(m_SelectedSharpEdge[i]);

				pStartVertex->x(pMesh->xinterval[index] + deltax * pMesh->baseinterval);
				pEndVertex->x(pMesh->xinterval[index] + deltax * pMesh->baseinterval);

				//Update the vertical array	
				if (std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_VerticalSegments[index].end())
					pPolyOmino->m_VerticalSegments[index].erase(std::find(pPolyOmino->m_VerticalSegments[index].begin(), pPolyOmino->m_VerticalSegments[index].end(), m_SelectedSharpEdge[i]));

				if (deltax > 0)
				{
					pPolyOmino->m_VerticalSegments.insert(pPolyOmino->m_VerticalSegments.begin() + index + 1, edgearray);
					if (pPolyOmino->m_VerticalSegments[index].size() == 0)
					{
						pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + index);
					}
				}
				else
				{
					pPolyOmino->m_VerticalSegments.insert(pPolyOmino->m_VerticalSegments.begin() + index, edgearray);
					if (pPolyOmino->m_VerticalSegments[index + 1].size() == 0)
					{
						pPolyOmino->m_VerticalSegments.erase(pPolyOmino->m_VerticalSegments.begin() + index + 1);
					}
				}
			}
			
		}
		else
		{
			index = horizontalindex;
			/*for (int j = 0; j < pPolyOmino->m_HorizontalSegments.size(); j++)
			{
			if (std::find(pPolyOmino->m_HorizontalSegments[j].begin(), pPolyOmino->m_HorizontalSegments[j].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_HorizontalSegments[j].end())
			{
			index = j;
			break;
			}
			}*/
			/*for (int j = 0; j < pMesh->yinterval.size(); j++)
			{
			if (fabs(y1 - pMesh->yinterval[j]) < 1e-8)
			{
			index = j;
			break;
			}
			}*/
			if (index < 0)
				ASSERT(0);

			if (fabs(fabs(pMesh->yinterval[index + deltay] - pMesh->yinterval[index]) - pMesh->baseinterval) < 1e-6)
			{
				pStartVertex->y(pMesh->yinterval[index + deltay]);
				pEndVertex->y(pMesh->yinterval[index + deltay]);

				//Update the vertical array	
				if (std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_HorizontalSegments[index].end())
					pPolyOmino->m_HorizontalSegments[index].erase(std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]));
				pPolyOmino->m_HorizontalSegments[index + deltay].push_back(m_SelectedSharpEdge[i]);
				if (pPolyOmino->m_HorizontalSegments[index].size() == 0)
				{
					pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + index);
				}
			}
			else
			{
				std::vector<int> edgearray;
				edgearray.clear();
				edgearray.push_back(m_SelectedSharpEdge[i]);

				pStartVertex->y(pMesh->yinterval[index] + deltay * pMesh->baseinterval);
				pEndVertex->y(pMesh->yinterval[index] + deltay * pMesh->baseinterval);

				//Update the vertical array	
				if (std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]) != pPolyOmino->m_HorizontalSegments[index].end())
					pPolyOmino->m_HorizontalSegments[index].erase(std::find(pPolyOmino->m_HorizontalSegments[index].begin(), pPolyOmino->m_HorizontalSegments[index].end(), m_SelectedSharpEdge[i]));

				if (deltay > 0)
				{
					pPolyOmino->m_HorizontalSegments.insert(pPolyOmino->m_HorizontalSegments.begin() + index + 1, edgearray);
					if (pPolyOmino->m_HorizontalSegments[index].size() == 0)
					{
						pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + index);
					}
				}
				else
				{
					pPolyOmino->m_HorizontalSegments.insert(pPolyOmino->m_HorizontalSegments.begin() + index, edgearray);
					if (pPolyOmino->m_HorizontalSegments[index + 1].size() == 0)
					{
						pPolyOmino->m_HorizontalSegments.erase(pPolyOmino->m_HorizontalSegments.begin() + index + 1);
					}
				}
			}
		}
		OnBnClickedButtonInitializeuniformgrid();
	}

	pMesh->m_ArraySelectedSharpEdgeIndex.clear();
	//Update Uniform Grid
	OnBnClickedButtonInitializeuniformgrid();
	//
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	/*iterationnum++;
	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\final\\%d.m", iterationnum);
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);
	//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\dense\\final\\%d.txt", iterationnum);
	pPolyOmino->SaveDebugInformation(filename);*/

	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino2::OnBnClickedButtonUpPolyominoBase()
{
	UpdateSelectedSharpEdgePosition_Base(pPolyOmino->pUniformGridMesh, 0, 1);
}


void Action_PolyOmino2::OnBnClickedButtonLeftPolyominoBase()
{
	UpdateSelectedSharpEdgePosition_Base(pPolyOmino->pUniformGridMesh, -1, 0);
}


void Action_PolyOmino2::OnBnClickedButtonDownPolyominoBase()
{
	UpdateSelectedSharpEdgePosition_Base(pPolyOmino->pUniformGridMesh, 0, -1);
}


void Action_PolyOmino2::OnBnClickedButtonRightPolyominoBase()
{
	UpdateSelectedSharpEdgePosition_Base(pPolyOmino->pUniformGridMesh, 1, 0);
}


void Action_PolyOmino2::OnBnClickedButtonSetfacepointsSelected()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex.clear();
	
	if(m_PuzzleYNum < pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.size())
		pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex = pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray[m_PuzzleYNum];

	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonSetfacepointsDelete()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//pPolyOmino->pUniformGridMesh->m_ArraySharpVertexIndex.clear();

	if (m_PuzzleYNum < pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.size())
	{
		pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.begin() + m_PuzzleYNum);
		pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + m_PuzzleYNum);
		m_ArrayDeleteFaces.push_back(m_PuzzleYNum);
		OnBnClickedButtonSetfacepointsSelected();
	}

	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino2::RemoveDeletedFaces()
{
	std::ifstream file("D:\\deletedfaces.txt");
	m_ArrayDeleteFaces.clear();
	int n,val;
	file >> n;
	for (int i = 0; i < n; i++)
	{
		file >> val;
		m_ArrayDeleteFaces.push_back(val);
	}
	file.close();

	for (int i = 0; i < m_ArrayDeleteFaces.size(); i++)
	{
		pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.begin() + m_ArrayDeleteFaces[i]);
		pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + m_ArrayDeleteFaces[i]);
	}
}
void Action_PolyOmino2::OnBnClickedButtonSavedeletedfaces()
{
	std::ofstream file("D:\\deletedfaces.txt");
	file << m_ArrayDeleteFaces.size() << "\n";
	for (int i = 0; i < m_ArrayDeleteFaces.size(); i++)
	{
		file << m_ArrayDeleteFaces[i] << "\n";
	}
	file.close();
}


void Action_PolyOmino2::OnBnClickedButtonSavepuzzlepiece()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//std::vector<int> dataset;
	//dataset.push_back(m_MinIndex);
	//dataset.push_back(m_MaxIndex);
	if (m_PuzzleSize > 8 || m_PuzzleSize < 0)
		return;
	if (pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.size() != 9)
	{
		pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.resize(9);
	}
	pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[m_PuzzleSize].push_back(m_MinIndex);
	pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[m_PuzzleSize].push_back(m_MaxIndex);
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonSavepuzzlepiece2()
{
	std::ofstream file("D:\\puzzles.txt");
	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.size(); i++)
	{
		file << i << " ";
		for (int j = 0; j < pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[i].size(); j++)
		{
			file << pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[i][j] << " ";
		}
		file << " -1 -1 \n";
	}
	file.close();
}


void Action_PolyOmino2::OnBnClickedButtonLoadpuzzlepiece()
{
	std::ifstream file("D:\\puzzles.txt");
	if (pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.size() != 9)
	{
		pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.resize(9);
	}
	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.size(); i++)
	{
		pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[i].clear();
	}
	int index,val1,val2;
	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArrayPuzzles.size(); i++)
	{
		file >> index;
		file >> val1;
		file >> val2;
		while (val1 >= 0)
		{
			pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[i].push_back(val1);
			pPolyOmino->pUniformGridMesh->m_ArrayPuzzles[i].push_back(val2);
			file >> val1;
			file >> val2;
		}
	}
	file.close();
}


void Action_PolyOmino2::OnBnClickedButtonCheckpointwithfiveregions()
{
	// TODO: Add your control notification handler code here
	std::vector<std::vector<int>> m_AarrayVertexFlag;
	m_AarrayVertexFlag.resize(pPolyOmino->pMesh->m_ArrayVertex.GetSize());

	for (int i = 0; i < pPolyOmino->pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int, int> pair = pPolyOmino->pMesh->m_ArraySharpEdges[i];
		if(std::find(m_AarrayVertexFlag[pair.first].begin(), m_AarrayVertexFlag[pair.first].end(), pair.second) == m_AarrayVertexFlag[pair.first].end())
			m_AarrayVertexFlag[pair.first].push_back(pair.second);
		if (std::find(m_AarrayVertexFlag[pair.second].begin(), m_AarrayVertexFlag[pair.second].end(), pair.first) == m_AarrayVertexFlag[pair.second].end())
			m_AarrayVertexFlag[pair.second].push_back(pair.first);
	}
	
	std::vector<int> pointedgearray;
	pointedgearray.resize(10, 0);
	for (int i = 0; i < m_AarrayVertexFlag.size(); i++)
	{
		pointedgearray[m_AarrayVertexFlag[i].size()]++;
		if (m_AarrayVertexFlag[i].size() >= 5)
		{
			pPolyOmino->pMesh->m_ArraySharpVertexIndex.push_back(i);
			//int ij = 0;
		}
	}
}
int Action_PolyOmino2::FindPointLocatedPolygon(CMesh3d* pMesh, std::vector<std::vector<int>>& m_ArrayFacePointsArray, double x, double y)
{
	
	int i, j, c = 0;
	CVertex3d* pVertexi, *pVertexj;
	for (int k = 0; k < m_ArrayFacePointsArray.size(); k++)
	{
		c = 0;
		for (i = 0, j = m_ArrayFacePointsArray[k].size() - 1; i < m_ArrayFacePointsArray[k].size(); j = i++)
		{
			pVertexi = pMesh->GetVertex(m_ArrayFacePointsArray[k][i]);
			pVertexj = pMesh->GetVertex(m_ArrayFacePointsArray[k][j]);
			if (((pVertexi->y() > y) != (pVertexj->y() > y)) &&
				(x < (pVertexj->x() - pVertexi->x()) * (y - pVertexi->y()) / (pVertexj->y() - pVertexi->y()) + pVertexi->x()))
				c = !c;
		}
		if (c)
			return k;
	}
	return -1;

	/*for (i = 0, j = nvert - 1; i < nvert; j = i++)
	{
		if (((verty[i] > testy) != (verty[j] > testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
			c = !c;
	}
	return c;*/
}
int Action_PolyOmino2::FindCellLocatedtPolygon(CMesh3d* pMesh, double x1, double y1, double x2, double y2)
{
	//subdivide with 10 * 10, find how many points lies in the face
	int subx = 10;
	int suby = 10;
	double x, y;
	int polygonindex;
	std::vector<int> m_ArrayPointsInPolygon;
	m_ArrayPointsInPolygon.resize(pMesh->m_ArrayFacePointsArray.size());
	for (int i = 0; i < m_ArrayPointsInPolygon.size(); i++)
		m_ArrayPointsInPolygon[i] = 0;

	for(int i = 0; i <= subx; i++)
		for (int j = 0; j <= suby; j++)
		{
			x = x1 + (x2 - x1) * (i + 0.0) / subx;
			y = y1 + (y2 - y1) * (j + 0.0) / suby;

			polygonindex = FindPointLocatedPolygon(pMesh,pMesh->m_ArrayFacePointsArray, x, y);
			if(polygonindex >= 0)
				m_ArrayPointsInPolygon[polygonindex]++;
		}

	/*int tol = (1 + subx) * (1 + suby) / 2;
	for (int i = 0; i < m_ArrayPointsInPolygon.size(); i++)
	{
		if (m_ArrayPointsInPolygon[i] > tol)
			return i;
	}*/
	double largestvalue = -1;
	int largestindex;
	for (int i = 0; i < m_ArrayPointsInPolygon.size(); i++)
	{
		if (m_ArrayPointsInPolygon[i] > largestvalue)
		{
			largestvalue = m_ArrayPointsInPolygon[i];
			largestindex = i;
		}
	}
	if (largestvalue > (1 + subx) * (1 + suby) * 0.1)
		return largestindex;

	return -1;
}
void Action_PolyOmino2::OnBnClickedButtonUsrastrerization()
{
	// TODO: Add your control notification handler code here

	//pDoc->OnOpenDocument("D:\\Razib\\lamcc_smooth_org_normalize.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	//pPolyOmino->pMesh->m_showLandmarkVertex = TRUE;
	pPolyOmino->pMesh->m_showFace = FALSE;
	pPolyOmino->pMesh->m_showVertexIndex = FALSE;
	pPolyOmino->pMesh->m_showSharpEdgeIndex = FALSE;
	pPolyOmino->pMesh->m_showSelectedSharpVertex = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertexIndex = FALSE;

	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);

	//return;
	std::vector<int> m_ArrayFacePoints, m_ArrayFaceEdges;
	pPolyOmino->pUniformGridMesh = pPolyOmino->pMesh;

	//return;
	//Trace Faces
	pPolyOmino->TraceFaces(m_ArrayFacePoints, m_ArrayFaceEdges);

	//return;

	//Remove the outer face
	//pPolyOmino->RemoveOuterFace(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray, pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray);
	pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.begin() + 3);
	pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + 3);

	pPolyOmino->pUniformGridMesh->m_ArrayStates.resize(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.size() - 1);
	for (int i = 0; i < 48; i++)
		pPolyOmino->pUniformGridMesh->m_ArrayStates[i].push_back(i);
	for (int i = 48; i < pPolyOmino->pUniformGridMesh->m_ArrayStates.size() - 1; i++)
		pPolyOmino->pUniformGridMesh->m_ArrayStates[i].push_back(i + 1);
	pPolyOmino->pUniformGridMesh->m_ArrayStates[40].push_back(48);

	pPolyOmino->pUniformGridMesh->m_ArrayUniformCells.resize(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.size());
	pPolyOmino->pUniformGridMesh->m_ArrayStatesColor.resize(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.size());

	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArrayStatesColor.size(); i++)
	{
		switch (i)
		{
		case 0:
		case 3:
		case 15:
		case 6:
		case 16:
		case 18:
		case 49:
		case 45:
		case 25:
		case 21:
		case 37:
		case 39:
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(0);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(0);
			break;
		case 1:
		case 7:
		case 8:
		case 34:
		case 32:
		case 43:
		case 19:
		case 11:
		case 14:
		case 4:
		case 28:
		case 12:
		case 40:
		case 48:
		case 20:
		case 9:
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(0);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(0);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
			break;
		case 2:
		case 30:
		case 5:
		case 10:
		case 42:
		case 35:
		case 13:
		case 41:
		case 47:
		case 22:
		case 27:
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(0);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(0);
			break;
		case 33:
		case 38:
		case 17:
		case 36:
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(0);
			break;
		default:
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(0);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
			pPolyOmino->pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
			break;
		}

	}
	//SetColor

	//Generate a New Mesh and Generate New Grid Landmark
	//first use a step size
	Point3D lower, upper;
	pPolyOmino->pMesh->ComputeBoundingBox(lower, upper);
	//int xgridnum = 10;
	//double step = (upper.x - lower.x) / xgridnum;
	double step = 0.02095;

	//generate a new mesh and add points into it.
	CMesh3d* pNewMesh3D = new CMesh3d();

	double x, y;
	for(x = lower.x; x < upper.x; x += step)
		for (y = lower.y; y < upper.y + step; y += step)
		{
			pNewMesh3D->AddVertex(new CVertex3d(x, y, 0));
		}
	for (x = lower.x; x < upper.x; x += step)
		pPolyOmino->pUniformGridMesh->xinterval.push_back(x);
	for (y = lower.y; y < upper.y + step; y += step)
		pPolyOmino->pUniformGridMesh->yinterval.push_back(y);

	pPolyOmino->pUniformGridMesh->baseinterval = step;

	int xnum = (upper.x - lower.x) / step + 1;
	int ynum = (upper.y - lower.y) / step + 2;
	int index = 0;

	for(int i = 0; i < xnum; i++)
		for (int j = 0; j < ynum - 1; j++)
		{
			pNewMesh3D->m_ArraySharpEdges.push_back(std::make_pair(ynum * i + j, ynum * i + j + 1));
		}
	for (int j = 0; j < ynum; j++)
		for (int i = 0; i < xnum - 1; i++)
		{
			pNewMesh3D->m_ArraySharpEdges.push_back(std::make_pair(ynum * i + j, ynum * (i + 1) + j));
		}

	pNewMesh3D->m_showSharpEdge = 1;
	pNewMesh3D->m_Transform.Copy(&pPolyOmino->pMesh->m_Transform);
	pDoc->m_SceneGraph.Add(pNewMesh3D);

	std::vector<std::vector<int>> cellfaceindex;
	cellfaceindex.resize(xnum - 1);
	for (int i = 0; i < xnum - 1; i++)
		cellfaceindex[i].resize(ynum - 1);

	for (int i = 0; i < xnum - 1; i++)
		for (int j = 0; j < ynum - 1; j++)
		{
			cellfaceindex[i][j] = -1;
		}
	//pDoc->UpdateTreeControl();
	//pDoc->UpdateAllViews(NULL);
	//return;
	//Set Each grid to its faces
	for (int i = 0; i < xnum - 1; i++)
		for (int j = 0; j < ynum - 1; j++)
		{
			cellfaceindex[i][j] = FindCellLocatedtPolygon(pPolyOmino->pUniformGridMesh, lower.x + step * i, lower.y + step * j, lower.x + step * (i + 1), lower.y + step * (j + 1));

			if (cellfaceindex[i][j] > -1)
			{
				pPolyOmino->pUniformGridMesh->m_ArrayUniformCells[cellfaceindex[i][j]].push_back(std::make_pair(i, j));
			}
		}

	for (int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArrayUniformCells.size(); i++)
	{
		if (pPolyOmino->pUniformGridMesh->m_ArrayUniformCells[i].size() == 0)
			TRACE("Deleted Region: %d ", i);
	}
	//set the plot 
	OutputRastizationPovRay(cellfaceindex,xnum,ynum,pPolyOmino->pUniformGridMesh,"D:\\uniformgrid.inc");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino2::OutputRastizationPovRay(std::vector<std::vector<int>> &cellfaceindex,int xnum,int ynum, CMesh3d* pNewMesh3D, char* filename)
{
	std::ofstream outfile(filename);
	//output the edges
	for (int i = 0; i < xnum - 1; i++)
		for (int j = 0; j < ynum - 1; j++)
		{
			OutputRastizationOneEdgePovRay(cellfaceindex, xnum, ynum, pNewMesh3D, outfile, i,j, -1, 0);
			OutputRastizationOneEdgePovRay(cellfaceindex, xnum, ynum, pNewMesh3D, outfile, i, j, 1, 0);
			OutputRastizationOneEdgePovRay(cellfaceindex, xnum, ynum, pNewMesh3D, outfile, i, j, 0, -1);
			OutputRastizationOneEdgePovRay(cellfaceindex, xnum, ynum, pNewMesh3D, outfile, i, j, 0, 1);
		}
	std::vector<std::pair<int, int>>  m_totalArrayConnectedCells;
	std::vector<std::pair<int, int>> m_ArrayConnectedCells;
	//output the undesignated cells
	for (int i = 0; i < xnum - 1; i++)
		for (int j = 0; j < ynum - 1; j++)
		{
			if (cellfaceindex[i][j] >= 0)
				continue;

			m_ArrayConnectedCells.clear();

			SearchForConnectedCells(cellfaceindex, m_ArrayConnectedCells, xnum, ynum, i, j);
			if (m_ArrayConnectedCells.size() < 10)
			{
				//
				m_totalArrayConnectedCells.insert(m_totalArrayConnectedCells.begin(), m_ArrayConnectedCells.begin(), m_ArrayConnectedCells.end());
			}
		}

	double TOL = pNewMesh3D->baseinterval / 16;
	double x1, y1, x2, y2;
	//output the face
	for (int i = 0; i < m_totalArrayConnectedCells.size(); i++)
	{
		x1 = pNewMesh3D->xinterval[m_totalArrayConnectedCells[i].first];
		x2 = pNewMesh3D->xinterval[m_totalArrayConnectedCells[i].first + 1];
		y1 = pNewMesh3D->yinterval[m_totalArrayConnectedCells[i].second];
		y2 = pNewMesh3D->yinterval[m_totalArrayConnectedCells[i].second + 1];
		//x1 = x1 + TOL;
		//y1 = y1 + TOL;
		//x2 = x2 - TOL;
		//y2 = y2 - TOL;
		outfile << "object{box{<" << x1 << " , " << y1 << " , " << 0 << " >,<" << x2 << " , " << y2 << " , h >  texture { cell_filltex} }}" << std::endl;
	}
	outfile.close();
}
void Action_PolyOmino2::SearchForConnectedCells(std::vector<std::vector<int>> &cellfaceindex, std::vector<std::pair<int, int>> &m_ArrayConnectedCells, int xnum, int ynum, int i, int j)
{
	std::vector<std::vector<int>> cellfaceindexbak = cellfaceindex;
	for (int i1 = 0; i1 < cellfaceindexbak.size(); i1++)
	{
		for (int j1 = 0; j1 < cellfaceindexbak[i1].size(); j1++)
		{
			cellfaceindexbak[i1][j1] = 0;
		}
	}
	std::pair<int, int> pair1;

	std::stack<std::pair<int, int>> stack1;
	stack1.push(std::make_pair(i, j));
	m_ArrayConnectedCells.push_back(std::make_pair(i, j));
	cellfaceindexbak[i][j] = 1;
	int xoffset, yoffset;

	while (!stack1.empty())
	{
		pair1 = stack1.top();
		stack1.pop();
		i = pair1.first;
		j = pair1.second;

		xoffset = 1;
		yoffset = 0;
		if (!(i + xoffset >= xnum - 1 || i + xoffset < 0 || j + yoffset >= ynum - 1 || j + yoffset < 0))
			if (!cellfaceindexbak[i + xoffset][j + yoffset] && cellfaceindex[i + xoffset][j + yoffset] == -1)
			{
				stack1.push(std::make_pair(i + xoffset, j + yoffset));
				m_ArrayConnectedCells.push_back(std::make_pair(i + xoffset, j + yoffset));
				cellfaceindexbak[i + xoffset][j + yoffset] = 1;
			}

		xoffset = -1;
		yoffset = 0;
		if (!(i + xoffset >= xnum - 1 || i + xoffset < 0 || j + yoffset >= ynum - 1 || j + yoffset < 0))
			if (!cellfaceindexbak[i + xoffset][j + yoffset] && cellfaceindex[i + xoffset][j + yoffset] == -1)
			{
				stack1.push(std::make_pair(i + xoffset, j + yoffset));
				m_ArrayConnectedCells.push_back(std::make_pair(i + xoffset, j + yoffset));
				cellfaceindexbak[i + xoffset][j + yoffset] = 1;
			}
		xoffset = 0;
		yoffset = 1;

		if (!(i + xoffset >= xnum - 1 || i + xoffset < 0 || j + yoffset >= ynum - 1 || j + yoffset < 0))
			if (!cellfaceindexbak[i + xoffset][j + yoffset] && cellfaceindex[i + xoffset][j + yoffset] == -1)
			{
				stack1.push(std::make_pair(i + xoffset, j + yoffset));
				m_ArrayConnectedCells.push_back(std::make_pair(i + xoffset, j + yoffset));
				cellfaceindexbak[i + xoffset][j + yoffset] = 1;
			}
		xoffset = 0;
		yoffset = -1;

		if (!(i + xoffset >= xnum - 1 || i + xoffset < 0 || j + yoffset >= ynum - 1 || j + yoffset < 0))
			if (!cellfaceindexbak[i + xoffset][j + yoffset] && cellfaceindex[i + xoffset][j + yoffset] == -1)
			{
				stack1.push(std::make_pair(i + xoffset, j + yoffset));
				m_ArrayConnectedCells.push_back(std::make_pair(i + xoffset, j + yoffset));
				cellfaceindexbak[i + xoffset][j + yoffset] = 1;
			}
		if (m_ArrayConnectedCells.size() > 10)
			return;
	}
}
void Action_PolyOmino2::OutputRastizationOneEdgePovRay(std::vector<std::vector<int>> &cellfaceindex, int xnum, int ynum, CMesh3d* pNewMesh3D, std::ofstream &outfile, int i, int j, int xoffset, int yoffset)
{
	//output the common edges between the celles
	//if the two cells belongs to two different polygons, one type. otherwise another type
	if (i + xoffset >= xnum - 1 || i + xoffset < 0 || j + yoffset >= ynum - 1 || j + yoffset < 0)
		return;

	if (cellfaceindex[i][j] == -1 && cellfaceindex[i + xoffset][j + yoffset] == -1)
		return;

	if (xoffset > 0)
		return;
	if (yoffset > 0)
		return;
	char edgewidthandtex[200];
	if (cellfaceindex[i][j] != cellfaceindex[i + xoffset][j + yoffset])
	{
		if(xoffset == 0)
			sprintf(edgewidthandtex, "%s", " > ,r_edge_hor_2Region texture { edge_tex_hor_2Region } }");
		else
			sprintf(edgewidthandtex, "%s", " > , r_edge_ver_2Region texture { edge_tex_ver_2Region } }");
	}
	else
	{
		if (xoffset == 0)
			sprintf(edgewidthandtex, "%s", " > ,r_edge_hor_1Region texture { edge_tex_hor_1Region } }");
		else
			sprintf(edgewidthandtex, "%s", " > , r_edge_ver_1Region texture { edge_tex_ver_1Region } }");
	}

	double x1, y1, x2, y2;
	if (xoffset == 0)
	{
		if (yoffset == -1)
		{
			x1 = pNewMesh3D->xinterval[i];
			x2 = pNewMesh3D->xinterval[i + 1];
			y1 = y2 = pNewMesh3D->yinterval[j];
		}
		else
		{
			x1 = pNewMesh3D->xinterval[i];
			x2 = pNewMesh3D->xinterval[i + 1];
			y1 = y2 = pNewMesh3D->yinterval[j + 1];
		}
	}
	else
	{
		if (xoffset == -1)
		{
			y1 = pNewMesh3D->yinterval[j];
			y2 = pNewMesh3D->yinterval[j + 1];
			x1 = x2 = pNewMesh3D->xinterval[i];
		}
		else
		{
			y1 = pNewMesh3D->yinterval[j];
			y2 = pNewMesh3D->yinterval[j + 1];
			x1 = x2 = pNewMesh3D->xinterval[i + 1];
		}
	}

	outfile << "cylinder {<" << x1 << " , " << y1 << " , " << 0 << " >,<" << x2 << " , " << y2 << " , " << 0 << edgewidthandtex << std::endl;
}

void Action_PolyOmino2::OnBnClickedButtonLoadchina()
{
	// TODO: 在此添加控件通知处理程序代码
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = FALSE;
	pPolyOmino->pMesh->m_showLandmarkEdge = FALSE;
	pPolyOmino->pMesh->m_showFace = FALSE;

	pPolyOmino->pMesh->SetVertexFlagAsZero();

	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\china\\Dense_mesh_Landmark.txt");
	sprintf(message,"D:\\papers\\code\\data\\Geo\\ditu\\china\\Dense_mesh_Landmark40.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);
}


void Action_PolyOmino2::OnBnClickedButtonLoadchinaTriangulate()
{
	// TODO: 在此添加控件通知处理程序代码
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = FALSE;
	pPolyOmino->pMesh->m_showFace = FALSE;
	pPolyOmino->pMesh->m_showLandmarkVertexIndex = FALSE;
	pPolyOmino->pMesh->m_showLandmarkIndex = FALSE;

	pPolyOmino->pMesh->SetVertexFlagAsZero();

	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\china\\Dense_mesh_Landmark.txt");
	sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate_Landmak87.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);
}


void Action_PolyOmino2::OnBnClickedButtonSetalllandmarkRegular()
{
	// TODO: Add your control notification handler code here
	if (!pPolyOmino->pMesh)
		return;
	for (int i = 0; i < pPolyOmino->pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pPolyOmino->pMesh->m_LandMarkArray[i];
		pLandMark->isAuxillary = 0;
	}
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonDeleteduplicatepointsinlandmark()
{
	if (!pPolyOmino->pMesh)
		return;
	for (int i = 0; i < pPolyOmino->pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pPolyOmino->pMesh->m_LandMarkArray[i];
		for (int j = 1; j < pLandMark->landmark_points.size(); j++)
		{
			if (pLandMark->landmark_points[j] == pLandMark->landmark_points[j - 1])
			{
				pLandMark->landmark_points.erase(pLandMark->landmark_points.begin() + j);
				j--;
			}
		}
	}
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino2::OnBnClickedButtonLoadchinastraightened()
{
	// TODO: 在此添加控件通知处理程序代码
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87_straightened.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->pMesh->m_showLandmarkEdge = TRUE;
	pPolyOmino->pMesh->m_showLandmarkVertex = FALSE;
	pPolyOmino->pMesh->m_showFace = FALSE;
	pPolyOmino->pMesh->m_showLandmarkVertexIndex = FALSE;
	pPolyOmino->pMesh->m_showLandmarkIndex = FALSE;

	pPolyOmino->pMesh->SetVertexFlagAsZero();

	pPolyOmino->OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\china\\Dense_mesh_Landmark.txt");
	sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate_Landmak87.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	pPolyOmino->Loadlandmark(landmarkname);
}


void Action_PolyOmino2::OnBnClickedButtonSavedatafrommesh()
{
	CString FileName = "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\3414.m";
	CString s1;
	CString DebugInformation = FileName;
	DebugInformation = DebugInformation.Left(DebugInformation.ReverseFind('.'));

	CString filename1 = DebugInformation.Right(DebugInformation.GetLength() - DebugInformation.ReverseFind('\\') - 1);
	iterationnum = atoi(filename1);
	DebugInformation = DebugInformation + ".txt";
	//Mesh
	//OnBnClickedOminoLoadusastreighten();
	//OnBnClickedButtonLoadfinalrectilinearmap();
	pDoc->OnOpenDocument(FileName);
	pPolyOmino->pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	pPolyOmino->pMesh = pPolyOmino->pUniformGridMesh;
	//pPolyOmino->GenerateUniformGridMesh();

	//Update edge connection
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	//
	pPolyOmino->InitializeHorizontalandVerticalSegmentArray();

	pPolyOmino->MergeHorizontalVerticalSegments();

	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\3414.txt");
	pPolyOmino->SaveDebugInformation(filename);
}


void Action_PolyOmino2::OnBnClickedButtonLoadrectilinearmesh()
{
	CString FileName;
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;
	//CString FileName = "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\3414.m";
	CString s1;
	CString DebugInformation = FileName;
	DebugInformation = DebugInformation.Left(DebugInformation.ReverseFind('.'));

	CString filename1 = DebugInformation.Right(DebugInformation.GetLength() - DebugInformation.ReverseFind('\\') - 1);
	iterationnum = atoi(filename1);
	DebugInformation = DebugInformation + ".txt";
	//Mesh
	//OnBnClickedOminoLoadusastreighten();
	//OnBnClickedButtonLoadfinalrectilinearmap();
	pDoc->OnOpenDocument(FileName);
	pPolyOmino->pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	pPolyOmino->pMesh = pPolyOmino->pUniformGridMesh;
	//pPolyOmino->GenerateUniformGridMesh();

	//Update edge connection
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	//
	pPolyOmino->InitializeHorizontalandVerticalSegmentArray();

	pPolyOmino->MergeHorizontalVerticalSegments();
}


void Action_PolyOmino2::OnBnClickedButtonModifyrectilinear3414()
{
	CString FileName = "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\3414.m";
	CString s1;
	CString DebugInformation = FileName;
	DebugInformation = DebugInformation.Left(DebugInformation.ReverseFind('.'));

	CString filename1 = DebugInformation.Right(DebugInformation.GetLength() - DebugInformation.ReverseFind('\\') - 1);
	iterationnum = atoi(filename1);
	DebugInformation = DebugInformation + ".txt";
	//Mesh
	//OnBnClickedOminoLoadusastreighten();
	//OnBnClickedButtonLoadfinalrectilinearmap();
	pDoc->OnOpenDocument(FileName);
	pPolyOmino->pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	pPolyOmino->pMesh = pPolyOmino->pUniformGridMesh;

	//
	CVertex3d* A1 = new CVertex3d();
	//A1->Set((pPolyOmino->pMesh->GetVertex(8)->x() + pPolyOmino->pMesh->GetVertex(9)->x()) / 2,
	//	(pPolyOmino->pMesh->GetVertex(8)->y() + pPolyOmino->pMesh->GetVertex(9)->y()) / 2,
	//	(pPolyOmino->pMesh->GetVertex(8)->z() + pPolyOmino->pMesh->GetVertex(9)->z()) / 2);
		A1->Set((pPolyOmino->pMesh->GetVertex(274)->x()),
		(pPolyOmino->pMesh->GetVertex(8)->y()),
		(pPolyOmino->pMesh->GetVertex(8)->z() ));
	int A1index = pPolyOmino->pMesh->NbVertex();
	pPolyOmino->pMesh->AddVertex(A1);

	CVertex3d* A2 = new CVertex3d();
	A2->Set((pPolyOmino->pMesh->GetVertex(24)->x() + pPolyOmino->pMesh->GetVertex(232)->x()) / 2,
		(pPolyOmino->pMesh->GetVertex(24)->y() + pPolyOmino->pMesh->GetVertex(232)->y()) / 2,
		(pPolyOmino->pMesh->GetVertex(24)->z() + pPolyOmino->pMesh->GetVertex(232)->z()) / 2);
	int A2index = pPolyOmino->pMesh->NbVertex();
	pPolyOmino->pMesh->AddVertex(A2);

	CVertex3d* A3 = new CVertex3d();
	A3->Set((pPolyOmino->pMesh->GetVertex(232)->x() + pPolyOmino->pMesh->GetVertex(233)->x()) / 2,
		(pPolyOmino->pMesh->GetVertex(232)->y() + pPolyOmino->pMesh->GetVertex(233)->y()) / 2,
		(pPolyOmino->pMesh->GetVertex(232)->z() + pPolyOmino->pMesh->GetVertex(233)->z()) / 2);
	int A3index = pPolyOmino->pMesh->NbVertex();
	pPolyOmino->pMesh->AddVertex(A3);

	CVertex3d* A4 = new CVertex3d();
	A4->Set(A3->x(),A2->y(),A2->z());
	int A4index = pPolyOmino->pMesh->NbVertex();
	pPolyOmino->pMesh->AddVertex(A4);

	pPolyOmino->pMesh->m_ArraySharpEdges.erase(pPolyOmino->pMesh->m_ArraySharpEdges.begin() + 312);
	pPolyOmino->pMesh->m_ArraySharpEdges.erase(pPolyOmino->pMesh->m_ArraySharpEdges.begin() + 224);
	pPolyOmino->pMesh->m_ArraySharpEdges.erase(pPolyOmino->pMesh->m_ArraySharpEdges.begin() + 152);
	pPolyOmino->pMesh->m_ArraySharpEdges.erase(pPolyOmino->pMesh->m_ArraySharpEdges.begin() + 125);
	pPolyOmino->pMesh->m_ArraySharpEdges.erase(pPolyOmino->pMesh->m_ArraySharpEdges.begin() + 111);
	pPolyOmino->pMesh->m_ArraySharpEdges.erase(pPolyOmino->pMesh->m_ArraySharpEdges.begin() + 102);
	pPolyOmino->pMesh->m_ArraySharpEdges.erase(pPolyOmino->pMesh->m_ArraySharpEdges.begin() + 91);

	std::pair<int, int> pair1;
	pair1.first = A1index;
	pair1.second = 8;
	pPolyOmino->pMesh->m_ArraySharpEdges.push_back(pair1);

	pair1.first = A2index;
	pair1.second = A4index;
	pPolyOmino->pMesh->m_ArraySharpEdges.push_back(pair1);

	pair1.first = A1index;
	pair1.second = 274;
	pPolyOmino->pMesh->m_ArraySharpEdges.push_back(pair1);

	pair1.first = A4index;
	pair1.second = A3index;
	pPolyOmino->pMesh->m_ArraySharpEdges.push_back(pair1);

	pair1.first = 26;
	pair1.second = A2index;
	pPolyOmino->pMesh->m_ArraySharpEdges.push_back(pair1);

	pair1.first = 232;
	pair1.second = A2index;
	pPolyOmino->pMesh->m_ArraySharpEdges.push_back(pair1);

	pair1.first = 232;
	pair1.second = A3index;
	pPolyOmino->pMesh->m_ArraySharpEdges.push_back(pair1);

	pPolyOmino->CheckandFixTopologyValidation(3414);

	//Update edge connection
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	//
	pPolyOmino->InitializeHorizontalandVerticalSegmentArray();

	pPolyOmino->MergeHorizontalVerticalSegments();

	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\3414.m");
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);

	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\3414.txt");
	pPolyOmino->SaveDebugInformation(filename);
}


void Action_PolyOmino2::OnBnClickedButtonModifyrectilinearmesh3350()
{
	CString FileName = "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\3350.m";
	CString s1;
	CString DebugInformation = FileName;
	DebugInformation = DebugInformation.Left(DebugInformation.ReverseFind('.'));

	CString filename1 = DebugInformation.Right(DebugInformation.GetLength() - DebugInformation.ReverseFind('\\') - 1);
	iterationnum = atoi(filename1);
	DebugInformation = DebugInformation + ".txt";
	//Mesh
	//OnBnClickedOminoLoadusastreighten();
	//OnBnClickedButtonLoadfinalrectilinearmap();
	pDoc->OnOpenDocument(FileName);
	pPolyOmino->pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	pPolyOmino->pMesh = pPolyOmino->pUniformGridMesh;

	//
	CVertex3d* A1 = new CVertex3d();
	A1->Set(pPolyOmino->pMesh->GetVertex(497)->x(),
		pPolyOmino->pMesh->GetVertex(12)->y(),
		0);
	int A1index = pPolyOmino->pMesh->NbVertex();
	pPolyOmino->pMesh->AddVertex(A1);

	

	pPolyOmino->pMesh->m_ArraySharpEdges.erase(pPolyOmino->pMesh->m_ArraySharpEdges.begin() + 522);
	pPolyOmino->pMesh->m_ArraySharpEdges.erase(pPolyOmino->pMesh->m_ArraySharpEdges.begin() + 314);

	std::pair<int, int> pair1;
	pair1.first = A1index;
	pair1.second = 12;
	pPolyOmino->pMesh->m_ArraySharpEdges.push_back(pair1);

	pair1.first = A1index;
	pair1.second = 497;
	pPolyOmino->pMesh->m_ArraySharpEdges.push_back(pair1);

	pPolyOmino->CheckandFixTopologyValidation(3350);

	//Update edge connection
	pPolyOmino->UpdatePointConnectedSelectedEdgeIndex();

	//
	pPolyOmino->InitializeHorizontalandVerticalSegmentArray();

	pPolyOmino->MergeHorizontalVerticalSegments();

	char filename[200];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\3350.m");
	pPolyOmino->pUniformGridMesh->SaveMFile(filename);

	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\3350.txt");
	pPolyOmino->SaveDebugInformation(filename);
}


void Action_PolyOmino2::OnBnClickedButtonLoadfinalchinapolyomino()
{
	// TODO: 在此添加控件通知处理程序代码
	int iterationnum;
	pPolyOmino->LoadData1("D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\3414.m", iterationnum);

	pPolyOmino->AdjustHorizontalandVerticalGroupofLinePositions();

	//pPolyOmino->DetectInvalidSharpEdges();
	//pPolyOmino->AdjustHorizontalandVerticalGroupofLinePositions_3base();
	//Each four elements form one group

	//return;
	std::vector<int> m_ArrayFacePoints, m_ArrayFaceEdges;

	//Trace Faces
	pPolyOmino->TraceFaces(m_ArrayFacePoints, m_ArrayFaceEdges);

	//Remove the outer face
	//pPolyOmino->RemoveOuterFace(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray, pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray);

	//pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.begin() + 2);
	//pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + 2);

	pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFacePointsArray.begin() + 2);
	pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pPolyOmino->pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + 2);

	//Remove Duplicate Faces
	RemoveDulicateFaces();

	//
	//RemoveDeletedFaces();

	//return;
	//
	pPolyOmino->ComputeFacesFourElementArray(m_ArrayFacePoints, m_ArrayFaceEdges);

	//
	pPolyOmino->AssignUniformCellstoFaces();

	pPolyOmino->Loadworldpopulation();
	//return;
	//pPolyOmino->LoadTargetFaceArea("D:\\papers\\code\\data\\Geo\\dense\\1788_usapopulation.txt");

	//Get the maximal density for each grid cell
	double maximaldensityeachcell = -1;
	double minimaldensityeachcell = 1e10;
	double maximalstatepopulation = -1;
	double minimalstatepopulation = 1e10;

	//SetStatesArray_worldmap();
	//
	//pPolyOmino->SetStatesArray();

	//
	//pPolyOmino->LoadStateName("D:\\papers\\code\\data\\Geo\\dense\\1788_districtname.txt");
	//LoadStateName("D:\\papers\\code\\data\\Geo\\dense\\1788_districtname_abbre.txt");
	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usapopulation_2010_2014.txt");
	//pPolyOmino->LoadTargetFaceAreaArray(1);
	//LoadTargetFaceAreaArray_facebookpenetration("D:\\papers\\code\\data\\Geo\\dense\\1788_facebookpenetration.txt");

	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usa_completed_highschool.txt");

	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usa_familyincome.txt");

	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usa_median_household_income.txt");

	//
	//pPolyOmino->ComputetheDifference(0, 24);

	//
	//GetFacesDifferenceMaximalandMinimalDensityPerCell_Array(maximaldensityeachcell,minimaldensityeachcell,maximalstatepopulation,minimalstatepopulation);

	//
	pPolyOmino->GetStateMaximalandMinimalDensityPerCell_Array(maximaldensityeachcell, minimaldensityeachcell, maximalstatepopulation, minimalstatepopulation);

	//PlotFaceCellDifference
	//PlotFaceCellByDifference_Array(3,maximaldensityeachcell,minimaldensityeachcell,maximalstatepopulation,minimalstatepopulation);

	//
	//pPolyOmino->GetFacesMaximalandMinimalDensityPerCell_Array(maximaldensityeachcell, minimaldensityeachcell, maximalstatepopulation, minimalstatepopulation);

	//PlotFaceCellsBySeveralColorsFromTopLeft(3,maximaldensityeachcell,minimaldensityeachcell);
	//2014 24 
	//2000 10
	pPolyOmino->PlotFaceCellsBySeveralColorsFromTopLeft_Array(0, 3, maximaldensityeachcell, minimaldensityeachcell, maximalstatepopulation, minimalstatepopulation);

	//colorbar
	//PlotFaceCellsbyContinuousColor(maximaldensityeachcell);
	//pPolyOmino->LoadStateName("D:\\papers\\code\\data\\Geo\\dense\\1788_districtname_abbre.txt");*/
	pDoc->UpdateAllViews(NULL);
}
