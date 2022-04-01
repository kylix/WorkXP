#include "textelement.h"
#include "elementpicker.h"
#include "basespace.h"
#include <QGraphicsTextItem>
#include <QDebug>
#include <QTextDocument>
#include <QPainter>
#include <QTextCursor>
#include <QKeyEvent>

class TextItem_private : public QGraphicsTextItem
{
    TextElement * m_parent;
    bool m_selecting;
public:
    TextItem_private(TextElement * parent)
        :QGraphicsTextItem(parent),m_parent(parent),m_selecting(false){

        QTextDocument * doc = new QTextDocument(this);
        this->setDocument(doc);
        //this->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
        //this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);
        connect(this->document(),&QTextDocument::contentsChanged,this,[this](){
            //m_parent->setRect(this->boundingRect());
            setTextWidth(-1);
            setTextWidth(boundingRect().width());
            QRectF r = this->mapRectToParent(this->boundingRect());
            m_parent->setRect(r);

        });
    }

protected:

    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


};


void TextItem_private::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyReleaseEvent(event);
    m_parent->emitCursorChanged();
    event->accept();;


}

void TextItem_private::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseMoveEvent(event);
    if (m_selecting){
        m_parent->emitCursorChanged();
    }
}

void TextItem_private::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mousePressEvent(event);
    m_selecting = true;
    m_parent->emitCursorChanged();
}

void TextItem_private::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);
    m_selecting = false;
    m_parent->emitCursorChanged();

}


TextElement::TextElement(BaseElement *parent, BaseSpace *space, QRectF initRect):
    BaseElement(parent,space,initRect)
{
    setElementName(m_space->nameElementAuto(this->type()));
    setResizable(false);
    m_picker = createPicker();
    m_textItem = new TextItem_private(this);
    m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    connect(this,&TextElement::rectChanged,this,[this](const QRectF & arg1){
        m_textItem->setPos(arg1.topLeft());
        //this->m_picker->show();
    });
}

QGraphicsTextItem *TextElement::textItem() const
{
    return qobject_cast<QGraphicsTextItem *>(m_textItem);
}

void TextElement::emitCursorChanged()
{
    emit cursorPositionChanged(m_textItem->textCursor());
}

void TextElement::setAlignment(Qt::Alignment alignment)
{
    QTextBlockFormat format;
    format.setAlignment(alignment);
    QTextCursor cursor = m_textItem->textCursor();
    cursor.mergeBlockFormat(format);
    m_textItem->setTextCursor(cursor);
}

void TextElement::startDraw(const QPointF &p)
{
    QPointF p1 = this->mapToScene(p);
    this->setPos(p1);
    this->m_textItem->setPos(this->boundingRect().topLeft());
    //this->m_picker->show();

}

void TextElement::doDrawing(const QPointF &p)
{
    Q_UNUSED(p)
}

void TextElement::clickDraw(const QPointF &)
{
    if (this->m_textItem->toPlainText().size()==0){
        this->abortDraw();
    }
}

void TextElement::doubleClickDraw(const QPointF &)
{
}




BasePicker *TextElement::createPicker()
{
    return new ElementPicker(this);
}

void TextElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    Q_UNUSED(painter)
    BaseElement::paint(painter,option,widget);
}

void TextElement::keyPressEvent(QKeyEvent *event)
{
    event->accept();

}

void TextElement::keyReleaseEvent(QKeyEvent *event)
{
    event->accept();

}






