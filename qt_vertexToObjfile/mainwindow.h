#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWindow>
#include <QOpenGLVertexArrayObject>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLVertexArrayObject>
#include <QMouseEvent>
#include <QMouseEventTransition>
#include <QOpenGLShader>
#include <QtOpenGL/QtOpenGL>
#include <QVector3D>
#include <QArrayData>
#include <gl/GL.h>
#include <gl/GL.h>
#include <math.h>
#include <QMatrix4x4>

#include <QScopedPointer>
#include <QElapsedTimer>

//#include <mainwindow.h>
//#inlude  <ui_mainwindow.h>
//#include <vertex.h>

#include <QTextCodec>
// For Japanese
# define TR(s) (QString::fromLocal8Bit(s))

#include <QFile> //kuroda
#include <QFileInfo> //kuroda
#include <tuple> //kuroda 2021.06.xx-01 //2つ以上の帰り値を返すため　std::tuple, std::forward_as_tuple, std::tie で使用する
#include <QTextStream>

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

    //void func_getPointOfMesh_plane(QVector3D in_pointBL, QVector3D in_pointTR, int surfaceNo, int input_MateNo, int in_surfaceCnt);//kuroda　もとのgetPointOfMesh 関数から変更
    void func_getPointOfMesh_plane();

    void func_objfile_write(int input_MateNo, int in_surfaceCnt, QString in_msg1,  QVector3D in_vertex1, QVector3D in_vertex2, QVector3D in_vertex3, QVector3D in_vertex4); //objファイル書き込み

    void on_comboBox_plane_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
