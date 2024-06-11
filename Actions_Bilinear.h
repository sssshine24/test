#pragma once


// Actions_Bilinear dialog
class CMesh3d;
class CNurbsSuface;
class Actions_Bilinear : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_Bilinear)

public:
	Actions_Bilinear();
	virtual ~Actions_Bilinear();
	double ComputeConformalEnergy(CNurbsSuface* pSurf);

// Dialog Data
	enum { IDD = IDD_ActionsBilinear };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBilinearOptimize();
};
