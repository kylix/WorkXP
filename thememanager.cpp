#include "thememanager.h"
#include "QtAwesome/QtAwesome.h"
#include "fontfamilies.h"
#include "preferences.h"
#include "workxpproxystyle.h"
#include "commondef.h"
#include "utilities.h"

#include <QtAwesome/QtAwesome.h>

#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QColor>
#include <QCursor>
#include <QStyle>
#include <QPixmapCache>
#include <QStyleFactory>
#include <QStringList>
#include <QSettings>

#define COLOR_VALUE(x) Utilities::parseColor(settings.value(x).toString())


ThemeManager * ThemeManager::m_instance = 0;

static QPalette createPalette(const QColor &windowColor,const QColor &highlightColor)
{
    int hue, sat, windowValue;
    windowColor.getHsv(&hue, &sat, &windowValue);

    auto fromValue = [=](int value) {
        return QColor::fromHsv(hue, sat, qBound(0, value, 255));
    };

    const bool isLight = windowValue > 128;
    const int baseValue = isLight ? windowValue + 48 : windowValue - 24;

    const int lightTextValue = qMin(255, windowValue + 160);
    const int darkTextValue = qMax(0, windowValue - 160);

    const QColor lightText = QColor(lightTextValue, lightTextValue, lightTextValue);
    const QColor darkText = QColor(darkTextValue, darkTextValue, darkTextValue);
    const QColor lightDisabledText = QColor(lightTextValue, lightTextValue, lightTextValue, 128);
    const QColor darkDisabledText = QColor(darkTextValue, darkTextValue, darkTextValue, 128);

    QPalette palette(fromValue(windowValue));
    palette.setColor(QPalette::Base, fromValue(baseValue));
    palette.setColor(QPalette::AlternateBase, fromValue(baseValue - 10));
    palette.setColor(QPalette::WindowText, isLight ? darkText : lightText);
    palette.setColor(QPalette::ButtonText, isLight ? darkText : lightText);
    palette.setColor(QPalette::Text, isLight ? darkText : lightText);
    palette.setColor(QPalette::Light, fromValue(windowValue + 55));
    palette.setColor(QPalette::Dark, fromValue(windowValue - 55));
    palette.setColor(QPalette::Mid, fromValue(windowValue - 27));
    palette.setColor(QPalette::Midlight, fromValue(windowValue + 27));

    palette.setColor(QPalette::Disabled, QPalette::WindowText, isLight ? darkDisabledText : lightDisabledText);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, isLight ? darkDisabledText : lightDisabledText);
    palette.setColor(QPalette::Disabled, QPalette::Text, isLight ? darkDisabledText : lightDisabledText);

    bool highlightIsDark = qGray(highlightColor.rgb()) < 120;
    palette.setColor(QPalette::Highlight, highlightColor);
    palette.setColor(QPalette::HighlightedText, highlightIsDark ? Qt::white : Qt::black);

    return palette;

}

static QPalette createPalette2(int style)
{
    QSettings settings(":/styles/palette.ini",QSettings::IniFormat);
    if (style==THEME_LIGHTEN){
        settings.beginGroup("Lighten");
    }
    else if (style==THEME_DARKEN) {
        settings.beginGroup("Darken");
    }
    QColor windowColor = COLOR_VALUE("Window");
    QColor windowTextColor  = COLOR_VALUE("WindowText");
    QColor baseColor = COLOR_VALUE("Base");
    QColor alternateBaseColor = COLOR_VALUE("AlternateBase");
    //QColor toolTipBaseColor = COLOR_VALUE("ToolTipBase");
    //QColor toolTipTextColor = COLOR_VALUE("ToolTipText");
    QColor textColor = COLOR_VALUE("Text");
    QColor buttonColor = COLOR_VALUE("Button");
    QColor buttonTextColor = COLOR_VALUE("ButtonText");
    //QColor brightTextColor = COLOR_VALUE("BrightText");
    QColor highlightColor = COLOR_VALUE("Highlight");
    QColor highlightTextColor = COLOR_VALUE("HighlightedText");
    //QColor linkColor = COLOR_VALUE("Link");
    //QColor linkVisitedColor = COLOR_VALUE("LinkVisited");


    int hue, sat, windowValue;
    windowColor.getHsv(&hue, &sat, &windowValue);

    auto fromValue = [=](int value) {
        return QColor::fromHsv(hue, sat, qBound(0, value, 255));
    };

    const bool isLight = windowValue > 128;

    const int lightTextValue = qMin(255, windowValue + 160);
    const int darkTextValue = qMax(0, windowValue - 160);

    const QColor lightDisabledText = QColor(lightTextValue, lightTextValue, lightTextValue, 128);
    const QColor darkDisabledText = QColor(darkTextValue, darkTextValue, darkTextValue, 128);

    QPalette palette(buttonColor,windowColor);
    palette.setColor(QPalette::Base, baseColor);
    palette.setColor(QPalette::AlternateBase, alternateBaseColor);
    palette.setColor(QPalette::WindowText, windowTextColor);
    palette.setColor(QPalette::ButtonText, buttonTextColor);
    palette.setColor(QPalette::Text, textColor);
    palette.setColor(QPalette::Light, fromValue(windowValue + 55));
    palette.setColor(QPalette::Dark, fromValue(windowValue - 55));
    palette.setColor(QPalette::Mid, fromValue(windowValue - 27));
    palette.setColor(QPalette::Midlight, fromValue(windowValue + 27));

    palette.setColor(QPalette::Disabled, QPalette::WindowText, isLight ? darkDisabledText : lightDisabledText);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, isLight ? darkDisabledText : lightDisabledText);
    palette.setColor(QPalette::Disabled, QPalette::Text, isLight ? darkDisabledText : lightDisabledText);

    bool highlightIsDark = qGray(highlightColor.rgb()) < 120;
    palette.setColor(QPalette::Highlight, highlightColor);
    palette.setColor(QPalette::HighlightedText, highlightTextColor);


    QTAWESOME->setDefaultOption("color",QColor("#D36221"));
    QTAWESOME->setDefaultOption("color-off",buttonTextColor);


    return palette;



}


ThemeManager *ThemeManager::instance(QObject *parent)
{
    Q_UNUSED(parent)
    if (m_instance)
        return m_instance;
    m_instance = new ThemeManager();
    return m_instance;
}


ThemeManager::ThemeManager() : QObject(),
    m_defaultStyle(QApplication::style()->objectName())
  , m_defaultPalette(QApplication::palette())
{

    QApplication * app = qobject_cast<QApplication *>(qApp);

    QtAwesome::instance(app)->initFontAwesome();


    //Set font size to 8
    FontFamilies fontFamilies;
    QFont font =  fontFamilies.resort();
    font.setPointSize(12);
    app->setFont(font);

    //initialize resource from font awsome
    m_rotateCursorPm = QTAWESOME->icon(fa::refresh).pixmap(24,24);
    m_drawingCursorPm = QIcon(":/icons/drawing.svg").pixmap(24,24);


    Preferences *preferences = Preferences::instance();
    connect(preferences,&Preferences::styleChanged,this, &ThemeManager::themeChanged);

    themeChanged(preferences->style());

}

void ThemeManager::apply()
{
    Preferences *preferences = Preferences::instance();

    QPalette desiredPalette = createPalette(preferences->baseColor(),
                                   preferences->selectionColor());
    QStyle * style = QStyleFactory::create(QLatin1String("fusion"));
    style = new WorkxpProxyStyle(desiredPalette, style);
    QApplication::setStyle(style);

    if (QApplication::palette() != desiredPalette) {
        QPixmapCache::clear();
        QApplication::setPalette(desiredPalette);

        if (auto *style = qobject_cast<WorkxpProxyStyle*>(QApplication::style()))
            style->setPalette(desiredPalette);
    }

    emit styleApplied();

}

void ThemeManager::themeChanged(int arg1)
{
    QPalette desiredPalette = createPalette2(arg1);
    QStyle * style = QStyleFactory::create(QLatin1String("fusion"));
    style = new WorkxpProxyStyle(desiredPalette, style);
    QApplication::setStyle(style);

    if (QApplication::palette() != desiredPalette) {
        QPixmapCache::clear();
        QApplication::setPalette(desiredPalette);

        if (auto *style = qobject_cast<WorkxpProxyStyle*>(QApplication::style()))
            style->setPalette(desiredPalette);
    }

    emit styleApplied();

}




