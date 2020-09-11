#include "sceneview.h"
#include <QOpenGLFunctions_1_1>

#include <QDebug>

SceneView::SceneView(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);

    //bullet physics initialize
    m_collisionConfig = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,
                                                  m_overlappingPairCache,
                                                  m_solver,
                                                  m_collisionConfig);
    m_dynamicsWorld->setGravity(qv3tobtv3(m_gravity));
}

SceneView::~SceneView()
{
    //delete gizmo
    for(int i=0;i<3;i++)
    {
        delete m_gizmo_translate[i];
        delete m_gizmo_rotate[i];
        delete m_gizmo_scale[i];
    }
    m_gizmo_translate.clear();
    m_gizmo_rotate.clear();
    m_gizmo_scale.clear();

    //delete coordinate lines
    delete m_lines;

    //delete geometry
    for(int i=0;i<scene.size();i++)delete scene[i];

    //clear scene data
    scene.clear();

    //clear collision shapes ary
    collisionShapes.clear();

    //delete dynamics world and its related component
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_collisionConfig;
}

void SceneView::addGeo(GLuint type,QVector3D pos,QVector3D rot,QVector3D scale,bool selectable,bool staticity)
{
    Geometry* geo;
    switch(type)
    {
        case 0: //cube
        {
            geo = new CubeGeo(pos,rot,scale,selectable,staticity);

            //collision
            collisionShapes.push_back(geo->getCollisionShape());
            m_dynamicsWorld->addRigidBody(geo->getRigidBody());

            scene.append(geo);
            break;
        }
        case 1: //plane
        {
            geo = new PlaneGeo(pos,rot,scale,selectable,staticity);

            //collision
            collisionShapes.push_back(geo->getCollisionShape());
            m_dynamicsWorld->addRigidBody(geo->getRigidBody());

            scene.append(geo);
            break;
        }
        case 2: //grid
        {
            geo = new GridGeo(pos,rot,scale,selectable,staticity);

            //collision
            collisionShapes.push_back(geo->getCollisionShape());
            m_dynamicsWorld->addRigidBody(geo->getRigidBody());

            scene.append(geo);
            break;
        }
        case 3: //sphere
        {
            geo = new SphereGeo(pos,rot,scale,selectable,staticity);

            //collision
            collisionShapes.push_back(geo->getCollisionShape());
            m_dynamicsWorld->addRigidBody(geo->getRigidBody());

            scene.append(geo);
            break;
        }
    }
}

int SceneView::selectHit(GLint hits, GLuint *buffer)
{
    if(hits<=0) return -1;

    GLuint *ptr;
    ptr = (GLuint*)buffer;

    GLuint depth_min = ptr[2];
    GLuint selectedGeoID = ptr[3];
    for(int i=0;i<hits;i++)
    {
        if(ptr[4*i+2] < depth_min)
        {
            depth_min = ptr[4*i+2];
            selectedGeoID = ptr[4*i+3];
        }
    }

    return selectedGeoID;
}

void SceneView::setGravity(QVector3D gravity)
{
    m_gravity = gravity;
    m_dynamicsWorld->setGravity(qv3tobtv3(m_gravity));
}

void SceneView::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Space)m_space = true;

    //short cut key
    if(e->key() == Qt::Key_Q) m_mode = 3;
    if(e->key() == Qt::Key_W) m_mode = 0;
    if(e->key() == Qt::Key_E) m_mode = 1;
    if(e->key() == Qt::Key_R) m_mode = 2;
}

void SceneView::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Space)m_space = false;
}

void SceneView::mousePressEvent(QMouseEvent *e)
{
    //mouse position
    m_mousePosition = QVector2D(e->localPos());

    if(m_space == true)m_moving = true;

    //set button type
    if(e->button() == Qt::LeftButton)m_mousebutton = 0;
    else if(e->button() == Qt::MidButton)m_mousebutton = 1;
    else if(e->button() == Qt::RightButton)m_mousebutton = 2;
    //select mode
    if(m_space != true && m_mousebutton == 0)
    {
        if(m_mode == 3) //object picking
        {
            GLuint bufmax = 100;
            GLuint select_buffer[bufmax];
            GLint view[4];
            glGetIntegerv(GL_VIEWPORT,view);
            glSelectBuffer(bufmax,select_buffer);
            (void)glRenderMode(GL_SELECT);
            glInitNames();

            //get screen coordinate
            GLfloat sx = 2*m_mousePosition.x()/m_width-1;
            GLfloat sy = 1-2*m_mousePosition.y()/m_height;

            //draw geometry
            GLuint i = 0;
            for(QVector<Geometry*>::iterator itr = scene.begin();itr != scene.end();itr++)
            {
                if((*itr)->getSelectable())
                {
                    glPushName(i);
                    (*itr)->drawGeo(camera.getPickingProjectionMat(m_width,m_height,sx,sy),camera.getViewMat());
                    glPopName();
                }
                i++;
            }

            GLint hit = glRenderMode(GL_RENDER);
            if(m_selected > 0)scene[m_selected]->setSelected(0);
            m_selected = selectHit(hit,select_buffer);
            if(m_selected > 0)scene[m_selected]->setSelected(1);

            emit clicked();
        }
        else //gizmo picking
        {
            GLuint bufmax = 100;
            GLuint select_buffer[bufmax];
            GLint view[4];
            glGetIntegerv(GL_VIEWPORT,view);
            glSelectBuffer(bufmax,select_buffer);
            (void)glRenderMode(GL_SELECT);
            glInitNames();

            //get screen coordinate
            GLfloat sx = 2*m_mousePosition.x()/m_width-1;
            GLfloat sy = 1-2*m_mousePosition.y()/m_height;

            //draw gizmo
            drawGizmosForPicking(sx,sy);

            GLint hit = glRenderMode(GL_RENDER);
            m_selected_gizmo_axis = selectHit(hit,select_buffer);
        }
    }
}

void SceneView::mouseMoveEvent(QMouseEvent *e)
{

    QVector2D diff = QVector2D(e->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(e->localPos());

    //camera transform
    if(m_space == true)
    {
        switch(m_mousebutton)
        {
            //left
            case 0:
            {
                QVector3D rot;
                GLfloat amp = 0.25;
                rot.setX(diff.y()*amp);
                rot.setY(diff.x()*amp);
                rot.setZ(0.0);
                rot += camera.getRot();
                camera.setRot(rot);
                break;
            }

            //mid
            case 1:
            {
                QVector3D cam_right;
                QVector3D cam_up = camera.getUp();
                QVector3D cam_front = camera.getFront();
                cam_right = QVector3D::crossProduct(cam_up,cam_front);
                QVector3D translate = - cam_right * diff.x()
                                      + cam_up * diff.y();
                GLfloat amp = 0.1;
                QVector3D pos = camera.getPos() + translate * amp;
                camera.setPos(pos);
                break;
            }

            //right
            case 2:
            {
                QVector3D cam_front = camera.getFront();
                QVector3D translate = - cam_front * diff.x()
                                      - cam_front * diff.y();
                GLfloat amp = 0.1;
                QVector3D pos = camera.getPos() + translate * amp;
                camera.setPos(pos);
                break;
            }
        }
    }

    //selected object transform
    if(m_space != true && m_selected_gizmo_axis != -1)
    {
        switch(m_mode)
        {
            case 0: //translate
            {
                QVector3D cam_right = camera.getRight() * diff.x() * 0.05;
                QVector3D cam_up = camera.getUp() * diff.y() * 0.05;
                QVector3D axis = {0,0,0};
                axis[m_selected_gizmo_axis] = 1;

                QVector3D rot = scene[m_selected]->getModelRot();
                QMatrix4x4 rot_mat;
                rot_mat.setToIdentity();
                rot_mat.rotate(rot.x(),{1,0,0});
                rot_mat.rotate(rot.y(),{0,1,0});
                rot_mat.rotate(rot.z(),{0,0,1});
                axis = rot_mat * axis;

                GLfloat r_dis = QVector3D::dotProduct(cam_right,axis);
                GLfloat u_dis = QVector3D::dotProduct(cam_up,axis);

                QVector3D pos_diff = {0,0,0};
                pos_diff += (r_dis - u_dis)*axis;

                QVector3D pos = scene[m_selected]->getModelPos() + pos_diff;
                scene[m_selected]->setPos(pos);

                emit clicked();
                break;
            }
            case 1: //rotate
            {
                QVector3D cam_right = camera.getRight() * diff.x() * 0.5;
                QVector3D cam_up = camera.getUp() * diff.y() * 0.5;
                QVector3D axis = {0,0,0};
                axis[m_selected_gizmo_axis] = 1;
                axis = scene[m_selected]->getModelQuaterion().rotatedVector(axis);

                QVector3D raxis = {0,0,0};
                raxis[(m_selected_gizmo_axis+1)%3] = 1;
                GLfloat r_dis = QVector3D::dotProduct(cam_right,raxis);
                GLfloat u_dis = QVector3D::dotProduct(cam_up,raxis);
                GLfloat angle = r_dis + u_dis;

                QQuaternion rot = QQuaternion::fromAxisAndAngle(axis,angle);
                QQuaternion new_q = rot * scene[m_selected]->getModelQuaterion();
                scene[m_selected]->setRot(new_q);

                emit clicked();
                break;
            }
            case 2: //scale
            {
                QVector3D cam_right = camera.getRight() * diff.x() * 0.05;
                QVector3D cam_up = camera.getUp() * diff.y() * 0.05;
                QVector3D axis = {0,0,0};
                axis[m_selected_gizmo_axis] = 1;
                GLfloat r_dis = QVector3D::dotProduct(cam_right,axis);
                GLfloat u_dis = QVector3D::dotProduct(cam_up,axis);

                QVector3D scale_diff = {0,0,0};
                scale_diff[m_selected_gizmo_axis] += r_dis - u_dis;
                QVector3D scale = scene[m_selected]->getModelScale() + scale_diff;
                scene[m_selected]->setScale(scale);

                emit clicked();
                break;
            }
        }
    }
}

void SceneView::mouseReleaseEvent(QMouseEvent*)
{
    m_moving = false;
    m_selected_gizmo_axis = -1;
}

void SceneView::wheelEvent(QWheelEvent *e)
{
    //zoom in/out
    if(m_space == true)
    {
        GLfloat fov = camera.getFov() - e->angleDelta().y()/40;
        if(fov < 0.0)fov = 0.0;
        if(fov > 89.0)fov = 89.0;
        camera.setFov(fov);
    }
}

void SceneView::timerEvent(QTimerEvent *)
{
    if(m_simulate == true)
    {
        sim();
        sim_apply();
    }
    update();
}

void SceneView::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2,0.2,0.2,1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    timer.start(12,this);

    //create reference grid
    addGeo(1,{0,0,0},{0,0,0},{10,1,10},0,1);
    //create coordinate lines
    m_lines = new CoordinateLines;
    //create translate gizmo
    for(int i=0;i<3;i++)
    {
        m_gizmogeo = new GizmoTranslate(i);
        m_gizmo_translate.append(m_gizmogeo);
        m_gizmogeo = new GizmoRotate(i);
        m_gizmo_rotate.append(m_gizmogeo);
        m_gizmogeo = new GizmoScale(i);
        m_gizmo_scale.append(m_gizmogeo);
    }
}

void SceneView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw geometry
    for(QVector<Geometry*>::iterator itr = scene.begin();itr != scene.end();itr++)
    {
        (*itr)->drawGeo(camera.getProjectionMat(m_width,m_height),camera.getViewMat());
    }

    //draw manip lines
    drawGizmos();

    //draw coordinate lines
    m_lines->drawGeo(camera.getProjectionMat(m_width,m_height),camera.getViewMatForCoordinate());

}

void SceneView::resizeGL(int w, int h)
{
    m_height = h;
    m_width = w;
}

void SceneView::drawGizmos()
{
    if(m_selected <= 0)return;
    switch(m_mode)
    {
    case 0: //gizmo for translate
        for(QVector<GizmoGeo*>::iterator itr = m_gizmo_translate.begin();itr != m_gizmo_translate.end();itr++)
        {
            (*itr)->setRot(getSelectedRot(getSelected()));
            (*itr)->setPos(getSelectedPos(getSelected()));
            (*itr)->drawGeo(camera.getProjectionMat(m_width,m_height),camera.getViewMat());
        }
        return;
    case 1: //gizmo for rotate
        for(QVector<GizmoGeo*>::iterator itr = m_gizmo_rotate.begin();itr != m_gizmo_rotate.end();itr++)
        {

            (*itr)->setRot(getSelectedRot(getSelected()));
            (*itr)->setPos(getSelectedPos(getSelected()));
            (*itr)->drawGeo(camera.getProjectionMat(m_width,m_height),camera.getViewMat());
        }
        return;
    case 2: //gizmo for scale
        for(QVector<GizmoGeo*>::iterator itr = m_gizmo_scale.begin();itr != m_gizmo_scale.end();itr++)
        {
            (*itr)->setRot(getSelectedRot(getSelected()));
            (*itr)->setPos(getSelectedPos(getSelected()));
            (*itr)->drawGeo(camera.getProjectionMat(m_width,m_height),camera.getViewMat());
        }
        return;
    default:
        return;
    }
}

void SceneView::drawGizmosForPicking(GLfloat sx, GLfloat sy)
{
    if(m_selected <= 0)return;

    GLuint i = 0;
    switch(m_mode)
    {
    case 0: //gizmo for translate
        for(QVector<GizmoGeo*>::iterator itr = m_gizmo_translate.begin();itr != m_gizmo_translate.end();itr++)
        {
            glPushName(i);
            (*itr)->setRot(getSelectedRot(getSelected()));
            (*itr)->setPos(getSelectedPos(getSelected()));
            (*itr)->setPicking(1);
            (*itr)->drawGeo(camera.getPickingProjectionMat(m_width,m_height,sx,sy),camera.getViewMat());
            glPopName();
            i++;
        }
        return;
    case 1: //gizmo for rotate
        for(QVector<GizmoGeo*>::iterator itr = m_gizmo_rotate.begin();itr != m_gizmo_rotate.end();itr++)
        {
            glPushName(i);
            (*itr)->setRot(getSelectedRot(getSelected()));
            (*itr)->setPos(getSelectedPos(getSelected()));
            (*itr)->setPicking(1);
            (*itr)->drawGeo(camera.getPickingProjectionMat(m_width,m_height,sx,sy),camera.getViewMat());
            glPopName();
            i++;
        }
        return;
    case 2: //gizmo for scale
        for(QVector<GizmoGeo*>::iterator itr = m_gizmo_scale.begin();itr != m_gizmo_scale.end();itr++)
        {
            glPushName(i);
            (*itr)->setRot(getSelectedRot(getSelected()));
            (*itr)->setPos(getSelectedPos(getSelected()));
            (*itr)->setPicking(1);
            (*itr)->drawGeo(camera.getPickingProjectionMat(m_width,m_height,sx,sy),camera.getViewMat());
            glPopName();
            i++;
        }
        return;
    }
}

void SceneView::sim()
{
    m_dynamicsWorld->stepSimulation(0.012,10);
    emit clicked();
}

void SceneView::sim_init()
{
    //send geometry data to rigidbody
    for(int i=0;i<scene.size();i++)
    {
        //keep initial position
        scene[i]->setInitPos(scene[i]->getModelPos());
        scene[i]->setInitRot(scene[i]->getModelQuaterion());

        btTransform trans;
        trans.setOrigin(qv3tobtv3(scene[i]->getModelPos()));
        trans.setRotation(qqtobtq(scene[i]->getModelQuaterion()));

        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        btMotionState* mState = body->getMotionState();
        btCollisionShape* colShape = body->getCollisionShape();

        mState->setWorldTransform(trans);
        colShape->setLocalScaling(qv3tobtv3(scene[i]->getModelScale()));
        body->setMotionState(mState);
        body->activate(true);
    }
}

void SceneView::sim_apply()
{
    //apply simulation data to geometry
    for(int i=0;i<m_dynamicsWorld->getNumCollisionObjects();i++)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        body->getMotionState()->getWorldTransform(trans);

        scene[i]->setPos(btv3toqv3(trans.getOrigin()));
        scene[i]->setRot(btqtoqq(trans.getRotation()));
    }
}

void SceneView::sim_reset()
{
    //reset simulaton
    for(int i=0;i<scene.size();i++)
    {
        scene[i]->setPos(scene[i]->getInitModelPos());
        scene[i]->setRot(scene[i]->getInitModelQuaternion());

        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        btMotionState* mState = body->getMotionState();

        body->clearForces();
        btVector3 zeroVec = {0.0,0.0,0.0};
        body->setLinearVelocity(zeroVec);
        body->setAngularVelocity(zeroVec);

        btTransform initTrans;
        initTrans.setOrigin(qv3tobtv3(scene[i]->getInitModelPos()));
        initTrans.setRotation(qqtobtq(scene[i]->getInitModelQuaternion()));

        mState->setWorldTransform(initTrans);
        body->setMotionState(mState);
    }
}
