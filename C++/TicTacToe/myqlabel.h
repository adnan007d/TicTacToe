#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class myqlabel : public QLabel
{
    Q_OBJECT
public:
    explicit myqlabel(QWidget* parent = 0);

    void mousePressEvent(QMouseEvent *ev);
signals:
    void Mouse_Pressed();
};

#endif // MYQLABEL_H
