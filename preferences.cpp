#include "preferences.h"

#include <QSettings>
#include <QDebug>
Preferences *Preferences::m_instance;


Preferences *Preferences::instance()
{
    if (!m_instance)
        m_instance = new Preferences;
    return m_instance;
}


Preferences::Preferences():m_settings(new QSettings(this))
{

    m_settings->beginGroup(QLatin1String("General"));
    m_defaultUnit = intValue("DefaultUnit",0);
    m_autoSave = boolValue("AutoSave",true);
    m_hardwareAcceleration = boolValue("HardwareAcceleration",true);
    m_language = stringValue("Language","en");
    m_openLastFileOnStartup = boolValue("OpenLastFileOnStartup",true);
    m_settings->endGroup();

    m_settings->beginGroup(QLatin1String("Canvas"));
    m_rulerVisible = boolValue("RulerVisible",true);
    m_snapToGrid = boolValue("SnapToGrid",true);
    m_gridVisible = m_snapToGrid?boolValue("GridVisible",true):false;
    m_gridColor = colorValue("GridColor",Qt::darkGray);
    m_girdSize = intValue("GridSize",4);
    m_settings->endGroup();

    m_settings->beginGroup(QLatin1String("Theme"));
    m_style = intValue("style",THEME_DARKEN);
    m_baseColor = colorValue("BaseColor",QColor(242,242,242,100));
    m_selectionColor = colorValue("SelectionColor",QColor(48, 140, 198));
    m_settings->endGroup();

    m_settings->beginGroup(QLatin1String("System"));
    m_defaultPageSize = sizeValue("DefaultPageSize",QSize(297,210));
    m_settings->endGroup();

}


void Preferences::setGridVisible(bool gridVisible)
{
    if (m_gridVisible == gridVisible)
        return;

    m_gridVisible = gridVisible;
    m_settings->setValue(QLatin1String("Canvas/GridVisible"), gridVisible);
    emit gridVisibleChanged(m_gridVisible);

}

void Preferences::setGridColor(QColor gridColor)
{
    if (m_gridColor == gridColor)
        return;

    m_gridColor = gridColor;
    m_settings->setValue(QLatin1String("Canvas/GridColor"), gridColor);
    emit gridColorChanged(m_gridColor);

}

void Preferences::setGridFine(int gridFine)
{
    if (m_gridFine == gridFine)
        return;

    m_gridFine = gridFine;
    m_settings->setValue(QLatin1String("Canvas/GridFine"), gridFine);
    emit gridFineChanged(m_gridFine);
}

void Preferences::setLanguage(QString language)
{
    if (m_language == language)
        return;

    m_language = language;
    m_settings->setValue(QLatin1String("General/Language"), language);
    emit languageChanged(m_language);
}


void Preferences::setRulerVisible(bool rulerVisible)
{
    if (m_rulerVisible == rulerVisible)
        return;

    m_rulerVisible = rulerVisible;
    m_settings->setValue(QLatin1String("Canvas/RulerVisible"), rulerVisible);
    emit rulerVisibleChanged(m_rulerVisible);
}


void Preferences::setSnapToGrid(bool snapToGrid)
{
    {
        if (m_snapToGrid == snapToGrid)
            return;

        m_snapToGrid = snapToGrid;
        m_settings->setValue(QLatin1String("Canvas/SnapToGrid"), snapToGrid);
        emit snapToGridChanged(m_snapToGrid);
    }

}

void Preferences::setDefaultPageSize(QSize defaultPageSize)
{
    if (m_defaultPageSize==defaultPageSize)
        return;

    m_defaultPageSize = defaultPageSize;
}

void Preferences::setGridSize(int girdSize)
{
    if (m_girdSize == girdSize)
        return;

    m_girdSize = girdSize;
    m_settings->setValue(QLatin1String("Canvas/GridSize"), girdSize);
    emit gridSizeChanged(m_girdSize);
}


void Preferences::setBaseColor(QColor baseColor)
{
    if (m_baseColor == baseColor)
        return;

    m_baseColor = baseColor;
    m_settings->setValue(QLatin1String("Theme/BaseColor"), baseColor);
    emit baseColorChanged(m_baseColor);
}


void Preferences::setSelectionColor(QColor selectionColor)
{
    if (m_selectionColor == selectionColor)
        return;

    m_selectionColor = selectionColor;
    m_settings->setValue(QLatin1String("Theme/SelectionColor"), selectionColor);
    emit selectionColorChanged(m_selectionColor);
}

void Preferences::setStyle(int style)
{
    if (m_style == style)
        return;

    m_style = style;
    m_settings->setValue(QLatin1String("Theme/Style"), style);
    emit styleChanged(m_style);
}

void Preferences::setDefaultUnit(int defaultUnit)
{
    if (m_defaultUnit == defaultUnit)
        return;

    m_defaultUnit = defaultUnit;
    m_settings->setValue(QLatin1String("General/DefaultUnit"), defaultUnit);
    emit defaultUnitChanged(m_defaultUnit);
}

void Preferences::setAutoSave(bool autoSave)
{
    if (m_autoSave == autoSave)
        return;

    m_autoSave = autoSave;
    m_settings->setValue(QLatin1String("General/AutoSave"), autoSave);
    emit autoSaveChanged(m_autoSave);
}

void Preferences::setHardwareAcceleration(bool hardwareAcceleration)
{
    if (m_hardwareAcceleration == hardwareAcceleration)
        return;

    m_hardwareAcceleration = hardwareAcceleration;
    m_settings->setValue(QLatin1String("General/HardwareAcceleration"), hardwareAcceleration);
    emit hardwareAccelerationChanged(m_hardwareAcceleration);
}


void Preferences::setOpenLastFileOnStartup(bool openLastFileOnStartup)
{
    if (m_openLastFileOnStartup == openLastFileOnStartup)
        return;

    m_openLastFileOnStartup = openLastFileOnStartup;
    m_settings->setValue(QLatin1String("General/OpenLastFileOnStartup"), openLastFileOnStartup);
    emit openLastFileOnStartupChanged(m_openLastFileOnStartup);
}



bool Preferences::boolValue(const char *key, bool defaultValue) const
{
    return m_settings->value(QLatin1String(key), defaultValue).toBool();
}

QColor Preferences::colorValue(const char *key, const QColor &def) const
{
    const QString name = m_settings->value(QLatin1String(key),
                                          def.name()).toString();
    if (!QColor::isValidColor(name))
        return QColor();

    return QColor(name);
}

QString Preferences::stringValue(const char *key, const QString &def) const
{
    return m_settings->value(QLatin1String(key), def).toString();
}

int Preferences::intValue(const char *key, int defaultValue) const
{
    return m_settings->value(QLatin1String(key), defaultValue).toInt();
}

qreal Preferences::realValue(const char *key, qreal defaultValue) const
{
    return m_settings->value(QLatin1String(key), defaultValue).toReal();
}

QSize Preferences::sizeValue(const char *key, QSize defaultValue) const
{
    return m_settings->value(QLatin1String(key), defaultValue).toSize();
}
