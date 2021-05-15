#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization/q3dscatter.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QTimer;
QT_END_NAMESPACE

using namespace QtDataVisualization;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QWidget *getWidgetContent();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;
    QTimer *m_timer;
    QTimer *m_rotationTimer;
    Q3DScatter *m_graph;
};
#endif // MAINWINDOW_H
