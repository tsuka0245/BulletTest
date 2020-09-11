#include "coordinate_lines.h"

CoordinateLines::CoordinateLines()
{
    //transform
    QVector3D amp = {0.1,0.1,0.1};
    setScale(amp);

    //initialize shader
    m_vshader_path = ":/v_coordinate_line.glsl";
    m_fshader_path = ":/f_coordinate_line.glsl";
    initShader();

    //initialize verices
    m_vdata.clear();
    //vertex data for x-axis
    m_vdata.append({QVector3D(1.0f,0.0f,0.0f),QVector2D(0.0,0.0)});
    m_vdata.append({QVector3D(0.0f,0.1f,0.0f),QVector2D(0.0,0.0)});
    m_vdata.append({QVector3D(0.0f,0.0f,0.1f),QVector2D(0.0,0.0)});
    m_vdata.append({QVector3D(0.0f,-0.1f,0.0f),QVector2D(0.0,0.0)});
    m_vdata.append({QVector3D(0.0f,0.0f,-0.1f),QVector2D(0.0,0.0)});
    //vertex data for y-axis
    m_vdata.append({QVector3D(0.0f,1.0f,0.0f),QVector2D(1.0,1.0)});
    m_vdata.append({QVector3D(0.1f,0.0f,0.0f),QVector2D(1.0,1.0)});
    m_vdata.append({QVector3D(0.0f,0.0f,0.1f),QVector2D(1.0,1.0)});
    m_vdata.append({QVector3D(-0.1f,0.0f,0.0f),QVector2D(1.0,1.0)});
    m_vdata.append({QVector3D(0.0f,0.0f,-0.1f),QVector2D(1.0,1.0)});
    //vertex data for z-axis
    m_vdata.append({QVector3D(0.0f,0.0f,1.0f),QVector2D(0.0,1.0)});
    m_vdata.append({QVector3D(0.1f,0.0f,0.0f),QVector2D(0.0,1.0)});
    m_vdata.append({QVector3D(0.0f,0.1f,0.0f),QVector2D(0.0,1.0)});
    m_vdata.append({QVector3D(-0.1f,0.0f,0.0f),QVector2D(0.0,1.0)});
    m_vdata.append({QVector3D(0.0f,-0.1f,0.0f),QVector2D(0.0,1.0)});

    //initialize index
    m_indices.clear();
    m_indices.append({
                         1,2,0,3,4,4,0,0,1,4,2,3,3,
                         6,7,5,8,9,9,5,5,6,9,7,8,8,
                         11,12,10,13,14,14,10,10,11,14,12,13,13
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

CoordinateLines::~CoordinateLines()
{
}

void CoordinateLines::drawGeo(QMatrix4x4 projection,QMatrix4x4 view)
{
    //bind shader program
    m_program.bind();

    //send data to shader
    QMatrix4x4 screen;
    screen.setToIdentity();
    screen.translate({-0.9,-0.9,0.0});
    m_program.setUniformValue("screen_mat",screen);
    m_program.setUniformValue("proj_mat",projection);
    m_program.setUniformValue("view_mat",view);
    m_program.setUniformValue("model_mat",getModelMat());

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
    glDrawElements(GL_TRIANGLE_STRIP,m_indices.size(),GL_UNSIGNED_INT,(GLuint*)0);

    m_vbo.release();
    m_ibo.release();

    m_program.release();
}
