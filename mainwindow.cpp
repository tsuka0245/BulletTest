#include "mainwindow.h"
#include "ui_mainwindow.h"

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);
    connect(ui->viewer,&SceneView::clicked,
            this,&mainwindow::show_attrib);
}

mainwindow::~mainwindow()
{
    delete ui;
}

void mainwindow::on_AddBox_clicked()
{
    ui->viewer->addGeo(0,{0,1,0},{0,0,0},{1,1,1},1,0);
}

void mainwindow::on_AddGrid_clicked()
{
    ui->viewer->addGeo(2,{0,1,0},{0,0,0},{1,1,1},1,0);
}

void mainwindow::on_AddSphere_clicked()
{
    ui->viewer->addGeo(3,{0,1,0},{0,0,0},{1,1,1},1,0);
}

void mainwindow::on_AddGoal_clicked()
{

}

void mainwindow::show_attrib()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;
    QString name = ui->viewer->getSelectedName(geo_id);
    GLfloat pos_x = ui->viewer->getSelectedPos(geo_id).x();
    GLfloat pos_y = ui->viewer->getSelectedPos(geo_id).y();
    GLfloat pos_z = ui->viewer->getSelectedPos(geo_id).z();
    GLfloat rot_x = ui->viewer->getSelectedRot(geo_id).x();
    GLfloat rot_y = ui->viewer->getSelectedRot(geo_id).y();
    GLfloat rot_z = ui->viewer->getSelectedRot(geo_id).z();
    GLfloat scale_x = ui->viewer->getSelectedScale(geo_id).x();
    GLfloat scale_y = ui->viewer->getSelectedScale(geo_id).y();
    GLfloat scale_z = ui->viewer->getSelectedScale(geo_id).z();
    GLfloat density = ui->viewer->getSelectedDensity(geo_id);
    bool staticity = ui->viewer->getSelectedStaticity(geo_id);

    ui->geo_name->setText(name);
    ui->pos_x->setText(QString::number(pos_x));
    ui->pos_y->setText(QString::number(pos_y));
    ui->pos_z->setText(QString::number(pos_z));
    ui->rot_x->setText(QString::number(rot_x));
    ui->rot_y->setText(QString::number(rot_y));
    ui->rot_z->setText(QString::number(rot_z));
    ui->scale_x->setText(QString::number(scale_x));
    ui->scale_y->setText(QString::number(scale_y));
    ui->scale_z->setText(QString::number(scale_z));
    ui->density->setText(QString::number(density));
    ui->check_static->setChecked(staticity);

    if(m_constraint1 == true)
    {
        GLint num = ui->viewer->getSelected();
        ui->p2p_con_geo1->setText(QString::number(num));
        m_constraint1 = false;
    }
    if(m_constraint2 == true)
    {
        GLuint num = ui->viewer->getSelected();
        ui->p2p_con_geo2->setText(QString::number(num));
        m_constraint2 = false;
    }
}

void mainwindow::on_geo_name_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    QString new_name = ui->geo_name->text();
    ui->viewer->setSelectedName(geo_id,new_name);
}

void mainwindow::on_pos_x_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_pos_x = ui->pos_x->text().toFloat();
    QVector3D pos = ui->viewer->getSelectedPos(geo_id);
    pos.setX(new_pos_x);
    ui->viewer->setSelectedPos(geo_id,pos);
}

void mainwindow::on_pos_y_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_pos_y = ui->pos_y->text().toFloat();
    QVector3D pos = ui->viewer->getSelectedPos(geo_id);
    pos.setY(new_pos_y);
    ui->viewer->setSelectedPos(geo_id,pos);
}

void mainwindow::on_pos_z_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_pos_z = ui->pos_z->text().toFloat();
    QVector3D pos = ui->viewer->getSelectedPos(geo_id);
    pos.setZ(new_pos_z);
    ui->viewer->setSelectedPos(geo_id,pos);
}

void mainwindow::on_rot_x_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_rot_x = ui->rot_x->text().toFloat();
    QVector3D rot = ui->viewer->getSelectedRot(geo_id);
    rot.setX(new_rot_x);
    ui->viewer->setSelectedRot(geo_id,rot);
}

void mainwindow::on_rot_y_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_rot_y = ui->rot_y->text().toFloat();
    QVector3D rot = ui->viewer->getSelectedRot(geo_id);
    rot.setY(new_rot_y);
    ui->viewer->setSelectedRot(geo_id,rot);
}

void mainwindow::on_rot_z_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_rot_z = ui->rot_z->text().toFloat();
    QVector3D rot = ui->viewer->getSelectedRot(geo_id);
    rot.setZ(new_rot_z);
    ui->viewer->setSelectedRot(geo_id,rot);
}

void mainwindow::on_scale_x_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_scale_x = ui->scale_x->text().toFloat();
    QVector3D scale = ui->viewer->getSelectedScale(geo_id);
    scale.setX(new_scale_x);
    ui->viewer->setSelectedScale(geo_id,scale);
}

void mainwindow::on_scale_y_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_scale_y = ui->scale_y->text().toFloat();
    QVector3D scale = ui->viewer->getSelectedScale(geo_id);
    scale.setY(new_scale_y);
    ui->viewer->setSelectedScale(geo_id,scale);
}

void mainwindow::on_scale_z_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_scale_z = ui->scale_z->text().toFloat();
    QVector3D scale = ui->viewer->getSelectedScale(geo_id);
    scale.setZ(new_scale_z);
    ui->viewer->setSelectedScale(geo_id,scale);
}

void mainwindow::on_translate_clicked()
{
    ui->viewer->setMode(0);
}

void mainwindow::on_rotate_clicked()
{
    ui->viewer->setMode(1);
}

void mainwindow::on_scale_clicked()
{
    ui->viewer->setMode(2);
}

void mainwindow::on_select_clicked()
{
    ui->viewer->setMode(3);
}

void mainwindow::on_gravity_x_editingFinished()
{
    GLfloat new_gravity_x = ui->gravity_x->text().toFloat();
    QVector3D gravity = ui->viewer->getGravity();
    gravity.setX(new_gravity_x);
    ui->viewer->setGravity(gravity);
}

void mainwindow::on_gravity_y_editingFinished()
{
    GLfloat new_gravity_y = ui->gravity_y->text().toFloat();
    QVector3D gravity = ui->viewer->getGravity();
    gravity.setY(new_gravity_y);
    ui->viewer->setGravity(gravity);
}

void mainwindow::on_gravity_z_editingFinished()
{
    GLfloat new_gravity_z = ui->gravity_z->text().toFloat();
    QVector3D gravity = ui->viewer->getGravity();
    gravity.setZ(new_gravity_z);
    ui->viewer->setGravity(gravity);
}

void mainwindow::on_density_editingFinished()
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    GLfloat new_density = ui->density->text().toFloat();
    ui->viewer->setSelectedDensity(geo_id,new_density);
}

void mainwindow::on_check_static_stateChanged(int arg1)
{
    GLint geo_id = ui->viewer->getSelected();
    if(geo_id <= 0)return;

    if(arg1 == 0) ui->viewer->setSelectedStaticity(geo_id,false);
    else ui->viewer->setSelectedStaticity(geo_id,true);
}

void mainwindow::on_Run_clicked()
{
    ui->Stop->setEnabled(true);
    ui->Run->setEnabled(false);
    ui->AddBox->setEnabled(false);
    ui->AddGrid->setEnabled(false);
    ui->AddSphere->setEnabled(false);
    ui->AddGoal->setEnabled(false);
    ui->geo_name->setEnabled(false);
    ui->pos_x->setEnabled(false);
    ui->pos_y->setEnabled(false);
    ui->pos_z->setEnabled(false);
    ui->rot_x->setEnabled(false);
    ui->rot_y->setEnabled(false);
    ui->rot_z->setEnabled(false);
    ui->scale_x->setEnabled(false);
    ui->scale_y->setEnabled(false);
    ui->scale_z->setEnabled(false);
    ui->density->setEnabled(false);
    ui->check_static->setEnabled(false);

    ui->viewer->setSimulate(true);
    ui->viewer->sim_init();
}

void mainwindow::on_Stop_clicked()
{
    ui->Stop->setEnabled(false);
    ui->Run->setEnabled(true);
    ui->AddBox->setEnabled(true);
    ui->AddGrid->setEnabled(true);
    ui->AddSphere->setEnabled(true);
    ui->AddGoal->setEnabled(true);
    ui->geo_name->setEnabled(true);
    ui->pos_x->setEnabled(true);
    ui->pos_y->setEnabled(true);
    ui->pos_z->setEnabled(true);
    ui->rot_x->setEnabled(true);
    ui->rot_y->setEnabled(true);
    ui->rot_z->setEnabled(true);
    ui->scale_x->setEnabled(true);
    ui->scale_y->setEnabled(true);
    ui->scale_z->setEnabled(true);
    ui->density->setEnabled(true);
    ui->check_static->setEnabled(true);

    ui->viewer->setSimulate(false);
    ui->viewer->sim_reset();
}

void mainwindow::on_p2p_con_select_geo1_clicked()
{
    ui->viewer->setMode(3);
    m_constraint1 = true;
}

void mainwindow::on_p2p_con_select_geo2_clicked()
{
    ui->viewer->setMode(3);
    m_constraint2 = true;
}

void mainwindow::on_p2p_con_apply_clicked()
{
    GLuint geo1 = ui->p2p_con_geo1->text().toUInt();
    GLuint geo2 = ui->p2p_con_geo2->text().toUInt();

    ui->viewer->setP2PCon(geo1,geo2);

}
