#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QPixmap>
#include <QColor>
#include <QPalette>

class MainWindow;

#define THEME ThemeManager::instance()

class ThemeManager : public QObject
{
    Q_OBJECT


    static ThemeManager * m_instance;



    QString m_defaultStyle;
    QPalette m_defaultPalette;
    QPixmap m_rotateCursorPm;
    QPixmap m_drawingCursorPm;


public:



    static ThemeManager *instance(QObject *mainwindow = nullptr);


    inline QPixmap rotateCursorPm(){return m_rotateCursorPm;}
    inline QPixmap drawingCursorPm(){return m_drawingCursorPm;}


signals:

    void styleApplied();



private:
    //only allow singleton instanc
    ThemeManager();
    void apply();

    QColor parseColor(const QString & str);


public slots:
    void themeChanged(int arg1);


};

#endif // THEMEMANAGER_H
