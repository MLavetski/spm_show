#include "sshowsub.h"
#include "ui_sshowsub.h"
#include "surface.h"
#include "triangles.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QMouseEvent>
#include "imlabel.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QColorDialog>

void sshowsub::resizeWindow()
{
    int biggerOne;
    biggerOne = ui->label->width();
    this->setFixedSize(((ui->label->x())+(ui->label->width())+10),(52+(ui->label->height())+22));
    this->statusBar()->setSizeGripEnabled(false);
}

void sshowsub::showim(int num, int Nx, int Ny)
{
    field = QImage(Nx, Ny, QImage::Format_RGB32);
    QRgb value;
    float min = dataMuliplied[num][0], max = dataMuliplied[num][0];
    for(int i=0;i<Nx*Ny;i++)
    {
        if(dataMuliplied[num][i]<min && dataMuliplied[num][i]!=0)
            min = dataMuliplied[num][i];
        if(dataMuliplied[num][i]>max)
            max = dataMuliplied[num][i];
    }
    for(int i=0;i<Nx*Ny;i++)
    {
        if(dataMuliplied[num][i] != 0)
        {
            float x = dataMuliplied[num][i];
            float multi = (x - min)/(max - min);
            value = qRgb(multi*R,multi*G,multi*B);
        }
        else value = (0,0,0);
        field.setPixel(i%Nx,floor(i/Nx),value);
    }
    ui->label->setGeometry(ui->label->x(), ui->label->y(), Nx, Ny);
    QDesktopWidget wid;

    int screenWidth = wid.screen()->width();
    int screenHeight = wid.screen()->height();
    if(Nx>256 && Ny>256)
    {
        this->setGeometry(0,0,Nx+320,Ny+94);
        this->setGeometry((screenWidth/2)-(this->width()/2),(screenHeight/2)-(this->height()/2),Nx+320,Ny+94);
    }
    ui->label->setPixmap(QPixmap::fromImage(field));
    QString save = "D://testImageFor3d.png";
    field.save(save);
    resizeWindow();
    drawProfileLine();
}

void sshowsub::drawProfileLine()
{
    QImage fieldLine = field;
    QPainter pen(&fieldLine);
    pen.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
    if(avaliablePointsX.size()==1)
    {
        qint16 ax=avaliablePointsX[0];
        qint16 ay=avaliablePointsY[0];
        pen.setPen(QPen(Qt::white, 4, Qt::SolidLine, Qt::RoundCap));
        pen.drawPoint(ax,ay);
        pen.drawText(ax,ay,"A");
    }
    for(int i=0;i<avaliablePointsX.size()-1;i++)
    {
        qint16 ax=avaliablePointsX[i];
        qint16 ay=avaliablePointsY[i];
        qint16 bx=avaliablePointsX[i+1];
        qint16 by=avaliablePointsY[i+1];
        pen.setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
        pen.drawLine(ax,ay,bx,by);
        QString newName = "A";
        newName[0]=newName[0].unicode()+i;
        if(i==0)
        pen.drawText(ax, ay, newName);
        newName[0]=newName[0].unicode()+1;
        pen.drawText(bx,by,newName);
        pen.setPen(QPen(Qt::white, 4, Qt::SolidLine, Qt::RoundCap));
        if(i==0)
        pen.drawPoint(ax,ay);
        pen.drawPoint(bx,by);
    }
    ui->label->setPixmap(QPixmap::fromImage(fieldLine));
}


void sshowsub::openspm()
{
    if(inputfilename=="")//Default inputfilename
        {
            QMessageBox MTname;
            MTname.setText("Не выбран .spm файл. Пожалуйста, выберите.");
            MTname.exec();
            inputfilename = QFileDialog::getOpenFileName(this, tr("Открыть .spm файл"),"/", tr("Spm files (*.spm)"));
        }
    QFile spm(inputfilename);//Opening input file
        if(!spm.open(QIODevice::ReadOnly))
            return;
        QDataStream data(&spm);
        qint8 spmType; data>>spmType;//Getting filetype.
        char HEAD_SPM1[223]; //Getting HEAD_SPM1 from file.
        data.readRawData(HEAD_SPM1,223);
        quint16 MaxNx, MaxNy, field_amount=0;
        quint16 what_this;
        memcpy(&MaxNx,HEAD_SPM1+49,2);
        memcpy(&MaxNy,HEAD_SPM1+51,2);
        memcpy(&what_this,HEAD_SPM1+59,2);
        while(what_this)//Subfield what_this contains info about stored fields witch we can use to find amount of those.
        {
            field_amount++;
            what_this &= (what_this-1); //current AND itself-1 to remove the least bit. repeat untill its 0.
        }
        char notifications[16][336];//Getting all notifications about stored fields of data.
        for(int i=0;i<field_amount;i++)
            data.readRawData(notifications[i], 336);
        for(int i=0; i<field_amount;i++)//Getting that information.
        {
            strncpy(fieldName[i], notifications[i], 32);
            strncpy(scaleXYname[i], notifications[i]+68, 6);
            strncpy(scaleZname[i], notifications[i]+74, 6);
            memcpy(&Nx[i], notifications[i]+34, 2);
            memcpy(&Ny[i], notifications[i]+36, 2);
            memcpy(&scaleX[i], notifications[i]+40, 4);
            memcpy(&scaleY[i], notifications[i]+44, 4);
            memcpy(&scaleZ[i], notifications[i]+48, 4);
        }

        //Getting all raw data
        for(int i=0; i<field_amount;i++)
            data.readRawData(dataRaw[i], Nx[i]*Ny[i]*2);
        for(int i=0; i<field_amount;i++)//Trasform raw data into 16bit integers.
        {
            for(int j=0; j<(Nx[i]*Ny[i]);j++)
            {
                memcpy(&dataShort[i][j], dataRaw[i]+(2*j), 2);
            }
        }
        for(int i=0; i<field_amount;i++)//Multiping data by z-multiplier
        {
            for(int j=0; j<(Nx[i]*Ny[i]);j++)
            {
                dataMuliplied[i][j]=dataShort[i][j]*scaleZ[i];
            }
        }
        for(int i=0;i<field_amount;i++)
        ui->fieldselect->insertItem(i, fieldName[i]);
        int extPointPos = inputfilename.lastIndexOf("/") + 1;
        QString inputfilenameCopy = inputfilename;
        QString WindowTitle = inputfilenameCopy.remove(inputfilename.left(extPointPos));
        sshowsub::setWindowTitle(WindowTitle);
        showim(0, Nx[0], Ny[0]);
}

sshowsub::sshowsub(QWidget *parent, QString filename) :
    QMainWindow(parent),
    ui(new Ui::sshowsub)
{
    inputfilename = filename;
    ui->setupUi(this);
    openspm();
}

sshowsub::~sshowsub()
{
    delete ui;
}

void sshowsub::on_fieldselect_activated(int index)
{
    showim(index, Nx[index], Ny[index]);
}

void sshowsub::getPointsOnLineFromAToB()
{
    int loaded=0;
    for(int i=0;i<avaliablePointsX.size()-1;i++)
    {
        qint16 ax=avaliablePointsX[i],ay=avaliablePointsY[i],
               bx=avaliablePointsX[i+1],by=avaliablePointsY[i+1];
        int dx = bx - ax, dy = by-ay;
        int countx = abs(dx), county = abs(dy), decrx=1, decry=1;
        float slope, xdiff, ydiff;
        bool vertical=false;
        if(dx!=0)
            slope = (float)dy/(float)dx;
        else
        {
            slope = 0;
            vertical = true;
        }
        if(dx>=0)
        {
            if(slope<1 && slope>(-1))
            {
                xdiff=1; ydiff=slope;
            }
            else
            {
                if(dy>0)
                {
                    ydiff=1; xdiff=(1/slope);
                }
                else
                {
                    ydiff=-1; xdiff=(1/slope*(-1));
                }
            }
        }
        else
        {
            if(slope<1 && slope>(-1))
            {
                xdiff=-1; ydiff=slope*(-1);
            }
            else
            {
                if(dy>0)
                {
                    ydiff=1; xdiff=(1/slope);
                }
                else
                {
                    ydiff=-1; xdiff=(1/slope)*(-1);
                }
            }
        }
        if(vertical)
        {
            xdiff=0;
            if(dy>0)
                ydiff=1;
            else
                ydiff=-1;
        }
        float currX=ax, currY=ay;
        loaded=pointsX.size();
        while(true)
        {
            pointsX.append(round(currX));
            pointsY.append(round(currY));
            currX+=xdiff;
            currY+=ydiff;
            countx-=decrx; county-=decry;
            if(countx<=0)
                decrx=0;
            if(county<=0)
                decry=0;
            if(countx<=0 && county<=0)
                break;
        }
        int curInd = ui->fieldselect->currentIndex();
        for(int j=loaded;j<pointsX.size();j++)
        {
            int point = pointsY[j]*Nx[curInd]+pointsX[j];
            val.append(dataMuliplied[curInd][point]);
        }
        PointsCoordinastesStoredAsNumber.append(loaded);
    }
}

void sshowsub::on_profBut_clicked()
{
    getPointsOnLineFromAToB();
    proflist.append(new showprofile(val, pointsX, pointsY, PointsCoordinastesStoredAsNumber, this, inputfilename, openedprof));
    proflist[openedprof]->show();
    openedprof++;
    pointsX.clear();
    pointsY.clear();
    val.clear();
}


void sshowsub::on_label_mouseMoved(int x, int y)
{
    int curInd = ui->fieldselect->currentIndex();
    int point = y*Nx[curInd]+x;
    float value = dataMuliplied[curInd][point];
    QString coord = "x=" + QString::number(x*scaleX[curInd]) + scaleXYname[curInd] + "; "
                  + "y=" + QString::number(y*scaleY[curInd]) + scaleXYname[curInd] + "; "
                  + "value=" + QString::number(value) + scaleZname[curInd];
    statusBar()->showMessage(coord);
    for(int i=0;i<avaliablePointsX.size();i++)
    {
        qint16 curX=avaliablePointsX[i], curY=avaliablePointsY[i];
        if(x>(curX-5) && x<(curX+5) && y>(curY-5) && y<(curY+5))
        {
            ui->label->setCursor(Qt::OpenHandCursor);
            return;
        }
        else
        {
            ui->label->setCursor(Qt::CrossCursor);
        }
    }
}

void sshowsub::on_label_mousePressedLeft(int x, int y)
{
    for(int i=0;i<avaliablePointsX.size();i++)
    {
        qint16 curX=avaliablePointsX[i], curY=avaliablePointsY[i];
        if(x>(curX-5) && x<(curX+5) && y>(curY-5) && y<(curY+5))
        {
            avaliablePointsX[i]=x;
            avaliablePointsY[i]=y;
            drawProfileLine();
        }
    }
}

void sshowsub::on_label_mousePressedRight(int x, int y)
{
    for(int i=0;i<avaliablePointsX.size();i++)
    {
        qint16 curX=avaliablePointsX[i], curY=avaliablePointsY[i];
        if(x>(curX-5) && x<(curX+5) && y>(curY-5) && y<(curY+5))
        {
            avaliablePointsX.removeAt(i);
            avaliablePointsY.removeAt(i);
            drawProfileLine();
        }
    }
    if(avaliablePointsX.size() < 2)
        ui->profBut->setEnabled(false);
}

void sshowsub::on_label_doubleCicked(int x, int y)
{
    avaliablePointsX.append(x);
    avaliablePointsY.append(y);
    QString newName = "A";
    newName[0]=newName[0].unicode()+(avaliablePointsX.size());
    drawProfileLine();
    if(avaliablePointsX.size() > 1)
        ui->profBut->setEnabled(true);
}


void sshowsub::on_infoButton_clicked()//NEED to update
{
    QMessageBox info;
    QString MT = NULL;
    info.setText(MT + "*Двойнок клик по полю - создание точки в конце линии профиля в данной точке.\n"
                 "*Правый клик по точке - удалить данную точку.\n"
                 "*Удержать левую кнопку по точке - Перемещение точки.\n"
                 "*Кнопка 'Построить профиль' - создать срез значений на основе выбраных точек.\n"
                 "*Кнопка 'Выбор цвета' - выбор основного цвета для отображения поверхности.");
    info.setIcon(QMessageBox::Information);
    info.exec();
}

void sshowsub::on_PickColorBut_clicked()
{
    QColorDialog picker;
    QColor selectedColor = picker.getColor();
    R=selectedColor.red();
    G=selectedColor.green();
    B=selectedColor.blue();
    int index = ui->fieldselect->currentIndex();
    showim(index, Nx[index], Ny[index]);
}

void sshowsub::on_surfaceBut_clicked()
{
    int index = ui->fieldselect->currentIndex();
    fixSlope = ui->toFixSlope->isChecked();
    if(!fixSlope)
    {
        surface3d = new surface(this, dataMuliplied[index], scaleX[index], scaleY[index],
                                scaleXYname[index],scaleZname[index], Nx[index], Ny[index]);
        surface3d->show();
    }
    /*else
    {
        for(int i = 0; i < (Nx[index]*Ny[index]); i++)
        {
            dataFixed[i] = dataMuliplied[index][i];
        }


        for(int i=0;i<(Nx[index]*Ny[index]);i++)
        {
            if(dataFixed[i] != 0)
            {
                int x=i%Nx[index],y=floor(i/Nx[index]);
                dataFixed[i] -= (((A*(x-pos1x))+(B*(y-pos1y)))/(C*(-1)))+pos1z;
            }
        }
        //send dataFixed as data array to 3dSurface
        surface3d = new surface(this, dataFixed, scaleX[index], scaleY[index],
                                scaleXYname[index],scaleZname[index], Nx[index], Ny[index]);
        surface3d->show();
    }*/
}

void sshowsub::on_saveToStlBut_clicked()
{
    //We have an array of data witch can be repressented as matrix Ny by Nx
    //that function takes points in said matrix in specified order(points MUST be CCW) to represent
    //verticles of triangles, stores them in vector of triangle class objects, calls calcNormal function
    //for each triange, and writes them into binary .stl file
    quint16 index = ui->fieldselect->currentIndex();
    quint16 sizeX = Nx[index], sizeY = Ny[index];
    char header[80];
    for(int i=0;i<80;i++)
    {
        header[i]=0;
    }
    quint32 amountTriangles = (sizeX-1)*(sizeY-1)*2;
    QVector<triangle> triangles(amountTriangles);
    int i=0;//counter for current triangle
    for(int y=0;y<sizeY-1;y++)  //write coordinates into corresponding triangle
    {                           //there are to types of triangles
        for(int x=0;x<sizeX;x++)//flat side up and down
        {
            if(x!=0)//here we writing flat side down triangle
            {
                triangles[i].point3[0] = x*scaleX[index];
                triangles[i].point3[1] = y*scaleY[index];
                triangles[i].point3[2] = dataMuliplied[index][y*Nx[index]+x];
                triangles[i].point2[0] = (x-1)*scaleX[index];
                triangles[i].point2[1] = (y+1)*scaleY[index];
                triangles[i].point2[2] = dataMuliplied[index][(y+1)*Nx[index]+(x-1)];
                triangles[i].point1[0] = x*scaleX[index];
                triangles[i].point1[1] = (y+1)*scaleY[index];
                triangles[i].point1[2] = dataMuliplied[index][(y+1)*Nx[index]+x];
                i++;
            }
            if(x!=sizeX-1)//here we writing flat side up triangle
            {
                triangles[i].point3[0] = x*scaleX[index];
                triangles[i].point3[1] = y*scaleY[index];
                triangles[i].point3[2] = dataMuliplied[index][y*Nx[index]+x];
                triangles[i].point2[0] = x*scaleX[index];
                triangles[i].point2[1] = (y+1)*scaleY[index];
                triangles[i].point2[2] = dataMuliplied[index][(y+1)*Nx[index]+x];
                triangles[i].point1[0] = (x+1)*scaleX[index];
                triangles[i].point1[1] = y*scaleY[index];
                triangles[i].point1[2] = dataMuliplied[index][y*Nx[index]+(x+1)];
                i++;
            }
        }
    }
    for(quint32 j=0;j<amountTriangles;j++)
    {
        triangles[j].calcNorm();
    }
    triangle test[20];
    for(int i=0;i<20;i++)
        test[i] = triangles[i];
    //ok, we got triangles ready, now write them into .stl file
    //first, generate filemane for output file.
    int extPointPos = inputfilename.lastIndexOf(".");
    QString outputfilename = inputfilename.left(extPointPos);
    //then, we open the file with generatated name. If it doesn't exist, create it.
    QFile file(outputfilename + "_" + fieldName[ui->fieldselect->currentIndex()] + ".stl");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out.setByteOrder(QDataStream::LittleEndian);
    //write 80-byte header into file. In our case, header is filled with empty symbols.
    out.writeRawData(header, 80);
    //write int with amount of triangles in file
    out.writeRawData((char*)&amountTriangles,4);
    //write each triangle
    for(quint32 i=0;i<amountTriangles;i++)
    {
        //first 3 dimensions of normal vector
        out.writeRawData((char*)&triangles[i].normal[0],4);
        out.writeRawData((char*)&triangles[i].normal[1],4);
        out.writeRawData((char*)&triangles[i].normal[2],4);
        //3 dim. of first point
        out.writeRawData((char*)&triangles[i].point1[0],4);
        out.writeRawData((char*)&triangles[i].point1[1],4);
        out.writeRawData((char*)&triangles[i].point1[2],4);
        //second point
        out.writeRawData((char*)&triangles[i].point2[0],4);
        out.writeRawData((char*)&triangles[i].point2[1],4);
        out.writeRawData((char*)&triangles[i].point2[2],4);
        //third point
        out.writeRawData((char*)&triangles[i].point3[0],4);
        out.writeRawData((char*)&triangles[i].point3[1],4);
        out.writeRawData((char*)&triangles[i].point3[2],4);
        //and the attribyte file count, witch, by default, is zero.
        out.writeRawData((char*)&triangles[i].attrByteCount,2);
    }
    //then we close the output file and send notification of successfull conversion
    file.close();
    QMessageBox yep;
    yep.setText("yep");
    yep.exec();
}
