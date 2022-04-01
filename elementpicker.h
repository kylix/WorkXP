#ifndef ELEMENTPICKER_H
#define ELEMENTPICKER_H

#include <QGraphicsObject>
#include <QCursor>
#include <QPen>
#include "basepicker.h"
#include "graphicsitemtype.h"

QT_BEGIN_NAMESPACE
class QGraphicsRectItem;
QT_END_NAMESPACE


class BaseElement;
class ElementPickerRect;
class ElementPickerOutline;
class ElementPickerRotate;
class ElementPicker;
class ElementPickerWraper;
class BaseSpace;

#define DEFAULT_ARCHOR_WIDTH 6

//ElementPicker < ElementPickerWrap < ElementPickerOutline+ElementPickerRotate < ElementPickerArchor


enum ElementPickerArchorShape
{
    PS_CIRCLE,
    PS_RECTANGLE
};


class ElementPickerArchor : public QGraphicsItem
{
    ElementPickerOutline * m_pickRect;
    PickerDirection m_direction;
    ElementPickerArchorShape  m_shape;

    QRectF m_boundingRect;

    enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_PICKER_ARCHOR };

public:
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

public:
    ElementPickerArchor(ElementPickerOutline * parent,
                        PickerDirection direction,
                        ElementPickerArchorShape  shape=ElementPickerArchorShape::PS_RECTANGLE);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    QRectF boundingRect() const override;

    inline void setDirection(PickerDirection arg1) { m_direction = arg1; }
    inline PickerDirection direction() const { return m_direction;}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void setPickerDirection(PickerDirection direction);

};

class ElementPickerOutline : public QGraphicsItem
{
    ElementPickerWraper * m_wraper;
    QRectF  m_elementRect;
    QPointF m_transformCenter;
    QPointF m_lastPos;


    ElementPickerArchor * m_archors[8];

    enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_PICKER_OUTLINE };

public:
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

public:
    ElementPickerOutline(ElementPickerWraper * parent);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;


    void setElementRect(const QRectF & rect);
    inline QRectF elementRect() const { return m_elementRect;}

    void setPickerDirection(PickerDirection direction);

    ElementTransform calcTransform(QPointF p,PickerDirection direction);
    void adjust(const ElementTransform & t);
    void beginAdjust(PickerDirection direction);

    void setResizable(bool arg1);

    ElementPickerArchor * archorAt(const QPointF & p) const;



};

class ElementPickerRotateArchor : public QGraphicsItem
{
    ElementPickerRotate * m_rotate;
    const qreal WIDTH = 24;
    const qreal HEIGHT = 24;
    QCursor m_cursor;
public:
    ElementPickerRotateArchor(ElementPickerRotate * rotate);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    QRectF boundingRect() const override;
    void setPickerDirection(PickerDirection direction);
    BaseSpace * space() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

};

class ElementPickerRotate : public QGraphicsItem
{
    ElementPickerWraper * m_wraper;
    ElementPickerRotateArchor * m_archor;

public:
    ElementPickerRotate(ElementPickerWraper * wraper);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    QRectF boundingRect() const override;
    void setPickerDirection(PickerDirection direction);
    BaseSpace * space() const;
};

class ElementPickerWraper : public QGraphicsItem
{
    ElementPicker * m_picker;
    ElementPickerRotate * m_rotate;
    qreal m_rotate_angle;

public:
    ElementPickerWraper(ElementPicker * picker);

    ElementPickerOutline * m_outline;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    QRectF boundingRect() const override;

    void setElementRect(const QRectF & rect);
    QRectF elementRect() const;
    qreal rotateAngle() const {return m_rotate_angle;}


    void setDirection(PickerDirection direction);
    BaseSpace *space();

    ElementTransform calcTransform(QPointF pos, PickerDirection direction);
    void adjust(const ElementTransform & t);
    void beginAdjust(PickerDirection direction);

    void rotateByCenter(qreal angle,const QPointF & cneter);

    inline ElementPicker * picker() { return m_picker;}
    ElementPickerArchor * archorAt(const QPointF & p) const;

};


/*!
\class ElementPicker
\brief The transformation handler for Element inherited by BaseElement
*/
class ElementPicker  : public BasePicker
{
Q_OBJECT
    ElementPickerWraper * m_wraper;

    //record the offset between element's rect and mouse press postion
    QPointF m_start_offset;
    //record the initial position before rotate
    QPointF m_start_pos;

    //indicate the direction of adjust
    PickerDirection m_direction;
    enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_RECT_PICKER };

    QCursor m_prevCursor;


public:
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

public:
    explicit ElementPicker(BaseElement *element);

    PickerDirection direction(const QPointF & p) const;
    inline void setDirection(PickerDirection direction) { m_direction = direction;}
    void setByElement() override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setElementRotation(qreal angle,const QPointF & center) override;
    void setElementRect(const QRectF & rect) override;

    void beginAdjust(PickerDirection direction);
    virtual void adjust(const ElementTransform &t) override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

};


#endif // ELEMENTPICKER_H
