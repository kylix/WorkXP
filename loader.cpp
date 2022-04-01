#include "loader.h"
#include <QPainter>
#include <QDebug>
#include <QtMath>

LoaderPixmap::LoaderPixmap(QGraphicsItem *parent):
    QGraphicsObject(parent),
    m_size(120,120),
    m_progress(0),
    m_parent(parent)
{
    this->setCacheMode(QGraphicsItem::NoCache);
    m_loaderPixmap.load(":/styles/light/loading.png");
}


QRectF LoaderPixmap::boundingRect() const
{
    qreal halfw = (m_size.width()/2)/qSin(45);
    return QRectF(QPointF(-halfw,-halfw),QSizeF(halfw*2,halfw*2));

}

void LoaderPixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->rotate(m_rotate);
    qreal halfw = m_size.width()/2;
    painter->translate(-halfw,-halfw);

    painter->drawPixmap(QRectF(QPointF(0,0),
                               m_size),m_loaderPixmap,m_loaderPixmap.rect());
    painter->restore();
    painter->setPen(Qt::white);
    painter->drawText(boundingRect(),Qt::AlignCenter,QString("%1%").arg(m_progress));
}



void LoaderPixmap::setSize(const QSizeF &value)
{
   if (m_size!=value){
       m_size = value;
   }
}

void LoaderPixmap::start()
{
    connect(&m_timer,&QTimer::timeout, this, [this](){
        m_rotate+=1;
        if (m_rotate>=360) m_rotate = 0;
        m_parent->update();
    });

    QRectF parentRect = QRectF(m_parent->pos(),m_parent->boundingRect().size());
    QPointF c = parentRect.center();
    this->setPos(c);
    m_timer.start(10);

}

void LoaderPixmap::stop()
{
    m_timer.stop();
}

void LoaderPixmap::setProgress(int progress)
{
    m_progress = progress;
}




Loader::Loader(QGraphicsItem *parent) :
    QGraphicsObject(parent),
    m_loaderPixmap(new LoaderPixmap(this)),
    m_parent(parent)
{
    this->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    this->setOpacity(0.68);
    this->hide();
}

QRectF Loader::boundingRect() const
{
    return QRectF(QPointF(0,0),m_parent->boundingRect().size());

}

void Loader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    QBrush brush;
    brush.setColor(QColor(144,144,144,50));
    painter->setBrush(Qt::gray);
    painter->drawRect(boundingRect());
    painter->restore();
}



void Loader::start()
{
    this->setPos(0,0);
  m_loaderPixmap->start();
  this->show();
}

void Loader::stop()
{
    this->hide();
    m_loaderPixmap->stop();
}

void Loader::setProgress(int progress)
{
    m_loaderPixmap->setProgress(progress);
}




