#include "myqlabel.h"

myqlabel::myqlabel(QWidget* parent) :
    QLabel(parent)
{

}

void myqlabel::mousePressEvent(QMouseEvent *ev)
{
    emit Mouse_Pressed();
}
