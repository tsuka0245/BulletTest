#ifndef CUBEGEO_H
#define CUBEGEO_H

#include <QOpenGLFunctions>
#include "geometry.h"

class CubeGeo : public Geometry
{
public:
    CubeGeo(QVector3D pos,QVector3D rot,QVector3D scale,bool selectable,bool staticity);
    ~CubeGeo();

    void drawGeo(QMatrix4x4 projection,QMatrix4x4 view) override;
    void setDensity(GLfloat density);
};

#endif // CUBEGEO_H
