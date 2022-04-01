#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "commondef.h"

#include <QObject>
#include <QColor>
#include <QSize>



class QSettings;

class Preferences : public QObject
{


Q_OBJECT
    /* General */
    Q_PROPERTY(int defaultUnit    READ defaultUnit  WRITE setDefaultUnit      NOTIFY  defaultUnitChanged)
    Q_PROPERTY(bool autoSave READ autoSave WRITE setAutoSave NOTIFY autoSaveChanged)
    Q_PROPERTY(bool hardwareAcceleration READ hardwareAcceleration WRITE setHardwareAcceleration NOTIFY hardwareAccelerationChanged)
    Q_PROPERTY(QString    language       READ language       WRITE setLanguage       NOTIFY languageChanged)
    Q_PROPERTY(bool openLastFileOnStartup READ openLastFileOnStartup WRITE setOpenLastFileOnStartup NOTIFY openLastFileOnStartupChanged)

    /* Canvas */

    Q_PROPERTY(bool snapToGrid READ snapToGrid WRITE setSnapToGrid NOTIFY snapToGridChanged)

    Q_PROPERTY(bool       rulerVisible   READ rulerVisible WRITE setRulerVisible     NOTIFY rulerVisibleChanged)

    Q_PROPERTY(bool       gridVisible    READ gridVisible    WRITE setGridVisible    NOTIFY gridVisibleChanged)
    Q_PROPERTY(QColor     gridColor      READ gridColor      WRITE setGridColor      NOTIFY gridColorChanged)
    Q_PROPERTY(int        gridFine       READ gridFine       WRITE setGridFine       NOTIFY gridFineChanged)
    Q_PROPERTY(int        girdSize       READ gridSize       WRITE setGridSize       NOTIFY gridSizeChanged)


    /* Theme */
    Q_PROPERTY(int      style          READ style          WRITE setStyle          NOTIFY styleChanged)
    Q_PROPERTY(QColor     baseColor      READ baseColor      WRITE setBaseColor      NOTIFY baseColorChanged)
    Q_PROPERTY(QColor     selectionColor READ selectionColor WRITE setSelectionColor NOTIFY selectionColorChanged)

    /* System */

    //Default Page Size in MM
    Q_PROPERTY(QSize defaultPageSize READ defaultPageSize WRITE setDefaultPageSize)

    static Preferences * m_instance;


public:
    static Preferences *instance();




    bool gridVisible() const
    {
        return m_gridVisible;
    }

    QColor gridColor() const
    {
        return m_gridColor;
    }

    int gridFine() const
    {
        return m_gridFine;
    }

    QString language() const
    {
        return m_language;
    }

    QColor baseColor() const
    {
        return m_baseColor;
    }

    bool rulerVisible() const
    {
        return m_rulerVisible;
    }

    int gridSize() const
    {
        return m_girdSize;
    }


    int defaultUnit() const
    {
        return m_defaultUnit;
    }

    QColor selectionColor() const
    {
        return m_selectionColor;
    }

    int style() const
    {
        return m_style;
    }

    bool autoSave() const
    {
        return m_autoSave;
    }

    bool hardwareAcceleration() const
    {
        return m_hardwareAcceleration;
    }

    bool openLastFileOnStartup() const
    {
        return m_openLastFileOnStartup;
    }

    bool snapToGrid() const
    {
        return m_snapToGrid;
    }

    QSize defaultPageSize() const
    {
        return m_defaultPageSize;
    }

public slots:
    void setGridVisible(bool gridVisible);

    void setGridColor(QColor gridColor);

    void setGridFine(int gridFine);

    void setLanguage(QString language);

    void setBaseColor(QColor baseColor);

    void setRulerVisible(bool rulerVisible);

    void setGridSize(int girdSize);

    void setDefaultUnit(int defaultUnit);

    void setSelectionColor(QColor selectionColor);

    void setStyle(int style);

    void setAutoSave(bool autoSave);

    void setHardwareAcceleration(bool hardwareAcceleration);

    void setOpenLastFileOnStartup(bool openLastFileOnStartup);

    void setSnapToGrid(bool snapToGrid);

    void setDefaultPageSize(QSize defaultPageSize);

signals:
    void gridVisibleChanged(bool gridVisible);

    void gridColorChanged(QColor gridColor);

    void gridFineChanged(int gridFine);

    void languageChanged(QString language);

    void baseColorChanged(QColor baseColor);

    void rulerVisibleChanged(bool rulerVisible);


    void gridSizeChanged(int girdSize);


    void defaultUnitChanged(int defaultUnit);

    void selectionColorChanged(QColor selectionColor);

    void styleChanged(int style);

    void autoSaveChanged(bool autoSave);

    void hardwareAccelerationChanged(bool hardwareAcceleration);

    void openLastFileOnStartupChanged(bool openLastFileOnStartup);

    void snapToGridChanged(bool snapToGrid);

private:
    Preferences();
    bool m_gridVisible;
    QColor m_gridColor;
    int m_gridFine;
    QString m_language;
    QColor m_baseColor;
    bool m_rulerVisible;
    int m_girdSize;
    int m_defaultUnit;
    QColor m_selectionColor;
    int m_style;
    QSettings * m_settings;

 private:
    bool boolValue(const char *key, bool def = false) const;
    QColor colorValue(const char *key, const QColor &def = QColor()) const;
    QString stringValue(const char *key, const QString &def = QString()) const;
    int intValue(const char *key, int defaultValue) const;
    qreal realValue(const char *key, qreal defaultValue) const;
    QSize sizeValue(const char *key,QSize defaultValue) const;

    bool m_autoSave;
    bool m_hardwareAcceleration;
    bool m_openLastFileOnStartup;
    bool m_snapToGrid;
    QSize m_defaultPageSize;
};

#endif // PREFERENCES_H
