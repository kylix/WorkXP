#include "bezierelement.h"

#include <QPainter>
#include <QPen>
#include "bezierpicker.h"
#include "basespace.h"

BezierElement::BezierElement(BaseElement *parent, BaseSpace *space, QRectF initRect):
    LineElement(parent,space,initRect)
{
    setElementName(m_space->nameElementAuto(this->type()));
//    delete m_picker;
    m_picker = createPicker();
}

void BezierElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPainterPath path;
    painter->setPen(pen());
   // painter->setBrush(brush());
    path.moveTo(m_points.at(0));

    int i=1;
    while ( i + 2 < m_points.size()) {
        path.cubicTo(m_points.at(i), m_points.at(i+1), m_points.at(i+2));
        i += 3;
    }

    while (i < m_points.size()) {
        path.lineTo(m_points.at(i));
        ++i;
    }



//    while (i < m_points.size()) {
//        path.lineTo(m_points.at(i));
//        ++i;
//    }
    painter->drawPath(path);

}

BasePicker *BezierElement::createPicker()
{
   return new BezierPicker(this);

}
