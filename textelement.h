#ifndef TEXTELEMENT_H
#define TEXTELEMENT_H

#include "baseelement.h"


class QGraphicsTextItem;
class TextItem_private;

class TextElement : public BaseElement
{
Q_OBJECT
    TextItem_private * m_textItem;

public:
    enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_TEXT };
    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    TextElement(
            BaseElement *parent,
            BaseSpace * space,
            QRectF initRect = QRectF(0,0,100,100)
            );

    QGraphicsTextItem * textItem() const;

    void emitCursorChanged();

    void setAlignment(Qt::Alignment alignment);

protected:
    virtual void startDraw(const QPointF & p) override;
    virtual void doDrawing(const QPointF & p) override;
    virtual void clickDraw(const QPointF & p) override;
    virtual void doubleClickDraw(const QPointF & p) override;

    virtual BasePicker * createPicker() override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget=nullptr) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void cursorPositionChanged(const QTextCursor  & cursor);
};

#endif // TEXTELEMENT_H
