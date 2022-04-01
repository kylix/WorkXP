#include "rectelement.h"
#include "elementpicker.h"
#include "basespace.h"
#include <QPainter>
#include <QDebug>

RectElement::RectElement(BaseElement *parent, BaseSpace *space, QRectF initRect):
    BaseElement(parent,space,initRect)
{
    setElementName(m_space->nameElementAuto(this->type()));
    m_picker = createPicker();
    m_pen.setWidth(1);
    m_pen.setColor(Qt::black);

}

QPainterPath RectElement::shape() const
{
    QPainterPath path;

    path.addRect(this->rect());
    return path;

}

void RectElement::setPen(const QPen &pen)
{
    if (m_pen==pen) return;
    m_pen = pen;
    emit penChanged(pen);

}

BasePicker *RectElement::createPicker()
{
    return new ElementPicker(this);
}

void RectElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing,true);
    QRectF rect = this->rect();
    painter->setPen(m_pen);
    qreal nOffset = m_pen.widthF()/2;

    rect.adjust(nOffset,nOffset,-nOffset,-nOffset);
    painter->drawRect(rect);

    BaseElement::paint(painter,option,widget);
}
