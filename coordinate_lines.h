#ifndef COORDINATE_LINES_H
#define COORDINATE_LINES_H

#include <QOpenGLFunctions>
#include "geometry.h"

class CoordinateLines : public Geometry
{
public:
    CoordinateLines();
    ~CoordinateLines();

    void drawGeo(QMatrix4x4 projection,QMatrix4x4 view) override;
};

#endif // COORDINATE_LINES_H
