#ifndef WSPLITTER_H
#define WSPLITTER_H

#include <QSplitter>

class WSplitter : public QSplitter
{
    Q_OBJECT
public:
    explicit WSplitter(QWidget *parent = nullptr);

    void moveChild(int pos, int index);

signals:

public slots:

};

#endif // WSPLITTER_H
