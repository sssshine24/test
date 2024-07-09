//********************************************
// Base3d.h
// 3d Toolbox Includes 
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 15/12/97
// Modified : 27/04/98
//********************************************

#ifndef _BASE_3D_
#define _BASE_3D_

// 3d library
#include "Object3d.h"
#include "Vertex3d.h"
#include "Vector3d.h"
#include "Edge3d.h"
#include "Face3d.h"
#include "Mesh3d.h"
#include "SceneGraph3d.h"
#include "Utils3d.h"
#include "Transform.h"

// Standard
#include "Array3d.h"

// VRML Parser
#include "..\ParseSmf.h"
// Apparence attributes
#include "Color.h"
#include "ColorRamp.h"
#include "Material.h"
#include "Texture.h"


// Object types
enum {TYPE_OBJECT3D,
			TYPE_VERTEX3D,
			TYPE_VECTOR3D,
			TYPE_FACE3D,
			TYPE_POLYGON3D,
			TYPE_MESH3D,
			TYPE_EDGE3D,
			TYPE_VECTORSET3D,
            TYPE_NURBSSURFACE,
			TYPE_TSPLINE,
			TYPE_DISLINE2D,
			TYPE_NURBSCURVE3D,
			TYPE_MESH3D_OPENMESH,
			TYPE_POLYGON,
<<<<<<< HEAD
			TYPE_BREP_ACIS,
			TYPE_BREP_ACIS_FACE};
=======
			TYPE_BREP_ACIS};
>>>>>>> 4d531c4dcf38db10b06bd94d29ba52f86eb4a186

// Normal binding
enum {NORMAL_PER_VERTEX,
			NORMAL_PER_FACE};

enum {COLOR_PER_VERTEX,
			COLOR_PER_FACE};

enum  {POTENTIAL_EDGE_LENGTH,
			 POTENTIAL_CURVE,
       POTENTIAL_CURVE_AND_EDGE_LENGTH};

// Misc
const float MAX_FLOAT = 3.4e+38f; 
const double MAX_DOUBLE = 1.7e+308; 
const double PI =  3.14159265359;

#endif // _BASE_3D_


// ** EOF **
