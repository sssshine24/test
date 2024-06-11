#pragma once
#include "object3d.h"
#include "Array3d.h"
#include "Vertex3d.h"
#include "Face3d.h"
#include "Vector3d.h"
#include "Material.h"
#include "Transform.h"
#include "..\Point3D.h"	// Added by ClassView
#include "..\Dib.h"
#include "..\OpenMesh.h"
#define PROP_VERTEX_SELECTED std::string("vprop_VertexSeletedFlag") //Vertex Seleted Flag
#define PROP_VERTEX_INEQUAL_CONSTRAINT std::string("vprop_VertexInequalConstraint") //Vertex Unequal Constraint
#define PROP_EDGE_SELECTED std::string("eprop_EdgeSeletedFlag") //Vertex Seleted Flag
#define PROP_EDGE_CONSTRAINED std::string("eprop_EdgeConstrainedFlag") //Edge Constrained Flag
#define PROP_EDGE_LAPLACE_WEIGHT std::string("vprop_EdgeLaplaceWeight") //Edge weight Laplace
#define PROP_EDGE_LAPLACE_ENERGY std::string("vprop_EdgeLaplaceEnergy") //Edge Energy Laplace
#define PROP_FACE_SELECTED std::string("vprop_FaceSeletedFlag") //Vertex Seleted Flag
#define PROP_VERTEX_UV std::string("vprop_Vertex_UV") //Vertex UV 
#define PROP_VERTEX_NON_BOUNDARY_INDEX std::string("vprop_VertexNoneboundaryIndex") //Vertex non-boundary index
#define PROP_VERTEX_MU std::string("vprop_Vertex_MU") //Quasi conformal MU
#define PROP_VERTEX_DISPLACEMENT std::string("vprop_Vertex_DISPLACEMENT") //Quasi conformal MU
#define PROP_FACE_MU std::string("fprop_Face_MU") //Quasi conformal MU
#define PROP_FACE_INTERMEDIATEINFO std::string("fprop_Face_INTERINFO") //Intermediate information
#define PROP_VERTEX_ORIGINALLOCATION std::string("vprop_Vertex_OriginalLocation") //Vertex Original Location 
#define PROP_VERTEX_INTERMEDIATEINFO std::string("vprop_Vertex_INTERINFO") //Intermediate information
#define PROP_EDGE_INTERMEDIATEINFO std::string("eprop_Edge_INTERINFO") //Intermediate information
#define PROP_HEDGE_INTERMEDIATEINFO std::string("Hprop_Edge_INTERINFO") //Intermediate information
#define PROP_HEDGE_TRANSFORMATIONINFO std::string("Hprop_Edge_TRANSFORMATIONINFO") //Intermediate information

typedef OpenMesh::FPropHandleT< std::vector<double> > FPROP_FACE_INTERINFO; // Face Intermediate Information.
typedef OpenMesh::VPropHandleT< std::vector<int> > VPROP_VERTEX_INTERINFO; // Vertex Intermediate Information.
typedef OpenMesh::FPropHandleT< std::vector<double> > FPROP_FACE_MU; // Face MU.
typedef OpenMesh::VPropHandleT< std::vector<double> > VPROP_VERTEX_MU; // Vertex MU.
typedef OpenMesh::VPropHandleT< std::vector<double> > VPROP_VERTEX_ORIGINALLOCATION; // Vertex MU.
typedef OpenMesh::VPropHandleT< std::vector<double> > VPROP_VERTEX_DISPLACEMENT; // Vertex Displacement.
typedef OpenMesh::VPropHandleT< int > VPROP_VERTEX_SELECTED_FLAG; // vertex flag.
typedef OpenMesh::VPropHandleT< int > VPROP_VERTEX_INEQUAL_CONSTRAINT; // vertex unequal flag.
typedef OpenMesh::EPropHandleT< int > EPROP_EDGE_SELECTED_FLAG; // edge flag.
typedef OpenMesh::EPropHandleT< int > EPROP_EDGE_CONSTRAINED_FLAG; // constrained edge flag.
typedef OpenMesh::EPropHandleT< double > EPROP_EDGE_LAPLACE_WEIGHT; // edge weight.
typedef OpenMesh::EPropHandleT< double > EPROP_EDGE_LAPLACE_ENERGY; // edge energy.
typedef OpenMesh::EPropHandleT< std::vector<double> > EPROP_EDGE_INTERINFO; // edge Intermediate Information..
typedef OpenMesh::HPropHandleT< std::vector<double> > HPROP_EDGE_INTERINFO; // edge Intermediate Information..
typedef OpenMesh::FPropHandleT< int > FPROP_FACE_SELECTED_FLAG; // face flag.
typedef OpenMesh::VPropHandleT< std::vector<double> > VPROP_VERTEX_UV; // Vertex UV.
typedef OpenMesh::VPropHandleT< int > VPROP_VERTEX_NON_BOUNDARY_INDEX; // vertex flag.
typedef OpenMesh::HPropHandleT< double > HPROP_EDGE_TRANSFORMATIONINFO; // edge Transformation Information..
class CColorRamp;
class CTexture;
class GeoMapLandMark;
class CMesh3d_OpenMesh;
class Thread_Structure{
public:
	CMesh3d_OpenMesh* pMesh;
	double m_CurrentStep;
	double* m_ReturnResult;
};
class CMesh3d_OpenMesh :
	public CObject3d
{
public:
	MyMesh* pmesh;
	CMesh3d_OpenMesh* pBoundaryMeshReference;

	//int m_Show;
	int m_showPointsOnSurface;
	int hasregion;
	int m_showFeatureVertexIndex;
	int m_showFeatureVertex;
	int m_showVertex;
	int m_showEdge;
	int m_showEdgeIndex;
	int m_showFace;
	int m_showVertexIndex;
	int m_showFaceIndex;
	int m_showSharpEdge;
	int m_showLandmarkEdge;
	int m_showSharpEdgeIndex;
	int m_showSelectedVertex;
	int m_showSelectedEdge;
	int m_showSelectedSharpVertex;
	int m_showSelectedFace;
	int m_showVertexMUVectorField;
	int m_showVertexDisplacement;
	int m_showSelectedVertexIndex;

	int m_NormalBinding;
	int m_ColorBinding;
	int m_IndexTexture;
	int m_showVertexMUMou;
	int m_showVertexMUAngle;
	int m_showTriangleMU;
	int m_Initialization_Linear_Equaiton;
	std::vector<GeoMapLandMark*> m_LandMarkArray;
	std::vector<int> m_FixVerticesArray; //outervertices
	std::vector<double> m_FixVerticesArray_uv; //outervertices_uv
	std::vector<int> m_FixVerticesArray_Inner; //outervertices
	std::vector<double> m_FixVerticesArray_Inner_uv; //outervertices_uv
	std::vector<int> m_FixVerticesArray_Inner_Landmark;
	std::vector<std::pair<int,std::vector<double>>> m_ArrayConstraints;
	std::vector<std::pair<int,int>> m_ArraySharpEdges;
	std::vector<int> m_SelectedLandmarkIndexArray;
	//Set the colors
	CColor m_SharpeEdgeColor;
	CColor m_FaceColor;
	CColor m_EdgeColor;
public:
	CMesh3d_OpenMesh(void);
	~CMesh3d_OpenMesh(void);
	//
	void CopyGaussCurvature(CMesh3d* pMesh);

	//
	void CalculateGaussCurvature();

	//
	void CopyLaplaceOperator(CMesh3d* pMesh);

	//
	void CalculateLaplaceBeltramiOperator();

	//
	int AssignUVtoMeshPointLocation();

	//
	int GetEdgeHandleFromTwoVertices(int vindex1, int vindex2);

	//
	int ShowEdgeIndex();

	//
	int ShowFaceIndex();

	//
	void SaveOBJParameterDomainFile(char *filename);

	//Get the bounding box
	void ComputeBoundingBox(Point3D &lower, Point3D &upper);

	//
	void LoadOBJFile(char *filename);

	//
	void SaveOBJFile(char *filename);

	//
	void SaveMFile(char *filename);

	//
	int splitEdge(int index1, int index2);

	//Get Edge Handle Idx From its end points
	int GetEdgeHandleIdx(int vindex1, int vindex2);

	//
	double GetTriangleSignArea_2D(int vindex1, int vindex2, int vindex3);

	//
	void GetEdgeOppositeVertices(MyMesh::EdgeHandle &eh, int &vindex3, int &vindex4);

	//
	BOOL IsExistEdge(int vindex1, int vindex2);

	//
	BOOL IsTriangleDegenerate_Colinear_UV(int facehandleindex);

	//
	BOOL IsTriangleDegenerate_Colinear(int facehandleindex);

	//
	static double GetTriangleSignArea(Vector3D v1, Vector3D v2, Vector3D v3);

	//
	double GetTriangleSignArea(int facehandleindex);

	//
	double GetTriangleSignArea_UV(int facehandleindex);

	//
	double GetTriangleSignArea_Mesh_UV_Interpolate(int facehandleindex,double t);

	//
	void FindSwitchEdge(OpenMesh::FaceHandle fh, double t, int &vindex1, int &vindex2, int &edgeindex);

	//
	int ShowVertexIndex();

	//
	int ShowVertexDisplacement();

	//
	int ShowEdge();

	//
	int ShowVertexMUVectorField();

	//
	int ShowLandmarkEdge();

	//
	int ShowSelectedEdge();

	//
	int ShowSharpEdges();

	//
	CMesh3d_OpenMesh* copy();

	//
	CMesh3d* ConvertMesh3d();

	// Transform
	void SetTransform(CTransform &transform) { m_Transform.Copy(transform); }
	CTransform *GetTransform(void) { return &m_Transform; }

	//
	int glDraw();

	//
	int glBuildList();

	//
	int ShowSelectedVertex();

	//
	int ShowTriangleMU();

	//
	int ShowVertexMUMou();

	//
	int ShowVertexMUAngle();

	//
	int ShowVertexMU();

	//
	int ShowSelectedFace();

	//
	double GetTriangleAngle_UV(int facehandleindex, double &angle1, double &angle2, double &angle3);

	//
	double GetTriangleAngle(int facehandleindex, double &angle1, double &angle2, double &angle3);

	//
	double GetTriangleAngle(Vector3D v1, Vector3D v2, Vector3D v3, double &angle1, double &angle2, double &angle3);

	//
	std::vector<int> check_cutpath_valid(std::vector<std::pair<int, int>> m_ArraySharpEdges);

};
