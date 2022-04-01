#include "domtreeview.h"
#include "baseelement.h"
#include "utilities.h"
#include "groupelement.h"

DomTreeView::DomTreeView(QWidget *parent):AdvTreeView(parent)
{

}

DomTreeView::~DomTreeView()
{

}

bool DomTreeView::isAllowChildren(const QModelIndex &index)
{
    BaseElement * element = static_cast<BaseElement *>(index.internalPointer());
    return ((element->type()==QGraphicsItem::UserType+GRAPHICSITEM_TYPE_FOLDER)
        ||  (element->type()==QGraphicsItem::UserType+GRAPHICSITEM_TYPE_GROUP));


}

void DomTreeView::startMoveUnder(void *dragItem, void *dropItem)
{

}

void DomTreeView::startMoveAfter(void *dragItem, void *dropItem)
{

}

void DomTreeView::selectElements(QList<BaseElement *> elements)
{

    QList<BaseElement *> domElements,spaceElements;

    if ((elements.count()==1) && (elements[0]->type()==QGraphicsItem::UserType+GRAPHICSITEM_TYPE_GROUP_PICKER)){
        spaceElements = static_cast<GroupPickerElement *>(elements[0])->childElements();
    }
    else {
        spaceElements = elements;
    }

    foreach(QModelIndex index, this->selectedIndexes()){
        BaseElement * element = static_cast<BaseElement *>(index.internalPointer());
        domElements.append(element);
    }

    if (spaceElements.count()==0){
        this->clearSelection();
    }
    else {
//        if (Utilities::compare(elements,m_groupPicker->childElements()))
//        {
//            return;
//        }


    }

}

