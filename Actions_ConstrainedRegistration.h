#pragma once


// Actions_ConstrainedRegistration dialog
class CMesh3d_OpenMesh;
class CMesh3d;
class GeoMapLandMark;
class Actions_ConstrainedRegistration;
class Actions_ConstrainedRegistration : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_ConstrainedRegistration)
public:
	std::vector<int> m_ArrayFeatureArrayPoints;
public:
	Actions_ConstrainedRegistration();
	virtual ~Actions_ConstrainedRegistration();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ConstrainedRegistration };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	//
	void CopyImageColorToTargetMesh(CObject3d* pMesh);

	//
	void LoadLandmarkOnParameterization_OnBrain_1(CMesh3d_OpenMesh* pMesh1, CMesh3d_OpenMesh* pMesh2);

	//
	void LoadBrainModel_1(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2, int Type_Parameterdomain, int HasLandmark);

	//
	void ComputeVertexMu_Registration(CMesh3d_OpenMesh* pMesh3D, CMesh3d_OpenMesh* pMesh3D_UV);

	//
	void OnBnClickedConstrainedregistrationTargetvertexcolorMu_Registration();

	//
	void OnBnClickedConstrainedregistrationBothMeshAreaStretch_Registration();

	//
	double Length(Vector3D mu);

	//
	void OnBnClickedConstrainedregistrationBothMeshAreaStretch();

	//
	void Bijective2DMapping_Initialize_Budata1(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2);

	//
	void Bijective2DMapping_Budata(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2);

	//
	void LoadFeaturePoints(CString FileName);

	//
	void OnBnClickedLoadexample2Straightening_Constrainedregistration();

	//
	void Bijective2DMapping_Initialize_Budata(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2);

	//
	void LoadBudataModel(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2, int Type_Parameterdomain, int HasLandmark);

	//
	double GetFaceArea(CMesh3d_OpenMesh* pMesh3D, MyMesh::FaceHandle fh);

	//
	void ChangeVertexNearBoundaryMu(CMesh3d_OpenMesh* pMesh3D);

	//
	void ComputeVertexAreaStretch_Registration(CMesh3d_OpenMesh* pMesh3D, CMesh3d_OpenMesh* pMesh3D_uv);

	//
	void ComputeVertexAreaStretch(CMesh3d_OpenMesh* pMesh3D);

	//
	void ComputeVertexMu(CMesh3d_OpenMesh* pMeshCopy);

	//
	double Length(std::vector<double> mu);
	//
	void SetVertexColorByMu(CMesh3d* pMesh, CMesh3d* pMesh_UV, CMesh3d_OpenMesh* pMeshCopy);

	//
	void LoadSophiaSearchStepResults();

	//
	void SetPointsUV(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2, std::vector<MyMesh::VertexHandle> &m_ArrarBoundaryVertices);

	//
	void GetVerticesbetwwenTwoPoints(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2, std::vector<MyMesh::VertexHandle> &m_ArrarBoundaryVertices, MyMesh::VertexHandle m_startVertexHandle, MyMesh::VertexHandle m_endVertexHandle);

	//
	void LoadSophiaStraighteningModel(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2);

	//
	void Bijective2DMapping_Initialize_Sophia(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2);

	//
	void Bijective2DMapping_Sophia(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2);

	//
	void LoadBrainModel(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2, int Type_Parameterdomain, int HasLandmark);

	//
	void LoadLandmarkOnParameterization_OnBrain(CMesh3d_OpenMesh* pMesh1, CMesh3d_OpenMesh* pMesh2);

	//
	void LoadLandmarkOnParameterization(int index1, int index2);

	//
	void LoadSophiaModel(CMesh3d_OpenMesh* &pOpenMesh1, CMesh3d_OpenMesh* &pOpenMesh2, int Type_Parameterdomain, int HasLandmark);

	//
	CObject3d* pMesh, *pMesh_UV;

	//
	void ReverseLandmark(GeoMapLandMark* pLandmark);

	//
	void RotateMesh(CMesh3d* pMesh);

	//
	void Bijective2DMapping_Initialize(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2);

	//
	void Bijective2DMapping(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2);

	//
	void MapTo2D(CMesh3d_OpenMesh* pOpenMesh1, CMesh3d_OpenMesh* pOpenMesh2, int boundarytype = 0);

	//
	void CopyMeshVertexColor(CObject3d* pMesh, CObject3d* pMesh_UV);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConstrainedregistrationLoadface();
	afx_msg void OnBnClickedConstrainedregistrationConstrainedharmonic();
	afx_msg void OnBnClickedConstrainedregistrationLoadface2();
	afx_msg void OnBnClickedConstrainedregistrationLoadsophia();
	afx_msg void OnBnClickedConstrainedregistrationLoadsophia2();
	afx_msg void OnBnClickedConstrainedregistrationChangelandmarkindex();
	afx_msg void OnBnClickedButtonSetmeshuvMesh();
	afx_msg void OnBnClickedButtonSetmeshuvUv();
	afx_msg void OnBnClickedConstrainedregistrationSetmeshuv();
	afx_msg void OnBnClickedConstrainedregistrationLoadsophiaHolomorphic();
	afx_msg void OnBnClickedConstrainedregistrationConformalparameterizationQuad();
	afx_msg void OnBnClickedConstrainedregistrationCopyvertexcolor();
	afx_msg void OnBnClickedConstrainedregistrationConformalparameterizationCircle();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircle();
	afx_msg void OnBnClickedConstrainedregistrationLoadbrain();
	afx_msg void OnBnClickedConstrainedregistrationBrainConformalparameterizationCircle();
	afx_msg void OnBnClickedConstrainedregistrationExtractboundaryedges();
	afx_msg void OnBnClickedConstrainedregistrationConstrainedharmonicSophia();
	afx_msg void OnBnClickedConstrainedregistrationLoadsophiastraightening();
	afx_msg void OnBnClickedConstrainedregistrationLoadsophiaSearchstep();
	afx_msg void OnBnClickedConstrainedregistrationCopyvertexcolorDifferenttopology();
	afx_msg void OnBnClickedConstrainedregistrationLoadsophiaInitialregistrationHarmonic();
	afx_msg void OnBnClickedConstrainedregistrationCopyuvDifferenttopology();
	afx_msg void OnBnClickedConstrainedregistrationSetreferencemesh();
	afx_msg void OnBnClickedConstrainedregistrationLoadbrain2();
	afx_msg void OnBnClickedConstrainedregistrationCopyuvDifferenttopology2();
	afx_msg void OnBnClickedConstrainedregistrationTargetvertexcolorMu();
	afx_msg void OnBnClickedConstrainedregistrationLoadsophiaConformalcircle();
	afx_msg void OnBnClickedConstrainedregistrationMaprectangletocircle();
	afx_msg void OnBnClickedConstrainedregistrationConformalparameterizationCircleHolomorphic();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleHolomorphic();
	afx_msg void OnBnClickedConstrainedregistrationLoadexample2Budata();
	afx_msg void OnBnClickedConstrainedregistrationLoadexample2Holomorphic();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningRectangleHolomorphicBudata();
	afx_msg void OnBnClickedConstrainedregistrationLoadexample2Straightening();
	afx_msg void OnBnClickedConstrainedregistrationConstrainedharmonicBudata();
	afx_msg void OnBnClickedConstrainedregistrationConstrainedharmonicBudataBijectiveregistration();
	afx_msg void OnBnClickedConstrainedregistrationLoadexample2RegistrationFlip();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataAn();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataDi();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataFe();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataHa();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataNe();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataSa();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataSu();
	afx_msg void OnBnClickedConstrainedregistrationLoadexample2Budata2();
	afx_msg void OnBnClickedConstrainedregistrationLoadfeaturepoints();
	afx_msg void OnBnClickedConstrainedregistrationLoadfeaturepointsLandmark();
	afx_msg void OnBnClickedConstrainedregistrationLoadexample2Budata3();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataAn2();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBudataAn3();
	afx_msg void OnBnClickedConstrainedregistrationConstrainedharmonicSophia2();
	afx_msg void OnBnClickedConstrainedregistrationLoadexample2Budata4();
	afx_msg void OnBnClickedConstrainedregistrationLoadexample2Budata5();
	afx_msg void OnBnClickedConstrainedregistrationConformalstraighteningCircleBrain();
	afx_msg void OnBnClickedConstrainedregistrationMeshnormalization();
	afx_msg void OnBnClickedConstrainedregistrationDeteletefeaturepoints();
	afx_msg void OnBnClickedConstrainedregistrationBrainConformalparameterizationCircle2();
	afx_msg void OnBnClickedConstrainedregistration1627brainstrainghtening();
	afx_msg void OnBnClickedConstrainedregistrationCopyimagetosourcemesh();
	afx_msg void OnBnClickedConstrainedregistrationSetnormalascolor();
	afx_msg void OnBnClickedConstrainedregistrationSetcurvaturelascolor();
	afx_msg void OnBnClickedConstrainedregistrationSetgausscurvaturelascolor();
	afx_msg void OnBnClickedConstrainedregistrationCopyimagetomesh();
	afx_msg void OnBnClickedConstrainedregistrationSmoothgausscurvature();
	afx_msg void OnBnClickedConstrainedregistrationDetectteetchline();
	afx_msg void OnBnClickedConstrainedregistrationSharpengausscurvature();
};
