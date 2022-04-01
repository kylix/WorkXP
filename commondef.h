#ifndef WORKXP_COMMONDEF_H
#define WORKXP_COMMONDEF_H

#include <QSize>

#define UNIT_TYPE_MM 0
#define UNIT_TYPE_INCH 1
#define UNIT_TYPE_PIXCEL 2

#define THEME_LIGHTEN 0
#define THEME_DARKEN 1


#define PX_PER_MM 3.543307
#define PX_PER_INCH 90

typedef struct _PhysicalSize
{
    QSize size;
    int   unit;
} PhysicalSize;

//#define DRAW_PICKER_BOUNDING
//#define DRAW_PICKER_WRAPER_BOUNDING
//#define DRAW_PICKER_OUTLINE_BOUNDING
//#define DRAW_PICKER_ROTATE_BOUNDING
//#define DRAW_PICKER_ARCHOR_BOUNDING

#define DRAW_ELEMENT_BOUNDING

#endif // COMMONDEF_H
