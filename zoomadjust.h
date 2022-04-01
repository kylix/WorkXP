#ifndef ZOOMADJUST_H
#define ZOOMADJUST_H

#include <QWidget>

namespace Ui {
class ZoomAdjust;
}

class ZoomAdjust : public QWidget
{
    Q_OBJECT

public:
    explicit ZoomAdjust(QWidget *parent = 0);
    ~ZoomAdjust();

private:
    Ui::ZoomAdjust *ui;
    int m_factor;

public slots:
    void setFactor(int value);

signals:
    void factorChanged(int value);
    void zoomToFit();


};

#endif // ZOOMADJUST_H
