#ifndef FOLDERELEMENT_H
#define FOLDERELEMENT_H
#include "lineelement.h"


class FolderElement : public BaseElement
{
    Q_OBJECT
            QPen m_pen;
            enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_BEZIER};

public:
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }
    FolderElement(
            BaseElement *parent,
            BaseSpace * space,
            QRectF initRect = QRectF(0,0,100,100)
            );

    inline BasePicker * createPicker() override { return 0;}

};

#endif // FOLDERELEMENT_H
