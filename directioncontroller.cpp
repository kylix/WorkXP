#include "directioncontroller.h"
#include <QPainter>
#include <QSvgRenderer>

DirectionController::DirectionController(QWidget *parent) : QWidget(parent)
{

    m_svgArrowDown =  new QSvgRenderer(this);
    m_svgArrowLeft = new QSvgRenderer(this);
    m_svgArrowUp =  new QSvgRenderer(this);
    m_svgArrowRight = new QSvgRenderer(this);
    m_svgHome =  new QSvgRenderer(this);


    m_svgArrowDown->load(QString(":/icons/arrowdown.svg"));
    m_svgArrowLeft->load(QString(":/icons/arrowleft.svg"));
    m_svgArrowUp->load(QString(":/icons/arrowup.svg"));
    m_svgArrowRight->load(QString(":/icons/arrowright.svg"));
    m_svgHome->load(QString(":/icons/home.svg"));


}

void DirectionController::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setColor(QColor("white"));
    pen.setWidthF(1);


    //painter.setPen(pen);
    QRect rect = this->rect();
    QPoint center = rect.center();
    center.setX(MARGIN_LEFT+CIRCLE_DIAMETER/2);

    int nCircle = CIRCLE_DIAMETER - CIRCLE_THICK;

    QRect rect1(center.x()-nCircle/2,center.y()-nCircle/2,nCircle,nCircle);

   pen.setWidthF(CIRCLE_THICK);
    painter.setPen(pen);
    int nStartAngle = 45;
    while(nStartAngle<=360){
        painter.drawArc(rect1,(nStartAngle+CIRCLE_OFFSET)*16,(90-CIRCLE_OFFSET)*16);
        nStartAngle+=90;
    }

    pen.setWidthF(1);
    painter.setPen(pen);

    QRect rectBounding = QRect(center.x()-CIRCLE_DIAMETER/2,
                               center.y()-CIRCLE_DIAMETER/2,
                               CIRCLE_DIAMETER,
                               CIRCLE_DIAMETER);

    //painter.drawRect(rectBounding);


    m_svgHome->render(&painter,QRect(center.x()-12,center.y()-12,24,24));

    m_svgArrowUp->render(&painter,
      QRect(rectBounding.topLeft().x()+rectBounding.width()/2-12,
            rectBounding.topLeft().y()+rectBounding.height()/10-12,
            24,24));

    m_svgArrowDown->render(&painter,
      QRect(rectBounding.topLeft().x()+rectBounding.width()/2-12,
            rectBounding.topLeft().y()+rectBounding.height()*9/10-12,
            24,24));

    m_svgArrowLeft->render(&painter,
      QRect(rectBounding.topLeft().x()+rectBounding.width()/10-12,
            rectBounding.topLeft().y()+rectBounding.height()/2-12,
            24,24));

    m_svgArrowRight->render(&painter,
      QRect(rectBounding.topLeft().x()+rectBounding.width()*9/10-12,
            rectBounding.topLeft().y()+rectBounding.height()/2-12,
            24,24));


}
