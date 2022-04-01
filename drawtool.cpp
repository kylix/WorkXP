#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QRectF>
#include <QDebug>

#include "drawtool.h"
#include "baseelement.h"


DrawTool::DrawTool(BaseElement *element) : QObject(element),m_element(element),m_drawing(false)
{

}

void DrawTool::mouseMove(QGraphicsSceneMouseEvent *event)
{
    if (m_drawing){
        if (m_initPos.isNull()){
            m_initPos = event->scenePos();
        }else{
            m_element->doDrawing(event->scenePos());
        }
    }
}

void DrawTool::mousePress(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        if (!m_drawing){
            m_drawing = true;
            m_initPos = event->scenePos();
            m_element->startDraw(m_initPos);
        }
        else {
            m_element->clickDraw(event->scenePos());

        }
    }

}

void DrawTool::mouseRelease(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        if (event->scenePos()==m_initPos){
            m_drawing = false;
            m_element->abortDraw();
        }
    }
}



void DrawTool::keyPress(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Escape){
        m_drawing = false;
        m_element->abortDraw();

    }

}

void DrawTool::mouseDoubleClick(QGraphicsSceneMouseEvent *event)
{

    m_element->doubleClickDraw(event->scenePos());
}
