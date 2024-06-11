// Polyomino_Edge_Subdivision.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Polyomino_Edge_Subdivision.h"
#include "afxdialogex.h"


// Polyomino_Edge_Subdivision dialog

IMPLEMENT_DYNAMIC(Polyomino_Edge_Subdivision, CDialog)

Polyomino_Edge_Subdivision::Polyomino_Edge_Subdivision(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_EDGE_SUBDIVISION, pParent)
{

}

Polyomino_Edge_Subdivision::~Polyomino_Edge_Subdivision()
{
}

void Polyomino_Edge_Subdivision::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Polyomino_Edge_Subdivision, CDialog)
	ON_BN_CLICKED(IDOK, &Polyomino_Edge_Subdivision::OnBnClickedOk)
END_MESSAGE_MAP()


// Polyomino_Edge_Subdivision message handlers


void Polyomino_Edge_Subdivision::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString str;
	((CEdit*)GetDlgItem(IDC_EDIT_EDGE_SUBDIVISION))->GetWindowText(str);
	m_EdgeSubdivisionNum = atoi(str);
	CDialog::OnOK();
}
