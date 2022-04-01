#include "wsplitter.h"

WSplitter::WSplitter(QWidget *parent) : QSplitter(parent)
{

}

void WSplitter::moveChild(int pos, int index)
{
    this->moveSplitter(pos,index);

}
