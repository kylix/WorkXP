#include "zoomadjust.h"
#include "ui_zoomadjust.h"
#include "QtAwesome.h"

ZoomAdjust::ZoomAdjust(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZoomAdjust),
    m_factor(100)
{
    ui->setupUi(this);
    ui->btnZoomIn->setIcon(QTAWESOME->icon(fa::minus));
    ui->btnZoomOut->setIcon(QTAWESOME->icon(fa::plus));
    ui->btnFit->setIcon(QTAWESOME->icon(fa::fittocanvas));

    connect(ui->slider,&QSlider::valueChanged,this,&ZoomAdjust::setFactor);

    connect(ui->btnZoomOut,&QPushButton::clicked,this,[this](bool){
       ui->slider->setValue(ui->slider->value()-1);
    });

    connect(ui->btnZoomIn,&QPushButton::clicked,this,[this](bool){
       ui->slider->setValue(ui->slider->value()+1);
    });

    connect(ui->btnFit,     &QPushButton::clicked,   this, &ZoomAdjust::zoomToFit);

    connect(ui->editFactor, QOverload<int>::of(&QSpinBox::valueChanged), this, &ZoomAdjust::setFactor);

}

ZoomAdjust::~ZoomAdjust()
{
    delete ui;
}

void ZoomAdjust::setFactor(int value)
{
    if (m_factor==value)
        return;
    ui->slider->blockSignals(true);
    ui->editFactor->blockSignals(true);
    ui->slider->setValue(value);
    ui->editFactor->setValue(value);
    ui->slider->blockSignals(false);
    ui->editFactor->blockSignals(false);
    emit factorChanged(value);
}
