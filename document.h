#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QSize>
#include <QAbstractItemModel>
#include <QList>
QT_BEGIN_NAMESPACE
class QGraphicsItem;
QT_END_NAMESPACE

class BaseSpace;
class BaseElement;

class Document : public QAbstractItemModel
{
Q_OBJECT
    Q_PROPERTY(QSize pageSize MEMBER m_pageSize READ pageSize WRITE setPageSize NOTIFY pageSizeChanged)
    Q_PROPERTY(QString fileName MEMBER m_fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)


public:

    Document(BaseSpace *space = nullptr);

    inline QSize pageSize() const { return m_pageSize;}
    inline QString fileName() const { return m_fileName;}

    void load(const QString & fileName);
    void save(const QString & fileName);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    //calculate the row number of element if parent exists or return 0;
    int childNumber(const BaseElement * element) const;
    //get the element
    BaseElement * getItem(const QModelIndex &index) const;
    QList<QGraphicsItem *> getItems(const QModelIndex &index) const;

    void reset();

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
public slots:
    void setPageSize(QSize value);

    void setFileName(QString fileName);


signals:
    void pageSizeChanged(QSize pageSize);
    void fileNameChanged(QString fileName);

private:
    QSize m_pageSize;
    QString m_fileName;
    BaseSpace * m_space;


};






#endif // DOCUMENT_H
