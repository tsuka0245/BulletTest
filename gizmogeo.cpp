#include "gizmogeo.h"

#include <QDebug>

GizmoGeo::GizmoGeo()
{
    m_selectable = 1;

    //initialize shader
    m_vshader_path = ":/v_gizmo.glsl";
    m_fshader_path = ":/f_gizmo.glsl";
    initShader();
}

GizmoGeo::~GizmoGeo()
{
}

void GizmoGeo::drawGeo(QMatrix4x4 projection,QMatrix4x4 view)
{
    //bind shader program
    m_program.bind();

    //send data to shader
    m_program.setUniformValue("proj_mat",projection);
    m_program.setUniformValue("view_mat",view);
    QMatrix4x4 rModel0 = getModelMat();
    QMatrix4x4 rModel1 = getModelMat();
    switch(m_axis)
    {
    case 0:
        m_program.setUniformValue("model_mat",getModelMat());
        break;
    case 1:
        rModel0.rotate(90,{0,0,1});
        m_program.setUniformValue("model_mat",rModel0);
        break;
    case 2:
        rModel1.rotate(-90,{0,1,0});
        m_program.setUniformValue("model_mat",rModel1);
        break;
    }


    //bind buffer
    m_vbo.bind();
    m_ibo.bind();

    //offset for position
    quintptr offset = 0;

    //send position attribute to shader
    int vertexLocation = m_program.attributeLocation("a_pos");
    m_program.enableAttributeArray(vertexLocation);
    m_program.setAttributeBuffer(vertexLocation,GL_FLOAT,offset,3,sizeof(VertexData));

    //offset for uv
    offset += sizeof(QVector3D);

    //send uv attribute to shader
    int uvLocation = m_program.attributeLocation("a_uv");
    m_program.enableAttributeArray(uvLocation);
    m_program.setAttributeBuffer(uvLocation,GL_FLOAT,offset,2,sizeof(VertexData));

    //draw
    if(m_picking == true)
    {
        glDrawElements(GL_TRIANGLE_STRIP,m_indices.size(),GL_UNSIGNED_INT,(GLuint*)0);
        m_picking = false;
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLE_STRIP,m_indices.size(),GL_UNSIGNED_INT,(GLuint*)0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }


    m_vbo.release();
    m_ibo.release();

    m_program.release();
}
