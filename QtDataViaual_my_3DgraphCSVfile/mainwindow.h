#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization/q3dscatter.h>

//-start- Qt_Official_sample:custominput
//#include <QtDataVisualization/q3dscatter.h>
#include <QtGui/QFont>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSequentialAnimationGroup>
#include <QtGui/QVector3D>
//-end- Qt_Official_sample:custominput

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


private:
    Ui::MainWindow *ui;
    Q3DScatter *m_graph;
};
#endif // MAINWINDOW_H
