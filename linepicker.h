#ifndef LINEPICKER_H
#define LINEPICKER_H

#include "basepicker.h"

class BaseElement;
class LineElement;
class LinePicker;
#define DEFAULT_ARCHOR_WIDTH 6

class LinePickerArchor : public QGraphicsItem
{
    QRectF m_boundingRect;
    bool m_picked;
    QPointF m_point;
    bool m_hovered;

public:
    LinePickerArchor(LinePicker * parent);
    void setPicked(bool arg1);
    inline bool picked() const {return m_picked;}

    inline const QPointF & point() const { return m_point;}
    inline void setPoint(const QPointF & p ) { m_point = p;}

    QRectF boundingRect() const override;


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

    LinePicker * m_picker;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;

};

class LinePicker : public BasePicker
{
public:
    explicit LinePicker(LineElement * element);
    ~LinePicker();
    void setByElement() override;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void setCurrentArchor( LinePickerArchor * archor);
    inline void setCurrentArchor(int arg1) { m_currentArchor = arg1;}

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    //void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    //void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

private:
    //QList<LinePickerArchor *> m_anchors;
    int m_currentArchor;
    QVector<LinePickerArchor *> m_archors;
protected:
    LineElement * m_lineElement;

};

#endif // LINEPICKER_H
