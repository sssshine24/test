#if !defined(OPENMESH_TYPE)
#define OPENMESH_TYPE

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

// define traits
struct MyTraits : public OpenMesh::DefaultTraits
{
	// use double valued coordinates
	typedef OpenMesh::Vec3d Point;

	// use vertex normals and vertex colors
	VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);

	// store the previous halfedge
	HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );

	//edge
	EdgeAttributes(OpenMesh::Attributes::Status);  

	// use face normals
	FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status);

	// store a face handle for each vertex
	VertexTraits{};

};
struct TsplineTraits : public OpenMesh::DefaultTraits
{
	// use double valued coordinates
	typedef OpenMesh::Vec3d Point;

	// use vertex normals and vertex colors
	VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);

	// store the previous halfedge
	HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );

	//edge
	EdgeAttributes(OpenMesh::Attributes::Status);  

	// use face normals
	FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status);

	// store a face handle for each vertex
	VertexTraits{};
};

// Select mesh type (TriMesh) and kernel (ArrayKernel)
// and define my personal mesh type (MyMesh)
typedef OpenMesh::PolyMesh_ArrayKernelT<MyTraits>  MyMesh;
typedef OpenMesh::PolyMesh_ArrayKernelT<TsplineTraits>  TsplineMesh;
#endif 