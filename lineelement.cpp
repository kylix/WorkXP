#include "lineelement.h"
#include "linepicker.h"
#include "drawtool.h"
#include "basespace.h"
#include <QDebug>
#include <QOpenGLPaintDevice>

#include <QPainter>

LineElement::LineElement(BaseElement *parent, BaseSpace *space, QRectF initRect):
    BaseElement(parent,space,initRect)
{
    setElementName(m_space->nameElementAuto(this->type()));
    m_picker = this->createPicker();
    m_pen.setWidth(1);
    m_pen.setStyle(Qt::SolidLine);
    m_pen.setColor(Qt::black);

}

QRectF LineElement::calcBoundingRect() const
{
    QPointF p;
    qreal x1 = 0,x2 = 0,y1=0,y2=0;
    for(int i = 0;i<m_points.size();++i){
        const QPointF & p = m_points.at(i);
        if (i==0){
            x1 = x2 = p.x();
            y1 = y2 = p.y();
        }
        x1 = (p.x()<=x1)?p.x():x1;
        y1 = (p.y()<=y1)?p.y():y1;
        x2 = (p.x()>=x2)?p.x():x2;
        y2 = (p.y()>=y2)?p.y():y2;

    }
    return QRectF(QPointF(x1,y1),QPointF(x2,y2));

}


void LineElement::doDrawing(const QPointF & p)
{

    QPointF p1 = this->mapFromScene(p);
    if (m_points.count()>1){
        m_points.replace(m_points.count()-1,p1);
    }
    this->setRect(calcBoundingRect());
    this->prepareGeometryChange();
}

void LineElement::startDraw(const QPointF & p)
{
    QPointF p1 = this->mapFromScene(p);
    m_points.clear();
    m_points.append(p1);
    m_points.append(p1);

    this->setRect(calcBoundingRect());
    this->prepareGeometryChange();
}

void LineElement::clickDraw(const QPointF & p)
{
    if (m_points.count()>0){
        QPointF p1 = this->mapFromScene(p);
        m_points.replace(m_points.count()-1,p1);
        m_points.append(p1);
        this->setRect(calcBoundingRect());
        this->prepareGeometryChange();
    }

}

void LineElement::doubleClickDraw(const QPointF &scenePos)
{
    Q_UNUSED(scenePos)
    if (m_points.count()>0){
        //m_polygon.replace(m_polygon.count()-1,this->mapFromScene(p));
        //QPointF p1 = this->mapFromScene(p);
        //m_points.replace(m_points.count()-1,scenePos);

        //remove the last point on last left clicking
        m_points.remove(m_points.count()-1);

        this->setRect(calcBoundingRect());
        m_picker->show();
        m_drawTool->stop();
        this->prepareGeometryChange();
        emit drawFinished();
    }

}

void LineElement::replacePoint(const QPointF &p1, const QPointF &p2)
{
    int index = m_points.indexOf(p1);
    if (index>=0){
        m_points.replace(index,p2);
        this->setRect(calcBoundingRect());
        //qDebug()<<"replacePoint";
        this->prepareGeometryChange();
    }

}




void LineElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //painter->beginNativePainting();
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(m_pen);
    QPointF p1,p2;
    QVector<QPointF>::const_iterator it = m_points.begin();
    p1 = (*it);
    ++it;
    for(;it<m_points.end();++it){
        p2 = (*it);
        painter->drawLine(p1,p2);
        p1 = p2;
    }
    BaseElement::paint(painter,option,widget);


    //painter->endNativePainting();

}

BasePicker *LineElement::createPicker()
{
    return new LinePicker(this);
}

void LineElement::setPen(const  QPen & pen)
{
    if (m_pen == pen)
        return;

    m_pen = pen;
    emit penChanged(m_pen);
}


