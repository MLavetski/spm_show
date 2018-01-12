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
    explicit surface(QWidget *parent = 0, float dataMultiplied[] = NULL, float scaleX=1, float scaleY=1, char XYscaleName[]=NULL,char ZscaleName[] = NULL, int x = 0, int y = 0);
        surface(const surface&) = default;
            surface& operator=(const surface&) = default;
        ~surface();

private slots:

            void on_useGrad_toggled(bool useGrad);

            void on_pckMClr_triggered();


            void on_chkdCold_triggered();

            void on_chkdWarm_triggered();

            void on_chkdNeon_triggered();

            void themeChanged(int value);


private:
    Ui::surface *ui;
    Q3DSurface *graph;
    float data[262144];//Data after applying z-multiplier.
    float Xscale, Yscale;
    char scaleXYname[32], scaleZname[32];
    int Nx, Ny;
    bool useGrad = false;
    QColor mainColor = Qt::darkGreen;
    QVector<QLinearGradient> gradList;
    int selectedGradient=0;
    short selectedTheme = 0;

    void drawWidget();
    void buildSurface();
    void applyColor();
    void setupGradients();
};

#endif // SURFACE_H
