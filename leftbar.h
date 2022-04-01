#ifndef LEFTBAR_H
#define LEFTBAR_H

#include <QWidget>


class QAction;

namespace Ui {
class LeftBar;
}

class LeftBar : public QWidget
{
    Q_OBJECT

public:
    explicit LeftBar(QWidget *parent = 0);
    ~LeftBar();

    void setSelectAction(QAction * action);
    void setPictureAction(QAction * action);
    void setRectangleAction(QAction * action);
    void setCircleAction(QAction * action);
    void setLineAction(QAction * action);
    void setCurveAction(QAction * action);
    void setTextAction(QAction * action);
    void setMachineAction(QAction * action);
    void setLayersAction(QAction * action);



private:
    Ui::LeftBar *ui;



signals:


};

#endif // LEFTBAR_H
