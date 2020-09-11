#include "geometry.h"

#include <QDebug>

Geometry::Geometry()
    : m_ibo(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    m_model.setToIdentity();
}

Geometry::~Geometry()
{
    m_vbo.destroy();
    m_ibo.destroy();
    if(m_texture != nullptr)delete m_texture;
    m_vdata.clear();
    m_indices.clear();

    delete m_colShape;
    delete m_motionState;
    delete m_body;
}

void Geometry::setStaticity(bool staticity)
{
    m_static = staticity;
    if(staticity == true)m_body->setMassProps(0.0,btVector3(0.0,0.0,0.0));
    else
    {
        m_body->setMassProps(m_mass,getInertia());
        m_body->activate(true);
    }
}

QMatrix4x4 Geometry::getModelMat()
{
    m_model.setToIdentity();

    //transform order : scale-rot-translate
    m_model.translate(m_pos);
    m_model.rotate(m_quaternion);
    m_model.scale(m_scale);

    return m_model;
}

btVector3 Geometry::getInertia()
{
    btVector3 localInertia;
    m_colShape->calculateLocalInertia(m_mass,localInertia);
    return localInertia;
}

void Geometry::initShader()
{
    //compile shader
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,m_vshader_path);
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,m_fshader_path);
    m_program.link();
    m_program.bind();
}

void Geometry::initTexture()
{
    //import texture
    m_texture = new QOpenGLTexture(QImage(m_texture_path).mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

