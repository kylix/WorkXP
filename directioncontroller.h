#ifndef DIRECTIONCONTROLLER_H
#define DIRECTIONCONTROLLER_H

#include <QWidget>

class QSvgRenderer;

class DirectionController : public QWidget
{
    Q_OBJECT
public:
    explicit DirectionController(QWidget *parent = nullptr);
    const int CIRCLE_DIAMETER = 160;
    const int CIRCLE_THICK = 40;
    const int CIRCLE_OFFSET = 2;
    const int MARGIN_LEFT = 40;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QSvgRenderer * m_svgArrowUp;
    QSvgRenderer * m_svgArrowDown;
    QSvgRenderer * m_svgArrowLeft;
    QSvgRenderer * m_svgArrowRight;
    QSvgRenderer * m_svgHome;
};

#endif // DIRECTIONCONTROLLER_H
