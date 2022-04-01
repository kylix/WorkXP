#ifndef DOMTREEVIEW_H
#define DOMTREEVIEW_H

#include "advtreeview.h"

class BaseElement;

class DomTreeView : public AdvTreeView
{
public:
    DomTreeView(QWidget *parent = nullptr);
    ~DomTreeView();

protected:
    virtual bool isAllowChildren(const QModelIndex & index) override;
    virtual void startMoveUnder(void * dragItem,void * dropItem) override;
    virtual void startMoveAfter(void * dragItem,void * dropItem) override;

public slots:
    void selectElements(QList<BaseElement *> elements);
};

#endif // DOMTREEVIEW_H
