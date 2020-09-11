#include "qtbtconvert.h"

QVector3D btv3toqv3(btVector3 v)
{
    QVector3D qv = {v.x(),v.y(),v.z()};
    return qv;
}
btVector3 qv3tobtv3(QVector3D v)
{
    btVector3 btv = {v.x(),v.y(),v.z()};
    return btv;
}
QQuaternion btqtoqq(btQuaternion q)
{
    QQuaternion qq = {q.w(),q.x(),q.y(),q.z()};
    return qq;
}
btQuaternion qqtobtq(QQuaternion q)
{
    QVector4D v4 = q.toVector4D();
    btQuaternion btq = {v4.x(),v4.y(),v4.z(),v4.w()};
    return btq;
}
