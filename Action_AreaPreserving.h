#pragma once


// Action_AreaPreserving dialog
class C_NurbsSurf;
class Action_AreaPreserving : public CPropertyPage
{
	DECLARE_DYNAMIC(Action_AreaPreserving)

public:
	Action_AreaPreserving();
	virtual ~Action_AreaPreserving();

	//Generate a initial face 
	void GenerateFaceComposite_InitialSurface(C_NurbsSurf* pSurface);

	//
	void ComputeMaximal_Minimal_Pos_uv(std::vector<double> &m_Array_Zero_Derivatives,int SUBDIVISION_NUM, C_NurbsSurf* pNurbsSurf,int flag);

	//
	void ComputeOptimalSubdivision(std::vector<double> &m_ArrayUsubdivision,std::vector<double> &m_ArrayVsubdivision, C_NurbsSurf* pNurbsSurf);

	//
	double Compute_Optimal_Composite_Mobiustransformation(int Mseg,int Nseg,C_NurbsSurf* pNurbsSurf);

	//
	void CheckMobius_CompositeMobius_Ability();

// Dialog Data
	enum { IDD = IDD_AreaPreserving };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCartopstep1();
	afx_msg void OnBnClickedCartopstep2();
	afx_msg void OnBnClickedFace2step1();
	afx_msg void OnBnClickedFace2step2();
	afx_msg void OnBnClickedBezierexamplestep1();
	afx_msg void OnBnClickedCompositereparabezier();
	afx_msg void OnBnClickedFacecompositetransformation();
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnBnClickedFaceexample();
	afx_msg void OnBnClickedBonestep1();
	afx_msg void OnBnClickedButton6();
};
