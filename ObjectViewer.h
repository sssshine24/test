#pragma once
#include "afxcmn.h"


// ObjectViewer dialog

class ObjectViewer : public CPropertyPage
{
	DECLARE_DYNAMIC(ObjectViewer)

public:
	ObjectViewer();
	virtual ~ObjectViewer();

	int		m_slider1;
	int		m_slider2;

// Dialog Data
	enum { IDD = IDD_objViewerTab };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIsoparametricLine();
	afx_msg void OnNMRClickObjects(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickObjects(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTexture();
	afx_msg void OnBnClickedCheck1();
	CSliderCtrl m_MoveSpeedSlider;
	CSliderCtrl m_FovySlider;
	afx_msg void OnBnClickedZoomall();
	afx_msg void OnBnClickedUpdateTreecontrol();
	afx_msg void OnBnClickedExtractFreeformtransformation();
	afx_msg void OnBnClickedOpenmeshClearVertexSelection();
	afx_msg void OnNMCustomdrawSliderfov(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlidermovespeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnSelchangedObjects(NMHDR* pNMHDR, LRESULT* pResult);
	CTreeCtrl m_tree;
};
