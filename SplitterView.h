#if !defined(AFX_SPLITTERVIEW_H__C50FD508_CEFA_11D1_9B01_0060970863EE__INCLUDED_)
#define AFX_SPLITTERVIEW_H__C50FD508_CEFA_11D1_9B01_0060970863EE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SplitterView.h : header file
//
#include "MySplitterWnd.h" // for CMySplitterWnd

/////////////////////////////////////////////////////////////////////////////
class CSplitterView : public CView
{
public:	
	CSplitterView();           
	DECLARE_DYNCREATE(CSplitterView)

// Attributes
public:
	int type;
	bool m_bInitialized;
	bool m_bShouldSetXColumn;
	CMySplitterWnd m_wndSplitter,m_wndSplitter1;
	CMySplitterWnd* m_pWndSplitter;
	HGLRC m_hRC;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSplitterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplitterView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTERVIEW_H__C50FD508_CEFA_11D1_9B01_0060970863EE__INCLUDED_)
