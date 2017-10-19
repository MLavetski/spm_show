#ifndef SHOWPROFILE_H
#define SHOWPROFILE_H

#include <QMainWindow>

namespace Ui {
class showprofile;
}


class showprofile : public QMainWindow
{
    Q_OBJECT

public:
    explicit showprofile(QVector<float> val, QVector<int> Sxs, QVector<int> Sys, QVector<int> SignificantPoints ,QWidget *parent = 0, QString inputfilename = NULL, int openedprof = 0);
    showprofile(const showprofile&) = default;
        showprofile& operator=(const showprofile&) = default;
    ~showprofile();
    QVector<float> values;
    QVector<int> xs, ys, sigPoints;
    void generateValues();
    void drawValues();
    QString inputfile;
    QImage prof;
    float max, min;
    double sizeMult=0.8;

private slots:
    void on_valueSelect_valueChanged(int value);

private:
    Ui::showprofile *ui;
};

#endif // SHOWPROFILE_H
