#include "basespace.h"
#include "baseelement.h"
#include "drawtool.h"
#include "preferences.h"
#include "elementpicker.h"
#include "groupelement.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>


BaseElement::BaseElement(
        BaseElement *parent,
        BaseSpace *space,
        QRectF initRect ):
    QGraphicsObject(parent),
    m_flags(ELEMENT_PICKABLE),
    m_adjusting(false),
    m_lockScale(false),
    m_rotateAngle(0),
    m_space(space),
    m_parentElement(0)
{
    m_drawTool = new DrawTool(this);
    this->setFlag(QGraphicsItem::ItemIsSelectable,false);
    this->setFlag(QGraphicsItem::ItemIsMovable,false);
    this->setFlag(QGraphicsItem::ItemIsFocusable,false);
    this->setFlag(QGraphicsItem::ItemAcceptsInputMethod,false);

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);

    this->setRect(initRect);
    BaseSpaceScene * scene = qobject_cast<BaseSpaceScene *>(m_space->scene());
    scene->addItem(this);
    connect(scene,&BaseSpaceScene::elementUnpicked,this,&BaseElement::hidePicker);
    m_space->emitElementCreated(this);

}

BaseElement::~BaseElement()
{
 m_picker->deleteLater();
}

void BaseElement::beginAdjust()
{
    m_adjusting = true;
    this->setOpacity(BASEELEMENT_ADJUSTING_OPACITY);
}

void BaseElement::endAdjust()
{
    m_adjusting = false;
    this->setOpacity(1);
}


void BaseElement::showPicker()
{
    //m_picker->grabItem(this);
    m_picker->show();
}

void BaseElement::setRect(const QRectF & r)
{
    if (!r.isValid())
        return;
    if (r == m_rect)
        return;
    m_rect = r.normalized();
    if (m_picker){
        m_picker->setElementRect(r);
    }
    this->prepareGeometryChange();
    emit rectChanged(r);
}



void BaseElement::hidePicker()
{
    this->m_picker->hide();
}

void BaseElement::setRotateAngle(qreal arg1,const QPointF & center)
{
    if (qFuzzyCompare(arg1,0))
            return;


    BaseGroupElement * groupElement = qobject_cast<BaseGroupElement *>(this);
    if (groupElement){
        QPointF centerPoint = this->rect().center();
        foreach(QGraphicsItem * item ,groupElement->childElements()){
            if (item->type()>QGraphicsItem::UserType+GRAPHICSITEM_TYPE_BASEELEMENT){
                BaseElement * childElement = static_cast<BaseElement * >(item);
                childElement->setRotateAngle(arg1,this->mapToItem(childElement,centerPoint));
            }
        }
    }



    m_rotateAngle += arg1;

    m_picker->setElementRotation(arg1,center);
    this->prepareGeometryChange();
    emit rotateAngleChanged(m_rotateAngle);

}

void BaseElement::setLockScale(bool lockScale)
{
    if (m_lockScale == lockScale)
        return;

    m_lockScale = lockScale;
    m_scaleRatio = m_rect.width()/m_rect.height();
    emit lockScaleChanged(m_lockScale);
}

void BaseElement::setElementName(QString elementName)
{
    if (m_elementName == elementName)
        return;

    m_elementName = elementName;
    emit elementNameChanged(m_elementName);

}



//void BaseElement::changeParent()
//{
//    BaseSpace * space = Q_NULLPTR;
//    QList<QGraphicsView *>  views = this->scene()->views();
//    Q_ASSERT(views.length()==1);
//    if (views[0]->objectName().toUpper()=="BASESPACE"){
//        space = qobject_cast<BaseSpace *>(views[0]);
//    }
//}

QRectF BaseElement::boundingRect() const
{
    return m_rect;
}

QRect BaseElement::defaultPhysicalRect() const
{
    return physicalRect(Preferences::instance()->defaultUnit());

}

void BaseElement::setDefaultPhysicalRect(const QRect &rect)
{
    setPhysicalRect(rect,Preferences::instance()->defaultUnit());
}

QRect BaseElement::physicalRect(int unit) const
{
    qreal nPxPerUnit = unit?PX_PER_INCH:PX_PER_MM;
    QPointF topLeft = m_rect.topLeft();
    int x = qRound(topLeft.x() / nPxPerUnit);
    int y = qRound(topLeft.y() / nPxPerUnit);
    int w = qRound(m_rect.width() / nPxPerUnit);
    int h = qRound(m_rect.height() / nPxPerUnit);

    return QRect(x,y,w,h);
}

void BaseElement::setPhysicalRect(const QRect &rect, int unit)
{

    qreal nPxPerUnit = unit?PX_PER_INCH:PX_PER_MM;
    QPointF topLeft = rect.topLeft();
    qreal x = topLeft.x() * nPxPerUnit;
    qreal y = topLeft.y() * nPxPerUnit;
    qreal w = rect.width() * nPxPerUnit;
    qreal h = rect.height() * nPxPerUnit;


    setRect(QRectF(x,y,w,h));
}

QPoint BaseElement::defaultPhysicalPos() const
{
    return physicalPos(Preferences::instance()->defaultUnit());
}

void BaseElement::setDefaultPhysicalPos(const QPoint &pos)
{
    setPhysicalPos(pos,Preferences::instance()->defaultUnit());
}

QPoint BaseElement::physicalPos(int unit) const
{
    qreal nPxPerUnit = unit?PX_PER_INCH:PX_PER_MM;
    QPointF p = this->mapToScene(m_rect.topLeft());
    int x = qRound(p.x()/nPxPerUnit);
    int y = qRound(p.y()/nPxPerUnit);
    return QPoint(x,y);
}

void BaseElement::setPhysicalPos(const QPoint &pos,int unit)
{
    qreal nPxPerUnit = unit?PX_PER_INCH:PX_PER_MM;
    qreal x = pos.x() * nPxPerUnit;
    qreal y = pos.y() * nPxPerUnit;
    m_rect.moveTopLeft(QPointF(x,y));
    //this->setPos(QPointF(x,y) - m_rect.topLeft());
    this->setRect(m_rect);
}


void BaseElement::doDrawing(const QPointF & p)
{
    this->setRect(QRectF(m_drawTool->initPos(),p));
}

void BaseElement::startDraw(const QPointF & )
{
    m_picker->hide();
}

void BaseElement::doubleClickDraw(const QPointF &)
{

}

void BaseElement::clickDraw(const QPointF & )
{
    m_picker->show();
    m_drawTool->stop();
    emit drawFinished();
}

void BaseElement::abortDraw()
{
    this->deleteLater();
    emit drawAborted();
}

void BaseElement::drawToolMouseMove(QGraphicsSceneMouseEvent *event)
{
    m_drawTool->mouseMove(event);
}

void BaseElement::drawToolMousePress(QGraphicsSceneMouseEvent *event)
{
    m_drawTool->mousePress(event);

}

void BaseElement::drawToolMouseRelease(QGraphicsSceneMouseEvent *event)
{
    m_drawTool->mouseRelease(event);

}

void BaseElement::drawToolKeyPress(QKeyEvent *event)
{
    m_drawTool->keyPress(event);

}

void BaseElement::drawToolDoublcClick(QGraphicsSceneMouseEvent *event)
{
    m_drawTool->mouseDoubleClick(event);
}

QVariant BaseElement::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change){
    case QGraphicsItem::ItemEnabledChange:
        break;
    case QGraphicsItem::ItemEnabledHasChanged:
        break;
    case QGraphicsItem::ItemPositionChange:
        break;
    case QGraphicsItem::ItemPositionHasChanged:
        break;
    case QGraphicsItem::ItemTransformChange:
        break;
    case QGraphicsItem::ItemTransformHasChanged:
        //m_picker->elementChanged(boundingRect(),transform());
        //m_picker->setByElement();
        //qDebug()<<m_rect;
        //emit transformChanged(transform());
        break;
    case QGraphicsItem::ItemRotationChange:
        break;
    case QGraphicsItem::ItemRotationHasChanged:
        break;
    case QGraphicsItem::ItemScaleChange:
        break;
    case QGraphicsItem::ItemScaleHasChanged:
        break;
    case QGraphicsItem::ItemTransformOriginPointChange:
        break;
    case QGraphicsItem::ItemTransformOriginPointHasChanged:
        break;
    case QGraphicsItem::ItemSelectedChange:
        break;
    case QGraphicsItem::ItemSelectedHasChanged:
        break;
    case QGraphicsItem::ItemVisibleChange:
        if (!value.toBool()){
            m_picker->setVisible(false);
        }
        break;
    case QGraphicsItem::ItemVisibleHasChanged:
        break;
    case QGraphicsItem::ItemParentChange:
        break;
    case QGraphicsItem::ItemParentHasChanged:
        break;
    case QGraphicsItem::ItemChildAddedChange:
        break;
    case QGraphicsItem::ItemChildRemovedChange:
        break;
    case QGraphicsItem::ItemSceneChange:
        break;
    case QGraphicsItem::ItemSceneHasChanged:
        break;
    case QGraphicsItem::ItemCursorChange:
        break;
    case QGraphicsItem::ItemCursorHasChanged:
        break;
    case QGraphicsItem::ItemToolTipChange:
        break;
    case QGraphicsItem::ItemToolTipHasChanged:
        break;
    case QGraphicsItem::ItemFlagsChange:
        break;
    case QGraphicsItem::ItemFlagsHaveChanged:
        break;
    case QGraphicsItem::ItemZValueChange:
        break;
    case QGraphicsItem::ItemZValueHasChanged:
        break;
    case QGraphicsItem::ItemOpacityChange:
        break;
    case QGraphicsItem::ItemOpacityHasChanged:
        break;
    case QGraphicsItem::ItemScenePositionHasChanged:
        //this->m_picker->setByElement();
        //emit posChanged(value.toPointF());
        break;
    default:
        break;
    }
    return QGraphicsItem::itemChange(change, value);


}

void BaseElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

#ifdef DRAW_ELEMENT_BOUNDING
    painter->setRenderHint(QPainter::Antialiasing,true);
    QPen pen(Qt::red);
    pen.setWidth(2);
    pen.setCapStyle(Qt::SquareCap);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    QRectF bounding = this->boundingRect();
    painter->drawRect(bounding.x(),bounding.y(),bounding.width(),bounding.height());
#endif

}
