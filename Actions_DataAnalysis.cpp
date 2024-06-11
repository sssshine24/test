// Actions_DataAnalysis.cpp : implementation file
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
#include "Actions_DynamicQuasiconformal.h"
//#pragma comment(lib,"Shlwapi.lib")
//#include <gl/glew.h>
using namespace std;
// Actions_DataAnalysis dialog
Actions_DataAnalysis* pDataAnalysis;
IMPLEMENT_DYNAMIC(Actions_DataAnalysis, CPropertyPage)
extern Actions_DynamicQuasiconformal* pDynamicQuasiconformal;
Actions_DataAnalysis::Actions_DataAnalysis()
	: CPropertyPage(IDD_DIALOG_DATA_ANALYSIS)
{
	pIntermediateSurface_Openmesh_Morph = NULL;
	pDataAnalysis = this;
}

Actions_DataAnalysis::~Actions_DataAnalysis()
{
}

void Actions_DataAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_DATAANALYSIS_MORPH_SLIDER, m_DataAnalysisMorph_Slider);
}


BEGIN_MESSAGE_MAP(Actions_DataAnalysis, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_LOADFIRST, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadfirst)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_LOADNEXT, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadnext)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_ADDTOSET, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisAddtoset)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_SAVEFILESET, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisSavefileset)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_LOADFILESET, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadfileset)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_LOADSETFIRST, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadsetfirst)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_LOADSETNEXT, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadsetnext)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysis)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_CNN, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisCnn)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_CNN_NORMALIZE, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisCnnNormalize)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_COMPUTEDATA, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisComputedata)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_PRENEUTRAL, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisPreneutral)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_PRENEUTRAL_FourLevel, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisPreneutralFourlevel)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_PRENEUTRAL_FourLevel2, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisPreneutralFourlevel2)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_IMAGE_CNN, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisImageCnn)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_CNN_MATLAB, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisCnnMatlab)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_CENTERED_ATVERTEX, &Actions_DataAnalysis::OnBnClickedButtonZoomCenteredAtvertex)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_READSAVEDMESH, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisReadsavedmesh)
	ON_BN_CLICKED(IDC_BUTTON_SetMeshUV_DATAANALYSIS_Mesh, &Actions_DataAnalysis::OnBnClickedButtonSetmeshuvDataanalysisMesh)
	ON_BN_CLICKED(IDC_BUTTON_SetMeshUV_DATAANALYSIS_TARGET, &Actions_DataAnalysis::OnBnClickedButtonSetmeshuvDataanalysisTarget)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_GENERATEMORPHMESH, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisGeneratemorphmesh)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONICTEST, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonictest)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_PRE, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicPre)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_ITERATIONS, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterations)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_ITERATIONS_EDGESWAP, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsEdgeswap)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_AFTER, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicAfter)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_PRE_CDT, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicPreCdt)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_ITERATIONS_CDT, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsCdt)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_ITERATIONS_EDGESWAP_CDT, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsEdgeswapCdt)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_AFTER_CDT, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicAfterCdt)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_CDT, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicCdt)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_PRE_CDT2, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicPreCdt2)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_ITERATIONS_EDGESWAP_CDT_MOVE, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsEdgeswapCdtMove)
	ON_BN_CLICKED(IDC_BUTTON_DATAANALYSIS_DATAANALYSIS_OPTIMALHARMONIC_CDT_EXAMPLE1, &Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicCdtExample1)
END_MESSAGE_MAP()


// Actions_DataAnalysis message handlers
extern Action_PolyOmino* pPolyOmino;
void getFiles(string path, string exd, vector<string>& files)
{
	//cout << "getFiles()" << path<< endl;   
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//cout << fileinfo.name << endl;   

			//如果是文件夹中仍有文件夹,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(pathName.assign(path).append("\\").append(fileinfo.name), exd, files);
			}
			else
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(pathName.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadfirst()
{
	// TODO: Add your control notification handler code here
//
	filePath = "E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized";

	//获取该路径下的所有文件  
	getFiles(filePath, "m", files);
	m_currentindex = 0;
	pDoc->OnOpenDocument(files[m_currentindex].c_str());
	//pDoc->SetTitle(files[m_currentindex].c_str());
	char message[800];
	sprintf(message, " %d//%d", m_currentindex, files.size());
	std::string str = files[m_currentindex] + message;
	AfxGetMainWnd()->SetWindowText((str.c_str()));
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	pMesh->m_showFace = 0;
	pMesh->m_showEdge = 1;
	pMesh->m_showSharpEdge = 1;
	m_currentindex++;
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadnext()
{
	if (m_currentindex == files.size())
	{
		AfxMessageBox("End!");
	}
	else
	{
		if (pDoc->m_SceneGraph.NbObject() == 1)
		{
			delete pDoc->m_SceneGraph.GetAt(0);
			pDoc->m_SceneGraph.RemoveAt(0);
		}

		pDoc->OnOpenDocument(files[m_currentindex].c_str());
		char message[800];
		sprintf(message, " %d//%d", m_currentindex, files.size());
		std::string str = files[m_currentindex] + message;
		AfxGetMainWnd()->SetWindowText((str.c_str()));
		CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
		pMesh->m_showFace = 0;
		pMesh->m_showEdge = 1;
		pMesh->m_showSharpEdge = 1;
		m_currentindex++;
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisAddtoset()
{
	filesnewset.push_back(files[m_currentindex-1]);

	//
	OnBnClickedButtonDataanalysisLoadnext();
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisSavefileset()
{
	std::ofstream outfile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	for (int i = 0; i < filesnewset.size(); i++)
	{
		CString path = filesnewset[i].c_str();
		path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
		outfile << path << "\n";
	}
	outfile.close();
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadfileset()
{
	// TODO: Add your control notification handler code here
	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}

	filePath = "E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized";

	//获取该路径下的所有文件  
	getFiles(filePath, "m", files);

	for (int i = 0; i < files.size(); i++)
	{
		std::size_t found = files[i].find(str);
		if (found != std::string::npos)
		{
			m_currentindex = i;
			break;
		}
	}
	
	int index;

	std::ifstream infile1("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\edges_ends_index.txt");
	while (!infile1.eof())
	{
		std::string str;
		infile1 >> str;
		std::vector<int> endpoints;
		for (int i = 0; i < 22; i++)
		{
			infile1 >> index;
			endpoints.push_back(index - 1);
		}
		m_ArrayEndPoints[str] = endpoints;
	}

	infile1.close();
	//
	OnBnClickedButtonDataanalysisLoadnext();

	infile.close();
}

extern Action_PolyOmino* pPolyOmino;
void Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadsetfirst()
{
	filePath = "E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized";

	//获取该路径下的所有文件  
	if (filesnewset.size() == 0)
		return;
	m_currentindex = 0;
	if (pDoc->m_SceneGraph.NbObject() == 1)
	{
		delete pDoc->m_SceneGraph.GetAt(0);
		pDoc->m_SceneGraph.RemoveAt(0);
	}

	pDoc->OnOpenDocument(filesnewset[m_currentindex].c_str());
	//pDoc->SetTitle(files[m_currentindex].c_str());
	char message[800];
	sprintf(message, " %d//%d", m_currentindex, files.size());
	std::string str = filesnewset[m_currentindex] + message;
	
	AfxGetMainWnd()->SetWindowText((str.c_str()));
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	pMesh->m_ArraySharpEndPoints = m_ArrayEndPoints[filesnewset[m_currentindex]];

	pMesh->m_showFace = 0;
	pMesh->m_showEdge = 1;
	pMesh->m_showSharpEdge = 1;
	m_currentindex++;
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisLoadsetnext()
{
	if (m_currentindex == filesnewset.size())
	{
		AfxMessageBox("End!");
	}
	else
	{
		if (pDoc->m_SceneGraph.NbObject() == 1)
		{
			delete pDoc->m_SceneGraph.GetAt(0);
			pDoc->m_SceneGraph.RemoveAt(0);
		}

		pDoc->OnOpenDocument(filesnewset[m_currentindex].c_str());
		char message[800];
		sprintf(message, " %d//%d", m_currentindex, files.size());
		std::string str = filesnewset[m_currentindex] + message;
		AfxGetMainWnd()->SetWindowText((str.c_str()));
		CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
		pMesh->m_ArraySharpEndPoints = m_ArrayEndPoints[filesnewset[m_currentindex]];
		pMesh->m_showFace = 0;
		pMesh->m_showEdge = 1;
		pMesh->m_showSharpEdge = 1;
		m_currentindex++;
		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
}

void Actions_DataAnalysis::SVM_MULTIPLECLASSIFY_MATLAB(int typenum, std::vector<std::vector<double>> &train_data,
	std::vector<int> &train_label, std::vector<std::vector<double>> &test_data,std::vector<int> &test_label)
{
	//For each type, compute the svm using matlab
	/*std::vector<int> newtrain_label;
	std::vector<int> test_label1;
	std::vector<>
	test_label.resize()
	for (int i = 0; i < typenum; i++)
	{
		newtrain_label = train_label;
		for (int j = 0; j < newtrain_label.size(); j++)
		{
			if (newtrain_label[j] != i)
				newtrain_label[j] = -1;
		}

		//
		pPolyOmino->SolveSVMClassification_matlab(train_data, newtrain_label, test_data, test_label1);


	}*/


	
}
void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysis()
{
	// TODO: Add your control notification handler code here
	std::vector<std::vector<double>> dataarray;
	std::vector<double> data;
	std::vector<int> labelarray;

	std::ifstream infile4("C:\\matlab701\\work\\AA_Save.txt");
	int index;
	labelarray.clear();
	std::vector<double> rowdata;
	double ddata;
	while (!infile4.eof())
	{
		rowdata.clear();
		for (int i = 0; i < 44; i++)
		{
			infile4 >> ddata;
			if (infile4.eof())
				break;
			rowdata.push_back(ddata);
		}
		if (infile4.eof())
			break;
		dataarray.push_back(rowdata);
	}
	infile4.close();

	std::ifstream infile3("C:\\matlab701\\work\\bb_Save.txt");
	//int index;
	labelarray.clear();
	while (!infile3.eof())
	{
		infile3 >> index;
		labelarray.push_back(index);
	}
	infile3.close();

	std::vector<std::vector<double>> train_data;
	std::vector<int> train_label;

	int traindatanum = dataarray.size() * 0.9;
	for (int i = 0; i < traindatanum; i++)
	{
		train_data.push_back(dataarray[i]);
		train_label.push_back(labelarray[i]);
	}
	std::vector<std::vector<double>> test_data;
	std::vector<int> test_label;
	for (int i = traindatanum; i < dataarray.size(); i++)
	{
		test_data.push_back(dataarray[i]);
		test_label.push_back(labelarray[i]);
	}
	//Predictor svm;

	//svm.train(train_label, train_data);

	//std::vector<int> result = svm.predict(test_data);
	std::vector<int> result;
	pPolyOmino->SolveSVMClassification_matlab(train_data, train_label, test_data, result);

	int correctnum = 0;
	for (int i = 0; i < test_label.size(); i++)
	{
		if (test_label[i] == result[i])
			correctnum++;
	}
	double successratio = correctnum / (double)test_data.size();
}

void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisCnn()
{
	// TODO: Add your control notification handler code here
	std::vector<std::vector<double>> dataarray;
	std::vector<double> data;
	std::vector<int> labelarray;

	std::ifstream infile4("C:\\matlab701\\work\\AA_PreNeutral_Save.txt");
	int index;
	labelarray.clear();
	std::vector<double> rowdata;
	double ddata;
	while (!infile4.eof())
	{
		rowdata.clear();
		for (int i = 0; i < 88; i++)
		{
			infile4 >> ddata;
			if (infile4.eof())
				break;
			rowdata.push_back(ddata);
		}
		if (infile4.eof())
			break;
		dataarray.push_back(rowdata);
	}
	infile4.close();

	std::ifstream infile3("C:\\matlab701\\work\\bb_PreNeutral_Save.txt");
	//int index;
	labelarray.clear();
	while (!infile3.eof())
	{
		infile3 >> index;
		labelarray.push_back(index);
	}
	infile3.close();

	std::vector<std::vector<double>> train_data;
	std::vector<int> train_label;

	int traindatanum = dataarray.size() * 1;
	for (int i = 0; i < traindatanum; i++)
	{
		train_data.push_back(dataarray[i]);
		train_label.push_back(labelarray[i]);
	}
	std::vector<std::vector<double>> test_data;
	std::vector<int> test_label;
	for (int i = traindatanum; i < dataarray.size(); i++)
	{
		test_data.push_back(dataarray[i]);
		test_label.push_back(labelarray[i]);
	}
	//Predictor svm;
	//svm.train(train_label, train_data);
	//std::vector<int> result = svm.predict(test_data);
	std::vector<int> result;
	pPolyOmino->SolveCNNClassification_matlab_SelfTest(train_data, train_label);
	//pPolyOmino->SolveCNNClassification_matlab(train_data, train_label, test_data, result);

	/*int correctnum = 0;
	for (int i = 0; i < test_label.size(); i++)
	{
		if (test_label[i] == result[i])
			correctnum++;
	}
	double successratio = correctnum / (double)test_data.size();*/
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisCnnNormalize()
{
	// TODO: Add your control notification handler code here
	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}
	infile.close();

	std::vector<std::vector<double>> dataarray;
	std::vector<double> data;
	std::vector<int> labelarray;

	std::ifstream infile4("C:\\matlab701\\work\\AA_Save.txt");
	int index;
	labelarray.clear();
	std::vector<double> rowdata;
	double ddata;
	while (!infile4.eof())
	{
		rowdata.clear();
		for (int i = 0; i < 44; i++)
		{
			infile4 >> ddata;
			if (infile4.eof())
				break;
			rowdata.push_back(ddata);
		}
		if (infile4.eof())
			break;
		dataarray.push_back(rowdata);
	}
	infile4.close();

	std::ifstream infile3("C:\\matlab701\\work\\bb_Save.txt");
	//int index;
	labelarray.clear();
	while (!infile3.eof())
	{
		infile3 >> index;
		labelarray.push_back(index);
	}
	infile3.close();

	std::vector<std::vector<double>> dataarray_normalize;
	std::vector<int> labelarray_normalize;

	//int index;
	std::vector<double> m_rowdata;
	for (int i = 0; i < dataarray.size(); i++)
	{
		if (labelarray[i] == 0)
			continue;
		else
		{
			std::string str = filesnewset[i];
			str[6] = 'N';
			str[7] = 'E';
			str[8] = '0';
			str[9] = '0';
			index = std::find(filesnewset.begin(), filesnewset.end(), str) - filesnewset.begin();
			if (index >= 0 && index < filesnewset.size())
			{
				m_rowdata.clear();
				for (int j = 0; j < dataarray[i].size(); j++)
				{
					m_rowdata.push_back(dataarray[i][j] - dataarray[index][j]);
				}
				dataarray_normalize.push_back(m_rowdata);
				labelarray_normalize.push_back(labelarray[i]);
			}
			else
				continue;
		}
	}

	pPolyOmino->SaveMatrix(dataarray_normalize, dataarray_normalize.size(), dataarray_normalize[0].size(), "C:\\matlab701\\work\\AA_Normalize_Save.txt");
	pPolyOmino->SaveVector(labelarray_normalize, labelarray_normalize.size(), "C:\\matlab701\\work\\bb_Normalize_Save.txt");
}

void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisComputedata()
{
	// TODO: Add your control notification handler code here
	// NE: 0, AN: 1, DI: 2, FE: 3, HA: 4, SA: 5, SU: 6
	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}
	infile.close();

	int index;

	std::ifstream infile1("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\edges_ends_index.txt");
	while (!infile1.eof())
	{
		std::string str;
		infile1 >> str;
		std::vector<int> endpoints;
		for (int i = 0; i < 22; i++)
		{
			infile1 >> index;
			endpoints.push_back(index - 1);
		}
		m_ArrayEndPoints[str] = endpoints;
	}

	infile1.close();

	std::vector<std::vector<double>> dataarray;
	std::vector<double> data;
	std::vector<int> labelarray;
	std::vector<int> endpoints;
	int type;
	filePath = "E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\";
	for (int i = 0; i < filesnewset.size(); i++)
	{
		data.clear();
		endpoints = m_ArrayEndPoints[filesnewset[i]];
		char file[800];
		sprintf(file, "%s%s", filePath, filesnewset[i].c_str());
		pDoc->OnOpenDocument(file);
		CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);

		for (int j = 0; j < endpoints.size(); j++)
		{
			data.push_back(pMesh->GetVertex(endpoints[j])->x());
			data.push_back(pMesh->GetVertex(endpoints[j])->y());
		}

		dataarray.push_back(data);
		if (filesnewset[i][6] == 'N')
			type = 0;
		else if (filesnewset[i][6] == 'A')
			type = 1;
		else if (filesnewset[i][6] == 'D')
			type = 2;
		else if (filesnewset[i][6] == 'F')
			type = 3;
		else if (filesnewset[i][6] == 'H')
			type = 4;
		else if (filesnewset[i][6] == 'S' && filesnewset[i][7] == 'A')
			type = 5;
		else if (filesnewset[i][6] == 'S' && filesnewset[i][7] == 'U')
			type = 6;
		labelarray.push_back(type);

		if (pDoc->m_SceneGraph.NbObject() == 1)
		{
			delete pDoc->m_SceneGraph.GetAt(0);
			pDoc->m_SceneGraph.RemoveAt(0);
		}
	}
	//pPolyOmino->SolveSVMClassification_matlab(dataarray, labelarray);
	pPolyOmino->SaveMatrix(dataarray, dataarray.size(), dataarray[0].size(), "C:\\matlab701\\work\\AA_Save.txt");
	pPolyOmino->SaveVector(labelarray, labelarray.size(), "C:\\matlab701\\work\\bb_Save.txt");
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisPreneutral()
{
	// TODO: Add your control notification handler code here
	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}
	infile.close();

	std::vector<std::vector<double>> dataarray;
	std::vector<double> data;
	std::vector<int> labelarray;

	std::ifstream infile4("C:\\matlab701\\work\\AA_Save.txt");
	int index;
	labelarray.clear();
	std::vector<double> rowdata;
	double ddata;
	while (!infile4.eof())
	{
		rowdata.clear();
		for (int i = 0; i < 44; i++)
		{
			infile4 >> ddata;
			if (infile4.eof())
				break;
			rowdata.push_back(ddata);
		}
		if (infile4.eof())
			break;
		dataarray.push_back(rowdata);
	}
	infile4.close();

	std::ifstream infile3("C:\\matlab701\\work\\bb_Save.txt");
	//int index;
	labelarray.clear();
	while (!infile3.eof())
	{
		infile3 >> index;
		labelarray.push_back(index);
	}
	infile3.close();

	std::vector<std::vector<double>> dataarray_normalize;
	std::vector<int> labelarray_normalize;

	//int index;
	std::vector<double> m_rowdata;
	for (int i = 0; i < dataarray.size(); i++)
	{
		m_rowdata.clear();
		if (labelarray[i] == 0)
		{
			for (int j = 0; j < dataarray[i].size(); j++)
			{
				m_rowdata.push_back(dataarray[i][j]);
			}
			for (int j = 0; j < dataarray[i].size(); j++)
			{
				m_rowdata.push_back(dataarray[i][j]);
			}
			dataarray_normalize.push_back(m_rowdata);
			labelarray_normalize.push_back(labelarray[i]);
		}
		else
		{
			std::string str = filesnewset[i];
			str[6] = 'N';
			str[7] = 'E';
			str[8] = '0';
			str[9] = '0';
			index = std::find(filesnewset.begin(), filesnewset.end(), str) - filesnewset.begin();
			if (index >= 0 && index < filesnewset.size())
			{
				for (int j = 0; j < dataarray[i].size(); j++)
				{
					m_rowdata.push_back(dataarray[index][j]);
				}
				for (int j = 0; j < dataarray[i].size(); j++)
				{
					m_rowdata.push_back(dataarray[i][j]);
				}
				dataarray_normalize.push_back(m_rowdata);
				labelarray_normalize.push_back(labelarray[i]);
			}
			else
				continue;
		}
	}

	pPolyOmino->SaveMatrix(dataarray_normalize, dataarray_normalize.size(), dataarray_normalize[0].size(), "C:\\matlab701\\work\\AA_PreNeutral_Save.txt");
	pPolyOmino->SaveVector(labelarray_normalize, labelarray_normalize.size(), "C:\\matlab701\\work\\bb_PreNeutral_Save.txt");
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisPreneutralFourlevel()
{
	// TODO: Add your control notification handler code here
	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}
	infile.close();

	std::vector<std::vector<double>> dataarray;
	std::vector<double> data;
	std::vector<int> labelarray;

	std::ifstream infile4("C:\\matlab701\\work\\AA_Save.txt");
	int index;
	labelarray.clear();
	std::vector<double> rowdata;
	double ddata;
	while (!infile4.eof())
	{
		rowdata.clear();
		for (int i = 0; i < 44; i++)
		{
			infile4 >> ddata;
			if (infile4.eof())
				break;
			rowdata.push_back(ddata);
		}
		if (infile4.eof())
			break;
		dataarray.push_back(rowdata);
	}
	infile4.close();

	std::ifstream infile3("C:\\matlab701\\work\\bb_Save.txt");
	//int index;
	labelarray.clear();
	while (!infile3.eof())
	{
		infile3 >> index;
		labelarray.push_back(index);
	}
	infile3.close();

	std::vector<std::vector<double>> dataarray_normalize;
	std::vector<int> labelarray_normalize;

	//int index;
	std::vector<double> m_rowdata;
	std::vector<std::string> m_NameData;
	for (int i = 0; i < dataarray.size(); i++)
	{
		m_rowdata.clear();
		if (labelarray[i] == 0)
		{
			for (int j = 0; j < dataarray[i].size(); j++)
			{
				m_rowdata.push_back(dataarray[i][j]);
			}
			for (int j = 0; j < dataarray[i].size(); j++)
			{
				m_rowdata.push_back(dataarray[i][j]);
			}
			dataarray_normalize.push_back(m_rowdata);
			labelarray_normalize.push_back(labelarray[i]);
			m_NameData.push_back(filesnewset[i]);
		}
		else
		{
			std::string str = filesnewset[i];
			//if (str[9] != '4' && str[9] != '3')
			if (str[9] != '4')
				continue;

			str[6] = 'N';
			str[7] = 'E';
			str[8] = '0';
			str[9] = '0';
			
			index = std::find(filesnewset.begin(), filesnewset.end(), str) - filesnewset.begin();
			if (index >= 0 && index < filesnewset.size())
			{
				for (int j = 0; j < dataarray[i].size(); j++)
				{
					m_rowdata.push_back(dataarray[index][j]);
				}
				for (int j = 0; j < dataarray[i].size(); j++)
				{
					m_rowdata.push_back(dataarray[i][j]);
				}
				dataarray_normalize.push_back(m_rowdata);
				labelarray_normalize.push_back(labelarray[i]);
				m_NameData.push_back(filesnewset[i]);
			}
			else
				continue;
		}
	}

	pPolyOmino->SaveMatrix(dataarray_normalize, dataarray_normalize.size(), dataarray_normalize[0].size(), "C:\\matlab701\\work\\AA_PreNeutral_Only4_Save.txt");
	pPolyOmino->SaveVector(labelarray_normalize, labelarray_normalize.size(), "C:\\matlab701\\work\\bb_PreNeutral_Only4_Save.txt");

	std::vector<std::vector<double>> traindata;
	std::vector<int> trainlabel;

	std::vector<std::vector<double>> testdata;
	std::vector<int> testlabel,resultlabel;

	for (int i = 0; i < dataarray_normalize.size() * 0.95; i++)
	{
		traindata.push_back(dataarray_normalize[i]);
		trainlabel.push_back(labelarray_normalize[i]);
	}
	std::vector<std::string> testfilenames;
	for (int i = dataarray_normalize.size() * 0.95; i < dataarray_normalize.size(); i++)
	{
		testdata.push_back(dataarray_normalize[i]);
		testlabel.push_back(labelarray_normalize[i]); 
		testfilenames.push_back(m_NameData[i]);
	}

	double max_correct_rate = 0.0;
	TRACE("\n");
	double correct_rate;
	//for (int i = 0; i < 100; i++) 
	{
		pPolyOmino->SolveCNNClassification_matlab(traindata, trainlabel, testdata, resultlabel);
		double correctnum = 0;
		for (int j = 0; j < resultlabel.size(); j++)
			if (resultlabel[j] == testlabel[j])
				correctnum = correctnum + 1.0;
			else
				TRACE("%s\n", testfilenames[j].c_str());

		correct_rate = correctnum / resultlabel.size();
		max_correct_rate = std::max(max_correct_rate, correct_rate);
	}
	
	//pPolyOmino->SolveCNNClassification_matlab(train_data, train_label, test_data, result);

	/*int correctnum = 0;
	for (int i = 0; i < test_label.size(); i++)
	{
	if (test_label[i] == result[i])
	correctnum++;
	}
	double successratio = correctnum / (double)test_data.size();*/
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisPreneutralFourlevel2()
{
	// TODO: Add your control notification handler code here
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisImageCnn()
{
	// TODO: Add your control notification handler code here
	//filePath = "E:\\BU_Data\\images";
	filePath = "E:\\BU_Data\\muimages";
	int len = strlen(filePath) + 1;

	//获取该路径下的所有文件  
	getFiles(filePath, "bmp", files);
	m_currentindex = 0;

	//pDoc->OnOpenDocument(files[m_currentindex].c_str());
	CImage *ResultImage;
	int scalewidth = 512;
	int scaleheight = 512;
	std::vector<std::vector<double>> dataarray;
	std::vector<int> labelarray;

	std::vector<double> imagedata;
	int type;

	for (int i = 0; i < files.size(); i++)
	{
		if (files[i][len + 9] != '3' && files[i][len + 9] != '4')
			continue;

		CImage m_TextureImage;
		m_TextureImage.Load(files[i].c_str());
		int width = m_TextureImage.GetWidth();
		int height = m_TextureImage.GetHeight();
		imagedata.clear();
		ResultImage = new CImage();
		CreateStretchImage(&m_TextureImage, ResultImage, scalewidth, scaleheight);

		for (int ii = 0; ii < scalewidth; ii++)
			for (int jj = 0; jj < scaleheight; jj++)
			{
				COLORREF color = ResultImage->GetPixel(ii, jj);
				imagedata.push_back(GetRValue(color) / 256.0);
				imagedata.push_back(GetGValue(color) / 256.0);
				imagedata.push_back(GetBValue(color) / 256.0);
			}
		dataarray.push_back(imagedata);

		//if (files[i][6 + len] == 'N')
		//	type = 0;
		//else 
			if (files[i][6 + len] == 'A')
			type = 1;
		else if (files[i][6 + len] == 'D')
			type = 2;
		else if (files[i][6 + len] == 'F')
			type = 3;
		else if (files[i][6 + len] == 'H')
			type = 4;
		else if (files[i][6 + len] == 'S' && files[i][7 + len] == 'A')
			type = 5;
		else if (files[i][6 + len] == 'S' && files[i][7 + len] == 'U')
			type = 6;
		labelarray.push_back(type);
		m_TextureImage.Destroy();
		ResultImage->Destroy();
		delete ResultImage;
	}

	pPolyOmino->SaveMatrix(dataarray, dataarray.size(), dataarray[0].size(), "C:\\matlab701\\work\\AA_Image_Save1.txt");
	pPolyOmino->SaveVector(labelarray, labelarray.size(), "C:\\matlab701\\work\\bb_Image_Save1.txt");

	std::vector<std::vector<double>> traindata;
	std::vector<int> trainlabel;

	std::vector<std::vector<double>> testdata;
	std::vector<int> testlabel, resultlabel;

	for (int i = 0; i < dataarray.size() * 0.95; i++)
	//for (int i = 0; i < dataarray.size() * 1; i++)
	{
		traindata.push_back(dataarray[i]);
		trainlabel.push_back(labelarray[i]);
	}
	std::vector<std::string> testfilenames;
	for (int i = dataarray.size() * 0.95; i < dataarray.size(); i++)
	{
		testdata.push_back(dataarray[i]);
		testlabel.push_back(labelarray[i]);
		//testfilenames.push_back(m_NameData[i]);
	}

	double max_correct_rate = 0.0;
	TRACE("\n");
	double correct_rate;
	//for (int i = 0; i < 100; i++) 
	{
		pPolyOmino->SolveCNNClassification_matlab(traindata, trainlabel, testdata, resultlabel);
		double correctnum = 0;
		for (int j = 0; j < resultlabel.size(); j++)
			if (resultlabel[j] == testlabel[j])
				correctnum = correctnum + 1.0;
			//else
				//TRACE("%s\n", testfilenames[j].c_str());

		correct_rate = correctnum / resultlabel.size();
		max_correct_rate = std::max(max_correct_rate, correct_rate);
	}
}
void Actions_DataAnalysis::CreateStretchImage(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth)
{
	if (pImage->IsDIBSection())
	{
		// 取得 pImage 的 DC
		CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC()); 
		// Image 因為有自己的 DC, 所以必須使用 FromHandle 取得對應的 DC

		CBitmap *bitmap1 = pImageDC1->GetCurrentBitmap();
		BITMAP bmpInfo;
		bitmap1->GetBitmap(&bmpInfo);


		// 建立新的 CImage
		ResultImage->Create(StretchWidth, StretchHeight, bmpInfo.bmBitsPixel);
		CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());


		// 當 Destination 比較小的時候, 會根據 Destination DC 上的 Stretch Blt mode 決定是否要保留被刪除點的資訊
		ResultImageDC->SetStretchBltMode(HALFTONE); // 使用最高品質的方式
		::SetBrushOrgEx(ResultImageDC->m_hDC, 0, 0, NULL); // 調整 Brush 的起點


														   // 把 pImage 畫到 ResultImage 上面
		StretchBlt(*ResultImageDC, 0, 0, StretchWidth, StretchHeight, *pImageDC1, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);
		// pImage->Draw(*ResultImageDC,0,0,StretchWidth,StretchHeight,0,0,pImage->GetWidth(),pImage->GetHeight());


		pImage->ReleaseDC();
		ResultImage->ReleaseDC();
	}
}




void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisCnnMatlab()
{
	// TODO: Add your control notification handler code here
	pPolyOmino->SolveCNN_matlab();
}

extern CFormView3 *p_FormView3;
extern Actions_HarmonicMapping* pHarmonicMapping;
void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic()
{
	//GenerateQuasiConformalMuImage("", "", "D:\\1.bmp");
	//return;

	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}
	infile.close();

	std::string filename;
	std::string path("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\");
	std::string imagepath("E:\\BU_Data\\muimages\\");
	for (int i = 0; i < filesnewset.size(); i++)
	{
		//
		//if (i >= 300)
		//	return;

		filename = filesnewset[i];

		if (filename[6] != 'N')
		{
			filename[6] = 'N';
			filename[7] = 'E';
			filename[8] = '0';
			filename[9] = '0';

			int index = std::find(filesnewset.begin(), filesnewset.end(), filename) - filesnewset.begin();
			if (index >= 0 && index < filesnewset.size())
			{
				std::string filename1 = path + filesnewset[i];
				std::string filename2 = path + filesnewset[index];
				std::string muimagename = filesnewset[i];
				muimagename.replace(muimagename.size() - 2, 2, ".bmp");
				muimagename = imagepath + muimagename;
				CString file = muimagename.c_str();
				BOOL flag = PathFileExists(file);

				//if the file exist,then skip
				if(!flag)
					GenerateQuasiConformalMuImage(filename2.c_str(), filename1.c_str(), muimagename.c_str());
			}
		}
	}
	//pDoc->UpdateAllViews(NULL);
}
void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic_Debug_CDT()
{
	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}
	infile.close();

	std::string filename;
	std::string path("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\");
	std::string imagepath("E:\\BU_Data\\muimages\\");
	for (int i = 0; i < filesnewset.size(); i++)
	{
		//
		//if (i >= 300)
		//	return;

		filename = filesnewset[i];

		if (filename[6] != 'N')
		{
			filename[6] = 'N';
			filename[7] = 'E';
			filename[8] = '0';
			filename[9] = '0';

			int index = std::find(filesnewset.begin(), filesnewset.end(), filename) - filesnewset.begin();
			if (index >= 0 && index < filesnewset.size())
			{
				std::string filename1 = path + filesnewset[i];
				std::string filename2 = path + filesnewset[index];
				std::string muimagename = filesnewset[i];
				muimagename.replace(muimagename.size() - 2, 2, ".bmp");
				muimagename = imagepath + muimagename;
				CString file = muimagename.c_str();
				BOOL flag = PathFileExists(file);

				//if the file exist,then skip
				if (!flag)
				{
					GenerateQuasiConformalMuImage_Debug_Pre(filename2.c_str(), filename1.c_str(), muimagename.c_str());
					return;
				}

			}
		}
	}
}
void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic_Debug()
{
	//GenerateQuasiConformalMuImage("", "", "D:\\1.bmp");
	//return;

	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}
	infile.close();

	std::string filename;
	std::string path("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\");
	std::string imagepath("E:\\BU_Data\\muimages\\");
	for (int i = 0; i < filesnewset.size(); i++)
	{
		//
		//if (i >= 300)
		//	return;

		filename = filesnewset[i];

		if (filename[6] != 'N')
		{
			filename[6] = 'N';
			filename[7] = 'E';
			filename[8] = '0';
			filename[9] = '0';

			int index = std::find(filesnewset.begin(), filesnewset.end(), filename) - filesnewset.begin();
			if (index >= 0 && index < filesnewset.size())
			{
				std::string filename1 = path + filesnewset[i];
				std::string filename2 = path + filesnewset[index];
				std::string muimagename = filesnewset[i];
				muimagename.replace(muimagename.size() - 2, 2, ".bmp");
				muimagename = imagepath + muimagename;
				CString file = muimagename.c_str();
				BOOL flag = PathFileExists(file);

				//if the file exist,then skip
				if (!flag)
				{
					GenerateQuasiConformalMuImage_Debug_Pre(filename2.c_str(), filename1.c_str(), muimagename.c_str());
					return;
				}

			}
		}
	}
	//pDoc->UpdateAllViews(NULL);
}
void Actions_DataAnalysis::GenerateQuasiConformalMuImage_CDT_PRE(const char* sourcemesh, const char* targetmesh, const char* muimagefilename)
{
	pDoc->OnOpenDocument(sourcemesh);
	pSoureceSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pSoureceSurface_Openmesh = pSoureceSurface_Mesh3d->Convert_OpenMesh();
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->GenerateLandMarks_KeepSharpEdgesConsistent();
	pSoureceSurface_Openmesh->m_LandMarkArray = pSoureceSurface_Mesh3d->m_LandMarkArray;
	CString path = targetmesh;
	path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	pSoureceSurface_Openmesh->filename = path;

	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_AN03WH_F3D_par_norm.m");
	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_AN04WH_F3D_par_norm.m");
	pDoc->OnOpenDocument(targetmesh);
	pTargetSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pTargetSurface_Openmesh = pTargetSurface_Mesh3d->Convert_OpenMesh();
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->GenerateLandMarks_KeepSharpEdgesConsistent();
	pTargetSurface_Openmesh->m_LandMarkArray = pTargetSurface_Mesh3d->m_LandMarkArray;
	//pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);

	//pSoureceSurface_Mesh3d->pReferenceMesh = pTargetSurface_Mesh3d;
	//Add the selected vertices to the fix array
	AddLandmarkEndPointsToFixArray(pSoureceSurface_Openmesh, pTargetSurface_Openmesh);
}
void Actions_DataAnalysis::GenerateQuasiConformalMuImage_CDT(const char* sourcemesh, const char* targetmesh, const char* muimagefilename)
{
	pDoc->OnOpenDocument(sourcemesh);
	pSoureceSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pSoureceSurface_Openmesh = pSoureceSurface_Mesh3d->Convert_OpenMesh();
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->GenerateLandMarks_KeepSharpEdgesConsistent();
	pSoureceSurface_Openmesh->m_LandMarkArray = pSoureceSurface_Mesh3d->m_LandMarkArray;
	CString path = targetmesh;
	path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	pSoureceSurface_Openmesh->filename = path;

	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_AN03WH_F3D_par_norm.m");
	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_AN04WH_F3D_par_norm.m");
	pDoc->OnOpenDocument(targetmesh);
	pTargetSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pTargetSurface_Openmesh = pTargetSurface_Mesh3d->Convert_OpenMesh();
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->GenerateLandMarks_KeepSharpEdgesConsistent();
	pTargetSurface_Openmesh->m_LandMarkArray = pTargetSurface_Mesh3d->m_LandMarkArray;
	//pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);

	//pSoureceSurface_Mesh3d->pReferenceMesh = pTargetSurface_Mesh3d;
	//Add the selected vertices to the fix array
	AddLandmarkEndPointsToFixArray(pSoureceSurface_Openmesh, pTargetSurface_Openmesh);

	//first Make the original mesh to be a CDT
	pDynamicQuasiconformal->GetCDTMesh_Simplify(pSoureceSurface_Openmesh);

	//harmonic map iterations
	int flag = HarmonicIterationsbyEdgeSwap_CDT(pSoureceSurface_Openmesh);

	if (flag)
	{
		//
		pHarmonicMapping->SetZeroMu(pSoureceSurface_Openmesh);

		//
		pHarmonicMapping->Compute_Mu(pSoureceSurface_Openmesh);

		//CFramebuffer* buffer = new CFramebuffer();
		//buffer->begin();
		//PlotMeshMu(pSoureceSurface_Openmesh);
		//buffer->saveFrameBuff(muimagefilename);
		//buffer->end();
		//delete buffer;
	}

	//
	pDoc->m_SceneGraph.Free();
}
void Actions_DataAnalysis::GenerateQuasiConformalMuImage(const char* sourcemesh, const char* targetmesh, const char* muimagefilename)
{
	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_NE00WH_F3D_par_norm.m");
	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_NE00WH_F3D_par_norm.m");
	pDoc->OnOpenDocument(sourcemesh);
	pSoureceSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pSoureceSurface_Openmesh = pSoureceSurface_Mesh3d->Convert_OpenMesh();
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->GenerateLandMarks_KeepSharpEdgesConsistent();
	pSoureceSurface_Openmesh->m_LandMarkArray = pSoureceSurface_Mesh3d->m_LandMarkArray;
	CString path = targetmesh;
	path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	pSoureceSurface_Openmesh->filename = path;

	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_AN03WH_F3D_par_norm.m");
	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_AN04WH_F3D_par_norm.m");
	pDoc->OnOpenDocument(targetmesh);
	pTargetSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pTargetSurface_Openmesh = pTargetSurface_Mesh3d->Convert_OpenMesh();
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->GenerateLandMarks_KeepSharpEdgesConsistent();
	pTargetSurface_Openmesh->m_LandMarkArray = pTargetSurface_Mesh3d->m_LandMarkArray;
	//pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);

	//pSoureceSurface_Mesh3d->pReferenceMesh = pTargetSurface_Mesh3d;
	//Add the selected vertices to the fix array
	AddLandmarkEndPointsToFixArray(pSoureceSurface_Openmesh, pTargetSurface_Openmesh);

	//harmonic map iterations
	int flag = HarmonicIterationsbyEdgeSwap(pSoureceSurface_Openmesh);

	if (flag)
	{
		//
		pHarmonicMapping->SetZeroMu(pSoureceSurface_Openmesh);

		//
		pHarmonicMapping->Compute_Mu(pSoureceSurface_Openmesh);

		//CFramebuffer* buffer = new CFramebuffer();
		//buffer->begin();
		//PlotMeshMu(pSoureceSurface_Openmesh);
		//buffer->saveFrameBuff(muimagefilename);
		//buffer->end();
		//delete buffer;
	}

	//
	pDoc->m_SceneGraph.Free();
}
void Actions_DataAnalysis::GenerateQuasiConformalMuImage_Debug_Pre(const char* sourcemesh, const char* targetmesh, const char* muimagefilename)
{
	//For Debug
	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_NE00WH_F3D_par_norm.m");
	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_NE00WH_F3D_par_norm.m");
	pDoc->OnOpenDocument(sourcemesh);
	pSoureceSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pSoureceSurface_Openmesh = pSoureceSurface_Mesh3d->Convert_OpenMesh();
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->GenerateLandMarks_KeepSharpEdgesConsistent();
	pSoureceSurface_Openmesh->m_LandMarkArray = pSoureceSurface_Mesh3d->m_LandMarkArray;
	CString path = targetmesh;
	path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	pSoureceSurface_Openmesh->filename = path;

	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_AN03WH_F3D_par_norm.m");
	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_AN04WH_F3D_par_norm.m");
	pDoc->OnOpenDocument(targetmesh);
	pTargetSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pTargetSurface_Openmesh = pTargetSurface_Mesh3d->Convert_OpenMesh();
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	pPolyOmino->GenerateLandMarks_KeepSharpEdgesConsistent();
	pTargetSurface_Openmesh->m_LandMarkArray = pTargetSurface_Mesh3d->m_LandMarkArray;
	//pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);

	//pSoureceSurface_Mesh3d->pReferenceMesh = pTargetSurface_Mesh3d;
	//Add the selected vertices to the fix array
	AddLandmarkEndPointsToFixArray(pSoureceSurface_Openmesh, pTargetSurface_Openmesh);

	iternum_global = 0;
}
void Actions_DataAnalysis::GenerateQuasiConformalMuImage_Debug_Iterations()
{
	//add constraints
	AddLandmarkEndPointsToFixArray(pSoureceSurface_Openmesh, pTargetSurface_Openmesh);

	int flag = HarmonicIterationsbyEdgeSwap_Debug(pSoureceSurface_Openmesh);
}
void Actions_DataAnalysis::GenerateQuasiConformalMuImage_Debug_After(const char* sourcemesh, const char* targetmesh, const char* muimagefilename)
{
	//harmonic map iterations
	//int flag = HarmonicIterationsbyEdgeSwap_Debug(pSoureceSurface_Openmesh);
	int flag = 1;
	if (flag)
	{
		//
		AddResultantMesh(pSoureceSurface_Openmesh);

		//Save
		SaveResultantMesh(pSoureceSurface_Openmesh);
	}

	if (flag)
	{
		//
		pHarmonicMapping->SetZeroMu(pSoureceSurface_Openmesh);

		//
		pHarmonicMapping->Compute_Mu(pSoureceSurface_Openmesh);

		//CFramebuffer* buffer = new CFramebuffer();
		//buffer->begin();
		//PlotMeshMu(pSoureceSurface_Openmesh);
		//buffer->saveFrameBuff(muimagefilename);
		//buffer->end();
		//delete buffer;
	}

	//
	pDoc->m_SceneGraph.Free();
}
extern CMyTreeView* pTreeView;

int Actions_DataAnalysis::HarmonicIterationsbyEdgeSwap_CDT_DEBUG(CMesh3d_OpenMesh* pSourceMesh)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	iternum_global = 0;

	//Vertex UV
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSourceMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//while (1)
	{
		pHarmonicMapping->Compute_Linear_Equations_WithStraighteningConstraints_FixEndPoints(pSourceMesh);

		//CMesh3d_OpenMesh* ptarget_para_Openmesh_Intermediate = pCurrentMesh->copy();
		//if (!pHarmonicMapping->IsMeshUVFlip(pSourceMesh, 0))
		//{
		//	AddResultantMesh(pSourceMesh);

			//Save
		//	SaveResultantMesh(pSourceMesh);
		//	break;
		//}
		//Search the step
		//double step;
		//int edgeindex;
		step_global = pDynamicQuasiconformal->SearchSmallestStep(pSourceMesh, edgeindex_global);
		//pCurrentMesh = ptarget_para_Openmesh_Intermediate;
		iternum_global++;
	}
	return 1;
}
void Actions_DataAnalysis::MeshCopyPosition(CMesh3d_OpenMesh* pSourceMesh, CMesh3d_OpenMesh* pTargetMesh)
{

}
void Actions_DataAnalysis::CombineAllMappingsByAddingSteinerPoints(CMesh3d_OpenMesh* pSourceMesh)
{
	//First Copy Position
	//MeshCopyPosition(pSourceMesh, pSourceMesh_OrigialTopology);

	for (int i = 0; i < pDynamicQuasiconformal->m_ArrayMeshMappingSequences.size(); i++)
	{
		if (pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->m_IsEdgeSwap &&
			pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->m_IsAddSteinerPoint)
		{
			//Add Steiner points
			int vh1 = pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->m_SwapEdgeEndPonits.first;
			int vh2 = pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->m_SwapEdgeEndPonits.second;

			int edgeindex = pSourceMesh->GetEdgeHandleIdx(vh1, vh2);

			MyMesh::EdgeHandle eh = pSourceMesh->pmesh->edge_handle(edgeindex);

			MyMesh::HalfedgeHandle hh0 = pSourceMesh->pmesh->halfedge_handle(eh, 0);
			MyMesh::HalfedgeHandle hh1 = pSourceMesh->pmesh->halfedge_handle(eh, 1);

			double t = pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->step;

			MyMesh::VertexHandle vh1_handle = pSourceMesh->pmesh->vertex_handle(vh1);
			MyMesh::VertexHandle vh2_handle = pSourceMesh->pmesh->vertex_handle(vh2);

			OpenMesh::Vec3d v1 = pSourceMesh->pmesh->point(vh1_handle);
			OpenMesh::Vec3d v2 = pSourceMesh->pmesh->point(vh2_handle);

			OpenMesh::Vec3d newpoint;
			newpoint[0] = v1[0] * (1 - t) + v2[0] * t;
			newpoint[1] = v1[1] * (1 - t) + v2[1] * t;
			newpoint[2] = v1[2] * (1 - t) + v2[2] * t;

			MyMesh::FaceHandle fh0 = pSourceMesh->pmesh->face_handle(hh0);
			MyMesh::FaceHandle fh1 = pSourceMesh->pmesh->face_handle(hh1);

			MyMesh::HalfedgeHandle fh_0 = pSourceMesh->pmesh->next_halfedge_handle(hh0);
			MyMesh::HalfedgeHandle fh_1 = pSourceMesh->pmesh->prev_halfedge_handle(hh0);
			MyMesh::HalfedgeHandle fh_2 = pSourceMesh->pmesh->next_halfedge_handle(hh1);
			MyMesh::HalfedgeHandle fh_3 = pSourceMesh->pmesh->prev_halfedge_handle(hh1);

			//first delete the two faces
			pSourceMesh->pmesh->delete_face(fh0, FALSE);
			pSourceMesh->pmesh->delete_face(fh1, FALSE);

			//Add One Point
			MyMesh::VertexHandle newpointhandle = pSourceMesh->pmesh->add_vertex(newpoint);

			//Add four faces
			std::vector<MyMesh::VertexHandle> m_ArrayPoints;
			//
			m_ArrayPoints.clear();
			m_ArrayPoints.push_back(pSourceMesh->pmesh->from_vertex_handle(fh_0));
			m_ArrayPoints.push_back(pSourceMesh->pmesh->to_vertex_handle(fh_0));
			m_ArrayPoints.push_back(newpointhandle);
			pSourceMesh->pmesh->add_face(m_ArrayPoints);

			m_ArrayPoints.clear();
			m_ArrayPoints.push_back(pSourceMesh->pmesh->from_vertex_handle(fh_1));
			m_ArrayPoints.push_back(pSourceMesh->pmesh->to_vertex_handle(fh_1));
			m_ArrayPoints.push_back(newpointhandle);
			pSourceMesh->pmesh->add_face(m_ArrayPoints);

			m_ArrayPoints.clear();
			m_ArrayPoints.push_back(pSourceMesh->pmesh->from_vertex_handle(fh_2));
			m_ArrayPoints.push_back(pSourceMesh->pmesh->to_vertex_handle(fh_2));
			m_ArrayPoints.push_back(newpointhandle);
			pSourceMesh->pmesh->add_face(m_ArrayPoints);

			m_ArrayPoints.clear();
			m_ArrayPoints.push_back(pSourceMesh->pmesh->from_vertex_handle(fh_3));
			m_ArrayPoints.push_back(pSourceMesh->pmesh->to_vertex_handle(fh_3));
			m_ArrayPoints.push_back(newpointhandle);
			pSourceMesh->pmesh->add_face(m_ArrayPoints);

			pSourceMesh->pmesh->garbage_collection();
		}
	}
}
int Actions_DataAnalysis::SetMeshPositionandUV(CMesh3d_OpenMesh* pSourceMesh_New, CMesh3d_OpenMesh* pSourceMesh)
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSourceMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Assert the two meshes have the same topology
	int n1 = pSourceMesh_New->pmesh->n_vertices();
	int n2 = pSourceMesh->pmesh->n_vertices();

	int e1 = pSourceMesh_New->pmesh->n_edges();
	int e2 = pSourceMesh->pmesh->n_edges();

	int f1 = pSourceMesh_New->pmesh->n_faces();
	int f2 = pSourceMesh->pmesh->n_faces();

	ASSERT(n1 == n2);
	ASSERT(e1 == e2);
	ASSERT(f1 == f2);

	for (MyMesh::EdgeIter e_it = pSourceMesh->pmesh->edges_begin(); e_it != pSourceMesh->pmesh->edges_end(); ++e_it)
	{
		MyMesh::EdgeHandle eh = e_it.handle();

		MyMesh::HalfedgeHandle hh0 = pSourceMesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pSourceMesh->pmesh->halfedge_handle(eh, 1);

		int vh1 = pSourceMesh->pmesh->to_vertex_handle(hh0).idx();
		int vh2 = pSourceMesh->pmesh->from_vertex_handle(hh0).idx();

		int eindex = pSourceMesh_New->GetEdgeHandleIdx(vh1, vh2);
		ASSERT(eindex >= 0);
	}
	std::vector<double> uvvalues;
	uvvalues.resize(2);
	for (MyMesh::VertexIter v_it = pSourceMesh->pmesh->vertices_begin(); v_it != pSourceMesh->pmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d v1 = pSourceMesh->pmesh->point(v_it.handle());
		OpenMesh::Vec3d v2 = pSourceMesh_New->pmesh->point(v_it.handle());

		uvvalues[0] = v1[0];
		uvvalues[1] = v1[1];
		pSourceMesh->pmesh->set_point(v_it.handle(), v2);
		pSourceMesh->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
	}
	return 1;
}
int Actions_DataAnalysis::HarmonicIterationsbyEdgeSwap_CDT(CMesh3d_OpenMesh* pSourceMesh)
{
	VPROP_VERTEX_UV vprop_vertex_uv;
	iternum_global = 0;

	//Vertex UV
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSourceMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Generate a new copy
	CMesh3d_OpenMesh* pSourceMesh_New = pSourceMesh->copy();

	while (1)
	{
		pHarmonicMapping->Compute_Linear_Equations_WithStraighteningConstraints_FixEndPoints(pSourceMesh);

		//CMesh3d_OpenMesh* ptarget_para_Openmesh_Intermediate = pCurrentMesh->copy();
		if (!pHarmonicMapping->IsMeshUVFlip(pSourceMesh, 0))
		{
			//
			pSourceMesh->AssignUVtoMeshPointLocation();
			
			//
			CombineAllMappingsByAddingSteinerPoints(pSourceMesh);

			//
			AddResultMesh_CDT(pSourceMesh_New, pSourceMesh);

			//
			SetMeshPositionandUV(pSourceMesh_New, pSourceMesh);

			delete pSourceMesh_New;
			//AddResultantMesh(pSourceMesh);

			//Save
			//SaveResultantMesh(pSourceMesh);
			break;
		}
		//Search the step
		double step;
		int edgeindex;
		step = pDynamicQuasiconformal->SearchSmallestStep_CDT(pSourceMesh, edgeindex);

		if (step > TOLER)
		{
			std::vector<double> uvvalues;
			uvvalues.resize(2);
			OpenMesh::Vec3d targetpoint, initialpoint;
			for (MyMesh::VertexIter v_it = pSourceMesh->pmesh->vertices_begin(); v_it != pSourceMesh->pmesh->vertices_end(); ++v_it)
			{
				initialpoint = pSourceMesh->pmesh->point(v_it.handle());
				uvvalues = pSourceMesh->pmesh->property(vprop_vertex_uv, v_it.handle());
				targetpoint[0] = initialpoint[0] * (1 - step) + uvvalues[0] * step;
				targetpoint[1] = initialpoint[1] * (1 - step) + uvvalues[1] * step;
				targetpoint[2] = 0;
				pSourceMesh->pmesh->set_point(v_it.handle(), targetpoint);
			}
		}


		/*for (MyMesh::VertexIter v_it = ptarget_para_Openmesh_Intermediate->pmesh->vertices_begin(); v_it != ptarget_para_Openmesh_Intermediate->pmesh->vertices_end(); ++v_it)
		{
		initialpoint = ptarget_para_Openmesh_Intermediate->pmesh->point(v_it.handle());

		uvvalues[0] = initialpoint[0];
		uvvalues[1] = initialpoint[1];
		pCurrentMesh->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
		}*/
		//
		AddMeshMappingtoSequence(pSourceMesh_New, pSourceMesh, edgeindex, step);

		//Change Topology of the original Mesh
		//ModifyMeshTopology(pSourceMesh_OrigialTopology,edgeindex)

		//edgeswap
		pHarmonicMapping->SwapAdjacentTriangles(pSourceMesh, edgeindex);
		pSourceMesh->pmesh->garbage_collection();

		//pCurrentMesh = ptarget_para_Openmesh_Intermediate;
		iternum_global++;
	}
	return 1;
}
void Actions_DataAnalysis::AddMeshMappingtoSequence(CMesh3d_OpenMesh* pSourceMesh_New, CMesh3d_OpenMesh* pSourceMesh, int edgeindex, double step)
{
	MeshMapping* pMeshMapping;

	//Add to the mesh mapping array
	if (step > TOLER)
	{
		pMeshMapping = new MeshMapping();
		//pMeshMapping->pStartMesh = pCurrentMesh;
		pMeshMapping->m_IsEdgeSwap = 0;
		pMeshMapping->step = step;
		pDynamicQuasiconformal->m_ArrayMeshMappingSequences.push_back(pMeshMapping);
	}

	pMeshMapping = new MeshMapping();
	//pMeshMapping->pStartMesh = pCurrentMesh;
	pMeshMapping->m_IsEdgeSwap = 1;

	MyMesh::EdgeHandle eh = pSourceMesh->pmesh->edge_handle(edgeindex);

	MyMesh::HalfedgeHandle hh0 = pSourceMesh->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pSourceMesh->pmesh->halfedge_handle(eh, 1);

	int vh1 = pSourceMesh->pmesh->to_vertex_handle(hh0).idx();
	int vh2 = pSourceMesh->pmesh->from_vertex_handle(hh0).idx();

	std::vector<int> oppsitevertices;
	pHarmonicMapping->GetTwoOppositePoints(pSourceMesh, edgeindex, oppsitevertices);

	pMeshMapping->m_SwapEdgeEndPonits.first = oppsitevertices[0];
	pMeshMapping->m_SwapEdgeEndPonits.second = oppsitevertices[1];

	//Make sure that one diagonal edges will not exchange for more than one times.
	if (SearchMeshMappingSequencesForSpecificDiagonalSwap(vh1, vh2) > -1)
		ASSERT(0);

	pMeshMapping->step = ComputeIntersectionPointonAnotherDiagonalRatio(pSourceMesh, edgeindex);

	pDynamicQuasiconformal->m_ArrayMeshMappingSequences.push_back(pMeshMapping);

	//Change the topology of the original mesh.
	int edgeindex_original = pSourceMesh_New->GetEdgeHandleIdx(vh1, vh2);
	int isValid = pHarmonicMapping->IsValidSwapAdjacentTriangles(pSourceMesh_New, edgeindex_original);
	if (isValid)
	{
		pHarmonicMapping->SwapAdjacentTriangles(pSourceMesh_New, edgeindex_original);
		pMeshMapping->m_IsAddSteinerPoint = 0;
		pSourceMesh_New->pmesh->garbage_collection();
	}
	else
	{
		//std::vector<int> oppsitevertices;
		//pHarmonicMapping->GetTwoOppositePoints(pSourceMesh_New, edgeindex_original, oppsitevertices);
		//ASSERT(oppsitevertices.size() == 2);

		//int vh3 = oppsitevertices[0];
		//int vh4 = oppsitevertices[1];

		//Add Steiner Points
		double t = ComputeIntersectionPointonGivenDiagonalRatio(pSourceMesh, edgeindex);

		MyMesh::VertexHandle vh1_handle = pSourceMesh_New->pmesh->vertex_handle(vh1);
		MyMesh::VertexHandle vh2_handle = pSourceMesh_New->pmesh->vertex_handle(vh2);
		//MyMesh::VertexHandle vh3_handle = pSourceMesh_New->pmesh->vertex_handle(vh3);
		//MyMesh::VertexHandle vh4_handle = pSourceMesh_New->pmesh->vertex_handle(vh4);

		OpenMesh::Vec3d v1 = pSourceMesh_New->pmesh->point(vh1_handle);
		OpenMesh::Vec3d v2 = pSourceMesh_New->pmesh->point(vh2_handle);

		OpenMesh::Vec3d newpoint;
		newpoint[0] = v1[0] * (1 - t) + v2[0] * t;
		newpoint[1] = v1[1] * (1 - t) + v2[1] * t;
		newpoint[2] = v1[2] * (1 - t) + v2[2] * t;

		MyMesh::FaceHandle fh0 = pSourceMesh_New->pmesh->face_handle(hh0);
		MyMesh::FaceHandle fh1 = pSourceMesh_New->pmesh->face_handle(hh1);

		MyMesh::HalfedgeHandle fh_0 = pSourceMesh_New->pmesh->next_halfedge_handle(hh0);
		MyMesh::HalfedgeHandle fh_1 = pSourceMesh_New->pmesh->prev_halfedge_handle(hh0);
		MyMesh::HalfedgeHandle fh_2 = pSourceMesh_New->pmesh->next_halfedge_handle(hh1);
		MyMesh::HalfedgeHandle fh_3 = pSourceMesh_New->pmesh->prev_halfedge_handle(hh1);

		//first delete the two faces
		pSourceMesh_New->pmesh->delete_face(fh0);
		pSourceMesh_New->pmesh->delete_face(fh1);

		//Add One Point
		MyMesh::VertexHandle newpointhandle = pSourceMesh_New->pmesh->add_vertex(newpoint);

		//Add four faces
		std::vector<MyMesh::VertexHandle> m_ArrayPoints;
		//
		m_ArrayPoints.clear();
		m_ArrayPoints.push_back(pSourceMesh_New->pmesh->from_vertex_handle(fh_0));
		m_ArrayPoints.push_back(pSourceMesh_New->pmesh->to_vertex_handle(fh_0));
		m_ArrayPoints.push_back(newpointhandle);
		pSourceMesh_New->pmesh->add_face(m_ArrayPoints);

		m_ArrayPoints.clear();
		m_ArrayPoints.push_back(pSourceMesh_New->pmesh->from_vertex_handle(fh_1));
		m_ArrayPoints.push_back(pSourceMesh_New->pmesh->to_vertex_handle(fh_1));
		m_ArrayPoints.push_back(newpointhandle);
		pSourceMesh_New->pmesh->add_face(m_ArrayPoints);

		m_ArrayPoints.clear();
		m_ArrayPoints.push_back(pSourceMesh_New->pmesh->from_vertex_handle(fh_2));
		m_ArrayPoints.push_back(pSourceMesh_New->pmesh->to_vertex_handle(fh_2));
		m_ArrayPoints.push_back(newpointhandle);
		pSourceMesh_New->pmesh->add_face(m_ArrayPoints);

		m_ArrayPoints.clear();
		m_ArrayPoints.push_back(pSourceMesh_New->pmesh->from_vertex_handle(fh_3));
		m_ArrayPoints.push_back(pSourceMesh_New->pmesh->to_vertex_handle(fh_3));
		m_ArrayPoints.push_back(newpointhandle);
		pSourceMesh_New->pmesh->add_face(m_ArrayPoints);

		pMeshMapping->m_IsAddSteinerPoint = 1;
		pSourceMesh_New->pmesh->garbage_collection();
	}
}
int Actions_DataAnalysis::SearchMeshMappingSequencesForSpecificDiagonalSwap(int vh1,int vh2)
{
	for (int i = 0; i < pDynamicQuasiconformal->m_ArrayMeshMappingSequences.size(); i++)
	{
		if (pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->m_IsEdgeSwap)
		{
			if (pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->m_SwapEdgeEndPonits.first == vh1 &&
				pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->m_SwapEdgeEndPonits.second == vh2 ||
				pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->m_SwapEdgeEndPonits.first == vh2 &&
				pDynamicQuasiconformal->m_ArrayMeshMappingSequences[i]->m_SwapEdgeEndPonits.second == vh1)
				return i;
		}
	}
	return -1;
}
double Actions_DataAnalysis::ComputeIntersectionPointonGivenDiagonalRatio(CMesh3d_OpenMesh* pSourceMesh, int edgeindex)
{
	MyMesh::EdgeHandle eh = pSourceMesh->pmesh->edge_handle(edgeindex);

	MyMesh::HalfedgeHandle hh0 = pSourceMesh->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pSourceMesh->pmesh->halfedge_handle(eh, 1);

	MyMesh::VertexHandle vh1 = pSourceMesh->pmesh->to_vertex_handle(hh0);
	MyMesh::VertexHandle vh2 = pSourceMesh->pmesh->from_vertex_handle(hh0);

	std::vector<int> oppsitevertices;
	pHarmonicMapping->GetTwoOppositePoints(pSourceMesh, edgeindex, oppsitevertices);
	ASSERT(oppsitevertices.size() == 2);

	int vh3_index = oppsitevertices[0];
	int vh4_index = oppsitevertices[1];

	MyMesh::VertexHandle vh3 = pSourceMesh->pmesh->vertex_handle(vh3_index);
	MyMesh::VertexHandle vh4 = pSourceMesh->pmesh->vertex_handle(vh4_index);

	OpenMesh::Vec3d v1 = pSourceMesh->pmesh->point(vh1);
	OpenMesh::Vec3d v2 = pSourceMesh->pmesh->point(vh2);
	OpenMesh::Vec3d v3 = pSourceMesh->pmesh->point(vh3);
	OpenMesh::Vec3d v4 = pSourceMesh->pmesh->point(vh4);

	Vector3D v1_v3d, v2_v3d, v3_v3d, v4_v3d, vv;
	v1_v3d.SetValue(v1[0], v1[1], v1[2]);
	v2_v3d.SetValue(v2[0], v2[1], v2[2]);
	v3_v3d.SetValue(v3[0], v3[1], v3[2]);
	v4_v3d.SetValue(v4[0], v4[1], v4[2]);
	vv = v4_v3d - v3_v3d;
	double x = -vv[1];
	double y = vv[0];
	vv[0] = x;
	vv[1] = y;
	double step = ((v1_v3d - v3_v3d) * vv) / ((v1_v3d - v2_v3d) * vv);
	return step;
}
double Actions_DataAnalysis::ComputeIntersectionPointonAnotherDiagonalRatio(CMesh3d_OpenMesh* pSourceMesh, int edgeindex)
{
	MyMesh::EdgeHandle eh = pSourceMesh->pmesh->edge_handle(edgeindex);

	MyMesh::HalfedgeHandle hh0 = pSourceMesh->pmesh->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle hh1 = pSourceMesh->pmesh->halfedge_handle(eh, 1);

	MyMesh::VertexHandle vh1 = pSourceMesh->pmesh->to_vertex_handle(hh0);
	MyMesh::VertexHandle vh2 = pSourceMesh->pmesh->from_vertex_handle(hh0);

	std::vector<int> oppsitevertices;
	pHarmonicMapping->GetTwoOppositePoints(pSourceMesh, edgeindex, oppsitevertices);
	ASSERT(oppsitevertices.size() == 2);

	int vh3_index = oppsitevertices[0];
	int vh4_index = oppsitevertices[1];

	MyMesh::VertexHandle vh3 = pSourceMesh->pmesh->vertex_handle(vh3_index);
	MyMesh::VertexHandle vh4 = pSourceMesh->pmesh->vertex_handle(vh4_index);

	OpenMesh::Vec3d v1 = pSourceMesh->pmesh->point(vh1);
	OpenMesh::Vec3d v2 = pSourceMesh->pmesh->point(vh2);
	OpenMesh::Vec3d v3 = pSourceMesh->pmesh->point(vh3);
	OpenMesh::Vec3d v4 = pSourceMesh->pmesh->point(vh4);
	
	Vector3D v1_v3d, v2_v3d,v3_v3d,v4_v3d, vv;
	v1_v3d.SetValue(v1[0], v1[1], v1[2]);
	v2_v3d.SetValue(v2[0], v2[1], v2[2]);
	v3_v3d.SetValue(v3[0], v3[1], v3[2]);
	v4_v3d.SetValue(v4[0], v4[1], v4[2]);
	vv = v2_v3d - v1_v3d;
	double x = -vv[1];
	double y = vv[0];
	vv[0] = x;
	vv[1] = y;
	double step = ((v3_v3d - v1_v3d) * vv) / ((v3_v3d - v4_v3d) * vv);
	return step;
}
int Actions_DataAnalysis::HarmonicIterationsbyEdgeSwap(CMesh3d_OpenMesh* pSourceMesh)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;

	//Vertex selected flag;
	if (!pSourceMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pSourceMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	int iternum = 0;
	while (1)
	{
		iternum++;
		if (iternum > 50)
			return 0;
		//
		pHarmonicMapping->Compute_Linear_Equations_WithStraighteningConstraints_FixEndPoints(pSourceMesh);

		//pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(4777)) = 1;
		//pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(4694)) = 1;
		//pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(4693)) = 1;

		//

		int flag = pHarmonicMapping->IsMeshUVFlip(pSourceMesh);

		FPROP_FACE_SELECTED_FLAG fprop_face_selectedflag;

		//Selected Face
		if (!pSourceMesh->pmesh->get_property_handle(fprop_face_selectedflag, PROP_FACE_SELECTED))
			pSourceMesh->pmesh->add_property(fprop_face_selectedflag, PROP_FACE_SELECTED);

		double mint = 1;
		int minfaceindex1 = -1;
		if (flag)
		{
			//find the face with the smallest t
			for (MyMesh::FaceIter f_it = pSourceMesh->pmesh->faces_begin(); f_it != pSourceMesh->pmesh->faces_end(); ++f_it)
			{
				int flag = pSourceMesh->pmesh->property(fprop_face_selectedflag, f_it.handle());
				if (flag)
				{
					//find the t where it becomes colinear, degenerate
					double t = FindColinearParameter(pSourceMesh, f_it.handle());
					if (t < mint)
					{
						mint = t;
						minfaceindex1 = f_it.handle().idx();
					}
				}
			}

			//Find the edge to switch the longest edge
			int vindex1, vindex2, edgeindex;
			pSourceMesh->FindSwitchEdge(pSourceMesh->pmesh->face_handle(minfaceindex1), mint, vindex1, vindex2, edgeindex);

			//change topology of the mesh
			std::vector<int> m_ArrayIds;
			//pHarmonicMapping->SwapAdjacentTriangles(pSourceMesh, edgeindex);
			int flag1 = pHarmonicMapping->SwapAdjacentTriangles_Propogate(pSourceMesh, edgeindex);
			if (!flag1)
				return 0;
			pSourceMesh->pmesh->garbage_collection();

			/*double minarea = 1e10;
			int facenum = pSourceMesh->pmesh->n_faces();
			int faceindex;
			for (MyMesh::FaceIter f_it = pSourceMesh->pmesh->faces_begin(); f_it != pSourceMesh->pmesh->faces_end(); ++f_it)
			{
				faceindex = f_it.handle().idx();
				MyMesh::FaceVertexIter fv_it = pSourceMesh->pmesh->fv_iter(f_it.handle());

				OpenMesh::Vec3d v1 = pSourceMesh->pmesh->point(fv_it.handle());
				int facevertexindex1 = fv_it.handle().idx();
				fv_it++;
				OpenMesh::Vec3d v2 = pSourceMesh->pmesh->point(fv_it.handle());
				int facevertexindex2 = fv_it.handle().idx();
				fv_it++;
				OpenMesh::Vec3d v3 = pSourceMesh->pmesh->point(fv_it.handle());
				int facevertexindex3 = fv_it.handle().idx();

				double area = pSourceMesh->GetTriangleSignArea(faceindex);
				if (area < 0)
				{
					//minarea = area;
					minfaceindex1 = f_it.handle().idx();
				}
			}*/
			//if (minarea < 0)
			//{
			//	int ij = 0;
			//}
		}
		else
		{
			break;
		}
	}

	//
	AddResultantMesh(pSourceMesh);

	//Save
	SaveResultantMesh(pSourceMesh);
	//pHarmonicMapping->GenerateParameterDomain(pSourceMesh);
	return 1;
	//
	//char filename[80];
	//sprintf(filename, "D:\\1.bmp");
	//OutPutMeshMutoFile(pSourceMesh,filename);
}
int Actions_DataAnalysis::HarmonicIterationsbyEdgeSwap_Debug(CMesh3d_OpenMesh* pSourceMesh)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	EPROP_EDGE_SELECTED_FLAG eprop_selectedflag;

	//Vertex selected flag;
	if (!pSourceMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pSourceMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Edge selected flag;
	if (!pSourceMesh->pmesh->get_property_handle(eprop_selectedflag, PROP_EDGE_SELECTED))
		pSourceMesh->pmesh->add_property(eprop_selectedflag, PROP_EDGE_SELECTED);

	//if (pDoc->m_SceneGraph.NbObject() == 4)
	//{
	//	pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);
	//	pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);
	//}
	//int iternum = 0;
	//while (1)
	{
		iternum_global++;

		//if (iternum > 50)
		//	return 0;
		//
		pHarmonicMapping->Compute_Linear_Equations_WithStraighteningConstraints_FixEndPoints(pSourceMesh);

		//pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(4777)) = 1;
		//pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(4694)) = 1;
		//pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(4693)) = 1;

		//

		int flag = pHarmonicMapping->IsMeshUVFlip(pSourceMesh);

		if (!flag)
			AfxMessageBox("Finish Sucessfully!");

		FPROP_FACE_SELECTED_FLAG fprop_face_selectedflag;

		//Selected Face
		if (!pSourceMesh->pmesh->get_property_handle(fprop_face_selectedflag, PROP_FACE_SELECTED))
			pSourceMesh->pmesh->add_property(fprop_face_selectedflag, PROP_FACE_SELECTED);

		double mint = 1;
		int minfaceindex1 = -1;
		if (flag)
		{
			//find the face with the smallest t
			for (MyMesh::FaceIter f_it = pSourceMesh->pmesh->faces_begin(); f_it != pSourceMesh->pmesh->faces_end(); ++f_it)
			{
				int flag = pSourceMesh->pmesh->property(fprop_face_selectedflag, f_it.handle());
				if (flag)
				{
					//find the t where it becomes colinear, degenerate
					double t = FindColinearParameter(pSourceMesh, f_it.handle());
					if (t < mint)
					{
						mint = t;
						minfaceindex1 = f_it.handle().idx();
					}
				}
			}

			//Find the edge to switch the longest edge
			//int vindex1, vindex2;
			pSourceMesh->FindSwitchEdge(pSourceMesh->pmesh->face_handle(minfaceindex1), mint, vindex1_global, vindex2_global, edgeindex_global);

			for (MyMesh::EdgeIter e_it = pSourceMesh->pmesh->edges_begin(); e_it != pSourceMesh->pmesh->edges_end(); ++e_it)
			{
				pSourceMesh->pmesh->property(eprop_selectedflag, e_it.handle()) = 0;
			}

			for (MyMesh::VertexIter v_it = pSourceMesh->pmesh->vertices_begin(); v_it != pSourceMesh->pmesh->vertices_end(); ++v_it)
			{
				pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
			}

			pSourceMesh->pmesh->property(eprop_selectedflag, pSourceMesh->pmesh->edge_handle(edgeindex_global)) = 1;

			pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(vindex1_global)) = 1;
			pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(vindex2_global)) = 1;

			if (pDoc->m_SceneGraph.NbObject() == 4)
			{
				UpdateResultantMesh_Debug(pSourceMesh);

				CMesh3d_OpenMesh* pOpenMesh1 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
				CMesh3d_OpenMesh* pOpenMesh2 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);

				OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh1, vindex1_global);
				OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh2, vindex1_global);
			}
			else
			{
				AddResultantMesh_Debug(pSourceMesh);

				//Zoom
				CMesh3d_OpenMesh* pOpenMesh1 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
				CMesh3d_OpenMesh* pOpenMesh2 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);

				OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh1, vindex1_global);
				OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh2, vindex1_global);

				pOpenMesh1->m_showFace = 0;
				pOpenMesh1->m_showEdge = 1;
				pOpenMesh1->m_showSelectedVertex = 1;
				pOpenMesh1->m_showSelectedEdge = 1;

				pOpenMesh2->m_showFace = 0;
				pOpenMesh2->m_showEdge = 0;
				pOpenMesh2->m_showSelectedVertex = 0;
				pOpenMesh2->m_showSelectedEdge = 0;
			}

			char message[1000];
			sprintf(message, "Iteration Num:%d, Edge Index:%d, Vertex Index(%d,%d)", iternum_global, edgeindex_global, vindex1_global, vindex2_global);
			std::string str = message;
			AfxGetMainWnd()->SetWindowText((str.c_str()));

			return 1;
			//Zoom
			//OnBnClickedButtonZoomCenteredAtvertex(pSourceMesh, vindex1);

			//change topology of the mesh
			//std::vector<int> m_ArrayIds;
			//pHarmonicMapping->SwapAdjacentTriangles(pSourceMesh, edgeindex);



			/*double minarea = 1e10;
			int facenum = pSourceMesh->pmesh->n_faces();
			int faceindex;
			for (MyMesh::FaceIter f_it = pSourceMesh->pmesh->faces_begin(); f_it != pSourceMesh->pmesh->faces_end(); ++f_it)
			{
			faceindex = f_it.handle().idx();
			MyMesh::FaceVertexIter fv_it = pSourceMesh->pmesh->fv_iter(f_it.handle());

			OpenMesh::Vec3d v1 = pSourceMesh->pmesh->point(fv_it.handle());
			int facevertexindex1 = fv_it.handle().idx();
			fv_it++;
			OpenMesh::Vec3d v2 = pSourceMesh->pmesh->point(fv_it.handle());
			int facevertexindex2 = fv_it.handle().idx();
			fv_it++;
			OpenMesh::Vec3d v3 = pSourceMesh->pmesh->point(fv_it.handle());
			int facevertexindex3 = fv_it.handle().idx();

			double area = pSourceMesh->GetTriangleSignArea(faceindex);
			if (area < 0)
			{
			//minarea = area;
			minfaceindex1 = f_it.handle().idx();
			}
			}*/
			//if (minarea < 0)
			//{
			//	int ij = 0;
			//}
		}
		//else
		//{
		//	break;
		//}
	}


	//pHarmonicMapping->GenerateParameterDomain(pSourceMesh);
	return 1;
	//
	//char filename[80];
	//sprintf(filename, "D:\\1.bmp");
	//OutPutMeshMutoFile(pSourceMesh,filename);
}
void Actions_DataAnalysis::SaveResultantMesh(CMesh3d_OpenMesh* pSourceMesh)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pSourceMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pSourceMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSourceMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Vertex MU
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pSourceMesh->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face MU
	if (!pSourceMesh->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pSourceMesh->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	//persistant
	pSourceMesh->pmesh->property(vprop_vertex_uv).set_persistent(true);
	//pSourceMesh->pmesh->property(vprop_selectedflag).set_persistent(true);
	//pSourceMesh->pmesh->property(fprop_face_mu).set_persistent(true);
	//pSourceMesh->pmesh->property(vprop_vertex_mu).set_persistent(true);

	std::string path = "E:\\BU_Data\\resultantmesh\\";
	std::string filename = pSourceMesh->filename;
	std::string fullfilename = path + filename;

	//fullfilename.replace(fullfilename.size() - 2, 2, ".obj");
	fullfilename.replace(fullfilename.size() - 2, 2, ".om");
	//pSourceMesh->filename
	OpenMesh::IO::write_mesh(*pSourceMesh->pmesh, fullfilename);

	//Additional Information
	//fullfilename.replace(fullfilename.size() - 4,4, ".om");
	//std::ofstream outfile(fullfilename);
	//pSourceMesh->m_Transform.Save(outfile);
	//outfile.close();
}
void Actions_DataAnalysis::ReadResultantMesh(std::string fullfilename)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//
	CMesh3d_OpenMesh* pSourceMesh = new CMesh3d_OpenMesh();

	//Vertex selected flag;
	if (!pSourceMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pSourceMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSourceMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Vertex MU
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pSourceMesh->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face MU
	if (!pSourceMesh->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pSourceMesh->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);


	//persistant
	pSourceMesh->pmesh->property(vprop_vertex_uv).set_persistent(true);
	pSourceMesh->pmesh->property(vprop_selectedflag).set_persistent(true);
	pSourceMesh->pmesh->property(fprop_face_mu).set_persistent(true);
	pSourceMesh->pmesh->property(vprop_vertex_mu).set_persistent(true);

	//std::string path = "E:\\BU_Data\\resultantmesh\\";
	//std::string filename = pSourceMesh->filename;
	//std::string fullfilename = path + filename;

	//pSourceMesh->filename
	if (!OpenMesh::IO::read_mesh(*pSourceMesh->pmesh, fullfilename))
	{
		return;
	}
	//fullfilename.replace(fullfilename.size() - 2, 2, ".om");
	//std::ifstream infile(fullfilename);
	//pSourceMesh->m_Transform.Read(infile);
	//infile.close();

	AddResultantMesh(pSourceMesh);
}
void Actions_DataAnalysis::PlotMeshMu(CMesh3d_OpenMesh* pSourceMesh)
{
	//view point
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	double aspect = 1;

	glViewport(0, 0, 512, 512);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(pTreeView->FOVY, aspect, 0.1, 2000);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	//glDrawBuffer(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//gluLookAt(g_eye[0],g_eye[1],g_eye[2],g_look[0],g_look[1],g_look[2],g_dir[0],g_dir[1],g_dir[2]);
	gluLookAt(pTreeView->g_eye[0], pTreeView->g_eye[1], pTreeView->g_eye[2], pTreeView->g_look[0], pTreeView->g_look[1], pTreeView->g_look[2], pTreeView->g_dir[0], pTreeView->g_dir[1], pTreeView->g_dir[2]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
	glScalef(pSourceMesh->m_Transform.GetScale()->x(),
		pSourceMesh->m_Transform.GetScale()->y(),
		pSourceMesh->m_Transform.GetScale()->z());

	glTranslatef(pSourceMesh->m_Transform.GetTranslation()->x(),
		pSourceMesh->m_Transform.GetTranslation()->y(),
		pSourceMesh->m_Transform.GetTranslation()->z());

	glRotatef(pSourceMesh->m_Transform.GetValueRotation(),
		pSourceMesh->m_Transform.GetRotation()->x(),
		pSourceMesh->m_Transform.GetRotation()->y(),
		pSourceMesh->m_Transform.GetRotation()->z());

	pSourceMesh->ShowVertexMU();

	//
	pSourceMesh->m_SharpeEdgeColor.Set(255, 255, 255);
	pSourceMesh->ShowSharpEdges();

	glPopMatrix();
	glPopMatrix();
	// Double buffer
	//SwapBuffers(hDC);
	//SwapBuffers();
	glFlush();
	glPopMatrix();
	glPopAttrib();

	//recover the viewpoint
	/*CRect rect;
	pTreeView->GetClientRect(&rect);
	double cx = rect.Width();
	double cy = rect.Height();
	glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(pTreeView->FOVY, aspect, 0.1, 2000);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();*/
	pDoc->UpdateAllViews(NULL);
}
void Actions_DataAnalysis::OutPutMeshMutoFile(CMesh3d_OpenMesh* pSourceMesh,char* filename)
{
	CImage *pResultImage;
	pResultImage = new CImage();
	pResultImage->Create(512, 512, 32, CImage::createAlphaChannel);
	Point3D lower, upper;
	pSourceMesh->ComputeBoundingBox(lower, upper);
	double length = upper[0] - lower[0];
	if (upper[1] - lower[1] > length)
		length = upper[1] - lower[1];

	double step = length / 512;
	COLORREF color;
	for (int i = 0; i <= 512; i++)
		for (int j = 0; j <= 512; j++)
		{
			double x = lower[0] + step * i;
			double y = lower[1] + step * j;

			GetMuColorFromMesh(pSourceMesh, x, y,color);
			pResultImage->SetPixel(i, j, color);
		}

	pResultImage->Save(filename);
}
static GLuint g_FBO;
static GLuint g_FBOTex;
void Actions_DataAnalysis::DrawMuImageinBuffer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glGenFramebuffers(1, &g_FBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, g_FBO);

	//glGenTextures(1, &g_FBOTex); // Create a empty texture as the render target
	//glBindTexture(GL_TEXTURE_2D, g_FBOTex);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 400, 400, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Attach the texture object to frame buffer object
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_FBOTex, 0);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the frame buffer
}
void Actions_DataAnalysis::GetMuColorFromMesh(CMesh3d_OpenMesh* pSourceMesh, double x, double y, COLORREF color)
{
	color = RGB(255, 255, 255);

	CImage m_HotTextureImage;
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_MU vprop_vertex_mu;
	FPROP_FACE_MU fprop_face_mu;

	//Vertex UV
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSourceMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex mu
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_mu, PROP_VERTEX_MU))
		pSourceMesh->pmesh->add_property(vprop_vertex_mu, PROP_VERTEX_MU);

	//Face mu
	if (!pSourceMesh->pmesh->get_property_handle(fprop_face_mu, PROP_FACE_MU))
		pSourceMesh->pmesh->add_property(fprop_face_mu, PROP_FACE_MU);

	std::vector<double> mu;
	mu.resize(2);
	for (MyMesh::FaceIter f_it = pSourceMesh->pmesh->faces_begin(); f_it != pSourceMesh->pmesh->faces_end(); ++f_it)
	{
		MyMesh::FaceVertexIter fv_it = pSourceMesh->pmesh->fv_iter(f_it.handle());
		
		//std::vector<double> uv1 = pMesh3D->pmesh->property(vprop_vertex_originallocation, fv_it.handle());
		OpenMesh::VertexHandle vh1 = fv_it.handle();
		std::vector<double> mu1 = pSourceMesh->pmesh->property(vprop_vertex_mu, vh1);

		OpenMesh::Vec3d v1 = pSourceMesh->pmesh->point(vh1);
		fv_it++;
		//std::vector<double> uv2 = pMesh3D->pmesh->property(vprop_vertex_originallocation, fv_it.handle());
		OpenMesh::VertexHandle vh2 = fv_it.handle();
		OpenMesh::Vec3d v2 = pSourceMesh->pmesh->point(vh2);
		std::vector<double> mu2 = pSourceMesh->pmesh->property(vprop_vertex_mu, vh2);

		fv_it++;
		OpenMesh::VertexHandle vh3 = fv_it.handle();
		OpenMesh::Vec3d v3 = pSourceMesh->pmesh->point(vh3);
		std::vector<double> mu3 = pSourceMesh->pmesh->property(vprop_vertex_mu, vh3);
		//std::vector<double> uv3 = pMesh3D->pmesh->property(vprop_vertex_originallocation, fv_it.handle());

		int flag = CMesh3d::JudeWhetherPointLocatedInTriangle(x, y,v1[0],v1[1],v2[0],v2[1],v3[0],v3[1]);
		if (flag)
		{
			double A = CMesh3d::TriangleArea(v1[0], v1[1], v2[0], v2[1], v3[0], v3[1]);
			double A1 = CMesh3d::TriangleArea(x, y, v2[0], v2[1], v3[0], v3[1]);
			double A2 = CMesh3d::TriangleArea(v1[0], v1[1], x, y, v3[0], v3[1]);
			double A3 = CMesh3d::TriangleArea(v1[0], v1[1], v2[0], v2[1], x, y);

			A1 = A1 / A;
			A2 = A2 / A;
			A3 = A3 / A;

			mu[0] = mu1[0] * A1 + mu2[0] * A2 + mu3[0] * A3;
			mu[1] = mu1[1] * A1 + mu2[1] * A2 + mu3[1] * A3;

			double mumou = sqrt(SQR(mu[0]) + SQR(mu[1]));
			int x = mumou * m_HotTextureImage.GetWidth();
			if (x >= m_HotTextureImage.GetWidth())
				x = m_HotTextureImage.GetWidth() - 1;
			color = m_HotTextureImage.GetPixel(x, 0);
			break;
		}
	}
}
void Actions_DataAnalysis::AddResultMesh_CDT(CMesh3d_OpenMesh* pSourceMesh_New, CMesh3d_OpenMesh* pSourceMesh)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	//pDoc->m_SceneGraph.Add(pSourceMesh);

	//
	MyMesh* newmesh1 = new MyMesh(*pSourceMesh_New->pmesh);
	MyMesh* newmesh2 = new MyMesh(*pSourceMesh->pmesh);

	CMesh3d_OpenMesh* pNewMesh1 = new CMesh3d_OpenMesh();
	CMesh3d_OpenMesh* pNewMesh2 = new CMesh3d_OpenMesh();
	pNewMesh1->pmesh = newmesh1;
	pNewMesh2->pmesh = newmesh2;

	pNewMesh1->m_Transform.Copy(&pSourceMesh_New->m_Transform);
	pNewMesh1->m_LandMarkArray = pSourceMesh_New->m_LandMarkArray;
	pNewMesh1->m_FixVerticesArray = pSourceMesh_New->m_FixVerticesArray;
	pNewMesh1->m_FixVerticesArray_uv = pSourceMesh_New->m_FixVerticesArray_uv;
	pNewMesh1->m_showFace = 0;
	pNewMesh1->m_showLandmarkEdge = 1;
	pDoc->m_SceneGraph.Add(pNewMesh1);

	pNewMesh2->m_Transform.Copy(&pSourceMesh->m_Transform);
	pNewMesh2->m_LandMarkArray = pSourceMesh->m_LandMarkArray;
	pNewMesh2->m_FixVerticesArray = pSourceMesh->m_FixVerticesArray;
	pNewMesh2->m_FixVerticesArray_uv = pSourceMesh->m_FixVerticesArray_uv;
	pNewMesh2->m_showFace = 0;
	pNewMesh2->m_showLandmarkEdge = 1;
	pDoc->m_SceneGraph.Add(pNewMesh2);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_DataAnalysis::AddResultantMesh(CMesh3d_OpenMesh* pSourceMesh)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	pDoc->m_SceneGraph.Add(pSourceMesh);

	//
	MyMesh* newmesh = new MyMesh(*pSourceMesh->pmesh);

	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;

	//Vertex UV
	if (!newmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		newmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!newmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		newmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	for (MyMesh::VertexIter v_it = newmesh->vertices_begin(); v_it != newmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d p;
		std::vector<double> uv = newmesh->property(vprop_vertex_uv, v_it.handle());
		if (uv.size() > 0)
		{
			p[0] = uv[0];
			p[1] = uv[1];
			p[2] = 0;
			newmesh->set_point(v_it.handle(), p);
		}
	}

	CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pNewMesh->pmesh = newmesh;

	pNewMesh->m_Transform.Copy(&pSourceMesh->m_Transform);
	pNewMesh->m_LandMarkArray = pSourceMesh->m_LandMarkArray;
	pNewMesh->m_FixVerticesArray = pSourceMesh->m_FixVerticesArray;
	pNewMesh->m_FixVerticesArray_uv = pSourceMesh->m_FixVerticesArray_uv;
	pNewMesh->m_showFace = 0;
	pNewMesh->m_showLandmarkEdge = 1;
	pDoc->m_SceneGraph.Add(pNewMesh);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_DataAnalysis::AddResultantMesh_Debug(CMesh3d_OpenMesh* pSourceMesh)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	pDoc->m_SceneGraph.Add(pSourceMesh);
	pSourceMesh->m_showFace = 0;
	pSourceMesh->m_showEdge = 0;

	//
	MyMesh* newmesh = new MyMesh(*pSourceMesh->pmesh);

	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;

	//Vertex UV
	if (!newmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		newmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!newmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		newmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	for (MyMesh::VertexIter v_it = newmesh->vertices_begin(); v_it != newmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d p;
		std::vector<double> uv = newmesh->property(vprop_vertex_uv, v_it.handle());
		if (uv.size() > 0)
		{
			p[0] = uv[0];
			p[1] = uv[1];
			p[2] = 0;
			newmesh->set_point(v_it.handle(), p);
		}
	}

	CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pNewMesh->pmesh = newmesh;

	pNewMesh->m_Transform.Copy(&pSourceMesh->m_Transform);
	pNewMesh->m_LandMarkArray = pSourceMesh->m_LandMarkArray;
	pNewMesh->m_FixVerticesArray = pSourceMesh->m_FixVerticesArray;
	pNewMesh->m_FixVerticesArray_uv = pSourceMesh->m_FixVerticesArray_uv;
	pNewMesh->m_showFace = 0;
	pNewMesh->m_showEdge = 1;
	pNewMesh->m_showLandmarkEdge = 1;
	pDoc->m_SceneGraph.Add(pNewMesh);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void Actions_DataAnalysis::UpdateResultantMesh_Debug(CMesh3d_OpenMesh* pSourceMesh)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)(p_FormView3->GetDocument());
	CMesh3d_OpenMesh* pMesh = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	//pDoc->m_SceneGraph.Add(pSourceMesh);
	//pSourceMesh->m_showFace = 0;
	//pSourceMesh->m_showEdge = 0;

	//
	MyMesh* newmesh = new MyMesh(*pSourceMesh->pmesh);

	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_NON_BOUNDARY_INDEX vprop_nonboundaryindex;

	//Vertex UV
	if (!newmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		newmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!newmesh->get_property_handle(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX))
		newmesh->add_property(vprop_nonboundaryindex, PROP_VERTEX_NON_BOUNDARY_INDEX);

	for (MyMesh::VertexIter v_it = newmesh->vertices_begin(); v_it != newmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d p;
		std::vector<double> uv = newmesh->property(vprop_vertex_uv, v_it.handle());
		if (uv.size() > 0)
		{
			p[0] = uv[0];
			p[1] = uv[1];
			p[2] = 0;
			newmesh->set_point(v_it.handle(), p);
		}
	}
	delete pMesh->pmesh;
	//CMesh3d_OpenMesh* pNewMesh = new CMesh3d_OpenMesh();
	pMesh->pmesh = newmesh;

	//pMesh->m_Transform.Copy(&pSourceMesh->m_Transform);
	//pMesh->m_LandMarkArray = pSourceMesh->m_LandMarkArray;
	//pNewMesh->m_FixVerticesArray = pSourceMesh->m_FixVerticesArray;
	//pNewMesh->m_FixVerticesArray_uv = pSourceMesh->m_FixVerticesArray_uv;
	//pNewMesh->m_showFace = 0;
	//pNewMesh->m_showEdge = 1;
	//pNewMesh->m_showLandmarkEdge = 1;
	//pDoc->m_SceneGraph.Add(pNewMesh);

	//pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Actions_DataAnalysis::AddLandmarkEndPointsToFixArray(CMesh3d_OpenMesh* pSourceMesh, CMesh3d_OpenMesh* pTargetMesh)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pSourceMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pSourceMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSourceMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uvvalues;
	uvvalues.resize(2);
	OpenMesh::Vec3d v0;

	//initialization
	for (MyMesh::VertexIter v_it = pSourceMesh->pmesh->vertices_begin(); v_it != pSourceMesh->pmesh->vertices_end(); ++v_it)
	{
		pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}

	//fix the boundary vertices at its original positions
	for (MyMesh::VertexIter v_it = pSourceMesh->pmesh->vertices_begin(); v_it != pSourceMesh->pmesh->vertices_end(); ++v_it)
	{
		int flag = pSourceMesh->pmesh->is_boundary(v_it.handle());
		if (flag)
		{
			pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 1;

			pSourceMesh->m_FixVerticesArray.push_back(v_it.handle().idx());

			v0 = pSourceMesh->pmesh->point(v_it.handle());
			uvvalues[0] = v0[0];
			uvvalues[1] = v0[1];
			pSourceMesh->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
		}
		//pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}
	//fix the end points of the landmark
	for (int i = 0; i < pSourceMesh->m_LandMarkArray.size(); i++)
	{
		//source landmark and its..
		GeoMapLandMark* pSourceLandmark = pSourceMesh->m_LandMarkArray[i];
		int startpointindex_Source = pSourceLandmark->landmark_points[0];
		int endpointindex_Source = pSourceLandmark->landmark_points[pSourceLandmark->landmark_points.size() - 1];

		pSourceMesh->m_FixVerticesArray.push_back(startpointindex_Source);
		pSourceMesh->m_FixVerticesArray.push_back(endpointindex_Source);

		pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(startpointindex_Source)) = 1;
		pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(endpointindex_Source)) = 1;

		//target landmark and its
		GeoMapLandMark* pTargetLandmark = pTargetMesh->m_LandMarkArray[i];
		int startpointindex_Target = pTargetLandmark->landmark_points[0];
		int endpointindex_Target = pTargetLandmark->landmark_points[pTargetLandmark->landmark_points.size() - 1];

		OpenMesh::Vec3d targetlandmark_startpointpos = pTargetMesh->pmesh->point(pTargetMesh->pmesh->vertex_handle(startpointindex_Target));
		OpenMesh::Vec3d targetlandmark_endpointpos = pTargetMesh->pmesh->point(pTargetMesh->pmesh->vertex_handle(endpointindex_Target));

		uvvalues[0] = targetlandmark_startpointpos[0];
		uvvalues[1] = targetlandmark_startpointpos[1];

		pSourceMesh->pmesh->property(vprop_vertex_uv, pSourceMesh->pmesh->vertex_handle(startpointindex_Source)) = uvvalues;

		uvvalues[0] = targetlandmark_endpointpos[0];
		uvvalues[1] = targetlandmark_endpointpos[1];

		pSourceMesh->pmesh->property(vprop_vertex_uv, pSourceMesh->pmesh->vertex_handle(endpointindex_Source)) = uvvalues;
	}
}

double Actions_DataAnalysis::FindColinearParameter(CMesh3d_OpenMesh* pSourceMesh, OpenMesh::FaceHandle fh)
{
	double sourecearea = pSourceMesh->GetTriangleSignArea(fh.idx());
	double targetarea = pSourceMesh->GetTriangleSignArea_UV(fh.idx());
	double mint = 0;
	double maxt = 1;
	double t;
	double area;
	while (sourecearea > 0 && targetarea < 0)
	{
		t = (mint + maxt) / 2;
		area = pSourceMesh->GetTriangleSignArea_Mesh_UV_Interpolate(fh.idx(), t);
		if (area < 0)
		{
			maxt = t;
		}
		else
		{
			mint = t;
		}
		if (fabs(maxt - mint) < 1e-5)
			break;
	}
	return mint;
}

void Actions_DataAnalysis::OnBnClickedButtonZoomCenteredAtvertex()
{
	// TODO: Add your control notification handler code here
	CEdit* pZoomCenter;
	pZoomCenter = (CEdit*)GetDlgItem(IDC_EDIT_ZOOM_CENTER_VERTEX);
	CString str;
	pZoomCenter->GetWindowText(str);
	int vindex = atoi(str);

	CObject3d * pObject = pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
	double x, y, z;
	Point3D lower, upper;
	if (pObject->GetType() == TYPE_MESH3D)
	{
		CMesh3d* pMesh3d = (CMesh3d*)pObject;
		x = pMesh3d->GetVertex(vindex)->x();
		y = pMesh3d->GetVertex(vindex)->y();
		z = pMesh3d->GetVertex(vindex)->z();
		pMesh3d->ComputeBoundingBox(lower, upper);
		CVector3d m_translation;
		CVector3d m_Scale;
		CTransform transform;
		transform.Clear();
		m_translation.Set(-x, -y, -z);
		float length = upper[0] - lower[0];
		if (length < upper[1] - lower[1])
			length = upper[1] - lower[1];
		if (length < upper[2] - lower[2])
			length = upper[2] - lower[2];

		transform.SetTranslation(&m_translation);
		length = length * 0.05 / 2;
		m_Scale.Set(1 / length, 1 / length, 1 / length);
		transform.SetScale(&m_Scale);
		pMesh3d->SetTransform(transform);
	}
	else if (pObject->GetType() == TYPE_MESH3D_OPENMESH)
	{
		CMesh3d_OpenMesh* pOpenMesh = (CMesh3d_OpenMesh*)pObject;
		OpenMesh::Vec3d point = pOpenMesh->pmesh->point(pOpenMesh->pmesh->vertex_handle(vindex));
		x = point[0];
		y = point[1];
		z = point[2];
		pOpenMesh->ComputeBoundingBox(lower, upper);
		CVector3d m_translation;
		CVector3d m_Scale;
		CTransform transform;
		transform.Clear();
		m_translation.Set(-x, -y, -z);
		float length = upper[0] - lower[0];
		if (length < upper[1] - lower[1])
			length = upper[1] - lower[1];
		if (length < upper[2] - lower[2])
			length = upper[2] - lower[2];

		transform.SetTranslation(&m_translation);
		length = length * 0.05 / 2;
		m_Scale.Set(1 / length, 1 / length, 1 / length);
		transform.SetScale(&m_Scale);
		pOpenMesh->SetTransform(transform);
	}
	pDoc->UpdateAllViews(NULL);
}
void Actions_DataAnalysis::OnBnClickedButtonZoomCenteredAtvertex(CMesh3d_OpenMesh* pOpenMesh, int vindex)
{
	// TODO: Add your control notification handler code here
	OpenMesh::Vec3d point = pOpenMesh->pmesh->point(pOpenMesh->pmesh->vertex_handle(vindex));
	double x, y, z;
	Point3D lower, upper;
	x = point[0];
	y = point[1];
	z = point[2];
	pOpenMesh->ComputeBoundingBox(lower, upper);
	CVector3d m_translation;
	CVector3d m_Scale;
	CTransform transform;
	transform.Clear();
	m_translation.Set(-x, -y, -z);
	float length = upper[0] - lower[0];
	if (length < upper[1] - lower[1])
		length = upper[1] - lower[1];
	if (length < upper[2] - lower[2])
		length = upper[2] - lower[2];

	transform.SetTranslation(&m_translation);
	length = length * 0.05 / 2;
	m_Scale.Set(1 / length, 1 / length, 1 / length);
	transform.SetScale(&m_Scale);
	pOpenMesh->SetTransform(transform);

	pDoc->UpdateAllViews(NULL);
}

void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisReadsavedmesh()
{
	// TODO: Add your control notification handler code here
	CString FileName;
	CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
	}
	else
		return;

	std::string fullfilename = FileName;

	//
	ReadResultantMesh(fullfilename);

	//SaveLandmark(FileName);
}


void Actions_DataAnalysis::OnBnClickedButtonSetmeshuvDataanalysisMesh()
{
	// TODO: Add your control notification handler code here
	pSoureceSurface_Openmesh_Morph = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
	char message[80];
	sprintf(message, "%d", p_FormView3->currentindex);
	GetDlgItem(IDC_BUTTON_SetMeshUV_Mesh_DATAANALYSIS_Text)->SetWindowText(message);
}


void Actions_DataAnalysis::OnBnClickedButtonSetmeshuvDataanalysisTarget()
{
	// TODO: Add your control notification handler code here
	pTargetSurface_Openmesh_Morph = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
	char message[80];
	sprintf(message, "%d", p_FormView3->currentindex);
	GetDlgItem(IDC_BUTTON_SetMeshUV_TARGET_DATAANALYSIS_Text)->SetWindowText(message);
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisGeneratemorphmesh()
{
	// TODO: Add your control notification handler code here
	pIntermediateSurface_Openmesh_Morph = pSoureceSurface_Openmesh_Morph->copy();
	pIntermediateSurface_Openmesh_Morph->filename = "Morph";
	pDoc->m_SceneGraph.Add(pIntermediateSurface_Openmesh_Morph);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Actions_DataAnalysis::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);

	// Check which slider sent the notification  
	if (pSlider == &m_DataAnalysisMorph_Slider)
	{
		m_DataAnlysis_Pos = m_DataAnalysisMorph_Slider.GetPos();
		MorphIntermediateMesh();
		//sprintf(Post_Message, "Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d, Angle: %d", pMesh->NbFace(), pMesh->NbVertex(), pMesh->m_ArraySharpEdges.size(), pMesh->m_LandMarkArray.size(), m_SplitAngle);
		//::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	}
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
BOOL Actions_DataAnalysis::MorphIntermediateMesh()
{
	if (!pIntermediateSurface_Openmesh_Morph)
		return 0;

	for (MyMesh::VertexIter v_it = pSoureceSurface_Openmesh_Morph->pmesh->vertices_begin(); v_it != pSoureceSurface_Openmesh_Morph->pmesh->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d v0 = pSoureceSurface_Openmesh_Morph->pmesh->point(v_it.handle());
		OpenMesh::Vec3d v1 = pTargetSurface_Openmesh_Morph->pmesh->point(v_it.handle());
		OpenMesh::Vec3d v2;
		double t = m_DataAnalysisMorph_Slider.GetPos() / 100.0;
		v2[0] = (1 - t) * v0[0] + t * v1[0];
		v2[1] = (1 - t) * v0[1] + t * v1[1];
		v2[2] = (1 - t) * v0[2] + t * v1[2];

		pIntermediateSurface_Openmesh_Morph->pmesh->set_point(v_it.handle(), v2);
		//pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}
	pDoc->UpdateAllViews(NULL);
}

BOOL Actions_DataAnalysis::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_DataAnalysisMorph_Slider.SetRangeMin(0);
	m_DataAnalysisMorph_Slider.SetRangeMax(100);
	m_DataAnalysisMorph_Slider.SetRange(0, 100);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void Actions_DataAnalysis::FixBoundaryandOneInnerVertex(CMesh3d_OpenMesh* pSourceMesh, CMesh3d_OpenMesh* pTargetSurface_Openmesh)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pSourceMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pSourceMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSourceMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uvvalues;
	uvvalues.resize(2);
	OpenMesh::Vec3d v0;

	//initialization
	for (MyMesh::VertexIter v_it = pSourceMesh->pmesh->vertices_begin(); v_it != pSourceMesh->pmesh->vertices_end(); ++v_it)
	{
		pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}

	//fix the boundary vertices at its original positions
	for (MyMesh::VertexIter v_it = pSourceMesh->pmesh->vertices_begin(); v_it != pSourceMesh->pmesh->vertices_end(); ++v_it)
	{
		int flag = pSourceMesh->pmesh->is_boundary(v_it.handle());
		if (flag)
		{
			pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 1;

			pSourceMesh->m_FixVerticesArray.push_back(v_it.handle().idx());

			v0 = pSourceMesh->pmesh->point(v_it.handle());
			uvvalues[0] = v0[0];
			uvvalues[1] = v0[1];
			pSourceMesh->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
		}
		//pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}
	
	pSourceMesh->m_FixVerticesArray.push_back(8);
	pSourceMesh->pmesh->property(vprop_selectedflag, pSourceMesh->pmesh->vertex_handle(8)) = 1;
	uvvalues[0] = 4.5;
	uvvalues[1] = 1;
	pSourceMesh->pmesh->property(vprop_vertex_uv, pSourceMesh->pmesh->vertex_handle(8)) = uvvalues;
}
void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonictest()
{
	// TODO: Add your control notification handler code here
	//pDoc->Read_Mesh_OpenMesh("D:\\papers\\code\\data\\dynamicquasiconformal\\example3\\sourcesurface.obj", m_SceneGraph1);

	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_NE00WH_F3D_par_norm.m");
	//pDoc->OnOpenDocument("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\F0001_NE00WH_F3D_par_norm.m");
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\dynamicquasiconformal\\example3\\sourcesurface.obj");
	pSoureceSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	pSoureceSurface_Openmesh = pSoureceSurface_Mesh3d->Convert_OpenMesh();
	pPolyOmino->OnBnClickedOminoSetcurrentmesh();

	//
	pTargetSurface_Openmesh = pSoureceSurface_Openmesh->copy();

	//pDoc->OnOpenDocument(targetmesh);
	//pTargetSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	//p_FormView3->currentindex = pDoc->m_SceneGraph.NbObject() - 1;
	//pTargetSurface_Openmesh = pTargetSurface_Mesh3d->Convert_OpenMesh();
	//pPolyOmino->OnBnClickedOminoSetcurrentmesh();
	//pPolyOmino->GenerateLandMarks_KeepSharpEdgesConsistent();
	//pTargetSurface_Openmesh->m_LandMarkArray = pTargetSurface_Mesh3d->m_LandMarkArray;
	//pDoc->m_SceneGraph.RemoveAt(pDoc->m_SceneGraph.NbObject() - 1);

	//pSoureceSurface_Mesh3d->pReferenceMesh = pTargetSurface_Mesh3d;
	//Add the selected vertices to the fix array
	//AddLandmarkEndPointsToFixArray(pSoureceSurface_Openmesh, pTargetSurface_Openmesh);
	FixBoundaryandOneInnerVertex(pSoureceSurface_Openmesh, pTargetSurface_Openmesh);

	//harmonic map iterations
	HarmonicIterationsbyEdgeSwap(pSoureceSurface_Openmesh);

	//
	//pHarmonicMapping->SetZeroMu(pSoureceSurface_Openmesh);

	//
	//pHarmonicMapping->Compute_Mu(pSoureceSurface_Openmesh);

	//CFramebuffer* buffer = new CFramebuffer();
	//buffer->begin();
	//PlotMeshMu(pSoureceSurface_Openmesh);
	//buffer->saveFrameBuff(muimagefilename);
	//buffer->end();
	//delete buffer;

	//
	//pDoc->m_SceneGraph.Free();
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicPre()
{
	OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic_Debug();
	CMesh3d* pSurface1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pSurface2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);

	pSurface1->m_showFace = 0;
	pSurface1->m_showEdgeIndex = 0;
	pSurface1->m_showLandmarkVertexIndex = 0;

	pSurface2->m_showFace = 0;
	pSurface2->m_showEdgeIndex = 0;
	pSurface2->m_showLandmarkVertexIndex = 0;

	pDoc->UpdateAllViews(NULL);
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterations()
{
	GenerateQuasiConformalMuImage_Debug_Iterations();
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsEdgeswap()
{
	// TODO: Add your control notification handler code here
	int flag1 = pHarmonicMapping->SwapAdjacentTriangles_Propogate(pSoureceSurface_Openmesh, edgeindex_global);
	if (!flag1)
		AfxMessageBox("Edge Swap Error");

	UpdateResultantMesh_Debug(pSoureceSurface_Openmesh);
	//AddResultantMesh_Debug(pSoureceSurface_Openmesh);

	//Zoom
	//CMesh3d_OpenMesh* pOpenMesh1 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	//CMesh3d_OpenMesh* pOpenMesh2 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);

	//OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh1, vindex1_global);
	//OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh2, vindex1_global);

	/*pOpenMesh1->m_showFace = 0;
	pOpenMesh1->m_showEdge = 1;
	pOpenMesh1->m_showSelectedVertex = 1;
	pOpenMesh1->m_showSelectedEdge = 1;

	pOpenMesh2->m_showFace = 0;
	pOpenMesh2->m_showEdge = 0;
	pOpenMesh2->m_showSelectedVertex = 0;
	pOpenMesh2->m_showSelectedEdge = 0;

	pSoureceSurface_Openmesh->pmesh->garbage_collection();*/
	pDoc->UpdateAllViews(NULL);
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicAfter()
{
	// TODO: Add your control notification handler code here
	UpdateResultantMesh_Debug(pSoureceSurface_Openmesh);
	pDoc->UpdateAllViews(NULL);
}

void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic_CDT_Debug()
{
	// TODO: Add your control notification handler code here
	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}
	infile.close();

	std::string filename;
	std::string path("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\");
	std::string imagepath("E:\\BU_Data\\muimages\\");
	for (int i = 0; i < filesnewset.size(); i++)
	{
		//
		//if (i >= 300)
		//	return;

		filename = filesnewset[i];

		if (filename[6] != 'N')
		{
			filename[6] = 'N';
			filename[7] = 'E';
			filename[8] = '0';
			filename[9] = '0';

			int index = std::find(filesnewset.begin(), filesnewset.end(), filename) - filesnewset.begin();
			if (index >= 0 && index < filesnewset.size())
			{
				std::string filename1 = path + filesnewset[i];
				std::string filename2 = path + filesnewset[index];
				std::string muimagename = filesnewset[i];
				muimagename.replace(muimagename.size() - 2, 2, ".bmp");
				muimagename = imagepath + muimagename;
				CString file = muimagename.c_str();
				BOOL flag = PathFileExists(file);

				//if the file exist,then skip
				if (!flag)
				{
					GenerateQuasiConformalMuImage_CDT_PRE(filename2.c_str(), filename1.c_str(), muimagename.c_str());
					return;
				}
			}
		}
	}
}
void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicPreCdt()
{
	//Pre
	OnBnClickedButtonDataanalysisDataanalysisOptimalharmonic_CDT_Debug();

	//Set
	iternum_global = 0;

	CMesh3d* pSurface1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pSurface2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);

	pSurface1->m_showFace = 0;
	pSurface1->m_showEdge = 1;
	pSurface1->m_showEdgeIndex = 0;
	pSurface1->m_showLandmarkVertexIndex = 0;

	pSurface2->m_showFace = 0;
	pSurface2->m_showEdgeIndex = 0;
	pSurface2->m_showLandmarkVertexIndex = 0;

	pDoc->UpdateAllViews(NULL);
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsCdt()
{
	//Add the selected vertices to the fix array
	AddLandmarkEndPointsToFixArray(pSoureceSurface_Openmesh, pTargetSurface_Openmesh);

	VPROP_VERTEX_UV vprop_vertex_uv;
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	EPROP_EDGE_SELECTED_FLAG eprop_selectedflag;

	//Vertex UV
	if (!pSoureceSurface_Openmesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSoureceSurface_Openmesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	//Vertex selected flag;
	if (!pSoureceSurface_Openmesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pSoureceSurface_Openmesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Edge selected flag;
	if (!pSoureceSurface_Openmesh->pmesh->get_property_handle(eprop_selectedflag, PROP_EDGE_SELECTED))
		pSoureceSurface_Openmesh->pmesh->add_property(eprop_selectedflag, PROP_EDGE_SELECTED);

	//while (1)
	{
		pHarmonicMapping->Compute_Linear_Equations_WithStraighteningConstraints_FixEndPoints(pSoureceSurface_Openmesh);

		//CMesh3d_OpenMesh* ptarget_para_Openmesh_Intermediate = pCurrentMesh->copy();
		if (!pHarmonicMapping->IsMeshUVFlip(pSoureceSurface_Openmesh, 0))
		{
			AddResultantMesh(pSoureceSurface_Openmesh);

			//Save
			SaveResultantMesh(pSoureceSurface_Openmesh);
			return;
		}
		//Search the step
		//double step;
		//int edgeindex;
		step_global = pDynamicQuasiconformal->SearchSmallestStep_CDT(pSoureceSurface_Openmesh, edgeindex_global);

		MyMesh::EdgeHandle eh = pSoureceSurface_Openmesh->pmesh->edge_handle(edgeindex_global);

		MyMesh::HalfedgeHandle hh0 = pSoureceSurface_Openmesh->pmesh->halfedge_handle(eh, 0);
		MyMesh::HalfedgeHandle hh1 = pSoureceSurface_Openmesh->pmesh->halfedge_handle(eh, 1);

		vindex1_global = pSoureceSurface_Openmesh->pmesh->to_vertex_handle(hh0).idx();
		vindex2_global = pSoureceSurface_Openmesh->pmesh->from_vertex_handle(hh0).idx();

		for (MyMesh::EdgeIter e_it = pSoureceSurface_Openmesh->pmesh->edges_begin(); e_it != pSoureceSurface_Openmesh->pmesh->edges_end(); ++e_it)
		{
			pSoureceSurface_Openmesh->pmesh->property(eprop_selectedflag, e_it.handle()) = 0;
		}

		for (MyMesh::VertexIter v_it = pSoureceSurface_Openmesh->pmesh->vertices_begin(); v_it != pSoureceSurface_Openmesh->pmesh->vertices_end(); ++v_it)
		{
			pSoureceSurface_Openmesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
		}

		pSoureceSurface_Openmesh->pmesh->property(eprop_selectedflag, pSoureceSurface_Openmesh->pmesh->edge_handle(edgeindex_global)) = 1;

		pSoureceSurface_Openmesh->pmesh->property(vprop_selectedflag, pSoureceSurface_Openmesh->pmesh->vertex_handle(vindex1_global)) = 1;
		pSoureceSurface_Openmesh->pmesh->property(vprop_selectedflag, pSoureceSurface_Openmesh->pmesh->vertex_handle(vindex2_global)) = 1;

		if (pDoc->m_SceneGraph.NbObject() == 4)
		{
			UpdateResultantMesh_Debug(pSoureceSurface_Openmesh);

			CMesh3d_OpenMesh* pOpenMesh1 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
			CMesh3d_OpenMesh* pOpenMesh2 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);

			OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh1, vindex1_global);
			OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh2, vindex1_global);
		}
		else
		{
			AddResultantMesh_Debug(pSoureceSurface_Openmesh);

			//Zoom
			CMesh3d_OpenMesh* pOpenMesh1 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
			CMesh3d_OpenMesh* pOpenMesh2 = (CMesh3d_OpenMesh*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 2);

			OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh1, vindex1_global);
			OnBnClickedButtonZoomCenteredAtvertex(pOpenMesh2, vindex1_global);

			pOpenMesh1->m_showFace = 0;
			pOpenMesh1->m_showEdge = 1;
			pOpenMesh1->m_showSelectedVertex = 1;
			pOpenMesh1->m_showSelectedEdge = 1;

			pOpenMesh2->m_showFace = 0;
			pOpenMesh2->m_showEdge = 0;
			pOpenMesh2->m_showSelectedVertex = 0;
			pOpenMesh2->m_showSelectedEdge = 0;
		}

		char message[1000];
		sprintf(message, "Iteration Num:%d, Edge Index:%d, Vertex Index(%d,%d)", iternum_global, edgeindex_global, vindex1_global, vindex2_global);
		std::string str = message;
		AfxGetMainWnd()->SetWindowText((str.c_str()));

		//pCurrentMesh = ptarget_para_Openmesh_Intermediate;
		iternum_global++;
	}
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsEdgeswapCdt()
{
	std::vector<int> m_ArrayEdgeId;

	MeshMapping* pMeshMapping = new MeshMapping();
	//pMeshMapping->pStartMesh = pCurrentMesh;
	pMeshMapping->m_IsEdgeSwap = 1;
	pMeshMapping->m_SwapEdgeEndPonits.first = vindex1_global;
	pMeshMapping->m_SwapEdgeEndPonits.second = vindex2_global;
	pDynamicQuasiconformal->m_ArrayMeshMappingSequences.push_back(pMeshMapping);

	//edgeswap
	pHarmonicMapping->SwapAdjacentTriangles(pSoureceSurface_Openmesh, edgeindex_global, m_ArrayEdgeId);
	pSoureceSurface_Openmesh->pmesh->garbage_collection();

	UpdateResultantMesh_Debug(pSoureceSurface_Openmesh);
	pDoc->UpdateAllViews(NULL);
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicAfterCdt()
{
	// TODO: Add your control notification handler code here
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicCdt()
{
	// TODO: Add your control notification handler code here
	std::ifstream infile("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\filelist.txt");
	char message[1000];
	std::string str;
	while (!infile.eof())
	{
		infile >> message;
		if (strlen(message) == 0)
			break;
		str = message;
		filesnewset.push_back(str);
	}
	infile.close();

	std::string filename;
	std::string path("E:\\BU_Data\\BUdata_fixed_From_ZengStudent\\BU_DATA\\BU_DATA\\IJCV\\normalizedpara\\normalized_normalized_onlyrotate_eyeshorizontal\\normalized\\");
	std::string imagepath("E:\\BU_Data\\muimages\\");
	for (int i = 0; i < filesnewset.size(); i++)
	{
		//
		//if (i >= 300)
		//	return;

		filename = filesnewset[i];

		if (filename[6] != 'N')
		{
			filename[6] = 'N';
			filename[7] = 'E';
			filename[8] = '0';
			filename[9] = '0';

			int index = std::find(filesnewset.begin(), filesnewset.end(), filename) - filesnewset.begin();
			if (index >= 0 && index < filesnewset.size())
			{
				std::string filename1 = path + filesnewset[i];
				std::string filename2 = path + filesnewset[index];
				std::string muimagename = filesnewset[i];
				muimagename.replace(muimagename.size() - 2, 2, ".bmp");
				muimagename = imagepath + muimagename;
				CString file = muimagename.c_str();
				BOOL flag = PathFileExists(file);

				//if the file exist,then skip
				if (!flag)
					GenerateQuasiConformalMuImage_CDT(filename2.c_str(), filename1.c_str(), muimagename.c_str());
			}
		}
	}
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicPreCdt2()
{
	//first Make the original mesh to be a CDT
	pDynamicQuasiconformal->GetCDTMesh_Simplify(pSoureceSurface_Openmesh);
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicIterationsEdgeswapCdtMove()
{
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex UV
	if (!pSoureceSurface_Openmesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSoureceSurface_Openmesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uvvalues;
	uvvalues.resize(2);
	OpenMesh::Vec3d targetpoint, initialpoint;
	for (MyMesh::VertexIter v_it = pSoureceSurface_Openmesh->pmesh->vertices_begin(); v_it != pSoureceSurface_Openmesh->pmesh->vertices_end(); ++v_it)
	{
		initialpoint = pSoureceSurface_Openmesh->pmesh->point(v_it.handle());
		uvvalues = pSoureceSurface_Openmesh->pmesh->property(vprop_vertex_uv, v_it.handle());
		targetpoint[0] = initialpoint[0] * (1 - step_global) + uvvalues[0] * step_global;
		targetpoint[1] = initialpoint[1] * (1 - step_global) + uvvalues[1] * step_global;
		targetpoint[2] = 0;
		pSoureceSurface_Openmesh->pmesh->set_point(v_it.handle(), targetpoint);
	}

	/*for (MyMesh::VertexIter v_it = ptarget_para_Openmesh_Intermediate->pmesh->vertices_begin(); v_it != ptarget_para_Openmesh_Intermediate->pmesh->vertices_end(); ++v_it)
	{
	initialpoint = ptarget_para_Openmesh_Intermediate->pmesh->point(v_it.handle());

	uvvalues[0] = initialpoint[0];
	uvvalues[1] = initialpoint[1];
	pCurrentMesh->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
	}*/

	//Add to the mesh mapping array
	MeshMapping* pMeshMapping = new MeshMapping();
	//pMeshMapping->pStartMesh = pCurrentMesh;
	pMeshMapping->m_IsEdgeSwap = 0;
	pMeshMapping->step = step_global;
	pDynamicQuasiconformal->m_ArrayMeshMappingSequences.push_back(pMeshMapping);

	UpdateResultantMesh_Debug(pSoureceSurface_Openmesh);
	pDoc->UpdateAllViews(NULL);
}


void Actions_DataAnalysis::OnBnClickedButtonDataanalysisDataanalysisOptimalharmonicCdtExample1()
{
	// TODO: Add your control notification handler code here
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\dynamicquasiconformal\\example1\\3Dface.obj");
	CMesh3d* p3DSurface_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	CMesh3d_OpenMesh* pSourceMesh = p3DSurface_Mesh3d->Convert_OpenMesh();

	pDoc->OnOpenDocument("D:\\papers\\code\\data\\dynamicquasiconformal\\example1\\3Dface_targetpara.obj");
	CMesh3d* pconvex_para_Mesh3d = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	CMesh3d_OpenMesh* pTargetMesh = pconvex_para_Mesh3d->Convert_OpenMesh();

	//return;

	//Fix Boundary
	FixBoundaryVertices(pSourceMesh, pTargetMesh);

	//
	//first Make the original mesh to be a CDT
	pDynamicQuasiconformal->GetCDTMesh_Simplify(pSourceMesh);

	//harmonic map iterations
	int flag = HarmonicIterationsbyEdgeSwap_CDT(pSourceMesh);

	if (flag)
	{
		//
		pHarmonicMapping->SetZeroMu(pSourceMesh);

		//
		pHarmonicMapping->Compute_Mu(pSourceMesh);

		//CFramebuffer* buffer = new CFramebuffer();
		//buffer->begin();
		//PlotMeshMu(pSourceMesh);
		//buffer->saveFrameBuff("D:\\1.bmp");
		//buffer->end();
		//delete buffer;
	}

	//
	pDoc->m_SceneGraph.Free();

}
void Actions_DataAnalysis::FixBoundaryVertices(CMesh3d_OpenMesh* pSourceMesh, CMesh3d_OpenMesh* pTargetMesh)
{
	VPROP_VERTEX_SELECTED_FLAG vprop_selectedflag;
	VPROP_VERTEX_UV vprop_vertex_uv;

	//Vertex selected flag;
	if (!pSourceMesh->pmesh->get_property_handle(vprop_selectedflag, PROP_VERTEX_SELECTED))
		pSourceMesh->pmesh->add_property(vprop_selectedflag, PROP_VERTEX_SELECTED);

	//Vertex UV
	if (!pSourceMesh->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV))
		pSourceMesh->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV);

	std::vector<double> uvvalues;
	uvvalues.resize(2);
	OpenMesh::Vec3d v0, v0_source, v0_target;

	//initialization
	for (MyMesh::VertexIter v_it = pSourceMesh->pmesh->vertices_begin(); v_it != pSourceMesh->pmesh->vertices_end(); ++v_it)
	{
		pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 0;
	}

	//fix the boundary vertices at its original positions
	for (MyMesh::VertexIter v_it = pSourceMesh->pmesh->vertices_begin(); v_it != pSourceMesh->pmesh->vertices_end(); ++v_it)
	{
		int flag = pSourceMesh->pmesh->is_boundary(v_it.handle());
		if (flag)
		{
			pSourceMesh->pmesh->property(vprop_selectedflag, v_it.handle()) = 1;

			pSourceMesh->m_FixVerticesArray.push_back(v_it.handle().idx());

			v0_target = pTargetMesh->pmesh->point(v_it.handle());

			uvvalues[0] = v0_target[0];
			uvvalues[1] = v0_target[1];
			pSourceMesh->pmesh->property(vprop_vertex_uv, v_it.handle()) = uvvalues;
		}
	}
}