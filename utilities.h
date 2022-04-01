#ifndef UTILITIES_H
#define UTILITIES_H
#include <QString>
#include <QJsonObject>
#include <QJsonValue>
#include <QRectF>
#include <QPolygonF>
#include <QColor>

class Utilities
{
public:

    enum LengthType {
        LT_PERCENT,
        LT_PX,
        LT_PC,
        LT_PT,
        LT_MM,
        LT_CM,
        LT_IN,
        LT_OTHER
    };

    Utilities();

    static QString loadFile(const QString & filename);
    static QJsonObject rectToJson(const QRectF & value);
    static QRectF jsonToRect(const QJsonObject & json);
    static QJsonObject transformToJson(const QTransform &value);
    static QTransform jsonToTransform(const QJsonObject &json);

    static qreal twoPointDistance(const QPointF &pt1, const QPointF &pt2);
    static qreal square(const qreal num);
    static QPolygonF rectToPoly(QRectF rect);
    static QRectF polyToRect(QPolygonF rect);
    static qreal convertToPixels(qreal len, Utilities::LengthType type);
    static qreal parseToPixels(const QStringRef &str);


    static qreal defaultDpiScale();
    static qreal dpiScaled(qreal value);
    static QSize dpiScaled(QSize value);
    static QPoint dpiScaled(QPoint value);
    static QRectF dpiScaled(QRectF value);

    template<typename T>
    static bool compare(const T & l1,const T & l2 );


    static QColor parseColor(const QString &str);
};

template<typename T>
bool Utilities::compare(const T &l1, const T &l2)
{
    if (l1.count()==0)
        return false;
    if (l1.count()!=l2.count())
        return false;
    if (l1==l2)
        return true;
    typename T::const_iterator it = l1.begin();
    for(;it!=l1.end();++it){
        if (l2.indexOf(*it)<0)
            return false;
    }

    it = l2.begin();
    for(;it!=l2.end();++it){
        if (l1.indexOf(*it)<0)
            return false;
    }

    return true;

}
#endif // UTILITIES_H
