#include "bezierpicker.h"
#include "bezierelement.h"
#include <QPainter>
#include <QDebug>

BezierPicker::BezierPicker(LineElement *element):LinePicker(element)
{

}

void BezierPicker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen pen;
    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DotLine);
    pen.setWidth(0);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(pen);
    QVector<QPointF>::const_iterator it = m_lineElement->m_points.begin();
    QPointF p1 = (*it),p2,p3;
    ++it;
    for(;it<m_lineElement->m_points.end();++it){
        p2 = (*it);
        painter->drawLine(elementToPicker(p1),elementToPicker(p2));
        qDebug()<<"draw line"<<p1<<p2;
        p1 = p2;
    }
#ifdef DRAW_PICKER_BOUNDING
    painter->setRenderHint(QPainter::Antialiasing,true);
    pen.setWidth(0);
    pen.setCapStyle(Qt::SquareCap);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    QRectF bounding = this->boundingRect();
    painter->drawRect(bounding.x(),bounding.y(),bounding.width(),bounding.height());
#endif

}
