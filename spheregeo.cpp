#include "spheregeo.h"

SphereGeo::SphereGeo(QVector3D pos,QVector3D rot,QVector3D scale,bool selectable,bool staticity)
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
    // Vertex data
    int stn = 12;
    int sln = 6;
    GLfloat phi = 0, theta = 0;
    for(int i=0;i<=sln;i++)
    {
       for(int j=0;j<=stn;j++)
       {
           QVector3D pos;
           pos.setX(qSin(qDegreesToRadians(phi))*qCos(qDegreesToRadians(theta)));
           pos.setY(qCos(qDegreesToRadians(phi)));
           pos.setZ(qSin(qDegreesToRadians(phi))*qSin(qDegreesToRadians(theta)));
           m_vdata.append({pos,QVector2D(1.0,1.0)});
           theta += 360/stn;
       }
       phi += 180/sln;
    }
    //initialize index
    m_indices.clear();
    for(int i=0;i<sln;i++)
    {
        for(int j=0;j<stn;j++)
        {
            GLuint p0,p1,p2,p3;
            p0 = (stn+1)*i+j;
            p1 = p0 +stn+1;
            p2 = p0 +1;
            p3 = p1 +1;
            m_indices.append({p0,p0,p1,p2,p3,p3});
        }
    }
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
    m_colShape = new btSphereShape(1);
    m_colShape->setLocalScaling(qv3tobtv3(getModelScale()));

    //motion state
    btTransform initTransform;
    initTransform.setIdentity();
    initTransform.setOrigin(qv3tobtv3(getModelPos()));
    initTransform.setRotation(qqtobtq(getModelQuaterion()));
    m_motionState = new btDefaultMotionState(initTransform);

    //mass and inertia
    btScalar mass = 0.0;
    btVector3 localInertia = {0.0,0.0,0.0};
    if(staticity == false)
    {
        mass = m_density * 4*M_PI/3;
        m_colShape->calculateLocalInertia(mass,localInertia);
    }

    //rigid body
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,m_motionState,m_colShape,localInertia);
    m_body = new btRigidBody(rbInfo);
}

SphereGeo::~SphereGeo()
{
}

void SphereGeo::drawGeo(QMatrix4x4 projection, QMatrix4x4 view)
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

void SphereGeo::setDensity(GLfloat density)
{
    m_density = density;

    btScalar mass = m_density * 4*M_PI/3;
    m_mass = mass;
    m_body->setMassProps(mass,getInertia());
}
