#include "sshow.h"
#include "sshowsub.h"
#include "ui_sshow.h"
#include <QFileDialog>
#include <QFileSystemModel>

SShow::SShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SShow)
{
    ui->setupUi(this);

    model = new QFileSystemModel;
    model->setRootPath("D:/");
    QStringList filters;
    filters << "*.spm";
    model->setNameFilters(filters);
    model->setNameFilterDisables(false);
    ui->treeView->setModel(model);
    for(int i=1;i<4;i++)
    {
        ui->treeView->hideColumn(i);
    }
}

SShow::~SShow()
{
    delete ui;
}

void SShow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString filepath = model->filePath(index);
    QFileInfo info(filepath);
    QString suffix = info.completeSuffix();
    if(suffix=="spm")
    {
        sublist.append(new sshowsub(this, filepath));
        sublist[openedfiles]->show();
        openedfiles++;
    }
}
