#include "portselector.h"
#include "ui_portselector.h"
#include "QtAwesome/QtAwesome.h"
PortSelector::PortSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortSelector)
{
    ui->setupUi(this);
    ui->btnRefresh->setIcon(QTAWESOME->icon(fa::refresh));
    QPalette palette = ui->comboBaud->palette();
    palette.setColor(QPalette::Window,Qt::red);
    ui->comboBaud->setPalette(palette);
}

PortSelector::~PortSelector()
{
    delete ui;
}
