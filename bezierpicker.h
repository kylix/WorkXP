#ifndef BEZIERPICKER_H
#define BEZIERPICKER_H
#include "linepicker.h"

class BezierPicker : public LinePicker
{
public:
    explicit BezierPicker(LineElement * element);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) Q_DECL_OVERRIDE;

};

#endif // BEZIERPICKER_H
