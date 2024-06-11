# include general settings


# are we using external libraries like EvoluteTools


# which additional libraries do we use
CONFIG(debug, debug|release) {
   
} else {
   
}

QT += opengl

# Input
HEADERS += Bspline.h
HEADERS += C_NurbsSurf.h
HEADERS += C_ParseNurbs.h
HEADERS += ChildFrm.h
HEADERS += ClosenessEnergy.h
HEADERS += ColorFormView.h
HEADERS += convexFunction.h
HEADERS += Curve.h
HEADERS += Dib.h
HEADERS += disline.h
HEADERS += Energy.h
HEADERS += FormView3.h
HEADERS += GeneralBilinearTransformation.h
HEADERS += GeneralBilinearTransformation_.h
HEADERS += GeneralBilinearTransformationdlg.h
HEADERS += GeneralTransformationDlg.h

SOURCES += MeshEvolutionNewPlugin.cpp
SOURCES += PQMeshNeighborhood.cpp
SOURCES += MeshEvolutionNewDefs.cpp
SOURCES += PolyMeshOper.cpp
SOURCES += Singularity_ImplicitFuncPlanarity.cpp
SOURCES += Singularity_ImplicitFuncFairness.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_Evolution.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_Evolution.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_AspectRatio_Evolution.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_Conical_Evolution.cpp
SOURCES += Graph2DPlot.cpp
SOURCES += Graph2DWidget.cpp
SOURCES += Exploration2DPlot.cpp
SOURCES += Exploration3DWidget.cpp
SOURCES += Singularity_ManifoldProjection.cpp
SOURCES += Singularity_ImplicitFunc.cpp
SOURCES += Singularity_ImplicitFuncQuadAspectRatio.cpp
SOURCES += Singularity_ImplicitFuncConical.cpp
SOURCES += Singularity_ImplicitFuncCircular.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_Circular_Evolution.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterAbsFairness_Evolution.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterCombinedFairness_Evolution.cpp
SOURCES += Singularity_ImplicitFuncAbsFairness.cpp
SOURCES += Singularity_ImplicitFuncThirdFairness.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterConstrainedFairness_Evolution.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterConstrainedFairness_LShape_Evolution.cpp
SOURCES += Singularity_ImplicitFuncEqualEdgeLength.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_Geodesic_Evolution.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_EqualEdgeLength_Evolution.cpp
SOURCES += Singularity_ImplicitFuncAbsThirdFairness.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterConstrained_PathPrediction.cpp
SOURCES += Singularity_ImplicitFuncOrthogonality.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_Exploration_Evolution.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_PathPredictionTangent.cpp
SOURCES += Singularity_ImplicitFuncTangential.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_NewtonValley_Evolution.cpp
SOURCES += Singularity_ImplicitFuncPCATorsion.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterFairness_CircularExploration_Evolution.cpp
SOURCES += Singularity_PQMesh_ShapeSpace_PlanarityAlterConstrainedFairness_Hypersurface_Evolution.cpp

FORMS += meshevolutionnewpanel.ui
FORMS += meshevolution2dplotnewform.ui


