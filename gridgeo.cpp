#include "gridgeo.h"

#include <QDebug>

GridGeo::GridGeo(QVector3D pos,QVector3D rot,QVector3D scale,bool selectable,bool staticity)
{
    //transform
    setPos(pos);
    setRot(rot);
    setScale(scale);

    m_selectable = selectable;
    m_static = staticity;

    //initialize shader
    m_vshader_path = ":/v_grid.glsl";
    m_fshader_path = ":/f_grid.glsl";
    initShader();

    //initialize vertices and uv
    m_vdata.clear();
    // Vertex data for face 0
    m_vdata.append({QVector3D( 1.0f, 0.0f,  1.0f), QVector2D(1.0f, 1.0f)});
    m_vdata.append({QVector3D(-1.0f, 0.0f,  1.0f), QVector2D(0.0f, 1.0f)});
    m_vdata.append({QVector3D( 1.0f,  0.0f,  -1.0f), QVector2D(1.0f, 0.0f)});
    m_vdata.append({QVector3D(-1.0f,  0.0f,  -1.0f), QVector2D(0.0f, 0.0f)});

    //initialize index
    m_indices.clear();
    m_indices.append({
                         0,1,2,3,3,
                         1,1,0,3,2
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


    //collision shape
    m_colShape = new btBoxShape(btVector3(1,0.05,1));
    m_colShape->setLocalScaling(qv3tobtv3(getModelScale()));

    //motion state
    btTransform initTransform;
    initTransform.setIdentity();
    initTransform.setOrigin(qv3tobtv3(getModelPos()));
    initTransform.setRotation(qqtobtq(getModelQuaterion()));
    m_motionState = new btDefaultMotionState(initTransform);

    //mass and inertia
    m_volume = 2*2*0.1;
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

GridGeo::~GridGeo()
{
}

void GridGeo::drawGeo(QMatrix4x4 projection, QMatrix4x4 view)
{
    //bind shader program
    m_program.bind();

    //send data to shader
    m_program.setUniformValue("proj_mat",projection);
    m_program.setUniformValue("view_mat",view);
    m_program.setUniformValue("model_mat",getModelMat());
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
    glDrawElements(GL_TRIANGLE_STRIP,m_indices.size(),GL_UNSIGNED_INT,(GLuint*)0);

    m_vbo.release();
    m_ibo.release();

    m_program.release();
}
