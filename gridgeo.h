#ifndef GRIDGEO_H
#define GRIDGEO_H

#include <QOpenGLFunctions>
#include "geometry.h"

class GridGeo : public Geometry
{
public:
    GridGeo(QVector3D pos,QVector3D rot,QVector3D scale,bool selectable,bool staticity);
    ~GridGeo();

    void drawGeo(QMatrix4x4 projection,QMatrix4x4 view) override;
    void setDensity(GLfloat density);
};

#endif // GRIDGEO_H
