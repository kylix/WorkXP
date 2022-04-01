#ifndef BASEELEMENT_H
#define BASEELEMENT_H

#include <QGraphicsObject>
#include "commondef.h"
#include "graphicsitemtype.h"
#include "basepicker.h"

#define BASEELEMENT_ADJUSTING_OPACITY 0.3
class BaseNode;
class BaseSpace;
class DrawTool;

class BaseElement : public QGraphicsObject
{
Q_OBJECT
    int m_flags;
    bool m_adjusting;
    bool m_lockScale;
    QRectF m_rect;
    qreal m_rotateAngle;

    Q_PROPERTY(qreal rotateAngle READ rotateAngle WRITE setRotateAngle NOTIFY rotateAngleChanged)
    Q_PROPERTY(bool lockScale READ lockScale WRITE setLockScale NOTIFY lockScaleChanged)
    Q_PROPERTY(qreal scaleRatio READ scaleRatio)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(QString elementName READ elementName WRITE setElementName NOTIFY elementNameChanged)
    Q_PROPERTY(BaseElement * parentElement READ parentElement WRITE setParentElement)

    qreal m_scaleRatio;

    QString m_elementName;

    BaseElement * m_parentElement;

public:
    enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_BASEELEMENT };
    virtual int type() const Q_DECL_OVERRIDE
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    enum {
        ELEMENT_NONE = 0,
        ELEMENT_MOVABLE = 0x1,
        ELEMENT_RESIZABLE = 0x2,
        ELEMENT_ROTABLE = 0x4,
        ELEMENT_PICKABLE = (ELEMENT_MOVABLE | ELEMENT_RESIZABLE | ELEMENT_ROTABLE)
    };

    inline BasePicker * picker() const {return m_picker;}

public:
    //Which node this element belongs to
    explicit BaseElement(
            BaseElement *parent,
            BaseSpace * space,
            QRectF initRect = QRectF(0,0,10,10)
            );

    ~BaseElement();


    inline bool pickable() const {return m_flags & ELEMENT_PICKABLE;}
    inline bool movable() const {return m_flags & ELEMENT_MOVABLE;}
    inline bool resizable() const {return m_flags & ELEMENT_RESIZABLE;}
    inline bool rotable() const { return m_flags & ELEMENT_ROTABLE;}


    inline void setFlags(int flag,bool enable) {if (enable) {m_flags |= flag;} else {m_flags=m_flags & (~flag);}}

    inline void setPickable(bool enable) { setFlags(ELEMENT_PICKABLE,enable);}
    inline void setMovable(bool enable) { setFlags(ELEMENT_MOVABLE,enable);}
    inline void setResizable(bool enable) { setFlags(ELEMENT_RESIZABLE,enable);}
    inline void setRotable(bool enable) { setFlags(ELEMENT_ROTABLE,enable);}

    void beginAdjust();
    void endAdjust();
    inline bool adjusting() const { return m_adjusting;}

    inline BaseSpace * space() const { return m_space;}

    /** Position and size */
    inline QRectF rect() const { return m_rect;}
    QRect defaultPhysicalRect() const;
    void setDefaultPhysicalRect(const QRect & rect);
    QRect physicalRect(int unit) const;
    void setPhysicalRect(const QRect & rect,int unit);
    QPoint defaultPhysicalPos() const;
    void setDefaultPhysicalPos(const QPoint & pos);
    QPoint physicalPos(int unit) const;
    void setPhysicalPos(const QPoint & pos,int unit);


    inline qreal rotateAngle() const { return m_rotateAngle;}


    /** drawing  section **/
    /** BaseSpace mouse/key event ====>> DrawTool =====>> Element **/
    virtual void doDrawing(const QPointF & p);
    virtual void startDraw(const QPointF & p);
    virtual void clickDraw(const QPointF & p);
    virtual void doubleClickDraw(const QPointF & p);
    virtual void abortDraw();

    void drawToolMouseMove(QGraphicsSceneMouseEvent * event);
    void drawToolMousePress(QGraphicsSceneMouseEvent *event);
    void drawToolMouseRelease(QGraphicsSceneMouseEvent *event);
    void drawToolKeyPress(QKeyEvent *event);
    void drawToolDoublcClick(QGraphicsSceneMouseEvent * event);


    inline bool lockScale() const
    {
        return m_lockScale;
    }

    inline qreal scaleRatio() const
    {
        return m_scaleRatio;
    }

    QRectF boundingRect() const override;



    inline QString elementName() const
    {
        return m_elementName;
    }


    BaseElement * parentElement() const
    {
        return m_parentElement;
    }

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    virtual BasePicker * createPicker() =0;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

    BasePicker * m_picker;
    DrawTool * m_drawTool;

protected:
    BaseSpace * m_space;

public slots:
   void setRect(const QRectF & r);

   void hidePicker();
   void showPicker();

   void setRotateAngle(qreal arg1,const QPointF & center=QPointF());

   void setLockScale(bool lockScale);


   void setElementName(QString elementName);

   void setParentElement(BaseElement * parentElement)
   {
       m_parentElement = parentElement;
   }

signals:
   void rectChanged(const QRectF & arg1);
   void rotateAngleChanged(qreal arg1);
   void drawFinished();
   void drawAborted();

   void lockScaleChanged(bool lockScale);
   void elementNameChanged(QString elementName);
};

#endif // BASEELEMENT_H
