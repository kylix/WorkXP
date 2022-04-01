#include <QGraphicsScene>

#include "basepicker.h"
#include "baseelement.h"
#include "basespace.h"

BasePicker::BasePicker(BaseElement *element)
{
    m_element = element;
    m_space = element->space();
    this->hide();
    element->scene()->addItem(this);
    this->setFlag(QGraphicsItem::ItemIsSelectable,false);
    this->setFlag(QGraphicsItem::ItemIsMovable,false);
    this->setFlag(QGraphicsItem::ItemIsFocusable,false);
    this->setFlag(QGraphicsItem::ItemAcceptsInputMethod,false);

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges,false);
    this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges,false);
    this->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);

    this->setAcceptHoverEvents(true);
    connect(element->space(),&BaseSpace::scaled,this,&BasePicker::sceneScaled);
    element->stackBefore(this);

}

void BasePicker::sceneScaled()
{
    this->setByElement();
}



QVariant BasePicker::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change==QGraphicsItem::ItemVisibleChange){
        if (value.toBool()){
           this->setByElement();
        }
    }
    return QGraphicsObject::itemChange(change,value);

}

QPointF BasePicker::elementToPicker(const QPointF &p) const
{
    BaseSpace * space = this->space();
    return this->deviceTransform(space->viewportTransform()).inverted().map(
                m_element->deviceTransform(space->viewportTransform()).map(p)
                );
}

QPointF BasePicker::pickerToElement(const QPointF &p) const
{
    BaseSpace * space = this->space();
    return m_element->deviceTransform(space->viewportTransform()).inverted().map(
                this->deviceTransform(space->viewportTransform()).map(p)
                );
}
