#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QVector3D>

MainWindow::MainWindow
(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_makePlane_clicked()
{

}

void MainWindow::on_pushButton_makeRect_clicked()
{

}

//始点・終点から　複数メッシュが連続するような図形から、平面座標を取得して、objファイルとして作成する
void MainWindow::func_getPointOfMesh_plane(QVector3D in_pointBL, QVector3D in_pointTR, int surfaceNo, int input_MateNo, int in_surfaceCnt) //kuroda　もとのgetPointOfMesh 関数から変更
{
    float X1 = ui->lineEdit_pX1->text().toFloat();
    float Y1 = ui->lineEdit_pX1->text().toFloat();
    float Z1 = ui->lineEdit_pX1->text().toFloat();
    float X2 = ui->lineEdit_pX2->text().toFloat();
    float Y2 = ui->lineEdit_pX2->text().toFloat();
    float Z2 = ui->lineEdit_pX2->text().toFloat();

    QString strSurface = ui->comboBox_plane->currentText();
    float MateNo = 99;
    float surfaceCnt = 1;

    QVector3D pBL, pBR, pTL, pTR;
    pBL = QVector3D(X1, Z1, Y2); // .objファイルは右手系座標　glX:水平　glY:高さ glZ:奥行（手前がプラス・奥マイナス)
    pTR = QVector3D(X2, Z2, Y1);
    if(strSurface.startsWith("Front")){ //FRONT or BACK
        pBR = QVector3D(X2, Z1, Y2); // .objファイルは右手系座標　glX:水平　glY:高さ glZ:奥行（手前がプラス・奥マイナス)
        pTL = QVector3D(X1, Z2, Y1);
        QString msg1;
        QString msg2 = strSurface + " " + msg1.asprintf("X1,Y1,Z1=%d, %d, %d   X2 Y2 Z2=%d %d %d",  X1, Y1, Z1, X2, Y2, Z2);
        func_objfile_write(MateNo, surfaceCnt, msg1,  pTL, pBL, pTR, pTL) ;
    }

    if(strSurface.startsWith("Right")){ //Right or Back　　※GUI入力は左下としてE点(0, -1, -1)を入力すること前提
        pBR = QVector3D(X1, Z1, Y1); // .objファイルは右手系座標　glX:水平　glY:高さ glZ:奥行（手前がプラス・奥マイナス)
        pTL = QVector3D(X2, Z2, Y2);
        QString msg1;
        QString msg2 = strSurface + " " + msg1.asprintf("X1,Y1,Z1=%d, %d, %d   X2 Y2 Z2=%d %d %d",  X1, Y1, Z1, X2, Y2, Z2);
        func_objfile_write(MateNo, surfaceCnt, msg1,  pTL, pBL, pTR, pTL) ; //FRONTと違う
    }

    if(strSurface.startsWith("Top")){ //Top or Bottom
        pBR = QVector3D(X2, Z1, Y2); // .objファイルは右手系座標　glX:水平　glY:高さ glZ:奥行（手前がプラス・奥マイナス)
        pTL = QVector3D(X1, Z2, Y1);

        QString msg1;
        QString msg2 = strSurface + " " + msg1.asprintf("X1,Y1,Z1=%d, %d, %d   X2 Y2 Z2=%d %d %d",  X1, Y1, Z1, X2, Y2, Z2);
        func_objfile_write(MateNo, surfaceCnt, msg1,  pBL, pBR, pTL, pTR) ; //FRONTと違う
    }

}

// 平面1つ分を.objファイルに記述する
void MainWindow::func_objfile_write(int input_MateNo, int in_surfaceCnt, QString in_msg1,  QVector3D in_vertex1, QVector3D in_vertex2, QVector3D in_vertex3, QVector3D in_vertex4) //objファイル書き込み
{
    //-start- objファイル作成前準備
    //QString outfileDir =  QFileInfo(QFile(voxfilePath)).absolutePath() + "/objfile_" + QFileInfo(QFile(voxfilePath)).fileName() ;//2021.06.xx-01 for-obj
    //QString outfilePath = outfileDir + "/" + QFileInfo(QFile(voxfilePath)).fileName() + "_" + QString::number(input_MateNo) + ".obj"; //
    QString outfileDir = "c:/tmp";
    QString outfilePath = "c:/tmp/tmp.obj";
    QFile outfile1(outfilePath);
    if(!outfile1.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::information(this, tr("Unable to openfile"), outfile1.errorString());
        return;
    }
    QTextStream out_obj1(&outfile1);
    //-end- objファイル作成

    //qDebug() << "[DEBUG]01miWidget.cpp-func_objfile_write()";

    //objファイル書き込み
    //例 FRONT
    //mtllib sample_plane_front_blender.mtl
    //o Plane
    //v -1.000000 1.000000 1.000000
    //v -1.000000 -1.000000 1.000000
    //v 1.000000 1.000000 1.000000
    //v 1.000000 -1.000000 1.000000
    out_obj1 << "# " + in_msg1 << endl;
    out_obj1 << "o Plane." + QString::number(in_surfaceCnt) +  "\n";
    //objでは、(x,y,z) 水平：Xは小さいほうから、奥行Zは大きい方から。高さYはx,z に合わせて。
    out_obj1 << "v " << in_vertex1.x() << " " << in_vertex1.y() << " " << in_vertex1.z() <<endl; //objの場合↑とは描き方が違うので注意すること　objテキストファイルに書き出すは4頂点だけ
    out_obj1 << "v " << in_vertex2.x() << " " << in_vertex2.y() << " " << in_vertex2.z() <<endl; //頂点順番はこれから修正する
    out_obj1 << "v " << in_vertex3.x() << " " << in_vertex3.y() << " " << in_vertex3.z() <<endl;
    out_obj1 << "v " << in_vertex4.x() << " " << in_vertex4.y() << " " << in_vertex4.z() <<endl;
    //vt,vn,s行：固定
    out_obj1 << "vt 0.000000 0.000000" << endl;
    out_obj1 << "vt 1.000000 0.000000" << endl;
    out_obj1 << "vt 1.000000 1.000000" << endl;
    out_obj1 << "vt 0.000000 1.000000" << endl;
    out_obj1 << "vn 0.0000 1.0000 0.0000" << endl;
    out_obj1 << "s off"<< endl;
    //f行：可変。Planeごとにカウントアップ必要。
    //out_obj1 << "f 1/1/1 2/2/1 4/3/1"<< endl; //D A B
    //out_obj1 << "f 4/3/1 3/4/1 1/1/1"<< endl; //B C D
    QList<int> vList,vtList;
    vList << 1 << 2 << 4 << 4 << 3 << 1;
    vtList << 1 << 2 << 3 << 3 << 4 << 1;
    for(int i=0; i < vList.count(); i++ ){
        vList[i] = vList.at(i) + 4 * in_surfaceCnt;
        vtList[i] = vtList.at(i) + 4 * in_surfaceCnt;
    }
    QString tmpstr;
    out_obj1 << tmpstr.asprintf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                                 vList.at(0), vtList.at(0), in_surfaceCnt + 1,
                                 vList.at(1), vtList.at(1), in_surfaceCnt + 1,
                                 vList.at(2), vtList.at(2), in_surfaceCnt + 1);
    out_obj1 << tmpstr.asprintf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                                 vList.at(3), vtList.at(3), in_surfaceCnt + 1,
                                 vList.at(4), vtList.at(4), in_surfaceCnt + 1,
                                 vList.at(5), vtList.at(5), in_surfaceCnt + 1);

    outfile1.close(); //objファイル書き込み。
}
