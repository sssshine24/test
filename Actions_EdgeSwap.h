#pragma once


// Actions_EdgeSwap 对话框

class Actions_EdgeSwap : public CDialogEx
{
	DECLARE_DYNAMIC(Actions_EdgeSwap)

public:
	Actions_EdgeSwap(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Actions_EdgeSwap();

	double alpha_, belta_, gama_;
	//
	void PlotInitialMeshCircularEdgeFlipPointMove(CPaintDC &dc);

	//
	void PlotInitialMeshCircularEdgeFlip(CPaintDC &dc);
	//
	void PlotInitialMeshCircular(CPaintDC &dc);
	//
	void PlotInitialMesh(CPaintDC &dc);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDGESWAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
