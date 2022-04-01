#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QStringList>
#include <QApplication>

class QTranslator;

#define LANGUAGEMANAGER LanguageManager::instance()
class LanguageManager : public QObject
{
public:
    static LanguageManager *instance();

    /**
     * Installs the translators on the application for Qt and Tiled. Should be
     * called again when the language changes.
     */
    void installTranslators();

    /**
     * Returns the available languages as a list of country codes.
     */
    const QStringList & availableLanguages();

    inline const QString & translationsDir() { return mTranslationsDir ;}

private:
    LanguageManager(QObject * parent = qApp);

    void loadAvailableLanguages();

    QString mTranslationsDir;
    QStringList mLanguages;
    QTranslator *m_qtTranslator;
    QTranslator *m_appTranslator;

    static LanguageManager *m_instance;
};

#endif // LANGUAGEMANAGER_H
