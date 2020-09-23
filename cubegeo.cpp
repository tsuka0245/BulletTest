#include "cubegeo.h"

#include <QDebug>

CubeGeo::CubeGeo(QVector3D pos,QVector3D rot,QVector3D scale,bool selectable,bool staticity)
{
    //transform
    setPos(pos);
    setRot(rot);
    setScale(scale);

    m_selectable = selectable;
    m_static = staticity;

    //initialize shader
    m_vshader_path = ":/v_cube.glsl";
    m_fshader_path = ":/f_cube.glsl";
    initShader();

    //initialize texture
    m_texture_path = ":/cube.png";
    initTexture();

    //initialize vertices and uv
    m_vdata.clear();
    // Vertex data for face 0
    m_vdata.append({QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)});  // v0
    m_vdata.append({QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}); // v1
    m_vdata.append({QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)});  // v2
    m_vdata.append({QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}); // v3

    // Vertex data for face 1
    m_vdata.append({QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}); // v4
    m_vdata.append({QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}); // v5
    m_vdata.append({QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)});  // v6
    m_vdata.append({QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}); // v7

    // Vertex data for face 2
    m_vdata.append({QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}); // v8
    m_vdata.append({QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)});  // v9
    m_vdata.append({QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}); // v10
    m_vdata.append({QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)});  // v11

    // Vertex data for face 3
    m_vdata.append({QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}); // v12
    m_vdata.append({QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)});  // v13
    m_vdata.append({QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}); // v14
    m_vdata.append({QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)});  // v15

    // Vertex data for face 4
    m_vdata.append({QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}); // v16
    m_vdata.append({QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}); // v17
    m_vdata.append({QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}); // v18
    m_vdata.append({QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}); // v19

    // Vertex data for face 5
    m_vdata.append({QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}); // v20
    m_vdata.append({QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}); // v21
    m_vdata.append({QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}); // v22
    m_vdata.append({QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)});  // v23

    //initialize index
    m_indices.clear();
    m_indices.append({0,1,2,3,3,
                      4,4,5,6,7,7,
                      8,8,9,10,11,11,
                      12,12,13,14,15,15,
                      16,16,17,18,19,19,
                      20,20,21,22,23,23});
    //vbo
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vdata.begin(),24 * sizeof(VertexData));
    m_vbo.release();
    //ibo
    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(m_indices.begin(),34 * sizeof(GLuint));
    m_ibo.release();

    //collision shape
    m_colShape = new btBoxShape(btVector3(1,1,1));
    m_colShape->setLocalScaling(qv3tobtv3(getModelScale()));

    //motion state
    btTransform initTransform;
    initTransform.setOrigin(qv3tobtv3(getModelPos()));
    initTransform.setRotation(qqtobtq(getModelQuaterion()));
    m_motionState = new btDefaultMotionState(initTransform);

    //mass and inertia
    m_volume = 8.0;
    btScalar mass = 0.0;
    btVector3 localInertia = {0.0,0.0,0.0};
    if(staticity == false)
    {
        mass = m_density * m_volume;
        m_colShape->calculateLocalInertia(mass,localInertia);
    }

    //rigid body
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,m_motionState,m_colShape,localInertia);
    m_body = new btRigidBody(rbInfo);
}

CubeGeo::~CubeGeo()
{
}

void CubeGeo::drawGeo(QMatrix4x4 projection, QMatrix4x4 view)
{
    //bind shader program
    m_program.bind();

    //bind texture
    m_texture->bind();

    //send data to shader
    m_program.setUniformValue("proj_mat",projection);
    m_program.setUniformValue("view_mat",view);
    m_program.setUniformValue("model_mat",getModelMat());
    m_program.setUniformValue("texture",0);
    m_program.setUniformValue("selected",m_selected);

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
    glDrawElements(GL_TRIANGLE_STRIP,34,GL_UNSIGNED_INT,(GLuint*)0);

    m_vbo.release();
    m_ibo.release();

    m_texture->release();

    m_program.release();
}
