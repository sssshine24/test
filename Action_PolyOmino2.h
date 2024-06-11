#pragma once
#include "afxcmn.h"


// Action_PolyOmino2 dialog
class CObject3d;
class CMesh3d;
class Action_PolyOmino2 : public CPropertyPage
{
	DECLARE_DYNAMIC(Action_PolyOmino2)

public:
	std::vector<int> m_SourceSharpEdges;
	std::vector<int> m_TargetSharpEdges;
	std::vector<int> m_ArrayDeleteFaces;
	int iterationnum;
	Action_PolyOmino2();
	virtual ~Action_PolyOmino2();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POLYOMINO2 };
#endif
public:
	CObject3d* pMesh,*pMesh_UV;
	//
	void SearchForConnectedCells(std::vector<std::vector<int>> &cellfaceindex, std::vector<std::pair<int, int>> &m_ArrayConnectedCells, int xnum, int ynum, int i, int j);

	//
	void OutputRastizationOneEdgePovRay(std::vector<std::vector<int>> &cellfaceindex, int xnum, int ynum, CMesh3d* pNewMesh3D, std::ofstream &outfile, int i, int j, int xoffset, int yoffset);

	//
	void OutputRastizationPovRay(std::vector<std::vector<int>> &cellfaceindex, int xnum, int ynum, CMesh3d* pNewMesh3D, char* filename);

	//
	int FindPointLocatedPolygon(CMesh3d* pMesh, std::vector<std::vector<int>>& m_ArrayFacePointsArray, double x, double y);

	int FindCellLocatedtPolygon(CMesh3d* pMesh, double x1, double y1, double x2, double y2);

	//
	//void AdjustHorizontalandVerticalGroupofLinePositions_3base();

	//
	//void AdjustHorizontalandVerticalGroupofLinePositions_3base();
	//
	//
	void RemoveDeletedFaces();

	//
	void MergeHorizontalAndVerticalSegments();

	//
	void UpdateSelectedSharpEdgePosition_Base(CMesh3d* pMesh, int deltax, int deltay);

	//
	int IsFaceEqualPoints(int i, int j);

	//
	void RemoveDulicateFaces();

	//
	void SetStatesArray_worldmap();

	//
	void UpdateSelectedSharpEdgePosition_(CMesh3d* pMesh, double deltax, double deltay);

	//
	void UpdateSelectedSharpEdgePosition(CMesh3d* pMesh, int deltax, int deltay);

	//
	void UpdateSelectedLandmarkPosition(CMesh3d* pMesh, double deltax, double deltay);

	//
	void GetFinalRectilinearGeomap();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetmeshuvMeshPolyomino();
	afx_msg void OnBnClickedButtonSetmeshuvUvPolyomino();
	afx_msg void OnBnClickedButtonCopyselectedlandmarkvertexposition();
	afx_msg void OnBnClickedButtonLoadfinalrectilinearmap();
	double m_Step;
	afx_msg void OnBnClickedButtonUpPolyomino();
	afx_msg void OnBnClickedButtonRightPolyomino();
	afx_msg void OnBnClickedButtonDownPolyomino();
	afx_msg void OnBnClickedButtonLeftPolyomino();
	afx_msg void OnBnClickedButtonUpdateStepPolyomino();
	afx_msg void OnBnClickedButtonLoadFirstuniformgrid();
	afx_msg void OnBnClickedButtonInitializeuniformgrid();
	afx_msg void OnBnClickedButtonClearselectedsharpedges();
	afx_msg void OnBnClickedButtonSetsourcesharpedge();
	afx_msg void OnBnClickedButtonSettargetsharpedge2();
	afx_msg void OnBnClickedButtonMovesourcetotarget();
	afx_msg void OnBnClickedButtonUpdatesharpstructure();
	afx_msg void OnBnClickedButtonSubdividesharpedge();
	int m_Subdivision_Index;
	afx_msg void OnBnClickedButtonDeleteselectedsharpedges();
	afx_msg void OnBnClickedButtonUpPolyomino2();
	afx_msg void OnBnClickedButtonRightPolyomino2();
	afx_msg void OnBnClickedButtonLeftPolyomino2();
	afx_msg void OnBnClickedButtonDownPolyomino2();
	afx_msg void OnBnClickedButtonDeleteselectedsharpedges2();
	afx_msg void OnBnClickedButtonDeleteselectedsharpedges3();
	afx_msg void OnBnClickedButtonDeleteselectedsharpedges4();
	afx_msg void OnBnClickedButtonPolyominoGeneratenewpoint();
	afx_msg void OnBnClickedButtonPolyominoGeneratetwopointline();
	afx_msg void OnBnClickedButtonPolyominoGeneratenewpoint2line();
	afx_msg void OnBnClickedButtonLoadFinaluniformgrid();
	afx_msg void OnBnClickedButtonVisualizationpopulation();
	afx_msg void OnBnClickedButtonMiddlecenter();
	int m_MinIndex;
	int m_MaxIndex;
	afx_msg void OnBnClickedButtonAverageHorizontal();
	afx_msg void OnBnClickedButtonAverageVertical();
	CSpinButtonCtrl m_SpinRows;
	CSpinButtonCtrl m_spinColumns;
	virtual BOOL OnInitDialog();
	CSpinButtonCtrl m_SpinPuzzleSize;
	afx_msg void OnThemechangedSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnThemechangedSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnThemechangedSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_PuzzleSize;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonAddpuzzle();
	afx_msg void OnBnClickedButtonLoadpuzzle();
	afx_msg void OnBnClickedButtonSavepuzzle();
	int m_PuzzleXNum;
	int m_PuzzleYNum;
	CSpinButtonCtrl m_SpinXNum;
	CSpinButtonCtrl m_SpinYNum;
	afx_msg void OnBnClickedButtonUpPolyominoBase();
	afx_msg void OnBnClickedButtonLeftPolyominoBase();
	afx_msg void OnBnClickedButtonDownPolyominoBase();
	afx_msg void OnBnClickedButtonRightPolyominoBase();
	afx_msg void OnBnClickedButtonSetfacepointsSelected();
	afx_msg void OnBnClickedButtonSetfacepointsDelete();
	afx_msg void OnBnClickedButtonSavedeletedfaces();
	afx_msg void OnBnClickedButtonSavepuzzlepiece();
	afx_msg void OnBnClickedButtonSavepuzzlepiece2();
	afx_msg void OnBnClickedButtonLoadpuzzlepiece();
	afx_msg void OnBnClickedButtonCheckpointwithfiveregions();
	afx_msg void OnBnClickedButtonUsrastrerization();
	afx_msg void OnBnClickedButtonLoadchina();
	afx_msg void OnBnClickedButtonLoadchinaTriangulate();
	afx_msg void OnBnClickedButtonSetalllandmarkRegular();
	afx_msg void OnBnClickedButtonDeleteduplicatepointsinlandmark();
	afx_msg void OnBnClickedButtonLoadchinastraightened();
	afx_msg void OnBnClickedButtonSavedatafrommesh();
	afx_msg void OnBnClickedButtonLoadrectilinearmesh();
	afx_msg void OnBnClickedButtonModifyrectilinear3414();
	afx_msg void OnBnClickedButtonModifyrectilinearmesh3350();
	afx_msg void OnBnClickedButtonLoadfinalchinapolyomino();
};
