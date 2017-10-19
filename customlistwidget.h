#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H
#include <QListWidget>

class CustomListWidget: public QListWidget
{
    Q_OBJECT

signals:
    void mousePressedRight(int,int);
    void mouseProssedLeft();

public:
    CustomListWidget( QWidget* parent = 0);
    ~CustomListWidget();


protected:
    void mousePressEvent(QMouseEvent *event);
};
#endif // CUSTOMLISTWIDGET_H
