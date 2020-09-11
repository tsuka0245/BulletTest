#ifndef QTBTCONVERT_H
#define QTBTCONVERT_H

#include <QVector3D>
#include <QVector4D>
#include <QQuaternion>
#include <btBulletDynamicsCommon.h>

QVector3D btv3toqv3(btVector3 v);
btVector3 qv3tobtv3(QVector3D v);
QQuaternion btqtoqq(btQuaternion q);
btQuaternion qqtobtq(QQuaternion q);

#endif // QTBTCONVERT_H
