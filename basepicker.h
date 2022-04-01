#ifndef BASESELECTPICKER_H
#define BASESELECTPICKER_H

#include <QGraphicsObject>
class BaseElement;
class BaseSpace;

enum PickerDirection
{
    PD_NONE,
    PD_TOP,
    PD_LEFT,
    PD_BOTTOM,
    PD_RIGHT,
    PD_TOP_LEFT,
    PD_TOP_RIGHT,
    PD_BOTTOM_LEFT,
    PD_BOTTOM_RIGHT,
    PD_MOVE,
    PD_ROTATE
};

class ElementTransform
{
public:
    inline ElementTransform():dx(0),dy(0),sx(0),sy(0),type(QTransform::TxNone),direction(PD_NONE)
    {}

    qreal dx;
    qreal dy;
    qreal sx;
    qreal sy;
    QTransform::TransformationType  type;
    PickerDirection direction;
    QPointF origin;

};

class BasePicker : public QGraphicsObject
{
Q_OBJECT

public:
    explicit BasePicker(BaseElement *element);
    virtual void setByElement() = 0;
    //outside =====>> element->setRotation =====>>  m_picker->setElementRotation =====>> element->setElementRotation
    virtual void setElementRotation(qreal angle,const QPointF & center){Q_UNUSED(angle);Q_UNUSED(center);}
    //outside =====>> element->setRect =====>>  m_picker->setElementRect =====>> element->setElementRect
    virtual void setElementRect(const QRectF & r) {Q_UNUSED(r);}
    virtual void adjust(const ElementTransform & t) {Q_UNUSED(t);}

private:
    BaseElement * m_element;
    BaseSpace * m_space;

public:
    inline BaseSpace * space() const {return m_space;}
    inline BaseElement * element() const {return m_element;}

public slots:
    void sceneScaled();

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

    QPointF elementToPicker(const QPointF & p) const;
    QPointF pickerToElement(const QPointF & p) const;

};

#endif // BASESELECTPICKER_H
