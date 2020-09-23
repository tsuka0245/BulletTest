#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QString>
#include <QtMath>

#include "qtbtconvert.h"

#include <btBulletDynamicsCommon.h>

struct VertexData
{
    QVector3D position;
    QVector2D uv;
};

class Geometry : protected QOpenGLFunctions
{
public:
    Geometry();
    virtual ~Geometry();

    virtual void drawGeo(QMatrix4x4 projection,QMatrix4x4 view) = 0;

    void setName(QString name){m_name = name;}
    void setGeoID(GLint id){m_geoid = id;}

    void setPos(QVector3D position){m_pos = position;}
    void setInitPos(QVector3D position){m_initpos = position;}
    void setRot(QVector3D rotation){m_quaternion = QQuaternion::fromEulerAngles(rotation);}
    void setRot(QQuaternion quaternion){m_quaternion = quaternion;}
    void setInitRot(QVector3D rotation){m_initquaternion = QQuaternion::fromEulerAngles(rotation);}
    void setInitRot(QQuaternion quaternion){m_initquaternion = quaternion;}
    void setScale(QVector3D scale);
    void setDensity(GLfloat density);
    void setSelected(bool selected){m_selected = selected;}
    void setStaticity(bool staticity);

    QString getName(){return m_name;}
    GLint getGeoId(){return m_geoid;}

    QMatrix4x4 getModelMat();
    QVector3D getModelPos(){return m_pos;}
    QVector3D getInitModelPos(){return m_initpos;}
    QVector3D getModelRot(){return m_quaternion.toEulerAngles();}
    QVector3D getModelScale(){return m_scale;}
    QQuaternion getModelQuaterion(){return m_quaternion;}
    QQuaternion getInitModelQuaternion(){return m_initquaternion;}

    GLfloat getDensity(){return m_density;}
    btScalar getMass(){return m_mass;}
    btVector3 getInertia();

    bool getStaticity(){return m_static;}
    bool getSelectable(){return m_selectable;}

    btCollisionShape* getCollisionShape(){return m_colShape;}
    btRigidBody* getRigidBody(){return m_body;}


protected:
    void initShader();
    void initTexture();

    //vertex data
    QVector<VertexData> m_vdata;
    QVector<GLuint> m_indices;

    //model data
    QString m_name = "geo";
    GLint m_geoid = 0; //-1:constraint 0<:geometry id

    QVector3D m_pos = {0,0,0};
    QVector3D m_scale = {1,1,1};
    QQuaternion m_quaternion = {0,0,0,0};
    QMatrix4x4 m_model;

    QVector3D m_initpos = {0,0,0};
    QQuaternion m_initquaternion = {0,0,0,0};

    //physical property
    GLfloat m_volume = 1.0;
    GLfloat m_density = 1.0;
    GLfloat m_mass = 1.0;
    bool m_static = 0;

    //collision
    btCollisionShape* m_colShape = nullptr;
    btDefaultMotionState* m_motionState = nullptr;
    btRigidBody* m_body = nullptr;

    //buffer
    QOpenGLBuffer m_vbo; //vertex data
    QOpenGLBuffer m_ibo; //index data

    //shader
    QOpenGLShaderProgram m_program;
    QString m_vshader_path;
    QString m_fshader_path;

    //texture
    QOpenGLTexture *m_texture = nullptr;
    QString m_texture_path;

    //picking
    bool m_selectable;
    bool m_selected = 0;


};

#endif // GEOMETRY_H
