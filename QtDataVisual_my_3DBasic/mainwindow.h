#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization/q3dscatter.h>

//-start- Qt-official-sample bars-視点移動：回転
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtGui/QFont>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QPointer>
#include <QtCore/QPropertyAnimation>
//-end- Qt-official-sample bars-視点移動：回転

//-start- 3Dscatter(サーフェス表示用途)
#include <QtDataVisualization/q3dscatter.h>
#include <QtGui/QFont>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSequentialAnimationGroup>
#include <QtGui/QVector3D>
//-end- 3Dscatter(サーフェス表示用途)

//-start- objfile-customitems (サーフェスグラフや、3Dデータ.objファイル表示用途)
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtCore/QPropertyAnimation>
//-end- objfile-customitems (サーフェスグラフや、3Dデータ.objファイル表示用途)

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

    //-start- 視点移動
    void changePresetCamera();
    void rotateX(int rotation);
    void rotateY(int rotation);
    void func_GUIdefault();//kuroda GUIフォームの初期化
    //-end- 視点移動


private slots:
    void on_rotationSliderX_valueChanged(int value);
    void on_rotationSliderY_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    //Q3DScatter *m_graph;

    //-start- objfile-custominput
    Q3DSurface *m_graph;
    QLabel *m_textField;
    QPropertyAnimation *m_selectionAnimation;
    QCustom3DItem *m_previouslyAnimatedItem;
    QVector3D m_previousScaling;
    //-end- objfile-custominput

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
