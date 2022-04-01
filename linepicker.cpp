#include "linepicker.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "lineelement.h"
#include "basespace.h"
#include "commondef.h"


LinePicker::LinePicker(LineElement *element):
    BasePicker(element),
    m_currentArchor(-1),
    m_lineElement(element)
{
}

LinePicker::~LinePicker()
{
    QVector<LinePickerArchor *>::iterator it = m_archors.begin();
    for(;it!=m_archors.end();++it){
        if (*it){
            delete (*it);
        }
    }

}

void LinePicker::setByElement()
{
    if (m_lineElement->m_points.size()<m_archors.size()){
        QVector<LinePickerArchor *> archorsNew;
        for(int i = 0;i<m_archors.size();++i){
            LinePickerArchor * archor = m_archors.at(i);
            if (m_lineElement->m_points.indexOf(archor->point())<0){
                delete archor;
            }
            else {
                archorsNew.append(archor);
            }
        }
        m_archors = archorsNew;
    }

    QPointF pickerPoint;
    for(int i = 0; i<m_lineElement->m_points.size(); ++i){
        const QPointF & elementPoint = m_lineElement->m_points.at(i);
        LinePickerArchor*  archor = 0;
        if (m_archors.size()<i+1){
            archor = new LinePickerArchor(this);
            m_archors.append(archor);
        }
        else{
            archor = m_archors.at(i);
        }
        pickerPoint = elementToPicker(elementPoint);
        archor->setPoint(elementPoint);
        archor->setPos(pickerPoint);
    }

    Q_ASSERT(m_lineElement->m_points.size()==m_archors.size());
    this->prepareGeometryChange();

}

void LinePicker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
#ifdef DRAW_PICKER_BOUNDING
    painter->setRenderHint(QPainter::Antialiasing,true);
    QPen pen(Qt::gray);
    pen.setWidth(0);
    pen.setCapStyle(Qt::SquareCap);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    QRectF bounding = this->boundingRect();
    painter->drawRect(bounding.x(),bounding.y(),bounding.width(),bounding.height());
#endif
}

QRectF LinePicker::boundingRect() const
{
    QRectF r;
    QVector<LinePickerArchor *>::const_iterator it = m_archors.begin();
    for(;it!=m_archors.end();++it){
        const LinePickerArchor * archor = *it;
        if (*it){
            r = r.united(this->mapFromItem(archor,archor->boundingRect()).boundingRect());
        }
    }
    return r;
}

void LinePicker::setCurrentArchor(LinePickerArchor *archor)
{
    int i = m_archors.indexOf(archor);
    m_currentArchor = i;
}

void LinePicker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_currentArchor>-1){
        QPointF eventPos = event->pos();
        QPointF elementPos = pickerToElement(eventPos);
        LinePickerArchor * archor = m_archors.at(m_currentArchor);
        m_lineElement->replacePoint(archor->point(),elementPos);
        archor->setPoint(elementPos);
        archor->setPos(this->mapToParent(eventPos));
        //qDebug()<<this->mapToParent(event->pos());
        //this->setByElement();
        this->prepareGeometryChange();
    }
}

void LinePicker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_currentArchor>-1){
        space()->setPicking(true);
        element()->beginAdjust();
        event->accept();
    }
}

void LinePicker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_currentArchor>-1){
        space()->setPicking(false);
        element()->endAdjust();
        event->accept();
    }
}



LinePickerArchor::LinePickerArchor(LinePicker *parent):QGraphicsItem(parent),
    m_boundingRect(-DEFAULT_ARCHOR_WIDTH,-DEFAULT_ARCHOR_WIDTH,DEFAULT_ARCHOR_WIDTH*2,DEFAULT_ARCHOR_WIDTH*2),
    m_hovered(false),
    m_picker(parent)
{
    this->setAcceptHoverEvents(true);
}

void LinePickerArchor::setPicked(bool arg1)
{
    m_picked = arg1;
}

void LinePickerArchor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);

    QPen pen(Qt::gray);
    pen.setWidth(0);
    pen.setCapStyle(Qt::SquareCap);
    painter->setPen(pen);

    QBrush brush(m_hovered?Qt::blue:Qt::white);

    painter->setBrush(brush);
    painter->setPen(pen);

    qreal w2 = DEFAULT_ARCHOR_WIDTH * 2;
    QRectF rect(-DEFAULT_ARCHOR_WIDTH+0.5,
                -DEFAULT_ARCHOR_WIDTH+0.5,
                w2-1,
                w2-1);
    painter->drawEllipse(rect);

#ifdef DRAW_PICKER_ARCHOR_BOUNDING
    painter->setRenderHint(QPainter::Antialiasing,true);
    pen.setColor(Qt::gray);
    pen.setWidth(0);
    pen.setCapStyle(Qt::SquareCap);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    QRectF bounding = this->boundingRect();
    painter->drawRect(bounding.x(),bounding.y(),bounding.width(),bounding.height());
#endif

}


QRectF LinePickerArchor::boundingRect() const
{
    return m_boundingRect;
}

void LinePickerArchor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        m_picked = true;
        m_picker->setCurrentArchor(this);
    }
    event->ignore();
}

void LinePickerArchor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        m_picked=false;
        m_picker->setCurrentArchor(-1);
    }
    event->ignore();
}

void LinePickerArchor::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    m_hovered = false;
   qDebug()<<"hover leave";
   this->update();
   event->ignore();

}

void LinePickerArchor::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    m_hovered = true;
    this->update();
    event->ignore();


}

void LinePickerArchor::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    Q_UNUSED(event)
    this->update();
    event->ignore();

}



