#ifndef P2PCON_H
#define P2PCON_H

#include <QOpenGLFunctions>
#include "geometry.h"

class P2PCon : public Geometry
{
public:
    P2PCon(QVector3D pos,GLuint geo1,GLuint geo2);
    ~P2PCon();

    void drawGeo(QMatrix4x4 projection,QMatrix4x4 view) override;

    void setInitGeo1Local(QVector3D pos){m_init_geo1_local = pos;}
    void setGeo1Id(GLuint geo1){m_geo1 = geo1;}
    void setGeo2Id(GLuint geo2){m_geo2 = geo2;}

    QVector3D getInitGeo1Local(){return m_init_geo1_local;}
    GLuint getGeo1Id(){return m_geo1;}
    GLuint getGeo2Id(){return m_geo2;}
    GLuint getType(){return m_type;}
private:
    GLuint m_geo1;
    GLuint m_geo2;
    GLuint m_type = 0;

    QVector3D m_init_geo1_local = {0,0,0};
};

#endif // P2PCON_H
