#include "dockpanel.h"
#include "QtAwesome/QtAwesome.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMainWindow>

DockPanelTitleBar::DockPanelTitleBar(QWidget *parent):QWidget(parent),m_expanded(true)
{
    m_closePm = QTAWESOME->icon(fa::close).pixmap(16,16);

}

QSize DockPanelTitleBar::minimumSizeHint() const
{
    return QSize(80,18);
}

void DockPanelTitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    if (!dw) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect();

    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawLine(rect.topLeft(),QPointF(rect.topRight()));

    if (!m_expanded){
        painter.drawLine(rect.bottomLeft(),QPointF(rect.bottomRight()));
    }
    QBrush brush(Qt::black);
    painter.setBrush(brush);
    QPolygonF polygon;
    if (m_expanded){
//        polygon<<QPointF(8,10)<<QPointF(14,18)<<QPointF(20,10);
        polygon<<(QPointF(0,0)+QPointF(4,4))<<(QPointF(6,8)+QPointF(4,4))<<(QPointF(12,0)+QPointF(4,4));
    }
    else{
        //polygon<<QPointF(13,8)<<QPointF(21,14)<<QPointF(13,20);
        polygon<<(QPointF(5,0)+QPointF(3,3))<<(QPointF(13,6)+QPointF(3,3))<<(QPointF(5,12)+QPointF(3,3));
    }
    painter.drawPolygon(polygon);

    QLinearGradient fade(QPointF(0,0),rect.bottomRight());
    fade.setColorAt(0, QColor(255,255,255,150));
    fade.setColorAt(1, QColor(230,230,230,255));
    painter.fillRect(rect,fade);

    painter.drawText(rect.adjusted(22,0,0,0),Qt::AlignLeft|Qt::AlignVCenter,dw->windowTitle());
    painter.drawPixmap(rect.topRight() - QPoint(m_closePm.width() - 1, 0), m_closePm);

}

void DockPanelTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != 0);
    //rect.contains()
}

DockPanel::DockPanel(QMainWindow *parent, Qt::WindowFlags flags):QDockWidget(parent,flags)
{
    this->setTitleBarWidget(new DockPanelTitleBar(this));

}
