// Actions_General.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Actions_General.h"
#include "Lib3D/Base3d.h"
#include "Lib3D/SceneGraph3d.h"
#include "FormView3.h"

#include  <io.h>
#include  <string>
#include  <vector>
#include <afx.h>
#include <windows.h>
#include "MyTreeView.h"
using   namespace   std;

int sequence = 0;//显示文件的序号
int objnum = 0;//统计obj文件数

// Actions_General dialog
int lag = 0;
int post = 0;
int done = false;

IMPLEMENT_DYNAMIC(Actions_General, CPropertyPage)

Actions_General::Actions_General()
	: CPropertyPage(Actions_General::IDD)
{

}

Actions_General::~Actions_General()
{
}

void Actions_General::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIMESLIDER, m_slider);
}


BEGIN_MESSAGE_MAP(Actions_General, CPropertyPage)
	ON_BN_CLICKED(IDC_ROTATION_PARAMETERIZATION, &Actions_General::OnBnClickedRotationParameterization)
	ON_BN_CLICKED(IDC_StopSequence, &Actions_General::OnBnClickedStopsequence)
	ON_BN_CLICKED(IDC_AnimationSequence, &Actions_General::OnBnClickedAnimationsequence)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TIMESLIDER, &Actions_General::OnNMCustomdrawTimeslider)
	ON_BN_CLICKED(IDC_Continue, &Actions_General::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_ReadFile, &Actions_General::OnBnClickedReadfile)
	ON_BN_CLICKED(IDC_SaveFile, &Actions_General::OnBnClickedSavefile)
END_MESSAGE_MAP()


// Actions_General message handlers

extern CSDIViewSwitchDoc* pDoc;
void CALLBACK timerTest(HWND hWnd,      // handle of CWnd that called SetTimer
						UINT nMsg,      // WM_TIMER
						UINT_PTR nIDEvent,   // timer identification
						DWORD dwTime    // system time
						)
{
	((CMesh3d*)(pDoc->m_SceneGraph.GetAt(sequence)))->m_Show=0;
	sequence=(++sequence)%objnum;
	((CMesh3d*)(pDoc->m_SceneGraph.GetAt(sequence)))->m_Show=1;
	pDoc->UpdateReadTree();
	pDoc->UpdateShowView();
}
void Actions_General::OnBnClickedRotationParameterization()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	p_FormView3->currentindex = 0;
	if(p_FormView3->currentindex < 0 || p_FormView3->currentindex >= num)
		return;

	if(pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);

		double centerx,centery;
		double averagex,averagey;
		double sumx = 0;
		double sumy = 0;
		int n = pMesh->NbVertex();
		double term1 = 0;
		double term2 = 0;
		//first compute the inclination angle.
		for(int i = 0; i < n; i++)
		{
			CVertex3d* pVertex = pMesh->GetVertex(i);

			double x = pVertex->x();
			double y = pVertex->y();

			sumx += x;
			sumy += y;
		}
		centerx = sumx / n;
		centery = sumy / n;

		averagex = 0;
		averagey = 0;
		for(int i = 0; i < n; i++)
		{
			CVertex3d* pVertex = pMesh->GetVertex(i);

			double x = pVertex->x();
			double y = pVertex->y();

			term1 += x * y;
			term2 += x * x;

			averagex += x;
			averagey += y;
		}
		averagex = averagex / n;
		averagey = averagey / n;
		
		double k = (term1 - n * averagex * averagey) / (term2 - n * averagex * averagex);
		double angle = PI / 2 + atan (k);
		double cosangle = cos(angle);
		double sinangle = sin(angle);

		CMesh3d* pMesh1 = new CMesh3d();
		pMesh1->Copy(pMesh);
		for(int i = 0; i < n; i++)
		{
			CVertex3d* pVertex = pMesh1->GetVertex(i);

			double x = pVertex->x();
			double y = pVertex->y();

			x = x- centerx;
			y = y - centery;

			double x1 = x * cosangle - y * sinangle + centerx;
			double y1 = x * sinangle + y * cosangle + centery;

			pVertex->x(x1);
			pVertex->y(y1);
		}

		angle = PI / 2 - atan (k);
		cosangle = cos(angle);
		sinangle = sin(angle);

		CMesh3d* pMesh2 = new CMesh3d();
		pMesh2->Copy(pMesh);
		for(int i = 0; i < n; i++)
		{
			CVertex3d* pVertex = pMesh2->GetVertex(i);

			double x = pVertex->x();
			double y = pVertex->y();

			x = x- centerx;
			y = y - centery;

			double x1 = x * cosangle - y * sinangle + centerx;
			double y1 = x * sinangle + y * cosangle + centery;

			pVertex->x(x1);
			pVertex->y(y1);
		}
		pDoc->m_SceneGraph.Add(pMesh1);
		pDoc->m_SceneGraph.Add(pMesh2);
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
}
void Actions_General::OnBnClickedStopsequence()
{
	// TODO: 在此添加控件通知处理程序代码
	//TRACE("***********************");
	KillTimer(1);
	done=false;
}

void Actions_General::OnBnClickedAnimationsequence()
{
	// TODO: 在此添加控件通知处理程序代码
	//TRACE("***********************OnBnClickedAnimationsequence");
	pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	vector<string> files;
	string path="E:\\download\\budata";
	//文件句柄
	long hFile=0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if((hFile=_findfirst(p.assign(path).append("\\*").c_str(),&fileinfo))!=-1)
	{
		do
		{
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));//将文件名字存储

		}while(_findnext(hFile, &fileinfo)==0);
		_findclose(hFile);
	}
	int len=files.size();
	SetCurrentDirectory(path.c_str());
	for(int i=0;i<len;i++)
	{
		CString file(files[i].c_str());
		CString extension=file;
		extension = extension.Right(extension.GetLength() - extension.ReverseFind('.'));
		extension.MakeLower();
		if(extension==".obj")
		{
			objnum++;
			TRACE("%s\n",file);
			TRACE("start");
			LPCSTR cpath=file;
			CParseOBJ parser;
			if(parser.Run2((char *)cpath,&pDoc->m_SceneGraph))
			{
				pDoc->m_SceneGraph.BuildAdjacency();
				pDoc->m_SceneGraph.CalculateNormalPerFace();
				pDoc->m_SceneGraph.CalculateNormalPerVertex();
			}	
		}		
	}
	for (int j=0;j<objnum;j++)
	{
		((CMesh3d*)(pDoc->m_SceneGraph.GetAt(j)))->m_Show=0;
	}
	((CMesh3d*)(pDoc->m_SceneGraph.GetAt(sequence)))->m_Show=1;

	pDoc->UpdateTreeControl();
	pDoc->UpdateShowView();
	done=TRUE;
	m_slider.SetPos(0);//设置进度条为0则设置显示速度为1s）
	lag=1000;	
	SetTimer(1,lag,timerTest);
	TRACE("读入完毕！");
}



void Actions_General::OnNMCustomdrawTimeslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_slider.SetLineSize(99);
	int post=m_slider.GetPos();
	lag=1000-post*10;
	if (done)
	{
		SetTimer(1,lag,timerTest);
	}
}

void Actions_General::OnBnClickedContinue()
{
	// TODO: 在此添加控件通知处理程序代码
	///done=true;
	//if (lag!=0)
	//{
	//	SetTimer(1,lag,timerTest);
	//}
	done=true;	
	lag=1000-m_slider.GetPos()*10;
	SetTimer(1,lag,timerTest);
}

void Actions_General::OnBnClickedReadfile()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal()==IDOK)
	{
		FileName=dlg.GetPathName();
		char *name=FileName.GetBuffer(0);
		pDoc->m_SceneGraph.ReadFile(name);
		objnum=pDoc->m_SceneGraph.NbObject();
		pDoc->UpdateReadTree();
		pDoc->UpdateShowView();
	}

}

void Actions_General::OnBnClickedSavefile()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	CString filename;
	CFileDialog dlg(FALSE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal()==IDOK)
	{
		filename=dlg.GetPathName();
		char *name=filename.GetBuffer(0);
		pDoc->m_SceneGraph.SaveFileRaw(name);
	}
}
