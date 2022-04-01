#ifndef PORTSELECTOR_H
#define PORTSELECTOR_H

#include <QWidget>

namespace Ui {
class PortSelector;
}

class PortSelector : public QWidget
{
    Q_OBJECT

public:
    explicit PortSelector(QWidget *parent = 0);
    ~PortSelector();

private:
    Ui::PortSelector *ui;
};

#endif // PORTSELECTOR_H
