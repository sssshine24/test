#pragma once

#include <string>  
#include <io.h>  
#include <vector>  
#include <iostream>
#include "afxcmn.h"
// Actions_DataAnalysis dialog
class CMesh3d;
class CMesh3d_OpenMesh;
class Actions_DataAnalysis : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_DataAnalysis)

public:
	std::vector<std::string> files,filesnewset;
	std::map<std::string, std::vector<int>> m_ArrayEndPoints;
	char * filePath;
	int m_currentindex;
	int iternum_global;
	int edgeindex_global;
	int vindex1_global, vindex2_global;
	double step_global;

	CMesh3d* pSoureceSurface_Mesh3d;
	CMesh3d* pTargetSurface_Mesh3d;
	CMesh3d_OpenMesh* pSoureceSurface_Openmesh;
	CMesh3d_OpenMesh* pTargetSurface_Openmesh;
	CMesh3d_OpenMesh* pSoureceSurface_Openmesh_Morph;
	CMesh3d_OpenMesh* pTargetSurface_Openmesh_Morph;
	CMesh3d_OpenMesh* pIntermediateSurface_Openmesh_Morph;

	int m_DataAnlysis_Pos;
	Actions_DataAnalysis();
	virtual ~Actions_DataAnalysis();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DATA_ANALYSIS };
#endif
public:
	//
	void FixBoundaryVertices(CMesh3d_OpenMesh* pSourceMesh, CMesh3d_OpenMesh* pTargetMesh);

	//
	int SetMeshPositionandUV(CMesh3d_OpenMesh* pSourceMesh_New, CMesh3d_OpenMesh* pSourceMesh);

	//
	void AddResultMesh_CDT(CMesh3d_OpenMesh* pSourceMesh_New, CMesh3d_OpenMesh* pSourceMesh);

	//
	double ComputeIntersectionPointonGivenDiagonalRatio(CMesh3d_OpenMesh* pSourceMesh, int edgeindex);

	//
	int SearchMeshMappingSequencesForSpecificDiagonalSwap(int vh1, int vh2);

	//
	void AddMeshMappingtoSequence(CMesh3d_OpenMesh* pSourceMesh_New, CMesh3d_OpenMesh* pSourceMesh, int edgeindex, double step);

	//
	void MeshCopyPosition(CMesh3d_OpenMesh* pSourceMesh, CMesh3d_OpenMesh* pTargetMesh);

	//
	void CombineAllMappingsByAddingSteinerPoints(CMesh3d_OpenMesh* pSourceMesh);

	//
	double ComputeIntersectionPointonAnotherDiagonalRatio(CMesh3d_OpenMesh* pSourceMesh, int edgeindex);

	//
	void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic_CDT_Debug();

	//
	void GenerateQuasiConformalMuImage_CDT_PRE(const char* sourcemesh, const char* targetmesh, const char* muimagefilename);

	//
	int HarmonicIterationsbyEdgeSwap_CDT_DEBUG(CMesh3d_OpenMesh* pSourceMesh);

	//
	int HarmonicIterationsbyEdgeSwap_CDT(CMesh3d_OpenMesh* pSourceMesh);

	//
	void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic_Debug_CDT();

	//
	void UpdateResultantMesh_Debug(CMesh3d_OpenMesh* pSourceMesh);

	//
	void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic_Debug();

	//
	void AddResultantMesh_Debug(CMesh3d_OpenMesh* pSourceMesh);

	//
	void OnBnClickedButtonZoomCenteredAtvertex(CMesh3d_OpenMesh* pOpenMesh, int vindex);

	//
	void GenerateQuasiConformalMuImage_Debug_Iterations();

	//
	void GenerateQuasiConformalMuImage_Debug_Pre(const char* sourcemesh, const char* targetmesh, const char* muimagefilename);

	//
	void GenerateQuasiConformalMuImage_Debug_After(const char* sourcemesh, const char* targetmesh, const char* muimagefilename);

	//
	void FixBoundaryandOneInnerVertex(CMesh3d_OpenMesh* pSoureceSurface_Openmesh, CMesh3d_OpenMesh* pTargetSurface_Openmesh);

	//
	BOOL MorphIntermediateMesh();

	//
	void ReadResultantMesh(std::string fullfilename);

	//
	void SaveResultantMesh(CMesh3d_OpenMesh* pSourceMesh);

	//
	void GenerateQuasiConformalMuImage_CDT(const char* sourcemesh, const char* targetmesh, const char* muimagefilename);

	//
	void GenerateQuasiConformalMuImage(const char* sourcemesh, const char* targetmesh, const char* muimagefilename);

	//
	void PlotMeshMu(CMesh3d_OpenMesh* pSourceMesh);

	//
	void DrawMuImageinBuffer();

	//
	void GetMuColorFromMesh(CMesh3d_OpenMesh* pSourceMesh, double x, double y, COLORREF color);

	//
	void OutPutMeshMutoFile(CMesh3d_OpenMesh* pSourceMesh, char* filename);

	//
	void AddResultantMesh(CMesh3d_OpenMesh* pSourceMesh);

	//
	int HarmonicIterationsbyEdgeSwap_Debug(CMesh3d_OpenMesh* pSourceMesh);

	//
	int HarmonicIterationsbyEdgeSwap(CMesh3d_OpenMesh* pSoureceSurface_Openmesh);

	//
	double FindColinearParameter(CMesh3d_OpenMesh* pSourceMesh, OpenMesh::FaceHandle fh);

	//
	void AddLandmarkEndPointsToFixArray(CMesh3d_OpenMesh* pSourceMesh, CMesh3d_OpenMesh* pTargetMesh);

	void CreateStretchImage(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth);

	void SVM_MULTIPLECLASSIFY_MATLAB(int typenum, std::vector<std::vector<double>> &train_data,
		std::vector<int> &train_label, std::vector<std::vector<double>> &test_data, std::vector<int> &test_label);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDataanalysisLoadfirst();
	afx_msg void OnBnClickedButtonDataanalysisLoadnext();
	afx_msg void OnBnClickedButtonDataanalysisAddtoset();
	afx_msg void OnBnClickedButtonDataanalysisSavefileset();
	afx_msg void OnBnClickedButtonDataanalysisLoadfileset();
	afx_msg void OnBnClickedButtonDataanalysisLoadsetfirst();
	afx_msg void OnBnClickedButtonDataanalysisLoadsetnext();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysis();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisCnn();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisCnnNormalize();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisComputedata();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisPreneutral();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisPreneutralFourlevel();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisPreneutralFourlevel2();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisImageCnn();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisCnnMatlab();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic();
	afx_msg void OnBnClickedButtonZoomCenteredAtvertex();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisReadsavedmesh();
	afx_msg void OnBnClickedButtonSetmeshuvDataanalysisMesh();
	afx_msg void OnBnClickedButtonSetmeshuvDataanalysisTarget();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisGeneratemorphmesh();
	CSliderCtrl m_DataAnalysisMorph_Slider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonictest();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicPre();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterations();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsEdgeswap();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicAfter();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicPreCdt();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsCdt();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsEdgeswapCdt();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicAfterCdt();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicCdt();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicPreCdt2();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsEdgeswapCdtMove();
	afx_msg void OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicCdtExample1();
};
