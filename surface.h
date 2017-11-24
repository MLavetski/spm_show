#ifndef SURFACE_H
#define SURFACE_H

#include <QMainWindow>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>

using namespace QtDataVisualization;

namespace Ui {
class surface;
}

class surface : public QMainWindow
{
    Q_OBJECT

public:
    explicit surface(QWidget *parent = 0, float dataMultiplied[] = NULL, int x = 0, int y = 0);
        surface(const surface&) = default;
            surface& operator=(const surface&) = default;
        ~surface();

private slots:

            void on_useGrad_toggled(bool useGrad);

            void on_pckMClr_triggered();


            void on_chkdCold_triggered();

            void on_chkdWarm_triggered();

            void on_chkdNeon_triggered();

private:
    Ui::surface *ui;
    Q3DSurface *graph;
    float data[262144];//Data after applying z-multiplier.
    int Nx, Ny;
    bool useGrad = false;
    QColor mainColor = Qt::darkGreen;
    QVector<QLinearGradient> gradList;
    int selectedGradient=0;

    void drawWidget();
    void buildSurface();
    void applyColor();
    void setupGradients();
};

#endif // SURFACE_H
