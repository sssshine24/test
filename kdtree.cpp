#include "stdafx.h"
#include "lib3d/Vertex3d.h"
#include "lib3d/Mesh3d.h"
#include "kdtree.h"
#include "Nurbs3D.h"
KDTree::KDTree(void)
{
	proot = NULL;
}

KDTree::~KDTree(void)
{
}

int KDTree::searchLocatedDistrict(Vector2D point)
{
	//first search the nearest edge
	searchForNearestEdge(point);

	//search the district
	GeoMapLandMark* pLandmark = pOriginalMesh->m_LandMarkArray[m_EdgeLandmarkIndexArray[nearLandmarkindex.first]];
	
	//std::pair<int, int> edge = pOriginalMesh->m_ArraySharpEdges[m_EdgeLandmarkIndexArray[nearLandmarkindex.first]];
	Vector2D a(pOriginalMesh->GetVertex(pLandmark->landmark_points[nearLandmarkindex.second])->x(), pOriginalMesh->GetVertex(pLandmark->landmark_points[nearLandmarkindex.second])->y());
	Vector2D b(pOriginalMesh->GetVertex(pLandmark->landmark_points[nearLandmarkindex.second + 1])->x(), pOriginalMesh->GetVertex(pLandmark->landmark_points[nearLandmarkindex.second + 1])->y());
	
	Vector2D dir = b - a;
	Vector3D dir3d(dir.x,dir.y,0);

	Vector2D dirpa = point - a;
	Vector3D dirpa3d(dirpa.x,dirpa.y,0);

	Vector3D vv = dir3d ^ dirpa3d;
	if(vv[2] > 0)
	{
		//left
		return m_ArrayEdgeDistricts[nearLandmarkindex.first].first;
	}else
	{
		return m_ArrayEdgeDistricts[nearLandmarkindex.first].second;
	}
}
void KDTree::searchForNearestEdge(Vector2D point)
{
	double boxHalfLength = maxDeviation * 2;
	KDTree_node* pNode = proot;
	while(pNode->leftchild || pNode->rightchild)
	{
		if(pNode->axis == 0)
		{
			if(point.x <= pNode->val)
				if(!pNode->leftchild)
					break;
				else
					pNode = pNode->leftchild;
			else
				if(!pNode->rightchild)
					break;
				else
					pNode = pNode->rightchild;
		}
		else
		{
			if(point.y <= pNode->val)
				if(!pNode->leftchild)
					break;
				else
					pNode = pNode->leftchild;
			else
				if(!pNode->rightchild)
					break;
				else
					pNode = pNode->rightchild;
		}
	}


	KDTree_node* pPreviousNode = pNode;
	pNode = pNode->parent;
	nearLandmarkindex.first = pNode->m_ArrayEdges[0];
	minimaldistance = pointEdgeDist(point,pNode->m_ArrayEdges[0]);
	nearLandmarkindex.second = nearLandmarkedgeindex;
	//Find the leaf node
	while(pNode)
	{
		if(pNode->axis == 0)
		{
			if(pPreviousNode == pNode->leftchild)
			{
				//whether to trace the right child
				if(pNode->val < point.x + boxHalfLength)
				{
					TraceKDTreeForEdges(point,pNode->rightchild);
				}
			}else
			if(pPreviousNode == pNode->rightchild)
			{
				//whether to trace the left child
				if(pNode->val > point.x - boxHalfLength)
				{
					TraceKDTreeForEdges(point,pNode->leftchild);
				}
			}
			if(pNode->val <= point.x + boxHalfLength || pNode->val >= point.x - boxHalfLength)
			{
				SearchCandidateNearestEdges(point,pNode->m_ArrayEdges);
			}
		}else
		if(pNode->axis == 1)
		{
			if(pPreviousNode == pNode->leftchild)
			{
				//whether to trace the right child
				if(pNode->val < point.y + boxHalfLength)
				{
					TraceKDTreeForEdges(point,pNode->rightchild);
				}
			}else
				if(pPreviousNode == pNode->rightchild)
				{
					//whether to trace the left child
					if(pNode->val > point.y - boxHalfLength)
					{
						TraceKDTreeForEdges(point,pNode->leftchild);
					}
				}
				if(pNode->val <= point.y + boxHalfLength || pNode->val >= point.y - boxHalfLength)
				{
					SearchCandidateNearestEdges(point,pNode->m_ArrayEdges);
				}
		}
		pPreviousNode = pNode;
		pNode = pNode->parent;
	}
}
void KDTree::TraceKDTreeForEdges(Vector2D point,KDTree_node *pnode)
{
	if(!pnode)
		return;

	double boxHalfLength = maxDeviation * 2;
	Vector2D leftlower1,upperright1,leftlower2,rightupper2;
	leftlower1.x = point.x - boxHalfLength;
	leftlower1.y = point.y - boxHalfLength;
	upperright1.x = point.x + boxHalfLength;
	upperright1.y = point.y + boxHalfLength;

	leftlower2 = pnode->leftlower;
	rightupper2 = pnode->rightupper;
	if(IsBoundingboxIntersection(leftlower1,upperright1,leftlower2,rightupper2))
	{
		if(pnode->axis == 0)
		{
			if(pnode->val >= point.x - boxHalfLength && pnode->val <= point.x + boxHalfLength)
			{
				SearchCandidateNearestEdges(point,pnode->m_ArrayEdges);
			}
		}else
		{
			if(pnode->val >= point.y - boxHalfLength && pnode->val <= point.y + boxHalfLength)
			{
				SearchCandidateNearestEdges(point,pnode->m_ArrayEdges);
			}
		}
		if(pnode->leftchild)
			TraceKDTreeForEdges(point,pnode->leftchild);
		if(pnode->rightchild)
			TraceKDTreeForEdges(point,pnode->rightchild);
	}
}
void KDTree::SearchCandidateNearestEdges(Vector2D point,const std::vector<int>& m_ArrayEdges)
{
	double boxHalfLength = maxDeviation * 2;	
	for(int i = 0; i < m_ArrayEdges.size(); i++)
	{
		int edgeindex = m_ArrayEdges[i];
		//std::pair<int, int> edge = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
		double distance = pointEdgeDist(point,edgeindex);
		if( distance < minimaldistance)
		{
			minimaldistance = distance;
			nearLandmarkindex.first = edgeindex;
			nearLandmarkindex.second = nearLandmarkedgeindex;
		}
	}
}
BOOL KDTree::IsPointInBoundingBox(Vector2D point, Vector2D leftlower, Vector2D upperright)
{
	if(point.x <= upperright.x && point.x >= leftlower.x && point.y <= upperright.y && point.y >= leftlower.y)
		return TRUE;
	else
		return FALSE;
}
BOOL KDTree::IsBoundingboxIntersection(Vector2D leftlower1,Vector2D rightupper1,Vector2D leftlower2,Vector2D rightupper2)
{
	Vector2D leftupper1(leftlower1.x,rightupper1.y),rightlower1(rightupper1.x,leftlower1.y);
	Vector2D leftupper2(leftlower2.x,rightupper2.y),rightlower2(rightupper2.x,leftlower2.y);

	if(IsPointInBoundingBox(leftlower1,leftlower2,rightupper2) || IsPointInBoundingBox(rightupper1,leftlower2,rightupper2) ||
		IsPointInBoundingBox(leftupper1,leftlower2,rightupper2) || IsPointInBoundingBox(rightlower1,leftlower2,rightupper2))
		return TRUE;

	if(IsPointInBoundingBox(leftlower2,leftlower1,rightupper1) || IsPointInBoundingBox(rightupper2,leftlower1,rightupper1) ||
		IsPointInBoundingBox(leftupper2,leftlower1,rightupper1) || IsPointInBoundingBox(rightlower2,leftlower1,rightupper1))
		return TRUE;
	
	return FALSE;
}

double KDTree::pointEdgeDist(Vector2D p,int edgeindex)
{
	int m_landmarkindex = m_EdgeLandmarkIndexArray[edgeindex];
	GeoMapLandMark* pLandmark = pOriginalMesh->m_LandMarkArray[m_landmarkindex];

	double mindis = 1e10;
	for(int i = 0; i < pLandmark->landmark_points.size() - 1; i++)
	{
		Vector2D a(pOriginalMesh->GetVertex(pLandmark->landmark_points[i])->x(), pOriginalMesh->GetVertex(pLandmark->landmark_points[i])->y());
		Vector2D b(pOriginalMesh->GetVertex(pLandmark->landmark_points[i + 1])->x(), pOriginalMesh->GetVertex(pLandmark->landmark_points[i + 1])->y());
		double dis = pointEdgeDist(p, a, b);
		if(dis < mindis)
		{
			mindis = dis;
			nearLandmarkedgeindex = i;
		}
	}
	//std::pair<int, int> edge = pUniformGridMesh->m_ArraySharpEdges[edgeindex];
	
	return mindis;
}
double KDTree::pointEdgeDist(Vector2D p, Vector2D a, Vector2D b)
{
	if((p - a) * (b - a) <= 0)
	{
		return Distance(p,a);
	}else
	if((p - b) * (a - b) <= 0)
	{
		return Distance(p,b);
	}else
	{
		Vector2D pa = p - a;
		Vector2D dir = b - a;
		dir.SetUnit();
		double lencos = pa * dir;
		double lensin = sqrt(pa * pa - SQR(lencos));
		return lensin;
	}
}
extern int maxdep;
KDTree_node* KDTree::ConstructionKdTree_Edges(CMesh3d* pOriginalMesh,CMesh3d* pMesh,std::vector<std::vector<int>> &m_HorizontalSegments,std::vector<std::vector<int>> &m_VerticalSegments,int depth,KDTree_node* pparentnode,Vector2D lower,Vector2D upper,int maxdepth)
{
	//if(maxdepth <= 0)
	//	return NULL;
	if (depth > maxdep)
		maxdep = depth;

	int axis = depth % 2;
	if(m_VerticalSegments.size() > 0 && m_HorizontalSegments.size() == 0)
		axis = 0;
	if(m_VerticalSegments.size() == 0 && m_HorizontalSegments.size() > 0)
		axis = 1;

	KDTree_node* pnode;

	pnode = new KDTree_node();
	pnode->axis = axis;
	pnode->leftlower = lower;
	pnode->rightupper = upper;
	pnode->parent = pparentnode;

	if(axis == 0)
	{
		if(m_VerticalSegments.size() == 0)
		{
			return NULL;
		}
		int N = m_VerticalSegments.size();
		std::pair<int,int> pair = pMesh->m_ArraySharpEdges[m_VerticalSegments[N / 2][0]];
		pnode->m_ArrayEdges = m_VerticalSegments[N / 2];
		double sum = 0;
		for(int i = 0; i < pnode->m_ArrayEdges.size(); i++)
		{
			int index = pnode->m_ArrayEdges[i];

			if(index == 1194)
			{
				int ij = 0;
			}
			std::pair<int,int> pair = pMesh->m_ArraySharpEdges[index];
			if(pnode->axis == 0)
			{
				sum += pOriginalMesh->GetVertex(m_VertexIndexArray[pair.first])->x();
				sum += pOriginalMesh->GetVertex(m_VertexIndexArray[pair.second])->x();
			}else
			{
				sum += pOriginalMesh->GetVertex(m_VertexIndexArray[pair.first])->y();
				sum += pOriginalMesh->GetVertex(m_VertexIndexArray[pair.second])->y();
			}
		}
		sum /= pnode->m_ArrayEdges.size() * 2;
		double value1 = sum;
		double value2 = pMesh->GetVertex(pair.first)->x();
		//if(value1 < lower.x || value1 > upper.x)
			//ASSERT(0);
			pnode->val = value2;
		//else
		//	pnode->val = value1;
		//pnode->val = pMesh->GetVertex(pair.first)->x();
		std::vector<std::vector<int>> m_VerticalSegments_Left,m_VerticalSegments_Right;
		m_VerticalSegments_Left.insert(m_VerticalSegments_Left.begin(),m_VerticalSegments.begin(),m_VerticalSegments.begin() + N / 2);
		m_VerticalSegments_Right.insert(m_VerticalSegments_Right.begin(),m_VerticalSegments.begin()+ N / 2 + 1,m_VerticalSegments.end());

		std::vector<std::vector<int>> m_HorizontalSegments_Left,m_HorizontalSegments_Right;
		//divide the horizontal edges
		for(int i = 0; i < m_HorizontalSegments.size(); i++)
		{
			std::vector<int> m_Array_left,m_Array_right;
			m_Array_left.clear();
			m_Array_right.clear();
			for(int j = 0; j < m_HorizontalSegments[i].size(); j++)
			{
				int index = m_HorizontalSegments[i][j];
				pair = pMesh->m_ArraySharpEdges[index];
				int vindex1,vindex2;
				vindex1 = pair.first;
				vindex2 = pair.second;
				if((pMesh->GetVertex(pair.first)->x() < pnode->val || pMesh->GetVertex(pair.second)->x() < pnode->val))
					m_Array_left.push_back(index);
				if((pMesh->GetVertex(pair.first)->x() > pnode->val || pMesh->GetVertex(pair.second)->x() > pnode->val))
					m_Array_right.push_back(index);
			}
			if(m_Array_left.size() >= 1)
				m_HorizontalSegments_Left.push_back(m_Array_left);
			if(m_Array_right.size() >= 1)
				m_HorizontalSegments_Right.push_back(m_Array_right);
		}

		/*for(int i = 0; i < m_HorizontalSegments.size(); i++)
		{
			std::vector<int> m_Array;
			m_Array.clear();
			for(int j = 0; j < m_HorizontalSegments[i].size(); j++)
			{
				int index = m_HorizontalSegments[i][j];
				pair = pMesh->m_ArraySharpEdges[index];
				int vindex1,vindex2;
				vindex1 = pair.first;
				vindex2 = pair.second;
				if((pMesh->GetVertex(pair.first)->x() > pnode->val || pMesh->GetVertex(pair.second)->x() > pnode->val))
					m_Array.push_back(index);
			}
			if(m_Array.size() >= 1)
				m_HorizontalSegments_Right.push_back(m_Array);
		}*/

		//left child
		Vector2D leftlower = lower,leftupper = upper;
		Vector2D rightlower = lower,rightupper = upper;
		leftupper.x = pnode->val;
		rightlower.x = pnode->val;
		pnode->leftchild = ConstructionKdTree_Edges(pOriginalMesh,pMesh,m_HorizontalSegments_Left,m_VerticalSegments_Left,depth + 1,pnode,leftlower,leftupper,maxdepth - 1);
		//right child
		pnode->rightchild = ConstructionKdTree_Edges(pOriginalMesh,pMesh,m_HorizontalSegments_Right,m_VerticalSegments_Right,depth + 1,pnode,rightlower,rightupper,maxdepth - 1);

		if(depth == 0)
			proot = pnode;
		return pnode;
	}else
	{
		if(m_HorizontalSegments.size() == 0)
			return NULL;

		int N = m_HorizontalSegments.size();
		std::pair<int,int> pair = pMesh->m_ArraySharpEdges[m_HorizontalSegments[N / 2][0]];
		pnode->m_ArrayEdges = m_HorizontalSegments[N / 2];
		double sum = 0;
		for(int i = 0; i < pnode->m_ArrayEdges.size(); i++)
		{
			int index = pnode->m_ArrayEdges[i];

			std::pair<int,int> pair = pMesh->m_ArraySharpEdges[index];
			if(pnode->axis == 0)
			{
				sum += pOriginalMesh->GetVertex(m_VertexIndexArray[pair.first])->x();
				sum += pOriginalMesh->GetVertex(m_VertexIndexArray[pair.second])->x();
			}else
			{
				sum += pOriginalMesh->GetVertex(m_VertexIndexArray[pair.first])->y();
				sum += pOriginalMesh->GetVertex(m_VertexIndexArray[pair.second])->y();
			}
		}
		sum /= pnode->m_ArrayEdges.size() * 2;
		double value1 = sum;
		double value2 = pMesh->GetVertex(pair.first)->y();
		//if(value1 < lower.y || value1 > upper.y)
			//ASSERT(0);
			pnode->val = value2;
		//else
		//	pnode->val = value1;
		//pnode->val = pMesh->GetVertex(pair.first)->y();
		std::vector<std::vector<int>> m_HorizontalSegments_Left,m_HorizontalSegments_Right;
		m_HorizontalSegments_Left.insert(m_HorizontalSegments_Left.begin(),m_HorizontalSegments.begin(),m_HorizontalSegments.begin() + N / 2);
		m_HorizontalSegments_Right.insert(m_HorizontalSegments_Right.begin(),m_HorizontalSegments.begin()+ N / 2 + 1,m_HorizontalSegments.end());

		std::vector<std::vector<int>> m_VerticalSegments_Left,m_VerticalSegments_Right;
		//divide the vertical edges
		for(int i = 0; i < m_VerticalSegments.size(); i++)
		{
			std::vector<int> m_Array_Left,m_Array_Right;
			m_Array_Left.clear();
			m_Array_Right.clear();
			for(int j = 0; j < m_VerticalSegments[i].size(); j++)
			{
				int index = m_VerticalSegments[i][j];
				pair = pMesh->m_ArraySharpEdges[index];
				int vindex1,vindex2;
				vindex1 = pair.first;
				vindex2 = pair.second;
				if((pMesh->GetVertex(pair.first)->y() < pnode->val || pMesh->GetVertex(pair.second)->y() < pnode->val))
					m_Array_Left.push_back(index);
				if((pMesh->GetVertex(pair.first)->y() > pnode->val || pMesh->GetVertex(pair.second)->y() > pnode->val))
					m_Array_Right.push_back(index);
			}
			if(m_Array_Left.size() >= 1)
				m_VerticalSegments_Left.push_back(m_Array_Left);
			if(m_Array_Right.size() >= 1)
				m_VerticalSegments_Right.push_back(m_Array_Right);

		}

		/*for(int i = 0; i < m_VerticalSegments.size(); i++)
		{
			std::vector<int> m_Array;
			m_Array.clear();
			for(int j = 0; j < m_VerticalSegments[i].size(); j++)
			{
				int index = m_VerticalSegments[i][j];
				pair = pMesh->m_ArraySharpEdges[index];
				int vindex1,vindex2;
				vindex1 = pair.first;
				vindex2 = pair.second;
				if((pMesh->GetVertex(pair.first)->y() > pnode->val || pMesh->GetVertex(pair.second)->y() > pnode->val))
					m_Array.push_back(index);
			}
			if(m_Array.size() >= 1)
				m_VerticalSegments_Right.push_back(m_Array);
		}*/

		Vector2D leftlower = lower,leftupper = upper;
		Vector2D rightlower = lower,rightupper = upper;
		leftupper.y = pnode->val;
		rightlower.y = pnode->val;

		//left child
		pnode->leftchild = ConstructionKdTree_Edges(pOriginalMesh,pMesh,m_HorizontalSegments_Left,m_VerticalSegments_Left,depth + 1,pnode,leftlower,leftupper,maxdepth - 1);
		//right child
		pnode->rightchild = ConstructionKdTree_Edges(pOriginalMesh,pMesh,m_HorizontalSegments_Right,m_VerticalSegments_Right,depth + 1,pnode,rightlower,rightupper,maxdepth - 1);

		if(depth == 0)
			proot = pnode;
		return pnode;
	}
}
void KDTree::Collect_Points_NURBS(KDTree_node* pnode,std::vector<double> &m_ArrayFaceTessellationPointscoordiantes)
{
	if (pnode->leftchild && pnode->rightchild)
	{
		if (pnode->point[0] > 1e-6 &&  pnode->point[0] < 1 - 1e-6 && pnode->point[1] > 1e-6 &&  pnode->point[1] < 1 - 1e-6)
		{
			m_ArrayFaceTessellationPointscoordiantes.push_back(pnode->point[0]);
			m_ArrayFaceTessellationPointscoordiantes.push_back(pnode->point[1]);
		}
		if (pnode->point_[0] > 1e-6 &&  pnode->point_[0] < 1 - 1e-6 && pnode->point_[1] > 1e-6 &&  pnode->point_[1] < 1 - 1e-6)
		{
			m_ArrayFaceTessellationPointscoordiantes.push_back(pnode->point_[0]);
			m_ArrayFaceTessellationPointscoordiantes.push_back(pnode->point_[1]);
		}
		//m_ArrayFaceTessellationPointscoordiantes.push_back(pnode->point_[0]);
		//m_ArrayFaceTessellationPointscoordiantes.push_back(pnode->point_[1]);
		Collect_Points_NURBS(pnode->leftchild, m_ArrayFaceTessellationPointscoordiantes);
		Collect_Points_NURBS(pnode->rightchild, m_ArrayFaceTessellationPointscoordiantes);
	}
	else
	{
		if (pnode->point[0] > 1e-6 &&  pnode->point[0] < 1 - 1e-6 && pnode->point[1] > 1e-6 &&  pnode->point[1] < 1 - 1e-6)
		{
			m_ArrayFaceTessellationPointscoordiantes.push_back(pnode->point[0]);
			m_ArrayFaceTessellationPointscoordiantes.push_back(pnode->point[1]);
		}
	}
}
KDTree_node* KDTree::ConstructionKdTree_NURBSSurface(double umin,double umax,double vmin,double vmax,int depth, KDTree_node* pparentnode)
{
	//select the axis based on depth so that axis cycles through all valid values
	int axis;

	double umiddle = (umin + umax) / 2;
	double vmiddle = (vmin + vmax) / 2;
	Vector3D vvmidlepoint = pNurbsSurface->ComputPosition(umiddle, vmiddle) - ZeroP3D;
	Point3D p1 = pNurbsSurface->ComputPosition(umiddle, vmin);
	Point3D p2 = pNurbsSurface->ComputPosition(umiddle, vmax);
	Point3D p3 = (p1 + p2) / 2;

	Point3D p4 = pNurbsSurface->ComputPosition(umin, vmiddle);
	Point3D p5 = pNurbsSurface->ComputPosition(umax, vmiddle);
	Point3D p6 = (p4 + p5) / 2;

	if (Distance(p3, vvmidlepoint) < Distance(p6, vvmidlepoint))
		axis = 0;
	else
		axis = 1;

	KDTree_node* pnode;
	pnode = new KDTree_node();

	if (depth == 0)
		proot = pnode;

	Vector3D p00 = pNurbsSurface->ComputPosition(umin, vmin) - ZeroP3D;
	Vector3D p10 = pNurbsSurface->ComputPosition(umax, vmin) - ZeroP3D;
	Vector3D p01 = pNurbsSurface->ComputPosition(umin, vmax) - ZeroP3D;
	Vector3D p11 = pNurbsSurface->ComputPosition(umax, vmax) - ZeroP3D;
	Vector3D pmiddle = (p00 + p10 + p01 + p11) / 4;
	Vector3D pmiddle_ = pNurbsSurface->ComputPosition((umin + umax) / 2, (vmin + vmax) / 2) - ZeroP3D;
	double dis = Distance(pmiddle, pmiddle_);
	if (dis < maxDeviation)
	{
		pnode->point.SetValue((umin + umax) / 2, (vmin + vmax) / 2);
		pnode->axis = -1;
		return pnode;
	}

	if (axis == 0)
	{
		pnode->point.SetValue(umiddle, vmin);
		pnode->point_.SetValue(umiddle, vmax);

		//left child
		pnode->leftchild = ConstructionKdTree_NURBSSurface(umin,umiddle,vmin,vmax, depth + 1, pnode);
		//right child
		pnode->rightchild = ConstructionKdTree_NURBSSurface(umiddle,umax,vmin,vmax, depth + 1, pnode);
	}
	else
	{
		pnode->point.SetValue(umin, vmiddle);
		pnode->point_.SetValue(umax, vmiddle);

		//left child
		pnode->leftchild = ConstructionKdTree_NURBSSurface(umin, umax, vmin, vmiddle, depth + 1, pnode);
		//right child
		pnode->rightchild = ConstructionKdTree_NURBSSurface(umin, umax, vmiddle, vmax, depth + 1, pnode);
	}

	pnode->parent = pparentnode;
	pnode->axis = axis;
	if (depth == 0)
		proot = pnode;
	return pnode;
}
KDTree_node* KDTree::ConstructionKdTree_Points(std::vector<Vector2D> &m_ArrayPoints,int depth,KDTree_node* pparentnode)
{
	//select the axis based on depth so that axis cycles through all valid values
	int axis = depth % 2;
	
	int N = m_ArrayPoints.size();
	if(N == 0)
		return NULL;
	KDTree_node* pnode;
	//sort point list and choose the median point as the pivot element
	if(N == 1)
	{
		pnode = new KDTree_node();
		pnode->point = m_ArrayPoints[0];
	}
	else
	{
		if(axis == 0)
		{
			struct sort_pred {
				bool operator()(const Vector2D &left, const Vector2D &right) 
				{
					return left.x < right.x;
				}
			};

			std::sort(m_ArrayPoints.begin(), m_ArrayPoints.end(), sort_pred());
		}else
		{
			struct sort_pred {
				bool operator()(const Vector2D &left, const Vector2D &right) 
				{
					return left.y < right.y;
				}
			};

			std::sort(m_ArrayPoints.begin(), m_ArrayPoints.end(), sort_pred());
		}

		pnode = new KDTree_node();
		pnode->point = m_ArrayPoints[N / 2];
		if(depth == 0)
			proot = pnode;
		std::vector<Vector2D> m_ArrayLeftPoints;
		std::vector<Vector2D> m_ArrayRightPoints;

		m_ArrayLeftPoints.insert(m_ArrayLeftPoints.begin(),m_ArrayPoints.begin(),m_ArrayPoints.begin() + N / 2);
		m_ArrayRightPoints.insert(m_ArrayRightPoints.begin(),m_ArrayPoints.begin() + N / 2 + 1,m_ArrayPoints.end());
		//left child
		pnode->leftchild = ConstructionKdTree_Points(m_ArrayLeftPoints,depth + 1,pnode);
		//right child
		pnode->rightchild = ConstructionKdTree_Points(m_ArrayRightPoints,depth + 1,pnode);
	}
	pnode->parent = pparentnode;
	pnode->axis = axis;
	if(depth == 0)
		proot = pnode;
	return pnode;
}

KDTree_node::KDTree_node()
{
	leftchild = rightchild = parent = NULL;
	leftsonlocatedstate = rightsonlocatedstate = -1;
}