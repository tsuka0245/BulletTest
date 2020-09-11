#ifndef GIZMOGEO_H
#define GIZMOGEO_H

#include <QOpenGLFunctions>
#include "geometry.h"

class GizmoGeo : public Geometry
{
public:
    GizmoGeo();
    ~GizmoGeo();

    void drawGeo(QMatrix4x4 projection, QMatrix4x4 view) override;

    void setPicking(GLboolean a){m_picking = a;}

protected:
    GLuint m_axis = 0;//x:0 y:1 z:2
    GLboolean m_picking = 0;
};

#endif // GIZMOGEO_H
