#include "utilities.h"
#include "graphicsitemtype.h"

#include <QFile>
#include <QGraphicsItem>
#include <QtMath>
#include "qmath.h"
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
Utilities::Utilities()
{

}



QString Utilities::loadFile(const QString & filename){
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly);
    QString text = file.readAll();
    file.close();
    return text;
}


QJsonObject Utilities::rectToJson(const QRectF & value){
    QJsonObject json;
    json["x"] = value.x();
    json["y"] = value.y();
    json["width"] = value.width();
    json["height"] = value.height();
    return json;
}

QRectF Utilities::jsonToRect(const QJsonObject &json)
{
    QRectF rect;
    rect.setX(json["x"].toDouble());
    rect.setY(json["y"].toDouble());
    rect.setWidth(json["width"].toDouble());
    rect.setHeight(json["height"].toDouble());
    return rect;

}

QJsonObject Utilities::transformToJson(const QTransform & value){
    QJsonObject json;
    json["m11"] = value.m11();
    json["m12"] = value.m12();
    json["m13"] = value.m13();
    json["m21"] = value.m21();
    json["m22"] = value.m22();
    json["m23"] = value.m23();
    json["m31"] = value.m31();
    json["m32"] = value.m32();
    json["m33"] = value.m33();
    return json;
}

QTransform Utilities::jsonToTransform(const QJsonObject &json)
{
    return QTransform(
                json["m11"].toDouble(),json["m12"].toDouble(),json["m13"].toDouble(),
                json["m21"].toDouble(),json["m22"].toDouble(),json["m23"].toDouble(),
                json["m31"].toDouble(),json["m32"].toDouble(),json["m33"].toDouble()
            );
}



qreal Utilities::square(const qreal num){return num * num;}


qreal Utilities::twoPointDistance(const QPointF& pt1, const QPointF& pt2)
{
    return qSqrt(double(square(pt2.x() - pt1.x()) + square(pt2.y() - pt1.y())));
}

QPolygonF Utilities::rectToPoly(QRectF rect)
{
    QPolygonF poly;
    poly  << rect.topLeft() << rect.bottomRight();
    return poly;
}

QRectF Utilities::polyToRect(QPolygonF poly)
{
    return QRectF(poly.at(0),poly.at(1));
}



static inline bool isDigit(ushort ch)
{
    static quint16 magic = 0x3ff;
    return ((ch >> 4) == 3) && (magic >> (ch & 15));
}

static qreal toDouble(const QChar *&str)
{
    const int maxLen = 255;//technically doubles can go til 308+ but whatever
    char temp[maxLen+1];
    int pos = 0;

    if (*str == QLatin1Char('-')) {
        temp[pos++] = '-';
        ++str;
    } else if (*str == QLatin1Char('+')) {
        ++str;
    }
    while (isDigit(str->unicode()) && pos < maxLen) {
        temp[pos++] = str->toLatin1();
        ++str;
    }
    if (*str == QLatin1Char('.') && pos < maxLen) {
        temp[pos++] = '.';
        ++str;
    }
    while (isDigit(str->unicode()) && pos < maxLen) {
        temp[pos++] = str->toLatin1();
        ++str;
    }
    bool exponent = false;
    if ((*str == QLatin1Char('e') || *str == QLatin1Char('E')) && pos < maxLen) {
        exponent = true;
        temp[pos++] = 'e';
        ++str;
        if ((*str == QLatin1Char('-') || *str == QLatin1Char('+')) && pos < maxLen) {
            temp[pos++] = str->toLatin1();
            ++str;
        }
        while (isDigit(str->unicode()) && pos < maxLen) {
            temp[pos++] = str->toLatin1();
            ++str;
        }
    }

    temp[pos] = '\0';

    qreal val;
    if (!exponent && pos < 10) {
        int ival = 0;
        const char *t = temp;
        bool neg = false;
        if(*t == '-') {
            neg = true;
            ++t;
        }
        while(*t && *t != '.') {
            ival *= 10;
            ival += (*t) - '0';
            ++t;
        }
        if(*t == '.') {
            ++t;
            int div = 1;
            while(*t) {
                ival *= 10;
                ival += (*t) - '0';
                div *= 10;
                ++t;
            }
            val = ((qreal)ival)/((qreal)div);
        } else {
            val = ival;
        }
        if (neg)
            val = -val;
    } else {
        val = QByteArray::fromRawData(temp, pos).toDouble();
    }
    return val;

}
static qreal toDouble(const QString &str, bool *ok = NULL)
{
    const QChar *c = str.constData();
    qreal res = toDouble(c);
    if (ok) {
        *ok = ((*c) == QLatin1Char('\0'));
    }
    return res;
}

static qreal toDouble(const QStringRef &str, bool *ok = NULL)
{
    const QChar *c = str.constData();
    qreal res = toDouble(c);
    if (ok) {
        *ok = (c == (str.constData() + str.length()));
    }
    return res;
}


qreal Utilities::convertToPixels(qreal len, Utilities::LengthType type)
{
    switch (type) {
    case Utilities::LT_PERCENT:
        break;
    case Utilities::LT_PX:
        break;
    case Utilities::LT_PC:
        break;
    case Utilities::LT_PT:
        return len * 1.25;
        break;
    case Utilities::LT_MM:
        return len * 3.543307;
        break;
    case Utilities::LT_CM:
        return len * 35.43307;
        break;
    case Utilities::LT_IN:
        return len * 90;
        break;
    case Utilities::LT_OTHER:
        break;
    default:
        break;
    }
    return len;
}

qreal Utilities::parseToPixels(const QStringRef &str)
{
    Utilities::LengthType lengthType;
    QStringRef numStr = str.trimmed();
    if (numStr.endsWith(QLatin1Char('%'))) {
        numStr.chop(1);
        lengthType = Utilities::LT_PERCENT;
    } else if (numStr.endsWith(QLatin1String("px"))){
        numStr.chop(2);
        lengthType = Utilities::LT_PX;
    } else if (numStr.endsWith(QLatin1String("pc"))) {
        numStr.chop(2);
        lengthType = Utilities::LT_PC;
    } else if (numStr.endsWith(QLatin1String("pt"))) {
        numStr.chop(2);
        lengthType = Utilities::LT_PT;
    } else if (numStr.endsWith(QLatin1String("mm"))) {
        numStr.chop(2);
        lengthType = Utilities::LT_MM;
    } else if (numStr.endsWith(QLatin1String("cm"))) {
        numStr.chop(2);
        lengthType = Utilities::LT_CM;
    } else if (numStr.endsWith(QLatin1String("in"))) {
        numStr.chop(2);
        lengthType = Utilities::LT_IN;
    } else {
        lengthType = LT_PX;
    }
    bool ok = false;
    qreal len = toDouble(numStr, &ok);
    if (!ok) {
        len = 0;
        qWarning()<<"Error when convert string to qreal: "<<str;
    }
    //qDebug()<<"len is "<<len<<", from '"<<numStr << "'";
    return convertToPixels(len,lengthType);

}

qreal Utilities::defaultDpiScale()
{
    static qreal scale = []{
        if (const QScreen *screen = QGuiApplication::primaryScreen())
            return screen->logicalDotsPerInchX() / 96.0;
        return 1.0;
    }();
    return scale;
}

qreal Utilities::dpiScaled(qreal value)
{
#ifdef Q_OS_MAC
    // On mac the DPI is always 72 so we should not scale it
    return value;
#else
    static const qreal scale = defaultDpiScale();
    return value * scale;
#endif
}

QSize Utilities::dpiScaled(QSize value)
{
    return QSize(qRound(dpiScaled(value.width())),
                 qRound(dpiScaled(value.height())));
}

QPoint Utilities::dpiScaled(QPoint value)
{
    return QPoint(qRound(dpiScaled(value.x())),
                  qRound(dpiScaled(value.y())));
}

QRectF Utilities::dpiScaled(QRectF value)
{
    return QRectF(dpiScaled(value.x()),
                  dpiScaled(value.y()),
                  dpiScaled(value.width()),
                  dpiScaled(value.height()));
}

QColor Utilities::parseColor(const QString &str)
{
    if (QColor::isValidColor(str))
        return QColor(str);

    QStringList sList = str.split(' ');
    if (sList.count()<3){
        qDebug()<<"invalid color settings";
    }
    int r = 0, g = 0 , b = 0;
    qreal alpha = 1;
    r = sList[0].toInt();
    g = sList[1].toInt();
    b = sList[2].toInt();
    if (sList.count()>3){
        alpha = sList[3].toFloat();
    }
    return QColor(r,g,b,alpha);

}


