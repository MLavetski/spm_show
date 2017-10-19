#include "imlabel.h"
#include <QMouseEvent>

imlabel::imlabel( QWidget * parent, Qt::WindowFlags f )
    : QLabel( parent, f )
{

}

imlabel::imlabel( const QString& text, QWidget* parent, Qt::WindowFlags f )
    : QLabel( text, parent, f )
{

}

void imlabel::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    if(x>(this->width()-1) || x<0 || y>(this->height()-1) || y<0)
    {
        return;
    }
    emit mouseMoved(x,y);
    if(event->buttons() & Qt::LeftButton)
    {
        emit mousePressedLeft(x,y);
    }
    else if(event->buttons() & Qt::RightButton)
    {
        emit mousePressedRight(x,y);
    }
}

void imlabel::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    if(x>(this->width()-1) || x<0 || y>(this->height()-1) || y<0)
    {
        return;
    }
    if(event->buttons() & Qt::LeftButton)
    {
        emit mousePressedLeft(x,y);
    }
    else if(event->buttons() & Qt::RightButton)
    {
        emit mousePressedRight(x,y);
    }
}

void imlabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    if(x>(this->width()-1) || x<0 || y>(this->height()-1) || y<0)
    {
        return;
    }
    if(event->buttons() & Qt::LeftButton)
    {
        emit doubleCicked(x,y);
    }
}
