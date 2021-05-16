#include "mainwindow.h"
#include "ui_mainwindow.h"

//参考：
//・3Dグラフ表示　座標ファイルを読み込んで散布図(scatterとして表示)　（マウス操作：ズームあり、視点移動なし）
//https://github.com/fengfanchen/Qt/tree/master/3D%20move%20rotation
//
//・参考：Qt付属のExampleコード　customplot(CSVファイルデータ入力でグラフ表示), bars　（マウス操作：ズームあり、視点移動あり）

#include <QtDataVisualization/QScatter3DSeries>
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <QHBoxLayout>

//-start- Qt_Official_sample:custominput(CSVファイルデータ入力でグラフ表示) , bars(視点移動)
#include <QtDataVisualization/QScatterDataProxy>
#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DScene>
#include <QtDataVisualization/Q3DCamera>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/Q3DTheme>
#include <QtCore/qmath.h>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
//-end- Qt_Official_sample:custominput

using namespace QtDataVisualization;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_xRotation(0.0f)
    , m_yRotation(0.0f)
{
    ui->setupUi(this);
    this->setWindowTitle("3D scatterGraph . QtDataVisualization");
    func_GUIdefault(); //GUIフォーム初期化

    m_graph = new Q3DScatter();
    m_graph->axisX()->setRange(-10, 10);
    m_graph->axisY()->setRange(-10, 10);
    m_graph->axisZ()->setRange(-10, 10);
    QWidget *container = QWidget::createWindowContainer(m_graph);

    QScatter3DSeries *series = new QScatter3DSeries;
    series->setItemLabelFormat(QStringLiteral("@xLabel, @yLabel, @zLabel"));
    series->setMesh(QAbstract3DSeries::MeshCube);
    series->setItemSize(0.15f);
    m_graph->addSeries(series); // m_graph->seriesList().at(0) の作成

    if (!m_graph->hasContext()) {

        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return;
    }

    addData(); //csvファイルからデータを読み込んで追加

    //设置到控件上　コントロールに設定→ Widgetに登録、画面で表示されるようになる
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(container, 1);
    ui->widget->setLayout(hLayout);

    //-start- 視点移動ができるように設定 Qt-Official-sample bars
    changePresetCamera();
    // Set up property animations for zooming to the selected bar
    Q3DCamera *camera = m_graph->scene()->activeCamera();
    m_defaultAngleX = camera->xRotation();
    m_defaultAngleY = camera->yRotation();
    m_defaultZoom = camera->zoomLevel();
    m_defaultTarget = camera->target();

    m_animationCameraX.setTargetObject(camera);
    m_animationCameraY.setTargetObject(camera);
    m_animationCameraZoom.setTargetObject(camera);
    m_animationCameraTarget.setTargetObject(camera);

    m_animationCameraX.setPropertyName("xRotation");
    m_animationCameraY.setPropertyName("yRotation");
    m_animationCameraZoom.setPropertyName("zoomLevel");
    m_animationCameraTarget.setPropertyName("target");

    int duration = 1700;
    m_animationCameraX.setDuration(duration);
    m_animationCameraY.setDuration(duration);
    m_animationCameraZoom.setDuration(duration);
    m_animationCameraTarget.setDuration(duration);

    // The zoom always first zooms out above the graph and then zooms in
    qreal zoomOutFraction = 0.3;
    m_animationCameraX.setKeyValueAt(zoomOutFraction, QVariant::fromValue(0.0f));
    m_animationCameraY.setKeyValueAt(zoomOutFraction, QVariant::fromValue(90.0f));
    m_animationCameraZoom.setKeyValueAt(zoomOutFraction, QVariant::fromValue(50.0f));
    m_animationCameraTarget.setKeyValueAt(zoomOutFraction,
    QVariant::fromValue(QVector3D(0.0f, 0.0f, 0.0f)));
    //-end- 視点移動ができるように設定 Qt-Official-sample bars


}

MainWindow::~MainWindow()
{
    qDebug() << "Widget::~Widget()";
    delete m_graph;
    delete ui;
}


void MainWindow::addData()
{
    QVector<QVector3D> itemList;

    //座標ファイルから読み込んで表示する場合
    // Read data items from the file to QVector
    //QTextStream stream;
    //QFile dataFile(":/data/data.txt");
    //if (dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //    stream.setDevice(&dataFile);
    //    while (!stream.atEnd()) {
    //        QString line = stream.readLine();
    //        if (line.startsWith("#")) // Ignore comments
    //            continue;
    //        QStringList strList = line.split(",", Qt::SkipEmptyParts);
    //        // Each line has three data items: xPos, yPos and zPos value
    //        if (strList.size() < 3) {
    //            qWarning() << "Invalid row read from data:" << line;
    //            continue;
    //        }
    //        itemList.append(QVector3D(
    //                             strList.at(0).trimmed().toFloat(),
    //                             strList.at(1).trimmed().toFloat(),
    //                             strList.at(2).trimmed().toFloat()));
    //    }
    //} else {
    //    qWarning() << "Unable to open data file:" << dataFile.fileName();
    //}
    //QScatterDataArray *dataArray = new QScatterDataArray;
    //dataArray->resize(itemList.count());
    //QScatterDataItem *ptrToDataArray = &dataArray->first();
    //for (int i = 0; i < itemList.count(); i++) {
    //    ptrToDataArray->setPosition(itemList.at(i));
    //    ptrToDataArray++;
    //}
    //m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
    //m_graph->seriesList().at(0)->setBaseColor(Qt::red); //ポイントの色設定


    //座標を直接値入力でグラフ表示させる
    itemList.clear();
    //itemList.append(QVector3D(-10.0,5.01799,-5.04425)); //表示軸　-10～10なので超えた物は表示されない
    itemList.append(QVector3D(-10.0, -1, -2));
    itemList.append(QVector3D(-8.0, -1, -2));
    // Add data from the QVector to datamodel 行、データを生成する
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(itemList.count());
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    for (int i = 0; i < itemList.count(); i++) {
        ptrToDataArray->setPosition(itemList.at(i));
        ptrToDataArray++;
    }
    m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);   
    m_graph->seriesList().at(0)->setBaseColor(Qt::red); //ポイントの色設定 (単一色の場合)
    //ポイントの色設定 (グラデーションカラーにする場合)
    //QLinearGradient linearGrad(QPointF(100, 100), QPointF(200, 200));
    //linearGrad.setColorAt(0, Qt::blue);
    //linearGrad.setColorAt(1, Qt::red);
    //m_graph->seriesList().at(0)->setBaseGradient(linearGrad);
    //m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyle::ColorStyleObjectGradient);

    //2つ目のデータ seriesのまま使いまわしできない。別に用意する必要がある。
    QScatter3DSeries *series2 = new QScatter3DSeries; //既に使っている変数seriesを使いまわしできない。series2として別変数名にしている。
    series2->setItemLabelFormat(QStringLiteral("@xLabel, @yLabel, @zLabel"));
    series2->setMesh(QAbstract3DSeries::MeshCube);
    series2->setItemSize(0.15f);
    m_graph->addSeries(series2); // m_graph->seriesList().at(1) の作成
    itemList.clear();
    itemList.append(QVector3D(5.0, 6, 7));
    itemList.append(QVector3D(4.0, 6, 7));
    // Add data from the QVector to datamodel 行、データを生成する
    QScatterDataArray *dataArray2 = new QScatterDataArray;
    dataArray2->resize(itemList.count());
    QScatterDataItem *ptrToDataArray2 = &dataArray2->first();
    for (int i = 0; i < itemList.count(); i++) {
        ptrToDataArray2->setPosition(itemList.at(i));
        ptrToDataArray2++;
    }
    m_graph->seriesList().at(1)->dataProxy()->resetArray(dataArray2);
    m_graph->seriesList().at(1)->setBaseColor(Qt::green);
    //-end- 2つ目のデータ

    //-start- 3つ目のデータ 座標追加とグラフへの反映
    itemList.clear();
    itemList.append(QVector3D(1.0, 1, 2));
    itemList.append(QVector3D(2.0, 1, 2));
    itemList.append(QVector3D(3.0, 1, 2));
    func_addGraphpoint(itemList, "yellow");

    //-start- 4つ目のデータ 座標追加とグラフへの反映
    itemList.clear();
    itemList.append(QVector3D(1.0, -8, -9));
    itemList.append(QVector3D(2.0, -8, -9));
    itemList.append(QVector3D(3.0, -8, -9));
    itemList.append(QVector3D(4.0, -8, -9));
    func_addGraphpoint(itemList, "blue");

}

//-start-  視点移動 Qt-official-sample bar より流用
void MainWindow::func_addGraphpoint(QVector<QVector3D> in_itemList, QString in_color) //グラフ(散布図）に色を変えてポイント座標を追加する
{
     QVector<QVector3D> itemList = in_itemList;

    //m_graphへのシリーズ追加。 関数化されてない場合は、既に使っている変数seriesを使いまわしできなかった名残で、series1として別変数名にしている。）
    QScatter3DSeries *series1 = new QScatter3DSeries;
    series1->setItemLabelFormat(QStringLiteral("@xLabel, @yLabel, @zLabel"));
    series1->setMesh(QAbstract3DSeries::MeshCube);
    series1->setItemSize(0.15f);
    m_graph->addSeries(series1); // m_graph->seriesList().at(1) の作成

    //座標追加
    QScatterDataArray *dataArray1 = new QScatterDataArray;
    dataArray1->resize(itemList.count());
    QScatterDataItem *ptrTodataArray1 = &dataArray1->first();
    for (int i = 0; i < itemList.count(); i++) {
        ptrTodataArray1->setPosition(itemList.at(i));
        ptrTodataArray1++;
    }
    //m_graph->seriesList().at(1)->dataProxy()->resetArray(dataArray1);
    //m_graph->seriesList().at(1)->setBaseColor(Qt::blue);

    int tmp_id = m_graph->seriesList().count()-1;
    m_graph->seriesList().at(tmp_id)->dataProxy()->resetArray(dataArray1);
    m_graph->seriesList().at(tmp_id)->setBaseColor(Qt::blue);
    if(in_color == "red"){ m_graph->seriesList().at(tmp_id)->setBaseColor(Qt::red); }
    if(in_color == "green"){ m_graph->seriesList().at(tmp_id)->setBaseColor(Qt::green); }
    if(in_color == "yellow"){ m_graph->seriesList().at(tmp_id)->setBaseColor(Qt::yellow); }
}

//-start-  視点移動 Qt-official-sample bar より流用
void MainWindow::changePresetCamera()
{
    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();

    // Restore camera target in case animation has changed it
    m_graph->scene()->activeCamera()->setTarget(QVector3D(0.0f, 0.0f, 0.0f));

    //! [10]
    static int preset = Q3DCamera::CameraPresetFront;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
    //! [10]
}

void MainWindow::on_rotationSliderX_valueChanged(int value)
{
    rotateX(value);
}

void MainWindow::on_rotationSliderY_valueChanged(int value)
{
    rotateY(value);
}

void MainWindow::rotateX(int rotation) //3D視点移動
{
    m_xRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}

void MainWindow::rotateY(int rotation) //3D視点移動
{
    m_yRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}

void MainWindow::func_GUIdefault() //GUIフォームの初期化
{
    //QSlider *rotationSliderX = new QSlider(Qt::Horizontal, widget);
    ui->rotationSliderX->setTickInterval(30);
    ui->rotationSliderX->setTickPosition(QSlider::TicksBelow);
    ui->rotationSliderX->setMinimum(-180);
    ui->rotationSliderX->setValue(0);
    ui->rotationSliderX->setMaximum(180);
    //QSlider *rotationSliderY = new QSlider(Qt::Horizontal, widget);
    ui->rotationSliderY->setTickInterval(15);
    ui->rotationSliderY->setTickPosition(QSlider::TicksAbove);
    ui->rotationSliderY->setMinimum(-90);
    ui->rotationSliderY->setValue(0);
    ui->rotationSliderY->setMaximum(90);
}
//-end-  視点移動 Qt-official-sample bar より流用
