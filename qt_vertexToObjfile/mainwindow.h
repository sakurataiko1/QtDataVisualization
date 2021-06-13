#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_makePlane_clicked();
    void on_pushButton_makeRect_clicked();
    void func_getPointOfMesh_plane(QVector3D in_pointBL, QVector3D in_pointTR, int surfaceNo, int input_MateNo, int in_surfaceCnt);//kuroda　もとのgetPointOfMesh 関数から変更

    void func_objfile_write(int input_MateNo, int in_surfaceCnt, QString in_msg1,  QVector3D in_vertex1, QVector3D in_vertex2, QVector3D in_vertex3, QVector3D in_vertex4); //objファイル書き込み


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
