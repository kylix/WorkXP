#include "document.h"
#include "baseelement.h"
#include "basespace.h"
#include "QtAwesome/QtAwesome.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QCoreApplication>
#include <QUuid>
#include <QDateTime>



Document::Document(BaseSpace * space):
    QAbstractItemModel(space),
    m_space(space)
{
    connect(space,&BaseSpace::elementCreated,this,[this](BaseElement * element){
        this->beginResetModel();
        this->endResetModel();
        connect(element,&BaseElement::destroyed,this,[this](QObject *obj){

            this->beginResetModel();
            this->endResetModel();

        });
    });
}


int Document::columnCount(const QModelIndex &) const
{
    return 5;
}

QVariant Document::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    BaseElement * element = getItem(index);
    if (!element) return QVariant();
    switch (role) {
    case Qt::DisplayRole:
        if (index.column()==1){
            return element->elementName();
        }
        break;
    case Qt::DecorationRole:
        switch (index.column()) {
        case 0:
            switch(element->type()-QGraphicsItem::UserType){
            case GRAPHICSITEM_TYPE_RECT:
                return QTAWESOME->icon(fa::rectangle);
            case GRAPHICSITEM_TYPE_CIRCLE:
                return QTAWESOME->icon(fa::circle);
            case GRAPHICSITEM_TYPE_LINE:
                return QTAWESOME->icon(fa::line);
            case GRAPHICSITEM_TYPE_BEZIER:
                return QTAWESOME->icon(fa::curve);
            case GRAPHICSITEM_TYPE_TEXT:
                return QTAWESOME->icon(fa::text);
            case GRAPHICSITEM_TYPE_PIXMAP:
                return QTAWESOME->icon(fa::bitmap);
            }
        case 2:
            return (element->isVisible())?QTAWESOME->icon(fa::eye):QTAWESOME->icon(fa::eyeslash);
        case 3:
            return (element->pickable())?QTAWESOME->icon(fa::unlock):QTAWESOME->icon(fa::lock);
        case 4:
            return QTAWESOME->icon(fa::trash);
        }
        break;
    default:
        break;
    }
    return QVariant();
}

Qt::ItemFlags Document::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
    if (!index.isValid())
        return 0;
    //BaseElement * element = static_cast<BaseElement *>(index.internalPointer());

    Qt::ItemFlags flags;

    switch (index.column()){
    case 1:
        flags = Qt::ItemIsSelectable | Qt::ItemIsEditable;
        break;
    case 2:
    case 3:
    case 4:
        flags =  Qt::ItemIsUserCheckable;
        break;
    default:
        break;
    }
    //flags |=  QAbstractItemModel::flags(index);
    return flags;
}

bool Document::hasChildren(const QModelIndex &parent) const
{

    QList<QGraphicsItem *> items = getItems(parent);

    foreach(QGraphicsItem * item , items){
        if (item->type()>QGraphicsItem::UserType+GRAPHICSITEM_TYPE_BASEELEMENT){
            return true;
        }
    }
    return false;
}

QModelIndex Document::index(int row, int column, const QModelIndex &parent) const
{
    int i = -1;
    QList<QGraphicsItem *> items = getItems(parent);
    foreach(QGraphicsItem * item ,items){
        if (item->type()>QGraphicsItem::UserType+GRAPHICSITEM_TYPE_BASEELEMENT){
            i++;
            if (row == i){
                //beware here, you should cast it to baseelement otherwise static_cast can not work well
                return createIndex(row,column,static_cast<BaseElement *>(item));
            }
        }
    }
    return QModelIndex();
}

QModelIndex Document::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    BaseElement *childItem = getItem(index);
    if (!childItem) return QModelIndex();


    QGraphicsItem * parentItem = childItem->parentItem();
    if (parentItem){
        if (parentItem->type()>QGraphicsItem::UserType+GRAPHICSITEM_TYPE_BASEELEMENT){
            return createIndex(childNumber(static_cast<BaseElement *>(parentItem)), 0, parentItem);
        }
    }

    return QModelIndex();

}

int Document::rowCount(const QModelIndex &parent) const
{
    QList<QGraphicsItem *> items = getItems(parent);
    int i = 0;
    foreach(QGraphicsItem * item , items){
        if (item->type()>QGraphicsItem::UserType+GRAPHICSITEM_TYPE_BASEELEMENT){
            i++;
        }
    }
    return i;

}

BaseElement *Document::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        BaseElement *item = static_cast<BaseElement*>(index.internalPointer());
        if (item)
            return item;
    }
    return 0;
}

QList<QGraphicsItem *> Document::getItems(const QModelIndex &index) const
{
    if (index.isValid()) {
        BaseElement *item = static_cast<BaseElement *>(index.internalPointer());
        if (item){
            return item->childItems();
        }
    }
    else {
        return m_space->scene()->items();
    }
    return QList<QGraphicsItem *>();

}

void Document::reset()
{
    this->beginResetModel();
    this->endResetModel();

}

bool Document::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QAbstractItemModel::setData(index,value,role);
    emit dataChanged(index,index);
    return true;

}

int Document::childNumber(const BaseElement *element) const
{
    QList<QGraphicsItem *> items;
    QGraphicsItem * parent = element->parentItem();
    if (parent) {
        items = parent->childItems();
    }
    else {
        items = m_space->scene()->items();
    }
    int i = -1;
    foreach(QGraphicsItem * item,items){
        if (item->type()>QGraphicsItem::UserType+GRAPHICSITEM_TYPE_BASEELEMENT){
            i++;
            if (item==static_cast<const QGraphicsItem *>(element)){
                return i;
            }

        }
    }
    return 0;
}



void Document::setPageSize(QSize value)
{
    if (m_pageSize!=value){
        m_pageSize = value;
        emit pageSizeChanged(m_pageSize);
    }

}

void Document::setFileName(QString fileName)
{
    if (m_fileName == fileName)
        return;

    m_fileName = fileName;
    emit fileNameChanged(fileName);
}






void Document::load(const QString &fileName)
{
    Q_UNUSED(fileName)

}




void Document::save(const QString &fileName)
{
    Q_UNUSED(fileName)
}


