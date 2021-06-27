/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "surfacegraph.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtCore/qmath.h>

#include <QDebug> //add_kuroda
#include <QFile> //add_kuroda
#include <QFileInfo> //add_kuroda
#include <QMessageBox> //add_kuroda

using namespace QtDataVisualization;

const int sampleCountX = 50;
const int sampleCountZ = 50;
const int heightMapGridStepX = 6;
const int heightMapGridStepZ = 6;
const float sampleMin = -8.0f;
const float sampleMax = 8.0f;

SurfaceGraph::SurfaceGraph(Q3DSurface *surface)
    : m_graph(surface)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    //! [0]
    m_sqrtSinProxy = new QSurfaceDataProxy();
    m_sqrtSinSeries = new QSurface3DSeries(m_sqrtSinProxy);
    //! [0]
    fillSqrtSinProxy();

    //! [2]
    QImage heightMapImage(":/maps/mountain");
    m_heightMapProxy = new QHeightMapSurfaceDataProxy(heightMapImage);
    m_heightMapSeries = new QSurface3DSeries(m_heightMapProxy);
    m_heightMapSeries->setItemLabelFormat(QStringLiteral("(@xLabel, @zLabel): @yLabel"));
    m_heightMapProxy->setValueRanges(34.0f, 40.0f, 18.0f, 24.0f);
    //! [2]
    m_heightMapWidth = heightMapImage.width();
    m_heightMapHeight = heightMapImage.height();
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

//! [1]
void SurfaceGraph::fillSqrtSinProxy()
{
    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);

    //-start- もとのコード  =====================================================================
//    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
//    dataArray->reserve(sampleCountZ);
//    for (int i = 0 ; i < sampleCountZ ; i++) {
//        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
//        // Keep values within range bounds, since just adding step can cause minor drift due
//        // to the rounding errors.
//        float z = qMin(sampleMax, (i * stepZ + sampleMin));
//        int index = 0;
//        for (int j = 0; j < sampleCountX; j++) {
//            float x = qMin(sampleMax, (j * stepX + sampleMin));
//            float R = qSqrt(z * z + x * x) + 0.01f;
//            float y = (qSin(R) / R + 0.24f) * 1.61f;
//            (*newRow)[index++].setPosition(QVector3D(x, y, z));
//        }
//        *dataArray << newRow;
//    }
//       m_sqrtSinProxy->resetArray(dataArray);
    //-end- もとのコード  =====================================================================


/*
    //-start- sample01 表示OK===============================================
    //-start- sample01  https://www.coder.work/article/2785034
    // 補足図のみ：https://forum.qt.io/topic/99340/qtdatavisualization-semi-transparent-qsurface3dseries
    // sample c++ - Q3DSurface:半透明QSurface3DSeries
    //Init memory
    //Q3DSurface *poSurface = new Q3DSurface();
    //QSurface3DSeries *poSeries = new QSurface3DSeries();
    QSurfaceDataArray *poDataArray = new QSurfaceDataArray();
    //
    // Generating test surface series
    QStringList tmplogList;
    for ( int i = 0, k = 0; i < 10; ++i){
        QSurfaceDataRow *poRow = new QSurfaceDataRow();

        for ( int j = 0; j < 10; ++j ){
            float x = j;
            float y = i;
            float z = k;
            poRow->append( QSurfaceDataItem( QVector3D( x, y, z ) ) );
            //qDebug() << "[DEBUG]SurfaceGraph::fillSqrtSinProxy() x=" << QString::number(x) << "  y=" << QString::number(y) << "  z=" << QString::number(z)  ;
            tmplogList << QString::number(x) + "," +  QString::number(y) + "," +  QString::number(z) ;
        }
        poDataArray->append( poRow );

        if ( i % 2 == 0 ){ ++k; }
    }
    m_sqrtSinProxy->resetArray(poDataArray); //表示OK
    //poSeries->dataProxy()->resetArray( poDataArray );
    //poSurface->addSeries( poSeries );
    //poSeries->setBaseColor( QColor( 100, 100, 100, 100 ));
    //
    fileWriteForWindows("c:/tmp/DEBUGlog_DataVisualSurface_xyz.txt", "WriteOnly", tmplogList); //[DEBUG]座標値をファイルに書き出し
    //
    //-end- sampe01  https://www.coder.work/article/2785034
    //-end- sample01===============================================
    //表示NG　落ちる→// m_graph->seriesList().at(0)->dataProxy()->resetArray(poDataArray); //この書き方ではNG。　scatterとは書式が違う？
*/

/*
    //-start- base-sample01 表示OK
    QSurfaceDataArray *poDataArray02 = new QSurfaceDataArray();
    // Generating test surface series
    QStringList tmplogList02;
    for ( int i = 0, k = 0; i < 3; ++i){
        QSurfaceDataRow *poRow = new QSurfaceDataRow();

        for ( int j = -4; j < 0; ++j ){
            float x = j;
            float y = i;
            float z = k;
            poRow->append( QSurfaceDataItem( QVector3D( x, y, z ) ) );
            //qDebug() << "[DEBUG]SurfaceGraph::fillSqrtSinProxy() x=" << QString::number(x) << "  y=" << QString::number(y) << "  z=" << QString::number(z)  ;
            tmplogList02 << QString::number(x) + "," +  QString::number(y) + "," +  QString::number(z) ;
        }
        poDataArray02->append( poRow );

        if ( i % 2 == 0 ){ ++k; }
    }
    //m_sqrtSinProxy->resetArray(poDataArray02);
    //fileWriteForWindows("c:/tmp/DEBUGlog_DataVisualSurface_xyz2.txt", "WriteOnly", tmplogList02); //[DEBUG]座標値をファイルに書き出し
    //
    //-end- sampe01  https://www.coder.work/article/2785034
    //-end- sample01===============================================
*/

    //-start- kuroda02  表示OK 単体 =======================================
    QSurfaceDataArray *data01 = new QSurfaceDataArray;
    QSurfaceDataRow *dataRow1 = new QSurfaceDataRow;
    QSurfaceDataRow *dataRow2 = new QSurfaceDataRow;
    *dataRow1 << QVector3D(4.0f, 0.1f, 0.5f) << QVector3D(6.0f, 0.5f, 0.5f);
    *dataRow2 << QVector3D(4.0f, 1.8f, 1.0f) << QVector3D(6.0f, 1.2f, 1.0f);
    *data01 << dataRow1 << dataRow2;
    m_sqrtSinProxy->resetArray(data01);

    //    //-start- kuroda03 2つ目のサーフェスとして追加する→動作未
    //    QSurface3DSeries *m_my01Series = new QSurface3DSeries(m_sqrtSinProxy);
    //    //m_my01Series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    //    //m_my01Series->setFlatShadingEnabled(true);
    //    m_graph->addSeries(m_my01Series);
    //    m_graph->seriesList().at(1)->dataProxy()->resetArray(data01);
    //    //m_graph->seriesList().at(1)->setBaseColor(Qt::green);
        //-end- kuroda03   =======================================

    //-start- kuroda04  表示OK 単体 2つ目のサーフェスとして追加する 動作OK =======================================
//    QSurfaceDataProxy *m_sqrtSinProxy02;
//    m_sqrtSinProxy02 = new QSurfaceDataProxy();
//    QSurface3DSeries *m_sqrtSinSeries02;
//    m_sqrtSinSeries02 = new QSurface3DSeries(m_sqrtSinProxy02);
//    m_graph->addSeries(m_sqrtSinSeries02);
//    //
//    QSurfaceDataArray *data02 = new QSurfaceDataArray;
//    QSurfaceDataRow *dataRow2_1 = new QSurfaceDataRow;
//    QSurfaceDataRow *dataRow2_2 = new QSurfaceDataRow;
//    *dataRow2_1 << QVector3D(-7.0f, 0.1f, 0.5f) << QVector3D(-3.0f, 0.5f, 0.5f);
//    *dataRow2_2 << QVector3D(-7.0f, 1.8f, 1.0f) << QVector3D(-3.0f, 1.2f, 1.0f);
//    *data02 << dataRow2_1 << dataRow2_2;
//    m_sqrtSinProxy02->resetArray(data02);
    //-end- kuroda04   =======================================

    //-start- kuroda05  関数化 表示OK 単体 2つ目のサーフェスとして追加する 動作OK =
    //2-1 OK Front/Back (水平, 高さ, 奥行=手前がマイナス)
    func_addsurface(QVector3D(-7.0f, 0.5f, 0.6f) , QVector3D(-3.0f, 0.5f, 0.6f),
                    QVector3D(-7.0f, 1.0f, 0.6f) , QVector3D(-3.0f, 1.0f, 0.6f));

    //2-2 Right / Left (水平, 高さ, 奥行=手前がマイナス)
    func_addsurface(QVector3D(1.0f, 0.1f, -2.0f) , QVector3D(2.0f, 0.1f, -2.0f),
                    QVector3D(1.0f, 0.5f, 1.0f) , QVector3D(2.0f, 0.5f, 1.0f));


    //2-3 OK Bottom/Top (水平, 高さ, 奥行=手前がマイナス)
    func_addsurface(QVector3D(4.0f, 0.1f, -5.0f) , QVector3D(7.0f, 0.1f, -5.0f),
                    QVector3D(4.0f, 0.2f, -8.0f) , QVector3D(7.0f, 0.2f, -8.0f));


    //2-3 OK Bottom/Top
    //func_addsurface(QVector3D(-7.0f, 0.6f, 0.1f) , QVector3D(-3.0f, 0.6f, 0.1f),
    //                QVector3D(-7.0f, 0.6f, 0.5f) , QVector3D(-3.0f, 0.6f, 0.5f));


    //3つ目　OK
    //func_addsurface(QVector3D(-2.0f, 0.1f, 0.5f) , QVector3D(1.0f, 0.5f, 0.5f),
    //                QVector3D(-2.0f, 1.8f, 1.0f) , QVector3D(1.0f, 1.2f, 1.0f));

    //1000個までは動作した　目視確認はできてないけど。。 1万個はだめかも
    //for(int i=0; i<10000; i++){
    //    func_addsurface(QVector3D(-7.0f, 0.1f, 1.5f) , QVector3D(-3.0f, 0.5f, 2.5f),  QVector3D(-7.0f, 1.8f, 1.5f) , QVector3D(-3.0f, 1.2f, 2.5f));
    //}
    //-end- kuroda05


}
//! [1]

void SurfaceGraph::func_addsurface(QVector3D in_vec3D_01, QVector3D in_vec3D_02, QVector3D in_vec3D_03, QVector3D in_vec3D_04){ //surfaceの追加
    QSurfaceDataProxy *m_sqrtSinProxy02;
    m_sqrtSinProxy02 = new QSurfaceDataProxy();
    QSurface3DSeries *m_sqrtSinSeries02;
    m_sqrtSinSeries02 = new QSurface3DSeries(m_sqrtSinProxy02);
    m_graph->addSeries(m_sqrtSinSeries02);
    //
    QSurfaceDataArray *data02 = new QSurfaceDataArray;
    QSurfaceDataRow *dataRow2_1 = new QSurfaceDataRow;
    QSurfaceDataRow *dataRow2_2 = new QSurfaceDataRow;
    //*dataRow2_1 << QVector3D(-7.0f, 0.1f, 0.5f) << QVector3D(-3.0f, 0.5f, 0.5f);
    //*dataRow2_2 << QVector3D(-7.0f, 1.8f, 1.0f) << QVector3D(-3.0f, 1.2f, 1.0f);
    *dataRow2_1 << in_vec3D_01 <<in_vec3D_02;
    *dataRow2_2 << in_vec3D_03 <<in_vec3D_04;
    *data02 << dataRow2_1 << dataRow2_2;
    m_sqrtSinProxy02->resetArray(data02);
}

void SurfaceGraph::enableSqrtSinModel(bool enable)
{
    if (enable) {
        //! [3]
        m_sqrtSinSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        m_sqrtSinSeries->setFlatShadingEnabled(true);

        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");
        m_graph->axisX()->setRange(sampleMin, sampleMax);
        m_graph->axisY()->setRange(0.0f, 2.0f);
        m_graph->axisZ()->setRange(sampleMin, sampleMax);
        m_graph->axisX()->setLabelAutoRotation(30);
        m_graph->axisY()->setLabelAutoRotation(90);
        m_graph->axisZ()->setLabelAutoRotation(30);

        m_graph->removeSeries(m_heightMapSeries);
        m_graph->addSeries(m_sqrtSinSeries);
        //! [3]

        //! [8]
        // Reset range sliders for Sqrt&Sin
        m_rangeMinX = sampleMin;
        m_rangeMinZ = sampleMin;
        m_stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
        m_stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);
        m_axisMinSliderX->setMaximum(sampleCountX - 2);
        m_axisMinSliderX->setValue(0);
        m_axisMaxSliderX->setMaximum(sampleCountX - 1);
        m_axisMaxSliderX->setValue(sampleCountX - 1);
        m_axisMinSliderZ->setMaximum(sampleCountZ - 2);
        m_axisMinSliderZ->setValue(0);
        m_axisMaxSliderZ->setMaximum(sampleCountZ - 1);
        m_axisMaxSliderZ->setValue(sampleCountZ - 1);
        //! [8]
    }
}

void SurfaceGraph::enableHeightMapModel(bool enable)
{
    if (enable) {
        //! [4]
        m_heightMapSeries->setDrawMode(QSurface3DSeries::DrawSurface);
        m_heightMapSeries->setFlatShadingEnabled(false);

        m_graph->axisX()->setLabelFormat("%.1f N");
        m_graph->axisZ()->setLabelFormat("%.1f E");
        m_graph->axisX()->setRange(34.0f, 40.0f);
        m_graph->axisY()->setAutoAdjustRange(true);
        m_graph->axisZ()->setRange(18.0f, 24.0f);

        m_graph->axisX()->setTitle(QStringLiteral("Latitude"));
        m_graph->axisY()->setTitle(QStringLiteral("Height"));
        m_graph->axisZ()->setTitle(QStringLiteral("Longitude"));

        m_graph->removeSeries(m_sqrtSinSeries);
        m_graph->addSeries(m_heightMapSeries);
        //! [4]

        // Reset range sliders for height map
        int mapGridCountX = m_heightMapWidth / heightMapGridStepX;
        int mapGridCountZ = m_heightMapHeight / heightMapGridStepZ;
        m_rangeMinX = 34.0f;
        m_rangeMinZ = 18.0f;
        m_stepX = 6.0f / float(mapGridCountX - 1);
        m_stepZ = 6.0f / float(mapGridCountZ - 1);
        m_axisMinSliderX->setMaximum(mapGridCountX - 2);
        m_axisMinSliderX->setValue(0);
        m_axisMaxSliderX->setMaximum(mapGridCountX - 1);
        m_axisMaxSliderX->setValue(mapGridCountX - 1);
        m_axisMinSliderZ->setMaximum(mapGridCountZ - 2);
        m_axisMinSliderZ->setValue(0);
        m_axisMaxSliderZ->setMaximum(mapGridCountZ - 1);
        m_axisMaxSliderZ->setValue(mapGridCountZ - 1);
    }
}

void SurfaceGraph::adjustXMin(int min)
{
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustXMax(int max)
{
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustZMin(int min)
{
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void SurfaceGraph::adjustZMax(int max)
{
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}

//! [5]
void SurfaceGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void SurfaceGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}
//! [5]

//! [6]
void SurfaceGraph::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}
//! [6]

void SurfaceGraph::setBlackToYellowGradient()
{
    //! [7]
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    //! [7]
}

void SurfaceGraph::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

//ファイル書き込み(windowsで使うファイル用)
void SurfaceGraph::fileWriteForWindows(QString WriteFilePath, QString WriteMode, QStringList msgList)
{
     QFile file(WriteFilePath);
     if(WriteMode == "Append"){
         if(!file.open(QIODevice::Append| QIODevice::Text)){
             //QMessageBox::information(this, tr("Unable to openfile"), file.errorString());
             qDebug() << "Error!! file-open" << WriteFilePath;
             return;
         }
     } else {
         if(!file.open(QIODevice::WriteOnly| QIODevice::Text)){
             //QMessageBox::information(this, tr("Unable to openfile"), file.errorString());
             qDebug() << "Error!! file-open" << WriteFilePath;
             return;
         }
     }

     QTextStream out(&file);
     for (int n=0; n<msgList.size(); n++){
         out << msgList.at(n) << endl;
     }
     file.close();
}

QStringList SurfaceGraph::readTextFileAll(QString fileName)
{
    QStringList lines;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        QString errStr =  "file open error:" + file.errorString();
        qDebug() << errStr;
        //return(lines);
    }
    QTextStream in(&file);
    QString inStr = in.readAll();
    file.close();

    //voxファイルのLF改行に対応
    //QStringList lines = inStr.split("\r\n");
    lines = inStr.split("\r\n"); //kuroda add
    lines = inStr.split("\n");
    return lines;

}

void SurfaceGraph::readTextFileLine(QString fileName)
{
     QFile file(fileName);
     if(!file.open(QIODevice::ReadOnly)){
         QString errStr =  "file open error:" + file.errorString();
         qDebug() << errStr;
         return;
     }
     QTextStream in(&file);
     while(! in.atEnd()){
         qDebug() << in.readLine();
     }
     file.close();
}
