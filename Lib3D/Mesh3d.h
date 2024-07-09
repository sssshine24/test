//********************************************
// Mesh3d.h
// class CMesh3d
//********************************************
// A mesh : simplicial complex
// + Array of vertex
// + Array of faces
// + Binding infos
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 15/01/98
// Modified : 07/07/98
//********************************************

#ifndef _MESH_3D_
#define _MESH_3D_

#include "Array3d.h"
#include "Vertex3d.h"
#include "Face3d.h"
#include "Vector3d.h"
#include "Material.h"
#include "Transform.h"
#include "..\Point3D.h"	// Added by ClassView
#include "..\Dib.h"
#include "..\GeoMapLandMark.h"
#include <fstream>
class CColorRamp;
class CTexture;
class CMesh3d_OpenMesh;
class CMesh3d;
class Shape_Polygon;
class CMesh3d : public CObject3d
{
friend class CParserVrml;
friend class CParseSmf;
friend class CParseTM;
public :
	//
	Shape_Polygon* pPolygon;
	//
	CMesh3d* pReferenceMesh;

	// Datas
	CArray3d<CVertex3d> m_ArrayVertex;
	CArray3d<CFace3d>   m_ArrayFace;

	//the Sharp edges for the m file
	std::vector<std::pair<int,int>> m_ArraySharpEdges;
	std::vector<int> m_ArraySharpEdgesFlag;
	std::vector<int> m_ArraySharpEdges_Isboundary;
	std::vector<int> m_ArraySharpEdges_IsDeleted;
	std::vector<int> m_ArraySharpVertexIndex;
	std::vector<int> m_ArrayFeatureVertexIndex;
	std::vector<int> m_ArraySelectedSharpEdgeIndex;
	std::vector<int> m_LandMarkEndPoints;
	std::vector<int> m_FixVerticesArray;
	std::vector<double> m_FixVerticesArray_uv;
	std::vector<int> m_FixVerticesArray_Inner; //
	std::vector<double> m_FixVerticesArray_Inner_uv; //
	std::vector<int> m_ArraySharpEndPoints;

	std::vector<std::vector<int>> m_ArrayFaceAreaArray;
	std::vector<double> m_ArrayTargetFaceArea;
	std::vector<std::vector<int>> m_2DArrayTargetFaceArea;
	std::vector<int>  m_FaceAreaDifference;
	std::vector<double> xinterval,yinterval;
	std::vector<std::vector<int>> m_ArrayFacePointsArray;
	std::vector<std::vector<int>> m_ArrayFaceEdgesArray;
	std::vector<double> m_ArrayFaceArea;
	std::vector<double> m_ArrayDifferenceFaceArea;
	std::vector<std::vector<std::pair<int,int>>> m_ArrayUniformCells;
	std::vector<double> m_ArrayDistrictDensity;
	std::vector<std::vector<double>> m_ArrayStatesColor;
	std::vector<double> m_ArrayStateRatio;
	std::vector<std::vector<double>> m_FillCells;
	std::vector<std::vector<double>> m_FillCellsColor;
	std::vector<std::vector<int>> m_FillCellColorIndex;
	std::vector<int> m_FillCellStyle;  //0 rectangle 1 circle
	std::vector<std::vector<int>> m_ArrayStates;
	std::vector<CString> m_ArrayDistrictName;
	std::vector<std::vector<int>> m_ArrayPuzzles;
	std::vector<int> m_ArrayAnchorPoints;
	std::vector<double> m_ArrayPointCoordinates;
	std::vector<std::vector<double>> m_ArraySharpEdges_Seperate;
	int m_CurrentPuzzleXPos, m_CurrentPuzzleYPos, m_CurrentPuzzleSize;
	int m_CurrentPuzzleXNum, m_CurrentPuzzleYNum;
	int m_currentfaceindex;
	int xintervelnum,yintervalnum;

	//Set the colors
	CColor m_SharpeEdgeColor;
	CColor m_FaceColor;
	CColor m_EdgeColor;

	//for uniform grid
	double baseinterval;
	//Merge Constraints
	std::vector<std::pair<int,int>> m_ArrayMergeConstraints;

	CString m_Name;

	BOOL hasvt;
	BOOL hasuv;
	BOOL hasregion;

	// Apparence & texture
	CMaterial m_Material; // material
	int m_NormalBinding;  // normals defined at vertices or faces ? (Gouraud)
	int m_ColorBinding;   // colors defined at vertices or faces ? (Gouraud)

	// Texture
	int m_IndexTexture;   // which texture in SceneGraph ? (-1 : no texture)
	float *m_pTextureCoordinate;
	int *m_pTextureCoordinateIndex;

	// OpenGL-specific
	//unsigned int m_ListOpenGL;
	//unsigned int m_ListDone;
	//int m_Modified;
	//int m_Show;

	BOOL m_SetVertexNormal;
	BOOL m_SetFaceNormal;
	BOOL m_SetFlagOnVertices_ArrayIndex;

	GLdouble   mView[16]; //model matrix.

	//test
	CVector3d  test;

public :
	int m_showFeatureVertexIndex;
	int m_showFeatureVertex;
	int m_showVertex;
	int m_showVertexIndex;
	int m_showFaceIndex;
	int m_showSharpEdgeIndex;
	int m_showEdge;
	int m_showEdgeIndex;
	int m_showFace;
	int m_showLandmarkEdge;
	int m_showSelectedLandmarkEdge;
	int m_showLandmarkVertex;
	int m_showSharpEdge;
	int m_showSelectedSharpEdge;
	int m_SetVertexFlagAsItsIndex;
	int m_showSelectedVertex;
	int m_showPointsOnSurface;
	int m_showSelectedEdge;
	int m_showSelectedVertexIndex;
	int m_showSelectedSharpVertex;
	int m_showSelectedFace;
	int m_showFaceArea;
	int m_showTargetFaceArea;
	int m_showDifferenceFaceArea;
	int m_showFaceXYIndex;
	int m_showCellXYIndex;
	int m_showLandmarkIndex;
	int m_showLandmarkVertexIndex;
	int m_showMergeConstraints;
	int m_showUniformGrid;
	int m_showDistrictIndex;
	int m_showJianZhi;
	int m_showStateName;
	int m_showPuzzleGraph;
	int m_showVertexMUMou;
	int m_showVertexMUAngle;
	int m_showTriangleMU;
	int m_showRectilinearGrid;
	int m_showSharpEdgeBaseNum;
	int m_showAnchorPoints;

	//
	int ShowInputPointsOnMesh();

	//
	int ComputeNewWeightedShortestPath(int vstartindex, int &vendindex, std::vector<int> &path, std::vector<int> &m_TargetVertex);

	//
	double GetWeightedEdgeLength(CVertex3d* pStartVertex, CVertex3d* pEndVertex);

	//
	int ComputeWeightedShortestPath(int vstartindex, int vendindex, std::vector<int> &path);

	//
	double CalculateMaximalGaussCurvature();

	//
	double CalculateMinimalGaussCurvature();

	//
	int SmoothGaussCurvature();

	//
	int SharpGaussCurvature();

	//
	int CalculateGaussCurvature();

	//
	double CalculateMaximalMeanCurvature();

	//
	double CalculateMinimalMeanCurvature();

	//
	static double TriangleArea(Vector3D v1, Vector3D v2, Vector3D v3);

	//
	int ShowFeaturePointSequenceIndex();

	//
	void SetFaceFlagAsItsIndex();

	//
	int ShowLandmarkVertexIndex();

	//
	void CopyVertexRegion(CMesh3d* pStartMesh);

	//
	int ShowEdgeIndex();

	//
	int PlotPolyominoes_world();

	//
	int plotPolyominoType_world(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile);

	//
	int ShowCurrentPuzzle_world();

	//
	int ShowCurrentPuzzle();

	//
	int ShowSharpEdgeBaseNum();

	//
	int PlotUniformCells_Base();

	//
	int ShowRectilinearGrid();

	//
	CFace3d* FindFace(CVertex3d* pVertex1, CVertex3d* pVertex2, CVertex3d* &pVertex3);

	//
	int ShowFixedVertices_Inner();

	//
	int ShowFeatureVertexIndex();

	//
	int ShowFeatureVertex();

	//
	int ShowSelectedVertexIndex();

	//
	int ShowFaceIndex();

	//
	int ShowAnchorPoints();

	//
	int ShowSelectedFaces();

	//
	int plotPolyominoType3(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile);

	//
	void ExportPovray(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, double x7, double y7, double x8, double y8, double x9, double y9, double x10, double y10, std::ofstream &file, std::ofstream &edgefile, int type);

	//
	void ExportPovray(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, double x7, double y7, double x8, double y8, std::ofstream &file, std::ofstream &edgefile, int type);

	//
	void ExportPovray(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, std::ofstream &file, std::ofstream &edgefile, int type);

	//
	void ExportPovray(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, std::ofstream &file, std::ofstream &edgefile, int type);

	//
	int plotPolyominoType2(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile);

	//
	int plotPolyominoType1(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile);

	//
	int PlotPolyominoes5();

	//
	int PlotPolyominoes4();

	//
	int PlotPolyominoes3();

	//
	int plotPolyominoType1(int type, int x, int y);

	//
	int plotPolyominoType(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile);

	//
	int plotPolyominoType(int type, int x, int y);

	//
	int PlotCell(int x, int y);

	//
	int PlotPolyominoes2();

	//
	int PlotPolyominoes1();

	//
	int ShowSelectedSharpVertex();

	//
	int ShowVertexMUMou();

	//
	int ShowVertexMUAngle();

	//
	int ShowTriangleMU();

	//
	int OutputUniformPovRay();

	//
	int IsStateAdjacent(int i,int j);

	//
	int IsFaceAdjacent(int i, int j);

	//
	void DrawCircle(double x,double y,double z,double radius);

	//
	int ShowPuzzleGraph_();

	//
	int ShowStateName();

	//
	int ShowJianZhi();

	//
	void PlotUniformCells();

	//
	void PlotCellsUsingColorTable();

	//
	void PlotCellsSeveralColors();

	//
	void PlotCellsContinuousColor();

	//
	int ShowFaceAreaDifference();

	//
	int ShowDistrictIndex();

	//
	int ShowCellXYIndex();

	//
	int ShowFaceXYIndex();

	//
	int ShowFaceArea();

	//
	int ShowTargetFaceArea();

	//
	int ShowFixedVertices();

	//
	int ShowSelectedVertex();

	//
	int ShowLandmarkVertex();

	//
	int ShowFace();

	//
	int ShowLandmarkEdge();

	//
	int ShowSelectedLandmarkEdge();

	//
	int ShowVertexIndex();

	//
	int ShowVertex();

	//
	int ShowLandmarkIndex();

	//
	int ShowSharpEdgeIndex();

	//
	int ShowSelectedEdges();

	//
	int ShowSharpEdges();

	//
	int ShowSharpEdges_Seperate();

	//
	int ShowEdges();

	//
	int showNURBScurves();

	//
	int ShowUniformGrid();

	//
	//int ShowPuzzleGraph();

	//
	void GetColorTable(std::vector<std::vector<double>>& m_ArrayColorTable);

	//
	BOOL IsEdgeOnBoundary(int vstartindex,int vendindex);

	void DrawCoordinateAxis();
	void ConstructTetrahedra();
	void DrawScence();
	// Constructor
	CMesh3d();
	virtual ~CMesh3d();

	// Datas
	void Free();
	virtual int GetType();
	int IsValid();
	void Copy(CMesh3d *pMesh);

	// Faces
	int NbFace() { return m_ArrayFace.GetSize(); }
	CFace3d *GetFace(int index) { return m_ArrayFace[index]; }
	void AddFace(CFace3d *pFace) { m_ArrayFace.Add(pFace); }
	void SetNbFace(int NbFace) { m_ArrayFace.SetSize(NbFace); }
	CArray3d<CFace3d> *GetArrayFace() { return &m_ArrayFace; }
	int Has(CFace3d *pFace) { return m_ArrayFace.Has(pFace); }
	int FindFace(CFace3d *pFace); // even in neighborign faces

	// Flags
	void SetFlagOnFaces(int flag);
	void SetFlagOnVertices(int flag);
	CVertex3d *GetFirstVertexWithFlag(int flag);
	int GetMaxFlagOnFaces();
	int FindSmallestFlagOnVerticesDiffThan(int flag,int *pFounded);
	int FindFlagOnVerticesDiffThan(int flag);
	CVertex3d *GetAnyVertexWithFlagRootPrefered(int flag);
	void SetFlagOnVertices_ArrayIndex();

	// Vertices
	int NbVertex() { return m_ArrayVertex.GetSize(); }
	void SetNbVertex(int NbVertex)  { m_ArrayVertex.SetSize(NbVertex); }
	CArray3d<CVertex3d> *GetArrayVertex() { return &m_ArrayVertex; }
	void AddVertex(CVertex3d *pVertex) { m_ArrayVertex.Add(pVertex); }
	CVertex3d *AddVertex_NODUPLICATE(double x, double y, double z);
	int DeleteVertex(CVertex3d *pVertex);
	int DeleteVertex(int index);
	int DeleteFace(CFace3d *pFace);
	int DeleteFace(int index);
	int DeleteFaceNbNeighbors(int NbNeighbor);
	CVertex3d *GetVertex(int index) {return m_ArrayVertex[index];}
	int Has(CVertex3d *pVertex) { return m_ArrayVertex.Has(pVertex); }

	// Edges
	float GetMeanLengthEdge();

	//Compute  Shortest Path using Dijkstra
	int ComputeShortestPath(int vstartindex,int vendindex,std::vector<int> &path);

	// Name
	CString GetName(void) { return m_Name; } 
	void SetName(CString &string) { m_Name = string; } 

	// I/O
	int WriteFile(CStdioFile &file);
	int WriteFileRaw(CFile &file);
	int  ReadFile(CFile &file);

	// Range
	void Range(int coord,float *min,float *max); 
	void Range(int coord,float min,float max); 
	void Offset(int coord,float offset);
	void Scale(int coord,float scale);
	void Move(float dx,float dy,float dz);

	// Predefined
	int GenerateMap(int line,int col,float min,float max);
	int GenerateBox(float dx,float dy,float dz);
	int GenerateMap(CTexture *pTexture,int width,int height,int FlagColor = 1);

	// OpenGL
	virtual int glBuildList();
	virtual int glDraw();
	void Show(int flag) { m_Show = flag; }

	// Debug
	void Trace();

	// Adjacency 
	int BuildAdjacency();

	// Processing
	int IndexFrom(CFace3d *pFace);
	int IndexFrom(CVertex3d *pVertex);

	// Modif
	void SetModified(int flag = 1) { m_Modified = flag; }
	int GetModified() { return m_Modified; }


	// Vertex removal
	int VertexRemoval(CVertex3d *pV);
	int VertexRemoval();

	// Search
	int FindVertex(CVertex3d *pVertex);

	// Normals
	int CalculateNormalPerVertex(void);
	int CalculateNormalPerFace(void);
	void SetNormalBinding(int type);
	int GetNormalBinding(void);
	CVectorSet3d *GetVectorSetNormalPerFace(void); // alloc on the global heap

	int CalculateLaplaceBeltramiOperator_NoArea();

	int CalculateLaplaceBeltramiOperator();

	// Adjacency
	void Rebuild();

	// Distance
	double SquareDistanceToVertex(CVertex3d *pVertex,CVertex3d **pVertexRef);
	double SquareDistanceToFace(CVertex3d *pVertex,CVertex3d *pVertexRef,CFace3d **ppFaceRef);

	// Sharp edges
	int ColorSharpEdge(float threshold,CColor &color);
	CVectorSet3d *GetSharpEdges(float threshold);

	// Strings
	CVectorSet3d *GetStringBetweenVertices();

	// Color
	void SetColorBinding(int type);
	void SetColor(CColor &color);
	int GetColorBinding(void);

	// Material
	CMaterial *GetMaterial() { return &m_Material; }
	void SetMaterial(CMaterial *pMaterial) { m_Material.Copy(pMaterial); }

	// Texture
	int GetTextureIndex() { return m_IndexTexture; }

	// Subdivision
	int SubdivisionLoop(int MoveOnBundary = 1); // Charles Loop (1987)
	int Subdivision(void); 
	float Alpha(int n);

	// Smoothing
	int Smooth(int MoveOnBundary = 1);

	// Coloring (curvature, compacity, etc...)
	void ColorCurvature(CColorRamp *pRamp);
	void ColorNormalSpace(CColorRamp *pRamp);
	void ColorCompacity(CColorRamp *pRamp);
	void ColorHeight(CColorRamp *pRamp);
	void ColorFacesFromFlagPastels();
	void ColorFacesFromFlagGrey();

	//
	void SetVertexFlagAsZero();

	//Set Vertex Flag As its Index
	void SetVertexFlagAsItsIndex();

	// Intersection
	int NearestIntersectionWithLine(CVertex3d *pV0,CVertex3d *pV1,CVertex3d *pVertexResult,
		                              CFace3d **ppFaceResult,int *pNbFaceVisited);
	// Area
	double GetMinArea(CFace3d **ppFace = NULL);
	double GetMeanArea();

	//Computer 3D position from 2D parameter point
	int ComputerPosition(double u,double v, Vector3D& resultvec);
	static double TriangleArea(double u0,double v0,double u1, double v1,double u2, double v2);
	static int JudeWhetherPointLocatedInTriangle(double u,double v,double u0,double v0,double u1,double v1,double u2,double v2);

	//save tm format file
	void SaveTMFile(char *filename);

	//
	void SaveBoundingBoxOBJFile(char *filename);

	//
	void SaveOBJFile(char *filename);

	//
	void SaveMFile(char* file_name);

	//Get the bounding box
	void ComputeBoundingBox(Point3D &lower, Point3D &upper);

	//
	void showMergeConstraints();

	//
	void Normalize();

	//compute the four corner points
	void GetFourCornerPoints(int &index00,int &indexm0,int &index0n,int &indexmn);

	//
	CMesh3d_OpenMesh* Convert_OpenMesh();
	//
	std::vector<GeoMapLandMark*> m_LandMarkArray;
	std::vector<GeoMapLandMark*> m_ArrayLandMarks_Original;
	std::vector<int> m_SelectedLandmarkIndexArray;
	//std::vector<int> m_newlandmarks_style;
	//std::vector<int> m_ArrayDeleteFlag;
	//std::vector<int> m_ArrayFixLandMarks; //The landmark can not be deleted.
	std::vector<std::vector<int>> m_ArrayPointConnectedLandmarks;
};
#endif // _MESH_3D_
