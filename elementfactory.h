#ifndef ELEMENTFACTORY_H
#define ELEMENTFACTORY_H

class BaseElement;
class BaseSpace;

#include <QRectF>


typedef BaseElement * (*ElementFactoryMethod) (BaseElement * parent, BaseSpace * space, QRectF initRect);



#endif // ELEMENTFACTORY_H
