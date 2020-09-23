#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private slots:
    void on_AddBox_clicked();
    void on_AddGrid_clicked();
    void on_AddSphere_clicked();
    void on_AddGoal_clicked();

    void show_attrib();
    void on_geo_name_editingFinished();
    void on_pos_x_editingFinished();
    void on_pos_y_editingFinished();
    void on_pos_z_editingFinished();
    void on_rot_x_editingFinished();
    void on_rot_y_editingFinished();
    void on_rot_z_editingFinished();
    void on_scale_x_editingFinished();
    void on_scale_y_editingFinished();
    void on_scale_z_editingFinished();

    void on_translate_clicked();
    void on_rotate_clicked();
    void on_scale_clicked();
    void on_select_clicked();
    void on_gravity_x_editingFinished();
    void on_gravity_y_editingFinished();
    void on_gravity_z_editingFinished();
    void on_density_editingFinished();

    void on_check_static_stateChanged(int arg1);

    void on_p2p_con_select_geo1_clicked();
    void on_p2p_con_select_geo2_clicked();
    void on_p2p_con_apply_clicked();

    void on_Run_clicked();
    void on_Stop_clicked();





private:
    Ui::mainwindow *ui;

    bool m_constraint1 = false;
    bool m_constraint2 = false;
};
#endif // MAINWINDOW_H
