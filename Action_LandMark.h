#pragma once
#include <vector>
#include "Point3D.h"
// Action_LandMark dialog

class Actions_LandMark : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_LandMark)

public:
	Actions_LandMark();
	virtual ~Actions_LandMark();

	//bone
	void OnRemoveBoundaries_bone();

	void OnRemoveBoundaries();

	std::vector<std::pair<double,double>> m_Array;

	//Rotate LandMark
	void OnRotateLandMark();

	//Compute the circle center
	void  circle_center(Vector3D  *center,Vector3D  *pt,double  *radius);

// Dialog Data
	enum { IDD = IDD_LANDMARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadlandmark();
	afx_msg void OnBnClickedReadsurfacewith();
	afx_msg void OnBnClickedReadsurfaceplane();
	afx_msg void OnBnClickedReadlionsurfacewithgraph();
	afx_msg void OnBnClickedCopyselectedpointstoanchorpoints();
	afx_msg void OnBnClickedSaveanchorpoints();
	afx_msg void OnBnClickedLandmarkComputemu();
	afx_msg void OnBnClickedLandmarkComputeareastretch();
	afx_msg void OnBnClickedLandmarkComputemuRegistration();
	afx_msg void OnBnClickedLandmarkComputeareastretchRegitration();
};
