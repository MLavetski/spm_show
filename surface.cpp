#include "surface.h"
#include "ui_surface.h"
#include <QMessageBox>
#include <QScreen>
#include <QtWidgets>

surface::surface(QWidget *parent, float dataMultiplied[],float scaleX, float scaleY,char XYscaleName[], char ZscaleName[], int x, int y) :
    QMainWindow(parent),
    ui(new Ui::surface)
{
    Nx = x; Ny = y;
    Xscale = scaleX;
    Yscale = scaleY;
    for(int i = 0; i< x*y; i++)
        this->data[i] = dataMultiplied[i];
    for(int i=0; i<32; i++)
    {
        scaleXYname[i] = XYscaleName[i];
        scaleZname[i] = ZscaleName[i];
    }

    ui->setupUi(this);

    QComboBox *themeSlktr = new QComboBox(ui->menubar);//create combo box for theme selection.
    themeSlktr->addItem("Qt");
    themeSlktr->addItem("PrimaryColors");
    themeSlktr->addItem("Digia");
    themeSlktr->addItem("StoneMoss");
    themeSlktr->addItem("ArmyBlue");
    themeSlktr->addItem("Retro");
    themeSlktr->addItem("Ebony");
    themeSlktr->addItem("Isabelle");
    QWidgetAction *comboAction = new QWidgetAction(ui->menubar);
    comboAction->setDefaultWidget(themeSlktr);
    QMenu *themeMenu = ui->menubar->addMenu("Основная тема");
    themeMenu->addAction(comboAction);
    connect(themeSlktr, SIGNAL(currentIndexChanged(int)),this, SLOT(themeChanged(int)));


    QActionGroup* group = new QActionGroup( this );//combine actions for selecting gradient type into one group
    ui->chkdCold->setActionGroup(group);
    ui->chkdWarm->setActionGroup(group);
    ui->chkdNeon->setActionGroup(group);

    setupGradients();
    drawWidget();
}

surface::~surface()
{
    delete ui;
}

void surface::drawWidget()
{
    graph = new Q3DSurface();
    QWidget *container = createWindowContainer(graph, this);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(container);
    ui->centralwidget->setLayout(hbox);
    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 5, screenSize.height() / 4.5));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);


    graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    graph->resize(320,240);
    graph->setPosition(50,50);
    graph->setAxisX(new QValue3DAxis);//prepare to set up the graph axis
    graph->setAxisY(new QValue3DAxis);
    graph->setAxisZ(new QValue3DAxis);
    QString MT = NULL;
    graph->axisX()->setLabelFormat(MT + "%.2f " + scaleXYname);//in 3dSurface Y is height, and in ASM data
    graph->axisY()->setLabelFormat(MT + "%.2f " + scaleZname);// height is Z.
    graph->axisZ()->setLabelFormat(MT + "%.2f " + scaleXYname);
    graph->axisX()->setLabelAutoRotation(30);
    graph->axisY()->setLabelAutoRotation(90);
    graph->axisZ()->setLabelAutoRotation(30);

    buildSurface();
}

void surface::buildSurface()//literally build surface feeding an array of floats trhough proxy
{
    QSurfaceDataProxy *heightMapProxy = new QSurfaceDataProxy;
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(Ny);
    QSurfaceDataRow *dataRow;
    for(int y=0;y<Ny;y++)
    {
        dataRow = new QSurfaceDataRow(Nx);
        for(int x=0;x<Nx;x++)
        {
            (*dataRow)[x].setPosition(QVector3D(x*Xscale,data[(y*Nx)+x],(Ny-y)*Yscale));
        }
        *dataArray << dataRow;
    }
    heightMapProxy->resetArray(dataArray);
    QSurface3DSeries *series = new QSurface3DSeries(heightMapProxy);
    series->setDrawMode(QSurface3DSeries::DrawSurface);
    graph->flipHorizontalGrid();
    graph->addSeries(series);
    graph->setOptimizationHints(QAbstract3DGraph::OptimizationStatic);
    graph->show();
}



void surface::on_useGrad_toggled(bool arg1)
{
    useGrad=arg1;
    applyColor();
}


void surface::applyColor()
{
    if(useGrad)
    {
        graph->seriesList().at(0)->setBaseGradient(gradList[selectedGradient]);
        graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
        return;
    }
    else
    {
        graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleUniform);
        graph->activeTheme()->setType(Q3DTheme::Theme(selectedTheme));
        return;
    }
}

void surface::setupGradients()
{
    gradList.resize(3);
    gradList[0].setColorAt(1, Qt::white);
    gradList[0].setColorAt(0.75, QColor(0xdc, 0xed, 0xc8));
    gradList[0].setColorAt(0.45, QColor(0x42,0xb3,0xd5));
    gradList[0].setColorAt(0.25, QColor(0x1a,0x23,0x7e));
    gradList[0].setColorAt(0, Qt::black);
    gradList[1].setColorAt(1, Qt::white);
    gradList[1].setColorAt(0.85, QColor(0xfe, 0xeb, 0x65));
    gradList[1].setColorAt(0.65, QColor(0xe4,0x52,0x1b));
    gradList[1].setColorAt(0.3, QColor(0x4d,0x34,0x2f));
    gradList[1].setColorAt(0, Qt::black);
    gradList[2].setColorAt(1, Qt::white);
    gradList[2].setColorAt(0.62, QColor(0xff, 0xec, 0xb3));
    gradList[2].setColorAt(0.45, QColor(0xe8,0x52,0x85));
    gradList[2].setColorAt(0.2, QColor(0x6a,0x1b,0x9a));
    gradList[2].setColorAt(0, Qt::black);
}

void surface::on_pckMClr_triggered()
{
    QColorDialog picker;
    mainColor =  picker.getColor();
    applyColor();
}

void surface::on_chkdCold_triggered()
{
    selectedGradient=0;
    applyColor();
}

void surface::on_chkdWarm_triggered()
{
    selectedGradient=1;
    applyColor();
}

void surface::on_chkdNeon_triggered()
{
    selectedGradient=2;
    applyColor();
}

void surface::themeChanged(int value)
{
    selectedTheme = value;
    graph->activeTheme()->setType(Q3DTheme::Theme(value));
    if(ui->useGrad->isChecked())
        ui->useGrad->trigger();
}
