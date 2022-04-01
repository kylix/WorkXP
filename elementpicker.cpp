#include "elementpicker.h"
#include "baseelement.h"
#include "basespace.h"
#include "utilities.h"
#include "thememanager.h"
#include "groupelement.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QDebug>
#include <QtMath>

ElementPickerArchor::ElementPickerArchor(
        ElementPickerOutline *parent,
        PickerDirection direction,
        ElementPickerArchorShape shape):
    QGraphicsItem(parent),
    m_pickRect(parent),
    m_direction(direction),
    m_shape(shape),
    m_boundingRect(-DEFAULT_ARCHOR_WIDTH,-DEFAULT_ARCHOR_WIDTH,DEFAULT_ARCHOR_WIDTH*2,DEFAULT_ARCHOR_WIDTH*2)
{
    this->setAcceptHoverEvents(true);
}

void ElementPickerArchor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);

    QPen pen(Qt::gray);
    pen.setWidth(0);
    pen.setCapStyle(Qt::SquareCap);
    painter->setPen(pen);
    QBrush brush(Qt::white);

    painter->setBrush(brush);
    painter->setPen(pen);

    qreal w2 = DEFAULT_ARCHOR_WIDTH * 2;
    QRectF rect(-DEFAULT_ARCHOR_WIDTH+0.5,
                -DEFAULT_ARCHOR_WIDTH+0.5,
                w2-1,
                w2-1);
    if(m_shape==ElementPickerArchorShape::PS_RECTANGLE){
        rect  = rect.marginsRemoved(QMarginsF(0.5,0.5,0.5,0.5));
        painter->drawRect(rect);
    }
    else {
        painter->drawEllipse(rect);
    }

}

QRectF ElementPickerArchor::boundingRect() const
{
    return m_boundingRect;
}


void ElementPickerArchor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        this->setPickerDirection(m_direction);
    }
    event->ignore();
}

void ElementPickerArchor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        this->setPickerDirection(PickerDirection::PD_NONE);
    }
    event->ignore();
}

void ElementPickerArchor::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    switch(this->m_direction){
    case PickerDirection::PD_BOTTOM:
        this->setCursor(Qt::SizeVerCursor);
        break;
    case PickerDirection::PD_BOTTOM_LEFT:
        this->setCursor(Qt::SizeBDiagCursor);
        break;
    case PickerDirection::PD_BOTTOM_RIGHT:
        this->setCursor(Qt::SizeFDiagCursor);
        break;
    case PickerDirection::PD_LEFT:
        this->setCursor(Qt::SizeHorCursor);
        break;
    case PickerDirection::PD_MOVE:
        //this->setCursor(Qt::SizeAllCursor);
        break;
    case PickerDirection::PD_RIGHT:
        this->setCursor(Qt::SizeHorCursor);
        break;
    case PickerDirection::PD_TOP:
        this->setCursor(Qt::SizeVerCursor);
        break;
    case PickerDirection::PD_TOP_LEFT:
        this->setCursor(Qt::SizeFDiagCursor);
        break;
    case PickerDirection::PD_TOP_RIGHT:
        this->setCursor(Qt::SizeBDiagCursor);
        break;
    case PickerDirection::PD_ROTATE:
        break;
    default:
        break;
    }
}

void ElementPickerArchor::setPickerDirection(PickerDirection direction)
{
   m_pickRect->setPickerDirection(direction);
}


ElementPickerOutline::ElementPickerOutline(ElementPickerWraper *parent):
    QGraphicsItem(parent),
    m_wraper(parent)
{

    for(int i = 0;i<8;++i){
        m_archors[i] = new ElementPickerArchor(this,PickerDirection::PD_NONE);
    }

    this->setAcceptHoverEvents(true);
}

QRectF ElementPickerOutline::boundingRect() const
{
    QRectF rect;
    for(int i = 0;i<8;++i){
        rect = rect.united(this->mapFromItem(m_archors[i],m_archors[i]->boundingRect()).boundingRect());
    }
    return rect;
}

void ElementPickerOutline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);
    QPen pen(Qt::gray);
    pen.setWidth(0);
    pen.setCapStyle(Qt::SquareCap);
    painter->setPen(pen);
    painter->setBrush(Qt::transparent);
    painter->drawRect(QRectF(-0.5+m_elementRect.x(), -0.5+m_elementRect.y(), m_elementRect.width()+1, m_elementRect.height()+1));

#ifdef DRAW_PICKER_OUTLINE_BOUNDING
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->drawRect(this->boundingRect());
#endif
}

void ElementPickerOutline::setElementRect(const QRectF & rect)
{
    if (!rect.isValid())
        return;
    if (!rect.normalized().isValid())
        return;
    if (rect==m_elementRect)
        return;
    //this->prepareGeometryChange();

    m_elementRect = rect.normalized();
    QPointF center = rect.center();

    m_archors[0]->setPos(rect.topLeft());
    m_archors[0]->setDirection(PickerDirection::PD_TOP_LEFT);

    m_archors[1]->setPos(rect.bottomLeft());
    m_archors[1]->setDirection(PickerDirection::PD_BOTTOM_LEFT);

    m_archors[2]->setPos(rect.topRight());
    m_archors[2]->setDirection(PickerDirection::PD_TOP_RIGHT);

    m_archors[3]->setPos(rect.bottomRight());
    m_archors[3]->setDirection(PickerDirection::PD_BOTTOM_RIGHT);

    m_archors[4]->setPos(QPointF(center.x(),rect.top()));
    m_archors[4]->setDirection(PickerDirection::PD_TOP);

    m_archors[5]->setPos(QPointF(center.x(),rect.bottom()));
    m_archors[5]->setDirection(PickerDirection::PD_BOTTOM);

    m_archors[6]->setPos(QPointF(rect.left(),center.y()));
    m_archors[6]->setDirection(PickerDirection::PD_LEFT);

    m_archors[7]->setPos(QPointF(rect.right(),center.y()));
    m_archors[7]->setDirection(PickerDirection::PD_RIGHT);

}

void ElementPickerOutline::setPickerDirection(PickerDirection direction)
{
    m_wraper->setDirection(direction);
}

ElementTransform ElementPickerOutline::calcTransform(QPointF pos, PickerDirection direction)
{
    //this->prepareGeometryChange();
    ElementTransform tf;
    QRectF rect = m_elementRect;
    QPointF origin = m_elementRect.center();

    if (direction==PickerDirection::PD_NONE) {
        tf.direction = PickerDirection::PD_NONE;
        return tf;
    }
    else if (direction==PickerDirection::PD_ROTATE) {
        tf.direction = PickerDirection::PD_ROTATE;
        return tf;
    }
    else if (direction==PickerDirection::PD_MOVE) {
        rect.moveTo(pos);
        //tf.translate(pos.x() - m_elementRect.left(),pos.y()-m_elementRect.top());
        tf.type = QTransform::TxTranslate;
        tf.dx = pos.x() - m_elementRect.left();
        tf.dy = pos.y() - m_elementRect.top();
        tf.direction = PickerDirection::PD_MOVE;
        return tf;
    }
    else
    {
        if (m_lastPos == m_elementRect.topLeft()){
            rect.setTopLeft(pos);
            m_lastPos = rect.topLeft();
            tf.direction = PickerDirection::PD_TOP_LEFT;
        }
        else if (m_lastPos == m_elementRect.bottomRight()){
            rect.setBottomRight(pos);
            m_lastPos = rect.bottomRight();
            tf.direction = PickerDirection::PD_BOTTOM_RIGHT;
        }
        else if (m_lastPos == m_elementRect.bottomLeft()){
            rect.setBottomLeft(pos);
            m_lastPos = rect.bottomLeft();
            tf.direction = PickerDirection::PD_BOTTOM_LEFT;
        }
        else if (m_lastPos == m_elementRect.topRight()){
            rect.setTopRight(pos);
            m_lastPos = rect.topRight();
            tf.direction = PickerDirection::PD_TOP_RIGHT;
        }
        else if (m_lastPos == QPointF(m_elementRect.center().x(),m_elementRect.top())){
            rect.setTop(pos.y());
            m_lastPos = QPointF(rect.center().x(),rect.top());
            tf.direction = PickerDirection::PD_TOP;
        }
        else if (m_lastPos == QPointF(m_elementRect.center().x(),m_elementRect.bottom())){
            rect.setBottom(pos.y());
            m_lastPos = QPointF(rect.center().x(),rect.bottom());
            tf.direction = PickerDirection::PD_BOTTOM;
        }
        else if (m_lastPos == QPointF(m_elementRect.right(),m_elementRect.center().y())){
            rect.setRight(pos.x());
            m_lastPos = QPointF(rect.right(),rect.center().y());
            tf.direction = PickerDirection::PD_RIGHT;
        }
        else if (m_lastPos == QPointF(m_elementRect.left(),m_elementRect.center().y())){
            rect.setLeft(pos.x());
            m_lastPos = QPointF(rect.left(),rect.center().y());
            tf.direction = PickerDirection::PD_LEFT;
        }
        else {
            return tf;
        }
    }


    origin = m_transformCenter;

    BaseElement * element = this->m_wraper->picker()->element();
    qreal sx = rect.width()/m_elementRect.width(),
          sy = rect.height()/m_elementRect.height();
    if (element->lockScale()){
        switch(direction){
          case PickerDirection::PD_TOP:
          case PickerDirection::PD_BOTTOM:
            sx=sy;
            break;
          default:
            sy=sx;
            break;
        }
    };



    if (!qFuzzyCompare(sx+sy,0)){
        tf.type =  QTransform::TxScale;
        tf.sx = sx;
        tf.sy = sy;
        tf.origin = this->mapToScene(origin);

//        tf.translate(origin.x(),origin.y());
//        tf.scale(sx,sy);
//        tf.translate(-origin.x(),-origin.y());
    }

     return tf;

}

void ElementPickerOutline::adjust(const ElementTransform &t)
{
    //if (!t.isAffine()) return;

    BaseElement * element = this->m_wraper->picker()->element();
    BaseElement * parentElement = element->parentElement();
    qreal angle1 = 0;
    if (parentElement){
        angle1  = parentElement->rotateAngle();
    }
    qreal qcos =m_wraper->transform().m11();

    qreal angle = qRadiansToDegrees(qAcos(qcos));
    if (parentElement){
        angle = element->rotateAngle() - parentElement->rotateAngle();
    }
    else {
        angle = 0;
    }

    QTransform tf;

    //tf.translate(t.m31(),t.m32());

    if (t.type==QTransform::TxTranslate){
        if (!qFuzzyCompare(angle,0)){
            tf.rotate(-angle);
        }
        tf.translate(t.dx,t.dy);
        if (!qFuzzyCompare(angle,0)){
            tf.rotate(angle);
        }
    }

    if (t.type==QTransform::TxScale){
       QPointF c = t.origin;


        c = this->mapFromScene(t.origin);

       if (!qFuzzyCompare(angle,0)){
           //tf.translate(m_elementRect.center().x(),m_elementRect.center().y());
           //tf.rotate(angle);
           //tf.translate(-m_elementRect.center().x(),-m_elementRect.center().y());
       }
        tf.translate(c.x(),c.y());
        tf.scale(t.sx,t.sy);
        //tf.scale(1/qCos(angle),1/qCos(angle));
        tf.translate(-c.x(),-c.y());
        if (!qFuzzyCompare(angle,0)){
            //tf.translate(m_elementRect.center().x(),m_elementRect.center().y());
            //tf.rotate(-angle);
            //tf.translate(-m_elementRect.center().x(),-m_elementRect.center().y());
        }
        if (!tf.isAffine()) return;
        QPolygonF poly = tf.map(Utilities::rectToPoly(m_elementRect));
        QRectF rect = Utilities::polyToRect(poly);
        setElementRect(rect);
        return;

    }

    if (!tf.isAffine()) return;

    setElementRect(tf.mapRect(m_elementRect));
}

void ElementPickerOutline::beginAdjust(PickerDirection direction)
{
    m_transformCenter = m_elementRect.center();
    switch(direction){
      case PickerDirection::PD_TOP_LEFT:
        m_transformCenter = m_elementRect.bottomRight();
        m_lastPos = m_elementRect.topLeft();
        break;
      case PickerDirection::PD_BOTTOM_RIGHT:
        m_transformCenter = m_elementRect.topLeft();
        m_lastPos = m_elementRect.bottomRight();
        break;
      case PickerDirection::PD_TOP_RIGHT:
        m_transformCenter = m_elementRect.bottomLeft();
        m_lastPos = m_elementRect.topRight();
        break;
      case PickerDirection::PD_BOTTOM_LEFT:
        m_transformCenter = m_elementRect.topRight();
        m_lastPos = m_elementRect.bottomLeft();
        break;
      case PickerDirection::PD_TOP:
        m_transformCenter.setY(m_elementRect.bottom());
        m_lastPos = QPointF(m_elementRect.center().x(),m_elementRect.top());
        break;
      case PickerDirection::PD_BOTTOM:
        m_transformCenter.setY(m_elementRect.top());
        m_lastPos = QPointF(m_elementRect.center().x(),m_elementRect.bottom());
        break;
      case PickerDirection::PD_RIGHT:
        m_transformCenter.setX(m_elementRect.left());
        m_lastPos = QPointF(m_elementRect.right(),m_elementRect.center().y());
        break;
      case PickerDirection::PD_LEFT:
        m_transformCenter.setX(m_elementRect.right());
        m_lastPos = QPointF(m_elementRect.left(),m_elementRect.center().y());
        break;
      default:
        m_transformCenter = QPointF();
        break;
    }

}

void ElementPickerOutline::setResizable(bool arg1)
{
    for(int i =0;i<8;++i){
        m_archors[i]->setVisible(arg1);
    }
}

ElementPickerArchor *ElementPickerOutline::archorAt(const QPointF &p) const
{

    for(int i =0;i<8;++i){
        QPointF p1 = this->mapToItem(m_archors[i],p);
        QRectF rect = m_archors[i]->boundingRect();
        if (rect.contains(p1)){
            return m_archors[i];
        }
    }
    return nullptr;
}


ElementPickerRotateArchor::ElementPickerRotateArchor(ElementPickerRotate *rotate):
    QGraphicsItem(rotate),
    m_rotate(rotate),
    m_cursor(THEME->rotateCursorPm())
{
    setAcceptHoverEvents(true);
}

void ElementPickerRotateArchor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);

    QPen pen(Qt::gray);
    pen.setWidth(0);
    pen.setCapStyle(Qt::SquareCap);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->setRenderHint(QPainter::Antialiasing,true);
    qreal h = ElementPickerRotateArchor::HEIGHT, w=ElementPickerRotateArchor::WIDTH;
    painter->drawEllipse(-w/2,-h/2,w,h);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    w-=10;h-=10;
    painter->drawEllipse(-w/2,-h/2,w,h);

    painter->drawRect(this->boundingRect());
}

QRectF ElementPickerRotateArchor::boundingRect() const
{
    qreal h = ElementPickerRotateArchor::HEIGHT, w=ElementPickerRotateArchor::WIDTH;
    return QRectF(-w/2,-h/2,w,h);
}

void ElementPickerRotateArchor::setPickerDirection(PickerDirection direction)
{
    m_rotate->setPickerDirection(direction);
}

BaseSpace *ElementPickerRotateArchor::space() const
{
    return m_rotate->space();
}

void ElementPickerRotateArchor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        m_rotate->setPickerDirection(PickerDirection::PD_ROTATE);
    }
    event->ignore();

}

void ElementPickerRotateArchor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        this->setPickerDirection(PickerDirection::PD_NONE);
    }
    event->ignore();

}

void ElementPickerRotateArchor::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    this->setCursor(m_cursor);
}

ElementPickerRotate::ElementPickerRotate(ElementPickerWraper *wraper):
    QGraphicsItem(wraper),
    m_wraper(wraper)
{
    m_archor = new ElementPickerRotateArchor(this);
    m_archor->setPos(0,-10-m_archor->boundingRect().height()/2);

}

void ElementPickerRotate::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);
    QPen pen(Qt::gray);
    pen.setWidth(0);
    pen.setCapStyle(Qt::SquareCap);
    painter->setPen(pen);
    painter->drawLine(QPointF(0,0),QPointF(0,-10));

#ifdef DRAW_PICKER_ROTATE_BOUNDING
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->drawRect(this->boundingRect());
#endif
}

QRectF ElementPickerRotate::boundingRect() const
{
    QRectF archorRect = this->mapFromItem(m_archor,m_archor->boundingRect()).boundingRect();
    QRectF lineRect(QPointF(0,0),QPointF(0,-10));
    return archorRect.united(lineRect);
}

void ElementPickerRotate::setPickerDirection(PickerDirection direction)
{
    m_wraper->setDirection(direction);
}

BaseSpace *ElementPickerRotate::space() const
{
  return m_wraper->space();
}

ElementPickerWraper::ElementPickerWraper(ElementPicker * picker):
    QGraphicsItem(picker),
    m_picker(picker),
    m_rotate_angle(0)
{
    Q_ASSERT(picker);
    m_rotate = new ElementPickerRotate(this);
    m_outline = new ElementPickerOutline(this);
    m_outline->setResizable(picker->element()->resizable());
    m_outline->setPos(0,0);
}

void ElementPickerWraper::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
#ifdef DRAW_PICKER_WRAPER_BOUNDING
    painter->setRenderHint(QPainter::Antialiasing,true);
    QPen pen(Qt::gray);
    pen.setWidth(1);
    pen.setCapStyle(Qt::SquareCap);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->drawRect(this->boundingRect());
#else
    Q_UNUSED(painter)
#endif
}

QRectF ElementPickerWraper::boundingRect() const
{
    QRectF outlineRect = this->mapRectFromItem(m_outline,m_outline->boundingRect());
    QRectF rotateRect = this->mapRectFromItem(m_rotate,m_rotate->boundingRect());
    return outlineRect.united(rotateRect);
}

void ElementPickerWraper::setDirection(PickerDirection direction)
{
    m_picker->setDirection(direction);
}

BaseSpace *ElementPickerWraper::space()
{
    return m_picker->space();
}

ElementTransform ElementPickerWraper::calcTransform(QPointF pos, PickerDirection direction)
{
    ElementTransform tf;
    //this->prepareGeometryChange();
    tf = m_outline->calcTransform(this->mapToItem(m_outline,pos),direction);

    //Reset the rotation handler position
    QRectF rect = elementRect().normalized();
    m_rotate->setPos(rect.center().x(),rect.y());
    return tf;
}

void ElementPickerWraper::adjust(const ElementTransform &t)
{
    m_outline->adjust(t);

    //Reset the rotation handler position
    QRectF rect = elementRect().normalized();
    m_rotate->setPos(rect.center().x(),rect.y());
}

void ElementPickerWraper::beginAdjust(PickerDirection direction)
{
    m_outline->beginAdjust(direction);
}


void ElementPickerWraper::rotateByCenter(qreal angle,const QPointF & c)
{
    if (angle==0)
        return;
    //this->prepareGeometryChange();

    QRectF bbox = this->elementRect().normalized();
    QPointF center = c;
    if (c.isNull()){
        center = bbox.center();
    }
    QTransform transform = QTransform::fromTranslate(center.x(), center.y());
    transform.rotate(angle,Qt::ZAxis);

    transform.translate(-center.x(), -center.y());
    this->setTransform(transform,true);
    m_rotate_angle += angle;
}

ElementPickerArchor *ElementPickerWraper::archorAt(const QPointF &p) const
{
    QPointF p1 = this->mapToItem(m_outline,p);
    return m_outline->archorAt(p1);
}

//void ElementPickerWraper::rotateByCenter

QRectF ElementPickerWraper::elementRect() const
{
    return this->mapRectFromItem(m_outline,m_outline->elementRect());
}

void ElementPickerWraper::setElementRect(const QRectF & rect)
{
    if (this->elementRect()==rect)
        return;
    this->prepareGeometryChange();
    m_outline->setElementRect(this->mapRectToItem(m_outline,rect));

    QRectF r = rect.normalized();
    m_rotate->setPos(r.center().x(),r.y());
}

ElementPicker::ElementPicker(BaseElement *element):BasePicker(element)
{
    m_wraper = new ElementPickerWraper(this);

}

PickerDirection ElementPicker::direction(const QPointF & p) const
{
    if ((m_direction==PickerDirection::PD_MOVE) || (m_direction==PickerDirection::PD_ROTATE))
        return m_direction;


    QPointF p1 = this->mapToItem(m_wraper,p);
    ElementPickerArchor * archor = this->m_wraper->archorAt(p1);


    if (!archor){
        return PickerDirection::PD_NONE;
    }

    return archor->direction();
}

void ElementPicker::setByElement()
{
    BaseElement * element = this->element();
    BaseSpace * space = this->space();
    QPolygonF poly = Utilities::rectToPoly(element->rect());
    poly = element->deviceTransform(space->viewportTransform()).map(poly);
    poly = m_wraper->deviceTransform(space->viewportTransform()).inverted().map(poly);
    m_wraper->setElementRect(Utilities::polyToRect(poly));
    //m_wraper->rotateByCenter(element->rotateAngle());
}

void ElementPicker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
#ifdef DRAW_PICKER_BOUNDING
    painter->setRenderHint(QPainter::Antialiasing,true);
    QPen pen(Qt::gray);
    pen.setWidth(1);
    pen.setCapStyle(Qt::SquareCap);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    QRectF bounding = this->boundingRect();
    painter->drawRect(bounding.x(),bounding.y(),bounding.width(),bounding.height());
    QPolygonF poly = this->mapFromItem(m_wraper,m_wraper->boundingRect());
    painter->drawPolygon(poly);
#else
    Q_UNUSED(painter)
#endif

}

QRectF ElementPicker::boundingRect() const
{
    return this->mapRectFromItem(m_wraper,m_wraper->boundingRect());
}

QPainterPath ElementPicker::shape() const
{
    QPolygonF poly = this->mapFromItem(m_wraper,m_wraper->boundingRect());
    QPainterPath path;
    path.addPolygon(poly);
    return path;
}

void ElementPicker::setElementRotation(qreal angle,const QPointF & center)
{
    if (qFuzzyCompare(angle,0))
            return;

    BaseElement * element = this->element();
    BaseSpace * space = element->space();


    QPointF centerPoint = center.isNull()?element->rect().center():center;

    //transform center point from element to picker's wrapper
    QPointF centerWraper = element->deviceTransform(space->viewportTransform()).map(centerPoint);
    centerWraper = m_wraper->deviceTransform(space->viewportTransform()).inverted().map(centerWraper);
    this->m_wraper->rotateByCenter(angle,centerWraper);


     element->setTransform(QTransform()
                           .translate(centerPoint.x(), centerPoint.y())
                           .rotate(angle)
                           .translate(-centerPoint.x(), -centerPoint.y()),true);
    setByElement();

}

void ElementPicker::setElementRect(const QRectF &rect)
{

    BaseElement * element = this->element();
    element->setRect(rect);
    BaseSpace * space = this->space();
    QPolygonF poly = Utilities::rectToPoly(rect);
    poly = element->deviceTransform(space->viewportTransform()).map(poly);
    poly = m_wraper->deviceTransform(space->viewportTransform()).inverted().map(poly);
    m_wraper->setElementRect(Utilities::polyToRect(poly));
}

void ElementPicker::adjust(const ElementTransform &t)
{
    BaseSpace * space = this->space();
    BaseElement * element = this->element();
    m_wraper->adjust(t);

    BaseGroupElement * groupElement = qobject_cast<BaseGroupElement *>(element);
    if (groupElement){
        foreach(BaseElement * childElement ,groupElement->childElements()){
            childElement->picker()->adjust(t);
        }
    }


    QPolygonF poly = Utilities::rectToPoly(m_wraper->elementRect());
    //poly = m_wraper->transform().inverted().map(poly);
    poly = m_wraper->deviceTransform(space->viewportTransform()).map(poly);
    poly = element->deviceTransform(space->viewportTransform()).inverted().map(poly);


    //poly = element->mapFromParent(poly);
    //poly = element->transform().map(poly);
    QRectF rectNew = Utilities::polyToRect(poly);

    element->setRect(rectNew);



}

void ElementPicker::beginAdjust(PickerDirection direction)
{
   m_wraper->beginAdjust(m_direction);
}

void ElementPicker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    BaseElement * element = this->element();
    BaseSpace * space = this->space();

    if (m_direction==PickerDirection::PD_NONE) return;

    QPointF mousePos = event->pos();
    if (PickerDirection::PD_ROTATE == m_direction) {
        QPointF centerPos = this->mapFromItem(m_wraper,m_wraper->elementRect().center());
        QLineF line1(centerPos,mousePos);
        QLineF line2(centerPos,m_start_pos);
        qreal angle = line1.angleTo(line2);
        m_start_pos = mousePos;
        element->setRotateAngle(angle);

    }
    else {
        if (PickerDirection::PD_MOVE == m_direction){
            mousePos = mousePos-m_start_offset;
        }

        //The transform is based on picker Coordinate System, so all the adjust is by picker, not element itself
        ElementTransform tf = m_wraper->calcTransform(this->mapToItem(m_wraper,mousePos),m_direction);
        this->adjust(tf);

        QPolygonF poly = Utilities::rectToPoly(m_wraper->elementRect());
        //poly = m_wraper->transform().inverted().map(poly);
        poly = m_wraper->deviceTransform(space->viewportTransform()).map(poly);
        poly = element->deviceTransform(space->viewportTransform()).inverted().map(poly);
        //poly = element->transform().map(poly);
        QRectF rectNew = Utilities::polyToRect(poly);

        element->setRect(rectNew);
    }
    event->accept();
}

void ElementPicker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        m_prevCursor = this->space()->viewport()->cursor();
        space()->setPicking(true);
        QPointF topLeft = this->mapFromItem(m_wraper,this->m_wraper->elementRect().topLeft());
        m_start_offset =event->pos()-topLeft;
        m_start_pos = event->pos();
        if (m_direction==PickerDirection::PD_NONE){
            setDirection(PickerDirection::PD_MOVE);

        }
        if (m_direction==PickerDirection::PD_MOVE){
            this->space()->viewport()->setCursor(Qt::SizeAllCursor);
        }
        beginAdjust(m_direction);
        element()->beginAdjust();
        event->accept();
    }
}


void ElementPicker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        element()->endAdjust();
        this->setDirection(PickerDirection::PD_NONE);
        space()->setPicking(false);
        this->space()->viewport()->setCursor(m_prevCursor);
        event->accept();
    }
}



