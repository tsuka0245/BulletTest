#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QBasicTimer>
#include <QVector>
#include <QMatrix4x4>
#include <QQuaternion>

#include "camera.h"
#include "geometry.h"
#include "cubegeo.h"
#include "planegeo.h"
#include "gridgeo.h"
#include "spheregeo.h"
#include "coordinate_lines.h"
#include "gizmogeo.h"
#include "gizmotranslate.h"
#include "gizmorotate.h"
#include "gizmoscale.h"

#include "qtbtconvert.h"

#include <btBulletDynamicsCommon.h>


class SceneView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SceneView(QWidget *parent = 0);
    ~SceneView();

    void addGeo(GLuint type,QVector3D pos,QVector3D rot,QVector3D scale,bool selectable,bool staticity);

    //picking
    int selectHit(GLint hits,GLuint *buffer);
    int getGeoSize(){return scene.size();}
    int getSelected(){return m_selected;}

    //selected geometry data
    void setSelectedPos(GLint id, QVector3D pos){scene[id]->setPos(pos);}
    void setSelectedRot(GLint id, QVector3D rot){scene[id]->setRot(rot);}
    void setSelectedScale(GLint id, QVector3D scale){scene[id]->setScale(scale);}
    void setSelectedDensity(GLint id, GLfloat density){scene[id]->setDensity(density);}
    void setSelectedStaticity(GLuint id, bool staticity){scene[id]->setStaticity(staticity);}

    QVector3D getSelectedPos(GLint id){return scene[id]->getModelPos();}
    QVector3D getSelectedRot(GLint id){return scene[id]->getModelRot();}
    QVector3D getSelectedScale(GLint id){return scene[id]->getModelScale();}
    GLfloat getSelectedDensity(GLint id){return scene[id]->getDensity();}
    bool getSelectedStaticity(GLint id){return scene[id]->getStaticity();}

    //select mode (0:pos 1:rot 2:scale 3:select)
    void setMode(GLuint mode){m_mode = mode;}

    //simulation
    void setSimulate(bool sim){m_simulate = sim;}
    void setGravity(QVector3D gravity);
    QVector3D getGravity(){return m_gravity;}

    void sim_init();
    void sim_reset();

signals:
    void clicked();

protected:
    //key and mouse
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e)override;
    void timerEvent(QTimerEvent *e) override;

    //opengl
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    //gizmo and its picking
    void drawGizmos();
    void drawGizmosForPicking(GLfloat sx, GLfloat sy);

    //bullet simulation
    void sim();
    void sim_apply();

private:
    QBasicTimer timer;
    bool m_simulate = false;

    //scene
    QVector<Geometry*> scene;

    //necessary geometry
    Camera camera;
    CoordinateLines *m_lines = nullptr;
    GizmoGeo *m_gizmogeo = nullptr;
    QVector<GizmoGeo*> m_gizmo_translate;
    QVector<GizmoGeo*> m_gizmo_rotate;
    QVector<GizmoGeo*> m_gizmo_scale;

    //key and mouse
    QVector2D m_mousePosition;
    int m_mousebutton = 0;
    bool m_moving = false;
    bool m_space = false;

    //display
    int m_height;
    int m_width;

    //selection
    int m_selected = 0;
    int m_mode = 3; //translate:0 rotate:1 scale:2 select:3
    int m_selected_gizmo_axis = -1; //x:0 y:1 z:2

    //Bullet Physics
    btDefaultCollisionConfiguration* m_collisionConfig = nullptr;
    btCollisionDispatcher* m_dispatcher = nullptr;
    btBroadphaseInterface* m_overlappingPairCache = nullptr;
    btSequentialImpulseConstraintSolver* m_solver = nullptr;
    btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;

    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    QVector3D m_gravity = {0.0,-9.8,0.0};

};

#endif // SCENEVIEW_H
