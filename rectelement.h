#ifndef RECTELEMENT_H
#define RECTELEMENT_H

#include <QPen>

#include "baseelement.h"
class RectElement : public BaseElement
{

Q_OBJECT
    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    QPen m_pen;

    enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_RECT };
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

public:
    RectElement(
            BaseElement *parent,
            BaseSpace * space,
            QRectF initRect = QRectF(0,0,100,100)
            );
    QPen pen() const
    {
        return m_pen;
    }

    QPainterPath shape() const override;

public slots:
    void setPen(const QPen & pen);

signals:
    void penChanged(QPen pen);

protected:
    BasePicker *createPicker() Q_DECL_OVERRIDE;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget=nullptr) override;
};

#endif // RECTELEMENT_H
