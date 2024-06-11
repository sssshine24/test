#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class MeshMapping
{
public:
	CMesh3d_OpenMesh* pStartMesh;
	//CMesh3d_OpenMesh* pEndMesh;
	BOOL m_IsEdgeSwap;
	BOOL m_IsAddSteinerPoint;
	int	 m_StartEdgeIndex;
	int  m_EndEdgeIndex;
	double step;
	std::pair<int, int> m_SwapEdgeEndPonits;
};
// Actions_DynamicQuasiconformal dialog

class Actions_DynamicQuasiconformal : public CDialogEx
{
	DECLARE_DYNAMIC(Actions_DynamicQuasiconformal)

public:
	double meshleft_convexpara;
	double meshright_convexpara;
	double meshtop_convexpara;
	double meshbottom_convexpara;
	double scale_convexpara;
	double scale_targetpara;

	double meshleft_targetpara;
	double meshright_targetpara;
	double meshtop_targetpara;
	double meshbottom_targetpara;

	int m_FirstConvexMeshPlot;
	int m_FirstTargetMeshPlot;
	int selectvindex;
	int convexmesh_onlyshowboundary;
	int tarmesh_onlyshowboundary;
	CMesh3d* p3DSurface_Mesh3d;
	CMesh3d* pconvex_para_Mesh3d;
	CMesh3d* ptarget_para_Mesh3d;

	CMesh3d_OpenMesh* p3DSurface_Openmesh;
	CMesh3d_OpenMesh* pconvex_para_Openmesh;
	CMesh3d_OpenMesh* ptarget_para_Openmesh;
	CMesh3d_OpenMesh* ptarget_para_Openmesh_current;
	CMesh3d_OpenMesh* ptarget_para_Openmesh_Intermediate;

	std::vector<MeshMapping*> m_ArrayMeshMappingSequences;

	//
	BOOL m_ShowMeshMapping;

	//
	int m_CurrentMeshMapping;

	//
	int m_show_target_intermediate;

	//
	double GetUVHarmonicEnergy(CMesh3d_OpenMesh* pOpenmesh);

	//
	void SetIntermediateMesh();

	//
	void findCircle(double x1, double y1, double x2, double y2, double x3, double y3, double &centerx, double &centery, double &radius);

	//
	double ComputeStepDelaunay(CMesh3d_OpenMesh* pMesh3D, int ehindex, double step1, double step2);

	//
	double GetSmallestStep(CMesh3d_OpenMesh* pMesh3D, int ehindex);

	//
	double ComputeSumOppositeAngle(CMesh3d_OpenMesh* pMesh3D, int ehindex, double t);

	//
	double SearchSmallestStep_CDT(CMesh3d_OpenMesh* pCurrentMesh, int &edgeindex);

	//
	double SearchSmallestStep(CMesh3d_OpenMesh* pCurrentMesh, int &edgeindex);

	//
	double MeshDifferences(CMesh3d_OpenMesh* pMesh1, CMesh3d_OpenMesh* pMesh2);

	//
	void PlotTargetIntermediatePara(CPaintDC &dc);

	//
	void GetHarmonicIterations(CMesh3d_OpenMesh* pCurrentMesh);

	//
	void PlotCurrentMeshMapping(CPaintDC &dc);

	//
	void GetCDTMesh_Simplify(CMesh3d_OpenMesh* pCurrentMesh);

	//
	void GetCDTMesh(CMesh3d_OpenMesh* pCurrentMesh);

	//Plot Mesh Para
	void PlotConvexMeshPara(CPaintDC &dc, BOOL Is_Boundary);

	//
	void PlotTargetMeshPara(CPaintDC &dc, BOOL Is_Boundary);

	Actions_DynamicQuasiconformal(CWnd* pParent = NULL);   // standard constructor
	virtual ~Actions_DynamicQuasiconformal();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DynamicQuasi };
#endif
public:
	double GetHarmonicEnergy(CMesh3d_OpenMesh* pOpenmesh);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDynamicQuasiLoad3dsurface();
	afx_msg void OnBnClickedDynamicComputeharmonicenergy();
	afx_msg void OnBnClickedDynamicCheckdelaunay();
	afx_msg void OnBnClickedDynamicQuasiLoadtwotriangles();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedDynamicSettargetboundary();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedDynamicSavetargetparamesh();
	afx_msg void OnBnClickedDynamicLoadtargetparamesh();
	afx_msg void OnBnClickedDynamicDynamicharmonic();
	afx_msg void OnBnClickedDynamicDynamicharmonicMappingsequences();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_DynamicMappingSlider;
	CSpinButtonCtrl m_DynamicMappingSpin;
	CEdit m_DynamicMappingEdit;
	CString m_DynamicMappingValue;
	CComboBox m_ActionsCombo;
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposDynamicquasiconformalSpin(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_ShowIntialConvexPara;
	CButton m_ShowInitialTargetPara;
	CButton m_ShowIntermediateTargetPara;
	afx_msg void OnBnClickedDynamicquasiShowinitialconvexpara();
	afx_msg void OnBnClickedDynamicquasiShowinitialtargetpara();
	afx_msg void OnBnClickedDynamicquasiShowintermediatetargetpara();
	CButton m_ShowIndex;
	CButton m_ShowCircle;
	afx_msg void OnBnClickedDynamicOptimalharmonic();
	afx_msg void OnBnClickedDialogDynamicquasiTargetparaStatic();
	afx_msg void OnNMThemeChangedScrollbar1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton15();
	afx_msg void OnEnChangeDynamicquasiEdit();
};
