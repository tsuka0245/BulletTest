#include "gizmotranslate.h"

GizmoTranslate::GizmoTranslate(GLuint axis)
{
    m_axis = axis;

    //initialize verices
    m_vdata.clear();
    //vertex data for x-axis
    QVector2D uv;
    if(m_axis == 0)uv = {0.0,0.0};
    if(m_axis == 1)uv = {1.0,1.0};
    if(m_axis == 2)uv = {0.0,1.0};
    m_vdata.append({QVector3D(1.0f,0.0f,0.0f),uv});
    m_vdata.append({QVector3D(0.8f,0.1f,0.0f),uv});
    m_vdata.append({QVector3D(0.8f,0.0f,0.1f),uv});
    m_vdata.append({QVector3D(0.8f,-0.1f,0.0f),uv});
    m_vdata.append({QVector3D(0.8f,0.0f,-0.1f),uv});

    m_vdata.append({QVector3D(0.0f,0.05f,0.0f),uv});
    m_vdata.append({QVector3D(0.8f,0.05f,0.0f),uv});
    m_vdata.append({QVector3D(0.0f,0.0f,0.05f),uv});
    m_vdata.append({QVector3D(0.8f,0.0f,0.05f),uv});
    m_vdata.append({QVector3D(0.0f,-0.05f,0.0f),uv});
    m_vdata.append({QVector3D(0.8f,-0.05f,0.0f),uv});
    m_vdata.append({QVector3D(0.0f,0.0f,-0.05f),uv});
    m_vdata.append({QVector3D(0.8f,0.0f,-0.05f),uv});

    //initialize index
    m_indices.clear();
    m_indices.append({
                         1,2,0,3,4,4,0,0,1,4,2,3,3,
                         5,5,6,7,8,8,
                         7,7,8,9,10,10,
                         9,9,10,11,12,12,
                         11,11,12,5,6,6
                     });
    //vbo
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vdata.begin(),m_vdata.size()*sizeof(VertexData));
    m_vbo.release();
    //ibo
    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(m_indices.begin(),m_indices.size()*sizeof(GLuint));
    m_ibo.release();
}
