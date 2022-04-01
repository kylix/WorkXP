#include "workspace.h"

#include <QCommonStyle>
#include <QScrollBar>
#include "pixmapelement.h"
#include "document.h"
#include "rulebar.h"
#include "utilities.h"

#include <QDebug>

//WorkSheet::WorkSheet(QGraphicsItem *parent):
//    QGraphicsItem(parent),
//    m_geometry(QRectF(0,0,1280,1024)),
//    m_background(Qt::white)
//{
//    //m_outline = new Outline(this);
//}

//QRectF WorkSheet::boundingRect() const
//{
//    return QRectF(QPoint(-1,-1),m_geometry.size()+QSize(2,2));
//}

//void WorkSheet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    Q_UNUSED(option)
//    Q_UNUSED(widget)
//    painter->setBrush(m_background);
//    painter->setPen(QPen(Qt::black,1));
//    painter->drawRect(QRectF(QPointF(-0.5,-0.5),m_geometry.size()+QSize(1,1)));

//}

//void WorkSheet::setSize(const QSizeF &value)
//{
//    if (value!=m_geometry.size()){
//        prepareGeometryChange();
//        m_geometry.setSize(value);
//    }
//}

//void WorkSheet::setGeometry(qreal x, qreal y, qreal w, qreal h)
//{
//    QRectF rect(x,y,w,h);
//    setGeometry(rect);
//}

//void WorkSheet::setGeometry(const QRectF &value)
//{
//   if (value!=m_geometry){
//       prepareGeometryChange();
//       m_geometry = value;
//   }
//}

//void WorkSheet::clear()
//{
//    QList<QGraphicsItem *> children = this->childItems();
//    QList<QGraphicsItem *>::iterator it = children.begin();
//    while(it!=children.end()){
//        this->scene()->removeItem(*it);
//        delete (*it);
//        it++;
//    }

//}

Workspace::Workspace(QWidget *parent):
    BaseSpace(parent),
    m_document(0)
{
    m_hruler = new QtRuleBar(Qt::Horizontal,this,this);
    m_vruler = new QtRuleBar(Qt::Vertical,this,this);
    box = new QtCornerBox(this);
    connect(this,&BaseSpace::scaled,this,&Workspace::updateRuler);
}

Workspace::~Workspace()
{
}

void Workspace::init()
{
    BaseSpace::init();
    this->updateRuler();
}

QPixmap Workspace::getTileRect()
{
    int w = 8;//128;
    int w2 = 2*w;
    QPixmap tile(w2, w2) ;
    tile.fill(Qt::white);
    QPainter pt(&tile);
    QColor color(190, 190, 190);
    pt.fillRect(0, 0, w, w, color);
    pt.fillRect(w, w, w, w, color);
    pt.end();
    return tile;
}

void Workspace::setDocument(Document * const &document)
{
    if (document == m_document)
        return;
    if (m_document){
        disconnect(m_document,0,this,0);
    }
    m_document = document;

    //clear all items

}

void Workspace::changePageSize(QSize pageSize)
{
    Q_UNUSED(pageSize)
}

void Workspace::mouseMoveEvent(QMouseEvent *event)
{
    QPoint eventPos = event->pos();
    m_hruler->updatePosition(eventPos);
    m_vruler->updatePosition(eventPos);

    BaseSpace::mouseMoveEvent(event);
}

void Workspace::resizeEvent(QResizeEvent *event)
{
    BaseSpace::resizeEvent(event);
    this->setViewportMargins(RULER_SIZE-1,RULER_SIZE-1,0,0);
    m_hruler->resize(this->size().width()- RULER_SIZE - 1,RULER_SIZE);
    m_hruler->move(RULER_SIZE,0);
    m_vruler->resize(RULER_SIZE,this->size().height() - RULER_SIZE - 1);
    m_vruler->move(0,RULER_SIZE);

    box->resize(RULER_SIZE,RULER_SIZE);
    box->move(0,0);
    updateRuler();

}

void Workspace::scrollContentsBy(int dx, int dy)
{
    BaseSpace::scrollContentsBy(dx,dy);
    updateRuler();

}

void Workspace::updateRuler()
{
    if ( scene() == 0) return;
    qreal unitFactor = Preferences::instance()->defaultUnit()?PX_PER_INCH:PX_PER_MM;
    QRectF viewbox = this->rect();
    QPointF offset = mapFromScene(scene()->sceneRect().topLeft());
    double factor =  1./transform().m11();
    double lower_x =  factor * ( viewbox.left()  - offset.x() )/unitFactor;
    double upper_x =  factor * ( viewbox.right() -RULER_SIZE- offset.x()  )/unitFactor;

    m_hruler->setRange(lower_x,upper_x,upper_x-lower_x );
    m_hruler->update();

    double lower_y = factor * ( viewbox.top() - offset.y()) * -1/unitFactor;
    double upper_y = factor * ( viewbox.bottom() - RULER_SIZE - offset.y() ) * -1 /unitFactor;

    m_vruler->setRange(lower_y,upper_y,upper_y - lower_y );
    m_vruler->update();

    //qDebug()<<viewbox<<QPoint(lower_x,upper_x) << QPoint(lower_y,upper_y) << offset;
}



