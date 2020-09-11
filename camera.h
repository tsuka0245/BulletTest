#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

class Camera
{
public:
    Camera();

    void setPos(QVector3D pos){m_position = pos;}
    void setUp(QVector3D up){m_up = up.normalized();}
    void setFront(QVector3D front){m_front = front.normalized();}
    void setRight(QVector3D right){m_right = right.normalized();}
    void setRot(QVector3D rot);
    void setFov(GLfloat fov){m_fov = fov;}
    void setNearClip(GLfloat nearclip){m_nearclip = nearclip;}
    void setFarClip(GLfloat farclip){m_farclip = farclip;}

    QVector3D getPos(){return m_position;}
    QVector3D getUp(){return m_up;}
    QVector3D getFront(){return m_front;}
    QVector3D getRight(){return m_right;}
    QVector3D getRot(){return m_rot;}
    GLfloat getFov(){return m_fov;}
    GLfloat getNearClip(){return m_nearclip;}
    GLfloat getFarClip(){return m_farclip;}

    QMatrix4x4 getViewMat();
    QMatrix4x4 getViewMatForCoordinate();
    QMatrix4x4 getProjectionMat(GLfloat w, GLfloat h);
    QMatrix4x4 getPickingProjectionMat(GLfloat w, GLfloat h, GLfloat sx, GLfloat sy);

private:
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    QVector3D m_position = {5.0,0.0,0.0};
    QVector3D m_up = {0.0,1.0,0.0};
    QVector3D m_front = {1.0,0.0,0.0};
    QVector3D m_right = {0.0,0.0,-1.0};
    QVector3D m_rot = {0.0,0.0,0.0}; //pitch,yaw,roll

    GLfloat m_fov = 45;
    GLfloat m_nearclip = 0.01;
    GLfloat m_farclip = 100;
};

#endif // CAMERA_H
