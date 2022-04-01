#include "connectionform.h"
#include "ui_connectionform.h"

ConnectionForm::ConnectionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionForm)
{
    ui->setupUi(this);

    ui->comboBaud->addItem("hi");
}

ConnectionForm::~ConnectionForm()
{
    delete ui;
}
