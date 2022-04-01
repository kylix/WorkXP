#ifndef LINEELEMENT_H
#define LINEELEMENT_H

#include <QPen>
#include <QPolygonF>

#include "baseelement.h"
#define DEFAULT_ARCHOR_WIDTH 6


class LineElement : public BaseElement
{
Q_OBJECT
    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    QPen m_pen;
    enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_LINE };

public:
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }



public:
    QVector<QPointF> m_points;
    LineElement(
            BaseElement *parent,
            BaseSpace * space,
            QRectF initRect = QRectF(0,0,100,100)
            );

    inline QPen pen() const
    {
        return m_pen;
    }

    QRectF calcBoundingRect() const;

    virtual void doDrawing(const QPointF & p) override;
    virtual void startDraw(const QPointF & p) override;
    virtual void clickDraw(const QPointF & p) override;
    virtual void doubleClickDraw(const QPointF & scenePos) override;

    void replacePoint(const QPointF & p1, const QPointF & p2);

    //virtual QVector<LinePoint> & points() {return m_points;}
   // QRectF boundingRect() const override;


protected:
     void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget=nullptr) Q_DECL_OVERRIDE;
     BasePicker *createPicker() Q_DECL_OVERRIDE;

public slots:
    void setPen(const QPen & pen);

signals:
   void penChanged(const  QPen & pen);


};

#endif // LINEELEMENT_H
