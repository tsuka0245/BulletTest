#ifndef PLANEGEO_H
#define PLANEGEO_H

#include <QOpenGLFunctions>
#include "geometry.h"

class PlaneGeo : public Geometry
{
public:
    PlaneGeo(QVector3D pos,QVector3D rot,QVector3D scale,bool selectable,bool staticity);

    void drawGeo(QMatrix4x4 projection,QMatrix4x4 view) override;
};

#endif // PLANEGEO_H
