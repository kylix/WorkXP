#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include <QObject>
#include <QPointF>

class BaseElement;
class QGraphicsSceneMouseEvent;
class QKeyEvent;

class DrawTool : public QObject
{
Q_OBJECT

    BaseElement * m_element;
    bool m_drawing;
    QPointF m_initPos;

public:
    DrawTool(BaseElement * element);
    void mouseMove(QGraphicsSceneMouseEvent * event);
    void mousePress(QGraphicsSceneMouseEvent *event);
    void mouseRelease(QGraphicsSceneMouseEvent *event);
    void keyPress(QKeyEvent *event);
    void mouseDoubleClick(QGraphicsSceneMouseEvent *event);

    const QPointF & initPos() const { return m_initPos;}

    inline void stop() { m_drawing=false;}

};

#endif // DRAWTOOL_H
