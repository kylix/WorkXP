#include "languagemanager.h"

#include "preferences.h"

#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

LanguageManager * LanguageManager::m_instance;

LanguageManager *LanguageManager::instance()
{
    if (!m_instance)
        m_instance = new LanguageManager;
    return m_instance;
}

LanguageManager::LanguageManager(QObject *parent )
    : QObject(parent),m_qtTranslator(new QTranslator(parent))
    , m_appTranslator(new QTranslator(parent))
{
    mTranslationsDir = ":/translations";
    return;
    mTranslationsDir = QCoreApplication::applicationDirPath();
#if defined(Q_OS_WIN32)
    mTranslationsDir += QLatin1String("/translations");
#elif defined(Q_OS_MAC)
    mTranslationsDir += QLatin1String("/../Translations");
#else
    mTranslationsDir += QLatin1String("/../share/tiled/translations");
#endif
}

void LanguageManager::installTranslators()
{
    // Delete previous translators
    delete m_qtTranslator;
    delete m_appTranslator;

    m_qtTranslator = new QTranslator;
    m_appTranslator = new QTranslator;

    QString language = Preferences::instance()->language();
    if (language.isEmpty())
        language = QLocale::system().name();

    const QString qtTranslationsDir =
            QLibraryInfo::location(QLibraryInfo::TranslationsPath);

    if (m_qtTranslator->load(QLatin1String("qt_") + language,
                            qtTranslationsDir)) {
        QCoreApplication::installTranslator(m_qtTranslator);
    } else {
        delete m_qtTranslator;
        m_qtTranslator = nullptr;
    }

    if (m_appTranslator->load(QLatin1String("wxp_") + language,
                             mTranslationsDir)) {
        QCoreApplication::installTranslator(m_appTranslator);
    } else {
        delete m_appTranslator;
        m_appTranslator = nullptr;
    }
}

const QStringList & LanguageManager::availableLanguages()
{
    if (mLanguages.isEmpty())
        loadAvailableLanguages();
    return mLanguages;
}

void LanguageManager::loadAvailableLanguages()
{
    mLanguages.clear();

    QStringList nameFilters;
    nameFilters.append(QLatin1String("wxp_*.qm"));

    QDirIterator iterator(mTranslationsDir, nameFilters,
                          QDir::Files | QDir::Readable);

    while (iterator.hasNext()) {
        iterator.next();
        const QString baseName = iterator.fileInfo().completeBaseName();
        // Cut off "tiled_" from the start
        mLanguages.append(baseName.mid(4));
    }
}


