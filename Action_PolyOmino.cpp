// Action_PolyOmino.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Action_PolyOmino.h"
#include "FormView3.h"
#include "SDIViewSwitchDoc.h"
#include "lib3d/Mesh3d.h"
#include "MyTreeView.h"
#include <stack>
#include "DialogInputInformation.h"
#include "FaceInputArea.h"
#include "engine.h"
#include "matrix.h"
#include <atlimage.h>
#include <string>
#include <sstream>
#include "kdtree.h"
#include "Nurbs3D.h"
#include "lib3d/Base3d.h"
#include "lib3d/Mesh3d_OpenMesh.h"
#include "MySparseMatrix.h"
#include <map>
#include "Shape_Polygon.h"
#include "ShapeFile_Polygon_Header.h"
#include "Polyomino_Edge_Subdivision.h"
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#define bswap16(x) ((x)>>8 | ((x)&255)<<8)
#define bswap32(x) ((bswap16((x) >> 16) & 65535) | (bswap16((x)& 65535) << 16))
#define PI 3.1415926
#define TOL 1e-6
#define MAX_RATIO 1000
#define MIN_RATIO 0
// Action_PolyOmino dialog

IMPLEMENT_DYNAMIC(Action_PolyOmino, CPropertyPage)
Action_PolyOmino* pPolyOmino;
extern Actions_HarmonicMapping* pHarmonicMapping;
Action_PolyOmino::Action_PolyOmino()
	: CPropertyPage(Action_PolyOmino::IDD)
{
	m_SplitAngle = 90;
	m_MergeAngle = 45;
	m_EdgeSubdivisionNum = 3;
	m_bSplitDirection = TRUE;

	m_SplitLandmarkFirstRun = TRUE;
	m_SloppingedgesubdivisionFirstRun = TRUE;
	m_MergeOneEdgeWithSlopingFirstRun = TRUE;
	m_MergeSmallEdgesFirstRun = TRUE;
	pPolyOmino = this;
	m_LatestWorldMapIndex = -1;
	m_LatestLandmarkIndex = -1;
	m_LatestSelectedLandmarkIndex = -1;
	pFinalRectilinearMesh = NULL;
	pUniformGridMesh = NULL;
}

Action_PolyOmino::~Action_PolyOmino()
{
}

void Action_PolyOmino::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Omino_SplitAngleSlider, m_SplitAngleSlider);
	DDX_Control(pDX, IDC_SplitDirectionCHECK1, m_SplitDirection);
	DDX_Control(pDX, IDC_Omino_MergeAngleSlider, m_OminoMergeAngle);
	DDX_Control(pDX, IDC_Omino_SloppingEdgeSubdivision, m_EdgeSubdivision);
	DDX_Control(pDX, IDC_Omino_SmallLandmarkLengthSlider, m_MergeSmallLandmarkLength);
	DDX_Control(pDX, IDC_Omino_MergeSmallAngleSlider, m_MergeSmallLandmarkAngle);
}


BEGIN_MESSAGE_MAP(Action_PolyOmino, CPropertyPage)
	ON_BN_CLICKED(IDC_GENERATELANDMARKS, &Action_PolyOmino::OnBnClickedGeneratelandmarks)
	ON_BN_CLICKED(IDC_Polyomino_Rectification, &Action_PolyOmino::OnBnClickedPolyominoRectification)
	ON_BN_CLICKED(IDC_Polyomino_MergeSlope, &Action_PolyOmino::OnBnClickedPolyominoMergeslope)
	ON_BN_CLICKED(IDC_Polyomino_DeleteLandmark, &Action_PolyOmino::OnBnClickedPolyominoDeletelandmark)
	ON_BN_CLICKED(IDC_Polyomino_MergeSameStyle, &Action_PolyOmino::OnBnClickedPolyominoMergesamestyle)
	ON_BN_CLICKED(IDC_Polyomino_ThreeConnected, &Action_PolyOmino::OnBnClickedPolyominoThreeconnected)
	ON_BN_CLICKED(IDC_Polyomino_MergeSmall, &Action_PolyOmino::OnBnClickedPolyominoMergesmall)
	ON_BN_CLICKED(IDC_Polyomino_SaveLandmark, &Action_PolyOmino::OnBnClickedPolyominoExportlandmark)
	ON_BN_CLICKED(IDC_Polyomino_ExtendSmall, &Action_PolyOmino::OnBnClickedPolyominoExtendsmall)
	ON_BN_CLICKED(IDC_Polyomino_CurveStraight, &Action_PolyOmino::OnBnClickedPolyominoCurvestraight)
	ON_BN_CLICKED(IDC_Omino_SetCurrentMesh, &Action_PolyOmino::OnBnClickedOminoSetcurrentmesh)
	ON_BN_CLICKED(IDC_Omino_SPLITLANDMARK, &Action_PolyOmino::OnBnClickedOminoSplitlandmark)
	ON_WM_HSCROLL()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SplitDirectionCHECK1, &Action_PolyOmino::OnBnClickedSplitdirectioncheck1)
	ON_BN_CLICKED(IDC_Polyomino_SloppingEdgeSubdivision, &Action_PolyOmino::OnBnClickedPolyominoSloppingedgesubdivision)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_Polyomino_DeleteSelectedLandmarks, &Action_PolyOmino::OnBnClickedPolyominoDeleteselectedlandmarks)
	ON_BN_CLICKED(IDC_Omino_LoadUSAMap, &Action_PolyOmino::OnBnClickedOminoLoadusamap)
	ON_BN_CLICKED(IDC_Polyomino_MergeSeletedLandMarks, &Action_PolyOmino::OnBnClickedPolyominoMergeseletedlandmarks)
	ON_BN_CLICKED(IDC_Polyomino_ClearLandMarkSelection, &Action_PolyOmino::OnBnClickedPolyominoClearlandmarkselection)
	ON_BN_CLICKED(IDC_Polyomino_InverseSeletedLandMarkStyle, &Action_PolyOmino::OnBnClickedPolyominoInverseseletedlandmarkstyle)
	ON_BN_CLICKED(IDC_Polyomino_Save_Landmark, &Action_PolyOmino::OnBnClickedPolyominoSaveLandmark)
	ON_BN_CLICKED(IDC_Polyomino_LoadLandmark, &Action_PolyOmino::OnBnClickedPolyominoLoadlandmark)
	ON_BN_CLICKED(IDC_Polyomino_SmoothLandMarks, &Action_PolyOmino::OnBnClickedPolyominoSmoothlandmarks)
	ON_BN_CLICKED(IDC_Polyomino_SmoothSelectedLandMarks, &Action_PolyOmino::OnBnClickedPolyominoSmoothselectedlandmarks)
	ON_BN_CLICKED(IDC_Polyomino_Triangulate, &Action_PolyOmino::OnBnClickedPolyominoTriangulate)
	ON_BN_CLICKED(IDC_Polyomino_SplitLandmarks, &Action_PolyOmino::OnBnClickedPolyominoSplitlandmarks)
	ON_BN_CLICKED(IDC_Polyomino_ClearVertexSelection, &Action_PolyOmino::OnBnClickedPolyominoClearvertexselection)
	ON_BN_CLICKED(IDC_Polyomino_CheckInvalidEdges, &Action_PolyOmino::OnBnClickedPolyominoCheckinvalidedges)
	ON_BN_CLICKED(IDC_Polyomino_AddLandmark, &Action_PolyOmino::OnBnClickedPolyominoAddlandmark)
	ON_BN_CLICKED(IDC_Polyomino_BackUpLandMarks, &Action_PolyOmino::OnBnClickedPolyominoBackuplandmarks)
	ON_BN_CLICKED(IDC_Polyomino_SetAuxillaryLandmark, &Action_PolyOmino::OnBnClickedPolyominoSetauxillarylandmark)
	ON_BN_CLICKED(IDC_Polyomino_AuxillaryVisiable, &Action_PolyOmino::OnBnClickedPolyominoAuxillaryvisiable)
	ON_BN_CLICKED(IDC_Polyomino_DeleteAuxillaryLandmarks, &Action_PolyOmino::OnBnClickedPolyominoDeleteauxillarylandmarks)
	ON_BN_CLICKED(IDC_Polyomino_FixPoints, &Action_PolyOmino::OnBnClickedPolyominoFixpoints)
	ON_BN_CLICKED(IDC_Polyomino_UnFixPoints, &Action_PolyOmino::OnBnClickedPolyominoUnfixpoints)
	ON_BN_CLICKED(IDC_Polyomino_CopyFixPoints, &Action_PolyOmino::OnBnClickedPolyominoCopyfixpoints)
	ON_BN_CLICKED(IDC_Omino_LoadUSAStreighten, &Action_PolyOmino::OnBnClickedOminoLoadusastreighten)
	ON_BN_CLICKED(IDC_Omino_LoadGeorgiaStreighten, &Action_PolyOmino::OnBnClickedOminoLoadGeorgiaStreighten)
	ON_BN_CLICKED(IDC_UniformGrid_Initialization, &Action_PolyOmino::OnBnClickedUniformgridInitialization)
	ON_BN_CLICKED(IDC_UniformGrid_Progessive_Mesh_Iteration, &Action_PolyOmino::OnBnClickedUniformgridProgessiveMeshIteration)
	ON_BN_CLICKED(IDC_Polyomino_ShortestPath, &Action_PolyOmino::OnBnClickedPolyominoShortestpath)
	ON_BN_CLICKED(IDC_Polyomino_Save_M_Landmark, &Action_PolyOmino::OnBnClickedPolyominoSaveMLandmark)
	ON_BN_CLICKED(IDC_Polyomino_TuteEmbedding, &Action_PolyOmino::OnBnClickedPolyominoTuteembedding)
	ON_BN_CLICKED(IDC_Omino_LoadDebugInformation, &Action_PolyOmino::OnBnClickedOminoLoaddebuginformation)
	ON_BN_CLICKED(IDC_Polyomino_AddVertexSelection, &Action_PolyOmino::OnBnClickedPolyominoAddvertexselection)
	ON_BN_CLICKED(IDC_Polyomino_AddEdgeSelection, &Action_PolyOmino::OnBnClickedPolyominoAddedgeselection)
	ON_BN_CLICKED(IDC_Omino_DataDrivenMorph, &Action_PolyOmino::OnBnClickedOminoDatadrivenmorph)
	ON_BN_CLICKED(IDC_Omino_DataDrivenMorphOptimization, &Action_PolyOmino::OnBnClickedOminoDatadrivenmorphoptimization)
	ON_BN_CLICKED(IDC_Polyomino_SetFaceArea, &Action_PolyOmino::OnBnClickedPolyominoSetArea)
	ON_BN_CLICKED(IDC_Omino_SAVEFACEAREA, &Action_PolyOmino::OnBnClickedOminoSavefacearea)
	ON_BN_CLICKED(IDC_Omino_LOADFACEAREA, &Action_PolyOmino::OnBnClickedOminoLoadfacearea)
	ON_BN_CLICKED(IDC_Omino_AddMergeConstraints, &Action_PolyOmino::OnBnClickedOminoAddmergeconstraints)
	ON_BN_CLICKED(IDC_Omino_SaveMergeConstraints, &Action_PolyOmino::OnBnClickedOminoSavemergeconstraints)
	ON_BN_CLICKED(IDC_Omino_LoadMergeConstraints, &Action_PolyOmino::OnBnClickedOminoLoadmergeconstraints)
	ON_BN_CLICKED(IDC_Omino_LoadData, &Action_PolyOmino::OnBnClickedOminoLoaddata)
	ON_BN_CLICKED(IDC_Omino_LoadGeorgiaMap, &Action_PolyOmino::OnBnClickedOminoLoadgeorgiamap)
	ON_BN_CLICKED(IDC_Omino_UniformGrid, &Action_PolyOmino::OnBnClickedOminoUniformgrid)
	ON_BN_CLICKED(IDC_Omino_JianZhi, &Action_PolyOmino::OnBnClickedOminoJianzhi)
	ON_BN_CLICKED(IDC_Polyomino_CheckTriangulation, &Action_PolyOmino::OnBnClickedPolyominoChecktriangulation)
	ON_BN_CLICKED(IDC_Omino_LoadIllinoisMap, &Action_PolyOmino::OnBnClickedOminoLoadillinoismap)
	ON_BN_CLICKED(IDC_Omino_LoadIllinoisStreighten, &Action_PolyOmino::OnBnClickedOminoLoadillinoisstreighten)
	ON_BN_CLICKED(IDC_Polyomino_InverseLandmarkSelection, &Action_PolyOmino::OnBnClickedPolyominoInverselandmarkselection)
	ON_BN_CLICKED(IDC_Polyomino_InverseAllLandmarkStyle, &Action_PolyOmino::OnBnClickedPolyominoInversealllandmarkStyle)
	ON_BN_CLICKED(IDC_BUTTON_KDTREE, &Action_PolyOmino::OnBnClickedButtonKdtree)
	ON_BN_CLICKED(IDC_BUTTON_KDTREE_ITERATION, &Action_PolyOmino::OnBnClickedButtonKdtreeIteration)
	ON_BN_CLICKED(IDC_Polyomino_OpenMesh, &Action_PolyOmino::OnBnClickedPolyominoOpenmesh)
	ON_BN_CLICKED(IDC_BUTTON_LOADWORLDMAP, &Action_PolyOmino::OnBnClickedButtonLoadworldmap)
	ON_BN_CLICKED(IDC_BUTTON_SAVESIMPLIFIEDMFILE, &Action_PolyOmino::OnBnClickedButtonSavesimplifiedmfile)
	ON_BN_CLICKED(IDC_BUTTON_SAVESELECTEDPOINTS, &Action_PolyOmino::OnBnClickedButtonSaveselectedPoints)
	ON_BN_CLICKED(IDC_BUTTON_LOADSELECTEDPOINTS, &Action_PolyOmino::OnBnClickedButtonLoadselectedPoints)
	ON_BN_CLICKED(IDC_Polyomino_CurveStraight_Selected, &Action_PolyOmino::OnBnClickedPolyominoCurvestraightSelected)
	ON_BN_CLICKED(IDC_BUTTON_SUBDIVIDE_EDGE, &Action_PolyOmino::OnBnClickedButtonSubdivideEdge)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_LANDMARK, &Action_PolyOmino::OnBnClickedButtonSearchLandmark)
	ON_BN_CLICKED(IDC_BUTTON_SUBDIVIDE_EDGE_SLOPPING_HORIZONTAL, &Action_PolyOmino::OnBnClickedButtonSubdivideEdgeSloppingHorizontal)
	ON_BN_CLICKED(IDC_BUTTON_SUBDIVIDE_EDGE_SLOPPING_VERTICAL, &Action_PolyOmino::OnBnClickedButtonSubdivideEdgeSloppingVertical)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_LANDMARK2, &Action_PolyOmino::OnBnClickedButtonSearchLandmark2)
	ON_BN_CLICKED(IDC_Polyomino_Mesh_FixVertices, &Action_PolyOmino::OnBnClickedPolyominoMeshFixvertices)
	ON_BN_CLICKED(IDC_Polyomino_Mesh_FixVertices2, &Action_PolyOmino::OnBnClickedPolyominoMeshFixvertices2)
	ON_BN_CLICKED(IDC_Polyomino_Mesh_FixVertices3, &Action_PolyOmino::OnBnClickedPolyominoMeshFixvertices3)
	ON_BN_CLICKED(IDC_Polyomino_Mesh_FixVertices4, &Action_PolyOmino::OnBnClickedPolyominoMeshFixvertices4)
	ON_BN_CLICKED(IDC_Polyomino_Mesh_SharpeVertex_Save, &Action_PolyOmino::OnBnClickedPolyominoMeshSharpevertexSave)
	ON_BN_CLICKED(IDC_Polyomino_Mesh_SharpeVertex_Load, &Action_PolyOmino::OnBnClickedPolyominoMeshSharpevertexLoad)
	ON_BN_CLICKED(IDC_BUTTON_SELECTLANDMARKS_TWOPOINTS, &Action_PolyOmino::OnBnClickedButtonSelectlandmarksTwopoints)
	ON_BN_CLICKED(IDC_BUTTON_SAVESELECTEDLANDMARK, &Action_PolyOmino::OnBnClickedButtonSaveselectedlandmark)
	ON_BN_CLICKED(IDC_BUTTON_LOADSELECTEDLANDMARK, &Action_PolyOmino::OnBnClickedButtonLoadselectedlandmark)
	ON_BN_CLICKED(IDC_BUTTON_AddLandmarks, &Action_PolyOmino::OnBnClickedButtonAddlandmarks)
	ON_BN_CLICKED(IDC_BUTTON_ONEPOINTLANDMARK, &Action_PolyOmino::OnBnClickedButtonOnepointlandmark)
	ON_BN_CLICKED(IDC_BUTTON_DETECT_OPENLANDMARK, &Action_PolyOmino::OnBnClickedButtonDetectOpenlandmark)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_DUPLICATE_LANDMARK, &Action_PolyOmino::OnBnClickedButtonDeleteDuplicateLandmark)
	ON_BN_CLICKED(IDC_BUTTON_SUBDIVIDESHARPEEDGE, &Action_PolyOmino::OnBnClickedButtonSubdividesharpeedge)
END_MESSAGE_MAP()


// Action_PolyOmino message handlers
extern char Post_Message[1000];
void Action_PolyOmino::OnBnClickedGeneratelandmarks()
{
	// TODO: Add your control notification handler code here
	GenerateLandMarks();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoRectification()
{
	// TODO: Add your control notification handler code here
	TJuctionRectification();
}

void Action_PolyOmino::OnBnClickedPolyominoMergeslope()
{
	// TODO: Add your control notification handler code here
	MergeOneEdgeWithSloping();
}

void Action_PolyOmino::OnBnClickedPolyominoDeletelandmark()
{
	// TODO: Add your control notification handler code here
	DeleteVertexConnectedLandmarks();
}

void Action_PolyOmino::OnBnClickedPolyominoMergesamestyle()
{
	// TODO: Add your control notification handler code here
	MergeConnectedLandMarkswithSameStyle();

	//
	SaveCurrentLandmak();
}

void Action_PolyOmino::OnBnClickedPolyominoThreeconnected()
{
	// TODO: Add your control notification handler code here
	MergeThreeConnectedLandmarks();
}

void Action_PolyOmino::OnBnClickedPolyominoMergesmall()
{
	// TODO: Add your control notification handler code here
	MergeSmallEdges();
}

void Action_PolyOmino::OnBnClickedPolyominoExportlandmark()
{
	// TODO: Add your control notification handler code here
	SaveGeoMap();
}

void Action_PolyOmino::OnBnClickedPolyominoExtendsmall()
{
	// TODO: Add your control notification handler code here
	ExtendSmall();
}
void Action_PolyOmino::GenerateLandMarks()
{
	pMesh->m_LandMarkArray.clear();

	//Find all the corners
	std::vector<int> m_CornerPointArray;
	m_CornerPointArray.resize(pMesh->m_ArrayVertex.GetSize(),0);

	for(int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int,int> data;
		data = pMesh->m_ArraySharpEdges[i];
		m_CornerPointArray[data.first]++; 
		m_CornerPointArray[data.second]++; 
	}
	std::vector<std::vector<int>> m_ArrayPointConnectedSelectEdgeIndex;
	m_ArrayPointConnectedSelectEdgeIndex.resize(pMesh->m_ArrayVertex.GetSize());
	for(int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int,int> data;
		data = pMesh->m_ArraySharpEdges[i];
		m_ArrayPointConnectedSelectEdgeIndex[data.first].push_back(i);
		m_ArrayPointConnectedSelectEdgeIndex[data.second].push_back(i);
	}

	std::vector<GeoMapLandMark*> m_ArrayLandMarks;
	std::vector<int> newlandmark;
	std::vector<int> m_edghandledflag;
	m_edghandledflag.resize(pMesh->m_ArraySharpEdges.size(),0);
	//while(1)
	{
		for(int i = 0; i < m_CornerPointArray.size(); i++)
		{
			if(m_CornerPointArray[i] >= 3 || m_CornerPointArray[i] == 1)
			{
				//m_CornerPointArray[i] = -3;
				for(int j = 0; j < m_ArrayPointConnectedSelectEdgeIndex[i].size(); j++)
				{
					newlandmark.clear();
					newlandmark.push_back(i);
					int edgeindex = m_ArrayPointConnectedSelectEdgeIndex[i][j];

					int prevertexindex = i;
					std::pair<int,int> data;
					if(m_edghandledflag[edgeindex] == 0)
					{
						while(1)
						{
							//handled edge
							m_edghandledflag[edgeindex] = 1;

							data = pMesh->m_ArraySharpEdges[edgeindex];
							if(data.first == prevertexindex)
							{
								newlandmark.push_back(data.second);
								prevertexindex = data.second;
							}else
							{
								newlandmark.push_back(data.first);
								prevertexindex = data.first;
							}
							if(m_CornerPointArray[prevertexindex] >= 3 || m_CornerPointArray[prevertexindex] == 1 )
								break;

							if(m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][0] == edgeindex)
								edgeindex = m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][1];
							else
								edgeindex = m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][0];
						}
						if (newlandmark.size() == 2)
						{
							int ij = 0;
						}
						GeoMapLandMark* pLandMark = new GeoMapLandMark();
						pLandMark->landmark_points = newlandmark;
						m_ArrayLandMarks.push_back(pLandMark);
					}

				}
			}
		}
	}

	//check whether has a loop landmark
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		if (m_edghandledflag[i] == 0)
		{
			int edgeindex = i;
			int prevertexindex = pMesh->m_ArraySharpEdges[edgeindex].second;
			newlandmark.clear();
			newlandmark.push_back(pMesh->m_ArraySharpEdges[edgeindex].second);
			std::pair<int, int> data;
			while (1)
			{
				//handled edge
				m_edghandledflag[edgeindex] = 1;

				data = pMesh->m_ArraySharpEdges[edgeindex];
				if (data.first == prevertexindex)
				{
					newlandmark.push_back(data.second);
					prevertexindex = data.second;
				}
				else
				{
					newlandmark.push_back(data.first);
					prevertexindex = data.first;
				}
				if (m_CornerPointArray[prevertexindex] >= 3 || m_CornerPointArray[prevertexindex] == 1)
					break;

				if (m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][0] == edgeindex)
					edgeindex = m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][1];
				else
					edgeindex = m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][0];
				//
				if (m_edghandledflag[edgeindex] == 1)
					break;
			}
			if (newlandmark.size() == 2)
			{
				int ij = 0;
			}
			GeoMapLandMark* pLandMark = new GeoMapLandMark();
			pLandMark->landmark_points = newlandmark;
			m_ArrayLandMarks.push_back(pLandMark);
		}
	}
	pMesh->m_LandMarkArray = m_ArrayLandMarks;

	//pMesh->m_ArrayLandMarks_Original = pMesh->m_LandMarkArray;
	sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
}
void Action_PolyOmino::InsertLandMark(std::vector<int> &landmarkpoints,int style)
{
	if(style == -1)
	{
		int start = landmarkpoints[0];
		int end = landmarkpoints[landmarkpoints.size() - 1];
		Vector3D line13vec(pMesh->m_ArrayVertex[start]->x() - pMesh->m_ArrayVertex[end]->x(),pMesh->m_ArrayVertex[start]->y() - pMesh->m_ArrayVertex[end]->y(),pMesh->m_ArrayVertex[start]->z() - pMesh->m_ArrayVertex[end]->z());
		line13vec.SetUnit();
		Vector3D yaxis(0,1,0);
		double angle = acos(line13vec * yaxis ) * 180/ 3.1415926;
		if(angle < 45 || angle > 135)
			style = 1;
		else
			style = 0;
	}

	GeoMapLandMark* pLandMark = new GeoMapLandMark();
	pLandMark->landmark_points = landmarkpoints;
	pLandMark->style = style;
	pMesh->m_LandMarkArray.push_back(pLandMark);
}
void Action_PolyOmino::SplitLandmark()
{
	if(m_SplitLandmarkFirstRun)
	{
		pMesh->m_ArrayLandMarks_Original = pMesh->m_LandMarkArray;
	}
	m_SplitLandmarkFirstRun = FALSE;
	//return;
	//Split the landmarks
	std::vector<GeoMapLandMark*> m_ArrayLandMarks = pMesh->m_ArrayLandMarks_Original;
	std::vector<int> newlandmark;

	pMesh->m_LandMarkArray.clear();

	GeoMapLandMark* pLandMark;
	std::vector<int> landmark;
	pMesh->m_ArrayPointConnectedLandmarks.resize(pMesh->m_ArrayVertex.GetSize());
	//std::vector<int> newlandmark;
	int sign1,sign2;
	//int inflectionpoint;
	for(int i = 0; i < m_ArrayLandMarks.size(); i++)
	{
		//inflectionpoint = 0;
		pLandMark = m_ArrayLandMarks[i];
		landmark = pLandMark->landmark_points;
		newlandmark.clear();
		newlandmark.push_back(pLandMark->landmark_points[0]);
		Vector3D yaxis(0,1,0);

		Vector3D vv1(pMesh->m_ArrayVertex[landmark[0]]->x(),pMesh->m_ArrayVertex[landmark[0]]->y(),pMesh->m_ArrayVertex[landmark[0]]->z());
		Vector3D vv2(pMesh->m_ArrayVertex[landmark[1]]->x(),pMesh->m_ArrayVertex[landmark[1]]->y(),pMesh->m_ArrayVertex[landmark[1]]->z());
		Vector3D v1 = vv2 - vv1;
		double angle1 = acos(v1 * yaxis / Length(v1)) * 180/ 3.1415926;
		if(angle1 < 45 || angle1 >135)
			sign1 = 1;
		else 
			sign1 = 0;

		for(int j = 1; j < landmark.size() - 1 ; j++)
		{
			newlandmark.push_back(landmark[j]);
			int index = landmark[j];
			int preindex = landmark[j - 1];
			int nextindex = landmark[j + 1];
			Vector3D prev(pMesh->m_ArrayVertex[preindex]->x(),pMesh->m_ArrayVertex[preindex]->y(),pMesh->m_ArrayVertex[preindex]->z());
			Vector3D v(pMesh->m_ArrayVertex[index]->x(),pMesh->m_ArrayVertex[index]->y(),pMesh->m_ArrayVertex[index]->z());
			Vector3D nextv(pMesh->m_ArrayVertex[nextindex]->x(),pMesh->m_ArrayVertex[nextindex]->y(),pMesh->m_ArrayVertex[nextindex]->z());
			v1 = v - prev;
			Vector3D v2 = nextv - v;
			double val = v1 * v2 / (Length(v1) * Length(v2));
			double angle_ = acos(val) * 180/ 3.1415926;

			double angle2 = acos(v2 * yaxis / Length(v2)) * 180/ 3.1415926;
			if(angle2 < 45 || angle2 >135)
				sign2 = 1;
			else 
				sign2 = 0;
			//if(angle_ > 90 || sign2 != sign1)
			//if(sign2 != sign1 && m_bSplitDirection) 
			if(angle_ > m_SplitAngle || sign2 != sign1 && m_bSplitDirection) 
			{
				InsertLandMark(newlandmark,sign1);
				//pMesh->m_LandMarkArray.push_back(newlandmark);
				//pMesh->m_LandMarkArray_style.push_back(sign1);

				//case 
				if(angle_ > 90 && sign2 == sign1)
				{
					sign1 = 1 - sign2;
					//inflectionpoint = 1;

				}
				//if(angle_ > 45 && sign2 == sign1)

				else
					sign1 = sign2;

				newlandmark.clear();
				newlandmark.push_back(landmark[j]);
			}
		}
		newlandmark.push_back(landmark[landmark.size() - 1]);
		InsertLandMark(newlandmark,sign1);
		//pMesh->m_LandMarkArray.push_back(newlandmark);
		//pMesh->m_LandMarkArray_style.push_back(sign1);
	}
	sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::SaveSelectedGeoMap()
{
	CString s1 = "D:\\papers\\code\\bin_Curve\\lamcc_dir_div_merge.m";
	//CString s1 = "D:\\papers\\code\\bin_Curve\\USA_33.m";

	std::ofstream file(s1);
	std::ifstream *texturefile = NULL;
	COLORREF color;
	double r, g, b;

	//CImage m_TextureImage; 
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->m_Flag = i + 1;

		file << "Vertex " << i + 1 << " " << pVertex->m_Coord[0] << " " << pVertex->m_Coord[1] << " " << pVertex->m_Coord[2] << "\n";

		//file << " {";
		/*if(hasvt)
		{
		color = m_TextureImage.GetPixel(m_TextureImage.GetWidth() * pVertex->u,m_TextureImage.GetHeight() * (1 - pVertex->v));

		b = ((color & 0xff0000) >> 16);
		g = ((color & 0x00ff00) >> 8);
		r = (color & 0x0000ff);

		file << "rgb=("<<r/255.0<<" "<<g/255.0<<" "<<b/255.0<<") ";*/
		//file << "uv=(" << pVertex->u << " " << pVertex->v << ") ";
		//file << "uv=(" << pVertex->m_Coord[0] << " " << pVertex->m_Coord[1] << ") ";
		//}
		if (0)
		{
			file << "normal=(" << pVertex->GetNormal()->x() << " " << pVertex->GetNormal()->y() << " " << pVertex->GetNormal()->z() << ") ";
		}
		//file << "}\n";
	}
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace3d = pMesh->GetFace(i);
		file << "Face " << i + 1 << " " << pFace3d->v1()->m_Flag << " " << pFace3d->v2()->m_Flag << " " << pFace3d->v3()->m_Flag << "\n";
	}

	int landmarkindex;
	for (int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
	{
		landmarkindex = pMesh->m_SelectedLandmarkIndexArray[i];

		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[landmarkindex];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			CVertex3d* pVertex1 = pMesh->GetVertex(pMesh->m_LandMarkArray[i]->landmark_points[j]);
			CVertex3d* pVertex2 = pMesh->GetVertex(pMesh->m_LandMarkArray[i]->landmark_points[j + 1]);
			file << "Edge " << " " << pVertex1->m_Flag << " " << pVertex2->m_Flag << " {sharp landmark=(" << i << ") dir=(" << pMesh->m_LandMarkArray[i]->style << ")}" << "\n";
		}
	}
	file<< "Edge 1 540550 {sharp}" << "\n";
	file<< "Edge 1 6483 {sharp}" << "\n";
	file<< "Edge 1 540554 {sharp}" << "\n";

	file << "Edge 2 539981 {sharp}" << "\n";
	file << "Edge 2 536151 {sharp}" << "\n";
	file << "Edge 2 539825 {sharp}" << "\n";

	//file<< "Edge 1 52488 {sharp}" << "\n";
	//file<< "Edge 4 52427 {sharp}" << "\n";
	//file<< "Edge 2 52416 {sharp}" << "\n";
	//file<< "Edge 3 52257 {sharp}" << "\n";

	//file<< "Edge 2 73125 {sharp}" << "\n";
	//file<< "Edge 3 73208 {sharp}" << "\n";	
	//file<< "Edge 1 72728 {sharp}" << "\n";   
	//file<< "Edge 4 69663 {sharp}" << "\n";	

	//file<< "Edge 2 255933 {sharp}" << "\n";
	//file<< "Edge 4 255978 {sharp}" << "\n";	
	//file<< "Edge 1 255990 {sharp}" << "\n";   
	//file<< "Edge 3 255998 {sharp}" << "\n";	

	//file<< "Edge 2 20043 {sharp}" << "\n";
	//file<< "Edge 4 20005 {sharp}" << "\n";	
	//file<< "Edge 1 20058 {sharp}" << "\n";   
	//file<< "Edge 3 20078 {sharp}" << "\n";
	//file<< "Edge 2 54777 {sharp}" << "\n";
	//file<< "Edge 4 55601 {sharp}" << "\n";	
	//file<< "Edge 1 56049 {sharp}" << "\n";   
	//file<< "Edge 3 55573 {sharp}" << "\n";

	//file<< "Edge 2 7276 {sharp}" << "\n";
	//file<< "Edge 4 7296 {sharp}" << "\n";	
	//file<< "Edge 1 7302 {sharp}" << "\n";   
	//file<< "Edge 3 7227 {sharp}" << "\n";

	//file<< "Edge 2 82789 {sharp}" << "\n";
	//file<< "Edge 4 82802 {sharp}" << "\n";	
	//file<< "Edge 1 82800 {sharp}" << "\n";   
	//file<< "Edge 3 82849 {sharp}" << "\n";	

	//file<< "Edge 2 91252 {sharp}" << "\n";
	//file<< "Edge 4 91294 {sharp}" << "\n";	
	//file<< "Edge 1 91307 {sharp}" << "\n";   
	//file<< "Edge 3 91318 {sharp}" << "\n";	

	//file<< "Edge 1 122074 {sharp}" << "\n";
	//file<< "Edge 2 110694 {sharp}" << "\n";
	//file<< "Edge 3 102055 {sharp}" << "\n";
	//file<< "Edge 4 121829 {sharp}" << "\n";
	file.close();
}
void Action_PolyOmino::SaveGeoMap()
{
	CString s1 = "D:\\papers\\code\\bin_Curve\\lamcc_dir_div_merge.m"; 
	//CString s1 = "D:\\papers\\code\\bin_Curve\\USA_33.m";

	std::ofstream file(s1);
	std::ifstream *texturefile = NULL;
	COLORREF color;
	double r,g,b;

	//CImage m_TextureImage; 
	for(int i  = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->m_Flag = i + 1;

		file << "Vertex " << i + 1 << " " << pVertex->m_Coord[0] << " " << pVertex->m_Coord[1] << " " << pVertex->m_Coord[2] << "\n";

		//file << " {";
		/*if(hasvt)
		{
			color = m_TextureImage.GetPixel(m_TextureImage.GetWidth() * pVertex->u,m_TextureImage.GetHeight() * (1 - pVertex->v));

			b = ((color & 0xff0000) >> 16);
			g = ((color & 0x00ff00) >> 8);
			r = (color & 0x0000ff);

			file << "rgb=("<<r/255.0<<" "<<g/255.0<<" "<<b/255.0<<") ";*/
			//file << "uv=(" << pVertex->u << " " << pVertex->v << ") ";
			//file << "uv=(" << pVertex->m_Coord[0] << " " << pVertex->m_Coord[1] << ") ";
		//}
		if(0)
		{
			file << "normal=(" << pVertex->GetNormal()->x() << " "<< pVertex->GetNormal()->y() << " " << pVertex->GetNormal()->z()<<") ";
		}
		//file << "}\n";
	}
	for(int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace3d = pMesh->GetFace(i);
		file << "Face " << i + 1 <<" " << pFace3d->v1()->m_Flag << " " << pFace3d->v2()->m_Flag << " " << pFace3d->v3()->m_Flag << "\n";
	}

	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		for(int j = 0; j < pLandMark->landmark_points.size() - 1;j++)
		{
			CVertex3d* pVertex1 = pMesh->GetVertex(pMesh->m_LandMarkArray[i]->landmark_points[j]);
			CVertex3d* pVertex2 = pMesh->GetVertex(pMesh->m_LandMarkArray[i]->landmark_points[j + 1]);
			file<< "Edge " << " " << pVertex1->m_Flag << " " << pVertex2->m_Flag << " {sharp landmark=("<<i<<") dir=("<<pMesh->m_LandMarkArray[i]->style<<")}" << "\n";
		}
	}
	//file<< "Edge 1 52488 {sharp}" << "\n";
	//file<< "Edge 4 52427 {sharp}" << "\n";
	//file<< "Edge 2 52416 {sharp}" << "\n";
	//file<< "Edge 3 52257 {sharp}" << "\n";

	//file<< "Edge 2 73125 {sharp}" << "\n";
	//file<< "Edge 3 73208 {sharp}" << "\n";	
 	//file<< "Edge 1 72728 {sharp}" << "\n";   
 	//file<< "Edge 4 69663 {sharp}" << "\n";	
	
	//file<< "Edge 2 255933 {sharp}" << "\n";
	//file<< "Edge 4 255978 {sharp}" << "\n";	
	//file<< "Edge 1 255990 {sharp}" << "\n";   
	//file<< "Edge 3 255998 {sharp}" << "\n";	

	//file<< "Edge 2 20043 {sharp}" << "\n";
	//file<< "Edge 4 20005 {sharp}" << "\n";	
	//file<< "Edge 1 20058 {sharp}" << "\n";   
	//file<< "Edge 3 20078 {sharp}" << "\n";
	//file<< "Edge 2 54777 {sharp}" << "\n";
	//file<< "Edge 4 55601 {sharp}" << "\n";	
	//file<< "Edge 1 56049 {sharp}" << "\n";   
	//file<< "Edge 3 55573 {sharp}" << "\n";

	//file<< "Edge 2 7276 {sharp}" << "\n";
	//file<< "Edge 4 7296 {sharp}" << "\n";	
	//file<< "Edge 1 7302 {sharp}" << "\n";   
	//file<< "Edge 3 7227 {sharp}" << "\n";

	//file<< "Edge 2 82789 {sharp}" << "\n";
	//file<< "Edge 4 82802 {sharp}" << "\n";	
	//file<< "Edge 1 82800 {sharp}" << "\n";   
	//file<< "Edge 3 82849 {sharp}" << "\n";	

	//file<< "Edge 2 91252 {sharp}" << "\n";
	//file<< "Edge 4 91294 {sharp}" << "\n";	
	//file<< "Edge 1 91307 {sharp}" << "\n";   
	//file<< "Edge 3 91318 {sharp}" << "\n";	

	//file<< "Edge 1 122074 {sharp}" << "\n";
	//file<< "Edge 2 110694 {sharp}" << "\n";
	//file<< "Edge 3 102055 {sharp}" << "\n";
	//file<< "Edge 4 121829 {sharp}" << "\n";
	file.close();
}


void Action_PolyOmino::Sloppingedgesubdivision()
{
	if(m_SloppingedgesubdivisionFirstRun)
	{
		pMesh->m_ArrayLandMarks_Original = pMesh->m_LandMarkArray;
	}
	m_SloppingedgesubdivisionFirstRun = FALSE;

	pMesh->m_LandMarkArray = pMesh->m_ArrayLandMarks_Original;
	//pMesh->m_LandMarkArray_style = m_newlandmarks_style_Original;

	int index1,index2;
	//index1 = 4696;
	//index2 = 4695;
	//index1 = 6780;
	//index2 = 20564;
	//index1 = 11605;
	//index2 = 38745;
 
	//index1 = 11605;
	//index2 = 24893;
	//index1 = 692;
	//index2 = 693;
	//index1 = 3003;
	//index2 = 3004;
	index1 = 360;
	index2 = 344;
	Vector3D vv1(pMesh->m_ArrayVertex[index1]->x() - pMesh->m_ArrayVertex[index2]->x(),pMesh->m_ArrayVertex[index1]->y() - pMesh->m_ArrayVertex[index2]->y(),pMesh->m_ArrayVertex[index1]->z() - pMesh->m_ArrayVertex[index2]->z());
	double minedgelength_tol = Length(vv1)/2;

	//Merge one edge
	std::vector<int> m_ArrayDeleteFlag;
	m_ArrayDeleteFlag.resize(pMesh->m_LandMarkArray.size(),0);
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		if(i < m_ArrayDeleteFlag.size())
			if(m_ArrayDeleteFlag[i] == 1)
				continue;

		if(i >= m_ArrayDeleteFlag.size())
			continue;
		int startvindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
		int endvindex = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
		Vector3D vv1(pMesh->m_ArrayVertex[startvindex]->x(),pMesh->m_ArrayVertex[startvindex]->y(),pMesh->m_ArrayVertex[startvindex]->z());
		Vector3D vv2(pMesh->m_ArrayVertex[endvindex]->x(),pMesh->m_ArrayVertex[endvindex]->y(),pMesh->m_ArrayVertex[endvindex]->z());

		Vector3D dir = vv2 - vv1;
		dir.SetUnit();
		double angle_ = acos(fabs(dir[0])) * 180 / 3.1415926;
		double len = Length(vv1 - vv2);
		if(len > minedgelength_tol && angle_ > 30 && angle_ < 60)
		{
			//int segments = (int)(len / minedgelength_tol);
			//if(segments % 2 == 0)
			//	segments++;
			int segments = m_EdgeSubdivisionNum * 2 - 1;
			double edgenum = pMesh->m_LandMarkArray[i]->landmark_points.size() / (double)segments;
			double landmarklength = GetLandMarkLength(pMesh->m_LandMarkArray,i);
			double seglength = landmarklength / segments;
			std::vector<int> newlandmark_;
			int index = 0;
			double edgelength = 0;
			int style = pMesh->m_LandMarkArray[i]->style;

			int segnum = 0;
			newlandmark_.push_back(pMesh->m_LandMarkArray[i]->landmark_points[0]);
			for(int jj = 1; jj < pMesh->m_LandMarkArray[i]->landmark_points.size(); jj++)
			{
				newlandmark_.push_back(pMesh->m_LandMarkArray[i]->landmark_points[jj]);
				int currentindex = pMesh->m_LandMarkArray[i]->landmark_points[jj];
				int preindex = pMesh->m_LandMarkArray[i]->landmark_points[jj - 1];
				Vector3D prevv(pMesh->m_ArrayVertex[currentindex]->x(),pMesh->m_ArrayVertex[currentindex]->y(),pMesh->m_ArrayVertex[currentindex]->z());
				Vector3D currentvv(pMesh->m_ArrayVertex[preindex]->x(),pMesh->m_ArrayVertex[preindex]->y(),pMesh->m_ArrayVertex[preindex]->z());
				edgelength += Length(currentvv - prevv);	
				index++;
				if(index >= edgenum && segnum < segments - 1 || jj == pMesh->m_LandMarkArray[i]->landmark_points.size() - 1)
					//if(edgelength >= seglength && segnum < segments - 1 || jj == pMesh->m_LandMarkArray[i].size() - 1)
				{
					InsertLandMark(newlandmark_,style);
					pMesh->m_LandMarkArray[pMesh->m_LandMarkArray.size() - 1]->fixedflag = 1;
					//pMesh->m_LandMarkArray.push_back(newlandmark_);
					//pMesh->m_LandMarkArray_style.push_back(style);
					style = 1 - style;
					newlandmark_.clear();
					newlandmark_.push_back(pMesh->m_LandMarkArray[i]->landmark_points[jj]);
					index = 1;
					edgelength = 0;
					segnum++;
				}
			}
			m_ArrayDeleteFlag[i] = 1;
		}
	}
	for(int ii = m_ArrayDeleteFlag.size() - 1; ii >= 0; ii--)
	{
		if(m_ArrayDeleteFlag[ii])
		{
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + ii);
			//pMesh->m_LandMarkArray_style.erase(pMesh->m_LandMarkArray_style.begin() + ii);
		}
	}
	//pMesh->m_ArrayLandMarks_Original = pMesh->m_LandMarkArray;
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::MergeOneEdgeWithSloping()
{
	if(m_MergeOneEdgeWithSlopingFirstRun)
	{
		pMesh->m_ArrayLandMarks_Original = pMesh->m_LandMarkArray;
	}
	m_MergeOneEdgeWithSlopingFirstRun = FALSE;

	pMesh->m_LandMarkArray = pMesh->m_ArrayLandMarks_Original;
	//pMesh->m_LandMarkArray_style = m_newlandmarks_style_Original;

	//Find all the corners
	std::vector<int> m_CornerPointArray;
	m_CornerPointArray.resize(pMesh->m_ArrayVertex.GetSize(),0);
	for(int i = 0; i < pMesh->m_ArrayPointConnectedLandmarks.size(); i++)
	{
		pMesh->m_ArrayPointConnectedLandmarks[i].clear();
	}

	for(int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int,int> data;
		data = pMesh->m_ArraySharpEdges[i];
		m_CornerPointArray[data.first]++; 
		m_CornerPointArray[data.second]++; 
	}
	std::vector<std::vector<int>> m_ArrayPointConnectedSelectEdgeIndex;
	m_ArrayPointConnectedSelectEdgeIndex.resize(pMesh->m_ArrayVertex.GetSize());
	for(int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int,int> data;
		data = pMesh->m_ArraySharpEdges[i];
		m_ArrayPointConnectedSelectEdgeIndex[data.first].push_back(i);
		m_ArrayPointConnectedSelectEdgeIndex[data.second].push_back(i);
	}
	int index1,index2;
	//index1 = 4695;
	//index2 = 4696;

	//index1 = 11605;
	//index2 = 38745;
	//index1 = 11605;
	//index2 = 24893;
	index1 = 701;
	index2 = 4719;
	Vector3D vv1(pMesh->m_ArrayVertex[index1]->x() - pMesh->m_ArrayVertex[index2]->x(),pMesh->m_ArrayVertex[index1]->y() - pMesh->m_ArrayVertex[index2]->y(),pMesh->m_ArrayVertex[index1]->z() - pMesh->m_ArrayVertex[index2]->z());
	double minedgelength_tol = Length(vv1) ;

	//Merge one edge
	std::vector<int> m_ArrayDeleteFlag;
	m_ArrayDeleteFlag.resize(pMesh->m_LandMarkArray.size(),0);
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		if(pMesh->m_LandMarkArray[i]->fixedflag)
			continue;
		if(i < m_ArrayDeleteFlag.size())
			if(m_ArrayDeleteFlag[i] == 1)
				continue;
		if(pMesh->m_LandMarkArray[i]->landmark_points.size() == 2)
		{
			int startvindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
			int endvindex = pMesh->m_LandMarkArray[i]->landmark_points[1];

			if(pMesh->m_ArrayPointConnectedLandmarks[startvindex].size() == 2 && pMesh->m_ArrayPointConnectedLandmarks[endvindex].size() == 2)
			{
				int preedgeindex = pMesh->m_ArrayPointConnectedLandmarks[startvindex][0];
				if(preedgeindex == i)
					preedgeindex = pMesh->m_ArrayPointConnectedLandmarks[startvindex][1];
				int nextedgeindex = pMesh->m_ArrayPointConnectedLandmarks[endvindex][0];
				if(nextedgeindex == i)
					nextedgeindex = pMesh->m_ArrayPointConnectedLandmarks[endvindex][1];

				//if(m_ArrayDeleteFlag[preedgeindex] == 1 || )
				int prestartvindex = pMesh->m_LandMarkArray[preedgeindex]->landmark_points[0];
				int preendvindex;
				if(prestartvindex != startvindex)
				{
					prestartvindex = pMesh->m_LandMarkArray[preedgeindex]->landmark_points[pMesh->m_LandMarkArray[preedgeindex]->landmark_points.size() - 1];
					preendvindex = pMesh->m_LandMarkArray[preedgeindex]->landmark_points[pMesh->m_LandMarkArray[preedgeindex]->landmark_points.size() - 2];
				}else
				{
					prestartvindex = pMesh->m_LandMarkArray[preedgeindex]->landmark_points[0];
					preendvindex = pMesh->m_LandMarkArray[preedgeindex]->landmark_points[1];
				}

				int nextstartvindex = pMesh->m_LandMarkArray[nextedgeindex]->landmark_points[0];
				int nextendvindex;
				if(nextstartvindex != endvindex)
				{
					nextstartvindex = pMesh->m_LandMarkArray[nextedgeindex]->landmark_points[pMesh->m_LandMarkArray[nextedgeindex]->landmark_points.size() - 1];
					nextendvindex = pMesh->m_LandMarkArray[nextedgeindex]->landmark_points[pMesh->m_LandMarkArray[nextedgeindex]->landmark_points.size() - 2];
				}else
				{
					nextstartvindex = pMesh->m_LandMarkArray[nextedgeindex]->landmark_points[0];
					nextendvindex = pMesh->m_LandMarkArray[nextedgeindex]->landmark_points[1];
				}
				Vector3D currentedge(pMesh->m_ArrayVertex[endvindex]->x() - pMesh->m_ArrayVertex[startvindex]->x(),pMesh->m_ArrayVertex[endvindex]->y() - pMesh->m_ArrayVertex[startvindex]->y(),pMesh->m_ArrayVertex[endvindex]->z() - pMesh->m_ArrayVertex[startvindex]->z());
				Vector3D preedge(pMesh->m_ArrayVertex[preendvindex]->x() - pMesh->m_ArrayVertex[prestartvindex]->x(),pMesh->m_ArrayVertex[preendvindex]->y() - pMesh->m_ArrayVertex[prestartvindex]->y(),pMesh->m_ArrayVertex[preendvindex]->z() - pMesh->m_ArrayVertex[prestartvindex]->z());
				Vector3D nextedge(pMesh->m_ArrayVertex[nextendvindex]->x() - pMesh->m_ArrayVertex[nextstartvindex]->x(),pMesh->m_ArrayVertex[nextendvindex]->y() - pMesh->m_ArrayVertex[nextstartvindex]->y(),pMesh->m_ArrayVertex[nextendvindex]->z() - pMesh->m_ArrayVertex[nextstartvindex]->z());

				double edgelength = Length(currentedge);
				currentedge.SetUnit();
				preedge.SetUnit();
				nextedge.SetUnit();

				double angle1 = acos(fabs(currentedge * preedge)) * 180 / 3.1415926;
				double angle2 = acos(fabs(currentedge * nextedge)) * 180 / 3.1415926;

				//if(angle1 < 45 && angle2 < 45 || edgelength <= minedgelength_tol * 0.8)
				if(angle1 < m_MergeAngle && angle2 < m_MergeAngle)
				{
					//Merge
					std::vector<int> newlandmark_;
					GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
					newlandmark_ = pLandMark->landmark_points;

					if(pMesh->m_LandMarkArray[preedgeindex]->deleteflag == 0)
					//if(m_ArrayDeleteFlag[preedgeindex] == 0)
						if(pMesh->m_LandMarkArray[preedgeindex]->landmark_points[0] == startvindex)
						//if(pMesh->m_LandMarkArray[preedgeindex][0] == startvindex)
						{
							for(int ii = 1; ii < pMesh->m_LandMarkArray[preedgeindex]->landmark_points.size(); ii++)
							{
								newlandmark_.insert(newlandmark_.begin(),pMesh->m_LandMarkArray[preedgeindex]->landmark_points[ii]);
							}
						}else
						{
							for(int ii = pMesh->m_LandMarkArray[preedgeindex]->landmark_points.size() - 2; ii >= 0 ; ii--)
							{
								newlandmark_.insert(newlandmark_.begin(),pMesh->m_LandMarkArray[preedgeindex]->landmark_points[ii]);
							}
						}
						
						if(pMesh->m_LandMarkArray[nextedgeindex]->deleteflag == 0)
						//if(m_ArrayDeleteFlag[nextedgeindex] == 0)
							if(pMesh->m_LandMarkArray[nextedgeindex]->landmark_points[0] == endvindex)
							{
								for(int ii = 1; ii < pMesh->m_LandMarkArray[nextedgeindex]->landmark_points.size(); ii++)
								{
									newlandmark_.push_back(pMesh->m_LandMarkArray[nextedgeindex]->landmark_points[ii]);
								}
							}else
							{
								for(int ii = pMesh->m_LandMarkArray[nextedgeindex]->landmark_points.size() - 2; ii >= 0 ; ii--)
								{
									newlandmark_.push_back(pMesh->m_LandMarkArray[nextedgeindex]->landmark_points[ii]);
								}
							}
							int style = pMesh->m_LandMarkArray[preedgeindex]->style;
							//int style = pMesh->m_LandMarkArray_style[preedgeindex];
							InsertLandMark(newlandmark_,style);
							//pMesh->m_LandMarkArray.push_back(newlandmark_);
							//pMesh->m_LandMarkArray_style.push_back(style);
							m_ArrayDeleteFlag[preedgeindex] = 1;
							m_ArrayDeleteFlag[i] = 1;
							m_ArrayDeleteFlag[nextedgeindex] = 1;
							//delete
							//std::vector<int> m_deleteitems;
							//m_deleteitems.push_back(nextedgeindex);
							//m_deleteitems.push_back(i);
							//m_deleteitems.push_back(preedgeindex);

							//std::sort(m_deleteitems.begin(),m_deleteitems.end());
							//m_newlandmarks.erase(m_newlandmarks.begin() + m_deleteitems[2]);
							//m_newlandmarks.erase(m_newlandmarks.begin() + m_deleteitems[1]);
							//m_newlandmarks.erase(m_newlandmarks.begin() + m_deleteitems[0]);


							//m_newlandmarks_style.erase(m_newlandmarks_style.begin() + m_deleteitems[2]);
							//m_newlandmarks_style.erase(m_newlandmarks_style.begin() + m_deleteitems[1]);
							//m_newlandmarks_style.erase(m_newlandmarks_style.begin() + m_deleteitems[0]);

							//i = 0;
				}
			}
		}
	}
	for(int ii = m_ArrayDeleteFlag.size() - 1; ii >= 0; ii--)
	{
		if(m_ArrayDeleteFlag[ii])
		{
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + ii);
			//pMesh->m_LandMarkArray_style.erase(pMesh->m_LandMarkArray_style.begin() + ii);
		}
	}
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::TJuctionRectification()
{
	pMesh->m_ArrayPointConnectedLandmarks.resize(pMesh->m_ArrayVertex.GetSize());
	for(int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		pMesh->m_ArrayPointConnectedLandmarks[i].clear();
	}
	//Modify the direction At T-Junction
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		int start = pMesh->m_LandMarkArray[i]->landmark_points[0];
		int end = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
		pMesh->m_ArrayPointConnectedLandmarks[start].push_back(i);
		pMesh->m_ArrayPointConnectedLandmarks[end].push_back(i);
	}

	for(int i = 0; i < pMesh->m_ArrayPointConnectedLandmarks.size(); i++)
	{
		int size1 = pMesh->m_ArrayPointConnectedLandmarks[i].size();
		if (size1 == 3)
		{
			//T-Junction
			int index1 = pMesh->m_ArrayPointConnectedLandmarks[i][0];
			int index2 = pMesh->m_ArrayPointConnectedLandmarks[i][1];
			int index3 = pMesh->m_ArrayPointConnectedLandmarks[i][2];

			//
			int line1start = pMesh->m_LandMarkArray[index1]->landmark_points[0];
			int line1end = pMesh->m_LandMarkArray[index1]->landmark_points[pMesh->m_LandMarkArray[index1]->landmark_points.size() / 2];
			if(line1start == line1end)
				line1end = pMesh->m_LandMarkArray[index1]->landmark_points[pMesh->m_LandMarkArray[index1]->landmark_points.size() - 1];
			if(line1start != i)
			{
				line1start = pMesh->m_LandMarkArray[index1]->landmark_points[pMesh->m_LandMarkArray[index1]->landmark_points.size() - 1];
				line1end = pMesh->m_LandMarkArray[index1]->landmark_points[pMesh->m_LandMarkArray[index1]->landmark_points.size() / 2];
				if(line1start == line1end)
					line1end = pMesh->m_LandMarkArray[index1]->landmark_points[0];
			}

			int line2start = pMesh->m_LandMarkArray[index2]->landmark_points[0];
			int line2end = pMesh->m_LandMarkArray[index2]->landmark_points[pMesh->m_LandMarkArray[index2]->landmark_points.size() / 2];
			if(line2start == line2end)
				line2end = pMesh->m_LandMarkArray[index2]->landmark_points[pMesh->m_LandMarkArray[index2]->landmark_points.size() - 1];
			if(line2start != i)
			{
				line2start = pMesh->m_LandMarkArray[index2]->landmark_points[pMesh->m_LandMarkArray[index2]->landmark_points.size() - 1];
				line2end = pMesh->m_LandMarkArray[index2]->landmark_points[pMesh->m_LandMarkArray[index2]->landmark_points.size() / 2];
				if(line2start == line2end)
					line2end = pMesh->m_LandMarkArray[index2]->landmark_points[0];
			}

			int line3start = pMesh->m_LandMarkArray[index3]->landmark_points[0];
			int line3end = pMesh->m_LandMarkArray[index3]->landmark_points[pMesh->m_LandMarkArray[index3]->landmark_points.size() / 2];
			if(line3start == line3end)
				line3end = pMesh->m_LandMarkArray[index3]->landmark_points[pMesh->m_LandMarkArray[index3]->landmark_points.size() - 1];
			if(line3start != i)
			{
				line3start = pMesh->m_LandMarkArray[index3]->landmark_points[pMesh->m_LandMarkArray[index3]->landmark_points.size() - 1];
				line3end = pMesh->m_LandMarkArray[index3]->landmark_points[pMesh->m_LandMarkArray[index3]->landmark_points.size() / 2];
				if(line3start == line3end)
					line3end = pMesh->m_LandMarkArray[index3]->landmark_points[0];
			}
			Vector3D line1vec(pMesh->m_ArrayVertex[line1end]->x() - pMesh->m_ArrayVertex[line1start]->x(),pMesh->m_ArrayVertex[line1end]->y() - pMesh->m_ArrayVertex[line1start]->y(),pMesh->m_ArrayVertex[line1end]->z() - pMesh->m_ArrayVertex[line1start]->z());
			Vector3D line2vec(pMesh->m_ArrayVertex[line2end]->x() - pMesh->m_ArrayVertex[line2start]->x(),pMesh->m_ArrayVertex[line2end]->y() - pMesh->m_ArrayVertex[line2start]->y(),pMesh->m_ArrayVertex[line2end]->z() - pMesh->m_ArrayVertex[line2start]->z());
			Vector3D line3vec(pMesh->m_ArrayVertex[line3end]->x() - pMesh->m_ArrayVertex[line3start]->x(),pMesh->m_ArrayVertex[line3end]->y() - pMesh->m_ArrayVertex[line3start]->y(),pMesh->m_ArrayVertex[line3end]->z() - pMesh->m_ArrayVertex[line3start]->z());

			line1vec.SetUnit();
			line2vec.SetUnit();
			line3vec.SetUnit();

			double angle12= acos(line1vec * line2vec) * 180 / 3.1415926;
			double angle23= acos(line2vec * line3vec) * 180 / 3.1415926;
			double angle13= acos(line1vec * line3vec) * 180 / 3.1415926;

			Vector3D yaxis(0,1,0);
			if(angle12 >= angle23 && angle12 >= angle13)
			{
				Vector3D line12vec(pMesh->m_ArrayVertex[line1end]->x() - pMesh->m_ArrayVertex[line2end]->x(),pMesh->m_ArrayVertex[line1end]->y() - pMesh->m_ArrayVertex[line2end]->y(),pMesh->m_ArrayVertex[line1end]->z() - pMesh->m_ArrayVertex[line2end]->z());
				line12vec.SetUnit();

				double angle2 = acos(line12vec * yaxis ) * 180/ 3.1415926;
				double angle3 = acos(line3vec * yaxis ) * 180/ 3.1415926;
				if(fabs(angle2 - 90) < fabs(angle3 - 90))
				{
					//line12 xaxis, line3 yaxis
					pMesh->m_LandMarkArray[index1]->style = 0;
					pMesh->m_LandMarkArray[index2]->style = 0;
					pMesh->m_LandMarkArray[index3]->style = 1;
					//pMesh->m_LandMarkArray_style[index1] = 0;
					//pMesh->m_LandMarkArray_style[index2] = 0;
					//pMesh->m_LandMarkArray_style[index3] = 1;
				}else
				{
					//line3 xaxis, line12 yaxis
					pMesh->m_LandMarkArray[index1]->style = 1;
					pMesh->m_LandMarkArray[index2]->style = 1;
					pMesh->m_LandMarkArray[index3]->style = 0;
					//pMesh->m_LandMarkArray_style[index1] = 1;
					//pMesh->m_LandMarkArray_style[index2] = 1;
					//pMesh->m_LandMarkArray_style[index3] = 0;
				}
			}else
				if(angle23 >= angle12 && angle23 >= angle13)
				{
					Vector3D line23vec(pMesh->m_ArrayVertex[line3end]->x() - pMesh->m_ArrayVertex[line2end]->x(),pMesh->m_ArrayVertex[line3end]->y() - pMesh->m_ArrayVertex[line2end]->y(),pMesh->m_ArrayVertex[line3end]->z() - pMesh->m_ArrayVertex[line2end]->z());
					line23vec.SetUnit();

					double angle2 = acos(line23vec * yaxis ) * 180/ 3.1415926;
					double angle3 = acos(line1vec * yaxis ) * 180/ 3.1415926;
					if(fabs(angle2 - 90) < fabs(angle3 - 90))
					{
						//line23 xaxis, line1 yaxis
						pMesh->m_LandMarkArray[index1]->style = 1;
						pMesh->m_LandMarkArray[index2]->style = 0;
						pMesh->m_LandMarkArray[index3]->style = 0;
						//pMesh->m_LandMarkArray_style[index1] = 1;
						//pMesh->m_LandMarkArray_style[index2] = 0;
						//pMesh->m_LandMarkArray_style[index3] = 0;
					}else
					{
						//line1 xaxis, line23 yaxis
						pMesh->m_LandMarkArray[index1]->style = 0;
						pMesh->m_LandMarkArray[index2]->style = 1;
						pMesh->m_LandMarkArray[index3]->style = 1;
						//pMesh->m_LandMarkArray_style[index1] = 0;
						//pMesh->m_LandMarkArray_style[index2] = 1;
						//pMesh->m_LandMarkArray_style[index3] = 1;
					}
				}else
					if(angle13 >= angle12 && angle13 >= angle23)
					{
						Vector3D line13vec(pMesh->m_ArrayVertex[line3end]->x() - pMesh->m_ArrayVertex[line1end]->x(),pMesh->m_ArrayVertex[line3end]->y() - pMesh->m_ArrayVertex[line1end]->y(),pMesh->m_ArrayVertex[line3end]->z() - pMesh->m_ArrayVertex[line1end]->z());
						line13vec.SetUnit();

						double angle2 = acos(line13vec * yaxis ) * 180/ 3.1415926;
						double angle3 = acos(line2vec * yaxis ) * 180/ 3.1415926;
						if(fabs(angle2 - 90) < fabs(angle3 - 90))
						{
							//line13 xaxis, line2 yaxis
							pMesh->m_LandMarkArray[index1]->style = 0;
							pMesh->m_LandMarkArray[index2]->style = 1;
							pMesh->m_LandMarkArray[index3]->style = 0;
							//pMesh->m_LandMarkArray_style[index1] = 0;
							//pMesh->m_LandMarkArray_style[index2] = 1;
							//pMesh->m_LandMarkArray_style[index3] = 0;
						}else
						{
							//line2 xaxis, line13 yaxis
							pMesh->m_LandMarkArray[index1]->style = 1;
							pMesh->m_LandMarkArray[index2]->style = 0;
							pMesh->m_LandMarkArray[index3]->style = 1;
							//pMesh->m_LandMarkArray_style[index1] = 1;
							//pMesh->m_LandMarkArray_style[index2] = 0;
							//pMesh->m_LandMarkArray_style[index3] = 1;
						}
					}
		}
	}
	//pMesh->m_ArrayLandMarks_Original = pMesh->m_LandMarkArray;
	//m_newlandmarks_style_Original = pMesh->m_LandMarkArray_style;
	pDoc->UpdateAllViews(NULL);
}
double Action_PolyOmino::GetAngleBetweenTwoLandmarks(int index)
{
	if(pMesh->m_ArrayPointConnectedLandmarks[index].size() != 2)
		return -1;

	int landmark1 = pMesh->m_ArrayPointConnectedLandmarks[index][0];
	int landmark2 = pMesh->m_ArrayPointConnectedLandmarks[index][1];

	int landmark1start = pMesh->m_LandMarkArray[landmark1]->landmark_points[0];
	int landmark1end = pMesh->m_LandMarkArray[landmark1]->landmark_points[pMesh->m_LandMarkArray[landmark1]->landmark_points.size() - 1];

	int landmark2start = pMesh->m_LandMarkArray[landmark2]->landmark_points[0];
	int landmark2end = pMesh->m_LandMarkArray[landmark2]->landmark_points[pMesh->m_LandMarkArray[landmark2]->landmark_points.size() - 1];

	int index1,index2,index3;
	if(landmark1start == landmark2start)
	{
		index1 = landmark1end;
		index2 = landmark1start;
		index3 = landmark2end;
	}else
	if(landmark1end == landmark2start)
	{
		index1 = landmark1start;
		index2 = landmark1end;
		index3 = landmark2end;	
	}else
	if(landmark1start == landmark2end)
	{
		index1 = landmark1end;
		index2 = landmark1start;
		index3 = landmark2start;	
	}else
	if(landmark1end == landmark2end)
	{
		index1 = landmark1start;
		index2 = landmark1end;
		index3 = landmark2start;	
	}

	Vector3D vec1(pMesh->m_ArrayVertex[index1]->x(),pMesh->m_ArrayVertex[index1]->y(),pMesh->m_ArrayVertex[index1]->z());
	Vector3D vec2(pMesh->m_ArrayVertex[index2]->x(),pMesh->m_ArrayVertex[index2]->y(),pMesh->m_ArrayVertex[index2]->z());
	Vector3D vec3(pMesh->m_ArrayVertex[index3]->x(),pMesh->m_ArrayVertex[index3]->y(),pMesh->m_ArrayVertex[index3]->z());

	//vec1.SetUnit();
	//vec2.SetUnit();
	//vec3.SetUnit();

	Vector3D dir1 = vec2 - vec1;
	Vector3D dir2 = vec3 - vec2;

	dir1.SetUnit();
	dir2.SetUnit();

	double angle = acos(dir1 * dir2) * 180 / 3.1415926;
	return angle;
}
void Action_PolyOmino::MergeSmallEdges()
{
	if(m_MergeSmallEdgesFirstRun)
	{
		pMesh->m_ArrayLandMarks_Original = pMesh->m_LandMarkArray;
	}

	pMesh->m_LandMarkArray = pMesh->m_ArrayLandMarks_Original;
	
	m_MergeSmallEdgesFirstRun = FALSE;
	pMesh->m_ArrayPointConnectedLandmarks.resize(pMesh->m_ArrayVertex.GetSize());
	
	int vindex1; 
	int vindex2; 
	//vindex1 = 13095;
	//vindex2 = 13094;
	//vindex1 = 11605;
	//vindex2 = 38745;
	//vindex1 = 4695;
	//vindex2 = 4696;
	vindex1 = 11605;
	vindex2 = 24893;
	Vector3D vv1(pMesh->m_ArrayVertex[vindex1]->x() - pMesh->m_ArrayVertex[vindex2]->x(),pMesh->m_ArrayVertex[vindex1]->y() - pMesh->m_ArrayVertex[vindex2]->y(),pMesh->m_ArrayVertex[vindex1]->z() - pMesh->m_ArrayVertex[vindex2]->z());
	double minedgelength_tol = Length(vv1) ;
	//double tol = minedgelength_tol * 0.2;
	double lengthscale = m_MergeSmallLandmarkLength.GetPos();
	double tol = minedgelength_tol * lengthscale / 500.0;
	//double tol = minedgelength_tol * lengthscale / 20.0;
	double angle_tol = m_MergeSmallLandmarkAngle.GetPos();
	while(1)
	{
		int num = pMesh->m_LandMarkArray.size();
		//pMesh->m_ArrayDeleteFlag.clear();
		//pMesh->m_ArrayDeleteFlag.resize(pMesh->m_LandMarkArray.size(),0);
		for(int i = 0; i < pMesh->m_ArrayPointConnectedLandmarks.size(); i++)
		{
			pMesh->m_ArrayPointConnectedLandmarks[i].clear();	
		}
		for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		{
			int startvindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
			int endvindex = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
			pMesh->m_ArrayPointConnectedLandmarks[startvindex].push_back(i);
			pMesh->m_ArrayPointConnectedLandmarks[endvindex].push_back(i);
		}
		for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		{
			GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
			int startindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
			int endindex = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
			double landmarklength = GetLandMarkLength(pMesh->m_LandMarkArray,i);
			if(pLandMark->deleteflag)
				continue;

			//if(landmarklength < tol)
			{
				if(pMesh->m_ArrayPointConnectedLandmarks[startindex].size() == 2 && pMesh->m_ArrayPointConnectedLandmarks[endindex].size() == 2)
				{
					int prelandmarkindex,nextlandmarkindex;
					if(pMesh->m_ArrayPointConnectedLandmarks[startindex][0] != i)
						prelandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[startindex][0];
					else
						prelandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[startindex][1];

					if(pMesh->m_ArrayPointConnectedLandmarks[endindex][0] != i)
						nextlandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[endindex][0];
					else
						nextlandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[endindex][1];

					if(pMesh->m_LandMarkArray[prelandmarkindex]->deleteflag || pMesh->m_LandMarkArray[nextlandmarkindex]->deleteflag)
					//if(pMesh->m_ArrayDeleteFlag[prelandmarkindex] || pMesh->m_ArrayDeleteFlag[nextlandmarkindex])
						continue;

					double angle1 = GetAngleBetweenTwoLandmarks(startindex);
					double angle2 = GetAngleBetweenTwoLandmarks(endindex);

					int fixstart,fixend;
					std::vector<int>::iterator fiterator;
					fiterator = std::find(pMesh->m_FixVerticesArray.begin(),pMesh->m_FixVerticesArray.end(),startindex);
					if(fiterator != pMesh->m_FixVerticesArray.end())
						fixstart = TRUE;
					else
						fixstart = FALSE;
					fiterator = std::find(pMesh->m_FixVerticesArray.begin(),pMesh->m_FixVerticesArray.end(),endindex);
					if(fiterator != pMesh->m_FixVerticesArray.end())
						fixend = TRUE;
					else
						fixend = FALSE;
					//if(angle1 < angle_tol && angle2 < angle_tol)
					if(!fixstart || !fixend)
					//if(landmarklength < tol || angle1 < angle_tol && angle2 < angle_tol )
					if(landmarklength < tol)
					//if(GetLandMarkLength(m_newlandmarks,prelandmarkindex) < tol && GetLandMarkLength(m_newlandmarks,nextlandmarkindex) < tol)
					{
						std::vector<int> newlandmark = pMesh->m_LandMarkArray[i]->landmark_points;
						if(!fixstart)
						{
							MergeTwoLandmarks(newlandmark,pMesh->m_LandMarkArray[prelandmarkindex]->landmark_points);
							pMesh->m_LandMarkArray[prelandmarkindex]->deleteflag = 1;
						}
						if(!fixend)
						{
							MergeTwoLandmarks(newlandmark,pMesh->m_LandMarkArray[nextlandmarkindex]->landmark_points);
							pMesh->m_LandMarkArray[nextlandmarkindex]->deleteflag = 1;
						}
					
						pMesh->m_LandMarkArray[i]->deleteflag = 1;
						
						//pMesh->m_ArrayDeleteFlag[i] = 1;
						//pMesh->m_ArrayDeleteFlag[prelandmarkindex] = 1;
						//pMesh->m_ArrayDeleteFlag[nextlandmarkindex] = 1;
						//InsertLandMark(newlandmark,pMesh->m_LandMarkArray[prelandmarkindex]->style);
						InsertLandMark(newlandmark,-1);
						//pMesh->m_LandMarkArray.push_back(newlandmark);
						//pMesh->m_LandMarkArray_style.push_back(pMesh->m_LandMarkArray_style[prelandmarkindex]);
					}
				}
			}
		}

		for(int i = pMesh->m_LandMarkArray.size() - 1; i >= 0; i--)
		{
			if(pMesh->m_LandMarkArray[i]->deleteflag)
			{
				pMesh->m_LandMarkArray[i]->deleteflag = 0;
				pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
				//pMesh->m_LandMarkArray_style.erase(pMesh->m_LandMarkArray_style.begin() + i);
			}
		}
		if(pMesh->m_LandMarkArray.size() == num)
			break;
	}
	sprintf(Post_Message,"Length:%d,Angle %d,Landmark num:%d",lengthscale,angle_tol,pMesh->m_LandMarkArray.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::ExtendSmall()
{
	//
	int index1,index2;
	//index1 = 11605;
	//index2 = 38745;
	index1 = 11605;
	index2 = 24893;
	Vector3D vv1(pMesh->m_ArrayVertex[index1]->x() - pMesh->m_ArrayVertex[index2]->x(),pMesh->m_ArrayVertex[index1]->y() - pMesh->m_ArrayVertex[index2]->y(),pMesh->m_ArrayVertex[index1]->z() - pMesh->m_ArrayVertex[index2]->z());
	double minedgelength_tol = Length(vv1) ;
	double tol = minedgelength_tol * 0.2;
	double angle_tol = 60;
	int num = pMesh->m_LandMarkArray.size();
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pMesh->m_LandMarkArray[i]->deleteflag = 0;
	}
	//pMesh->m_ArrayDeleteFlag.clear();
	//pMesh->m_ArrayDeleteFlag.resize(pMesh->m_LandMarkArray.size(),0);
	for(int i = 0; i < pMesh->m_ArrayPointConnectedLandmarks.size(); i++)
	{
		pMesh->m_ArrayPointConnectedLandmarks[i].clear();	
	}
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		int startvindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
		int endvindex = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
		pMesh->m_ArrayPointConnectedLandmarks[startvindex].push_back(i);
		pMesh->m_ArrayPointConnectedLandmarks[endvindex].push_back(i);
	}
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		int startindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
		int endindex = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
		double landmarklength = GetLandMarkLength(pMesh->m_LandMarkArray,i);
		if(pMesh->m_LandMarkArray[i]->deleteflag)
			continue;

		if(landmarklength < tol)
		{
			if(pMesh->m_ArrayPointConnectedLandmarks[startindex].size() == 2 && pMesh->m_ArrayPointConnectedLandmarks[endindex].size() == 2)
			{
				int prelandmarkindex,nextlandmarkindex;
				if(pMesh->m_ArrayPointConnectedLandmarks[startindex][0] != i)
					prelandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[startindex][0];
				else
					prelandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[startindex][1];

				if(pMesh->m_ArrayPointConnectedLandmarks[endindex][0] != i)
					nextlandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[endindex][0];
				else
					nextlandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[endindex][1];

				if(pMesh->m_LandMarkArray[prelandmarkindex]->deleteflag || pMesh->m_LandMarkArray[nextlandmarkindex]->deleteflag)
				//if(pMesh->m_ArrayDeleteFlag[prelandmarkindex] || pMesh->m_ArrayDeleteFlag[nextlandmarkindex])
					continue;

				double angle1 = GetAngleBetweenTwoLandmarks(startindex);
				double angle2 = GetAngleBetweenTwoLandmarks(endindex);

				if(!(angle1 < angle_tol && angle2 < angle_tol))
					continue;

				int prelength = pMesh->m_LandMarkArray[prelandmarkindex]->landmark_points.size();
				int nextlength = pMesh->m_LandMarkArray[nextlandmarkindex]->landmark_points.size();
				if(prelength >  6 || nextlength >  6)
				{
					int shringlandmarkindex;
					if(prelength > nextlength)
					{
						shringlandmarkindex = prelandmarkindex;
					}
					else
						shringlandmarkindex = nextlandmarkindex;

					int shrinklandmarkstartv = pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points[0];
					int shrinklandmarkendv = pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points[pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.size() - 1];

					if(startindex == shrinklandmarkstartv)
					{
						for(int  j = 0; j < 3; j++)
						{
							pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.erase(pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.begin());
							pMesh->m_LandMarkArray[i]->landmark_points.insert(pMesh->m_LandMarkArray[i]->landmark_points.begin(),pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points[0]);
						}

					}else
						if(startindex == shrinklandmarkendv)
						{
							for(int  j = 0; j < 3; j++)
							{
								pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.erase(pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.begin() + pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.size() - 1);
								pMesh->m_LandMarkArray[i]->landmark_points.insert(pMesh->m_LandMarkArray[i]->landmark_points.begin(),pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points[pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.size() - 1]);
							}
						}else
							if(endindex == shrinklandmarkstartv)
							{
								for(int  j = 0; j < 3; j++)
								{
									pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.erase(pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.begin());
									pMesh->m_LandMarkArray[i]->landmark_points.push_back(pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points[0]);
								}
							}else
								if(endindex == shrinklandmarkendv)
								{
									for(int  j = 0; j < 3; j++)
									{
										pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.erase(pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.begin() + pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.size() - 1);
										pMesh->m_LandMarkArray[i]->landmark_points.push_back(pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points[pMesh->m_LandMarkArray[shringlandmarkindex]->landmark_points.size() - 1]);
									}
								}


				}
			}
		}
	}
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::DeleteVertexConnectedLandmarks()
{
	//1:delete the two landmarks connected with vertex 936
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		pMesh->m_LandMarkArray[i]->deleteflag = 0;
	//pMesh->m_ArrayDeleteFlag.clear();
	//pMesh->m_ArrayDeleteFlag.resize(pMesh->m_LandMarkArray.size(),0);

	std::vector<int> m_deleteLandmarkconnectedwithpoints;
	//m_deleteLandmarkconnectedwithpoints.push_back(4288);
	//m_deleteLandmarkconnectedwithpoints.push_back(4281);
	//m_deleteLandmarkconnectedwithpoints.push_back(4284);
	//m_deleteLandmarkconnectedwithpoints.push_back(927);
	//m_deleteLandmarkconnectedwithpoints.push_back(936);
	for(int i = 0; i < pMesh->m_ArrayPointConnectedLandmarks.size(); i++)
	{
		pMesh->m_ArrayPointConnectedLandmarks[i].clear();	
	}
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		int startvindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
		int endvindex = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
		pMesh->m_ArrayPointConnectedLandmarks[startvindex].push_back(i);
		pMesh->m_ArrayPointConnectedLandmarks[endvindex].push_back(i);
	}

	for(int i = 0; i < m_deleteLandmarkconnectedwithpoints.size(); i++)
	{
		int index1 = pMesh->m_ArrayPointConnectedLandmarks[m_deleteLandmarkconnectedwithpoints[i]][0];
		int index2 = pMesh->m_ArrayPointConnectedLandmarks[m_deleteLandmarkconnectedwithpoints[i]][1];
		pMesh->m_LandMarkArray[index1]->deleteflag = 1;
		pMesh->m_LandMarkArray[index2]->deleteflag = 1;
		//pMesh->m_ArrayDeleteFlag[index1] = 1;
		//pMesh->m_ArrayDeleteFlag[index2] = 1;
	}
	for(int i = pMesh->m_LandMarkArray.size() - 1; i >= 0; i--)
	{
		if(pMesh->m_LandMarkArray[i]->deleteflag)
		{
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
			//pMesh->m_LandMarkArray_style.erase(pMesh->m_LandMarkArray_style.begin() + i);
		}

	}
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::MergeThreeConnectedLandmarks()
{
	int index1,index2;
	//index1 = 11605;
	//index2 = 38745;
	//index1 = 11605;
	//index2 = 24893;
	index1 = 701;
	index2 = 4719;
	Vector3D vv1(pMesh->m_ArrayVertex[index1]->x() - pMesh->m_ArrayVertex[index2]->x(),pMesh->m_ArrayVertex[index1]->y() - pMesh->m_ArrayVertex[index2]->y(),pMesh->m_ArrayVertex[index1]->z() - pMesh->m_ArrayVertex[index2]->z());
	double minedgelength_tol = Length(vv1) ;
	double tol = minedgelength_tol * 0.2;
	double angle_tol = 60;
	while(1)
	{
		int num = pMesh->m_LandMarkArray.size();
		for(int i = 0; i < num; i++)
			pMesh->m_LandMarkArray[i]->deleteflag = 0;
		//pMesh->m_ArrayDeleteFlag.clear();
		//pMesh->m_ArrayDeleteFlag.resize(pMesh->m_LandMarkArray.size(),0);

		for(int i = 0; i < pMesh->m_ArrayPointConnectedLandmarks.size(); i++)
		{
			pMesh->m_ArrayPointConnectedLandmarks[i].clear();	
		}
		for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		{
			int startvindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
			int endvindex = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
			pMesh->m_ArrayPointConnectedLandmarks[startvindex].push_back(i);
			pMesh->m_ArrayPointConnectedLandmarks[endvindex].push_back(i);
		}
		for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		{
			int startindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
			int endindex = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
			double landmarklength = GetLandMarkLength(pMesh->m_LandMarkArray,i);
			if(pMesh->m_LandMarkArray[i]->deleteflag)
				continue;

			if(landmarklength < tol)
			{
				if(pMesh->m_ArrayPointConnectedLandmarks[startindex].size() == 2 && pMesh->m_ArrayPointConnectedLandmarks[endindex].size() == 2)
				{
					int prelandmarkindex,nextlandmarkindex;
					if(pMesh->m_ArrayPointConnectedLandmarks[startindex][0] != i)
						prelandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[startindex][0];
					else
						prelandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[startindex][1];

					if(pMesh->m_ArrayPointConnectedLandmarks[endindex][0] != i)
						nextlandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[endindex][0];
					else
						nextlandmarkindex = pMesh->m_ArrayPointConnectedLandmarks[endindex][1];

					if(pMesh->m_LandMarkArray[prelandmarkindex]->deleteflag || pMesh->m_LandMarkArray[nextlandmarkindex]->deleteflag)
					//if(pMesh->m_ArrayDeleteFlag[prelandmarkindex] || pMesh->m_ArrayDeleteFlag[nextlandmarkindex])
						continue;


					if(GetLandMarkLength(pMesh->m_LandMarkArray,prelandmarkindex) < tol && GetLandMarkLength(pMesh->m_LandMarkArray,nextlandmarkindex) < tol)
					{
						std::vector<int> newlandmark = pMesh->m_LandMarkArray[i]->landmark_points;
						double angle1 = GetAngleBetweenTwoLandmarks(startindex);
						double angle2 = GetAngleBetweenTwoLandmarks(endindex);

						if(!(angle1 < angle_tol && angle2 < angle_tol))
							continue;

						MergeTwoLandmarks(newlandmark,pMesh->m_LandMarkArray[prelandmarkindex]->landmark_points);
						MergeTwoLandmarks(newlandmark,pMesh->m_LandMarkArray[nextlandmarkindex]->landmark_points);
						pMesh->m_LandMarkArray[i]->deleteflag = 1;
						pMesh->m_LandMarkArray[prelandmarkindex]->deleteflag = 1;
						pMesh->m_LandMarkArray[nextlandmarkindex]->deleteflag = 1;
						//pMesh->m_ArrayDeleteFlag[i] = 1;
						//pMesh->m_ArrayDeleteFlag[prelandmarkindex] = 1;
						//pMesh->m_ArrayDeleteFlag[nextlandmarkindex] = 1;
						InsertLandMark(newlandmark,pMesh->m_LandMarkArray[prelandmarkindex]->style);
						//pMesh->m_LandMarkArray.push_back(newlandmark);
						//pMesh->m_LandMarkArray_style.push_back(pMesh->m_LandMarkArray_style[prelandmarkindex]);
					}
				}
			}
		}

		for(int i = pMesh->m_LandMarkArray.size() - 1; i >= 0; i--)
		{
			if(pMesh->m_LandMarkArray[i]->deleteflag)
			{
				pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
				//pMesh->m_LandMarkArray_style.erase(pMesh->m_LandMarkArray_style.begin() + i);
			}
		}
		if(pMesh->m_LandMarkArray.size() == num)
			break;
	}
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::MergeSeletedlandmarks()
{
	if(pMesh->m_SelectedLandmarkIndexArray.size() < 1)
		return;

	int HorizontalLandmarkNum = 0; 
	int VerticalLandmarkNum = 0;
	GeoMapLandMark* pLandMark,*pLandMark1;
	for(int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
	{
		if(pMesh->m_SelectedLandmarkIndexArray[i] >= pMesh->m_LandMarkArray.size())
			return;

		pLandMark = pMesh->m_LandMarkArray[pMesh->m_SelectedLandmarkIndexArray[i]];
		if(pLandMark->style == 0)
			HorizontalLandmarkNum++;
		else
			VerticalLandmarkNum++;
	}
	pLandMark = pMesh->m_LandMarkArray[pMesh->m_SelectedLandmarkIndexArray[0]];
	std::vector<int> mergedlandmarkpointarray = pLandMark->landmark_points;
	pLandMark->deleteflag = 1;

	std::vector<int> selectedlandmarks = pMesh->m_SelectedLandmarkIndexArray;
	selectedlandmarks.erase(selectedlandmarks.begin());
	
	//

	for(int i = 0; i < selectedlandmarks.size(); i++)
	{
		pLandMark1 = pMesh->m_LandMarkArray[selectedlandmarks[i]];
		BOOL IsMerged = MergeTwoLandmarks(mergedlandmarkpointarray,pLandMark1->landmark_points);
		if(IsMerged)
		{
			selectedlandmarks.erase(selectedlandmarks.begin() + i);
			pLandMark1->deleteflag = 1;
			i = -1;
		}
	}

	
	int style = 0;
	if(VerticalLandmarkNum > HorizontalLandmarkNum)
		style = 1;
	InsertLandMark(mergedlandmarkpointarray,style);
	pMesh->m_SelectedLandmarkIndexArray.clear();

	for(int i = pMesh->m_LandMarkArray.size() - 1; i >= 0; i-- )
	{
		pLandMark = pMesh->m_LandMarkArray[i];
		if(pLandMark->deleteflag)
		{
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
			i++;
		}
	}
}
BOOL Action_PolyOmino::MergeTwoLandmarks(std::vector<int> &landmark1,std::vector<int> &landmark2)
{

	if(landmark2[0] == landmark1[0])
	{
		for(int i = 1; i < landmark2.size(); i++)
		{
			landmark1.insert(landmark1.begin(),landmark2[i]);
		}
		return TRUE;
	}else
	if(landmark2[landmark2.size() - 1] == landmark1[0])
	{
		for(int i = landmark2.size() - 2; i >= 0; i--)
		{
			landmark1.insert(landmark1.begin(),landmark2[i]);
		}
		return TRUE;
	}else
	if(landmark2[0] == landmark1[landmark1.size() - 1])
	{
		for(int i = 1; i < landmark2.size(); i++)
		{
			landmark1.push_back(landmark2[i]);
		}
		return TRUE;
	}else
	if(landmark2[landmark2.size() - 1] == landmark1[landmark1.size() - 1])
	{
		for(int i = landmark2.size() - 2; i >= 0; i--)
		{
			landmark1.push_back(landmark2[i]);
		}
		return TRUE;
	}
	return FALSE;
}
double Action_PolyOmino::GetLandMarkLength(std::vector<GeoMapLandMark*> &m_newlandmarks, int index)
{
	int startindex = m_newlandmarks[index]->landmark_points[0];
	int endindex = m_newlandmarks[index]->landmark_points[m_newlandmarks[index]->landmark_points.size() - 1];

	Vector3D startvec(pMesh->m_ArrayVertex[startindex]->x(),pMesh->m_ArrayVertex[startindex]->y(),pMesh->m_ArrayVertex[startindex]->z());
	Vector3D endvec(pMesh->m_ArrayVertex[endindex]->x(),pMesh->m_ArrayVertex[endindex]->y(),pMesh->m_ArrayVertex[endindex]->z());

	double landmarklength = Length(endvec - startvec);
	return landmarklength;
}
void Action_PolyOmino::MergeConnectedLandMarkswithSameStyle()
{
	//return;
	//Merge the connected landmarks of the same style horizontal or vertical
	int initialnum;
	while(1)
	{
		initialnum = pMesh->m_LandMarkArray.size();

		for(int i = 0; i < initialnum; i++)
			pMesh->m_LandMarkArray[i]->deleteflag = 0;
		//pMesh->m_ArrayDeleteFlag.clear();
		//pMesh->m_ArrayDeleteFlag.resize(pMesh->m_LandMarkArray.size(),0);
		//Merge connected landmarks.
		pMesh->m_ArrayPointConnectedLandmarks.resize(pMesh->m_ArrayVertex.GetSize());
		for(int i = 0; i < pMesh->m_ArrayPointConnectedLandmarks.size(); i++)
		{
			pMesh->m_ArrayPointConnectedLandmarks[i].clear();	
		}
		for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		{
			int startvindex = pMesh->m_LandMarkArray[i]->landmark_points[0];
			int endvindex = pMesh->m_LandMarkArray[i]->landmark_points[pMesh->m_LandMarkArray[i]->landmark_points.size() - 1];
			pMesh->m_ArrayPointConnectedLandmarks[startvindex].push_back(i);
			pMesh->m_ArrayPointConnectedLandmarks[endvindex].push_back(i);
		}
		for(int i = 0; i < pMesh->m_ArrayPointConnectedLandmarks.size(); i++)
		{
			if(pMesh->m_ArrayPointConnectedLandmarks[i].size() == 2)
			{

				if(pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][0]]->deleteflag || pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->deleteflag)
				//if(pMesh->m_ArrayDeleteFlag[pMesh->m_ArrayPointConnectedLandmarks[i][0]] || pMesh->m_ArrayDeleteFlag[pMesh->m_ArrayPointConnectedLandmarks[i][1]])
					continue;
				if(pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][0]]->style == pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->style)
				{
					pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][0]]->deleteflag = 1;
					pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->deleteflag = 1;
					//Merge the two landmarks.
					std::vector<int> landmark;
					landmark = pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][0]]->landmark_points;
					int starvindex = landmark[0];
					int endvindex = landmark[landmark.size() - 1];
					int startvindexnew = pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points[0];
					int endvindexnew = pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points[pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points.size() - 1];
					if(startvindexnew == starvindex)
					{
						for(int ii = 1; ii < pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points.size(); ii++)
						{
							landmark.insert(landmark.begin(),pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points[ii]);
						}
					}else
						if(endvindexnew == starvindex)
						{
							for(int ii = pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points.size() - 2; ii >= 0; ii--)
							{
								landmark.insert(landmark.begin(),pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points[ii]);
							}
						}else
							if(startvindexnew == endvindex)
							{
								for(int ii = 1; ii < pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points.size(); ii++)
								{
									landmark.push_back(pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points[ii]);
								}
							}else
								if(endvindexnew == endvindex)
								{
									for(int ii = pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points.size() - 2; ii >= 0; ii--)
									{
										landmark.push_back(pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][1]]->landmark_points[ii]);
									}
								}
								InsertLandMark(landmark,pMesh->m_LandMarkArray[pMesh->m_ArrayPointConnectedLandmarks[i][0]]->style);
								//pMesh->m_LandMarkArray.push_back(landmark);
								//pMesh->m_LandMarkArray_style.push_back(pMesh->m_LandMarkArray_style[pMesh->m_ArrayPointConnectedLandmarks[i][0]]);
				}
			}
		}
		for(int ii = pMesh->m_LandMarkArray.size() - 1; ii >= 0; ii--)
		{
			if(pMesh->m_LandMarkArray[ii]->deleteflag)
			{
				pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + ii);
				//pMesh->m_LandMarkArray_style.erase(pMesh->m_LandMarkArray_style.begin() + ii);
			}
		}
		if(pMesh->m_LandMarkArray.size() == initialnum)
			break;
	}
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::CurveStraight_Holomorphic()
{
	// TODO: Add your control notification handler code here
	SaveGeoMap();

	char meshname[100];
	sprintf(meshname, "lamcc_dir_div_merge");

	SetCurrentDirectory("D:\\papers\\code\\bin_Curve");
	//Step
	char step10[200];
	sprintf(step10, "Filtermesh.exe %s.m -rmcomp 10 > state.m", meshname);
	system(step10);

	char step11[200];
	sprintf(step11, "copy state.m %s.m /y", meshname);
	system(step11);

	//Step one
	char step1[200];
	system("echo 0 > tmp");
	sprintf(step1, "conformalmodulous.exe -holo_form_landmark %s.m %s <tmp", meshname, meshname);
	system(step1);
	remove("tmp");

	//step 2
	char step2[200];
	sprintf(step2, "holomorphicform.exe %s_0.du.m %s_1.du.m", meshname, meshname);
	system(step2);

	//step 3
	char step3[200];
	sprintf(step3, "integrate_conformalmodulous.exe test_0.m %s.m %s_0.uv.m", meshname, meshname);
	system(step3);

	//step 4
	char step4[200];
	sprintf(step4, "filtermesh.exe %s_0.uv.m -uvtopos > %s_0.pos.m", meshname, meshname);
	system(step4);

	//step 5
	char step5[200];
	sprintf(step5, "integrate_conformalmodulous.exe test_1.m %s.m %s_1.uv.m ", meshname, meshname);
	system(step5);

	//step 6
	char step6[200];
	sprintf(step6, "filtermesh.exe %s_1.uv.m -uvtopos > %s_1.pos.m", meshname, meshname);
	system(step6);

	//%mesh%_1.pos.m
	char resultmeshname[100];
	sprintf(resultmeshname, "D:\\papers\\code\\bin_Curve\\%s_1.pos.m", meshname);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument(resultmeshname);

	//
	CMesh3d* pMesh3doriginal = (CMesh3d*)pDoc->m_SceneGraph[0];
	CMesh3d* pMesh3d1 = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	/*for(int i = 0; i < pMesh3d1->m_ArraySharpEdges.size(); i++)
	{
	std::pair<int,int> pair;
	pair = pMesh3d1->m_ArraySharpEdges[i];
	if(pair.first == 0 && pair.second == 122073 || pair.first == 122073 && pair.second == 0)
	{
	pMesh3d1->m_ArraySharpEdges.erase(pMesh3d1->m_ArraySharpEdges.begin() + i);
	i--;
	}
	if(pair.first == 1 && pair.second == 110693 || pair.first == 110693 && pair.second == 1)
	{
	pMesh3d1->m_ArraySharpEdges.erase(pMesh3d1->m_ArraySharpEdges.begin() + i);
	i--;
	}
	if(pair.first == 2 && pair.second == 102054 || pair.first == 102054 && pair.second == 2)
	{
	pMesh3d1->m_ArraySharpEdges.erase(pMesh3d1->m_ArraySharpEdges.begin() + i);
	i--;
	}
	if(pair.first == 3 && pair.second == 121828 || pair.first == 121828 && pair.second == 3)
	{
	pMesh3d1->m_ArraySharpEdges.erase(pMesh3d1->m_ArraySharpEdges.begin() + i);
	i--;
	}
	}*/
	pMesh = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	pMesh->m_LandMarkArray = pMesh3doriginal->m_LandMarkArray;
	//GenerateLandMarks();
	Point3D lower, upper;
	pMesh->ComputeBoundingBox(lower, upper);
	Point3D center = (lower + upper) / 2;
	for (int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();
		pVertex->Set(-x, -y, z);
	}
	pMesh->ComputeBoundingBox(lower, upper);
	for (int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		double x = pVertex->x() - lower.x;
		double y = pVertex->y() - lower.y;
		double z = pVertex->z() - lower.z;
		pVertex->Set(x, y, z);
	}
	//Point3D lower,upper;
	//pMesh->ComputeBoundingBox(lower,upper);
	//Point3D center = (lower + upper) / 2;
	//for(int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	//{
	//	CVertex3d* pVertex = pMesh->GetVertex(i);
	//	double x = pVertex->x() - lower.x;
	//	double y = pVertex->y() - lower.y;
	//	double z = pVertex->z() - lower.z;
	//	pVertex->Set(y,-x,z);
	//}
	pMesh->m_showFace = FALSE;
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::CurveStraight_Harmonic()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();
	if (p_FormView3->currentindex < 0 || p_FormView3->currentindex >= num)
	{
		AfxMessageBox("No mesh is selected!");
		return;
	}
	CMesh3d_OpenMesh* pOpenMesh;
	CMesh3d* pMesh3d;

	if (pDoc->m_SceneGraph[p_FormView3->currentindex]->GetType() == TYPE_MESH3D_OPENMESH)
	{
		pOpenMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph[p_FormView3->currentindex];
	}
	else if (pDoc->m_SceneGraph[p_FormView3->currentindex]->GetType() == TYPE_MESH3D)
	{
		pMesh3d = (CMesh3d*)pDoc->m_SceneGraph[p_FormView3->currentindex];
		pOpenMesh = pMesh->Convert_OpenMesh();
	}

	//Fix four boundary vertices
	pOpenMesh->m_FixVerticesArray.push_back(0);
	pOpenMesh->m_FixVerticesArray.push_back(1);
	pOpenMesh->m_FixVerticesArray.push_back(2);
	pOpenMesh->m_FixVerticesArray.push_back(3);

	//Set uv
	/*pOpenMesh->m_FixVerticesArray_uv.push_back(71.3952);
	pOpenMesh->m_FixVerticesArray_uv.push_back(16.1618);
	pOpenMesh->m_FixVerticesArray_uv.push_back(136.8);
	pOpenMesh->m_FixVerticesArray_uv.push_back(16.1618);
	pOpenMesh->m_FixVerticesArray_uv.push_back(136.8);
	pOpenMesh->m_FixVerticesArray_uv.push_back(55.4949);
	pOpenMesh->m_FixVerticesArray_uv.push_back(71.3952);
	pOpenMesh->m_FixVerticesArray_uv.push_back(55.4949);*/
	pOpenMesh->m_FixVerticesArray_uv.push_back(0);
	pOpenMesh->m_FixVerticesArray_uv.push_back(0);
	pOpenMesh->m_FixVerticesArray_uv.push_back(1);
	pOpenMesh->m_FixVerticesArray_uv.push_back(0);
	pOpenMesh->m_FixVerticesArray_uv.push_back(1);
	pOpenMesh->m_FixVerticesArray_uv.push_back(0.656463);
	pOpenMesh->m_FixVerticesArray_uv.push_back(0);
	pOpenMesh->m_FixVerticesArray_uv.push_back(0.656463);


	//delete some landmarks
	/*for (int i = 10; i < pOpenMesh->m_LandMarkArray.size(); i++)
	{
		pOpenMesh->m_LandMarkArray.erase(pOpenMesh->m_LandMarkArray.begin() + i);
		i--;
	}*/
	//pHarmonicMapping->HarmonicStraightening_Selected(pOpenMesh, 0);

	//
	//OnlyKeepSelectedLandmarks(pMesh3d, pOpenMesh);

	//fix the boundary
	pHarmonicMapping->Compute_Boundary_Vertices_Parameter_Positions_Quad(pOpenMesh);

	//linear constraints
	//pHarmonicMapping->Compute_Linear_Equations_LinearConstraints_SingleVariable(pOpenMesh,0);
	pHarmonicMapping->Compute_Linear_Equations_LinearConstraints_SingleVariable_Selected(pOpenMesh, 0);

	//Generate the straighteing
	pHarmonicMapping->GenerateParameterDomain(pOpenMesh);
	//Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pOpenMesh);

	//Obtain the linear equations
	//Compute_Linear_Equations_LinearConstraints(pOpenMesh);

	//Generate a new parameter domain mesh
	//GenerateParameterDomain(pOpenMesh);
}
void Action_PolyOmino::OnBnClickedPolyominoCurvestraight()
{
	//CurveStraight_Holomorphic();
	CurveStraight_Harmonic();
}

void Action_PolyOmino::SetCurrentMesh(int ChangeMeshType_to_Mesh3D)
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();
	if (p_FormView3->currentindex < 0 || p_FormView3->currentindex >= num)
	{
		AfxMessageBox("No mesh is selected!");
		return;
	}

	if (pDoc->m_SceneGraph[p_FormView3->currentindex]->GetType() == TYPE_MESH3D_OPENMESH && ChangeMeshType_to_Mesh3D)
	{
		pMesh = ((CMesh3d_OpenMesh*)pDoc->m_SceneGraph[p_FormView3->currentindex])->ConvertMesh3d();
		pDoc->m_SceneGraph.SetAt(p_FormView3->currentindex, pMesh);
	}
	else if (pDoc->m_SceneGraph[p_FormView3->currentindex]->GetType() == TYPE_MESH3D_OPENMESH && !ChangeMeshType_to_Mesh3D)
	{
		pMesh = NULL;
	}
	else if (pDoc->m_SceneGraph[p_FormView3->currentindex]->GetType() == TYPE_MESH3D)
	{
		pMesh = (CMesh3d*)pDoc->m_SceneGraph[p_FormView3->currentindex];
	}
	sprintf(Post_Message, "Current Mesh %d", p_FormView3->currentindex);
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
}
void Action_PolyOmino::OnBnClickedOminoSetcurrentmesh()
{
	SetCurrentMesh(0);
}

void Action_PolyOmino::OnBnClickedOminoSplitlandmark()
{
	// TODO: Add your control notification handler code here
	SplitLandmark();
}

void Action_PolyOmino::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);  

	// Check which slider sent the notification  
	if (pSlider == &m_SplitAngleSlider)  
	{  
		m_SplitAngle = m_SplitAngleSlider.GetPos();
		SplitLandmark();
		sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d, Angle: %d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size(),m_SplitAngle);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	}else
	if(pSlider == &m_OminoMergeAngle)
	{
		m_MergeAngle = m_OminoMergeAngle.GetPos();
		MergeOneEdgeWithSloping();
		sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d, Angle: %d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size(),m_MergeAngle);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	}else
	if(pSlider == &m_EdgeSubdivision)
	{
		m_EdgeSubdivisionNum = m_EdgeSubdivision.GetPos();
		Sloppingedgesubdivision();
		sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d, Edge Subdivision Num: %d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size(),m_EdgeSubdivisionNum * 2 -1);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);	
	}else
	if(pSlider == &m_MergeSmallLandmarkLength || pSlider == &m_MergeSmallLandmarkAngle)
	{
		MergeSmallEdges();
		sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d, Edge Subdivision Num: %d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size(),m_EdgeSubdivisionNum * 2 -1);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);	
	}
	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

int Action_PolyOmino::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

BOOL Action_PolyOmino::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_SplitAngleSlider.SetRangeMin(0);
	m_SplitAngleSlider.SetRangeMax(180);
	m_SplitAngleSlider.SetRange(0,180);
	m_OminoMergeAngle.SetRange(0,180);
	m_EdgeSubdivision.SetRange(0,10);
	m_MergeSmallLandmarkLength.SetRange(0,2000);
	m_MergeSmallLandmarkAngle.SetRange(0,180);
	m_EdgeSubdivision.SetPos(m_EdgeSubdivisionNum);
	m_SplitAngleSlider.SetPos(m_SplitAngle);
	m_OminoMergeAngle.SetPos(m_MergeAngle);
	m_SplitDirection.SetCheck(m_bSplitDirection);
	m_MergeSmallLandmarkLength.SetPos(50);
	m_MergeSmallLandmarkAngle.SetPos(50);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void Action_PolyOmino::OnBnClickedSplitdirectioncheck1()
{
	// TODO: Add your control notification handler code here
	m_bSplitDirection = m_SplitDirection.GetCheck();
	SplitLandmark();
	sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d, Angle: %d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size(),m_SplitAngle);
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
}

void Action_PolyOmino::OnBnClickedPolyominoSloppingedgesubdivision()
{
	// TODO: Add your control notification handler code here
	Sloppingedgesubdivision();
}

extern CMyTreeView* pTreeView;


void Action_PolyOmino::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnChar(nChar, nRepCnt, nFlags);
}
BOOL Action_PolyOmino::PreTranslateMessage(MSG* pMsg) 
{   // TODO: 在此添加专用代码和/或调用基类  
	//判断是否为键盘消息   
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)  
	{    
		//判断是否按下键盘Enter键   
		if(pMsg->wParam==VK_RETURN)   
		{      
			//Do anything what you want to    return TRUE;  
			return TRUE;
		}  
	}   
	return CDialog::PreTranslateMessage(pMsg); 
} 
void Action_PolyOmino::OnBnClickedPolyominoDeleteselectedlandmarks()
{
	// TODO: Add your control notification handler code here
	std::sort(pMesh->m_SelectedLandmarkIndexArray.begin(),pMesh->m_SelectedLandmarkIndexArray.end());
	for(int i = pMesh->m_SelectedLandmarkIndexArray.size() - 1; i >= 0; i--)
	{
		if(pMesh->m_SelectedLandmarkIndexArray[i] >= pMesh->m_LandMarkArray.size() || pMesh->m_SelectedLandmarkIndexArray[i] < 0)
			return;
		pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + pMesh->m_SelectedLandmarkIndexArray[i]);
		//pMesh->m_LandMarkArray_style.erase(pMesh->m_LandMarkArray_style.begin() + pMesh->m_SelectedLandmarkIndexArray[i]);

	}
	pMesh->m_SelectedLandmarkIndexArray.clear();

	//
	SaveCurrentLandmak();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedOminoLoadusamap()
{
	// TODO: Add your control notification handler code here
	//pDoc->OnOpenDocument("D:\\Razib\\lamcc_smooth_org_normalize.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	OnBnClickedOminoSetcurrentmesh();
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pMesh->m_showFace = FALSE;

	OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message,"D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark1.txt");

	CString landmarkname = message;
	Loadlandmark(landmarkname);
}

void Action_PolyOmino::OnBnClickedPolyominoMergeseletedlandmarks()
{
	// TODO: Add your control notification handler code here
	MergeSeletedlandmarks();

	//
	SaveCurrentLandmak();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoClearlandmarkselection()
{
	// TODO: Add your control notification handler code here
	pMesh->m_SelectedLandmarkIndexArray.clear();

	sprintf(Post_Message,"All selected landmark are cleared!");
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoInverseseletedlandmarkstyle()
{
	// TODO: Add your control notification handler code here
	GeoMapLandMark* pLandMark,*pLandMark1;
	for(int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
	{
		pLandMark = pMesh->m_LandMarkArray[pMesh->m_SelectedLandmarkIndexArray[i]];
		pLandMark->style = 1 - pLandMark->style;
	}

	//
	SaveCurrentLandmak();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::SaveLandmark(CString FileName)
{
	std::ofstream file(FileName);

	file << pMesh->m_LandMarkArray.size() << "\n";
	int vertexoffset = 0;
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		file << pLandMark->deleteflag << "\n";
		file << pLandMark->fixedflag << "\n";
		file << pLandMark->style << "\n";
		file << pLandMark->isAuxillary << "\n";
		file << pLandMark->landmark_points.size() << "\n";
		for(int j = 0; j < pLandMark->landmark_points.size(); j++)
		{
			file << pLandMark->landmark_points[j] + vertexoffset << " ";
		}
	}
	int num = pMesh->m_FixVerticesArray.size();
	file << num << "\n";
	for(int i = 0; i < num; i++)
	{
		file << pMesh->m_FixVerticesArray[i] + vertexoffset << " ";
	}
	file.close();
}
void Action_PolyOmino::OnBnClickedPolyominoSaveLandmark()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(FALSE);
	if(dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}else
		return;

	SaveLandmark(FileName);
}
void Action_PolyOmino::LoadSelectedPoints(CString FileName)
{
	pMesh->m_LandMarkArray.clear();
	std::ifstream file(FileName);

	int num, pointnum;
	//file >> num;
	//pMesh->m_LandMarkArray.resize(1);
	GeoMapLandMark* pLandMark;

	pLandMark = new GeoMapLandMark();

	int index;
	while (!file.eof())
	{
		file >> index;
		pLandMark->landmark_points.push_back(index);
	}
	pMesh->m_LandMarkArray.push_back(pLandMark);

	file.close();
}

void Action_PolyOmino::Loadlandmark(CString FileName)
{
	pMesh->m_LandMarkArray.clear();
	std::ifstream file(FileName);

	int num,pointnum;
	file >> num;
	pMesh->m_LandMarkArray.resize(num);
	GeoMapLandMark* pLandMark;
	for(int i = 0; i < num; i++)
	{
		pLandMark = new GeoMapLandMark();
		file >> pLandMark->deleteflag;
		file >> pLandMark->fixedflag;
		file >> pLandMark->style;
		file >> pLandMark->isAuxillary;
		file >> pointnum ;
		pLandMark->landmark_points.resize(pointnum);
		for(int j = 0; j < pointnum; j++)
		{
			file >> pLandMark->landmark_points[j];
		}
		pMesh->m_LandMarkArray[i] = pLandMark;
	}
	file >> num;
	int val;
	pMesh->m_FixVerticesArray.clear();
	for(int i = 0; i < num; i++)
	{
		file >> val;
		pMesh->m_FixVerticesArray.push_back(val);
	}
	file.close();
}
void Action_PolyOmino::Loadlandmark(CMesh3d*pMesh, CString FileName)
{
	pMesh->m_LandMarkArray.clear();
	std::ifstream file(FileName);

	int num,pointnum;
	file >> num;
	pMesh->m_LandMarkArray.resize(num);
	GeoMapLandMark* pLandMark;
	for(int i = 0; i < num; i++)
	{
		pLandMark = new GeoMapLandMark();
		file >> pLandMark->deleteflag;
		file >> pLandMark->fixedflag;
		file >> pLandMark->style;
		file >> pLandMark->isAuxillary;
		file >> pointnum ;
		pLandMark->landmark_points.resize(pointnum);
		for(int j = 0; j < pointnum; j++)
		{
			file >> pLandMark->landmark_points[j];
		}
		pMesh->m_LandMarkArray[i] = pLandMark;
	}
	file >> num;
	int val;
	pMesh->m_FixVerticesArray.clear();
	for(int i = 0; i < num; i++)
	{
		file >> val;
		pMesh->m_FixVerticesArray.push_back(val);
	}
	file.close();
}
void Action_PolyOmino::Loadlandmark(CMesh3d_OpenMesh*pMesh, CString FileName)
{
	pMesh->m_LandMarkArray.clear();
	std::ifstream file(FileName);

	int num, pointnum;
	file >> num;
	pMesh->m_LandMarkArray.resize(num);
	GeoMapLandMark* pLandMark;
	for (int i = 0; i < num; i++)
	{
		pLandMark = new GeoMapLandMark();
		file >> pLandMark->deleteflag;
		file >> pLandMark->fixedflag;
		file >> pLandMark->style;
		file >> pLandMark->isAuxillary;
		file >> pointnum;
		pLandMark->landmark_points.resize(pointnum);
		for (int j = 0; j < pointnum; j++)
		{
			file >> pLandMark->landmark_points[j];
		}
		pMesh->m_LandMarkArray[i] = pLandMark;
	}
	file >> num;
	int val;
	pMesh->m_FixVerticesArray.clear();
	for (int i = 0; i < num; i++)
	{
		file >> val;
		pMesh->m_FixVerticesArray.push_back(val);
	}
	file.close();
}
void Action_PolyOmino::OnBnClickedPolyominoLoadlandmark()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}else
		return;

	Loadlandmark(FileName);
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoSmoothlandmarks()
{
	// TODO: Add your control notification handler code here
	std::vector<Point3D> m_ArrayPointArray;
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		m_ArrayPointArray.clear();
		m_ArrayPointArray.resize(pLandMark->landmark_points.size());
		for(int j = 1; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int preindex = pLandMark->landmark_points[j - 1];
			int nextindex = pLandMark->landmark_points[j + 1];
			double x1 = pMesh->m_ArrayVertex[preindex]->x();
			double y1 = pMesh->m_ArrayVertex[preindex]->y();
			double z1 = pMesh->m_ArrayVertex[preindex]->z();
			double x2 = pMesh->m_ArrayVertex[nextindex]->x();
			double y2 = pMesh->m_ArrayVertex[nextindex]->y();
			double z2 = pMesh->m_ArrayVertex[nextindex]->z();
			double x = (x1 + x2) / 2;
			double y = (y1 + y2) / 2;
			double z = (z1 + z2) / 2;
			Point3D point;
			point.x = x;
			point.y = y;
			point.z = z;
			m_ArrayPointArray[j] = point;
		}
		for(int j = 1; j < pLandMark->landmark_points.size() - 1; j++)
		{
			Point3D point = m_ArrayPointArray[j];
			pMesh->m_ArrayVertex[pLandMark->landmark_points[j]]->Set(point.x,point.y,point.z);
		}
	}
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoSmoothselectedlandmarks()
{
	// TODO: Add your control notification handler code here
	std::vector<Point3D> m_ArrayPointArray;
	for(int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
	{
		int index = pMesh->m_SelectedLandmarkIndexArray[i];
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[index];
		m_ArrayPointArray.clear();
		m_ArrayPointArray.resize(pLandMark->landmark_points.size());
		for(int j = 1; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int preindex = pLandMark->landmark_points[j - 1];
			int nextindex = pLandMark->landmark_points[j + 1];
			double x1 = pMesh->m_ArrayVertex[preindex]->x();
			double y1 = pMesh->m_ArrayVertex[preindex]->y();
			double z1 = pMesh->m_ArrayVertex[preindex]->z();
			double x2 = pMesh->m_ArrayVertex[nextindex]->x();
			double y2 = pMesh->m_ArrayVertex[nextindex]->y();
			double z2 = pMesh->m_ArrayVertex[nextindex]->z();
			double x = (x1 + x2) / 2;
			double y = (y1 + y2) / 2;
			double z = (z1 + z2) / 2;
			Point3D point;
			point.x = x;
			point.y = y;
			point.z = z;
			m_ArrayPointArray[j] = point;
		}
		for(int j = 1; j < pLandMark->landmark_points.size() - 1; j++)
		{
			Point3D point = m_ArrayPointArray[j];
			pMesh->m_ArrayVertex[pLandMark->landmark_points[j]]->Set(point.x,point.y,point.z);
		}
	}
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::GeneratePSLGFile(std::vector<int>& m_ArrayOutputVerticesIndex,char* outputfile)
{
	//First Get the four boundary vertices
	Point3D lower,upper;
	pMesh->ComputeBoundingBox(lower,upper);
	CVertex3d* pVertex;
	int index00,index10,index11,index01;
	index00 = index10 = index11 = index01 = -1;
	double TOL1 = 1e0;
	//offset
	lower.x -= 2;
	lower.y -= 2;
	upper.x += 2;
	upper.y += 2;

	//Add bounding box four vertices
	pMesh->AddVertex(new CVertex3d(lower.x, lower.y, 0));
	pMesh->AddVertex(new CVertex3d(lower.x, upper.y, 0));
	pMesh->AddVertex(new CVertex3d(upper.x, lower.y, 0));
	pMesh->AddVertex(new CVertex3d(upper.x, upper.y, 0));
	for(int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		pVertex = pMesh->m_ArrayVertex[i];
		double d1 = Distance(pVertex, lower);
		double d2 = Distance(pVertex, Point3D(upper.x, lower.y, 0));
		double d3 = Distance(pVertex, upper);
		double d4 = Distance(pVertex, Point3D(lower.x, upper.y, 0));
		if(d1 < TOL1)
			index00 = i;
		else
			if(d2 < TOL1)
				index10 = i;
			else
				if(d3 < TOL1)
					index11 = i;
				else
					if(d4 < TOL1)
						index01 = i;
	}
	
	m_ArrayOutputVerticesIndex.push_back(index00);
	m_ArrayOutputVerticesIndex.push_back(index10);
	m_ArrayOutputVerticesIndex.push_back(index11);
	m_ArrayOutputVerticesIndex.push_back(index01);
	std::vector<int>::iterator finditerator;
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		finditerator = std::find(m_ArrayOutputVerticesIndex.begin(),m_ArrayOutputVerticesIndex.end(),pLandMark->landmark_points[0]);
		if(finditerator == m_ArrayOutputVerticesIndex.end())
		{
			m_ArrayOutputVerticesIndex.push_back(pLandMark->landmark_points[0]);
		}
		finditerator = std::find(m_ArrayOutputVerticesIndex.begin(),m_ArrayOutputVerticesIndex.end(),pLandMark->landmark_points[pLandMark->landmark_points.size() - 1]);
		if(finditerator == m_ArrayOutputVerticesIndex.end())
		{
			m_ArrayOutputVerticesIndex.push_back(pLandMark->landmark_points[pLandMark->landmark_points.size() - 1]);
		}
		for(int j = 1; j < pLandMark->landmark_points.size() - 1; j++)
		{
			finditerator = std::find(m_ArrayOutputVerticesIndex.begin(), m_ArrayOutputVerticesIndex.end(), pLandMark->landmark_points[j]);
			if (finditerator == m_ArrayOutputVerticesIndex.end())
			{
				m_ArrayOutputVerticesIndex.push_back(pLandMark->landmark_points[j]);
			}
			//m_ArrayOutputVerticesIndex.push_back(pLandMark->landmark_points[j]);
		}
	}

	//
	//std::sort(m_ArrayOutputVerticesIndex.begin(),m_ArrayOutputVerticesIndex.end());

	//

	std::ofstream outfile(outputfile);
	outfile << m_ArrayOutputVerticesIndex.size() << " 2 0 1" << "\n";
	for(int i = 0; i < m_ArrayOutputVerticesIndex.size(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(m_ArrayOutputVerticesIndex[i]);
		if(i < 4)
			outfile << i  << " " << pVertex->x() << " " << pVertex->y() << " 1 " << "\n";
		else
			outfile << i  << " " << pVertex->x() << " " << pVertex->y() << " 0 " << "\n";
	}
	int numofsegments = 4;
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int start = pLandMark->landmark_points[j];
			int end = pLandMark->landmark_points[j + 1];
			if (start != end)
			{
				numofsegments++;
			}
		}
		//numofsegments += pLandMark->landmark_points.size() - 1;
	}

	//numofsegments = numofsegments / 2;
	outfile << numofsegments << " 1 " << "\n";

	outfile << " 0  0 1 1" <<"\n"; 
	outfile << " 1  1 2 1" <<"\n"; 
	outfile << " 2  2 3 1" <<"\n"; 
	outfile << " 3  3 0 1" <<"\n"; 

	int triangleid = 4;
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		for(int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			if (triangleid == numofsegments)
				break;
			int start = pLandMark->landmark_points[j];
			int end = pLandMark->landmark_points[j + 1];

			finditerator = std::find(m_ArrayOutputVerticesIndex.begin(),m_ArrayOutputVerticesIndex.end(),start);
			start = finditerator - m_ArrayOutputVerticesIndex.begin();
			finditerator = std::find(m_ArrayOutputVerticesIndex.begin(),m_ArrayOutputVerticesIndex.end(),end);
			end = finditerator - m_ArrayOutputVerticesIndex.begin();
			if (start != end)
			{
				outfile << triangleid << " " << start << " " << end << " 0 " << "\n";
				triangleid++;
			}
		}
		if (triangleid == numofsegments)
			break;
	}
	outfile << " 0 " << "\n";
	outfile.close();
}
void Action_PolyOmino::SimplifyLandmarks()
{
	int deletenum = 0;
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		for(int j = 1; j < pLandMark->landmark_points.size() - 1; j++)
		{
			Vector3D p1,p2,p3;
			CVertex3d* pVertex1 = pMesh->GetVertex(pLandMark->landmark_points[j - 1]);
			CVertex3d* pVertex2 = pMesh->GetVertex(pLandMark->landmark_points[j]);
			CVertex3d* pVertex3 = pMesh->GetVertex(pLandMark->landmark_points[j + 1]);
			p1.SetValue(pVertex1->x(), pVertex1->y(),pVertex1->z());
			p2.SetValue(pVertex2->x(), pVertex2->y(),pVertex2->z());
			p3.SetValue(pVertex3->x(), pVertex3->y(),pVertex3->z());

			double a = (p2 - p3) * (p2 - p1) / ((p1 - p2) * (p1 - p2));
			Vector3D p4 = p1 * a + p2 * (1 - a);
			double len = Length(p3 - p4);

			if(len < 0.001)
			{
				pLandMark->landmark_points.erase(pLandMark->landmark_points.begin() + j);
				j--;
				deletenum++;
			}
		}
	}
}
void Action_PolyOmino::OnBnClickedPolyominoTriangulate()
{
	std::vector<int> m_ArrayOutputVerticesIndex;
	//Delete Duplicate Vertices
	pUniformGridMesh = pMesh;

	DeleteDuplicateVertices_Poly();

	//Simplify the Landmark Array
	SimplifyLandmarks();

	//first write the smooth results to A pslg file
	GeneratePSLGFile(m_ArrayOutputVerticesIndex,"D:\\papers\\code\\Triangle\\Geo.poly");

	char step1[200];
	SetCurrentDirectory("D:\\papers\\code\\triangle");
	//sprintf(step1,"triangle.exe -pq33.8 Geo");
	//sprintf(step1, "triangle.exe -pq30 Geo");
	sprintf(step1,"triangle.exe -p Geo");
	system(step1);

	char resultmeshname[100];
	sprintf(resultmeshname,"D:\\papers\\code\\triangle\\Geo.1.ele");
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument(resultmeshname);

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];

	//
	DetectandSplitSharpEdges(pMesh1,m_ArrayOutputVerticesIndex);

	//
	DeleteDuplicateVertices_Mesh3d(pMesh1);

	pUniformGridMesh = pMesh1;

	this->pMesh = pMesh1;

	DeleteZeroLandmarks(pMesh1);

	DeleteZeroLandmarks(pMesh);
	//GenerateLandMarks();
	pMesh1->m_showFace = FALSE;
	pMesh1->m_showSharpEdge = TRUE;
	pMesh1->m_showLandmarkEdge = TRUE;
	pMesh1->m_showLandmarkVertex = FALSE;
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::DeleteZeroLandmarks(CMesh3d* pMesh)
{
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		if (pLandMark->landmark_points.size() == 0)
		{
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
			i--;
		}
	}
}
void Action_PolyOmino::DetectandSplitSharpEdges(CMesh3d* pMesh1,std::vector<int> &m_ArrayOutputVerticesIndex)
{
	std::vector<int>::iterator finditerator;
	std::pair<int,int> pair;
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		for(int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int start = pLandMark->landmark_points[j];
			int end = pLandMark->landmark_points[j + 1];

			finditerator = std::find(m_ArrayOutputVerticesIndex.begin(),m_ArrayOutputVerticesIndex.end(),start);
			start = finditerator - m_ArrayOutputVerticesIndex.begin();
			finditerator = std::find(m_ArrayOutputVerticesIndex.begin(),m_ArrayOutputVerticesIndex.end(),end);
			end = finditerator - m_ArrayOutputVerticesIndex.begin();
			pair.first = start;
			pair.second = end;
			pMesh1->m_ArraySharpEdges.push_back(pair);
		}
	}

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		GeoMapLandMark* pLandMark1 = new GeoMapLandMark();
		for (int j = 0; j < pLandMark->landmark_points.size(); j++)
		{
			int start = pLandMark->landmark_points[j];

			finditerator = std::find(m_ArrayOutputVerticesIndex.begin(), m_ArrayOutputVerticesIndex.end(), start);
			start = finditerator - m_ArrayOutputVerticesIndex.begin();
			
			pLandMark1->landmark_points.push_back(start);
		}
		pLandMark1->style = pLandMark->style;
		pLandMark1->flag = pLandMark->flag;
		pMesh1->m_LandMarkArray.push_back(pLandMark1);
	}

	//Check whether steiner points lies on selected edges
	for(int i = m_ArrayOutputVerticesIndex.size(); i < pMesh1->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh1->GetVertex(i);
		for(int j = 0; j < pMesh1->m_ArraySharpEdges.size(); j++)
		{
			pair = pMesh1->m_ArraySharpEdges[j];
			int start = pair.first;
			int end = pair.second;
			CVertex3d* pStartVertex = pMesh1->m_ArrayVertex[start];
			CVertex3d* pEndVertex = pMesh1->m_ArrayVertex[end];

			BOOL flag = pTreeView->JudgeWhetherPointLiesOnLineSegment(pVertex->x(),pVertex->y(),pStartVertex->x(),pStartVertex->y(),pEndVertex->x(),pEndVertex->y(),1e-5);
			if(flag)
			{
				pMesh1->m_ArraySharpEdges.erase(pMesh1->m_ArraySharpEdges.begin() + j);
				pair.first = start;
				pair.second = i;
				pMesh1->m_ArraySharpEdges.push_back(pair);
				pair.first = i;
				pair.second = end;
				pMesh1->m_ArraySharpEdges.push_back(pair);
				break;
			}
		}
	}
}
void Action_PolyOmino::OnBnClickedPolyominoSplitlandmarks()
{
	// TODO: Add your control notification handler code here
	//if(pMesh->m_ArraySeletedVertexIndex.size() != 1)
	//	return;

	//Update the selected Landmarks
	std::vector<GeoMapLandMark*> m_ArraySelectedLandmarks;
	m_ArraySelectedLandmarks.resize(pMesh->m_SelectedLandmarkIndexArray.size());
	for (int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
	{
		m_ArraySelectedLandmarks[i] = pMesh->m_LandMarkArray[pMesh->m_SelectedLandmarkIndexArray[i]];
	}
	
	int flag = 0;
	for(int ii = 0; ii < pMesh->m_ArraySharpVertexIndex.size(); ii++)
	{
		int index = pMesh->m_ArraySharpVertexIndex[ii];
		flag = 0;
		for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		{
			GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
			for(int j = 1; j < pLandMark->landmark_points.size() - 1; j++)
			{
				if(pLandMark->landmark_points[j] == index)
				{
					GeoMapLandMark* pLandMark1 = new GeoMapLandMark();
					GeoMapLandMark* pLandMark2 = new GeoMapLandMark();
					pLandMark1->deleteflag = pLandMark->deleteflag;
					pLandMark2->deleteflag = pLandMark->deleteflag;
					pLandMark1->fixedflag = pLandMark->fixedflag;
					pLandMark2->fixedflag = pLandMark->fixedflag;
					pLandMark1->style = pLandMark->style;
					pLandMark2->style = pLandMark->style;
					pLandMark1->landmark_points.insert(pLandMark1->landmark_points.begin(),pLandMark->landmark_points.begin(),pLandMark->landmark_points.begin() + j + 1);
					pLandMark2->landmark_points.insert(pLandMark2->landmark_points.begin(),pLandMark->landmark_points.begin() + j,pLandMark->landmark_points.end());
					pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
					pMesh->m_LandMarkArray.push_back(pLandMark1);
					pMesh->m_LandMarkArray.push_back(pLandMark2);

					//update the selected array
					for (int k = 0; k < m_ArraySelectedLandmarks.size(); k++)
					{
						if (m_ArraySelectedLandmarks[k] == pLandMark)
						{
							m_ArraySelectedLandmarks.erase(m_ArraySelectedLandmarks.begin() + k);
							m_ArraySelectedLandmarks.push_back(pLandMark1);
							m_ArraySelectedLandmarks.push_back(pLandMark2);
							break;
						}
					}

					flag = 1;
					break;
				}
			}
			if(flag)
				break;
		}
		
	}

	//update the selected array
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pMesh->m_LandMarkArray[i]->flag = i;
	}
	pMesh->m_SelectedLandmarkIndexArray.clear();
	for (int i = 0; i < m_ArraySelectedLandmarks.size(); i++)
	{
		pMesh->m_SelectedLandmarkIndexArray.push_back(m_ArraySelectedLandmarks[i]->flag);
	}
	m_ArraySelectedLandmarks.clear();
	sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

	//
	SaveCurrentLandmak();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoClearvertexselection()
{
	// TODO: Add your control notification handler code here
	pMesh->m_ArraySharpVertexIndex.clear();

	sprintf(Post_Message,"total %d points",pMesh->m_ArraySharpVertexIndex.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::DetectSelfIntersectLandmarks()
{
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		int style = pLandmark->style;
		for(int j = 1; j < pLandmark->landmark_points.size() - 1; j++)
		{
			if(style == 0)
			{
				//Horizontal
				int preindex = pLandmark->landmark_points[j - 1];
				int index = pLandmark->landmark_points[j];
				int nextindex = pLandmark->landmark_points[j + 1];

				double prex = pMesh->GetVertex(preindex)->x();
				double x = pMesh->GetVertex(index)->x();
				double nextx = pMesh->GetVertex(nextindex)->x();

				if(x >= prex && x >= nextx || x <= prex && x <= nextx)
				{
					pMesh->m_SelectedLandmarkIndexArray.push_back(i);
					pMesh->m_ArraySharpVertexIndex.push_back(index);
					break;
				}
			}else
			{
				//Vertical
				int preindex = pLandmark->landmark_points[j - 1];
				int index = pLandmark->landmark_points[j];
				int nextindex = pLandmark->landmark_points[j + 1];

				double prey = pMesh->GetVertex(preindex)->y();
				double y = pMesh->GetVertex(index)->y();
				double nexty = pMesh->GetVertex(nextindex)->y();

				if(y >= prey && y >= nexty || y <= prey && y <= nexty)
				{
					pMesh->m_SelectedLandmarkIndexArray.push_back(i);
					pMesh->m_ArraySharpVertexIndex.push_back(index);
					break;
				}
			}
		}
	}
}
void Action_PolyOmino::DetectIntersectLandmarks()
{
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		for(int j = 0; j < pMesh->m_LandMarkArray.size(); j++)
		{
			if (j == i)
				continue;
			//
			GeoMapLandMark* pLandmark1 = pMesh->m_LandMarkArray[j];

			if(pLandmark->style != pLandmark1->style)
			{
				double x,y,minx,maxx,miny,maxy;
				//
				if(pLandmark->style == 1)
				{
					x = pMesh->m_ArrayVertex[pLandmark->landmark_points[0]]->x();
					miny = pMesh->m_ArrayVertex[pLandmark->landmark_points[0]]->y();
					maxy = pMesh->m_ArrayVertex[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]]->y();

					if(miny > maxy)
					{
						double bak = miny;
						miny = maxy;
						maxy = bak;
					}
				}else
				{
					y = pMesh->m_ArrayVertex[pLandmark->landmark_points[0]]->y();
					minx = pMesh->m_ArrayVertex[pLandmark->landmark_points[0]]->x();
					maxx = pMesh->m_ArrayVertex[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]]->x();

					if(minx > maxx)
					{
						double bak = minx;
						minx = maxx;
						maxx = bak;
					}
				}

				if(pLandmark1->style == 1)
				{
					x = pMesh->m_ArrayVertex[pLandmark1->landmark_points[0]]->x();
					miny = pMesh->m_ArrayVertex[pLandmark1->landmark_points[0]]->y();
					maxy = pMesh->m_ArrayVertex[pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1]]->y();

					if(miny > maxy)
					{
						double bak = miny;
						miny = maxy;
						maxy = bak;
					}
				}else
				{
					y = pMesh->m_ArrayVertex[pLandmark1->landmark_points[0]]->y();
					minx = pMesh->m_ArrayVertex[pLandmark1->landmark_points[0]]->x();
					maxx = pMesh->m_ArrayVertex[pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1]]->x();

					if(minx > maxx)
					{
						double bak = minx;
						minx = maxx;
						maxx = bak;
					}
				}

				//
				if(x > minx + 1e-3 && x < maxx - 1e-3 && y > miny + 1e-3 && y < maxy - 1e-3)
				{
					pMesh->m_SelectedLandmarkIndexArray.push_back(i);
					pMesh->m_SelectedLandmarkIndexArray.push_back(j);
				}
			}else
			{
				double minvalue1,maxvalue1,minvalue2,maxvalue2;
				double y1,y2;
				if(pLandmark1->style == 0)
				{
					//horizontal
					minvalue1 = pMesh->m_ArrayVertex[pLandmark1->landmark_points[0]]->x();
					maxvalue1 = pMesh->m_ArrayVertex[pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1]]->x();
					y1 = pMesh->m_ArrayVertex[pLandmark1->landmark_points[0]]->y();

					if(minvalue1 > maxvalue1)
					{
						double test = minvalue1;
						minvalue1 = maxvalue1;
						maxvalue1 = test;
					}

					minvalue2 = pMesh->m_ArrayVertex[pLandmark->landmark_points[0]]->x();
					maxvalue2 = pMesh->m_ArrayVertex[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]]->x();
					y2 = pMesh->m_ArrayVertex[pLandmark->landmark_points[0]]->y();

					if(minvalue2 > maxvalue2)
					{
						double test = minvalue2;
						minvalue2 = maxvalue2;
						maxvalue2 = test;
					}
					if(fabs(y1 - y2) < 1e-6)
					if(minvalue1 > minvalue2 + 1e-6 && minvalue1 < maxvalue2 - 1e-6 || maxvalue1 > minvalue2 + 1e-6 && maxvalue1 < maxvalue2 - 1e-6 || 
						minvalue2 > minvalue1 + 1e-6 && minvalue2 < maxvalue1 - 1e-6 || maxvalue2 > minvalue1 + 1e-6 && maxvalue2 < maxvalue1 - 1e-6)
					{
						pMesh->m_SelectedLandmarkIndexArray.push_back(i);
						pMesh->m_SelectedLandmarkIndexArray.push_back(j);
					}
				}else
				{
					double x1,x2;
					//Vertical
					minvalue1 = pMesh->m_ArrayVertex[pLandmark1->landmark_points[0]]->y();
					maxvalue1 = pMesh->m_ArrayVertex[pLandmark1->landmark_points[pLandmark1->landmark_points.size() - 1]]->y();
					x1 = pMesh->m_ArrayVertex[pLandmark1->landmark_points[0]]->x();

					if(minvalue1 > maxvalue1)
					{
						double test = minvalue1;
						minvalue1 = maxvalue1;
						maxvalue1 = test;
					}

					minvalue2 = pMesh->m_ArrayVertex[pLandmark->landmark_points[0]]->y();
					maxvalue2 = pMesh->m_ArrayVertex[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]]->y();
					x2 = pMesh->m_ArrayVertex[pLandmark->landmark_points[0]]->x();

					if(minvalue2 > maxvalue2)
					{
						double test = minvalue2;
						minvalue2 = maxvalue2;
						maxvalue2 = test;
					}
					if(fabs(x1 - x2) < 1e-6)
						if(minvalue1 > minvalue2 + 1e-6 && minvalue1 < maxvalue2 - 1e-6 || maxvalue1 > minvalue2 + 1e-6 && maxvalue1 < maxvalue2 - 1e-6 || 
							minvalue2 > minvalue1 + 1e-6 && minvalue2 < maxvalue1 - 1e-6 || maxvalue2 > minvalue1 + 1e-6 && maxvalue2 < maxvalue1 - 1e-6)
						{
							pMesh->m_SelectedLandmarkIndexArray.push_back(i);
							pMesh->m_SelectedLandmarkIndexArray.push_back(j);
						}
				}

				
			}

		}
	}
}
void Action_PolyOmino::OnBnClickedPolyominoCheckinvalidedges()
{
	// TODO: Add your control notification handler code here
	//Detect all invalid landmarks
	//First self intersect landmarks
	DetectSelfIntersectLandmarks();

	//Second detect intersect landmarks
	DetectIntersectLandmarks();

	sprintf(Post_Message,"Invalid LandMarks %d",pMesh->m_SelectedLandmarkIndexArray.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);

	if(pDoc->m_SceneGraph.NbObject() == 2)
	{
		CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1 - p_FormView3->currentindex);
		pMesh1->m_SelectedLandmarkIndexArray = pMesh->m_SelectedLandmarkIndexArray;
	}
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoAddlandmark()
{
	// TODO: Add your control notification handler code here
	GeoMapLandMark* pLandmark = new GeoMapLandMark();
	pLandmark->deleteflag = 0;
	pLandmark->isAuxillary = 1;
	pLandmark->landmark_points = pMesh->m_ArraySharpVertexIndex;
	CVertex3d * pVertex1 = pMesh->GetVertex(pLandmark->landmark_points[0]);
	CVertex3d * pVertex2 = pMesh->GetVertex(pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]);
	double x = pVertex1->x() - pVertex2->x();
	double y = pVertex1->y() - pVertex2->y();
	double z = pVertex1->z() - pVertex2->z();

	if (fabs(y) > fabs(x))
	{
		pLandmark->style = 1;
	}
	else
	{
		pLandmark->style = 0;
	}
	pMesh->m_LandMarkArray.push_back(pLandmark);
	pMesh->m_ArraySharpVertexIndex.clear();
	//SaveLandmark("P:\\Dr_YYJ\\data\\landmarkbak.txt");
	//
	SaveCurrentLandmak();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoBackuplandmarks()
{
	// TODO: Add your control notification handler code here
	pMesh->m_ArrayLandMarks_Original = pMesh->m_LandMarkArray;
}

void Action_PolyOmino::OnBnClickedPolyominoSetauxillarylandmark()
{
	// TODO: Add your control notification handler code here
	if(!pMesh)
		return ;
	for(int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[pMesh->m_SelectedLandmarkIndexArray[i]];
		pLandMark->isAuxillary = !pLandMark->isAuxillary;
	}
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoAuxillaryvisiable()
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		if(pLandMark->isAuxillary)
			pLandMark->isVisible = !pLandMark->isVisible;
	}
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoDeleteauxillarylandmarks()
{
	// TODO: Add your control notification handler code here
	for(int i = pMesh->m_LandMarkArray.size() - 1; i >= 0; i--)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		if(pLandMark->isAuxillary)
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
	}

	//
	SaveCurrentLandmak();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoFixpoints()
{
	// TODO: Add your control notification handler code here
	if(!pMesh)
		return;
	for(int i = 0; i < pMesh->m_ArraySharpVertexIndex.size(); i++)
	{
		int index = pMesh->m_ArraySharpVertexIndex[i];
		std::vector<int>::iterator fiterator;
		fiterator = std::find(pMesh->m_FixVerticesArray.begin(),pMesh->m_FixVerticesArray.end(),index);
		if(fiterator == pMesh->m_FixVerticesArray.end())
		{
			pMesh->m_FixVerticesArray.push_back(index);
		}
	}
	//pMesh->m_FixVerticesArray = pMesh->m_ArraySeletedVertexIndex;
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoUnfixpoints()
{
	// TODO: Add your control notification handler code here
	if(!pMesh)
		return;
	for(int i = 0; i < pMesh->m_ArraySharpVertexIndex.size(); i++)
	{
		int index = pMesh->m_ArraySharpVertexIndex[i];
		std::vector<int>::iterator fiterator;
		fiterator = std::find(pMesh->m_FixVerticesArray.begin(),pMesh->m_FixVerticesArray.end(),index);
		if(fiterator != pMesh->m_FixVerticesArray.end())
		{
			pMesh->m_FixVerticesArray.erase(fiterator);
		}
	}
	//pMesh->m_FixVerticesArray = pMesh->m_ArraySeletedVertexIndex;
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoCopyfixpoints()
{
	// TODO: Add your control notification handler code here
	if(!pMesh)
		return;
	pMesh->m_ArraySharpVertexIndex = pMesh->m_FixVerticesArray;
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::OnBnClickedOminoLoadGeorgiaStreighten()
{
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Georgia\\georgia_county_normalized_33_simplified_cornerflip_streighten.m");

	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	OnBnClickedOminoSetcurrentmesh();
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pMesh->m_showFace = FALSE;

	//Generate Landmarks
	OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message,"D:\\papers\\code\\data\\Geo\\Georgia\\landmark36.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	Loadlandmark(landmarkname);

	//Load Constraints
	//LoadMergeConstraints("D:\\papers\\code\\data\\Geo\\dense\\mergeconstraints.txt");

	//Delete Auxillary Landmarks
	//OnBnClickedPolyominoDeleteauxillarylandmarks();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::OnBnClickedOminoLoadusastreighten()
{
	// TODO: Add your control notification handler code here
	//pDoc->OnOpenDocument("P:\\Dr_YYJ\\code\\data\\Geo\\dense\\USA_Streighten.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\dense\\USA_Streighten.m");
	
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	OnBnClickedOminoSetcurrentmesh();
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pMesh->m_showFace = FALSE;

	//Generate Landmarks
	OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message,"D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark_Fine.txt");
	sprintf(message,"D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	Loadlandmark(landmarkname);

	//Load Constraints
	LoadMergeConstraints("D:\\papers\\code\\data\\Geo\\dense\\mergeconstraints.txt");

	//Delete Auxillary Landmarks
	//OnBnClickedPolyominoDeleteauxillarylandmarks();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::GenerateUniformGridMesh()
{
	if(!pMesh)
		return;

	//Initialization
	Point3D lower,upper;
	pMesh->ComputeBoundingBox(lower,upper);
	pUniformGridMesh = new CMesh3d();
	pDoc->m_SceneGraph.Add(pUniformGridMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);

	std::vector<int> m_ArrayLandmarkEndpoints;
	std::vector<int>::iterator literator;

	//Add All endpoints of the landmark to the uniformgrid
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		if(pLandmark->isAuxillary)
			continue;
		int start = pLandmark->landmark_points[0];
		int end = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
		literator = std::find(m_ArrayLandmarkEndpoints.begin(),m_ArrayLandmarkEndpoints.end(),start);
		if(literator == m_ArrayLandmarkEndpoints.end())
		{
			m_ArrayLandmarkEndpoints.push_back(start);
		}
		literator = std::find(m_ArrayLandmarkEndpoints.begin(),m_ArrayLandmarkEndpoints.end(),end);
		if(literator == m_ArrayLandmarkEndpoints.end())
		{
			m_ArrayLandmarkEndpoints.push_back(end);
		}
	}

	//Add new points to the new mesh
	for(int i = 0; i < m_ArrayLandmarkEndpoints.size(); i++)
	{
		CVertex3d* pCurrentVertex = pMesh->GetVertex(m_ArrayLandmarkEndpoints[i]);
		CVertex3d* pVertex = new CVertex3d(pCurrentVertex);
		pVertex->m_OriginalIndex = m_ArrayLandmarkEndpoints[i];
		pUniformGridMesh->AddVertex(pVertex);
	}

	std::pair<int,int> pair;

	//Add four corner points
	//int vindex1 = pUniformGridMesh->NbVertex();
	//CVertex3d* pVertex = new CVertex3d(lower.x,lower.y,lower.z);
	//pUniformGridMesh->AddVertex(pVertex);
	//pVertex = new CVertex3d(lower.x,upper.y,lower.z);
	//pUniformGridMesh->AddVertex(pVertex);
	//pVertex = new CVertex3d(upper.x,upper.y,lower.z);
	//pUniformGridMesh->AddVertex(pVertex);
	//pVertex = new CVertex3d(upper.x,lower.y,lower.z);
	//pUniformGridMesh->AddVertex(pVertex);

	//pair.first = vindex1;
	//pair.second = vindex1 + 1;
	//pUniformGridMesh->m_ArraySharpEdges.push_back(pair);

	//pair.first = vindex1;
	//pair.second = vindex1 + 3;
	//pUniformGridMesh->m_ArraySharpEdges.push_back(pair);

	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		//if(pLandmark->isAuxillary)
		//	continue;
		int start = pLandmark->landmark_points[0];
		int end = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];

		literator = std::find(m_ArrayLandmarkEndpoints.begin(),m_ArrayLandmarkEndpoints.end(),start);
		int startindex = literator - m_ArrayLandmarkEndpoints.begin();
		literator = std::find(m_ArrayLandmarkEndpoints.begin(),m_ArrayLandmarkEndpoints.end(),end);
		int endindex = literator - m_ArrayLandmarkEndpoints.begin();

		pair.first = startindex;
		pair.second = endindex;
		if(startindex != endindex)
			pUniformGridMesh->m_ArraySharpEdges.push_back(pair);
	}

	//Constraints
	pUniformGridMesh->m_ArrayMergeConstraints = pMesh->m_ArrayMergeConstraints;

	pUniformGridMesh->SetTransform(*pMesh->GetTransform());
	pUniformGridMesh->m_showSharpEdge = TRUE;
	pUniformGridMesh->m_showVertex = TRUE;
}
void Action_PolyOmino::InitializeHorizontalandVerticalSegmentArray()
{
	//If mutiple curves are on the same horizontal or vertical line, combine them together.
	std::vector<int> m_ArrayFlag;
	std::vector<int> segments;
	std::pair<int,int> pair;
	m_HorizontalSegments.clear();
	m_VerticalSegments.clear();
	m_ArrayFlag.resize(pUniformGridMesh->m_ArraySharpEdges.size(),0);
	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		segments.clear();

		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		int start = pair.first;
		int end = pair.second;

		if(m_ArrayFlag[i])
			continue;

		m_ArrayFlag[i] = 1;

		int style = GetSegmentStyle(i);

		segments.push_back(i);

		int nextvindex;	
		int nextsegment;
		nextsegment = i;
		while(1)
		{
			nextsegment = GetConnectedSegment_SameStyle(nextsegment,start,nextvindex);
			if(nextsegment == -1)
				break;
			segments.push_back(nextsegment);
			m_ArrayFlag[nextsegment] = 1;
			start = nextvindex;
		}
		nextsegment = i;
		while(1)
		{
			nextsegment = GetConnectedSegment_SameStyle(nextsegment,end,nextvindex);
			if(nextsegment == -1)
				break;
			segments.push_back(nextsegment);
			m_ArrayFlag[nextsegment] = 1;
			end = nextvindex;
		}

		if(style == 0)
			m_HorizontalSegments.push_back(segments);
		else
			m_VerticalSegments.push_back(segments);
	}

	//Add Vertices of Segments
	std::vector<int> m_vertices;
	std::vector<int>::iterator fiterator;

	//Horizontal lines
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		m_vertices.clear();

		segments = m_HorizontalSegments[i];
		for(int j = 0; j < segments.size(); j++)
		{
			pair = pUniformGridMesh->m_ArraySharpEdges[segments[j]];
			int start = pair.first;
			int end = pair.second;

			fiterator = std::find(m_vertices.begin(),m_vertices.end(),start);
			if(fiterator == m_vertices.end())
				m_vertices.push_back(start);

			fiterator = std::find(m_vertices.begin(),m_vertices.end(),end);
			if(fiterator == m_vertices.end())
				m_vertices.push_back(end);

		}

		//Average the y coordinates
		double sumy = 0;
		for(int j = 0; j < m_vertices.size(); j++)
		{
			CVertex3d* pVertex = pUniformGridMesh->GetVertex(m_vertices[j]);
			sumy += pVertex->y();
		}
		double averagey = sumy / m_vertices.size();

		//Set the y coordinates		
		for(int j = 0; j < m_vertices.size(); j++)
		{
			CVertex3d* pVertex = pUniformGridMesh->GetVertex(m_vertices[j]);
			pVertex->y(averagey);
		}
	}

	//Vertical lines
	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		m_vertices.clear();

		segments = m_VerticalSegments[i];
		for(int j = 0; j < segments.size(); j++)
		{
			pair = pUniformGridMesh->m_ArraySharpEdges[segments[j]];
			int start = pair.first;
			int end = pair.second;

			fiterator = std::find(m_vertices.begin(),m_vertices.end(),start);
			if(fiterator == m_vertices.end())
				m_vertices.push_back(start);

			fiterator = std::find(m_vertices.begin(),m_vertices.end(),end);
			if(fiterator == m_vertices.end())
				m_vertices.push_back(end);

		}

		//Average the y coordinates
		double sumx = 0;
		for(int j = 0; j < m_vertices.size(); j++)
		{
			CVertex3d* pVertex = pUniformGridMesh->GetVertex(m_vertices[j]);
			sumx += pVertex->x();
		}
		double averagex = sumx / m_vertices.size();

		//Set the y coordinates		
		for(int j = 0; j < m_vertices.size(); j++)
		{
			CVertex3d* pVertex = pUniformGridMesh->GetVertex(m_vertices[j]);
			pVertex->x(averagex);
		}
	}

	std::pair<int,int> edgepair;
	double x,y;
	std::vector<double> xinterval,yinterval;
	std::vector<int> valarray;
	//sort
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		int edgeindex = m_HorizontalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
		y = pVertex->y();
		yinterval.push_back(y);
	}
	for(int i = 0; i < yinterval.size(); i++)
		for(int j = 0; j < yinterval.size() - 1; j++)
		{
			//exchange
			if(yinterval[j] > yinterval[j + 1])
			{
				double val = yinterval[j];
				yinterval[j] = yinterval[j + 1];
				yinterval[j + 1] = val;

				valarray = m_HorizontalSegments[j];
				m_HorizontalSegments[j] = m_HorizontalSegments[j + 1];
				m_HorizontalSegments[j + 1] = valarray;
			}
		}

		for(int i = 0; i < m_VerticalSegments.size(); i++)
		{
			int edgeindex = m_VerticalSegments[i][0];
			edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

			CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
			x = pVertex->x();
			xinterval.push_back(x);
		}
		for(int i = 0; i < xinterval.size(); i++)
			for(int j = 0; j < xinterval.size() - 1; j++)
			{
				//exchange
				if(xinterval[j] > xinterval[j + 1])
				{
					double val = xinterval[j];
					xinterval[j] = xinterval[j + 1];
					xinterval[j + 1] = val;

					valarray = m_VerticalSegments[j];
					m_VerticalSegments[j] = m_VerticalSegments[j + 1];
					m_VerticalSegments[j + 1] = valarray;
				}
			}

			for(int i = 0; i < pUniformGridMesh->NbVertex(); i++)
			{
				pUniformGridMesh->GetVertex(i)->m_Flag = i;
			}
}
void Action_PolyOmino::MergeHorizontalVerticalSegments()
{
	std::vector<double> xinterval, yinterval;
	std::pair<int, int> edgepair;
	double x, y;

	for (int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		int edgeindex = m_HorizontalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex = pUniformGridMesh->GetVertex(edgepair.first);
		y = pVertex->y();
		yinterval.push_back(y);
	}

	for (int i = 0; i < m_VerticalSegments.size(); i++)
	{
		int edgeindex = m_VerticalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex = pUniformGridMesh->GetVertex(edgepair.first);
		x = pVertex->x();
		xinterval.push_back(x);
	}

	for (int i = 0; i < xinterval.size() - 1; i++)
	{
		if (fabs(xinterval[i] - xinterval[i + 1]) < 1e-6)
		{
			m_VerticalSegments[i].insert(m_VerticalSegments[i].begin(), m_VerticalSegments[i + 1].begin(), m_VerticalSegments[i + 1].end());
			m_VerticalSegments.erase(m_VerticalSegments.begin() + i + 1);
			xinterval.erase(xinterval.begin() + i + 1);
			i--;
		}
	}
	for (int i = 0; i < yinterval.size() - 1; i++)
	{
		if (fabs(yinterval[i] - yinterval[i + 1]) < 1e-6)
		{
			m_HorizontalSegments[i].insert(m_HorizontalSegments[i].begin(), m_HorizontalSegments[i + 1].begin(), m_HorizontalSegments[i + 1].end());
			m_HorizontalSegments.erase(m_HorizontalSegments.begin() + i + 1);
			yinterval.erase(yinterval.begin() + i + 1);
			i--;
		}
	}
	pUniformGridMesh->xinterval = xinterval;
	pUniformGridMesh->yinterval = yinterval;
}
void Action_PolyOmino::UpdateMergeConstraints(std::vector<int> edgeset,int newindex)
{
	//
	for(int i = 0; i < pUniformGridMesh->m_ArrayMergeConstraints.size(); i++)
	{
		std::pair<int,int> pair = pUniformGridMesh->m_ArrayMergeConstraints[i];
		for(int j = 0; j < edgeset.size(); j++)
		{
			if(pair.first == edgeset[j])
			{
				pair.first = newindex;
			}
			if(pair.second == edgeset[j])
			{
				pair.second = newindex;
			}
		}
		pUniformGridMesh->m_ArrayMergeConstraints[i] = pair;
	}
}
void Action_PolyOmino::UpdateMergeConstraints_Save(std::vector<int> edgeset,int newindex)
{
	std::vector<int> vecset;
	vecset.insert(vecset.begin(),edgeset.begin(),edgeset.end());
	vecset.push_back(newindex);
	m_MergeConstraintsSave.push_back(vecset);
}
void Action_PolyOmino::OnBnClickedUniformgridInitialization()
{
	// TODO: Add your control notification handler code here

	//Generate a uniform grid mesh
	GenerateUniformGridMesh();

	//Update edge connection
	UpdatePointConnectedSelectedEdgeIndex();
	
	//
	InitializeHorizontalandVerticalSegmentArray();

	/*CMesh3d* pTestMesh1 = new CMesh3d(*pUniformGridMesh);
	pDoc->m_SceneGraph.Add(pTestMesh1);
	//CMesh3d* pTestMesh2 = new CMesh3d(*pUniformGridMesh);
	//pDoc->m_SceneGraph.Add(pTestMesh2);

	std::pair<int,int> edge1,edge2;
	//pTestMesh1->m_ArraySeletedVertexIndex.push_back(585);
	//pTestMesh1->m_ArraySeletedVertexIndex.push_back(1324);
	edge1.first = 2040;
	edge1.second = 314;
	//edge2.first = 645;
	//edge2.second = 658;
	//edge1 = pUniformGridMesh->m_ArraySharpEdges[764];
	//edge2 = pUniformGridMesh->m_ArraySharpEdges[1534];
	pTestMesh1->m_ArraySharpEdges.clear();
	pTestMesh1->m_ArraySharpEdges.push_back(edge1);
	//pTestMesh2->m_ArraySharpEdges.clear();
	//pTestMesh2->m_ArraySharpEdges.push_back(edge2);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	return;//*/
	if (CheckHorizontalEdgeIntersection())
	{
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		return;
	}
		//ASSERT(0);

	if (CheckVerticalEdgeIntersection())
		//ASSERT(0);
	{
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		return;
	}
	//Initialization the face num
	m_initialfacenum = ComputeFaceNum();

	SetSharpEdgeFlag();

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
int Action_PolyOmino::ComputeFaceboundingbox(std::vector<std::vector<int>>& m_ArrayFaceArray,int index,Point3D& lower,Point3D& upper)
{
	std::vector<int> pointarray = m_ArrayFaceArray[index];
	lower.setValue(1e10,1e10,0);
	upper.setValue(-1e10,-1e10,0);
	for(int i = 0; i < pointarray.size(); i++)
	{
		int index = pointarray[i];
		CVertex3d* pVertex = pUniformGridMesh->GetVertex(index);
		double x = pVertex->x();
		double y = pVertex->y();
		if(x < lower.x)
			lower.x = x;
		if(y < lower.y)
			lower.y = y;
		if(x > upper.x)
			upper.x = x;
		if(y > upper.y)
			upper.y = y;
	}
	return 1;
}
int Action_PolyOmino::RemoveOuterFace(std::vector<std::vector<int>>& m_ArrayFaceArray)
{
	Point3D lower,upper;
	pUniformGridMesh->ComputeBoundingBox(lower,upper);

	int size = m_ArrayFaceArray.size();
	Point3D lower1,upper1;
	for(int i = 0; i < size; i++)
	{
		ComputeFaceboundingbox(m_ArrayFaceArray,i,lower1,upper1);
		if(Length(lower - lower1) < 1e-8 && Length(upper - upper1) < 1e-8)
		{
			m_ArrayFaceArray.erase(m_ArrayFaceArray.begin() + i);
			break;
		}
	}
	return 1;
}
int Action_PolyOmino::RemoveOuterFace(std::vector<std::vector<int>>& m_ArrayFacePointsArray,std::vector<std::vector<int>>& m_ArrayFaceEdgesArray)
{
	Point3D lower,upper;
	pUniformGridMesh->ComputeBoundingBox(lower,upper);

	int size = m_ArrayFacePointsArray.size();
	Point3D lower1,upper1;
	for(int i = 0; i < size; i++)
	{
		ComputeFaceboundingbox(m_ArrayFacePointsArray,i,lower1,upper1);
		if(Length(lower - lower1) < 1e-5 && Length(upper - upper1) < 1e-5)
		{
			m_ArrayFacePointsArray.erase(m_ArrayFacePointsArray.begin() + i);
			m_ArrayFaceEdgesArray.erase(m_ArrayFaceEdgesArray.begin() + i);
			break;
		}
	}
	return 1;
}
int Action_PolyOmino::SaveObjMesh(int iterationnum)
{
	//Trace the number of faces
	//Half edge structure, each edge with be traced twice and in different directions

	//write a obj file
	std::pair<int,int> halfedge;
	std::vector<std::vector<int>> m_ArrayEdge_flags;
	m_ArrayEdge_flags.resize(pUniformGridMesh->m_ArraySharpEdges.size());

	std::vector<int> m_ArrayFacePoints;
	std::vector<std::vector<int>> m_ArrayFaceArray;
	//get one halfedge from the array
	for(int i = 0; i < m_ArrayEdge_flags.size(); i++)
	{
		m_ArrayFacePoints.clear();
		int size = m_ArrayEdge_flags[i].size();
		if(size ==0)
		{
			if(TraceFace(i,1,m_ArrayEdge_flags,m_ArrayFacePoints))
				m_ArrayFaceArray.push_back(m_ArrayFacePoints);
		}else
			if(size ==1)
			{
				if(TraceFace(i,-m_ArrayEdge_flags[i][0],m_ArrayEdge_flags,m_ArrayFacePoints))
					m_ArrayFaceArray.push_back(m_ArrayFacePoints);
			}
	}

	//Remove the outer face
	RemoveOuterFace(m_ArrayFaceArray);

	//
	char filename[100];
	sprintf(filename,"D:\\papers\\code\\data\\Geo\\dense\\%d.obj",iterationnum);

	CString s1;

	std::ofstream file(filename);

	int vnum = pUniformGridMesh->m_ArrayVertex.GetSize();
	for(int i = 0; i < vnum; i++)
	{
		CVertex3d* pVertex = pUniformGridMesh->GetVertex(i);
		file << "v " << pVertex->x() << " " << pVertex->y() << " 0 " << "\n";
	}
	int fnum = m_ArrayFaceArray.size();
	std::vector<int> fpointarray;
	for(int i = 0; i < fnum; i++)
	{
		file << "f ";
		fpointarray = m_ArrayFaceArray[i];
		for(int j = 0; j < fpointarray.size(); j++)
		{
			if(j < fpointarray.size() - 1)
				file << fpointarray[j] << " ";
			else
				file << fpointarray[j];
		}
		file << "\n";
	}
	file.close();
	return 1;
}
int Action_PolyOmino::ComputeFaceNum()
{
	//Trace the number of faces
	//Half edge structure, each edge with be traced twice and in different directions

	//write a obj file
	std::pair<int,int> halfedge;
	std::vector<std::vector<int>> m_ArrayEdge_flags;
	m_ArrayEdge_flags.resize(pUniformGridMesh->m_ArraySharpEdges.size());

	std::vector<int> m_ArrayFacePoints;
	std::vector<std::vector<int>> m_ArrayFaceArray;
	//get one halfedge from the array
	for(int i = 0; i < m_ArrayEdge_flags.size(); i++)
	{
		m_ArrayFacePoints.clear();
		int size = m_ArrayEdge_flags[i].size();
		if(size == 0)
		{
			TraceFace(i,1,m_ArrayEdge_flags,m_ArrayFacePoints);
			m_ArrayFaceArray.push_back(m_ArrayFacePoints);
		}else
		if(size == 1)
		{
			TraceFace(i,-m_ArrayEdge_flags[i][0],m_ArrayEdge_flags,m_ArrayFacePoints);
			m_ArrayFaceArray.push_back(m_ArrayFacePoints);
		}
	}

	//Remove the outer face
	//RemoveOuterFace(m_ArrayFaceArray);
	/*for(int i = 0; i < m_ArrayFaceArray.size(); i++)
	{
		std::vector<int> pointset = m_ArrayFaceArray[i];
		CMesh3d* pNewMesh = new CMesh3d();
		pNewMesh->Copy(pUniformGridMesh);
		pNewMesh->m_ArraySeletedVertexIndex = pointset;
		pDoc->m_SceneGraph.Add(pNewMesh);
	}
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);*/
	return m_ArrayFaceArray.size() - 1;
}
int Action_PolyOmino::GetConnectedSegment_SameStyle(int segmentindex,int vindex,int& nextvindex)
{
	int style = GetSegmentStyle(segmentindex);
	std::pair<int,int> pair;
	for(int ii = 0; ii < m_ArrayPointConnectedSelectEdgeIndex[vindex].size(); ii++)
	{
		int index = m_ArrayPointConnectedSelectEdgeIndex[vindex][ii];
		int style1 = GetSegmentStyle(index);
		if(index != segmentindex && style == style1)
		{
			pair = pUniformGridMesh->m_ArraySharpEdges[index];
			if(pair.first == vindex)
				nextvindex = pair.second;
			else
				nextvindex = pair.first;
			return index;
		}
	}
	return -1;
}
int Action_PolyOmino::HandleIntersecting_innerholes(int index, int isVertical, int ischeck,int firstvindex,int nextvindex,int edgeindex1,int edgeindex2)
{
	int flag = HandleIntersecting_innerholes_onediretion(index,isVertical,ischeck,firstvindex,nextvindex,edgeindex1,edgeindex2);
	if(!flag)
		flag = HandleIntersecting_innerholes_onediretion(index,isVertical,ischeck,nextvindex,firstvindex,edgeindex1,edgeindex2);
	return flag;
}
int Action_PolyOmino::HandleIntersecting_innerholes_onediretion(int index, int isVertical, int ischeck,int firstvindex,int nextvindex,int edgeindex1,int edgeindex2)
{
	std::vector<int> m_EdgeArray;
	m_EdgeArray.push_back(edgeindex1);
	int vindex = firstvindex;
	int nextvindex1 = nextvindex;
	while(1)
	{
		int nextedgeindex = GetConnectedSegment(edgeindex1,vindex,nextvindex1);
		if(nextedgeindex != -1)
		{
			if(nextedgeindex == edgeindex2)
			{
				m_EdgeArray.push_back(nextedgeindex);

				std::pair<int,int> newedge;
				newedge.first = nextvindex;
				newedge.second = nextvindex1;
				InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
				//pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
				//m_VerticalSegments[i].push_back(pUniformGridMesh->m_ArrayEdges.size() - 1);
				m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),m_EdgeArray.begin(),m_EdgeArray.end());
				//RemoveEdgeSet(m_EdgeArray);
				return 1;
			}else
			{
				m_EdgeArray.push_back(nextedgeindex);
				vindex = nextvindex1;
				edgeindex1 = nextedgeindex;
			}
		}else
			return 0;
	}
}
int Action_PolyOmino::IsDegenerateEdge(int segmentindex)
{
	std::pair<int,int> pair;
	pair = pUniformGridMesh->m_ArraySharpEdges[segmentindex];
	if(pair.first == pair.second)
		return 1;
	else
		return 0;
}
int Action_PolyOmino::GetConnectedUnDegenerateEdgeNum(int vindex)
{
	int num = 0;
	for(int ii = 0; ii < m_ArrayPointConnectedSelectEdgeIndex[vindex].size(); ii++)
	{
		if(IsDegenerateEdge(m_ArrayPointConnectedSelectEdgeIndex[vindex][ii]))
			continue;
		num++;
	}
	return num;
}
int Action_PolyOmino::GetConnectedSegment(int segmentindex,int vindex,int& nextvindex)
{
	if(GetConnectedUnDegenerateEdgeNum(vindex) != 2)
		return -1;

	std::pair<int,int> pair;
	for(int ii = 0; ii < m_ArrayPointConnectedSelectEdgeIndex[vindex].size(); ii++)
	{
		int index = m_ArrayPointConnectedSelectEdgeIndex[vindex][ii];
		if(IsDegenerateEdge(index))
			continue;
		if(index != segmentindex)
		{
			pair = pUniformGridMesh->m_ArraySharpEdges[index];
			if(pair.first == vindex)
				nextvindex = pair.second;
			else
				nextvindex = pair.first;
			return index;
		}
	}
	return -1;
}

int Action_PolyOmino::GetSegmentStyle(int index)
{
	//Get the style of the segement, if horizontal 0, vertical 1
	std::pair<int,int> pair;
	pair = pUniformGridMesh->m_ArraySharpEdges[index];
	int start = pair.first;
	int end = pair.second;

	CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(start);
	CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(end);
	if(fabs(pStartVertex->y() - pEndVertex->y()) >  fabs(pStartVertex->x() - pEndVertex->x()))
		return 1;
	else
		return 0;
}
int Action_PolyOmino::InsertNewSelectedEdge_Save(std::pair<int,int> newedge,int index,int isVertical,int ischeck)
{
	std::vector<int> newedgearray;
	for(int i = 0; i < m_InsertEdgeArray.size(); i++)
	{
		newedgearray = m_InsertEdgeArray[i];
		if(newedgearray[0] == newedge.first && newedgearray[1] == newedge.second ||
			newedgearray[1] == newedge.first && newedgearray[0] == newedge.second)
			return i;
	}
	newedgearray.clear();
	newedgearray.push_back(newedge.first);
	newedgearray.push_back(newedge.second);
	newedgearray.push_back(index);
	newedgearray.push_back(isVertical);
	newedgearray.push_back(ischeck);
	m_InsertEdgeArray.push_back(newedgearray);
	return m_InsertEdgeArray.size() - 1;
}
int Action_PolyOmino::InsertNewSelectedEdge(std::pair<int,int> newedge,int index,int isVertical,int ischeck)
{
	//First Judege whether there is already has such a edge
	std::pair<int,int> edge;
	if(ischeck)
	{
		for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
		{
			edge = pUniformGridMesh->m_ArraySharpEdges[i];
			if(edge.first == newedge.first && edge.second == newedge.second || 
				edge.second == newedge.first && edge.first == newedge.second)
				return i;
		}
	}

	//
	pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
	double x1 = pUniformGridMesh->GetVertex(newedge.first)->x();
	double y1 = pUniformGridMesh->GetVertex(newedge.first)->y();
	double x2 = pUniformGridMesh->GetVertex(newedge.second)->x();
	double y2 = pUniformGridMesh->GetVertex(newedge.second)->y();
	int flag;
	if(isVertical)
	{
		//if(fabs(x1 - x2) > 1e-8)
		//	ASSERT(0);
		m_VerticalSegments[index].push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
		if(y1 < y2)
		{
			flag = 1;
		}else
			flag = 2;
	}
	else
	{
		//if(fabs(y1 - y2) > 1e-8)
		//	ASSERT(0);
		m_HorizontalSegments[index].push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
		if(x1 < x2)
		{
			flag = 3;
		}else
			flag = 4;
	}
	pUniformGridMesh->m_ArraySharpEdgesFlag.push_back(flag);
	return pUniformGridMesh->m_ArraySharpEdges.size() - 1;
}
int Action_PolyOmino::CheckVerticalEdgeIntersection_EdgeArray(std::vector<int> edgeset,int i)
{
	std::pair<int,int> edgepair1,edgepair2;
	int edgesetsize = edgeset.size();
	for(int j = 0; j < edgesetsize; j++)
	{
		for(int k = 0; k < edgesetsize; k++)
		{
			if(j == k)
				continue;
			int edgeindex1 = edgeset[j];
			int edgeindex2 = edgeset[k];

			edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
			edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];
			CVertex3d* pEdge1Start = pUniformGridMesh->GetVertex(edgepair1.first);
			CVertex3d* pEdge1End = pUniformGridMesh->GetVertex(edgepair1.second);
			CVertex3d* pEdge2Start = pUniformGridMesh->GetVertex(edgepair2.first);
			CVertex3d* pEdge2End = pUniformGridMesh->GetVertex(edgepair2.second);

			double edge1starty = pEdge1Start->y();
			double edge1endy = pEdge1End->y();
			double edge2starty = pEdge2Start->y();
			double edge2endy = pEdge2End->y();
			int flag = 0;

			if(edge1endy > edge2starty + TOL && edge1endy < edge2endy - TOL || edge1starty > edge2starty + TOL && edge1starty < edge2endy - TOL ||
				edge1endy > edge2endy + TOL && edge1endy < edge2starty - TOL || edge1starty > edge2endy + TOL && edge1starty < edge2starty - TOL ||
				edge2endy > edge1starty + TOL && edge2endy < edge1endy - TOL || edge2starty > edge1starty + TOL && edge2starty < edge1endy - TOL ||
				edge2endy > edge1endy + TOL && edge2endy < edge1starty - TOL || edge2starty > edge1endy + TOL && edge2starty < edge1starty - TOL)
			{
				if (pUniformGridMesh)
				{
					pUniformGridMesh->m_ArraySharpVertexIndex.push_back(edgepair1.first);
					pUniformGridMesh->m_ArraySharpVertexIndex.push_back(edgepair1.second);
					pUniformGridMesh->m_ArraySharpVertexIndex.push_back(edgepair2.first);
					pUniformGridMesh->m_ArraySharpVertexIndex.push_back(edgepair2.second);
				}
				if (pMesh)
				{
					pMesh->m_ArraySharpVertexIndex.push_back(pEdge1Start->m_OriginalIndex);
					pMesh->m_ArraySharpVertexIndex.push_back(pEdge1End->m_OriginalIndex);
					pMesh->m_ArraySharpVertexIndex.push_back(pEdge2Start->m_OriginalIndex);
					pMesh->m_ArraySharpVertexIndex.push_back(pEdge2End->m_OriginalIndex);
				}
				return 1;
			}
		}
	}
	return 0;
}
int Action_PolyOmino::DetectandHandleVerticalIntersectingEdges_EdgeArray(std::vector<int> edgeset,int i)
{
	std::pair<int,int> edgepair1,edgepair2;
	int edgesetsize = edgeset.size();
	for(int j = 0; j < edgesetsize; j++)
	{
		for(int k = j + 1; k < edgesetsize; k++)
		{
			//if(j == k)
			//	continue;
			int edgeindex1 = edgeset[j];
			int edgeindex2 = edgeset[k];

			if(std::find(m_DeletedEdgeArray.begin(),m_DeletedEdgeArray.end(),edgeindex1) != m_DeletedEdgeArray.end())
				continue;
			if(std::find(m_DeletedEdgeArray.begin(),m_DeletedEdgeArray.end(),edgeindex2) != m_DeletedEdgeArray.end())
				continue;

			edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
			edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];
			CVertex3d* pEdge1Start = pUniformGridMesh->GetVertex(edgepair1.first);
			CVertex3d* pEdge1End = pUniformGridMesh->GetVertex(edgepair1.second);
			CVertex3d* pEdge2Start = pUniformGridMesh->GetVertex(edgepair2.first);
			CVertex3d* pEdge2End = pUniformGridMesh->GetVertex(edgepair2.second);

			double edge1starty = pEdge1Start->y();
			double edge1endy = pEdge1End->y();
			double edge2starty = pEdge2Start->y();
			double edge2endy = pEdge2End->y();
			int flag = 0;

			if(edge1endy > edge2starty + TOL && edge1endy < edge2endy - TOL || edge1starty > edge2starty + TOL && edge1starty < edge2endy - TOL||
				edge1endy > edge2endy + TOL && edge1endy < edge2starty - TOL || edge1starty > edge2endy + TOL && edge1starty < edge2starty - TOL ||
				edge2endy > edge1starty + TOL && edge2endy < edge1endy - TOL || edge2starty > edge1starty + TOL && edge2starty < edge1endy - TOL||
				edge2endy > edge1endy + TOL && edge2endy < edge1starty - TOL || edge2starty > edge1endy + TOL && edge2starty < edge1starty - TOL)
			{
				TRACE("\n4\n%d\n%d\n%d\n%d\n", edgepair1.first, edgepair1.second, edgepair2.first, edgepair2.second);
				//TRACE("\n");
				//TRACE("%d\n", edgepair1.first);
				//TRACE("%d\n", edgepair1.second);
				//TRACE("%d\n", edgepair2.first);
				//TRACE("%d\n", edgepair2.second);
				//Invalid
				//Case 1: Just add new edges
				flag = HandleIntersecting_General(i,1,0,edgeindex1,edgeindex2);

				//Case 2: delete the inner hole
				if(!flag)
					flag = HandleIntersecting_innerholes(i,1,0,edgepair1.second,edgepair1.first,edgeindex1,edgeindex2);

				//Case 3: Remove the additional face
				if(!flag)
					flag = HandleIntersecting_AditionalFace(i,1,0,edgeindex1,edgeindex2);

				if(!flag)
					ASSERT(0);

				if(flag)
				{
					IsUniformGridChanged = TRUE;
				}
				//return flag;
			}
		}
	}
	return 1;
}
int Action_PolyOmino::CheckVerticalEdgeIntersection()
{
	std::vector<int> edgeset;

	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		edgeset = m_VerticalSegments[i];

		int flag = CheckVerticalEdgeIntersection_EdgeArray(edgeset,i);
		if(flag)
		{
			return 1;
		}
	}
	return 0;
}
void Action_PolyOmino::DetectandHandleVerticalIntersectingEdges()
{
	std::vector<int> edgeset;

	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		edgeset = m_VerticalSegments[i];

		int flag = DetectandHandleVerticalIntersectingEdges_EdgeArray(edgeset,i);
	}
}
int Action_PolyOmino::SameCoordinate(CVertex3d* pVertex1,CVertex3d* pVertex2)
{
	if(fabs(pVertex1->x() - pVertex2->x()) < 1e-8 && fabs(pVertex1->y() - pVertex2->y()) < 1e-8 && fabs(pVertex1->z() - pVertex2->z()) < 1e-8)
		return 1;
	return 0;
}

int Action_PolyOmino::HandleIntersecting_General(int index, int isVertical, int ischeck,int edgeindex1,int edgeindex2)
{
	std::pair<int,int> edgepair1,edgepair2;
	int newedgeindex,newedgeindex1,newedgeindex2; 
	edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
	edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];
	CVertex3d* pEdge1Start = pUniformGridMesh->GetVertex(edgepair1.first);
	CVertex3d* pEdge1End = pUniformGridMesh->GetVertex(edgepair1.second);
	CVertex3d* pEdge2Start = pUniformGridMesh->GetVertex(edgepair2.first);
	CVertex3d* pEdge2End = pUniformGridMesh->GetVertex(edgepair2.second);
	std::vector<int> edgeset;
	edgeset.push_back(edgeindex1);
	edgeset.push_back(edgeindex2);
	if(SameCoordinate(pEdge1Start,pEdge2Start) || SameCoordinate(pEdge1Start,pEdge2End) ||
		SameCoordinate(pEdge1End,pEdge2Start) || SameCoordinate(pEdge1End,pEdge2End))
	//if(pEdge1Start == pEdge2Start || pEdge1Start == pEdge2End ||
	//	pEdge1End == pEdge2Start || pEdge1End == pEdge2End)
	{
		double edge1start,edge1end,edge2start,edge2end;
		if(isVertical)
		{
			edge1start = pEdge1Start->y();
			edge1end = pEdge1End->y();
			edge2start = pEdge2Start->y();
			edge2end = pEdge2End->y();
		}else
		{
			edge1start = pEdge1Start->x();
			edge1end = pEdge1End->x();
			edge2start = pEdge2Start->x();
			edge2end = pEdge2End->x();
		}

		std::pair<int,double> pair;
		std::vector<std::pair<int,double>> fourpointsarray;

		//first add the end points of the two segments
		pair.first = edgepair1.first;
		pair.second = edge1start;
		fourpointsarray.push_back(pair);

		pair.first = edgepair1.second;
		pair.second = edge1end;
		fourpointsarray.push_back(pair);

		pair.first = edgepair2.first;
		pair.second = edge2start;
		fourpointsarray.push_back(pair);

		pair.first = edgepair2.second;
		pair.second = edge2end;
		fourpointsarray.push_back(pair);

		struct sort_pred {
			bool operator()(const std::pair<int,double> &left, const std::pair<int,double> &right) {
				return left.second < right.second;
			}
		};

		std::sort(fourpointsarray.begin(), fourpointsarray.end(), sort_pred());

		std::pair<int,int> newedge;
		/*newedge.first = fourpointsarray[0].first;
		newedge.second = fourpointsarray[1].first;

		if(newedge.first != newedge.second)
		{
			InsertNewSelectedEdge(newedge,index,isVertical,ischeck);
			//pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
			//m_VerticalSegments[i].push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
		}

		newedge.first = fourpointsarray[1].first;
		newedge.second = fourpointsarray[2].first;
		if(newedge.first != newedge.second)
		{
			InsertNewSelectedEdge(newedge,index,isVertical,ischeck);
			//pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
			//m_VerticalSegments[i].push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
		}*/

		if(fabs(fourpointsarray[0].second - fourpointsarray[1].second) < 1e-8)
		{
			if(m_ArrayPointConnectedSelectEdgeIndex[fourpointsarray[0].first].size() == 2 &&
				m_ArrayPointConnectedSelectEdgeIndex[fourpointsarray[1].first].size() == 2)
			{
				newedge.first = fourpointsarray[2].first;
				newedge.second = fourpointsarray[3].first;
				newedgeindex = InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
			}else
			{
				newedge.first = fourpointsarray[1].first;
				newedge.second = fourpointsarray[2].first;
				newedgeindex = InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
				newedge.first = fourpointsarray[2].first;
				newedge.second = fourpointsarray[3].first;
				newedgeindex = InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
			}

		}else
		if(fabs(fourpointsarray[2].second - fourpointsarray[3].second) < 1e-8)
		{
			if(m_ArrayPointConnectedSelectEdgeIndex[fourpointsarray[2].first].size() == 2 &&
				m_ArrayPointConnectedSelectEdgeIndex[fourpointsarray[3].first].size() == 2)
			{
				newedge.first = fourpointsarray[0].first;
				newedge.second = fourpointsarray[1].first;
				newedgeindex = InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
			}else
			{
				newedge.first = fourpointsarray[0].first;
				newedge.second = fourpointsarray[1].first;
				newedgeindex = InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
				newedge.first = fourpointsarray[1].first;
				newedge.second = fourpointsarray[2].first;
				newedgeindex = InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
			}
		}else
			ASSERT(0);

		//Update Merge Constraints
		/*for(int i = 0; i < pUniformGridMesh->m_ArrayMergeConstraints.size(); i++)
		{
			std::pair<int,int> pair = pUniformGridMesh->m_ArrayMergeConstraints[i];
			if(pair.first == edgeindex1 || pair.first == edgeindex2)
				pair.first = newedgeindex;
			if(pair.second == edgeindex1 || pair.second == edgeindex2)
				pair.second = newedgeindex;
			pUniformGridMesh->m_ArrayMergeConstraints[i] = pair;
		}*/
		std::vector<int> m_EdgeArray;
		m_EdgeArray.clear();
		m_EdgeArray.push_back(edgeindex1);
		m_EdgeArray.push_back(edgeindex2);

		UpdateMergeConstraints_Save(m_EdgeArray,newedgeindex);
		m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),m_EdgeArray.begin(),m_EdgeArray.end());

		//RemoveEdgeSet(m_EdgeArray);
		return 1;
	}
	return 0;
}
int Action_PolyOmino::HandleIntersecting_General_NOJudgement(int index, int isVertical, int ischeck,int edgeindex1,int edgeindex2)
{
	std::pair<int,int> edgepair1,edgepair2;
	edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
	edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];
	CVertex3d* pEdge1Start = pUniformGridMesh->GetVertex(edgepair1.first);
	CVertex3d* pEdge1End = pUniformGridMesh->GetVertex(edgepair1.second);
	CVertex3d* pEdge2Start = pUniformGridMesh->GetVertex(edgepair2.first);
	CVertex3d* pEdge2End = pUniformGridMesh->GetVertex(edgepair2.second);
	int newedgeindex;
	std::vector<int> m_EdgeArray;
	m_EdgeArray.clear();
	m_EdgeArray.push_back(edgeindex1);
	m_EdgeArray.push_back(edgeindex2);

	//if(pEdge1Start == pEdge2Start || pEdge1Start == pEdge2End ||
	//	pEdge1End == pEdge2Start || pEdge1End == pEdge2End)
	//{
		double edge1start,edge1end,edge2start,edge2end;
		if(isVertical)
		{
			edge1start = pEdge1Start->y();
			edge1end = pEdge1End->y();
			edge2start = pEdge2Start->y();
			edge2end = pEdge2End->y();
		}else
		{
			edge1start = pEdge1Start->x();
			edge1end = pEdge1End->x();
			edge2start = pEdge2Start->x();
			edge2end = pEdge2End->x();
		}

		std::pair<int,double> pair;
		std::vector<std::pair<int,double>> fourpointsarray;

		//first add the end points of the two segments
		pair.first = edgepair1.first;
		pair.second = edge1start;
		fourpointsarray.push_back(pair);

		pair.first = edgepair1.second;
		pair.second = edge1end;
		fourpointsarray.push_back(pair);

		pair.first = edgepair2.first;
		pair.second = edge2start;
		fourpointsarray.push_back(pair);

		pair.first = edgepair2.second;
		pair.second = edge2end;
		fourpointsarray.push_back(pair);

		struct sort_pred {
			bool operator()(const std::pair<int,double> &left, const std::pair<int,double> &right) {
				return left.second < right.second;
			}
		};

		std::sort(fourpointsarray.begin(), fourpointsarray.end(), sort_pred());

		std::pair<int,int> newedge;
		newedge.first = fourpointsarray[0].first;
		newedge.second = fourpointsarray[1].first;

		if(newedge.first != newedge.second)
		{
			newedgeindex = InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
			UpdateMergeConstraints_Save(m_EdgeArray,newedgeindex);
			//pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
			//m_VerticalSegments[i].push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
		}

		newedge.first = fourpointsarray[1].first;
		newedge.second = fourpointsarray[2].first;
		if(newedge.first != newedge.second)
		{
			newedgeindex = InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
			UpdateMergeConstraints_Save(m_EdgeArray,newedgeindex);
			//pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
			//m_VerticalSegments[i].push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
		}

		newedge.first = fourpointsarray[2].first;
		newedge.second = fourpointsarray[3].first;
		if(newedge.first != newedge.second)
		{
			newedgeindex = InsertNewSelectedEdge_Save(newedge,index,isVertical,ischeck);
			UpdateMergeConstraints_Save(m_EdgeArray,newedgeindex);
			//pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
			//m_VerticalSegments[i].push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
		}

		//std::vector<int> m_EdgeArray;
		//m_EdgeArray.clear();
		//m_EdgeArray.push_back(edgeindex1);
		//m_EdgeArray.push_back(edgeindex2);
		//RemoveEdgeSet(m_EdgeArray);
		return 1;
	//}
	//return 0;
}
int Action_PolyOmino::TraceFace(int edgeindex1,int direction,std::vector<std::vector<int>>& m_ArrayEdge_flags,std::vector<int>& m_ArrayPoints)
{
	std::vector<int>::iterator eiterator;
	std::vector<int> edgeflags = m_ArrayEdge_flags[edgeindex1];
	eiterator = std::find(edgeflags.begin(),edgeflags.end(),direction);
	if(eiterator != edgeflags.end())
		return 0;
	m_ArrayEdge_flags[edgeindex1].push_back(direction);

	std::pair<int,int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
	int firstvindex;
	if(direction == 1)
	{
		firstvindex = pair.second;
	}else
	{
		firstvindex = pair.first;
	}

	int vindex = firstvindex;

	int vindex1;
	int edgeindex = edgeindex1;
	int nextedgeindex;
	m_ArrayPoints.push_back(firstvindex);
	int direction1;
	while(1)
	{
		nextedgeindex = GetConnectedDirectedSegment(edgeindex,vindex,vindex1);
		
		if(nextedgeindex == -1)
			break;

		if(nextedgeindex == edgeindex1)
			break;

		m_ArrayPoints.push_back(vindex1);
		if(m_ArrayPoints.size() > pUniformGridMesh->m_ArrayVertex.GetSize())
			ASSERT(0);

		if(nextedgeindex != -1)
		{
			std::pair<int,int> pair1 = pUniformGridMesh->m_ArraySharpEdges[nextedgeindex];
			if(vindex1 == pair1.first)
				direction1 = -1;
			else 
				direction1 = 1;
			edgeindex = nextedgeindex;
			vindex = vindex1;
			m_ArrayEdge_flags[edgeindex].push_back(direction1);
		}
	}
	return 1;
}
int Action_PolyOmino::TraceFace(int edgeindex1,int direction,std::vector<std::vector<int>>& m_ArrayEdge_flags,std::vector<int>& m_ArrayPoints,std::vector<int>& m_ArrayEdges)
{
	std::vector<int>::iterator eiterator;
	std::vector<int> edgeflags = m_ArrayEdge_flags[edgeindex1];
	eiterator = std::find(edgeflags.begin(),edgeflags.end(),direction);
	if(eiterator != edgeflags.end())
		return 0;
	m_ArrayEdge_flags[edgeindex1].push_back(direction);
	m_ArrayEdges.push_back(edgeindex1);

	std::pair<int,int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
	int firstvindex;
	if(direction == 1)
	{
		firstvindex = pair.second;
	}else
	{
		firstvindex = pair.first;
	}

	int vindex = firstvindex;

	int vindex1;
	int edgeindex = edgeindex1;
	int nextedgeindex;
	m_ArrayPoints.push_back(firstvindex);
	int direction1;
	while(1)
	{
		nextedgeindex = GetConnectedDirectedSegment(edgeindex,vindex,vindex1);

		if(nextedgeindex == -1)
			break;

		if(nextedgeindex == edgeindex1)
			break;

		m_ArrayPoints.push_back(vindex1);
		m_ArrayEdges.push_back(nextedgeindex);

		if(m_ArrayPoints.size() > pUniformGridMesh->m_ArrayVertex.GetSize())
			ASSERT(0);

		if(nextedgeindex != -1)
		{
			std::pair<int,int> pair1 = pUniformGridMesh->m_ArraySharpEdges[nextedgeindex];
			if(vindex1 == pair1.first)
				direction1 = -1;
			else 
				direction1 = 1;
			edgeindex = nextedgeindex;
			vindex = vindex1;
			m_ArrayEdge_flags[edgeindex].push_back(direction1);
		}
	}
	return 1;
}
int Action_PolyOmino::GetConnectedDirectedSegmentArray(int edgeindex1,int edgeindex2,int firstvindex,int nextvindex,std::vector<int>& m_EdgeArray)
{
	int vindex = firstvindex;
	int vindex1;
	int edgeindex = edgeindex1;
	int nextedgeindex;
	m_EdgeArray.push_back(edgeindex);
	while(1)
	{
		nextedgeindex = GetConnectedDirectedSegment(edgeindex,vindex,vindex1,edgeindex2);
		if(nextedgeindex != -1)
		{
			edgeindex = nextedgeindex;
			vindex = vindex1;
			m_EdgeArray.push_back(edgeindex);
		}else
			break;                                                                                                                                                                      
		if(nextedgeindex == edgeindex1)
			break;
		if(m_EdgeArray.size() > pUniformGridMesh->m_ArrayVertex.GetSize())
			ASSERT(0);
	}
	std::vector<int>::iterator eiterator;
	eiterator = std::find(m_EdgeArray.begin(),m_EdgeArray.end(),edgeindex2);
	if(eiterator == m_EdgeArray.end())
	{
		m_EdgeArray.clear();
		vindex = nextvindex;
		edgeindex = edgeindex1;
		m_EdgeArray.push_back(edgeindex);
		while(1)
		{
			nextedgeindex = GetConnectedDirectedSegment(edgeindex,vindex,vindex1,edgeindex2);
			if(nextedgeindex != -1)
			{
				edgeindex = nextedgeindex;
				vindex = vindex1;
				m_EdgeArray.push_back(edgeindex);
			}else
			break;
			if(nextedgeindex == edgeindex1)
				break;

			if(m_EdgeArray.size() > pUniformGridMesh->m_ArrayVertex.GetSize())
				ASSERT(0);
		}
		std::vector<int>::iterator eiterator;
		eiterator = std::find(m_EdgeArray.begin(),m_EdgeArray.end(),edgeindex2);
		if(eiterator == m_EdgeArray.end())
			ASSERT(0);
	}
	return 1;
}
int Action_PolyOmino::GetConnectedDirectedSegment(int segmentindex,int vindex,int& nextvindex)
{
	//if(m_ArrayPointConnectedSelectEdgeIndex[vindex].size() != 2)
	//	return -1;
	int startindex,endindex;
	std::pair<int,int> pair;
	pair = pUniformGridMesh->m_ArraySharpEdges[segmentindex];
	if(pair.first == vindex)
	{
		startindex = pair.second;
		endindex = vindex;
	}else
	{
		startindex = pair.first;
		endindex = vindex;
	}
	Vector3D startpoint(pUniformGridMesh->GetVertex(startindex)->x(),pUniformGridMesh->GetVertex(startindex)->y(),0);
	Vector3D endpoint(pUniformGridMesh->GetVertex(endindex)->x(),pUniformGridMesh->GetVertex(endindex)->y(),0);
	Vector3D edgevec = endpoint - startpoint;

	if(m_ArrayPointConnectedSelectEdgeIndex[vindex].size() == 2)
	{
		std::pair<int,int> pair;
		for(int ii = 0; ii < m_ArrayPointConnectedSelectEdgeIndex[vindex].size(); ii++)
		{
			int index = m_ArrayPointConnectedSelectEdgeIndex[vindex][ii];
			if(IsDegenerateEdge(index))
				continue;
			if(index != segmentindex)
			{
				pair = pUniformGridMesh->m_ArraySharpEdges[index];
				if(pair.first == vindex)
					nextvindex = pair.second;
				else
					nextvindex = pair.first;
				return index;
			}
		}
	}else
	{
		int startindex1,endindex1;
		std::pair<int,double> pair1;
		std::vector<std::pair<int,double>> m_ArraySharpEdges;
		//sort the edges
		std::pair<int,int> pair;
		for(int ii = 0; ii < m_ArrayPointConnectedSelectEdgeIndex[vindex].size(); ii++)
		{
			int index = m_ArrayPointConnectedSelectEdgeIndex[vindex][ii];
			if(index == segmentindex)
				continue;
			
			if(IsDegenerateEdge(index))
				continue;

			pair = pUniformGridMesh->m_ArraySharpEdges[index];
			//zero length edge,ignore it
			if(pair.first == pair.second)
				continue;
			startindex1 = vindex;
			if(pair.first == vindex)
			{
				endindex1 = pair.second;
			}
			else
				endindex1 = pair.first;

			Vector3D startpoint1(pUniformGridMesh->GetVertex(startindex1)->x(),pUniformGridMesh->GetVertex(startindex1)->y(),0);
			Vector3D endpoint1(pUniformGridMesh->GetVertex(endindex1)->x(),pUniformGridMesh->GetVertex(endindex1)->y(),0);
			Vector3D edgevec1 = endpoint1 - startpoint1;
			edgevec.SetUnit();
			edgevec1.SetUnit();
			Vector3D zaxis = edgevec ^ edgevec1;
			pair1.first = index;
			pair1.second = zaxis[2];
		
			m_ArraySharpEdges.push_back(pair1);
		}
		struct sort_pred {
			bool operator()(const std::pair<int,double> &left, const std::pair<int,double> &right) {
				return left.second < right.second;
			}
		};
		int index;
		std::sort(m_ArraySharpEdges.begin(),m_ArraySharpEdges.end(),sort_pred());
		if (m_ArraySharpEdges.size() == 2 && ((segmentindex == 4871 && vindex == 4570) || (segmentindex == 6753 && vindex == 2281) || (segmentindex == 5407 && vindex == 2339) || (segmentindex == 4875 && vindex == 4705)))
			//|| (segmentindex == 6571 && vindex == 1142)|| (segmentindex == 6047 && vindex == 1149) || (segmentindex == 8177 && vindex == 1667)
		{
			if (m_ArraySharpEdges[0].first == 4640 && m_ArraySharpEdges[1].first == 6558)
			{
				index = 4640;
			}
			else 
			if (m_ArraySharpEdges[0].first == 6757 && m_ArraySharpEdges[1].first == 6755)
			{
					index = 6757;
			}else
			if (m_ArraySharpEdges[0].first == 5406 && m_ArraySharpEdges[1].first == 1780)
			{
				index = 5406;
			}
			else
				if (m_ArraySharpEdges[0].first == 5571 && m_ArraySharpEdges[1].first == 4872)
				{
					index = 5571;
				}
			/*else
				if (m_ArraySharpEdges[0].first == 921 && m_ArraySharpEdges[1].first == 941)
				{
					index = 921;
				}
				else
					if (m_ArraySharpEdges[0].first == 1262 && m_ArraySharpEdges[1].first == 1261)
					{
						index = 1262;
					}
					else
						if (m_ArraySharpEdges[0].first == 1587 && m_ArraySharpEdges[1].first == 4976)
						{
							index = 1587;
						}*/
						else
					{
						index = m_ArraySharpEdges[m_ArraySharpEdges.size() - 1].first;
					}
		}
		else
		{
			index = m_ArraySharpEdges[m_ArraySharpEdges.size() - 1].first;
		}

	
		pair = pUniformGridMesh->m_ArraySharpEdges[index];
		if(pair.first == vindex)
		{
			nextvindex = pair.second;
		}
		else
			nextvindex = pair.first;
		return index;
	}
	return -1;
}
int Action_PolyOmino::GetConnectedDirectedSegment(int segmentindex,int vindex,int& nextvindex,int passingedge)
{
	//if(m_ArrayPointConnectedSelectEdgeIndex[vindex].size() != 2)
	//	return -1;
	int startindex,endindex;
	std::pair<int,int> pair;
	int direction;
	pair = pUniformGridMesh->m_ArraySharpEdges[segmentindex];
	if(pair.first == vindex)
	{
		startindex = pair.second;
		endindex = vindex;
		direction = 0;
	}else
	{
		startindex = pair.first;
		endindex = vindex;
		direction = 1;
	}
	//Vector3D startpoint(pUniformGridMesh->GetVertex(startindex)->x(),pUniformGridMesh->GetVertex(startindex)->y(),0);
	//Vector3D endpoint(pUniformGridMesh->GetVertex(endindex)->x(),pUniformGridMesh->GetVertex(endindex)->y(),0);
	//Vector3D edgevec = endpoint - startpoint;
	Vector3D edgevec;
	GetDirectionVector(segmentindex,direction,edgevec);

	if(m_ArrayPointConnectedSelectEdgeIndex[vindex].size() == 2)
	{
		std::pair<int,int> pair;
		for(int ii = 0; ii < m_ArrayPointConnectedSelectEdgeIndex[vindex].size(); ii++)
		{
			int index = m_ArrayPointConnectedSelectEdgeIndex[vindex][ii];
			if(IsDegenerateEdge(index))
				continue;
			if(index != segmentindex)
			{
				pair = pUniformGridMesh->m_ArraySharpEdges[index];
				if(pair.first == vindex)
					nextvindex = pair.second;
				else
					nextvindex = pair.first;
				return index;
			}
		}
	}else
	{
		int startindex1,endindex1;
		std::pair<int,std::pair<double,double>> pair1;
		std::vector<std::pair<int,std::pair<double,double>>> m_ArraySharpEdges;
		//sort the edges
		std::pair<int,int> pair;
		for(int ii = 0; ii < m_ArrayPointConnectedSelectEdgeIndex[vindex].size(); ii++)
		{
			int index = m_ArrayPointConnectedSelectEdgeIndex[vindex][ii];

			if(index == segmentindex)
				continue;

			//if(IsDegenerateEdge(index))
			//	continue;

			/*if(index == passingedge)
			{
				//must passing the passingedge
				pair = pUniformGridMesh->m_ArraySharpEdges[index];
				if(pair.first == vindex)
				{
					nextvindex = pair.second;
				}
				else
					nextvindex = pair.first;
				return index;
			}*/

			pair = pUniformGridMesh->m_ArraySharpEdges[index];
			//zero length edge,ignore it
			//if(pair.first == pair.second)
			//	continue;
			startindex1 = vindex;

			if(pair.first == vindex)
			{
				endindex1 = pair.second;
				direction = 1;
			}
			else
			{
				endindex1 = pair.first;
				direction = 0;
			}
			Vector3D edgevec1;
			GetDirectionVector(index,direction,edgevec1);
			//Vector3D startpoint1(pUniformGridMesh->GetVertex(startindex1)->x(),pUniformGridMesh->GetVertex(startindex1)->y(),0);
			//Vector3D endpoint1(pUniformGridMesh->GetVertex(endindex1)->x(),pUniformGridMesh->GetVertex(endindex1)->y(),0);
			//Vector3D edgevec1 = endpoint1 - startpoint1;
			edgevec.SetUnit();
			//edgevec1.SetUnit();
			Vector3D zaxis = edgevec ^ edgevec1;
			double val = edgevec * edgevec1;
			pair1.first = index;
			pair1.second.first = zaxis[2];
			pair1.second.second = val;

			m_ArraySharpEdges.push_back(pair1);
		}
		struct sort_pred {
			bool operator()(const std::pair<int,std::pair<double,double>> &left, const std::pair<int,std::pair<double,double>> &right) 
			{
				if(fabs(left.second.first - right.second.first) < 1e-6)
				{
					return left.second.second > right.second.second;
				}else
				{
					return left.second.first < right.second.first;
				}
			}
		};
		std::sort(m_ArraySharpEdges.begin(),m_ArraySharpEdges.end(),sort_pred());
		int sharpedgenum = m_ArraySharpEdges.size();
		int index = m_ArraySharpEdges[sharpedgenum - 1].first;
		int index1 = sharpedgenum - 2;
		while(index1 >= 0 && fabs(m_ArraySharpEdges[index1].second.first -  m_ArraySharpEdges[sharpedgenum - 1].second.first) < 1e-6)
		{
			if(m_ArraySharpEdges[index1].first == passingedge)
			{
				index = passingedge;
				break;
			}else
			{
				index1--;
			}
		}
		pair = pUniformGridMesh->m_ArraySharpEdges[index];
		if(pair.first == vindex)
		{
			nextvindex = pair.second;
		}
		else
			nextvindex = pair.first;
		return index;
	}
	return -1;
}
int Action_PolyOmino::GetDirectionVector(int index,int direction,Vector3D& edgevec1)
{
	//Vector3D edgevec1;
	if(pUniformGridMesh->m_ArraySharpEdgesFlag[index] == 1)
	{
		if(direction)
		{
			edgevec1[0] = 0;
			edgevec1[1] = 1;
			edgevec1[2] = 0;
		}else
		{
			edgevec1[0] = 0;
			edgevec1[1] = -1;
			edgevec1[2] = 0;
		}
	}else
		if(pUniformGridMesh->m_ArraySharpEdgesFlag[index] == 2)
		{
			if(direction)
			{
				edgevec1[0] = 0;
				edgevec1[1] = -1;
				edgevec1[2] = 0;
			}else
			{
				edgevec1[0] = 0;
				edgevec1[1] = 1;
				edgevec1[2] = 0;
			}		
		}else
			if(pUniformGridMesh->m_ArraySharpEdgesFlag[index] == 3)
			{
				if(direction)
				{
					edgevec1[0] = 1;
					edgevec1[1] = 0;
					edgevec1[2] = 0;
				}else
				{
					edgevec1[0] = -1;
					edgevec1[1] = 0;
					edgevec1[2] = 0;
				}		
			}
			else
				if(pUniformGridMesh->m_ArraySharpEdgesFlag[index] == 4)
				{
					if(direction)
					{
						edgevec1[0] = -1;
						edgevec1[1] = 0;
						edgevec1[2] = 0;
					}else
					{
						edgevec1[0] = 1;
						edgevec1[1] = 0;
						edgevec1[2] = 0;
					}		
				}
	return 1;
}
int Action_PolyOmino::JudgewhetherTwoedgesOntheSameFace(int edgeindex1,int edgeindex2)
{	
	std::pair<int,int> edgepair1,edgepair2;
	edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
	edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];
	CVertex3d* pEdge1Start = pUniformGridMesh->GetVertex(edgepair1.first);
	CVertex3d* pEdge1End = pUniformGridMesh->GetVertex(edgepair1.second);
	CVertex3d* pEdge2Start = pUniformGridMesh->GetVertex(edgepair2.first);
	CVertex3d* pEdge2End = pUniformGridMesh->GetVertex(edgepair2.second);

	//Find the left face of pEdge1Start->pEdge1End

	return 1;
}
int Action_PolyOmino::CheckHorizontalEdgeIntersection()
{
	int edgeindex1,edgeindex2;
	std::vector<int> edgeset;
	int edgesetsize;
	std::pair<int,int> edgepair1,edgepair2;
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		edgeset = m_HorizontalSegments[i];
		edgesetsize = edgeset.size();

		int flag = CheckHorizontalEdgeIntersection_EdgeArray(edgeset,i);
		if(flag)
		{
			return 1;
		}
	}
	return 0;
}
int Action_PolyOmino::CheckHorizontalEdgeIntersection_EdgeArray(std::vector<int> edgeset,int i)
{
	std::pair<int,int> edgepair1,edgepair2;
	int edgesetsize = edgeset.size();
	for(int j = 0; j < edgesetsize; j++)
	{
		for(int k = 0; k < edgesetsize; k++)
		{
			if(j == k)
				continue;
			int edgeindex1 = edgeset[j];
			int edgeindex2 = edgeset[k];

			edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
			edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];
			CVertex3d* pEdge1Start = pUniformGridMesh->GetVertex(edgepair1.first);
			CVertex3d* pEdge1End = pUniformGridMesh->GetVertex(edgepair1.second);
			CVertex3d* pEdge2Start = pUniformGridMesh->GetVertex(edgepair2.first);
			CVertex3d* pEdge2End = pUniformGridMesh->GetVertex(edgepair2.second);

			double edge1startx = pEdge1Start->x();
			double edge1endx = pEdge1End->x();
			double edge2startx = pEdge2Start->x();
			double edge2endx = pEdge2End->x();

			if(edge1endx > edge2startx + TOL && edge1endx < edge2endx - TOL || edge1startx > edge2startx + TOL && edge1startx < edge2endx - TOL || 
				edge1endx > edge2endx + TOL && edge1endx < edge2startx - TOL || edge1startx > edge2endx + TOL && edge1startx < edge2startx - TOL ||
				edge2endx > edge1startx + TOL && edge2endx < edge1endx - TOL || edge2startx > edge1startx + TOL && edge2startx < edge1endx - TOL || 
				edge2endx > edge1endx + TOL && edge2endx < edge1startx - TOL || edge2startx > edge1endx + TOL && edge2startx < edge1startx - TOL)
			{
				if (pUniformGridMesh)
				{
					pUniformGridMesh->m_ArraySharpVertexIndex.push_back(edgepair1.first);
					pUniformGridMesh->m_ArraySharpVertexIndex.push_back(edgepair1.second);
					pUniformGridMesh->m_ArraySharpVertexIndex.push_back(edgepair2.first);
					pUniformGridMesh->m_ArraySharpVertexIndex.push_back(edgepair2.second);
				}
				if (pMesh)
				{
					pMesh->m_ArraySharpVertexIndex.push_back(pEdge1Start->m_OriginalIndex);
					pMesh->m_ArraySharpVertexIndex.push_back(pEdge1End->m_OriginalIndex);
					pMesh->m_ArraySharpVertexIndex.push_back(pEdge2Start->m_OriginalIndex);
					pMesh->m_ArraySharpVertexIndex.push_back(pEdge2End->m_OriginalIndex);
				}

				return 1;
			}
		}
	}
	return 0;
}
void Action_PolyOmino::DetectandHandleHorizontalIntersectingEdges()
{
	int edgeindex1,edgeindex2;
	std::vector<int> edgeset;
	int edgesetsize;
	std::pair<int,int> edgepair1,edgepair2;
	std::vector<int> vecarray;
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		edgeset = m_HorizontalSegments[i];
		//edgesetsize = edgeset.size();
		int flag = DetectandHandleHorizontalIntersectingEdges_EdgeArray(edgeset,i);
	}
}
int Action_PolyOmino::DetectandHandleHorizontalIntersectingEdges_EdgeArray(std::vector<int> edgeset,int i)
{
	std::pair<int,int> edgepair1,edgepair2;
	int edgesetsize = edgeset.size();
	for(int j = 0; j < edgesetsize; j++)
	{
		for(int k = j + 1; k < edgesetsize; k++)
		{
			//if(j == k)
			//	continue;
			int edgeindex1 = edgeset[j];
			int edgeindex2 = edgeset[k];

			if(std::find(m_DeletedEdgeArray.begin(),m_DeletedEdgeArray.end(),edgeindex1) != m_DeletedEdgeArray.end())
				continue;
			if(std::find(m_DeletedEdgeArray.begin(),m_DeletedEdgeArray.end(),edgeindex2) != m_DeletedEdgeArray.end())
				continue;

			edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
			edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];
			CVertex3d* pEdge1Start = pUniformGridMesh->GetVertex(edgepair1.first);
			CVertex3d* pEdge1End = pUniformGridMesh->GetVertex(edgepair1.second);
			CVertex3d* pEdge2Start = pUniformGridMesh->GetVertex(edgepair2.first);
			CVertex3d* pEdge2End = pUniformGridMesh->GetVertex(edgepair2.second);

			double edge1startx = pEdge1Start->x();
			double edge1endx = pEdge1End->x();
			double edge2startx = pEdge2Start->x();
			double edge2endx = pEdge2End->x();

			if(edge1endx > edge2startx + TOL && edge1endx < edge2endx - TOL || edge1startx > edge2startx + TOL && edge1startx < edge2endx - TOL ||
				edge1endx > edge2endx + TOL && edge1endx < edge2startx - TOL || edge1startx > edge2endx + TOL && edge1startx < edge2startx - TOL ||
				edge2endx > edge1startx + TOL && edge2endx < edge1endx - TOL || edge2startx > edge1startx + TOL && edge2startx < edge1endx - TOL ||
				edge2endx > edge1endx + TOL && edge2endx < edge1startx - TOL || edge2startx > edge1endx + TOL && edge2startx < edge1startx - TOL)
			{
				TRACE("\n4\n%d\n%d\n%d\n%d\n", edgepair1.first, edgepair1.second, edgepair2.first, edgepair2.second);
				//TRACE("%d\n", edgepair1.first);
				//TRACE("%d\n", edgepair1.second);
				//TRACE("%d\n", edgepair2.first);
				//TRACE("%d\n", edgepair2.second);

				//Invalid
				int nextvindex;
				std::vector<int> m_EdgeArray;
				m_EdgeArray.push_back(edgeindex1);
				int vindex = edgepair1.first;
				int firstvindex = edgepair1.second;
				int flag;

				//Case 1: Just add new edges 
				flag = HandleIntersecting_General(i,0,0,edgeindex1,edgeindex2);

				//Case 2: delete the inner hole
				if(!flag)
					flag = HandleIntersecting_innerholes(i,0,0,edgepair1.second,edgepair1.first,edgeindex1,edgeindex2);

				//Case 3: Bridge case, remove the additional face
				if(!flag)
					flag = HandleIntersecting_AditionalFace(i,0,0,edgeindex1,edgeindex2);

				if(!flag)
					ASSERT(0);

				if(flag)
				{
					//i--;
					IsUniformGridChanged = TRUE;
				}
				//return flag;
			}
		}
	}
	return 1;
}
int Action_PolyOmino::HandleIntersecting_AditionalFace(int index, int isVertical, int ischeck,int edgeindex1,int edgeindex2)
{
	std::pair<int,int> edgepair1,edgepair2;
	edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
	edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];

	int firstvindex = edgepair1.first;
	int nextvindex = edgepair1.second;

	std::vector<int> m_ArrayEdge;
	GetConnectedDirectedSegmentArray(edgeindex1,edgeindex2,firstvindex,nextvindex,m_ArrayEdge);

	//divide into two edge sets
	std::vector<int>::iterator eiterator;
	eiterator = std::find(m_ArrayEdge.begin(),m_ArrayEdge.end(),edgeindex2);
	int edgeindex2_loc = eiterator - m_ArrayEdge.begin();
	eiterator = std::find(m_ArrayEdge.begin(),m_ArrayEdge.end(),edgeindex1);
	int edgeindex1_loc = eiterator - m_ArrayEdge.begin();

	std::vector<int> m_subvectorarray1,m_subvectorarray2;
	if(edgeindex1_loc > edgeindex2_loc)
	{
		int test = edgeindex1_loc;
		edgeindex1_loc = edgeindex2_loc;
		edgeindex2_loc = test;
	}
	m_subvectorarray1.insert(m_subvectorarray1.begin(),m_ArrayEdge.begin() + edgeindex1_loc,m_ArrayEdge.begin() + edgeindex2_loc);
	m_subvectorarray2.insert(m_subvectorarray2.end(),m_ArrayEdge.begin() + edgeindex2_loc,m_ArrayEdge.end());
	m_subvectorarray2.insert(m_subvectorarray2.end(),m_ArrayEdge.begin(),m_ArrayEdge.begin() + edgeindex1_loc);

	std::vector<int> m_deletedarray;
	if(m_subvectorarray1.size() < m_subvectorarray2.size())
	{
		m_deletedarray = m_subvectorarray1;
	}else
		m_deletedarray = m_subvectorarray2;

	//Delete some edges to remove the hole
	int edgeindex = m_deletedarray[1];
	std::pair<int,int> edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
	int vindex,nextedgeindex;
	std::vector<int> m_ArrayEdge1;
	m_ArrayEdge1.push_back(edgeindex);
	vindex = edgepair.first;
	while(1)
	{
		nextedgeindex = GetConnectedSegment(edgeindex,vindex,nextvindex);
		if(nextedgeindex == -1 || nextedgeindex == edgeindex1 || nextedgeindex == edgeindex2)
			break;
		m_ArrayEdge1.push_back(nextedgeindex);
		vindex = nextvindex;
		edgeindex = nextedgeindex;
	}
	vindex = edgepair.second;
	edgeindex = m_deletedarray[1];
	while(1)
	{
		nextedgeindex = GetConnectedSegment(edgeindex,vindex,nextvindex);
		if(nextedgeindex == -1 || nextedgeindex == edgeindex1 || nextedgeindex == edgeindex2)
			break;
		m_ArrayEdge1.push_back(nextedgeindex);
		vindex = nextvindex;
		edgeindex = nextedgeindex;
	}

	m_ArrayEdge1.push_back(edgeindex1);
	m_ArrayEdge1.push_back(edgeindex2);

	//
	HandleIntersecting_General_NOJudgement(index,isVertical,ischeck,edgeindex1,edgeindex2);
	if(m_ArrayEdge1.size() > 0)
		//RemoveEdgeSet(m_ArrayEdge1);
		m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),m_ArrayEdge1.begin(),m_ArrayEdge1.end());

	return 1;
}

int Action_PolyOmino::HandleIntersecting_AditionalFace_IdenticalPoint(int index, int isVertical, int ischeck,int edgeindex1,int edgeindex2,int vindex1,int vindex2,std::vector<int>& removeedgeset)
{
	std::pair<int,int> edgepair1,edgepair2;
	edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
	edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];

	int firstvindex = edgepair1.first;
	int nextvindex = edgepair1.second;

	std::vector<int> m_ArrayEdge;
	GetConnectedDirectedSegmentArray(edgeindex1,edgeindex2,firstvindex,nextvindex,m_ArrayEdge);

	//divide into two edge sets
	std::vector<int>::iterator eiterator;
	eiterator = std::find(m_ArrayEdge.begin(),m_ArrayEdge.end(),edgeindex2);
	int edgeindex2_loc = eiterator - m_ArrayEdge.begin();
	eiterator = std::find(m_ArrayEdge.begin(),m_ArrayEdge.end(),edgeindex1);
	int edgeindex1_loc = eiterator - m_ArrayEdge.begin();

	std::vector<int> m_subvectorarray1,m_subvectorarray2;
	if(edgeindex1_loc > edgeindex2_loc)
	{
		int test = edgeindex1_loc;
		edgeindex1_loc = edgeindex2_loc;
		edgeindex2_loc = test;
	}
	m_subvectorarray1.insert(m_subvectorarray1.begin(),m_ArrayEdge.begin() + edgeindex1_loc,m_ArrayEdge.begin() + edgeindex2_loc);
	m_subvectorarray2.insert(m_subvectorarray2.end(),m_ArrayEdge.begin() + edgeindex2_loc,m_ArrayEdge.end());
	m_subvectorarray2.insert(m_subvectorarray2.end(),m_ArrayEdge.begin(),m_ArrayEdge.begin() + edgeindex1_loc);

	std::vector<int> m_deletedarray;
	if(m_subvectorarray1.size() < m_subvectorarray2.size())
	{
		m_deletedarray = m_subvectorarray1;
	}else
		m_deletedarray = m_subvectorarray2;

	//Delete some edges to remove the hole
	std::vector<int> m_ArrayEdge1;
	if(m_deletedarray.size() < 2)
		return 0;
	//m_ArrayEdge1.push_back(m_deletedarray[0]);
	int edgeindex = m_deletedarray[1];
	if(std::find(removeedgeset.begin(),removeedgeset.end(),edgeindex) != removeedgeset.end())
		edgeindex = m_deletedarray[m_deletedarray.size() - 2];
	std::pair<int,int> edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
	int vindex,nextedgeindex;
	m_ArrayEdge1.push_back(edgeindex);
	vindex = edgepair.first;
	while(1)
	{
		if(vindex == vindex1 || vindex == vindex2)
			break;
		nextedgeindex = GetConnectedSegment(edgeindex,vindex,nextvindex);
		if(nextedgeindex == -1 )
			break;
		m_ArrayEdge1.push_back(nextedgeindex);
		if(nextvindex == vindex1 || nextvindex == vindex2)
			break;
		vindex = nextvindex;
		edgeindex = nextedgeindex;
	}
	vindex = edgepair.second;
	edgeindex = m_deletedarray[1];
	while(1)
	{
		if(vindex == vindex1 || vindex == vindex2)
			break;
		nextedgeindex = GetConnectedSegment(edgeindex,vindex,nextvindex);
		if(nextedgeindex == -1 )
			break;
		m_ArrayEdge1.push_back(nextedgeindex);
		if(nextvindex == vindex1 || nextvindex == vindex2)
			break;
		vindex = nextvindex;
		edgeindex = nextedgeindex;
	}

	//m_ArrayEdge1.push_back(edgeindex1);
	//m_ArrayEdge1.push_back(edgeindex2);

	//
	//HandleIntersecting_General_NOJudgement(index,isVertical,ischeck,edgeindex1,edgeindex2);

	for(int i = 0; i < m_ArrayEdge1.size(); i++)
	{
		if(std::find(removeedgeset.begin(),removeedgeset.end(),m_ArrayEdge1[i]) == removeedgeset.end())
			removeedgeset.push_back(m_ArrayEdge1[i]);
	}
	//if(m_ArrayEdge1.size() > 0)
	//	RemoveEdgeSet(m_ArrayEdge1);

	return 1;
}
void Action_PolyOmino::UpdatePointConnectedSelectedEdgeIndex()
{
	std::vector<int> m_CornerPointArray;
	m_CornerPointArray.resize(pUniformGridMesh->m_ArrayVertex.GetSize(),0);

	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int,int> data;
		data = pUniformGridMesh->m_ArraySharpEdges[i];
		m_CornerPointArray[data.first]++; 
		m_CornerPointArray[data.second]++; 
	}

	m_ArrayPointConnectedSelectEdgeIndex.resize(pUniformGridMesh->m_ArrayVertex.GetSize());
	for(int i = 0; i < pUniformGridMesh->m_ArrayVertex.GetSize(); i++)
	{
		m_ArrayPointConnectedSelectEdgeIndex[i].clear();
	}
	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int,int> data;
		data = pUniformGridMesh->m_ArraySharpEdges[i];
		m_ArrayPointConnectedSelectEdgeIndex[data.first].push_back(i);
		m_ArrayPointConnectedSelectEdgeIndex[data.second].push_back(i);
	}
}
void Action_PolyOmino::InsertSavedEdgeSet()
{
	for(int i = 0; i < m_InsertEdgeArray.size(); i++)
	{
		std::pair<int,int> pair;
		pair.first = m_InsertEdgeArray[i][0];
		pair.second = m_InsertEdgeArray[i][1];
		int newedgeindex = InsertNewSelectedEdge(pair,m_InsertEdgeArray[i][2],m_InsertEdgeArray[i][3],m_InsertEdgeArray[i][4]);

		for(int j = 0; j < m_MergeConstraintsSave.size(); j++)
		{
			if(m_MergeConstraintsSave[j][m_MergeConstraintsSave[j].size() - 1] == i)
				m_MergeConstraintsSave[j][m_MergeConstraintsSave[j].size() - 1] = newedgeindex;
		}
	}
	m_InsertEdgeArray.clear();

	std::vector<int> vecarray,vecarray1;
	for(int i = 0; i < m_MergeConstraintsSave.size(); i++)
	{
		vecarray.clear();
		vecarray1 = m_MergeConstraintsSave[i];
		vecarray.insert(vecarray.begin(),vecarray1.begin(),vecarray1.end() - 1);
		int newindex = vecarray1[vecarray1.size() - 1];
		UpdateMergeConstraints(vecarray,newindex);
	}
	m_MergeConstraintsSave.clear();
}
void Action_PolyOmino::CheckandFixTopologyValidation(int iterationnum)
{
	//std::pair<int,int> edge1,edge2;
	//edge1 = pUniformGridMesh->m_ArraySharpEdges[893];
	//edge2 = pUniformGridMesh->m_ArraySharpEdges[894];
	//pUniformGridMesh->m_ArraySharpEdges.clear();
	//pUniformGridMesh->m_ArraySharpEdges.push_back(edge1);
	//pUniformGridMesh->m_ArraySharpEdges.push_back(edge2);
	//return;
	/*if(iterationnum == 93)
	{
		CMesh3d* pTestMesh1 = new CMesh3d(*pUniformGridMesh);
		pDoc->m_SceneGraph.Add(pTestMesh1);
		CMesh3d* pTestMesh2 = new CMesh3d(*pUniformGridMesh);
		pDoc->m_SceneGraph.Add(pTestMesh2);

		std::pair<int,int> edge1,edge2;
		edge1.first = 794;
		edge1.second = 795;
		edge2.first = 1654;
		edge2.second = 1655;
		//edge1 = pUniformGridMesh->m_ArraySharpEdges[764];
		//edge2 = pUniformGridMesh->m_ArraySharpEdges[1534];
		pTestMesh1->m_ArraySharpEdges.clear();
		pTestMesh1->m_ArraySharpEdges.push_back(edge1);
		pTestMesh2->m_ArraySharpEdges.clear();
		pTestMesh2->m_ArraySharpEdges.push_back(edge2);
		pDoc->UpdateTreeControl();
		return;
	}*/

	//Delete Unused Vertices, only keep the end points of the edges
	//Computethefacesum, if it is larger than the original, delete it
	//
	//IsValidMesh();
	//DeleteDuplicateEdges();
	//IsValidMesh();

	//DeleteInnerHoles();
	//IsValidMesh();
	//DeleteUnusedVertices();
	//IsValidMesh();
	//Delete Duplicate Vertices
	//DeleteDuplicateVertices();
	while(1)
	{
		int flag = 1;
		/*while(flag > 0)
		{
			flag = 0;

			//
			flag += DeleteInnerHoles();

			flag += DeleteUnusedVertices();

			flag += DeleteDuplicateVertices();
			//
			flag += DeleteDuplicateEdges();

			//Merge Connected Segments
			flag += MergeConnectedSegments();

			//isolated edges();
			flag += DeleteEdgesWithDegreeoneEndPoint();

			flag += DetectandHandleEdgeWithZeroLength();

			if(flag > 0)
				IsUniformGridChanged = TRUE;
		}*/

		m_DeletedEdgeArray.clear();
		m_InsertEdgeArray.clear();

		//
		UpdatePointConnectedSelectedEdgeIndex();

		//Kernal Code
		//Detect and handle Horizontal intersecting edges
		DetectandHandleHorizontalIntersectingEdges();

		//Detect and handle Vertical intersecting edges
		DetectandHandleVerticalIntersectingEdges();

		InsertSavedEdgeSet();
		if(m_DeletedEdgeArray.size() > 0)
			RemoveEdgeSet(m_DeletedEdgeArray);
		m_DeletedEdgeArray.clear();

		flag = 1;
		while(flag > 0)
		{
			flag = 0;

			//
			flag += DeleteInnerHoles();

			//flag += DeleteUnusedVertices();

			flag += DeleteDuplicateVertices();

			//
			InsertSavedEdgeSet();
			if(m_DeletedEdgeArray.size() > 0)
				RemoveEdgeSet(m_DeletedEdgeArray);

			m_DeletedEdgeArray.clear();

			//
			flag += DeleteDuplicateEdges();

			//pUniformGridMesh->SaveMFile("D:\\test.m");
			//Merge Connected Segments
			flag += MergeConnectedSegments();

			//isolated edges();
			flag += DeleteEdgesWithDegreeoneEndPoint();

			flag += DetectandHandleEdgeWithZeroLength();

			if(flag > 0)
				IsUniformGridChanged = TRUE;

			InsertSavedEdgeSet();

			//
			InsertSavedEdgeSet();
			if(m_DeletedEdgeArray.size() > 0)
				RemoveEdgeSet(m_DeletedEdgeArray);

			m_DeletedEdgeArray.clear();	
		}

		flag = 1;
		while(flag > 0)
		{
			flag = 0;

			//
			flag += DeleteInnerHoles();

			flag += DeleteUnusedVertices();

			flag += DeleteDuplicateVertices();
			//
			flag += DeleteDuplicateEdges();

			//Merge Connected Segments
			flag += MergeConnectedSegments();

			//isolated edges();
			flag += DeleteEdgesWithDegreeoneEndPoint();

			flag += DetectandHandleEdgeWithZeroLength();

			if(flag > 0)
				IsUniformGridChanged = TRUE;

			InsertSavedEdgeSet();

			if(m_DeletedEdgeArray.size() > 0)
				RemoveEdgeSet(m_DeletedEdgeArray);

			m_DeletedEdgeArray.clear();
		}

		if(!CheckVerticalEdgeIntersection() && !CheckHorizontalEdgeIntersection())
			break;
	}
}
int Action_PolyOmino::DeleteEdgesWithDegreeoneEndPoint()
{
	UpdatePointConnectedSelectedEdgeIndex();
	
	int size = pUniformGridMesh->m_ArrayVertex.GetSize();
	std::vector<int> m_EdgeArray;
	for(int i = 0; i < size; i++)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[i].size() == 1)
		{
			//delete the edge
			m_EdgeArray.push_back(m_ArrayPointConnectedSelectEdgeIndex[i][0]);
		}
	}
	if(m_EdgeArray.size() > 0)
	{
		//RemoveEdgeSet(m_EdgeArray);
		m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),m_EdgeArray.begin(),m_EdgeArray.end());
		return 1;
	}
	return 0;
}
int Action_PolyOmino::DeleteInnerHoles()
{
	return 0;
	//Delete Inner Holes
	int flag = 0;
	std::vector<int> m_ArrayFlag;
	m_ArrayFlag.resize(pUniformGridMesh->m_ArraySharpEdges.size(),0);
	std::stack<int> m_ArrayStackEdges;
	std::vector<int> m_ArrayCurrentLoops;
	std::vector<int> m_ArrayEndPoints;
	std::pair<int,int> pair;
	//
	UpdatePointConnectedSelectedEdgeIndex();

	int nextvindex;
	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		if(m_ArrayFlag[i] == 0)
		{			
			m_ArrayCurrentLoops.clear();
			m_ArrayStackEdges.push(i);
			m_ArrayCurrentLoops.push_back(i);
			m_ArrayFlag[i] = 1;
			m_ArrayEndPoints.clear();
			while(!m_ArrayStackEdges.empty())
			{
				int top = m_ArrayStackEdges.top();
				pair = pUniformGridMesh->m_ArraySharpEdges[top];
				m_ArrayStackEdges.pop();
				int nextedgeindex = GetConnectedSegment(top,pair.first,nextvindex);

				if(nextedgeindex == -1)
				{
					m_ArrayEndPoints.push_back(pair.first);
				}else
				if(m_ArrayFlag[nextedgeindex] == 0)
				{
					m_ArrayStackEdges.push(nextedgeindex);
					m_ArrayCurrentLoops.push_back(nextedgeindex);
					m_ArrayFlag[nextedgeindex] = 1;
				}

				nextedgeindex = GetConnectedSegment(top,pair.second,nextvindex);

				if(nextedgeindex == -1)
				{
					m_ArrayEndPoints.push_back(pair.second);
				}else
					if(m_ArrayFlag[nextedgeindex] == 0)
					{
						m_ArrayStackEdges.push(nextedgeindex);
						m_ArrayCurrentLoops.push_back(nextedgeindex);
						m_ArrayFlag[nextedgeindex] = 1;
					}
			}
			if(m_ArrayEndPoints.size() == 2 && m_ArrayEndPoints[0] == m_ArrayEndPoints[1])
			{
				//Delete the holes
				//RemoveEdgeSet(m_ArrayCurrentLoops);
				m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),m_ArrayCurrentLoops.begin(),m_ArrayCurrentLoops.end());
				flag = 1;
			}
		}
	}
	return flag;
}
int Action_PolyOmino::DeleteUnusedVertices()
{
	int flag = 0;
	//This function aims to delete the unused vertices,
	//only keep the end points of the selected edges
	std::vector<int> m_VerticesArray;
	std::vector<int>::iterator viterator;
	std::pair<int,int> pair;
	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		viterator = std::find(m_VerticesArray.begin(),m_VerticesArray.end(),pair.first);
		if(viterator == m_VerticesArray.end())
			m_VerticesArray.push_back(pair.first);
		viterator = std::find(m_VerticesArray.begin(),m_VerticesArray.end(),pair.second);
		if(viterator == m_VerticesArray.end())
			m_VerticesArray.push_back(pair.second);
	}
	std::vector<int> m_ArrayFlag;
	m_ArrayFlag.resize(pUniformGridMesh->m_ArrayVertex.GetSize(),0);
	for(int i = 0; i < m_VerticesArray.size(); i++)
	{
		m_ArrayFlag[m_VerticesArray[i]] = 1;
	}

	//delete the vertices with flag 0
	std::vector<int> m_ArrayIndex;
	m_ArrayIndex.resize(pUniformGridMesh->m_ArrayVertex.GetSize(),0);
	for(int i = 0; i < m_ArrayIndex.size(); i++)
	{
		m_ArrayIndex[i] = i;
	}
	for(int i = m_ArrayFlag.size() - 1; i >= 0; i--)
	{
		if(m_ArrayFlag[i] == 0)
		{
			//delete it
			pUniformGridMesh->m_ArrayVertex.RemoveAt(i);
			flag = 1;
			for(int ii = i + 1; ii < m_ArrayIndex.size(); ii++)
			{
				m_ArrayIndex[ii]--;
			}
		}
	}

	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		pair.first = m_ArrayIndex[pair.first];
		pair.second = m_ArrayIndex[pair.second];
		pUniformGridMesh->m_ArraySharpEdges[i] = pair;
	}
	return flag;
}
int Action_PolyOmino::DeleteDuplicateEdges()
{
	UpdatePointConnectedSelectedEdgeIndex();

	std::vector<int> m_ArrayDeletedEdgeSet;
	int size = pUniformGridMesh->m_ArrayVertex.GetSize();
	for(int i = 0; i < size; i++)
	{
		std::vector<int> edgeset = m_ArrayPointConnectedSelectEdgeIndex[i];
		for(int j = 0; j < edgeset.size(); j++)
		{
			for(int k = j + 1; k < edgeset.size(); k++)
			{
				std::pair<int,int> pair1 = pUniformGridMesh->m_ArraySharpEdges[edgeset[j]];
				std::pair<int,int> pair2 = pUniformGridMesh->m_ArraySharpEdges[edgeset[k]];
				if(pair1.first == pair2.first && pair1.second == pair2.second || 
					pair1.second == pair2.first && pair1.first == pair2.second)
				{
					std::vector<int>::iterator eiterator;
					eiterator = std::find(m_ArrayDeletedEdgeSet.begin(),m_ArrayDeletedEdgeSet.end(),edgeset[j]);
					if(eiterator == m_ArrayDeletedEdgeSet.end())
						m_ArrayDeletedEdgeSet.push_back(edgeset[j]);
				}

			}
		}
	}

	if(m_ArrayDeletedEdgeSet.size() > 0)
	{
		m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),m_ArrayDeletedEdgeSet.begin(),m_ArrayDeletedEdgeSet.end());
		RemoveEdgeSet(m_ArrayDeletedEdgeSet);
		return 1;
	}
	return 0;
}
int Action_PolyOmino::DeleteDuplicateVertices_Poly()
{
	int flag = 0;
	std::vector<int> m_VerticesArray;
	std::vector<int>::iterator viterator;
	std::pair<int, int> pair;
	std::vector<int> deleteedgeset;
	for (int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		viterator = std::find(m_VerticesArray.begin(), m_VerticesArray.end(), pair.first);
		if (viterator == m_VerticesArray.end())
			m_VerticesArray.push_back(pair.first);
		viterator = std::find(m_VerticesArray.begin(), m_VerticesArray.end(), pair.second);
		if (viterator == m_VerticesArray.end())
			m_VerticesArray.push_back(pair.second);
	}
	std::pair<int, std::pair<double, double>> vertex, vertex1;
	std::vector<std::pair<int, std::pair<double, double>>> vertices_array;
	std::vector<std::pair<int, int>> m_ReplaceArray;
	for (int i = 0; i < m_VerticesArray.size(); i++)
	{
		CVertex3d* pVertex = pUniformGridMesh->GetVertex(m_VerticesArray[i]);
		double x = pVertex->x();
		double y = pVertex->y();
		vertex.first = m_VerticesArray[i];
		vertex.second.first = x;
		vertex.second.second = y;
		vertices_array.push_back(vertex);
	}
	struct sort_pred {
		bool operator()(const std::pair<int, std::pair<double, double>> &left, const std::pair<int, std::pair<double, double>> &right)
		{
			if (fabs(left.second.first - right.second.first) < 1e-8)
			{
				return left.second.second < right.second.second;
			}
			else
				return left.second.first < right.second.first;
		}
	};
	std::sort(vertices_array.begin(), vertices_array.end(), sort_pred());
	std::vector<int> samevertices;
	for (int i = 0; i < vertices_array.size() - 1; i++)
	{
		samevertices.clear();
		vertex = vertices_array[i];
		vertex1 = vertices_array[i + 1];

		double d = Distance(Point2D(vertex.second.first, vertex.second.second), Point2D(vertex1.second.first, vertex1.second.second));
		if (d < 1e-4)
		{
			samevertices.push_back(i + 1);
			for (int j = i + 2; j < vertices_array.size() - 1; j++)
			{
				vertex1 = vertices_array[j];
				d = Distance(Point2D(vertex.second.first, vertex.second.second), Point2D(vertex1.second.first, vertex1.second.second));
				if (d < 1e-4)
				{
					samevertices.push_back(j);
					i++;
				}
				else
					break;
			}

			flag = 1;
			int index = vertex.first;

			//Maybe serveral vertices are the same!!!
			int index1;
			if (samevertices.size() >= 2)
			{
				int kk = 0;
			}
			for (int j = 0; j < samevertices.size(); j++)
			{
				vertex1 = vertices_array[samevertices[j]];
				index1 = vertex1.first;
				for (int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
				{
					if (pUniformGridMesh->m_ArraySharpEdges[i].first == index1)
						pUniformGridMesh->m_ArraySharpEdges[i].first = index;
					if (pUniformGridMesh->m_ArraySharpEdges[i].second == index1)
						pUniformGridMesh->m_ArraySharpEdges[i].second = index;
				}
				for (int i = 0; i < pUniformGridMesh->m_LandMarkArray.size(); i++)
				{
					GeoMapLandMark* pLandmark = pUniformGridMesh->m_LandMarkArray[i];
					for (int j = 0; j < pLandmark->landmark_points.size(); j++)
					{
						if (pLandmark->landmark_points[j] == index1)
						{
							pLandmark->landmark_points[j] = index;
						}
					}
				}
			}

			//if these two vertices are on a same edge
			//int sameedgeindex = OnSameEdge(index, index1);
			//if (sameedgeindex == -1)
			//{

			//}
		}
	}
	return 1;
}

int Action_PolyOmino::DeleteDuplicateLandmarks(CMesh3d* pMesh)
{
	GeoMapLandMark* pLandmark1, *pLandmark2;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark1 = pMesh->m_LandMarkArray[i];
		pLandmark1->deleteflag = 0;
	}

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark1 = pMesh->m_LandMarkArray[i];
		for (int j = i + 1; j < pMesh->m_LandMarkArray.size(); j++)
		{
			pLandmark2 = pMesh->m_LandMarkArray[j];
			if (pLandmark1->IsEqual(pLandmark2))
			{
				pLandmark2->deleteflag = 1;
			}
		}
	}

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark1 = pMesh->m_LandMarkArray[i];
		if (pLandmark1->deleteflag)
		{
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark1 = pMesh->m_LandMarkArray[i];
		if (pLandmark1->landmark_points.size() == 0)
		{
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
			i--;
		}
	}
	return 1;
}
int Action_PolyOmino::DeleteDuplicateVertices_Mesh3d(CMesh3d* pMesh)
{
	int flag = 0;
	std::vector<int> m_VerticesArray;
	std::vector<int>::iterator viterator;
	std::pair<int, int> pair;
	std::vector<int> deleteedgeset;
	//
	pMesh->SetVertexFlagAsItsIndex();

	//
	std::vector<std::pair<CVertex3d*, CVertex3d*>> m_ArraySharpEdges_;
	std::vector<std::vector<CVertex3d*>> m_ArrayLandmarks_;

	//SharpEdges
	std::pair<CVertex3d*, CVertex3d*> pair1;
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		pair1.first = pMesh->GetVertex(pair.first);
		pair1.second = pMesh->GetVertex(pair.second);
		m_ArraySharpEdges_.push_back(pair1);
	}

	//landmarks 
	std::vector<CVertex3d*> landmarkpoints;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		landmarkpoints.clear();
		for (int j = 0; j < pLandmark->landmark_points.size(); j++)
		{
			landmarkpoints.push_back(pMesh->GetVertex(pLandmark->landmark_points[j]));
		}
		m_ArrayLandmarks_.push_back(landmarkpoints);
	}

	//Add Vertex
	for (int i = 0; i < pMesh->NbVertex(); i++)
		m_VerticesArray.push_back(i);

	std::pair<int, std::pair<double, double>> vertex, vertex1;
	std::vector<std::pair<int, std::pair<double, double>>> vertices_array;
	std::vector<std::pair<int, int>> m_ReplaceArray;
	for (int i = 0; i < m_VerticesArray.size(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(m_VerticesArray[i]);
		double x = pVertex->x();
		double y = pVertex->y();
		vertex.first = m_VerticesArray[i];
		vertex.second.first = x;
		vertex.second.second = y;
		vertices_array.push_back(vertex);
	}
	struct sort_pred {
		bool operator()(const std::pair<int, std::pair<double, double>> &left, const std::pair<int, std::pair<double, double>> &right)
		{
			if (fabs(left.second.first - right.second.first) < 1e-8)
			{
				return left.second.second < right.second.second;
			}
			else
				return left.second.first < right.second.first;
		}
	};
	std::sort(vertices_array.begin(), vertices_array.end(), sort_pred());
	std::vector<int> m_ArrayVerticesFlag;
	m_ArrayVerticesFlag.resize(vertices_array.size(), -1);

	std::vector<int> samevertices;
	for (int i = 0; i < vertices_array.size() - 1; i++)
	{
		samevertices.clear();
		vertex = vertices_array[i];
		vertex1 = vertices_array[i + 1];

		if (vertex.first == 1420 || vertex1.first == 1420 ||
			vertex.first == 1421 || vertex1.first == 1421 ||
			vertex.first == 1422 || vertex1.first == 1422)
		{
			int ij = 0;
		}
		double d = Distance(Point2D(vertex.second.first, vertex.second.second), Point2D(vertex1.second.first, vertex1.second.second));
		if (d < 7e-3)
		{
			samevertices.push_back(i + 1);
			//continue;
			for (int j = i + 2; j < vertices_array.size() - 1; j++)
			{
				vertex1 = vertices_array[j];
				d = Distance(Point2D(vertex.second.first, vertex.second.second), Point2D(vertex1.second.first, vertex1.second.second));
				if (d < 7e-3)
				{
					samevertices.push_back(j);
					i++;
				}
				else
					break;
			}

			flag = 1;
			int index = vertex.first;

			//Maybe serveral vertices are the same!!!
			int index1;
			if (samevertices.size() >= 2)
			{
				int kk = 0;
			}
			for (int j = 0; j < samevertices.size(); j++)
			{
				vertex1 = vertices_array[samevertices[j]];
				index1 = vertex1.first;

				m_ArrayVerticesFlag[index1] = index;
				for (int ii = 0; ii < pMesh->NbFace(); ii++)
				{
					CFace3d* pFace = pMesh->GetFace(ii);
					CVertex3d* pVertex1 = pFace->v1();
					CVertex3d* pVertex2 = pFace->v2();
					CVertex3d* pVertex3 = pFace->v3();
					if (pVertex1->m_Flag == index1)
						pFace->v1(pMesh->GetVertex(index));
					if (pVertex2->m_Flag == index1)
						pFace->v2(pMesh->GetVertex(index));
					if (pVertex3->m_Flag == index1)
						pFace->v3(pMesh->GetVertex(index));
				}

				//
				CVertex3d* pVertex1 = pMesh->GetVertex(index);
				if (m_ArrayVerticesFlag[index] > -1)
				{
					int ik = 0;
				}
				for (int ii = 0; ii < m_ArraySharpEdges_.size(); ii++)
				{
					pair1 = m_ArraySharpEdges_[ii];
					if (pair1.first->m_Flag == index1)
					{
						m_ArraySharpEdges_[ii].first = pVertex1;
					}

					if (pair1.second->m_Flag == index1)
					{
						m_ArraySharpEdges_[ii].second = pVertex1;
					}
				}
			}
		}
	}

	for (int i = pMesh->NbVertex() - 1; i >= 0; i--)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		if (m_ArrayVerticesFlag[i] > -1)
		{
			pVertex->m_Flag = -1;
			pMesh->m_ArrayVertex.RemoveAt(i);
		}
	}
	pMesh->m_SetVertexFlagAsItsIndex = 0;
	pMesh->SetVertexFlagAsItsIndex();

	//SharpEdges
	for (int i = 0; i < m_ArraySharpEdges_.size(); i++)
	{
		pair1 = m_ArraySharpEdges_[i];

		pair.first = pair1.first->m_Flag;
		pair.second = pair1.second->m_Flag;

		if (pair.first == -1 || pair.second == -1)
			ASSERT(0);

		pMesh->m_ArraySharpEdges[i] = pair;
	}

	//landmarks 
	for (int i = 0; i < m_ArrayLandmarks_.size(); i++)
	{
		landmarkpoints = m_ArrayLandmarks_[i];
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < landmarkpoints.size(); j++)
		{
			pLandmark->landmark_points[j] = landmarkpoints[j]->m_Flag;
		}
		for (int j = 0; j < pLandmark->landmark_points.size(); j++)
		{
			if (pLandmark->landmark_points[j] == -1)
			{
				pLandmark->landmark_points.erase(pLandmark->landmark_points.begin() + j);
				j--;
			}
		}
	}

	//Degenerate triangles and sharpedges
	for (int i = pMesh->NbFace() - 1; i >= 0; i--)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();
		if (pVertex1 == pVertex2 || pVertex1 == pVertex3 || pVertex2 == pVertex3)
		{
			pMesh->m_ArrayFace.RemoveAt(i);
		}
	}
	for (int i = pMesh->m_ArraySharpEdges.size() - 1; i >= 0; i--)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		if (pair.first == pair.second)
		{
			pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
		}
	}
	return 1;
}
int Action_PolyOmino::DeleteDuplicateVertices()
{
	int flag = 0;
	std::vector<int> m_VerticesArray;
	std::vector<int>::iterator viterator;
	std::pair<int,int> pair;
	std::vector<int> deleteedgeset;
	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		viterator = std::find(m_VerticesArray.begin(),m_VerticesArray.end(),pair.first);
		if(viterator == m_VerticesArray.end())
			m_VerticesArray.push_back(pair.first);
		viterator = std::find(m_VerticesArray.begin(),m_VerticesArray.end(),pair.second);
		if(viterator == m_VerticesArray.end())
			m_VerticesArray.push_back(pair.second);
	}
	std::pair<int,std::pair<double,double>> vertex,vertex1;
	std::vector<std::pair<int,std::pair<double,double>>> vertices_array;
	std::vector<std::pair<int,int>> m_ReplaceArray;
	for(int i = 0; i < m_VerticesArray.size(); i++)
	{
		CVertex3d* pVertex = pUniformGridMesh->GetVertex(m_VerticesArray[i]);
		double x = pVertex->x();
		double y = pVertex->y();
		vertex.first = m_VerticesArray[i];
		vertex.second.first = x;
		vertex.second.second = y;
		vertices_array.push_back(vertex);
	}
	struct sort_pred {
		bool operator()(const std::pair<int,std::pair<double,double>> &left, const std::pair<int,std::pair<double,double>> &right) 
		{
			if(fabs(left.second.first - right.second.first) < 1e-2)
			{
				return left.second.second < right.second.second;
			}else
				return left.second.first < right.second.first;
		}
	};
	std::sort(vertices_array.begin(),vertices_array.end(),sort_pred());
	for(int i = 0; i < vertices_array.size() - 1; i++)
	{
		vertex =  vertices_array[i];
		vertex1 =  vertices_array[i + 1];

		double deltax = fabs(vertex.second.first - vertex1.second.first);
		double deltay = fabs(vertex.second.second - vertex1.second.second);
		if( deltax < 1e-4 && deltay < 1e-4)
		{
			flag = 1;
			int index = vertex.first;
			int index1 = vertex1.first;

			int edgeindex,edgeindex1;
			//if these two vertices are on a same edge
			int sameedgeindex = OnSameEdge(index,index1);
			if(sameedgeindex == -1)
			{
				TRACE("\n2\n%d\n%d\n", vertex.first, vertex1.first);
				edgeindex = edgeindex1 = -1;
				if(m_IsMergeHorizontal)
				{
					std::vector<int> edgeset = m_HorizontalSegments[m_MergeIndex];
					for(int j = 0; j < edgeset.size(); j++)
					{
						int eindex = edgeset[j];
						pair = pUniformGridMesh->m_ArraySharpEdges[eindex];
						if(pair.first == index || pair.second == index)
						{
							edgeindex = eindex;
							break;
						}
					}
					for(int j = 0; j < edgeset.size(); j++)
					{
						int eindex = edgeset[j];
						pair = pUniformGridMesh->m_ArraySharpEdges[eindex];
						if(pair.first == index1 || pair.second == index1)
						{
							edgeindex1 = eindex;
							break;
						}
					}
					//HandleIntersecting_AditionalFace(m_MergeIndex,0,0,edgeindex,edgeindex1);
				}else
				{
					std::vector<int> edgeset = m_VerticalSegments[m_MergeIndex];
					for(int j = 0; j < edgeset.size(); j++)
					{
						pair = pUniformGridMesh->m_ArraySharpEdges[edgeset[j]];
						if(pair.first == index || pair.second == index)
						{
							edgeindex = edgeset[j];
							break;
						}
					}
					for(int j = 0; j < edgeset.size(); j++)
					{
						pair = pUniformGridMesh->m_ArraySharpEdges[edgeset[j]];
						if(pair.first == index1 || pair.second == index1)
						{
							edgeindex1 = edgeset[j];
							break;
						}
					}
					
				}

				if(edgeindex >= 0 && edgeindex1 >= 0)
				{
					//
					UpdatePointConnectedSelectedEdgeIndex();
					//
					HandleIntersecting_AditionalFace_IdenticalPoint(m_MergeIndex,!m_IsMergeHorizontal,0,edgeindex,edgeindex1,index,index1,deleteedgeset);
				}
			}else
			{
				deleteedgeset.push_back(sameedgeindex);
			}
			std::pair<int,int> pair1;
			pair1.first = index;
			pair1.second = index1;
			m_ReplaceArray.push_back(pair1);
			/*for(int ii = 0; ii < pUniformGridMesh->m_ArraySharpEdges.size(); ii++)
			{
				pair = pUniformGridMesh->m_ArraySharpEdges[ii];
				if(pair.first == index)
					pair.first = index1;
				if(pair.second == index)
					pair.second = index1;
				pUniformGridMesh->m_ArraySharpEdges[ii] = pair;
			}*/
		}
	}

	if(deleteedgeset.size() > 0)
		m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),deleteedgeset.begin(),deleteedgeset.end());
		//RemoveEdgeSet(deleteedgeset);

	for(int jj = 0; jj < m_ReplaceArray.size(); jj++)
	{
		std::pair<int,int> pair1;
		pair1 = m_ReplaceArray[jj];
		for(int ii = 0; ii < pUniformGridMesh->m_ArraySharpEdges.size(); ii++)
		{
			pair = pUniformGridMesh->m_ArraySharpEdges[ii];
			if(pair.first == pair1.first)
				pair.first = pair1.second;
			if(pair.second == pair1.first)
				pair.second = pair1.second;
			pUniformGridMesh->m_ArraySharpEdges[ii] = pair;
		}
	}
	
	return flag;
}
int Action_PolyOmino::OnSameEdge(int index1,int index2)
{
	std::pair<int,int> pair;
	for(int ii = 0; ii < pUniformGridMesh->m_ArraySharpEdges.size(); ii++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[ii];
		if(pair.first == index1 && pair.second == index2 || pair.first == index2 && pair.second == index1)
			return ii;
	}
	return -1;
}
int Action_PolyOmino::DetectandHandleEdgeWithZeroLength()
{
	//Detect the zero length edge
	std::pair<int,int> pair,pair1;
	std::vector<int> edgeset;
	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(pair.first);
		CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(pair.second);

		double x1 = pStartVertex->x();
		double y1 = pStartVertex->y();

		double x2 = pEndVertex->x();
		double y2 = pEndVertex->y();

		if(fabs(x1 - x2) < 1e-8 && fabs(y1 - y2) < 1e-8)
		{
			//Delete the edge.
			//pUniformGridMesh->m_ArraySharpEdges.erase(pUniformGridMesh->m_ArrayEdges.begin() + i);
			edgeset.push_back(i);

			for(int j = 0; j < pUniformGridMesh->m_ArraySharpEdges.size(); j++)
			{
				if(j == i)
					continue;

				pair1 = pUniformGridMesh->m_ArraySharpEdges[j];
				if(pair1.first == pair.second)
					pair1.first = pair.first;
				else
				if(pair1.second == pair.second)
					pair1.second = pair.first;

				pUniformGridMesh->m_ArraySharpEdges[j] = pair1;
			}
			//i--;
		}
	}

	if(edgeset.size() > 0)
	{
		//RemoveEdgeSet(edgeset);
		m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),edgeset.begin(),edgeset.end());
		return 1;
	}
	return 0;
}
int Action_PolyOmino::MergeConnectedSegments()
{
	int mergnum = 0;
	int flag = 0;
	//check whether to merge the segments
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		flag = 0;
		for(int j = 0; j < m_HorizontalSegments[i].size(); j++)
		{
			for(int k = j + 1; k < m_HorizontalSegments[i].size(); k++)
			{
				//if(j == k)
				//	continue;

				flag = MergeTwoSegments(i,m_HorizontalSegments[i][j],m_HorizontalSegments[i][k],1);
				mergnum += flag;
				//if(flag)
				//{
					//i--;
				//	break;
				//}
			}
			//if(flag)
			//	break;
		}
	}
	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		flag = 0;
		for(int j = 0; j < m_VerticalSegments[i].size(); j++)
		{
			for(int k = j + 1; k < m_VerticalSegments[i].size(); k++)
			{
				//if(j == k)
				//	continue;

				flag = MergeTwoSegments(i,m_VerticalSegments[i][j],m_VerticalSegments[i][k],0);
				mergnum += flag;
				//if(flag)
				//{
				//	i--;
				//	break;
				//}
			}
			//if(flag)
			//	break;
		}
	}
	return mergnum;
}
int Action_PolyOmino::MergeTwoSegments(int index,int index1,int index2,int IsHorizontal)
{
	std::pair<int,int> edgepair1 = pUniformGridMesh->m_ArraySharpEdges[index1];
	std::pair<int,int> edgepair2 = pUniformGridMesh->m_ArraySharpEdges[index2];

	CVertex3d* pEdge1Start = pUniformGridMesh->GetVertex(edgepair1.first);
	CVertex3d* pEdge1End = pUniformGridMesh->GetVertex(edgepair1.second);
	CVertex3d* pEdge2Start = pUniformGridMesh->GetVertex(edgepair2.first);
	CVertex3d* pEdge2End = pUniformGridMesh->GetVertex(edgepair2.second);

	double edge1start;
	double edge1end;
	double edge2start;
	double edge2end;
	if(IsHorizontal)
	{
		edge1start = pEdge1Start->x();
		edge1end = pEdge1End->x();
		edge2start = pEdge2Start->x();
		edge2end = pEdge2End->x();
	}else
	{
		edge1start = pEdge1Start->y();
		edge1end = pEdge1End->y();
		edge2start = pEdge2Start->y();
		edge2end = pEdge2End->y();
	}

	std::pair<int,int> newedge;
	std::vector<int> edgeset;
	edgeset.push_back(index1);
	edgeset.push_back(index2);
	for(int ii = 0; ii < edgeset.size(); ii++)
	{
		if(std::find(m_DeletedEdgeArray.begin(),m_DeletedEdgeArray.end(),edgeset[ii]) != m_DeletedEdgeArray.end())
			return 0;
	}

	//if(fabs(edge1start - edge2start) < 1e-8)
	if(edgepair1.first == edgepair2.first)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[edgepair1.first].size() == 2)
		{
			newedge.first = edgepair1.second;
			newedge.second = edgepair2.second;
			int newindex = InsertNewSelectedEdge_Save(newedge,index,!IsHorizontal,0);
			
			UpdateMergeConstraints_Save(edgeset,newindex);
			
			//pUniformGridMesh->m_ArrayEdges.push_back(newedge);
			//edgesArray.push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
			//RemoveEdgeSet(edgeset);
			m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),edgeset.begin(),edgeset.end());

			return 1;
		}
	}else
	if(edgepair1.first == edgepair2.second)
	//if(fabs(edge1start - edge2end) < 1e-8)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[edgepair1.first].size() == 2)
		{
			newedge.first = edgepair1.second;
			newedge.second = edgepair2.first;
			int newindex = InsertNewSelectedEdge_Save(newedge,index,!IsHorizontal,0);

			UpdateMergeConstraints_Save(edgeset,newindex);
			//pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
			//edgesArray.push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
			//RemoveEdgeSet(edgeset);
			m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),edgeset.begin(),edgeset.end());

			return 1;
		}
	}else
	if(edgepair1.second == edgepair2.second)
	//if(fabs(edge1end - edge2end) < 1e-8)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[edgepair1.second].size() == 2)
		{
			newedge.first = edgepair1.first;
			newedge.second = edgepair2.first;
			int newindex = InsertNewSelectedEdge_Save(newedge,index,!IsHorizontal,0);

			UpdateMergeConstraints_Save(edgeset,newindex);
			//pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
			//edgesArray.push_back(pUniformGridMesh->m_ArraySharpEdges.size() - 1);
			//RemoveEdgeSet(edgeset);
			m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),edgeset.begin(),edgeset.end());

			return 1;
		}
	}else
	if(edgepair1.second == edgepair2.first)
	//if(fabs(edge1end - edge2start) < 1e-8)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[edgepair1.second].size() == 2)
		{
			newedge.first = edgepair1.first;
			newedge.second = edgepair2.second;
			int newindex = InsertNewSelectedEdge_Save(newedge,index,!IsHorizontal,0);

			UpdateMergeConstraints_Save(edgeset,newindex);
			//pUniformGridMesh->m_ArraySharpEdges.push_back(newedge);
			//edgesArray.push_back(pUniformGridMesh->m_ArraySeletedEdges.size() - 1);
			//RemoveEdgeSet(edgeset);
			m_DeletedEdgeArray.insert(m_DeletedEdgeArray.begin(),edgeset.begin(),edgeset.end());
			//RemoveEdgeSet(m_ArrayPointConnectedSelectEdgeIndex,edgeset);
			return 1;
		}
	}
	return 0;
}
void Action_PolyOmino::RemoveEdgeSet(std::vector<int>& m_EdgeArray)
{
	if(pUniformGridMesh->m_ArraySharpEdges.size() != pUniformGridMesh->m_ArraySharpEdgesFlag.size())
		ASSERT(0);

	//Remove some constraints
	for(int i = 0; i < pUniformGridMesh->m_ArrayMergeConstraints.size(); i++)
	{
		std::pair<int,int> pair = pUniformGridMesh->m_ArrayMergeConstraints[i];
		if(std::find(m_EdgeArray.begin(),m_EdgeArray.end(),pair.first) != m_EdgeArray.end() || 
			std::find(m_EdgeArray.begin(),m_EdgeArray.end(),pair.second) != m_EdgeArray.end())
		{
			pUniformGridMesh->m_ArrayMergeConstraints.erase(pUniformGridMesh->m_ArrayMergeConstraints.begin() + i);
			i--;
		}
	}

	//first remove repeated items
	for(int i = 0; i < m_EdgeArray.size(); i++)
	{
		for(int j = i + 1; j < m_EdgeArray.size(); j++)
		{
			if(m_EdgeArray[i] == m_EdgeArray[j])
			{
				m_EdgeArray.erase(m_EdgeArray.begin() + j);
				j--;
			}
		}
	}
	//first remove the edges from the horizontal and vertical array
	std::vector<int> m_EdgeArrayBak = m_EdgeArray;
	for(int ii = 0; ii < m_EdgeArray.size(); ii++)
	{
		for(int i = 0; i < m_HorizontalSegments.size(); i++)
		{
			for(int j = 0; j < m_HorizontalSegments[i].size(); j++)
			{
				if( m_HorizontalSegments[i][j] == m_EdgeArray[ii])
				{
					m_HorizontalSegments[i].erase(m_HorizontalSegments[i].begin() + j);
					if(m_HorizontalSegments[i].size() == 0)
					{
						m_HorizontalSegments.erase(m_HorizontalSegments.begin() + i);
						i--;
					}
					m_EdgeArray.erase(m_EdgeArray.begin() + ii);
					if(m_EdgeArray.size() == 0)
						goto label11;
					ii--;
					j--;
					goto label2;
				}
			}
		}
label2:
		int ij = -1;
	}
	for(int ii = 0; ii < m_EdgeArray.size(); ii++)
	{
		for(int i = 0; i < m_VerticalSegments.size(); i++)
		{
			for(int j = 0; j < m_VerticalSegments[i].size(); j++)
			{
				if( m_VerticalSegments[i][j] == m_EdgeArray[ii])
				{
					m_VerticalSegments[i].erase(m_VerticalSegments[i].begin() + j);
					if(m_VerticalSegments[i].size() == 0)
					{
						m_VerticalSegments.erase(m_VerticalSegments.begin() + i);
						i--;
					}
					m_EdgeArray.erase(m_EdgeArray.begin() + ii);
					if(m_EdgeArray.size() == 0)
						goto label11;
					ii--;
					j--;
					goto label1;
				}
			}
		}
label1:
		int ij = -1;
	}
label11:
	//Remove the edges from the SelectedEdge Array and modify its index in horizontal and vertical array
	std::vector<int> m_SelectedEdgeIndex;
	m_SelectedEdgeIndex.resize(pUniformGridMesh->m_ArraySharpEdges.size(),0);
	for(int ii = 0; ii < pUniformGridMesh->m_ArraySharpEdges.size(); ii++)
	{
		m_SelectedEdgeIndex[ii] = ii;
	}
	int edgesize = pUniformGridMesh->m_ArraySharpEdges.size();
	std::sort(m_EdgeArrayBak.begin(),m_EdgeArrayBak.end());
	for(int ii = m_EdgeArrayBak.size() - 1; ii >= 0; ii--)
	{
		for(int jj = m_EdgeArrayBak[ii] + 1; jj < edgesize; jj++)
		{
			m_SelectedEdgeIndex[jj]--;
		}
		pUniformGridMesh->m_ArraySharpEdges.erase(pUniformGridMesh->m_ArraySharpEdges.begin() + m_EdgeArrayBak[ii]);
		//delete flag
		pUniformGridMesh->m_ArraySharpEdgesFlag.erase(pUniformGridMesh->m_ArraySharpEdgesFlag.begin() + m_EdgeArrayBak[ii]);
	}

	//change the edge index
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		for(int j = 0; j < m_HorizontalSegments[i].size(); j++)
		{
			m_HorizontalSegments[i][j] = m_SelectedEdgeIndex[m_HorizontalSegments[i][j]];
			if(m_HorizontalSegments[i][j] >= pUniformGridMesh->m_ArraySharpEdges.size())
			{
				ASSERT(0);
			}
		}
	}
	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		for(int j = 0; j < m_VerticalSegments[i].size(); j++)
		{
			m_VerticalSegments[i][j] = m_SelectedEdgeIndex[m_VerticalSegments[i][j]];
			if(m_VerticalSegments[i][j] >= pUniformGridMesh->m_ArraySharpEdges.size())
			{
				ASSERT(0);
			}
		}
	}

	//change the constraint index
	for(int i = 0; i < pUniformGridMesh->m_ArrayMergeConstraints.size(); i++)
	{
		std::pair<int,int> pair = pUniformGridMesh->m_ArrayMergeConstraints[i];
		if(pair.first < m_SelectedEdgeIndex.size())
			pair.first = m_SelectedEdgeIndex[pair.first];
		if(pair.second < m_SelectedEdgeIndex.size())
			pair.second = m_SelectedEdgeIndex[pair.second];
		pUniformGridMesh->m_ArrayMergeConstraints[i] = pair;
	}

	//point edge connection
	UpdatePointConnectedSelectedEdgeIndex();

	//Updatae DeleteEdgeArray
	m_DeletedEdgeArray.clear();
}
int Action_PolyOmino::SearchSamePositionVertices(CMesh3d* pMesh3d,std::vector<int> changedvertices,int index)
{
	CVertex3d* pCurrentVertex = pMesh3d->GetVertex(index);
	double x = pCurrentVertex->x();
	double y = pCurrentVertex->y();
	double z = pCurrentVertex->z();
	for(int i = 0; i < changedvertices.size(); i++)
	{
		CVertex3d* pVertex = pMesh3d->GetVertex(changedvertices[i]);
		double x1 = pVertex->x();
		double y1 = pVertex->y();
		double z1 = pVertex->z();

		if(fabs(x - x1) < 1e-8 && fabs(y - y1) < 1e-8 && fabs(z - z1) < 1e-8)
		{
			return changedvertices[i];
		}
	}
	return -1;
}
int Action_PolyOmino::SetSharpEdgeFlag()
{
	std::pair<int,int> pair;
	pUniformGridMesh->m_ArraySharpEdgesFlag.clear();
	pUniformGridMesh->m_ArraySharpEdgesFlag.resize(pUniformGridMesh->m_ArraySharpEdges.size());
	int flag;
	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		int start = pair.first;
		int end = pair.second;
		CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(start);
		CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(end);
		double x1 = pStartVertex->x();
		double y1 = pStartVertex->y();
		double x2 = pEndVertex->x();
		double y2 = pEndVertex->y();
		if(fabs(x1 - x2) < 1e-8)
		{
			if(y1 < y2)
			{
				flag = 1;
			}else
				flag = 2;
		}
		else
		{
			if(x1 < x2)
			{
				flag = 3;
			}else
				flag = 4;
		}
		pUniformGridMesh->m_ArraySharpEdgesFlag[i] = flag;
	}
	return 1;
}
int Action_PolyOmino::UniformSimplificationOneIteration()
{
	std::pair<int,int> pair;
	//Check the valid

	pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	//Point3D lower,upper;
	//pMesh->ComputeBoundingBox(lower,upper);

	std::vector<double> xinterval,yinterval;

	//set m_ArraySharpEdgeFlag
	SetSharpEdgeFlag();	

	//show the x lines on the bottom and y lines on the left
	/*if(pUniformGridMesh->m_ArraySeletedVertexIndex.size() > 0)
	{
		std::sort(pUniformGridMesh->m_ArraySeletedVertexIndex.begin(),pUniformGridMesh->m_ArraySeletedVertexIndex.end());
		for(int i = pUniformGridMesh->m_ArraySeletedVertexIndex.size() - 1; i >= 0; i--)
		{
			int index = pUniformGridMesh->m_ArraySeletedVertexIndex[i];
			CVertex3d* pVertex = pUniformGridMesh->m_ArrayVertex.GetAt(index);
			pUniformGridMesh->m_ArrayVertex.RemoveAt(index);
			delete pVertex;
		}
		pUniformGridMesh->m_ArraySeletedVertexIndex.clear();
	}*/

	/*for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		int start = pair.first;
		int end = pair.second;
		CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(start);
		CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(end);
		if(fabs(pStartVertex->y() - pEndVertex->y()) < 1e-8)
		{
			//Horizontal
			CVertex3d* pNewVertex = new CVertex3d();
			pNewVertex->Set(lower.x,pStartVertex->y(),0);
			pUniformGridMesh->AddVertex(pNewVertex);
			//pUniformGridMesh->m_ArraySeletedVertexIndex.push_back(pUniformGridMesh->NbVertex() - 1);
		}else
		{
			//Vertical
			CVertex3d* pNewVertex = new CVertex3d();
			pNewVertex->Set(pStartVertex->x(),lower.y,0);
			pUniformGridMesh->AddVertex(pNewVertex);
			//pUniformGridMesh->m_ArraySeletedVertexIndex.push_back(pUniformGridMesh->NbVertex() - 1);
		}
	}*/

	//Search for the closest edges.
	int ii;
	std::pair<int,int> edgepair;
	double x,y;
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		int edgeindex = m_HorizontalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
			
		CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
		y = pVertex->y();
		yinterval.push_back(y);
	}
	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		int edgeindex = m_VerticalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
		x = pVertex->x();
		xinterval.push_back(x);
	}

	//
	//std::sort(xinterval.begin(),xinterval.end());
	//std::sort(yinterval.begin(),yinterval.end());

	std::vector<int> m_ArrayXindex,m_ArrayYindex;
	double minimalxinterval,minimalyinterval;
	while(1)
	{
		minimalxinterval = 1e10;
		minimalyinterval = 1e10;
		int minimalxindex,minimalyindex;
		for(int i = 0; i < xinterval.size() - 1; i++)
		{
			if(std::find(m_ArrayXindex.begin(),m_ArrayXindex.end(),i) != m_ArrayXindex.end())
				continue;
			double val = xinterval[i + 1] - xinterval[i];
			if(val < minimalxinterval)
			{
				minimalxinterval = val;
				minimalxindex = i;
			}
		}

		for(int i = 0; i < yinterval.size() - 1; i++)
		{
			if(std::find(m_ArrayYindex.begin(),m_ArrayYindex.end(),i) != m_ArrayYindex.end())
				continue;
			double val = yinterval[i + 1] - yinterval[i];
			if(val < minimalyinterval)
			{
				minimalyinterval = val;
				minimalyindex = i;
			}
		}
		if(xinterval.size() == 1 && yinterval.size() == 1)
			return 0;

		BOOL isChanged = FALSE;

		if(minimalxinterval < minimalyinterval)
		{
			//first Search whether it is lies in the  Merge constaints
			BOOL hasmergeconstaints = HasMergeConstraints(minimalxindex,1);

			if(hasmergeconstaints)
			{
				isChanged = EnlargeInterval(xinterval,yinterval,minimalxindex,1);
				if(!isChanged)
				{
					m_ArrayXindex.push_back(minimalxindex);
				}
			}
			else
			{
				MergeInterval(xinterval,yinterval,minimalxindex,1);
				isChanged = TRUE;
			}
		}else
		{
			//first Search whether it is lies in the  Merge constaints
			BOOL hasmergeconstaints = HasMergeConstraints(minimalyindex,0);

			if(hasmergeconstaints)
			{
				isChanged = EnlargeInterval(xinterval,yinterval,minimalyindex,0);
				if(!isChanged)
				{
					m_ArrayYindex.push_back(minimalyindex);
				}
			}
			else
			{
				MergeInterval(xinterval,yinterval,minimalyindex,0);
				isChanged = TRUE;
			}
		}
		if(isChanged)
			break;
	}

	pUniformGridMesh->SetModified(TRUE);

	//Is valid Mesh
	//IsValidMesh();

	return 1;
}
BOOL Action_PolyOmino::EnlargeInterval(std::vector<double> &xinterval,std::vector<double> &yinterval,int minimalindex,BOOL isVertical)
{
	std::vector<int> segments;
	std::pair<int,int> edgepair;
	BOOL isChanged = FALSE;
	if(isVertical)
	{
		int size = xinterval.size();
		if(minimalindex > 0 && minimalindex < size - 2)
		{
			double x1 = xinterval[minimalindex - 1];
			double x4 = xinterval[minimalindex + 2];
			double x2 = (x1 * 2 + x4) / 3;
			double x3 = (x1 + x4 * 2) / 3;

			segments = m_VerticalSegments[minimalindex];
			for(int i = 0; i < segments.size(); i++)
			{
				edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

				CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
				if(fabs(pVertex->x() - x2) > 1e-8)
				{
					pVertex->x(x2);
					isChanged = TRUE;
				}

				pVertex  = pUniformGridMesh->GetVertex(edgepair.second);

				if(fabs(pVertex->x() - x2) > 1e-8)
				{
					pVertex->x(x2);
					isChanged = TRUE;
				}
			}

			segments = m_VerticalSegments[minimalindex + 1];
			for(int i = 0; i < segments.size(); i++)
			{
				edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

				CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
				if(fabs(pVertex->x() - x3) > 1e-8)
				{
					pVertex->x(x3);
					isChanged = TRUE;
				}

				pVertex  = pUniformGridMesh->GetVertex(edgepair.second);

				if(fabs(pVertex->x() - x3) > 1e-8)
				{
					pVertex->x(x3);
					isChanged = TRUE;
				}
			}
			IsUniformGridChanged = TRUE;
		}else if(minimalindex == 0)
		{
			double x1 = xinterval[minimalindex];
			double x3 = xinterval[minimalindex + 2];
			double x2 = (x1 + x3) / 2;

			segments = m_VerticalSegments[minimalindex + 1];
			for(int i = 0; i < segments.size(); i++)
			{
				edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

				CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
				//pVertex->x(x2);
				if(fabs(pVertex->x() - x2) > 1e-8)
				{
					pVertex->x(x2);
					isChanged = TRUE;
				}

				pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
				//pVertex->x(x2);
				if(fabs(pVertex->x() - x2) > 1e-8)
				{
					pVertex->x(x2);
					isChanged = TRUE;
				}
			}
			IsUniformGridChanged = TRUE;
		}else if(minimalindex == size - 2)
		{
			double x1 = xinterval[minimalindex - 1];
			double x3 = xinterval[minimalindex + 1];
			double x2 = (x1 + x3) / 2;

			segments = m_VerticalSegments[minimalindex];
			for(int i = 0; i < segments.size(); i++)
			{
				edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

				CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
				//pVertex->x(x2);
				if(fabs(pVertex->x() - x2) > 1e-8)
				{
					pVertex->x(x2);
					isChanged = TRUE;
				}

				pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
				//pVertex->x(x2);
				if(fabs(pVertex->x() - x2) > 1e-8)
				{
					pVertex->x(x2);
					isChanged = TRUE;
				}
			}
			IsUniformGridChanged = TRUE;
		}

		m_IsMergeHorizontal = FALSE;
		m_MergeIndex = minimalindex;
	}
	else
	{
		int size = yinterval.size();
		if(minimalindex > 0 && minimalindex < size - 2)
		{
			double y1 = yinterval[minimalindex - 1];
			double y4 = yinterval[minimalindex + 2];
			double y2 = (y1 * 2 + y4) / 3;
			double y3 = (y1 + y4 * 2) / 3;


			segments = m_HorizontalSegments[minimalindex];
			for(int i = 0; i < segments.size(); i++)
			{
				edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

				CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
				//pVertex->y(y2);
				if(fabs(pVertex->y() - y2) > 1e-8)
				{
					pVertex->y(y2);
					isChanged = TRUE;
				}

				pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
				//pVertex->y(y2);
				if(fabs(pVertex->y() - y2) > 1e-8)
				{
					pVertex->y(y2);
					isChanged = TRUE;
				}
			}

			segments = m_HorizontalSegments[minimalindex + 1];
			for(int i = 0; i < segments.size(); i++)
			{
				edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

				CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
				//pVertex->y(y3);
				if(fabs(pVertex->y() - y3) > 1e-8)
				{
					pVertex->y(y3);
					isChanged = TRUE;
				}

				pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
				//pVertex->y(y3);
				if(fabs(pVertex->y() - y3) > 1e-8)
				{
					pVertex->y(y3);
					isChanged = TRUE;
				}
			}
			IsUniformGridChanged = TRUE;
		}else if(minimalindex == 0)
		{
			double y1 = yinterval[minimalindex];
			double y3 = yinterval[minimalindex + 2];
			double y2 = (y1 + y3) / 2;

			segments = m_HorizontalSegments[minimalindex + 1];
			for(int i = 0; i < segments.size(); i++)
			{
				edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

				CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
				//pVertex->y(y2);
				if(fabs(pVertex->y() - y2) > 1e-8)
				{
					pVertex->y(y2);
					isChanged = TRUE;
				}

				pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
				//pVertex->y(y2);
				if(fabs(pVertex->y() - y2) > 1e-8)
				{
					pVertex->y(y2);
					isChanged = TRUE;
				}
			}
			IsUniformGridChanged = TRUE;
		}else if(minimalindex == size - 2)
		{
			double y1 = yinterval[minimalindex - 1];
			double y3 = yinterval[minimalindex + 1];
			double y2 = (y1 + y3) / 2;

			segments = m_HorizontalSegments[minimalindex];
			for(int i = 0; i < segments.size(); i++)
			{
				edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

				CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
				//pVertex->y(y2);
				if(fabs(pVertex->y() - y2) > 1e-8)
				{
					pVertex->y(y2);
					isChanged = TRUE;
				}

				pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
				//pVertex->y(y2);
				if(fabs(pVertex->y() - y2) > 1e-8)
				{
					pVertex->y(y2);
					isChanged = TRUE;
				}
			}
			IsUniformGridChanged = TRUE;
		}

		m_IsMergeHorizontal = TRUE;
		m_MergeIndex = minimalindex;
	}
	return isChanged;
}
BOOL Action_PolyOmino::HasMergeConstraints(int minimalindex,BOOL isVertical)
{
	if(isVertical)
	{
		for(int i = 0; i < pUniformGridMesh->m_ArrayMergeConstraints.size(); i++)
		{
			std::pair<int,int> pair = pUniformGridMesh->m_ArrayMergeConstraints[i];
			if(std::find(m_VerticalSegments[minimalindex].begin(),m_VerticalSegments[minimalindex].end(),pair.first) != m_VerticalSegments[minimalindex].end() &&
			   std::find(m_VerticalSegments[minimalindex + 1].begin(),m_VerticalSegments[minimalindex + 1].end(),pair.second) != m_VerticalSegments[minimalindex + 1].end() ||
			   std::find(m_VerticalSegments[minimalindex].begin(),m_VerticalSegments[minimalindex].end(),pair.second) != m_VerticalSegments[minimalindex].end() && 
			   std::find(m_VerticalSegments[minimalindex + 1].begin(),m_VerticalSegments[minimalindex + 1].end(),pair.first) != m_VerticalSegments[minimalindex + 1].end())
				return TRUE;
		}
		return FALSE;
	}else
	{
		for(int i = 0; i < pUniformGridMesh->m_ArrayMergeConstraints.size(); i++)
		{
			std::pair<int,int> pair = pUniformGridMesh->m_ArrayMergeConstraints[i];
			if(std::find(m_HorizontalSegments[minimalindex].begin(),m_HorizontalSegments[minimalindex].end(),pair.first) != m_HorizontalSegments[minimalindex].end() &&
				std::find(m_HorizontalSegments[minimalindex + 1].begin(),m_HorizontalSegments[minimalindex + 1].end(),pair.second) != m_HorizontalSegments[minimalindex + 1].end() ||
				std::find(m_HorizontalSegments[minimalindex].begin(),m_HorizontalSegments[minimalindex].end(),pair.second) != m_HorizontalSegments[minimalindex].end() && 
				std::find(m_HorizontalSegments[minimalindex + 1].begin(),m_HorizontalSegments[minimalindex + 1].end(),pair.first) != m_HorizontalSegments[minimalindex + 1].end())
				return TRUE;
		}
		return FALSE;
	}
}
void Action_PolyOmino::MergeInterval(std::vector<double> &xinterval,std::vector<double> &yinterval,int minimalindex,BOOL isVertical)
{
	std::vector<int> segments;
	std::vector<int> changedvertices;
	std::pair<int,int> edgepair;
	if(isVertical)
	{
		int minimalxindex = minimalindex;
		//Move The related Vertical edges.
		double xnewval = (xinterval[minimalxindex + 1] + xinterval[minimalxindex]) / 2;

		segments = m_VerticalSegments[minimalxindex];
		for(int i = 0; i < segments.size(); i++)
		{
			edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

			CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
			pVertex->x(xnewval);
			//int index = SearchSamePositionVertices(pUniformGridMesh,changedvertices,pVertex->m_Flag);
			//if(index >= 0)
			//{
			//	edgepair.first = index;
			//	pUniformGridMesh->m_ArraySharpEdges[segments[i]] = edgepair;
			//}else
			//	changedvertices.push_back(pVertex->m_Flag);

			pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
			pVertex->x(xnewval);
			//index = SearchSamePositionVertices(pUniformGridMesh,changedvertices,pVertex->m_Flag);
			//if(index >= 0)
			//{
			//	edgepair.second = index;
			//	pUniformGridMesh->m_ArraySharpEdges[segments[i]] = edgepair;
			//}else
			//	changedvertices.push_back(pVertex->m_Flag);
		}

		segments = m_VerticalSegments[minimalxindex + 1];
		for(int i = 0; i < segments.size(); i++)
		{
			edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

			CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
			pVertex->x(xnewval);
			//int index = SearchSamePositionVertices(pUniformGridMesh,changedvertices,pVertex->m_Flag);
			//if(index >= 0)
			//{
			//	edgepair.first = index;
			//	pUniformGridMesh->m_ArraySharpEdges[segments[i]] = edgepair;
			//}else
			//	changedvertices.push_back(pVertex->m_Flag);

			pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
			pVertex->x(xnewval);
			//index = SearchSamePositionVertices(pUniformGridMesh,changedvertices,pVertex->m_Flag);
			//if(index >= 0)
			//{
			//	edgepair.second = index;
			//	pUniformGridMesh->m_ArraySharpEdges[segments[i]] = edgepair;
			//}else
			//	changedvertices.push_back(pVertex->m_Flag);
		}

		//Merge
		//m_VerticalSegments[minimalxindex].push_back(m_VerticalSegments[minimalxindex + 1]);
		m_VerticalSegments[minimalxindex].insert(m_VerticalSegments[minimalxindex].end(),m_VerticalSegments[minimalxindex + 1].begin(),m_VerticalSegments[minimalxindex + 1].end());
		m_VerticalSegments.erase(m_VerticalSegments.begin() + minimalxindex + 1);
		m_IsMergeHorizontal = FALSE;
		m_MergeIndex = minimalxindex;
		IsUniformGridChanged = TRUE;
	}else
	{
		int minimalyindex = minimalindex;
		//Move The two Horizontal edges.
		double ynewval = (yinterval[minimalyindex + 1] + yinterval[minimalyindex]) / 2;

		segments = m_HorizontalSegments[minimalyindex];
		for(int i = 0; i < segments.size(); i++)
		{
			edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

			CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
			pVertex->y(ynewval);
			//int index = SearchSamePositionVertices(pUniformGridMesh,changedvertices,pVertex->m_Flag);
			//if(index >= 0)
			//{
			//	edgepair.first = index;
			//	pUniformGridMesh->m_ArraySharpEdges[segments[i]] = edgepair;
			//}else
			//	changedvertices.push_back(pVertex->m_Flag);

			pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
			pVertex->y(ynewval);
			//index = SearchSamePositionVertices(pUniformGridMesh,changedvertices,pVertex->m_Flag);
			//if(index >= 0)
			//{
			//	edgepair.second = index;
			//	pUniformGridMesh->m_ArraySharpEdges[segments[i]] = edgepair;
			//}else
			//	changedvertices.push_back(pVertex->m_Flag);
		}

		segments = m_HorizontalSegments[minimalyindex + 1];
		for(int i = 0; i < segments.size(); i++)
		{
			edgepair = pUniformGridMesh->m_ArraySharpEdges[segments[i]];

			CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
			pVertex->y(ynewval);
			//int index = SearchSamePositionVertices(pUniformGridMesh,changedvertices,pVertex->m_Flag);
			//if(index >= 0)
			//{
			//	edgepair.first = index;
			//	pUniformGridMesh->m_ArraySharpEdges[segments[i]] = edgepair;
			//}else
			//	changedvertices.push_back(pVertex->m_Flag);

			pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
			pVertex->y(ynewval);
			//index = SearchSamePositionVertices(pUniformGridMesh,changedvertices,pVertex->m_Flag);
			//if(index >= 0)
			//{
			//	edgepair.second = index;
			//	pUniformGridMesh->m_ArraySharpEdges[segments[i]] = edgepair;
			//}else
			//	changedvertices.push_back(pVertex->m_Flag);
		}

		//Merge
		m_HorizontalSegments[minimalyindex].insert(m_HorizontalSegments[minimalyindex].end(),m_HorizontalSegments[minimalyindex + 1].begin(),m_HorizontalSegments[minimalyindex + 1].end());
		m_HorizontalSegments.erase(m_HorizontalSegments.begin() + minimalyindex + 1);
		m_IsMergeHorizontal = TRUE;
		m_MergeIndex = minimalyindex;
		IsUniformGridChanged = TRUE;
	}
}
void Action_PolyOmino::IsValidMesh()
{
	std::pair<int,int> pair;
	//Check the valid
	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		int start = pair.first;
		int end = pair.second;
		CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(start);
		CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(end);
		double x1 = pStartVertex->x();
		double y1 = pStartVertex->y();
		double x2 = pEndVertex->x();
		double y2 = pEndVertex->y();
		if(fabs(y1 - y2) > 1e-8 && fabs(x1 - x2) > 1e-8)
		{
			ASSERT(0);
		}
	}
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		std::vector<int> edgeset = m_HorizontalSegments[i];
		for(int j = 0; j < edgeset.size(); j++)
		{
			pair = pUniformGridMesh->m_ArraySharpEdges[edgeset[j]];
			int start = pair.first;
			int end = pair.second;
			CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(start);
			CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(end);
			double x1 = pStartVertex->x();
			double y1 = pStartVertex->y();
			double x2 = pEndVertex->x();
			double y2 = pEndVertex->y();
			if(fabs(y1 - y2) > 1e-8 )
			{
				ASSERT(0);
			}
		}
	}
	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		std::vector<int> edgeset = m_VerticalSegments[i];
		for(int j = 0; j < edgeset.size(); j++)
		{
			pair = pUniformGridMesh->m_ArraySharpEdges[edgeset[j]];
			int start = pair.first;
			int end = pair.second;
			CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(start);
			CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(end);
			double x1 = pStartVertex->x();
			double y1 = pStartVertex->y();
			double x2 = pEndVertex->x();
			double y2 = pEndVertex->y();
			if(fabs(x1 - x2) > 1e-8 )
			{
				ASSERT(0);
			}
		}
	}

	std::vector<int> m_CornerPointArray;
	m_CornerPointArray.resize(pUniformGridMesh->m_ArrayVertex.GetSize(),0);

	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int,int> data;
		data = pUniformGridMesh->m_ArraySharpEdges[i];
		m_CornerPointArray[data.first]++; 
		m_CornerPointArray[data.second]++; 
	}
	for(int i = 0; i < m_CornerPointArray.size(); i++)
	{
		if(m_CornerPointArray[i] == 1)
			ASSERT(0);
	}

	if(CheckVerticalEdgeIntersection())
		ASSERT(0);

	if(CheckHorizontalEdgeIntersection())
		ASSERT(0);
}
void Action_PolyOmino::OnBnClickedUniformgridProgessiveMeshIteration()
{
	// TODO: Add your control notification handler code he

	int iterationnum = 0;
	
	char filename[200];
	while(1)
	{	
		IsUniformGridChanged = FALSE;
		if(iterationnum == 5)
		{			
			int ii = 0;
			//CMesh3d* pNewMesh = new CMesh3d();
			//pNewMesh->Copy(pUniformGridMesh);
			//pDoc->m_SceneGraph.Add(pNewMesh);
			//pDoc->UpdateTreeControl();
		}
		//check valid
		if(iterationnum > 0)
			CheckandFixTopologyValidation(iterationnum);

		IsValidMesh();
		if(pUniformGridMesh->m_ArraySharpEdges.size() <= 4)
			break;
		/*if(iterationnum == 5)
		{
			CMesh3d* pTestMesh1 = new CMesh3d();
			pTestMesh1->Copy(pUniformGridMesh);
			CMesh3d* pTestMesh2 = new CMesh3d();
			pTestMesh2->Copy(pUniformGridMesh);
			CMesh3d* pTestMesh3 = new CMesh3d();
			pTestMesh3->Copy(pUniformGridMesh);
			std::pair<int,int> edge1;
			std::pair<int,int> edge2,edge3;
			//pTestMesh1->m_ArraySeletedVertexIndex.push_back(585);
			//pTestMesh1->m_ArraySeletedVertexIndex.push_back(1324);
			edge1.first = 637;
			edge1.second = 635;
			edge2.first = 635;
			edge2.second = 1968;
			edge3.first = 1968;
			edge3.second = 1969;
			//edge1 = pUniformGridMesh->m_ArraySharpEdges[764];
			//edge2 = pUniformGridMesh->m_ArraySharpEdges[1534];
			pTestMesh1->m_ArraySharpEdges.clear();
			pTestMesh1->m_ArraySharpEdges.push_back(edge1);
			pTestMesh2->m_ArraySharpEdges.clear();
			pTestMesh2->m_ArraySharpEdges.push_back(edge2);
			pTestMesh3->m_ArraySharpEdges.clear();
			pTestMesh3->m_ArraySharpEdges.push_back(edge3);
			pDoc->m_SceneGraph.Add(pTestMesh1);
			pDoc->m_SceneGraph.Add(pTestMesh2);
			pDoc->m_SceneGraph.Add(pTestMesh3);
			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);
			return;
		}*/

		//if(num == 93)
		//	break;
		//return;
		//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.m",num);
		sprintf(filename,"D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\%d.m",iterationnum);
		pUniformGridMesh->SaveMFile(filename);
		//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
		sprintf(filename,"D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\%d.txt",iterationnum);
		SaveDebugInformation(filename);
		//
		//SaveObjMesh(iterationnum);

		//int currentfacenum = ComputeFaceNum();
		//if(currentfacenum < m_initialfacenum)
		//	break;

		iterationnum++;
		int flag = UniformSimplificationOneIteration();

		sprintf(filename,"D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\%d_.m",iterationnum - 1);
		pUniformGridMesh->SaveMFile(filename);

		if(!flag)
			break;
		if(!IsUniformGridChanged)
			break;
	}
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::UniformgridProgessiveMeshOneIteration()
{
	// TODO: Add your control notification handler code he

	int iterationnum = 0;
	
	char filename[200];
	//while(1)
	{	
		IsUniformGridChanged = FALSE;
		if(iterationnum == 5)
		{			
			int ii = 0;
			//CMesh3d* pNewMesh = new CMesh3d();
			//pNewMesh->Copy(pUniformGridMesh);
			//pDoc->m_SceneGraph.Add(pNewMesh);
			//pDoc->UpdateTreeControl();
		}
		//check valid
		
		//if(pUniformGridMesh->m_ArraySharpEdges.size() <= 4)
		//	break;
		/*if(iterationnum == 5)
		{
			CMesh3d* pTestMesh1 = new CMesh3d();
			pTestMesh1->Copy(pUniformGridMesh);
			CMesh3d* pTestMesh2 = new CMesh3d();
			pTestMesh2->Copy(pUniformGridMesh);
			CMesh3d* pTestMesh3 = new CMesh3d();
			pTestMesh3->Copy(pUniformGridMesh);
			std::pair<int,int> edge1;
			std::pair<int,int> edge2,edge3;
			//pTestMesh1->m_ArraySeletedVertexIndex.push_back(585);
			//pTestMesh1->m_ArraySeletedVertexIndex.push_back(1324);
			edge1.first = 637;
			edge1.second = 635;
			edge2.first = 635;
			edge2.second = 1968;
			edge3.first = 1968;
			edge3.second = 1969;
			//edge1 = pUniformGridMesh->m_ArraySharpEdges[764];
			//edge2 = pUniformGridMesh->m_ArraySharpEdges[1534];
			pTestMesh1->m_ArraySharpEdges.clear();
			pTestMesh1->m_ArraySharpEdges.push_back(edge1);
			pTestMesh2->m_ArraySharpEdges.clear();
			pTestMesh2->m_ArraySharpEdges.push_back(edge2);
			pTestMesh3->m_ArraySharpEdges.clear();
			pTestMesh3->m_ArraySharpEdges.push_back(edge3);
			pDoc->m_SceneGraph.Add(pTestMesh1);
			pDoc->m_SceneGraph.Add(pTestMesh2);
			pDoc->m_SceneGraph.Add(pTestMesh3);
			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);
			return;
		}*/

		//if(num == 93)
		//	break;
		//return;
		//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.m",num);
		sprintf(filename,"D:\\papers\\code\\data\\Geo\\dense\\optimization\\%d.m",iterationnum);
		pUniformGridMesh->SaveMFile(filename);
		//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
		sprintf(filename,"D:\\papers\\code\\data\\Geo\\dense\\optimization\\%d.txt",iterationnum);
		SaveDebugInformation(filename);
		//
		//SaveObjMesh(iterationnum);

		//int currentfacenum = ComputeFaceNum();
		//if(currentfacenum < m_initialfacenum)
		//	break;

		//iterationnum++;
		int flag = UniformSimplificationOneIteration();

		//
		CheckandFixTopologyValidation(iterationnum);

		//
		IsValidMesh();
		//sprintf(filename,"D:\\papers\\code\\data\\Geo\\dense\\optimization\\%d_.m",iterationnum - 1);
		//pUniformGridMesh->SaveMFile(filename);
	}
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::SaveDebugInformation(char *filename)
{
	CString s1;

	std::ofstream file(filename);
	std::ifstream *texturefile = NULL;
	COLORREF color;
	double r,g,b;

	int currentfacenum = ComputeFaceNum();
	file << currentfacenum << "\n";

	//initial face num
	file << m_initialfacenum << "\n";

	std::pair<int,int> pair;
	int num = pUniformGridMesh->m_ArraySharpEdges.size();
	file << num << "\n";
	//first selected edges
	for(int i = 0; i < num; i++)
	{
		pair = pUniformGridMesh->m_ArraySharpEdges[i];
		file << pair.first << " " << pair.second << "\n";
	}
	
	//horizontal edges
	num = m_HorizontalSegments.size();
	file << num << "\n";
	for(int i = 0; i < num; i++)
	{
		int num1 = m_HorizontalSegments[i].size();
		file << num1 << "\n";
		for(int j = 0; j < num1; j++)
		{
			file << m_HorizontalSegments[i][j] << " ";
		}
		file << "\n";
	}
	//vertical edges
	num = m_VerticalSegments.size();
	file << num << "\n";
	for(int i = 0; i < num; i++)
	{
		int num1 = m_VerticalSegments[i].size();
		file << num1 << "\n";
		for(int j = 0; j < num1; j++)
		{
			file << m_VerticalSegments[i][j] << " ";
		}
		file << "\n";
	}

	//constraints
	num = pUniformGridMesh->m_ArrayMergeConstraints.size();
	file << num << "\n";
	for(int i = 0; i < num; i++)
	{
		std::pair<int,int> pair = pUniformGridMesh->m_ArrayMergeConstraints[i];
		file << pair.first << " " << pair.second << "\n";
	}
	file.close();
}
void Action_PolyOmino::OnBnClickedPolyominoShortestpath()
{
	// TODO: Add your control notification handler code here
	if(pMesh->m_ArraySharpVertexIndex.size() != 2)
		return;
	std::vector<int> path;
	pMesh->ComputeShortestPath(pMesh->m_ArraySharpVertexIndex[0],pMesh->m_ArraySharpVertexIndex[1],path);
	pMesh->m_ArraySharpVertexIndex = path;
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoSaveMLandmark()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(FALSE);
	if(dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}else
		return;
	//CString s1 = "D:\\papers\\code\\bin_Curve\\lamcc_dir_div_merge.m"; 
	//CString s1 = "D:\\papers\\code\\bin_Curve\\USA_33.m";

	std::ofstream file(FileName);
	std::ifstream *texturefile = NULL;
	COLORREF color;
	double r,g,b;

	//CImage m_TextureImage; 
	for(int i  = 0; i < pMesh->NbVertex(); i++)
	{

		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->m_Flag = i + 1;

		//file << "Vertex " << i + 1 << " " << pVertex->m_Coord[0] << " " << pVertex->m_Coord[1] << " " << pVertex->m_Coord[2] ;
		file << "Vertex " << pVertex->m_OriginalIndex << " " << pVertex->m_Coord[0] << " " << pVertex->m_Coord[1] << " " << pVertex->m_Coord[2] ;
		file << " {";
		/*if(hasvt)
		{
			color = m_TextureImage.GetPixel(m_TextureImage.GetWidth() * pVertex->u,m_TextureImage.GetHeight() * (1 - pVertex->v));

			b = ((color & 0xff0000) >> 16);
			g = ((color & 0x00ff00) >> 8);
			r = (color & 0x0000ff);

			file << "rgb=("<<r/255.0<<" "<<g/255.0<<" "<<b/255.0<<") ";*/
			//file << "uv=(" << pVertex->u << " " << pVertex->v << ") ";
			//file << "uv=(" << pVertex->m_Coord[0] << " " << pVertex->m_Coord[1] << ") ";
		//}

		file << "rgb=(" << pVertex->GetColor()->r() /255.0 << " "<< pVertex->GetColor()->g() /255.0 << " " << pVertex->GetColor()->b() /255.0<<") ";
		if(1)
		{
			file << "normal=(" << pVertex->GetNormal()->x() << " "<< pVertex->GetNormal()->y() << " " << pVertex->GetNormal()->z()<<") ";
		}
		file << "}\n";
	}
	for(int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* pFace3d = pMesh->GetFace(i);
		file << "Face " << i + 1 <<" " << pFace3d->v1()->m_OriginalIndex << " " << pFace3d->v2()->m_OriginalIndex << " " << pFace3d->v3()->m_OriginalIndex << "\n";
	}

	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		for(int j = 0; j < pLandMark->landmark_points.size() - 1;j++)
		{
			CVertex3d* pVertex1 = pMesh->GetVertex(pMesh->m_LandMarkArray[i]->landmark_points[j]);
			CVertex3d* pVertex2 = pMesh->GetVertex(pMesh->m_LandMarkArray[i]->landmark_points[j + 1]);
			file<< "Edge " << " " << pVertex1->m_OriginalIndex << " " << pVertex2->m_OriginalIndex << " {sharp landmark=("<<i<<") dir=("<<pMesh->m_LandMarkArray[i]->style<<")}" << "\n";
		}
	}
	file.close();
}

void Action_PolyOmino::OnBnClickedPolyominoTuteembedding()
{
	std::vector<std::pair<int,int>> m_BoundaryVertices;
	std::vector<std::vector<int>> m_ArrayPointConnectedSelectEdgeIndex;
	BOOL isUniform = TRUE;
	std::vector<std::pair<int,std::pair<double,double>>> m_InteriorPointsArray;
	std::vector<double> boundaryedgelength;
	std::vector<int> m_boundaryRingVertices;
	//Fix the boundary 
	Compute_Boundary_Vertices_Parameter_Positions_TutteEmbedding(pMesh,m_boundaryRingVertices,m_BoundaryVertices,m_ArrayPointConnectedSelectEdgeIndex,boundaryedgelength,isUniform);

	//Compute the Linear System
	Compute_Linear_Equations_TutteEmbedding(pMesh,m_BoundaryVertices,m_ArrayPointConnectedSelectEdgeIndex,m_InteriorPointsArray,isUniform);

	isUniform = FALSE;

	Compute_Boundary_Vertices_Parameter_Positions_TutteEmbedding(pMesh,m_boundaryRingVertices,m_BoundaryVertices,m_ArrayPointConnectedSelectEdgeIndex,boundaryedgelength,isUniform);

	//Compute the Linear System
	Compute_Linear_Equations_TutteEmbedding(pMesh,m_BoundaryVertices,m_ArrayPointConnectedSelectEdgeIndex,m_InteriorPointsArray,isUniform);

	//
	
	//Harmonic_ConvexSubdivision(pMesh,m_boundaryRingVertices,m_BoundaryVertices,m_ArrayPointConnectedSelectEdgeIndex,m_InteriorPointsArray,isUniform);
}
void Action_PolyOmino::Harmonic_ConvexSubdivision(CMesh3d* pMesh3D,std::vector<int> &m_boundaryRingVertices,std::vector<std::pair<int,int>>& m_BoundaryVertices,std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex,std::vector<std::pair<int,std::pair<double,double>>> &m_InteriorPointsArray,BOOL IsUniform)
{	
	//first fix the boundary uv coordinates
	double sum = ComputeEdgeLength(pMesh3D,m_boundaryRingVertices);
	std::vector<int> vertices;
	double angle = PI * 2;
	//First Fix all the boundaries vertices
	for(int i = 0; i < m_boundaryRingVertices.size(); i++)
	{
		vertices.clear();
		vertices.insert(vertices.begin(),m_boundaryRingVertices.begin(),m_boundaryRingVertices.begin() + i);
		double length = ComputeEdgeLength(pMesh3D,vertices);

		CVertex3d* pVertex = pMesh3D->GetVertex(m_boundaryRingVertices[i]);
		if(IsUniform)
		{
			pVertex->u = cos(angle * i / m_BoundaryVertices.size()+ PI / 2);
			pVertex->v = sin(angle * i / m_BoundaryVertices.size()+ PI / 2);
		}else
		{
			pVertex->u = cos(angle * length / sum + PI / 2);
			pVertex->v = sin(angle * length / sum + PI / 2);
		}
	}

	//
	Compute_Linear_Equations_TutteEmbedding_ConvexSubdivision(pMesh3D,m_BoundaryVertices,m_ArrayPointConnectedSelectEdgeIndex,m_InteriorPointsArray,IsUniform);

}

void Action_PolyOmino::Compute_Boundary_Vertices_Parameter_Positions_TutteEmbedding_FixOuterBoundary(CMesh3d* pMesh3D, std::vector<int> &m_boundaryRingVertices, std::vector<std::pair<int, int>>& m_BoundaryVertices, std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex, std::vector<double> &boundaryedgelength, BOOL IsUniform)
{
	if (m_BoundaryVertices.size() == 0)
	{
		//Find all the corners
		std::vector<int> m_CornerPointArray;
		m_CornerPointArray.resize(pMesh3D->m_ArrayVertex.GetSize(), 0);

		for (int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
		{
			GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[i];
			std::pair<int, int> data;
			m_CornerPointArray[pLandmark->landmark_points[0]]++;
			m_CornerPointArray[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]]++;
		}
		m_ArrayPointConnectedSelectEdgeIndex.resize(pMesh3D->m_ArrayVertex.GetSize());
		for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		{
			GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[i];
			m_ArrayPointConnectedSelectEdgeIndex[pLandmark->landmark_points[0]].push_back(i);
			m_ArrayPointConnectedSelectEdgeIndex[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]].push_back(i);
		}

		//first find the boundary vertices

		for (int i = 0; i < m_CornerPointArray.size(); i++)
		{
			CVertex3d* pVertex = pMesh3D->GetVertex(i);
			if (m_CornerPointArray[i] > 0 && pVertex->IsOnBoundary())
			{
				m_BoundaryVertices.push_back(std::make_pair(i, i));
			}
		}

		//Set Vertex Flag
		pMesh3D->SetVertexFlagAsItsIndex();

		//sort the boundary vertices by their sequence on the boundaries
		//first vertex
		int firstindex = m_BoundaryVertices[0].first;
		//int firstindex = 6589;
		CVertex3d* pStartVertex = pMesh3D->GetVertex(firstindex);

		//find all the vertices on the boundary

		m_boundaryRingVertices.push_back(pStartVertex->m_Flag);
		for (int i = 0; i < pStartVertex->NbVertexNeighbor(); i++)
		{
			CVertex3d* pVertex = pStartVertex->GetVertexNeighbor(i);
			if (pVertex != pStartVertex && pVertex->IsOnBoundary() && pMesh3D->IsEdgeOnBoundary(pStartVertex->m_Flag, pVertex->m_Flag))
			{
				m_boundaryRingVertices.push_back(pVertex->m_Flag);
				break;
			}
		}

		//
		while (1)
		{
			int previousvindex = m_boundaryRingVertices[m_boundaryRingVertices.size() - 2];
			int vindex = m_boundaryRingVertices[m_boundaryRingVertices.size() - 1];

			CVertex3d* pPreviousVertex = pMesh3D->GetVertex(previousvindex);
			CVertex3d* pCurrentVertex = pMesh3D->GetVertex(vindex);
			for (int i = 0; i < pCurrentVertex->NbVertexNeighbor(); i++)
			{
				CVertex3d* pVertex = pCurrentVertex->GetVertexNeighbor(i);
				if (pVertex != pPreviousVertex && pVertex->IsOnBoundary() && pMesh3D->IsEdgeOnBoundary(pCurrentVertex->m_Flag, pVertex->m_Flag))
				{
					m_boundaryRingVertices.push_back(pVertex->m_Flag);
					break;
				}
			}
			if (m_boundaryRingVertices[0] == m_boundaryRingVertices[m_boundaryRingVertices.size() - 1])
				break;
		}

		//delete the end vertex
		m_boundaryRingVertices.erase(m_boundaryRingVertices.begin() + m_boundaryRingVertices.size() - 1);
		std::vector<int>::iterator biterator;
		for (int i = 0; i < m_BoundaryVertices.size(); i++)
		{
			biterator = std::find(m_boundaryRingVertices.begin(), m_boundaryRingVertices.end(), m_BoundaryVertices[i].first);
			if (biterator == m_boundaryRingVertices.end())
			{
				m_BoundaryVertices.erase(m_BoundaryVertices.begin() + i);
				i--;
				continue;
				//ASSERT(0);
			}
			m_BoundaryVertices[i].second = biterator - m_boundaryRingVertices.begin();
		}
		struct sort_pred {
			bool operator()(const std::pair<int, int> &left, const std::pair<int, int> &right) {
				return left.second < right.second;
			}
		};
		std::sort(m_BoundaryVertices.begin(), m_BoundaryVertices.end(), sort_pred());

		std::vector<int> vertices;
		double sum = 0;
		for (int i = 0; i < m_BoundaryVertices.size() - 1; i++)
		{
			vertices.clear();
			vertices.insert(vertices.begin(), m_boundaryRingVertices.begin() + m_BoundaryVertices[i].second, m_boundaryRingVertices.begin() + m_BoundaryVertices[i + 1].second + 1);
			double length = ComputeEdgeLength(pMesh3D, vertices);
			sum += length;
			boundaryedgelength.push_back(sum);

		}
		vertices.clear();
		vertices.insert(vertices.begin(), m_boundaryRingVertices.begin() + m_BoundaryVertices[m_BoundaryVertices.size() - 1].second, m_boundaryRingVertices.end());
		vertices.insert(vertices.end(), m_boundaryRingVertices.begin(), m_boundaryRingVertices.begin() + 1);
		double length = ComputeEdgeLength(pMesh3D, vertices);
		sum += length;
		for (int i = 0; i < m_BoundaryVertices.size() - 1; i++)
		{
			boundaryedgelength[i] = boundaryedgelength[i] / sum;
		}

		boundaryedgelength.insert(boundaryedgelength.begin(), 0);
	}
	double angle = PI * 2;
	for (int i = 0; i < m_BoundaryVertices.size(); i++)
	{
		CVertex3d* pVertex = pMesh3D->GetVertex(m_BoundaryVertices[i].first);

		pVertex->u = pVertex->x();
		pVertex->v = pVertex->y();
	}
}
void Action_PolyOmino::Compute_Boundary_Vertices_Parameter_Positions_TutteEmbedding(CMesh3d* pMesh3D,std::vector<int> &m_boundaryRingVertices,std::vector<std::pair<int,int>>& m_BoundaryVertices,std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex,std::vector<double> &boundaryedgelength,BOOL IsUniform)
{
	// TODO: Add your control notification handler code here
	if(m_BoundaryVertices.size() == 0)
	{
			//Find all the corners
			std::vector<int> m_CornerPointArray;
			m_CornerPointArray.resize(pMesh3D->m_ArrayVertex.GetSize(),0);

			for(int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
			{
				GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[i];
				std::pair<int,int> data;
				m_CornerPointArray[pLandmark->landmark_points[0]]++; 
				m_CornerPointArray[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]]++; 
			}
			m_ArrayPointConnectedSelectEdgeIndex.resize(pMesh3D->m_ArrayVertex.GetSize());
			for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
			{
				GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[i];
				m_ArrayPointConnectedSelectEdgeIndex[pLandmark->landmark_points[0]].push_back(i);
				m_ArrayPointConnectedSelectEdgeIndex[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]].push_back(i);
			}

			//first find the boundary vertices

			for(int i = 0; i < m_CornerPointArray.size(); i++)
			{
				CVertex3d* pVertex = pMesh3D->GetVertex(i);
				if(m_CornerPointArray[i] >0 && pVertex->IsOnBoundary())
				{
					m_BoundaryVertices.push_back(std::make_pair (i,i));
				}
			}

			//Set Vertex Flag
			pMesh3D->SetVertexFlagAsItsIndex();

			//sort the boundary vertices by their sequence on the boundaries
			//first vertex
			int firstindex = m_BoundaryVertices[0].first;
			//int firstindex = 6589;
			CVertex3d* pStartVertex = pMesh3D->GetVertex(firstindex);

			//find all the vertices on the boundary

			m_boundaryRingVertices.push_back(pStartVertex->m_Flag);
			for(int i = 0; i < pStartVertex->NbVertexNeighbor(); i++)
			{
				CVertex3d* pVertex = pStartVertex->GetVertexNeighbor(i);
				if(pVertex != pStartVertex && pVertex->IsOnBoundary() && pMesh3D->IsEdgeOnBoundary(pStartVertex->m_Flag,pVertex->m_Flag))
				{
					m_boundaryRingVertices.push_back(pVertex->m_Flag);
					break;
				}
			}

			//
			while(1)
			{
				int previousvindex = m_boundaryRingVertices[m_boundaryRingVertices.size() - 2];
				int vindex = m_boundaryRingVertices[m_boundaryRingVertices.size() - 1];

				CVertex3d* pPreviousVertex = pMesh3D->GetVertex(previousvindex);
				CVertex3d* pCurrentVertex = pMesh3D->GetVertex(vindex);
				for(int i = 0; i < pCurrentVertex->NbVertexNeighbor(); i++)
				{
					CVertex3d* pVertex = pCurrentVertex->GetVertexNeighbor(i);
					if(pVertex != pPreviousVertex && pVertex->IsOnBoundary() && pMesh3D->IsEdgeOnBoundary(pCurrentVertex->m_Flag,pVertex->m_Flag))
					{
						m_boundaryRingVertices.push_back(pVertex->m_Flag);
						break;
					}
				}
				if(m_boundaryRingVertices[0] == m_boundaryRingVertices[m_boundaryRingVertices.size() - 1])
					break;
			}

			//delete the end vertex
			m_boundaryRingVertices.erase(m_boundaryRingVertices.begin() + m_boundaryRingVertices.size() - 1);
			std::vector<int>::iterator biterator;
			for(int i = 0; i < m_BoundaryVertices.size(); i++)
			{
				biterator = std::find(m_boundaryRingVertices.begin(),m_boundaryRingVertices.end(),m_BoundaryVertices[i].first);
				if(biterator == m_boundaryRingVertices.end())
				{
					m_BoundaryVertices.erase(m_BoundaryVertices.begin() + i);
					i--;
					continue;
					//ASSERT(0);
				}
				m_BoundaryVertices[i].second = biterator - m_boundaryRingVertices.begin();
			}
			struct sort_pred {
				bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) {
					return left.second < right.second;
				}
			};
			std::sort(m_BoundaryVertices.begin(),m_BoundaryVertices.end(),sort_pred());
		
		std::vector<int> vertices;
		double sum = 0;
		for(int i = 0; i < m_BoundaryVertices.size() - 1; i++)
		{
			vertices.clear();
			vertices.insert(vertices.begin(),m_boundaryRingVertices.begin() + m_BoundaryVertices[i].second,m_boundaryRingVertices.begin() + m_BoundaryVertices[i + 1].second + 1);
			double length = ComputeEdgeLength(pMesh3D,vertices);
			sum += length;
			boundaryedgelength.push_back(sum);
			
		}
		vertices.clear();
		vertices.insert(vertices.begin(),m_boundaryRingVertices.begin() + m_BoundaryVertices[m_BoundaryVertices.size() - 1].second,m_boundaryRingVertices.end());
		vertices.insert(vertices.end(), m_boundaryRingVertices.begin(), m_boundaryRingVertices.begin() + 1);
		double length = ComputeEdgeLength(pMesh3D,vertices);
		sum += length;
		for(int i = 0; i < m_BoundaryVertices.size() - 1; i++)
		{
			boundaryedgelength[i] = boundaryedgelength[i] / sum;
		}

		boundaryedgelength.insert(boundaryedgelength.begin(),0);
	}
	double angle = PI * 2;
	for(int i = 0; i < m_BoundaryVertices.size(); i++)
	{
		CVertex3d* pVertex = pMesh3D->GetVertex(m_BoundaryVertices[i].first);
		if(IsUniform)
		{
			pVertex->u = cos(angle * i / m_BoundaryVertices.size()+ PI / 2);
			pVertex->v = sin(angle * i / m_BoundaryVertices.size()+ PI / 2);
		}else
		{
			pVertex->u = cos(angle * boundaryedgelength[i] + PI / 2);
			pVertex->v = sin(angle * boundaryedgelength[i]+ PI / 2);
		}

	}
}
void Action_PolyOmino::Compute_Linear_Equations_TutteEmbedding(CMesh3d* pMesh3D,std::vector<std::pair<int,int>>& m_BoundaryVertices,std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex,std::vector<std::pair<int,std::pair<double,double>>> &m_InteriorPointsArray,BOOL IsUniform)
{

	//Use the m_OriginalIndex to store the interior vertex index


	//Set the vertices a non-boundary index 
	int m_interior_vertices_num = 0;
	for(int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
	{
		CVertex3d* pVertex = pMesh3D->GetVertex(i);
		if (pVertex->IsOnBoundary())
		{
			pVertex->m_interior_vertices_index = -1;
			continue;
		}

		if(m_ArrayPointConnectedSelectEdgeIndex[i].size() >= 3)
		{
			pVertex->m_interior_vertices_index = m_interior_vertices_num;
			m_interior_vertices_num++;
		}else
			pVertex->m_interior_vertices_index = -1;
	}

	std::vector<int> m_ArrayOneRingVertices;
	std::vector<double> omegaarray;
	std::vector<double> equation;
	std::vector<std::vector<double>> A;
	std::vector<double> b;
	std::vector<std::vector<double>> uvarray;
	uvarray.resize(2);
	OpenMesh::Vec3d v0,v_i,v_iminus1,v_iadd1;

	for(int ii = 0; ii < 2; ii++)
	{
		A.clear();
		b.clear();

		//all the linear equations
		for(int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
		{
			equation.clear();
			equation.resize(m_interior_vertices_num,0);
			if(m_ArrayPointConnectedSelectEdgeIndex[i].size() < 3)
				continue;

			int v0_index = pMesh3D->GetVertex(i)->m_interior_vertices_index;

			if (v0_index < 0)
				continue;
			//for each interior vertex
			m_ArrayOneRingVertices.clear();

			//search the connected vertices
			//for(int j = 0; j < m_ArrayPointConnectedSelectEdgeIndex[i].size(); j++)
			//{
				std::vector<int> landmarkset = m_ArrayPointConnectedSelectEdgeIndex[i];
				for(int k = 0; k < landmarkset.size(); k++)
				{
					GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[landmarkset[k]];
					int startindex = pLandmark->landmark_points[0];
					int endindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];

					if(startindex == i)
						m_ArrayOneRingVertices.push_back(endindex);
					else
					if(endindex == i)
						m_ArrayOneRingVertices.push_back(startindex);
					else
						ASSERT(0);
				}
			//}

			//
			omegaarray.clear();
			double sumlength = 0;
			for(int jj = 0; jj < m_ArrayOneRingVertices.size(); jj++)
			{
				if(IsUniform)
				{
					omegaarray.push_back(1.0);
					sumlength += 1.0;
				}
				else
				{
					double length = ComputeEdgeLength(pMesh3D,pMesh3D->m_LandMarkArray[landmarkset[jj]]->landmark_points);
					if(fabs(length) < 1e-8 || length < 0)
					{
						ASSERT(0);
					}
					length  = 1.0 / length;
					sumlength += length;
					omegaarray.push_back(length);
				}
				
				//omegaarray.push_back(1.0 / m_ArrayOneRingVertices.size());
			}
			for(int jj = 0; jj < m_ArrayOneRingVertices.size(); jj++)
			{
				omegaarray[jj] = omegaarray[jj] / sumlength;
				//omegaarray.push_back(1.0 / m_ArrayOneRingVertices.size());
			}

			double b1 = 0;
			for(int jj = 0; jj < m_ArrayOneRingVertices.size(); jj++)
			{
				int non_boundary_index = pMesh3D->GetVertex(m_ArrayOneRingVertices[jj])->m_interior_vertices_index;
				if(non_boundary_index == -1)
				{
					if(ii == 0)
						b1 += omegaarray[jj] * pMesh3D->GetVertex(m_ArrayOneRingVertices[jj])->u;
					else
						b1 += omegaarray[jj] * pMesh3D->GetVertex(m_ArrayOneRingVertices[jj])->v;
				}else
				{
					equation[non_boundary_index] = -omegaarray[jj];
				}
			}
			equation[v0_index] = 1;
			A.push_back(equation);
			b.push_back(b1);
		}
		
		p_FormView3->SolveLinearSystem(A,b,uvarray[ii]);
	}

	//Set the uv of interior vertices
	for(int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[i].size() < 3)
			continue;

		int v0_index = pMesh3D->GetVertex(i)->m_interior_vertices_index;
		if (v0_index < 0)
			continue;

		pMesh3D->GetVertex(i)->u = uvarray[0][v0_index];
		pMesh3D->GetVertex(i)->v = uvarray[1][v0_index];
	}

	//save
	CString s1;
	CString path = pMesh3D->filename;
	path = path.Left(path.ReverseFind('.'));
	path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	//if(IsUniform)
	//	s1 = "P:\\Dr_YYJ\\data\\" + path + "_ttuc_uniform.txt"; 
	//else
	//	s1 = "P:\\Dr_YYJ\\data\\" + path + "_ttuc_weighted.txt"; 

	if (IsUniform)
		s1 = "D:\\" + path + "_ttuc_uniform.txt";
	else
		s1 = "D:\\" + path + "_ttuc_weighted.txt";

	std::ofstream file(s1);

	//Generate a New Mesh
	CMesh3d* pNewMesh = new CMesh3d();
	//pNewMesh->SetTransform(*pMesh3D->GetTransform());
	Point3D lower,upper;
	//pMesh3D->ComputeBoundingBox(lower,upper);
	//double length = upper.x - lower.x;
	//if(upper.y - lower.y > length)
	//	length = upper.y - lower.y;
	//if(upper.z - lower.z > length)
	//	length = upper.z - lower.z;
	double length = 1;
	std::vector<int> m_PointIndex;
	m_PointIndex.resize(pMesh3D->NbVertex(),0);
	//first save the boundary
	for(int i = 0; i < m_BoundaryVertices.size(); i++)
	{
		CVertex3d* pOriginalVertex = pMesh3D->GetVertex(m_BoundaryVertices[i].first);
		CVertex3d* pVertex = new CVertex3d();
		pVertex->Set(pOriginalVertex->u * length,pOriginalVertex->v * length,0);
		pNewMesh->AddVertex(pVertex);
		m_PointIndex[m_BoundaryVertices[i].first] = pNewMesh->NbVertex() - 1;
		file << "Vertex " << pOriginalVertex->m_OriginalIndex << " " << pOriginalVertex->u  << " " << pOriginalVertex->v << " 0 " <<"\n";
	}
	/*for(int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[i].size() == 1)
		{
			CVertex3d* pOriginalVertex = pMesh3D->GetVertex(i);
			CVertex3d* pVertex = new CVertex3d();
			pVertex->Set(pOriginalVertex->u * length,pOriginalVertex->v * length,0);
			pNewMesh->AddVertex(pVertex);
			m_PointIndex[i] = pNewMesh->NbVertex() - 1;
			file << "Vertex " << pOriginalVertex->m_OriginalIndex << " " << pOriginalVertex->u  << " " << pOriginalVertex->v << " 0 " <<"\n";
		}
	}*/

	//then interior vertices
	/*for(int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[i].size() >= 3)
		{
			CVertex3d* pOriginalVertex = pMesh3D->GetVertex(i);
			CVertex3d* pVertex = new CVertex3d();
			pVertex->Set(pOriginalVertex->u * length,pOriginalVertex->v * length,0);
			pNewMesh->AddVertex(pVertex);
			m_PointIndex[i] = pNewMesh->NbVertex() - 1;
			file << "Vertex " << pOriginalVertex->m_OriginalIndex << " " << pOriginalVertex->u  << " " << pOriginalVertex->v << " 0 " <<"\n";
		}
	}*/
	std::pair<int,std::pair<double,double>> pair;
	if(m_InteriorPointsArray.size() == 0)
	{
	
		for(int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
		{
			if(m_ArrayPointConnectedSelectEdgeIndex[i].size() >= 3)
			{
				CVertex3d* pOriginalVertex = pMesh3D->GetVertex(i);
				pair.first = i;
				double angle = atan(pOriginalVertex->v / pOriginalVertex->u);
				if(pOriginalVertex->u< 0 && pOriginalVertex->v > 0)
				{
					angle = 3.1415926 - angle;
				}else
					if(pOriginalVertex->u< 0 && pOriginalVertex->v < 0)
					{
						angle = 3.1415926 + angle;
					}else
						if(pOriginalVertex->u > 0 && pOriginalVertex->v < 0)
						{
							angle = 3.1415926 * 2 + angle;
						}
						double length = sqrt(SQR(pOriginalVertex->u) + SQR(pOriginalVertex->v));
						pair.second.first = angle;
						pair.second.second = length;
						m_InteriorPointsArray.push_back(pair);
			}
		}


		struct sort_pred {
			bool operator()(const std::pair<int,std::pair<double,double>> &left, const std::pair<int,std::pair<double,double>> &right) 
			{
				if(fabs(left.second.first - right.second.first) < 1e-8)
					return left.second.second < right.second.second;
				else
					return left.second.first < right.second.first;
			}
		};
		std::sort(m_InteriorPointsArray.begin(),m_InteriorPointsArray.end(),sort_pred());
	}
	for(int i = 0; i < m_InteriorPointsArray.size(); i++)
	{
		pair = m_InteriorPointsArray[i];
		CVertex3d* pOriginalVertex = pMesh3D->GetVertex(pair.first);
		if(pOriginalVertex->IsOnBoundary())
		{
			CVertex3d* pVertex = new CVertex3d();
			pVertex->Set(pOriginalVertex->u * length,pOriginalVertex->v * length,0);
			pVertex->m_OriginalIndex = pOriginalVertex->m_OriginalIndex;
			pNewMesh->AddVertex(pVertex);
			m_PointIndex[pair.first] = pNewMesh->NbVertex() - 1;
			file << "Vertex " << pOriginalVertex->m_OriginalIndex << " " << pOriginalVertex->u  << " " << pOriginalVertex->v << " 0 " <<"\n";
		}
	}
	for(int i = 0; i < m_InteriorPointsArray.size(); i++)
	{

		pair = m_InteriorPointsArray[i];
		CVertex3d* pOriginalVertex = pMesh3D->GetVertex(pair.first);
		if(pOriginalVertex->IsOnBoundary())
			continue;
		CVertex3d* pVertex = new CVertex3d();
		pVertex->Set(pOriginalVertex->u * length,pOriginalVertex->v * length,0);
		pVertex->m_OriginalIndex = pOriginalVertex->m_OriginalIndex;
		pNewMesh->AddVertex(pVertex);
		m_PointIndex[pair.first] = pNewMesh->NbVertex() - 1;
		file << "Vertex " << pOriginalVertex->m_OriginalIndex << " " << pOriginalVertex->u  << " " << pOriginalVertex->v << " 0 " <<"\n";
	}
	file.close();
	pNewMesh->ComputeBoundingBox(lower,upper);

	CVector3d m_lower,m_upper;
	m_lower.Set(lower.x,lower.y,lower.z);
	m_upper.Set(upper.x,upper.y,upper.z);
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation = m_lower + m_upper;
	m_translation /= -2;
	length=m_upper.x()-m_lower.x();
	//float length1 = m_upper.y() - m_lower.y();
	if(length<m_upper.y()-m_lower.y())
		length=m_upper.y()-m_lower.y();
	if(length<m_upper.z()-m_lower.z())
		length=m_upper.z()-m_lower.z();

	CTransform transform;
	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1/length,1/length,1/length);
	transform.SetScale(&m_Scale);
	pNewMesh->SetTransform(transform);
	std::pair<int,int> pair1;
	for(int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		int startindex = pMesh3D->m_LandMarkArray[i]->landmark_points[0];
		int endindex = pMesh3D->m_LandMarkArray[i]->landmark_points[pMesh3D->m_LandMarkArray[i]->landmark_points.size() - 1];

		pair1.first = m_PointIndex[startindex];
		pair1.second = m_PointIndex[endindex];
		pNewMesh->m_ArraySharpEdges.push_back(pair1);
	}
	for(int i = 0; i < m_BoundaryVertices.size(); i++)
	{
		pair1.first = m_PointIndex[m_BoundaryVertices[i].first];
		pair1.second = m_PointIndex[m_BoundaryVertices[(i + 1) % m_BoundaryVertices.size()].first];
		pNewMesh->m_ArraySharpEdges.push_back(pair1);
	}
	pDoc->m_SceneGraph.Add(pNewMesh);
	pNewMesh->m_showSharpEdge = TRUE;
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::Compute_Linear_Equations_TutteEmbedding_ConvexSubdivision(CMesh3d* pMesh3D,std::vector<std::pair<int,int>>& m_BoundaryVertices,std::vector<std::vector<int>>& m_ArrayPointConnectedSelectEdgeIndex,std::vector<std::pair<int,std::pair<double,double>>> &m_InteriorPointsArray,BOOL IsUniform)
{
	for(int i = 0; i < pMesh3D->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3D->GetVertex(i);
		pVertex->m_Landmarknumber = -1;
	}
	for(int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		for(int j = 1; j < pMesh3D->m_LandMarkArray[i]->landmark_points.size() - 1; j++)
		{
			CVertex3d* pVertex = pMesh3D->GetVertex(pMesh3D->m_LandMarkArray[i]->landmark_points[j]);
			pVertex->m_Landmarknumber = i;
		}
	}
	//Set the vertices a non-boundary index 
	int m_interior_vertices_num = 0;
	for(int i = 0; i < pMesh3D->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3D->GetVertex(i);
		if(pVertex->IsOnBoundary() || m_ArrayPointConnectedSelectEdgeIndex[i].size() >= 3)
		{
			pVertex->m_interior_vertices_index = -1;
		}else
		if(pVertex->m_Landmarknumber >= 0)
		{
			pVertex->m_interior_vertices_index = m_interior_vertices_num;
			m_interior_vertices_num++;
		}else
		{
			pVertex->m_interior_vertices_index = m_interior_vertices_num;
			m_interior_vertices_num += 2;
		}
	}

	std::vector<int> m_ArrayOneRingVertices;
	std::vector<double> omegaarray;
	std::vector<double> equation;
	std::vector<std::vector<double>> A;
	std::vector<double> b;
	std::vector<double> uvarray;
	//uvarray.resize(2);
	Vector3D v0,v_i,v_iminus1,v_iadd1;

	//for(int ii = 0; ii < 2; ii++)
	//{
		//all the linear equations

		//first the vertices not on the landmarks
		for(int i = 0; i < pMesh3D->NbVertex(); i++)
		{
			CVertex3d* pVertex = pMesh3D->GetVertex(i);
			if(pVertex->m_interior_vertices_index == -1)
				continue;
			if(pVertex->m_Landmarknumber >= 0)
				continue;

			equation.clear();
			equation.resize(m_interior_vertices_num,0);
			int v0_index = pVertex->m_interior_vertices_index;
			m_ArrayOneRingVertices.clear();
			v0[0] = pVertex->x();
			v0[1] = pVertex->y();
			v0[2] = pVertex->z();

			for(int j = 0; j < pVertex->NbVertexNeighbor(); j++)
			{
				m_ArrayOneRingVertices.push_back(pVertex->GetVertexNeighbor(j)->m_Flag);
			}

			//first for the u
			omegaarray.clear();
			double sumweight = 0;
			//Vector3D v_i,v_iminus1,v_iadd1;
			//compute the weight
			for(int i = 0; i < m_ArrayOneRingVertices.size(); i++)
			{
				//
				CVertex3d* pVertex_v_i = pMesh3D->GetVertex(m_ArrayOneRingVertices[i]);
				v_i[0] = pVertex_v_i->x();
				v_i[1] = pVertex_v_i->y();
				v_i[2] = pVertex_v_i->z();

				CVertex3d* pVertex_v_iminus1 = pMesh3D->GetVertex(m_ArrayOneRingVertices[(i - 1 + m_ArrayOneRingVertices.size()) % m_ArrayOneRingVertices.size()]);
				v_iminus1[0] = pVertex_v_iminus1->x();
				v_iminus1[1] = pVertex_v_iminus1->y();
				v_iminus1[2] = pVertex_v_iminus1->z();

				CVertex3d* pVertex_v_iadd1 = pMesh3D->GetVertex(m_ArrayOneRingVertices[(i + 1) % m_ArrayOneRingVertices.size()]);
				v_iadd1[0] = pVertex_v_iadd1->x();
				v_iadd1[1] = pVertex_v_iadd1->y();
				v_iadd1[2] = pVertex_v_iadd1->z();

				double v0_v_iminus1 = Length(v0 - v_iminus1);
				double v0_v_i = Length(v0 - v_i);
				double v0_v_iadd1 = Length(v0 - v_iadd1);
				double vi_v_iadd1 = Length(v_i - v_iadd1);
				double vi_v_iminus1 = Length(v_i - v_iminus1);
				//double vi_v0 = Length(v_i - v0);

				double alpha_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(v0_v_i) - SQR(vi_v_iminus1)) / ( 2 * v0_v_iminus1 * v0_v_i));
				double alpha_i = acos((SQR(v0_v_iadd1) + SQR(v0_v_i) - SQR(vi_v_iadd1)) / ( 2 * v0_v_iadd1 * v0_v_i));
				double beta_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(vi_v_iminus1) - SQR(v0_v_i)) / ( 2 * v0_v_iminus1 * vi_v_iminus1));
				double gamai = acos((SQR(v0_v_iadd1) + SQR(vi_v_iadd1) - SQR(v0_v_i)) / ( 2 * v0_v_iadd1 * vi_v_iadd1));
				double w;		
				w = (tan(alpha_i_minus1 / 2) + tan(alpha_i / 2)) / v0_v_i;
				sumweight += w;
				omegaarray.push_back(w);
			}
			for(int i = 0; i < omegaarray.size(); i++)
			{
				omegaarray[i] = omegaarray[i] / sumweight;
			}

			double b1 = 0;
			for(int i = 0; i < m_ArrayOneRingVertices.size(); i++)
			{
				CVertex3d* pVertex = pMesh3D->GetVertex(m_ArrayOneRingVertices[i]);
				if(pVertex->m_interior_vertices_index == -1)
				{
					b1 += omegaarray[i] * pVertex->u;
				}else
				if(pVertex->m_Landmarknumber == -1)
				{
					equation[pVertex->m_interior_vertices_index] = -omegaarray[i];
					equation[pVertex->m_interior_vertices_index + 1] = 0;
				}else
				{
					GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[pVertex->m_Landmarknumber];
					int startindex = pLandmark->landmark_points[0];
					int endindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
					equation[pVertex->m_interior_vertices_index] = -omegaarray[i] * (pMesh3D->GetVertex(startindex)->u - pMesh3D->GetVertex(endindex)->u);
					b1 += omegaarray[i] * pMesh3D->GetVertex(endindex)->u;
				}
			}
			equation[v0_index] = 1;
			A.push_back(equation);
			b.push_back(b1);

			//
			equation.clear();
			equation.resize(m_interior_vertices_num,0);
			b1 = 0;
			for(int i = 0; i < m_ArrayOneRingVertices.size(); i++)
			{
				CVertex3d* pVertex = pMesh3D->GetVertex(m_ArrayOneRingVertices[i]);
				if(pVertex->m_interior_vertices_index == -1)
				{
					b1 += omegaarray[i] * pVertex->v;
				}else
					if(pVertex->m_Landmarknumber == -1)
					{
						equation[pVertex->m_interior_vertices_index] = 0;
						equation[pVertex->m_interior_vertices_index + 1] = -omegaarray[i];
					}else
					{
						GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[pVertex->m_Landmarknumber];
						int startindex = pLandmark->landmark_points[0];
						int endindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
						equation[pVertex->m_interior_vertices_index] = -omegaarray[i] * (pMesh3D->GetVertex(startindex)->v - pMesh3D->GetVertex(endindex)->v);
						b1 += omegaarray[i] * pMesh3D->GetVertex(endindex)->v;
					}
			}
			equation[v0_index] = 1;
			A.push_back(equation);
			b.push_back(b1);
		}

		for(int i = 0; i < pMesh3D->NbVertex(); i++)
		{
			CVertex3d* pVertex_current = pMesh3D->GetVertex(i);
			if(pVertex_current->m_Landmarknumber == -1)
				continue;

			equation.clear();
			equation.resize(m_interior_vertices_num,0);

			int v0_index = pVertex_current->m_interior_vertices_index;
			m_ArrayOneRingVertices.clear();

			for(int j = 0; j < pVertex_current->NbVertexNeighbor(); j++)
			{
				if(pVertex_current->GetVertexNeighbor(j)->m_Landmarknumber == pVertex_current->m_Landmarknumber ||
					m_ArrayPointConnectedSelectEdgeIndex[pVertex_current->GetVertexNeighbor(j)->m_Flag].size() >= 3)
					m_ArrayOneRingVertices.push_back(pVertex_current->GetVertexNeighbor(j)->m_Flag);
			}

			//first for the u
			omegaarray.clear();
			double sumweight = 0;
			Vector3D v_i,v_iminus1,v_iadd1;
			//compute the weight
			for(int i = 0; i < m_ArrayOneRingVertices.size(); i++)
			{
				//
				CVertex3d* pVertex_v_i = pMesh3D->GetVertex(m_ArrayOneRingVertices[i]);
				v_i[0] = pVertex_v_i->x();
				v_i[1] = pVertex_v_i->y();
				v_i[2] = pVertex_v_i->z();

				CVertex3d* pVertex_v_iminus1 = pMesh3D->GetVertex(m_ArrayOneRingVertices[(i - 1 + m_ArrayOneRingVertices.size()) % m_ArrayOneRingVertices.size()]);
				v_iminus1[0] = pVertex_v_iminus1->x();
				v_iminus1[1] = pVertex_v_iminus1->y();
				v_iminus1[2] = pVertex_v_iminus1->z();

				CVertex3d* pVertex_v_iadd1 = pMesh3D->GetVertex(m_ArrayOneRingVertices[(i + 1) % m_ArrayOneRingVertices.size()]);
				v_iadd1[0] = pVertex_v_iadd1->x();
				v_iadd1[1] = pVertex_v_iadd1->y();
				v_iadd1[2] = pVertex_v_iadd1->z();

				double v0_v_iminus1 = Length(v0 - v_iminus1);
				double v0_v_i = Length(v0 - v_i);
				double v0_v_iadd1 = Length(v0 - v_iadd1);
				double vi_v_iadd1 = Length(v_i - v_iadd1);
				double vi_v_iminus1 = Length(v_i - v_iminus1);
				//double vi_v0 = Length(v_i - v0);

				double alpha_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(v0_v_i) - SQR(vi_v_iminus1)) / ( 2 * v0_v_iminus1 * v0_v_i));
				double alpha_i = acos((SQR(v0_v_iadd1) + SQR(v0_v_i) - SQR(vi_v_iadd1)) / ( 2 * v0_v_iadd1 * v0_v_i));
				double beta_i_minus1 = acos((SQR(v0_v_iminus1) + SQR(vi_v_iminus1) - SQR(v0_v_i)) / ( 2 * v0_v_iminus1 * vi_v_iminus1));
				double gamai = acos((SQR(v0_v_iadd1) + SQR(vi_v_iadd1) - SQR(v0_v_i)) / ( 2 * v0_v_iadd1 * vi_v_iadd1));
				double w;		
				w = (tan(alpha_i_minus1 / 2) + tan(alpha_i / 2)) / v0_v_i;
				sumweight += w;
				omegaarray.push_back(w);
			}
			for(int i = 0; i < omegaarray.size(); i++)
			{
				omegaarray[i] = omegaarray[i] / sumweight;
			}

			double b1 = 0;
			GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[pVertex_current->m_Landmarknumber];
			int startindex = pLandmark->landmark_points[0];
			int endindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
			for(int i = 0; i < m_ArrayOneRingVertices.size(); i++)
			{
				CVertex3d* pVertex = pMesh3D->GetVertex(m_ArrayOneRingVertices[i]);
				
				if(pVertex->m_Landmarknumber == pVertex_current->m_Landmarknumber)
				{
					equation[pVertex->m_interior_vertices_index] = -omegaarray[i] * (pMesh3D->GetVertex(startindex)->u - pMesh3D->GetVertex(endindex)->u);
					b1 += omegaarray[i] * pMesh3D->GetVertex(endindex)->u;
				}else
				{
					b1 += omegaarray[i] * pVertex->u;
				}
			}
			equation[v0_index] = (pMesh3D->GetVertex(startindex)->u - pMesh3D->GetVertex(endindex)->u);
			b1 = b1 - pMesh3D->GetVertex(endindex)->u;
			A.push_back(equation);
			b.push_back(b1);
		}
	p_FormView3->SolveLinearSystem(A,b,uvarray);

	//Set the uv of interior vertices
	for(int i =0; i < pMesh3D->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh3D->GetVertex(i);
		if(pVertex->m_interior_vertices_index >= 0)
		{
			if(pVertex->m_Landmarknumber >= 0)
			{
				GeoMapLandMark* pLandmark = pMesh3D->m_LandMarkArray[pVertex->m_Landmarknumber];
				int startindex = pLandmark->landmark_points[0];
				int endindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
				double u1 = pMesh3D->GetVertex(startindex)->u;
				double v1 = pMesh3D->GetVertex(startindex)->v;
				double u2 = pMesh3D->GetVertex(endindex)->u;
				double v2 = pMesh3D->GetVertex(endindex)->v;
				double a = uvarray[pVertex->m_interior_vertices_index];
				pVertex->u = u1 * a + u2 * (1 - a);
				pVertex->v = v1 * a + v2 * (1 - a);
			}else
			{
				pVertex->u = uvarray[pVertex->m_interior_vertices_index];
				pVertex->v = uvarray[pVertex->m_interior_vertices_index + 1];
			}
		}
	}

	//save
	CString s1;
	CString path = pMesh3D->filename;
	path = path.Left(path.ReverseFind('.'));
	path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	if(IsUniform)
		s1 = "P:\\Dr_YYJ\\data\\" + path + "_ttuc_uniform.txt"; 
	else
		s1 = "P:\\Dr_YYJ\\data\\" + path + "_ttuc_weighted.txt"; 

	std::ofstream file(s1);

	//Generate a New Mesh
	CMesh3d* pNewMesh = new CMesh3d();
	pNewMesh->Copy(pMesh3D);
	for(int i = 0; i < pNewMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pNewMesh->GetVertex(i);
		pVertex->x(pMesh3D->GetVertex(i)->u);
		pVertex->y(pMesh3D->GetVertex(i)->v);
		pVertex->z(0);
	}
	//pNewMesh->SetTransform(*pMesh3D->GetTransform());
	Point3D lower,upper;
	//pMesh3D->ComputeBoundingBox(lower,upper);
	//double length = upper.x - lower.x;
	//if(upper.y - lower.y > length)
	//	length = upper.y - lower.y;
	//if(upper.z - lower.z > length)
	//	length = upper.z - lower.z;
	double length = 1;
	std::vector<int> m_PointIndex;
	//m_PointIndex.resize(pMesh3D->NbVertex(),0);
	//first save the boundary
	/*for(int i = 0; i < m_BoundaryVertices.size(); i++)
	{
		CVertex3d* pOriginalVertex = pMesh3D->GetVertex(m_BoundaryVertices[i].first);
		CVertex3d* pVertex = new CVertex3d();
		pVertex->Set(pOriginalVertex->u * length,pOriginalVertex->v * length,0);
		pNewMesh->AddVertex(pVertex);
		m_PointIndex[m_BoundaryVertices[i].first] = pNewMesh->NbVertex() - 1;
		//file << "Vertex " << pOriginalVertex->m_OriginalIndex << " " << pOriginalVertex->u  << " " << pOriginalVertex->v << " 0 " <<"\n";
	}*/
	/*for(int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[i].size() == 1)
		{
			CVertex3d* pOriginalVertex = pMesh3D->GetVertex(i);
			CVertex3d* pVertex = new CVertex3d();
			pVertex->Set(pOriginalVertex->u * length,pOriginalVertex->v * length,0);
			pNewMesh->AddVertex(pVertex);
			m_PointIndex[i] = pNewMesh->NbVertex() - 1;
			file << "Vertex " << pOriginalVertex->m_OriginalIndex << " " << pOriginalVertex->u  << " " << pOriginalVertex->v << " 0 " <<"\n";
		}
	}*/

	//then interior vertices
	/*for(int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
	{
		if(m_ArrayPointConnectedSelectEdgeIndex[i].size() >= 3)
		{
			CVertex3d* pOriginalVertex = pMesh3D->GetVertex(i);
			CVertex3d* pVertex = new CVertex3d();
			pVertex->Set(pOriginalVertex->u * length,pOriginalVertex->v * length,0);
			pNewMesh->AddVertex(pVertex);
			m_PointIndex[i] = pNewMesh->NbVertex() - 1;
			file << "Vertex " << pOriginalVertex->m_OriginalIndex << " " << pOriginalVertex->u  << " " << pOriginalVertex->v << " 0 " <<"\n";
		}
	}*/
	/*std::pair<int,std::pair<double,double>> pair;
	if(m_InteriorPointsArray.size() == 0)
	{
	
		for(int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
		{
			if(m_ArrayPointConnectedSelectEdgeIndex[i].size() >= 3)
			{
				CVertex3d* pOriginalVertex = pMesh3D->GetVertex(i);
				pair.first = i;
				double angle = atan(pOriginalVertex->v / pOriginalVertex->u);
				if(pOriginalVertex->u< 0 && pOriginalVertex->v > 0)
				{
					angle = 3.1415926 - angle;
				}else
					if(pOriginalVertex->u< 0 && pOriginalVertex->v < 0)
					{
						angle = 3.1415926 + angle;
					}else
						if(pOriginalVertex->u > 0 && pOriginalVertex->v < 0)
						{
							angle = 3.1415926 * 2 + angle;
						}
						double length = sqrt(SQR(pOriginalVertex->u) + SQR(pOriginalVertex->v));
						pair.second.first = angle;
						pair.second.second = length;
						m_InteriorPointsArray.push_back(pair);
			}
		}


		struct sort_pred {
			bool operator()(const std::pair<int,std::pair<double,double>> &left, const std::pair<int,std::pair<double,double>> &right) 
			{
				if(fabs(left.second.first - right.second.first) < 1e-8)
					return left.second.second < right.second.second;
				else
					return left.second.first < right.second.first;
			}
		};
		std::sort(m_InteriorPointsArray.begin(),m_InteriorPointsArray.end(),sort_pred());
	}

	for(int i = 0; i < m_InteriorPointsArray.size(); i++)
	{
		pair = m_InteriorPointsArray[i];
		CVertex3d* pOriginalVertex = pMesh3D->GetVertex(pair.first);
		CVertex3d* pVertex = new CVertex3d();
		pVertex->Set(pOriginalVertex->u * length,pOriginalVertex->v * length,0);
		pNewMesh->AddVertex(pVertex);
		m_PointIndex[pair.first] = pNewMesh->NbVertex() - 1;
		file << "Vertex " << pOriginalVertex->m_OriginalIndex << " " << pOriginalVertex->u  << " " << pOriginalVertex->v << " 0 " <<"\n";
	}
	file.close();
	pNewMesh->ComputeBoundingBox(lower,upper);

	CVector3d m_lower,m_upper;
	m_lower.Set(lower.x,lower.y,lower.z);
	m_upper.Set(upper.x,upper.y,upper.z);
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation = m_lower + m_upper;
	m_translation /= -2;
	length=m_upper.x()-m_lower.x();
	//float length1 = m_upper.y() - m_lower.y();
	if(length<m_upper.y()-m_lower.y())
		length=m_upper.y()-m_lower.y();
	if(length<m_upper.z()-m_lower.z())
		length=m_upper.z()-m_lower.z();

	CTransform transform;
	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1/length,1/length,1/length);
	transform.SetScale(&m_Scale);
	pNewMesh->SetTransform(transform);
	std::pair<int,int> pair1;
	for(int i = 0; i < pMesh3D->m_LandMarkArray.size(); i++)
	{
		int startindex = pMesh3D->m_LandMarkArray[i]->landmark_points[0];
		int endindex = pMesh3D->m_LandMarkArray[i]->landmark_points[pMesh3D->m_LandMarkArray[i]->landmark_points.size() - 1];

		pair1.first = m_PointIndex[startindex];
		pair1.second = m_PointIndex[endindex];
		pNewMesh->m_ArraySharpEdges.push_back(pair1);
	}
	for(int i = 0; i < m_BoundaryVertices.size(); i++)
	{
		pair1.first = m_PointIndex[m_BoundaryVertices[i].first];
		pair1.second = m_PointIndex[m_BoundaryVertices[(i + 1) % m_BoundaryVertices.size()].first];
		pNewMesh->m_ArraySharpEdges.push_back(pair1);
	}*/
	pDoc->m_SceneGraph.Add(pNewMesh);
	pNewMesh->m_showSharpEdge = TRUE;
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
double Action_PolyOmino::ComputeEdgeLength(CMesh3d* pMesh3D,std::vector<int> &landmarkset)
{
	double sum = 0;
	int size = landmarkset.size();
	for(int i = 0; i < size - 1; i++)
	{
		int index1 = landmarkset[i];
		int index2 = landmarkset[i + 1];
		CVertex3d* pVertex1 = pMesh3D->GetVertex(index1);
		CVertex3d* pVertex2 = pMesh3D->GetVertex(index2);

		double length = SQR(pVertex1->x() - pVertex2->x()) + SQR(pVertex1->y() - pVertex2->y()) + SQR(pVertex1->z() - pVertex2->z());
		length = sqrt(length);
		sum += length;	
	}
	return sum;
}
void Action_PolyOmino::LoadData(CString FileName,int &iterationnum)
{
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
	pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	pUniformGridMesh->m_ArraySharpEdges.clear();
	int num;
	std::ifstream file(DebugInformation);
	int facenum;

	file >> facenum;
	file >> m_initialfacenum;

	//Array seleted edges
	file >> num;
	pUniformGridMesh->m_ArraySharpEdges.resize(num);
	std::pair<int,int> pair;
	for(int i = 0; i < num; i++)
	{
		file >> pair.first >> pair.second;
		pUniformGridMesh->m_ArraySharpEdges[i] = pair;
	}

	//Horizontal edges
	int num1;
	file >> num;
	m_HorizontalSegments.resize(num);
	for(int i = 0; i < num; i++)
	{
		file >> num1;
		m_HorizontalSegments[i].resize(num1,0);
		for(int j = 0; j < num1; j++)
			file >> m_HorizontalSegments[i][j];
	}

	file >> num;
	m_VerticalSegments.resize(num);
	for(int i = 0; i < num; i++)
	{
		file >> num1;
		m_VerticalSegments[i].resize(num1,0);
		for(int j = 0; j < num1; j++)
			file >> m_VerticalSegments[i][j];
	}

	//constraints
	file >> num;
	for(int i = 0; i < num; i++)
	{
		std::pair<int,int> pair;
		file >> pair.first >> pair.second;
		pUniformGridMesh->m_ArrayMergeConstraints.push_back(pair);
	}
	file.close();

}
void Action_PolyOmino::LoadData1(CString FileName,int &iterationnum)
{
	CString s1;
	CString DebugInformation = FileName;
	DebugInformation = DebugInformation.Left(DebugInformation.ReverseFind('.'));

	CString filename1 = DebugInformation.Right(DebugInformation.GetLength() - DebugInformation.ReverseFind('\\') - 1);
	iterationnum = atoi(filename1);
	DebugInformation = DebugInformation + ".txt";
	//Mesh
	pDoc->OnOpenDocument(FileName);
	pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	pUniformGridMesh->m_ArraySharpEdges.clear();
	int num;
	std::ifstream file(DebugInformation);
	int facenum;

	file >> facenum;
	file >> m_initialfacenum;

	//Array seleted edges
	file >> num;
	pUniformGridMesh->m_ArraySharpEdges.resize(num);
	std::pair<int,int> pair;
	for(int i = 0; i < num; i++)
	{
		file >> pair.first >> pair.second;
		pUniformGridMesh->m_ArraySharpEdges[i] = pair;
	}

	//Horizontal edges
	int num1;
	file >> num;
	m_HorizontalSegments.resize(num);
	for(int i = 0; i < num; i++)
	{
		file >> num1;
		m_HorizontalSegments[i].resize(num1,0);
		for(int j = 0; j < num1; j++)
			file >> m_HorizontalSegments[i][j];
	}

	file >> num;
	m_VerticalSegments.resize(num);
	for(int i = 0; i < num; i++)
	{
		file >> num1;
		m_VerticalSegments[i].resize(num1,0);
		for(int j = 0; j < num1; j++)
			file >> m_VerticalSegments[i][j];
	}

	//constraints
	file >> num;
	for(int i = 0; i < num; i++)
	{
		std::pair<int,int> pair;
		file >> pair.first >> pair.second;
		pUniformGridMesh->m_ArrayMergeConstraints.push_back(pair);
	}
	file.close();

}
void Action_PolyOmino::OnBnClickedOminoLoaddebuginformation()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}else
		return;

	int iterationnum;
	pMesh = NULL;
	LoadData(FileName,iterationnum);

	char filename[200];

	while(1)
	{
		CheckandFixTopologyValidation(iterationnum);

	/*if(iterationnum == 752)
	{
		CMesh3d* pTestMesh1 = new CMesh3d(*pUniformGridMesh);
		pDoc->m_SceneGraph.Add(pTestMesh1);
		CMesh3d* pTestMesh2 = new CMesh3d(*pUniformGridMesh);
		pDoc->m_SceneGraph.Add(pTestMesh2);

		std::pair<int,int> edge1,edge2;
		//pTestMesh1->m_ArraySeletedVertexIndex.push_back(585);
		//pTestMesh1->m_ArraySeletedVertexIndex.push_back(1324);
		edge1.first = 389;
		edge1.second = 390;
		edge2.first = 390;
		edge2.second = 391;
		//edge1 = pUniformGridMesh->m_ArraySharpEdges[764];
		//edge2 = pUniformGridMesh->m_ArraySharpEdges[1534];
		pTestMesh1->m_ArraySharpEdges.clear();
		pTestMesh1->m_ArraySharpEdges.push_back(edge1);
		pTestMesh2->m_ArraySharpEdges.clear();
		pTestMesh2->m_ArraySharpEdges.push_back(edge2);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
		return;
	}//*/

		if(pUniformGridMesh->m_ArraySharpEdges.size() <= 4)
			break;

		//pUniformGridMesh->SaveMFile("D:\\test1.m");
		IsValidMesh();

		sprintf(filename,"D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\%d.m",iterationnum);
		pUniformGridMesh->SaveMFile(filename);
		//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
		sprintf(filename,"D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\%d.txt",iterationnum);
		SaveDebugInformation(filename);
		
		//int currentfacenum = ComputeFaceNum();
		//if(currentfacenum < m_initialfacenum)
		//	break;
		iterationnum++;

		int flag = UniformSimplificationOneIteration();
		if(!flag)
			break;

		sprintf(filename,"D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\%d.m",iterationnum);
		pUniformGridMesh->SaveMFile(filename);
		//sprintf(filename,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\%d.txt",num);
		//sprintf(filename,"D:\\papers\\code\\data\\Geo\\dense\\%d.txt",iterationnum);
		//SaveDebugInformation(filename);
	}

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoAddvertexselection()
{
	// TODO: Add your control notification handler code here
	if(!pMesh)
	{
		AfxMessageBox("Please first set current mesh");
		return;
	}
	DialogInputInformation dlg(this);
	if(dlg.DoModal() == IDOK)
	{
		pMesh->m_ArraySharpVertexIndex.insert(pMesh->m_ArraySharpVertexIndex.begin(),dlg.m_ArraySelectedVertex.begin(),dlg.m_ArraySelectedVertex.end());
	}
}
void Action_PolyOmino::SaveVector(double *vec,int num,char* filename)
{
	std::ofstream file(filename);
	for(int i = 0; i < num; i++)
	{
		if(i == num - 1)
			file << vec[i];
		else
			file << vec[i] << " ";
	}
	file.close();
}
void Action_PolyOmino::SaveVector(std::vector<double> vec,int num,char* filename)
{
	std::ofstream file(filename);
	for(int i = 0; i < num; i++)
	{
		if(i == num - 1)
			file << vec[i];
		else
			file << vec[i] << " ";
	}
	file.close();
}
void Action_PolyOmino::SaveVector(std::vector<int> vec, int num, char* filename)
{
	std::ofstream file(filename);
	for (int i = 0; i < num; i++)
	{
		if (i == num - 1)
			file << vec[i];
		else
			file << vec[i] << " ";
	}
	file.close();
}
void Action_PolyOmino::SaveMatrix(MySparseMatrix* ma,char* filename)
{
	int m = ma->m;
	int n = ma->n;
	std::ofstream file(filename);
	int maxi = -1,maxj = -1;
	std::map<std::pair<int,int>,double>::iterator it;
	for(it = ma->m_Array.begin(); it != ma->m_Array.end(); ++it)
	{
		if(it->first.first > maxi)
			maxi = it->first.first;
		if(it->first.second > maxj)
			maxj = it->first.second;
		file << it->first.first + 1 << " " << it->first.second + 1<< " "<< it->second << "\n";
	}
	/*for(int i = 0; i < ma->m_Array.size(); i++)
	{
		if(ma->m_Array[i].i > maxi)
			maxi = ma->m_Array[i].i;
		if(ma->m_Array[i].j > maxj)
			maxj = ma->m_Array[i].j;
		file << ma->m_Array[i].i + 1 << " " << ma->m_Array[i].j + 1<< " "<< ma->m_Array[i].val << "\n";
	}*/
	if(maxi < m - 1 || maxj < n - 1)
	{
		file << m << " " << n <<" "<< 0;
	}
	file.close();
}
void Action_PolyOmino::SaveMatrix(std::vector<std::vector<double>> &vec,int m,int n,char* filename)
{
	std::ofstream file(filename);
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(j == n - 1)
				file << vec[i][j];
			else
				file << vec[i][j] << " ";
		}
		file <<"\n";

	}
	file.close();
}
void Action_PolyOmino::SaveMatrix(std::vector<std::vector<double>> &vec, int m1, int m2, int n, char* filename)
{
	std::ofstream file(filename);
	for (int i = m1; i < m2; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (j == n - 1)
				file << vec[i][j];
			else
				file << vec[i][j] << " ";
		}
		file << "\n";

	}
	file.close();
}
void Action_PolyOmino::SaveMatrix(double **vec,int m,int n,char* filename)
{
	std::ofstream file(filename);
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(j == n - 1)
				file << vec[i][j];
			else
				file << vec[i][j] << " ";
		}
		file <<"\n";

	}
	file.close();
}
void Action_PolyOmino::SaveMatrix(int **vec,int m,int n,char* filename)
{
	std::ofstream file(filename);
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(j == n - 1)
				file << vec[i][j];
			else
				file << vec[i][j] << " ";
		}
		file <<"\n";

	}
	file.close();
}

void Action_PolyOmino::SolveSpareseLinearSystem_matlab_quadprog(MySparseMatrix *A, std::vector<double> &B, MySparseMatrix *Acon, std::vector<double> &Bcon, std::vector<double> &X)
{
	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		AfxMessageBox("can't start MATLAB engine!");
		exit(1);
	}
	int m = A->m;
	int n = A->n;
	char message[1000];
	engOutputBuffer(ep, message, 1000);
	engEvalString(ep, "cd('C:\\Users\\Administrator\\Documents\\MATLAB')");
	engEvalString(ep, "global T");
	pPolyOmino->SaveMatrix(A, "C:\\Users\\Administrator\\Documents\\MATLAB\\A.txt");
	engEvalString(ep, "T=load('C:\\Users\\Administrator\\Documents\\MATLAB\\A.txt');");

	engEvalString(ep, "global Tcon");
	pPolyOmino->SaveMatrix(Acon, "C:\\Users\\Administrator\\Documents\\MATLAB\\Acon.txt");
	engEvalString(ep, "Tcon=load('C:\\Users\\Administrator\\Documents\\MATLAB\\Acon.txt');");
	engEvalString(ep, "global Acon");
	engEvalString(ep, "Acon = spconvert(Tcon);");

	engEvalString(ep, "global A");
	engEvalString(ep, "A = spconvert(T);");
	engEvalString(ep, "global H");
	engEvalString(ep, "H = A'*A");

	engEvalString(ep, "global b");
	pPolyOmino->SaveVector(B, m, "C:\\Users\\Administrator\\Documents\\MATLAB\\b.txt");
	engEvalString(ep, "b=dlmread('C:\\Users\\Administrator\\Documents\\MATLAB\\b.txt');");

	engEvalString(ep, "global bcon");
	pPolyOmino->SaveVector(Bcon, Bcon.size(), "C:\\Users\\Administrator\\Documents\\MATLAB\\bcon.txt");
	engEvalString(ep, "bcon=dlmread('C:\\Users\\Administrator\\Documents\\MATLAB\\bcon.txt');");

	engEvalString(ep, "global f");
	engEvalString(ep, "f=-b*A");
	engEvalString(ep, "global xx1");
	engEvalString(ep, "xx1=quadprog(H,f,Acon,bcon);");
	TRACE("%s", message);
	mxArray *xx = engGetVariable(ep, "xx1");
	double *xval = mxGetPr(xx);
	X.resize(m);
	for (int i = 0; i < m; i++)
		X[i] = xval[i];
}
void Action_PolyOmino::SolveSVMClassification_matlab(std::vector<std::vector<double>> data, std::vector<int> label)
{
	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		AfxMessageBox("can't start MATLAB engine!");
		exit(1);
	}
	engEvalString(ep, "cd('C:\\matlab701\\work')");
	int m = data.size();
	int n = data[0].size();
	int traindatanum = m * 0.9;
	engEvalString(ep, "global TrainData");
	pPolyOmino->SaveMatrix(data, traindatanum, n, "C:\\matlab701\\work\\A.txt");
	engEvalString(ep, "TrainData=load('C:\\matlab701\\work\\A.txt');");

	engEvalString(ep, "global group");
	pPolyOmino->SaveVector(label, traindatanum, "C:\\matlab701\\work\\b.txt");
	engEvalString(ep, "group=dlmread('C:\\matlab701\\work\\b.txt');");

	engEvalString(ep, "global TestData");
	pPolyOmino->SaveMatrix(data, traindatanum, m, n, "C:\\matlab701\\work\\A1.txt");
	engEvalString(ep, "TestData=load('C:\\matlab701\\work\\A1.txt');");

	engEvalString(ep, "global Group1");
	engEvalString(ep, "SVMStruct = svmtrain(TrainData, group);");
	engEvalString(ep, "Group1 = svmclassify(SVMStruct, TestData);");

	mxArray *xx = engGetVariable(ep, "Group1");
	double *xval = mxGetPr(xx);
	std::vector<int> result;
	result.resize(m - traindatanum);
	for (int i = 0; i < m - traindatanum; i++)
		result[i] = (int)xval[i];

	int correctnum = 0;
	for (int i = 0; i < m - traindatanum; i++)
	{
		if (result[i] == label[traindatanum + i])
			correctnum++;
	}

	double correntpercentage = (double)correctnum / (m - traindatanum);
}
void Action_PolyOmino::SolveSpareseLinearSystem_matlab(MySparseMatrix *A, std::vector<double> &B,std::vector<double> &X)
{
	Engine *ep;     
	if(!(ep=engOpen(NULL))) 
	{
		AfxMessageBox("can't start MATLAB engine!");
		exit(1);
	}
	int m = A->m;
	int n = A->n;
	char message[1000];
	engOutputBuffer(ep, message, 1000);
	//engEvalString(ep,"cd('C:\\Users\\Administrator\\Documents\\MATLAB')");
	//engEvalString(ep, "cd('C:\\matlab701\\work')");
	engEvalString(ep, "cd('C:\\matlab701\\work')");
	engEvalString(ep,"global T");
	//pPolyOmino->SaveMatrix(A,"C:\\matlab701\\work\\A.txt");
	pPolyOmino->SaveMatrix(A, "C:\\matlab701\\work\\A.txt");
	engEvalString(ep,"T=load('C:\\matlab701\\work\\A.txt');");

	engEvalString(ep,"global A");
	engEvalString(ep,"A = spconvert(T);");

	engEvalString(ep,"global bb");
	pPolyOmino->SaveVector(B,m,"C:\\matlab701\\work\\b.txt");
	engEvalString(ep,"bb=dlmread('C:\\matlab701\\work\\b.txt');");

	engEvalString(ep,"global xx1");
	engEvalString(ep,"xx1=A\\bb';");
	TRACE("%s",message);
	mxArray *xx = engGetVariable(ep,"xx1");
	double *xval = mxGetPr(xx);
	X.resize(m);
	for(int i = 0; i < m; i++)
		X[i] = xval[i];
	//delete xval;
	//engClose(ep);
}
void Action_PolyOmino::SolveLinearSystem_matlab(std::vector<std::vector<double>> &A, std::vector<double> &B,std::vector<double> &X)
{
	Engine *ep;     
	if(!(ep=engOpen(NULL))) 
	{
		AfxMessageBox("can't start MATLAB engine!");
		exit(1);
	}
	int m = A.size();
	int n = A[0].size();
	engEvalString(ep,"global A");
	pPolyOmino->SaveMatrix(A,m,n,"C:\\Users\\Administrator\\Documents\\MATLAB\\A.txt");
	engEvalString(ep,"A=dlmread('A.txt');");

	engEvalString(ep,"global b");
	pPolyOmino->SaveVector(B,m,"C:\\Users\\Administrator\\Documents\\MATLAB\\b.txt");
	engEvalString(ep,"b=dlmread('b.txt');");

	engEvalString(ep,"cd('C:\\Users\\Administrator\\Documents\\MATLAB')");
	char message[1000];
	engOutputBuffer(ep, message, 1000);
	engEvalString(ep,"global xx1");
	engEvalString(ep,"xx1=linsolve(A,b');");
	TRACE("%s",message);
	mxArray *xx = engGetVariable(ep,"xx1");
	double *xval = mxGetPr(xx);
	X.resize(m);
	for(int i = 0; i < m; i++)
		X[i] = xval[i];
	engClose(ep);
}
void Action_PolyOmino::OnBnClickedPolyominoAddedgeselection()
{
	// TODO: Add your control notification handler code here
	if(!pMesh)
	{
		AfxMessageBox("Please first set current mesh");
		return;
	}
	DialogInputInformation dlg(this);
	if(dlg.DoModal() == IDOK)
	{
		pMesh->m_ArraySelectedSharpEdgeIndex.insert(pMesh->m_ArraySelectedSharpEdgeIndex.begin(),dlg.m_ArraySelectedEdge.begin(),dlg.m_ArraySelectedEdge.end());
	}
}
int m_constrainediterationnum;
void ComputeDataGradient(const real_1d_array &x, double &func,  real_1d_array &grad, void *ptr)
{
	m_constrainediterationnum++;
	//int size = pPolyOmino->xintervelnum + pPolyOmino->yintervalnum;
	std::vector<double> xinterval;
	std::vector<double> yinterval;
	
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum; i++)
	{
		xinterval.push_back(x[i]);
	}
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->yintervalnum; i++)
	{
		yinterval.push_back(x[pPolyOmino->pUniformGridMesh->xintervelnum + i]);
	}
	/*for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1; i++)
	{
		if(xinterval[i] > xinterval[i+1])
			ASSERT(0);

	}
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
	{
		if(yinterval[i] > yinterval[i + 1])
			ASSERT(0);
	}*/
	func = 0;
	std::vector<double> areaarray;
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->m_ArrayFaceAreaArray.size(); i++)
	{
		std::vector<int> pointset = pPolyOmino->pUniformGridMesh->m_ArrayFaceAreaArray[i];
		int size = pointset.size() / 4;
		double area = 0;
		for(int j = 0; j < size; j++)
		{
			int index1 = pointset[4 * j];
			int index2 = pointset[4 * j + 1];
			int index3 = pointset[4 * j + 2];
			int index4 = pointset[4 * j + 3];

			area += (xinterval[index2] - xinterval[index1]) * (yinterval[index4] - yinterval[index3]);
		}
		areaarray.push_back(area);
		func += (area - pPolyOmino->pUniformGridMesh->m_ArrayTargetFaceArea[i]) * (area - pPolyOmino->pUniformGridMesh->m_ArrayTargetFaceArea[i]);
	}
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum; i++)
		grad[i] = pPolyOmino->ComputeXGradient(xinterval,yinterval,i);
	grad[0] = 0;
	grad[pPolyOmino->pUniformGridMesh->xintervelnum - 1] = 0;
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->yintervalnum; i++)
		grad[i + pPolyOmino->pUniformGridMesh->xintervelnum] = pPolyOmino->ComputeYGradient(xinterval,yinterval,i);
	grad[pPolyOmino->pUniformGridMesh->xintervelnum] = 0;
	grad[pPolyOmino->pUniformGridMesh->yintervalnum + pPolyOmino->pUniformGridMesh->xintervelnum - 1] = 0;

	//
	/*if(m_constrainediterationnum <= 50)
	{
		CMesh3d* pNewMesh1 = new CMesh3d();
		pNewMesh1->Copy(pPolyOmino->pUniformGridMesh);
		for(int i = 0; i < pNewMesh1->NbVertex(); i++)
		{
			CVertex3d* pVertex = pNewMesh1->GetVertex(i);
			double x = pVertex->x();
			double y = pVertex->y();
			int xindex = std::find(pPolyOmino->pUniformGridMesh->xinterval.begin(),pPolyOmino->pUniformGridMesh->xinterval.end(),x) - pPolyOmino->pUniformGridMesh->xinterval.begin();
			int yindex = std::find(pPolyOmino->pUniformGridMesh->yinterval.begin(),pPolyOmino->pUniformGridMesh->yinterval.end(),y) - pPolyOmino->pUniformGridMesh->yinterval.begin();
			x = xinterval[xindex];
			y = yinterval[yindex];
			pVertex->Set(x,y,0);
		}
		pDoc->m_SceneGraph.Add(pNewMesh1);
	}*/
}
void UnconstrainedOptimization(real_1d_array &xarray)
{
	double epsg = 0.0000000001;
	double epsf = 0;
	double epsx = 0;
	ae_int_t maxits = 0;
	minlbfgsstate state;
	minlbfgsreport rep;

	minlbfgscreate(1, xarray, state);
	minlbfgssetcond(state, epsg, epsf, epsx, maxits);
	alglib::minlbfgsoptimize(state, ComputeDataGradient);
	minlbfgsresults(state, xarray, rep);
}
void ConstrainedOptimization_matlab(real_1d_array &xarray)
{
	Engine *ep;     
	if(!(ep=engOpen(NULL))) 
	{
		AfxMessageBox("can't start MATLAB engine!");
		exit(1);
	}
	int xintervalsize = pPolyOmino->pUniformGridMesh->xinterval.size();
	int yintervalsize = pPolyOmino->pUniformGridMesh->yinterval.size();
	double* xinterval = new double[xintervalsize];
	double* yinterval = new double[yintervalsize];
	double* x0 = new double[xintervalsize + yintervalsize];
	for(int i = 0; i < xintervalsize + yintervalsize; i++)
		x0[i] = xarray[i];

	for(int i = 0; i < xintervalsize; i++)
	{
		xinterval[i] = pPolyOmino->pUniformGridMesh->xinterval[i];
	}
	for(int i = 0; i < yintervalsize; i++)
	{
		yinterval[i] = pPolyOmino->pUniformGridMesh->yinterval[i];
	}
	
	int facenum = pPolyOmino->pUniformGridMesh->m_ArrayFaceArea.size();
	int faceiternum = -1;
	for(int i = 0; i < facenum; i++)
	{
		int num = pPolyOmino->pUniformGridMesh->m_ArrayFaceAreaArray[i].size();
		if(num > faceiternum)
			faceiternum = num;
	}
	double** m_ArrayFaceAreaArray = new double*[facenum];
	for(int i = 0; i  < facenum; i++)
		m_ArrayFaceAreaArray[i] = new double[faceiternum];

	for(int i = 0; i < facenum; i++)
		for(int j = 0; j < faceiternum; j++)
		{
			m_ArrayFaceAreaArray[i][j] = -1;
		}
	for(int i = 0; i < facenum; i++)
	{
		int size = pPolyOmino->pUniformGridMesh->m_ArrayFaceAreaArray[i].size();
		for(int j = 0; j < size; j++)
		{
			m_ArrayFaceAreaArray[i][j] = pPolyOmino->pUniformGridMesh->m_ArrayFaceAreaArray[i][j];
		}
	}

	//targetfacearea
	double* m_ArrayFaceTargetArea = new double[facenum];
	for(int i = 0; i < facenum; i++)
	{
		m_ArrayFaceTargetArea[i] = pPolyOmino->pUniformGridMesh->m_ArrayTargetFaceArea[i];
	}

	int rows = pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1;
	int columns = pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum;
	double** A = new double*[rows];
	for(int i = 0; i < rows; i++)
		A[i] = new double[columns];
	//double** A = new double[rows][columns];
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
		{
			A[i][j] = 0;
		}

	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1; i++)
	{
		// i i+1
		A[i][i] = 1.0;
		A[i][i + 1] = -1.0;
	}
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
	{
		// i i+1
		A[pPolyOmino->pUniformGridMesh->xintervelnum - 1 + i][pPolyOmino->pUniformGridMesh->xintervelnum + i] = 1.0;
		A[pPolyOmino->pUniformGridMesh->xintervelnum - 1 + i][pPolyOmino->pUniformGridMesh->xintervelnum + i + 1] = -1.0;
	}
	double* b = new double[rows];
	memset(b,0,rows * sizeof(double));
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1; i++)
	{
		b[i] = -(xinterval[i + 1] - xinterval[i]) / 2;
	}
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
	{
		b[pPolyOmino->pUniformGridMesh->xintervelnum - 1 + i] = -(yinterval[i + 1] - yinterval[i]) / 2;
	}

	//xinterval 
	//mxArray *xinterval_mx = mxCreateDoubleMatrix(1,xintervalsize,mxREAL);
	//memcpy(mxGetPr(xinterval_mx),xinterval,xintervalsize * sizeof(double));
	//engPutVariable(ep,"xinterval",xinterval_mx);
	engEvalString(ep,"global xinterval");
	pPolyOmino->SaveVector(xinterval,xintervalsize,"C:\\Users\\Administrator\\Documents\\MATLAB\\xinterval.txt");
	engEvalString(ep,"xinterval=dlmread('xinterval.txt');");

	//yinterval
	//mxArray *yinterval_mx = mxCreateDoubleMatrix(1,yintervalsize,mxREAL);
	//memcpy(mxGetPr(yinterval_mx),yinterval,yintervalsize * sizeof(double));
	//engPutVariable(ep,"yinterval",yinterval_mx);
	engEvalString(ep,"global yinterval");
	pPolyOmino->SaveVector(yinterval,yintervalsize,"C:\\Users\\Administrator\\Documents\\MATLAB\\yinterval.txt");
	engEvalString(ep,"yinterval=dlmread('yinterval.txt');");

	//targetfacearea
	//mxArray *m_ArrayFaceTargetArea_mx = mxCreateDoubleMatrix(1,facenum,mxREAL);
	//memcpy(mxGetPr(m_ArrayFaceTargetArea_mx),m_ArrayFaceTargetArea,facenum * sizeof(double));
	//engPutVariable(ep,"facetargetarea",m_ArrayFaceTargetArea_mx);
	engEvalString(ep,"global facetargetarea");
	pPolyOmino->SaveVector(m_ArrayFaceTargetArea,facenum,"C:\\Users\\Administrator\\Documents\\MATLAB\\ArrayFaceTargetArea.txt");
	engEvalString(ep,"facetargetarea=dlmread('ArrayFaceTargetArea.txt');");

	//m_ArrayFaceAreaArray
	//mxArray *faceareaarray_mx = mxCreateDoubleMatrix(facenum,faceiternum,mxREAL);
	//double *faceareaarray_double = mxGetPr(faceareaarray_mx);
	//for(int i = 0; i < facenum; i++)
	//	for(int j = 0; j < faceiternum; j++)
	//		faceareaarray_double[i * faceiternum + j] = m_ArrayFaceAreaArray[i][j];
	//memcpy(mxGetPr(faceareaarray_mx),m_ArrayFaceAreaArray,facenum * faceiternum * sizeof(double));
	engEvalString(ep,"global faceareaarray");
	//engPutVariable(ep,"faceareaarray",faceareaarray_mx);
	//mxArray *sd_mx = engGetVariable(ep,"faceareaarray");
	//for(int ij = 0; ij < sd
	//double *sd_double = mxGetPr(sd_mx);
	pPolyOmino->SaveMatrix(m_ArrayFaceAreaArray,facenum,faceiternum,"C:\\Users\\Administrator\\Documents\\MATLAB\\ArrayFaceAreaArray.txt");
	engEvalString(ep,"faceareaarray=dlmread('ArrayFaceAreaArray.txt');");

	//xnum
	mxArray *xintervalsize_mx = mxCreateDoubleScalar(xintervalsize);
	engEvalString(ep,"global xnum");
	engPutVariable(ep,"xnum",xintervalsize_mx);

	//ynum
	mxArray *yintervalsize_mx = mxCreateDoubleScalar(yintervalsize);
	engEvalString(ep,"global ynum");
	engPutVariable(ep,"ynum",yintervalsize_mx);

	//x0
	mxArray *m_ArrayX0_mx = mxCreateDoubleMatrix(1,xintervalsize + yintervalsize,mxREAL);
	memcpy(mxGetPr(m_ArrayX0_mx),x0,(xintervalsize + yintervalsize) * sizeof(double));
	//engEvalString(ep,"global x0");
	engPutVariable(ep,"x0",m_ArrayX0_mx);
	pPolyOmino->SaveVector(x0,xintervalsize + yintervalsize,"C:\\Users\\Administrator\\Documents\\MATLAB\\x0.txt");

	//A
	//mxArray *A_mx = mxCreateDoubleMatrix(rows,columns,mxREAL);
	//memcpy(mxGetPr(A_mx),A,rows * columns * sizeof(double));
	//engPutVariable(ep,"A",A_mx);
	pPolyOmino->SaveMatrix(A,rows,columns,"C:\\Users\\Administrator\\Documents\\MATLAB\\A.txt");
	engEvalString(ep,"A=dlmread('A.txt');");

	//b
	mxArray *b_mx = mxCreateDoubleMatrix(1,rows,mxREAL);
	memcpy(mxGetPr(b_mx),b,(rows) * sizeof(double));
	engPutVariable(ep,"b",b_mx);
	pPolyOmino->SaveVector(b,rows,"C:\\Users\\Administrator\\Documents\\MATLAB\\b.txt");

	engEvalString(ep,"cd('C:\\Users\\Administrator\\Documents\\MATLAB')");
	char message[1000];
	engOutputBuffer(ep, message, 1000);
	engEvalString(ep,"[xx,fval]=fmincon(@ComputeAreadifference,x0,A,b)");
	TRACE("%s",message);
	mxArray *xx = engGetVariable(ep,"xx");
	double *xval = mxGetPr(xx);
	for(int i = 0; i < xarray.length(); i++)
		xarray[i] = xval[i];
	engClose(ep);
	delete xinterval;
	delete yinterval;
	delete x0;
	for(int i = 0; i  < facenum; i++)
		delete m_ArrayFaceAreaArray[i];
	delete m_ArrayFaceAreaArray;
	delete m_ArrayFaceTargetArea;
	for(int i = 0; i < rows; i++)
		delete A[i];
	delete A;
	delete b;
	//delete xval;
	mxDestroyArray(xx);
	//mxDestroyArray(xinterval_mx);
	//mxDestroyArray(yinterval_mx);
	//mxDestroyArray(m_ArrayFaceTargetArea_mx);
	//mxDestroyArray(faceareaarray_mx);
	mxDestroyArray(xintervalsize_mx);
	mxDestroyArray(yintervalsize_mx);
	mxDestroyArray(m_ArrayX0_mx);
	//mxDestroyArray(A_mx);
	mxDestroyArray(b_mx);				
}

void ConstrainedOptimization(real_1d_array &xarray)
{
	//real_1d_array x = "[5,5]";
	
	real_2d_array c;
	c.setlength(pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1,pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum + 1);

	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
		for(int j = 0; j < pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum + 1; j++)
			c[i][j] = 0;

	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
			c[i][pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum] = 0;

	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1; i++)
	{
		// i i+1
		c[i][i] = 1.0;
		c[i][i + 1] = -1.0;
	}
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
	{
		// i i+1
		c[pPolyOmino->pUniformGridMesh->xintervelnum - 1 + i][pPolyOmino->pUniformGridMesh->xintervelnum + i] = 1.0;
		c[pPolyOmino->pUniformGridMesh->xintervelnum - 1 + i][pPolyOmino->pUniformGridMesh->xintervelnum + i + 1] = -1.0;
	}

	integer_1d_array ct;
	ct.setlength(pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1);
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
		ct[i] = -1;
	minbleicstate state;
	minbleicreport rep;

	double epsg = 0.000001;
	double epsf = 0;
	double epsx = 0;
	ae_int_t maxits = 4000;

	minbleiccreate(xarray, state);
	minbleicsetlc(state, c, ct);
	minbleicsetcond(state, epsg, epsf, epsx, maxits);
	alglib::minbleicoptimize(state, ComputeDataGradient);
	minbleicresults(state, xarray, rep);
}

void ConstrainedOptimization_boundedconstraits(real_1d_array &xarray)
{
	//First we set the four bounded lines fixed and 
	//Move the internal vertical and horizontal lines between the middle of 
	//previous and after intervals.

	real_1d_array lowerbound,upperbound;
	lowerbound.setlength(xarray.length());
	upperbound.setlength(xarray.length());
	double minx,maxx,miny,maxy;
	minx = xarray[0];
	maxx = xarray[pPolyOmino->pUniformGridMesh->xintervelnum - 1];
	miny = xarray[pPolyOmino->pUniformGridMesh->xintervelnum];
	maxy = xarray[pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum - 1];
	lowerbound[0] = upperbound[0] = minx;
	lowerbound[pPolyOmino->pUniformGridMesh->xintervelnum - 1] = upperbound[pPolyOmino->pUniformGridMesh->xintervelnum - 1] = maxx;
	for(int i  = 1; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1; i++)
	{
		lowerbound[i] = (xarray[i - 1] + xarray[i]) / 2;
		upperbound[i] = (xarray[i] + xarray[i + 1]) / 2;;
	}
	lowerbound[pPolyOmino->pUniformGridMesh->xintervelnum] = upperbound[pPolyOmino->pUniformGridMesh->xintervelnum] = miny;
	lowerbound[pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum - 1] = upperbound[pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum - 1] = maxy;
	for(int i  = 1; i < pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
	{
		lowerbound[pPolyOmino->pUniformGridMesh->xintervelnum + i] = (xarray[pPolyOmino->pUniformGridMesh->xintervelnum + i - 1] + xarray[pPolyOmino->pUniformGridMesh->xintervelnum + i]) / 2;
		upperbound[pPolyOmino->pUniformGridMesh->xintervelnum + i] = (xarray[pPolyOmino->pUniformGridMesh->xintervelnum + i] + xarray[pPolyOmino->pUniformGridMesh->xintervelnum + i + 1]) / 2;
	}
	/*c.setlength((pPolyOmino->pUniformGridMesh->xintervelnum - 2 + pPolyOmino->pUniformGridMesh->yintervalnum - 2) * 2,pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum + 1 - 4);

	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
		for(int j = 0; j < pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum + 1; j++)
			c[i][j] = 0;

	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
		c[i][pPolyOmino->pUniformGridMesh->xintervelnum + pPolyOmino->pUniformGridMesh->yintervalnum] = 0;

	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1; i++)
	{
		// i i+1
		c[i][i] = 1.0;
		c[i][i + 1] = -1.0;
	}
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
	{
		// i i+1
		c[pPolyOmino->pUniformGridMesh->xintervelnum - 1 + i][pPolyOmino->pUniformGridMesh->xintervelnum + i] = 1.0;
		c[pPolyOmino->pUniformGridMesh->xintervelnum - 1 + i][pPolyOmino->pUniformGridMesh->xintervelnum + i + 1] = -1.0;
	}

	integer_1d_array ct;
	ct.setlength(pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1);
	for(int i = 0; i < pPolyOmino->pUniformGridMesh->xintervelnum - 1 + pPolyOmino->pUniformGridMesh->yintervalnum - 1; i++)
		ct[i] = -1;*/
	minbleicstate state;
	minbleicreport rep;

	double epsg = 0.000001;
	double epsf = 0;
	double epsx = 0;
	ae_int_t maxits = 4000;

	minbleiccreate(xarray, state);
	//minbleicsetlc(state, c, ct);
	minbleicsetbc(state,lowerbound,upperbound);
	minbleicsetcond(state, epsg, epsf, epsx, maxits);
	alglib::minbleicoptimize(state, ComputeDataGradient);
	minbleicresults(state, xarray, rep);
}
void Action_PolyOmino::OnBnClickedOminoDatadrivenmorph()
{
	// TODO: Add your control notification handler code here
	//First Load the data. the last m file with 49 states
	//CString filename = "D:\\papers\\code\\data\\Geo\\dense\\1682.m";
	//CString filename = "D:\\papers\\code\\data\\Geo\\dense\\1724.m";
	//CString filename = "D:\\papers\\code\\data\\Geo\\dense\\1800.m";
	CString filename = "D:\\papers\\code\\data\\Geo\\dense\\1788.m";

	int iterationnum;
	//LoadData(filename,iterationnum);
	LoadData1(filename,iterationnum);
	UpdatePointConnectedSelectedEdgeIndex();

	//Build the struture for the area
	std::vector<int> m_ArrayFacePoints;
	std::vector<int> m_ArrayFaceEdges;
	//std::vector<std::vector<int>> m_ArrayFacePointsArray;
	//std::vector<std::vector<int>> m_ArrayFaceEdgesArray;
	TraceFaces(m_ArrayFacePoints,m_ArrayFaceEdges);

	//Remove the outer face
	RemoveOuterFace(pUniformGridMesh->m_ArrayFacePointsArray,pUniformGridMesh->m_ArrayFaceEdgesArray);

	pUniformGridMesh->xinterval.clear();
	pUniformGridMesh->yinterval.clear();
	std::pair<int,int> edgepair;
	double x,y;
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		int edgeindex = m_HorizontalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
		y = pVertex->y();
		pUniformGridMesh->yinterval.push_back(y);
	}
	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		int edgeindex = m_VerticalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
		x = pVertex->x();
		pUniformGridMesh->xinterval.push_back(x);
	}

	//
	ComputeFacesFourElementArray(m_ArrayFacePoints,m_ArrayFaceEdges);

	pUniformGridMesh->m_currentfaceindex = 0;
	pUniformGridMesh->m_ArraySharpVertexIndex = pUniformGridMesh->m_ArrayFacePointsArray[pUniformGridMesh->m_currentfaceindex];

	//ComputeFaceArea(pUniformGridMesh->xinterval,pUniformGridMesh->yinterval,pUniformGridMesh->m_ArrayFaceArea);

	pUniformGridMesh->m_ArrayTargetFaceArea.resize(pUniformGridMesh->m_ArrayFacePointsArray.size());

	LoadTargetFaceArea("D:\\papers\\code\\data\\Geo\\dense\\1788_usapopulation.txt");
	//load target face area
	///*CString faceareafilename = "D:\\papers\\code\\data\\Geo\\dense\\usapopulation2010new.txt";
	//LoadFaceArea(faceareafilename);

	ComputeFaceArea(pUniformGridMesh->xinterval,pUniformGridMesh->yinterval,pUniformGridMesh->m_ArrayFaceArea);

	double targetarea = 0;
	for(int i = 0; i < pUniformGridMesh->m_ArrayTargetFaceArea.size(); i++)
	{
		targetarea += pUniformGridMesh->m_ArrayTargetFaceArea[i];
	}
	double totalfacearea = 0;
	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceArea.size(); i++)
	{
		totalfacearea += pUniformGridMesh->m_ArrayFaceArea[i];
	}
	for(int i = 0; i < pUniformGridMesh->m_ArrayTargetFaceArea.size(); i++)
	{
		pUniformGridMesh->m_ArrayTargetFaceArea[i] = pUniformGridMesh->m_ArrayTargetFaceArea[i] * totalfacearea/ targetarea;
	}

	pUniformGridMesh->m_ArrayDifferenceFaceArea.resize(pUniformGridMesh->m_ArrayFaceArea.size());
	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceArea.size(); i++)
	{
		pUniformGridMesh->m_ArrayDifferenceFaceArea[i] = fabs(pUniformGridMesh->m_ArrayFaceArea[i] - pUniformGridMesh->m_ArrayTargetFaceArea[i]) / pUniformGridMesh->m_ArrayTargetFaceArea[i];
	}

	//first compute the largets and smallest ratio betwween targetarea/currentarea
	double maxratio = -1;
	double minratio = 1;
	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceArea.size(); i++)
	{
		double ratio = pUniformGridMesh->m_ArrayTargetFaceArea[i] / pUniformGridMesh->m_ArrayFaceArea[i];
		if(ratio > maxratio)
			maxratio = ratio;
		if(ratio < minratio)
			minratio = ratio;
	}
	TRACE("maxratio: %f minratio: %f\n",maxratio,minratio);

	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceArea.size(); i++)
	{
		double ratio = pUniformGridMesh->m_ArrayTargetFaceArea[i] / pUniformGridMesh->m_ArrayFaceArea[i];
		if(ratio > MAX_RATIO)
			pUniformGridMesh->m_ArrayTargetFaceArea[i] = pUniformGridMesh->m_ArrayFaceArea[i] * MAX_RATIO;
		if(ratio < MIN_RATIO)
			pUniformGridMesh->m_ArrayTargetFaceArea[i] = pUniformGridMesh->m_ArrayFaceArea[i] * MIN_RATIO;
	}

	//St. Louis 
	//pUniformGridMesh->m_ArrayTargetFaceArea[19] = pUniformGridMesh->m_ArrayFaceArea[19];
	//
	//
	/*std::vector<double> areaarray1;
	ComputeFaceArea(xinterval,yinterval,areaarray1);

	//set the target area
	m_ArrayTargetFaceArea = areaarray1;
	m_ArrayTargetFaceArea[1] = m_ArrayTargetFaceArea[1] * 2;

	//optimize
	real_1d_array xarray;
	xintervelnum = xinterval.size();
	yintervalnum = yinterval.size();
	int size = xintervelnum + yintervalnum;
	xarray.setlength( size);
	for(int i = 0; i < xintervelnum; i++)
	{
		xarray[i] = xinterval[i];
	}
	for(int i = 0; i < yintervalnum; i++)
	{
		xarray[i + xintervelnum] = yinterval[i];
	}

	TRACE("---------------------------------------------------------------------\n area diference is :");
	for(int i = 0; i< m_ArrayTargetFaceArea.size(); i++)
		TRACE("%5.6f\t", m_ArrayTargetFaceArea[i] - areaarray1[i]);
	TRACE("----------------------------------------------------------------------\n");
	UnconstrainedOptimization(xarray);

	CMesh3d* pNewMesh = new CMesh3d();
	pNewMesh->Copy(pUniformGridMesh);
	for(int i = 0; i < pNewMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pNewMesh->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		int xindex = std::find(xinterval.begin(),xinterval.end(),x) - xinterval.begin();
		int yindex = std::find(yinterval.begin(),yinterval.end(),y) - yinterval.begin();
		x = xarray[xindex];
		y = xarray[xintervelnum + yindex];
		pVertex->Set(x,y,0);
	}
	pDoc->m_SceneGraph.Add(pNewMesh);
	ComputeFaceArea(pNewMesh,areaarray1);
	TRACE("-----------------Uncontrained Optimization----------------------------------------------------\n area diference is :");
	for(int i = 0; i< m_ArrayTargetFaceArea.size(); i++)
		TRACE("%5.6f\t", m_ArrayTargetFaceArea[i] - areaarray1[i]);
	TRACE("----------------------------------------------------------------------\n");

	for(int i = 0; i < xintervelnum; i++)
	{
		xarray[i] = xinterval[i];
	}
	for(int i = 0; i < yintervalnum; i++)
	{
		xarray[i + xintervelnum] = yinterval[i];
	}
	ConstrainedOptimization(xarray);
	CMesh3d* pNewMesh1 = new CMesh3d();
	pNewMesh1->Copy(pUniformGridMesh);
	for(int i = 0; i < pNewMesh1->NbVertex(); i++)
	{
		CVertex3d* pVertex = pNewMesh1->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		int xindex = std::find(xinterval.begin(),xinterval.end(),x) - xinterval.begin();
		int yindex = std::find(yinterval.begin(),yinterval.end(),y) - yinterval.begin();
		x = xarray[xindex];
		y = xarray[xintervelnum + yindex];
		pVertex->Set(x,y,0);
	}
	pDoc->m_SceneGraph.Add(pNewMesh1);
	ComputeFaceArea(pNewMesh1,areaarray1);
	TRACE("-----------------contrained Optimization----------------------------------------------------\n area diference is :");
	for(int i = 0; i< m_ArrayTargetFaceArea.size(); i++)
		TRACE("%5.6f\t", m_ArrayTargetFaceArea[i] - areaarray1[i]);
	TRACE("----------------------------------------------------------------------\n");

	//linear method
	//fix y
	//LinearOptimization(xarray);*/
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::LinearOptimization(std::vector<double> &areaarray)
{
	//
}
double Action_PolyOmino::ComputeXGradient(std::vector<double> &xinterval,std::vector<double> &yinterval,int index)
{
	//
	double val = 0;
	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceAreaArray.size(); i++)
	{
		std::vector<int> pointset = pUniformGridMesh->m_ArrayFaceAreaArray[i];
		int size = pointset.size() / 4;
		double area = 0;
		for(int j = 0; j < size; j++)
		{
			int index1 = pointset[4 * j];
			int index2 = pointset[4 * j + 1];
			int index3 = pointset[4 * j + 2];
			int index4 = pointset[4 * j + 3];

			area += (xinterval[index2] - xinterval[index1]) * (yinterval[index4] - yinterval[index3]);
		}
		double val1 = (area - pUniformGridMesh->m_ArrayTargetFaceArea[i]) * 2;
		double val2 = 0;
		for(int j = 0; j < size; j++)
		{
			int index1 = pointset[4 * j];
			int index2 = pointset[4 * j + 1];
			int index3 = pointset[4 * j + 2];
			int index4 = pointset[4 * j + 3];

			if(index1 == index)
			{
				val2 += -(yinterval[index4] - yinterval[index3]);
			}else
			if(index2 == index)
			{
				val2 += (yinterval[index4] - yinterval[index3]);
			}	
		}
		val += val1 * val2;
	}
	return val;
}
double Action_PolyOmino::ComputeYGradient(std::vector<double> &xinterval,std::vector<double> &yinterval,int index)
{
	double val = 0;
	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceAreaArray.size(); i++)
	{
		std::vector<int> pointset = pUniformGridMesh->m_ArrayFaceAreaArray[i];
		int size = pointset.size() / 4;
		double area = 0;
		for(int j = 0; j < size; j++)
		{
			int index1 = pointset[4 * j];
			int index2 = pointset[4 * j + 1];
			int index3 = pointset[4 * j + 2];
			int index4 = pointset[4 * j + 3];

			area += (xinterval[index2] - xinterval[index1]) * (yinterval[index4] - yinterval[index3]);
		}
		double val1 = (area - pUniformGridMesh->m_ArrayTargetFaceArea[i]) * 2;
		double val2 = 0;
		for(int j = 0; j < size; j++)
		{
			int index1 = pointset[4 * j];
			int index2 = pointset[4 * j + 1];
			int index3 = pointset[4 * j + 2];
			int index4 = pointset[4 * j + 3];

			if(index3 == index)
			{
				val2 += -(xinterval[index2] - xinterval[index1]);
			}else
				if(index4 == index)
				{
					val2 += (xinterval[index2] - xinterval[index1]);
				}	
		}
		val += val1 * val2;
	}
	return val;
}

double Action_PolyOmino::ComputeFaceArea(CMesh3d* pMesh,std::vector<double> &areaarray)
{
	areaarray.clear();

	std::vector<double> xinterval,yinterval;
	std::pair<int,int> edgepair;
	double x,y;
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		int edgeindex = m_HorizontalSegments[i][0];
		edgepair = pMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex  = pMesh->GetVertex(edgepair.first);
		y = pVertex->y();
		yinterval.push_back(y);
	}
	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		int edgeindex = m_VerticalSegments[i][0];
		edgepair = pMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex  = pMesh->GetVertex(edgepair.first);
		x = pVertex->x();
		xinterval.push_back(x);
	}

	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceAreaArray.size(); i++)
	{
		std::vector<int> pointset = pUniformGridMesh->m_ArrayFaceAreaArray[i];
		int size = pointset.size() / 4;
		double area = 0;
		for(int j = 0; j < size; j++)
		{
			int index1 = pointset[4 * j];
			int index2 = pointset[4 * j + 1];
			int index3 = pointset[4 * j + 2];
			int index4 = pointset[4 * j + 3];

			area += (xinterval[index2] - xinterval[index1]) * (yinterval[index4] - yinterval[index3]);
		}
		areaarray.push_back(area);
	}
	return 1;
}

double Action_PolyOmino::ComputeFaceArea(std::vector<double> &xinterval,std::vector<double> &yinterval,std::vector<double> &areaarray)
{
	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceAreaArray.size(); i++)
	{
		std::vector<int> pointset = pUniformGridMesh->m_ArrayFaceAreaArray[i];
		int size = pointset.size() / 4;
		double area = 0;
		for(int j = 0; j < size; j++)
		{
			int index1 = pointset[4 * j];
			int index2 = pointset[4 * j + 1];
			int index3 = pointset[4 * j + 2];
			int index4 = pointset[4 * j + 3];

			area += (xinterval[index2] - xinterval[index1]) * (yinterval[index4] - yinterval[index3]);
		}
		areaarray.push_back(area);
	}
	return 1;
}

int Action_PolyOmino::IsHorizontalEdge(int edgeindex)
{
	std::pair<int,int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
	CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(pair.first);
	CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(pair.second);

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();

	if(fabs(y1 - y2) < 1e-8)
		return 1;
	else
		return 0;
}
void Action_PolyOmino::OnBnClickedOminoDatadrivenmorphoptimization()
{
	// TODO: Add your control notification handler code here
	//first normalize the target facea rea
	//std::vector<double> areaarray1;
	//ComputeFaceArea(pUniformGridMesh,areaarray1);
	
	//pUniformGridMesh->m_ArrayTargetFaceArea = areaarray1;
	pUniformGridMesh->xintervelnum = pUniformGridMesh->xinterval.size();
	pUniformGridMesh->yintervalnum = pUniformGridMesh->yinterval.size();
	real_1d_array xarray;
	int size = pUniformGridMesh->xintervelnum + pUniformGridMesh->yintervalnum;
	xarray.setlength( size);
	for(int i = 0; i < pUniformGridMesh->xintervelnum; i++)
	{
		xarray[i] = pUniformGridMesh->xinterval[i];
	}
	for(int i = 0; i < pUniformGridMesh->yintervalnum; i++)
	{
		xarray[i + pUniformGridMesh->xintervelnum] = pUniformGridMesh->yinterval[i];
	}
	m_constrainediterationnum = 0;
	//ConstrainedOptimization(xarray);
	//ConstrainedOptimization_boundedconstraits(xarray);
	ConstrainedOptimization_matlab(xarray);
	CMesh3d* pNewMesh1 = new CMesh3d();
	pNewMesh1->Copy(pUniformGridMesh);
	for(int i = 0; i < pNewMesh1->NbVertex(); i++)
	{
		CVertex3d* pVertex = pNewMesh1->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		int xindex = std::find(pUniformGridMesh->xinterval.begin(),pUniformGridMesh->xinterval.end(),x) - pUniformGridMesh->xinterval.begin();
		int yindex = std::find(pUniformGridMesh->yinterval.begin(),pUniformGridMesh->yinterval.end(),y) - pUniformGridMesh->yinterval.begin();
		x = xarray[xindex];
		y = xarray[pUniformGridMesh->xintervelnum + yindex];
		pVertex->Set(x,y,0);
	}
	pNewMesh1->m_ArrayTargetFaceArea = pUniformGridMesh->m_ArrayTargetFaceArea;
	pNewMesh1->m_ArrayFaceAreaArray = pUniformGridMesh->m_ArrayFaceAreaArray;
	std::vector<double> facearea;
	ComputeFaceArea(pNewMesh1,facearea);
	pNewMesh1->m_ArrayFaceArea = facearea;
	pNewMesh1->m_ArrayFacePointsArray = pUniformGridMesh->m_ArrayFacePointsArray;
	pNewMesh1->m_ArrayFaceEdgesArray = pUniformGridMesh->m_ArrayFaceEdgesArray;
	pNewMesh1->m_ArrayDifferenceFaceArea.resize(pNewMesh1->m_ArrayFaceArea.size());
	for(int i = 0; i < pNewMesh1->m_ArrayFaceArea.size(); i++)
	{
		pNewMesh1->m_ArrayDifferenceFaceArea[i] = fabs(pNewMesh1->m_ArrayFaceArea[i] - pNewMesh1->m_ArrayTargetFaceArea[i]) / pNewMesh1->m_ArrayTargetFaceArea[i];
	}
	pDoc->m_SceneGraph.Add(pNewMesh1);
	//pUniformGridMesh = pNewMesh1;
	pUniformGridMesh->m_ArraySharpEdges  = pUniformGridMesh->m_ArraySharpEdges;
	pUniformGridMesh = pNewMesh1;

	//
	//UniformgridProgessiveMeshOneIteration();
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::OnBnClickedPolyominoSetArea()
{
	FaceInputArea dlg;
	if(dlg.DoModal() == IDOK)
	{
		pUniformGridMesh->m_ArrayTargetFaceArea[pUniformGridMesh->m_currentfaceindex++] = dlg.area;
		if(pUniformGridMesh->m_currentfaceindex < pUniformGridMesh->m_ArrayFacePointsArray.size() )
			pUniformGridMesh->m_ArraySharpVertexIndex = pUniformGridMesh->m_ArrayFacePointsArray[pUniformGridMesh->m_currentfaceindex];
	}
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedOminoSavefacearea()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(FALSE);
	if(dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}else
		return;

	std::ofstream file(FileName);

	file << pUniformGridMesh->xintervelnum << " " << pUniformGridMesh->yintervalnum << "\n";
	file << pUniformGridMesh->m_currentfaceindex << "\n";
	file << pUniformGridMesh->xinterval.size() << "\n";
	for(int i = 0; i < pUniformGridMesh->xinterval.size(); i++)
	{
		file << pUniformGridMesh->xinterval[i] << " ";
	}
	file << "\n";
	file << pUniformGridMesh->yinterval.size() << "\n";
	for(int i = 0; i < pUniformGridMesh->yinterval.size(); i++)
	{
		file << pUniformGridMesh->yinterval[i] << " ";
	}
	file << "\n";
	file << pUniformGridMesh->m_ArrayTargetFaceArea.size() << "\n";
	for(int i = 0; i < pUniformGridMesh->m_ArrayTargetFaceArea.size(); i++)
		file << pUniformGridMesh->m_ArrayTargetFaceArea[i] << " ";
	file << "\n";
	file << pUniformGridMesh->m_ArrayFaceAreaArray.size() <<"\n";
	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceAreaArray.size(); i++)
	{
		file << pUniformGridMesh->m_ArrayFaceAreaArray[i].size() << "\n";
		for(int j = 0; j < pUniformGridMesh->m_ArrayFaceAreaArray[i].size(); j++)
		{
			file << pUniformGridMesh->m_ArrayFaceAreaArray[i][j] << " ";
		}
		file << "\n";
	}
	file <<pUniformGridMesh-> m_ArrayFaceAreaArray.size() <<"\n";
	for(int i = 0; i < pUniformGridMesh->m_ArrayFacePointsArray.size(); i++)
	{
		file << pUniformGridMesh->m_ArrayFacePointsArray[i].size() << "\n";
		for(int j = 0; j < pUniformGridMesh->m_ArrayFacePointsArray[i].size(); j++)
		{
			file << pUniformGridMesh->m_ArrayFacePointsArray[i][j] << " ";
		}
		file << "\n";
	}
	file << pUniformGridMesh->m_ArrayFaceEdgesArray.size() <<"\n";
	for(int i = 0; i < pUniformGridMesh->m_ArrayFaceEdgesArray.size(); i++)
	{
		file << pUniformGridMesh->m_ArrayFaceEdgesArray[i].size() << "\n";
		for(int j = 0; j < pUniformGridMesh->m_ArrayFaceEdgesArray[i].size(); j++)
		{
			file << pUniformGridMesh->m_ArrayFaceEdgesArray[i][j] << " ";
		}
		file << "\n";
	}
	file.close();
}
void Action_PolyOmino::LoadFaceArea(CString FileName)
{
	std::ifstream file(FileName);

	file >> pUniformGridMesh->xintervelnum >> pUniformGridMesh->yintervalnum;
	file >> pUniformGridMesh->m_currentfaceindex;
	int size;
	file >> size;
	//pUniformGridMesh->xinterval.resize(size);
	double val;
	for(int i = 0; i < size; i++)
	{
		//file >> pUniformGridMesh->xinterval[i];
		file >> val;
	}
	file >> size;
	//pUniformGridMesh->yinterval.resize(size);
	for(int i = 0; i < size; i++)
	{
		//file >> pUniformGridMesh->yinterval[i];
		file >> val;
	}

	file >> size;
	pUniformGridMesh->m_ArrayTargetFaceArea.resize(size);
	for(int i = 0; i < size; i++)
		file >> pUniformGridMesh->m_ArrayTargetFaceArea[i];

	return;
	file >> size;
	pUniformGridMesh->m_ArrayFaceAreaArray.resize(size);
	int size1;
	for(int i = 0; i < size; i++)
	{
		file >> size1;
		pUniformGridMesh->m_ArrayFaceAreaArray[i].resize(size1);
		for(int j = 0; j < size1; j++)
		{
			file >> pUniformGridMesh->m_ArrayFaceAreaArray[i][j];
		}
	}
	file >> size;
	pUniformGridMesh->m_ArrayFaceAreaArray.resize(size);
	for(int i = 0; i < size; i++)
	{
		file >> size1;
		pUniformGridMesh->m_ArrayFacePointsArray[i].resize(size1);
		for(int j = 0; j < size1; j++)
		{
			file >> pUniformGridMesh->m_ArrayFacePointsArray[i][j];
		}
	}
	file >> size;
	pUniformGridMesh->m_ArrayFaceEdgesArray.resize(size);
	for(int i = 0; i < size; i++)
	{
		file >> size1;
		pUniformGridMesh->m_ArrayFaceEdgesArray[i].resize(size1);
		for(int j = 0; j < size1; j++)
		{
			file >> pUniformGridMesh->m_ArrayFaceEdgesArray[i][j];
		}
	}
	file.close();
	pUniformGridMesh->m_ArraySharpVertexIndex = pUniformGridMesh->m_ArrayFacePointsArray[pUniformGridMesh->m_currentfaceindex];
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::LoadTargetFaceAreaArray(int index)
{
	//0: population 1: facebook 2: highschool 3: familyincome 4: household income
	//
	CString FileName;
	switch(index)
	{
	case 0:	
		FileName = "D:\\papers\\code\\data\\Geo\\dense\\1788_usapopulation_1990_2014.txt";
		break;
	case 1:
		FileName = "D:\\papers\\code\\data\\Geo\\dense\\1788_usa_facebookpenetration.txt";
		break;
	case 2:
		FileName = "D:\\papers\\code\\data\\Geo\\dense\\1788_usa_completed_highschool.txt";
		break;
	case 3:
		FileName = "D:\\papers\\code\\data\\Geo\\dense\\1788_usa_familyincome.txt";
		break;
	case 4:
		FileName = "D:\\papers\\code\\data\\Geo\\dense\\1788_usa_median_household_income.txt";
		break;
	}
	 

	std::ifstream file(FileName);
	pUniformGridMesh->m_2DArrayTargetFaceArea.resize(pUniformGridMesh->m_ArrayStates.size());
	int index1 = 0;
	while(!file.eof())
	{
		char message[500];

		//first name
		file.getline(message,500);

		//get the name.
		std::string inputmessage(message);
		std::istringstream is(inputmessage);

		std::string s1,s2;
		is >> s1;

		if(s1.compare("West") == 0 || s1.compare("South") == 0 || s1.compare("North") == 0 || s1.compare("New") == 0 || s1.compare("Rhode") == 0)
		{
			is >> s2;
			s1 = s1 + " " + s2;
		}else
		if(s1.compare("District") == 0)
		{
			is >> s2;
			s1 = s1 + " " + s2;
			is >> s2;
			s1 = s1 + " " + s2;
		}
		char* pResult;
		int index1 = -1;
		for(int i = 0; i < pUniformGridMesh->m_ArrayDistrictName.size(); i++)
		{
			if(s1.compare((LPSTR)(LPCTSTR)(pUniformGridMesh->m_ArrayDistrictName[i])) == 0)
			{
				index1 = i;
				break;
			}
		}

		if(index1 == -1)
			continue;

		if(index1 > 31)
			index1--;

		int num;
		switch(index)
		{
		case 0:
			{
				is >> num  >> num ;
				is >> num  >> num ;
				for(int j = 0; j < 25; j++)
					//for(int j = 0; j < 1; j++)
				{
					is >> num;
					pUniformGridMesh->m_2DArrayTargetFaceArea[index1].push_back(num);
				}
				is >> num  >> num ;
				break;
			}
		case 1:
			{
				for(int j = 0; j < 3; j++)
					//for(int j = 0; j < 1; j++)
				{
					is >> num;
					pUniformGridMesh->m_2DArrayTargetFaceArea[index1].push_back(num);
				}
				break;
			}
		default:
			{
				is >> num;
				pUniformGridMesh->m_2DArrayTargetFaceArea[index1].push_back(num);
			}
		}	
	}
	file.close();
}
void Action_PolyOmino::Loadworldpopulation()
{
	countryinfo info;
	//std::ifstream file("D:\\papers\\code\\data\\Geo\\ditu\\dense\\final\\worldmap.txt");
	//std::ifstream file("D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\chinagdp2017.txt");
	std::ifstream file("D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\final\\chinapopulation2017.txt");
	char message[100];
	int num,val;
	//std::vector<int> testval;
	while (!file.eof())
	{
		info.districtarray.clear();
		file >> info.index;
		file >> message;
		//file1 >> info.index;
		//file1 >> message;
		info.contryname = message;
		//file1 >> val;
		file >> info.population;
		//info.population = info.population / val;
		file >> num;
		//file1 >> num;
		for (int i = 0; i < num; i++)
		{
			file >> val;
			//file1 >> val;
			info.districtarray.push_back(val);
		}
		m_ArrayCountryInfo.push_back(info);
		//testval.insert(testval.begin(), info.districtarray.begin(), info.districtarray.end());
	}
	file.close();
	//file1.close();

	/*int num1 = 0;
	for (int i = 0; i < 138; i++)
	{
		num1 = 0;
		for (int j = 0; j < testval.size(); j++)
		{
			if (testval[j] == i)
				num1++;
		}
		if(num1 != 1)
		//if (std::find(testval.begin(), testval.end(), i) == testval.end())
		{
			pUniformGridMesh->m_ArraySharpVertexIndex = pUniformGridMesh->m_ArrayFacePointsArray[i];
			break;
			int ij = 0;
		}
	}*/

	//target population
	pUniformGridMesh->m_ArrayTargetFaceArea.resize(m_ArrayCountryInfo.size());
	pUniformGridMesh->m_2DArrayTargetFaceArea.resize(m_ArrayCountryInfo.size());
	double population = 0;
	for (int i = 0; i < m_ArrayCountryInfo.size(); i++)
	{
		pUniformGridMesh->m_ArrayTargetFaceArea[i] = m_ArrayCountryInfo[i].population;
		pUniformGridMesh->m_2DArrayTargetFaceArea[i].push_back(m_ArrayCountryInfo[i].population);
	}

	//
	pUniformGridMesh->m_ArrayStates.resize(m_ArrayCountryInfo.size());
	pUniformGridMesh->m_ArrayDistrictName.resize(m_ArrayCountryInfo.size());
	for (int i = 0; i < m_ArrayCountryInfo.size(); i++)
	{
		pUniformGridMesh->m_ArrayStates[i] = m_ArrayCountryInfo[i].districtarray;
		pUniformGridMesh->m_ArrayDistrictName[i] = m_ArrayCountryInfo[i].contryname;
	}
}
void Action_PolyOmino::LoadTargetFaceArea(CString FileName)
{
	std::ifstream file(FileName);

	int num;
	//file >> pUniformGridMesh->xintervelnum >> pUniformGridMesh->yintervalnum;
	//file >> pUniformGridMesh->m_currentfaceindex;
	file >> num >> num;
	file >> num;
	int size;
	file >> size;
	//pUniformGridMesh->xinterval.resize(size);
	double val;
	for(int i = 0; i < size; i++)
	{
		//file >> pUniformGridMesh->xinterval[i];
		file >> val;
	}
	file >> size;
	//pUniformGridMesh->yinterval.resize(size);
	for(int i = 0; i < size; i++)
	{
		//file >> pUniformGridMesh->yinterval[i];
		file >> val;
	}

	file >> size;
	pUniformGridMesh->m_ArrayTargetFaceArea.resize(size);
	for(int i = 0; i < size; i++)
		file >> pUniformGridMesh->m_ArrayTargetFaceArea[i];
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::OnBnClickedOminoLoadfacearea()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}else
		return;
	
	LoadFaceArea(FileName);
}

void Action_PolyOmino::OnBnClickedOminoAddmergeconstraints()
{
	// TODO: Add your control notification handler code here
	if(!pMesh)
	{
		AfxMessageBox("Please Set Current Mesh!");
		return;
	}
	if(pMesh->m_SelectedLandmarkIndexArray.size() != 2)
	{
		AfxMessageBox("Please Set Current Mesh!");
		return;
	}
	std::pair<int,int> pair;
	pair.first = pMesh->m_SelectedLandmarkIndexArray[0];
	pair.second = pMesh->m_SelectedLandmarkIndexArray[1];
	pMesh->m_ArrayMergeConstraints.push_back(pair);
}

void Action_PolyOmino::OnBnClickedOminoSavemergeconstraints()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(FALSE);
	if(dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}else
		return;

	SaveMergeConstraints(FileName);
}
void Action_PolyOmino::SaveMergeConstraints(CString filename)
{
	std::ofstream file(filename);
	int num = pMesh->m_ArrayMergeConstraints.size();
	file << num << "\n";
	std::pair<int,int> pair;
	for(int i = 0; i < num; i++)
	{
		pair = pMesh->m_ArrayMergeConstraints[i];
		file << pair.first << " " << pair.second << "\n";
	}
	file.close();
}
void Action_PolyOmino::LoadMergeConstraints(CString filename)
{
	std::ifstream file(filename);
	if(!pMesh)
	{
		AfxMessageBox("Please Set Current Mesh!");
		return;
	}
	pMesh->m_ArrayMergeConstraints.clear();
	int num;
	file >> num;
	std::pair<int,int> pair;
	for(int i = 0; i < num; i++)
	{
		file >> pair.first >> pair.second;
		pMesh->m_ArrayMergeConstraints.push_back(pair);
	}
	file.close();
}
void Action_PolyOmino::OnBnClickedOminoLoadmergeconstraints()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}else
		return;

	LoadMergeConstraints(FileName);
}

void Action_PolyOmino::OnBnClickedOminoLoaddata()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}else
		return;

	int iterationnum;
	LoadData1(FileName,iterationnum);
}

void Action_PolyOmino::OnBnClickedOminoLoadgeorgiamap()
{
	// TODO: Add your control notification handler code here
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\georgia\\georgia_county_normalized_25_cornerflip.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\georgia\\georgia_county_normalized_33_simplified_cornerflip.m");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	OnBnClickedOminoSetcurrentmesh();
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pMesh->m_showFace = FALSE;

	OnBnClickedGeneratelandmarks();
	//Load Landmarks
	char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\landmark36.txt");
	sprintf(message, "D:\\papers\\code\\data\\Geo\\Georgia\\landmark37.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	Loadlandmark(landmarkname);

	//Load Constraints
	//LoadMergeConstraints("D:\\papers\\code\\data\\Geo\\dense\\mergeconstraints.txt");

	//Delete Auxillary Landmarks
	//OnBnClickedPolyominoDeleteauxillarylandmarks();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::DetectInvalidSharpEdges()
{
	for (int i = 0; i < pUniformGridMesh->xinterval.size() - 1; i++)
	{
		double dis = pUniformGridMesh->xinterval[i + 1] - pUniformGridMesh->xinterval[0];
		if (dis / (pUniformGridMesh->baseinterval * 3) - int(dis / (pUniformGridMesh->baseinterval * 3)) > 0.01)
		{
			pUniformGridMesh->m_ArraySelectedSharpEdgeIndex.insert(pUniformGridMesh->m_ArraySelectedSharpEdgeIndex.begin(), m_VerticalSegments[i + 1].begin(), m_VerticalSegments[i + 1].end());
		}
	}
	for (int i = 0; i < pUniformGridMesh->yinterval.size() - 1; i++)
	{
		double dis = pUniformGridMesh->yinterval[i + 1] - pUniformGridMesh->yinterval[0];
		if (dis / (pUniformGridMesh->baseinterval * 3)- int(dis / (pUniformGridMesh->baseinterval * 3)) > 0.01)
		{
			pUniformGridMesh->m_ArraySelectedSharpEdgeIndex.insert(pUniformGridMesh->m_ArraySelectedSharpEdgeIndex.begin(), m_HorizontalSegments[i + 1].begin(), m_HorizontalSegments[i + 1].end());
		}
	}
	for (int i = 0; i < pUniformGridMesh->xinterval.size(); i++)
	{
		double dis = pUniformGridMesh->xinterval[i] - pUniformGridMesh->xinterval[0];
		dis = int(dis / (pUniformGridMesh->baseinterval * 3)) * (pUniformGridMesh->baseinterval * 3);
		pUniformGridMesh->xinterval[i] = pUniformGridMesh->xinterval[0] + dis;
	}
	for (int i = 0; i < pUniformGridMesh->yinterval.size(); i++)
	{
		double dis = pUniformGridMesh->yinterval[i] - pUniformGridMesh->yinterval[0];
		dis = int(dis / (pUniformGridMesh->baseinterval * 3)) * (pUniformGridMesh->baseinterval * 3);
		pUniformGridMesh->yinterval[i] = pUniformGridMesh->yinterval[0] + dis;
	}
	for (int i = 0; i < pUniformGridMesh->xinterval.size(); i++)
	{
		for (int j = 0; j < m_VerticalSegments[i].size(); j++)
		{
			std::pair<int, int> pair1 = pUniformGridMesh->m_ArraySharpEdges[m_VerticalSegments[i][j]];
			int vindex1_ = pair1.first;
			int vindex2_ = pair1.second;

			CVertex3d* pStartVertex1 = pUniformGridMesh->GetVertex(vindex1_);
			CVertex3d* pEndVertex1 = pUniformGridMesh->GetVertex(vindex2_);

			pStartVertex1->x(pUniformGridMesh->xinterval[i]);
			pEndVertex1->x(pUniformGridMesh->xinterval[i]);
		}
	}
	for (int i = 0; i < pUniformGridMesh->yinterval.size(); i++)
	{
		for (int j = 0; j < m_HorizontalSegments[i].size(); j++)
		{
			std::pair<int, int> pair1 = pUniformGridMesh->m_ArraySharpEdges[m_HorizontalSegments[i][j]];
			int vindex1_ = pair1.first;
			int vindex2_ = pair1.second;

			CVertex3d* pStartVertex1 = pUniformGridMesh->GetVertex(vindex1_);
			CVertex3d* pEndVertex1 = pUniformGridMesh->GetVertex(vindex2_);
			
			pStartVertex1->y(pUniformGridMesh->yinterval[i]);
			pEndVertex1->y(pUniformGridMesh->yinterval[i]);
		}
	}
}
void Action_PolyOmino::AdjustHorizontalandVerticalGroupofLinePositions() 
{
	int ii;
	//std::vector<double> yinterval,xinterval;
	pUniformGridMesh->xinterval.clear();
	pUniformGridMesh->yinterval.clear();
	std::pair<int,int> edgepair;
	double x,y;
	for(int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		int edgeindex = m_HorizontalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
		y = pVertex->y();
		pUniformGridMesh->yinterval.push_back(y);
	}
	for(int i = 0; i < m_VerticalSegments.size(); i++)
	{
		int edgeindex = m_VerticalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
		x = pVertex->x();
		pUniformGridMesh->xinterval.push_back(x);
	}

	//
	//std::sort(xinterval.begin(),xinterval.end());
	//std::sort(yinterval.begin(),yinterval.end());

	std::vector<int> m_ArrayXindex,m_ArrayYindex;
	double minimalxinterval,minimalyinterval;

	minimalxinterval = 1e10;
	minimalyinterval = 1e10;
	int minimalxindex,minimalyindex;
	for(int i = 0; i < pUniformGridMesh->xinterval.size() - 1; i++)
	{
		if(std::find(m_ArrayXindex.begin(),m_ArrayXindex.end(),i) != m_ArrayXindex.end())
			continue;
		double val = pUniformGridMesh->xinterval[i + 1] - pUniformGridMesh->xinterval[i];
		if(val < minimalxinterval)
		{
			minimalxinterval = val;
			minimalxindex = i;
		}
	}

	for(int i = 0; i < pUniformGridMesh->yinterval.size() - 1; i++)
	{
		if(std::find(m_ArrayYindex.begin(),m_ArrayYindex.end(),i) != m_ArrayYindex.end())
			continue;
		double val = pUniformGridMesh->yinterval[i + 1] - pUniformGridMesh->yinterval[i];
		if(val < minimalyinterval)
		{
			minimalyinterval = val;
			minimalyindex = i;
		}
	}
	//if(pUniformGridMesh->xinterval.size() == 1 && pUniformGridMesh->yinterval.size() == 1)
	//	return;

	BOOL isChanged = FALSE;

	//double baseinterval;
	if(minimalxinterval < minimalyinterval)
	{
		pUniformGridMesh->baseinterval = minimalxinterval;
	}else
		pUniformGridMesh->baseinterval = minimalyinterval;
	//pUniformGridMesh->baseinterval = pUniformGridMesh->baseinterval * 1.05;
	//pUniformGridMesh->baseinterval = 0.86789703369140625;
	pUniformGridMesh->baseinterval = 0.79700469970703125;
	//return;
	for(int i = 1; i < pUniformGridMesh->yinterval.size(); i++)
	{
		double interval = pUniformGridMesh->yinterval[i] - pUniformGridMesh->yinterval[i - 1];
		int division = interval / pUniformGridMesh->baseinterval + 0.5;
		double increment = pUniformGridMesh->baseinterval * division - interval;
		for(int j = i; j < pUniformGridMesh->yinterval.size(); j++)
		{
			pUniformGridMesh->yinterval[j] = pUniformGridMesh->yinterval[j] + increment;
		}
	}

	for(int i = 1; i < pUniformGridMesh->xinterval.size(); i++)
	{
		double interval = pUniformGridMesh->xinterval[i] - pUniformGridMesh->xinterval[i - 1];
		int division = interval / pUniformGridMesh->baseinterval + 0.5;
		double increment = pUniformGridMesh->baseinterval * division - interval;
		for(int j = i; j < pUniformGridMesh->xinterval.size(); j++)
		{
			pUniformGridMesh->xinterval[j] = pUniformGridMesh->xinterval[j] + increment;
		}
	}

	std::vector<int> edgeset;
	for(int i = 1; i < m_HorizontalSegments.size(); i++)
	{
		edgeset = m_HorizontalSegments[i];
		for(int j = 0; j < edgeset.size(); j++)
		{
			int edgeindex = m_HorizontalSegments[i][j];
			edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

			CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
			pVertex->y(pUniformGridMesh->yinterval[i]);
			pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
			pVertex->y(pUniformGridMesh->yinterval[i]);
		}

	}
	for(int i = 1; i < m_VerticalSegments.size(); i++)
	{
		edgeset = m_VerticalSegments[i];
		for(int j = 0; j < edgeset.size(); j++)
		{
			int edgeindex = m_VerticalSegments[i][j];
			edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

			CVertex3d* pVertex  = pUniformGridMesh->GetVertex(edgepair.first);
			pVertex->x(pUniformGridMesh->xinterval[i]);
			pVertex  = pUniformGridMesh->GetVertex(edgepair.second);
			pVertex->x(pUniformGridMesh->xinterval[i]);
		}
	}
}
void Action_PolyOmino::AdjustHorizontalandVerticalGroupofLinePositions_3base()
{
	int ii;
	//std::vector<double> yinterval,xinterval;
	pUniformGridMesh->xinterval.clear();
	pUniformGridMesh->yinterval.clear();
	std::pair<int, int> edgepair;
	double x, y;
	for (int i = 0; i < m_HorizontalSegments.size(); i++)
	{
		int edgeindex = m_HorizontalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex = pUniformGridMesh->GetVertex(edgepair.first);
		y = pVertex->y();
		pUniformGridMesh->yinterval.push_back(y);
	}
	for (int i = 0; i < m_VerticalSegments.size(); i++)
	{
		int edgeindex = m_VerticalSegments[i][0];
		edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		CVertex3d* pVertex = pUniformGridMesh->GetVertex(edgepair.first);
		x = pVertex->x();
		pUniformGridMesh->xinterval.push_back(x);
	}

	//
	//std::sort(xinterval.begin(),xinterval.end());
	//std::sort(yinterval.begin(),yinterval.end());

	std::vector<int> m_ArrayXindex, m_ArrayYindex;
	double minimalxinterval, minimalyinterval;

	minimalxinterval = 1e10;
	minimalyinterval = 1e10;
	int minimalxindex, minimalyindex;
	for (int i = 0; i < pUniformGridMesh->xinterval.size() - 1; i++)
	{
		if (std::find(m_ArrayXindex.begin(), m_ArrayXindex.end(), i) != m_ArrayXindex.end())
			continue;
		double val = pUniformGridMesh->xinterval[i + 1] - pUniformGridMesh->xinterval[i];
		if (val < minimalxinterval)
		{
			minimalxinterval = val;
			minimalxindex = i;
		}
	}

	for (int i = 0; i < pUniformGridMesh->yinterval.size() - 1; i++)
	{
		if (std::find(m_ArrayYindex.begin(), m_ArrayYindex.end(), i) != m_ArrayYindex.end())
			continue;
		double val = pUniformGridMesh->yinterval[i + 1] - pUniformGridMesh->yinterval[i];
		if (val < minimalyinterval)
		{
			minimalyinterval = val;
			minimalyindex = i;
		}
	}
	//if(pUniformGridMesh->xinterval.size() == 1 && pUniformGridMesh->yinterval.size() == 1)
	//	return;

	BOOL isChanged = FALSE;

	//double baseinterval;
	if (minimalxinterval < minimalyinterval)
	{
		pUniformGridMesh->baseinterval = minimalxinterval;
	}
	else
		pUniformGridMesh->baseinterval = minimalyinterval;

	for (int i = 1; i < pUniformGridMesh->yinterval.size(); i++)
	{
		double interval = pUniformGridMesh->yinterval[i] - pUniformGridMesh->yinterval[i - 1];
		int division = interval / (pUniformGridMesh->baseinterval * 3) + 0.5;
		if (division == 0)
			division = 1;
		double increment = pUniformGridMesh->baseinterval * 3 * division - interval;
		for (int j = i; j < pUniformGridMesh->yinterval.size(); j++)
		{
			pUniformGridMesh->yinterval[j] = pUniformGridMesh->yinterval[j] + increment;
		}
	}

	for (int i = 1; i < pUniformGridMesh->xinterval.size(); i++)
	{
		double interval = pUniformGridMesh->xinterval[i] - pUniformGridMesh->xinterval[i - 1];
		int division = interval / (pUniformGridMesh->baseinterval * 3) + 0.5;
		if (division == 0)
			division = 1;
		double increment = pUniformGridMesh->baseinterval * 3 * division - interval;
		for (int j = i; j < pUniformGridMesh->xinterval.size(); j++)
		{
			pUniformGridMesh->xinterval[j] = pUniformGridMesh->xinterval[j] + increment;
		}
	}

	std::vector<int> edgeset;
	for (int i = 1; i < m_HorizontalSegments.size(); i++)
	{
		edgeset = m_HorizontalSegments[i];
		for (int j = 0; j < edgeset.size(); j++)
		{
			int edgeindex = m_HorizontalSegments[i][j];
			edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

			CVertex3d* pVertex = pUniformGridMesh->GetVertex(edgepair.first);
			pVertex->y(pUniformGridMesh->yinterval[i]);
			pVertex = pUniformGridMesh->GetVertex(edgepair.second);
			pVertex->y(pUniformGridMesh->yinterval[i]);
		}

	}
	for (int i = 1; i < m_VerticalSegments.size(); i++)
	{
		edgeset = m_VerticalSegments[i];
		for (int j = 0; j < edgeset.size(); j++)
		{
			int edgeindex = m_VerticalSegments[i][j];
			edgepair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

			CVertex3d* pVertex = pUniformGridMesh->GetVertex(edgepair.first);
			pVertex->x(pUniformGridMesh->xinterval[i]);
			pVertex = pUniformGridMesh->GetVertex(edgepair.second);
			pVertex->x(pUniformGridMesh->xinterval[i]);
		}
	}
}
void Action_PolyOmino::TraceFaces(std::vector<int> &m_ArrayFacePoints,std::vector<int> &m_ArrayFaceEdges)
{
	std::vector<std::vector<int>> m_ArrayEdge_flags;
	UpdatePointConnectedSelectedEdgeIndex();
	m_ArrayEdge_flags.resize(pUniformGridMesh->m_ArraySharpEdges.size());
	//get one halfedge from the array
	for(int i = 0; i < m_ArrayEdge_flags.size(); i++)
	{
		m_ArrayFacePoints.clear();
		m_ArrayFaceEdges.clear();
		int size = m_ArrayEdge_flags[i].size();
		if(size ==0)
		{
			if(TraceFace(i,1,m_ArrayEdge_flags,m_ArrayFacePoints,m_ArrayFaceEdges))
			{
				pUniformGridMesh->m_ArrayFacePointsArray.push_back(m_ArrayFacePoints);
				pUniformGridMesh->m_ArrayFaceEdgesArray.push_back(m_ArrayFaceEdges);
				pUniformGridMesh->m_ArraySharpVertexIndex = m_ArrayFacePoints;
			}
		}else
			if(size ==1)
			{
				if(TraceFace(i,-m_ArrayEdge_flags[i][0],m_ArrayEdge_flags,m_ArrayFacePoints,m_ArrayFaceEdges))
				{
					pUniformGridMesh->m_ArrayFacePointsArray.push_back(m_ArrayFacePoints);
					pUniformGridMesh->m_ArrayFaceEdgesArray.push_back(m_ArrayFaceEdges);
					pUniformGridMesh->m_ArraySharpVertexIndex = m_ArrayFacePoints;
				}
			}
		//if (pUniformGridMesh->m_ArrayFacePointsArray.size() == 14)
		//	return;
	}
}
void Action_PolyOmino::ComputeFacesFourElementArray(std::vector<int> &m_ArrayFacePoints,std::vector<int> &m_ArrayFaceEdges)
{
	std::vector<int> xintervalindexarray;
	std::vector<int> yintervalindexarray;
	std::vector<double>::iterator intervaliterator;
	std::vector<int> areaarray;
	for(int i = 0; i < pUniformGridMesh->m_ArrayFacePointsArray.size(); i++)
	{
		areaarray.clear();
		xintervalindexarray.clear();
		yintervalindexarray.clear();
		//For each face
		m_ArrayFacePoints = pUniformGridMesh->m_ArrayFacePointsArray[i];
		m_ArrayFaceEdges = pUniformGridMesh->m_ArrayFaceEdgesArray[i];
		for(int ii = 0; ii < m_ArrayFaceEdges.size(); ii++)
		{
			int edgeindex = m_ArrayFaceEdges[ii];
			BOOL isHorizontal = IsHorizontalEdge(edgeindex);

			std::pair<int,int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
			CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(pair.first);
			CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(pair.second);

			double x1 = pStartVertex->x();
			double y1 = pStartVertex->y();
			double x2 = pEndVertex->x();
			double y2 = pEndVertex->y();
			if(!isHorizontal)
			{
				intervaliterator = std::find(pUniformGridMesh->xinterval.begin(),pUniformGridMesh->xinterval.end(),x1);
				if(intervaliterator == pUniformGridMesh->xinterval.end())
					ASSERT(0);

				int vindex = intervaliterator - pUniformGridMesh->xinterval.begin();
				if(std::find(xintervalindexarray.begin(),xintervalindexarray.end(),vindex) == xintervalindexarray.end())
					xintervalindexarray.push_back(vindex);
			}else
			{
				intervaliterator = std::find(pUniformGridMesh->yinterval.begin(),pUniformGridMesh->yinterval.end(),y1);
				if(intervaliterator == pUniformGridMesh->yinterval.end())
					ASSERT(0);

				int vindex = intervaliterator - pUniformGridMesh->yinterval.begin();
				if(std::find(yintervalindexarray.begin(),yintervalindexarray.end(),vindex) == yintervalindexarray.end())
					yintervalindexarray.push_back(vindex);
			}
		}
		/*for(int j = 0; j < m_ArrayFacePoints.size(); j++)
		{
			int index = m_ArrayFacePoints[j];
			CVertex3d* pVertex = pUniformGridMesh->GetVertex(index);
			double x = pVertex->x();
			double y = pVertex->y();

			intervaliterator = std::find(pUniformGridMesh->xinterval.begin(),pUniformGridMesh->xinterval.end(),x);
			if(intervaliterator == pUniformGridMesh->xinterval.end())
				ASSERT(0);
			
			int vindex = intervaliterator - pUniformGridMesh->xinterval.begin();
			if(std::find(xintervalindexarray.begin(),xintervalindexarray.end(),vindex) == xintervalindexarray.end())
				xintervalindexarray.push_back(vindex);

			intervaliterator = std::find(pUniformGridMesh->yinterval.begin(),pUniformGridMesh->yinterval.end(),y);
			if(intervaliterator == pUniformGridMesh->yinterval.end())
				ASSERT(0);

			vindex = intervaliterator - pUniformGridMesh->yinterval.begin();
			if(std::find(yintervalindexarray.begin(),yintervalindexarray.end(),vindex) == yintervalindexarray.end())
				yintervalindexarray.push_back(vindex);
		}*/
		//fix the top and left lines and move the others 
		//such that the other intervals are the interger multipliers of the base
		//
		std::sort(xintervalindexarray.begin(),xintervalindexarray.end());
		std::sort(yintervalindexarray.begin(),yintervalindexarray.end());

		//Use the middle vertical line to intersect with the horizontal lines
		for(int ii = 0; ii < xintervalindexarray.size() - 1; ii++)
		{
			int xindex1 = xintervalindexarray[ii];
			int xindex2 = xintervalindexarray[ii + 1];
			double x1 = pUniformGridMesh->xinterval[xindex1];
			double x2 = pUniformGridMesh->xinterval[xindex2];

			//
			//double middlex = (x1 + x2) / 2;
			double middlex = (x1 * 4.5 + x2) / 5.5;
			std::vector<int> yindexArray;
			for(int jj = 0; jj < m_ArrayFaceEdges.size(); jj++)
			{
				int edgeindex = m_ArrayFaceEdges[jj];
				BOOL isHorizontal = IsHorizontalEdge(edgeindex);

				if(isHorizontal)
				{
					std::pair<int,int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
					CVertex3d* pStartVertex = pUniformGridMesh->GetVertex(pair.first);
					CVertex3d* pEndVertex = pUniformGridMesh->GetVertex(pair.second);

					double x1 = pStartVertex->x();
					double y1 = pStartVertex->y();
					double x2 = pEndVertex->x();
					double y2 = pEndVertex->y();
					if(middlex > x1 && middlex < x2 || middlex > x2 && middlex < x1)
					{
						int yindex = std::find(pUniformGridMesh->yinterval.begin(),pUniformGridMesh->yinterval.end(),y1) - pUniformGridMesh->yinterval.begin();
						yindexArray.push_back(yindex);
					}
				}
			}
			//sort
			std::sort(yindexArray.begin(),yindexArray.end());
			
			if(yindexArray.size() % 2 != 0)
				ASSERT(0);

			int size = yindexArray.size() / 2;

			for(int i1 = 0; i1 < size; i1++)
			{
				areaarray.push_back(xindex1);
				areaarray.push_back(xindex2);
				areaarray.push_back(yindexArray[i1 * 2]);
				areaarray.push_back(yindexArray[i1 * 2 + 1]);
			}
		}
		pUniformGridMesh->m_ArrayFaceAreaArray.push_back(areaarray);
	}
}
void Action_PolyOmino::AssignUniformCellstoFaces()
{
	int xcellnum = (pUniformGridMesh->xinterval[pUniformGridMesh->xinterval.size() - 1] - pUniformGridMesh->xinterval[0]) / pUniformGridMesh->baseinterval;
	int ycellnum = (pUniformGridMesh->yinterval[pUniformGridMesh->yinterval.size() - 1] - pUniformGridMesh->yinterval[0]) / pUniformGridMesh->baseinterval;

	std::pair<int,int> pair;
	pUniformGridMesh->m_ArrayUniformCells.resize(pUniformGridMesh->m_ArrayFaceAreaArray.size());

	for(int i = 0; i < xcellnum; i++)
		for(int j = 0; j < ycellnum; j++)
		{
			//compute the center of the cell
			double centerx = pUniformGridMesh->xinterval[0] + pUniformGridMesh->baseinterval * i + pUniformGridMesh->baseinterval * 0.5 ;
			double centery = pUniformGridMesh->yinterval[0] + pUniformGridMesh->baseinterval * j + pUniformGridMesh->baseinterval * 0.5;

			pair.first = i;
			pair.second = j;

			//find which face it lies.
			int index = InDistrict(pUniformGridMesh,centerx,centery);
			if(index > -1)
			{
				pUniformGridMesh->m_ArrayUniformCells[index].push_back(pair);
			}
		}
}
void Action_PolyOmino::GetFacesMaximalandMinimalDensityPerCell(double &maximaldensityeachcell,double &minimaldensityeachcell)
{
	int minindex,maxindex;
	pUniformGridMesh->m_ArrayDistrictDensity.clear();
	for(int i = 0; i < pUniformGridMesh->m_ArrayUniformCells.size(); i++)
	{
		double value1 = pUniformGridMesh->m_ArrayTargetFaceArea[i] / pUniformGridMesh->m_ArrayUniformCells[i].size();
		pUniformGridMesh->m_ArrayDistrictDensity.push_back(value1);
		if(value1 > maximaldensityeachcell)
		{
			maximaldensityeachcell = value1;
			maxindex = i;
		}
		if(value1 < minimaldensityeachcell)
		{
			minimaldensityeachcell = value1;
			minindex = i;
		}
	}
}
void Action_PolyOmino::GetFacesDifferenceMaximalandMinimalDensityPerCell_Array(double &maximaldensityeachcell,double &minimaldensityeachcell,double &maximalstatepopulation,double &minimalstatepopulation)
{
	int minindex,maxindex;

	int statecells;
	double statetargetarea;

	int dimension = pUniformGridMesh->m_FaceAreaDifference.size();

	for(int i = 0; i < pUniformGridMesh->m_ArrayStates.size(); i++)
	{
		statecells = 0;
		for(int j = 0; j < pUniformGridMesh->m_ArrayStates[i].size(); j++)
		{
			statecells += pUniformGridMesh->m_ArrayUniformCells[pUniformGridMesh->m_ArrayStates[i][j]].size();
		}
		statetargetarea = fabs((double)pUniformGridMesh->m_FaceAreaDifference[i]);
		if(statetargetarea > maximalstatepopulation)
			maximalstatepopulation = statetargetarea;
		if(statetargetarea > minimalstatepopulation)
			minimalstatepopulation = statetargetarea;

		double value1 = statetargetarea / statecells;
		if(value1 > maximaldensityeachcell)
		{
			maximaldensityeachcell = value1;
			maxindex = i;
		}
		if(value1 < minimaldensityeachcell)
		{
			minimaldensityeachcell = value1;
			minindex = i;
		}
	}
}

void Action_PolyOmino::GetStateMaximalandMinimalDensityPerCell_Array(double &maximaldensityeachcell, double &minimaldensityeachcell, double &maximalstatepopulation, double &minimalstatepopulation)
{
	int minindex, maxindex;

	int statecells;
	double statetargetarea;
	//if (pUniformGridMesh->m_2DArrayTargetFaceArea.size() == 0)
	//	return;

	//int dimension = pUniformGridMesh->m_2DArrayTargetFaceArea[0].size();
	//for (int k = 0; k < dimension; k++)
	{
		for (int i = 0; i < pUniformGridMesh->m_ArrayStates.size(); i++)
		{
			statecells = 0;
			for (int j = 0; j < pUniformGridMesh->m_ArrayStates[i].size(); j++)
			{
				statecells += pUniformGridMesh->m_ArrayUniformCells[pUniformGridMesh->m_ArrayStates[i][j]].size();
			}
			statetargetarea = pUniformGridMesh->m_ArrayTargetFaceArea[i];

			if (i == 26 || i == 27 || i == 30)
				continue;

			if (statetargetarea > maximalstatepopulation)
				maximalstatepopulation = statetargetarea;
			if (statetargetarea < minimalstatepopulation)
				minimalstatepopulation = statetargetarea;

			double value1 = statetargetarea / statecells;
			if (value1 > maximaldensityeachcell)
			{
				maximaldensityeachcell = value1;
				maxindex = i;
			}
			if (value1 < minimaldensityeachcell)
			{
				minimaldensityeachcell = value1;
				minindex = i;
			}

		}
	}
}
void Action_PolyOmino::GetFacesMaximalandMinimalDensityPerCell_Array(double &maximaldensityeachcell,double &minimaldensityeachcell,double &maximalstatepopulation,double &minimalstatepopulation)
{
	int minindex,maxindex;

	int statecells;
	double statetargetarea;
	if(pUniformGridMesh->m_2DArrayTargetFaceArea.size() == 0)
		return;

	int dimension = pUniformGridMesh->m_2DArrayTargetFaceArea[0].size();
	for(int k = 0; k < dimension; k++)
	{
		for(int i = 0; i < pUniformGridMesh->m_ArrayStates.size(); i++)
		{
			statecells = 0;
			for(int j = 0; j < pUniformGridMesh->m_ArrayStates[i].size(); j++)
			{
				statecells += pUniformGridMesh->m_ArrayUniformCells[pUniformGridMesh->m_ArrayStates[i][j]].size();
			}
			statetargetarea = pUniformGridMesh->m_2DArrayTargetFaceArea[i][k];

			if(statetargetarea > maximalstatepopulation)
				maximalstatepopulation = statetargetarea;
			if(statetargetarea < minimalstatepopulation)
				minimalstatepopulation = statetargetarea;

			double value1 = statetargetarea / statecells;
			if(value1 > maximaldensityeachcell)
			{
				maximaldensityeachcell = value1;
				maxindex = i;
			}
			if(value1 < minimaldensityeachcell)
			{
				minimaldensityeachcell = value1;
				minindex = i;
			}

		}
	}
}
void Action_PolyOmino::PlotFaceCellsbySingleColorFromCenter(double maximaldensityeachcell)
{
	Point3D lower1,upper1,center,cellcenter;
	std::pair<int,double> cellpair;
	std::vector<std::pair<int,double>> cellpairarray;
	pUniformGridMesh->m_FillCells.resize(pUniformGridMesh->m_ArrayUniformCells.size());
	pUniformGridMesh->m_FillCellsColor.resize(pUniformGridMesh->m_ArrayUniformCells.size());

	for(int i = 0; i < pUniformGridMesh->m_ArrayUniformCells.size(); i++)
	{
		std::vector<std::pair<int,int>> cellarray = pUniformGridMesh->m_ArrayUniformCells[i];
		ComputeFaceboundingbox(pUniformGridMesh->m_ArrayFacePointsArray,i,lower1,upper1);
		cellpairarray.clear();
		center.setValue((lower1.x + upper1.x)/2,(lower1.y + upper1.y)/2,(lower1.z + upper1.z)/2);
		for(int j = 0; j < cellarray.size(); j++)
		{
			cellpair.first = j;
			cellcenter.setValue(pUniformGridMesh->xinterval[0] + cellarray[j].first * pUniformGridMesh->baseinterval + pUniformGridMesh->baseinterval / 2,
				pUniformGridMesh->yinterval[0] + cellarray[j].second * pUniformGridMesh->baseinterval + pUniformGridMesh->baseinterval / 2,0);

			cellpair.second = Length(cellcenter - center); 
			cellpairarray.push_back(cellpair);
		}

		struct sort_pred {
			bool operator()(const std::pair<int,double> &left, const std::pair<int,double> &right) {
				return left.second < right.second;
			}
		};
		//sort all the cells by the distance to the center.
		std::sort(cellpairarray.begin(),cellpairarray.end(),sort_pred());

		double num = pUniformGridMesh->m_ArrayTargetFaceArea[i] / maximaldensityeachcell;
		for(int j = 0; j < (int)(num); j++)
		{
			int index1 = cellpairarray[j].first;

			double x1 = pUniformGridMesh->xinterval[0] + cellarray[index1].first * pUniformGridMesh->baseinterval;
			double y1 = pUniformGridMesh->yinterval[0] + cellarray[index1].second * pUniformGridMesh->baseinterval;
			double x2 = x1 + pUniformGridMesh->baseinterval;
			double y2 = y1 + pUniformGridMesh->baseinterval;
			pUniformGridMesh->m_FillCells[i].push_back(x1);
			pUniformGridMesh->m_FillCells[i].push_back(y1);
			pUniformGridMesh->m_FillCells[i].push_back(x2);
			pUniformGridMesh->m_FillCells[i].push_back(y2);
			pUniformGridMesh->m_FillCellsColor[i].push_back(1.0);
			pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
			pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
		}
		double num1 = num - int(num);
		//num1 = num1 * 4;
		//for(int j = 0; j < num1; j++)
		if(num1 > 1e-6)
		{
			int index1 = cellpairarray[(int)num].first;

			double x1 = pUniformGridMesh->xinterval[0] + cellarray[index1].first * pUniformGridMesh->baseinterval;
			double y1 = pUniformGridMesh->yinterval[0] + cellarray[index1].second * pUniformGridMesh->baseinterval;
			if(num1 <= 0.375 && num1 >= 0.125)
			{
				double x2 = x1 + pUniformGridMesh->baseinterval * 0.5;
				double y2 = y1 + pUniformGridMesh->baseinterval * 0.5;
				pUniformGridMesh->m_FillCells[i].push_back(x1);
				pUniformGridMesh->m_FillCells[i].push_back(y1);
				pUniformGridMesh->m_FillCells[i].push_back(x2);
				pUniformGridMesh->m_FillCells[i].push_back(y2);
				pUniformGridMesh->m_FillCellsColor[i].push_back(1.0);
				pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
				pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
			}else
				if(num1 <= 0.625 && num1 > 0.375)
				{
					double x2 = x1 + pUniformGridMesh->baseinterval * 0.5;
					double y2 = y1 + pUniformGridMesh->baseinterval * 0.5;
					pUniformGridMesh->m_FillCells[i].push_back(x1);
					pUniformGridMesh->m_FillCells[i].push_back(y1);
					pUniformGridMesh->m_FillCells[i].push_back(x2);
					pUniformGridMesh->m_FillCells[i].push_back(y2);
					pUniformGridMesh->m_FillCellsColor[i].push_back(1.0);
					pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
					pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);

					x1 = pUniformGridMesh->xinterval[0] + cellarray[index1].first * pUniformGridMesh->baseinterval + pUniformGridMesh->baseinterval * 0.5;
					y1 = pUniformGridMesh->yinterval[0] + cellarray[index1].second * pUniformGridMesh->baseinterval; 
					x2 = x1 + pUniformGridMesh->baseinterval * 0.5;
					y2 = y1 + pUniformGridMesh->baseinterval * 0.5;
					pUniformGridMesh->m_FillCells[i].push_back(x1);
					pUniformGridMesh->m_FillCells[i].push_back(y1);
					pUniformGridMesh->m_FillCells[i].push_back(x2);
					pUniformGridMesh->m_FillCells[i].push_back(y2);
					pUniformGridMesh->m_FillCellsColor[i].push_back(1.0);
					pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
					pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
				}else 
					if(num1 > 0.625 && num1 < 0.875)
					{
						double x2 = x1 + pUniformGridMesh->baseinterval * 0.5;
						double y2 = y1 + pUniformGridMesh->baseinterval * 0.5;
						pUniformGridMesh->m_FillCells[i].push_back(x1);
						pUniformGridMesh->m_FillCells[i].push_back(y1);
						pUniformGridMesh->m_FillCells[i].push_back(x2);
						pUniformGridMesh->m_FillCells[i].push_back(y2);
						pUniformGridMesh->m_FillCellsColor[i].push_back(1.0);
						pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
						pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);

						x1 = pUniformGridMesh->xinterval[0] + cellarray[index1].first * pUniformGridMesh->baseinterval + pUniformGridMesh->baseinterval * 0.5;
						y1 = pUniformGridMesh->yinterval[0] + cellarray[index1].second * pUniformGridMesh->baseinterval; 
						x2 = x1 + pUniformGridMesh->baseinterval * 0.5;
						y2 = y1 + pUniformGridMesh->baseinterval * 0.5;
						pUniformGridMesh->m_FillCells[i].push_back(x1);
						pUniformGridMesh->m_FillCells[i].push_back(y1);
						pUniformGridMesh->m_FillCells[i].push_back(x2);
						pUniformGridMesh->m_FillCells[i].push_back(y2);
						pUniformGridMesh->m_FillCellsColor[i].push_back(1.0);
						pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
						pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);

						x1 = pUniformGridMesh->xinterval[0] + cellarray[index1].first * pUniformGridMesh->baseinterval;
						y1 = pUniformGridMesh->yinterval[0] + cellarray[index1].second * pUniformGridMesh->baseinterval + pUniformGridMesh->baseinterval * 0.5; 
						x2 = x1 + pUniformGridMesh->baseinterval * 0.5;
						y2 = y1 + pUniformGridMesh->baseinterval * 0.5;
						pUniformGridMesh->m_FillCells[i].push_back(x1);
						pUniformGridMesh->m_FillCells[i].push_back(y1);
						pUniformGridMesh->m_FillCells[i].push_back(x2);
						pUniformGridMesh->m_FillCells[i].push_back(y2);
						pUniformGridMesh->m_FillCellsColor[i].push_back(1.0);
						pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
						pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
					}else if(num1 > 0.875)
					{
						double x2 = x1 + pUniformGridMesh->baseinterval;
						double y2 = y1 + pUniformGridMesh->baseinterval;
						pUniformGridMesh->m_FillCells[i].push_back(x1);
						pUniformGridMesh->m_FillCells[i].push_back(y1);
						pUniformGridMesh->m_FillCells[i].push_back(x2);
						pUniformGridMesh->m_FillCells[i].push_back(y2);
						pUniformGridMesh->m_FillCellsColor[i].push_back(1.0);
						pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
						pUniformGridMesh->m_FillCellsColor[i].push_back(0.0);
					}
		}
	}
}
void Action_PolyOmino::PlotFaceCellsbyContinuousColor(double maximaldensityeachcell)
{
	//use color
	CImage m_TextureImage; 
	//colorbar
	//m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\inverse_hot_colorbar.PNG");
	pUniformGridMesh->m_ArrayStatesColor.resize(pUniformGridMesh->m_ArrayUniformCells.size());
	for(int i = 0; i < pUniformGridMesh->m_ArrayUniformCells.size(); i++)
	{
		double value1 = pUniformGridMesh->m_ArrayTargetFaceArea[i] / pUniformGridMesh->m_ArrayUniformCells[i].size();
		double value2 = value1 / maximaldensityeachcell;
		int x = value2 * m_TextureImage.GetWidth();
		if ( x == m_TextureImage.GetWidth())
			x--;
		COLORREF color = m_TextureImage.GetPixel(x,0);
		
		pUniformGridMesh->m_ArrayStatesColor[i].push_back(GetRValue(color) / 256.0);
		pUniformGridMesh->m_ArrayStatesColor[i].push_back(GetGValue(color) / 256.0);
		pUniformGridMesh->m_ArrayStatesColor[i].push_back(GetBValue(color) / 256.0);
	}
}
void Action_PolyOmino::PlotFaceCellsBySeveralColorsFromTopLeft(int colornum,double maximaldensityeachcell,double minimaldensityeachcell)
{
	//int colornum = 3;
	std::vector<double> colorarray;
	//colorarray.resize(colornum * 3,0);
	std::vector<double> densityarray;
	//densityarray.resize(colornum,0);

	CImage m_TextureImage; 
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	CImage m_GrayImage; 
	m_GrayImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\gray.PNG");
	//m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\inverse_hot_colorbar.PNG");
	double value_1 = maximaldensityeachcell;
	for(int i = 1; i < colornum; i++)
		value_1 = value_1 / 5;

	for(int i = 0; i < colornum; i++)
	{
		//densityarray.push_back(((double)i)  * (maximaldensityeachcell - minimaldensityeachcell)/ (colornum  - 1) + minimaldensityeachcell);
		densityarray.push_back(value_1);
		value_1 = value_1 * 5;
		double value2 = ((double)i) / (colornum - 1);
		int x = value2 * m_TextureImage.GetWidth();
		if ( x == m_TextureImage.GetWidth())
			x--;
		COLORREF color = m_TextureImage.GetPixel(x,0);
		colorarray.push_back(GetRValue(color) / 256.0);
		colorarray.push_back(GetGValue(color) / 256.0);
		colorarray.push_back(GetBValue(color) / 256.0);
	}

	Point3D lower1,upper1,center,cellcenter;
	std::pair<int,double> cellpair;
	std::vector<std::pair<int,double>> cellpairarray;

	//backgroundcolor
	std::vector<std::vector<double>> m_ArrayBackgroundColors;
	m_ArrayBackgroundColors.resize(colornum + 1);
	int minimalcolor = 250;
	int maximalcolor = 155;
	for(int i = 0; i <= colornum; i++)
	{
		int color = (minimalcolor * (colornum - i) + maximalcolor * i) / colornum;
		m_ArrayBackgroundColors[i].push_back(color/256.0);
		m_ArrayBackgroundColors[i].push_back(color/256.0);
		m_ArrayBackgroundColors[i].push_back(color/256.0);
	}

	//Resize 
	pUniformGridMesh->m_FillCells.resize(pUniformGridMesh->m_ArrayUniformCells.size());
	pUniformGridMesh->m_FillCellsColor.resize(pUniformGridMesh->m_ArrayUniformCells.size());
	pUniformGridMesh->m_ArrayStatesColor.resize(pUniformGridMesh->m_ArrayStates.size());

	double maximalstatepopulation = -1;
	for(int i = 0; i < pUniformGridMesh->m_ArrayStates.size(); i++)
	{
		double value11 = 0;
		std::vector<int> districtarray = pUniformGridMesh->m_ArrayStates[i];
		for(int j = 0; j < districtarray.size(); j++)
		{
			value11 += pUniformGridMesh->m_ArrayTargetFaceArea[districtarray[j]];
		}
		if(value11 > maximalstatepopulation)
			maximalstatepopulation = value11;
	}
	densityarray.push_back(maximalstatepopulation);

	for(int i = 0; i < pUniformGridMesh->m_ArrayStates.size(); i++)
	{
		std::vector<int> districtarray = pUniformGridMesh->m_ArrayStates[i];
		std::vector<std::pair<int,int>> statescellarray;
		for(int j = 0; j < districtarray.size(); j++)
			statescellarray.insert(statescellarray.begin(),pUniformGridMesh->m_ArrayUniformCells[districtarray[j]].begin(),
							pUniformGridMesh->m_ArrayUniformCells[districtarray[j]].end());

		struct sort_pred {
			bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) 
			{
				if(left.second < right.second)
					return FALSE;
				else if(left.second > right.second)
					return TRUE;
				else if(left.first < right.first)
					return TRUE;
				else
					return FALSE;
			}
		};
		//sort all the cells by the distance to the center.
		std::sort(statescellarray.begin(),statescellarray.end(),sort_pred());

		double statesTargetFaceArea = 0;
		for(int j = 0; j < districtarray.size(); j++)
			statesTargetFaceArea += pUniformGridMesh->m_ArrayTargetFaceArea[districtarray[j]];

		//double density = statesTargetFaceArea / statescellarray.size();

		//double value2 = density / maximaldensityeachcell;
		//double value2 = statesTargetFaceArea / maximalstatepopulation;
		//value2 = 1 - value2;
		//int x = value2 * m_GrayImage.GetWidth() * 0.5 + m_GrayImage.GetWidth() * 0.5;
		//if ( x == m_GrayImage.GetWidth())
		//	x--;
		//COLORREF color = m_GrayImage.GetPixel(x,0);
		//pUniformGridMesh->m_ArrayStatesColor[i].push_back(GetRValue(color) / 256.0);
		//pUniformGridMesh->m_ArrayStatesColor[i].push_back(GetGValue(color) / 256.0);
		//pUniformGridMesh->m_ArrayStatesColor[i].push_back(GetBValue(color) / 256.0);

		for(int kk = colornum - 1; kk >= 0; kk--)
		{
			if(statesTargetFaceArea > densityarray[kk])
			{
				double ratio = (statesTargetFaceArea - densityarray[kk]) / (densityarray[kk + 1] - densityarray[kk]);
				double r = m_ArrayBackgroundColors[kk][0] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][0] * ratio;
				double g = m_ArrayBackgroundColors[kk][1] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][1] * ratio;
				double b = m_ArrayBackgroundColors[kk][2] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][2] * ratio;

				pUniformGridMesh->m_ArrayStatesColor[i].push_back(r);
				pUniformGridMesh->m_ArrayStatesColor[i].push_back(g);
				pUniformGridMesh->m_ArrayStatesColor[i].push_back(b);
				break;
			}
		}

		int currentnum = 0;
		double value1 = statesTargetFaceArea;
		for(int kk = colornum - 1; kk >= 0; kk--)
		{
			int num = value1 / densityarray[kk];
			value1 = value1 - densityarray[kk] * num;

			for(int jj = 0; jj < num; jj++)
			{
				//int index1 = jj;
				if(currentnum >= statescellarray.size())
					break;

				double x1 = pUniformGridMesh->xinterval[0] + statescellarray[currentnum].first * pUniformGridMesh->baseinterval;
				double y1 = pUniformGridMesh->yinterval[0] + statescellarray[currentnum].second * pUniformGridMesh->baseinterval;
				double x2 = x1 + pUniformGridMesh->baseinterval;
				double y2 = y1 + pUniformGridMesh->baseinterval;
				pUniformGridMesh->m_FillCells[i].push_back(x1);
				pUniformGridMesh->m_FillCells[i].push_back(y1);
				pUniformGridMesh->m_FillCells[i].push_back(x2);
				pUniformGridMesh->m_FillCells[i].push_back(y2);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 1]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 2]);
				currentnum++;
			}
			//
		}
	}
	//sort the face cells
	/*for(int i = 0; i < pUniformGridMesh->m_ArrayUniformCells.size(); i++)
	{
		std::vector<std::pair<int,int>> cellarray = pUniformGridMesh->m_ArrayUniformCells[i];

		struct sort_pred {
			bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) 
			{
				if(left.second < right.second)
					return FALSE;
				else if(left.second > right.second)
					return TRUE;
				else if(left.first < right.first)
					return TRUE;
				else
					return FALSE;
			}
		};
		//sort all the cells by the distance to the center.
		std::sort(cellarray.begin(),cellarray.end(),sort_pred());

		//int num = pUniformGridMesh->m_ArrayTargetFaceArea[i] / maximaldensityeachcell + 0.5;
		
		//density
		double density = pUniformGridMesh->m_ArrayTargetFaceArea[i] / cellarray.size();

		double value2 = density / maximaldensityeachcell;
		int x = value2 * m_GrayImage.GetWidth();
		if ( x == m_GrayImage.GetWidth())
			x--;
		COLORREF color = m_GrayImage.GetPixel(x,0);
		pUniformGridMesh->m_ArrayDistrictColor[i].push_back(GetRValue(color) / 256.0);
		pUniformGridMesh->m_ArrayDistrictColor[i].push_back(GetGValue(color) / 256.0);
		pUniformGridMesh->m_ArrayDistrictColor[i].push_back(GetBValue(color) / 256.0);
		int j;*/
		//double TOL = 1e-8;
		/*for(j = 0; j < colornum; j++)
		{
			if(density < densityarray[j] + TOL)
				break;
			if(j == colornum - 1)
				break;
		}*/

		/*double value1 = pUniformGridMesh->m_ArrayTargetFaceArea[i];
		int currentnum = 0;
		for(int kk = colornum - 1; kk >= 0; kk--)
		{
			int num = value1 / densityarray[kk];
			value1 = value1 - densityarray[kk] * num;

			for(int jj = 0; jj < num; jj++)
			{
				//int index1 = jj;
				if(currentnum >= cellarray.size())
					break;

				double x1 = pUniformGridMesh->xinterval[0] + cellarray[currentnum].first * pUniformGridMesh->baseinterval;
				double y1 = pUniformGridMesh->yinterval[0] + cellarray[currentnum].second * pUniformGridMesh->baseinterval;
				double x2 = x1 + pUniformGridMesh->baseinterval;
				double y2 = y1 + pUniformGridMesh->baseinterval;
				pUniformGridMesh->m_FillCells[i].push_back(x1);
				pUniformGridMesh->m_FillCells[i].push_back(y1);
				pUniformGridMesh->m_FillCells[i].push_back(x2);
				pUniformGridMesh->m_FillCells[i].push_back(y2);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 1]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 2]);
				currentnum++;
			}
			//
		}

	}*/
}
void Action_PolyOmino::PlotFaceCellsBySeveralColorsFromTopLeft_Array(int index,int colornum,double maximaldensityeachcell,double minimaldensityeachcell,double &maximalstatepopulation,double &minimalstatepopulation)
{
	//int colornum = 3;
	std::vector<double> colorarray;
	//colorarray.resize(colornum * 3,0);
	std::vector<double> densityarray;
	//densityarray.resize(colornum,0);

	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	CImage m_GrayImage;
	m_GrayImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\gray.PNG");
	//m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\inverse_hot_colorbar.PNG");
	double value_1 = maximaldensityeachcell;
	for(int i = 1; i < colornum; i++)
		value_1 = value_1 / 5;

	for(int i = 0; i < colornum; i++)
	{
		//densityarray.push_back(((double)i)  * (maximaldensityeachcell - minimaldensityeachcell)/ (colornum  - 1) + minimaldensityeachcell);
		densityarray.push_back(value_1);
		value_1 = value_1 * 5;
		double value2 = ((double)i) / (colornum - 1);
		int x = value2 * m_TextureImage.GetWidth();
		if ( x == m_TextureImage.GetWidth())
			x--;
		COLORREF color = m_TextureImage.GetPixel(x,0);
		colorarray.push_back(GetRValue(color) / 256.0);
		colorarray.push_back(GetGValue(color) / 256.0);
		colorarray.push_back(GetBValue(color) / 256.0);
	}

	Point3D lower1,upper1,center,cellcenter;
	std::pair<int,double> cellpair;
	std::vector<std::pair<int,double>> cellpairarray;

	//backgroundcolor
	std::vector<std::vector<double>> m_ArrayBackgroundColors;
	m_ArrayBackgroundColors.resize(colornum + 1);
	//int minimalcolor = 250;
	//int maximalcolor = 155;
	int minimalcolor = 255;
	int maximalcolor = 0;
	for(int i = 0; i <= colornum; i++)
	{
		int color = (minimalcolor * (colornum - i) + maximalcolor * i) / colornum;
		m_ArrayBackgroundColors[i].push_back(color/256.0);
		m_ArrayBackgroundColors[i].push_back(color/256.0);
		m_ArrayBackgroundColors[i].push_back(color/256.0);
	}

	//Resize 
	pUniformGridMesh->m_FillCells.resize(pUniformGridMesh->m_ArrayUniformCells.size());
	pUniformGridMesh->m_FillCellsColor.resize(pUniformGridMesh->m_ArrayUniformCells.size());
	pUniformGridMesh->m_ArrayStatesColor.resize(pUniformGridMesh->m_ArrayStates.size());
	pUniformGridMesh->m_FillCellColorIndex.resize(pUniformGridMesh->m_ArrayUniformCells.size());
	/*double maximalstatepopulation = -1;
	double minimalstatepopulation = 1e10;
	for(int i = 0; i < pUniformGridMesh->m_ArrayStates.size(); i++)
	{
		double value11 = 0;
		value11 = pUniformGridMesh->m_2DArrayTargetFaceArea[i][index];
		if(value11 > maximalstatepopulation)
			maximalstatepopulation = value11;
		if(value11 < minimalstatepopulation)
			minimalstatepopulation = value11;
	}*/
	//densityarray[0] = minimalstatepopulation;
	densityarray.push_back(maximalstatepopulation);
	//densityarray.clear();
	//densityarray.push_back(239056.27999999997);
	//densityarray.push_back(1195281.3999999999);
	//densityarray.push_back(5976407.0000000000);
	//densityarray.push_back(37336011.000000000);
	for(int i = 0; i < pUniformGridMesh->m_ArrayStates.size(); i++)
	{
		std::vector<int> districtarray = pUniformGridMesh->m_ArrayStates[i];
		std::vector<std::pair<int,int>> statescellarray;
		for (int j = 0; j < districtarray.size(); j++)
		{
			if (districtarray[j] == 59 || districtarray[j] == 63)
				continue;
			statescellarray.insert(statescellarray.begin(), pUniformGridMesh->m_ArrayUniformCells[districtarray[j]].begin(),
				pUniformGridMesh->m_ArrayUniformCells[districtarray[j]].end());
		}


		struct sort_pred {
			bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) 
			{
				if(left.second < right.second)
					return FALSE;
				else if(left.second > right.second)
					return TRUE;
				else if(left.first < right.first)
					return TRUE;
				else
					return FALSE;
			}
		};
		//sort all the cells by the distance to the center.
		std::sort(statescellarray.begin(),statescellarray.end(),sort_pred());

		double statesTargetFaceArea = pUniformGridMesh->m_2DArrayTargetFaceArea[i][index];
		pUniformGridMesh->m_ArrayStateRatio.push_back((statesTargetFaceArea - minimalstatepopulation) / (maximalstatepopulation - minimalstatepopulation));
		//for(int j = 0; j < districtarray.size(); j++)
		//	statesTargetFaceArea += pUniformGridMesh->m_2DArrayTargetFaceArea[districtarray[j]][index];

		//double density = statesTargetFaceArea / statescellarray.size();

		//double value2 = density / maximaldensityeachcell;
		//double value2 = statesTargetFaceArea / maximalstatepopulation;
		//value2 = 1 - value2;
		//int x = value2 * m_GrayImage.GetWidth() * 0.5 + m_GrayImage.GetWidth() * 0.5;
		//if ( x == m_GrayImage.GetWidth())
		//	x--;
		//COLORREF color = m_GrayImage.GetPixel(x,0);
		//pUniformGridMesh->m_ArrayStatesColor[i].push_back(GetRValue(color) / 256.0);
		//pUniformGridMesh->m_ArrayStatesColor[i].push_back(GetGValue(color) / 256.0);
		//pUniformGridMesh->m_ArrayStatesColor[i].push_back(GetBValue(color) / 256.0);

		for(int kk = colornum - 1; kk >= 0; kk--)
		{
			if(statesTargetFaceArea >= densityarray[kk])
			{
				double ratio = (statesTargetFaceArea - densityarray[kk]) / (densityarray[kk + 1] - densityarray[kk]);
				double r = m_ArrayBackgroundColors[kk][0] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][0] * ratio;
				double g = m_ArrayBackgroundColors[kk][1] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][1] * ratio;
				double b = m_ArrayBackgroundColors[kk][2] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][2] * ratio;

				pUniformGridMesh->m_ArrayStatesColor[i].push_back(r);
				pUniformGridMesh->m_ArrayStatesColor[i].push_back(g);
				pUniformGridMesh->m_ArrayStatesColor[i].push_back(b);
				break;
			}
		}
		if (pUniformGridMesh->m_ArrayStatesColor[i].size() == 0)
		{
			//double ratio = (statesTargetFaceArea - densityarray[kk]) / (densityarray[kk + 1] - densityarray[kk]);
			//double r = m_ArrayBackgroundColors[kk][0] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][0] * ratio;
			//double g = m_ArrayBackgroundColors[kk][1] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][1] * ratio;
			//double b = m_ArrayBackgroundColors[kk][2] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][2] * ratio;

			pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
			pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
			pUniformGridMesh->m_ArrayStatesColor[i].push_back(1);
		}

		int currentnum = 0;
		double value1 = statesTargetFaceArea;
		for(int kk = colornum - 1; kk >= 0; kk--)
		{
			int num = value1 / densityarray[kk];
			value1 = value1 - densityarray[kk] * num;

			for(int jj = 0; jj < num; jj++)
			{
				//int index1 = jj;
				if(currentnum >= statescellarray.size())
					break;

				double x1 = pUniformGridMesh->xinterval[0] + statescellarray[currentnum].first * pUniformGridMesh->baseinterval;
				double y1 = pUniformGridMesh->yinterval[0] + statescellarray[currentnum].second * pUniformGridMesh->baseinterval;
				double x2 = x1 + pUniformGridMesh->baseinterval;
				double y2 = y1 + pUniformGridMesh->baseinterval;
				pUniformGridMesh->m_FillCells[i].push_back(x1);
				pUniformGridMesh->m_FillCells[i].push_back(y1);
				pUniformGridMesh->m_FillCells[i].push_back(x2);
				pUniformGridMesh->m_FillCells[i].push_back(y2);
				pUniformGridMesh->m_FillCellColorIndex[i].push_back(kk);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 1]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 2]);
				//rectangle
				pUniformGridMesh->m_FillCellStyle.push_back(0);
				currentnum++;
			}
			//
		}
	}
	//sort the face cells
	/*for(int i = 0; i < pUniformGridMesh->m_ArrayUniformCells.size(); i++)
	{
		std::vector<std::pair<int,int>> cellarray = pUniformGridMesh->m_ArrayUniformCells[i];

		struct sort_pred {
			bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) 
			{
				if(left.second < right.second)
					return FALSE;
				else if(left.second > right.second)
					return TRUE;
				else if(left.first < right.first)
					return TRUE;
				else
					return FALSE;
			}
		};
		//sort all the cells by the distance to the center.
		std::sort(cellarray.begin(),cellarray.end(),sort_pred());

		//int num = pUniformGridMesh->m_ArrayTargetFaceArea[i] / maximaldensityeachcell + 0.5;
		
		//density
		double density = pUniformGridMesh->m_ArrayTargetFaceArea[i] / cellarray.size();

		double value2 = density / maximaldensityeachcell;
		int x = value2 * m_GrayImage.GetWidth();
		if ( x == m_GrayImage.GetWidth())
			x--;
		COLORREF color = m_GrayImage.GetPixel(x,0);
		pUniformGridMesh->m_ArrayDistrictColor[i].push_back(GetRValue(color) / 256.0);
		pUniformGridMesh->m_ArrayDistrictColor[i].push_back(GetGValue(color) / 256.0);
		pUniformGridMesh->m_ArrayDistrictColor[i].push_back(GetBValue(color) / 256.0);
		int j;*/
		//double TOL = 1e-8;
		/*for(j = 0; j < colornum; j++)
		{
			if(density < densityarray[j] + TOL)
				break;
			if(j == colornum - 1)
				break;
		}*/

		/*double value1 = pUniformGridMesh->m_ArrayTargetFaceArea[i];
		int currentnum = 0;
		for(int kk = colornum - 1; kk >= 0; kk--)
		{
			int num = value1 / densityarray[kk];
			value1 = value1 - densityarray[kk] * num;

			for(int jj = 0; jj < num; jj++)
			{
				//int index1 = jj;
				if(currentnum >= cellarray.size())
					break;

				double x1 = pUniformGridMesh->xinterval[0] + cellarray[currentnum].first * pUniformGridMesh->baseinterval;
				double y1 = pUniformGridMesh->yinterval[0] + cellarray[currentnum].second * pUniformGridMesh->baseinterval;
				double x2 = x1 + pUniformGridMesh->baseinterval;
				double y2 = y1 + pUniformGridMesh->baseinterval;
				pUniformGridMesh->m_FillCells[i].push_back(x1);
				pUniformGridMesh->m_FillCells[i].push_back(y1);
				pUniformGridMesh->m_FillCells[i].push_back(x2);
				pUniformGridMesh->m_FillCells[i].push_back(y2);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 1]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 2]);
				currentnum++;
			}
			//
		}

	}*/
}
void Action_PolyOmino::LoadStateName(CString FileName)
{
	std::ifstream file(FileName);
	char message[100];
	pUniformGridMesh->m_ArrayDistrictName.clear();
	while(!file.eof())
	{
		file.getline(message,100);
		//file >> message;
		CString statename = message;
		pUniformGridMesh->m_ArrayDistrictName.push_back(statename);
	}
	file.close();
}
void Action_PolyOmino::OnBnClickedOminoUniformgrid()
{
	// TODO: Add your control notification handler code here
	int iterationnum;
	LoadData1("D:\\papers\\code\\data\\Geo\\dense\\1788.m",iterationnum);

	//LoadData1("D:\\papers\\code\\data\\Geo\\dense\\1800.m",iterationnum);
	//LoadData1("D:\\papers\\code\\data\\Geo\\dense\\1724.m",iterationnum);

	//move the horizontal and vertical lines such that the intervals are the 
	//integer times of the minimal interval distance.
	AdjustHorizontalandVerticalGroupofLinePositions();

	//Each four elements form one group

	std::vector<int> m_ArrayFacePoints,m_ArrayFaceEdges;

	//Trace Faces
	TraceFaces(m_ArrayFacePoints,m_ArrayFaceEdges);

	//Remove the outer face
	RemoveOuterFace(pUniformGridMesh->m_ArrayFacePointsArray,pUniformGridMesh->m_ArrayFaceEdgesArray);
	
	//
	ComputeFacesFourElementArray(m_ArrayFacePoints,m_ArrayFaceEdges);

	//
	AssignUniformCellstoFaces();
	
	//pUniformGridMesh->m_ArrayTargetFaceArea.resize(pUniformGridMesh->m_ArrayFaceAreaArray.size());
	p_FormView3->OutPutMeshPolyOminoEdges_PovRay(pUniformGridMesh,"D:\\polyominomesh.inc");

	LoadTargetFaceArea("D:\\papers\\code\\data\\Geo\\dense\\1788_usapopulation.txt");
	
	//Get the maximal density for each grid cell
	double maximaldensityeachcell = -1;
	double minimaldensityeachcell = 1e10;
	double maximalstatepopulation = -1;
	double minimalstatepopulation = 1e10;

	//
	SetStatesArray();

	//
	LoadStateName("D:\\papers\\code\\data\\Geo\\dense\\1788_districtname.txt");
	//LoadStateName("D:\\papers\\code\\data\\Geo\\dense\\1788_districtname_abbre.txt");
	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usapopulation_2010_2014.txt");
	LoadTargetFaceAreaArray(1);
	//LoadTargetFaceAreaArray_facebookpenetration("D:\\papers\\code\\data\\Geo\\dense\\1788_facebookpenetration.txt");

	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usa_completed_highschool.txt");

	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usa_familyincome.txt");

	//LoadTargetFaceAreaArray("D:\\papers\\code\\data\\Geo\\dense\\1788_usa_median_household_income.txt");

	//
	ComputetheDifference(0,24);

	//
	//GetFacesDifferenceMaximalandMinimalDensityPerCell_Array(maximaldensityeachcell,minimaldensityeachcell,maximalstatepopulation,minimalstatepopulation);
	//

	//PlotFaceCellDifference
	//PlotFaceCellByDifference_Array(3,maximaldensityeachcell,minimaldensityeachcell,maximalstatepopulation,minimalstatepopulation);

	//
	GetFacesMaximalandMinimalDensityPerCell_Array(maximaldensityeachcell,minimaldensityeachcell,maximalstatepopulation,minimalstatepopulation);

	//PlotFaceCellsBySeveralColorsFromTopLeft(3,maximaldensityeachcell,minimaldensityeachcell);
	//2014 24 
	//2000 10
	PlotFaceCellsBySeveralColorsFromTopLeft_Array(1,3,maximaldensityeachcell,minimaldensityeachcell,maximalstatepopulation,minimalstatepopulation);

	//colorbar
	//PlotFaceCellsbyContinuousColor(maximaldensityeachcell);
	LoadStateName("D:\\papers\\code\\data\\Geo\\dense\\1788_districtname_abbre.txt");
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::ComputetheDifference(int index1,int index2)
{
	int size = pUniformGridMesh->m_2DArrayTargetFaceArea.size();
	pUniformGridMesh->m_FaceAreaDifference.resize(size);
	for(int i = 0; i < size; i++)
	{
		int value1 = pUniformGridMesh->m_2DArrayTargetFaceArea[i][index2] - pUniformGridMesh->m_2DArrayTargetFaceArea[i][index1];
		pUniformGridMesh->m_FaceAreaDifference[i] = value1;
	}
}
void Action_PolyOmino::PlotFaceCellByDifference_Array(int colornum,double maximaldensityeachcell,double minimaldensityeachcell,double maximalstatepopulation,double minimalstatepopulation)
{
	//int colornum = 3;
	std::vector<double> colorarray;
	//colorarray.resize(colornum * 3,0);
	std::vector<double> densityarray;
	//densityarray.resize(colornum,0);

	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	CImage m_GrayImage;
	m_GrayImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\gray.PNG");
	//m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\inverse_hot_colorbar.PNG");
	double value_1 = maximaldensityeachcell;
	for(int i = 1; i < colornum; i++)
		value_1 = value_1 / 5;

	for(int i = 0; i < colornum; i++)
	{
		//densityarray.push_back(((double)i)  * (maximaldensityeachcell - minimaldensityeachcell)/ (colornum  - 1) + minimaldensityeachcell);
		densityarray.push_back(value_1);
		value_1 = value_1 * 5;
		double value2 = ((double)i) / (colornum - 1);
		int x = value2 * m_TextureImage.GetWidth();
		if ( x == m_TextureImage.GetWidth())
			x--;
		COLORREF color = m_TextureImage.GetPixel(x,0);
		colorarray.push_back(GetRValue(color) / 256.0);
		colorarray.push_back(GetGValue(color) / 256.0);
		colorarray.push_back(GetBValue(color) / 256.0);
	}

	Point3D lower1,upper1,center,cellcenter;
	std::pair<int,double> cellpair;
	std::vector<std::pair<int,double>> cellpairarray;

	//backgroundcolor
	std::vector<std::vector<double>> m_ArrayBackgroundColors;
	m_ArrayBackgroundColors.resize(colornum + 1);
	int minimalcolor = 256;
	int maximalcolor = 0;
	for(int i = 0; i <= colornum; i++)
	{
		int color = (minimalcolor * (colornum - i) + maximalcolor * i) / colornum;
		m_ArrayBackgroundColors[i].push_back(color/256.0);
		m_ArrayBackgroundColors[i].push_back(color/256.0);
		m_ArrayBackgroundColors[i].push_back(color/256.0);
	}

	//Resize 
	pUniformGridMesh->m_FillCells.resize(pUniformGridMesh->m_ArrayUniformCells.size());
	pUniformGridMesh->m_FillCellsColor.resize(pUniformGridMesh->m_ArrayUniformCells.size());
	pUniformGridMesh->m_ArrayStatesColor.resize(pUniformGridMesh->m_ArrayStates.size());
	pUniformGridMesh->m_FillCellColorIndex.resize(pUniformGridMesh->m_ArrayUniformCells.size());

	double maximalstatepopulation1 = -1;
	for(int i = 0; i < pUniformGridMesh->m_ArrayStates.size(); i++)
	{
		double value11 = 0;
		value11 = fabs((double)pUniformGridMesh->m_FaceAreaDifference[i]);
		if(value11 > maximalstatepopulation1)
			maximalstatepopulation1 = value11;
	}
	//densityarray[0] = minimalstatepopulation;
	densityarray.push_back(maximalstatepopulation1);
	//densityarray.clear();
	//densityarray.push_back(239056.27999999997);
	//densityarray.push_back(1195281.3999999999);
	//densityarray.push_back(5976407.0000000000);
	//densityarray.push_back(37336011.000000000);
	for(int i = 0; i < pUniformGridMesh->m_ArrayStates.size(); i++)
	{
		std::vector<int> districtarray = pUniformGridMesh->m_ArrayStates[i];
		std::vector<std::pair<int,int>> statescellarray;
		for(int j = 0; j < districtarray.size(); j++)
			statescellarray.insert(statescellarray.begin(),pUniformGridMesh->m_ArrayUniformCells[districtarray[j]].begin(),
							pUniformGridMesh->m_ArrayUniformCells[districtarray[j]].end());

		struct sort_pred {
			bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) 
			{
				if(left.second < right.second)
					return FALSE;
				else if(left.second > right.second)
					return TRUE;
				else if(left.first < right.first)
					return TRUE;
				else
					return FALSE;
			}
		};
		//sort all the cells by the distance to the center.
		std::sort(statescellarray.begin(),statescellarray.end(),sort_pred());

		double statesTargetFaceArea = fabs((double)pUniformGridMesh->m_FaceAreaDifference[i]);
		int sign1 = -1;
		if(pUniformGridMesh->m_FaceAreaDifference[i] > 0)
			sign1 = 1;

		for(int kk = colornum - 1; kk >= 0; kk--)
		{
			if(statesTargetFaceArea > densityarray[kk])
			{
				double ratio = (statesTargetFaceArea - densityarray[kk]) / (densityarray[kk + 1] - densityarray[kk]);
				double r = m_ArrayBackgroundColors[kk][0] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][0] * ratio;
				double g = m_ArrayBackgroundColors[kk][1] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][1] * ratio;
				double b = m_ArrayBackgroundColors[kk][2] * (1 - ratio) + m_ArrayBackgroundColors[kk + 1][2] * ratio;

				pUniformGridMesh->m_ArrayStatesColor[i].push_back(r);
				pUniformGridMesh->m_ArrayStatesColor[i].push_back(g);
				pUniformGridMesh->m_ArrayStatesColor[i].push_back(b);
				break;
			}
		}

		int currentnum = 0;
		double value1 = statesTargetFaceArea;
		for(int kk = colornum - 1; kk >= 0; kk--)
		{
			int num = value1 / densityarray[kk];
			value1 = value1 - densityarray[kk] * num;

			for(int jj = 0; jj < num; jj++)
			{
				//int index1 = jj;
				if(currentnum >= statescellarray.size())
					break;

				double x1 = pUniformGridMesh->xinterval[0] + statescellarray[currentnum].first * pUniformGridMesh->baseinterval;
				double y1 = pUniformGridMesh->yinterval[0] + statescellarray[currentnum].second * pUniformGridMesh->baseinterval;
				double x2 = x1 + pUniformGridMesh->baseinterval;
				double y2 = y1 + pUniformGridMesh->baseinterval;
				pUniformGridMesh->m_FillCells[i].push_back(x1);
				pUniformGridMesh->m_FillCells[i].push_back(y1);
				pUniformGridMesh->m_FillCells[i].push_back(x2);
				pUniformGridMesh->m_FillCells[i].push_back(y2);
				pUniformGridMesh->m_FillCellColorIndex[i].push_back(kk);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 1]);
				pUniformGridMesh->m_FillCellsColor[i].push_back(colorarray[kk * 3 + 2]);
				if(sign1 == 1)
					pUniformGridMesh->m_FillCellStyle.push_back(1);
				else
				if(sign1 == -1)
					pUniformGridMesh->m_FillCellStyle.push_back(1);			
				currentnum++;
			}
			//
		}
	}
}
void Action_PolyOmino::SetStatesArray()
{
	pUniformGridMesh->m_ArrayStates.resize(pUniformGridMesh->m_ArrayFaceAreaArray.size() - 1);

	for(int i = 0; i <= 29; i++)
	{
		pUniformGridMesh->m_ArrayStates[i].push_back(i);
	}
	pUniformGridMesh->m_ArrayStates[29].push_back(31);
	pUniformGridMesh->m_ArrayStates[30].push_back(30);
	for(int i = 32; i < pUniformGridMesh->m_ArrayFaceAreaArray.size(); i++)
		pUniformGridMesh->m_ArrayStates[i - 1].push_back(i);
	//29 and 31 For michigan
	//pUniformGridMesh->m_ArrayUniformCells[29].insert(pUniformGridMesh->m_ArrayUniformCells[29].begin(),
	//	pUniformGridMesh->m_ArrayUniformCells[31].begin(),pUniformGridMesh->m_ArrayUniformCells[31].end());
	//pUniformGridMesh->m_ArrayUniformCells.erase(pUniformGridMesh->m_ArrayUniformCells.begin() + 31);
	//pUniformGridMesh->m_ArrayDistrictDensity.erase(pUniformGridMesh->m_ArrayDistrictDensity.begin() + 31);
	//Draw 
	//PlotFaceCellsbySingleColorFromCenter(maximaldensityeachcell);
}
int Action_PolyOmino::InDistrict(CMesh3d* pMesh,double x,double y)
{
	int size = pMesh->m_ArrayFacePointsArray.size();
	int flag = 0;
	int intersectionnum = 0;
	for(int i = 0; i < size; i++)
	{
		int size1 = pMesh->m_ArrayFacePointsArray[i].size();
		flag = 0;
		intersectionnum = 0;
		for(int j = 0; j < size1; j++)
		{
			int index1 = pMesh->m_ArrayFacePointsArray[i][j];
			int index2 = pMesh->m_ArrayFacePointsArray[i][(j + 1) % size1];

			CVertex3d* pVertex1 = pMesh->GetVertex(index1);
			CVertex3d* pVertex2 = pMesh->GetVertex(index2);

			double x1 = pVertex1->x();
			double y1 = pVertex1->y();
			double z1 = pVertex1->z();
			double x2 = pVertex2->x();
			double y2 = pVertex2->y();
			double z2 = pVertex2->z();

			if(fabs(x1 - x2) < 1e-8)
			{
				if(x1 > x)
					if(y >= y1 && y <= y2 || y >= y2 && y <= y1)
						intersectionnum++;
			}
		}
		if(intersectionnum % 2 == 1)
		{
			return i;
		}
	}
	return -1;
}
void Action_PolyOmino::OnBnClickedOminoJianzhi()
{
	// TODO: Add your control notification handler code here
	//find the continuous horizontal and vertical edges and number them
	std::vector<std::vector<int>> m_HorizontalSegmentsbak = m_HorizontalSegments;
	std::vector<std::vector<int>> m_VerticalSegmentsbak = m_VerticalSegments;
	std::vector<int> edgeset1;
	for(int i = 0; i < m_HorizontalSegmentsbak.size(); i++)
	{
		std::vector<int> edgeset = m_HorizontalSegmentsbak[i];
		while(edgeset.size() > 0)
		{
			edgeset1.clear();
			edgeset1.push_back(edgeset[0]);
			edgeset.erase(edgeset.begin());
			GetConnectedEdgeSet(edgeset,edgeset1);
			m_HorizontalContinuousSegments.push_back(edgeset1);
		}
	}
	for(int i = 0; i < m_VerticalSegmentsbak.size(); i++)
	{
		std::vector<int> edgeset = m_VerticalSegmentsbak[i];
		while(edgeset.size() > 0)
		{
			edgeset1.clear();
			edgeset1.push_back(edgeset[0]);
			edgeset.erase(edgeset.begin());
			GetConnectedEdgeSet(edgeset,edgeset1);
			m_VerticalContinuousSegments.push_back(edgeset1);
		}
	}

	//Get the horizontal edge length
	double horizontallen = 0;
	int startvindex,endvindex;
	for(int i = 0; i < m_HorizontalContinuousSegments.size(); i++)
		horizontallen += GetConnectedEdgeEndPoints(m_HorizontalContinuousSegments[i],startvindex,endvindex);
	double verticallen = 0;
	for(int i = 0; i < m_VerticalContinuousSegments.size(); i++)
		verticallen += GetConnectedEdgeEndPoints(m_VerticalContinuousSegments[i],startvindex,endvindex);

	double paperwidth = 525; //mm
	double paperheight = 378; //mm

	//50mm height
	paperwidth = paperwidth * 6;

	if(horizontallen > verticallen)
	{
		horizontalscaleratio = paperwidth / horizontallen;
		verticalscaleratio = paperwidth / horizontallen;
	}else
	{
		horizontalscaleratio = paperwidth / verticallen;
		verticalscaleratio = paperwidth / verticallen;
	}
}
double Action_PolyOmino::GetConnectedEdgeEndPoints(std::vector<int> &edgeset,int &startvindex,int &endvindex)
{
	std::vector<int> endpoints;
	for(int i = 0; i < edgeset.size(); i++)
	{
		int edgeindex = edgeset[i];
		std::pair<int,int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
		std::vector<int>::iterator viterator = std::find(endpoints.begin(),endpoints.end(),pair.first);
		if(viterator != endpoints.end())
		{
			endpoints.erase(viterator);
		}else
		{
			endpoints.push_back(pair.first);
		}
		viterator = std::find(endpoints.begin(),endpoints.end(),pair.second);
		if(viterator != endpoints.end())
		{
			endpoints.erase(viterator);
		}else
		{
			endpoints.push_back(pair.second);
		}
	}
	startvindex = endpoints[0];
	endvindex = endpoints[1];
	CVertex3d* pStartVertex = pUniformGridMesh->m_ArrayVertex[startvindex];
	CVertex3d* pEndVertex = pUniformGridMesh->m_ArrayVertex[endvindex];

	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double z1 = pStartVertex->z();

	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	double z2 = pEndVertex->z();

	double len = sqrt(SQR(x1 - x2) + SQR(y1 - y2) + SQR(z1 - z2));
	return len;
}
void Action_PolyOmino::GetSplitEdgeEndPoints_JianZhi(std::vector<int> &edgeset,std::vector<double> &splitposition)
{
	int startvindex,endvindex;
	std::vector<int> endpoints;
	for(int i = 0; i < edgeset.size(); i++)
	{
		int edgeindex = edgeset[i];
		std::pair<int,int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
		std::vector<int>::iterator viterator = std::find(endpoints.begin(),endpoints.end(),pair.first);
		if(viterator != endpoints.end())
		{
			endpoints.erase(viterator);
		}else
		{
			endpoints.push_back(pair.first);
		}
		viterator = std::find(endpoints.begin(),endpoints.end(),pair.second);
		if(viterator != endpoints.end())
		{
			endpoints.erase(viterator);
		}else
		{
			endpoints.push_back(pair.second);
		}
	}
	startvindex = endpoints[0];
	endvindex = endpoints[1];

	CVertex3d* pStartVertex = pUniformGridMesh->m_ArrayVertex[startvindex];
	CVertex3d* pEndVertex = pUniformGridMesh->m_ArrayVertex[endvindex];

	endpoints.clear();
	for(int i = 0; i < edgeset.size(); i++)
	{
		int edgeindex = edgeset[i];
		std::pair<int,int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];

		std::vector<int>::iterator viterator = std::find(endpoints.begin(),endpoints.end(),pair.first);
		if(viterator != endpoints.end())
		{
		}else
		{
			endpoints.push_back(pair.first);
		}
		viterator = std::find(endpoints.begin(),endpoints.end(),pair.second);
		if(viterator != endpoints.end())
		{
		}else
		{
			endpoints.push_back(pair.second);
		}
	}

	//
	for(int i = 0; i < endpoints.size(); i++)
	{
		int vindex = endpoints[i];
		CVertex3d* pVertex = pUniformGridMesh->GetVertex(vindex);
		double len = sqrt(SQR(pVertex->x() - pStartVertex->x()) + SQR(pVertex->y() - pStartVertex->y()) + SQR(pVertex->z() - pStartVertex->z()));
		splitposition.push_back(len);
	}
	std::sort(splitposition.begin(),splitposition.end());
}
int Action_PolyOmino::IsEdgesConnected(int edgeindex1,int edgeindex2)
{
	std::pair<int,int> edgepair1 = pUniformGridMesh->m_ArraySharpEdges[edgeindex1];
	std::pair<int,int> edgepair2 = pUniformGridMesh->m_ArraySharpEdges[edgeindex2];
	if(edgepair1.first == edgepair2.first || edgepair1.first == edgepair2.second ||
		edgepair1.second == edgepair2.first || edgepair1.second == edgepair2.second)
		return TRUE;
	return FALSE;
}
void Action_PolyOmino::GetConnectedEdgeSet(std::vector<int> &sourceedgeset,std::vector<int> &targetedgeset)
{
	for(int i = 0; i < sourceedgeset.size(); i++)
	{
		int edgeindex = sourceedgeset[i];
		for(int j = 0; j < targetedgeset.size(); j++)
		{
			if(IsEdgesConnected(edgeindex,targetedgeset[j]))
			{
				targetedgeset.push_back(edgeindex);
				sourceedgeset.erase(sourceedgeset.begin() + i);
				i = -1;
				break;
			}
		}
	}
}

void Action_PolyOmino::OnBnClickedPolyominoChecktriangulation()
{
	// TODO: Add your control notification handler code here
	//check whether there exists a triangle whose two edges are sharp edges
	if(!pMesh)
		return;

	pMesh->SetVertexFlagAsItsIndex();
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		for(int j = 0; j < pLandmark->landmark_points.size() - 2; j++)
		{
			int index1 = pLandmark->landmark_points[j];
			int index2 = pLandmark->landmark_points[j + 1];
			int index3 = pLandmark->landmark_points[j + 2];

			CVertex3d* pVertex1 = pMesh->GetVertex(index1);
			CVertex3d* pVertex2 = pMesh->GetVertex(index2);
			CVertex3d* pVertex3 = pMesh->GetVertex(index3);
			for(int k = 0; k < pVertex1->NbFaceNeighbor(); k++)
			{
				CFace3d* pFace = pVertex1->GetFaceNeighbor(k);
				if((pFace->v1() == pVertex2 || pFace->v2() == pVertex2 || pFace->v3() == pVertex2) &&
					(pFace->v1() == pVertex3 || pFace->v2() == pVertex3 || pFace->v3() == pVertex3))
				{
					//pMesh->m_FixVerticesArray.push_back(index1);
					//pMesh->m_FixVerticesArray.push_back(index2);
					//pMesh->m_FixVerticesArray.push_back(index3);
					pLandmark->landmark_points.erase(pLandmark->landmark_points.begin() + j + 1);
					j--;
					break;
				}
			}
		}
	}
	/*std::pair<int,int> pair;
	int sharpedgenum,flag;
	std::vector<int> m_SharpEdges;

	pMesh->SetVertexFlagAsItsIndex();
	BOOL flag1;
	for(int i = 0; i < pMesh->m_ArrayFace.GetSize(); i++)
	{
		CFace3d* pFace = pMesh->m_ArrayFace[i];
		
		CVertex3d* pVertex1 = pFace->v1();
		CVertex3d* pVertex2 = pFace->v2();
		CVertex3d* pVertex3 = pFace->v3();

		sharpedgenum = 0;
		m_SharpEdges.clear();
		flag = 0;
		flag1 = 0;

		flag = EdgeInLandmark(pVertex1,pVertex2);
		if(flag > -1)
		{
			m_SharpEdges.push_back(flag);
			//sharpedgenum++;
		}
		flag = EdgeInLandmark(pVertex1,pVertex3);
		if(flag > -1)
		{
			if(std::find(m_SharpEdges.begin(),m_SharpEdges.end(),flag) == m_SharpEdges.end())
				m_SharpEdges.push_back(flag);
			else
			{
				flag1 = 1;
			}
			//sharpedgenum++;
		}
		flag = EdgeInLandmark(pVertex2,pVertex3);
		if(flag > -1)
		{
			if(std::find(m_SharpEdges.begin(),m_SharpEdges.end(),flag) == m_SharpEdges.end())
				m_SharpEdges.push_back(flag);
			else
			{
				flag1 = 1;
			}
			//m_SharpEdges.push_back(flag);
			//sharpedgenum++;
		}

		//ASSERT(sharpedgenum < 2);
		if(flag1)
		{
			//pMesh->m_ArraySelectedSharpEdgeIndex.insert(pMesh->m_ArraySelectedSharpEdgeIndex.begin(),m_SharpEdges.begin(),m_SharpEdges.end());
			pMesh->m_FixVerticesArray.push_back(pVertex1->m_Flag);
			pMesh->m_FixVerticesArray.push_back(pVertex2->m_Flag);
			pMesh->m_FixVerticesArray.push_back(pVertex3->m_Flag);
		}
	}*/
}
int Action_PolyOmino::EdgeInLandmark(CVertex3d* pVertex1,CVertex3d* pVertex2)
{
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		for(int j = 0; j < pLandmark->landmark_points.size() - 1; j++)
		{
			int index1 = pLandmark->landmark_points[j];
			int index2 = pLandmark->landmark_points[j + 1];
			
			if(pVertex1->m_Flag == index1 && pVertex2->m_Flag == index2 ||
				pVertex2->m_Flag == index2 && pVertex1->m_Flag == index1)
				return i;
		}
	}
	return -1;
	/*std::pair<int,int> pair;
	for(int j = 0; j < pMesh->m_ArraySharpEdges.size(); j++)
	{
		pair = pMesh->m_ArraySharpEdges[j];
		if(pVertex1->m_Flag == pair.first && pVertex2->m_Flag == pair.second ||
		   pVertex2->m_Flag == pair.first && pVertex1->m_Flag == pair.second)
		{
			return j + 1;
		}
	}
	return 0;*/
}


void Action_PolyOmino::OnBnClickedOminoLoadillinoismap()
{
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\georgia\\georgia_county_normalized_25_cornerflip.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Illinoise\\new\\illinois_county_fliped.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Illinoise\\illinois_county_normalized.m");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	OnBnClickedOminoSetcurrentmesh();
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pMesh->m_showFace = FALSE;

	OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\Illinoise\\new\\landmark7.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	Loadlandmark(landmarkname);
}

void Action_PolyOmino::OnBnClickedOminoLoadillinoisstreighten()
{
	// TODO: Add your control notification handler code here
	
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Illinoise\\illinois_county_normalized_streightened.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\Illinoise\\new\\illinois_county_fliped_streightened.m");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	OnBnClickedOminoSetcurrentmesh();
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pMesh->m_showFace = FALSE;

	//Generate Landmarks
	OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	sprintf(message,"D:\\papers\\code\\data\\Geo\\Illinoise\\new\\landmark7.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	Loadlandmark(landmarkname);

	//Load Constraints
	//LoadMergeConstraints("D:\\papers\\code\\data\\Geo\\dense\\mergeconstraints.txt");

	//Delete Auxillary Landmarks
	//OnBnClickedPolyominoDeleteauxillarylandmarks();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoInverselandmarkselection()
{
	// TODO: Add your control notification handler code here
	std::sort(pMesh->m_SelectedLandmarkIndexArray.begin(),pMesh->m_SelectedLandmarkIndexArray.end());
	int *m_ArrayFlag = new int[pMesh->m_LandMarkArray.size()];
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		m_ArrayFlag[i] = 0;

	for(int i = pMesh->m_SelectedLandmarkIndexArray.size() - 1; i >= 0; i--)
	{
		m_ArrayFlag[pMesh->m_SelectedLandmarkIndexArray[i]] = 1;
	}
	pMesh->m_SelectedLandmarkIndexArray.clear();
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		if(m_ArrayFlag[i] == 0)
		{
			pMesh->m_SelectedLandmarkIndexArray.push_back(i);
		}
	}
	delete m_ArrayFlag;
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::OnBnClickedPolyominoInversealllandmarkStyle()
{
	// TODO: Add your control notification handler code here
	GeoMapLandMark* pLandMark,*pLandMark1;
	for(int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandMark = pMesh->m_LandMarkArray[i];
		pLandMark->style = 1 - pLandMark->style;
	}
	pDoc->UpdateAllViews(NULL);
}
using namespace std;
void Action_PolyOmino::AdaptKdTreeEdges()
{
	CMesh3d* pNewMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	//
	std::vector<KDTree_node*> m_ArrayKdtreeNodes;
	m_ArrayKdtreeNodes.push_back(pKdTree->proot);

	int index = 0;
	while(m_ArrayKdtreeNodes.size() > 0)
	{
		KDTree_node* pNode = m_ArrayKdtreeNodes[0];
		m_ArrayKdtreeNodes.erase(m_ArrayKdtreeNodes.begin());

		if(pNode->leftchild)
			m_ArrayKdtreeNodes.push_back(pNode->leftchild);
		if(pNode->rightchild)
			m_ArrayKdtreeNodes.push_back(pNode->rightchild);

		double sum = 0;
		for(int i = 0; i < pNode->m_ArrayEdges.size(); i++)
		{
			int index = pNode->m_ArrayEdges[i];

			std::pair<int,int> pair = pOriginalMesh->m_ArraySharpEdges[index];
			if(pNode->axis == 0)
			{
				sum += pOriginalMesh->GetVertex(pair.first)->x();
				sum += pOriginalMesh->GetVertex(pair.second)->x();
			}else
			{
				sum += pOriginalMesh->GetVertex(pair.first)->y();
				sum += pOriginalMesh->GetVertex(pair.second)->y();
			}
		}
		sum /= pNode->m_ArrayEdges.size() * 2;
		pNode->val = sum;
	
		if(index == 0)
			break;
		index++;
	}

	CMesh3d* pMesh3D = pKdTree->generateMesh3D_Edges();
	pMesh3D->name = "KDTree";

	//bounding box
	Point3D lower,upper;
	pMesh3D->ComputeBoundingBox(lower,upper);
	CVector3d m_lower,m_upper;
	m_lower.Set(lower.x,lower.y,lower.z);
	m_upper.Set(upper.x,upper.y,upper.z);
	CTransform transform;
	transform.Clear();
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation=m_lower+m_upper;
	m_translation/=-2;
	float length=m_upper.x()-m_lower.x();
	float length1 = m_upper.y() - m_lower.y();
	if(length<m_upper.y()-m_lower.y())
		length=m_upper.y()-m_lower.y();
	if(length<m_upper.z()-m_lower.z())
		length=m_upper.z()-m_lower.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1/length,1/length,1/length);
	transform.SetScale(&m_Scale);
	CSceneGraph3d* pSceneGraph = &pDoc->m_SceneGraph;
	if(pSceneGraph->NbObject() >= 1)
	{
		if(pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pSurface = (CNurbsSuface*)pSceneGraph->GetAt(0);
			pMesh3D->SetTransform(*pSurface->GetTransform());
		}
		else
			if(pSceneGraph->GetAt(0)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pSurface = (CMesh3d*)pSceneGraph->GetAt(0);
				pMesh3D->SetTransform(*pSurface->GetTransform());
			}
	}else
		pMesh3D->SetTransform(transform);

	pMesh3D->SetFlagOnFaces(0);
	pSceneGraph->InsertAt(0,pMesh3D);
	//pSceneGraph->Add(pMesh3D);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::ConstructKdTreeFromEdges_China()
{
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);

	CMesh3d* pOriginalMesh;
	if (m_InitialIndex == 1)
		pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	else
		pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	Vector2D lower1, upper1;
	lower1.x = 71.395203;
	lower1.y = 16.1618;
	upper1.x = 136.800995;
	upper1.y = 55.4949;
	pKdTree->pOriginalMesh = pOriginalMesh;
	pKdTree->pUniformGridMesh = pMesh;
	pKdTree->m_VertexIndexArray = m_VertexIndexArray;
	pKdTree->m_EdgeLandmarkIndexArray = m_EdgeLandmarkIndexArray;

	pKdTree->ConstructionKdTree_Edges(pOriginalMesh, pMesh, m_HorizontalSegments, m_VerticalSegments, 0, NULL, lower1, upper1, m_InitialIndex);
	//set lower & upper
	pKdTree->lower.SetValue(1e10, 1e10);
	pKdTree->upper.SetValue(-1e10, -1e10);
	//Vector2D vv1 = kdtree1.nearestSearch(kdtree1.proot,vv);

	pKdTree->lower.x = 71.395203;
	pKdTree->upper.x = 136.800995;
	pKdTree->lower.y = 16.1618;
	pKdTree->upper.y = 55.4949;
	//plot the kd-tree
	CMesh3d* pMesh3D = pKdTree->generateMesh3D_Edges();
	pMesh3D->name = "KDTree";

	//
	//bounding box
	Point3D lower, upper;
	pMesh3D->ComputeBoundingBox(lower, upper);
	CVector3d m_lower, m_upper;
	m_lower.Set(lower.x, lower.y, lower.z);
	m_upper.Set(upper.x, upper.y, upper.z);
	CTransform transform;
	transform.Clear();
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation = m_lower + m_upper;
	m_translation /= -2;
	float length = m_upper.x() - m_lower.x();
	float length1 = m_upper.y() - m_lower.y();
	if (length < m_upper.y() - m_lower.y())
		length = m_upper.y() - m_lower.y();
	if (length < m_upper.z() - m_lower.z())
		length = m_upper.z() - m_lower.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1 / length, 1 / length, 1 / length);
	transform.SetScale(&m_Scale);
	CSceneGraph3d* pSceneGraph = &pDoc->m_SceneGraph;
	if (pSceneGraph->NbObject() >= 1)
	{
		if (pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pSurface = (CNurbsSuface*)pSceneGraph->GetAt(0);
			pMesh3D->SetTransform(*pSurface->GetTransform());
		}
		else
			if (pSceneGraph->GetAt(0)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pSurface = (CMesh3d*)pSceneGraph->GetAt(0);
				pMesh3D->SetTransform(*pSurface->GetTransform());
			}
	}
	else
		pMesh3D->SetTransform(transform);

	pMesh3D->SetFlagOnFaces(0);
	//pSceneGraph->InsertAt(0,pMesh3D);
	if (m_InitialIndex > 1)
		pSceneGraph->RemoveAt(pSceneGraph->NbObject() - 1);
	pSceneGraph->Add(pMesh3D);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	m_InitialIndex++;
}
void Action_PolyOmino::ConstructKdTreeFromEdges()
{
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	
	CMesh3d* pOriginalMesh;
	if(m_InitialIndex == 1)
		pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	else
		pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);
	Vector2D lower1,upper1;
	lower1.x = 0;
	lower1.y = 0;
	upper1.x = 1;
	upper1.y = 0.656463;
	pKdTree->pOriginalMesh = pOriginalMesh;
	pKdTree->pUniformGridMesh = pMesh;
	pKdTree->m_VertexIndexArray = m_VertexIndexArray;
	pKdTree->m_EdgeLandmarkIndexArray = m_EdgeLandmarkIndexArray;

	pKdTree->ConstructionKdTree_Edges(pOriginalMesh,pMesh,m_HorizontalSegments,m_VerticalSegments,0,NULL,lower1,upper1,m_InitialIndex);
	//set lower & upper
	pKdTree->lower.SetValue(1e10,1e10);
	pKdTree->upper.SetValue(-1e10,-1e10);
	//Vector2D vv1 = kdtree1.nearestSearch(kdtree1.proot,vv);

	pKdTree->lower.x = 0;
	pKdTree->upper.x = 1;
	pKdTree->lower.y = 0;
	pKdTree->upper.y = 0.656463;
	//plot the kd-tree
	CMesh3d* pMesh3D = pKdTree->generateMesh3D_Edges();
	pMesh3D->name = "KDTree";

	//
	//bounding box
	Point3D lower,upper;
	pMesh3D->ComputeBoundingBox(lower,upper);
	CVector3d m_lower,m_upper;
	m_lower.Set(lower.x,lower.y,lower.z);
	m_upper.Set(upper.x,upper.y,upper.z);
	CTransform transform;
	transform.Clear();
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation=m_lower+m_upper;
	m_translation/=-2;
	float length=m_upper.x()-m_lower.x();
	float length1 = m_upper.y() - m_lower.y();
	if(length<m_upper.y()-m_lower.y())
		length=m_upper.y()-m_lower.y();
	if(length<m_upper.z()-m_lower.z())
		length=m_upper.z()-m_lower.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1/length,1/length,1/length);
	transform.SetScale(&m_Scale);
	CSceneGraph3d* pSceneGraph = &pDoc->m_SceneGraph;
	if(pSceneGraph->NbObject() >= 1)
	{
		if(pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pSurface = (CNurbsSuface*)pSceneGraph->GetAt(0);
			pMesh3D->SetTransform(*pSurface->GetTransform());
		}
		else
			if(pSceneGraph->GetAt(0)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pSurface = (CMesh3d*)pSceneGraph->GetAt(0);
				pMesh3D->SetTransform(*pSurface->GetTransform());
			}
	}else
		pMesh3D->SetTransform(transform);

	pMesh3D->SetFlagOnFaces(0);
	//pSceneGraph->InsertAt(0,pMesh3D);
	if(m_InitialIndex > 1)
		pSceneGraph->RemoveAt(pSceneGraph->NbObject() - 1);
	pSceneGraph->Add(pMesh3D);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
	m_InitialIndex++;
}
void Action_PolyOmino::ConstructKdTreeFromPoints()
{
	std::vector<Vector2D> m_ArrayPointsArray;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	for(int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		Vector2D point;
		point.x = pVertex->x();
		point.y = pVertex->y();
		m_ArrayPointsArray.push_back(point);
	}

	KDTree kdtree1;
	kdtree1.ConstructionKdTree_Points(m_ArrayPointsArray,0,NULL);

	//set lower & upper
	kdtree1.lower.SetValue(1e10,1e10);
	kdtree1.upper.SetValue(-1e10,-1e10);
	for(int i = 0; i < m_ArrayPointsArray.size(); i++)
	{
		double x = m_ArrayPointsArray[i].x;
		double y = m_ArrayPointsArray[i].y;
		if(x < kdtree1.lower.x)
			kdtree1.lower.x = x;
		if(y < kdtree1.lower.y)
			kdtree1.lower.y = y;
		if(x < kdtree1.lower.x)
			kdtree1.lower.x = x;
		if(y < kdtree1.lower.y)
			kdtree1.lower.y = y;

		if(x > kdtree1.upper.x)
			kdtree1.upper.x = x;
		if(y > kdtree1.upper.y)
			kdtree1.upper.y = y;
		if(x > kdtree1.upper.x)
			kdtree1.upper.x = x;
		if(y > kdtree1.upper.y)
			kdtree1.upper.y = y;
	}
	//Vector2D vv1 = kdtree1.nearestSearch(kdtree1.proot,vv);

	kdtree1.lower.x = 0;
	kdtree1.upper.x = 1;
	kdtree1.lower.y = 0;
	kdtree1.upper.y = 0.656463;
	//plot the kd-tree
	CMesh3d* pMesh3D = kdtree1.generateMesh3D_Points();
	pMesh3D->name = "KDTree";

	//
	//bounding box
	Point3D lower,upper;
	pMesh3D->ComputeBoundingBox(lower,upper);
	CVector3d m_lower,m_upper;
	m_lower.Set(lower.x,lower.y,lower.z);
	m_upper.Set(upper.x,upper.y,upper.z);
	CTransform transform;
	transform.Clear();
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation=m_lower+m_upper;
	m_translation/=-2;
	float length=m_upper.x()-m_lower.x();
	float length1 = m_upper.y() - m_lower.y();
	if(length<m_upper.y()-m_lower.y())
		length=m_upper.y()-m_lower.y();
	if(length<m_upper.z()-m_lower.z())
		length=m_upper.z()-m_lower.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1/length,1/length,1/length);
	transform.SetScale(&m_Scale);
	CSceneGraph3d* pSceneGraph = &pDoc->m_SceneGraph;
	if(pSceneGraph->NbObject() >= 1)
	{
		if(pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pSurface = (CNurbsSuface*)pSceneGraph->GetAt(0);
			pMesh3D->SetTransform(*pSurface->GetTransform());
		}
		else
			if(pSceneGraph->GetAt(0)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pSurface = (CMesh3d*)pSceneGraph->GetAt(0);
				pMesh3D->SetTransform(*pSurface->GetTransform());
			}
	}else
		pMesh3D->SetTransform(transform);

	pMesh3D->SetFlagOnFaces(0);
	pSceneGraph->Add(pMesh3D);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::GetPointandEdgeCorrespondence_China()
{
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\dense\\USA_streighten.m");
	//pDoc->OnOpenDocument("D:\\usamapdetailedlandmark_streightened.obj");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87_straightened.m");

	CMesh3d* pOriginalRectilinearMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\landmark14.txt");
	//sprintf(message, "D:\\landmark14.txt");
	sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate_Landmak87.txt");

	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	Loadlandmark(pOriginalRectilinearMesh, landmarkname);
	m_VertexIndexArray.resize(pUniformGridMesh->m_ArrayVertex.GetSize());
	for (int i = 0; i < pUniformGridMesh->m_ArrayVertex.GetSize(); i++)
	{
		CVertex3d* pVertex = pUniformGridMesh->GetVertex(i);
		double maxlen = 1e10;
		int maxindex;
		for (int j = 0; j < pOriginalRectilinearMesh->m_ArrayVertex.GetSize(); j++)
		{
			CVertex3d* pVertex1 = pOriginalRectilinearMesh->GetVertex(j);
			double len = Distance(pVertex, pVertex1);
			if (len < maxlen)
			{
				maxlen = len;
				maxindex = j;
			}
		}
		m_VertexIndexArray[i] = maxindex;
	}
	m_EdgeLandmarkIndexArray.resize(pUniformGridMesh->m_ArraySharpEdges.size());
	for (int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int, int> pair = pUniformGridMesh->m_ArraySharpEdges[i];
		int vindex1 = m_VertexIndexArray[pair.first];
		int vindex2 = m_VertexIndexArray[pair.second];

		for (int j = 0; j < pOriginalRectilinearMesh->m_LandMarkArray.size(); j++)
		{
			GeoMapLandMark* pLandmark = pOriginalRectilinearMesh->m_LandMarkArray[j];
			int lindex1 = pLandmark->landmark_points[0];
			int lindex2 = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];

			if (vindex1 == lindex1 && vindex2 == lindex2 || vindex1 == lindex2 && vindex2 == lindex1)
			{
				m_EdgeLandmarkIndexArray[i] = j;
				break;
			}
		}
	}
	pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);
}
void Action_PolyOmino::GetPointandEdgeCorrespondence()
{
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\dense\\USA_streighten.m");
	//pDoc->OnOpenDocument("D:\\usamapdetailedlandmark_streightened.obj");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87_straightened.m");

	CMesh3d* pOriginalRectilinearMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	char message[200];
	sprintf(message,"D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message, "D:\\landmark14.txt");
	//sprintf(message, "D:\\landmark14.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate_Landmak87.txt");

	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	Loadlandmark(pOriginalRectilinearMesh,landmarkname);
	m_VertexIndexArray.resize(pUniformGridMesh->m_ArrayVertex.GetSize());
	for(int i = 0; i < pUniformGridMesh->m_ArrayVertex.GetSize(); i++)
	{
		CVertex3d* pVertex = pUniformGridMesh->GetVertex(i);
		double maxlen = 1e10;
		int maxindex;
		for(int j = 0; j < pOriginalRectilinearMesh->m_ArrayVertex.GetSize(); j++)
		{
			CVertex3d* pVertex1 = pOriginalRectilinearMesh->GetVertex(j);
			double len = Distance(pVertex,pVertex1);
			if(len < maxlen)
			{
				maxlen = len;
				maxindex = j;
			}
		}
		m_VertexIndexArray[i] = maxindex;
	}
	m_EdgeLandmarkIndexArray.resize(pUniformGridMesh->m_ArraySharpEdges.size());
	for(int i = 0; i < pUniformGridMesh->m_ArraySharpEdges.size(); i++)
	{	
		std::pair<int,int> pair = pUniformGridMesh->m_ArraySharpEdges[i];
		int vindex1 = m_VertexIndexArray[pair.first];
		int vindex2 = m_VertexIndexArray[pair.second];

		for(int j = 0; j < pOriginalRectilinearMesh->m_LandMarkArray.size(); j++)
		{
			GeoMapLandMark* pLandmark = pOriginalRectilinearMesh->m_LandMarkArray[j];
			int lindex1 = pLandmark->landmark_points[0];
			int lindex2 = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];

			if(vindex1 == lindex1 && vindex2 == lindex2 || vindex1 == lindex2 && vindex2 == lindex1)
			{
				m_EdgeLandmarkIndexArray[i] = j;
				break;
			}
		}
	}
	pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);
}
void Action_PolyOmino::PointSearchTest()
{
	Vector2D point(0.5, 0.5);
	LARGE_INTEGER m_liPerfFreq = { 0 };
	//获取每秒多少CPU Performance Tick
	QueryPerformanceFrequency(&m_liPerfFreq);
	LARGE_INTEGER m_liPerfStart = { 0 };
	QueryPerformanceCounter(&m_liPerfStart);

	int index = pKdTree->searchLocatedDistrict(point);

	LARGE_INTEGER liPerfNow = { 0 };
	// 计算CPU运行到现在的时间
	QueryPerformanceCounter(&liPerfNow);
	double time = (((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000.0) / m_liPerfFreq.QuadPart);
	//int province = pKdTree->search(pOriginalMesh, pUniformGridMesh, point, m_VertexIndexArray, m_ArrayEdgeDistricts);
}
void Action_PolyOmino::Initialization_KDTree1()
{
	int iterationnum;
	int MeshIndex = 0;
	char meshname[100];
	pKdTree = new KDTree();

	sprintf(meshname, "D:\\usamapdetailedlandmark_streightened.obj");
	//LoadData1("D:\\papers\\code\\data\\Geo\\dense\\1788.m",iterationnum);
	//LoadData1(meshname, iterationnum);
	pDoc->OnOpenDocument(meshname);

	char message[200];
	sprintf(message, "D:\\landmark14.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;
	pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);

	Loadlandmark(pMesh, landmarkname);
	//sharpedge.
	GenerateUniformGridMesh();

	//Horizontal and Vertical
	//Update edge connection
	UpdatePointConnectedSelectedEdgeIndex();

	//
	InitializeHorizontalandVerticalSegmentArray();

	pDoc->m_SceneGraph.RemoveAt(0);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");

	//
	GetPointandEdgeCorrespondence();

	CMesh3d* pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	//char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//CString landmarkname = message;

	Loadlandmark(pOriginalMesh, landmarkname);
	//CMesh3d* temp = pUniformGridMesh;
	//pUniformGridMesh = pOriginalMesh;
	UpdatePointConnectedSelectedEdgeIndex();

	//Build the struture for the area
	std::vector<int> m_ArrayFacePoints;
	std::vector<int> m_ArrayFaceEdges;
	//std::vector<std::vector<int>> m_ArrayFacePointsArray;
	//std::vector<std::vector<int>> m_ArrayFaceEdgesArray;
	//TraceFaces(m_ArrayFacePoints, m_ArrayFaceEdges);

	//Remove the outer face
	//RemoveOuterFace(pUniformGridMesh->m_ArrayFacePointsArray, pUniformGridMesh->m_ArrayFaceEdgesArray);

	//pUniformGridMesh = temp;

	//
	/*pKdTree->m_ArrayEdgeDistricts.resize(pUniformGridMesh->m_ArraySharpEdges.size());
	for (int i = 0; i < pKdTree->m_ArrayEdgeDistricts.size(); i++)
	{
		pKdTree->m_ArrayEdgeDistricts[i].first = pKdTree->m_ArrayEdgeDistricts[i].second = -1;
	}

	int m_Array[1];
	for (int i = 0; i < pUniformGridMesh->m_ArrayFaceEdgesArray.size(); i++)
	{
		for (int j = 0; j < pUniformGridMesh->m_ArrayFaceEdgesArray[i].size(); j++)
		{
			int edgeindex = pUniformGridMesh->m_ArrayFaceEdgesArray[i][j];
			std::pair<int, int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
			m_Array[0] = pair.first;
			int findex = std::search(pUniformGridMesh->m_ArrayFacePointsArray[i].begin(), pUniformGridMesh->m_ArrayFacePointsArray[i].end(), m_Array, m_Array + 1) - pUniformGridMesh->m_ArrayFacePointsArray[i].begin();
			int sum = pUniformGridMesh->m_ArrayFacePointsArray[i].size();
			if (pair.second == pUniformGridMesh->m_ArrayFacePointsArray[i][(findex + 1) % sum])
				pKdTree->m_ArrayEdgeDistricts[edgeindex].first = i;
			else
				pKdTree->m_ArrayEdgeDistricts[edgeindex].second = i;
		}
	}*/
}
void Action_PolyOmino::Initialization_KDTree_China()
{
	int iterationnum;
	int MeshIndex = 0;
	char meshname[100];
	pKdTree = new KDTree();

	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87_straightened.m");

	//sprintf(meshname, "D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87_straightened.m");
	sprintf(meshname, "D:\\papers\\code\\data\\Geo\\ditu\\china\\dense\\%d.m", MeshIndex);
	//LoadData1("D:\\papers\\code\\data\\Geo\\dense\\1788.m",iterationnum);
	LoadData1(meshname, iterationnum);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87.m");

	//pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	//
	GetPointandEdgeCorrespondence_China();

	CMesh3d* pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate_Landmak87.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;

	Loadlandmark(pOriginalMesh, landmarkname);
	//CMesh3d* temp = pUniformGridMesh;
	//pUniformGridMesh = pOriginalMesh;
	UpdatePointConnectedSelectedEdgeIndex();

	//Build the struture for the area
	std::vector<int> m_ArrayFacePoints;
	std::vector<int> m_ArrayFaceEdges;
	//std::vector<std::vector<int>> m_ArrayFacePointsArray;
	//std::vector<std::vector<int>> m_ArrayFaceEdgesArray;
	TraceFaces(m_ArrayFacePoints, m_ArrayFaceEdges);

	//Remove the outer face
	//RemoveOuterFace(pUniformGridMesh->m_ArrayFacePointsArray, pUniformGridMesh->m_ArrayFaceEdgesArray);
	pUniformGridMesh->m_ArrayFacePointsArray.erase(pUniformGridMesh->m_ArrayFacePointsArray.begin() + 4);
	pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + 4);
	//pUniformGridMesh = temp;

	//
	pKdTree->m_ArrayEdgeDistricts.resize(pUniformGridMesh->m_ArraySharpEdges.size());
	for (int i = 0; i < pKdTree->m_ArrayEdgeDistricts.size(); i++)
	{
		pKdTree->m_ArrayEdgeDistricts[i].first = pKdTree->m_ArrayEdgeDistricts[i].second = -1;
	}

	int m_Array[1];
	for (int i = 0; i < pUniformGridMesh->m_ArrayFaceEdgesArray.size(); i++)
	{
		for (int j = 0; j < pUniformGridMesh->m_ArrayFaceEdgesArray[i].size(); j++)
		{
			int edgeindex = pUniformGridMesh->m_ArrayFaceEdgesArray[i][j];
			std::pair<int, int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
			m_Array[0] = pair.first;
			int findex = std::search(pUniformGridMesh->m_ArrayFacePointsArray[i].begin(), pUniformGridMesh->m_ArrayFacePointsArray[i].end(), m_Array, m_Array + 1) - pUniformGridMesh->m_ArrayFacePointsArray[i].begin();
			int sum = pUniformGridMesh->m_ArrayFacePointsArray[i].size();
			if (pair.second == pUniformGridMesh->m_ArrayFacePointsArray[i][(findex + 1) % sum])
				pKdTree->m_ArrayEdgeDistricts[edgeindex].first = i;
			else
				pKdTree->m_ArrayEdgeDistricts[edgeindex].second = i;
		}
	}
}
void Action_PolyOmino::Initialization_KDTree_World()
{
	int iterationnum;
	int MeshIndex = 0;
	char meshname[100];
	pKdTree = new KDTree();

	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87_straightened.m");

	//sprintf(meshname, "D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87_straightened.m");
	sprintf(meshname, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\finalrectilinearmap\\worldmaptriangulation_coarse_more253.m");
	//LoadData1("D:\\papers\\code\\data\\Geo\\dense\\1788.m",iterationnum);
	//LoadData1(meshname, iterationnum);
	pDoc->OnOpenDocument(meshname);
	pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph[pDoc->m_SceneGraph.NbObject() - 1];
	pMesh = pUniformGridMesh;
	//pPolyOmino->GenerateUniformGridMesh();

	//Update edge connection
	UpdatePointConnectedSelectedEdgeIndex();

	//
	InitializeHorizontalandVerticalSegmentArray();

	MergeHorizontalVerticalSegments();

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_coarse.m");

	//pUniformGridMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	//
	GetPointandEdgeCorrespondence_China();

	CMesh3d* pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;

	Loadlandmark(pOriginalMesh, landmarkname);
	//CMesh3d* temp = pUniformGridMesh;
	//pUniformGridMesh = pOriginalMesh;
	UpdatePointConnectedSelectedEdgeIndex();

	//Build the struture for the area
	std::vector<int> m_ArrayFacePoints;
	std::vector<int> m_ArrayFaceEdges;
	//std::vector<std::vector<int>> m_ArrayFacePointsArray;
	//std::vector<std::vector<int>> m_ArrayFaceEdgesArray;
	TraceFaces(m_ArrayFacePoints, m_ArrayFaceEdges);

	//Remove the outer face
	//RemoveOuterFace(pUniformGridMesh->m_ArrayFacePointsArray, pUniformGridMesh->m_ArrayFaceEdgesArray);
	pUniformGridMesh->m_ArrayFacePointsArray.erase(pUniformGridMesh->m_ArrayFacePointsArray.begin() + 4);
	pUniformGridMesh->m_ArrayFaceEdgesArray.erase(pUniformGridMesh->m_ArrayFaceEdgesArray.begin() + 4);
	//pUniformGridMesh = temp;

	//
	pKdTree->m_ArrayEdgeDistricts.resize(pUniformGridMesh->m_ArraySharpEdges.size());
	for (int i = 0; i < pKdTree->m_ArrayEdgeDistricts.size(); i++)
	{
		pKdTree->m_ArrayEdgeDistricts[i].first = pKdTree->m_ArrayEdgeDistricts[i].second = -1;
	}

	int m_Array[1];
	for (int i = 0; i < pUniformGridMesh->m_ArrayFaceEdgesArray.size(); i++)
	{
		for (int j = 0; j < pUniformGridMesh->m_ArrayFaceEdgesArray[i].size(); j++)
		{
			int edgeindex = pUniformGridMesh->m_ArrayFaceEdgesArray[i][j];
			std::pair<int, int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
			m_Array[0] = pair.first;
			int findex = std::search(pUniformGridMesh->m_ArrayFacePointsArray[i].begin(), pUniformGridMesh->m_ArrayFacePointsArray[i].end(), m_Array, m_Array + 1) - pUniformGridMesh->m_ArrayFacePointsArray[i].begin();
			int sum = pUniformGridMesh->m_ArrayFacePointsArray[i].size();
			if (pair.second == pUniformGridMesh->m_ArrayFacePointsArray[i][(findex + 1) % sum])
				pKdTree->m_ArrayEdgeDistricts[edgeindex].first = i;
			else
				pKdTree->m_ArrayEdgeDistricts[edgeindex].second = i;
		}
	}
}
void Action_PolyOmino::Initialization_KDTree_WorldMap()
{
	int iterationnum;
	int MeshIndex = 0;
	char meshname[100];
	pKdTree = new KDTree();

	sprintf(meshname, "D:\\papers\\code\\data\\Geo\\dense\\%d.m", MeshIndex);
	//LoadData1("D:\\papers\\code\\data\\Geo\\dense\\1788.m",iterationnum);
	LoadData1(meshname, iterationnum);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");

	//
	GetPointandEdgeCorrespondence();

	CMesh3d* pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;

	Loadlandmark(pOriginalMesh, landmarkname);
	//CMesh3d* temp = pUniformGridMesh;
	//pUniformGridMesh = pOriginalMesh;
	UpdatePointConnectedSelectedEdgeIndex();

	//Build the struture for the area
	std::vector<int> m_ArrayFacePoints;
	std::vector<int> m_ArrayFaceEdges;
	//std::vector<std::vector<int>> m_ArrayFacePointsArray;
	//std::vector<std::vector<int>> m_ArrayFaceEdgesArray;
	TraceFaces(m_ArrayFacePoints, m_ArrayFaceEdges);

	//Remove the outer face
	RemoveOuterFace(pUniformGridMesh->m_ArrayFacePointsArray, pUniformGridMesh->m_ArrayFaceEdgesArray);

	//pUniformGridMesh = temp;

	//
	pKdTree->m_ArrayEdgeDistricts.resize(pUniformGridMesh->m_ArraySharpEdges.size());
	for (int i = 0; i < pKdTree->m_ArrayEdgeDistricts.size(); i++)
	{
		pKdTree->m_ArrayEdgeDistricts[i].first = pKdTree->m_ArrayEdgeDistricts[i].second = -1;
	}

	int m_Array[1];
	for (int i = 0; i < pUniformGridMesh->m_ArrayFaceEdgesArray.size(); i++)
	{
		for (int j = 0; j < pUniformGridMesh->m_ArrayFaceEdgesArray[i].size(); j++)
		{
			int edgeindex = pUniformGridMesh->m_ArrayFaceEdgesArray[i][j];
			std::pair<int, int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
			m_Array[0] = pair.first;
			int findex = std::search(pUniformGridMesh->m_ArrayFacePointsArray[i].begin(), pUniformGridMesh->m_ArrayFacePointsArray[i].end(), m_Array, m_Array + 1) - pUniformGridMesh->m_ArrayFacePointsArray[i].begin();
			int sum = pUniformGridMesh->m_ArrayFacePointsArray[i].size();
			if (pair.second == pUniformGridMesh->m_ArrayFacePointsArray[i][(findex + 1) % sum])
				pKdTree->m_ArrayEdgeDistricts[edgeindex].first = i;
			else
				pKdTree->m_ArrayEdgeDistricts[edgeindex].second = i;
		}
	}
}
void Action_PolyOmino::Initialization_KDTree()
{
	int iterationnum;
	int MeshIndex = 0;
	char meshname[100];
	pKdTree = new KDTree();

	sprintf(meshname, "D:\\papers\\code\\data\\Geo\\dense\\%d.m", MeshIndex);
	//LoadData1("D:\\papers\\code\\data\\Geo\\dense\\1788.m",iterationnum);
	LoadData1(meshname, iterationnum);

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");

	//
	GetPointandEdgeCorrespondence();

	CMesh3d* pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	//sprintf(message,"P:\\Dr_YYJ\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");
	CString landmarkname = message;

	Loadlandmark(pOriginalMesh, landmarkname);
	//CMesh3d* temp = pUniformGridMesh;
	//pUniformGridMesh = pOriginalMesh;
	UpdatePointConnectedSelectedEdgeIndex();

	//Build the struture for the area
	std::vector<int> m_ArrayFacePoints;
	std::vector<int> m_ArrayFaceEdges;
	//std::vector<std::vector<int>> m_ArrayFacePointsArray;
	//std::vector<std::vector<int>> m_ArrayFaceEdgesArray;
	TraceFaces(m_ArrayFacePoints, m_ArrayFaceEdges);

	//Remove the outer face
	RemoveOuterFace(pUniformGridMesh->m_ArrayFacePointsArray, pUniformGridMesh->m_ArrayFaceEdgesArray);

	//pUniformGridMesh = temp;

	//
	pKdTree->m_ArrayEdgeDistricts.resize(pUniformGridMesh->m_ArraySharpEdges.size());
	for (int i = 0; i < pKdTree->m_ArrayEdgeDistricts.size(); i++)
	{
		pKdTree->m_ArrayEdgeDistricts[i].first = pKdTree->m_ArrayEdgeDistricts[i].second = -1;
	}

	int m_Array[1];
	for (int i = 0; i < pUniformGridMesh->m_ArrayFaceEdgesArray.size(); i++)
	{
		for (int j = 0; j < pUniformGridMesh->m_ArrayFaceEdgesArray[i].size(); j++)
		{
			int edgeindex = pUniformGridMesh->m_ArrayFaceEdgesArray[i][j];
			std::pair<int, int> pair = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
			m_Array[0] = pair.first;
			int findex = std::search(pUniformGridMesh->m_ArrayFacePointsArray[i].begin(), pUniformGridMesh->m_ArrayFacePointsArray[i].end(), m_Array, m_Array + 1) - pUniformGridMesh->m_ArrayFacePointsArray[i].begin();
			int sum = pUniformGridMesh->m_ArrayFacePointsArray[i].size();
			if (pair.second == pUniformGridMesh->m_ArrayFacePointsArray[i][(findex + 1) % sum])
				pKdTree->m_ArrayEdgeDistricts[edgeindex].first = i;
			else
				pKdTree->m_ArrayEdgeDistricts[edgeindex].second = i;
		}
	}
}
int maxdep;
void Action_PolyOmino::KdtreeChina()
{
	Initialization_KDTree_China();

	pKdTree->maxDeviation = GetLargestPointDeviations_China();

	//return;
	//ConstructKdTreeFromPoints();
	//return;

	//GetPointandEdgeCorrespondence();

	m_InitialIndex = 1;

	maxdep = 1;
	ConstructKdTreeFromEdges_China();

	PointSearchTest();
}
void Action_PolyOmino::KdtreeWorld()
{
	Initialization_KDTree_China();
	//return;
	pKdTree->maxDeviation = GetLargestPointDeviations_China();

	//return;
	//ConstructKdTreeFromPoints();
	//return;

	//GetPointandEdgeCorrespondence();

	m_InitialIndex = 1;

	maxdep = 1;
	ConstructKdTreeFromEdges_China();

	PointSearchTest();
}
void Action_PolyOmino::OnBnClickedButtonKdtree()
{
	KdtreeChina();
	//KdtreeWorld();
	return;
	// TODO: Add your control notification handler code here
	Initialization_KDTree_WorldMap();
	//Initialization_KDTree();
	//Initialization_KDTree1();

	//Meare the largest deviations
	pKdTree->maxDeviation = GetLargestPointDeviations();

	//return;
	//ConstructKdTreeFromPoints();
	//return;

	//GetPointandEdgeCorrespondence();

	m_InitialIndex = 1;

	maxdep = 1;
	ConstructKdTreeFromEdges();

	PointSearchTest();

	//
	//AdaptKdTreeEdges();

	//
	//ComputeKdTreeNodeDenotedAreas();

	//
	//ComputeKdTreeLeafLocatedStates();
	//Search which state the point (0.5,0.5) lies in.

}
void Action_PolyOmino::ComputeKdTreeNodeDenotedAreas()
{
	//Compute each located areas.
	pKdTree->proot;

}
void Action_PolyOmino::ComputeKdTreeLeafLocatedStates()
{
	//
	
}
double Action_PolyOmino::GetLargestPointDeviations_China()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\china\\china_triangulate87_straightened.m");
	//pDoc->OnOpenDocument("D:\\usamapdetailedlandmark_streightened.obj");
	CMesh3d* pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	CMesh3d* pNewMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);

	maxPointDeviations = -1e10;
	int maxindex;
	for (int i = 0; i < pNewMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pNewMesh->GetVertex(i);
		CVertex3d* pVertex1 = pOriginalMesh->GetVertex(i);

		double len = sqrt(SQR(pVertex->x() - pVertex1->x()) + SQR(pVertex->y() - pVertex1->y()) + SQR(pVertex->z() - pVertex1->z()));
		if (len > maxPointDeviations)
		{
			maxPointDeviations = len;
			maxindex = i;
		}
	}
	//Remove the original one
	pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);
	pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);
	pDoc->UpdateTreeControl();
	return maxPointDeviations;
}
double Action_PolyOmino::GetLargestPointDeviations()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\USA_33.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\dense\\USA_Streighten.m");
	//pDoc->OnOpenDocument("D:\\usamapdetailedlandmark_streightened.obj");
	CMesh3d* pOriginalMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	CMesh3d* pNewMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);

	maxPointDeviations = -1e10;
	int maxindex;
	for(int i = 0; i < pNewMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pNewMesh->GetVertex(i);
		CVertex3d* pVertex1 = pOriginalMesh->GetVertex(i);

		double len = sqrt(SQR(pVertex->x() - pVertex1->x()) + SQR(pVertex->y() - pVertex1->y()) + SQR(pVertex->z() - pVertex1->z()));
		if(len > maxPointDeviations)
		{
			maxPointDeviations = len;
			maxindex = i;
		}
	}
	//Remove the original one
	pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);
	pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);
	pDoc->UpdateTreeControl();
	return maxPointDeviations;
}
CMesh3d* KDTree::generateMesh3D_Points()
{
	CMesh3d* pMesh3d = new CMesh3d();

	//plot the Mesh3D
	KDTree_node* pnode = proot;
	double lowerx = lower.x;
	double lowery = lower.y;
	double upperx = upper.x;
	double uppery = upper.y;

	//the boundary
	pMesh3d->AddVertex(new CVertex3d(lowerx,lowery,0));
	pMesh3d->AddVertex(new CVertex3d(upperx,lowery,0));
	std::pair<int,int> data;
	data.first = pMesh3d->NbVertex() - 2;
	data.second = pMesh3d->NbVertex() - 1;
	pMesh3d->m_ArraySharpEdges.push_back(data);

	pMesh3d->AddVertex(new CVertex3d(lowerx,lowery,0));
	pMesh3d->AddVertex(new CVertex3d(lowerx,uppery,0));
	data.first = pMesh3d->NbVertex() - 2;
	data.second = pMesh3d->NbVertex() - 1;
	pMesh3d->m_ArraySharpEdges.push_back(data);

	pMesh3d->AddVertex(new CVertex3d(upperx,uppery,0));
	pMesh3d->AddVertex(new CVertex3d(lowerx,uppery,0));
	data.first = pMesh3d->NbVertex() - 2;
	data.second = pMesh3d->NbVertex() - 1;
	pMesh3d->m_ArraySharpEdges.push_back(data);

	pMesh3d->AddVertex(new CVertex3d(upperx,uppery,0));
	pMesh3d->AddVertex(new CVertex3d(upperx,lowery,0));
	data.first = pMesh3d->NbVertex() - 2;
	data.second = pMesh3d->NbVertex() - 1;
	pMesh3d->m_ArraySharpEdges.push_back(data);

	AddMeshEdge_Points(pMesh3d,proot,lowerx,lowery,upperx,uppery);
	return pMesh3d;
}
CMesh3d* KDTree::generateMesh3D_Edges()
{
	CMesh3d* pMesh3d = new CMesh3d();

	//plot the Mesh3D
	KDTree_node* pnode = proot;
	double lowerx = lower.x;
	double lowery = lower.y;
	double upperx = upper.x;
	double uppery = upper.y;

	//the boundary
	pMesh3d->AddVertex(new CVertex3d(lowerx,lowery,0));
	pMesh3d->AddVertex(new CVertex3d(upperx,lowery,0));
	std::pair<int,int> data;
	data.first = pMesh3d->NbVertex() - 2;
	data.second = pMesh3d->NbVertex() - 1;
	pMesh3d->m_ArraySharpEdges.push_back(data);

	pMesh3d->AddVertex(new CVertex3d(lowerx,lowery,0));
	pMesh3d->AddVertex(new CVertex3d(lowerx,uppery,0));
	data.first = pMesh3d->NbVertex() - 2;
	data.second = pMesh3d->NbVertex() - 1;
	pMesh3d->m_ArraySharpEdges.push_back(data);

	pMesh3d->AddVertex(new CVertex3d(upperx,uppery,0));
	pMesh3d->AddVertex(new CVertex3d(lowerx,uppery,0));
	data.first = pMesh3d->NbVertex() - 2;
	data.second = pMesh3d->NbVertex() - 1;
	pMesh3d->m_ArraySharpEdges.push_back(data);

	pMesh3d->AddVertex(new CVertex3d(upperx,uppery,0));
	pMesh3d->AddVertex(new CVertex3d(upperx,lowery,0));
	data.first = pMesh3d->NbVertex() - 2;
	data.second = pMesh3d->NbVertex() - 1;
	pMesh3d->m_ArraySharpEdges.push_back(data);

	AddMeshEdge_Edges(pMesh3d,proot,lowerx,lowery,upperx,uppery);
	return pMesh3d;
}
void KDTree::AddMeshEdge_Points(CMesh3d* pMesh3d,KDTree_node* pnode,double lowerx,double lowery,double upperx,double uppery)
{
	if(!pnode)
		return;

	if(pnode->axis == 0)
	{
		double x = pnode->point.x;
		pMesh3d->AddVertex(new CVertex3d(x,lowery,0));
		pMesh3d->AddVertex(new CVertex3d(x,uppery,0));
		std::pair<int,int> data;
		data.first = pMesh3d->NbVertex() - 2;
		data.second = pMesh3d->NbVertex() - 1;
		pMesh3d->m_ArraySharpEdges.push_back(data);
		AddMeshEdge_Points(pMesh3d,pnode->leftchild,lowerx,lowery,x,uppery);
		AddMeshEdge_Points(pMesh3d,pnode->rightchild,x,lowery,upperx,uppery);	
	}else
	{
		double y = pnode->point.y;
		pMesh3d->AddVertex(new CVertex3d(lowerx,y,0));
		pMesh3d->AddVertex(new CVertex3d(upperx,y,0));
		std::pair<int,int> data;
		data.first = pMesh3d->NbVertex() - 2;
		data.second = pMesh3d->NbVertex() - 1;
		pMesh3d->m_ArraySharpEdges.push_back(data);
		AddMeshEdge_Points(pMesh3d,pnode->leftchild,lowerx,lowery,upperx,y);
		AddMeshEdge_Points(pMesh3d,pnode->rightchild,lowerx,y,upperx,uppery);	
	}
}
void KDTree::AddMeshEdge_Edges(CMesh3d* pMesh3d,KDTree_node* pnode,double lowerx,double lowery,double upperx,double uppery)
{
	if(!pnode)
		return;

	if(pnode->axis == 0)
	{
		double x = pnode->val;
		pMesh3d->AddVertex(new CVertex3d(x,lowery,0));
		pMesh3d->AddVertex(new CVertex3d(x,uppery,0));
		std::pair<int,int> data;
		data.first = pMesh3d->NbVertex() - 2;
		data.second = pMesh3d->NbVertex() - 1;
		pMesh3d->m_ArraySharpEdges.push_back(data);
		AddMeshEdge_Edges(pMesh3d,pnode->leftchild,lowerx,lowery,x,uppery);
		AddMeshEdge_Edges(pMesh3d,pnode->rightchild,x,lowery,upperx,uppery);	
	}else
	{
		double y = pnode->val;
		pMesh3d->AddVertex(new CVertex3d(lowerx,y,0));
		pMesh3d->AddVertex(new CVertex3d(upperx,y,0));
		std::pair<int,int> data;
		data.first = pMesh3d->NbVertex() - 2;
		data.second = pMesh3d->NbVertex() - 1;
		pMesh3d->m_ArraySharpEdges.push_back(data);
		AddMeshEdge_Edges(pMesh3d,pnode->leftchild,lowerx,lowery,upperx,y);
		AddMeshEdge_Edges(pMesh3d,pnode->rightchild,lowerx,y,upperx,uppery);	
	}
}
void Action_PolyOmino::OnBnClickedButtonKdtreeIteration()
{
	// TODO: Add your control notification handler code here
	ConstructKdTreeFromEdges();
}

void Action_PolyOmino::OnBnClickedPolyominoOpenmesh()
{
	// TODO: Add your control notification handler code here
	//Generate An Openmesh from the current mesh
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_SceneGraph.NbObject() == 0)
		return;
	if(pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
		//Generate Openmesh
		CMesh3d_OpenMesh* pOpenMesh = pMesh->Convert_OpenMesh();
		pOpenMesh->m_FixVerticesArray.push_back(0);
		pOpenMesh->m_FixVerticesArray.push_back(1);
		pOpenMesh->m_FixVerticesArray.push_back(2);
		pOpenMesh->m_FixVerticesArray.push_back(3);
		pOpenMesh->m_Transform.Copy(*pMesh->GetTransform());
		pDoc->m_SceneGraph.Add(pOpenMesh);
		Actions_HarmonicMapping::ComputeHarmonic_LinearConstraints(pOpenMesh);
		CMesh3d_OpenMesh* pOpenMesh1 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
		CMesh3d* pNewMesh3d = pOpenMesh1->ConvertMesh3d();
		pNewMesh3d->m_LandMarkArray = pMesh->m_LandMarkArray;
		pNewMesh3d->SetTransform(*pMesh->GetTransform());
		pDoc->m_SceneGraph.Add(pNewMesh3d);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}else
	if(pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1)->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
		//Generate Openmesh
		//CMesh3d_OpenMesh* pOpenMesh = pMesh->Convert_OpenMesh();
		pMesh->m_FixVerticesArray.push_back(0);
		pMesh->m_FixVerticesArray.push_back(1);
		pMesh->m_FixVerticesArray.push_back(2);
		pMesh->m_FixVerticesArray.push_back(3);
		//pOpenMesh->m_Transform.Copy(*pMesh->GetTransform());
		//pDoc->m_SceneGraph.Add(pOpenMesh);
		Actions_HarmonicMapping::ComputeHarmonic_LinearConstraints(pMesh);
		//pDoc->UpdateTreeControl();
		//pDoc->UpdateAllViews(NULL);
	}
}
void Action_PolyOmino::DeleteSmallPolygon(Shape_Polygon* pPolygon)
{
	Record* m_pRecord;
	double area;
	double minarea = 1e10;
	double maxarea = -1e10;
	for (int i = 0; i < pPolygon->m_ArrayRecords.size(); i++)
	{
		m_pRecord = pPolygon->m_ArrayRecords[i];
		for (int j = 0; j < m_pRecord->m_recordcontent.m_NumParts; j++)
		{
			area = m_pRecord->m_recordcontent.m_ArrayArea[j];
			if (area < minarea)
			{
				minarea = fabs(area);
			}
			if (area > maxarea)
			{
				maxarea = fabs(area);
			}
		}
	}
	double t = 0.000001;
	double areabounding = minarea * (1 - t) + maxarea * t;
	for (int i = 0; i < pPolygon->m_ArrayRecords.size(); i++)
	{
		m_pRecord = pPolygon->m_ArrayRecords[i];
		for (int j = 0; j < m_pRecord->m_recordcontent.m_NumParts; j++)
		{
			if (m_pRecord->m_recordcontent.m_ArrayArea.size() == 0)
			{
				//m_pRecord->m_recordcontent.m_ArrayArea.resize(m_pRecord->m_recordcontent.m_NumParts, 0);
				m_pRecord->m_recordcontent.m_ArrayFlag.resize(m_pRecord->m_recordcontent.m_NumParts, 0);
			}
			area = m_pRecord->m_recordcontent.m_ArrayArea[j];
			if (fabs(area) < areabounding)
			{
				m_pRecord->m_recordcontent.m_ArrayFlag[j] = 1;
			}
		}
	}
}
void Action_PolyOmino::SetPolygonRingIndex(Shape_Polygon* pPolygon)
{
	Record* m_pRecord;
	double area;
	int m_Ringindex = 0;
	for (int i = 0; i < pPolygon->m_ArrayRecords.size(); i++)
	{
		m_pRecord = pPolygon->m_ArrayRecords[i];
		for (int j = 0; j < m_pRecord->m_recordcontent.m_NumParts; j++)
		{
			if (m_pRecord->m_recordcontent.m_ArrayRingIndex.size() == 0)
			{
				m_pRecord->m_recordcontent.m_ArrayRingIndex.resize(m_pRecord->m_recordcontent.m_NumParts, 0);
				//m_pRecord->m_recordcontent.m_ArrayFlag.resize(m_pRecord->m_recordcontent.m_NumParts, 0);
			}
			m_pRecord->m_recordcontent.m_ArrayRingIndex[j] = m_Ringindex;
			m_Ringindex++;
		}
	}
}
CMesh3d* Action_PolyOmino::ExportMesh3D(Shape_Polygon* pPolygon)
{
	Record* m_pRecord;
	double area;
	CVertex3d* pVertex;
	std::pair<CVertex3d*, CVertex3d*> pair;
	std::vector<CVertex3d*> m_VertexArray;
	std::vector<std::pair<CVertex3d*, CVertex3d*>> m_ArraySharpEdges;
	CMesh3d* pMesh1 = new CMesh3d();
	for (int i = 0; i < pPolygon->m_ArrayRecords.size(); i++)
	{
		m_pRecord = pPolygon->m_ArrayRecords[i];
		for (int j = 0; j < m_pRecord->m_recordcontent.m_NumParts; j++)
		{
			if (m_pRecord->m_recordcontent.m_ArrayFlag[j])
				continue;

			int firstvindex, endvindex;
			firstvindex = m_pRecord->m_recordcontent.m_ArrayParts[j];
			if (j < m_pRecord->m_recordcontent.m_NumParts - 1)
				endvindex = m_pRecord->m_recordcontent.m_ArrayParts[j + 1];
			else
				endvindex = m_pRecord->m_recordcontent.m_ArrayPoints.size();
			int firstMeshVindex = pMesh1->m_ArrayVertex.GetSize();

			//Add Vertex
			m_VertexArray.clear();
			for (int k = firstvindex; k < endvindex; k++)
			{
				pVertex = pMesh1->AddVertex_NODUPLICATE(m_pRecord->m_recordcontent.m_ArrayPoints[k].x, m_pRecord->m_recordcontent.m_ArrayPoints[k].y, 0);
				m_VertexArray.push_back(pVertex);
			}

			//Add Sharp Edge
			for (int k = firstvindex; k < endvindex - 1; k++)
			{
				int newvstartindex = k - firstvindex;
				int newvendindex = newvstartindex + 1;
				pair.first = m_VertexArray[newvstartindex];
				pair.second = m_VertexArray[newvendindex];
				m_ArraySharpEdges.push_back(pair);
			}
		}
	}

	//Add the sharpe edges to the mesh3d
	pMesh1->SetFlagOnVertices_ArrayIndex();

	std::pair<int, int> pair1;
	for (int i = 0; i < m_ArraySharpEdges.size(); i++)
	{
		pair1.first = m_ArraySharpEdges[i].first->m_Flag;
		pair1.second = m_ArraySharpEdges[i].second->m_Flag;
		pMesh1->m_ArraySharpEdges.push_back(pair1);
	}

	return pMesh1;
}
void Action_PolyOmino::ComputePolygonRingArea(Shape_Polygon* pPolygon)
{
	Record* m_pRecord;
	double area;
	for (int i = 0; i < pPolygon->m_ArrayRecords.size(); i++)
	{
		m_pRecord = pPolygon->m_ArrayRecords[i];
		for (int j = 0; j < m_pRecord->m_recordcontent.m_NumParts; j++)
		{
			if (m_pRecord->m_recordcontent.m_ArrayArea.size() == 0)
			{
				m_pRecord->m_recordcontent.m_ArrayArea.resize(m_pRecord->m_recordcontent.m_NumParts, 0);
				//m_pRecord->m_recordcontent.m_ArrayFlag.resize(m_pRecord->m_recordcontent.m_NumParts, 0);
			}
			area = pPolygon->polygonArea(m_pRecord, j);
			m_pRecord->m_recordcontent.m_ArrayArea[j] = area;
		}
	}
}
void Action_PolyOmino::LoadNanJingZhouRings()
{
	std::ifstream file("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\nanjizhou.txt");

	int index;
	std::vector<int> m_ArrayDeleteRings;
	while (!file.eof())
	{
		file >> index;
		m_ArrayDeleteRings.push_back(index);
	}
	Record* m_pRecord;
	double area;

	for (int i = 0; i < pPolygon->m_ArrayRecords.size(); i++)
	{
		m_pRecord = pPolygon->m_ArrayRecords[i];
		for (int j = 0; j < m_pRecord->m_recordcontent.m_NumParts; j++)
		{
			int currentringindex = m_pRecord->m_recordcontent.m_ArrayRingIndex[j];

			std::vector<int>::iterator fiterator;
			fiterator = std::find(m_ArrayDeleteRings.begin(), m_ArrayDeleteRings.end(), currentringindex);
			if (fiterator == m_ArrayDeleteRings.end())
			{
				m_pRecord->m_recordcontent.m_ArrayFlag[j] = 1;
			}
		}
	}
	file.close();
}
void Action_PolyOmino::LoadDeleteRings()
{
	std::ifstream file("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\deletesmallrings.txt");

	int index;
	std::vector<int> m_ArrayDeleteRings;
	while (!file.eof())
	{
		file >> index;
		m_ArrayDeleteRings.push_back(index);
	}
	Record* m_pRecord;
	double area;
	
	for (int i = 0; i < pPolygon->m_ArrayRecords.size(); i++)
	{
		m_pRecord = pPolygon->m_ArrayRecords[i];
		for (int j = 0; j < m_pRecord->m_recordcontent.m_NumParts; j++)
		{
			int currentringindex = m_pRecord->m_recordcontent.m_ArrayRingIndex[j];

			std::vector<int>::iterator fiterator;
			fiterator = std::find(m_ArrayDeleteRings.begin(), m_ArrayDeleteRings.end(), currentringindex);
			if (fiterator != m_ArrayDeleteRings.end())
			{
				m_pRecord->m_recordcontent.m_ArrayFlag[j] = 1;
			}
		}
	}
	file.close();
}
void Action_PolyOmino::LoadShape_Polygon()
{
	//Load Shape Polygon
	std::ifstream file("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries.shp", std::ios::in | std::ios::binary);
	
	pPolygon = new Shape_Polygon();
	int filelength;
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_FileCode), sizeof(int));
	pPolygon->header.m_FileCode = bswap32(pPolygon->header.m_FileCode);
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_ArrayUnused[0]), sizeof(int));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_ArrayUnused[1]), sizeof(int));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_ArrayUnused[2]), sizeof(int));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_ArrayUnused[3]), sizeof(int));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_ArrayUnused[4]), sizeof(int));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_FileLength), sizeof(int));
	pPolygon->header.m_FileLength = bswap32(pPolygon->header.m_FileLength);
	filelength = pPolygon->header.m_FileLength;
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_Version), sizeof(int));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_ShapeType), sizeof(int));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_boundingbox_xmin), sizeof(double));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_boundingbox_ymin), sizeof(double));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_boundingbox_xmax), sizeof(double));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_boundingbox_ymax), sizeof(double));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_boundingbox_zmin), sizeof(double));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_boundingbox_zmax), sizeof(double));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_boundingbox_mmin), sizeof(double));
	file.read(reinterpret_cast<char*>(&pPolygon->header.m_boundingbox_mmax), sizeof(double));
	filelength = filelength - 50;

	Record* m_pRecord;
	//Shape_Polygon* pPolygon;
	//int m_RecordNumber;
	//int m_ContentLength;
	while (!file.eof())
	{
		//Add the polygon
		//pPolygon = new Shape_Polygon();
		//pPolygonArray.push_back(pPolygon);
		m_pRecord = new Record;
		//ContentHeader
		file.read(reinterpret_cast<char*>(&m_pRecord->m_recordheader.m_recordnumber), sizeof(int));
		m_pRecord->m_recordheader.m_recordnumber = bswap32(m_pRecord->m_recordheader.m_recordnumber);
		file.read(reinterpret_cast<char*>(&m_pRecord->m_recordheader.m_contentlength), sizeof(int));
		m_pRecord->m_recordheader.m_contentlength = bswap32(m_pRecord->m_recordheader.m_contentlength);

		filelength = filelength - m_pRecord->m_recordheader.m_contentlength - 4;
		
		//Read Polygon Header Information
		file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_ShapeTye), sizeof(int));
		if (m_pRecord->m_recordcontent.m_ShapeTye == 5)
		{
			file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_BoundingBox[0]), sizeof(double));
			file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_BoundingBox[1]), sizeof(double));
			file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_BoundingBox[2]), sizeof(double));
			file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_BoundingBox[3]), sizeof(double));
			file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_NumParts), sizeof(int));
			file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_NumPoints), sizeof(int));

			m_pRecord->m_recordcontent.m_ArrayParts.resize(m_pRecord->m_recordcontent.m_NumParts);
			m_pRecord->m_recordcontent.m_ArrayFlag.resize(m_pRecord->m_recordcontent.m_NumParts,0);
			m_pRecord->m_recordcontent.m_ArrayPoints.resize(m_pRecord->m_recordcontent.m_NumPoints);

			//Read Parts
			for (int i = 0; i < m_pRecord->m_recordcontent.m_NumParts; i++)
			{
				file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_ArrayParts[i]), sizeof(int));
			}

			//Read Points
			for (int i = 0; i < m_pRecord->m_recordcontent.m_NumPoints; i++)
			{
				file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_ArrayPoints[i].x), sizeof(double));
				file.read(reinterpret_cast<char*>(&m_pRecord->m_recordcontent.m_ArrayPoints[i].y), sizeof(double));
			}
			pPolygon->m_ArrayRecords.push_back(m_pRecord);
		}
		else
		{
			int ii = 0;
		}
	}
	file.close();
	//Set Polygon Ring Index
	SetPolygonRingIndex(pPolygon);

	//Compute the area for all the rings
	ComputePolygonRingArea(pPolygon);

	//delete small rings
	//DeleteSmallPolygon(pPolygon);

	//LoadDeletedRings
	//LoadDeleteRings();
	LoadNanJingZhouRings();

	//ExporttoMesh3D
	CMesh3d* pMesh3D = ExportMesh3D(pPolygon);

	double xmin = pPolygon->header.m_boundingbox_xmin;
	double ymin = pPolygon->header.m_boundingbox_ymin;
	double xmax = pPolygon->header.m_boundingbox_xmax;
	double ymax = pPolygon->header.m_boundingbox_ymax;
	double zmin = 0;
	double zmax = 0;

	CVector3d m_lower, m_upper;
	m_lower.Set(xmin, ymin, zmin);
	m_upper.Set(xmax, ymax, zmax);
	//CTransform transform;
	pPolygon->m_Transform.Clear();
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation = m_lower + m_upper;
	m_translation /= -2;
	float length = m_upper.x() - m_lower.x();
	float length1 = m_upper.y() - m_lower.y();
	if (length < m_upper.y() - m_lower.y())
		length = m_upper.y() - m_lower.y();
	if (length < m_upper.z() - m_lower.z())
		length = m_upper.z() - m_lower.z();

	pPolygon->m_Transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1 / length, 1 / length, 1 / length);
	pPolygon->m_Transform.SetScale(&m_Scale);
	//pMesh->SetTransform(transform);
	pDoc->m_SceneGraph.Add(pPolygon);

	//Add Mesh3D
	pMesh3D->SetTransform(pPolygon->m_Transform);
	pDoc->m_SceneGraph.Add(pMesh3D);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::GetLatestWorldMapIndex()
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	StringCchCopy(szDir, MAX_PATH, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\maps");
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		m_LatestWorldMapIndex = -1;
		return;
	}

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString filename = ffd.cFileName;
			int currentfileindex;
			sscanf(filename,"worldmaptriangulation_coarse_more%d",&currentfileindex);
			if (currentfileindex > m_LatestWorldMapIndex)
				m_LatestWorldMapIndex = currentfileindex;
		}
	} while (FindNextFile(hFind, &ffd) != 0);
}
void Action_PolyOmino::GetLatestLandmarkIndex()
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	StringCchCopy(szDir, MAX_PATH, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmarks");
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		m_LatestLandmarkIndex = -1;
		return;
	}

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString filename = ffd.cFileName;
			int currentfileindex;
			sscanf(filename, "landmark_coarse_more_%d", &currentfileindex);
			if (currentfileindex > m_LatestLandmarkIndex)
				m_LatestLandmarkIndex = currentfileindex;
		}
	} while (FindNextFile(hFind, &ffd) != 0);
}
void Action_PolyOmino::GetLatestSelectedLandmarkIndex()
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	StringCchCopy(szDir, MAX_PATH, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\selectedlandmarks");
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		m_LatestSelectedLandmarkIndex = -1;
		return;
	}

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString filename = ffd.cFileName;
			int currentfileindex;
			sscanf(filename, "selectedlandmark%d", &currentfileindex);
			if (currentfileindex > m_LatestSelectedLandmarkIndex)
				m_LatestSelectedLandmarkIndex = currentfileindex;
		}
	} while (FindNextFile(hFind, &ffd) != 0);
}
void Action_PolyOmino::GetLatestWorldMap()
{
	if (m_LatestWorldMapIndex == -1)
		GetLatestWorldMapIndex();
	char filename[180];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\maps\\worldmaptriangulation_coarse_more%d.m", m_LatestWorldMapIndex);
	pDoc->OnOpenDocument(filename);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	OnBnClickedOminoSetcurrentmesh();
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pMesh->m_showFace = FALSE;
	pMesh->m_showSelectedSharpVertex = TRUE;
	pMesh->m_showSelectedLandmarkEdge = TRUE;
	OnBnClickedGeneratelandmarks();

	//
}
void Action_PolyOmino::SaveCurrentRectilinearWorldMap()
{
	char filename[180];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\finalrectilinearmap\\worldmaptriangulation_coarse_more253.m");

	//
	pFinalRectilinearMesh->SaveMFile(filename);
}
void Action_PolyOmino::SaveCurrentWorldMap()
{
	if (m_LatestWorldMapIndex == -1)
		GetLatestWorldMapIndex();

	m_LatestWorldMapIndex++;
	char filename[180];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\maps\\worldmaptriangulation_coarse_more%d.m", m_LatestWorldMapIndex);

	//
	pMesh->SaveMFile(filename);
}

void Action_PolyOmino::GetLatestLandmark()
{
	if (m_LatestLandmarkIndex == -1)
		GetLatestLandmarkIndex();

	char message[180];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmarks\\landmark_coarse_more_%d.txt", m_LatestLandmarkIndex);
	CString landmarkname = message;
	Loadlandmark(landmarkname);

	pMesh->SetVertexFlagAsZero();
}
void Action_PolyOmino::SaveCurrentLandmak()
{
	if (m_LatestLandmarkIndex == -1)
		GetLatestLandmarkIndex();

	m_LatestLandmarkIndex++;
	char filename[180];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmarks\\landmark_coarse_more_%d.txt", m_LatestLandmarkIndex);

	//
	SaveLandmark(filename);
}

void Action_PolyOmino::GetLatestSelectedLandmark()
{
	if (m_LatestSelectedLandmarkIndex == -1)
		GetLatestSelectedLandmarkIndex();

	char message[180];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\selectedlandmarks\\selectedlandmark%d.txt", m_LatestSelectedLandmarkIndex);
	LoadSelectedPoints(message);
}
void Action_PolyOmino::SaveCurrentSelectedLandmark()
{
	if (m_LatestSelectedLandmarkIndex == -1)
		GetLatestSelectedLandmarkIndex();

	m_LatestSelectedLandmarkIndex++;
	char filename[180];
	sprintf(filename, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\selectedlandmarks\\selectedlandmark%d.txt", m_LatestSelectedLandmarkIndex);

	//
	SaveSelectedSharpVertices(filename);
}
void Action_PolyOmino::OnBnClickedButtonLoadNanjizhoumap_Triangulation()
{
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\nanjizhou\\nanjizhou1.m");
	OnBnClickedOminoSetcurrentmesh();
	char message[200];
	sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\nanjizhou\\nanjizhoulandmark1.txt");
	CString landmarkname = message;
	Loadlandmark(landmarkname);

	pMesh->SetVertexFlagAsZero();
}
void Action_PolyOmino::OnBnClickedButtonLoadworldmap_Triangulation()
{
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_dense.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_coarse.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_coarse_more.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_coarse_more2.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_coarse_more4.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_coarse_more5.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_coarse_more6.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_coarse_more7.m");
	//pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmaptriangulation_coarse_more8.m");

	//find the latest saved map
	GetLatestWorldMapIndex();

	//find the latest saved landmark
	GetLatestLandmarkIndex();

	//find the latest selected landmark index
	GetLatestSelectedLandmarkIndex();

	//GetLatestWorldMap
	GetLatestWorldMap();

	//GetLatestLandmark
	GetLatestLandmark();

	//Load Landmarks
	//char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_dense1.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse_more_1.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse_more_2.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse_more_4.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse_more_5.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse_more_6.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse_more_7.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse_more_8.txt");
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\landmark_coarse_more_73.txt");
	//CString landmarkname = message;
	//Loadlandmark(landmarkname);

	//pMesh->SetVertexFlagAsZero();

	//Duplicate Vertices
	//DeleteDuplicateVertices_Mesh3d(pMesh);

	//Duplicate landmarks
	//DeleteDuplicateLandmarks(pMesh);
}
void Action_PolyOmino::DeleteDuplicateSharpEdges()
{
	//
	UpdatePointConnectedSelectedEdgeIndex();
	std::vector<int> m_ArraySharpFlags;
	m_ArraySharpFlags.resize(pMesh->m_ArraySharpEdges.size(), 0);
	for (int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex.size(); i++)
	{
		std::vector<int> edgeset = m_ArrayPointConnectedSelectEdgeIndex[i];
		for (int j = 0; j < edgeset.size(); j++)
		{
			for (int k = j + 1; k < edgeset.size(); k++)
			{
				int edgeindex1 = edgeset[j];
				int edgeindex2 = edgeset[k];
				if (pMesh->m_ArraySharpEdges[edgeindex1].first == pMesh->m_ArraySharpEdges[edgeindex2].first &&
					pMesh->m_ArraySharpEdges[edgeindex1].second == pMesh->m_ArraySharpEdges[edgeindex2].second ||
					pMesh->m_ArraySharpEdges[edgeindex1].first == pMesh->m_ArraySharpEdges[edgeindex2].second &&
					pMesh->m_ArraySharpEdges[edgeindex1].second == pMesh->m_ArraySharpEdges[edgeindex2].first)
				{
					m_ArraySharpFlags[edgeindex1] = 1;
					break;
				}
			}
		}
	}

	//delete all the duplicate sharp edges
	for (int i = pMesh->m_ArraySharpEdges.size() - 1; i >= 0; i--)
	{
		if (m_ArraySharpFlags[i])
		{
			pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
		}
	}
}
void Action_PolyOmino::OnBnClickedButtonLoadworldmap()
{
	//OnBnClickedButtonLoadNanjizhoumap_Triangulation();
	//return;
	//OnBnClickedButtonLoadworldmap_Triangulation();
	//return;
	//Load original data and generate the .m mesh
	//LoadShape_Polygon();
	//return;

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\worldmap.m");
	OnBnClickedOminoSetcurrentmesh();
	pUniformGridMesh = pMesh;
	//Delete Duplicate Sharp edges
	DeleteDuplicateSharpEdges();

	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	OnBnClickedOminoSetcurrentmesh();
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pMesh->m_showFace = FALSE;

	OnBnClickedGeneratelandmarks();

	//Load Landmarks
	char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");

	//CString landmarkname = message;
	//Loadlandmark(landmarkname);
	return;
	// TODO: Add your control notification handler code here
	//pDoc->OnOpenDocument("D:\\Razib\\lamcc_smooth_org_normalize.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\worldmap3.m");
	//pDoc->OnOpenDocument("D:\\41.obj");
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	OnBnClickedOminoSetcurrentmesh();
	pMesh->m_showLandmarkEdge = TRUE;
	pMesh->m_showLandmarkVertex = TRUE;
	pMesh->m_showFace = FALSE;

	OnBnClickedGeneratelandmarks();

	//Load Landmarks
	//char message[200];
	//sprintf(message, "D:\\papers\\code\\data\\Geo\\dense\\Dense_mesh_Landmark.txt");

	//CString landmarkname = message;
	//Loadlandmark(landmarkname);
}

void Action_PolyOmino::AssignLandmarkToSharpEdges()
{
	//
	pMesh->m_ArraySharpEdges.clear();

	std::pair<int, int> pair;
	//
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size() - 1; j++)
		{
			pair.first = pLandmark->landmark_points[j];
			pair.second = pLandmark->landmark_points[j + 1];
			pMesh->m_ArraySharpEdges.push_back(pair);
		}
	}
}
void Action_PolyOmino::OnBnClickedButtonSavesimplifiedmfile()
{
	// TODO: Add your control notification handler code here
	pUniformGridMesh = pMesh;

	//
	AssignLandmarkToSharpEdges();

	//Delete UnusedVertices
	DeleteUnusedVertices();

	//
	pMesh->SaveMFile("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\worldmap3.m");
}


void Action_PolyOmino::OnBnClickedButtonSaveselectedPoints()
{
	CString FileName;
	CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;

	SaveSelectedSharpVertices(FileName);
	//SaveCurrentSelectedLandmark();
}
void Action_PolyOmino::SaveSelectedLandmarks(CString FileName)
{
	std::ofstream file(FileName);

	file << pMesh->m_SelectedLandmarkIndexArray.size() << "\n";

	for (int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
	{
		file << pMesh->m_SelectedLandmarkIndexArray[i] << "\n";
	}
	file.close();
}
void Action_PolyOmino::SaveSelectedSharpVertices(CString FileName)
{
	std::ofstream file(FileName);

	file << pMesh->m_ArraySharpVertexIndex.size() << "\n";

	for (int i = 0; i < pMesh->m_ArraySharpVertexIndex.size(); i++)
	{
		file << pMesh->m_ArraySharpVertexIndex[i] << "\n";
	}
	file.close();
}
void Action_PolyOmino::LoadSelectedLandmarks(CString FileName)
{
	std::ifstream file(FileName);

	int n, index;
	file >> n;
	//pMesh->m_ArraySharpVertexIndex.resize(n);

	for (int i = 0; i < n; i++)
	{
		file >> index;
		pMesh->m_SelectedLandmarkIndexArray.push_back(index);
	}
	file.close();
}
void Action_PolyOmino::LoadSelectedSharpVertices(CString FileName)
{
	std::ifstream file(FileName);

	int n,index;
	file >> n;
	//pMesh->m_ArraySharpVertexIndex.resize(n);

	for (int i = 0; i < n; i++)
	{
		file >> index;
		pMesh->m_ArraySharpVertexIndex.push_back(index);
	}
	file.close();
}
void Action_PolyOmino::OnBnClickedButtonLoadselectedPoints()
{
	CString FileName;
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;

	LoadSelectedSharpVertices(FileName);
	//
	//GetLatestSelectedLandmark();
}
void Action_PolyOmino::OnlyKeepSelectedLandmarks(CMesh3d* pMesh,CMesh3d_OpenMesh* pOpenMesh)
{
	for (int i = 0; i < pOpenMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pOpenMesh->m_LandMarkArray[i];
		pLandmark->deleteflag = 1;
	}
	for (int i = 0; i < pOpenMesh->m_SelectedLandmarkIndexArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pOpenMesh->m_LandMarkArray[pOpenMesh->m_SelectedLandmarkIndexArray[i]];
		pLandmark->deleteflag = 0;
	}
	for (int i = 0; i < pOpenMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pOpenMesh->m_LandMarkArray[i];
		if (pLandmark->deleteflag)
		{
			pOpenMesh->m_LandMarkArray.erase(pOpenMesh->m_LandMarkArray.begin() + i);
			i--;
			//delete pLandmark;
		}
	}
	pOpenMesh->m_SelectedLandmarkIndexArray.clear();
	pMesh->m_SelectedLandmarkIndexArray.clear();
}

void Action_PolyOmino::OnBnClickedPolyominoCurvestraightSelected()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	int num = pDoc->m_SceneGraph.NbObject();
	if (p_FormView3->currentindex < 0 || p_FormView3->currentindex >= num)
	{
		AfxMessageBox("No mesh is selected!");
		return;
	}
	CMesh3d_OpenMesh* pOpenMesh;
	CMesh3d* pMesh3d;

	if (pDoc->m_SceneGraph[p_FormView3->currentindex]->GetType() == TYPE_MESH3D_OPENMESH)
	{
		pOpenMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph[p_FormView3->currentindex];
	}else if (pDoc->m_SceneGraph[p_FormView3->currentindex]->GetType() == TYPE_MESH3D)
	{
		pMesh3d = (CMesh3d*)pDoc->m_SceneGraph[p_FormView3->currentindex];
		pOpenMesh = pMesh->Convert_OpenMesh();
	}

	//Fix four boundary vertices
	pOpenMesh->m_FixVerticesArray.push_back(0);
	pOpenMesh->m_FixVerticesArray.push_back(1);
	pOpenMesh->m_FixVerticesArray.push_back(2);
	pOpenMesh->m_FixVerticesArray.push_back(3);

	//Set uv
	/*pOpenMesh->m_FixVerticesArray_uv.push_back(-182);
	pOpenMesh->m_FixVerticesArray_uv.push_back(-57.6118);
	pOpenMesh->m_FixVerticesArray_uv.push_back(182);
	pOpenMesh->m_FixVerticesArray_uv.push_back(-57.6118);
	pOpenMesh->m_FixVerticesArray_uv.push_back(182);
	pOpenMesh->m_FixVerticesArray_uv.push_back(85.6451);
	pOpenMesh->m_FixVerticesArray_uv.push_back(-182);
	pOpenMesh->m_FixVerticesArray_uv.push_back(85.6451);*/

	/*pOpenMesh->m_FixVerticesArray_uv.push_back(-182);
	pOpenMesh->m_FixVerticesArray_uv.push_back(-92);
	pOpenMesh->m_FixVerticesArray_uv.push_back(182);
	pOpenMesh->m_FixVerticesArray_uv.push_back(-92);
	pOpenMesh->m_FixVerticesArray_uv.push_back(182);
	pOpenMesh->m_FixVerticesArray_uv.push_back(-61.2707);
	pOpenMesh->m_FixVerticesArray_uv.push_back(-182);
	pOpenMesh->m_FixVerticesArray_uv.push_back(-61.2707);*/
	pOpenMesh->m_FixVerticesArray_uv.push_back(71.3952);
	pOpenMesh->m_FixVerticesArray_uv.push_back(16.1618);
	pOpenMesh->m_FixVerticesArray_uv.push_back(136.8);
	pOpenMesh->m_FixVerticesArray_uv.push_back(16.1618);
	pOpenMesh->m_FixVerticesArray_uv.push_back(136.8);
	pOpenMesh->m_FixVerticesArray_uv.push_back(55.4949);
	pOpenMesh->m_FixVerticesArray_uv.push_back(71.3952);
	pOpenMesh->m_FixVerticesArray_uv.push_back(55.4949);

	//pHarmonicMapping->HarmonicStraightening_Selected(pOpenMesh, 0);

	//
	OnlyKeepSelectedLandmarks(pMesh3d,pOpenMesh);

	//fix the boundary
	pHarmonicMapping->Compute_Boundary_Vertices_Parameter_Positions_Quad(pOpenMesh);

	//fix the inner points
	//pHarmonicMapping->FixInnerPoints(pOpenMesh);

	//linear constraints
	//pHarmonicMapping->Compute_Linear_Equations_LinearConstraints_SingleVariable(pOpenMesh,0);
	pHarmonicMapping->Compute_Linear_Equations_LinearConstraints_SingleVariable_Selected(pOpenMesh, 0);

	//Generate the straighteing
	pHarmonicMapping->GenerateParameterDomain(pOpenMesh);
	//Compute_Boundary_Vertices_Parameter_Positions_LinearConstraints(pOpenMesh);

	//Obtain the linear equations
	//Compute_Linear_Equations_LinearConstraints(pOpenMesh);

	//Generate a new parameter domain mesh
	//GenerateParameterDomain(pOpenMesh);
}

void Action_PolyOmino::SubdivideMeshEdge(CMesh3d* pMesh, int m_Edge_Subdivision_Num)
{
	//Must Select two vertices.
	if (pMesh->m_ArraySharpVertexIndex.size() != 2)
		return;

	pMesh->SetVertexFlagAsItsIndex();

	CVertex3d* pVertex1 = pMesh->GetVertex(pMesh->m_ArraySharpVertexIndex[0]);
	CVertex3d* pVertex2 = pMesh->GetVertex(pMesh->m_ArraySharpVertexIndex[1]);

	CVertex3d* pVertex3, *pVertex4;
	CFace3d* pFace1 = pMesh->FindFace(pVertex1, pVertex2, pVertex3);
	CFace3d* pFace2 = pMesh->FindFace(pVertex2, pVertex1, pVertex4);

	//Delete the two vertices
	pMesh->DeleteFace(pFace1);
	pMesh->DeleteFace(pFace2);

	//Add Vertices
	std::vector<CVertex3d*> m_ArrayNewVertices;
	m_ArrayNewVertices.resize(m_Edge_Subdivision_Num + 1);
	m_ArrayNewVertices[0] = pVertex1;
	m_ArrayNewVertices[m_Edge_Subdivision_Num] = pVertex2;
	double x, y, z;
	CVertex3d* pNewVertex;
	for (int i = 1; i < m_Edge_Subdivision_Num; i++)
	{
		double t = (i + 0.0) / m_Edge_Subdivision_Num;
		x = pVertex1->x() * (1 - t) + pVertex2->x() * t;
		y = pVertex1->y() * (1 - t) + pVertex2->y() * t;
		z = pVertex1->z() * (1 - t) + pVertex2->z() * t;

		pNewVertex = new CVertex3d(x, y, z);
		pMesh->AddVertex(pNewVertex);

		pFace1 = new CFace3d(pVertex3, m_ArrayNewVertices[i - 1], pNewVertex);
		pFace2 = new CFace3d(pNewVertex, m_ArrayNewVertices[i - 1], pVertex4);
		pMesh->AddFace(pFace1);
		pMesh->AddFace(pFace2);

		m_ArrayNewVertices[i] = pNewVertex;
	}

	//
	pFace1 = new CFace3d(pVertex3, m_ArrayNewVertices[m_Edge_Subdivision_Num - 1], m_ArrayNewVertices[m_Edge_Subdivision_Num]);
	pFace2 = new CFace3d(m_ArrayNewVertices[m_Edge_Subdivision_Num], m_ArrayNewVertices[m_Edge_Subdivision_Num - 1], pVertex4);
	pMesh->AddFace(pFace1);
	pMesh->AddFace(pFace2);

	pMesh->m_SetVertexFlagAsItsIndex = 0;
	pMesh->SetVertexFlagAsItsIndex();
	//Update Sharp Edge Array and Landmark Array
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		if (pair.first == pVertex1->m_Flag && pair.second == pVertex2->m_Flag ||
			pair.second == pVertex1->m_Flag && pair.first == pVertex2->m_Flag)
		{
			pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
			break;
		}
	}

	for (int i = 0; i < m_ArrayNewVertices.size() - 1; i++)
	{
		pair.first = m_ArrayNewVertices[i]->m_Flag;
		pair.second = m_ArrayNewVertices[i + 1]->m_Flag;
		pMesh->m_ArraySharpEdges.push_back(pair);
	}

	GeoMapLandMark* pLandmark = NULL;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark1 = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark1->landmark_points.size() - 1; j++)
		{
			int index1 = pLandmark1->landmark_points[j];
			int index2 = pLandmark1->landmark_points[j + 1];
			if (index1 == pVertex1->m_Flag && index2 == pVertex2->m_Flag)
			{
				pLandmark = pLandmark1;
				for (int k = m_ArrayNewVertices.size() - 2; k >= 1; k--)
				{
					pLandmark1->landmark_points.insert(pLandmark1->landmark_points.begin() + j + 1, m_ArrayNewVertices[k]->m_Flag);
				}
			}
			else if (index2 == pVertex1->m_Flag && index1 == pVertex2->m_Flag)
			{
				pLandmark = pLandmark1;
				for (int k = 1; k <= m_ArrayNewVertices.size() - 2; k++)
				{
					pLandmark1->landmark_points.insert(pLandmark1->landmark_points.begin() + j + 1, m_ArrayNewVertices[k]->m_Flag);
				}
			}
			if (pLandmark)
				break;
		}
		if (pLandmark)
			break;
	}
}
void Action_PolyOmino::OnBnClickedButtonSubdivideEdge()
{
	// TODO: Add your control notification handler code here
	Polyomino_Edge_Subdivision dialog;
	int m_Edge_Subdivision_Num;
	if (dialog.DoModal() == IDOK)
	{
		m_Edge_Subdivision_Num = dialog.m_EdgeSubdivisionNum;
	}

	//
	pFinalRectilinearMesh->m_ArraySharpVertexIndex = pMesh->m_ArraySharpVertexIndex;

	//
	SubdivideMeshEdge(pMesh, m_Edge_Subdivision_Num);

	//
	SubdivideMeshEdge(pFinalRectilinearMesh, m_Edge_Subdivision_Num);

	//SaveCurrentWorldMap
	SaveCurrentWorldMap();

	//
	SaveCurrentRectilinearWorldMap();

	pMesh->SetModified(1);
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedButtonSearchLandmark()
{
	// TODO: Add your control notification handler code here
	//Must Select one vertices.
	if (pMesh->m_ArraySharpVertexIndex.size() < 1)
		return;

	std::vector<int> m_CornerPointArray;
	m_CornerPointArray.resize(pMesh->m_ArrayVertex.GetSize(), 0);

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		int start = pLandmark->landmark_points[0];
		int end = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
		m_CornerPointArray[start]++;
		m_CornerPointArray[end]++;
	}

	std::vector<std::vector<int>> m_ArrayPointConnectedSelectEdgeIndex;
	m_ArrayPointConnectedSelectEdgeIndex.resize(pMesh->m_ArrayVertex.GetSize());
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{

		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];

		for (int j = 0; j < pLandmark->landmark_points.size(); j++)
		{
			int pointindex = pLandmark->landmark_points[j];
			m_ArrayPointConnectedSelectEdgeIndex[pointindex].push_back(i);
		}
		//int start = pLandmark->landmark_points[0];
		//int end = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
		//m_ArrayPointConnectedSelectEdgeIndex[start].push_back(i);
		//m_ArrayPointConnectedSelectEdgeIndex[end].push_back(i);
	}

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		pLandmark->flag = i;
	}

	std::vector<int> m_SelectedLandmarks;
	std::vector<GeoMapLandMark*> m_ArrayCurrentLandmarks;
	for (int j = 0; j < pMesh->m_ArraySharpVertexIndex.size(); j++)
	{
		int vindex = pMesh->m_ArraySharpVertexIndex[j];
		GeoMapLandMark* pCurrentLandmark = NULL;
		/*for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		{
			GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
			for (int j = 0; j < pLandmark->landmark_points.size(); j++)
			{
				if (pLandmark->landmark_points[j] == vindex)
				{
					pCurrentLandmark = pLandmark;
					m_ArrayCurrentLandmarks.push_back(pCurrentLandmark);
					break;
				}
			}
			//if (pCurrentLandmark)
			//	break;
		}*/

		for (int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex[vindex].size(); i++)
		{
			GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[m_ArrayPointConnectedSelectEdgeIndex[vindex][i]];
			m_ArrayCurrentLandmarks.push_back(pLandmark);
		}

		//
		if (m_ArrayCurrentLandmarks.size() == 0)
			return;

		int landmarkstart, landmarkend;
		for (int i = 0; i < m_ArrayCurrentLandmarks.size(); i++)
		{
			pCurrentLandmark = m_ArrayCurrentLandmarks[i];

			landmarkstart = pCurrentLandmark->landmark_points[0];
			landmarkend = pCurrentLandmark->landmark_points[pCurrentLandmark->landmark_points.size() - 1];

			m_SelectedLandmarks.clear();
			m_SelectedLandmarks.push_back(pCurrentLandmark->flag);
			//
			//Find all the corner

			//Trace the landmarks
			while (1)
			{
				if (m_ArrayPointConnectedSelectEdgeIndex[landmarkstart].size() == 2)
				{
					int landmark1 = m_ArrayPointConnectedSelectEdgeIndex[landmarkstart][0];
					int landmark2 = m_ArrayPointConnectedSelectEdgeIndex[landmarkstart][1];
					int nextlandmark = landmark1;
					if (std::find(m_SelectedLandmarks.begin(), m_SelectedLandmarks.end(), landmark1) != m_SelectedLandmarks.end())
						nextlandmark = landmark2;

					if (std::find(m_SelectedLandmarks.begin(), m_SelectedLandmarks.end(), nextlandmark) != m_SelectedLandmarks.end())
						break;

					GeoMapLandMark* pNextLandmark = pMesh->m_LandMarkArray[nextlandmark];
					if (pNextLandmark->landmark_points[0] == landmarkstart)
						landmarkstart = pNextLandmark->landmark_points[pNextLandmark->landmark_points.size() - 1];
					else
						landmarkstart = pNextLandmark->landmark_points[0];
					m_SelectedLandmarks.push_back(nextlandmark);
				}
				else
				{
					break;
				}
			}

			while (1)
			{
				if (m_ArrayPointConnectedSelectEdgeIndex[landmarkend].size() == 2)
				{
					int landmark1 = m_ArrayPointConnectedSelectEdgeIndex[landmarkend][0];
					int landmark2 = m_ArrayPointConnectedSelectEdgeIndex[landmarkend][1];
					int nextlandmark = landmark1;
					if (std::find(m_SelectedLandmarks.begin(), m_SelectedLandmarks.end(), landmark1) != m_SelectedLandmarks.end())
						nextlandmark = landmark2;

					if (std::find(m_SelectedLandmarks.begin(), m_SelectedLandmarks.end(), nextlandmark) != m_SelectedLandmarks.end())
						break;

					GeoMapLandMark* pNextLandmark = pMesh->m_LandMarkArray[nextlandmark];
					if (pNextLandmark->landmark_points[0] == landmarkend)
						landmarkend = pNextLandmark->landmark_points[pNextLandmark->landmark_points.size() - 1];
					else
						landmarkend = pNextLandmark->landmark_points[0];
					m_SelectedLandmarks.push_back(nextlandmark);
				}
				else
				{
					break;
				}
			}

			for (int ii = 0; ii < m_SelectedLandmarks.size(); ii++)
			{
				if (std::find(pMesh->m_SelectedLandmarkIndexArray.begin(), pMesh->m_SelectedLandmarkIndexArray.end(), m_SelectedLandmarks[ii]) == pMesh->m_SelectedLandmarkIndexArray.end())
				{
					pMesh->m_SelectedLandmarkIndexArray.push_back(m_SelectedLandmarks[ii]);
				}
			}
		}
	}

	//
	SaveCurrentSelectedLandmark();
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::SubdivideEdgesSloppingHorizontal(CMesh3d* pMesh, int m_Edge_Subdivision_Num)
{
	//Must Select two vertices.
	if (pMesh->m_ArraySharpVertexIndex.size() != 2)
		return;

	pMesh->SetVertexFlagAsItsIndex();

	CVertex3d* pVertex1 = pMesh->GetVertex(pMesh->m_ArraySharpVertexIndex[0]);
	CVertex3d* pVertex2 = pMesh->GetVertex(pMesh->m_ArraySharpVertexIndex[1]);

	double deltax = fabs(pVertex2->x() - pVertex1->x());
	double deltay = fabs(pVertex2->y() - pVertex1->y());

	double horizontalratio = deltax / (deltax + deltay);
	double verticalratio = deltay / (deltax + deltay);

	CVertex3d* pVertex3, *pVertex4;
	CFace3d* pFace1 = pMesh->FindFace(pVertex1, pVertex2, pVertex3);
	CFace3d* pFace2 = pMesh->FindFace(pVertex2, pVertex1, pVertex4);

	//Delete the two vertices
	pMesh->DeleteFace(pFace1);
	pMesh->DeleteFace(pFace2);

	//Add Vertices
	std::vector<CVertex3d*> m_ArrayNewVertices;
	m_ArrayNewVertices.resize(m_Edge_Subdivision_Num + 1);
	m_ArrayNewVertices[0] = pVertex1;
	m_ArrayNewVertices[m_Edge_Subdivision_Num] = pVertex2;

	std::vector<double> m_ArrayRatios;
	m_ArrayRatios.resize(m_Edge_Subdivision_Num + 1);
	m_ArrayRatios[0] = 0;
	m_ArrayRatios[m_Edge_Subdivision_Num] = 1;

	//set ratio
	int horizontalnum = m_Edge_Subdivision_Num / 2 + m_Edge_Subdivision_Num % 2;
	int verticalnum = m_Edge_Subdivision_Num - horizontalnum;

	double ratiosum = 0;
	for (int i = 1; i < m_Edge_Subdivision_Num; i++)
	{
		if (i % 2 == 1)
		{
			ratiosum += horizontalratio / horizontalnum;
		}
		else
		{
			ratiosum += verticalratio / verticalnum;
		}
		m_ArrayRatios[i] = ratiosum;
	}

	double x, y, z;
	CVertex3d* pNewVertex;
	for (int i = 1; i < m_Edge_Subdivision_Num; i++)
	{
		//double t = (i + 0.0) / m_Edge_Subdivision_Num;
		double t = m_ArrayRatios[i];
		x = pVertex1->x() * (1 - t) + pVertex2->x() * t;
		y = pVertex1->y() * (1 - t) + pVertex2->y() * t;
		z = pVertex1->z() * (1 - t) + pVertex2->z() * t;

		pNewVertex = new CVertex3d(x, y, z);
		pMesh->AddVertex(pNewVertex);

		pFace1 = new CFace3d(pVertex3, m_ArrayNewVertices[i - 1], pNewVertex);
		pFace2 = new CFace3d(pNewVertex, m_ArrayNewVertices[i - 1], pVertex4);
		pMesh->AddFace(pFace1);
		pMesh->AddFace(pFace2);

		m_ArrayNewVertices[i] = pNewVertex;
	}

	//
	pFace1 = new CFace3d(pVertex3, m_ArrayNewVertices[m_Edge_Subdivision_Num - 1], m_ArrayNewVertices[m_Edge_Subdivision_Num]);
	pFace2 = new CFace3d(m_ArrayNewVertices[m_Edge_Subdivision_Num], m_ArrayNewVertices[m_Edge_Subdivision_Num - 1], pVertex4);
	pMesh->AddFace(pFace1);
	pMesh->AddFace(pFace2);

	pMesh->m_SetVertexFlagAsItsIndex = 0;
	pMesh->SetVertexFlagAsItsIndex();
	//Update Sharp Edge Array and Landmark Array
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		if (pair.first == pVertex1->m_Flag && pair.second == pVertex2->m_Flag ||
			pair.second == pVertex1->m_Flag && pair.first == pVertex2->m_Flag)
		{
			pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
			break;
		}
	}

	for (int i = 0; i < m_ArrayNewVertices.size() - 1; i++)
	{
		pair.first = m_ArrayNewVertices[i]->m_Flag;
		pair.second = m_ArrayNewVertices[i + 1]->m_Flag;
		pMesh->m_ArraySharpEdges.push_back(pair);
	}

	GeoMapLandMark* pLandmark = NULL;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark1 = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark1->landmark_points.size() - 1; j++)
		{
			int index1 = pLandmark1->landmark_points[j];
			int index2 = pLandmark1->landmark_points[j + 1];
			if (index1 == pVertex1->m_Flag && index2 == pVertex2->m_Flag)
			{
				pLandmark = pLandmark1;
				for (int k = m_ArrayNewVertices.size() - 2; k >= 1; k--)
				{
					pLandmark1->landmark_points.insert(pLandmark1->landmark_points.begin() + j + 1, m_ArrayNewVertices[k]->m_Flag);
				}
			}
			else if (index2 == pVertex1->m_Flag && index1 == pVertex2->m_Flag)
			{
				pLandmark = pLandmark1;
				for (int k = 1; k <= m_ArrayNewVertices.size() - 2; k++)
				{
					pLandmark1->landmark_points.insert(pLandmark1->landmark_points.begin() + j + 1, m_ArrayNewVertices[k]->m_Flag);
				}
			}
			if (pLandmark)
				break;
		}
		if (pLandmark)
			break;
	}
}
void Action_PolyOmino::OnBnClickedButtonSubdivideEdgeSloppingHorizontal()
{
	// TODO: Add your control notification handler code here
	Polyomino_Edge_Subdivision dialog;
	int m_Edge_Subdivision_Num;
	if (dialog.DoModal() == IDOK)
	{
		m_Edge_Subdivision_Num = dialog.m_EdgeSubdivisionNum;
	}

	//
	pFinalRectilinearMesh->m_ArraySharpVertexIndex = pMesh->m_ArraySharpVertexIndex;

	//
	SubdivideEdgesSloppingHorizontal(pMesh, m_Edge_Subdivision_Num);

	//
	SubdivideEdgesSloppingHorizontal(pFinalRectilinearMesh, m_Edge_Subdivision_Num);

	//Save Modified World Map
	SaveCurrentWorldMap();

	//
	SaveCurrentRectilinearWorldMap();

	pMesh->SetModified(1);
	pDoc->UpdateAllViews(NULL);
}

void Action_PolyOmino::SubdivideEdgeSloppingVertical(CMesh3d* pMesh,int m_Edge_Subdivision_Num)
{
	//Must Select two vertices.
	if (pMesh->m_ArraySharpVertexIndex.size() != 2)
		return;

	pMesh->SetVertexFlagAsItsIndex();

	CVertex3d* pVertex1 = pMesh->GetVertex(pMesh->m_ArraySharpVertexIndex[0]);
	CVertex3d* pVertex2 = pMesh->GetVertex(pMesh->m_ArraySharpVertexIndex[1]);

	double deltax = fabs(pVertex2->x() - pVertex1->x());
	double deltay = fabs(pVertex2->y() - pVertex1->y());

	double horizontalratio = deltax / (deltax + deltay);
	double verticalratio = deltay / (deltax + deltay);

	CVertex3d* pVertex3, *pVertex4;
	CFace3d* pFace1 = pMesh->FindFace(pVertex1, pVertex2, pVertex3);
	CFace3d* pFace2 = pMesh->FindFace(pVertex2, pVertex1, pVertex4);

	//Delete the two vertices
	pMesh->DeleteFace(pFace1);
	pMesh->DeleteFace(pFace2);

	//Add Vertices
	std::vector<CVertex3d*> m_ArrayNewVertices;
	m_ArrayNewVertices.resize(m_Edge_Subdivision_Num + 1);
	m_ArrayNewVertices[0] = pVertex1;
	m_ArrayNewVertices[m_Edge_Subdivision_Num] = pVertex2;

	std::vector<double> m_ArrayRatios;
	m_ArrayRatios.resize(m_Edge_Subdivision_Num + 1);
	m_ArrayRatios[0] = 0;
	m_ArrayRatios[m_Edge_Subdivision_Num] = 1;

	//set ratio
	int verticalnum = m_Edge_Subdivision_Num / 2 + m_Edge_Subdivision_Num % 2;
	int horizontalnum = m_Edge_Subdivision_Num - verticalnum;

	double ratiosum = 0;
	for (int i = 1; i < m_Edge_Subdivision_Num; i++)
	{
		if (i % 2 == 1)
		{
			ratiosum += verticalratio / verticalnum;
		}
		else
		{
			ratiosum += horizontalratio / horizontalnum;
		}
		m_ArrayRatios[i] = ratiosum;
	}

	double x, y, z;
	CVertex3d* pNewVertex;
	for (int i = 1; i < m_Edge_Subdivision_Num; i++)
	{
		//double t = (i + 0.0) / m_Edge_Subdivision_Num;
		double t = m_ArrayRatios[i];
		x = pVertex1->x() * (1 - t) + pVertex2->x() * t;
		y = pVertex1->y() * (1 - t) + pVertex2->y() * t;
		z = pVertex1->z() * (1 - t) + pVertex2->z() * t;

		pNewVertex = new CVertex3d(x, y, z);
		pMesh->AddVertex(pNewVertex);

		pFace1 = new CFace3d(pVertex3, m_ArrayNewVertices[i - 1], pNewVertex);
		pFace2 = new CFace3d(pNewVertex, m_ArrayNewVertices[i - 1], pVertex4);
		pMesh->AddFace(pFace1);
		pMesh->AddFace(pFace2);

		m_ArrayNewVertices[i] = pNewVertex;
	}

	//
	pFace1 = new CFace3d(pVertex3, m_ArrayNewVertices[m_Edge_Subdivision_Num - 1], m_ArrayNewVertices[m_Edge_Subdivision_Num]);
	pFace2 = new CFace3d(m_ArrayNewVertices[m_Edge_Subdivision_Num], m_ArrayNewVertices[m_Edge_Subdivision_Num - 1], pVertex4);
	pMesh->AddFace(pFace1);
	pMesh->AddFace(pFace2);

	pMesh->m_SetVertexFlagAsItsIndex = 0;
	pMesh->SetVertexFlagAsItsIndex();
	//Update Sharp Edge Array and Landmark Array
	std::pair<int, int> pair;
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		pair = pMesh->m_ArraySharpEdges[i];
		if (pair.first == pVertex1->m_Flag && pair.second == pVertex2->m_Flag ||
			pair.second == pVertex1->m_Flag && pair.first == pVertex2->m_Flag)
		{
			pMesh->m_ArraySharpEdges.erase(pMesh->m_ArraySharpEdges.begin() + i);
			break;
		}
	}

	for (int i = 0; i < m_ArrayNewVertices.size() - 1; i++)
	{
		pair.first = m_ArrayNewVertices[i]->m_Flag;
		pair.second = m_ArrayNewVertices[i + 1]->m_Flag;
		pMesh->m_ArraySharpEdges.push_back(pair);
	}

	GeoMapLandMark* pLandmark = NULL;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark1 = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark1->landmark_points.size() - 1; j++)
		{
			int index1 = pLandmark1->landmark_points[j];
			int index2 = pLandmark1->landmark_points[j + 1];
			if (index1 == pVertex1->m_Flag && index2 == pVertex2->m_Flag)
			{
				pLandmark = pLandmark1;
				for (int k = m_ArrayNewVertices.size() - 2; k >= 1; k--)
				{
					pLandmark1->landmark_points.insert(pLandmark1->landmark_points.begin() + j + 1, m_ArrayNewVertices[k]->m_Flag);
				}
			}
			else if (index2 == pVertex1->m_Flag && index1 == pVertex2->m_Flag)
			{
				pLandmark = pLandmark1;
				for (int k = 1; k <= m_ArrayNewVertices.size() - 2; k++)
				{
					pLandmark1->landmark_points.insert(pLandmark1->landmark_points.begin() + j + 1, m_ArrayNewVertices[k]->m_Flag);
				}
			}
			if (pLandmark)
				break;
		}
		if (pLandmark)
			break;
	}
}
void Action_PolyOmino::OnBnClickedButtonSubdivideEdgeSloppingVertical()
{
	// TODO: Add your control notification handler code here
	Polyomino_Edge_Subdivision dialog;
	int m_Edge_Subdivision_Num;
	if (dialog.DoModal() == IDOK)
	{
		m_Edge_Subdivision_Num = dialog.m_EdgeSubdivisionNum;
	}
	//
	pFinalRectilinearMesh->m_ArraySharpVertexIndex = pMesh->m_ArraySharpVertexIndex;

	//
	SubdivideEdgeSloppingVertical(pMesh, m_Edge_Subdivision_Num);

	//
	SubdivideEdgeSloppingVertical(pFinalRectilinearMesh, m_Edge_Subdivision_Num);

	//Save Modified World Map
	SaveCurrentWorldMap();

	//Save Modfified Rectilinear Map
	SaveCurrentRectilinearWorldMap();

	pMesh->SetModified(1);
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedButtonSearchLandmark2()
{
	//Must Select one vertices.
	if (pMesh->m_ArraySharpVertexIndex.size() < 1)
		return;

	std::vector<int> m_CornerPointArray;
	m_CornerPointArray.resize(pMesh->m_ArrayVertex.GetSize(), 0);

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		int start = pLandmark->landmark_points[0];
		int end = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
		m_CornerPointArray[start]++;
		m_CornerPointArray[end]++;
	}

	std::vector<std::vector<int>> m_ArrayPointConnectedSelectEdgeIndex;
	m_ArrayPointConnectedSelectEdgeIndex.resize(pMesh->m_ArrayVertex.GetSize());
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{

		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];

		for (int j = 0; j < pLandmark->landmark_points.size(); j++)
		{
			int pointindex = pLandmark->landmark_points[j];
			m_ArrayPointConnectedSelectEdgeIndex[pointindex].push_back(i);
		}
		//int start = pLandmark->landmark_points[0];
		//int end = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
		//m_ArrayPointConnectedSelectEdgeIndex[start].push_back(i);
		//m_ArrayPointConnectedSelectEdgeIndex[end].push_back(i);
	}

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		pLandmark->flag = i;
	}

	std::vector<int> m_SelectedLandmarks;
	std::vector<GeoMapLandMark*> m_ArrayCurrentLandmarks;
	std::vector<int> m_ArraySharpVertexIndex_ = pMesh->m_ArraySharpVertexIndex;
	for (int j = 0; j < m_ArraySharpVertexIndex_.size(); j++)
	{
		int vindex = m_ArraySharpVertexIndex_[j];
		GeoMapLandMark* pCurrentLandmark = NULL;
		/*for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
		{
		GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandmark->landmark_points.size(); j++)
		{
		if (pLandmark->landmark_points[j] == vindex)
		{
		pCurrentLandmark = pLandmark;
		m_ArrayCurrentLandmarks.push_back(pCurrentLandmark);
		break;
		}
		}
		//if (pCurrentLandmark)
		//	break;
		}*/

		for (int i = 0; i < m_ArrayPointConnectedSelectEdgeIndex[vindex].size(); i++)
		{
			GeoMapLandMark* pLandmark = pMesh->m_LandMarkArray[m_ArrayPointConnectedSelectEdgeIndex[vindex][i]];
			m_ArrayCurrentLandmarks.push_back(pLandmark);
		}

		//
		if (m_ArrayCurrentLandmarks.size() == 0)
			return;

		int landmarkstart, landmarkend;
		for (int i = 0; i < m_ArrayCurrentLandmarks.size(); i++)
		{
			pCurrentLandmark = m_ArrayCurrentLandmarks[i];

			landmarkstart = pCurrentLandmark->landmark_points[0];
			landmarkend = pCurrentLandmark->landmark_points[pCurrentLandmark->landmark_points.size() - 1];

			m_SelectedLandmarks.clear();
			m_SelectedLandmarks.push_back(pCurrentLandmark->flag);
			//
			//Find all the corner

			//Trace the landmarks
			while (1)
			{
				if (m_ArrayPointConnectedSelectEdgeIndex[landmarkstart].size() == 2)
				{
					int landmark1 = m_ArrayPointConnectedSelectEdgeIndex[landmarkstart][0];
					int landmark2 = m_ArrayPointConnectedSelectEdgeIndex[landmarkstart][1];
					int nextlandmark = landmark1;
					if (std::find(m_SelectedLandmarks.begin(), m_SelectedLandmarks.end(), landmark1) != m_SelectedLandmarks.end())
						nextlandmark = landmark2;

					if (std::find(m_SelectedLandmarks.begin(), m_SelectedLandmarks.end(), nextlandmark) != m_SelectedLandmarks.end())
						break;

					GeoMapLandMark* pNextLandmark = pMesh->m_LandMarkArray[nextlandmark];
					if (pNextLandmark->landmark_points[0] == landmarkstart)
						landmarkstart = pNextLandmark->landmark_points[pNextLandmark->landmark_points.size() - 1];
					else
						landmarkstart = pNextLandmark->landmark_points[0];
					m_SelectedLandmarks.push_back(nextlandmark);
				}
				else
				{
					if (std::find(m_ArraySharpVertexIndex_.begin(), m_ArraySharpVertexIndex_.end(), landmarkstart) == m_ArraySharpVertexIndex_.end())
						m_ArraySharpVertexIndex_.push_back(landmarkstart);
					break;
				}
			}

			while (1)
			{
				if (m_ArrayPointConnectedSelectEdgeIndex[landmarkend].size() == 2)
				{
					int landmark1 = m_ArrayPointConnectedSelectEdgeIndex[landmarkend][0];
					int landmark2 = m_ArrayPointConnectedSelectEdgeIndex[landmarkend][1];
					int nextlandmark = landmark1;
					if (std::find(m_SelectedLandmarks.begin(), m_SelectedLandmarks.end(), landmark1) != m_SelectedLandmarks.end())
						nextlandmark = landmark2;

					if (std::find(m_SelectedLandmarks.begin(), m_SelectedLandmarks.end(), nextlandmark) != m_SelectedLandmarks.end())
						break;

					GeoMapLandMark* pNextLandmark = pMesh->m_LandMarkArray[nextlandmark];
					if (pNextLandmark->landmark_points[0] == landmarkend)
						landmarkend = pNextLandmark->landmark_points[pNextLandmark->landmark_points.size() - 1];
					else
						landmarkend = pNextLandmark->landmark_points[0];
					m_SelectedLandmarks.push_back(nextlandmark);
				}
				else
				{
					if (std::find(m_ArraySharpVertexIndex_.begin(), m_ArraySharpVertexIndex_.end(), landmarkend) == m_ArraySharpVertexIndex_.end())
						m_ArraySharpVertexIndex_.push_back(landmarkend);
					break;
				}
			}

			for (int ii = 0; ii < m_SelectedLandmarks.size(); ii++)
			{
				if (std::find(pMesh->m_SelectedLandmarkIndexArray.begin(), pMesh->m_SelectedLandmarkIndexArray.end(), m_SelectedLandmarks[ii]) == pMesh->m_SelectedLandmarkIndexArray.end())
				{
					pMesh->m_SelectedLandmarkIndexArray.push_back(m_SelectedLandmarks[ii]);
				}
			}
		}
	}

	//
	SaveCurrentSelectedLandmark();
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedPolyominoMeshFixvertices()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < pMesh->m_ArraySharpVertexIndex.size(); i++)
	{
		if (std::find(pMesh->m_FixVerticesArray_Inner.begin(), pMesh->m_FixVerticesArray_Inner.end(), pMesh->m_ArraySharpVertexIndex[i]) == pMesh->m_FixVerticesArray_Inner.end())
			pMesh->m_FixVerticesArray_Inner.push_back(pMesh->m_ArraySharpVertexIndex[i]);
	}
	pMesh->m_FixVerticesArray_Inner_uv.clear();
	//pMesh->m_FixVerticesArray_Inner.insert(pMesh->m_FixVerticesArray_Inner.begin(), pMesh->m_ArraySharpVertexIndex.begin(), pMesh->m_ArraySharpVertexIndex.end());
	for (int i = 0; i < pMesh->m_FixVerticesArray_Inner.size(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(pMesh->m_FixVerticesArray_Inner[i]);
		double x = pVertex->x();
		double y = pVertex->y();
		pMesh->m_FixVerticesArray_Inner_uv.push_back(x);
		pMesh->m_FixVerticesArray_Inner_uv.push_back(y);
	}
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedPolyominoMeshFixvertices2()
{
	// TODO: Add your control notification handler code here
	pMesh->m_FixVerticesArray_Inner.clear();
	pMesh->m_FixVerticesArray_Inner_uv.clear();
	pMesh->m_FixVerticesArray_Inner.insert(pMesh->m_FixVerticesArray_Inner.begin(), pMesh->m_ArraySharpVertexIndex.begin(), pMesh->m_ArraySharpVertexIndex.end());
	for (int i = 0; i < pMesh->m_FixVerticesArray_Inner.size(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(pMesh->m_FixVerticesArray_Inner[i]);
		double x = pVertex->x();
		double y = pVertex->y();
		pMesh->m_FixVerticesArray_Inner_uv.push_back(x);
		pMesh->m_FixVerticesArray_Inner_uv.push_back(y);
	}
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedPolyominoMeshFixvertices3()
{
	// TODO: Add your control notification handler code here
	std::ofstream file("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\finalrectilinearmap\\fixvertices.txt");
	file << pMesh->m_FixVerticesArray_Inner.size() << "\n"; 
	for (int i = 0; i < pMesh->m_FixVerticesArray_Inner.size(); i++)
	{
		file << pMesh->m_FixVerticesArray_Inner[i] << " " << pMesh->m_FixVerticesArray_Inner_uv[i * 2] << " " << pMesh->m_FixVerticesArray_Inner_uv[i * 2 + 1] << "\n";
	}
	file.close();
}


void Action_PolyOmino::OnBnClickedPolyominoMeshFixvertices4()
{
	// TODO: Add your control notification handler code here
	std::ifstream file("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\finalrectilinearmap\\fixvertices.txt");
	pMesh->m_FixVerticesArray_Inner.clear();
	pMesh->m_FixVerticesArray_Inner_uv.clear();
	int n;
	file >> n;
	int val;
	double x, y;
	for (int i = 0; i < n; i++)
	{
		file >> val;
		file >> x;
		file >> y;
		pMesh->m_FixVerticesArray_Inner.push_back(val);
		pMesh->m_FixVerticesArray_Inner_uv.push_back(x);
		pMesh->m_FixVerticesArray_Inner_uv.push_back(y);
	}
	file.close();
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedPolyominoMeshSharpevertexSave()
{
	// TODO: Add your control notification handler code here
	std::ofstream file("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\finalrectilinearmap\\Sharpvertices.txt");
	file << pMesh->m_ArraySharpVertexIndex.size() << "\n";
	for (int i = 0; i < pMesh->m_ArraySharpVertexIndex.size(); i++)
	{
		file << pMesh->m_ArraySharpVertexIndex[i] << "\n";
	}
	file.close();
}


void Action_PolyOmino::OnBnClickedPolyominoMeshSharpevertexLoad()
{
	// TODO: Add your control notification handler code here
	std::ifstream file("D:\\papers\\code\\data\\Geo\\ditu\\worldmap\\natural_earth_vector\\110m_cultural\\ne_110m_admin_0_countries\\finalrectilinearmap\\Sharpvertices.txt");
	pMesh->m_ArraySharpVertexIndex.clear();
	//pMesh->m_FixVerticesArray_Inner.clear();
	//pMesh->m_FixVerticesArray_Inner_uv.clear();
	int n;
	file >> n;
	int val;
	double x, y;
	for (int i = 0; i < n; i++)
	{
		file >> val;
		pMesh->m_ArraySharpVertexIndex.push_back(val);
	}
	file.close();
	pDoc->UpdateAllViews(NULL);
}
void Action_PolyOmino::SolveCNN_matlab()
{
	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		AfxMessageBox("can't start MATLAB engine!\n");
		exit(1);
	}

	engEvalString(ep, "inputSize = [64 64];");

	engEvalString(ep, "keySet = {'AN','DI','FE','HA','NE','SA','SU'};");
	engEvalString(ep, "valueSet = [1,2,3,4,5,6,7];");
	engEvalString(ep, "labelMap = containers.Map(keySet,valueSet);");

	// TODO: !!! Change imageDatastore parameter to corrosponding directory !!!
	engEvalString(ep, "imds = imageDatastore(\"images/\");");
	engEvalString(ep, "imds.ReadFcn = @(loc)imresize(imread(loc),inputSize);");

	engEvalString(ep, "labels = [];");
	engEvalString(ep, "for i = 1:length(imds.Files)\n"
		"    filename = imds.Files(i);\n"
		"    filename = filename{1};\n"
		"    splited = strsplit(filename, \"\\\"); \n"
		"    name = splited(length(splited));\n"
		"    name = name{1};\n"
		"    label = extractBetween(name, 7, 8);\n"
		"    %label = label{1};\n"
		"    %label = labelMap(label);\n"
		"    labels = [labels, label];\n"
		"end");
	engEvalString(ep, "for i = 1:length(imds.Files)\n"
		"    filename = imds.Files(i);\n"
		"    filename = filename{1};\n"
		"    splited = strsplit(filename, \"\\\"); \n"
		"    name = splited(length(splited));\n"
		"    name = name{1};\n"
		"    label = extractBetween(name, 7, 8);\n"
		"    %label = label{1};\n"
		"    %label = labelMap(label);\n"
		"    labels = [labels, label];\n"
		"end");

	engEvalString(ep, "imds.Labels = categorical(labels);");
	engEvalString(ep, "labels_num = height(countEachLabel(imds));");

	engEvalString(ep, "[imdsTrain,imdsValidation] = splitEachLabel(imds,0.9,'randomize');");

	engEvalString(ep, "layers = ["
		"    imageInputLayer([64 64 3])\n"

		"    convolution2dLayer(3,8,'Padding',1)\n"
		"    batchNormalizationLayer\n"
		"    reluLayer\n"

		"    maxPooling2dLayer(2,'Stride',2)\n"

		"    convolution2dLayer(3,16,'Padding',1)\n"
		"    batchNormalizationLayer\n"
		"    reluLayer\n"

		"    maxPooling2dLayer(2,'Stride',2)\n"

		"    convolution2dLayer(3,32,'Padding',1)\n"
		"    batchNormalizationLayer\n"
		"    reluLayer\n"

		"    fullyConnectedLayer(labels_num)\n"
		"    softmaxLayer\n"
		"    classificationLayer];");

	engEvalString(ep, "options = trainingOptions('sgdm',"
		"'MaxEpochs',30,"
		"'ValidationData',imdsValidation,"
		"'ValidationFrequency',30,"
		"'Verbose',false,"
		"'Plots','training-progress');");

	engEvalString(ep, "net = trainNetwork(imdsTrain,layers,options);");

	engEvalString(ep, "YPred = classify(net,imdsValidation);");
	engEvalString(ep, "YValidation = imdsValidation.Labels;");

	engEvalString(ep, "accuracy = sum(YPred == YValidation)/numel(YValidation);");

	mxArray *mxAccuracy = engGetVariable(ep, "accuracy");
	double accuracy = mxGetDoubles(mxAccuracy)[0];
	// use result?

	engClose(ep);
}
void Action_PolyOmino::SolveSpareseLinearSystem_matlab_FAST(MySparseMatrix *A, std::vector<double> &B, std::vector<double> &X)
{
	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		AfxMessageBox("can't start MATLAB engine!");
		exit(1);
	}
	int m = A->m;
	int n = A->n;

	//Judge whether the Array has the maximal indexs
	int maxi = -1, maxj = -1;
	std::map<std::pair<int, int>, double>::iterator it;
	for (it = A->m_Array.begin(); it != A->m_Array.end(); ++it)
	{
		if (it->first.first > maxi)
			maxi = it->first.first;
		if (it->first.second > maxj)
			maxj = it->first.second;
	}
	std::pair<int, int> pair;
	if (maxi < m - 1 || maxj < n - 1)
	{
		pair.first = m - 1;
		pair.second = n - 1;

		A->m_Array[pair] = 0;
	}

	double (*A_)[3] = new double[A->m_Array.size()][3];
	int index = 0;
	double val;

	mxArray *A_mx = mxCreateDoubleMatrix(A->m_Array.size(),3,mxREAL);

	for (it = A->m_Array.begin(); it != A->m_Array.end(); ++it)
	{
		val = it->first.first + 1;
		memcpy((void*)(mxGetPr(A_mx) + (index)), (void*)&val, sizeof(double));
		val = it->first.second + 1;
		memcpy((void*)(mxGetPr(A_mx) + (index + 1 * A->m_Array.size())), (void*)&val, sizeof(double));
		val = it->second;
		memcpy((void*)(mxGetPr(A_mx) + (index + 2 * A->m_Array.size())), (void*)&val, sizeof(double));

		index++;
	}

	engEvalString(ep, "global T");
	engPutVariable(ep,"T",A_mx);
	
	engEvalString(ep, "global A");
	engEvalString(ep, "A = spconvert(T);");
	delete A_;

	double* bvec = new double[m];
	for (int i = 0; i < m; i++)
		bvec[i] = B[i];

	mxArray *b_mx = mxCreateDoubleMatrix(1, m, mxREAL);
	memcpy(mxGetDoubles(b_mx), bvec, sizeof(double) * m);

	engEvalString(ep, "global bb");
	engPutVariable(ep, "bb", b_mx);
	delete bvec;

	engEvalString(ep, "global xx1");
	engEvalString(ep, "xx1=A\\bb;");

	mxArray *xx = engGetVariable(ep, "xx1");
	double *xval = mxGetPr(xx);
	X.resize(m);
	for (int i = 0; i < m; i++)
		X[i] = xval[i];
	//delete xval;
	//engClose(ep);
}
double Action_PolyOmino::SolveCNNClassification_matlab_SelfTest(std::vector<std::vector<double>> &traindata, std::vector<int> &trainlabel)
{
	return 1;
}
void Action_PolyOmino::SearchSharpEdgeWithEndPoints(std::vector<std::pair<int, int>> &m_ArraySharpEdges, std::vector<int> &newlandmark, std::vector<std::vector<std::pair<int, int>>>& m_CornerPointArray)
{
	int start = newlandmark[0];
	int end = newlandmark[newlandmark.size() - 1];
	std::pair<int, int> pair;
	while (1)
	{
		start = newlandmark[0];
		end = newlandmark[newlandmark.size() - 1];

		if (m_CornerPointArray[start].size() == 1)
		{
			pair = m_CornerPointArray[start][0];
			if (pair.first == start)
			{
				newlandmark.insert(newlandmark.begin(), pair.second);
			}
			else
			{
				newlandmark.insert(newlandmark.begin(), pair.first);
			}
			//
			m_ArraySharpEdges.erase(std::find(m_ArraySharpEdges.begin(), m_ArraySharpEdges.end(), pair));
			m_CornerPointArray[pair.first].erase(std::find(m_CornerPointArray[pair.first].begin(), m_CornerPointArray[pair.first].end(), pair));
			m_CornerPointArray[pair.second].erase(std::find(m_CornerPointArray[pair.second].begin(), m_CornerPointArray[pair.second].end(), pair));
		}else
		if (m_CornerPointArray[end].size() == 1)
		{
			pair = m_CornerPointArray[end][0];
			if (pair.first == end)
			{
				newlandmark.push_back(pair.second);
			}
			else
			{
				newlandmark.push_back(pair.first);
			}

			//
			m_ArraySharpEdges.erase(std::find(m_ArraySharpEdges.begin(), m_ArraySharpEdges.end(), pair));
			m_CornerPointArray[pair.first].erase(std::find(m_CornerPointArray[pair.first].begin(), m_CornerPointArray[pair.first].end(), pair));
			m_CornerPointArray[pair.second].erase(std::find(m_CornerPointArray[pair.second].begin(), m_CornerPointArray[pair.second].end(), pair));
		}
		else
		{
			break;
		}
	}
}
void Action_PolyOmino::GenerateLandMarks_KeepSharpEdgesConsistent()
{
	pMesh->m_LandMarkArray.clear();

	std::vector<std::pair<int, int>> m_ArraySharpEdges_bak;
	m_ArraySharpEdges_bak = pMesh->m_ArraySharpEdges;

	//Find all the corners
	std::vector<std::vector<std::pair<int, int>>> m_CornerPointArray;
	m_CornerPointArray.resize(pMesh->m_ArrayVertex.GetSize());

	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int, int> data;
		data = pMesh->m_ArraySharpEdges[i];
		m_CornerPointArray[data.first].push_back(data);
		m_CornerPointArray[data.second].push_back(data);
	}

	std::vector<int> newlandmark;
	std::pair<int, int> pair;
	std::vector<GeoMapLandMark*> m_ArrayLandMarks;
	while (m_ArraySharpEdges_bak.size() > 0)
	{
		newlandmark.clear();

		pair = m_ArraySharpEdges_bak[0];
		m_ArraySharpEdges_bak.erase(m_ArraySharpEdges_bak.begin());

		newlandmark.push_back(pair.first);
		newlandmark.push_back(pair.second);

		//delete
		m_CornerPointArray[pair.first].erase(std::find(m_CornerPointArray[pair.first].begin(), m_CornerPointArray[pair.first].end(), pair));
		m_CornerPointArray[pair.second].erase(std::find(m_CornerPointArray[pair.second].begin(), m_CornerPointArray[pair.second].end(), pair));

		SearchSharpEdgeWithEndPoints(m_ArraySharpEdges_bak, newlandmark, m_CornerPointArray);

		GeoMapLandMark* pLandMark = new GeoMapLandMark();
		pLandMark->landmark_points = newlandmark;
		m_ArrayLandMarks.push_back(pLandMark);
	}
	pMesh->m_LandMarkArray = m_ArrayLandMarks;
	/*//Find all the corners
	std::vector<int> m_CornerPointArray;
	m_CornerPointArray.resize(pMesh->m_ArrayVertex.GetSize(), 0);

	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int, int> data;
		data = pMesh->m_ArraySharpEdges[i];
		m_CornerPointArray[data.first]++;
		m_CornerPointArray[data.second]++;
	}
	std::vector<std::vector<int>> m_ArrayPointConnectedSelectEdgeIndex;
	m_ArrayPointConnectedSelectEdgeIndex.resize(pMesh->m_ArrayVertex.GetSize());
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int, int> data;
		data = pMesh->m_ArraySharpEdges[i];
		m_ArrayPointConnectedSelectEdgeIndex[data.first].push_back(i);
		m_ArrayPointConnectedSelectEdgeIndex[data.second].push_back(i);
	}

	std::vector<GeoMapLandMark*> m_ArrayLandMarks;
	std::vector<int> newlandmark;
	std::vector<int> m_edghandledflag;
	m_edghandledflag.resize(pMesh->m_ArraySharpEdges.size(), 0);
	//while(1)
	{
		for (int i = 0; i < m_CornerPointArray.size(); i++)
		{
			if (m_CornerPointArray[i] >= 3 || m_CornerPointArray[i] == 1)
			{
				//m_CornerPointArray[i] = -3;
				for (int j = 0; j < m_ArrayPointConnectedSelectEdgeIndex[i].size(); j++)
				{
					newlandmark.clear();
					newlandmark.push_back(i);
					int edgeindex = m_ArrayPointConnectedSelectEdgeIndex[i][j];

					int prevertexindex = i;
					std::pair<int, int> data;
					if (m_edghandledflag[edgeindex] == 0)
					{
						while (1)
						{
							//handled edge
							m_edghandledflag[edgeindex] = 1;

							data = pMesh->m_ArraySharpEdges[edgeindex];
							if (data.first == prevertexindex)
							{
								newlandmark.push_back(data.second);
								prevertexindex = data.second;
							}
							else
							{
								newlandmark.push_back(data.first);
								prevertexindex = data.first;
							}
							if (m_CornerPointArray[prevertexindex] >= 3 || m_CornerPointArray[prevertexindex] == 1)
								break;

							if (m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][0] == edgeindex)
								edgeindex = m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][1];
							else
								edgeindex = m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][0];
						}
						if (newlandmark.size() == 2)
						{
							int ij = 0;
						}
						GeoMapLandMark* pLandMark = new GeoMapLandMark();
						pLandMark->landmark_points = newlandmark;
						m_ArrayLandMarks.push_back(pLandMark);
					}

				}
			}
		}
	}

	//check whether has a loop landmark
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		if (m_edghandledflag[i] == 0)
		{
			int edgeindex = i;
			int prevertexindex = pMesh->m_ArraySharpEdges[edgeindex].second;
			newlandmark.clear();
			newlandmark.push_back(pMesh->m_ArraySharpEdges[edgeindex].second);
			std::pair<int, int> data;
			while (1)
			{
				//handled edge
				m_edghandledflag[edgeindex] = 1;

				data = pMesh->m_ArraySharpEdges[edgeindex];
				if (data.first == prevertexindex)
				{
					newlandmark.push_back(data.second);
					prevertexindex = data.second;
				}
				else
				{
					newlandmark.push_back(data.first);
					prevertexindex = data.first;
				}
				if (m_CornerPointArray[prevertexindex] >= 3 || m_CornerPointArray[prevertexindex] == 1)
					break;

				if (m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][0] == edgeindex)
					edgeindex = m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][1];
				else
					edgeindex = m_ArrayPointConnectedSelectEdgeIndex[prevertexindex][0];
				//
				if (m_edghandledflag[edgeindex] == 1)
					break;
			}
			if (newlandmark.size() == 2)
			{
				int ij = 0;
			}
			GeoMapLandMark* pLandMark = new GeoMapLandMark();
			pLandMark->landmark_points = newlandmark;
			m_ArrayLandMarks.push_back(pLandMark);
		}
	}
	pMesh->m_LandMarkArray = m_ArrayLandMarks;

	//pMesh->m_ArrayLandMarks_Original = pMesh->m_LandMarkArray;
	sprintf(Post_Message, "Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d", pMesh->NbFace(), pMesh->NbVertex(), pMesh->m_ArraySharpEdges.size(), pMesh->m_LandMarkArray.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);*/
}
void Action_PolyOmino::SolveCNNClassification_matlab(std::vector<std::vector<double>> &traindata, std::vector<int> &trainlabel, std::vector<std::vector<double>> &testdata, std::vector<int> &testlabel)
{
	if (traindata.size() != trainlabel.size()
		|| testdata.size() != testlabel.size()) {
		AfxMessageBox("size mismatch.");
		return;
	}

	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		AfxMessageBox("can't start MATLAB engine!\n");
		exit(1);
	}
	mxArray *mxTrainData = NULL, *mxTrainLabel = NULL, *mxTestData = NULL, *mxTestLabel = NULL;
	mxTrainData = mxCreateDoubleMatrix(traindata[0].size(), traindata.size(), mxREAL);
	mxTrainLabel = mxCreateDoubleMatrix(1, trainlabel.size(), mxREAL);
	mxTestData = mxCreateDoubleMatrix(testdata[0].size(), testdata.size(), mxREAL);
	mxTestLabel = mxCreateDoubleMatrix(1, testlabel.size(), mxREAL);

	// create a double array for copying
	double *dTrainData = new double[traindata.size()*traindata[0].size()];
	for (int i = 0; i < traindata.size(); i++) {
		memcpy(dTrainData + (i*traindata[0].size()), traindata[i].data(), sizeof(double)*traindata[0].size());
	}
	double *dTrainLabel = new double[trainlabel.size()];
	for (int i = 0; i < trainlabel.size(); i++) {
		dTrainLabel[i] = trainlabel[i];
	}
	double *dTestData = new double[testdata.size()*testdata[0].size()];
	for (int i = 0; i < testdata.size(); i++) {
		memcpy(dTestData + (i*testdata[0].size()), testdata[i].data(), sizeof(double)*testdata[0].size());
	}
	double *dTestLabel = new double[testlabel.size()];
	for (int i = 0; i < testlabel.size(); i++) {
		dTestLabel[i] = testlabel[i];
	}

	// copy values to mxTrainData
	memcpy(mxGetDoubles(mxTrainData), dTrainData, sizeof(double)*traindata.size()*traindata[0].size());
	memcpy(mxGetDoubles(mxTrainLabel), dTrainLabel, sizeof(double)*trainlabel.size());
	memcpy(mxGetDoubles(mxTestData), dTestData, sizeof(double)*testdata.size()*testdata[0].size());
	memcpy(mxGetDoubles(mxTestLabel), dTestLabel, sizeof(double)*testlabel.size());
	delete[] dTrainData;
	delete[] dTrainLabel;
	delete[] dTestData;
	delete[] dTestLabel;

	engPutVariable(ep, "traindata", mxTrainData);
	engPutVariable(ep, "trainlabel", mxTrainLabel);
	engPutVariable(ep, "testdata", mxTestData);
	engPutVariable(ep, "testlabel", mxTestLabel);

	engEvalString(ep, "trainlabel_oh = ind2vec(trainlabel);");
	engEvalString(ep, "testlabel_oh = ind2vec(testlabel);");
	engEvalString(ep, "net = patternnet(10);");
	engEvalString(ep, "net = train(net, traindata, trainlabel_oh);");
	engEvalString(ep, "y = net(testdata);");
	engEvalString(ep, "p = perform(net, testlabel, y);");
	mxArray *mxY = engGetVariable(ep, "y");
	mxArray *mxP = engGetVariable(ep, "p");

	if (!(mxIsDouble(mxY) /*&& mxGetNumberOfElements(mxY) != testdata.size()*/)) {
		// error happens
		AfxMessageBox("Cannot get y\n");
		return;
	}

	double *dResult = new double[mxGetNumberOfElements(mxY)];
	memcpy(dResult, mxGetDoubles(mxY), sizeof(double)*mxGetNumberOfElements(mxY));

	// dResult is a 2*12 array. first number is class, second is confidence.

	int *result = new int[mxGetN(mxY)];
	for (int i = 0; i < mxGetN(mxY); i++) {
		result[i] = dResult[i * 2] + 1e-3;
	}
	double performance = mxGetDoubles(mxP)[0];
	// use results?

	// cleanup
	mxDestroyArray(mxTrainData);
	mxDestroyArray(mxTrainLabel);
	mxDestroyArray(mxTestData);
	mxDestroyArray(mxTestLabel);

	engClose(ep);

	delete[] dResult;
	delete[] result;
}
void Action_PolyOmino::SolveSVMClassification_matlab(std::vector<std::vector<double>> &traindata, std::vector<int> &trainlabel, std::vector<std::vector<double>> &testdata, std::vector<int> &testlabel)
{
	if (traindata.size() != trainlabel.size()
		|| testdata.size() != testlabel.size()) {
		AfxMessageBox("size mismatch.");
		return;
	}
	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		AfxMessageBox("can't start MATLAB engine!\n");
		exit(1);
	}
	mxArray *mxTrainData = NULL, *mxTrainLabel = NULL, *mxTestData = NULL, *mxTestLabel = NULL;
	mxTrainData = mxCreateDoubleMatrix(traindata[0].size(), traindata.size(), mxREAL);
	mxTrainLabel = mxCreateDoubleMatrix(1, trainlabel.size(), mxREAL);
	mxTestData = mxCreateDoubleMatrix(testdata[0].size(), testdata.size(), mxREAL);
	mxTestLabel = mxCreateDoubleMatrix(1, testlabel.size(), mxREAL);

	// create a double array for copying
	double *dTrainData = new double[traindata.size()*traindata[0].size()];
	for (int i = 0; i < traindata.size(); i++) {
		memcpy(dTrainData + (i*traindata[0].size()), traindata[i].data(), sizeof(double)*traindata[0].size());
	}
	double *dTrainLabel = new double[trainlabel.size()];
	for (int i = 0; i < trainlabel.size(); i++) {
		dTrainLabel[i] = trainlabel[i];
	}
	double *dTestData = new double[testdata.size()*testdata[0].size()];
	for (int i = 0; i < testdata.size(); i++) {
		memcpy(dTestData + (i*testdata[0].size()), testdata[i].data(), sizeof(double)*testdata[0].size());
	}
	double *dTestLabel = new double[testlabel.size()];
	for (int i = 0; i < testlabel.size(); i++) {
		dTestLabel[i] = testlabel[i];
	}

	// copy values to mxTrainData
	memcpy(mxGetDoubles(mxTrainData), dTrainData, sizeof(double)*traindata.size()*traindata[0].size());
	memcpy(mxGetDoubles(mxTrainLabel), dTrainLabel, sizeof(double)*trainlabel.size());
	memcpy(mxGetDoubles(mxTestData), dTestData, sizeof(double)*testdata.size()*testdata[0].size());
	memcpy(mxGetDoubles(mxTestLabel), dTestLabel, sizeof(double)*testlabel.size());
	delete[] dTrainData;
	delete[] dTrainLabel;
	delete[] dTestData;
	delete[] dTestLabel;

	engPutVariable(ep, "traindata", mxTrainData);
	engPutVariable(ep, "trainlabel", mxTrainLabel);
	engPutVariable(ep, "testdata", mxTestData);
	engPutVariable(ep, "testlabel", mxTestLabel);

	engEvalString(ep, "traindata = traindata';");
	engEvalString(ep, "trainlabel = trainlabel';");
	engEvalString(ep, "testdata = testdata';");
	engEvalString(ep, "testlabel = testlabel';");
	engEvalString(ep, "mdl = fitcecoc(traindata, trainlabel);");
	engEvalString(ep, "y = mdl.predict(testdata);");
	mxArray *mxY = engGetVariable(ep, "y");

	if (!(mxIsDouble(mxY) /*&& mxGetNumberOfElements(mxY) != testdata.size()*/)) {
		// error happens
		AfxMessageBox("Cannot get y\n");
		goto ret;
	}

	double *dResult = new double[mxGetNumberOfElements(mxY)];
	memcpy(dResult, mxGetDoubles(mxY), sizeof(double)*mxGetNumberOfElements(mxY));

	// dResult is a 2*12 array. first number is class, second is confidence.

	int *result = new int[mxGetNumberOfElements(mxY)];
	for (int i = 0; i < mxGetNumberOfElements(mxY); i++) {
		result[i] = dResult[i] + 1e-3;
	}

	if (mxGetNumberOfElements(mxY) != testlabel.size()) {
		AfxMessageBox("y size mismatch.");
		goto ret;
	}

	int all = mxGetNumberOfElements(mxY);
	int correct = 0;
	for (int i = 0; i < testlabel.size(); i++) {
		if (result[i] == testlabel[i]) correct += 1;
	}
	double accuracy = correct / (double)all;
	// use result?

	delete[] dResult;
	delete[] result;
	// cleanup
ret:
	mxDestroyArray(mxTrainData);
	mxDestroyArray(mxTrainLabel);
	mxDestroyArray(mxTestData);
	mxDestroyArray(mxTestLabel);

	engClose(ep);
}

void Action_PolyOmino::OnBnClickedButtonSelectlandmarksTwopoints()
{
	// TODO: 在此添加控件通知处理程序代码
	if (pMesh->m_ArraySharpVertexIndex.size() != 2)
	{
		AfxMessageBox("Please select two points!");
	}
	int start, end;
	start = pMesh->m_ArraySharpVertexIndex[0];
	end = pMesh->m_ArraySharpVertexIndex[1];
	pMesh->m_SelectedLandmarkIndexArray.clear();
	GeoMapLandMark* pLandmark;

	//Find all the corners
	std::vector<std::vector<int>> m_CornerPointLandmarkArray;
	m_CornerPointLandmarkArray.resize(pMesh->m_ArrayVertex.GetSize());

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark = pMesh->m_LandMarkArray[i];
		m_CornerPointLandmarkArray[pLandmark->landmark_points[0]].push_back(i);
		m_CornerPointLandmarkArray[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]].push_back(i);
	}

	int direction = 0;

	if (m_CornerPointLandmarkArray[start].size() != 2)
		return;

	int landmarkindex = m_CornerPointLandmarkArray[start][direction];

	while (1)
	{
		pMesh->m_SelectedLandmarkIndexArray.push_back(landmarkindex);
		pLandmark = pMesh->m_LandMarkArray[landmarkindex];
		if (pLandmark->landmark_points[0] == start)
			start = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
		else
			if (pLandmark->landmark_points[pLandmark->landmark_points.size() - 1] == start)
				start = pLandmark->landmark_points[0];

		if (m_CornerPointLandmarkArray[start].size() > 2)
		{
			start = pMesh->m_ArraySharpVertexIndex[0];
			landmarkindex = m_CornerPointLandmarkArray[start][1 - direction];
			pMesh->m_SelectedLandmarkIndexArray.clear();
			continue;
		}
		if (m_CornerPointLandmarkArray[start][0] == landmarkindex)
			landmarkindex = m_CornerPointLandmarkArray[start][1];
		else
			landmarkindex = m_CornerPointLandmarkArray[start][0];

		if (start == end)
			break;

	}

	//SaveLandmark("P:\\Dr_YYJ\\data\\landmarkbak.txt");
	//
	SaveCurrentLandmak();
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedButtonSaveselectedlandmark()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FileName;
	CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;

	SaveSelectedLandmarks(FileName);
}


void Action_PolyOmino::OnBnClickedButtonLoadselectedlandmark()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FileName;
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;

	LoadSelectedLandmarks(FileName);
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedButtonAddlandmarks()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (pMesh->m_ArraySharpVertexIndex.size() != 1)
	//{
	//	AfxMessageBox("Please select one points!");
	//}

	//Find all the corners
	std::vector<std::vector<int>> m_CornerPointLandmarkArray;
	m_CornerPointLandmarkArray.resize(pMesh->m_ArrayVertex.GetSize());
	GeoMapLandMark* pLandmark;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark = pMesh->m_LandMarkArray[i];
		m_CornerPointLandmarkArray[pLandmark->landmark_points[0]].push_back(i);
		m_CornerPointLandmarkArray[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]].push_back(i);
	}

	int direction;
	int point;
	for (int j = 0; j < pMesh->m_ArraySharpVertexIndex.size(); j++)
	{
		point = pMesh->m_ArraySharpVertexIndex[j];
		//pMesh->m_SelectedLandmarkIndexArray.clear();

		direction = 0;

		if (m_CornerPointLandmarkArray[point].size() != 2)
			return;

		int landmarkindex = m_CornerPointLandmarkArray[point][direction];

		while (1)
		{
			if (std::find(pMesh->m_SelectedLandmarkIndexArray.begin(), pMesh->m_SelectedLandmarkIndexArray.end(), landmarkindex) != pMesh->m_SelectedLandmarkIndexArray.end())
			{
				break;
			}

			pMesh->m_SelectedLandmarkIndexArray.push_back(landmarkindex);
			pLandmark = pMesh->m_LandMarkArray[landmarkindex];
			if (pLandmark->landmark_points[0] == point)
				point = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];
			else
				if (pLandmark->landmark_points[pLandmark->landmark_points.size() - 1] == point)
					point = pLandmark->landmark_points[0];

			if (m_CornerPointLandmarkArray[point].size() != 2)
			{
				direction = 1 - direction;
				point = pMesh->m_ArraySharpVertexIndex[j];
				landmarkindex = m_CornerPointLandmarkArray[point][direction];
				//pMesh->m_SelectedLandmarkIndexArray.clear();
				if (direction == 1)
					continue;
				else
					break;
			}
			if (m_CornerPointLandmarkArray[point][0] == landmarkindex)
				landmarkindex = m_CornerPointLandmarkArray[point][1];
			else
				landmarkindex = m_CornerPointLandmarkArray[point][0];

			//if (start == end)
			//	break;

		}
	}

	//SaveLandmark("P:\\Dr_YYJ\\data\\landmarkbak.txt");
	//
	//SaveCurrentLandmak();
	pDoc->UpdateAllViews(NULL);

}


void Action_PolyOmino::OnBnClickedButtonOnepointlandmark()
{
	// TODO: 在此添加控件通知处理程序代码
	GeoMapLandMark* pLandmark;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark = pMesh->m_LandMarkArray[i];
		if (pLandmark->landmark_points.size() <= 1)
		{
			//delete
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
			i--;
		}
	}
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedButtonDetectOpenlandmark()
{
	// TODO: 在此添加控件通知处理程序代码
		//Find all the corners
	std::vector<std::vector<int>> m_CornerPointLandmarkArray;
	m_CornerPointLandmarkArray.resize(pMesh->m_ArrayVertex.GetSize());
	GeoMapLandMark* pLandmark;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark = pMesh->m_LandMarkArray[i];
		m_CornerPointLandmarkArray[pLandmark->landmark_points[0]].push_back(i);
		m_CornerPointLandmarkArray[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]].push_back(i);
	}

	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark = pMesh->m_LandMarkArray[i];
		if (m_CornerPointLandmarkArray[pLandmark->landmark_points[0]].size() == 1 ||
			m_CornerPointLandmarkArray[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]].size() == 1)
		{
			pMesh->m_SelectedLandmarkIndexArray.push_back(i);
		}
	}
	pDoc->UpdateAllViews(NULL);
}

int Action_PolyOmino::IsSameLandmark(GeoMapLandMark* pLandmark1, GeoMapLandMark* pLandmark2)
{
	if (pLandmark1->landmark_points.size() != pLandmark2->landmark_points.size())
		return 0;
	int flag = 0;
	for (int i = 0; i < pLandmark1->landmark_points.size(); i++)
	{
		if (pLandmark1->landmark_points[i] == pLandmark2->landmark_points[i])
			flag++;
	}
	if (flag == pLandmark1->landmark_points.size())
		return 1;
	flag = 0;
	for (int i = 0; i < pLandmark1->landmark_points.size(); i++)
	{
		if (pLandmark1->landmark_points[i] == pLandmark2->landmark_points[pLandmark1->landmark_points.size() - 1 -i])
			flag++;
	}
	if (flag == pLandmark1->landmark_points.size())
		return 1;
	return 0;
}
void Action_PolyOmino::OnBnClickedButtonDeleteDuplicateLandmark()
{
	// TODO: 在此添加控件通知处理程序代码
	std::vector<std::vector<int>> m_CornerPointLandmarkArray;
	m_CornerPointLandmarkArray.resize(pMesh->m_ArrayVertex.GetSize());
	GeoMapLandMark* pLandmark,*pLandmark1,*pLandmark2;
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark = pMesh->m_LandMarkArray[i];
		m_CornerPointLandmarkArray[pLandmark->landmark_points[0]].push_back(i);
		m_CornerPointLandmarkArray[pLandmark->landmark_points[pLandmark->landmark_points.size() - 1]].push_back(i);
	}
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark = pMesh->m_LandMarkArray[i];
		pLandmark->deleteflag = 0;
	}
	for (int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		if (m_CornerPointLandmarkArray[i].size() > 1)
		{
			for(int j = 0; j < m_CornerPointLandmarkArray[i].size(); j++)
				for (int k = j + 1; k < m_CornerPointLandmarkArray[i].size(); k++)
				{
					pLandmark1 = pMesh->m_LandMarkArray[m_CornerPointLandmarkArray[i][j]];
					pLandmark2 = pMesh->m_LandMarkArray[m_CornerPointLandmarkArray[i][k]];
					if (pLandmark1->deleteflag || pLandmark2->deleteflag)
						continue;

					if (IsSameLandmark(pLandmark1, pLandmark2))
					{
						pLandmark2->deleteflag = 1;
					}
				}
		}
	}
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		pLandmark = pMesh->m_LandMarkArray[i];
		if (pLandmark->deleteflag)
		{
			pMesh->m_LandMarkArray.erase(pMesh->m_LandMarkArray.begin() + i);
			i--;
		}

	}
	pDoc->UpdateAllViews(NULL);
}


void Action_PolyOmino::OnBnClickedButtonSubdividesharpeedge()
{
	// TODO: Add your control notification handler code here
	Polyomino_Edge_Subdivision dialog;
	int m_Edge_Subdivision_Num;
	if (dialog.DoModal() == IDOK)
	{
		m_Edge_Subdivision_Num = dialog.m_EdgeSubdivisionNum;
	}

	//
	//pFinalRectilinearMesh->m_ArraySharpVertexIndex = pMesh->m_ArraySharpVertexIndex;

	//
	SubdivideMeshEdge(pMesh, m_Edge_Subdivision_Num);

	//
	//SubdivideMeshEdge(pFinalRectilinearMesh, m_Edge_Subdivision_Num);

	//SaveCurrentWorldMap
	//SaveCurrentWorldMap();

	//
	//SaveCurrentRectilinearWorldMap();

	pMesh->SetModified(1);
	pDoc->UpdateAllViews(NULL);
}
