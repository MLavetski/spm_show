#ifndef IMLABEL_H
#define IMLABEL_H


#include <QLabel>

class imlabel: public QLabel
{
    Q_OBJECT

signals:
    void mouseMoved(int,int);
    void mousePressedLeft(int,int);
    void mousePressedRight(int,int);
    void doubleCicked(int, int);

public:
    imlabel( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    imlabel( const QString& text, QWidget* parent = 0, Qt::WindowFlags f = 0 );


protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // IMLABEL_H
