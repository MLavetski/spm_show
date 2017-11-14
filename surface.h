#ifndef SURFACE_H
#define SURFACE_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>

using namespace QtDataVisualization;

class surface
{

public:
    explicit surface(float dataMultiplied[] = NULL, int Nx = 0, int y = 0);
    surface(const surface&) = default;
        surface& operator=(const surface&) = default;
    ~surface();

private slots:


private:
    Q3DSurface *graph;
    void drawWidget();
    float data[262144];//Data after applying z-multiplier.
    int Nx, Ny;
    void buildSurface();
};

#endif // SURFACE_H
