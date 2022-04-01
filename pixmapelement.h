#ifndef PIXMAPELEMENT_H
#define PIXMAPELEMENT_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QRectF>
#include <QRunnable>

#include "baseelement.h"

class PixmapNode;
class Loader;
class BaseSpace;

class PixmapElement : public BaseElement
{

    QPixmap m_pixmap;
    QString m_originName;

    bool m_loading;
    Loader * m_loader;

public:
    enum { Type = UserType + GRAPHICSITEM_TYPE_PIXMAP };
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    PixmapElement(BaseElement * parent,
                  BaseSpace * space,
                  QRectF initRect = QRectF(0,0,100,100)
            );

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;


    void load();
    void loadPixmap();

    inline QString originalName() const {return m_originName;}
    inline void setOriginalName(const QString & value) {m_originName = value;}
    inline QPixmap pixmap() const { return m_pixmap;}


private slots:
    void startLoad();
    void finishLoad(bool);
    void updateProgress(int progress);


};


class PixmapOpenJob : public QRunnable
{
    PixmapElement * m_element;

public:
    PixmapOpenJob(PixmapElement * element);
    void run() override;

};
#endif // PIXMAPELEMENT_H
