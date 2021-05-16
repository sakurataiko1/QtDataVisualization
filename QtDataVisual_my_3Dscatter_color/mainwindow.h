#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization/q3dscatter.h>

//#include <QtDataVisualization/q3dscatter.h>
#include <QtGui/QFont>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSequentialAnimationGroup>
#include <QtGui/QVector3D>

//-start- Qt-official-sample bars
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtGui/QFont>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QPointer>
#include <QtCore/QPropertyAnimation>
//-end- Qt-official-sample bars

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
//class QTimer;
QT_END_NAMESPACE

using namespace QtDataVisualization;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QWidget *getWidgetContent();

    void addData(); //Qt_Official_sample:custominput

    //-start- 視点移動
    void changePresetCamera();
    void rotateX(int rotation);
    void rotateY(int rotation);
    void func_GUIdefault();//kuroda GUIフォームの初期化
    //-end- 視点移動

    // グラフ(散布図）に色を変えてポイント座標を追加する
    void func_addGraphpoint(QVector<QVector3D> itemList, QString in_color); //グラフ(散布図）に色を変えてポイント座標を追加する
    //

private slots:
    void on_rotationSliderX_valueChanged(int value);

    void on_rotationSliderY_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    Q3DScatter *m_graph;

    float m_xRotation; //視点移動
    float m_yRotation;
    QPropertyAnimation m_animationCameraX;
    QPropertyAnimation m_animationCameraY;
    QPropertyAnimation m_animationCameraZoom;
    QPropertyAnimation m_animationCameraTarget;
    float m_defaultAngleX;
    float m_defaultAngleY;
    float m_defaultZoom;
    QVector3D m_defaultTarget;

};
#endif // MAINWINDOW_H
