#ifndef GROUPELEMENT_H
#define GROUPELEMENT_H

#include "baseelement.h"

class BaseGroupElement : public BaseElement
{

Q_OBJECT
public:
    BaseGroupElement(BaseElement *parent, BaseSpace *space, QRectF initRect);
    void addToGroup(BaseElement *element);
    void removeFromGroup(BaseElement *element);
    bool isObscuredBy(const QGraphicsItem *item) const override;
    QPainterPath opaqueArea() const override;


    inline QList<BaseElement *> childElements() const { return m_childElements;}

    //remove all elements from list and reset their parent to scene
    void clear();

protected:
    BasePicker *createPicker() override;

private:
    QList<BaseElement *> m_childElements;

};


class GroupPickerElement : public BaseGroupElement
{
    Q_OBJECT
    enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_GROUP_PICKER};

public:
    int type() const override
    {
         // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }
    GroupPickerElement(
            BaseSpace * space
            );


};

class GroupElement : public BaseGroupElement
{
    Q_OBJECT
    enum { Type = QGraphicsItem::UserType + GRAPHICSITEM_TYPE_GROUP};

public:
    int type() const override
    {
         // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }
    GroupElement(
            BaseElement *parent,
            BaseSpace * space);
};


#endif // GROUPELEMENT_H
