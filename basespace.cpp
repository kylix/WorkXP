#include <QGraphicsItem>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QBitmap>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QOpenGLWidget>
#include "basespace.h"
#include "baseelement.h"
#include "preferences.h"
#include "utilities.h"
#include "QtAwesome/QtAwesome.h"

#include "groupelement.h"

BaseSpace::BaseSpace(QWidget *parent):
    QGraphicsView(parent),
    m_scene(new BaseSpaceScene(this)),
    m_shiftLock(false)
{

    //this code will move to slots of useOpenGl
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setDepthBufferSize(0);   // No need for a depth buffer
    format.setSamples(10);           // Enable anti-aliasing

    QOpenGLWidget *openGLWidget = new QOpenGLWidget(this);
    openGLWidget->setFormat(format);

    this->setViewport(openGLWidget);

    this->setMouseTracking(true);
    this->setObjectName("BaseSpace");
    this->setResizeAnchor(QGraphicsView::NoAnchor);
    this->setTransformationAnchor(QGraphicsView::NoAnchor);
    this->setAlignment(Qt::AlignCenter);
    this->setDragMode(QGraphicsView::NoDrag);
    this->viewport()->setBackgroundRole(QPalette::NoRole);
    this->setFrameShape(QFrame::NoFrame);
    this->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    //this->setCacheMode(QGraphicsView::CacheBackground);
    //this->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing,true);
}

BaseSpace::~BaseSpace()
{

}


BaseElement *BaseSpace::elementAt(const QPointF &scenePos) const
{
    QGraphicsItem * item = scene()->itemAt(scenePos,QTransform());
    if (item) {

        if (item->type()==QGraphicsItem::UserType+GRAPHICSITEM_TYPE_RECT_PICKER){
            return (static_cast<BasePicker *>(item))->element();
        }

        if (item && (item->type()>BaseElement::Type)){
            return static_cast<BaseElement *>(item);
        }

    }
    return 0;
}

void BaseSpace::setDrawing(bool arg1, ElementFactoryMethod method)
{

    m_scene->setDrawing(arg1,method);

}

void BaseSpace::init()
{

    PhysicalSize psize;
    psize.size = Preferences::instance()->defaultPageSize();
    psize.unit = (int)UNIT_TYPE_MM;

    qobject_cast<BaseSpaceScene *>(this->scene())->setPhysicalSize(psize);

    this->fitInView(this->sceneRect().adjusted(8,8,8,8),Qt::KeepAspectRatio);

}

void BaseSpace::setPicking(bool arg1)
{
    m_scene->setPicking(arg1);
}

bool BaseSpace::picking() const
{
    return m_scene->picking();
}

void BaseSpace::emitElementCreated(BaseElement * element)
{
    emit elementCreated(element);
}

QString BaseSpace::nameElementAuto(int graphicsType) const
{
    int i = 0;
    foreach(QGraphicsItem * item, this->scene()->items()){
        if (item->type()==graphicsType){
            ++i;
        }
    }
    switch (graphicsType-QGraphicsItem::UserType) {
    case GRAPHICSITEM_TYPE_LINE:
        return tr("Line").append(QString::number(i));
        break;
    case GRAPHICSITEM_TYPE_BEZIER:
        return tr("Curve").append(QString::number(i));
        break;
    case GRAPHICSITEM_TYPE_TEXT:
        return tr("Text").append(QString::number(i));
        break;
    case GRAPHICSITEM_TYPE_RECT:
        return tr("Rect").append(QString::number(i));
        break;
    case GRAPHICSITEM_TYPE_CIRCLE:
        return tr("Circle").append(QString::number(i));
        break;
    case GRAPHICSITEM_TYPE_ARC:
        return tr("ARC").append(QString::number(i));
        break;
    case GRAPHICSITEM_TYPE_PIXMAP:
        return tr("Picture").append(QString::number(i));
        break;
    default:
        return tr("Shape").append(QString::number(i));
        break;
    }

}

void BaseSpace::pickElements(QList<BaseElement *> & elements)
{
   m_scene->pickElements(elements);
}


void BaseSpace::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    emit scaled();
}


void BaseSpace::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx,dy);
    emit scaled();
}


void BaseSpace::zoomToFit()
{
    this->fitInView(this->sceneRect(),Qt::KeepAspectRatio);
    emit scaled();
}

void BaseSpace::zoom(qreal factor){
    QMatrix matrix;
    matrix.scale(factor, factor);
    if (this->matrix()!=matrix){
        this->setMatrix(matrix);
        emit scaled();
    }
}


BaseSpaceScene::BaseSpaceScene(BaseSpace *space):QGraphicsScene(space),
    m_drawing(false),
    m_picking(false),
    m_elementFactoryMethod(0),
    m_drawingElement(0),
    m_activeElement(0),
    m_space(space)
{
    Q_ASSERT(space!=0);
    space->setScene(this);

    m_groupPicker = new GroupPickerElement(space);
    m_groupPicker->hide();
    setBackgroundBrush(Qt::transparent);

}

void BaseSpaceScene::setDrawing(bool arg1, ElementFactoryMethod method)
{
    m_drawingElement = 0;
    m_elementFactoryMethod = 0;
    if (arg1){
        m_space->viewport()->setCursor(Qt::CrossCursor);
        m_elementFactoryMethod = method;

    }else{
        m_space->viewport()->setCursor(Qt::ArrowCursor);
    }
    m_drawing = arg1;

}

QSize BaseSpaceScene::actualPxSize()
{
    return m_actualPxSize;

}

qreal BaseSpaceScene::scaleFactor()
{
    return this->sceneRect().width()/this->actualPxSize().width();
}

void BaseSpaceScene::setPicking(bool arg1)
{
    m_picking=arg1;
}

void BaseSpaceScene::pickElements(QList<BaseElement *> &elements)
{

    if (elements.count()==0){
        return;
    }
    else if (elements.count()==1){
        if (elements[0]==m_activeElement)
        {
            return;
        }
        else {
            emit elementUnpicked();
            m_groupPicker->clear();
            m_groupPicker->hide();
            m_activeElement = elements[0];
            m_activeElement->showPicker();
            emit elementPicked(m_activeElement);
        }
    }
    else {
        if (Utilities::compare(elements,m_groupPicker->childElements()))
        {
            return;
        }
        else {
            emit elementUnpicked();
            m_groupPicker->clear();
            delete m_groupPicker;
            m_groupPicker =new GroupPickerElement(m_space);
            foreach(BaseElement * element ,elements){
                m_groupPicker->addToGroup(element);
            }
            m_activeElement = m_groupPicker;
            m_groupPicker->show();
            m_groupPicker->showPicker();
            emit elementPicked(m_groupPicker);

        }
    }

}


void BaseSpaceScene::setPhysicalSize(PhysicalSize arg1)
{
    if ((m_physicalSize.size == arg1.size) && (m_physicalSize.unit == arg1.unit))
        return;
    m_physicalSize = arg1;

    m_actualPxSize = m_physicalSize.size * (m_physicalSize.unit?PX_PER_INCH:PX_PER_MM);
    this->setSceneRect(QRect(QPoint(0,0),m_actualPxSize));
    emit physicalSizeChanged(m_physicalSize);

}

void BaseSpaceScene::drawBackground(QPainter *painter, const QRectF &rect)
{

    QGraphicsScene::drawBackground(painter,rect);
    painter->fillRect(sceneRect(),Qt::white);
    //if( m_grid ){
    //    m_grid->paintGrid(painter,sceneRect().toRect());
    //}

}

void BaseSpaceScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if ((!m_picking) && (m_drawing) && (m_drawingElement)){
        m_drawingElement->drawToolMouseMove(event);
    }
}

void BaseSpaceScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->button()==Qt::LeftButton)
    {
        if (m_picking) return;
        emit elementUnpicked();
        if (m_drawing) {
            if (!m_drawingElement){
                m_drawingElement = (m_elementFactoryMethod)(0,m_space,QRectF());
                m_activeElement = m_drawingElement;
                connect(m_drawingElement,
                        &BaseElement::drawAborted,
                        this,
                        [=](){
                        this->removeItem(m_drawingElement);
                        m_drawingElement->deleteLater();
                        m_drawingElement = 0;
                        m_activeElement = 0;
                });
                connect(m_drawingElement,
                        &BaseElement::drawFinished,
                        this,
                        [=](){
                        m_activeElement->showPicker();
                        m_drawingElement = 0;
                        emit elementPicked(m_activeElement);
                });
                emit elementPicked(m_drawingElement);
            }

            m_drawingElement->drawToolMousePress(event);
        }
        else
        {
            //QPointF scenePos = this->mapToScene(event->pos());
            QPointF scenePos = event->scenePos();
            BaseElement * element = m_space->elementAt(scenePos);

            if (element) {
                if (event->modifiers().testFlag(Qt::ShiftModifier)){
                    m_groupPicker->show();
                    if (m_activeElement){
                        m_groupPicker->addToGroup(m_activeElement);
                    }
                    m_activeElement = m_groupPicker;
                    m_groupPicker->addToGroup(element);
                    m_groupPicker->showPicker();
                    emit elementPicked(m_groupPicker);

                }
                else{
                    m_groupPicker->clear();
                    m_groupPicker->hide();
                    m_activeElement = element;
                    element->showPicker();
                    emit elementPicked(element);

                }
            }else{
                m_activeElement = 0;
                m_groupPicker->clear();
                delete m_groupPicker;
                m_groupPicker =new GroupPickerElement(m_space);
                m_groupPicker->hide();
            }

        }

    }
}

void BaseSpaceScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if ((!m_picking) && m_drawing && m_drawingElement) {
        m_drawingElement->drawToolMouseRelease(event);
    }
}

void BaseSpaceScene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);
    if ((!m_picking) && m_drawing && (m_drawingElement)){
        m_drawingElement->drawToolKeyPress(event);
    }
}

void BaseSpaceScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);
    if ((!m_picking) && m_drawing && (m_drawingElement)){
        m_drawingElement->drawToolDoublcClick(event);
    }
}


