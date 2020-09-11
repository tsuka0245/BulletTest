#include "gizmorotate.h"

GizmoRotate::GizmoRotate(GLuint axis)
{
    m_axis = axis;

    //initialize verices
    m_vdata.clear();
    //vertex data for x-axis
    QVector2D uv;
    if(m_axis == 0)uv = {0.0,0.0};
    if(m_axis == 1)uv = {1.0,1.0};
    if(m_axis == 2)uv = {0.0,1.0};

    for(int i=0;i<32;i++)
    {
        GLfloat y = qCos(2*M_PI*i/32);
        GLfloat z = qSin(2*M_PI*i/32);
        m_vdata.append({QVector3D(0.0f,y,z),uv});
        m_vdata.append({QVector3D(0.0f,y*0.95,z*0.95),uv});
    }

    //initialize index
    m_indices.clear();
    for(int i=0;i<64;i++)
    {
        m_indices.append(i);
    }
    m_indices.append({0,1,1});
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
