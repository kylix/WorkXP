#include "advtreeview.h"
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QApplication>
#include <QHeaderView>
#include <QMenu>

AdvTreeView::AdvTreeView(QWidget *parent):QTreeView(parent),
    m_newLine(QLine()),m_oldLine(QLine()),m_child(true),m_paintInsertPos(false),m_middlePos(false)
{
    //启用拖放
    this->setDragEnabled(true);
    //设置拖放
    this->setAcceptDrops(true);
    //设置显示将要被放置的位置
    this->setDropIndicatorShown(true);
    //设置拖放模式为移动项目，否则为复制项目
    this->setDragDropMode(QAbstractItemView::InternalMove);


}

AdvTreeView::~AdvTreeView()
{

}

void AdvTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
        event->accept();
    else
        event->ignore();

}

void AdvTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        QModelIndex hitTest = indexAt(event->pos());
        event->setDropAction(Qt::MoveAction);
        if(hitTest.isValid())
        {
            QPoint pos = event->pos();
            m_dragOverRect = rowVirtualRect(hitTest);
            m_oldLine = m_newLine;
            m_newLine = QLine(m_dragOverRect.bottomLeft(), m_dragOverRect.bottomRight());
            int left = m_dragOverRect.left() + BM_ICON_WIDTH + 3;
            if(pos.x() > left )
            {
                m_newLine.setP1(QPoint(left, m_newLine.y1()));
                m_child = true;
            }
            else
                m_child = false;
            m_middlePos = isAllowChildren(hitTest) && (qAbs(m_dragOverRect.center().y()-pos.y())<2);

            QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
            void *dragItem = (void*)QVariant(itemData).toLongLong();

            if (dragItem!=hitTest.internalPointer()){
                m_paintInsertPos = true;
            }
            else {
                //在原item则不绘制线
                m_paintInsertPos = false;
                m_newLine = QLine();
            }
            viewport()->repaint();

        }
        event->accept();
    } else
        event->ignore();

}

void AdvTreeView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        void *item = (void*)QVariant(itemData).toLongLong();
        QModelIndex currentIndex = indexAt(event->pos());
        void *pItem = currentIndex.internalPointer();
        if(pItem == item || pItem == NULL)
        {
            event->ignore();
            return;
        }
        if (m_middlePos){
            startMoveUnder(item,pItem);
        }
        else {
            if (isExpanded(currentIndex)){
                startMoveUnder(item,pItem);
            }
            else {
                startMoveAfter(item,pItem);

            }
        }

//        //移除
//        if(item->parent())
//        {
//            item->parent()->removeChild(item);
//        }
//        else
//        {
//            takeTopLevelItem(indexOfTopLevelItem(item));
//        }
//        //插入
//        if(pItem &&m_child)
//        {
//            pItem->insertChild(0, item);
//        }
//        else if(pItem)
//        {
//            QTreeWidgetItem *parentItem = pItem->parent();
//            if(parentItem)
//                parentItem->insertChild(parentItem->indexOfChild(pItem)+1, item);
//            else
//                insertTopLevelItem(indexOfTopLevelItem(pItem)+1, item);
//        }
//        ////////////////////////////////////////////

        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else
        event->ignore();
    QTreeView::dropEvent(event);

}

void AdvTreeView::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);

    qDebug()<<"startDrag";
    QModelIndex current = this->currentIndex();
    if (!current.isValid()) return;

    QString pValue = QString::number(qlonglong(current.internalPointer()));
    QByteArray itemData;
    itemData = QVariant(pValue).toByteArray();

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-qabstractitemmodeldatalist", itemData);


    QRect rowRect = rowVirtualRect(current);
    QPixmap dragRowSnap = this->viewport()->grab(rowRect);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(dragRowSnap);
    //On X11, the pixmap may not be able to keep up with the mouse movements
    //if the hot spot causes the pixmap to be displayed directly under the cursor
    drag->setHotSpot(m_dragHotSpot);


    //设置鼠标移动时，拖动的图片数据
    //QPixmap pPixmap(":/Drag.png");
    //pPixmap = pPixmap.scaledToWidth(2);
    //drag->setPixmap(pPixmap);
    //drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()/2));

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
    {
    }

}

void AdvTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
           int distance = (event->pos() - m_startPos).manhattanLength();
           if (distance >= QApplication::startDragDistance()){
               m_startPos = event->pos();
               startDrag(Qt::MoveAction);
           }
       }
       QTreeView::mouseMoveEvent(event);
}

void AdvTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        m_dragHotSpot = m_startPos + visualRect(currentIndex()).topLeft();
    }
    QTreeView::mousePressEvent(event);
}

void AdvTreeView::paintEvent(QPaintEvent *event)
{

    paintInsertPos();
    QTreeView::paintEvent(event);
}

void AdvTreeView::startMoveUnder(void *dragItem, void *dropItem)
{
    Q_UNUSED(dragItem);
    Q_UNUSED(dropItem);
}

void AdvTreeView::startMoveAfter(void *dragItem, void *dropItem)
{
    Q_UNUSED(dragItem);
    Q_UNUSED(dropItem);
}

void AdvTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    if (m_menu)
       m_menu->exec(event->globalPos());

}

void AdvTreeView::paintInsertPos()
{
    QPainter painter(this->viewport());
    painter.save();
    /***********擦除旧线***********/
    //直线
    QPen pen(palette().base().color());
    //painter.setPen(pen);
    //painter.drawLine(m_oldLine);
    //三角
    //paintTriangle(m_oldLine.p1(), &painter);

    if (m_paintInsertPos) {
        pen.setColor(Qt::green);
        pen.setWidth(1);
        painter.setPen(pen);

        if (m_middlePos){
            painter.drawRect(m_dragOverRect);
        }
        else {
            /***********绘制新线***********/
            //直线
            painter.drawLine(m_newLine);
            //三角
            paintTriangle(m_newLine.p1(), &painter);

        }
    }
    painter.restore();

}

void AdvTreeView::paintTriangle(const QPoint &right, QPainter *painter)
{

    QPoint top = QPoint(right.x()-4, right.y()-3);
    QPoint bottom = QPoint(right.x()-4, right.y()+3);
    painter->drawLine(right, top);
    painter->drawLine(top, bottom);
    painter->drawLine(bottom, right);

}

QRect AdvTreeView::rowVirtualRect(const QModelIndex & index)
{
    QRect rect;

    for (int i = 0;i<this->header()->count();++i){
        if (i!=index.column()){
            rect = rect.united(this->visualRect(index.siblingAtColumn(i)));
        }
        else {
            rect = rect.united(this->visualRect(index));
        }
    }
    return rect;

}
