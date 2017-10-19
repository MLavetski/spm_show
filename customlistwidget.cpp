#include <customlistwidget.h>
#include <QMouseEvent>

CustomListWidget::CustomListWidget(QWidget * parent)
    : QListWidget( parent)
{

}

CustomListWidget::~CustomListWidget()
{

}

void CustomListWidget::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    if(x>(this->width()-1) || x<0 || y>(this->height()-1) || y<0)
    {
        return;
    }
    else if(event->buttons() & Qt::RightButton)
    {
        emit mousePressedRight(x, y);
    }
}
