#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "basespace.h"
#include "graphicsitemtype.h"
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QWebEngineView;
QT_END_NAMESPACE


class RectElement;
class BaseElement;
class Document;
class Page;
class BaseNode;

class QtRuleBar;
class QtCornerBox;


//class WorkSheet : public QGraphicsItem
//{
//    QRectF m_geometry;
//    QBrush m_background;
//    Outline * m_outline;

//public:
//    enum { Type = UserType+ GRAPHICSITEM_TYPE_WORKSHEET };
//    int type() const
//    {
//        // Enable the use of qgraphicsitem_cast with this item.
//        return Type;
//    }

//public:
//    WorkSheet(QGraphicsItem * parent= Q_NULLPTR);
//    QRectF boundingRect() const override;
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
//                     QWidget *widget) override;

//    void setSize(const QSizeF & value);
//    inline QSizeF size() const {return m_geometry.size();}
//    void setGeometry(qreal x,qreal y,qreal w,qreal h);
//    void setGeometry(const QRectF & value);
//    inline QRectF geometry() const {return m_geometry;}
//    inline void setBackground(const QBrush & value) {m_background = value;}
//    inline QBrush Background() const { return m_background;}
//    void clear();


//};



class Workspace: public BaseSpace
{
Q_OBJECT
    Document * m_document;
    //WorkSheet * m_currentSheet;
    QtRuleBar * m_hruler;
    QtRuleBar * m_vruler;
    QtCornerBox * box;


public:
    Workspace(QWidget *parent = Q_NULLPTR);
    ~Workspace();

    virtual void init() override;

private:
    QPixmap getTileRect();


public slots:
    void setDocument(Document * const &document);

protected:
    void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void scrollContentsBy(int dx, int dy) Q_DECL_OVERRIDE;


private slots:
    void changePageSize(QSize pageSize);
    void updateRuler();


};

#endif // WORKSPACE_H
