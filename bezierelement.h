#ifndef BEZIERELEMENT_H
#define BEZIERELEMENT_H

#include "lineelement.h"

class BezierElement : public LineElement
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

public:
    BezierElement(
            BaseElement *parent,
            BaseSpace * space,
            QRectF initRect = QRectF(0,0,100,100)
            );
protected:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget=nullptr) Q_DECL_OVERRIDE;
    BasePicker *createPicker() Q_DECL_OVERRIDE;


};

#endif // BEZIERELEMENT_H
