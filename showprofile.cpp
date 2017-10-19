#include "showprofile.h"
#include "ui_showprofile.h"
#include <QPainter>

void showprofile::drawValues()
{
    int vsize=values.size();
    max=values[0]; min=values[0];
    for(int i=1;i<vsize;i++)
    {
        if(max<values[i])
            max=values[i];
        if(min>values[i])
            min=values[i];
    }
    ui->valueSelect->setMaximum(vsize-1);
    //ui->valueSelect->setGeometry();
    ui->ValueLabel->setText(QString::number(values[0]));
    ui->XLabel->setText(QString::number(xs[0]));
    ui->Ylabel->setText(QString::number(ys[0]));
    int sp = 50, profHeight=256;
    if(vsize<512)
        sizeMult = 512/(double)vsize;
    else
    {
        sizeMult=1;
        ui->valueSelect->setGeometry(50,260,(vsize-1),22);
    }
    prof = QImage((vsize*sizeMult)+sp, profHeight+1, QImage::Format_RGB32);
    prof.fill(Qt::white);
    QPainter pen(&prof);
    pen.setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));
    for(int i=0;i<vsize-1;i++)
    {
        //prof.setPixel(i,max-values[i],color);
        pen.drawLine((i*sizeMult)+sp,((max-values[i])/(max-min)*(prof.height()-1)-1),
                     ((i+1)*sizeMult)+sp,((max-values[i+1])/(max-min)*(prof.height()-1)-1));
    }
    pen.setPen(QPen(Qt::gray,1,Qt::SolidLine,Qt::FlatCap));
    for(int i=0;i<sigPoints.size();i++)
    {
        pen.drawLine((sigPoints[i]*sizeMult)+sp,0,(sigPoints[i]*sizeMult)+sp,prof.height()-1);
    }
    pen.setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::FlatCap));
    pen.drawText(0,prof.height(),QString::number(min));
    pen.drawText(0,10,QString::number(max));
    int selectedValue = ui->valueSelect->value();
    pen.drawLine(sp,0,sp,prof.height()-1);
    pen.drawLine(sp,prof.height()-1,prof.width()-1,prof.height()-1);
    QImage profWithSelect = prof;
    QPainter selectDrawer(&profWithSelect);
    selectDrawer.setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::FlatCap));
    selectDrawer.drawLine(sp,0,sp,prof.height());
    selectDrawer.setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::FlatCap));
    const QPoint Center(selectedValue+50, (max-values[selectedValue]*0.98)/(max-min)*(prof.height())-1);
    selectDrawer.drawEllipse(Center, 2,2);
    ui->label->setPixmap(QPixmap::fromImage(profWithSelect));
    ui->label->setGeometry(0,0,profWithSelect.width(), profWithSelect.height());
    this->setGeometry(30,30,profWithSelect.width()+5,profWithSelect.height()+50);
    int extPointPos = inputfile.lastIndexOf(".");
    QString outfile = inputfile.left(extPointPos) + "_2.bmp";
    prof.save(outfile);
}

void showprofile::generateValues()
{
    values.append(0);
    for(float i=1;i<512;i++)
    {
        values.append(values[i-1]+((qrand()%5)-2));
    }
}

showprofile::showprofile(QVector<float> val, QVector<int> Sxs, QVector<int> Sys, QVector<int> SignificantPoints, QWidget *parent, QString inputfilename, int openedprof) :
    QMainWindow(parent),
    ui(new Ui::showprofile)
{
    ui->setupUi(this);
    inputfile = inputfilename;
    int extPointPos = inputfilename.lastIndexOf("/") + 1;
    QString inputfilenameCopy = inputfilename;
    QString WindowTitle = inputfilenameCopy.remove(inputfilename.left(extPointPos)) + ": Профиль #" + QString::number(openedprof+1);
    showprofile::setWindowTitle(WindowTitle);
    if(val.isEmpty())
    {
        generateValues();
    }
    else
    {
        values = val;
    }
    xs=Sxs;
    ys=Sys;
    sigPoints=SignificantPoints;
    drawValues();
}

showprofile::~showprofile()
{
    delete ui;
}

void showprofile::on_valueSelect_valueChanged(int value)
{
    ui->ValueLabel->setText(QString::number(values[value]));
    ui->XLabel->setText(QString::number(xs[value]));
    ui->Ylabel->setText(QString::number(ys[value]));
    QImage profWithSelect = prof;
    QPainter selectDrawer(&profWithSelect);
    selectDrawer.setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::FlatCap));
    selectDrawer.drawLine(value*sizeMult+50,0,value*sizeMult+50,prof.height()-1);
    selectDrawer.setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::FlatCap));
    const QPoint Center(value*sizeMult+50, (max-values[value])/(max-min)*(prof.height()-1)-1);
    selectDrawer.drawEllipse(Center, 2,2);
    ui->label->setPixmap(QPixmap::fromImage(profWithSelect));
}
