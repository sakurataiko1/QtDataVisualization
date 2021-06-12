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

//-start- objfile-customitems
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtCore/QPropertyAnimation>
//-end- objfile-customitems

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

    void func_onCheckBox1_3Ddraw_objfile(bool show); //3D描画でobjファイル描画
    void func_3Ddraw_objfile(QString in_objfilepath, QVector3D in_position, QColor in_color, int flag_toumei ); //objファイルを3D画面に描画する

private slots:
    void on_rotationSliderX_valueChanged(int value); //視点移動
    void on_rotationSliderY_valueChanged(int value); //視点移動

    //不要?objfile-custominput//void handleElementSelected(QAbstract3DGraph::ElementType type);
    //不要?objfile-custominput//void resetSelection();

    void on_checkBox1_stateChanged(int arg1);

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
