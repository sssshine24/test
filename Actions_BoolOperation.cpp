// Actions_BoolOperation.cpp: 实现文件
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Actions_DataAnalysis.h"
#include "afxdialogex.h"
#include "Lib3D/Mesh3d.h"
#include "Action_PolyOmino.h"
#include "svm/Predictor.h"
#include "FormView3.h"
#include "lib3d/Base3d.h"
#include "Lib3D/Mesh3d_OpenMesh.h"
#include <GL\glut.h>
#include "CFramebuffer.h"
#include "MyTreeView.h"
#include <shlwapi.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include "Actions_BoolOperation.h"


// Actions_BoolOperation 对话框
// 消息映射

BEGIN_MESSAGE_MAP(Actions_BoolOperation, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_BOOLOPERATION_INTERSECTIONCURVES, &Actions_BoolOperation::OnBnClickedButtonBooloperationIntersectioncurves)
END_MESSAGE_MAP()

Actions_BoolOperation::Actions_BoolOperation()
{

}
Actions_BoolOperation::~Actions_BoolOperation()
{
}


void Actions_BoolOperation::OnBnClickedButtonBooloperationIntersectioncurves()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();

	//CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph->GetAt(0);
	//CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph->GetAt(1);

	//Transform Mesh1 & Mesh2 

	//Compute the intersection

}
