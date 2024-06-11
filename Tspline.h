#pragma once
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
class  TsplinePreImage;
class TsplinePreImageVertex;
class Transformation;
#include "Lib3D\Object3d.h"
#include "lib3d\Transform.h"
#include "transformation.h"
#include "OpenMesh.h"
#include <iostream>
#define PROP_VERTEX_CORNER_LABEL std::string("vprop_VertexCornerLabel") //UV corner label
#define PROP_VERTEX_UV_COORDINATE_INDEX std::string("vprop_VertexUVCoordinateIndex") //UV Index
#define PROP_VERTEX_UV_COORDINATE std::string("vprop_VertexUVCoordinates") //UV
#define PROP_VERTEX_IMG_FLAG std::string("vprop_VertexImaginationFlag") //Imagination flag for the vertex
#define PROP_VERTEX_UKNOTS std::string("vprop_VertexUKnots") //U knot vector
#define PROP_VERTEX_VKNOTS std::string("vprop_VertexVKnots") //V knot vector
#define PROP_VERTEX_WEIGHT std::string("vprop_VertexWeight") //Vertex Weight
#define PROP_VERTEX_OFFENDING_FLAG std::string("vprop_OffendingVertexFlag") //Offending flag for the vertex
#define PROP_FACE_OFFENDING_FLAG std::string("fprop_OffendingFaceFlag") // offending face flag
#define PROP_FACE_UV_BOUNDINGBOX std::string("vprop_FaceUVBoundingBox") // offending face flag
#define PROP_VERTEX_TARGET_IN_CURRENT_INDEX std::string("vprop_VertexOfTargetInCurrentIndex") //
#define PROP_VERTEX_TARGET_IN_CURRENT_FLAG std::string("vprop_VertexOfTargetInCurrentFlag") // 0 vertex; 1 edge; 2 face
#define PROP_VERTEX_UV_PLOTCOORDINATE std::string("vprop_PlotVertexUVCoordinates") //Plot UV coordinates
typedef OpenMesh::VPropHandleT< std::vector<int> > VPROP_VERTEX_UV_COORDINATE_INDEX; // 2D coordinates (u,v) for vertex.
typedef OpenMesh::VPropHandleT< Vector2D > VPROP_VERTEX_UV_COORDINATE; // 2D coordinates (u,v) for vertex.
typedef OpenMesh::VPropHandleT< std::vector<double> > VPROP_VERTEX_UKNOTS; // U knot vector for the current vertex
typedef OpenMesh::VPropHandleT< std::vector<double> > VPROP_VERTEX_VKNOTS; // V knot vector for the current 
typedef OpenMesh::VPropHandleT< int > VPROP_VERTEX_IMG_FLAG; // Imageination flag for vertex.
typedef OpenMesh::VPropHandleT< int > VPROP_VERTEX_OFFENDING_FLAG; //OFFENDING FLAG
typedef OpenMesh::VPropHandleT< double > VPROP_VERTEX_WEIGHT; //Vertex weight
typedef OpenMesh::FPropHandleT< int > FPROP_FACE_OFFENDING_FLAG; // face flag.
typedef OpenMesh::FPropHandleT< std::vector<double> > FPROP_FACE_UK_BOUNDING_BOX; // Face bounding box
typedef OpenMesh::VPropHandleT< int > VPROP_VERTEX_TARGET_IN_CURRENT_INDEX; //
typedef OpenMesh::VPropHandleT< int > VPROP_VERTEX_TARGET_IN_CURRENT_FLAG; //
typedef OpenMesh::VPropHandleT< Vector2D > VPROP_VERTEX_UV_PLOTCOORDINATE; // 2D Plot coordinates (u,v) for vertex.
typedef OpenMesh::VPropHandleT< int > VPROP_VERTEX_CORNER_LABEL; // 1 the left bottom 2 right bottom 3 top left 4 top right

#ifndef DOXY_IGNORE_THIS
namespace OpenMesh {
	namespace IO {
		/*template <> struct binary< std::vector<int> > 
		{
			typedef std::vector< int > value_type;
			static const bool is_streamable = true;                       
			static size_t size_of(void)                                   
			{ 
				return IO::UnknownSize; 
			}                                   
			static size_t size_of(const value_type& _v)                   
			{ 
				return sizeof(int)*_v.size(); 
			} 
			static  size_t store(std::ostream& _os, const value_type& _v, bool _swap=false) 
			{ 
				size_t bytes=0;                                             
				if (_swap)                                                  
					bytes = std::accumulate( _v.begin(), _v.end(), bytes,FunctorStore<int>(_os,_swap) ); 
				else 
				{                                                      
					bytes = size_of(_v);                                       
					if(bytes > 0)                                               
					_os.write( reinterpret_cast<const char*>(&_v[0]), bytes ); 
				}                                                            
				return _os.good() ? bytes : 0;    
			}                                                             
			static size_t restore(std::istream& _is, value_type& _v, bool _swap=false) 
			{ 
				size_t bytes=0;                                             
				if ( _swap)                                                 
					bytes = std::accumulate( _v.begin(), _v.end(), 0,FunctorRestore<int>(_is, _swap) );           
				else                                                        
				{                                                           
					bytes = size_of(_v);                                      
					if(bytes > 0)                                              
					_is.read( reinterpret_cast<char*>(&_v[0]), bytes );       
				}                                                           
				return _is.good() ? bytes : 0;                              
			}                                                             
		};*/
		template <> struct binary< Vector2D > 
		{                           
			typedef Vector2D value_type;                                    
			static const bool is_streamable = true;                  
			static size_t size_of(const value_type&) 
			{ return sizeof(double) + sizeof(double); }   
			static size_t size_of(void) 
			{ return sizeof(double) + sizeof(double); }   
			static size_t store( std::ostream& _os, const value_type& _val, bool _swap=false) 
			{
				value_type tmp = _val;                                 
				if (_swap)
				{
					reverse_byte_order(tmp.x);  
					reverse_byte_order(tmp.y);
				}
				_os.write( (const char*)&tmp.x, sizeof(double) );    
				_os.write( (const char*)&tmp.y, sizeof(double) );  
				return _os.good() ? sizeof(double) * 2 : 0;            
			}                                                        
			static size_t restore( std::istream& _is, value_type& _val, bool _swap=false) 
			{                 
				_is.read( (char*)&_val.x, sizeof(double) );          
				_is.read( (char*)&_val.y, sizeof(double) );  
				if (_swap)
				{
					reverse_byte_order(_val.x); 
					reverse_byte_order(_val.y); 
				}
				return _is.good() ? sizeof(double) * 2 : 0;            
			}                                                        
		};

		/*template <> struct binary<Vector2D>
		{
			typedef Vector2D value_type;

			static const bool is_streamable = true;

			// return binary size of the value

			static size_t size_of(void) 
			{ 
				return sizeof(double) + sizeof(double); 
			}

			static size_t size_of(const value_type& _v) 
			{ 
				return size_of();
			}

			static size_t store(std::ostream& _os, const value_type& _v, bool _swap=false)
			{ 
				size_t   bytes;
				bytes  = IO::store( _os, _v.x, _swap );
				bytes  += IO::store( _os, _v.y, _swap );
				return _os.good() ? bytes : 0;
			}

			static size_t restore( std::istream& _is, value_type& _v, bool _swap=false)
			{ 
				size_t bytes;
				bytes  = IO::restore( _is, _v.x, _swap );
				bytes += IO::restore( _is, _v.y, _swap );
				return _is.good() ? bytes : 0;
			}
		};*/

	}}
#endif
class TsplineVertex3D{

public:
	//
	Point3D coordinate;

	//
	int udegree,vdegree;

	//
	std::vector<double> sknotarray;
	std::vector<double> tknotarray;

	//
	double ComputeBasis_S(double s);

	//
	double ComputeBasis_T(double t);

	//
	double GetSknot(int index);

	//
	double GetTknot(int index);

	//
	double ComputeBasis_S(int i, int p,double s);

	//
	double ComputeBasis_T(int i, int p,double t);
};
class CNurbsSuface;
class CMesh3d;
class Tspline : public CObject3d
{
public:
	Tspline(void);
	~Tspline(void);

	int            m_showSurface;
	int			   m_showControlPointsNetwork;
	int            m_showIsoParameterCurves;
	int			   m_showOptimalIsoParameterCurves;

	//the target NURBS vector
	std::vector<double> m_targetNurbsUKnots;
	std::vector<double> m_targetNurbsVKnots;

	//udegree and vdegree
	int udegree,vdegree;

	double umin,umax,vmin,vmax;

	int m_Show;
	CTransform	   m_Transform;

	int m_ListOpenGL;
	int m_ListDone,m_Modified;
	int m_ShowTexture;

	Transformation* ptransformation;

	TsplineMesh* pmesh;

	//
	void SetTransform(CTransform &transform) { m_Transform.Copy(transform); }

	//
	CTransform *GetTransform(void) { return &m_Transform; }

	//
	void GetBoundingBox(Vector3D &lower, Vector3D &upper);

	//
	void Read(std::ifstream &file,CString filename);

	//
	void Save(std::ofstream &file,CString filename);

	//
	void GenerateSamplingMesh(CMesh3d* &pMesh3D,CMesh3d* &pMesh2D,int samplesm,int samplesn);

	void glRenderTriangulation();

	void BuildTriangulationList();

	virtual int glDraw();

	//copy
	Tspline* copy();

	//
	double ComputeBasis(TsplineMesh::VertexHandle vh,double u,double v);

	//
	Point3D ComputePosition(double u,double v);

	//
	void BuildFromNURBS(CNurbsSuface* pNurbsSurface,std::vector<int> &m_uknotindex,std::vector<int> &m_vknotindex);

	//original control point network
	void DrawOriginalControlPointsNetwork();

	//control point network
	void DrawControlPointsNetwork();

	//Iso parameter curves
	void DrawIsoParameterCurves();

	//UV Knot Vector
	void SetUVKnotsVector();

	//
	double ComputeBasis_U(std::vector<double> &uknotarray, double u);

	//Basis U
	double ComputeBasis_U(std::vector<double> &uknotarray, int i, int p,double u);

	//
	double ComputeBasis_V(std::vector<double> &vknotarray, double v);

	//Basis V
	double ComputeBasis_V(std::vector<double> &vknotarray, int i, int p,double v);

	//get the non-imagination vertices
	int GetNon_ImaginationVertices();

	//
	void copySetting(Tspline* T);
};
