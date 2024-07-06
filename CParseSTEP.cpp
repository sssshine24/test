#include "stdafx.h"
#include "CParseSTEP.h"
#include <SPAIAcisDocument.h>
#include <SPAIArgumentParsing.h>
#include <kernapi.hxx>
#include <transf.hxx>
#include "lists.hxx"
#include "lib3d\Base3d.h"
#include "EntityList.h"
#include "intrapi.hxx"
#include "Action_PolyOmino.h"
#include "Nurbs3D.h"

#include "EntityList.h"
#include <Mesh3d.h>
#include <Array3d.h>
#include <vector>
#include <GL/gl.h>

//#include "acis/gme/faceter/gme_idx_mesh_utils.hxx"
#include "acistype.hxx"
#include "edge.hxx"
#include "face.hxx"
#include "fct_utl.hxx"
#include "getowner.hxx"
#include "kernapi.hxx"
#include "point.hxx"
#include "rnd_api.hxx"
#include "transf.hxx"
#include "vertex.hxx"
#include <SVNMesh.h>
//#include "mainwindow.h"

// CSceneGraph3d 类
//class CSceneGraph3d {
//public:
//    void AddEntityList(EntityList* entityList) {
//        m_EntityLists.push_back(entityList);
//    }
//
//    void Render() const {
//        for (EntityList* entityList : m_EntityLists) {
//            entityList->glDraw();
//        }
//    }
//
//private:
//    std::vector<EntityList*> m_EntityLists;
//};

void facet_entitylist(ENTITY_LIST& entitylist);

static void get_triangles_from_faceted_face(FACE* face, std::vector<float>& coords, std::vector<int>& triangles, std::vector<float>& normalCoords) {
    af_serializable_mesh* sm = GetSerializableMesh(face);
    if (nullptr == sm) {
        // Application decision: do we throw for unfaceted faces?
        return;
    }
    SPAtransf tr = get_owner_transf(face);

    const int nv = sm->number_of_vertices();
    int ntri = sm->number_of_polygons();

    coords.resize(3 * nv);
    sm->serialize_positions(coords.data());  // if std::vector::data is not available, &(coords[0]) will also work.
    if (!tr.identity()) {
        for (int ii = 0; ii < nv; ii++) {
            int jj = 3 * ii;
            SPAposition pos(coords[jj], coords[jj + 1], coords[jj + 2]);
            pos *= tr;
            coords[jj] = (float)pos.x();
            coords[jj + 1] = (float)pos.y();
            coords[jj + 2] = (float)pos.z();
        }
    }

    bool const has_normals = sm->has_normals() == TRUE;
    if (has_normals) {
        normalCoords.resize(3 * nv);
        sm->serialize_normals(normalCoords.data());
    }

    triangles.resize(3 * ntri);
    int ntri_actual = sm->serialize_triangles(triangles.data());
    while (ntri_actual < ntri) {
        triangles.pop_back();
        ntri_actual = static_cast<int>(triangles.size());
    }
}

static void get_triangles_from_faceted_faces(ENTITY_LIST& faces, std::vector<SVNMesh::FaceMesh>& faceData, std::vector<float>& coords, std::vector<int>& triangles, std::vector<float>& normalCoords) {
//static void get_triangles_from_faceted_faces(ENTITY_LIST& faces, std::vector<float>& coords, std::vector<int>& triangles, std::vector<float>& normalCoords) {

    int nF = 0;
    int nV = 0;
    int nI = 0;
    int numFaces = faces.iteration_count();
    //assert(numFaces == faceData.size());
    for (ENTITY* ent = faces.first(); ent; ent = faces.next()) {
        assert(nF < numFaces);
        assert(is_FACE(ent));
        if (!is_FACE(ent)) {
            continue;
        }

        FACE* face = (FACE*)ent;
        std::vector<float> temp_coords;
        std::vector<int> temp_triangles;
        std::vector<float> temp_normalCoords;
        get_triangles_from_faceted_face(face, temp_coords, temp_triangles, temp_normalCoords);
        {
            int nCoordsStart = (int)coords.size() / 3;
            int nCoords = (int)temp_coords.size();
            for (int ii = 0; ii < nCoords; ii++) {
                coords.push_back(temp_coords[ii]);
                normalCoords.push_back(temp_normalCoords[ii]);
            }
            int nTri = (int)temp_triangles.size();
            for (int jj = 0; jj < nTri; jj++) {
                triangles.push_back(temp_triangles[jj] + nCoordsStart);
            }
        }
        logical found = FALSE;
        outcome out = api_rh_get_entity_rgb(ent, faceData[nF].color, TRUE, found);

        if (!out.ok() || !found) {
            faceData[nF].color = rgb_color(1., 1., 1.);
        }
        faceData[nF].numIndices = (unsigned int)temp_triangles.size();
        faceData[nF].baseIndex = (unsigned int)nI;
        faceData[nF].baseVertex = (unsigned int)nV;
        faceData[nF].ptrFace = face;

        nI += (unsigned int)temp_triangles.size();
        nV += (unsigned int)temp_coords.size();
        nF++;
    }
}
int CParseSTEP::get_FaceList_from_EntityList(ENTITY_LIST* pAcisEntities, ENTITY_LIST* pFaceEntities)
{
    ENTITY_LIST facelist;
    for (ENTITY* ent = pAcisEntities->first(); ent; ent = pAcisEntities->next()) {
        //assert(nF < numFaces);
        //if (is_BODY(ent))
        
        facelist.clear();
        api_get_faces(ent, facelist);
        pFaceEntities->add(facelist);
        
    }
    return 1;
}
void CParseSTEP::SaveObjFile(char* filename, EntityList* pEntityList)
{
    std::ofstream file(filename);

    //file << NbVertex() << "  " << NbFace() << "\n";
    for (int i = 0; i < pEntityList->coords.size(); i++)
    {

    }
   /* //Vertices
    for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); ++v_it)
    {
        int vindex = v_it.handle().idx();
        OpenMesh::Vec3d pt;
        pt = pmesh->point(v_it.handle());
        file << "v " << pt[0] << " " << pt[1] << " " << pt[2] << " \n";
    }

    //Faces
    std::vector<int> vhandles;
    for (MyMesh::FaceIter f_it = pmesh->faces_begin(); f_it != pmesh->faces_end(); ++f_it)
    {
        vhandles.clear();
        for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
        {
            vhandles.push_back(fv_it.handle().idx());
        }
        file << "f " << vhandles[0] + 1 << " " << vhandles[1] + 1 << " " << vhandles[2] + 1 << " \n";
    }

    file.close();*/
}
int CParseSTEP::Run_STEP(char* filename, CSceneGraph3d* pSceneGraph)
{
    unlock_license();

    //// 启动ACIS模型处理器
    api_start_modeller(0);

    // 目标ACIS文档对象
    SPAIAcisDocument dst;
    // 创建源文档对象，并设置输入格式
    SPAIDocument src(filename);
    src.SetType("step");

    // 创建转换器对象，开始转换过程
    SPAIConverter converter;
    SPAIResult result;
    //将源文件文档转换为目标Acis文档对象
    result &= converter.Convert(src, dst);

    ENTITY_LIST* pAcisEntities = new ENTITY_LIST();

    ENTITY_LIST* pFaceEntities = new ENTITY_LIST();

    // 获取转换后的实体列表
    dst.GetEntities(pAcisEntities);

    


    EntityList* pEntityList = new EntityList();
    pEntityList->pAcisEntities = pAcisEntities;

    facet_entitylist(*pAcisEntities);

    get_FaceList_from_EntityList(pAcisEntities, pFaceEntities);

    pEntityList->faceData.resize(pFaceEntities->count());
    // 调用处理多个面的函数
    //static void get_triangles_from_faceted_faces(ENTITY_LIST & faces, std::vector<SVNMesh::FaceMesh>&faceData, std::vector<float>&coords, std::vector<int>&triangles, std::vector<float>&normalCoords) {
    get_triangles_from_faceted_faces(*pFaceEntities, pEntityList->faceData, pEntityList->coords, pEntityList->triangles, pEntityList->normalCoords);

    SaveObjFile("D:\\1.obj",pEntityList);
    //SPAposition min_pt, max_pt;
    //api_get_entity_box(*pAcisEntities, min_pt, max_pt);
    Vector3D m_lower, m_upper;
    pEntityList->ComputeBoundingBox(m_lower, m_upper);
    //get_EntityList_box(pEntityList, min_pt, max_pt);
    //设置包围盒
    CTransform transform;

    //translation
    //CVector3d translation(-(min_pt.x() + max_pt.x()) / 2, -(min_pt.y() + max_pt.y()) / 2, -(min_pt.z() + max_pt.z()) / 2);//传递给类CVector3d的构造函数并初始化对象的成员变量
    
    //transform.SetTranslation(&translation);

    //double length = max_pt.x() - min_pt.x();
    //if (length < max_pt.y() - min_pt.y())
    //    length = max_pt.y() - min_pt.y();
    //if (length < max_pt.z() - min_pt.z())
        //length = max_pt.z() - min_pt.z();
    //scale
    //CVector3d scale(1 / length, 1 / length, 1 / length);
    //transform.SetScale(&scale);
    Vector3D m_center;
    m_center = m_lower + m_upper;
    m_center /= -2;

    float length = m_upper.x - m_lower.x;
    if (length < m_upper.y - m_lower.y)
        length = m_upper.y - m_lower.y;
    if (length < m_upper.z - m_lower.z)
        length = m_upper.z - m_lower.z;

    length = length / 2;

   // CTransform transform;

    //translation
    CVector3d translation(m_center[0], m_center[1], m_center[2]);
    transform.SetTranslation(&translation);

    //scale
    CVector3d scale(1 / length, 1 / length, 1 / length);
    transform.SetScale(&scale);

    if (pSceneGraph->NbObject() >= 1)
    {
        if (pSceneGraph->GetAt(0)->GetType() == TYPE_NURBSSURFACE)
        {
            CNurbsSuface* pSurface = (CNurbsSuface*)pSceneGraph->GetAt(0);
            pEntityList->SetTransform(*pSurface->GetTransform());
        }
        else
            if (pSceneGraph->GetAt(0)->GetType() == TYPE_MESH3D)
            {
                CMesh3d* pSurface = (CMesh3d*)pSceneGraph->GetAt(0);
                pEntityList->SetTransform(*pSurface->GetTransform());
            }
    }
    else
    pEntityList->SetTransform(transform);
    pEntityList->filename = "ACIS_Model";
    pSceneGraph->Add(pEntityList);
    return 1;
}

void facet_entitylist(ENTITY_LIST& entitylist)
{
    for (auto entity : entitylist)
    {
        api_facet_entity(entity);
    }
}
