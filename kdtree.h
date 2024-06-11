#pragma once
#include <vector>
#include "Point3D.h"
class KDTree_node;
class CMesh3d;
class CNurbsSuface;
class KDTree
{
public:
	Vector2D lower,upper;
	double maxDeviation; // the maximal deviation between the original mesh and the rectilinear mesh
	std::vector<int> nearbyEdges;

	KDTree(void);
	~KDTree(void);
	KDTree_node* proot;
	CMesh3d* pOriginalMesh,*pUniformGridMesh;
	CNurbsSuface* pNurbsSurface;
	std::vector<int> m_VertexIndexArray;
	std::vector<int> m_EdgeLandmarkIndexArray;
	std::vector<std::pair<int,int>> m_ArrayEdgeDistricts;

	std::pair<int,int> nearLandmarkindex;
	int nearLandmarkedgeindex;
	double minimaldistance;
	//
	void Collect_Points_NURBS(KDTree_node* pparentnode, std::vector<double> &m_ArrayFaceTessellationPointscoordiantes);

	//
	KDTree_node* ConstructionKdTree_NURBSSurface(double umin, double umax, double vmin, double vmax, int depth, KDTree_node* pparentnode);

	//
	int searchLocatedDistrict(Vector2D point);

	//
	void TraceKDTreeForEdges(Vector2D point,KDTree_node *pnode);

	//
	void SearchCandidateNearestEdges(Vector2D point,const std::vector<int>& m_ArrayEdges);

	//
	BOOL IsBoundingboxIntersection(Vector2D leftlower1,Vector2D rightupper1,Vector2D leftlower2,Vector2D rightupper2);

	//
	BOOL IsPointInBoundingBox(Vector2D point, Vector2D leftlower, Vector2D upperright);

	//
	double pointEdgeDist(Vector2D p,int edgeindex);

	//
	double pointEdgeDist(Vector2D p, Vector2D a, Vector2D b);

	//nearest edges
	void searchForNearestEdge(Vector2D point);

	//nearest edges
	int searchForEdges_(KDTree_node *pnode, Vector2D point, double lowerx, double upperx, double lowery, double uppery);

	//search method concerning the maximal deviations
	int search(CMesh3d *pOriginalMesh, CMesh3d *pUniformGridMesh, Vector2D point, const std::vector<int>& m_VertexIndexArray, const std::vector<std::pair<int,int>>& m_ArrayEdgeDistricts);

	//construct the kdtree recursively from the points
	KDTree_node* ConstructionKdTree_Points(std::vector<Vector2D> &m_ArrayPoints,int depth,KDTree_node* pparentnode = NULL);

	//construct the kdtree recursively from the edges
	KDTree_node* ConstructionKdTree_Edges(CMesh3d* pOriginalMesh,CMesh3d* pMesh,std::vector<std::vector<int>> &m_HorizontalSegments,std::vector<std::vector<int>> &m_VerticalSegments,int depth,KDTree_node* pparentnode,Vector2D lower,Vector2D upper,int maxdepth = 1e10);

	//Nearest Search
	Vector2D nearestSearch(KDTree_node* pnode,Vector2D point);

	//Generate Mesh3D
	CMesh3d* generateMesh3D_Points();

	//
	CMesh3d* generateMesh3D_Edges();

	//
	void AddMeshEdge_Points(CMesh3d* pMesh3d,KDTree_node* pnode,double lowerx,double lowery,double upperx,double uppery);

	//
	void AddMeshEdge_Edges(CMesh3d* pMesh3d,KDTree_node* pnode,double lowerx,double lowery,double upperx,double uppery);
};

class KDTree_node
{
public:
	Vector2D point,point_;

	//the bounding box
	Vector2D leftlower,rightupper;
	double val;
	std::vector<int> m_ArrayEdges;
	KDTree_node* leftchild,*rightchild,*parent;
	int axis;

	//store which state the sons of the leaf are located
	int leftsonlocatedstate,rightsonlocatedstate;
	KDTree_node();
};