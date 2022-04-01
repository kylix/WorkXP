#ifndef BASEGRAPHICSVIEW_H
#define BASEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "elementfactory.h"
#include "preferences.h"
#include "utilities.h"
#include "commondef.h"
class BaseSpace;
class BaseElement;
class GroupPickerElement;


class BaseSpaceScene : public QGraphicsScene
{
Q_OBJECT
    bool m_drawing;
    bool m_picking;
    ElementFactoryMethod m_elementFactoryMethod;
    BaseElement * m_drawingElement;
    BaseElement * m_activeElement;
    BaseSpace * m_space;

    Q_PROPERTY(PhysicalSize physicalSize READ physicalSize WRITE setPhysicalSize NOTIFY physicalSizeChanged)

    PhysicalSize m_physicalSize;
    QSize m_actualPxSize;

    GroupPickerElement * m_groupPicker;


public:

    explicit BaseSpaceScene(BaseSpace *parent);
    void setDrawing(bool arg1,ElementFactoryMethod method = 0);
    QSize actualPxSize();

    inline PhysicalSize physicalSize() const
    {
        return m_physicalSize;
    }

    qreal scaleFactor();

    void setPicking(bool);
    bool picking() const { return m_picking ;}

    inline BaseElement * activeElement() const {return m_activeElement;}
    void pickElements(QList<BaseElement *>  & elements);

public slots:
    void setPhysicalSize(PhysicalSize physicalSize);

signals:
    void physicalSizeChanged(PhysicalSize physicalSize);
    void elementUnpicked();
    void elementPicked(BaseElement * element);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:
     friend class BaseSpace;
};


class BaseSpace : public QGraphicsView
{
Q_OBJECT
    BaseSpaceScene * m_scene;
    bool m_shiftLock;//shift key is pressed during resize or move


public:
    explicit BaseSpace(QWidget *parent = Q_NULLPTR);
    ~BaseSpace();

    //BaseElement * elementSelectd() const;

    BaseElement * elementAt(const QPointF & scenePos) const;

    void setDrawing(bool arg1,ElementFactoryMethod method = 0);

    virtual void init();

    void setPicking(bool);
    bool picking() const;

    inline BaseSpaceScene * baseScene() {return m_scene;}

    BaseElement * activeElement() const { return m_scene->activeElement();}

    void emitElementCreated(BaseElement * element);

    //automatically set a element name
    QString nameElementAuto(int graphicsType) const;



protected:
    //Picker * m_picker;
    void resizeEvent(QResizeEvent *event) override;

    void scrollContentsBy(int dx, int dy) override;

public slots:
    void zoom(qreal factor);
    void zoomToFit();
    void pickElements(QList<BaseElement *>  & elements);


signals:
    void scaled();
    void elementCreated(BaseElement * element);
    //void picked(BaseElement * value);
    //void unpicked(BaseElement * value);

    void originalSizeChanged(QSize originalSize);
};



#endif // BASEGRAPHICSVIEW_H
