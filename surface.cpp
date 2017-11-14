#include "surface.h"
#include <QMessageBox>
#include <QScreen>
#include <QtWidgets>
using namespace QtDataVisualization;

surface::surface(float dataMultiplied[], int x, int y)
{
    Nx = x; Ny = y;
    for(int i = 0; i< x*y; i++)
        this->data[i] = dataMultiplied[i];
    drawWidget();
}

surface::~surface()
{

}


void surface::drawWidget()
{
    QMainWindow *window = new QMainWindow();
    graph = new Q3DSurface();
    QWidget *container = QWidget::createWindowContainer(graph);
    window->setWindowTitle(QStringLiteral(""));
    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    window->setCentralWidget(widget);
    vLayout->setAlignment(Qt::AlignTop);


    graph->setShadowQuality(QAbstract3DGraph::ShadowQualityLow);
    graph->resize(320,240);
    graph->setPosition(50,50);

    buildSurface();
    window->show();
}

void surface::buildSurface()
{
    QSurfaceDataProxy *heightMapProxy = new QSurfaceDataProxy;
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(Nx);
    QSurfaceDataRow *dataRow;
    for(int y=0;y<Ny;y++)
    {
        dataRow = new QSurfaceDataRow(Ny);
        for(int x=0;x<Nx;x++)
        {
            (*dataRow)[x].setPosition(QVector3D(x,data[(y*Nx)+x],Ny-y));
        }
        *dataArray << dataRow;
    }
    heightMapProxy->resetArray(dataArray);
    QSurface3DSeries *series = new QSurface3DSeries(heightMapProxy);
    series->setDrawMode(QSurface3DSeries::DrawSurface);
    graph->flipHorizontalGrid();
    graph->addSeries(series);
    graph->show();
}
