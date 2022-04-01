#include "leftbar.h"
#include "ui_leftbar.h"

LeftBar::LeftBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeftBar)
{
    ui->setupUi(this);
}

LeftBar::~LeftBar()
{
    delete ui;
}

void LeftBar::setSelectAction(QAction *action)
{
    ui->btnSelect->setDefaultAction(action);
}

void LeftBar::setPictureAction(QAction *action)
{
    ui->btnPicture->setDefaultAction(action);
}

void LeftBar::setRectangleAction(QAction *action)
{
    ui->btnRectangle->setDefaultAction(action);
}

void LeftBar::setCircleAction(QAction *action)
{
    ui->btnCircle->setDefaultAction(action);
}

void LeftBar::setLineAction(QAction *action)
{
    ui->btnLine->setDefaultAction(action);
}

void LeftBar::setCurveAction(QAction *action)
{
    ui->btnCurve->setDefaultAction(action);
}

void LeftBar::setMachineAction(QAction *action)
{
    ui->btnMachine->setDefaultAction(action);
}

void LeftBar::setLayersAction(QAction *action)
{
    ui->btnLayers->setDefaultAction(action);
}

void LeftBar::setTextAction(QAction *action)
{
    ui->btnText->setDefaultAction(action);
}


