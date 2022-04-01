#include "folderelement.h"
#include "basespace.h"

FolderElement::FolderElement(BaseElement *parent, BaseSpace *space, QRectF initRect):
    BaseElement(parent,space,initRect)
{
    setFlag(QGraphicsItem::ItemHasNoContents);
    setElementName(m_space->nameElementAuto(this->type()));

}
