#ifndef SPHEREGEO_H
#define SPHEREGEO_H

#include <QOpenGLFunctions>
#include "geometry.h"

class SphereGeo : public Geometry
{
public:
    SphereGeo(QVector3D pos,QVector3D rot,QVector3D scale,bool selectable,bool staticity);
    ~SphereGeo();

    void drawGeo(QMatrix4x4 projection,QMatrix4x4 view) override;
};

#endif // SPHEREGEO_H
