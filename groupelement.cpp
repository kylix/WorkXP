#include "groupelement.h"
#include "elementpicker.h"
#include "QContextMenuEvent"
#include "basespace.h"
#include <QMatrix4x4>
#include <QGraphicsTransform>
#include <QRectF>
#include <QDebug>


static QRectF childElementsBoundingRect(BaseGroupElement *groupElement)
{
    //if element is an ancester of BaseGroupElement
    QRectF rect;
    foreach(BaseElement * element,groupElement->childElements()){
        rect = rect.united(element->mapToScene(element->boundingRect()).boundingRect());
    }

    return groupElement->mapFromScene(rect).boundingRect();

}

BaseGroupElement::BaseGroupElement(BaseElement *parent, BaseSpace *space, QRectF initRect):
    BaseElement(parent,space,initRect)
{
    setHandlesChildEvents(true);
    m_picker = createPicker();
}


BasePicker *BaseGroupElement::createPicker()
{
    return new ElementPicker(this);
}



void BaseGroupElement::addToGroup(BaseElement *element)
{
    if (!element) {
        //cannot add null item
        qWarning("addToGroup:cannot add null element to group");
        return;
    }
    if (element == this) {
        qWarning("addToGroup:cannot add a group to itself");
        return;
    }

    foreach(BaseElement * item,m_childElements){
        if (item==element){
            qWarning("addToGroup:cannot add a item already existed");
            return;
        }
    }
    m_childElements.insert(0,element);
    element->setParentElement(this);
    //element->setParentItem(this);
    this->setRect(childElementsBoundingRect(this));
    return;


}

void BaseGroupElement::removeFromGroup(BaseElement *element)
{
    if (!element) {
        qWarning("removeFromGroup: cannot remove null element");
        return;
    }


    m_childElements.removeOne(element);
    element->setParentElement(nullptr);

    this->setRect(childElementsBoundingRect(this));

}

bool BaseGroupElement::isObscuredBy(const QGraphicsItem *item) const
{
    return BaseElement::isObscuredBy(item);

}

QPainterPath BaseGroupElement::opaqueArea() const
{
    return BaseElement::opaqueArea();

}

void BaseGroupElement::clear()
{
    foreach(BaseElement * element,m_childElements){
        element->setParentElement(0);
    }
    m_childElements.clear();
//    this->setTransform(QTransform());
//    this->m_picker->setTransform(QTransform());
//    this->setRect(this->childrenBoundingRect());
}



GroupPickerElement::GroupPickerElement(BaseSpace *space):
    BaseGroupElement(nullptr,space,QRectF())
{
    m_picker->show();
//    disconnect(static_cast<BaseSpaceScene *>(space->scene()),&BaseSpaceScene::elementUnpicked,this,0);
    connect(static_cast<BaseSpaceScene *>(space->scene()),&BaseSpaceScene::elementUnpicked,this,[this](){
        this->hide();
    });

}



GroupElement::GroupElement(BaseElement *parent, BaseSpace *space):
    BaseGroupElement(parent,space,QRectF())
{
    setElementName(space->nameElementAuto(this->type()));

}


