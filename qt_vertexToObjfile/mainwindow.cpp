#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QVector3D>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <GL/GLU.h>
#include <gl/GL.h>

MainWindow::MainWindow
(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_savefilepath->setText("c:/tmp/tmp01/tmp.obj");

    ui->lineEdit_pX1->setText("0");
    ui->lineEdit_pY1->setText("0");
    ui->lineEdit_pZ1->setText("0");
    ui->lineEdit_pX2->setText("1");
    ui->lineEdit_pY2->setText("0");
    ui->lineEdit_pZ2->setText("1");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_makePlane_clicked()
{
    func_getPointOfMesh_plane();

}

void MainWindow::on_pushButton_makeRect_clicked()
{

}

//始点・終点から　複数メッシュが連続するような図形から、平面座標を取得して、objファイルとして作成する
void MainWindow::func_getPointOfMesh_plane() //kuroda　もとのgetPointOfMesh 関数から変更
{
   GLfloat X1, Y1, Z1, X2, Y2, Z2;
   X1 = ui->lineEdit_pX1->text().toFloat();
   Y1 = ui->lineEdit_pY1->text().toFloat();
   Z1 = ui->lineEdit_pZ1->text().toFloat();
   X2 = ui->lineEdit_pX2->text().toFloat();
   Y2 = ui->lineEdit_pY2->text().toFloat();
   Z2 = ui->lineEdit_pZ2->text().toFloat();

    QString strSurface = ui->comboBox_plane->currentText();
    float MateNo = 99;
    float surfaceCnt = 0;

    QVector3D pBL, pBR, pTL, pTR;
    pBL = QVector3D(X1, Z1, Y2); // .objファイルは右手系座標　glX:水平　glY:高さ glZ:奥行（手前がプラス・奥マイナス)
    pTR = QVector3D(X2, Z2, Y1);
    pBR = QVector3D(X2, Z1, Y2);//初期値不定回避のため、暫定入力。
    pTL = QVector3D(X1, Z2, Y1);//初期値不定回避のため、暫定入力。

    if(strSurface.startsWith("Front")){ //FRONT or BACK
        pBR = QVector3D(X2, Z1, Y2); // .objファイルは右手系座標　glX:水平　glY:高さ glZ:奥行（手前がプラス・奥マイナス)
        pTL = QVector3D(X1, Z2, Y1);
        QString msg1;
        QString msg2 = strSurface + " " + msg1.asprintf("X1,Y1,Z1=%0.1f %0.1f %0.1f   X2 Y2 Z2=%0.0f %0.0f %0.0f",  X1, Y1, Z1, X2, Y2, Z2);
        func_objfile_write(MateNo, surfaceCnt, msg2,  pTL, pBL, pTR, pBR) ;
    }

    if(strSurface.startsWith("Right")){ //Right or Back　　※GUI入力は左下としてE点(0, -1, -1)を入力すること前提
        pBR = QVector3D(X1, Z1, Y1); // .objファイルは右手系座標　glX:水平　glY:高さ glZ:奥行（手前がプラス・奥マイナス)
        pTL = QVector3D(X2, Z2, Y2);
        QString msg1;
        QString msg2 = strSurface + " " + msg1.asprintf("X1,Y1,Z1=%d, %d, %d   X2 Y2 Z2=%d %d %d",  X1, Y1, Z1, X2, Y2, Z2);
        func_objfile_write(MateNo, surfaceCnt, msg2,  pTL, pBL, pTR, pBR) ; //FRONTと同じ記述? 奥行も違う　内容座標も違う
    }

    if(strSurface.startsWith("Top")){ //Top or Bottom
        pBR = QVector3D(X2, Z1, Y2); // .objファイルは右手系座標　glX:水平　glY:高さ glZ:奥行（手前がプラス・奥マイナス)
        pTL = QVector3D(X1, Z2, Y1);

        QString msg1;
        QString msg2 = strSurface + " " + msg1.asprintf("X1,Y1,Z1=%d, %d, %d   X2 Y2 Z2=%d %d %d",  X1, Y1, Z1, X2, Y2, Z2);
        func_objfile_write(MateNo, surfaceCnt, msg2,  pBL, pBR, pTL, pTR) ; //FRONTと違う記述　内容座標も違う
    }

}

// 平面1つ分を.objファイルに記述する
void MainWindow::func_objfile_write(int input_MateNo, int in_surfaceCnt, QString in_msg1,  QVector3D in_vertex1, QVector3D in_vertex2, QVector3D in_vertex3, QVector3D in_vertex4) //objファイル書き込み
{
    //-start- objファイル作成前準備
    //QString outfileDir =  QFileInfo(QFile(voxfilePath)).absolutePath() + "/objfile_" + QFileInfo(QFile(voxfilePath)).fileName() ;//2021.06.xx-01 for-obj
    //QString outfilePath = outfileDir + "/" + QFileInfo(QFile(voxfilePath)).fileName() + "_" + QString::number(input_MateNo) + ".obj"; //
    //QString outfileDir = "c:/tmp";
    QString outfilePath = "c:/tmp/tmp01/tmp.obj";
    QString tmp_path = ui->lineEdit_savefilepath->text();
    if(QFile(tmp_path).exists()){ outfilePath = tmp_path; }
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
    for(int i=0; i<2; i++){
        QList<int> vList,vtList;
        if(i==0){
            vList << 1 << 2 << 4 << 4 << 3 << 1; //平面　表・裏の片面
            vtList << 1 << 2 << 3 << 3 << 4 << 1;
        }
        if(i==1){
            vList << 2 << 1 << 3 << 4 << 2 << 3; //平面　表・裏の片面　QtDataVisualizerで視点移動した時、平面の表・裏とも見えるようにするため両面書くことが必要。(Blenderとか一般ソフトは不要そうなのだが。。）
            vtList << 2 << 1 << 4 << 3 << 2 << 3;
        }
        for(int i=0; i < vList.count(); i++ ){
            vList[i] = vList.at(i) + 4 * in_surfaceCnt;
            vtList[i] = vtList.at(i) + 4 * in_surfaceCnt;
        }
        QString tmpstr;
        out_obj1 << tmpstr.asprintf("#plane %d/2\n", in_surfaceCnt, i);
        out_obj1 << tmpstr.asprintf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                                    vList.at(0), vtList.at(0), in_surfaceCnt + 1,
                                    vList.at(1), vtList.at(1), in_surfaceCnt + 1,
                                    vList.at(2), vtList.at(2), in_surfaceCnt + 1);
        out_obj1 << tmpstr.asprintf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                                    vList.at(3), vtList.at(3), in_surfaceCnt + 1,
                                    vList.at(4), vtList.at(4), in_surfaceCnt + 1,
                                    vList.at(5), vtList.at(5), in_surfaceCnt + 1);
    }
    outfile1.close(); //objファイル書き込み。

    QMessageBox::information(this, "notice", "save-file=" + outfilePath );
}


void MainWindow::on_comboBox_plane_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox_plane->currentText().startsWith("Front")){ //Front-Back共通
        //後: Back座標
        ui->lineEdit_pX1->setText("0");
        ui->lineEdit_pY1->setText("0");
        ui->lineEdit_pZ1->setText("0");
        ui->lineEdit_pX2->setText("1");
        ui->lineEdit_pY2->setText("0");
        ui->lineEdit_pZ2->setText("1");
    }
    if(ui->comboBox_plane->currentText().startsWith("Right")){ //Right-Left共通
        //左: Left座標
        ui->lineEdit_pX1->setText("0");
        ui->lineEdit_pY1->setText("0");
        ui->lineEdit_pZ1->setText("0");
        ui->lineEdit_pX2->setText("0");
        ui->lineEdit_pY2->setText("1");
        ui->lineEdit_pZ2->setText("1");
    }
    if(ui->comboBox_plane->currentText().startsWith("Top")){ //Top-Bottom共通
        //下: Bottom座標
        ui->lineEdit_pX1->setText("0");
        ui->lineEdit_pY1->setText("0");
        ui->lineEdit_pZ1->setText("0");
        ui->lineEdit_pX2->setText("1");
        ui->lineEdit_pY2->setText("1");
        ui->lineEdit_pZ2->setText("0");
    }
}
