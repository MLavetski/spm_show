#ifndef SSHOWSUB_H
#define SSHOWSUB_H

#include <QMainWindow>
#include "showprofile.h"
#include "surface.h"
#include "imlabel.h"

namespace Ui {
class sshowsub;
}

class sshowsub : public QMainWindow
{
    Q_OBJECT

public:
    explicit sshowsub(QWidget *parent = 0, QString filename = NULL);
    sshowsub(const sshowsub&) = default;
        sshowsub& operator=(const sshowsub&) = default;
    ~sshowsub();
    QString inputfilename;
    char dataRaw[16][524288];//Place for raw data of fields from spm1 files. 2 bytes for each point. Max 256*256 points.
    qint16 dataShort[16][262144];//Same data but transformed into 16 bit integers.
    float dataMuliplied[16][262144];//Data after applying z-multiplier.
    float dataFixed[262144];
    //16 is borderline amount of fields for that file format. The service info won't support more than 32.
    qint16 Nx[16],Ny[16];
    quint8 R=0,G=255,B=0;
    QImage field;
    QVector<qint16> avaliablePointsX, avaliablePointsY;
    char fieldName[16][32], scaleXYname[16][6], scaleZname[16][6];//info from notifications about fields
    float scaleX[16], scaleY[16], scaleZ[16];
    QVector<float> val;
    QVector<int> pointsX, pointsY, PointsCoordinastesStoredAsNumber;
    bool lockedSelection=false;

private slots:

    void on_fieldselect_activated(int index);

    void on_profBut_clicked();

    void on_label_mouseMoved(int x, int y);

    void on_label_mousePressedLeft(int x, int y);

    void on_label_mousePressedRight(int x, int y);


    void on_label_doubleCicked(int , int y);


    void on_infoButton_clicked();

    void on_PickColorBut_clicked();

    void on_surfaceBut_clicked();

    void on_saveToStlBut_clicked();

private:
    Ui::sshowsub *ui;
    QVector<showprofile*> proflist;
    surface *surface3d;
    bool fixSlope = false;
    int openedprof = 0;
    void openspm();
    void showim(int num, int Nx, int Ny);
    void drawProfileLine();
    void getPointsOnLineFromAToB();
    void resizeWindow();
};



#endif // SSHOWSUB_H
