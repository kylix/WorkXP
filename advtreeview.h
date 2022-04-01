#ifndef ADVTREEVIEW_H
#define ADVTREEVIEW_H

#include <QTreeView>
#define BM_ICON_WIDTH 0
class AdvTreeView : public QTreeView
{
Q_OBJECT
public:
    AdvTreeView(QWidget *parent = nullptr);
    ~AdvTreeView();

    inline void setMenu(QMenu * menu){m_menu= menu;}
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    virtual void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    virtual void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    virtual void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual bool isAllowChildren(const QModelIndex & ) {return true;}

    virtual void startMoveUnder(void * dragItem,void * dropItem);
    virtual void startMoveAfter(void * dragItem,void * dropItem);
    virtual void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
    void paintInsertPos();
    void paintTriangle(const QPoint &right, QPainter *painter);
    QRect rowVirtualRect(const QModelIndex & index);

    QLine m_oldLine;
    QLine m_newLine;
    bool m_child;
    bool m_middlePos;
    QPoint m_startPos;
    QPoint m_dragHotSpot;
    bool m_paintInsertPos;
    QRect m_dragOverRect;
    QMenu * m_menu;

};

#endif // ADVTREEVIEW_H
