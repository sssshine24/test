#include "stdafx.h"
//#include "RicciFlow.h"
//#include "Structure/Structure.h"
////for Extremal Lengths
//#include "Riemannian/RicciFlow/TangentialRicciExtremalLength.h"
//#include "Riemannian/RicciFlow/EuclideanEmbed.h"
//
//using namespace MeshLib;
//
//unsigned int CRicciFlowVertex::traits = 0;
//
//RicciFlow::RicciFlow(void)
//{
//}
//
//RicciFlow::~RicciFlow(void)
//{
//}
///******************************************************************************************************************************
//*
//*	Extremal Length
//*
//*******************************************************************************************************************************/
//
////-tangent_ricci_extremal_length sophie.remesh.m sophie.uv.m
//void RicciFlow::_tangent_ricci_extremal_length( const char * _input_mesh, const char * _mesh_with_uv )
//{
//	CRicciFlowVertex::traits = CRicciFlowVertex::traits | TRAIT_UV;
//
//	CRFMesh mesh;
//	mesh.read_m( _input_mesh );
//
//	CTangentialRicciFlowExtremalLength<CRicciFlowVertex,CRicciFlowEdge,CRicciFlowFace,CRicciFlowHalfEdge> mapper(&mesh);
//	mapper._calculate_metric();
//
//	CRFEmbed embed( &mesh );
//	embed._embed();
//	mesh.write_m( _mesh_with_uv );
//}