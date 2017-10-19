#ifndef SSHOW_H
#define SSHOW_H

#include <QMainWindow>
#include "sshowsub.h"
#include <QFileSystemModel>

namespace Ui {
class SShow;
}

class SShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SShow(QWidget *parent = 0);
    QString InputFileName;
    ~SShow();

private slots:

    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    Ui::SShow *ui;
    QVector<sshowsub*> sublist;
    int openedfiles = 0;
    QFileSystemModel *model;
};



#endif // SSHOW_H
