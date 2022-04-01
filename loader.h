#ifndef LOADER_H
#define LOADER_H

#include <QGraphicsObject>
#include <QSizeF>
#include <QPixmap>
#include <QTimer>
#include "graphicsitemtype.h"

class LoaderPixmap : public QGraphicsObject
{

    QSizeF m_size;

    QPixmap m_loaderPixmap;
    QPixmap m_pixmap;
    int m_rotate;
    int m_progress;
    QTimer m_timer;
    QGraphicsItem * m_parent;
public:
    enum { Type = UserType+ GRAPHICSITEM_TYPE_LOADERPIXMAP };
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

public:
    LoaderPixmap(QGraphicsItem * parent = Q_NULLPTR);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) override;

    //inline QSizeF size() {return m_size;} const;
    void setSize(const QSizeF & value);
    void start();
    void stop();
    void setProgress(int progress);

};

class Loader : public QGraphicsObject
{
Q_OBJECT
    LoaderPixmap * m_loaderPixmap;
    QGraphicsItem * m_parent;

public:
    enum { Type = UserType+ GRAPHICSITEM_TYPE_LOADER };
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }


public:
    Loader(QGraphicsItem *parent = Q_NULLPTR);


    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) override;

    void start();
    void stop();


public slots:
    void setProgress(int progress);


};

#endif // LOADER_H
