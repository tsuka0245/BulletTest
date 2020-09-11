#include "camera.h"

Camera::Camera()
{
    m_view.setToIdentity();
    m_projection.setToIdentity();
}

void Camera::setRot(QVector3D rot)
{
    m_rot = rot;
    if(m_rot.x() > 90)m_rot.setX(89.99);
    if(m_rot.x() < -90)m_rot.setX(-89.99);
}

//View Matrix
QMatrix4x4 Camera::getViewMat()
{
    //get front vector
    QVector3D dir;
    dir.setX(qCos(qDegreesToRadians(m_rot.x())) * qCos(qDegreesToRadians(m_rot.y())));
    dir.setY(qSin(qDegreesToRadians(m_rot.x())));
    dir.setZ(qCos(qDegreesToRadians(m_rot.x())) * qSin(qDegreesToRadians(m_rot.y())));
    m_front = dir;
    m_right = QVector3D::crossProduct({0,1,0},m_front);
    m_up = QVector3D::crossProduct(m_front,m_right);

    //get view mat
    m_view.setToIdentity();
    m_view.lookAt(m_position+m_front,m_position,m_up);
    return m_view;
}

//View Matrix for Coordinate Lines
QMatrix4x4 Camera::getViewMatForCoordinate()
{
    //get front vector
    QVector3D dir;
    dir.setX(qCos(qDegreesToRadians(m_rot.x())) * qCos(qDegreesToRadians(m_rot.y())));
    dir.setY(qSin(qDegreesToRadians(m_rot.x())));
    dir.setZ(qCos(qDegreesToRadians(m_rot.x())) * qSin(qDegreesToRadians(m_rot.y())));
    m_front = dir;
    m_right = QVector3D::crossProduct({0,1,0},m_front);
    m_up = QVector3D::crossProduct(m_front,m_right);

    //get view mat for coordinate lines
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(m_front.normalized()+m_front,m_front,m_up);
    return view;
}

//Projection Matrix
QMatrix4x4 Camera::getProjectionMat(GLfloat w, GLfloat h)
{
    GLfloat aspect = w / h;
    m_projection.setToIdentity();
    m_projection.perspective(m_fov,aspect,m_nearclip,m_farclip);
    return m_projection;
}

//Projection Matrix for picking
QMatrix4x4 Camera::getPickingProjectionMat(GLfloat w, GLfloat h, GLfloat sx, GLfloat sy)
{
    GLfloat sxmax,symax;
    symax = m_nearclip*qTan(qDegreesToRadians(m_fov/2));
    sxmax = symax*w/h;

    GLfloat right,left,top,bottom,dist;
    dist = 0.0001; //picking area setting
    right = sxmax*sx+dist;
    left = sxmax*sx-dist;
    top = symax*sy+dist;
    bottom = symax*sy-dist;

    //create component elements for projection matrix
    QVector4D r0,r1,r2,r3;
    r0 = {2*m_nearclip/(right-left),0.0,(right+left)/(right-left),0.0};
    r1 = {0.0,2*m_nearclip/(top-bottom),(top+bottom)/(top-bottom),0.0};
    r2 = {0.0,0.0,-(m_farclip+m_nearclip)/(m_farclip-m_nearclip),
          -2*m_farclip*m_nearclip/(m_farclip-m_nearclip)};
    r3 = {0.0,0.0,-1.0,0.0};

    //create projection matrix for picking
    QMatrix4x4 picking;
    picking.setToIdentity();
    picking.setRow(0,r0);
    picking.setRow(1,r1);
    picking.setRow(2,r2);
    picking.setRow(3,r3);

    return picking;
}

